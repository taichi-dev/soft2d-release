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

struct BasicShapes : public App {

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

    // Create a box-shape body
    S2Shape shape = make_box_shape(vec2(0.03f, 0.02f));

    S2Material material;
    material.type = S2MaterialType::S2_MATERIAL_TYPE_ELASTIC;
    material.density = 1000.0f;
    material.youngs_modulus = 1.0f;
    material.poissons_ratio = 0.2f;

    S2Kinematics kinematics{};

    kinematics.center = vec2(0.3f, 0.5f);
    kinematics.mobility = S2Mobility::S2_MOBILITY_DYNAMIC;

    create_body(world, material, kinematics, shape);

    // Create a circle-shape body
    S2Shape circle_shape = make_circle_shape(0.03f);

    S2Material circle_material{};
    circle_material.type = S2MaterialType::S2_MATERIAL_TYPE_ELASTIC;
    circle_material.density = 1000.0f;
    circle_material.youngs_modulus = 1.0f;
    circle_material.poissons_ratio = 0.2f;

    S2Kinematics circle_kinematics{};
    circle_kinematics.center = vec2(0.4f, 0.5f);
    circle_kinematics.mobility = S2Mobility::S2_MOBILITY_DYNAMIC;
    create_body(world, circle_material, circle_kinematics, circle_shape);

    // Create a ellipse-shape body
    S2Shape ellipse_shape = make_ellipse_shape(0.03f, 0.02f);

    S2Material ellipse_material{};
    ellipse_material.type = S2MaterialType::S2_MATERIAL_TYPE_ELASTIC;
    ellipse_material.density = 1000.0f;
    ellipse_material.youngs_modulus = 1.0f;
    ellipse_material.poissons_ratio = 0.2f;

    S2Kinematics ellipse_kinematics{};
    ellipse_kinematics.center = vec2(0.5f, 0.5f);
    ellipse_kinematics.mobility = S2Mobility::S2_MOBILITY_DYNAMIC;
    create_body(world, ellipse_material, ellipse_kinematics, ellipse_shape);

    // Create a capsule-shape body
    S2Shape capsule_shape = make_capsule_shape(0.03f, 0.02f);

    S2Material capsule_material{};
    capsule_material.type = S2MaterialType::S2_MATERIAL_TYPE_ELASTIC;
    capsule_material.density = 1000.0f;
    capsule_material.youngs_modulus = 1.0f;
    capsule_material.poissons_ratio = 0.2f;

    S2Kinematics capsule_kinematics{};
    capsule_kinematics.center = vec2(0.6f, 0.5f);
    capsule_kinematics.mobility = S2Mobility::S2_MOBILITY_DYNAMIC;
    create_body(world, capsule_material, capsule_kinematics, capsule_shape);

    // Create a polygon-shape body
    auto polygon_vertices = std::vector<S2Vec2>{
        {-0.05, -0.05},   {-0.015, -0.025}, {0.0, -0.05},   {0.0125, 0.0125},
        {0.0125, 0.0375}, {-0.0375, 0.025}, {-0.05625, 0.0}};
    S2Shape polygon_shape =
        make_polygon_shape(polygon_vertices.data(), polygon_vertices.size());

    S2Material polygon_material{};
    polygon_material.type = S2MaterialType::S2_MATERIAL_TYPE_ELASTIC;
    polygon_material.density = 1000.0f;
    polygon_material.youngs_modulus = 1.0f;
    polygon_material.poissons_ratio = 0.2f;

    S2Kinematics polygon_kinematics{};
    polygon_kinematics.center = vec2(0.75f, 0.5f);
    polygon_kinematics.mobility = S2Mobility::S2_MOBILITY_DYNAMIC;
    create_body(world, polygon_material, polygon_kinematics, polygon_shape);

    // Add the ground (no visualization)
    create_collider(world, make_kinematics({0.5f, 0.0f}),
                    make_box_shape(vec2(0.5f, 0.01f)));
    // Soft2D initialization ends

    // Renderer initialization begins
    Renderer &renderer = F.renderer();
    renderer.set_framebuffer_size(win_width, win_height);

    x_ = runtime.allocate_vertex_buffer(config.max_allowed_particle_num, 2);

    draw_points = runtime.draw_points(x_)
                      .point_size(preset_point_size)
                      .color(glm::vec3(1, 0.5, 0))
                      .build();
    // Renderer initialization ends
  }
  virtual bool update() override final {
    GraphicsRuntime &runtime = F.runtime();

    s2_step(world, 0.004);

    // Export particle position data to the external buffer
    TiNdArray particle_x;
    s2_get_buffer(world, S2_BUFFER_NAME_PARTICLE_POSITION, &particle_x);
    ndarray_data_copy(runtime.runtime(), x_.ndarray(), particle_x,
                      sizeof(float) * 2 *
                          s2_get_world_config(world).max_allowed_particle_num);

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
  return std::unique_ptr<App>(new BasicShapes);
}
