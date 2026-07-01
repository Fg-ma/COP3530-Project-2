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

  if (l < heap.size() && heap[l] > heap[largest]) largest = l;

  if (r < heap.size() && heap[r] > heap[largest]) largest = r;

  if (largest != index) {
    std::swap(heap[index], heap[largest]);

    heapifyDown(largest);
  }
}

// Public methods

void MaxHeap::insert(Product product) {
  product.setCompareField(compareField_);

  heap.push_back(product);
  heapifyUp(heap.size() - 1);
}

Product MaxHeap::getMax() const {
  if (heap.empty()) throw std::runtime_error("Heap empty");

  return heap[0];
}

Product MaxHeap::extractMax() {
  if (heap.empty()) throw std::runtime_error("Heap empty");

  Product maxProduct = heap[0];

  heap[0] = heap.back();
  heap.pop_back();

  if (!heap.empty()) heapifyDown(0);

  return maxProduct;
}

bool MaxHeap::empty() const {
  return heap.empty();
}

int MaxHeap::size() const {
  return heap.size();
}
