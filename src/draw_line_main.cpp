#include <iostream>

#include "draw_line.h"

int main(int argc, char* argv[]) {
  try {
    cg::LineDrawing* render = new cg::LineDrawing(1920, 1080);
    render->Clear();
    render->DrawScene();
    render->Save("results/draw_line.png");
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}