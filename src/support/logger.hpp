// logger
//
// Description: 
//        This class provides logger services to all the application. It is implemented as a singleton
//        so no need to manage the logger class on each separate instance
//
#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <spdlog/spdlog.h>


class Logger {

private:
    static Logger* instance;    

    std::string loggerFile;

    // -----------------------------------------------------------------------------------------    
    // Private Constructor get reference
    // ----------------------------------------------------------------------------------------- 
    Logger(const std::string & file);
    
public:
    enum class LogLevel {
             LOG_CRITICAL, 
             LOG_WARNING, 
             LOG_INFO, 
             LOG_DEBUG
    };

    // -----------------------------------------------------------------------------------------    
    // Create Class
    // ----------------------------------------------------------------------------------------- 
    static Logger * CreateInstance(const std::string & file);

    // -----------------------------------------------------------------------------------------    
    // Get Instance reference
    // ----------------------------------------------------------------------------------------- 
    static Logger * GetInstance();

    // -----------------------------------------------------------------------------------------    
    // To Log Level
    // -----------------------------------------------------------------------------------------
    static LogLevel ToLogLevel(std::string logStr);  

    // -----------------------------------------------------------------------------------------    
    // Set Log Level
    // -----------------------------------------------------------------------------------------
    void SetLogLevel(LogLevel level);     

    // -----------------------------------------------------------------------------------------    
    // Log
    // -----------------------------------------------------------------------------------------
    template <typename... Args>    
    void Log(LogLevel level, const std::string msg, Args &&...args)
    {
        switch (level) 
        {
            case LogLevel::LOG_CRITICAL:
                spdlog::critical(msg, std::forward<Args>(args)...);
                break;
            case LogLevel::LOG_WARNING:
                spdlog::warn(msg, std::forward<Args>(args)...);
                break;
            case LogLevel::LOG_INFO:
                spdlog::info(msg, std::forward<Args>(args)...);
                break;
            case LogLevel::LOG_DEBUG:
                spdlog::debug(msg, std::forward<Args>(args)...);
                break;
            default:
                spdlog::info(msg, std::forward<Args>(args)...);                                
        }
    }

    // -----------------------------------------------------------------------------------------    
    // Get Log History
    // -----------------------------------------------------------------------------------------
    std::vector<std::string> GetLogHistory();

};
