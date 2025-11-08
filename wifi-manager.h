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
    bool tryGetIpAddress(std::string &out) const;

private:
    WifiManager();
    ~WifiManager();

    bool isInitialized;

    bool tryInitializeHardware();
    bool tryConnect(const char *ssid, const char *password, int timeoutMs);
    bool tryGetIpAddressAsByte(std::array<uint8_t, 4> &out) const;
    void printConnectionSuccess(int attemptNumber);
    void printConnectionFailure(int attemptNumber, bool isLastAttempt);

    WifiManager(const WifiManager &) = delete;
    WifiManager &operator=(const WifiManager &) = delete;
};