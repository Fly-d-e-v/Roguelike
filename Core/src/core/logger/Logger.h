#pragma once

#include <string>
#include <queue>
#include <vector>
#include <mutex>

enum class LogVerbosity {
    None = 0,
    Info,
    Debug,
    Warning,
    Error
};

class Logger {

public:

    static void WriteFirstLog();

    static void Log(LogVerbosity verbosity, const char* log, ...);

    //Appends log file with queue contents and adds contents to loghistory
    static void ProcessQueue();

    //Clears log history
    static void ClearHistory();

private:

    static std::queue <std::string> _logQueue;
    static std::vector<std::string> _logHistory;

    static std::mutex _queueLock;

    static bool _logToFile;

};