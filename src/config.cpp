
#include "product_search/config.h"

#include <yaml-cpp/yaml.h>

#include <exception>
#include <string>

using namespace std;

Config config;

bool load_config(const string& path) {
  try {
    auto cfg = YAML::LoadFile(path);

    // ---------------- GENERAL ----------------
    if (auto general = cfg["general"]) {
      config.data_path = general["data_path"].as<string>("");
    }

    // ---------------- LOGS ----------------
    if (auto logs = cfg["logs"]) {
      config.log_file = logs["log_file"].as<string>("");

      config.max_log_file_size = logs["max_log_file_size"].as<int>(2097152);
    }

    return true;

  } catch (const exception& e) {
    return false;
  }
}
