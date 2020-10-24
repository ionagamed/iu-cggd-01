#pragma once

#include "projections.h"

namespace cg {

struct BoundingBox {
  float x_min;
  float y_min;
  float x_max;
  float y_max;

  BoundingBox() : x_min(1e9), y_min(1e9), x_max(0), y_max(0) {}

  BoundingBox(float x_min, float y_min, float x_max, float y_max)
      : x_min(x_min), y_min(y_min), x_max(x_max), y_max(y_max) {}
};

class TriangleRasterization : public Projections {
 public:
  TriangleRasterization(unsigned short width, unsigned short height,
                        std::string obj_file);
  virtual ~TriangleRasterization();

  virtual void DrawScene();

 protected:
  virtual color PixelShader(float2 coord, float3 bary, unsigned primitive_id);
  virtual void DrawTriangle(face triangle);
  BoundingBox GetBoundingBox(face triangle);
  float EdgeFunction(float2 a, float2 b, float2 x);
};

}  // namespace cg
