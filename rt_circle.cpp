#include "rt_circle.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

namespace bs {
Image::Image(uint32_t width, uint32_t height) : m_image(width * height) {
  m_size.x = width;
  m_size.y = height;
}
Image::~Image() {}
void Image::write(char *path) {
  stbi_write_png(path, m_size.x, m_size.y, 3, m_image.data(), m_size.x * 3);
}
Pixel Image::read_pixel(glm::vec2 coord) {
  return m_image[coord.y * m_size.x + coord.x];
}
void Image::write_pixel(glm::vec2 coord, Pixel pixel) {
  m_image[coord.y * m_size.x + coord.x] = pixel;
}
void Image::resize(glm::vec2 size) {
  m_size = size;
  m_image.resize(size.x * size.y);
}

Circle::Circle() {}
Circle::~Circle() {}

Ray::Ray(glm::vec2 uv_coordinates) {
  m_origin = {0, 0, 0};
  if (uv_coordinates.x == 0) {
    uv_coordinates.x = 1;
  }
  if (uv_coordinates.y == 0) {
    uv_coordinates.y = 1;
  }
  m_destination = {uv_coordinates.x, uv_coordinates.y, -100};
  m_direction = glm::normalize(m_destination);
  m_uv_coordinates = uv_coordinates;
}
std::optional<glm::vec3> Ray::intersection(Circle &circle) {
  auto a = glm::dot(direction(), direction());
  auto b = 2 * glm::dot(circle.position(), direction());
  auto c =
      glm::dot(circle.position(), circle.position()) - powf(circle.radius(), 2);
  float discriminant = (powf(b, 2) - 4 * a * c);
  if (discriminant <= 0) {
    return {};
  }
  auto distance1 = (-b + sqrt(discriminant)) / (2 * a);
  auto distance2 = (-b - sqrt(discriminant)) / (2 * a);
  return origin() + (direction() * std::min(distance1, distance2));
}

RTCircle::RTCircle() : m_image(1920, 1080), example_circle({0, 0, -5}, 4) {
  Circle example_circle_2({0, 0, -3}, 2);
  m_ray_image.resize(1920);
  for (int x = 0; x < 1920; x++) {
    for (int y = 0; y < 1080; y++) {
      m_ray_image[x].emplace_back(Ray({}));
      m_ray_image[x][y] = Ray({x - 960, y - 540});
    }
  }
  glm::vec3 sun_location = {-5, -5, -10};
  int coord_x{0}, coord_y{0};
  for (auto &x : m_ray_image) {
    coord_x++;
    for (auto &y : x) {
      coord_y++;
      auto opt_intersection = y.intersection(example_circle);
      if (opt_intersection) {
        m_image.write_pixel(
            {coord_x, coord_y},
            Pixel{255, 0, 0} * 0.5f *
                (2.5f / opt_intersection.value().length()) *
                glm::dot(
                    glm::normalize(example_circle.position() -
                                   opt_intersection.value()),
                    glm::normalize(sun_location - example_circle.position())));
      }
      auto opt_intersection2 = y.intersection(example_circle_2);
      if (opt_intersection2) {
        m_image.write_pixel(
            {coord_x, coord_y},
            Pixel{0, 0, 255} * 0.5f *
                (2.5f / opt_intersection2.value().length() *
                 glm::dot(glm::normalize(example_circle_2.position() -
                                         opt_intersection2.value()),
                          glm::normalize(sun_location -
                                         example_circle_2.position()))));
      }
    }
    coord_y = 0;
  }
  m_image.write("rt_circle.png");
}
RTCircle::~RTCircle() {}
} // namespace bs
