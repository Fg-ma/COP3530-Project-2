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

// Test function deleet later
std::vector<Product> generateRandomProducts(int count) {
  std::vector<Product> products;
  products.reserve(count);

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<int> idDist(100000000, 999999999);
  std::uniform_int_distribution<int> numberDist(0, 100000);
  std::uniform_real_distribution<double> priceDist(1.0, 3000.0);
  std::uniform_int_distribution<int> categoryDist(0, 8);

  const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

  std::uniform_int_distribution<int> charDist(0, chars.size() - 1);

  for (int i = 0; i < count; i++) {
    std::string description;

    // random product description
    for (int j = 0; j < 20; j++) {
      description += chars[charDist(gen)];
    }

    products.emplace_back(idDist(gen),      // product id
                          description,      // random description
                          priceDist(gen),   // price
                          numberDist(gen),  // reviews
                          numberDist(gen),  // stock
                          numberDist(gen),  // sales
                          static_cast<ProductCategory>(categoryDist(gen)));
  }

  return products;
}

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

    // Testing logging remove eventually
    // auto productIdAnalytics =
    //     searchAPI.search({ProductSearchField::ProductId, static_cast<double>(88519805)});
    // info("Found tree result: ", productIdAnalytics.treeResult->productDescription,
    //      " in time: ", productIdAnalytics.nsTreeTimeTaken,
    //      " nanoseconds. Found heap result: ", productIdAnalytics.heapResult->productDescription,
    //      " in time: ", productIdAnalytics.nsHeapTimeTaken, " nanoseconds.");

    // auto priceAnalytics = searchAPI.searchRange({ProductSearchField::Price, 199.0, 200.0});
    // info("Found price tree results: ", priceAnalytics.heapResultsReturned,
    //      " in time: ", priceAnalytics.nsTreeTimeTaken,
    //      " nanoseconds. Found heap result: ", priceAnalytics.treeResultsReturned,
    //      " in time: ", priceAnalytics.nsHeapTimeTaken, " nanoseconds.");

    // auto testProducts = generateRandomProducts(1000000);
    // InsertRequest request{std::nullopt, testProducts};
    // auto insertAnalytics = searchAPI.insert(request);
    // info("Inserted ", testProducts.size(), " products.");
    // info("Tree insert time: ", insertAnalytics.nsTreeTimeTaken, " nanoseconds.");
    // info("Heap insert time: ", insertAnalytics.nsHeapTimeTaken, " nanoseconds.");

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
