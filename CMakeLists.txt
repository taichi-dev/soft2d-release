cmake_minimum_required(VERSION 3.10)

project(soft2d_examples)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Release)

set(LIB_NAME soft2d_examples)

message(STATUS "Check VULKAN_SDK environment variable: $ENV{VULKAN_SDK}")
find_package(Vulkan REQUIRED)

set(TAICHI_C_API_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/taichi_c_api")

if (${PLATFORM_NAME} STREQUAL "linux")
    set(TAICHI_C_API_BINARY_DIR "${TAICHI_C_API_INSTALL_DIR}/bin/Linux/x86_64")
    set(SOFT2D_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/soft2d/bin/Linux/x86_64")
else() # Windows
    set(TAICHI_C_API_BINARY_DIR "${TAICHI_C_API_INSTALL_DIR}/lib/Windows/x86_64_vc22")
    set(SOFT2D_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/soft2d/lib/Windows/x86_64_vc22")
endif()

# Find built taichi C-API library in `TAICHI_C_API_INSTALL_DIR`.
find_library(taichi_c_api taichi_c_api HINTS
    ${TAICHI_C_API_BINARY_DIR}
    # CMake find root is overriden by Android toolchain.
    NO_CMAKE_FIND_ROOT_PATH)
if (NOT EXISTS ${taichi_c_api})
    message(FATAL_ERROR "Couldn't find Taichi C-API library; ensure your Taichi is built with `TI_WITH_CAPI=ON`")
endif()

list(APPEND Taichi_C_API_INCLUDE_DIRECTORIES
"${TAICHI_C_API_INSTALL_DIR}/include")

list(APPEND Taichi_C_API_LINK_LIBRARIES
${taichi_c_api}
)

# Find Soft2D library
find_library(soft2d soft2d HINTS
${SOFT2D_BINARY_DIR}
# CMake find root is overriden by Android toolchain.
NO_CMAKE_FIND_ROOT_PATH)
if (NOT EXISTS ${soft2d})
    message(FATAL_ERROR "Couldn't find soft2d library")
endif()

list(APPEND SOFT2D_INCLUDE_DIRECTORIES
"${CMAKE_CURRENT_SOURCE_DIR}/soft2d/include")


if(${BUILD_TEST})
    set(test_exec_name "tests")
    # Collect test files
    aux_source_directory(tests TEST_SOURCES)
    add_executable(${test_exec_name} ${TEST_SOURCES})

    # Add dependencies
    target_link_libraries(${test_exec_name} PUBLIC ${soft2d} ${taichi_c_api})
    target_include_directories(${test_exec_name} PRIVATE ${SOFT2D_INCLUDE_DIRECTORIES} ${Taichi_C_API_INCLUDE_DIRECTORIES})

else() # Build examples
    # Find renderder dependencies.
    add_subdirectory(renderer/external/glfw)
    add_subdirectory(renderer/external/VulkanMemoryAllocator)
    add_subdirectory(renderer/external/glslang)
    add_subdirectory(renderer/external/glm)


    add_library(GraphiT OBJECT
        "${CMAKE_CURRENT_SOURCE_DIR}/renderer/external/graphi-t/include/gft/args.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/renderer/external/graphi-t/include/gft/util.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/renderer/external/graphi-t/src/gft/args.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/renderer/external/graphi-t/src/gft/util.cpp")
    target_include_directories(GraphiT PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/renderer/external/graphi-t/include")

    # Declare framework target.
    list(APPEND TaichiAotDemoFramework_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/framework/include/taichi/aot_demo/common.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/framework/include/taichi/aot_demo/framework.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/framework/include/taichi/aot_demo/renderer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/framework/include/taichi/aot_demo/graphics_runtime.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/framework/include/taichi/aot_demo/asset_manager.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/framework/src/taichi/aot_demo/framework.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/framework/src/taichi/aot_demo/renderer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/framework/src/taichi/aot_demo/glslang.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/framework/src/taichi/aot_demo/graphics_runtime.cpp"
    # Draw functions.
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/framework/include/taichi/aot_demo/draws/draw_points.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/framework/include/taichi/aot_demo/draws/draw_mesh.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/framework/include/taichi/aot_demo/draws/draw_particles.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/framework/include/taichi/aot_demo/draws/draw_texture.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/framework/include/taichi/aot_demo/draws/draw_mesh.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/framework/src/taichi/aot_demo/draws/draw_points.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/framework/src/taichi/aot_demo/draws/draw_mesh.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/framework/src/taichi/aot_demo/draws/draw_particles.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/framework/src/taichi/aot_demo/draws/draw_texture.cpp"
    )

    list(APPEND TaichiAotDemoFramework_LINK_LIBRARIES
        ${Vulkan_LIBRARY}
        glm
        VulkanMemoryAllocator
        glslang
        SPIRV
        GraphiT)
    list(APPEND TaichiAotDemoFramework_INCLUDE_DIRECTORIES
        ${Vulkan_INCLUDE_DIR}
        "${CMAKE_CURRENT_SOURCE_DIR}/renderer/framework/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/renderer/external/VulkanMemoryAllocator/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/renderer/external/glslang/glslang/Include"
        "${CMAKE_CURRENT_SOURCE_DIR}/renderer/external/glm"
        "${CMAKE_CURRENT_SOURCE_DIR}/renderer/external/graphi-t/include")


    add_library(TaichiAotDemoFramework OBJECT ${TaichiAotDemoFramework_SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/framework/src/taichi/aot_demo/entry_points/glfw.cpp")
    target_link_libraries(TaichiAotDemoFramework PUBLIC ${TaichiAotDemoFramework_LINK_LIBRARIES})
    target_include_directories(TaichiAotDemoFramework PUBLIC ${TaichiAotDemoFramework_INCLUDE_DIRECTORIES})
    target_link_libraries(TaichiAotDemoFramework PUBLIC ${Taichi_C_API_LINK_LIBRARIES})
    target_include_directories(TaichiAotDemoFramework PUBLIC ${Taichi_C_API_INCLUDE_DIRECTORIES})
    # glfw entrypoint
    target_compile_definitions(TaichiAotDemoFramework PUBLIC TI_AOT_DEMO_WITH_GLFW=1)
    target_link_libraries(TaichiAotDemoFramework PUBLIC glfw)
    target_include_directories(TaichiAotDemoFramework PUBLIC ${Vulkan_INCLUDE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/renderer/external/glfw/include")


    if("${EXAMPLE_NAME}" STREQUAL "")
        # Build all examples
        file(GLOB example_cpps "examples/*.cpp")
    else()
        # Build a specific example
        file(GLOB example_cpps "examples/${EXAMPLE_NAME}.cpp")
    endif()

    foreach(file ${example_cpps})
        string(REGEX REPLACE ".*examples/(.+).cpp" "\\1" result "${file}")
        list(GET result 0 example_name)

        message("Building example: ${example_name}")

        add_executable(${example_name} ${file})
        target_link_libraries(${example_name} PUBLIC ${soft2d})
        target_include_directories(${example_name} PUBLIC ${SOFT2D_INCLUDE_DIRECTORIES})
        
        # glfw entrypoint
        target_link_libraries(${example_name} PUBLIC TaichiAotDemoFramework GraphiT)

    endforeach()
endif()

