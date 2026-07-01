#include "product_search/logger.h"

#include <date/date.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>

namespace fs = std::filesystem;

namespace Log {

static Logger* g_instance = nullptr;

void Logger::init(std::string logFile, size_t maxFileSize) {
  static std::once_flag once;
  std::call_once(once, [&]() { g_instance = new Logger(std::move(logFile), maxFileSize); });
}

Logger& Logger::instance() {
  if (!g_instance) {
    throw std::runtime_error("Log::Logger not initialized. Call Log::Logger::init() first.");
  }
  return *g_instance;
}

Logger::Logger(std::string logFile, size_t maxFileSize)
    : log_file_(std::move(logFile)), max_log_file_size_(maxFileSize) {}

std::string Logger::timestamp() {
  using namespace std::chrono;

  const auto now = system_clock::now();

  const auto dp = date::floor<date::days>(now);
  const date::year_month_day ymd{dp};

  const auto tod = duration_cast<seconds>(now - dp);

  const auto h = duration_cast<hours>(tod);
  const auto m = duration_cast<minutes>(tod - h);
  const auto s = duration_cast<seconds>(tod - h - m);

  std::ostringstream ss;

  ss << std::setfill('0') << std::setw(4) << int(ymd.year()) << "-" << std::setw(2)
     << unsigned(ymd.month()) << "-" << std::setw(2) << unsigned(ymd.day()) << " " << std::setw(2)
     << h.count() << ":" << std::setw(2) << m.count() << ":" << std::setw(2) << s.count();

  return ss.str();
}

void Logger::log(const std::string& level, const std::string& msg) {
  std::lock_guard<std::mutex> lock(mutex_);
  writeToFile(level, msg);
}

void Logger::writeToFile(const std::string& level, const std::string& msg) {
  if (log_file_.empty()) {
    return;
  }

  std::ofstream file(log_file_, std::ios::app);
  if (!file.is_open()) {
    return;
  }

  file << timestamp() << " [" << level << "] " << msg << std::endl;
  file.close();

  // Rotate if too large
  try {
    auto size = std::filesystem::file_size(log_file_);
    if (size > static_cast<uintmax_t>(max_log_file_size_)) {
      std::string rotated = log_file_ + ".1";
      if (std::filesystem::exists(rotated)) {
        std::filesystem::remove(rotated);
      }

      std::filesystem::rename(log_file_, rotated);

      std::ofstream new_file(log_file_, std::ios::trunc);
      new_file << timestamp() << " [INFO] [Log] Rotated log file (exceeded " << max_log_file_size_
               << " bytes)" << std::endl;
      new_file.close();
    }
  } catch (...) {
  }
}

}  // namespace Log
