/// @file
/// Soft2D utilizes Taichi C-API, some soft2d interfaces require taichi c-api
/// types such as `TiArch`, `TiRuntime`, `TiNdarray`. Thus, the Taichi C-API
/// header should be included before the soft2d header in practical coding. The
/// documentation of Taichi C-API could be found at [Taichi C-API
/// Documentation](https://docs.taichi-lang.org/docs/taichi_core).
///
/// Unless specified otherwise, physical quantity units in soft2d follow the
/// international standard. For instance, length-related parameters should be
/// given in 'meter', velocity-related parameters in 'meters per second'.
///
#pragma once

#ifndef S2_VERSION
#define S2_VERSION 2000
#endif // S2_VERSION

#include <soft2d/soft2d.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/// Alias `S2Bool`
///
/// A boolean value. Can be either [`S2_TRUE`](#definition-s2_true) or
/// [`S2_FALSE`](#definition-s2_false). Assignment with other values could lead
/// to undefined behavior.
typedef uint32_t S2Bool;

/// Definition `S2_FALSE`
///
/// A condition or a predicate is not satisfied; a statement is invalid.
#define S2_FALSE 0

/// Definition `S2_TRUE`
///
/// A condition or a predicate is satisfied; a statement is valid.
#define S2_TRUE 1

/// Alias `S2Flags`
///
/// A bit field that can be used to represent 32 orthogonal flags. Bits
/// unspecified in the corresponding flag enum are ignored.
///
/// Enumerations and bit-field flags in the C-API have a S2_XXX_MAX_ENUM case to
/// ensure the enum has a 32-bit range and in-memory size. It has no semantical
/// impact and can be safely ignored.
typedef uint32_t S2Flags;

/// Definition `S2_NULL_HANDLE`
///
/// A sentinel invalid handle that will never be produced from a valid call to
/// Soft2D C-API.
#define S2_NULL_HANDLE 0

/// Handle `S2World`
///
/// A world is a container that contains all simulation-related objects within a
/// scene and simulates them over time under physical laws.
typedef struct S2World_t *S2World;

/// Handle `S2Body`
///
/// A body is a continuum to be simulated, which is composed of a group of
/// particles. A body has properties such as `center`, `mobility`, `material`,
/// etc.
typedef struct S2Body_t *S2Body;

/// Handle `S2Collider`
///
/// A collider is an obstacle within the world that blocks the motion of bodies.
typedef struct S2Collider_t *S2Collider;

/// Handle `S2Trigger`
///
/// A trigger is a spatial area with a specific shape, which is able to detect
/// particles passing through it.
typedef struct S2Trigger_t *S2Trigger;

/// Structure `S2Vec2`
///
/// A 2D floating-point vector type.
typedef struct S2Vec2 {
  float x;
  float y;
} S2Vec2;

/// Structure `S2Vec2I`
///
/// A 2D integer vector type.
typedef struct S2Vec2I {
  int32_t x;
  int32_t y;
} S2Vec2I;

/// Enumeration `S2MaterialType`
///
/// Types of physical materials.
typedef enum S2MaterialType {
  /// Fluid type.
  S2_MATERIAL_TYPE_FLUID = 0,
  /// Elastic body type.
  S2_MATERIAL_TYPE_ELASTIC = 1,
  /// Snow type.
  S2_MATERIAL_TYPE_SNOW = 2,
  /// Sand type.
  S2_MATERIAL_TYPE_SAND = 3,
  S2_MATERIAL_TYPE_MAX_ENUM = 0xffffffff,
} S2MaterialType;

/// Structure `S2Material`
///
/// Physical Materials. Describing a body's physical properties. This must be
/// provided when creating a body.
typedef struct S2Material {
  /// Material's type.
  S2MaterialType type;
  /// Density (kg/m^3).
  float density;
  /// Young's modulus (MPa).
  float youngs_modulus;
  /// Poisson's ratio.
  float poissons_ratio;
} S2Material;

