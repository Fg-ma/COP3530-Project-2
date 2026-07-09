#include "product_search/search_api.h"

#include "product_search/csv.h"
#include "product_search/timer.h"

SearchAPI::SearchAPI(const std::string& dataPath)
    : dataPath_(dataPath),
      productIdTree_(ProductSearchField::ProductId),
      productIdHeap_(ProductSearchField::ProductId),
      productDescriptionTree_(ProductSearchField::ProductDescription),
      productDescriptionHeap_(ProductSearchField::ProductDescription),
      numReviewsTree_(ProductSearchField::NumReviews),
      numReviewsHeap_(ProductSearchField::NumReviews),
      priceTree_(ProductSearchField::Price),
      priceHeap_(ProductSearchField::Price),
      salesTree_(ProductSearchField::Sales),
      salesHeap_(ProductSearchField::Sales),
      stockTree_(ProductSearchField::Stock),
      stockHeap_(ProductSearchField::Stock) {
  // Load products from CSV
  std::vector<Product> products = loadCSV(dataPath_);

  for (const auto& product : products) {
    insert(product);
  }
}

SearchAnalytics SearchAPI::search(const SearchRequest& request) {
  SearchAnalytics analytics;
  Timer timer;

  if (std::holds_alternative<double>(request.searchValue)) {
    double searchValue = std::get<double>(request.searchValue);

    switch (request.searchField) {
      case ProductSearchField::ProductId:
        timer.start();
        analytics.treeResult = productIdTree_.search(searchValue);
        analytics.usTreeTimeTaken = timer.microseconds();
        analytics.nsTreeTimeTaken = timer.nanoseconds();

        timer.start();
        analytics.heapResult = productIdHeap_.search(searchValue);
        analytics.usHeapTimeTaken = timer.microseconds();
        analytics.nsHeapTimeTaken = timer.nanoseconds();
        break;
      case ProductSearchField::Price:
        timer.start();
        analytics.treeResult = priceTree_.search(searchValue);
        analytics.usTreeTimeTaken = timer.microseconds();
        analytics.nsTreeTimeTaken = timer.nanoseconds();

        timer.start();
        analytics.heapResult = priceHeap_.search(searchValue);
        analytics.usHeapTimeTaken = timer.microseconds();
        analytics.nsHeapTimeTaken = timer.nanoseconds();
        break;
      case ProductSearchField::NumReviews:
        timer.start();
        analytics.treeResult = numReviewsTree_.search(searchValue);
        analytics.usTreeTimeTaken = timer.microseconds();
        analytics.nsTreeTimeTaken = timer.nanoseconds();

        timer.start();
        analytics.heapResult = numReviewsHeap_.search(searchValue);
        analytics.usHeapTimeTaken = timer.microseconds();
        analytics.nsHeapTimeTaken = timer.nanoseconds();
        break;
      case ProductSearchField::Stock:
        timer.start();
        analytics.treeResult = stockTree_.search(searchValue);
        analytics.usTreeTimeTaken = timer.microseconds();
        analytics.nsTreeTimeTaken = timer.nanoseconds();

        timer.start();
        analytics.heapResult = stockHeap_.search(searchValue);
        analytics.usHeapTimeTaken = timer.microseconds();
        analytics.nsHeapTimeTaken = timer.nanoseconds();
        break;
      case ProductSearchField::Sales:
        timer.start();
        analytics.treeResult = salesTree_.search(searchValue);
        analytics.usTreeTimeTaken = timer.microseconds();
        analytics.nsTreeTimeTaken = timer.nanoseconds();

        timer.start();
        analytics.heapResult = salesHeap_.search(searchValue);
        analytics.usHeapTimeTaken = timer.microseconds();
        analytics.nsHeapTimeTaken = timer.nanoseconds();
        break;
      default:
        analytics.error = "Invalid search field for double value";
        return analytics;
    }
  } else if (std::holds_alternative<std::string>(request.searchValue)) {
    std::string searchValue = std::get<std::string>(request.searchValue);

    switch (request.searchField) {
      case ProductSearchField::ProductDescription:
        timer.start();
        analytics.treeResult = productDescriptionTree_.search(searchValue);
        analytics.usTreeTimeTaken = timer.microseconds();
        analytics.nsTreeTimeTaken = timer.nanoseconds();

        timer.start();
        analytics.heapResult = productDescriptionHeap_.search(searchValue);
        analytics.usHeapTimeTaken = timer.microseconds();
        analytics.nsHeapTimeTaken = timer.nanoseconds();
        break;
      default:
        analytics.error = "Invalid search field for string value";
        return analytics;
    }
  }

  return analytics;
};

