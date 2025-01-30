#ifndef DB_H
#define DB_H

#include "../include/Logger.h"
#include "../include/Account.h"
#include "../include/Game.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <typeinfo>
#include <stdexcept>

using namespace std;

class DB {
    public:
        // 获取单例实例
        static DB& getInstance() {
            static DB instance;
            return instance;
        }

        // 删除拷贝构造函数和赋值运算符
        DB(const DB&) = delete;
        DB& operator=(const DB&) = delete;

        void init();

        //通用保存方法
        template<typename T>
        bool save(const T& data) {
            string filename = getFilename<T>();
            string jsonData = data.to_json();
            
            try {
                // 检查文件是否存在，如果不存在则创建并初始化为空数组
                if (!filesystem::exists(filename)) {
                    ofstream newFile(filename);
                    newFile << "[]";
                    newFile.close();
                }

                // 读取现有文件内容
                ifstream inFile(filename);
                string content;
                getline(inFile, content);
                inFile.close();

                // 处理文件内容
                if (content.empty()) {
                    content = "[]";
                }

                // 在末尾插入新数据
                if (content == "[]") {
                    // 如果是空数组，直接插入数据
                    content = "[" + jsonData + "]";
                } else {
                    // 在最后一个 ']' 之前插入新数据
                    content.insert(content.length() - 1, "," + jsonData);
                }

                // 写入更新后的内容
                ofstream outFile(filename);
                if (!outFile.is_open()) {
                    LOG_ERROR("Failed to open file: " + filename);
                    return false;
                }
                
                outFile << content;
                outFile.close();
                LOG_INFO("Data saved to " + filename);
                return true;
            } catch (const exception& e) {
                LOG_ERROR("Error saving data: " + string(e.what()));
                return false;
            }
        }

        // 通用读取方法
        template<typename T>
        vector<T> load() {
            string filename = getFilename<T>();
            vector<T> results;
            
            try {
                // 检查文件是否存在
                if (!filesystem::exists(filename)) {
                    LOG_INFO("File not found: " + filename);
                    return results;
                }

                // 读取文件内容
                ifstream inFile(filename);
                string content;
                getline(inFile, content);
                inFile.close();

                // 处理空文件或空数组的情况
                if (content.empty() || content == "[]") {
                    return results;
                }

                // 解析 JSON 数组
                results = T::from_json(content);

                LOG_INFO("Data loaded from " + filename);
                return results;
            } catch (const exception& e) {
                LOG_ERROR("Error loading data: " + string(e.what()));
                return results;
            }
        }

        // 通用重置方法
        template<typename T>
        bool reset() {
            string filename = getFilename<T>();
            ofstream newFile(filename, ios::trunc);
            newFile << "[]";
            newFile.close();

            return true;
        }

    private:
        const string DATADIR = "../data";
        const string ACCOUNTDATA = "../data/Account.json";
        const string GAMEDATA = "../data/Game.json";

        // 私有构造函数
        DB() {}

        // 根据类型获取对应的文件名
        template<typename T>
        string getFilename() const {
            if (typeid(T) == typeid(Account) || typeid(T) == typeid(Player)) {
                return ACCOUNTDATA;
            } 
            else if (typeid(T) == typeid(Game)) {
                return GAMEDATA;
            }
            // 可以在这里添加其他类型的文件名映射
            throw runtime_error("Unsupported data type");
        }
};

#endif // DB_H