#pragma once

#include <stdint.h>
#include <string>
#include <array>
#include "ILogger.h"

class WifiManager
{
public:
    static WifiManager &getInstance(ILogger *logger = nullptr);

    bool connect(const char *ssid, const char *password, const int retryCount, const int timeoutMs);
    void disconnect();
    bool isConnected() const;
    bool tryGetIpAddress(std::string &out) const;

private:
    WifiManager(ILogger *logger);
    ~WifiManager();

    bool isInitialized;
    ILogger *logger;

    bool tryInitializeHardware();
    bool tryConnect(const char *ssid, const char *password, int timeoutMs);
    bool tryGetIpAddressAsByte(std::array<uint8_t, 4> &out) const;

    WifiManager(const WifiManager &) = delete;
    WifiManager &operator=(const WifiManager &) = delete;
};