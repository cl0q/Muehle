#include "Logger.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <stdexcept>
#include <iomanip>

Logger::Logger(LogLevel level, const std::string& fileName)
    : logLevel(level), logToFile(!fileName.empty()) {
    if (logToFile) {
        // Datei im Truncate-Modus öffnen, um sie zu leeren
        logFile.open(fileName, std::ios::out | std::ios::trunc);
        if (!logFile.is_open()) {
            throw std::runtime_error("Konnte Log-Datei nicht öffnen.");
        }
    }
}

Logger::~Logger() {
    if (logToFile && logFile.is_open()) {
        logFile.close();
    }
}

std::string Logger::getTimestamp() const {
    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&nowTime), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string Logger::levelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

std::string Logger::getColor(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG: return "\033[34m"; // Blau
        case LogLevel::INFO: return "\033[32m";  // Grün
        case LogLevel::WARNING: return "\033[33m"; // Gelb
        case LogLevel::ERROR: return "\033[31m";  // Rot
        default: return RESET;
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    // Erstelle die Log-Nachricht
    std::string logMessage = "[" + getTimestamp() + "] [" + levelToString(level) + "] " + message;

    // Schreibe alle Nachrichten in die Datei, unabhängig vom Log-Level
    if (logToFile) {
        logFile << logMessage << std::endl;
    }

    // Schreibe Nachrichten nur in die Konsole, wenn der Log-Level passt
    if (level >= logLevel) {
        std::cout << getColor(level) << logMessage << RESET << std::endl;
    }
}

void Logger::setLogLevel(LogLevel level) {
    logLevel = level;
}