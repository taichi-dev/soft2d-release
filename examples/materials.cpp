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

constexpr float win_fov = 1.0 * win_width / win_height;

struct Materials : public App {

  S2World world;
  std::vector<Emitter> emitters;

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
    // Use a smaller substep to avoid the numerical instability (caused by the
    // large young's modulus).
    config.substep_dt = 5e-5;
    world = s2_create_world(TiArch::TI_ARCH_VULKAN, runtime, &config);

    // A matrix to show the different material parameters.
    // From top to bottom: material is fluid, elastic, snow, sand.
    // From left to right: young's modulus is 0.01, 0.1, 1, 10 (MPa).
    float width = 1.0f / s2_get_world_config(world).grid_resolution * 2;
    int n = 4;
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        float y_top = 1.0 - i * 1.0 / n;
        float y_bottom = 1.0 - (i + 1) * 1.0 / n;
        float x_center = (j + 0.5) / n;
        float x_left = (j + 0.0) / n;
        float x_right = (j + 1.0) / n;
        float length = 0.5 * 1.0f / n;

        // top
        create_collider(
            world,
            make_kinematics(vec2(x_center, y_top - width / 2 /*- 0.03*/), 0.0,
                            vec2(0.0, 0.0), 0.0, S2_MOBILITY_STATIC),
            make_box_shape(vec2(length, width)));
        // bottom
        create_collider(world,
                        make_kinematics(vec2(x_center, y_bottom + width / 2),
                                        0.0, vec2(0.0, 0.0), 0.0,
                                        S2_MOBILITY_STATIC),
                        make_box_shape(vec2(length, width)));
        // left
        create_collider(world,
                        make_kinematics(vec2(x_left, 0.5 * (y_top + y_bottom)),
                                        0.0, vec2(0.0, 0.0), 0.0,
                                        S2_MOBILITY_STATIC),
                        make_box_shape(vec2(width, length)));
        // right
        create_collider(world,
                        make_kinematics(vec2(x_right, 0.5 * (y_top + y_bottom)),
                                        0.0, vec2(0.0, 0.0), 0.0,
                                        S2_MOBILITY_STATIC),
                        make_box_shape(vec2(width, length)));

        // Emitters
        auto shape = make_capsule_shape(0.02, 0.03);
        S2Material material = make_material((S2MaterialType)i, 1000.0,
                                            0.01 * std::pow(10, j), 0.2);
        S2Kinematics kinematics =
            make_kinematics(vec2(x_left + 0.3 * (x_right - x_left),
                                 y_bottom + 0.8 * (y_top - y_bottom)),
                            0.0, vec2(0.0, 0.0), 0.0, S2_MOBILITY_DYNAMIC);
        auto emitter = Emitter(world, material, kinematics, shape);
        emitter.SetFrequency(10);
        emitter.SetLifetime(10000);
        emitter.SetEmittingEndFrame(10);
        emitters.push_back(emitter);
      }
    }
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
                      .point_size(preset_point_size)
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

    for (auto &emitter : emitters) {
      emitter.Update(frame);
    }

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
  return std::unique_ptr<App>(new Materials);
}
