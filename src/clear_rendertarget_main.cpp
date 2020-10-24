#include <chrono>
#include <iostream>

#include "clear_rendertarget.h"

int main(int argc, char* argv[]) {
  try {
    // std::chrono::high_resolution_clock perf_clock;
    cg::ClearRenderTarget* render = new cg::ClearRenderTarget(1920, 1080);

    // auto time_before = perf_clock.now();
    render->Clear();
    // auto time_after = perf_clock.now();

    // std::cout << "render->Clear() took "
              // << std::chrono::duration_cast<std::chrono::milliseconds>(
                    //  time_after - time_before)
                    //  .count()
              // << "ms" << std::endl;

    render->Save("results/clear_rendertarget.png");
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}