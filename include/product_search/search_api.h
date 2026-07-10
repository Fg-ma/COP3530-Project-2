#pragma once

#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "product_search/max_heap.h"
#include "product_search/red_black_tree.h"
#include "product_search/share.h"

class SearchAPI {
 public:
  SearchAPI(const std::string& dataPath);
  ~SearchAPI() = default;

  SearchAnalytics search(const SearchRequest& request);
  SearchRangeAnalytics searchRange(const SearchRangeRequest& request);
  SearchAnalytics max(const MaxRequest& request);

  InsertAnalytics insert(const InsertRequest& request);

 private:
  std::string dataPath_;

  RedBlackTree productIdTree_;
  MaxHeap productIdHeap_;
  RedBlackTree productDescriptionTree_;
  MaxHeap productDescriptionHeap_;
  RedBlackTree numReviewsTree_;
  MaxHeap numReviewsHeap_;
  RedBlackTree priceTree_;
  MaxHeap priceHeap_;
  RedBlackTree salesTree_;
  MaxHeap salesHeap_;
  RedBlackTree stockTree_;
  MaxHeap stockHeap_;

 private:
  // Insertion helper
  void insert(const Product& product, std::optional<ProductSearchField> insertField = std::nullopt);
  void insertTree(const Product& product,
                  std::optional<ProductSearchField> insertField = std::nullopt);
  void insertHeap(const Product& product,
                  std::optional<ProductSearchField> insertField = std::nullopt);
};