SearchRangeAnalytics SearchAPI::searchRange(const SearchRangeRequest& request) {
  SearchRangeAnalytics analytics;
  Timer timer;

  if (std::holds_alternative<double>(request.searchMinValue) &&
      std::holds_alternative<double>(request.searchMaxValue)) {
    double searchMinValue = std::get<double>(request.searchMinValue);
    double searchMaxValue = std::get<double>(request.searchMaxValue);

    switch (request.searchField) {
      case ProductSearchField::ProductId:
        timer.start();
        analytics.treeResults = productIdTree_.searchRange(searchMinValue, searchMaxValue);
        analytics.usTreeTimeTaken = timer.microseconds();
        analytics.nsTreeTimeTaken = timer.nanoseconds();
        analytics.treeResultsReturned = analytics.treeResults.size();

        timer.start();
        analytics.heapResults = productIdHeap_.searchRange(searchMinValue, searchMaxValue);
        analytics.usHeapTimeTaken = timer.microseconds();
        analytics.nsHeapTimeTaken = timer.nanoseconds();
        analytics.heapResultsReturned = analytics.heapResults.size();
        break;
      case ProductSearchField::Price:
        timer.start();
        analytics.treeResults = priceTree_.searchRange(searchMinValue, searchMaxValue);
        analytics.usTreeTimeTaken = timer.microseconds();
        analytics.nsTreeTimeTaken = timer.nanoseconds();
        analytics.treeResultsReturned = analytics.treeResults.size();

        timer.start();
        analytics.heapResults = priceHeap_.searchRange(searchMinValue, searchMaxValue);
        analytics.usHeapTimeTaken = timer.microseconds();
        analytics.nsHeapTimeTaken = timer.nanoseconds();
        analytics.heapResultsReturned = analytics.heapResults.size();
        break;
      case ProductSearchField::NumReviews:
        timer.start();
        analytics.treeResults = numReviewsTree_.searchRange(searchMinValue, searchMaxValue);
        analytics.usTreeTimeTaken = timer.microseconds();
        analytics.nsTreeTimeTaken = timer.nanoseconds();
        analytics.treeResultsReturned = analytics.treeResults.size();

        timer.start();
        analytics.heapResults = numReviewsHeap_.searchRange(searchMinValue, searchMaxValue);
        analytics.usHeapTimeTaken = timer.microseconds();
        analytics.nsHeapTimeTaken = timer.nanoseconds();
        analytics.heapResultsReturned = analytics.heapResults.size();
        break;
      case ProductSearchField::Stock:
        timer.start();
        analytics.treeResults = stockTree_.searchRange(searchMinValue, searchMaxValue);
        analytics.usTreeTimeTaken = timer.microseconds();
        analytics.nsTreeTimeTaken = timer.nanoseconds();
        analytics.treeResultsReturned = analytics.treeResults.size();

        timer.start();
        analytics.heapResults = stockHeap_.searchRange(searchMinValue, searchMaxValue);
        analytics.usHeapTimeTaken = timer.microseconds();
        analytics.nsHeapTimeTaken = timer.nanoseconds();
        analytics.heapResultsReturned = analytics.heapResults.size();
        break;
      case ProductSearchField::Sales:
        timer.start();
        analytics.treeResults = salesTree_.searchRange(searchMinValue, searchMaxValue);
        analytics.usTreeTimeTaken = timer.microseconds();
        analytics.nsTreeTimeTaken = timer.nanoseconds();
        analytics.treeResultsReturned = analytics.treeResults.size();

        timer.start();
        analytics.heapResults = salesHeap_.searchRange(searchMinValue, searchMaxValue);
        analytics.usHeapTimeTaken = timer.microseconds();
        analytics.nsHeapTimeTaken = timer.nanoseconds();
        analytics.heapResultsReturned = analytics.heapResults.size();
        break;
      default:
        analytics.error = "Invalid search field for double value";
        return analytics;
    }
  } else if (std::holds_alternative<std::string>(request.searchMinValue) &&
             std::holds_alternative<std::string>(request.searchMaxValue)) {
    std::string searchMinValue = std::get<std::string>(request.searchMinValue);
    std::string searchMaxValue = std::get<std::string>(request.searchMaxValue);

    switch (request.searchField) {
      case ProductSearchField::ProductDescription:
        timer.start();
        analytics.treeResults = productDescriptionTree_.searchRange(searchMinValue, searchMaxValue);
        analytics.usTreeTimeTaken = timer.microseconds();
        analytics.nsTreeTimeTaken = timer.nanoseconds();
        analytics.treeResultsReturned = analytics.treeResults.size();

        timer.start();
        analytics.heapResults = productDescriptionHeap_.searchRange(searchMinValue, searchMaxValue);
        analytics.usHeapTimeTaken = timer.microseconds();
        analytics.nsHeapTimeTaken = timer.nanoseconds();
        analytics.heapResultsReturned = analytics.heapResults.size();
        break;
      default:
        analytics.error = "Invalid search field for string value";
        return analytics;
    }
  } else {
    analytics.error = "Mismatched types for search range values";
    return analytics;
  }

  return analytics;
};

