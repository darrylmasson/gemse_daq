#include "Logger.h"
#include <chrono>
#include <ctime>
#include <sstream>

Logger::Logger() {


  m_LevelName = {
    {Trace, "TRACE"},
    {Local, "LOCAL"},
    {Debug, "DEBUG"},
    {Info, "INFO"},
    {Warning, "WARNING"},
    {Error, "ERROR"},
    {Fatal, "FATAL"}
  };
}

std::string Logger::GetNow() {
  auto now = std::chrono::utc_clock::now();
  std::time_t t = std::chrono::system_clock::to_time_t(now);
}

std::string GetISODate() {
  return std::chrono::format("%F");
  std::chrono::format("%T");
}

void Logger::Entry(int level, std::string name, std::string message) {
  auto now = std::chrono::utc_clock::now();
}
