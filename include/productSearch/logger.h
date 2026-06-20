#pragma once

#include <mutex>
#include <sstream>
#include <string>

namespace Log {

class Logger {
public:
  // Must be called once at startup
  static void init(std::string logFile, size_t maxFileSize);

  static Logger &instance();

  void log(const std::string &level, const std::string &msg);

private:
  Logger(std::string logFile, size_t maxFileSize);
  ~Logger() = default;

  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;

  std::string log_file_;
  size_t max_log_file_size_;

  std::mutex mutex_;

  std::string timestamp();
  void writeToFile(const std::string &level, const std::string &msg);
};

// ---------- Public logging API (unchanged syntax) ----------

template <typename... Args> inline void info(Args &&...args) {
  std::ostringstream ss;
  (void)(ss << ... << args);
  Logger::instance().log("INFO", ss.str());
}

template <typename... Args> inline void warn(Args &&...args) {
  std::ostringstream ss;
  (void)(ss << ... << args);
  Logger::instance().log("WARN", ss.str());
}

template <typename... Args> inline void error(Args &&...args) {
  std::ostringstream ss;
  (void)(ss << ... << args);
  Logger::instance().log("ERROR", ss.str());
}

template <typename... Args> inline void debug(Args &&...args) {
  std::ostringstream ss;
  (void)(ss << ... << args);
  Logger::instance().log("DEBUG", ss.str());
}

} // namespace Log
