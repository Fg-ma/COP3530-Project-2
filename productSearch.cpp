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

    SearchAPI searchAPI(config.data_path);

    // Testing logging remove eventually
    // Product* productIdFound = productIdTree.search(88519805);
    // if (productIdFound) info(productIdFound->productDescription);
    // info("Product id max: ", productIdHeap.getMax().productDescription, " with ",
    //      productIdHeap.getMax().productId);
    // Product* productDescriptionFound = productDescriptionTree.search(
    //     "Google indoor Nest Security Cam 1080p (Wired) - 2nd Generation - Snow");
    // if (productDescriptionFound)
    //   info(productDescriptionFound->productDescription, " ", productDescriptionFound->productId,
    //        " ", productDescriptionFound->stock);
    // Product* productDescriptionHeapFound = productDescriptionHeap.search(
    //     "Google indoor Nest Security Cam 1080p (Wired) - 2nd Generation - Snow");
    // if (productDescriptionHeapFound)
    //   info(productDescriptionHeapFound->productDescription, " ",
    //        productDescriptionHeapFound->productId, " ", productDescriptionHeapFound->stock);
    // info("Product description max: ", productDescriptionHeap.getMax().productDescription, " with
    // ",
    //      productDescriptionHeap.getMax().productId);
    // Product* numReviewsFound = numReviewsTree.search(11495);
    // if (numReviewsFound) info(numReviewsFound->productDescription);
    // info("Number of reviews max: ", numReviewsHeap.getMax().productDescription, " with ",
    //      numReviewsHeap.getMax().numReviews);
    // Product* priceFound = priceTree.search(199);
    // if (priceFound) info(priceFound->productDescription);
    // info("Price max: ", priceHeap.getMax().productDescription, " with ",
    // priceHeap.getMax().price);
    // std::vector<Product> priceFoundRange = priceTree.searchRange(199, 200);
    // for (const auto& product : priceFoundRange) {
    //   info("Price range found: ", product.productDescription, " with ", product.price);
    // }
    // info(priceFoundRange.size(), " products found in price range 199-200");
    // std::vector<Product> priceFoundHeapRange = priceHeap.searchRange(199, 200);
    // for (const auto& product : priceFoundHeapRange) {
    //   info("Price range found: ", product.productDescription, " with ", product.price);
    // }
    // info(priceFoundHeapRange.size(), " products found in price range 199-200");
    // Product* salesFound = salesTree.search(9528); if (salesFound)
    // info(salesFound->productDescription); info("Sales max: ",
    // salesHeap.getMax().productDescription, " with ", salesHeap.getMax().sales); Product*
    // stockFound = stockTree.search(34); if (stockFound) info(stockFound->productDescription);
    // info("Stock max: ", stockHeap.getMax().productDescription, " with ",
    // stockHeap.getMax().stock);

    // Ui
    auto ui = std::make_shared<ProductSearchUI>();
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
