#pragma once

#include <string>

struct Config {
  std::string data_path;

  std::string log_file;
  int max_log_file_size;
};

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

enum ProductCompareField { ProductId, Price, NumReviews, Stock, Sales };

struct Product {
  int productId;
  std::string productDescription;
  double price;
  int numReviews;
  int stock;
  int sales;
  ProductCategory category;

  ProductCompareField compareField = ProductCompareField::ProductId;

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

  void setCompareField(ProductCompareField field) {
    compareField = field;
  }

  double getKey() const {
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

  double comparisonValue() const {
    switch (compareField) {
      case ProductCompareField::ProductId:
        return productId;
      case ProductCompareField::Price:
        return price;
      case ProductCompareField::NumReviews:
        return numReviews;
      case ProductCompareField::Stock:
        return stock;
      case ProductCompareField::Sales:
        return sales;
    }

    return 0;
  }

  bool operator<(const Product& other) const {
    return comparisonValue() < other.comparisonValue();
  }

  bool operator>(const Product& other) const {
    return comparisonValue() > other.comparisonValue();
  }

  bool operator==(const Product& other) const {
    return comparisonValue() == other.comparisonValue();
  }
};
