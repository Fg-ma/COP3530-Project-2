#pragma once

#include <vector>

#include "product_search/share.h"

class MaxHeap {
 public:
  MaxHeap(ProductCompareField compareField) : compareField_(compareField){};

  // Insert product - O(log n)
  void insert(Product product);

  // Get max and remove it - O(log n)
  Product extractMax();

  // Get max without removing it - O(1)
  Product getMax() const;

  // Check if heap is empty - O(1)
  bool empty() const;

  // Get size of heap - O(1)
  int size() const;

  // Search - O(n)
  Product* search(double key);
  Product* search(const std::string& key);
  std::vector<Product> searchRange(double minKey, double maxKey) const;
  std::vector<Product> searchRange(const std::string& minKey, const std::string& maxKey) const;

 private:
  ProductCompareField compareField_;

  std::vector<Product> heap;

  int parent(int i);
  int left(int i);
  int right(int i);

  void heapifyUp(int index);
  void heapifyDown(int index);
};
