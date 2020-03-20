#ifndef _LOGGER_H_
#define _LOGGER_H_ 1

#include <string>

class Logger {
  public:
    Logger(int, int);
    ~Logger();
    void Entry(int, std::string, std::string);

    enum level_t {
      Trace = -2,
      Local = -1,
      Debug = 0,
      Info = 1,
      Warning = 2,
      Error = 3,
      Fatal = 4
    };

  private:
    std::string GetNow();
    std::string GetISODate();
    std::map<int, std::string> m_LevelName;
};

#endif // _LOGGER_H_ defined
