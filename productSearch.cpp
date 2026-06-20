#include <signal.h>
#include <sodium.h>
#include <unistd.h>
#include <yaml-cpp/yaml.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <thread>

#include "productSearch/config.h"
#include "productSearch/gui/productSearchUi.h"
#include "productSearch/gui/productSearchUiRunner.h"
#include "productSearch/gui/share.h"
#include "productSearch/logger.h"
#include "productSearch/share.h"

using namespace std;
using namespace Log;

namespace fs = filesystem;
using json = nlohmann::json;

extern Config config;

int main(int argc, char** argv) {
  try {
    if (argc != 2) {
      return 1;
    }

    if (!load_config(argv[1])) return 1;

    Logger::init(config.log_file, config.max_log_file_size);

    auto ui = std::make_shared<ProductSearchUI>();

    std::thread uiThread([ui]() {
      try {
        runUIThread(ui);
      } catch (const std::exception& ex) {
        error("[UI] Fatal: ", ex.what());
      }
    });

    uiThread.join();

    return 0;
  } catch (const exception& ex) {
    error("Unhandled exception: ", ex.what());
    return 1;
  }
}
