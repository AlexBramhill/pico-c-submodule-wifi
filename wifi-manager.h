#pragma once

#include <stdint.h>
#include <string>
#include <array>

class WifiManager
{
public:
    // Singleton accessor
    static WifiManager &getInstance();

    bool connect(const char *ssid, const char *password);
    void disconnect();
    bool isConnected() const;
    bool tryGetIpAddress(std::array<uint8_t, 4> &out) const;
    bool tryGetIpAddressAsString(std::string &out) const;

private:
    WifiManager();
    ~WifiManager();

    bool isInitialized;

    WifiManager(const WifiManager &) = delete;
    WifiManager &operator=(const WifiManager &) = delete;
};