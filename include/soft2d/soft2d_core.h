#pragma once

#ifndef S2_VERSION
#define S2_VERSION 1000002
#endif // S2_VERSION

#include <soft2d/soft2d.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Alias `S2Bool`
typedef uint32_t S2Bool;

// Definition `S2_FALSE`
#define S2_FALSE 0

// Definition `S2_TRUE`
#define S2_TRUE 1

// Alias `S2Flags`
typedef uint32_t S2Flags;

// Definition `S2_NULL_HANDLE`
#define S2_NULL_HANDLE 0

// Handle `S2World`
typedef struct S2World_t *S2World;

// Handle `S2Body`
typedef struct S2Body_t *S2Body;

// Handle `S2Collider`
typedef struct S2Collider_t *S2Collider;

// Handle `S2Trigger`
typedef struct S2Trigger_t *S2Trigger;

// Structure `S2Vec2`
typedef struct S2Vec2 {
  float x;
  float y;
} S2Vec2;

// Structure `S2Vec2I`
typedef struct S2Vec2I {
  int32_t x;
  int32_t y;
} S2Vec2I;

// Enumeration `S2MaterialType`
typedef enum S2MaterialType {
  S2_MATERIAL_TYPE_FLUID = 0,
  S2_MATERIAL_TYPE_ELASTIC = 1,
  S2_MATERIAL_TYPE_SNOW = 2,
  S2_MATERIAL_TYPE_SAND = 3,
  S2_MATERIAL_TYPE_MAX_ENUM = 0xffffffff,
} S2MaterialType;

// Structure `S2Material`
typedef struct S2Material {
  S2MaterialType type;
  float density;
  float youngs_modulus;
  float poissons_ratio;
} S2Material;

// Enumeration `S2Mobility`
typedef enum S2Mobility {
  S2_MOBILITY_STATIC = 0,
  S2_MOBILITY_KINEMATIC = 1,
  S2_MOBILITY_DYNAMIC = 2,
  S2_MOBILITY_MAX_ENUM = 0xffffffff,
} S2Mobility;

// Enumeration `S2BodyMobility`
typedef enum S2BodyMobility {
  S2_BODY_MOBILITY_STATIC = 0,
  S2_BODY_MOBILITY_DYNAMIC = 1,
  S2_BODY_MOBILITY_MAX_ENUM = 0xffffffff,
} S2BodyMobility;

// Enumeration `S2ColliderMobility`
typedef enum S2ColliderMobility {
  S2_COLLIDER_MOBILITY_STATIC = 0,
  S2_COLLIDER_MOBILITY_KINEMATIC = 1,
  S2_COLLIDER_MOBILITY_MAX_ENUM = 0xffffffff,
} S2ColliderMobility;

// Structure `S2Kinematics`
typedef struct S2Kinematics {
  S2Vec2 center;
  S2Vec2 linear_velocity;
  float rotation;
  float angular_velocity;
  S2Mobility mobility;
} S2Kinematics;

// Enumeration `S2ShapeType`
typedef enum S2ShapeType {
  S2_SHAPE_TYPE_BOX = 0,
  S2_SHAPE_TYPE_CIRCLE = 1,
  S2_SHAPE_TYPE_ELLIPSE = 2,
  S2_SHAPE_TYPE_CAPSULE = 3,
  S2_SHAPE_TYPE_POLYGON = 4,
  S2_SHAPE_TYPE_MAX_ENUM = 0xffffffff,
} S2ShapeType;

// Structure `S2BoxShape`
typedef struct S2BoxShape {
  S2Vec2 half_extent;
} S2BoxShape;

// Structure `S2CircleShape`
typedef struct S2CircleShape {
  float radius;
} S2CircleShape;

// Structure `S2EllipseShape`
typedef struct S2EllipseShape {
  float radius_a;
  float radius_b;
} S2EllipseShape;

// Structure `S2CapsuleShape`
typedef struct S2CapsuleShape {
  float half_rect_length;
  float cap_radius;
} S2CapsuleShape;

// Structure `S2PolygonShape`
typedef struct S2PolygonShape {
  int32_t vertex_num;
  void *vertices;
} S2PolygonShape;

// Union `S2ShapeUnion`
typedef union S2ShapeUnion {
  S2BoxShape box;
  S2CircleShape circle;
  S2EllipseShape ellipse;
  S2CapsuleShape capsule;
  S2PolygonShape polygon;
} S2ShapeUnion;

// Structure `S2Shape`
typedef struct S2Shape {
  S2ShapeType type;
  S2ShapeUnion shape_union;
} S2Shape;

// Structure `S2Particle`
typedef struct S2Particle {
  uint32_t id;
  uint32_t tag;
  S2Vec2 position;
  S2Vec2 velocity;
  S2Bool is_removed;
} S2Particle;

