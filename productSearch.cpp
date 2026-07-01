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
#include "product_search/csv.h"
#include "product_search/gui/product_search_ui.h"
#include "product_search/gui/product_search_ui_runner.h"
#include "product_search/gui/share.h"
#include "product_search/logger.h"
#include "product_search/max_heap.h"
#include "product_search/red_black_tree.h"
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

    // Load products from CSV
    std::vector<Product> products = loadCSV(config.data_path);
    if (products.empty()) {
      error("Failed to load products");
      return 1;
    }

    // Build data structures
    RedBlackTree productIdTree(ProductCompareField::ProductId);
    MaxHeap productIdHeap(ProductCompareField::ProductId);
    RedBlackTree numReviewsTree(ProductCompareField::NumReviews);
    MaxHeap numReviewsHeap(ProductCompareField::NumReviews);
    RedBlackTree priceTree(ProductCompareField::Price);
    MaxHeap priceHeap(ProductCompareField::Price);
    RedBlackTree salesTree(ProductCompareField::Sales);
    MaxHeap salesHeap(ProductCompareField::Sales);
    RedBlackTree stockTree(ProductCompareField::Stock);
    MaxHeap stockHeap(ProductCompareField::Stock);

    for (auto& product : products) {
      productIdTree.insert(product);
      productIdHeap.insert(product);
      numReviewsTree.insert(product);
      numReviewsHeap.insert(product);
      priceTree.insert(product);
      priceHeap.insert(product);
      salesTree.insert(product);
      salesHeap.insert(product);
      stockTree.insert(product);
      stockHeap.insert(product);
    }

    // Testing logging
    // Product* productIdFound = productIdTree.search(88519805);
    // if (productIdFound) info(productIdFound->productDescription);
    // info("Product id max: ", productIdHeap.getMax().productDescription, " with ",
    //      productIdHeap.getMax().productId);
    // Product* numReviewsFound = numReviewsTree.search(11495);
    // if (numReviewsFound) info(numReviewsFound->productDescription);
    // info("Number of reviews max: ", numReviewsHeap.getMax().productDescription, " with ",
    //      numReviewsHeap.getMax().numReviews);
    // Product* priceFound = priceTree.search(199);
    // if (priceFound) info(priceFound->productDescription);
    // info("Price max: ", priceHeap.getMax().productDescription, " with ",
    // priceHeap.getMax().price); Product* salesFound = salesTree.search(9528); if (salesFound)
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
