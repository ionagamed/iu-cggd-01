#include <iostream>

#include "depth_buffer_color.h"

int main(int argc, char* argv[]) {
  try {
    cg::ZCullingColor* render = new cg::ZCullingColor(1920, 1080, "models/CornellBox-original.obj");
    render->Clear();
    render->DrawScene();
    render->LookAt(
      float3{0, 0, 3},
      float3{0, 0, 0},
      float3{0, 1, 0}
    );
    render->Save("results/depth_buffer_color.png");
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}