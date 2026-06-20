#include "productSearch/gui/productSearchUi.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <deque>
#include <fstream>
#include <sstream>
#include <vector>

#include "imgui.h"
#include "implot.h"

// Style constants

namespace {

constexpr ImVec4 kGreen{0.18f, 0.82f, 0.44f, 1.00f};
constexpr ImVec4 kRed{0.95f, 0.30f, 0.30f, 1.00f};
constexpr ImVec4 kYellow{0.98f, 0.82f, 0.25f, 1.00f};
constexpr ImVec4 kGray{0.55f, 0.55f, 0.60f, 1.00f};
constexpr ImVec4 kWhite{1.00f, 1.00f, 1.00f, 1.00f};
constexpr ImVec4 kDimWhite{0.80f, 0.80f, 0.80f, 1.00f};
constexpr ImVec4 kCardBg{0.10f, 0.11f, 0.14f, 1.00f};
constexpr ImVec4 kAccent{0.25f, 0.55f, 0.95f, 1.00f};

constexpr float kLeftWidth = 280.0f;
constexpr float kPadding = 10.0f;

void applyDarkTheme() {
  ImGuiStyle& s = ImGui::GetStyle();
  s.WindowPadding = {kPadding, kPadding};
  s.FramePadding = {6, 4};
  s.ItemSpacing = {8, 6};
  s.ItemInnerSpacing = {4, 4};
  s.ScrollbarSize = 10;
  s.WindowRounding = 6;
  s.FrameRounding = 4;
  s.TabRounding = 4;
  s.GrabRounding = 3;

  ImVec4* c = s.Colors;
  c[ImGuiCol_WindowBg] = {0.08f, 0.09f, 0.11f, 1.00f};
  c[ImGuiCol_ChildBg] = {0.10f, 0.11f, 0.14f, 1.00f};
  c[ImGuiCol_PopupBg] = {0.10f, 0.11f, 0.14f, 1.00f};
  c[ImGuiCol_Border] = {0.20f, 0.21f, 0.26f, 1.00f};
  c[ImGuiCol_FrameBg] = {0.14f, 0.15f, 0.18f, 1.00f};
  c[ImGuiCol_FrameBgHovered] = {0.18f, 0.19f, 0.23f, 1.00f};
  c[ImGuiCol_FrameBgActive] = {0.22f, 0.23f, 0.28f, 1.00f};
  c[ImGuiCol_TitleBg] = {0.08f, 0.09f, 0.11f, 1.00f};
  c[ImGuiCol_TitleBgActive] = {0.10f, 0.11f, 0.14f, 1.00f};
  c[ImGuiCol_MenuBarBg] = {0.10f, 0.11f, 0.14f, 1.00f};
  c[ImGuiCol_Header] = {0.18f, 0.22f, 0.32f, 1.00f};
  c[ImGuiCol_HeaderHovered] = {0.22f, 0.28f, 0.42f, 1.00f};
  c[ImGuiCol_HeaderActive] = {0.25f, 0.32f, 0.50f, 1.00f};
  c[ImGuiCol_Button] = {0.16f, 0.20f, 0.28f, 1.00f};
  c[ImGuiCol_ButtonHovered] = {0.22f, 0.28f, 0.40f, 1.00f};
  c[ImGuiCol_ButtonActive] = {0.25f, 0.32f, 0.50f, 1.00f};
  c[ImGuiCol_Tab] = {0.12f, 0.13f, 0.17f, 1.00f};
  c[ImGuiCol_TabHovered] = {0.22f, 0.28f, 0.42f, 1.00f};
  c[ImGuiCol_TabActive] = {0.18f, 0.22f, 0.35f, 1.00f};
  c[ImGuiCol_ScrollbarBg] = {0.08f, 0.09f, 0.11f, 1.00f};
  c[ImGuiCol_ScrollbarGrab] = {0.22f, 0.23f, 0.28f, 1.00f};
  c[ImGuiCol_ScrollbarGrabHovered] = {0.30f, 0.31f, 0.38f, 1.00f};
  c[ImGuiCol_Separator] = {0.18f, 0.19f, 0.24f, 1.00f};
  c[ImGuiCol_Text] = {0.92f, 0.92f, 0.94f, 1.00f};
  c[ImGuiCol_TextDisabled] = {0.45f, 0.45f, 0.50f, 1.00f};
}

}  // namespace

void ProductSearchUI::updateState(const ProductSearchUIState& state) {
  std::lock_guard<std::mutex> lock(_mutex);
  _state = state;
}

void ProductSearchUI::render() {
  // Pull a consistent snapshot for this frame.
  {
    std::lock_guard<std::mutex> lock(_mutex);
    _snap = _state;
  }

  // Full-screen host window
  const ImGuiIO& io = ImGui::GetIO();
  ImGui::SetNextWindowPos({0, 0});
  ImGui::SetNextWindowSize(io.DisplaySize);

  ImGuiWindowFlags hostFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                               ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::Begin("##ProductSearchHost", nullptr, hostFlags);
  ImGui::PopStyleVar(2);

  // Menu bar
  if (ImGui::BeginMenuBar()) {
    ImGui::TextColored(kAccent, "  ProductSearch");
    ImGui::SameLine();
    ImGui::TextDisabled("|");
    ImGui::SameLine();

    ImGui::EndMenuBar();
  }

  // Two-column body
  ImGui::SetCursorPos({kPadding, ImGui::GetCursorPosY()});

  ImGui::BeginGroup();
  ImGui::EndGroup();

  ImGui::SameLine(0, 8);

  const float rightW = io.DisplaySize.x - kLeftWidth - kPadding * 3 - 8;
  const float rightH = io.DisplaySize.y - ImGui::GetCursorPosY() - kPadding;
  ImGui::BeginChild("##right", {rightW, rightH}, false);
  ImGui::EndChild();

  ImGui::End();
}

void ProductSearchUI::initStyle() {
  applyDarkTheme();
}
