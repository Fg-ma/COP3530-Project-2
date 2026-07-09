# Amazon Inventory Management

## Overview

This project implements a product search system using custom red-black tree and max heap data structures for indexing and searching product data.

The system loads product information from a CSV file and builds multiple indexes using:

- **Red-Black Trees**
  - Provides efficient ordered searching and range queries.

- **Max Heaps**
  - Used as a comparison structure against the Red-Black Tree implementation.

The search system implements:

- Exact product search
- Range searches
- Product insertion
- Performance benchmarking between data structures

Supported search fields:

- Product ID
- Product Description
- Price
- Number of Reviews
- Stock
- Sales

---

# Requirements

The project requires:

- C++17 or newer
- CMake 3.15+
- A C++ compiler supporting C++17

## GUI Dependencies

The graphical interface requires:

- OpenGL development libraries
- GLFW3
- Dear ImGui
- ImPlot
- Qt6 development libraries

---

# Building the Project

Run the following commands from the **root directory** of the project:

```bash
sudo rm -rf build && sudo rm -rf out

mkdir build && mkdir out

cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=./out

cmake --build build -j$(nproc)

cmake --install build
```

### Build Explanation

1. Remove previous build files:

```bash
sudo rm -rf build out
```

2. Create new build directories:

```bash
mkdir build out
```

3. Generate build files using CMake:

```bash
cmake -S . -B build
```

4. Compile using all available CPU cores:

```bash
cmake --build build -j$(nproc)
```

5. Install generated files:

```bash
cmake --install build
```

---

# Running the Program

After building, run:

```bash
./build/productSearch ./productSearch.conf
```

Requires the configuration file path as an argument.

---

# Configuration File

The configuration file contains runtime settings such as:

- Dataset path
- Log output file
- Logging configuration

---

# Data Structure Comparison

## Red-Black Tree

Advantages:

- Ordered structure
- Efficient exact searches
- Efficient range searches
- Maintains sorted data

Complexities:

| Operation    | Complexity   |
| ------------ | ------------ |
| Insert       | O(log n)     |
| Search       | O(log n)     |
| Range Search | O(log n + k) |

## Max Heap

Advantages:

- Fast insertion
- Fast maximum retrieval
- Simple array-based implementation

Complexities:

| Operation    | Complexity |
| ------------ | ---------- |
| Insert       | O(log n)   |
| Get Max      | O(1)       |
| Search       | O(n)       |
| Range Search | O(n)       |