/// Enumeration `S2Mobility`
///
/// The mobility of an object (#S2Body, #S2Collider, or #S2Trigger).
/// - Bodies currently support `S2_MOBILITY_STATIC` and `S2_MOBILITY_DYNAMIC`
/// mobilities.
/// - Colliders currently support  `S2_MOBILITY_STATIC` and
/// `S2_MOBILITY_KINEMATIC` mobilities.
/// - Triggers only support `S2_MOBILITY_STATIC` mobility.
typedef enum S2Mobility {
  /// A static object's velocity is zero and its position can not be changed
  /// throughout the simulation.
  S2_MOBILITY_STATIC = 0,
  /// A kinematic object is able to move, but it would not 'collide' with other
  /// objects.
  S2_MOBILITY_KINEMATIC = 1,
  /// A dynamic object is able to move, and it is able to 'collide' with other
  /// objects.
  S2_MOBILITY_DYNAMIC = 2,
  S2_MOBILITY_MAX_ENUM = 0xffffffff,
} S2Mobility;

/// Structure `S2Kinematics`
///
/// The kinematic properties of an object (#S2Body, #S2Collider, or #S2Trigger).
///
/// Since a body often contains multiple particles, every particle has its own
/// kinematic state during the simulation. Thus, when creating a body, the
/// specified `S2Kinematics` parameter is only meaningful for body
/// initialization as the initial kinematic state of the body. A body's
/// kinematics value becomes meaningless once the simulation starts.
typedef struct S2Kinematics {
  /// The center of the object.
  S2Vec2 center;
  /// The rotation of the object (radians).
  float rotation;
  /// The linear velocity of the object (meters per second).
  S2Vec2 linear_velocity;
  /// The angular velocity of the object (radians per second).
  float angular_velocity;
  /// The mobility of the object.
  S2Mobility mobility;
} S2Kinematics;

/// Enumeration `S2ShapeType`
///
/// Types of predefined shapes.
typedef enum S2ShapeType {
  /// Box type.
  S2_SHAPE_TYPE_BOX = 0,
  /// Circle type.
  S2_SHAPE_TYPE_CIRCLE = 1,
  /// Ellipse type.
  S2_SHAPE_TYPE_ELLIPSE = 2,
  /// Capsule type.
  S2_SHAPE_TYPE_CAPSULE = 3,
  /// Polygon type.
  S2_SHAPE_TYPE_POLYGON = 4,
  S2_SHAPE_TYPE_MAX_ENUM = 0xffffffff,
} S2ShapeType;

/// Structure `S2BoxShape`
///
/// A box shape.
typedef struct S2BoxShape {
  /// Half of the width and height of the box.
  S2Vec2 half_extent;
} S2BoxShape;

/// Structure `S2CircleShape`
///
/// A circle shape.
typedef struct S2CircleShape {
  /// The radius of the circle.
  float radius;
} S2CircleShape;

/// Structure `S2EllipseShape`
///
/// An ellipse shape.
typedef struct S2EllipseShape {
  /// The radius along the x-axis of the ellipse.
  float radius_x;
  /// The radius along the y-axis of the ellipse.
  float radius_y;
} S2EllipseShape;

/// Structure `S2CapsuleShape`
///
/// A capsule shape. The default pose is lying along the x-axis.
typedef struct S2CapsuleShape {
  /// The half length of the central rectangle's width.
  float rect_half_length;
  /// The radius of caps on the two sides.
  float cap_radius;
} S2CapsuleShape;

/// Structure `S2PolygonShape`
///
/// A polygon shape. A polygon shape requires users to provide a pointer of
/// `S2Vec2` C array which stores all vertices of a polygon. These vertices
/// should be in a counter-clockwise order.
typedef struct S2PolygonShape {
  /// The number of vertices in the vertex buffer.
  uint32_t vertex_num;
  /// A user-specified vertex buffer.
  void *vertices;
} S2PolygonShape;

/// Union `S2ShapeUnion`
///
/// A union of shape structures, designed for a uniform representation of
/// various shape structures.
typedef union S2ShapeUnion {
  /// See `S2BoxShape`.
  S2BoxShape box;
  /// See `S2CircleShape`.
  S2CircleShape circle;
  /// See `S2EllipseShape`.
  S2EllipseShape ellipse;
  /// See `S2CapsuleShape`.
  S2CapsuleShape capsule;
  /// See `S2PolygonShape`.
  S2PolygonShape polygon;
} S2ShapeUnion;

