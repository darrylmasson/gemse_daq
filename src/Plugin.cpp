#include "Plugin.h"
#include "Buffer.h"
#include "Logger.h"

Plugin::Plugin(std::weak_ptr<Logger> logger, std::string name, std::weak_ptr<Buffer> in, std::weak_ptr<Buffer> out)  {
  if (logger.expired()) {
    throw std::runtime_error("Tried to init plugin with invalid logger");
  }
  m_Name = name;
  m_Logger = logger.lock();
  LogEntry(Logger::Trace, "Plugin ctor starting");

  if (in.expired()) {
    LogEntry(Logger::Local, "no input buffer, I hope I'm a source plugin");
  } else {
    m_Source = in.lock();
  }
  if (out.expired()) {
    LogEntry(Logger::Local, "no output buffer, I hope I'm a sink plugin");
  } else {
    m_Sink = out.lock();
  }

  std::signal(SIGINT, Plugin::Interrupt);
  std::signal(SIGTERM, Plugin::Interrupt);

  m_bRun = false;
  m_bQuit = false;
  m_bIsRunning = false;

  LogEntry(Logger::Trace, "Plugin ctor ending");
}

Plugin::~Plugin() {
  if (m_bIsRunning == true) LogEntry(Logger::Warning, "Still running?")
  m_bRun = false;
  LogEntry(Logger::Trace, "Plugin dtor starting");
  m_Source.reset();
  m_Sink.reset();
  LogEntry(Logger::Trace, "Plugin dtor ending");
  m_Logger.reset();
}



SourcePlugin::SourcePlugin(std::weak_ptr<Logger> logger, std::string name, std::weak_ptr<Buffer> out) :
  Plugin(logger, name, std::weak_ptr<Buffer>(), out) {
  LogEntry(Logger::Trace, "Source plugin ctor starting");



  LogEntry(Logger::Trace, "Source plugin ctor ending");
}

SourcePlugin::~SourcePlugin() {
  LogEntry(Logger::Trace, "Source plugin dtor starting");

  LogEntry(Logger::Trace, "Source plugin dtor ending");
}


SinkPlugin::SinkPlugin(std::weak_ptr<Logger> logger, std::string name, std::weak_ptr<Buffer> in) :
  Plugin(logger, name, in, std::weak_ptr<Buffer>()) {
  LogEntry(Logger::Trace, "Sink plugin ctor starting");

  LogEntry(Logger::Trace, "Sink plugin ctor ending");
}

SinkPlugin::~SinkPlugin() {
  LogEntry(Logger::Trace, "Sink plugin dtor starting");

  LogEntry(Logger::Trace, "Sink plugin dtor ending");
}
