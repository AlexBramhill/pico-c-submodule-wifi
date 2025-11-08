#pragma once

#include <stdint.h>
#include <string>
#include <array>

class WifiManager
{
public:
    static WifiManager &getInstance();

    bool connect(const char *ssid, const char *password, const int retryCount, const int timeoutMs);
    void disconnect();
    bool isConnected() const;
    bool tryGetIpAddressAsString(std::string &out) const;

private:
    WifiManager();
    ~WifiManager();

    bool isInitialized;

    bool tryGetIpAddress(std::array<uint8_t, 4> &out) const;

    WifiManager(const WifiManager &) = delete;
    WifiManager &operator=(const WifiManager &) = delete;
};