/// Structure `S2Shape`
///
/// A uniform representation of various shape structures. The value of `type`
/// should be consistent with the underlying shape structure of `shape_union`.
typedef struct S2Shape {
  /// Shape's type.
  S2ShapeType type;
  /// Shape's underlying data.
  S2ShapeUnion shape_union;
} S2Shape;

/// Structure `S2Particle`
///
/// Represents a particle in soft2d. Users could access particle attributes via
/// this structure. This structure is primarily used in
/// s2_manipulate_particles_in_trigger().
typedef struct S2Particle {
  /// The persistent and unique ID of a particle during the simulation. This
  /// value is read-only.
  uint32_t id;
  /// Particle's position. This value is read-only.
  S2Vec2 position;
  /// Particle's velocity. This value is read-only.
  S2Vec2 velocity;
  /// Particle's custom tag, mainly for user use. This value can be modified by
  /// users.
  uint32_t tag;
  /// This is a helper allowing users to conveniently remove a particle. `True`
  /// tells the engine to remove this particle at the next step, `False` means
  /// not remove. The default value of this property is `False`. This value can
  /// be modified by users.
  S2Bool is_removed;
} S2Particle;

/// Enumeration `S2OutWorldBoundaryPolicy`
///
/// This policy specifies the behavior of a body when it leaves the world.
typedef enum S2OutWorldBoundaryPolicy {
  /// The body will still exist in the world, but it will be deactivated. This
  /// body won't move or interact with other objects anymore.
  S2_OUT_WORLD_BOUNDARY_POLICY_DEACTIVATION = 0,
  /// All particles in this body will be removed from the world (leading to an
  /// empty body).
  S2_OUT_WORLD_BOUNDARY_POLICY_REMOVING = 1,
  S2_OUT_WORLD_BOUNDARY_POLICY_MAX_ENUM = 0xffffffff,
} S2OutWorldBoundaryPolicy;

/// Enumeration `S2CollisionType`
///
/// Colliders' collision types. Specifies a particle's behavior when colliding
/// with a collider.
typedef enum S2CollisionType {
  /// All velocity components of the particle will be set to 0.
  S2_COLLISION_TYPE_STICKY = 0,
  /// The particle's velocity component along the contact normal direction will
  /// be set to 0, which leads to the particle 'sliding' along the collider
  /// surface. With this type, users can modify
  /// `S2CollisionParameter.friction_coeff` to adjust the friction to control
  /// the particle's behavior.
  S2_COLLISION_TYPE_SLIP = 1,
  /// If the particle's velocity component along the contact normal direction is
  /// negative, then this part of the component will be set to 0. Otherwise, it
  /// keeps its original velocity. With this type, users can modify
  /// `S2CollisionParameter.friction_coeff` and
  /// `S2CollisionParameter.restitution_coeff` to adjust the friction and
  /// restitution to control the particle's behavior.
  S2_COLLISION_TYPE_SEPARATE = 2,
  S2_COLLISION_TYPE_MAX_ENUM = 0xffffffff,
} S2CollisionType;

/// Structure `S2CollisionParameter`
///
/// Colliders' collision parameters. Indicates a particle's behavior when
/// colliding with the collider.
typedef struct S2CollisionParameter {
  /// Collision's type.
  S2CollisionType collision_type;
  /// The coefficient of friction. A common range is [0, 1]. Higher values of
  /// `friction_coeff` result in increased loss of velocity when particles
  /// 'slide' across the collider's surface.
  float friction_coeff;
  /// The coefficient of restitution. A common range is [0, 1]. Higher values of
  /// `restitution_coeff` give particles a greater 'bounce' speed when they
  /// collide with the collider.
  float restitution_coeff;
} S2CollisionParameter;

