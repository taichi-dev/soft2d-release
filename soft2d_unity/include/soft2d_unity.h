#pragma once

#include <soft2d/soft2d.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Function `s2x_create_world_async_unity`
S2_API S2World S2_API_CALL s2x_create_world_async_unity(TiArch arch,
                                                        TiRuntime runtime,
                                                        S2WorldConfig config);

// Function `s2x_destory_world_async_unity`
S2_API void S2_API_CALL s2x_destory_world_async_unity(S2World world);

// Function `s2x_step_async_unity`
S2_API void S2_API_CALL s2x_step_async_unity(S2World world, float delta_time);

// Function `s2_remove_particles_in_trigger_async`
S2_API void S2_API_CALL s2_remove_particles_in_trigger_async(S2World world,
                                                             S2Trigger trigger);

// Function `s2x_manipulate_particles_in_tigger_async_unity`
S2_API void S2_API_CALL s2x_manipulate_particles_in_tigger_async_unity(
    S2Trigger trigger, S2ParticleManipulationCallback callback);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
