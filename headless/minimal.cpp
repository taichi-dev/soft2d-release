#include <taichi/taichi.h>
#include <taichi/cpp/taichi.hpp>
#include <soft2d/soft2d.h>

#include <iostream>
using namespace std;

int main() {
    TiArch arch;
    if (std::string(BACKEND_NAME) == "vulkan") {
        arch = TiArch::TI_ARCH_VULKAN;
    } else if (std::string(BACKEND_NAME) == "metal") {
        arch = TiArch::TI_ARCH_METAL;
    } else {
        // assert(0);
    }
    arch = TiArch::TI_ARCH_VULKAN;

    // Create a compute runtime and an world
    ti::Runtime runtime(arch);

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
    gravity.y = -9.8f;
    config.gravity = gravity;

    config.enable_debugging = false;
    config.enable_world_query = false;
    config.mesh_body_force_scale = 1e6f;
    config.collision_penalty_force_scale_along_normal_dir = 0.1f;
    config.collision_penalty_force_scale_along_velocity_dir = 0.1f;
    config.fine_grid_scale = 4;

    S2World world = s2_create_world(arch, runtime, config);
    for(int i = 0; i < 100; ++i) {
        s2_step(world, 1.6e-2f);
    }
    std::cout << "Hello, world!" << std::endl;
    return 0;
}