/**
 * @file Logger.h
 * @brief Header file for the Logger class implementing a thread-safe singleton logger
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>
#include <memory>
#include <filesystem>

using namespace std;

/**
 * @class Logger
 * @brief A thread-safe singleton logger class for application-wide logging
 * 
 * This class provides logging functionality with different severity levels,
 * timestamp-based logging, and thread-safe operations. It follows the singleton
 * pattern to ensure a single logging instance across the application.
 */
class Logger {
    public:
        /**
         * @brief Enumeration for different logging levels
         */
        enum class Level {
            INFO,   ///< Information level messages
            ERROR,  ///< Error level messages
        };

        /**
         * @brief Gets the singleton instance of the Logger
         * @return Reference to the singleton Logger instance
         */
        static Logger& getInstance() {
            static Logger instance;
            return instance;
        }

        /**
         * @brief Initializes the logger with a specified file
         * @param filename Name of the log file
         * 
         * Creates the log directory if it doesn't exist and opens the log file
         * in append mode.
         */
        void init(const string& filename);

        /**
         * @brief Main logging method
         * @param level Severity level of the log message
         * @param message The message to be logged
         * 
         * Logs a message with the specified severity level if it meets or exceeds
         * the current logging level threshold.
         */
        void log(Level level, const string& message);

        /**
         * @brief Convenience method for logging info messages
         * @param message The message to be logged at INFO level
         */
        void info(const string& message);

        /**
         * @brief Convenience method for logging error messages
         * @param message The message to be logged at ERROR level
         */
        void error(const string& message);

        /**
         * @brief Sets the minimum logging level
         * @param level The new minimum logging level
         */
        void setLevel(Level level) { 
            currentLevel = level; 
        }

        /**
         * @brief Gets the current minimum logging level
         * @return The current logging level
         */
        Level getLevel() const { 
            return currentLevel; 
        }

    private:
        /**
         * @brief Private constructor for singleton pattern
         */
        Logger() : currentLevel(Level::INFO) {}
        
        // Delete copy constructor and assignment operator
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        /**
         * @brief Gets the current timestamp as a formatted string
         * @return Formatted timestamp string
         */
        string getCurrentTime() const;

        /**
         * @brief Converts a logging level to its string representation
         * @param level The logging level to convert
         * @return String representation of the logging level
         */
        string levelToString(Level level) const;

        ofstream logFile;      ///< Output file stream for logging
        mutex mtx;            ///< Mutex for thread-safe logging
        Level currentLevel;   ///< Current minimum logging level
};

/**
 * @brief Macro for convenient INFO level logging
 * @param msg The message to log
 */
#define LOG_INFO(msg) Logger::getInstance().info(msg)

/**
 * @brief Macro for convenient ERROR level logging
 * @param msg The message to log
 */
#define LOG_ERROR(msg) Logger::getInstance().error(msg)

#endif