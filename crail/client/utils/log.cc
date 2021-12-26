#include "crail/client/utils/log.h"

using namespace mylogger;

Logger::Logger() {
    this->_enable = false;
    
    char *myEnv = getenv("LIBCRAIL_LOG");
    if (myEnv != NULL) {
        if (strcmp(myEnv, "1") == 0) {
            this->_enable = true;
            printf("libcrail log -> [enabled]\n");
        }
    } else {
        printf("libcrail log -> [disabled]\n");
    }
}

Logger::~Logger() {

}

void Logger::append(string header, string content, string time) {
    string str;

    if (this->_enable) {
        if (time == "") {
            time = GetCurrentTime();
        }

        str.append(time).append(" |").append(header).append("| ").append(content).append("\n");
        fprintf(stdout, "%s", str.c_str());
    }
}

Logger & Logger::get() {
    static Logger logger;
    return logger;
}
