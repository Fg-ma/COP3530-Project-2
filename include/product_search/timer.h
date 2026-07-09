#pragma once

#include <chrono>

class Timer {
 public:
  void start() {
    startTime = std::chrono::high_resolution_clock::now();
  }

  // Time since last start call in nanoseconds
  long long nanoseconds() const {
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - startTime).count();
  }

  // Time since last start call in microseconds
  long long microseconds() const {
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - startTime).count();
  }

 private:
  std::chrono::high_resolution_clock::time_point startTime;
};
