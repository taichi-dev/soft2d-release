// avoid clang-format disorders headers
// clang-format off
#include <taichi/taichi.h>
#include <soft2d/soft2d.h>
#include "common.h"
#include "globals.h"
#include "emitter.h"
#include "taichi/aot_demo/framework.hpp"
// clang-format on

using namespace ti::aot_demo;
using namespace std;

constexpr int win_width = 800;
constexpr int win_height = 800;
constexpr float win_fov = 1.0 * win_width / win_height;

struct Colliders : public App {

  S2World world;
  Emitter emitter;

  std::unique_ptr<GraphicsTask> draw_points;
  std::unique_ptr<GraphicsTask> draw_texture;

  ti::NdArray<float> x_;
  ti::Texture collider_texture_;

  virtual AppConfig cfg() const override final {
    AppConfig out{};
    out.app_name = "Soft2D";
    out.framebuffer_width = win_width;
    out.framebuffer_height = win_height;
    return out;
  }

  virtual void initialize() override final {

    // Renderer initialization
    Renderer &renderer = F.renderer();
    renderer.set_framebuffer_size(win_width, win_height);

    GraphicsRuntime &runtime = F.runtime();

    x_ = runtime.allocate_vertex_buffer(
        default_world_config.max_allowed_particle_num, 2);

    collider_texture_ =
        runtime.allocate_texture2d(default_world_config.grid_resolution *
                                       default_world_config.fine_grid_scale,
                                   default_world_config.grid_resolution *
                                       default_world_config.fine_grid_scale,
                                   TI_FORMAT_R32F, TI_NULL_HANDLE);

    draw_points = runtime.draw_points(x_)
                      .point_size(3.0f)
                      .color(glm::vec3(1, 0.5, 0))
                      .build();
    draw_texture = runtime.draw_texture(collider_texture_)
                       .is_single_channel()
                       .color(glm::vec3(0.2, 0.8, 0.0))
                       .build();

    // Soft2D initialization
    // Create a world
    S2WorldConfig config = default_world_config;
    world = s2_create_world(TiArch::TI_ARCH_VULKAN, runtime, &config);

    S2Shape shape = make_box_shape(vec2(0.02f, 0.02));

    S2Material material;
    material.type = S2MaterialType::S2_MATERIAL_TYPE_FLUID;
    material.density = 1000.0f;
    material.youngs_modulus = 1.0f;
    material.poissons_ratio = 0.2f;

    S2Kinematics kinematics{};

    kinematics.center = vec2(0.2f, 0.9f);
    kinematics.linear_velocity = vec2(4.0f, 0.0f);
    kinematics.mobility = S2Mobility::S2_MOBILITY_DYNAMIC;

    emitter = Emitter(world, material, kinematics, shape);
    emitter.SetFrequency(5);

    make_collider(world, make_kinematics({0.5f, 0.5f}, 0.3),
                  make_box_shape(vec2(0.05f, 0.05f)));

    make_collider(world, make_kinematics({0.8f, 0.8f}, 0.7854),
                  make_box_shape(vec2(0.1f, 0.01f)));

    make_collider(world, make_kinematics({0.8f, 0.2f}),
                  make_circle_shape(0.04));

    make_collider(world, make_kinematics({0.6f, 0.2f}),
                  make_ellipse_shape(0.04, 0.02));

    make_collider(world, make_kinematics({0.4f, 0.2f}),
                  make_capsule_shape(0.04, 0.02));

    auto polygon_vertices = std::vector<S2Vec2>{
        {-0.05, -0.05},   {-0.015, -0.025}, {0.0, -0.05},   {0.0125, 0.0125},
        {0.0125, 0.0375}, {-0.0375, 0.025}, {-0.05625, 0.0}};
    S2Shape polygon_shape =
        make_polygon_shape(polygon_vertices.data(), polygon_vertices.size());
    make_collider(world, make_kinematics({0.2f, 0.2f}, 1.4), polygon_shape);

    // Add the boundary
    // bottom
    make_collider(world, make_kinematics({0.5f, 0.0f}),
                  make_box_shape(vec2(0.5f, 0.01f)));
    // top
    make_collider(world, make_kinematics({0.5f, 1.0f}),
                  make_box_shape(vec2(0.5f, 0.01f)));
    // left
    make_collider(world, make_kinematics({0.0f, 0.5f}),
                  make_box_shape(vec2(0.01f, 0.5f)));
    // right
    make_collider(world, make_kinematics({1.0f, 0.5f}),
                  make_box_shape(vec2(0.01f, 0.5f)));
  }
  int frame = 0;
  virtual bool update() override final {
    GraphicsRuntime &runtime = F.runtime();

    emitter.Update(frame);

    s2_step(world, 0.004);

    // Export particle position data to the external buffer
    TiNdArray particle_x;
    s2_get_buffer(world, S2_BUFFER_NAME_PARTICLE_POSITION, &particle_x);
    ndarray_data_copy(runtime.runtime(), x_.ndarray(), particle_x,
                      sizeof(float) * 2 *
                          default_world_config.max_allowed_particle_num);

    // Export collider buffer to texture
    auto texture = collider_texture_.texture();
    s2_export_buffer_to_texture(world, S2_BUFFER_NAME_FINE_GRID_COLLIDER_NUM,
                                true, 0.8f, &texture);

    // Since taichi and renderer use different command buffers, we must
    // explicitly use flushing (submitting taichi's command list) here, which
    // provides a semaphore between two command buffers.
    runtime.flush();

    ++frame;
    return true;
  }
  virtual void render() override final {
    Renderer &renderer = F.renderer();
    renderer.enqueue_graphics_task(*draw_points);
    renderer.enqueue_graphics_task(*draw_texture);
  }
};

std::unique_ptr<App> create_app() {
  return std::unique_ptr<App>(new Colliders);
}