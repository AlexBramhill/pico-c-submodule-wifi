#include "wifi-manager.h"
#include "wifi-logger.h"
#include "pico/cyw43_arch.h"
#include <cstdio>
#include <string>
#include <array>

WifiManager &WifiManager::getInstance(ILogger *logger)
{
    static WifiManager instance(logger);
    return instance;
}

WifiManager::WifiManager(ILogger *logger) : isInitialized(false), logger(logger)
{
    WifiLogger::setLogger(logger);
}

WifiManager::~WifiManager()
{
    if (isInitialized)
    {
        cyw43_arch_deinit();
    }
}

bool WifiManager::connect(const char *ssid, const char *password, const int retryCount, const int timeoutMs)
{
    if (!tryInitializeHardware())
    {
        return false;
    }

    WifiLogger::logConnecting();

    for (int i = 0; i < retryCount; ++i)
    {
        if (tryConnect(ssid, password, timeoutMs))
        {
            std::string ip;
            tryGetIpAddress(ip);
            WifiLogger::logConnectionSuccess(i + 1, ip);
            return true;
        }

        const int attemptNumber = i + 1;
        const bool isLastAttempt = i == retryCount - 1;
        WifiLogger::logConnectionFailure(attemptNumber, isLastAttempt);
    }

    return false;
}

void WifiManager::disconnect()
{
    if (isConnected())
    {
        cyw43_arch_disable_sta_mode();
        WifiLogger::logDisconnected();
    }
}

bool WifiManager::isConnected() const
{
    if (!isInitialized)
    {
        return false;
    }

    return cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA) == CYW43_LINK_UP;
}

bool WifiManager::tryGetIpAddressAsByte(std::array<uint8_t, 4> &out) const
{
    if (!isConnected())
    {
        return false;
    }

    const uint8_t *ip_address = (const uint8_t *)&(cyw43_state.netif[0].ip_addr.addr);
    out = {ip_address[0], ip_address[1], ip_address[2], ip_address[3]};
    return true;
}

bool WifiManager::tryGetIpAddress(std::string &out) const
{
    std::array<uint8_t, 4> ip;
    if (!tryGetIpAddressAsByte(ip))
    {
        return false;
    }

    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    out = buffer;
    return true;
}

bool WifiManager::tryInitializeHardware()
{
    if (isInitialized)
    {
        return true;
    }

    if (cyw43_arch_init() != 0)
    {
        WifiLogger::logInitFailed();
        return false;
    }

    cyw43_arch_enable_sta_mode();
    isInitialized = true;
    return true;
}

bool WifiManager::tryConnect(const char *ssid, const char *password, int timeoutMs)
{
    return cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, timeoutMs) == 0;
}
