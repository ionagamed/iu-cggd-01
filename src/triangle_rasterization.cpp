#include "triangle_rasterization.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <algorithm>
#include <iostream>

cg::TriangleRasterization::TriangleRasterization(unsigned short width,
                                                 unsigned short height,
                                                 std::string obj_file)
    : cg::Projections(width, height, obj_file) {}

cg::TriangleRasterization::~TriangleRasterization() {}

void cg::TriangleRasterization::DrawScene() {
  unsigned id = 0;
  for (auto face : parser->GetFaces()) {
    face.primitive_id = id++;
    auto vs_out = ApplyVertexShader(face);
    Rasterizer(vs_out);
  }
}

void cg::TriangleRasterization::DrawTriangle(face triangle) {
  auto bbox = GetBoundingBox(triangle);

  for (unsigned ccw = 0; ccw < 2; ccw++) {
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
          float3 bary{e0 / area, e1 / area, e2 / area};
          auto ps_out = PixelShader(point, bary, triangle.primitive_id);
          SetPixel(round(x), round(y), ps_out);
        }
      }
    }
  }

  // Wireframe
  DrawLine(triangle.vertices[0].x, triangle.vertices[0].y,
           triangle.vertices[1].x, triangle.vertices[1].y,
           cg::color(255, 0, 0));
  DrawLine(triangle.vertices[1].x, triangle.vertices[1].y,
           triangle.vertices[2].x, triangle.vertices[2].y,
           cg::color(255, 0, 0));
  DrawLine(triangle.vertices[2].x, triangle.vertices[2].y,
           triangle.vertices[0].x, triangle.vertices[0].y,
           cg::color(255, 0, 0));
}

cg::color cg::TriangleRasterization::PixelShader(float2 coord, float3 bary,
                                                 unsigned primitive_id) {
  return color(bary[0] * 255, bary[1] * 255, bary[2] * 255);
}

float cg::TriangleRasterization::EdgeFunction(float2 a, float2 b, float2 x) {
  float dx = b.x - a.x;
  float dy = b.y - a.y;
  return (x.x - a.x) * dy - (x.y - a.y) * dx;
}

cg::BoundingBox cg::TriangleRasterization::GetBoundingBox(face triangle) {
  BoundingBox bbox;
  for (unsigned i = 0; i < 3; i++) {
    bbox.x_min = std::min(bbox.x_min, triangle.vertices[i].x);
    bbox.x_max = std::max(bbox.x_max, triangle.vertices[i].x);
    bbox.y_min = std::min(bbox.y_min, triangle.vertices[i].y);
    bbox.y_max = std::max(bbox.y_max, triangle.vertices[i].y);
  }
  return bbox;
}