/// Structure `S2WorldConfig`
typedef struct S2WorldConfig {
  /// World's capacity of particles.
  uint32_t max_allowed_particle_num;
  /// World's capacity of bodies.
  uint32_t max_allowed_body_num;
  /// World's capacity of triangle elements.
  uint32_t max_allowed_element_num;
  /// World's capacity of triggers.
  uint32_t max_allowed_trigger_num;
  /// The resolution of the world's background grid.
  uint32_t grid_resolution;
  /// World's offset. Indicates the location of the world's bottom-left corner.
  S2Vec2 offset;
  /// World's extent. The world's bounds form a rectangle, with the bottom-left
  /// corner at `world.offset` and top-right corner at `world.offset +
  /// world.extent`.
  S2Vec2 extent;
  /// The time step of the internal sub-step. A call of `s2_step()` function is
  /// divided into several sub-step iterations in the internal implementation.
  /// The number of iterations could be calculated as `ceil(dt/substep_dt)`.
  float substep_dt;
  /// World's gravity acceleration. A commonly used value is `(0, -9.8)` (meters
  /// per second squared).
  S2Vec2 gravity;
  /// The behavior when a body leaves out of the world. See
  /// #S2OutWorldBoundaryPolicy.
  S2OutWorldBoundaryPolicy out_world_boundary_policy;
  /// Enable debugging or not. Enabling this option allows users to export
  /// additional buffers (See #S2BufferName) for debugging and visualization,
  /// consuming more GPU memory.
  uint32_t enable_debugging;
  /// Enable world querying or not. Triggers won't work unless this option is
  /// enabled. Enabling this option leads to data read-back from GPU to CPU,
  /// potentially causing obvious performance degradation.
  uint32_t enable_world_query;
  /// A scale factor of mesh body's internal force.
  float mesh_body_force_scale;
  /// A parameter to prevent particles from penetrating colliders.
  float collision_penalty_force_scale_along_normal_dir;
  /// A parameter to prevent particles from penetrating colliders.
  float collision_penalty_force_scale_along_velocity_dir;
  /// A scale factor of fine grid resolution compared to background grid
  /// resolution. This parameter adjusts the precision of colliders when the
  /// world uses a relatively coarse grid.
  uint32_t fine_grid_scale;
} S2WorldConfig;

/// FunctionPointerType `S2ParticleManipulationCallback`
///
/// A function pointer type. The function pointer type is primarily used in
/// `s2_manipulate_particles_in_trigger()`.
///  \param particles
/// A particle buffer provided by soft2d.
///  \param size
/// The number of particles within the particle buffer.
typedef void (*S2ParticleManipulationCallback)(
    /// A particle buffer provided by soft2d.
    S2Particle *particles,
    /// The number of particles within the particle buffer.
    uint32_t size);

/// Enumeration `S2BufferName`
///
/// The names of soft2d's internal buffer, mainly used in `s2_get_buffer()`.
/// Soft2D exposes a series of internal GPU buffers, the subsequent table shows
/// the detailed descriptions of each buffer.
///
/// |Buffer Name  |Element Data Type |Dimension|Shape|Flatten size (in bytes) |
/// |-------------|----------|---------|------------|-------------------------|
/// |particle_num           |int    |1  |1           |1*sizeof(int)           |
/// |particle_position      |float2 |1  |P           |P*2*sizeof(float)       |
/// |particle_velocity      |float2 |1  |P           |P*2*sizeof(float)       |
/// |particle_tag           |int    |1  |P           |P*sizeof(int)           |
/// |particle_id            |int    |1  |P           |P*sizeof(int)           |
/// |fine_grid_collider_num |int    |2  |(FG.x, FG.y)|FG.x*FG.y*sizeof(int)   |
/// |fine_grid_trigger_id   |int    |2  |(FG.x, FG.y)|FG.x*FG.y*sizeof(int)   |
/// |element_indices        |int    |1  |E           |E*3*sizeof(int)         |
///
/// * P: Number of particles.
/// * E: Number of triangle elements.
/// * (FG.x, FG.y): Resolution of the fine grid.
typedef enum S2BufferName {
  /// This buffer only stores a single float value, indicating the current
  /// number of particles in the world.
  S2_BUFFER_NAME_PARTICLE_NUM = 0,
  /// Stores positions of all particles.
  S2_BUFFER_NAME_PARTICLE_POSITION = 1,
  /// Stores velocities of all particles.
  S2_BUFFER_NAME_PARTICLE_VELOCITY = 2,
  /// Stores tags of all particles.
  S2_BUFFER_NAME_PARTICLE_TAG = 3,
  /// Stores IDs of all particles.
  S2_BUFFER_NAME_PARTICLE_ID = 4,
  /// Stores the number of colliders at every cell of the fine grid (after
  /// discretization). This buffer can be exported only if
  /// `S2WorldConfig.enable_debugging` is true.
  S2_BUFFER_NAME_FINE_GRID_COLLIDER_NUM = 5,
  /// Stores the trigger's ID (0 if none) at every cell of the fine grid (after
  /// discretization). If multiple triggers overlap on a single cell, only the
  /// ID of the most recently added trigger is stored.  This buffer can be
  /// exported only if `S2WorldConfig.enable_debugging` is true.
  S2_BUFFER_NAME_FINE_GRID_TRIGGER_ID = 6,
  /// Stores all indices of particles of all triangle elements. Note this index
  /// means the particle's index in the particle buffer, instead of their ID.
  S2_BUFFER_NAME_ELEMENT_INDICES = 7,
  S2_BUFFER_NAME_MAX_ENUM = 0xffffffff,
} S2BufferName;

