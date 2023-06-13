// avoid clang-format disorders headers
// clang-format off
#include <taichi/cpp/taichi.hpp>
#include <soft2d/soft2d.h>
#include <iostream>
// clang-format on

using namespace std;

int main() {

  TiArch arch = TiArch::TI_ARCH_VULKAN;
  // Create a taichi runtime
  ti::Runtime runtime(arch);

  // Specify the world configuration
  S2WorldConfig config{};
  config.max_allowed_particle_num = 90000;
  config.max_allowed_body_num = 10000;
  config.max_allowed_element_num = 10000;
  config.max_allowed_trigger_num = 10000;
  config.grid_resolution = 128;
  config.offset = {0.0f, 0.0f};
  config.extent = {0.0f, 0.0f};
  config.substep_dt = 1e-4f;
  config.gravity = {0.0f, -9.8f};
  config.enable_debugging = false;
  config.enable_world_query = false;
  config.mesh_body_force_scale = 1e6f;
  config.collision_penalty_force_scale_along_normal_dir = 0.1f;
  config.collision_penalty_force_scale_along_velocity_dir = 0.1f;
  config.fine_grid_scale = 4;

  // Create a world
  S2World world = s2_create_world(arch, runtime, &config);

  // Simulate the world for 100 steps
  float time_step = 2e-3f;
  for (int i = 0; i < 100; ++i) {
    s2_step(world, time_step);
  }

  std::cout << "Hello, world!" << std::endl;
  return 0;
}
