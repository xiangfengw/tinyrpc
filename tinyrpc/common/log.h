#ifndef ROCKET_COMMON_LOG_H
#define ROCKET_COMMON_LOG_H

#include <string>
#include <queue>
#include <memory>

#include "tinyrpc/common/config.h"
#include "tinyrpc/common/mutex.h"

namespace tinyrpc {

template<typename... Args>
std::string formatString(const char* str, Args&&... args) {
    
    int size = snprintf(nullptr, 0, str, args...);

    std::string result;
    if(size > 0) {
        result.resize(size);
        snprintf(&result[0], size + 1, str, args...);
    }

    return result;
}

#define DEBUGLOG(str, ...)\
    if (tinyrpc::Logger::GetGlobalLogger()->getLogLevel() && tinyrpc::Logger::GetGlobalLogger()->getLogLevel() <= tinyrpc::Debug) \
    { \
        tinyrpc::Logger::GetGlobalLogger()->pushLog(tinyrpc::LogEvent(tinyrpc::LogLevel::Debug).toString() \
        + "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + tinyrpc::formatString(str, ##__VA_ARGS__) + "\n"); \
        tinyrpc::Logger::GetGlobalLogger()->log(); \
    } \

#define INFOLOG(str, ...)\
    tinyrpc::Logger::GetGlobalLogger()->pushLog((new tinyrpc::LogEvent(tinyrpc::LogLevel::Info))->toString() + tinyrpc::formatString(str, ##__VA_ARGS__) + "\n"); \
    tinyrpc::Logger::GetGlobalLogger()->log();\

#define ERRORLOG(str, ...)\
    tinyrpc::Logger::GetGlobalLogger()->pushLog((new tinyrpc::LogEvent(tinyrpc::LogLevel::Error))->toString() + tinyrpc::formatString(str, ##__VA_ARGS__) + "\n"); \
    tinyrpc::Logger::GetGlobalLogger()->log();\


enum LogLevel {
    Unknown = 0,
    Debug = 1,
    Info = 2,
    Error = 3
};

std::string LogLevel2String(LogLevel level);

LogLevel String2LogLevel(const std::string& log_level);

class Logger {
public:
    typedef std::shared_ptr<Logger> s_ptr;

    Logger(LogLevel level) : m_set_level(level) {}

    void pushLog(const std::string& msg);

    void log();

    LogLevel getLogLevel() const {
        return m_set_level;
    }

public:
    static Logger* GetGlobalLogger();

    static void InitGlobalLogger();

private:
    LogLevel m_set_level;
    std::queue<std::string> m_buffer;

    Mutex m_mutex;
};



class LogEvent {
public:

    LogEvent(LogLevel level) : m_level(level) {}

    std::string getFileName() const {
        return m_file_name;
    }

    LogLevel getLogLevel() const {
        return m_level;
    }

    std::string toString();

private:
    std::string m_file_name;
    int32_t m_file_line;
    int32_t m_pid;
    int32_t m_thread_id;

    LogLevel m_level;

};



}

#endif