#include "product_search/csv.h"

namespace {

std::string trimString(const std::string& str) {
  size_t start = str.find_first_not_of(" \t\r\n");
  size_t end = str.find_last_not_of(" \t\r\n");

  if (start == std::string::npos) return "";

  return str.substr(start, end - start + 1);
}

std::string normalizeNumber(std::string str) {
  str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
  str.erase(remove(str.begin(), str.end(), ','), str.end());

  return str;
}

double safeToDouble(const std::string& str) {
  std::string cleaned = normalizeNumber(str);

  if (cleaned.empty()) return 0.0;

  try {
    return std::stod(cleaned);
  } catch (...) {
    return 0.0;
  }
}

int safeToInt(const std::string& str) {
  std::string cleaned = normalizeNumber(str);

  if (cleaned.empty()) return 0;

  try {
    return std::stoi(cleaned);
  } catch (...) {
    return 0;
  }
}

ProductCategory parseCategory(const std::string& category) {
  std::string c = category;
  c.erase(remove_if(c.begin(), c.end(), ::isspace), c.end());

  if (c == "AudioVideo") return ProductCategory::AudioVideo;
  if (c == "Camera") return ProductCategory::Camera;
  if (c == "CarAccessories") return ProductCategory::CarAccessories;
  if (c == "Laptop") return ProductCategory::Laptop;
  if (c == "MenShoes") return ProductCategory::MenShoes;
  if (c == "Men") return ProductCategory::Men;
  if (c == "Mobile") return ProductCategory::Mobile;
  if (c == "Movies") return ProductCategory::Movies;
  if (c == "Toys") return ProductCategory::Toys;

  return ProductCategory::AudioVideo;
}

std::vector<std::string> parseCSVLine(const std::string& line) {
  std::vector<std::string> result;
  std::string field;
  bool inQuotes = false;

  for (char c : line) {
    if (c == '"') {
      inQuotes = !inQuotes;
    } else if (c == ',' && !inQuotes) {
      result.push_back(trimString(field));
      field.clear();
    } else {
      field += c;
    }
  }

  result.push_back(trimString(field));
  return result;
}

}  // namespace

std::vector<Product> loadCSV(const std::string& filename) {
  std::ifstream file(filename);
  std::vector<Product> products;

  if (!file.is_open()) {
    std::cerr << "Failed to open file\n";
    return products;
  }

  std::string line;

  std::getline(file, line);

  while (std::getline(file, line)) {
    if (line.empty()) continue;

    std::vector<std::string> cols = parseCSVLine(line);

    if (cols.size() < 7) continue;

    int productId = safeToInt(cols[0]);
    std::string description = cols[1];
    double price = safeToDouble(cols[2]);
    int numReviews = safeToInt(cols[3]);
    int stock = safeToInt(cols[4]);
    int sales = safeToInt(cols[5]);
    ProductCategory category = parseCategory(cols[6]);

    products.emplace_back(productId, description, price, numReviews, stock, sales, category);
  }

  return products;
}
