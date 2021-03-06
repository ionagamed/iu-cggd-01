#include <iostream>

#include "depth_buffer.h"

int main(int argc, char* argv[]) {
  try {
    cg::ZCulling* render = new cg::ZCulling(1920, 1080, "models/z_test.obj");
    // cg::ZCulling* render = new cg::ZCulling(1920, 1080, "models/CornellBox-original.obj");
    render->Clear();
    render->DrawScene();
    render->LookAt(
      float3{0, 0, 3},
      float3{0, 0, 0},
      float3{0, 1, 0}
    );
    render->Save("results/depth_buffer.png");
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}