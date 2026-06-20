#pragma once

#include <atomic>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "imgui.h"
#include "implot.h"
#include "productSearch/gui/share.h"

class ProductSearchUI {
 public:
  ProductSearchUI() = default;
  ~ProductSearchUI() = default;

  // Thread-safe snapshot update may be called from any thread.
  void updateState(const ProductSearchUIState& state);

  // Must be called from the ImGui render thread once per frame.
  void render();

  bool shouldClose() const {
    return _shouldClose.load(std::memory_order_relaxed);
  }

  void initStyle();

  void setDownloadCallback(std::function<void()> cb);

 private:
  // State
  std::mutex _mutex;
  ProductSearchUIState _state;  // protected by _mutex
  ProductSearchUIState _snap;   // render-thread copy, set at top of render()
  std::atomic<bool> _shouldClose{false};
};
