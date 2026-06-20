cmake_minimum_required(VERSION 3.14)
project(productSearch)

option(ENABLE_SANITIZERS "Enable Address & Undefined Behavior sanitizers" OFF)

if(ENABLE_SANITIZERS)
  message(STATUS "Sanitizers enabled")

  add_compile_options(-fsanitize=address -fsanitize=undefined
                      -fno-omit-frame-pointer -g)

  add_link_options(-fsanitize=address -fsanitize=undefined)
endif()

# ------------------------------------------
# C++ standard
# ------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE
      Debug
      CACHE STRING "Choose the type of build." FORCE)
endif()

# ------------------------------------------
# Paths
# ------------------------------------------
set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})
set(SRC_DIR ${PROJECT_ROOT}/src)
set(INCLUDE_DIR ${PROJECT_ROOT}/include)
set(LIB_DIR ${PROJECT_ROOT}/lib)

# --------------------
# System dependencies
# --------------------
find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Boost 1.70 REQUIRED COMPONENTS system thread)
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)

# ------------------------------------------
# Dear ImGui
# ------------------------------------------
add_library(
  imgui
  ${LIB_DIR}/imgui/imgui.cpp
  ${LIB_DIR}/imgui/imgui_draw.cpp
  ${LIB_DIR}/imgui/imgui_tables.cpp
  ${LIB_DIR}/imgui/imgui_widgets.cpp
  ${LIB_DIR}/imgui/backends/imgui_impl_opengl3.cpp
  ${LIB_DIR}/imgui/backends/imgui_impl_glfw.cpp)
target_include_directories(imgui PUBLIC ${LIB_DIR}/imgui
                                        ${LIB_DIR}/imgui/backends)
target_link_libraries(imgui PUBLIC OpenGL::GL)

# ------------------------------------------
# ImPlot
# ------------------------------------------
add_library(implot ${LIB_DIR}/implot/implot.cpp
                   ${LIB_DIR}/implot/implot_items.cpp)
target_include_directories(implot PUBLIC ${LIB_DIR}/implot)
target_link_libraries(implot PUBLIC imgui)

# --------------------
# JSON + JSON Schema Validator
# --------------------
include(FetchContent)

find_package(nlohmann_json QUIET)
if(NOT nlohmann_json_FOUND)
  FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP true)
  FetchContent_MakeAvailable(json)
endif()

FetchContent_Declare(
  json_validator
  GIT_REPOSITORY https://github.com/pboettch/json-schema-validator.git
  GIT_TAG main)
FetchContent_MakeAvailable(json_validator)

# --------------------
# Executable
# --------------------
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp")
add_executable(${PROJECT_NAME} ${PROJECT_ROOT}/productSearch.cpp ${SRC_FILES})

# ------------------------------------------
# Compile options
# ------------------------------------------
target_compile_options(
  ${PROJECT_NAME}
  PRIVATE $<$<CONFIG:Debug>:
          -O0
          -g
          -ggdb
          -fno-omit-frame-pointer
          -Wall
          -Wextra
          -Wpedantic
          -fsanitize=address,undefined
          >)

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>
                                                   $<$<CONFIG:Release>:NDEBUG>)

# ------------------------------------------
# Include directories
# ------------------------------------------
target_include_directories(
  ${PROJECT_NAME} PUBLIC ${INCLUDE_DIR} ${LIB_DIR}/imgui
                         ${LIB_DIR}/imgui/backends)

# --------------------
# Link libraries
# --------------------
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE Threads::Threads
          nlohmann_json::nlohmann_json
          nlohmann_json_schema_validator
          yaml-cpp
          Boost::system
          Boost::thread
          imgui
          implot
          OpenGL::GL
          glfw)

add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}>
          ${PROJECT_ROOT}/out/)
