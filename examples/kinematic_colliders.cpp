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

std::vector<S2Vec2> make_gear(int n, float r, float len, float width = 0.5f) {
  std::vector<S2Vec2> gear_points;
  int n_tooth = n;
  float r0 = r;
  float l = len;
  for (int i = 0; i < n_tooth; ++i) {
    float theta = 2 * M_PI / n_tooth * i;
    if (i & 1) {
      float delta = 2 * M_PI / n_tooth * 0.5f * width;
      auto dir = vec2(std::cos(theta), std::sin(theta));
      gear_points.push_back(
          mul(vec2(std::cos(theta - delta), std::sin(theta - delta)), r0));
      gear_points.push_back(
          add(mul(vec2(std::cos(theta - delta), std::sin(theta - delta)), r0),
              mul(dir, l)));
      gear_points.push_back(
          add(mul(vec2(std::cos(theta + delta), std::sin(theta + delta)), r0),
              mul(dir, l)));
      gear_points.push_back(
          mul(vec2(std::cos(theta + delta), std::sin(theta + delta)), r0));
    } else {
    }
  }
  return gear_points;
}

constexpr int win_width = 800;
constexpr int win_height = 800;
constexpr float win_fov = 1.0 * win_width / win_height;

struct KinematicColliders : public App {

  S2World world;
  Emitter emitter;
  S2Collider box_collider;

  std::unique_ptr<GraphicsTask> draw_points;
  std::unique_ptr<GraphicsTask> draw_collider_texture;

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
    GraphicsRuntime &runtime = F.runtime();

    // Soft2D initialization begins
    S2WorldConfig config = default_world_config;
    world = s2_create_world(TiArch::TI_ARCH_VULKAN, runtime, &config);

    emitter = Emitter(world,
                      make_material(S2_MATERIAL_TYPE_FLUID, 1000.0f, 1.0f, 0.2),
                      make_kinematics({0.2f, 0.9f}, 0.0f, {4.0f, 0.0f}, 0.0f,
                                      S2_MOBILITY_DYNAMIC),
                      make_box_shape(vec2(0.02f, 0.02f)));
    emitter.SetFrequency(5);

    box_collider =
        create_collider(world,
                        make_kinematics({0.5f, 0.6f}, 0.3, {2.0f, 0.0f}, -10.0f,
                                        S2_MOBILITY_KINEMATIC),
                        make_box_shape(vec2(0.05f, 0.05f)));

    create_collider(world,
                    make_kinematics({0.8f, 0.8f}, 0.7854, {0.0f, 0.0f}, 10.0f,
                                    S2_MOBILITY_DYNAMIC),
                    make_box_shape(vec2(0.1f, 0.01f)));

    auto polygon_vertices = make_gear(8, 0.015, 0.1, 1.0);
    create_collider(
        world,
        make_kinematics({0.2f, 0.2f}, 0.0, {}, -30.0f, S2_MOBILITY_KINEMATIC),
        make_polygon_shape(polygon_vertices.data(), polygon_vertices.size()));

    polygon_vertices = make_gear(8, 0.02, 0.04);
    create_collider(
        world,
        make_kinematics({0.9f, 0.4f}, 0.0, {}, 50.0f, S2_MOBILITY_KINEMATIC),
        make_polygon_shape(polygon_vertices.data(), polygon_vertices.size()));

    polygon_vertices = make_gear(10, 0.04, 0.02);
    create_collider(
        world,
        make_kinematics({0.75f, 0.3f}, 0.0, {}, 50.0f, S2_MOBILITY_KINEMATIC),
        make_polygon_shape(polygon_vertices.data(), polygon_vertices.size()));

    polygon_vertices = make_gear(16, 0.05, 0.01);
    create_collider(
        world,
        make_kinematics({0.55f, 0.2f}, 0.0, {}, 50.0f, S2_MOBILITY_KINEMATIC),
        make_polygon_shape(polygon_vertices.data(), polygon_vertices.size()));

    // Add the boundary
    // bottom
    create_collider(world, make_kinematics({0.5f, 0.0f}),
                    make_box_shape(vec2(0.5f, 0.01f)));
    // top
    create_collider(world, make_kinematics({0.5f, 1.0f}),
                    make_box_shape(vec2(0.5f, 0.01f)));
    // left
    create_collider(world, make_kinematics({0.0f, 0.5f}),
                    make_box_shape(vec2(0.01f, 0.5f)));
    // right
    create_collider(world, make_kinematics({1.0f, 0.5f}),
                    make_box_shape(vec2(0.01f, 0.5f)));

    // Soft2D initialization ends

    // Renderer initialization begins
    Renderer &renderer = F.renderer();
    renderer.set_framebuffer_size(win_width, win_height);

    x_ = runtime.allocate_vertex_buffer(config.max_allowed_particle_num, 2);

    collider_texture_ = runtime.allocate_texture2d(
        config.grid_resolution * config.fine_grid_scale,
        config.grid_resolution * config.fine_grid_scale, TI_FORMAT_R32F,
        TI_NULL_HANDLE);

    draw_points = runtime.draw_points(x_)
                      .point_size(3.0f)
                      .color(glm::vec3(1, 0.5, 0))
                      .build();
    draw_collider_texture = runtime.draw_texture(collider_texture_)
                                .is_single_channel()
                                .color(glm::vec3(0.2, 0.8, 0.0))
                                .build();
    // Renderer initialization ends
  }
  int frame = 0;
  virtual bool update() override final {
    GraphicsRuntime &runtime = F.runtime();

    auto v = vec2(2.0f, 0.0f);
    if (s2_get_collider_position(box_collider).x < 0.0f) {
      v.x = 2.0f;
      s2_set_collider_linear_velocity(box_collider, &v);
    }
    if (s2_get_collider_position(box_collider).x > 1.0f) {
      v.x = -2.0f;
      s2_set_collider_linear_velocity(box_collider, &v);
    }

    emitter.Update(frame);

    s2_step(world, 0.004);

    // Export particle position data to the external buffer
    TiNdArray particle_x;
    s2_get_buffer(world, S2_BUFFER_NAME_PARTICLE_POSITION, &particle_x);
    ndarray_data_copy(runtime.runtime(), x_.ndarray(), particle_x,
                      sizeof(float) * 2 *
                          s2_get_world_config(world).max_allowed_particle_num);

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
    renderer.enqueue_graphics_task(*draw_collider_texture);
  }
};

std::unique_ptr<App> create_app() {
  return std::unique_ptr<App>(new KinematicColliders);
}
