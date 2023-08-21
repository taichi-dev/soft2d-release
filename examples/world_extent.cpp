// avoid clang-format disorders headers
// clang-format off
#include <taichi/taichi.h>
#include <soft2d/soft2d.h>
#include "common.h"
#include "globals.h"
#include "emitter.h"
#include "taichi/aot_demo/framework.hpp"
#include <glm/gtc/matrix_transform.hpp>

// clang-format on

using namespace ti::aot_demo;
using namespace std;

constexpr float win_fov = 1.0 * win_width / win_height;

struct WorldExtent : public App {

  S2World world;
  // S2Trigger trigger;

  std::unique_ptr<GraphicsTask> draw_points;
  std::unique_ptr<GraphicsTask> draw_collider_texture;
  std::unique_ptr<GraphicsTask> draw_trigger_texture;
  std::unique_ptr<GraphicsTask> draw_mesh;
  ti::NdArray<uint32_t> element_indices_;

  ti::NdArray<float> x_;
  ti::Texture collider_texture_;
  ti::Texture trigger_texture_;

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
    // Enlarge the size of world to 50(m) x 50(m).
    config.extent = {50.0f, 50.0f};
    config.enable_debugging = true;
    config.enable_world_query = true;
    config.out_world_boundary_policy =
        S2OutWorldBoundaryPolicy::S2_OUT_WORLD_BOUNDARY_POLICY_DEACTIVATION;
    world = s2_create_world(TiArch::TI_ARCH_VULKAN, runtime, &config);

    // A body with a size of 0.1x0.1. This body contains only a single particle
    // because the particle-sampling accuracy is not enough in such a large
    // world size.
    create_body(
        world, make_material(S2_MATERIAL_TYPE_SNOW, 1000.0f, 1.0f, 0.2),
        make_kinematics({0.1f * config.extent.x, 0.5f * config.extent.x}, 0.0f,
                        {0.0f, 0.0f}, 0.0f, S2_MOBILITY_DYNAMIC),
        make_box_shape(vec2(0.1f, 0.1f)));

    // A body with a size of 1x1.
    create_body(
        world, make_material(S2_MATERIAL_TYPE_SNOW, 1000.0f, 1.0f, 0.2),
        make_kinematics({0.3f * config.extent.x, 0.5f * config.extent.x}, 0.0f,
                        {0.0f, 0.0f}, 0.0f, S2_MOBILITY_DYNAMIC),
        make_box_shape(vec2(1.0f, 1.0f)));

    // A body with a size of 10x10.
    create_body(
        world, make_material(S2_MATERIAL_TYPE_SNOW, 1000.0f, 1.0f, 0.2),
        make_kinematics({0.7f * config.extent.x, 0.5f * config.extent.x}, 0.0f,
                        {0.0f, 0.0f}, 0.0f, S2_MOBILITY_DYNAMIC),
        make_box_shape(vec2(10.0f, 10.0f)));

    // Add the boundary
    // bottom
    create_collider(
        world, make_kinematics({config.extent.x * 0.5f, 0.0f}),
        make_box_shape(vec2(config.extent.x * 0.5f, 0.01f * config.extent.x)));
    // top
    create_collider(
        world, make_kinematics({config.extent.x * 0.5f, config.extent.x}),
        make_box_shape(vec2(config.extent.x * 0.5f, 0.01f * config.extent.x)));
    // left
    create_collider(
        world, make_kinematics({0.0f, config.extent.y * 0.5f}),
        make_box_shape(vec2(0.01f * config.extent.x, config.extent.y * 0.5f)));
    // right
    create_collider(
        world, make_kinematics({config.extent.y, config.extent.y * 0.5f}),
        make_box_shape(vec2(0.01f * config.extent.x, config.extent.y * 0.5f)));
    // Soft2D initialization ends

    // Renderer initialization begins
    Renderer &renderer = F.renderer();
    renderer.set_framebuffer_size(win_width, win_height);

