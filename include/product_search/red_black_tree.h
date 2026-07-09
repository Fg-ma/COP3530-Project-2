#pragma once

#include <string>
#include <vector>

#include "product_search/share.h"

enum RedBlackTreeColor { RED, BLACK };

struct RedBlackNode {
  Product data;
  RedBlackTreeColor color;

  RedBlackNode* left;
  RedBlackNode* right;
  RedBlackNode* parent;

  RedBlackNode(Product p)
      : data(p), color(RedBlackTreeColor::RED), left(nullptr), right(nullptr), parent(nullptr){};
};

class RedBlackTree {
 public:
  RedBlackTree(ProductSearchField compareField);
  ~RedBlackTree();

  // Insert product - O(log n)
  void insert(Product product);

  // Find product by searchValue - O(log n)
  Product* search(double searchValue);
  Product* search(const std::string& searchValue);

  // Adverage - O(log n + k), worst - O(n)
  std::vector<Product> searchRange(double minValue, double maxValue);
  std::vector<Product> searchRange(const std::string& minValue, const std::string& maxValue);

  // Does tree contain a product with searchValue - O(log n)
  bool contains(double searchValue);

  // root node - O(1)
  RedBlackNode* getRoot() const;

 private:
  ProductSearchField compareField_;

  RedBlackNode* root;
  RedBlackNode* NIL;

  void leftRotate(RedBlackNode* x);
  void rightRotate(RedBlackNode* y);

  void rebalance(RedBlackNode* z);

  RedBlackNode* searchRecurse(RedBlackNode* node, double searchValue) const;
  RedBlackNode* searchRecurse(RedBlackNode* node, const std::string& searchValue) const;

  void searchRangeRecurse(RedBlackNode* node, double minValue, double maxValue,
                          std::vector<Product>& results) const;
  void searchRangeRecurse(RedBlackNode* node, const std::string& minValue,
                          const std::string& maxValue, std::vector<Product>& results) const;

  void destroyTree(RedBlackNode* node);
};
