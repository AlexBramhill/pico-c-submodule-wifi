#include "wifi-manager.h"
#include "pico/cyw43_arch.h"
#include <cstdio>
#include <string>
#include <array>

WifiManager &WifiManager::getInstance()
{
    static WifiManager instance;
    return instance;
}

WifiManager::WifiManager() : isInitialized(false) {}

WifiManager::~WifiManager()
{
    if (isInitialized)
    {
        cyw43_arch_deinit();
    }
}

bool WifiManager::connect(const char *ssid, const char *password)
{
    if (!isInitialized)
    {
        if (cyw43_arch_init() != 0)
        {
            printf("Wi-Fi init failed\n");
            return false;
        }
        cyw43_arch_enable_sta_mode();
        isInitialized = true;
    }

    printf("Connecting to Wi-Fi...\n");

    if (cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, 30000))
    {
        printf("Failed to connect.\n");
        return false;
    }

    printf("Connected.\n");

    std::string ip;
    if (tryGetIpAddressAsString(ip))
    {
        printf("IP address %s\n", ip.c_str());
    }

    return true;
}

void WifiManager::disconnect()
{
    if (isInitialized && isConnected())
    {
        cyw43_arch_disable_sta_mode();
        printf("Disconnected from Wi-Fi\n");
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

bool WifiManager::tryGetIpAddress(std::array<uint8_t, 4> &out) const
{
    if (!isInitialized || !isConnected())
    {
        return false;
    }

    const uint8_t *ip_address = (const uint8_t *)&(cyw43_state.netif[0].ip_addr.addr);
    out = {ip_address[0], ip_address[1], ip_address[2], ip_address[3]};
    return true;
}

bool WifiManager::tryGetIpAddressAsString(std::string &out) const
{
    std::array<uint8_t, 4> ip;
    if (!tryGetIpAddress(ip))
    {
        return false;
    }

    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    out = buffer;
    return true;
}
