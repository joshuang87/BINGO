#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>
#include <memory>
#include <filesystem>  // 添加文件系统头文件

using namespace std;

class Logger {
    public:
        // 日志级别枚举
        enum class Level {
            INFO,
            ERROR,
        };

        // 获取单例实例
        static Logger& getInstance() {
            static Logger instance;
            return instance;
        }

        // 初始化日志文件
        void init(const string& filename);

        // 写日志的主要方法
        void log(Level level, const string& message);

        // 便捷方法
        void info(const string& message);
        void error(const string& message);

        // 设置日志级别
        void setLevel(Level level) { 
            currentLevel = level; 
        }

        Level getLevel() const { 
            return currentLevel; 
        }

    private:
        // 私有构造函数（单例模式）
        Logger() : currentLevel(Level::INFO) {}
        
        // 删除拷贝构造和赋值操作符
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        // 获取当前时间字符串
        string getCurrentTime() const;
        // 将日志级别转换为字符串
        string levelToString(Level level) const;

        ofstream logFile;
        mutex mtx;
        Level currentLevel;
};

// 定义一个宏来方便使用
#define LOG_INFO(msg) Logger::getInstance().info(msg)
#define LOG_ERROR(msg) Logger::getInstance().error(msg)

#endif