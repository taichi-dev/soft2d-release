#include <soft2d/soft2d.h>

#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

S2WorldConfig default_world_config{
    .max_allowed_particle_num = 90000,
    .max_allowed_body_num = 10000,
    .max_allowed_element_num = 10000,
    .max_allowed_trigger_num = 10000,
    .grid_resolution = 128,
    .offset = {.x = 0.0f, .y = 0.0f},
    .extent = {.x = 1.0f, .y = 1.0f},
    .substep_dt = 1e-4f,
    .gravity = {.x = 0.0f, .y = -9.8f},
    .out_world_boundary_policy =
        S2OutWorldBoundaryPolicy::S2_OUT_WORLD_BOUNDARY_POLICY_REMOVING,
    .enable_debugging = true,
    .enable_world_query = false,
    .mesh_body_force_scale = 1e-6f,
    .collision_penalty_force_scale_along_normal_dir = 0.1f,
    .collision_penalty_force_scale_along_velocity_dir = 0.1f,
    .fine_grid_scale = 4,
};
