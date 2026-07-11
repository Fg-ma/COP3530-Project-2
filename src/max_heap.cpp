#include "product_search/max_heap.h"

#include <stdexcept>

// Get parent index = (i - 1) / 2 (int division)
int MaxHeap::parent(int i) {
  return (i - 1) / 2;
}

// Left index = 2 * i + 1
int MaxHeap::left(int i) {
  return 2 * i + 1;
}

// Right index = 2 * i + 2
int MaxHeap::right(int i) {
  return 2 * i + 2;
}

// Heapify

void MaxHeap::heapifyUp(int index) {
  while (index > 0 && heap[index] > heap[parent(index)]) {
    std::swap(heap[index], heap[parent(index)]);

    index = parent(index);
  }
}

void MaxHeap::heapifyDown(int index) {
  int largest = index;

  int l = left(index);
  int r = right(index);

  if (l < size_ && heap[l] > heap[largest]) largest = l;

  if (r < size_ && heap[r] > heap[largest]) largest = r;

  if (largest != index) {
    std::swap(heap[index], heap[largest]);

    heapifyDown(largest);
  }
}

// Public methods

void MaxHeap::insert(const Product& product) {
  if (size_ == capacity_) {
    resize();
  }

  heap[size_] = product;
  heapifyUp(size_);
  size_++;
}

Product* MaxHeap::max() const {
  if (empty()) return nullptr;

  return const_cast<Product*>(&heap[0]);
}

Product MaxHeap::extractMax() {
  if (empty()) throw std::runtime_error("Heap empty");

  Product maxProduct = heap[0];

  heap[0] = heap[size_ - 1];
  size_--;

  if (!empty()) heapifyDown(0);

  return maxProduct;
}

bool MaxHeap::empty() const {
  return size_ == 0;
}

int MaxHeap::size() const {
  return size_;
}

Product* MaxHeap::search(double key) {
  for (int i = 0; i < size_; i++) {
    if (heap[i].getDoubleKey() == key) {
      return &heap[i];
    }
  }

  return nullptr;
}

Product* MaxHeap::search(const std::string& key) {
  for (int i = 0; i < size_; i++) {
    if (heap[i].getStringKey() == key) {
      return &heap[i];
    }
  }

  return nullptr;
}

std::vector<Product> MaxHeap::searchRange(double minKey, double maxKey) const {
  std::vector<Product> results;

  for (int i = 0; i < size_; i++) {
    double value = heap[i].getDoubleKey();

    if (value >= minKey && value <= maxKey) {
      results.push_back(heap[i]);
    }
  }

  return results;
}

std::vector<Product> MaxHeap::searchRange(const std::string& minKey,
                                          const std::string& maxKey) const {
  std::vector<Product> results;

  for (int i = 0; i < size_; i++) {
    const std::string& value = heap[i].getStringKey();

    if (value >= minKey && value <= maxKey) {
      results.push_back(heap[i]);
    }
  }

  return results;
}
