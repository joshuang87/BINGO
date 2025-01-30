#include "../include/Logger.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;  // 添加命名空间别名

void Logger::init(const string& filename) {
    lock_guard<std::mutex> lock(mtx);
    
    // 检查并创建日志目录
    fs::path logDir("../Log");
    if (!fs::exists(logDir)) {
        fs::create_directory(logDir);
    }
    
    // 组合完整的日志文件路径
    fs::path logPath = logDir / filename;
    logFile.open(logPath, ios::app);
}

void Logger::log(Level level, const string& message) {
    if (level < currentLevel) return;
    
    lock_guard<std::mutex> lock(mtx);
    string output = getCurrentTime() + " [" + levelToString(level) + "] " + message + "\n";
    
    if (logFile.is_open()) {
        logFile << output;
        logFile.flush();
    }
}

void Logger::info(const string& message) { 
    log(Level::INFO, message); 
}
void Logger::error(const std::string& message) { 
    log(Level::ERROR, message); 
}

string Logger::getCurrentTime() const {
    auto now = chrono::system_clock::now();
    auto time = chrono::system_clock::to_time_t(now);
    stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

string Logger::levelToString(Level level) const {
    switch (level) {
        case Level::INFO: return "INFO";
        case Level::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
} 