#pragma once
#include "taichi/aot_demo/common.hpp"

namespace ti {
namespace aot_demo {

class Renderer;
class GraphicsTask;

class DrawTextureBuilder {
  using Self = DrawTextureBuilder;
  std::shared_ptr<Renderer> renderer_;

  TiTexture texture_ = {};

  bool is_single_channel_{false};
  glm::vec3 color_{0};

public:
  DrawTextureBuilder(const std::shared_ptr<Renderer> &renderer,
                     const ti::Texture &texture)
      : renderer_(renderer) {
    assert(texture.is_valid());
    texture_ = texture;

    assert(texture.texture().dimension == TI_IMAGE_DIMENSION_2D);
  }

  // color for single channel texture
  Self color(const glm::vec3 &color_) {
    this->color_ = color_;
    return *this;
  }
  Self is_single_channel() {
    this->is_single_channel_ = true;
    return *this;
  }

  std::unique_ptr<GraphicsTask> build();
};

} // namespace aot_demo
} // namespace ti
