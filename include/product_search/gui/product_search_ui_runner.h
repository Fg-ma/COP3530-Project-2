#pragma once

#include <memory>

#include "product_search/gui/product_search_ui.h"

void runUIThread(std::shared_ptr<ProductSearchUI> ui);
