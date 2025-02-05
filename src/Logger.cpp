/**
 * @file Logger.cpp
 * @brief Implementation of the Logger class functionality
 */

#include "../include/Logger.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

/**
 * @brief Initializes the logger with a specified file
 * @param filename The name of the log file
 * 
 * This method:
 * 1. Acquires a lock to ensure thread safety
 * 2. Creates a Log directory if it doesn't exist
 * 3. Opens the log file in append mode
 */
void Logger::init(const string& filename) {
    lock_guard<std::mutex> lock(mtx);
    
    fs::path logDir("../Log");
    if (!fs::exists(logDir)) {
        fs::create_directory(logDir);
    }
    
    fs::path logPath = logDir / filename;
    logFile.open(logPath, ios::app);
}

/**
 * @brief Logs a message with the specified severity level
 * @param level The severity level of the message
 * @param message The message to log
 * 
 * This method:
 * 1. Checks if the message meets the minimum logging level
 * 2. Acquires a lock for thread safety
 * 3. Formats the message with timestamp and level
 * 4. Writes to the log file and flushes immediately
 */
void Logger::log(Level level, const string& message) {
    if (level < currentLevel) return;
    
    lock_guard<std::mutex> lock(mtx);
    string output = getCurrentTime() + " [" + levelToString(level) + "] " + message + "\n";
    
    if (logFile.is_open()) {
        logFile << output;
        logFile.flush();
    }
}

/**
 * @brief Convenience method for logging INFO level messages
 * @param message The message to log
 */
void Logger::info(const string& message) { 
    log(Level::INFO, message); 
}

/**
 * @brief Convenience method for logging ERROR level messages
 * @param message The message to log
 */
void Logger::error(const std::string& message) { 
    log(Level::ERROR, message); 
}

/**
 * @brief Gets the current time as a formatted string
 * @return String containing the current timestamp in "YYYY-MM-DD HH:MM:SS" format
 * 
 * Uses the system clock to get the current time and formats it
 * using the specified format string.
 */
string Logger::getCurrentTime() const {
    auto now = chrono::system_clock::now();
    auto time = chrono::system_clock::to_time_t(now);
    stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

/**
 * @brief Converts a logging level to its string representation
 * @param level The logging level to convert
 * @return String representation of the logging level
 */
string Logger::levelToString(Level level) const {
    switch (level) {
        case Level::INFO: return "INFO";
        case Level::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}