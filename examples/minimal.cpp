#include <taichi/taichi.h>
#include <soft2d/soft2d.h>
#include "taichi/aot_demo/framework.hpp"

using namespace ti::aot_demo;
using namespace std;
// using namespace soft2d;

constexpr int win_width = 800;
constexpr int win_height = 800;
constexpr float win_fov = 1.0 * win_width / win_height;

struct Minimal : public App {

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

    // Renderer initialization
    Renderer &renderer = F.renderer();
    renderer.set_framebuffer_size(win_width, win_height);

    GraphicsRuntime &runtime = F.runtime();

    x_ = runtime.allocate_vertex_buffer(90000, 2);

    draw_points = runtime.draw_points(x_)
                      .point_size(3.0f)
                      .color(glm::vec3(1, 0.5, 0))
                      .build();

    // Soft2D initialization
    // Create a world
    S2WorldConfig config;
    config.max_allowed_particle_num = 90000;
    config.max_allowed_body_num = 10000;
    config.max_allowed_trigger_num = 10000;
    config.grid_resolution = 128;

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
    gravity.y = 0.0f;
    config.gravity = gravity;

    config.enable_debugging = false;
    config.enable_world_query = false;
    config.mesh_body_force_scale = 1e6f;
    config.collision_penalty_force_scale_along_normal_dir = 0.1f;
    config.collision_penalty_force_scale_along_velocity_dir = 0.1f;
    config.fine_grid_scale = 4;

    world = s2_create_world(TiArch::TI_ARCH_VULKAN, runtime, config);

    // Create a box body
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

    S2Material material;
    material.type = S2MaterialType::S2_MATERIAL_TYPE_ELASTIC;
    material.density = 1.0f;
    material.youngs_modulus = 1e3f;
    material.poissons_ratio = 0.2f;

    S2Kinematics kinematics;
    S2Vec2 center;
    center.x = 0.5f;
    center.y = 0.5f;
    kinematics.center = center;
    kinematics.mobility = S2Mobility::S2_MOBILITY_DYNAMIC;

    s2_create_body(world, &material, &kinematics, &shape, 0);
  }
  virtual bool update() override final {
    GraphicsRuntime &runtime = F.runtime();

    // Step forward
    s2_step(world,0.004);

    // Get particle position gpu buffer
    TiNdArray particle_x;
    s2_get_buffer(world, S2_BUFFER_NAME_PARTICLE_POSITION, &particle_x);

    // Export particle position data to the external buffer
    TiMemorySlice src;
    src.memory = particle_x.memory;
    src.offset = 0;
    src.size = sizeof(float) * 2 * 90000;
    TiMemorySlice dst;
    dst.memory = x_.memory();
    dst.offset = 0;
    dst.size = sizeof(float) * 2 * 90000;
    ti_copy_memory_device_to_device(runtime, &dst, &src);

    // Since taichi and renderer use different command buffers, we must
    // explictly use flushing (submitting taichi's command list) here, which
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
  return std::unique_ptr<App>(new Minimal);
}
