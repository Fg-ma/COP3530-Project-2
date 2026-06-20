#pragma once

#include <string>

struct Config {
  std::string data_path;

  std::string log_file;
  int max_log_file_size;
};
