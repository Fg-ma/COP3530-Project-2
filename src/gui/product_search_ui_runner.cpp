#include "product_search/gui/product_search_ui_runner.h"

#include <cstdio>
#include <memory>
#include <stdexcept>

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

namespace {

constexpr int kInitWidth = 1400;
constexpr int kInitHeight = 860;
constexpr double kTargetFPS = 30.0;
constexpr double kFrameTime = 1.0 / kTargetFPS;

void glfwErrorCallback(int error, const char* description) {
  std::fprintf(stderr, "[GLFW] error %d: %s\n", error, description);
}

}  // namespace

void runUIThread(std::shared_ptr<ProductSearchUI> ui) {
  // GLFW init
  glfwSetErrorCallback(glfwErrorCallback);
  if (!glfwInit()) throw std::runtime_error("ProductSearchUI: glfwInit() failed");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window =
      glfwCreateWindow(kInitWidth, kInitHeight, "Product search", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    throw std::runtime_error("ProductSearchUI: glfwCreateWindow() failed");
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  // ImGui init
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();

  ui->initStyle();

  ImGuiIO& io = ImGui::GetIO();
  io.IniFilename = nullptr;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  // Render loop
  while (!glfwWindowShouldClose(window) && !ui->shouldClose()) {
    const double frameStart = glfwGetTime();

    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ui->render();

    ImGui::Render();

    int displayW, displayH;
    glfwGetFramebufferSize(window, &displayW, &displayH);
    glViewport(0, 0, displayW, displayH);
    glClearColor(0.08f, 0.09f, 0.11f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);

    // FPS cap
    const double elapsed = glfwGetTime() - frameStart;
    if (elapsed < kFrameTime) {
      const double sleepUntil = frameStart + kFrameTime;
      while (glfwGetTime() < sleepUntil) {
      }
    }
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImPlot::DestroyContext();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}