// Enumeration `S2OutWorldBoundaryPolicy`
typedef enum S2OutWorldBoundaryPolicy {
  S2_OUT_WORLD_BOUNDARY_POLICY_DEACTIVATION = 0,
  S2_OUT_WORLD_BOUNDARY_POLICY_REMOVING = 1,
  S2_OUT_WORLD_BOUNDARY_POLICY_MAX_ENUM = 0xffffffff,
} S2OutWorldBoundaryPolicy;

// Enumeration `S2CollisionType`
typedef enum S2CollisionType {
  S2_COLLISION_TYPE_STICKY = 0,
  S2_COLLISION_TYPE_SLIP = 1,
  S2_COLLISION_TYPE_SEPARATE = 2,
  S2_COLLISION_TYPE_MAX_ENUM = 0xffffffff,
} S2CollisionType;

// Structure `S2CollisionParameter`
typedef struct S2CollisionParameter {
  S2CollisionType collision_type;
  float friction_coeff;
  float restitution_coeff;
} S2CollisionParameter;

// Structure `S2WorldConfig`
typedef struct S2WorldConfig {
  uint32_t max_allowed_particle_num;
  uint32_t max_allowed_body_num;
  uint32_t max_allowed_trigger_num;
  uint32_t grid_resolution;
  S2Vec2 offset;
  S2Vec2 extent;
  float substep_dt;
  S2Vec2 gravity;
  uint32_t enable_debugging;
  uint32_t enable_world_query;
  float mesh_body_force_scale;
  float collision_penalty_force_scale_along_normal_dir;
  float collision_penalty_force_scale_along_velocity_dir;
  uint32_t fine_grid_scale;
} S2WorldConfig;

// FunctionPointerType `S2ParticleManipulationCallback`
typedef void (*S2ParticleManipulationCallback)(S2Particle *particles,
                                               int32_t size);

// Enumeration `S2BufferName`
typedef enum S2BufferName {
  S2_BUFFER_NAME_PARTICLE_NUM = 0,
  S2_BUFFER_NAME_PARTICLE_POSITION = 1,
  S2_BUFFER_NAME_PARTICLE_VELOCITY = 2,
  S2_BUFFER_NAME_PARTICLE_TAG = 3,
  S2_BUFFER_NAME_PARTICLE_ID = 4,
  S2_BUFFER_NAME_FINE_GRID_COLLIDER_NUM = 5,
  S2_BUFFER_NAME_FINE_GRID_TRIGGER_ID = 6,
  S2_BUFFER_NAME_MAX_ENUM = 0xffffffff,
} S2BufferName;

// Function `s2_create_world`
S2_API S2World S2_API_CALL s2_create_world(TiArch arch, TiRuntime runtime,
                                           S2WorldConfig config);

// Function `s2_destroy_world`
S2_API void S2_API_CALL s2_destroy_world(S2World world);

// Function `s2_create_body`
S2_API S2Body S2_API_CALL s2_create_body(S2World world,
                                         const S2Material *material,
                                         const S2Kinematics *kinematics,
                                         const S2Shape *shape, uint32_t tag);

// Function `s2_create_custom_body`
S2_API S2Body S2_API_CALL s2_create_custom_body(
    S2World world, const S2Material *material, const S2Kinematics *kinematics,
    int32_t particle_num, void *particles_in_local_space, uint32_t tag);

// Function `s2_create_mesh_body`
S2_API S2Body S2_API_CALL s2_create_mesh_body(
    S2World world, const S2Material *material, const S2Kinematics *kinematics,
    int32_t particle_num, void *particles_in_local_space, int32_t index_num,
    void *indices, uint32_t tag);

// Function `s2_destroy_body`
S2_API void S2_API_CALL s2_destroy_body(S2World world, S2Body body);

// Function `s2_create_collider`
S2_API S2Collider S2_API_CALL s2_create_collider(
    S2World world, const S2Kinematics *kinematics, const S2Shape *shape,
    const S2CollisionParameter *collision_parameter);

// Function `s2_destroy_collider`
S2_API void S2_API_CALL s2_destroy_collider(S2World world, S2Collider collider);

// Function `s2_create_trigger`
S2_API S2Trigger S2_API_CALL s2_create_trigger(S2World world,
                                               const S2Kinematics *kinematics,
                                               const S2Shape *shape);

// Function `s2_destroy_trigger`
S2_API void S2_API_CALL s2_destroy_trigger(S2World world, S2Trigger trigger);

// Function `s2_step`
S2_API void S2_API_CALL s2_step(S2World world, float delta_time);

// Function `s2_get_world_config`
S2_API S2WorldConfig S2_API_CALL s2_get_world_config(S2World world);

// Function `s2_get_world_grid_resolution`
S2_API S2Vec2I S2_API_CALL s2_get_world_grid_resolution(S2World world);

// Function `s2_set_substep_timestep`
S2_API void S2_API_CALL s2_set_substep_timestep(S2World world, float dt);

