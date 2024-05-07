#include <string>

enum OutputLoggerType{
    CONSOLE,
    FILE,
    NOTHING
};

enum LogLevel{
    INFO,
    WARN,
    ERROR
};

class Logger{
    private:
        OutputLoggerType type;
    public:
        Logger(/* args */);
        ~Logger();
        void log(LogLevel level, std::string message);
};

Logger::Logger(/* args */){
}

Logger::~Logger(){
}

void Logger::log(LogLevel level, std::string message){
    switch (type){
        case CONSOLE:
        /* code */
            break;
        case FILE:
        /* code */
            break;
        default:
            break;
    }
}
