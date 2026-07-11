#pragma once

#include "product_search/share.h"

class MaxHeap {
 public:
  MaxHeap(ProductSearchField compareField) : compareField_(compareField), size_(0), capacity_(8) {
    heap = new Product[capacity_];
  };
  ~MaxHeap() {
    delete[] heap;
  }

  // Insert product - O(log n)
  void insert(const Product& product);

  // Get max and remove it - O(log n)
  Product extractMax();

  // Get max without removing it - O(1)
  Product* max() const;

  // Check if heap is empty - O(1)
  bool empty() const;

  // Get size of heap - O(1)
  int size() const;

  // Search - O(n)
  Product* search(double key);
  Product* search(const std::string& key);
  std::vector<Product> searchRange(double minKey, double maxKey) const;
  std::vector<Product> searchRange(const std::string& minKey, const std::string& maxKey) const;

  // Get the amount of space MaxHeap is taking up
  size_t diskSize() const {
    return sizeof(MaxHeap) + size_ * sizeof(Product);
  }

 private:
  ProductSearchField compareField_;

  Product* heap;
  int size_;
  int capacity_;

 private:
  void resize();

  int parent(int i);
  int left(int i);
  int right(int i);

  void heapifyUp(int index);
  void heapifyDown(int index);
};
