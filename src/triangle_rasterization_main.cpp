#include <chrono>
#include <iostream>

#include "triangle_rasterization.h"

int main(int argc, char* argv[]) {
  try {
    // cg::TriangleRasterization* render =
    //     new cg::TriangleRasterization(1920, 1080, "models/cube.obj");
    std::chrono::high_resolution_clock clock;
    cg::TriangleRasterization* render = new cg::TriangleRasterization(
        1920, 1080, "models/CornellBox-original.obj");
    render->Clear();

    auto time_before = clock.now();
    render->DrawScene();
    auto time_after = clock.now();

    auto time_delta_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        time_after - time_before);
    std::cout << "Rendering took " << time_delta_ms.count() << "ms"
              << std::endl;

    render->Save("results/triangle_rasterization.png");
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}