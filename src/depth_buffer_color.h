#pragma once

#include "triangle_rasterization.h"

using namespace linalg::ostream_overloads;

namespace cg {

class ZCullingColor : public TriangleRasterization {
 public:
  ZCullingColor(unsigned short width, unsigned short height, std::string obj_file);
  virtual ~ZCullingColor();

  void DrawScene();
  void Clear();

 protected:
  virtual void DrawTriangle(face triangle);
  virtual color PixelShader(float2 coord, float3 bary, unsigned primitive_id, float z);
  bool DepthTest(unsigned x, unsigned y, float z);
  void SetPixel(unsigned short x, unsigned short y, color color, float z);
  std::vector<float> depth_buffer;

  std::vector<color> colors;
  int draw_call_counter;
};

}  // namespace cg
