#pragma once

#include <cstdint>
#include <glm/common.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <optional>
#include <vector>

#include <glm/glm.hpp>

#define MAKE_NON_COPYABLE(x)                                                   \
  ~x();                                                                        \
  x(const x &) = delete;                                                       \
  x(x &&) noexcept {};
#define MAKE_NON_MOVABLE(x)                                                    \
  ~x();                                                                        \
  x &operator=(const x &) = delete;                                            \
  x &operator=(x &&) = delete;
#define MAKE_NON_COPYABLE_NOR_MOVABLE(x)                                       \
  ~x();                                                                        \
  x(const x &) = delete;                                                       \
  x(x &&) noexcept;                                                            \
  x &operator=(const x &) = delete;                                            \
  x &operator=(x &&) = delete;

namespace bs {
struct Pixel {
  operator glm::vec4() { return *this; }
  friend Pixel operator*(const Pixel &p, const float &x) {
    return {static_cast<uint8_t>(p.r * x), static_cast<uint8_t>(p.g * x),
            static_cast<uint8_t>(p.b * x) /*, static_cast<uint8_t>(p.a * x)*/};
  }

  uint8_t r;
  uint8_t g;
  uint8_t b;
  // uint8_t a;
};

class Image {
public:
  Image(uint32_t width, uint32_t height);
  MAKE_NON_COPYABLE_NOR_MOVABLE(Image)

  void write(char *path);
  Pixel read_pixel(glm::vec2 coord);
  void write_pixel(glm::vec2 coord, Pixel pixel);
  void resize(glm::vec2 size);
  glm::vec2 size() { return m_size; };

private:
  std::vector<Pixel> m_image;
  glm::vec2 m_size{0, 0};
};
class Circle {
public:
  Circle();
  Circle(glm::vec3 position, float radius) {
    m_position = position, m_radius = radius;
  };
  MAKE_NON_COPYABLE(Circle)
  float radius() { return m_radius; };
  glm::vec3 position() { return m_position; };

private:
  glm::vec3 m_position{};
  float m_radius{0.0f};
};
class Ray {
public:
  Ray(glm::vec2 uv_coordinates);
  glm::vec3 origin() { return m_origin; };
  glm::vec3 destination() { return m_destination; };
  glm::vec3 direction() { return m_direction; };
  glm::vec2 uv_coordinates() { return m_uv_coordinates; };
  std::optional<glm::vec3> intersection(Circle &circle);

private:
  glm::vec3 m_origin{};
  glm::vec3 m_destination{};
  glm::vec3 m_direction{};
  glm::vec2 m_uv_coordinates{};
};
class RTCircle {
public:
  RTCircle();
  MAKE_NON_COPYABLE_NOR_MOVABLE(RTCircle)

private:
  Image m_image;
  std::vector<std::vector<Ray>> m_ray_image;
  Circle example_circle;
};
} // namespace bs
