#include "product_search/search_api.h"

#include "product_search/csv.h"

SearchAPI::SearchAPI(const std::string& dataPath)
    : dataPath_(dataPath),
      productIdTree_(ProductCompareField::ProductId),
      productIdHeap_(ProductCompareField::ProductId),
      productDescriptionTree_(ProductCompareField::ProductDescription),
      productDescriptionHeap_(ProductCompareField::ProductDescription),
      numReviewsTree_(ProductCompareField::NumReviews),
      numReviewsHeap_(ProductCompareField::NumReviews),
      priceTree_(ProductCompareField::Price),
      priceHeap_(ProductCompareField::Price),
      salesTree_(ProductCompareField::Sales),
      salesHeap_(ProductCompareField::Sales),
      stockTree_(ProductCompareField::Stock),
      stockHeap_(ProductCompareField::Stock) {
  // Load products from CSV
  std::vector<Product> products = loadCSV(dataPath_);

  for (auto& product : products) {
    productIdTree_.insert(product);
    productIdHeap_.insert(product);
    productDescriptionTree_.insert(product);
    productDescriptionHeap_.insert(product);
    numReviewsTree_.insert(product);
    numReviewsHeap_.insert(product);
    priceTree_.insert(product);
    priceHeap_.insert(product);
    salesTree_.insert(product);
    salesHeap_.insert(product);
    stockTree_.insert(product);
    stockHeap_.insert(product);
  }
}
