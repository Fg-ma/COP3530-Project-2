#pragma once

#include <string>

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
  RedBlackTree(ProductCompareField compareField);
  ~RedBlackTree();

  // Insert product
  void insert(Product product);

  // Find product by searchValue
  Product* search(double searchValue);

  // Does tree contain a product with searchValue?
  bool contains(double searchValue);

  // root node
  RedBlackNode* getRoot() const;

 private:
  ProductCompareField compareField_;

  RedBlackNode* root;
  RedBlackNode* NIL;

  void leftRotate(RedBlackNode* x);
  void rightRotate(RedBlackNode* y);

  void rebalance(RedBlackNode* z);

  RedBlackNode* searchRecurse(RedBlackNode* node, double searchValue) const;

  void destroyTree(RedBlackNode* node);
};
