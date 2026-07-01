#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "product_search/share.h"

std::vector<Product> loadCSV(const std::string& filename);