/// Function `s2_create_world`
///
/// Create a world with specified parameters.
S2_API S2World S2_API_CALL s2_create_world(
    /// Architecture of the taichi runtime.
    TiArch arch,
    /// An instance of the taichi runtime.
    TiRuntime runtime,
    /// World's configuration.
    const S2WorldConfig *config);

/// Function `s2_destroy_world`
///
/// Destroy a world.
S2_API void S2_API_CALL s2_destroy_world(
    /// The world to be destroyed.
    S2World world);

/// Function `s2_create_body`
///
/// Create a body with a predefined shape. Soft2D automatically generates the
/// particles within the body.
S2_API S2Body S2_API_CALL s2_create_body(
    /// World handle.
    S2World world,
    /// Body's physical material.
    const S2Material *material,
    /// Body's kinematic properties.
    const S2Kinematics *kinematics,
    /// Body' shape.
    const S2Shape *shape,
    /// User-specified custom tag.
    uint32_t tag);

/// Function `s2_create_custom_body`
///
/// Create a body with user-specified particles. These particles form a complete
/// body and their locations should be provided in the body's local space.
S2_API S2Body S2_API_CALL s2_create_custom_body(
    /// World handle.
    S2World world,
    /// Body's physical material.
    const S2Material *material,
    /// Body's kinematic properties.
    const S2Kinematics *kinematics,
    /// The number of particles in the `particles_in_local_space` buffer.
    uint32_t particle_num,
    /// User-specified particles data. This should be provided as a pointer of C
    /// #S2Vec2 array. This array stores the local positions of all particles.
    void *particles_in_local_space,
    /// User-specified custom tag.
    uint32_t tag);

/// Function `s2_create_mesh_body`
///
/// Create a body with a 2D triangle mesh. The indices' winding order should be
/// counter-clockwise.
S2_API S2Body S2_API_CALL s2_create_mesh_body(
    /// World handle.
    S2World world,
    /// Body's physical material.
    const S2Material *material,
    /// Body's kinematic properties.
    const S2Kinematics *kinematics,
    /// The number of vertices in the `particles_in_local_space` buffer.
    uint32_t particle_num,
    /// The mesh's vertices data. This should be provided as a pointer of C
    /// #S2Vec2 array. This array stores the local positions of all particles.
    void *particles_in_local_space,
    /// The number of indices in the `indices` buffer.
    uint32_t index_num,
    /// The mesh's indices data. This should be provided as a pointer of C `int`
    /// array. This array stores the indices of all triangle elements. Indices
    /// for a triangle element should be in a counter-clockwise winding order.
    void *indices,
    /// User-specified custom tag.
    uint32_t tag);

/// Function `s2_destroy_body`
///
/// Remove a body from the world and destroy it.
S2_API void S2_API_CALL s2_destroy_body(
    /// The body to be destroyed.
    S2Body body);

/// Function `s2_create_collider`
///
/// Create a collider with specified parameters.
S2_API S2Collider S2_API_CALL s2_create_collider(
    /// World handle.
    S2World world,
    /// Collider's kinematic properties.
    const S2Kinematics *kinematics,
    /// Collider's shape.
    const S2Shape *shape,
    /// Collider's collision parameters.
    const S2CollisionParameter *collision_parameter);

