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

  float boxSize = (ImGui::GetContentRegionAvail().x - 6.0f) * 0.5f;

  if (renderNavGridItem("Search", mode_ == Mode::Search, ImVec2(boxSize, boxSize))) {
    mode_ = Mode::Search;
    showAnalytics_ = false;
  }
  ImGui::SameLine();
  if (renderNavGridItem("Range Search", mode_ == Mode::RangeSearch, ImVec2(boxSize, boxSize))) {
    mode_ = Mode::RangeSearch;
    showAnalytics_ = false;
  }

  ImGui::Dummy(ImVec2(0, 2.0f));

  if (renderNavGridItem("Max", mode_ == Mode::Max, ImVec2(boxSize, boxSize))) {
    mode_ = Mode::Max;
    showAnalytics_ = false;
  }
  ImGui::SameLine();
  if (renderNavGridItem("Insert", mode_ == Mode::Insert, ImVec2(boxSize, boxSize))) {
    mode_ = Mode::Insert;
    showAnalytics_ = false;
  }

  ImGui::Dummy(ImVec2(0, 2.0f));

  if (renderNavGridItem("Disk Usage", mode_ == Mode::DiskSize,
                        ImVec2(ImGui::GetContentRegionAvail().x, 42.0f))) {
    mode_ = Mode::DiskSize;
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
    case Mode::Max:
      renderMaxControls();
      break;
    case Mode::Insert:
      renderInsertControls();
      break;
  }

  if (!api_) {
    ImGui::Dummy(ImVec2(0, 20));
    ImGui::TextColored(ImVec4(0.95f, 0.45f, 0.45f, 1.0f), "No SearchAPI attached");
  }

  ImGui::Dummy(ImVec2(0.0f, 240.0f));
}

void ProductSearchUI::renderContent() {
  switch (mode_) {
    case Mode::Search:
      renderSearchResults();
      break;
    case Mode::RangeSearch:
      renderRangeResults();
      break;
    case Mode::Max:
      renderMaxResults();
      break;
    case Mode::Insert:
      renderInsertResults();
      break;
    case Mode::DiskSize:
      renderDiskSizeResults();
      break;
  }
}

