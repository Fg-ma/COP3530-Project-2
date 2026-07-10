#include "product_search/gui/product_search_ui.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <optional>
#include <random>
#include <sstream>
#include <vector>

#include "imgui.h"

namespace {

const ImVec4 kAccentTree(0.35f, 0.85f, 0.78f, 1.00f);
const ImVec4 kAccentHeap(0.75f, 0.55f, 0.95f, 1.00f);
const ImVec4 kAccentHover(0.45f, 0.90f, 0.83f, 1.00f);
const ImVec4 kTextMuted(0.58f, 0.61f, 0.66f, 1.00f);
const ImVec4 kSidebarBg(0.09f, 0.10f, 0.13f, 1.00f);
const ImVec4 kContentBg(0.055f, 0.062f, 0.078f, 1.00f);
const ImVec4 kCardBg(0.11f, 0.13f, 0.16f, 1.00f);
const ImVec4 kError(0.95f, 0.5f, 0.45f, 1.00f);

// Dark color scheme
void applyDarkTheme() {
  ImGuiStyle& style = ImGui::GetStyle();
  ImVec4* colors = style.Colors;

  style.WindowRounding = 8.0f;
  style.ChildRounding = 12.0f;
  style.FrameRounding = 6.0f;
  style.PopupRounding = 6.0f;
  style.ScrollbarRounding = 8.0f;
  style.GrabRounding = 6.0f;
  style.WindowPadding = ImVec2(12, 12);
  style.FramePadding = ImVec2(10, 8);
  style.ItemSpacing = ImVec2(10, 8);
  style.ScrollbarSize = 14.0f;
  style.WindowBorderSize = 0.0f;
  style.PopupBorderSize = 1.0f;
  style.FrameBorderSize = 1.0f;
  style.IndentSpacing = 16.0f;

  const ImVec4 bg(0.055f, 0.062f, 0.078f, 1.00f);
  const ImVec4 bgAlt(0.11f, 0.13f, 0.16f, 1.00f);
  const ImVec4 border(0.18f, 0.20f, 0.24f, 1.00f);
  const ImVec4 text(0.92f, 0.93f, 0.95f, 1.00f);

  colors[ImGuiCol_Text] = text;
  colors[ImGuiCol_TextDisabled] = kTextMuted;
  colors[ImGuiCol_WindowBg] = bg;
  colors[ImGuiCol_ChildBg] = kSidebarBg;
  colors[ImGuiCol_PopupBg] = bgAlt;
  colors[ImGuiCol_Border] = border;
  colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);
  colors[ImGuiCol_FrameBg] = bgAlt;
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.17f, 0.21f, 1.00f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.17f, 0.20f, 0.24f, 1.00f);
  colors[ImGuiCol_TitleBg] = bg;
  colors[ImGuiCol_TitleBgActive] = bg;
  colors[ImGuiCol_ScrollbarBg] = bg;
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.24f, 0.26f, 0.31f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.30f, 0.33f, 0.39f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive] = kAccentTree;
  colors[ImGuiCol_CheckMark] = kAccentTree;
  colors[ImGuiCol_SliderGrab] = kAccentTree;
  colors[ImGuiCol_SliderGrabActive] = kAccentHover;
  colors[ImGuiCol_Button] = ImVec4(0.16f, 0.19f, 0.23f, 1.00f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(kAccentTree.x, kAccentTree.y, kAccentTree.z, 0.35f);
  colors[ImGuiCol_ButtonActive] = ImVec4(kAccentTree.x, kAccentTree.y, kAccentTree.z, 0.55f);
  colors[ImGuiCol_Header] = ImVec4(kAccentTree.x, kAccentTree.y, kAccentTree.z, 0.20f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(kAccentTree.x, kAccentTree.y, kAccentTree.z, 0.28f);
  colors[ImGuiCol_HeaderActive] = ImVec4(kAccentTree.x, kAccentTree.y, kAccentTree.z, 0.35f);
  colors[ImGuiCol_Separator] = border;
  colors[ImGuiCol_SeparatorHovered] = kAccentTree;
  colors[ImGuiCol_SeparatorActive] = kAccentTree;
  colors[ImGuiCol_ResizeGrip] = ImVec4(kAccentTree.x, kAccentTree.y, kAccentTree.z, 0.25f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(kAccentTree.x, kAccentTree.y, kAccentTree.z, 0.55f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(kAccentTree.x, kAccentTree.y, kAccentTree.z, 0.75f);
  colors[ImGuiCol_TableHeaderBg] = bgAlt;
  colors[ImGuiCol_TableBorderStrong] = border;
  colors[ImGuiCol_TableBorderLight] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
  colors[ImGuiCol_TableRowBg] = bg;
  colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.08f, 0.09f, 0.11f, 1.00f);
  colors[ImGuiCol_TextSelectedBg] = ImVec4(kAccentTree.x, kAccentTree.y, kAccentTree.z, 0.35f);
  colors[ImGuiCol_NavHighlight] = kAccentTree;
}

// Make random garabge products to test insert speed
std::vector<Product> generateRandomProducts(int count) {
  std::vector<Product> products;
  products.reserve(static_cast<size_t>(std::max(count, 0)));

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<int> idDist(100000000, 999999999);
  std::uniform_int_distribution<int> numberDist(0, 100000);
  std::uniform_real_distribution<double> priceDist(1.0, 3000.0);
  std::uniform_int_distribution<int> categoryDist(0, 8);

  const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  std::uniform_int_distribution<int> charDist(0, static_cast<int>(chars.size()) - 1);

  for (int i = 0; i < count; i++) {
    std::string description;
    for (int j = 0; j < 20; j++) description += chars[charDist(gen)];

    products.emplace_back(idDist(gen), description, priceDist(gen), numberDist(gen),
                          numberDist(gen), numberDist(gen),
                          static_cast<ProductCategory>(categoryDist(gen)));
  }

  return products;
}

}  // namespace

// Layout

void ProductSearchUI::renderSidebar() {
  ImGui::TextColored(kAccentTree, "PRODUCT SEARCH");
  ImGui::TextDisabled("Red-Black Tree vs Max-Heap");
  ImGui::Dummy(ImVec2(0, 20));

  if (renderNavItem("Search", mode_ == Mode::Search)) {
    mode_ = Mode::Search;
    showAnalytics_ = false;
  }
  ImGui::Dummy(ImVec2(0, 4));
  if (renderNavItem("Range Search", mode_ == Mode::RangeSearch)) {
    mode_ = Mode::RangeSearch;
    showAnalytics_ = false;
  }
  ImGui::Dummy(ImVec2(0, 4));
  if (renderNavItem("Insert", mode_ == Mode::Insert)) {
    mode_ = Mode::Insert;
    showAnalytics_ = false;
  }

  ImGui::Dummy(ImVec2(0, 22));
  ImGui::Separator();
  ImGui::Dummy(ImVec2(0, 18));

  switch (mode_) {
    case Mode::Search:
      renderSearchControls();
      break;
    case Mode::RangeSearch:
      renderRangeControls();
      break;
    case Mode::Insert:
      renderInsertControls();
      break;
  }

  if (!api_) {
    ImGui::Dummy(ImVec2(0, 20));
    ImGui::TextColored(ImVec4(0.95f, 0.45f, 0.45f, 1.0f), "No SearchAPI attached");
  }
}

void ProductSearchUI::renderContent() {
  switch (mode_) {
    case Mode::Search:
      renderSearchResults();
      break;
    case Mode::RangeSearch:
      renderRangeResults();
      break;
    case Mode::Insert:
      renderInsertResults();
      break;
  }
}

bool ProductSearchUI::renderNavItem(const char* label, bool active) {
  ImVec2 size(ImGui::GetContentRegionAvail().x, 42);
  ImVec2 pos = ImGui::GetCursorScreenPos();

  ImGui::PushID(label);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(16, 0));
  bool clicked = ImGui::Selectable(label, active, 0, size);
  ImGui::PopStyleVar();
  ImGui::PopID();

  if (active) {
    ImDrawList* dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(pos, ImVec2(pos.x + 3, pos.y + size.y), ImGui::GetColorU32(kAccentTree),
                      2.0f);
  }
  return clicked;
}

