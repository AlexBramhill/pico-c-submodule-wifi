#pragma once

#include "ILogger.h"
#include <string>

class WifiLogger
{
public:
    static void setLogger(ILogger *logger);

    static void logConnecting();
    static void logConnectionSuccess(int attemptNumber, const std::string &ipAddress);
    static void logConnectionFailure(int attemptNumber, bool isLastAttempt);
    static void logDisconnected();
    static void logInitFailed();

private:
    static ILogger *logger;
};
