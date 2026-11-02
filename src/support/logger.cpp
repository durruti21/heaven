// -----------------------------------------------------------------------------------------    
// Logger
// -----------------------------------------------------------------------------------------    
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ringbuffer_sink.h>
#include "logger.hpp"


// startic multisinc logger
std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink;
std::shared_ptr<spdlog::sinks::basic_file_sink_mt>   file_sink;
std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt>   buffer_sink;    
std::shared_ptr<spdlog::logger>                      default_logger;

// -----------------------------------------------------------------------------------------    
// Private Constructor get reference
// ----------------------------------------------------------------------------------------- 
Logger::Logger(const std::string & file) {

    loggerFile = file;
    console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::debug);
    file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(loggerFile, true);
    file_sink->set_level(spdlog::level::debug);
    buffer_sink = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(200); // 200 messages
    buffer_sink->set_level(spdlog::level::debug);


    default_logger = std::make_shared<spdlog::logger>("multi_sink", spdlog::sinks_init_list{console_sink, file_sink, buffer_sink});
    spdlog::set_default_logger(default_logger);
    spdlog::set_pattern("[%H:%M:%S.%e] [%^%L%$] %v");

    return;
}


// -----------------------------------------------------------------------------------------    
// Create Class
// ----------------------------------------------------------------------------------------- 
Logger * Logger::CreateInstance(const std::string & file)
{
    if (instance == nullptr)
    {
        instance = new Logger(file);
    }
    return instance;
}

// -----------------------------------------------------------------------------------------    
// Get Instance reference
// ----------------------------------------------------------------------------------------- 
Logger * Logger::GetInstance()
{
    if (instance == nullptr)
    {
        // Set default config directory to be safe
        instance = new Logger("./log.txt");
    }
    return instance;
}

// -----------------------------------------------------------------------------------------    
// To Log Level
// -----------------------------------------------------------------------------------------
Logger::LogLevel Logger::ToLogLevel(std::string logStr)
{
    if       (logStr == "critical") return  LogLevel::LOG_CRITICAL;
    else if  (logStr == "warning")  return  LogLevel::LOG_WARNING;
    else if  (logStr == "info")     return  LogLevel::LOG_INFO;
    else                            return  LogLevel::LOG_DEBUG;
} 

// -----------------------------------------------------------------------------------------    
// Set Log Level
// -----------------------------------------------------------------------------------------
void Logger::SetLogLevel(LogLevel level)
{
    switch (level) 
    {
        case LogLevel::LOG_CRITICAL:
            spdlog::set_level(spdlog::level::critical);
            break;
        case LogLevel::LOG_WARNING:
            spdlog::set_level(spdlog::level::warn);
            break;
        case LogLevel::LOG_INFO:
            spdlog::set_level(spdlog::level::info);
            break;
        case LogLevel::LOG_DEBUG:
            spdlog::set_level(spdlog::level::debug);
            break;
        default:
            spdlog::set_level(spdlog::level::info);                                
    }
}


// -----------------------------------------------------------------------------------------    
// Get Log History
// -----------------------------------------------------------------------------------------
std::vector<std::string> Logger::GetLogHistory()
{
    return buffer_sink->last_formatted();
}