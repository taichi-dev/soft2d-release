#include "glm/glm.hpp"
#include <taichi/taichi.h>
#include <taichi/cpp/taichi.hpp>
#include <soft2d/soft2d.h>

// #include <iostream>

// #include "soft2d/world.h"
#include "taichi/aot_demo/framework.hpp"
#include <chrono>
// #include <iostream>
#include <thread>

using namespace ti::aot_demo;
using namespace std;
// using namespace soft2d;

constexpr int win_width = 800;
constexpr int win_height = 800;
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

    Renderer &renderer = F.renderer();
    renderer.set_framebuffer_size(win_width, win_height);

    GraphicsRuntime &runtime = F.runtime();

    x_ = runtime.allocate_vertex_buffer(90000, 2);

    draw_points = runtime.draw_points(x_)
                      .point_size(3.0f)
                      .color(glm::vec3(1, 0.5, 0))
                      .build();

    // world = std::make_unique<World>(runtime);


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

    // // box
    // auto box_shape = BoxShape(Vec2(0.03, 0.03));

    // Material material;
    // material.type = MaterialType::ELASTIC;
    // material.rho = 1.0f;
    // material.E = 1e3f;
    // material.nu = 0.2f;

    // Kinematics kinematics;
    // kinematics.center = Vec2(0.2, 0.5);
    // kinematics.rotation = 0.7854f;
    // kinematics.linear_velocity = Vec2(1.0, 10.0);
    // kinematics.angular_velocity = 0.0f;
    // kinematics.mobility = Mobility::DYNAMIC;

    // auto box = world->CreateBody(material, kinematics, box_shape);

    // // circle
    // Material material1;
    // material1.type = MaterialType::ELASTIC;
    // material1.rho = 1.0f;
    // material1.E = 1e3f;
    // material1.nu = 0.2f;

    // Kinematics kinematics1;
    // kinematics1.center = Vec2(0.3, 0.5);
    // kinematics1.rotation = 0.7854f;
    // kinematics1.linear_velocity = Vec2(0.0, 0.0);
    // kinematics1.angular_velocity = 0.0f;
    // kinematics1.mobility = Mobility::DYNAMIC;

    // auto circle_shape = CircleShape(0.03);
    // auto circle = world->CreateBody(material1, kinematics1, circle_shape);

    // // ellipse
    // auto ellipse_shape = EllipseShape(0.04, 0.03);
    // Material material2(MaterialType::ELASTIC, 1.0, 1e3, 0.2);
    // Kinematics kinematics2(Vec2(0.4, 0.5), 0.7854, Vec2(0.0, 0.0), 0.0,
    //                        Mobility::DYNAMIC);
    // auto ellipse = world->CreateBody(material2, kinematics2, ellipse_shape);

    // // capsule
    // auto capsule_shape = CapsuleShape(0.02, 0.02);
    // Material material3(MaterialType::ELASTIC, 1.0, 1e3, 0.2);
    // Kinematics kinematics3(Vec2(0.5, 0.5), 0.7854, Vec2(0.0, 0.0), 0.0,
    //                        Mobility::DYNAMIC);
    // auto capsule = world->CreateBody(material3, kinematics3, capsule_shape);

    // // polygon
    // auto polygon_shape = PolygonShape(std::vector<Vec2>{{-0.05, -0.05},
    //                                                     {-0.015, -0.025},
    //                                                     {0.0, -0.05},
    //                                                     {0.0125, 0.0125},
    //                                                     {0.0125, 0.0375},
    //                                                     {-0.0375, 0.025},
    //                                                     {-0.05625, 0.0}});
    // Material material4(MaterialType::ELASTIC, 1.0, 1e3, 0.2);
    // Kinematics kinematics4(Vec2(0.6, 0.5), 0.7854, Vec2(0.0, 0.0), 100.0,
    //                        Mobility::DYNAMIC);
    // auto polygon = world->CreateBody(material4, kinematics4, polygon_shape);
    //
    std::cout << "initialized!" << std::endl;
  }
  virtual bool update() override final {
    // world->Step();
    // world->ExportParticlePositionsD2D(x_);

    s2_step(world,0.004);
    TiNdArray particle_x;
    s2_get_buffer(world, S2_BUFFER_NAME_PARTICLE_POSITION, &particle_x);

    // Since taichi and renderer use different command buffers, we must
    // explictly use flushing (submitting taichi's command list) here, which
    // provides a semaphore between two command buffers.
    GraphicsRuntime &runtime = F.runtime();


    // runtime_.wait();
    TiMemorySlice src;
    src.memory = particle_x.memory;
    src.offset = 0;
    src.size = sizeof(float) * 2 * 90000;
    TiMemorySlice dst;
    dst.memory = x_.memory();
    dst.offset = 0;
    dst.size = sizeof(float) * 2 * 90000;
    ti_copy_memory_device_to_device(runtime, &dst, &src);


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


// int main() {
//     TiArch arch;
//     if (std::string(BACKEND_NAME) == "vulkan") {
//         arch = TiArch::TI_ARCH_VULKAN;
//     } else if (std::string(BACKEND_NAME) == "metal") {
//         arch = TiArch::TI_ARCH_METAL;
//     } else {
//         // assert(0);
//     }
//     arch = TiArch::TI_ARCH_VULKAN;

//     // Create a compute runtime and an world
//     ti::Runtime runtime(arch);

//     S2WorldConfig config;
//     config.max_allowed_particle_num = 90000;
//     config.max_allowed_body_num = 10000;
//     config.max_allowed_trigger_num = 10000;
//     config.grid_resolution = 128;

//     S2Vec2 offset;
//     offset.x = 0.0f;
//     offset.y = 0.0f;
//     config.offset = offset;

//     S2Vec2 extent;
//     extent.x = 1.0f;
//     extent.y = 1.0f;
//     config.extent = extent;
//     config.substep_dt = 1e-4f;

//     S2Vec2 gravity;
//     gravity.x = 0.0f;
//     gravity.y = -9.8f;
//     config.gravity = gravity;

//     config.enable_debugging = false;
//     config.enable_world_query = false;
//     config.mesh_body_force_scale = 1e6f;
//     config.collision_penalty_force_scale_along_normal_dir = 0.1f;
//     config.collision_penalty_force_scale_along_velocity_dir = 0.1f;
//     config.fine_grid_scale = 4;

//     S2World world = s2_create_world(arch, runtime, config);
//     for(int i = 0; i < 100; ++i) {
//         s2_step(world, 1.6e-2f);
//     }
//     std::cout << "Hello, world!" << std::endl;
//     return 0;
// }