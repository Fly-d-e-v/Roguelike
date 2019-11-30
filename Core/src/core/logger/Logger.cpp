#include "Logger.h"

#include <stdarg.h>
#include <ios>
#include <fstream>
#include <exception>

std::queue<std::string> Logger::_logQueue = std::queue<std::string>();
std::vector<std::string> Logger::_logHistory = std::vector<std::string>();
std::mutex Logger::_queueLock = std::mutex();
bool Logger::_logToFile = true;

void Logger::WriteFirstLog() {
    try {
        std::ofstream os("RogueLike.log");
        if (os.is_open()) {
            os << "Starting RogueLike Engine" << std::endl;
            os.close();
        }
    }
    catch (std::exception e) {
        printf(e.what());
    }
}

void Logger::Log(LogVerbosity verbosity, const char* log, ...) {
    
#ifndef _DEBUG
    if (verbosity == LogVerbosity::Debug)
        return;
#endif

    va_list argList;
    va_start(argList, log);
    char* buffer = new char[512];
    vsprintf(buffer, log, argList);
    va_end(argList);

    std::string formatedLog;

    switch (verbosity) {
    case LogVerbosity::Info:
        formatedLog = "[Info] ";
        break;
    case LogVerbosity::Debug:
        formatedLog = "[Debug]";
        break;
    case LogVerbosity::Warning:
        formatedLog = "[Warning] ";
        break;
    case LogVerbosity::Error:
        formatedLog = "[Error] ";
        break;
    }

    formatedLog.append(buffer);
    delete[] buffer;

    printf((formatedLog+"\n").c_str());
    std::lock_guard<std::mutex> Lock(_queueLock);   
    _logQueue.push(formatedLog);

}

void Logger::ProcessQueue() {
    
    std::lock_guard<std::mutex> Lock(_queueLock);
    if (_logToFile) {
        if (!_logQueue.empty()) {

            try {
                std::ofstream os("RogueLike.log", std::ios_base::app | std::ios_base::out);

                if (os.is_open()) {
                    while (!_logQueue.empty()) {
                        os << _logQueue.front() << std::endl;
                        _logHistory.push_back(_logQueue.front());
                        _logQueue.pop();
                    }
                    os.close();
                }
            }
            catch (std::exception e) {
                printf("Failed to log to file %s",e.what());
                _logToFile = false;
            }
        }
    }
    else {
        while (!_logQueue.empty()) {
            _logHistory.push_back(_logQueue.front());
            _logQueue.pop();
        }
    }
}

void Logger::ClearHistory() {
    _logHistory.clear();
}
