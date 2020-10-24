#include "depth_buffer_color.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>

cg::ZCullingColor::ZCullingColor(unsigned short width, unsigned short height,
                       std::string obj_file)
    : TriangleRasterization(width, height, obj_file) {
  depth_buffer = std::vector<float>(static_cast<size_t>(width * height), 1);
}

cg::ZCullingColor::~ZCullingColor() {}

void cg::ZCullingColor::DrawScene() {
  unsigned id = 0;
  for (auto face : parser->GetFaces()) {
    face.primitive_id = id++;
    auto vs_out = ApplyVertexShader(face);
    Rasterizer(vs_out);
  }
}

void cg::ZCullingColor::Clear() {
  frame_buffer = std::vector<color>(static_cast<size_t>(width * height));
  depth_buffer = std::vector<float>(static_cast<size_t>(width * height), 1e9);
}

void cg::ZCullingColor::DrawTriangle(face triangle) {
  auto bbox = GetBoundingBox(triangle);

  bool ccw = false;
  float d = ccw ? -1 : 1;
  float area =
      d * EdgeFunction(triangle.vertices[0].xy(), triangle.vertices[1].xy(),
                       triangle.vertices[2].xy());

  for (float x = bbox.x_min; x <= bbox.x_max; x += 1.f) {
    for (float y = bbox.y_min; y <= bbox.y_max; y += 1.f) {
      float2 point{x, y};
      float e0 = d * EdgeFunction(triangle.vertices[0].xy(),
                                  triangle.vertices[1].xy(), point);
      float e1 = d * EdgeFunction(triangle.vertices[1].xy(),
                                  triangle.vertices[2].xy(), point);
      float e2 = d * EdgeFunction(triangle.vertices[2].xy(),
                                  triangle.vertices[0].xy(), point);
      if (e0 >= 0 && e1 >= 0 && e2 >= 0) {
        float3 bary{e1 / area, e2 / area, e0 / area};
        float z = bary[0] * triangle.vertices[0].z +
                  bary[1] * triangle.vertices[1].z +
                  bary[2] * triangle.vertices[2].z;
        auto ps_out = PixelShader(point, bary, triangle.primitive_id, z);
        SetPixel(round(x), round(y), ps_out, z);
      }
    }
  }

  // Wireframe
  // DrawLine(triangle.vertices[0].x, triangle.vertices[0].y,
  //          triangle.vertices[1].x, triangle.vertices[1].y,
  //          cg::color(255, 0, 0));
  // DrawLine(triangle.vertices[1].x, triangle.vertices[1].y,
  //          triangle.vertices[2].x, triangle.vertices[2].y,
  //          cg::color(255, 0, 0));
  // DrawLine(triangle.vertices[2].x, triangle.vertices[2].y,
  //          triangle.vertices[0].x, triangle.vertices[0].y,
  //          cg::color(255, 0, 0));
}

cg::color cg::ZCullingColor::PixelShader(float2 coord, float3 bary, unsigned primitive_id, float z) {
  return color((1 - z) * 255, (1 - z) * 255, (1 - z) * 255);
}

bool cg::ZCullingColor::DepthTest(unsigned x, unsigned y, float z) {
  return z < depth_buffer[y * width + x];
}

void cg::ZCullingColor::SetPixel(unsigned short x, unsigned short y, color color,
                            float z) {
  if (DepthTest(x, y, z)) {
    frame_buffer[y * width + x] = color;
    depth_buffer[y * width + x] = z;
  }
}
