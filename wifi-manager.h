#pragma once

#include <stdint.h>
#include <string>
#include <optional>
#include <array>

class WifiManager
{
public:
    // Singleton accessor
    static WifiManager &getInstance();

    bool connect(const char *ssid, const char *password);
    void disconnect();
    bool isConnected() const;
    std::optional<std::array<uint8_t, 4>> getIpAddressOrDefault() const;
    std::optional<std::string> getIpAddressAsStringOrDefault() const;

private:
    WifiManager();
    ~WifiManager();

    bool isInitialized;

    WifiManager(const WifiManager &) = delete;
    WifiManager &operator=(const WifiManager &) = delete;
};