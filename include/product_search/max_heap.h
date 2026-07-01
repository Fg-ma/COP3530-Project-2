#pragma once

#include <vector>

#include "product_search/share.h"

class MaxHeap {
 public:
  MaxHeap(ProductCompareField compareField) : compareField_(compareField){};

  // Insert product
  void insert(Product product);

  // Get max and remove it
  Product extractMax();

  // Get max without removing it
  Product getMax() const;

  // Check if heap is empty
  bool empty() const;

  // Get size of heap
  int size() const;

 private:
  ProductCompareField compareField_;

  std::vector<Product> heap;

  int parent(int i);
  int left(int i);
  int right(int i);

  void heapifyUp(int index);
  void heapifyDown(int index);
};
