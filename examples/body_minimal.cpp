// avoid clang-format disorders headers
// clang-format off
#include <taichi/taichi.h>
#include <soft2d/soft2d.h>
#include "common.h"
#include "globals.h"
#include "taichi/aot_demo/framework.hpp"
// clang-format on

using namespace ti::aot_demo;
using namespace std;

constexpr float win_fov = 1.0 * win_width / win_height;

struct BodyMinimal : public App {

  S2World world;

  std::unique_ptr<GraphicsTask> draw_points;

  ti::NdArray<float> x_;

  virtual AppConfig cfg() const override final {
    AppConfig out{};
    out.app_name = "Soft2D";
    out.framebuffer_width = win_width;
    out.framebuffer_height = win_height;
    return out;
  }

  virtual void initialize() override final {
    GraphicsRuntime &runtime = F.runtime();

    // Soft2D initialization begins
    S2WorldConfig config = default_world_config;
    world = s2_create_world(TiArch::TI_ARCH_VULKAN, runtime, &config);

    // Create a body with a box shape
    S2BoxShape box_shape;
    S2Vec2 half_extent;
    half_extent.x = 0.03f;
    half_extent.y = 0.02f;
    box_shape.half_extent = half_extent;
    S2Shape shape;
    shape.type = S2ShapeType::S2_SHAPE_TYPE_BOX;
    S2ShapeUnion shape_union;
    shape_union.box = box_shape;
    shape.shape_union = shape_union;

    S2Material material{};
    material.type = S2MaterialType::S2_MATERIAL_TYPE_ELASTIC;
    material.density = 1000.0f;
    material.youngs_modulus = 0.3f;
    material.poissons_ratio = 0.2f;

    S2Kinematics kinematics{};
    S2Vec2 center;
    center.x = 0.5f;
    center.y = 0.5f;
    kinematics.center = center;
    kinematics.mobility = S2Mobility::S2_MOBILITY_DYNAMIC;

    s2_create_body(world, &material, &kinematics, &shape, /* tag = */ 0);
    // Soft2D initialization ends

    // Renderer initialization begins
    Renderer &renderer = F.renderer();
    renderer.set_framebuffer_size(win_width, win_height);

    x_ = runtime.allocate_vertex_buffer(90000, 2);

    draw_points = runtime.draw_points(x_)
                      .point_size(preset_point_size)
                      .color(glm::vec3(1, 0.5, 0))
                      .build();
    // Renderer initialization ends
  }
  virtual bool update() override final {
    GraphicsRuntime &runtime = F.runtime();

    // Simulate the world forward by a specified time step
    s2_step(world, 0.004);

    // Export particle position data to the external buffer
    TiNdArray particle_x;
    s2_get_buffer(world, S2_BUFFER_NAME_PARTICLE_POSITION, &particle_x);
    ndarray_data_copy(runtime.runtime(), x_.ndarray(), particle_x,
                      sizeof(float) * 2 * 90000);

    // Since taichi and renderer use different command buffers, we must
    // explicitly use flushing (submitting taichi's command list) here, which
    // provides a semaphore between two command buffers.
    runtime.flush();

    return true;
  }
  virtual void render() override final {
    Renderer &renderer = F.renderer();
    renderer.enqueue_graphics_task(*draw_points);
  }
};

std::unique_ptr<App> create_app() {
  return std::unique_ptr<App>(new BodyMinimal);
}
