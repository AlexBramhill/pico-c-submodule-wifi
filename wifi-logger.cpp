#include "wifi-logger.h"

ILogger *WifiLogger::logger = nullptr;

void WifiLogger::setLogger(ILogger *newLogger)
{
    logger = newLogger;
}

void WifiLogger::logConnecting()
{
    if (logger)
    {
        logger->info("Connecting to Wi-Fi...");
    }
}

void WifiLogger::logConnectionSuccess(int attemptNumber, const std::string &ipAddress)
{
    if (!logger)
    {
        return;
    }

    logger->info("Connected on attempt %d.", attemptNumber);

    if (!ipAddress.empty())
    {
        logger->info("IP address %s", ipAddress.c_str());
    }

    logger->info("Connected.");
}

void WifiLogger::logConnectionFailure(int attemptNumber, bool isLastAttempt)
{
    if (!logger)
    {
        return;
    }

    if (!isLastAttempt)
    {
        logger->warning("Attempt %d to connect failed. Retrying...", attemptNumber);
    }
    else
    {
        logger->error("Attempt %d to connect failed. No more retries left.", attemptNumber);
    }
}

void WifiLogger::logDisconnected()
{
    if (logger)
    {
        logger->info("Disconnected from Wi-Fi");
    }
}

void WifiLogger::logInitFailed()
{
    if (logger)
    {
        logger->error("Wi-Fi init failed");
    }
}