    x_ = runtime.allocate_vertex_buffer(config.max_allowed_particle_num, 2);
    collider_texture_ = runtime.allocate_texture2d(
        config.grid_resolution * config.fine_grid_scale,
        config.grid_resolution * config.fine_grid_scale, TI_FORMAT_R32F,
        TI_NULL_HANDLE);
    trigger_texture_ = runtime.allocate_texture2d(
        config.grid_resolution * config.fine_grid_scale,
        config.grid_resolution * config.fine_grid_scale, TI_FORMAT_R32F,
        TI_NULL_HANDLE);
    element_indices_ =
        runtime.allocate_index_buffer(config.max_allowed_element_num * 3, 1);

    draw_collider_texture = runtime.draw_texture(collider_texture_)
                                .is_single_channel()
                                .color(glm::vec3(0.2, 0.8, 0.0))
                                .build();
    draw_trigger_texture = runtime.draw_texture(trigger_texture_)
                               .is_single_channel()
                               .color(glm::vec3(0.85, 0.1, 0.0))
                               .build();
    // Renderer initialization ends
  }
  int frame = 0;
  virtual bool update() override final {
    GraphicsRuntime &runtime = F.runtime();

    s2_step(world, 0.004);

    // Export particle position data to the external buffer
    TiNdArray particle_x;
    s2_get_buffer(world, S2_BUFFER_NAME_PARTICLE_POSITION, &particle_x);
    ndarray_data_copy(runtime.runtime(), x_.ndarray(), particle_x,
                      sizeof(float) * 2 *
                          s2_get_world_config(world).max_allowed_particle_num);

    // Export collider and trigger buffers to texture
    auto collider_tex = collider_texture_.texture();
    auto trigger_tex = trigger_texture_.texture();
    s2_export_buffer_to_texture(world, S2_BUFFER_NAME_FINE_GRID_COLLIDER_NUM,
                                true, 0.8f, &collider_tex);
    s2_export_buffer_to_texture(world, S2_BUFFER_NAME_FINE_GRID_TRIGGER_ID,
                                true, 0.8f, &trigger_tex);

    // Export element indices to the external buffer
    TiNdArray element_indices_tmp;
    s2_get_buffer(world, S2_BUFFER_NAME_ELEMENT_INDICES, &element_indices_tmp);
    ndarray_data_copy(
        runtime.runtime(), element_indices_.ndarray(), element_indices_tmp,
        sizeof(int) * 3 * s2_get_world_config(world).max_allowed_element_num);

    // Since taichi and renderer use different command buffers, we must
    // explicitly use flushing (submitting taichi's command list) here, which
    // provides a semaphore between two command buffers.
    runtime.flush();

    // Update viewport since world offset is changing
    auto w_offset = s2_get_world_config(world).offset;
    auto w_extent = s2_get_world_config(world).extent;
    glm::vec2 offset = {w_offset.x, w_offset.y};
    glm::vec2 extent = {w_extent.x, w_extent.y};
    draw_points = runtime.draw_points(x_, offset, extent)
                      .point_size(preset_point_size)
                      .color(glm::vec3(1, 0.5, 0))
                      .build();
    // Set up orthogonal view
    glm::mat4 model2world = glm::mat4(1.0f);
    glm::mat4 camera2view =
        glm::ortho(1.0, -1.0, 1.0 * win_fov, -1.0 * win_fov, -1000.0, 1000.0);
    glm::mat4 world2camera = glm::lookAt(
        glm::vec3(0.0, 0.0, 10), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -1, 0));
    glm::mat4 world2view = camera2view * world2camera;

    draw_mesh = runtime.draw_mesh(x_, element_indices_, offset, extent)
                    .model2world(model2world)
                    .world2view(world2view)
                    .color(glm::vec3(1.0, 0.5, 0.0))
                    .polygon_mode(VK_POLYGON_MODE_LINE)
                    .build();

    ++frame;
    return true;
  }
  virtual void render() override final {
    Renderer &renderer = F.renderer();
    renderer.enqueue_graphics_task(*draw_mesh);
    renderer.enqueue_graphics_task(*draw_points);
    renderer.enqueue_graphics_task(*draw_collider_texture);
    renderer.enqueue_graphics_task(*draw_trigger_texture);
  }
};

std::unique_ptr<App> create_app() {
  return std::unique_ptr<App>(new WorldExtent);
}