/// Function `s2_destroy_collider`
///
/// Remove a collider from the world and destroy it.
S2_API void S2_API_CALL s2_destroy_collider(
    /// The collider to be destroyed.
    S2Collider collider);

/// Function `s2_create_trigger`
///
/// Create a trigger with specified parameters.
S2_API S2Trigger S2_API_CALL s2_create_trigger(
    /// World handle.
    S2World world,
    /// Trigger's kinematic properties.
    const S2Kinematics *kinematics,
    /// Trigger's shape.
    const S2Shape *shape);

/// Function `s2_destroy_trigger`
///
/// Remove a trigger from the world and destroy it.
S2_API void S2_API_CALL s2_destroy_trigger(
    /// The trigger to be destroyed.
    S2Trigger trigger);

/// Function `s2_step`
///
/// Advance a world's simulation by a real-world time step.
S2_API void S2_API_CALL s2_step(
    /// The world to be simulated.
    S2World world,
    /// The time step for a step, indicating a real-world time duration.
    float delta_time);

/// Function `s2_get_world_config`
///
/// Get the current configuration of the specified world.
S2_API S2WorldConfig S2_API_CALL s2_get_world_config(
    /// World handle.
    S2World world);

/// Function `s2_get_world_grid_resolution`
///
/// Get the current 2D grid resolution of the world. The grid resolution value
/// in `WorldConfig` is one single integer, indicating the maximum component of
/// grid resolution along the x-axis and the y-axis. This function returns both
/// two components of the grid resolution in `S2Vec2I` type.
S2_API S2Vec2I S2_API_CALL s2_get_world_grid_resolution(
    /// World handle.
    S2World world);

/// Function `s2_set_substep_timestep`
///
/// Set the time step for simulation sub-steps.
S2_API void S2_API_CALL s2_set_substep_timestep(
    /// World handle.
    S2World world,
    /// The new time step of each sub-step.
    float delta_time);

/// Function `s2_set_gravity`
///
/// Set a world's gravity.
S2_API void S2_API_CALL s2_set_gravity(
    /// World handle.
    S2World world,
    /// The new value of gravity.
    const S2Vec2 *gravity);

/// Function `s2_set_world_query_enabled`
///
/// Enable or disable the world querying.
S2_API void S2_API_CALL s2_set_world_query_enabled(
    /// World handle.
    S2World world,
    /// A @ref `S2Bool` value, indicating enabling or disabling.
    uint32_t enable);

/// Function `s2_set_world_offset`
///
/// Set a world's offset.
S2_API void S2_API_CALL s2_set_world_offset(
    /// World handle.
    S2World world, const S2Vec2 *offset);

/// Function `s2_set_world_extent`
///
/// Set a world's extent.
S2_API void S2_API_CALL s2_set_world_extent(
    /// World handle.
    S2World world, const S2Vec2 *extent);

/// Function `s2_set_mesh_body_force_scale`
///
/// Set the scale factor of the mesh body's internal force.
S2_API void S2_API_CALL s2_set_mesh_body_force_scale(
    /// World handle.
    S2World world,
    /// The new value of scale.
    float scale);

/// Function `s2_apply_impulse_in_circular_area`
///
/// Apply an impulse within a specified circular spatial region.
S2_API void S2_API_CALL s2_apply_impulse_in_circular_area(
    /// World handle.
    S2World world,
    /// Applied impulse, equivalent to applied_force * applied_time.
    const S2Vec2 *impulse,
    /// The center of the specified circular area.
    const S2Vec2 *center,
    /// The radius of the specified circular area.
    float radius);

/// Function `s2_get_buffer`
///
/// Get the handle of a soft2d internal GPU buffer, useful for rendering or
/// debugging.
S2_API void S2_API_CALL s2_get_buffer(
    /// World handle.
    S2World world,
    /// The name of the internal buffer as per #S2BufferName.
    S2BufferName buffer_name,
    /// A pointer to a `TiNdArray` to store the internal buffer reference.
    TiNdArray *buffer);

