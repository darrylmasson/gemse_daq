#ifndef _PLUGIN_H_
#define _PLUGIN_H_ 1

#include <csignal>
#include <memory>
#include <atomic>
#include <string>

class Buffer;
class Logger;

class Plugin {
  public:
    virtual Plugin(std::weak_ptr<Logger>, std::string, std::weak_ptr<Buffer>, std::weak_ptr<Buffer>)
    virtual ~Plugin();

    void Interrupt(int signal) {m_SignalStatus = signal; m_bRun = false; m_bQuit = true;}
    virtual void Run() = 0;
    void Quit() {m_bQuit = true;}

  protected:
    void LogEntry(int level, std::string msg){m_Logger->Entry(level, m_Name, msg);}

    std::atomic_bool m_bRun, m_bQuit, m_bIsRunning;
    std::shared_ptr<Buffer> m_EventSource;
    std::shared_ptr<Buffer> m_EventSink;
    std::shared_ptr<Logger> m_Logger;
    std::string m_Name;

    volatile std::sig_atomic_t m_SignalStatus;
};

class SourcePlugin : public Plugin {
  public:
    virtual SourcePlugin(std::weak_ptr<Logger>, std::string, std::weak_ptr<Buffer>);
    virtual ~SourcePlugin();

};

class SinkPlugin : public Plugin {
  public:
    virtual SinkPlugin(std::weak_ptr<Logger>, std::string, std::weak_ptr<Buffer>);
    virtual ~SinkPlugin();
};

#endif // _PLUGIN_H_ defined
