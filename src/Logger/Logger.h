//
// Created by Oliver Ilczuk on 05.01.25.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
private:
    LogLevel logLevel;
    std::ofstream logFile;
    bool logToFile;

    std::string getTimestamp() const;

    std::string levelToString(LogLevel level) const;

    std::string getColor(LogLevel level) const;

    const std::string RESET = "\033[0m";

public:
    Logger(LogLevel level, const std::string& fileName = "");
    ~Logger();

    void log(LogLevel level, const std::string& message);
    void setLogLevel(LogLevel level);
};

#endif // LOGGER_H