/// Function `s2_export_buffer_to_texture`
///
/// Export buffer data to a user-specified texture, useful for rendering or
/// debugging. Currently, this function only support
/// #S2_BUFFER_NAME_FINE_GRID_COLLIDER_NUM and
/// #S2_BUFFER_NAME_FINE_GRID_COLLIDER_NUM.
S2_API void S2_API_CALL s2_export_buffer_to_texture(
    /// World handle.
    S2World world,
    /// The buffer name to export. Currently, this arguments only supports
    /// #S2_BUFFER_NAME_FINE_GRID_COLLIDER_NUM or
    /// #S2_BUFFER_NAME_FINE_GRID_COLLIDER_NUM.
    S2BufferName buffer_name,
    /// A option to control whether flip the coordinates of the y-axis or not.
    S2Bool y_flipped,
    /// A factor to scale the numerical data to be exported.
    float scale,
    /// The destination texture for exporting.
    const TiTexture *texture);

/// Function `s2_apply_linear_impulse`
///
/// Apply a linear impulse to all particles within a body.
S2_API void S2_API_CALL s2_apply_linear_impulse(
    /// Body handle.
    S2Body body,
    /// Applied linear impulse.
    const S2Vec2 *impulse);

/// Function `s2_apply_angular_impulse`
///
/// Apply an angular impulse to all particles within a body. The average value
/// of positions of all particles is treated as the body's center.
S2_API void S2_API_CALL s2_apply_angular_impulse(
    /// Body handle.
    S2Body body,
    /// Applied angular impulse.
    float impulse);

/// Function `s2_set_body_material`
///
/// Set a body's physical material.
S2_API void S2_API_CALL s2_set_body_material(
    /// Body handle.
    S2Body body,
    /// The new value of material.
    const S2Material *material);

/// Function `s2_set_body_tag`
///
/// Set all particles' tags in a body to a single value.
S2_API void S2_API_CALL s2_set_body_tag(
    /// Body handle.
    S2Body body,
    /// The new value of tag.
    uint32_t tag);

/// Function `s2_set_collider_position`
///
/// Set a collider's position.
S2_API void S2_API_CALL s2_set_collider_position(
    /// Collider handle.
    S2Collider collider,
    /// The new value of position.
    const S2Vec2 *position);

/// Function `s2_get_collider_position`
///
/// Get a collider's position.
S2_API S2Vec2 S2_API_CALL s2_get_collider_position(
    /// Collider handle.
    S2Collider collider);

/// Function `s2_set_collider_rotation`
///
/// Set a collider's rotation.
S2_API void S2_API_CALL s2_set_collider_rotation(
    /// Collider handle.
    S2Collider collider,
    /// The new value of rotation.
    float rotation);

/// Function `s2_get_collider_rotation`
///
/// Get a collider's rotation.
S2_API float S2_API_CALL s2_get_collider_rotation(
    /// Collider handle.
    S2Collider collider);

/// Function `s2_set_collider_linear_velocity`
///
/// Set a collider's linear velocity.
S2_API void S2_API_CALL s2_set_collider_linear_velocity(
    /// Collider handle.
    S2Collider collider,
    /// The new value of linear velocity.
    const S2Vec2 *linear_velocity);

/// Function `s2_get_collider_linear_velocity`
///
/// Get a collider's linear velocity.
S2_API S2Vec2 S2_API_CALL s2_get_collider_linear_velocity(
    /// Collider handle.
    S2Collider collider);

/// Function `s2_set_collider_angular_velocity`
///
/// Set a collider's angular velocity.
S2_API void S2_API_CALL s2_set_collider_angular_velocity(
    /// Collider handle.
    S2Collider collider,
    /// The new value of angular velocity.
    float angular_velocity);

/// Function `s2_get_collider_angular_velocity`
///
/// Get a collider's angular velocity.
S2_API float S2_API_CALL s2_get_collider_angular_velocity(
    /// Collider handle.
    S2Collider collider);

/// Function `s2_set_trigger_position`
///
/// Set a trigger's position.
S2_API void S2_API_CALL s2_set_trigger_position(
    /// Trigger handle.
    S2Trigger trigger,
    /// The new value of position.
    const S2Vec2 *position);

