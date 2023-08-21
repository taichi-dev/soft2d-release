// avoid clang-format disorders headers
// clang-format off
#include <taichi/taichi.h>
#include <soft2d/soft2d.h>
#include "common.h"
#include "globals.h"
#include "taichi/aot_demo/framework.hpp"
#include "emitter.h"
// clang-format on

using namespace ti::aot_demo;
using namespace std;



constexpr float win_fov = 1.0 * win_width / win_height;

struct ParticleTag : public App {

  S2World world;
  S2Trigger trigger;
  Emitter emitters[2];

  std::unique_ptr<GraphicsTask> draw_points;
  std::unique_ptr<GraphicsTask> draw_collider_texture;
  std::unique_ptr<GraphicsTask> draw_trigger_texture;

  ti::NdArray<float> x_;
  ti::Texture collider_texture_;
  ti::Texture trigger_texture_;

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
    config.enable_world_query = true;
    world = s2_create_world(TiArch::TI_ARCH_VULKAN, runtime, &config);

    S2Shape shape = make_circle_shape(0.03f);

    S2Material material;
    material.type = S2MaterialType::S2_MATERIAL_TYPE_ELASTIC;
    material.density = 1000.0f;
    material.youngs_modulus = 1.0f;
    material.poissons_ratio = 0.2f;

    S2Kinematics kinematics{};
    kinematics.mobility = S2Mobility::S2_MOBILITY_DYNAMIC;

    // Assign different tags for each emitter
    kinematics.center = vec2(0.3f, 0.8f);
    emitters[0] = Emitter(world, material, kinematics, shape, /*tag = */ 0);
    kinematics.center = vec2(0.7f, 0.8f);
    emitters[1] = Emitter(world, material, kinematics, shape, /*tag = */ 1);

    trigger = create_trigger(world, make_kinematics({0.5f, 0.07f}),
                             make_box_shape(vec2(0.5f, 0.07f)));

    create_collider(world, make_kinematics({0.5f, 0.1f}),
                    make_box_shape(vec2(0.01f, 0.05f)));

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
    trigger_texture_ = runtime.allocate_texture2d(
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
    draw_trigger_texture = runtime.draw_texture(trigger_texture_)
                               .is_single_channel()
                               .color(glm::vec3(0.85, 0.1, 0.0))
                               .build();
    // Renderer initialization ends
  }
  int frame = 0;
  virtual bool update() override final {
    GraphicsRuntime &runtime = F.runtime();

    for (int i = 0; i < 2; ++i) {
      emitters[i].Update(frame);
    }

    s2_step(world, 0.004);

    // Remove particles with specified tags:
    // A particle will be removed at the next step if it satisfies the
    // condition: (particle.tag & mask) == tag.
    uint32_t tag = 0;
    uint32_t mask = 0xffffffff;
    s2_remove_particles_in_trigger_by_tag(trigger, tag, mask);

    // Export particle position data to the external buffer
    TiNdArray particle_x;
    s2_get_buffer(world, S2_BUFFER_NAME_PARTICLE_POSITION, &particle_x);
    ndarray_data_copy(runtime.runtime(), x_.ndarray(), particle_x,
                      sizeof(float) * 2 *
                          s2_get_world_config(world).max_allowed_particle_num);

    // Export collider and trigger buffers to texture
    auto collider_tex = collider_texture_.texture();
    auto trigger_tex = trigger_texture_.texture();
    s2_export_buffer_to_texture(world, S2_BUFFER_NAME_FINE_GRID_COLLIDER_NUM,
                                true, 0.8f, &collider_tex);
    s2_export_buffer_to_texture(world, S2_BUFFER_NAME_FINE_GRID_TRIGGER_ID,
                                true, 0.8f, &trigger_tex);

    // Since taichi and renderer use different command buffers, we must
    // explicitly use flushing (submitting taichi's command list) here, which
    // provides a semaphore between two command buffers.
    runtime.flush();

    frame += 1;
    return true;
  }
  virtual void render() override final {
    Renderer &renderer = F.renderer();
    renderer.enqueue_graphics_task(*draw_points);
    renderer.enqueue_graphics_task(*draw_collider_texture);
    renderer.enqueue_graphics_task(*draw_trigger_texture);
  }
};

std::unique_ptr<App> create_app() {
  return std::unique_ptr<App>(new ParticleTag);
}
