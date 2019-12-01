#include "Logger.h"

#include "imgui/imgui.h"

#include <stdarg.h>
#include <ios>
#include <fstream>
#include <exception>

std::queue<std::string> Logger::_logQueue = std::queue<std::string>();
std::vector<std::string> Logger::_logHistory = std::vector<std::string>();
std::mutex Logger::_queueLock = std::mutex();
std::mutex Logger::_historyLock = std::mutex();
bool Logger::_logToFile = true;

Logger::Logger() {
    _ToolName = "Output Log";
    _ShowTool = true;
    _searchFilter = new char[128]();
}

Logger::~Logger() {
    delete[] _searchFilter;
}

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
    char* buffer = new char[1024];
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
    
    std::lock_guard<std::mutex> QueueLock(_queueLock);
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
        std::lock_guard<std::mutex> HistoryLock(_historyLock);
        while (!_logQueue.empty()) {
            _logHistory.push_back(_logQueue.front());
            _logQueue.pop();
        }
    }
}

void Logger::ClearHistory() {
    std::lock_guard<std::mutex> Lock(_historyLock);
    _logHistory.clear();
}

void Logger::ToolMethod() {

    if (_ShowTool) {
        ImGui::Begin(_ToolName.c_str(), &_ShowTool);

        ImGui::InputText("Search",_searchFilter, sizeof(char)*128, ImGuiInputTextFlags_EnterReturnsTrue);

        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), true, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText
        
        //pls don't remove this scope
        {
            std::lock_guard<std::mutex> Lock(_historyLock);
            for (uint32_t i = 0; i < Logger::_logHistory.size(); i++) {

                const char* logItem = _logHistory[i].c_str();

                if (!strstr(logItem, _searchFilter))
                    continue;

                bool pop_color = false;
                if (strstr(logItem, "[Error]")) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f)); pop_color = true; }
                else if (strstr(logItem, "[Warning]")) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.0f, 1.0f)); pop_color = true; }
                ImGui::TextUnformatted(logItem);
                if (pop_color)
                    ImGui::PopStyleColor();
            }
        }

        ImGui::EndChild();

        if (ImGui::Button("Clear")) {
            Logger::ClearHistory();
        }

        ImGui::End();
    }
}