// Left sidebar controls

void ProductSearchUI::renderSearchControls() {
  ImGui::TextDisabled("FIELD");
  ImGui::SetNextItemWidth(-1);
  if (ImGui::BeginCombo("##searchfield", fieldLabel(allFields[searchFieldIdx_]))) {
    for (int i = 0; i < allFieldCount; i++) {
      bool selected = (searchFieldIdx_ == i);
      if (ImGui::Selectable(fieldLabel(allFields[i]), selected)) searchFieldIdx_ = i;
      if (selected) ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }

  ProductSearchField field = allFields[searchFieldIdx_];

  ImGui::Dummy(ImVec2(0, 10));
  ImGui::TextDisabled("VALUE");
  ImGui::SetNextItemWidth(-1);
  if (fieldIsNumeric(field)) {
    ImGui::InputText("##searchval", searchValueBuf_, sizeof(searchValueBuf_),
                     ImGuiInputTextFlags_CharsDecimal);
  } else {
    ImGui::InputText("##searchval", searchValueBuf_, sizeof(searchValueBuf_));
  }

  ImGui::Dummy(ImVec2(0, 16));
  if (ImGui::Button("Run Search", ImVec2(-1, 40))) {
    searchStatus_.clear();
    showAnalytics_ = false;
    hasSearchRun_ = true;
    searchResult_.reset();
    searchFoundTree_ = searchFoundHeap_ = false;

    if (!api_) {
      searchStatus_ = "Warning no SearchAPI attached to UI";
    } else {
      SearchRequest request;
      request.searchField = field;
      bool parsed = true;

      if (fieldIsNumeric(field)) {
        try {
          request.searchValue = std::stod(searchValueBuf_);
        } catch (...) {
          parsed = false;
          searchStatus_ = "Enter a valid number";
        }
      } else {
        request.searchValue = std::string(searchValueBuf_);
      }

      if (parsed) {
        SearchAnalytics analytics = api_->search(request);
        searchUsTree_ = analytics.usTreeTimeTaken;
        searchNsTree_ = analytics.nsTreeTimeTaken;
        searchUsHeap_ = analytics.usHeapTimeTaken;
        searchNsHeap_ = analytics.nsHeapTimeTaken;
        searchFoundTree_ = (analytics.treeResult != nullptr);
        searchFoundHeap_ = (analytics.heapResult != nullptr);
        if (analytics.treeResult) {
          searchResult_ = *analytics.treeResult;
        } else if (analytics.heapResult) {
          searchResult_ = *analytics.heapResult;
        } else {
          searchResult_.reset();
        }
        if (!analytics.error.empty()) searchStatus_ = analytics.error;
      }
    }
  }
}

void ProductSearchUI::renderRangeControls() {
  ImGui::TextDisabled("FIELD");
  ImGui::SetNextItemWidth(-1);
  if (ImGui::BeginCombo("##rangefield", fieldLabel(allFields[rangeFieldIdx_]))) {
    for (int i = 0; i < allFieldCount; i++) {
      bool selected = (rangeFieldIdx_ == i);
      if (ImGui::Selectable(fieldLabel(allFields[i]), selected)) rangeFieldIdx_ = i;
      if (selected) ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }

  ProductSearchField field = allFields[rangeFieldIdx_];

  ImGui::Dummy(ImVec2(0, 10));
  ImGui::TextDisabled("MIN");
  ImGui::SetNextItemWidth(-1);
  ImGui::InputText("##rangemin", rangeMinBuf_, sizeof(rangeMinBuf_));

  ImGui::Dummy(ImVec2(0, 10));
  ImGui::TextDisabled("MAX");
  ImGui::SetNextItemWidth(-1);
  ImGui::InputText("##rangemax", rangeMaxBuf_, sizeof(rangeMaxBuf_));

  ImGui::Dummy(ImVec2(0, 16));
  if (ImGui::Button("Run Range Search", ImVec2(-1, 40))) {
    rangeStatus_.clear();
    showAnalytics_ = false;
    hasRangeRun_ = true;
    rangeResults_.clear();
    rangeTreeCount_ = 0;
    rangeHeapCount_ = 0;

    if (!api_) {
      rangeStatus_ = "Warning no SearchAPI attached to UI";
    } else {
      SearchRangeRequest request;
      request.searchField = field;
      bool parsed = true;

      if (fieldIsNumeric(field)) {
        try {
          request.searchMinValue = std::stod(rangeMinBuf_);
          request.searchMaxValue = std::stod(rangeMaxBuf_);
        } catch (...) {
          parsed = false;
          rangeStatus_ = "Enter valid numbers for min and max parameters";
        }
      } else {
        request.searchMinValue = std::string(rangeMinBuf_);
        request.searchMaxValue = std::string(rangeMaxBuf_);
      }

      if (parsed) {
        SearchRangeAnalytics analytics = api_->searchRange(request);
        rangeUsTree_ = analytics.usTreeTimeTaken;
        rangeNsTree_ = analytics.nsTreeTimeTaken;
        rangeUsHeap_ = analytics.usHeapTimeTaken;
        rangeNsHeap_ = analytics.nsHeapTimeTaken;
        rangeTreeCount_ = analytics.treeResultsReturned;
        rangeHeapCount_ = analytics.heapResultsReturned;
        rangeResults_ = analytics.treeResults;
        if (!analytics.error.empty()) rangeStatus_ = analytics.error;
      }
    }
  }
}

void ProductSearchUI::renderInsertControls() {
  ImGui::TextDisabled("SINGLE PRODUCT");
  ImGui::Dummy(ImVec2(0, 8));

  ImGui::SetNextItemWidth(-1);
  ImGui::InputTextWithHint("##insertid", "Product ID", insertIdBuf_, sizeof(insertIdBuf_),
                           ImGuiInputTextFlags_CharsDecimal);
  ImGui::Dummy(ImVec2(0, 6));
  ImGui::SetNextItemWidth(-1);
  ImGui::InputTextWithHint("##insertdesc", "Description", insertDescBuf_, sizeof(insertDescBuf_));
  ImGui::Dummy(ImVec2(0, 6));
  ImGui::SetNextItemWidth(-1);
  ImGui::InputTextWithHint("##insertprice", "Price", insertPriceBuf_, sizeof(insertPriceBuf_),
                           ImGuiInputTextFlags_CharsDecimal);
  ImGui::Dummy(ImVec2(0, 6));
  ImGui::SetNextItemWidth(-1);
  ImGui::InputTextWithHint("##insertreviews", "Num Reviews", insertReviewsBuf_,
                           sizeof(insertReviewsBuf_), ImGuiInputTextFlags_CharsDecimal);
  ImGui::Dummy(ImVec2(0, 6));
  ImGui::SetNextItemWidth(-1);
  ImGui::InputTextWithHint("##insertstock", "Stock", insertStockBuf_, sizeof(insertStockBuf_),
                           ImGuiInputTextFlags_CharsDecimal);
  ImGui::Dummy(ImVec2(0, 6));
  ImGui::SetNextItemWidth(-1);
  ImGui::InputTextWithHint("##insertsales", "Sales", insertSalesBuf_, sizeof(insertSalesBuf_),
                           ImGuiInputTextFlags_CharsDecimal);

  ImGui::Dummy(ImVec2(0, 6));
  ImGui::SetNextItemWidth(-1);
  {
    std::string current = toString(insertCategory_);

    if (ImGui::BeginCombo("##insertcategory", current.c_str())) {
      for (int i = ProductCategory::AudioVideo; i <= ProductCategory::Toys; i++) {
        ProductCategory category = static_cast<ProductCategory>(i);

        bool selected = (insertCategory_ == category);

        std::string label = toString(category);

        if (ImGui::Selectable(label.c_str(), selected)) insertCategory_ = category;

        if (selected) ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
  }

  ImGui::Dummy(ImVec2(0, 14));
  if (ImGui::Button("Insert Product", ImVec2(-1, 40))) {
    insertStatus_.clear();
    showAnalytics_ = false;
    hasInsertRun_ = true;

    if (!api_) {
      insertStatus_ = "Warning no SearchAPI attached to UI";
    } else {
      try {
        int id = std::stoi(insertIdBuf_);
        double price = std::stod(insertPriceBuf_);
        int reviews = std::stoi(insertReviewsBuf_);
        int stock = std::stoi(insertStockBuf_);
        int sales = std::stoi(insertSalesBuf_);
        ProductCategory category = static_cast<ProductCategory>(insertCategory_);

        Product product(id, std::string(insertDescBuf_), price, reviews, stock, sales, category);
        InsertRequest request{std::nullopt, product};
        InsertAnalytics analytics = api_->insert(request);

        insertUsTree_ = analytics.usTreeTimeTaken;
        insertNsTree_ = analytics.nsTreeTimeTaken;
        insertUsHeap_ = analytics.usHeapTimeTaken;
        insertNsHeap_ = analytics.nsHeapTimeTaken;
        insertSummary_ =
            "Inserted product #" + std::to_string(id) + " \"" + std::string(insertDescBuf_) + "\"";
      } catch (const std::exception& ex) {
        insertStatus_ = std::string("Invalid input: ") + ex.what();
      }
    }
  }

  ImGui::Dummy(ImVec2(0, 22));
  ImGui::Separator();
  ImGui::Dummy(ImVec2(0, 18));

  ImGui::TextDisabled("BULK INSERT (RANDOM)");
  ImGui::Dummy(ImVec2(0, 8));
  ImGui::SetNextItemWidth(-1);
  ImGui::InputInt("##bulkcount", &bulkInsertCount_);
  bulkInsertCount_ = std::clamp(bulkInsertCount_, 1, 5000000);

  ImGui::Dummy(ImVec2(0, 10));
  if (ImGui::Button("Generate and insert", ImVec2(-1, 40))) {
    insertStatus_.clear();
    showAnalytics_ = false;
    hasInsertRun_ = true;

    if (!api_) {
      insertStatus_ = "Warning No SearchAPI attached to UI";
    } else {
      std::vector<Product> products = generateRandomProducts(bulkInsertCount_);
      InsertRequest request{std::nullopt, products};
      InsertAnalytics analytics = api_->insert(request);

      insertUsTree_ = analytics.usTreeTimeTaken;
      insertNsTree_ = analytics.nsTreeTimeTaken;
      insertUsHeap_ = analytics.usHeapTimeTaken;
      insertNsHeap_ = analytics.nsHeapTimeTaken;
      insertSummary_ =
          "Inserted " + std::to_string(products.size()) + " randomly generated products";
    }
  }
}

// Right side results

void ProductSearchUI::renderSearchResults() {
  ImGui::Dummy(ImVec2(0, 10));
  ImGui::TextColored(ImVec4(0.92f, 0.93f, 0.95f, 1.0f), "Search results");
  ImGui::Dummy(ImVec2(0, 4));

  if (!hasSearchRun_) {
    ImGui::TextDisabled("No results yet");
    return;
  }

  if (!searchStatus_.empty()) {
    ImGui::TextColored(kError, "%s", searchStatus_.c_str());
    ImGui::Dummy(ImVec2(0, 10));
  }

  if (searchResult_) {
    renderProductCard(*searchResult_);
  } else {
    ImGui::TextDisabled("Hmm no products seem to match this search...");
  }

  ImGui::Dummy(ImVec2(0, 18));
  if (ImGui::Button(showAnalytics_ ? "Hide analytics" : "Show analytics", ImVec2(180, 38))) {
    showAnalytics_ = !showAnalytics_;
  }

  if (showAnalytics_) {
    renderAnalyticsPanel(searchUsTree_, searchNsTree_, searchUsHeap_, searchNsHeap_);
    ImGui::Dummy(ImVec2(0, 4));
    ImGui::TextDisabled("Tree match: %s     Heap match: %s", searchFoundTree_ ? "yes" : "no",
                        searchFoundHeap_ ? "yes" : "no");
  }
}

void ProductSearchUI::renderRangeResults() {
  ImGui::Dummy(ImVec2(0, 10));
  ImGui::TextColored(ImVec4(0.92f, 0.93f, 0.95f, 1.0f), "Range search results");
  ImGui::Dummy(ImVec2(0, 4));

  if (!hasRangeRun_) {
    ImGui::TextDisabled("No results yet");
    return;
  }

  if (!rangeStatus_.empty()) {
    ImGui::TextColored(kError, "%s", rangeStatus_.c_str());
    ImGui::Dummy(ImVec2(0, 10));
  }

  ImGui::Text("%zu results found", rangeResults_.size());
  ImGui::Dummy(ImVec2(0, 10));

  ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders |
                          ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable;
  if (ImGui::BeginTable("##RangeTable", 5, flags, ImVec2(0, 320))) {
    ImGui::TableSetupColumn("ID");
    ImGui::TableSetupColumn("Description");
    ImGui::TableSetupColumn("Price");
    ImGui::TableSetupColumn("Reviews");
    ImGui::TableSetupColumn("Stock");
    ImGui::TableHeadersRow();

    size_t shown = std::min<size_t>(rangeResults_.size(), 500);
    for (size_t i = 0; i < shown; i++) {
      const auto& p = rangeResults_[i];
      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::Text("%d", p.productId);
      ImGui::TableSetColumnIndex(1);
      ImGui::TextUnformatted(p.productDescription.c_str());
      ImGui::TableSetColumnIndex(2);
      ImGui::Text("$%.2f", p.price);
      ImGui::TableSetColumnIndex(3);
      ImGui::Text("%d", p.numReviews);
      ImGui::TableSetColumnIndex(4);
      ImGui::Text("%d", p.stock);
    }
    ImGui::EndTable();
  }

  if (rangeResults_.size() > 500) {
    ImGui::TextDisabled("Showing first 500 of %zu products found", rangeResults_.size());
  }

  ImGui::Dummy(ImVec2(0, 18));
  if (ImGui::Button(showAnalytics_ ? "Hide analytics" : "Show analytics", ImVec2(180, 38))) {
    showAnalytics_ = !showAnalytics_;
  }

  if (showAnalytics_) {
    renderAnalyticsPanel(rangeUsTree_, rangeNsTree_, rangeUsHeap_, rangeNsHeap_);
    ImGui::Dummy(ImVec2(0, 4));
    ImGui::TextDisabled("Tree found %zu     Heap found %zu", rangeTreeCount_, rangeHeapCount_);
  }
}

void ProductSearchUI::renderInsertResults() {
  ImGui::Dummy(ImVec2(0, 10));
  ImGui::TextColored(ImVec4(0.92f, 0.93f, 0.95f, 1.0f), "Insert result");
  ImGui::Dummy(ImVec2(0, 4));

  if (!hasInsertRun_) {
    ImGui::TextDisabled("No results yet");
    return;
  }

  if (!insertStatus_.empty()) {
    ImGui::TextColored(kError, "%s", insertStatus_.c_str());
    ImGui::Dummy(ImVec2(0, 10));
  }

  if (!insertSummary_.empty()) {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, kCardBg);
    ImGui::BeginChild("##insertsummary", ImVec2(0, 70), true);
    ImGui::TextColored(kAccentTree, "Success");
    ImGui::TextWrapped("%s", insertSummary_.c_str());
    ImGui::EndChild();
    ImGui::PopStyleColor();
  }

  ImGui::Dummy(ImVec2(0, 18));
  if (ImGui::Button(showAnalytics_ ? "Hide analytics" : "Show analytics", ImVec2(180, 38))) {
    showAnalytics_ = !showAnalytics_;
  }

  if (showAnalytics_) {
    renderAnalyticsPanel(insertUsTree_, insertNsTree_, insertUsHeap_, insertNsHeap_);
  }
}

// Widgets

void ProductSearchUI::renderProductCard(const Product& product) {
  ImGui::PushStyleColor(ImGuiCol_ChildBg, kCardBg);
  ImGui::BeginChild("##productcard", ImVec2(ImGui::GetContentRegionAvail().x - 12.0f, 260), true);

  ImGui::TextColored(kAccentTree, "Product #%d", product.productId);
  ImGui::TextWrapped("%s", product.productDescription.c_str());
  ImGui::Dummy(ImVec2(0, 8));
  ImGui::Separator();
  ImGui::Dummy(ImVec2(0, 10));

  ImGui::Columns(2, nullptr, false);
  ImGui::TextDisabled("Price ($)");
  ImGui::Text("$%.2f", product.price);
  ImGui::NextColumn();
  ImGui::TextDisabled("Category");
  ImGui::Text("%s", toString(product.category).c_str());
  ImGui::NextColumn();

  ImGui::TextDisabled("Reviews");
  ImGui::Text("%d", product.numReviews);
  ImGui::NextColumn();
  ImGui::TextDisabled("Stock aviable");
  ImGui::Text("%d", product.stock);
  ImGui::NextColumn();

  ImGui::TextDisabled("Life time sales");
  ImGui::Text("%d", product.sales);
  ImGui::NextColumn();
  ImGui::NextColumn();
  ImGui::Columns(1);

  ImGui::EndChild();
  ImGui::PopStyleColor();
}

void ProductSearchUI::renderTimingBar(const char* label, ImVec4 color, long long ns,
                                      long long maxNs, long long us) {
  ImGui::TextColored(color, "%s", label);
  ImGui::SameLine(150);
  ImGui::Text("%lld ns  (%lld us)", ns, us);

  float frac = maxNs > 0 ? static_cast<float>(ns) / static_cast<float>(maxNs) : 0.0f;
  frac = std::clamp(frac, 0.02f, 1.0f);

  ImVec2 pos = ImGui::GetCursorScreenPos();
  float width = ImGui::GetContentRegionAvail().x - 12.0f;
  float height = 16.0f;

  ImDrawList* drawList = ImGui::GetWindowDrawList();
  ImU32 trackCol = ImGui::GetColorU32(ImVec4(1, 1, 1, 0.06f));
  ImU32 fillCol = ImGui::GetColorU32(color);

  drawList->AddRectFilled(pos, ImVec2(pos.x + width, pos.y + height), trackCol, height * 0.5f);
  drawList->AddRectFilled(pos, ImVec2(pos.x + width * frac, pos.y + height), fillCol,
                          height * 0.5f);

  ImGui::Dummy(ImVec2(width, height + 8));
}

void ProductSearchUI::renderAnalyticsPanel(long long usTree, long long nsTree, long long usHeap,
                                           long long nsHeap) {
  ImGui::Dummy(ImVec2(0, 6));
  ImGui::Separator();
  ImGui::Dummy(ImVec2(0, 12));
  ImGui::TextDisabled("PERFORMANCE");
  ImGui::Dummy(ImVec2(0, 8));

  long long maxNs = std::max<long long>(std::max<long long>(nsTree, nsHeap), 1);
  renderTimingBar("Red-Black Tree", kAccentTree, nsTree, maxNs, usTree);
  ImGui::Dummy(ImVec2(0, 6));
  renderTimingBar("Max Heap", kAccentHeap, nsHeap, maxNs, usHeap);

  ImGui::Dummy(ImVec2(0, 6));
  if (nsTree != nsHeap) {
    const char* faster = nsTree < nsHeap ? "Red-Black Tree" : "Max Heap";
    long long diff = std::llabs(nsTree - nsHeap);
    ImGui::TextDisabled("%s was faster by %lld ns", faster, diff);
  } else {
    ImGui::TextDisabled("Both took the same time");
  }
}

// Public funcs

void ProductSearchUI::render() {
  const ImGuiIO& io = ImGui::GetIO();
  ImGui::SetNextWindowPos({0, 0});
  ImGui::SetNextWindowSize(io.DisplaySize);

  ImGuiWindowFlags hostFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                               ImGuiWindowFlags_NoBringToFrontOnFocus;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::Begin("##ProductSearchHost", nullptr, hostFlags);
  ImGui::PopStyleVar(2);

  // Left sidebar
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 22));
  ImGui::PushStyleColor(ImGuiCol_ChildBg, kSidebarBg);
  ImGui::BeginChild("##Sidebar", ImVec2(340, 0), true);
  renderSidebar();
  ImGui::EndChild();
  ImGui::PopStyleColor();
  ImGui::PopStyleVar();

  ImGui::SameLine();

  // Right results
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(28, 26));
  ImGui::PushStyleColor(ImGuiCol_ChildBg, kContentBg);
  ImGui::BeginChild("##Content", ImVec2(0, 0), false);
  renderContent();
  ImGui::EndChild();
  ImGui::PopStyleColor();
  ImGui::PopStyleVar();

  ImGui::End();
}

void ProductSearchUI::initStyle() {
  ImGui::StyleColorsDark();
  applyDarkTheme();
}

void ProductSearchUI::setSearchAPI(std::shared_ptr<SearchAPI> api) {
  api_ = std::move(api);
}