/// Function `s2_get_trigger_position`
///
/// Get a trigger's position.
S2_API S2Vec2 S2_API_CALL s2_get_trigger_position(
    /// Trigger handle.
    S2Trigger trigger);

/// Function `s2_set_trigger_rotation`
///
/// Set a trigger's rotation.
S2_API void S2_API_CALL s2_set_trigger_rotation(
    /// Trigger handle.
    S2Trigger trigger,
    /// The new value of rotation.
    float rotation);

/// Function `s2_get_trigger_rotation`
///
/// Get a trigger's rotation.
S2_API float S2_API_CALL s2_get_trigger_rotation(
    /// Trigger handle.
    S2Trigger trigger);

/// Function `s2_query_trigger_overlapped`
///
/// Query whether there are particles in a trigger area. This function works
/// only when `S2WorldConfig.enable_world_query` is true.
S2_API uint32_t S2_API_CALL s2_query_trigger_overlapped(
    /// Trigger handle.
    S2Trigger trigger);

/// Function `s2_query_trigger_overlapped_by_tag`
///
/// Query whether there are specified particles in a trigger area. A particle is
/// treated as specified only when it satisfies the condition: `(particle.tag &
/// mask) == tag`. This function works only when
/// `S2WorldConfig.enable_world_query` is true.
S2_API uint32_t S2_API_CALL s2_query_trigger_overlapped_by_tag(
    /// Trigger handle.
    S2Trigger trigger,
    /// User-specified tag.
    uint32_t tag,
    /// User-specified mask.
    uint32_t mask);

/// Function `s2_query_particle_num_in_trigger`
///
/// Query the number of particles in a trigger area. This function works only
/// when `S2WorldConfig.enable_world_query` is true.
S2_API uint32_t S2_API_CALL s2_query_particle_num_in_trigger(
    /// Trigger handle.
    S2Trigger trigger);

/// Function `s2_query_particle_num_in_trigger_by_tag`
///
/// Query the number of specified particles in a trigger area. A particle is
/// treated as specified only when it satisfies the condition: `(particle.tag &
/// mask) == tag`. This function works only when
/// `S2WorldConfig.enable_world_query` is true.
S2_API uint32_t S2_API_CALL s2_query_particle_num_in_trigger_by_tag(
    /// Trigger handle.
    S2Trigger trigger,
    /// User-specified tag.
    uint32_t tag,
    /// User-specified mask.
    uint32_t mask);

/// Function `s2_remove_particles_in_trigger`
///
/// Remove all particles in a trigger area from the world. This function works
/// only when `S2WorldConfig.enable_world_query` is true.
S2_API void S2_API_CALL s2_remove_particles_in_trigger(
    /// Trigger handle.
    S2Trigger trigger);

/// Function `s2_remove_particles_in_trigger_by_tag`
///
/// Remove all specified particles in a trigger area from the world. A particle
/// is treated as specified only when it satisfies the condition: `(particle.tag
/// & mask) == tag`. This function works only when
/// `S2WorldConfig.enable_world_query` is true.
S2_API void S2_API_CALL s2_remove_particles_in_trigger_by_tag(
    /// Trigger handle.
    S2Trigger trigger,
    /// User-specified tag.
    uint32_t tag,
    /// User-specified mask.
    uint32_t mask);

/// Function `s2_manipulate_particles_in_trigger`
///
/// Manipulate particles in a trigger area. An internal particle buffer is
/// exposed to users via this function, and users could provide a callback
/// function operating on that buffer. The user-provided callback function will
/// be automatically executed before the next step. Currently, this function
/// allows two kinds of users' operations:
/// * Change the tag of a particle by modifying `S2Particle.tag`.
/// * Mark a particle for removal before the next step by setting
/// `S2Particle.is_removed` to true.
///
/// See `S2Particle` and #S2ParticleManipulationCallback for more details.
/// This function works only when `S2WorldConfig.enable_world_query` is true.
S2_API void S2_API_CALL s2_manipulate_particles_in_trigger(
    /// Trigger handle.
    S2Trigger trigger,
    /// User-specified callback function. This parameter should be a function
    /// pointer of the #S2ParticleManipulationCallback type.
    S2ParticleManipulationCallback callback);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
