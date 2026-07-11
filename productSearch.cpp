#include <signal.h>
#include <sodium.h>
#include <unistd.h>
#include <yaml-cpp/yaml.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>
#include <random>
#include <sstream>
#include <string>
#include <thread>

#include "product_search/config.h"
#include "product_search/gui/product_search_ui.h"
#include "product_search/gui/product_search_ui_runner.h"
#include "product_search/gui/share.h"
#include "product_search/logger.h"
#include "product_search/search_api.h"
#include "product_search/share.h"

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

    // Init config/logger
    if (!load_config(argv[1])) {
      error("Config failed to load");
      return 1;
    }
    Logger::init(config.log_file, config.max_log_file_size);

    auto searchAPI = std::make_shared<SearchAPI>(config.data_path);

    // Ui
    auto ui = std::make_shared<ProductSearchUI>();
    ui->setSearchAPI(searchAPI);
    std::thread uiThread([ui]() {
      try {
        runUIThread(ui);
      } catch (const std::exception& ex) {
        error("[UI] Fatal: ", ex.what());
      }
    });
    uiThread.join();

    info("============================== Finished ==============================");

    return 0;
  } catch (const exception& ex) {
    error("Unhandled exception: ", ex.what());
    return 1;
  }
}
