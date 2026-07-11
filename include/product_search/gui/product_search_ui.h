#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "imgui.h"
#include "product_search/gui/share.h"
#include "product_search/search_api.h"
#include "product_search/share.h"

class ProductSearchUI {
 public:
  ProductSearchUI() = default;
  ~ProductSearchUI() = default;

  // Called from thread once per frame
  void render();

  // Styles
  void initStyle();

  // API access
  void setSearchAPI(std::shared_ptr<SearchAPI> api);

  // Close
  bool shouldClose() const {
    return shouldClose_.load(std::memory_order_relaxed);
  }

 private:
  // Layout
  void renderSidebar();
  void renderContent();
  bool renderNavGridItem(const char* label, bool active, ImVec2 size);

  // Left sidebar controls
  void renderSearchControls();
  void renderRangeControls();
  void renderMaxControls();
  void renderInsertControls();

  // Right side results
  void renderSearchResults();
  void renderRangeResults();
  void renderMaxResults();
  void renderInsertResults();

  // Usefil widgets
  void renderProductCard(const Product& product);
  void renderTimingBar(const char* label, ImVec4 color, long long ns, long long maxNs,
                       long long us);
  void renderAnalyticsPanel(long long usTree, long long nsTree, long long usHeap, long long nsHeap);

  // Shared state
  std::mutex mutex_;

  std::shared_ptr<SearchAPI> api_;

  std::atomic<bool> shouldClose_{false};
  Mode mode_ = Mode::Search;
  bool showAnalytics_ = false;

  // Search values
  int searchFieldIdx_ = 0;
  char searchValueBuf_[128] = "";
  std::string searchStatus_;
  bool hasSearchRun_ = false;
  std::optional<Product> searchResult_;
  bool searchFoundTree_ = false;
  bool searchFoundHeap_ = false;
  long long searchUsTree_ = 0;
  long long searchNsTree_ = 0;
  long long searchUsHeap_ = 0;
  long long searchNsHeap_ = 0;

  // Range search values
  int rangeFieldIdx_ = 0;
  char rangeMinBuf_[128] = "";
  char rangeMaxBuf_[128] = "";
  std::string rangeStatus_;
  bool hasRangeRun_ = false;
  std::vector<Product> rangeResults_;
  long rangeTreeCount_ = 0;
  long rangeHeapCount_ = 0;
  long long rangeUsTree_ = 0;
  long long rangeNsTree_ = 0;
  long long rangeUsHeap_ = 0;
  long long rangeNsHeap_ = 0;
  size_t visibleRows_ = 100;
  ProductSearchField sortField_ = ProductSearchField::ProductId;
  bool sortAscending_ = true;

  // Max values
  int maxFieldIdx_ = 0;
  std::string maxStatus_;
  bool hasMaxRun_ = false;
  std::optional<Product> maxResult_;
  bool maxFoundTree_ = false;
  bool maxFoundHeap_ = false;
  long long maxUsTree_ = 0;
  long long maxNsTree_ = 0;
  long long maxUsHeap_ = 0;
  long long maxNsHeap_ = 0;

  // Insert valus
  char insertIdBuf_[64] = "";
  char insertDescBuf_[128] = "";
  char insertPriceBuf_[64] = "";
  char insertReviewsBuf_[64] = "";
  char insertStockBuf_[64] = "";
  char insertSalesBuf_[64] = "";
  ProductCategory insertCategory_ = ProductCategory::AudioVideo;
  int bulkInsertCount_ = 10000;
  std::string insertStatus_;
  bool hasInsertRun_ = false;
  std::string insertSummary_;
  long long insertUsTree_ = 0;
  long long insertNsTree_ = 0;
  long long insertUsHeap_ = 0;
  long long insertNsHeap_ = 0;
};
