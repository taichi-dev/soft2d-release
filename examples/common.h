#include <soft2d/soft2d.h>
#include <taichi/taichi.h>
#include <vector>

inline S2Vec2 vec2(float x, float y) {
  S2Vec2 out{};
  out.x = x;
  out.y = y;
  return out;
}

inline S2Vec2I vec2i(int x, int y) {
  S2Vec2I out{};
  out.x = x;
  out.y = y;
  return out;
}

inline S2Vec2 add(S2Vec2 a, S2Vec2 b) {
  S2Vec2 out{};
  out.x = a.x + b.x;
  out.y = a.y + b.y;
  return out;
}

inline S2Vec2 sub(S2Vec2 a, S2Vec2 b) {
  S2Vec2 out{};
  out.x = a.x - b.x;
  out.y = a.y - b.y;
  return out;
}

inline S2Vec2 mul(S2Vec2 a, float b) {
  S2Vec2 out{};
  out.x = a.x * b;
  out.y = a.y * b;
  return out;
}

inline S2Vec2 mul(float a, S2Vec2 b) {
  S2Vec2 out{};
  out.x = a * b.x;
  out.y = a * b.y;
  return out;
}

inline S2Vec2 div(S2Vec2 a, float b) {
  S2Vec2 out{};
  out.x = a.x / b;
  out.y = a.y / b;
  return out;
}

inline S2Shape make_box_shape(S2Vec2 half_extent) {
  S2Shape out{};
  S2BoxShape box{};
  box.half_extent = half_extent;
  out.type = S2ShapeType::S2_SHAPE_TYPE_BOX;
  out.shape_union.box = box;
  return out;
}

inline S2Shape make_circle_shape(float radius) {
  S2Shape out{};
  S2CircleShape circle{};
  circle.radius = radius;
  out.type = S2ShapeType::S2_SHAPE_TYPE_CIRCLE;
  out.shape_union.circle = circle;
  return out;
}

inline S2Shape make_ellipse_shape(float radius_x, float radius_y) {
  S2Shape out{};
  S2EllipseShape ellipse{};
  ellipse.radius_x = radius_x;
  ellipse.radius_y = radius_y;
  out.type = S2ShapeType::S2_SHAPE_TYPE_ELLIPSE;
  out.shape_union.ellipse = ellipse;
  return out;
}

inline S2Shape make_capsule_shape(float rect_half_length, float cap_radius) {
  S2Shape out{};
  S2CapsuleShape capsule{};
  capsule.rect_half_length = rect_half_length;
  capsule.cap_radius = cap_radius;
  out.type = S2ShapeType::S2_SHAPE_TYPE_CAPSULE;
  out.shape_union.capsule = capsule;
  return out;
}

inline S2Shape make_polygon_shape(S2Vec2 *vertices, int vertex_num) {
  S2Shape out{};
  S2PolygonShape polygon{};
  polygon.vertices = vertices;
  polygon.vertex_num = vertex_num;
  out.type = S2ShapeType::S2_SHAPE_TYPE_POLYGON;
  out.shape_union.polygon = polygon;
  return out;
}

inline S2Kinematics
make_kinematics(S2Vec2 center, float rotation = 0.0f,
                S2Vec2 linear_velocity = {0.0f, 0.0f},
                float angular_velocity = 0.0f,
                S2Mobility mobility = S2Mobility::S2_MOBILITY_STATIC) {
  S2Kinematics out{};
  out.center = center;
  out.rotation = rotation;
  out.linear_velocity = linear_velocity;
  out.angular_velocity = angular_velocity;
  out.mobility = mobility;
  return out;
}

inline S2Material make_material(S2MaterialType type, float density,
                                float youngs_modulus, float poissons_ratio) {
  S2Material out{};
  out.type = type;
  out.density = density;
  out.youngs_modulus = youngs_modulus;
  out.poissons_ratio = poissons_ratio;
  return out;
}

inline S2Body create_body(S2World world, S2Material material,
                          S2Kinematics kinematics, S2Shape shape,
                          uint32_t tag = 0) {
  S2Body out{};
  out = s2_create_body(world, &material, &kinematics, &shape, tag);
  return out;
}

inline S2Body create_custom_body(S2World world, S2Material material,
                                 S2Kinematics kinematics, uint32_t particle_num,
                                 void *particles_in_local_space,
                                 uint32_t tag = 0) {
  S2Body out{};
  out = s2_create_custom_body(world, &material, &kinematics, particle_num,
                              particles_in_local_space, tag);
  return out;
}

inline S2Body create_custom_body_from_vector(
    S2World world, S2Material material, S2Kinematics kinematics,
    std::vector<S2Vec2> particles_in_local_space, uint32_t tag = 0) {
  S2Body out{};
  out = s2_create_custom_body(world, &material, &kinematics,
                              particles_in_local_space.size(),
                              particles_in_local_space.data(), tag);
  return out;
}

inline S2Body create_mesh_body(S2World world, S2Material material,
                               S2Kinematics kinematics, uint32_t vertex_num,
                               void *vertices_in_local_space,
                               uint32_t triangle_num, void *triangle_indices,
                               uint32_t tag = 0) {
  S2Body out{};
  out = s2_create_mesh_body(world, &material, &kinematics, vertex_num,
                            vertices_in_local_space, triangle_num,
                            triangle_indices, tag);
  return out;
}

inline S2Body create_mesh_body_from_vector(
    S2World world, S2Material material, S2Kinematics kinematics,
    std::vector<S2Vec2> vertices_in_local_space,
    std::vector<int> triangle_indices, uint32_t tag = 0) {
  S2Body out{};
  out = create_mesh_body(world, material, kinematics,
                         vertices_in_local_space.size(),
                         vertices_in_local_space.data(),
                         triangle_indices.size(), triangle_indices.data(), tag);
  return out;
}

inline S2Collider
create_collider(S2World world, S2Kinematics kinematics, S2Shape shape,
                S2CollisionParameter cp = {
                    S2CollisionType::S2_COLLISION_TYPE_SEPARATE, 0.0f, 0.0f}) {
  S2Collider out{};
  out = s2_create_collider(world, &kinematics, &shape, &cp);
  return out;
}

inline S2Trigger create_trigger(S2World world, S2Kinematics kinematics,
                                S2Shape shape) {
  S2Trigger out{};
  out = s2_create_trigger(world, &kinematics, &shape);
  return out;
}

inline void ndarray_data_copy(const TiRuntime &runtime,
                              const TiNdArray &dst_arr,
                              const TiNdArray &src_arr, size_t size_in_bytes) {
  TiMemorySlice src;
  src.memory = src_arr.memory;
  src.offset = 0;
  src.size = size_in_bytes;
  TiMemorySlice dst;
  dst.memory = dst_arr.memory;
  dst.offset = 0;
  dst.size = size_in_bytes;
  ti_copy_memory_device_to_device(runtime, &dst, &src);
}
