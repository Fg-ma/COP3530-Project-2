#pragma once

#include <optional>
#include <string>
#include <variant>
#include <vector>

// Config

struct Config {
  std::string data_path;

  std::string log_file;
  int max_log_file_size;
};

// Products

enum ProductCategory {
  AudioVideo,
  Camera,
  CarAccessories,
  Laptop,
  MenShoes,
  Men,
  Mobile,
  Movies,
  Toys
};

inline std::string toString(ProductCategory category) {
  switch (category) {
    case ProductCategory::AudioVideo:
      return "Audio and Video";
    case ProductCategory::Camera:
      return "Cameras";
    case ProductCategory::CarAccessories:
      return "Car Accessories";
    case ProductCategory::Laptop:
      return "Laptops";
    case ProductCategory::MenShoes:
      return "Men's Shoes";
    case ProductCategory::Men:
      return "Men";
    case ProductCategory::Mobile:
      return "Mobile";
    case ProductCategory::Movies:
      return "Movies";
    case ProductCategory::Toys:
      return "Toys";
  }

  return "Unknown category";
}

enum ProductSearchField { ProductId, ProductDescription, Price, NumReviews, Stock, Sales };

inline std::string toString(ProductSearchField field) {
  switch (field) {
    case ProductSearchField::ProductId:
      return "ProductId";
    case ProductSearchField::ProductDescription:
      return "ProductDescription";
    case ProductSearchField::Price:
      return "Price";
    case ProductSearchField::NumReviews:
      return "NumReviews";
    case ProductSearchField::Stock:
      return "Stock";
    case ProductSearchField::Sales:
      return "Sales";
  }

  return "Unknown";
}

inline const char* fieldLabel(ProductSearchField field) {
  switch (field) {
    case ProductSearchField::ProductId:
      return "Product ID";
    case ProductSearchField::ProductDescription:
      return "Description";
    case ProductSearchField::Price:
      return "Price";
    case ProductSearchField::NumReviews:
      return "Num Reviews";
    case ProductSearchField::Stock:
      return "Stock";
    case ProductSearchField::Sales:
      return "Sales";
    default:
      return "Unknown";
  }
}

inline bool fieldIsNumeric(ProductSearchField field) {
  return field != ProductSearchField::ProductDescription;
}

constexpr ProductSearchField allFields[] = {
    ProductSearchField::ProductId, ProductSearchField::ProductDescription,
    ProductSearchField::Price,     ProductSearchField::NumReviews,
    ProductSearchField::Stock,     ProductSearchField::Sales,
};
constexpr int allFieldCount = 6;

struct Product {
  int productId;
  std::string productDescription;
  double price;
  int numReviews;
  int stock;
  int sales;
  ProductCategory category;

  ProductSearchField compareField = ProductSearchField::ProductId;

  Product()
      : productId(0),
        price(0.0),
        numReviews(0),
        stock(0),
        sales(0),
        category(ProductCategory::AudioVideo) {}

  Product(int productId_, const std::string& productDescription_, double price_, double numReviews_,
          int stock_, int sales_, ProductCategory category_)
      : productId(productId_),
        productDescription(productDescription_),
        price(price_),
        numReviews(numReviews_),
        stock(stock_),
        sales(sales_),
        category(category_) {}

  void setCompareField(ProductSearchField field) {
    compareField = field;
  }

  double getDoubleKey() const {
    switch (compareField) {
      case ProductId:
        return productId;
      case Price:
        return price;
      case NumReviews:
        return numReviews;
      case Stock:
        return stock;
      case Sales:
        return sales;
    }

    return 0;
  }

  const std::string& getStringKey() const {
    static const std::string emptyString{};

    switch (compareField) {
      case ProductDescription:
        return productDescription;
    }

    return emptyString;
  }

  bool operator<(const Product& other) const {
    switch (compareField) {
      case ProductSearchField::ProductId:
        return productId < other.productId;
      case ProductSearchField::ProductDescription:
        return productDescription < other.productDescription;
      case ProductSearchField::Price:
        return price < other.price;
      case ProductSearchField::NumReviews:
        return numReviews < other.numReviews;
      case ProductSearchField::Stock:
        return stock < other.stock;
      case ProductSearchField::Sales:
        return sales < other.sales;
    }
    return false;
  }

  bool operator>(const Product& other) const {
    switch (compareField) {
      case ProductSearchField::ProductId:
        return productId > other.productId;
      case ProductSearchField::ProductDescription:
        return productDescription > other.productDescription;
      case ProductSearchField::Price:
        return price > other.price;
      case ProductSearchField::NumReviews:
        return numReviews > other.numReviews;
      case ProductSearchField::Stock:
        return stock > other.stock;
      case ProductSearchField::Sales:
        return sales > other.sales;
    }
    return false;
  }

  bool operator==(const Product& other) const {
    switch (compareField) {
      case ProductSearchField::ProductId:
        return productId == other.productId;
      case ProductSearchField::ProductDescription:
        return productDescription == other.productDescription;
      case ProductSearchField::Price:
        return price == other.price;
      case ProductSearchField::NumReviews:
        return numReviews == other.numReviews;
      case ProductSearchField::Stock:
        return stock == other.stock;
      case ProductSearchField::Sales:
        return sales == other.sales;
    }
    return false;
  }
};

// Search Api

struct SearchAnalytics {
  // Heap
  long long usHeapTimeTaken = 0;
  long long nsHeapTimeTaken = 0;
  Product* heapResult = nullptr;

  // Tree
  long long usTreeTimeTaken = 0;
  long long nsTreeTimeTaken = 0;
  Product* treeResult = nullptr;

  // Error
  std::string error = "";
};

struct SearchRequest {
  ProductSearchField searchField;
  std::variant<double, std::string> searchValue;
};

struct SearchRangeAnalytics {
  // Heap
  long long usHeapTimeTaken = 0;
  long long nsHeapTimeTaken = 0;
  std::vector<Product> heapResults = {};
  int heapResultsReturned = 0;

  // Tree
  long long usTreeTimeTaken = 0;
  long long nsTreeTimeTaken = 0;
  std::vector<Product> treeResults = {};
  int treeResultsReturned = 0;

  // Error
  std::string error = "";
};

struct SearchRangeRequest {
  ProductSearchField searchField;
  std::variant<double, std::string> searchMinValue;
  std::variant<double, std::string> searchMaxValue;
};

struct InsertAnalytics {
  // Heap
  long long usHeapTimeTaken = 0;
  long long nsHeapTimeTaken = 0;

  // Tree
  long long usTreeTimeTaken = 0;
  long long nsTreeTimeTaken = 0;

  // Error
  std::string error = "";
};

struct InsertRequest {
  std::optional<ProductSearchField> insertField;
  std::variant<Product, std::vector<Product>> data;
};
