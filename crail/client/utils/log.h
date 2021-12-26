#ifndef LOG_HH
#define LOG_HH

#include <iostream>
#include <string>

#include <cstdlib>
#include <cstdarg>
#include <ctime>
#include <cstring>

using std::string;

namespace mylogger {

class Logger {
public:
    static Logger & get();
    void append(string header, string content, string time = "");
    
private:
    Logger();
    ~Logger();

    bool _enable;
};

inline string va_list_to_string(const char *format, ...)
{
    char buf[4096];

    va_list list;
    va_start(list, format);
    vsnprintf(buf, 4096, format, list);
    va_end(list);

    return string(buf);
}

inline string GetCurrentTime() {
    time_t rawtime;
    struct tm *timeinfo;
    char buffer [80];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return string(buffer);
}

#define log_trace(...)                                                         \
mylogger::Logger::get().append(" TRACE ",                                        \
mylogger::va_list_to_string(__VA_ARGS__)                                         \
        .append(" ")                                                           \
        .append(std::string(__FILE__))                                         \
        .append("-")                                                           \
        .append(std::to_string(__LINE__)))
#define log_debug(...)                                                         \
    mylogger::Logger::get().append(" DEBUG ",                                    \
      mylogger::va_list_to_string(__VA_ARGS__)                                   \
        .append(" ")                                                           \
        .append(std::string(__FILE__))                                         \
        .append("-")                                                           \
        .append(std::to_string(__LINE__)))
#define log_warn(...)                                                          \
    mylogger::Logger::get().append(" WARN  ",                                    \
      mylogger::va_list_to_string(__VA_ARGS__)                                   \
        .append(" ")                                                           \
        .append(std::string(__FILE__))                                         \
        .append("-")                                                           \
        .append(std::to_string(__LINE__)))
#define log_info(...)                                                          \
    mylogger::Logger::get().append(" INFO  ",                                    \
      mylogger::va_list_to_string(__VA_ARGS__)                                   \
        .append(" ")                                                           \
        .append(std::string(__FILE__))                                         \
        .append("-")                                                           \
        .append(std::to_string(__LINE__)))
#define log_error(...)                                                         \
    mylogger::Logger::get().append(" ERROR ",                                    \
      mylogger::va_list_to_string(__VA_ARGS__)                                   \
        .append(" ")                                                           \
        .append(std::string(__FILE__))                                         \
        .append("-")                                                           \
        .append(std::to_string(__LINE__)))
#define log_fatal(...)                                                         \
    mylogger::Logger::get().append(" FATAL ",                                    \
      mylogger::va_list_to_string(__VA_ARGS__)                                   \
        .append(" ")                                                           \
        .append(std::string(__FILE__))                                         \
        .append("-")                                                           \
        .append(std::to_string(__LINE__)))
}
#endif