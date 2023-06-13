// avoid clang-format disorders headers
// clang-format off
#include <taichi/taichi.h>
#include <soft2d/soft2d.h>
#include "common.h"
#include "taichi/aot_demo/framework.hpp"
// clang-format on

using namespace ti::aot_demo;
using namespace std;

template <typename T>
void interchange_vulkan_ndarray_texture(GraphicsRuntime &g_runtime,
                                        ti::Texture &vulkan_texture,
                                        ti::Runtime &vulkan_runtime,
                                        ti::NdArray<T> &vulkan_ndarray,
                                        bool texture_to_ndarray) {
  // Get Vulkan Ndarray Interop Info
  TiVulkanMemoryInteropInfo vulkan_interop_info;
  ti_export_vulkan_memory(vulkan_runtime.runtime(),
                          vulkan_ndarray.memory().memory(),
                          &vulkan_interop_info);

  VkBuffer ndarray_buffer = vulkan_interop_info.buffer;

  uint32_t width =
      vulkan_ndarray.shape().dim_count > 0 ? vulkan_ndarray.shape().dims[0] : 1;
  uint32_t height =
      vulkan_ndarray.shape().dim_count > 1 ? vulkan_ndarray.shape().dims[1] : 1;
  uint32_t channel =
      vulkan_ndarray.shape().dim_count > 2 ? vulkan_ndarray.shape().dims[2] : 1;

  // Get VkImage
  VkImageLayout image_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  g_runtime.transition_image(vulkan_texture.image(),
                             TI_IMAGE_LAYOUT_TRANSFER_DST);
  if (texture_to_ndarray) {
    image_layout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    g_runtime.transition_image(vulkan_texture.image(),
                               TI_IMAGE_LAYOUT_TRANSFER_SRC);
  }

  TiVulkanImageInteropInfo interop_info;
  ti_export_vulkan_image(g_runtime, vulkan_texture.image().image(),
                         &interop_info);

  VkImage vk_image = interop_info.image;
  VkDevice vk_device = g_runtime.renderer().device_;
  VkCommandPool cmd_pool = g_runtime.renderer().command_pool_;
  VkQueue graphics_queue = g_runtime.renderer().queue_;
  copyImage2Buffer(vk_device, cmd_pool, graphics_queue, vk_image,
                   ndarray_buffer, image_layout, width, height, channel,
                   texture_to_ndarray /*image_to_buffer*/);
}

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

    // Renderer initialization
    Renderer &renderer = F.renderer();
    renderer.set_framebuffer_size(win_width, win_height);

    GraphicsRuntime &runtime = F.runtime();

    x_ = runtime.allocate_vertex_buffer(
        default_world_config.max_allowed_particle_num, 2);

    draw_points = runtime.draw_points(x_)
                      .point_size(3.0f)
                      .color(glm::vec3(1, 0.5, 0))
                      .build();

    // Soft2D initialization
    // Create a world
    S2WorldConfig config{};
    config.max_allowed_particle_num = 90000;
    config.max_allowed_body_num = 10000;
    config.max_allowed_element_num = 10000;
    config.max_allowed_trigger_num = 10000;
    config.grid_resolution = 128;
    config.out_world_boundary_policy =
        S2OutWorldBoundaryPolicy::S2_OUT_WORLD_BOUNDARY_POLICY_REMOVING;

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

    world = s2_create_world(TiArch::TI_ARCH_VULKAN, runtime, &config);

    // Box
    // Create a box body
    // S2BoxShape box_shape;
    // S2Vec2 half_extent;
    // half_extent.x = 0.03f;
    // half_extent.y = 0.02f;
    // box_shape.half_extent = half_extent;
    // S2Shape shape;
    // shape.type = S2ShapeType::S2_SHAPE_TYPE_BOX;
    // S2ShapeUnion shape_union;
    // shape_union.box = box_shape;
    // shape.shape_union = shape_union;
    S2Shape shape = make_box_shape(vec2(0.03f, 0.02f));

    S2Material material;
    material.type = S2MaterialType::S2_MATERIAL_TYPE_ELASTIC;
    material.density = 1000.0f;
    material.youngs_modulus = 1.0f;
    material.poissons_ratio = 0.2f;

    S2Kinematics kinematics{};

    kinematics.center = vec2(0.3f, 0.5f);
    kinematics.mobility = S2Mobility::S2_MOBILITY_DYNAMIC;

    make_body(world, material, kinematics, shape);

    // Circle body
    S2Shape circle_shape = make_circle_shape(0.03f);

    S2Material circle_material{};
    circle_material.type = S2MaterialType::S2_MATERIAL_TYPE_ELASTIC;
    circle_material.density = 1000.0f;
    circle_material.youngs_modulus = 1.0f;
    circle_material.poissons_ratio = 0.2f;

    S2Kinematics circle_kinematics{};
    circle_kinematics.center = vec2(0.4f, 0.5f);
    circle_kinematics.mobility = S2Mobility::S2_MOBILITY_DYNAMIC;
    make_body(world, circle_material, circle_kinematics, circle_shape);

    // Ellipse body
    S2Shape ellipse_shape = make_ellipse_shape(0.03f, 0.02f);

    S2Material ellipse_material{};
    ellipse_material.type = S2MaterialType::S2_MATERIAL_TYPE_ELASTIC;
    ellipse_material.density = 1000.0f;
    ellipse_material.youngs_modulus = 1.0f;
    ellipse_material.poissons_ratio = 0.2f;

    S2Kinematics ellipse_kinematics{};
    ellipse_kinematics.center = vec2(0.5f, 0.5f);
    ellipse_kinematics.mobility = S2Mobility::S2_MOBILITY_DYNAMIC;
    make_body(world, ellipse_material, ellipse_kinematics, ellipse_shape);

    // Capsule body
    S2Shape capsule_shape = make_capsule_shape(0.03f, 0.02f);

    S2Material capsule_material{};
    capsule_material.type = S2MaterialType::S2_MATERIAL_TYPE_ELASTIC;
    capsule_material.density = 1000.0f;
    capsule_material.youngs_modulus = 1.0f;
    capsule_material.poissons_ratio = 0.2f;

    S2Kinematics capsule_kinematics{};
    capsule_kinematics.center = vec2(0.6f, 0.5f);
    capsule_kinematics.mobility = S2Mobility::S2_MOBILITY_DYNAMIC;
    make_body(world, capsule_material, capsule_kinematics, capsule_shape);

    // polygon body
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
    make_body(world, polygon_material, polygon_kinematics, polygon_shape);

    // Add a ground
    make_collider(world, make_kinematics({0.5f, 0.0f}),
                  make_box_shape(vec2(0.5f, 0.01f)));
  }
  virtual bool update() override final {
    GraphicsRuntime &runtime = F.runtime();

    // Step forward
    s2_step(world, 0.004);

    // Get particle position gpu buffer
    TiNdArray particle_x;
    s2_get_buffer(world, S2_BUFFER_NAME_PARTICLE_POSITION, &particle_x);

    // Export particle position data to the external buffer
    TiMemorySlice src;
    src.memory = particle_x.memory;
    src.offset = 0;
    src.size =
        sizeof(float) * 2 * default_world_config.max_allowed_particle_num;
    TiMemorySlice dst;
    dst.memory = x_.memory();
    dst.offset = 0;
    dst.size =
        sizeof(float) * 2 * default_world_config.max_allowed_particle_num;
    ti_copy_memory_device_to_device(runtime, &dst, &src);

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
