#pragma once

#include "core/utils/Tool.h"

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

class Logger : public Tool {

public:

    Logger();
    ~Logger();

    static void WriteFirstLog();

    static void Log(LogVerbosity verbosity, const char* log, ...);

    //Appends log file with queue contents and adds contents to loghistory
    static void ProcessQueue();

    //Clears log history
    static void ClearHistory();

    virtual void ToolMethod() override;

private:

    static std::queue <std::string> _logQueue;
    static std::vector<std::string> _logHistory;

    static std::mutex _queueLock;
    static std::mutex _historyLock;

    static bool _logToFile;

    char* _searchFilter;

};