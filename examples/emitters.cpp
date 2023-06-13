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

constexpr int win_width = 800;
constexpr int win_height = 800;
constexpr float win_fov = 1.0 * win_width / win_height;

struct BodyEmitters : public App {

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
    S2WorldConfig config{};
    config.max_allowed_particle_num = 90000;
    config.max_allowed_body_num = 10000;
    config.max_allowed_element_num = 10000;
    config.max_allowed_trigger_num = 10000;
    config.grid_resolution = 128;
    config.out_world_boundary_policy =
        S2OutWorldBoundaryPolicy::S2_OUT_WORLD_BOUNDARY_POLICY_REMOVING;

    S2Vec2 offset;
    offset.x = 0.0f;
    offset.y = 0.0f;
    config.offset = offset;

    S2Vec2 extent;
    extent.x = 1.0f;
    extent.y = 1.0f;
    config.extent = extent;
    config.substep_dt = 1e-4f;

    S2Vec2 gravity;
    gravity.x = 0.0f;
    gravity.y = -9.8f;
    config.gravity = gravity;

    config.enable_debugging = true;
    config.enable_world_query = false;
    config.mesh_body_force_scale = 1e6f;
    config.collision_penalty_force_scale_along_normal_dir = 0.1f;
    config.collision_penalty_force_scale_along_velocity_dir = 0.1f;
    config.fine_grid_scale = 4;

    world = s2_create_world(TiArch::TI_ARCH_VULKAN, runtime, &config);

    S2Shape shape = make_box_shape(vec2(0.03f, 0.02f));

    S2Material material;
    material.type = S2MaterialType::S2_MATERIAL_TYPE_ELASTIC;
    material.density = 1000.0f;
    material.youngs_modulus = 1.0f;
    material.poissons_ratio = 0.2f;

    S2Kinematics kinematics{};

    kinematics.center = vec2(0.3f, 0.5f);
    kinematics.linear_velocity = vec2(0.0f, 0.0f);
    kinematics.mobility = S2Mobility::S2_MOBILITY_DYNAMIC;

    // make_body(world, material, kinematics, shape);
    emitter = Emitter(world, material, kinematics, shape);

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
    // Step forward
    s2_step(world, 0.004);

    // Get particle position gpu buffer
    TiNdArray particle_x;
    s2_get_buffer(world, S2_BUFFER_NAME_PARTICLE_POSITION, &particle_x);

    // Export particle position data to the external buffer
    TiMemorySlice src;
    src.memory = particle_x.memory;
    src.offset = 0;
    src.size =
        sizeof(float) * 2 * default_world_config.max_allowed_particle_num;
    TiMemorySlice dst;
    dst.memory = x_.memory();
    dst.offset = 0;
    dst.size =
        sizeof(float) * 2 * default_world_config.max_allowed_particle_num;
    ti_copy_memory_device_to_device(runtime, &dst, &src);

    auto texture = collider_texture_.texture();
    s2_export_buffer_to_texture(world, S2_BUFFER_NAME_FINE_GRID_COLLIDER_NUM,
                                true, 0.8f, &texture);

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
    renderer.enqueue_graphics_task(*draw_texture);
  }
};

std::unique_ptr<App> create_app() {
  return std::unique_ptr<App>(new BodyEmitters);
}