// Function `s2_set_gravity`
S2_API void S2_API_CALL s2_set_gravity(S2World world, S2Vec2 gravity);

// Function `s2_set_world_query_enabled`
S2_API void S2_API_CALL s2_set_world_query_enabled(S2World world,
                                                   uint32_t enable);

// Function `s2_set_world_offset`
S2_API void S2_API_CALL s2_set_world_offset(S2World world, S2Vec2 offset);

// Function `s2_set_world_extent`
S2_API void S2_API_CALL s2_set_world_extent(S2World world, S2Vec2 extent);

// Function `s2_set_mesh_body_force_scale`
S2_API void S2_API_CALL s2_set_mesh_body_force_scale(S2World world,
                                                     float scale);

// Function `s2_apply_impulse_in_circular_area`
S2_API void S2_API_CALL s2_apply_impulse_in_circular_area(S2World world,
                                                          S2Vec2 impulse,
                                                          S2Vec2 center,
                                                          float radius);

// Function `s2_get_buffer`
S2_API void S2_API_CALL s2_get_buffer(S2World world, S2BufferName buffer_name,
                                      TiNdArray *buffer);

// Function `s2_apply_linear_impulse`
S2_API void S2_API_CALL s2_apply_linear_impulse(S2World world, S2Body body,
                                                S2Vec2 impulse);

// Function `s2_apply_angular_impulse`
S2_API void S2_API_CALL s2_apply_angular_impulse(S2World world, S2Body body,
                                                 float impulse);

// Function `s2_set_body_material`
S2_API void S2_API_CALL s2_set_body_material(S2World world, S2Body body,
                                             S2Material material);

// Function `s2_set_collider_position`
S2_API void S2_API_CALL s2_set_collider_position(S2Collider collider,
                                                 S2Vec2 position);

// Function `s2_get_collider_position`
S2_API S2Vec2 S2_API_CALL s2_get_collider_position(S2Collider collider);

// Function `s2_set_collider_rotation`
S2_API void S2_API_CALL s2_set_collider_rotation(S2Collider collider,
                                                 float rotation);

// Function `s2_get_collider_rotation`
S2_API float S2_API_CALL s2_get_collider_rotation(S2Collider collider);

// Function `s2_set_collider_linear_velocity`
S2_API void S2_API_CALL s2_set_collider_linear_velocity(S2Collider collider,
                                                        S2Vec2 linear_velocity);

// Function `s2_get_collider_linear_velocity`
S2_API S2Vec2 S2_API_CALL s2_get_collider_linear_velocity(S2Collider collider);

// Function `s2_set_collider_angular_velocity`
S2_API void S2_API_CALL
s2_set_collider_angular_velocity(S2Collider collider, float angular_velocity);

// Function `s2_get_collider_angular_velocity`
S2_API float S2_API_CALL s2_get_collider_angular_velocity(S2Collider collider);

// Function `s2_set_trigger_position`
S2_API void S2_API_CALL s2_set_trigger_position(S2Trigger trigger,
                                                S2Vec2 position);

// Function `s2_get_trigger_position`
S2_API S2Vec2 S2_API_CALL s2_get_trigger_position(S2Trigger trigger);

// Function `s2_set_trigger_rotation`
S2_API void S2_API_CALL s2_set_trigger_rotation(S2Trigger trigger,
                                                float rotation);

// Function `s2_get_trigger_rotation`
S2_API float S2_API_CALL s2_get_trigger_rotation(S2Trigger trigger);

// Function `s2_query_trigger_overlapped`
S2_API uint32_t S2_API_CALL s2_query_trigger_overlapped(S2World world,
                                                        S2Trigger trigger);

// Function `s2_query_trigger_overlapped_by_tag`
S2_API uint32_t S2_API_CALL s2_query_trigger_overlapped_by_tag(
    S2World world, S2Trigger trigger, uint32_t tag, uint32_t mask);

// Function `s2_query_particle_num_in_trigger`
S2_API uint32_t S2_API_CALL s2_query_particle_num_in_trigger(S2World world,
                                                             S2Trigger trigger);

// Function `s2_query_particle_num_in_trigger_by_tag`
S2_API uint32_t S2_API_CALL s2_query_particle_num_in_trigger_by_tag(
    S2World world, S2Trigger trigger, uint32_t tag, uint32_t mask);

// Function `s2_remove_particles_in_trigger`
S2_API void S2_API_CALL s2_remove_particles_in_trigger(S2World world,
                                                       S2Trigger trigger);

// Function `s2_remove_particles_in_trigger_by_tag`
S2_API void S2_API_CALL s2_remove_particles_in_trigger_by_tag(S2World world,
                                                              S2Trigger trigger,
                                                              uint32_t tag,
                                                              uint32_t mask);

// Function `s2_manipulate_particles_in_tigger`
S2_API void S2_API_CALL s2_manipulate_particles_in_tigger(
    S2World world, S2Trigger trigger, S2ParticleManipulationCallback callback);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