InsertAnalytics SearchAPI::insert(const InsertRequest& request) {
  InsertAnalytics analytics;
  Timer timer;

  if (std::holds_alternative<Product>(request.data)) {
    Product product = std::get<Product>(request.data);

    timer.start();
    insertTree(product, request.insertField);
    analytics.usTreeTimeTaken = timer.microseconds();
    analytics.nsTreeTimeTaken = timer.nanoseconds();

    timer.start();
    insertHeap(product, request.insertField);
    analytics.usHeapTimeTaken = timer.microseconds();
    analytics.nsHeapTimeTaken = timer.nanoseconds();
  } else if (std::holds_alternative<std::vector<Product>>(request.data)) {
    std::vector<Product> products = std::get<std::vector<Product>>(request.data);

    timer.start();
    for (const auto& product : products) insertTree(product, request.insertField);
    analytics.usTreeTimeTaken = timer.microseconds();
    analytics.nsTreeTimeTaken = timer.nanoseconds();

    timer.start();
    for (const auto& product : products) insertHeap(product, request.insertField);
    analytics.usHeapTimeTaken = timer.microseconds();
    analytics.nsHeapTimeTaken = timer.nanoseconds();
  }

  return analytics;
};

void SearchAPI::insert(const Product& product, std::optional<ProductSearchField> insertField) {
  if (insertField) {
    switch (insertField.value()) {
      case ProductSearchField::ProductId:
        productIdTree_.insert(product);
        productIdHeap_.insert(product);
        break;
      case ProductSearchField::ProductDescription:
        productDescriptionTree_.insert(product);
        productDescriptionHeap_.insert(product);
        break;
      case ProductSearchField::Price:
        priceTree_.insert(product);
        priceHeap_.insert(product);
        break;
      case ProductSearchField::NumReviews:
        numReviewsTree_.insert(product);
        numReviewsHeap_.insert(product);
        break;
      case ProductSearchField::Stock:
        stockTree_.insert(product);
        stockHeap_.insert(product);
        break;
      case ProductSearchField::Sales:
        salesTree_.insert(product);
        salesHeap_.insert(product);
        break;
      default:
        break;
    }
  } else {
    productIdTree_.insert(product);
    productIdHeap_.insert(product);
    productDescriptionTree_.insert(product);
    productDescriptionHeap_.insert(product);
    priceTree_.insert(product);
    priceHeap_.insert(product);
    numReviewsTree_.insert(product);
    numReviewsHeap_.insert(product);
    stockTree_.insert(product);
    stockHeap_.insert(product);
    salesTree_.insert(product);
    salesHeap_.insert(product);
  }
}

void SearchAPI::insertTree(const Product& product, std::optional<ProductSearchField> insertField) {
  if (insertField) {
    switch (insertField.value()) {
      case ProductSearchField::ProductId:
        productIdTree_.insert(product);
        break;
      case ProductSearchField::ProductDescription:
        productDescriptionTree_.insert(product);
        break;
      case ProductSearchField::Price:
        priceTree_.insert(product);
        break;
      case ProductSearchField::NumReviews:
        numReviewsTree_.insert(product);
        break;
      case ProductSearchField::Stock:
        stockTree_.insert(product);
        break;
      case ProductSearchField::Sales:
        salesTree_.insert(product);
        break;
      default:
        break;
    }
  } else {
    productIdTree_.insert(product);
    productDescriptionTree_.insert(product);
    priceTree_.insert(product);
    numReviewsTree_.insert(product);
    stockTree_.insert(product);
    salesTree_.insert(product);
  }
}

void SearchAPI::insertHeap(const Product& product, std::optional<ProductSearchField> insertField) {
  if (insertField) {
    switch (insertField.value()) {
      case ProductSearchField::ProductId:
        productIdHeap_.insert(product);
        break;
      case ProductSearchField::ProductDescription:
        productDescriptionHeap_.insert(product);
        break;
      case ProductSearchField::Price:
        priceHeap_.insert(product);
        break;
      case ProductSearchField::NumReviews:
        numReviewsHeap_.insert(product);
        break;
      case ProductSearchField::Stock:
        stockHeap_.insert(product);
        break;
      case ProductSearchField::Sales:
        salesHeap_.insert(product);
        break;
      default:
        break;
    }
  } else {
    productIdHeap_.insert(product);
    productDescriptionHeap_.insert(product);
    priceHeap_.insert(product);
    numReviewsHeap_.insert(product);
    stockHeap_.insert(product);
    salesHeap_.insert(product);
  }
}