bool ProductSearchUI::renderNavGridItem(const char* label, bool active, ImVec2 size) {
  ImGui::PushID(label);

  ImGui::PushStyleColor(
      ImGuiCol_ChildBg,
      active ? ImVec4(kAccentTree.x, kAccentTree.y, kAccentTree.z, 0.18f) : kCardBg);

  bool clicked = false;

  ImGui::BeginChild(label, size, true,
                    ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

  ImVec2 childSize = ImGui::GetWindowSize();
  ImVec2 childPos = ImGui::GetWindowPos();

  if (ImGui::InvisibleButton("button", childSize)) {
    clicked = true;
  }

  bool hovered = ImGui::IsItemHovered();

  if (hovered && !active) {
    ImGui::GetWindowDrawList()->AddRectFilled(
        childPos, ImVec2(childPos.x + childSize.x, childPos.y + childSize.y),
        ImGui::GetColorU32(ImVec4(kAccentTree.x, kAccentTree.y, kAccentTree.z, 0.12f)), 6.0f);
  }

  if (active) {
    ImGui::GetWindowDrawList()->AddRectFilled(childPos,
                                              ImVec2(childPos.x + 4, childPos.y + childSize.y),
                                              ImGui::GetColorU32(kAccentTree), 4.0f);
  }

  ImVec2 textSize = ImGui::CalcTextSize(label);

  ImVec2 textPos(childPos.x + (childSize.x - textSize.x) * 0.5f,
                 childPos.y + (childSize.y - textSize.y) * 0.5f);

  ImGui::GetWindowDrawList()->AddText(textPos, ImGui::GetColorU32(ImGuiCol_Text), label);

  ImGui::EndChild();

  ImGui::PopStyleColor();
  ImGui::PopID();

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

void ProductSearchUI::renderMaxControls() {
  ImGui::TextDisabled("FIELD");

  ImGui::SetNextItemWidth(-1);

  if (ImGui::BeginCombo("##maxfield", fieldLabel(allFields[maxFieldIdx_]))) {
    for (int i = 0; i < allFieldCount; i++) {
      bool selected = maxFieldIdx_ == i;

      if (ImGui::Selectable(fieldLabel(allFields[i]), selected)) {
        maxFieldIdx_ = i;
      }

      if (selected) ImGui::SetItemDefaultFocus();
    }

    ImGui::EndCombo();
  }

  ImGui::Dummy(ImVec2(0, 16));

  if (ImGui::Button("Find Maximum", ImVec2(-1, 40))) {
    maxStatus_.clear();
    maxResult_.reset();
    hasMaxRun_ = true;
    showAnalytics_ = false;

    if (!api_) {
      maxStatus_ = "Warning no SearchAPI attached to UI";
      return;
    }

    MaxRequest request;
    request.maxField = allFields[maxFieldIdx_];

    SearchAnalytics analytics = api_->max(request);

    maxUsTree_ = analytics.usTreeTimeTaken;
    maxNsTree_ = analytics.nsTreeTimeTaken;

    maxUsHeap_ = analytics.usHeapTimeTaken;
    maxNsHeap_ = analytics.nsHeapTimeTaken;

    maxFoundTree_ = analytics.treeResult != nullptr;
    maxFoundHeap_ = analytics.heapResult != nullptr;

    if (analytics.treeResult) {
      maxResult_ = *analytics.treeResult;
    } else if (analytics.heapResult) {
      maxResult_ = *analytics.heapResult;
    }

    if (!analytics.error.empty()) {
      maxStatus_ = analytics.error;
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
    insertSummary_.clear();
    showAnalytics_ = false;
    hasInsertRun_ = true;

    insertUsTree_ = 0;
    insertNsTree_ = 0;
    insertUsHeap_ = 0;
    insertNsHeap_ = 0;

    std::vector<std::string> missingFields;

    if (strlen(insertIdBuf_) == 0) missingFields.push_back("Product ID");

    if (strlen(insertDescBuf_) == 0) missingFields.push_back("Description");

    if (strlen(insertPriceBuf_) == 0) missingFields.push_back("Price");

    if (strlen(insertReviewsBuf_) == 0) missingFields.push_back("Num Reviews");

    if (strlen(insertStockBuf_) == 0) missingFields.push_back("Stock");

    if (strlen(insertSalesBuf_) == 0) missingFields.push_back("Sales");

    if (!missingFields.empty()) {
      insertStatus_ = "Missing required fields: ";

      for (size_t i = 0; i < missingFields.size(); i++) {
        insertStatus_ += missingFields[i];
        if (i + 1 < missingFields.size()) insertStatus_ += ", ";
      }
    } else {
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

          insertSummary_ = "Inserted product #" + std::to_string(id) + " \"" +
                           std::string(insertDescBuf_) + "\"";

        } catch (const std::exception& ex) {
          insertStatus_ = std::string("Invalid input: ") + ex.what();
        }
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
    return;
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
    return;
  }

  ImGui::Text("%zu results found", rangeResults_.size());
  ImGui::Dummy(ImVec2(0, 10));

  ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(10, 6));

  ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter |
                          ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_ScrollY |
                          ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable |
                          ImGuiTableFlags_Sortable;

  ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(10.0f, 8.0f));

  if (ImGui::BeginTable("##RangeTable", 7, flags,
                        ImVec2(ImGui::GetContentRegionAvail().x - 12.0f, 420))) {
    ImGui::TableSetupColumn("ID",
                            ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort,
                            70, ProductSearchField::ProductId);
    ImGui::TableSetupColumn("Description",
                            ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_None, 0,
                            ProductSearchField::ProductDescription);
    ImGui::TableSetupColumn("Price", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_None,
                            90, ProductSearchField::Price);
    ImGui::TableSetupColumn("Reviews",
                            ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_None, 80,
                            ProductSearchField::NumReviews);
    ImGui::TableSetupColumn("Stock", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_None,
                            80, ProductSearchField::Stock);
    ImGui::TableSetupColumn("Sales", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_None,
                            80, ProductSearchField::Sales);
    ImGui::TableSetupColumn("Category",
                            ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_None, 140, 42);

    ImGui::TableSetupScrollFreeze(0, 1);

    ImGui::TableHeadersRow();

    if (ImGuiTableSortSpecs* sortSpecs = ImGui::TableGetSortSpecs()) {
      if (sortSpecs->SpecsDirty) {
        const ImGuiTableColumnSortSpecs& spec = sortSpecs->Specs[0];

        bool ascending = spec.SortDirection == ImGuiSortDirection_Ascending;

        std::sort(
            rangeResults_.begin(), rangeResults_.end(), [&](const Product& a, const Product& b) {
              switch (spec.ColumnUserID) {
                case ProductSearchField::ProductId:
                  return ascending ? a.productId < b.productId : a.productId > b.productId;
                case ProductSearchField::ProductDescription:
                  return ascending ? a.productDescription < b.productDescription
                                   : a.productDescription > b.productDescription;
                case ProductSearchField::Price:
                  return ascending ? a.price < b.price : a.price > b.price;
                case ProductSearchField::NumReviews:
                  return ascending ? a.numReviews < b.numReviews : a.numReviews > b.numReviews;
                case ProductSearchField::Stock:
                  return ascending ? a.stock < b.stock : a.stock > b.stock;
                case ProductSearchField::Sales:
                  return ascending ? a.sales < b.sales : a.sales > b.sales;
                case 42:
                  return ascending ? a.category < b.category : a.category > b.category;
              }

              return false;
            });

        sortSpecs->SpecsDirty = false;
      }
    }

    size_t shown = std::min(visibleRows_, rangeResults_.size());

    for (size_t i = 0; i < shown; i++) {
      const auto& p = rangeResults_[i];

      ImGui::TableNextRow();

      if (ImGui::IsItemHovered()) {
        ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, IM_COL32(70, 70, 90, 80));
      }

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

      ImGui::TableSetColumnIndex(5);
      ImGui::Text("%d", p.sales);

      ImGui::TableSetColumnIndex(6);
      ImGui::TextUnformatted(toString(p.category).c_str());
    }

    ImGui::EndTable();

    if (shown < rangeResults_.size()) {
      ImGui::Dummy(ImVec2(0, 8));

      if (ImGui::Button("Show More", ImVec2(140, 36))) {
        visibleRows_ += 100;
      }

      ImGui::SameLine();

      if (ImGui::Button("Show All", ImVec2(140, 36))) {
        visibleRows_ = rangeResults_.size();
      }

      ImGui::TextDisabled("Showing %zu of %zu", shown, rangeResults_.size());
    }
  }

  ImGui::PopStyleVar(2);

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

void ProductSearchUI::renderMaxResults() {
  ImGui::Dummy(ImVec2(0, 10));

  ImGui::TextColored(ImVec4(0.92f, 0.93f, 0.95f, 1), "Maximum result");

  if (!hasMaxRun_) {
    ImGui::TextDisabled("No results yet");
    return;
  }

  if (!maxStatus_.empty()) {
    ImGui::TextColored(kError, "%s", maxStatus_.c_str());
    return;
  }

  if (maxResult_) {
    renderProductCard(*maxResult_);
  } else {
    ImGui::TextDisabled("No product found");
  }

  ImGui::Dummy(ImVec2(0, 18));

  if (ImGui::Button(showAnalytics_ ? "Hide analytics" : "Show analytics", ImVec2(180, 38))) {
    showAnalytics_ = !showAnalytics_;
  }

  if (showAnalytics_) {
    renderAnalyticsPanel(maxUsTree_, maxNsTree_, maxUsHeap_, maxNsHeap_);

    ImGui::Dummy(ImVec2(0, 4));

    ImGui::TextDisabled("Tree match: %s     Heap match: %s", maxFoundTree_ ? "yes" : "no",
                        maxFoundHeap_ ? "yes" : "no");
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
    return;
  }

  if (!insertSummary_.empty()) {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, kCardBg);
    ImGui::BeginChild("##insertsummary", ImVec2(ImGui::GetContentRegionAvail().x - 12.0f, 100),
                      true);
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

void ProductSearchUI::renderDiskSizeResults() {
  ImGui::Dummy(ImVec2(0, 10));
  ImGui::TextColored(kAccentTree, "Disk / Memory Usage");
  ImGui::Dummy(ImVec2(0, 10));

  if (!api_) {
    ImGui::TextColored(kError, "No SearchAPI attached.");
    return;
  }

  DiskSizeAnalytics sizes = api_->diskSize();

  ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(10.0f, 8.0f));

  if (ImGui::BeginTable(
          "##DiskSizes", 3,
          ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp,
          ImVec2(ImGui::GetContentRegionAvail().x - 12.0f, 0))) {
    ImGui::TableSetupColumn("Structure");
    ImGui::TableSetupColumn("Red-Black Tree");
    ImGui::TableSetupColumn("Max Heap");
    ImGui::TableHeadersRow();

    auto row = [](const char* label, size_t tree, size_t heap) {
      ImGui::TableNextRow();

      ImGui::TableSetColumnIndex(0);
      ImGui::TextUnformatted(label);

      ImGui::TableSetColumnIndex(1);
      ImGui::Text("%zu bytes", tree);

      ImGui::TableSetColumnIndex(2);
      ImGui::Text("%zu bytes", heap);
    };

    row("Product ID", sizes.productIdTreeSize, sizes.productIdHeapSize);

    row("Description", sizes.productDescriptionTreeSize, sizes.productDescriptionHeapSize);

    row("Price", sizes.priceTreeSize, sizes.priceHeapSize);

    row("Reviews", sizes.numReviewsTreeSize, sizes.numReviewsHeapSize);

    row("Stock", sizes.stockTreeSize, sizes.stockHeapSize);

    row("Sales", sizes.salesTreeSize, sizes.salesHeapSize);

    ImGui::EndTable();
  }

  ImGui::PopStyleVar();

  ImGui::Dummy(ImVec2(0, 20));

  size_t totalTree = sizes.productIdTreeSize + sizes.productDescriptionTreeSize +
                     sizes.priceTreeSize + sizes.numReviewsTreeSize + sizes.stockTreeSize +
                     sizes.salesTreeSize;

  size_t totalHeap = sizes.productIdHeapSize + sizes.productDescriptionHeapSize +
                     sizes.priceHeapSize + sizes.numReviewsHeapSize + sizes.stockHeapSize +
                     sizes.salesHeapSize;

  ImGui::Separator();
  ImGui::Dummy(ImVec2(0, 10));

  ImGui::TextColored(kAccentTree, "Totals");

  ImGui::Text("Red-Black Trees : %.2f MB", totalTree / (1024.0 * 1024.0));

  ImGui::TextColored(kAccentHeap, "Max Heaps      : %.2f MB", totalHeap / (1024.0 * 1024.0));

  ImGui::Text("Difference      : %.2f MB",
              std::abs((double)totalTree - (double)totalHeap) / (1024.0 * 1024.0));
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
  ImVec2 pos = ImGui::GetCursorScreenPos();
  float width = ImGui::GetContentRegionAvail().x - 12.0f;
  float thickness = 1.0f;
  ImGui::GetWindowDrawList()->AddLine(pos, ImVec2(pos.x + width, pos.y),
                                      ImGui::GetColorU32(ImGuiCol_Border), thickness);
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
  ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
  ImGui::BeginChild("##Sidebar", ImVec2(340, 0), true);
  renderSidebar();
  ImGui::EndChild();
  ImGui::PopStyleColor();
  ImGui::PopStyleVar(2);

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
