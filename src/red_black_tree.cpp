#include "product_search/red_black_tree.h"

RedBlackTree::RedBlackTree(ProductCompareField compareField) : compareField_(compareField) {
  NIL = new RedBlackNode(Product());

  NIL->color = RedBlackTreeColor::BLACK;
  NIL->left = NIL;
  NIL->right = NIL;

  root = NIL;
}

RedBlackTree::~RedBlackTree() {
  destroyTree(root);
  delete NIL;
}

void RedBlackTree::destroyTree(RedBlackNode* node) {
  if (node == NIL) return;

  destroyTree(node->left);
  destroyTree(node->right);

  delete node;
}

// Rotations

void RedBlackTree::leftRotate(RedBlackNode* x) {
  RedBlackNode* y = x->right;

  x->right = y->left;

  if (y->left != NIL) y->left->parent = x;

  y->parent = x->parent;

  if (x->parent == nullptr)
    root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;

  y->left = x;
  x->parent = y;
}

void RedBlackTree::rightRotate(RedBlackNode* y) {
  RedBlackNode* x = y->left;

  y->left = x->right;

  if (x->right != NIL) x->right->parent = y;

  x->parent = y->parent;

  if (y->parent == nullptr)
    root = x;
  else if (y == y->parent->right)
    y->parent->right = x;
  else
    y->parent->left = x;

  x->right = y;
  y->parent = x;
}

// Internal helpers

void RedBlackTree::rebalance(RedBlackNode* z) {
  while (z->parent && z->parent->color == RedBlackTreeColor::RED) {
    if (z->parent == z->parent->parent->left) {
      RedBlackNode* y = z->parent->parent->right;

      if (y->color == RedBlackTreeColor::RED) {
        z->parent->color = RedBlackTreeColor::BLACK;
        y->color = RedBlackTreeColor::BLACK;
        z->parent->parent->color = RedBlackTreeColor::RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->right) {
          z = z->parent;
          leftRotate(z);
        }

        z->parent->color = RedBlackTreeColor::BLACK;
        z->parent->parent->color = RedBlackTreeColor::RED;

        rightRotate(z->parent->parent);
      }
    } else {
      RedBlackNode* y = z->parent->parent->left;

      if (y->color == RedBlackTreeColor::RED) {
        z->parent->color = RedBlackTreeColor::BLACK;
        y->color = RedBlackTreeColor::BLACK;
        z->parent->parent->color = RedBlackTreeColor::RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->left) {
          z = z->parent;
          rightRotate(z);
        }

        z->parent->color = RedBlackTreeColor::BLACK;
        z->parent->parent->color = RedBlackTreeColor::RED;

        leftRotate(z->parent->parent);
      }
    }
  }

  root->color = RedBlackTreeColor::BLACK;
}

RedBlackNode* RedBlackTree::searchRecurse(RedBlackNode* node, double searchValue) const {
  if (node == NIL) return nullptr;

  double nodeKey = node->data.getKey();

  if (searchValue == nodeKey) return node;

  if (searchValue < nodeKey) return searchRecurse(node->left, searchValue);

  return searchRecurse(node->right, searchValue);
}

// Public methods

void RedBlackTree::insert(Product product) {
  product.setCompareField(compareField_);

  RedBlackNode* z = new RedBlackNode(product);

  z->left = NIL;
  z->right = NIL;

  RedBlackNode* y = nullptr;
  RedBlackNode* x = root;

  while (x != NIL) {
    y = x;

    if (z->data < x->data)
      x = x->left;
    else
      x = x->right;
  }

  z->parent = y;

  if (y == nullptr)
    root = z;
  else if (z->data < y->data)
    y->left = z;
  else
    y->right = z;

  rebalance(z);
}

Product* RedBlackTree::search(double searchValue) {
  RedBlackNode* result = searchRecurse(root, searchValue);

  if (result == nullptr) return nullptr;

  return &(result->data);
}

bool RedBlackTree::contains(double searchValue) {
  return search(searchValue) != nullptr;
}

RedBlackNode* RedBlackTree::getRoot() const {
  return root;
}
