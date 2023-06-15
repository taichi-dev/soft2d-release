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

constexpr int win_width = 800;
constexpr int win_height = 800;
constexpr float win_fov = 1.0 * win_width / win_height;

struct MeshTaichi : public App {

  S2World world;

  std::unique_ptr<GraphicsTask> draw_points;
  std::unique_ptr<GraphicsTask> draw_collider_texture;
  std::unique_ptr<GraphicsTask> draw_mesh;
  ti::NdArray<uint32_t> element_indices_;

  ti::NdArray<float> x_;
  ti::Texture collider_texture_;

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
    config.enable_debugging = true;
    world = s2_create_world(TiArch::TI_ARCH_VULKAN, runtime, &config);

    auto vertices = std::vector<S2Vec2>{};
    auto indices = std::vector<int>{};
    auto T_vertices = std::vector<S2Vec2>{};
    auto T_indices = std::vector<int>{};
    auto A_vertices = std::vector<S2Vec2>{};
    auto A_indices = std::vector<int>{};
    auto I_vertices = std::vector<S2Vec2>{};
    auto I_indices = std::vector<int>{};
    auto C_vertices = std::vector<S2Vec2>{};
    auto C_indices = std::vector<int>{};
    auto H_vertices = std::vector<S2Vec2>{};
    auto H_indices = std::vector<int>{};
    auto S_vertices = std::vector<S2Vec2>{};
    auto S_indices = std::vector<int>{};
    auto O_vertices = std::vector<S2Vec2>{};
    auto O_indices = std::vector<int>{};
    auto F_vertices = std::vector<S2Vec2>{};
    auto F_indices = std::vector<int>{};
    auto Two_vertices = std::vector<S2Vec2>{};
    auto Two_indices = std::vector<int>{};
    auto D_vertices = std::vector<S2Vec2>{};
    auto D_indices = std::vector<int>{};

    // T
    T_vertices = {
        {0.419500, 0.186000}, {0.485000, 0.175000}, {0.551623, 0.185365},
        {0.408986, 0.253493}, {0.485840, 0.233717}, {0.563000, 0.253000},
        {0.409000, 0.326000}, {0.486085, 0.300594}, {0.563000, 0.326000},
        {0.409000, 0.402000}, {0.486000, 0.382002}, {0.563000, 0.402000},
        {0.409000, 0.481000}, {0.486000, 0.482208}, {0.563000, 0.481000},
        {0.409000, 0.559000}, {0.485578, 0.589434}, {0.563000, 0.559000},
        {0.228367, 0.659376}, {0.312000, 0.645000}, {0.394574, 0.633604},
        {0.483514, 0.681314}, {0.576646, 0.633374}, {0.657000, 0.645000},
        {0.740883, 0.659117}, {0.228126, 0.743856}, {0.312000, 0.758999},
        {0.397000, 0.758999}, {0.481000, 0.758999}, {0.566000, 0.758999},
        {0.653000, 0.758999}, {0.740383, 0.744383}};
    T_indices = {0,  1,  4,  3,  0,  4,  1,  2,  4,  4,  2,  5,  3,  4,  7,  6,
                 3,  7,  4,  5,  7,  7,  5,  8,  6,  7,  10, 9,  6,  10, 7,  8,
                 10, 10, 8,  11, 9,  10, 13, 12, 9,  13, 10, 11, 13, 13, 11, 14,
                 12, 13, 15, 15, 13, 16, 13, 14, 17, 16, 13, 17, 15, 16, 20, 20,
                 16, 21, 16, 17, 22, 21, 16, 22, 18, 19, 25, 25, 19, 26, 19, 20,
                 27, 26, 19, 27, 20, 21, 27, 27, 21, 28, 21, 22, 29, 28, 21, 29,
                 22, 23, 29, 29, 23, 30, 23, 24, 31, 30, 23, 31};

    // A
    A_vertices = {
        {0.225444, 0.181000}, {0.317055, 0.175912}, {0.666971, 0.181176},
        {0.729000, 0.175000}, {0.793718, 0.182612}, {0.222812, 0.243000},
        {0.307474, 0.260509}, {0.367942, 0.222493}, {0.638182, 0.231491},
        {0.696858, 0.274251}, {0.788185, 0.238000}, {0.246097, 0.306000},
        {0.351958, 0.352574}, {0.407367, 0.277367}, {0.479000, 0.281000},
        {0.545000, 0.281000}, {0.607477, 0.277292}, {0.747298, 0.348491},
        {0.768073, 0.292509}, {0.268600, 0.367000}, {0.290867, 0.427000},
        {0.440660, 0.404511}, {0.501493, 0.388985}, {0.566854, 0.401500},
        {0.650801, 0.371574}, {0.725864, 0.406509}, {0.313067, 0.487000},
        {0.378080, 0.459319}, {0.456000, 0.476507}, {0.555182, 0.471000},
        {0.620932, 0.482152}, {0.704276, 0.465000}, {0.335267, 0.547000},
        {0.416350, 0.566044}, {0.477729, 0.539492}, {0.533323, 0.535000},
        {0.660361, 0.583508}, {0.682610, 0.523492}, {0.357839, 0.608000},
        {0.382058, 0.673507}, {0.505216, 0.591732}, {0.508750, 0.671644},
        {0.636746, 0.647492}, {0.414145, 0.743325}, {0.484493, 0.759012},
        {0.562850, 0.758499}, {0.611536, 0.715507}};
    A_indices = {0,  1,  6,  5,  0,  6,  6,  1,  7,  2,  3,  9,  8,  2,  9,
                 3,  4,  10, 9,  3,  10, 5,  6,  11, 11, 6,  12, 6,  7,  13,
                 12, 6,  13, 8,  9,  16, 9,  10, 18, 17, 9,  18, 11, 12, 19,
                 19, 12, 20, 12, 13, 21, 13, 14, 21, 14, 15, 22, 21, 14, 22,
                 15, 16, 23, 22, 15, 23, 16, 9,  24, 9,  17, 24, 23, 16, 24,
                 24, 17, 25, 20, 12, 27, 12, 21, 27, 26, 20, 27, 27, 21, 28,
                 23, 24, 30, 29, 23, 30, 24, 25, 31, 30, 24, 31, 26, 27, 32,
                 27, 28, 33, 32, 27, 33, 33, 28, 34, 29, 30, 35, 35, 30, 36,
                 30, 31, 37, 36, 30, 37, 32, 33, 38, 38, 33, 39, 33, 34, 40,
                 39, 33, 40, 39, 40, 41, 35, 36, 42, 40, 35, 42, 41, 40, 42,
                 39, 41, 43, 43, 41, 44, 44, 41, 45, 41, 42, 46, 45, 41, 46};

    // I
    I_vertices = {
        {0.425670, 0.232804}, {0.492000, 0.213000}, {0.562101, 0.229980},
        {0.419000, 0.317000}, {0.494614, 0.286729}, {0.571000, 0.314000},
        {0.419000, 0.400000}, {0.495348, 0.366982}, {0.571000, 0.399000},
        {0.419000, 0.481000}, {0.495144, 0.452692}, {0.571000, 0.481000},
        {0.419000, 0.560000}, {0.495000, 0.537561}, {0.571000, 0.560000},
        {0.419000, 0.637000}, {0.490320, 0.628789}, {0.571000, 0.637000},
        {0.419000, 0.712000}, {0.499856, 0.715017}, {0.571000, 0.713000},
        {0.428184, 0.783770}, {0.493000, 0.797000}, {0.560135, 0.785124}};
    I_indices = {0,  1,  4,  3,  0,  4,  1,  2,  4,  4,  2,  5,  3,  4,
                 7,  6,  3,  7,  4,  5,  7,  7,  5,  8,  6,  7,  10, 9,
                 6,  10, 7,  8,  10, 10, 8,  11, 9,  10, 13, 12, 9,  13,
                 10, 11, 13, 13, 11, 14, 12, 13, 16, 15, 12, 16, 13, 14,
                 16, 16, 14, 17, 15, 16, 18, 16, 17, 19, 18, 16, 19, 19,
                 17, 20, 18, 19, 21, 21, 19, 22, 19, 20, 23, 22, 19, 23};

    // C
    C_vertices = {
        {0.360000, 0.271038}, {0.418492, 0.239323}, {0.487000, 0.220389},
        {0.562000, 0.215000}, {0.640506, 0.222557}, {0.716187, 0.247680},
        {0.312085, 0.314511}, {0.374320, 0.360534}, {0.509492, 0.337270},
        {0.576000, 0.327171}, {0.648507, 0.337600}, {0.741032, 0.304000},
        {0.255516, 0.424000}, {0.277564, 0.365491}, {0.418133, 0.427000},
        {0.453640, 0.372560}, {0.725717, 0.362162}, {0.245364, 0.488000},
        {0.328868, 0.470766}, {0.403188, 0.489000}, {0.245970, 0.554000},
        {0.331309, 0.585785}, {0.403587, 0.552508}, {0.256933, 0.617000},
        {0.279132, 0.673509}, {0.419119, 0.613491}, {0.455480, 0.667160},
        {0.725717, 0.675838}, {0.313348, 0.723000}, {0.378121, 0.683132},
        {0.511492, 0.701429}, {0.578000, 0.710829}, {0.649507, 0.700164},
        {0.741032, 0.734000}, {0.362000, 0.766333}, {0.420508, 0.797492},
        {0.488493, 0.815808}, {0.563000, 0.820999}, {0.640506, 0.813621},
        {0.715507, 0.789315}};
    C_indices = {0,  1,  7,  6,  0,  7,  1,  2,  8,  2,  3,  8,  3,  4,  9,  8,
                 3,  9,  4,  5,  10, 9,  4,  10, 10, 5,  11, 6,  7,  13, 7,  1,
                 15, 1,  8,  15, 14, 7,  15, 10, 11, 16, 12, 13, 18, 13, 7,  18,
                 7,  14, 18, 17, 12, 18, 18, 14, 19, 17, 18, 20, 20, 18, 21, 18,
                 19, 22, 21, 18, 22, 20, 21, 23, 23, 21, 24, 21, 22, 25, 24, 21,
                 29, 28, 24, 29, 21, 25, 29, 25, 26, 29, 32, 27, 33, 28, 29, 34,
                 29, 26, 35, 34, 29, 35, 26, 30, 35, 35, 30, 36, 30, 31, 37, 36,
                 30, 37, 31, 32, 38, 37, 31, 38, 32, 33, 39, 38, 32, 39};

    // H
    H_vertices = {
        {0.252283, 0.232404}, {0.320000, 0.213000}, {0.388763, 0.230330},
        {0.627796, 0.227327}, {0.700000, 0.213000}, {0.765051, 0.234103},
        {0.245000, 0.317000}, {0.319612, 0.312708}, {0.397000, 0.310000},
        {0.616987, 0.304493}, {0.695028, 0.290650}, {0.771000, 0.318000},
        {0.245000, 0.400000}, {0.328067, 0.403238}, {0.397000, 0.385000},
        {0.617000, 0.379000}, {0.689485, 0.379049}, {0.771000, 0.400000},
        {0.245000, 0.481000}, {0.312643, 0.476783}, {0.410230, 0.451816},
        {0.480000, 0.461000}, {0.547000, 0.461000}, {0.608313, 0.448265},
        {0.677475, 0.472781}, {0.771000, 0.481000}, {0.245000, 0.560000},
        {0.347575, 0.535540}, {0.410395, 0.583837}, {0.479000, 0.575000},
        {0.546000, 0.575000}, {0.608099, 0.586938}, {0.693546, 0.540543},
        {0.771000, 0.560000}, {0.245000, 0.637000}, {0.322350, 0.613916},
        {0.396985, 0.647492}, {0.617000, 0.653000}, {0.693822, 0.618423},
        {0.771000, 0.636000}, {0.245000, 0.712000}, {0.320129, 0.706614},
        {0.397014, 0.716507}, {0.617000, 0.722000}, {0.695718, 0.708226},
        {0.771000, 0.710000}, {0.254899, 0.784068}, {0.321000, 0.797000},
        {0.386744, 0.785233}, {0.630230, 0.787816}, {0.699507, 0.797014},
        {0.763046, 0.782465}};
    H_indices = {0,  1,  7,  6,  0,  7,  1,  2,  7,  7,  2,  8,  3,  4,  10, 9,
                 3,  10, 4,  5,  10, 10, 5,  11, 6,  7,  12, 12, 7,  13, 7,  8,
                 14, 13, 7,  14, 9,  10, 16, 15, 9,  16, 10, 11, 16, 16, 11, 17,
                 12, 13, 19, 18, 12, 19, 13, 14, 20, 19, 13, 20, 15, 16, 23, 16,
                 17, 24, 23, 16, 24, 24, 17, 25, 18, 19, 26, 19, 20, 27, 26, 19,
                 27, 27, 20, 28, 20, 21, 29, 28, 20, 29, 21, 22, 30, 29, 21, 30,
                 22, 23, 31, 30, 22, 31, 23, 24, 31, 24, 25, 32, 31, 24, 32, 32,
                 25, 33, 26, 27, 35, 34, 26, 35, 27, 28, 35, 35, 28, 36, 31, 32,
                 38, 37, 31, 38, 32, 33, 38, 38, 33, 39, 34, 35, 41, 40, 34, 41,
                 35, 36, 41, 41, 36, 42, 37, 38, 44, 43, 37, 44, 38, 39, 44, 44,
                 39, 45, 40, 41, 46, 46, 41, 47, 41, 42, 48, 47, 41, 48, 43, 44,
                 49, 49, 44, 50, 44, 45, 51, 50, 44, 51};

    // S
    S_vertices = {
        {0.303222, 0.243741}, {0.388495, 0.202577}, {0.492000, 0.189763},
        {0.603000, 0.197679}, {0.704505, 0.253273}, {0.307165, 0.334535},
        {0.401000, 0.316458}, {0.505504, 0.302372}, {0.587684, 0.364977},
        {0.746593, 0.355000}, {0.394000, 0.463326}, {0.500000, 0.433211},
        {0.649505, 0.529443}, {0.730958, 0.456979}, {0.288744, 0.604000},
        {0.313342, 0.525646}, {0.447443, 0.601861}, {0.548000, 0.558887},
        {0.692739, 0.666304}, {0.298493, 0.679507}, {0.343794, 0.746529},
        {0.482610, 0.678037}, {0.587505, 0.679561}, {0.695356, 0.757374},
        {0.419000, 0.784714}, {0.508000, 0.794915}, {0.603000, 0.788583}};
    S_indices = {0,  1,  6,  5,  0,  6,  1,  2,  7,  6,  1,  7,  2,  3,  7,
                 3,  4,  8,  7,  3,  8,  8,  4,  9,  11, 8,  12, 8,  9,  13,
                 12, 8,  13, 10, 11, 16, 14, 15, 16, 15, 10, 16, 11, 12, 17,
                 16, 11, 17, 14, 16, 19, 19, 16, 20, 20, 16, 21, 22, 18, 23,
                 20, 21, 24, 21, 22, 25, 24, 21, 25, 22, 23, 26, 25, 22, 26};

    // F
    F_vertices = {
        {0.321390, 0.222495}, {0.392000, 0.203000}, {0.458330, 0.222804},
        {0.313000, 0.311000}, {0.388789, 0.278520}, {0.465000, 0.305000},
        {0.313000, 0.397000}, {0.387861, 0.359343}, {0.465000, 0.381000},
        {0.313000, 0.480000}, {0.393431, 0.441339}, {0.483473, 0.444659},
        {0.560000, 0.451000}, {0.633000, 0.451000}, {0.699411, 0.468695},
        {0.312987, 0.559507}, {0.407038, 0.522562}, {0.477366, 0.578376},
        {0.554000, 0.565000}, {0.630000, 0.565000}, {0.698590, 0.548506},
        {0.313013, 0.635493}, {0.395446, 0.609455}, {0.476175, 0.657787},
        {0.552000, 0.673000}, {0.629000, 0.673000}, {0.702557, 0.677266},
        {0.313000, 0.709000}, {0.388116, 0.697302}, {0.466081, 0.721449},
        {0.542049, 0.733189}, {0.629418, 0.729463}, {0.721000, 0.728000},
        {0.327527, 0.777527}, {0.404000, 0.786999}, {0.482000, 0.786999},
        {0.559000, 0.786999}, {0.633000, 0.786999}, {0.704321, 0.781456}};
    F_indices = {0,  1,  4,  3,  0,  4,  1,  2,  4,  4,  2,  5,  3,  4,  7,  6,
                 3,  7,  4,  5,  7,  7,  5,  8,  6,  7,  10, 9,  6,  10, 7,  8,
                 10, 10, 8,  11, 9,  10, 16, 15, 9,  16, 10, 11, 16, 16, 11, 17,
                 11, 12, 18, 17, 11, 18, 12, 13, 19, 18, 12, 19, 13, 14, 20, 19,
                 13, 20, 15, 16, 22, 21, 15, 22, 16, 17, 22, 22, 17, 23, 21, 22,
                 28, 27, 21, 28, 22, 23, 28, 23, 24, 29, 28, 23, 29, 29, 24, 30,
                 24, 25, 31, 30, 24, 31, 25, 26, 31, 31, 26, 32, 27, 28, 33, 28,
                 29, 34, 33, 28, 34, 29, 30, 35, 34, 29, 35, 30, 31, 36, 35, 30,
                 36, 36, 31, 37, 31, 32, 38, 37, 31, 38};

    // 2
    Two_vertices = {
        {0.316000, 0.215500}, {0.395494, 0.202988}, {0.479000, 0.203000},
        {0.561000, 0.203000}, {0.641000, 0.203000}, {0.717825, 0.218214},
        {0.304649, 0.291247}, {0.409580, 0.287758}, {0.502131, 0.322729},
        {0.571000, 0.313000}, {0.644507, 0.313013}, {0.717960, 0.298420},
        {0.345509, 0.351208}, {0.395489, 0.395532}, {0.441000, 0.435909},
        {0.549000, 0.379593}, {0.601000, 0.425600}, {0.486000, 0.475696},
        {0.531959, 0.518980}, {0.649191, 0.470553}, {0.691020, 0.519490},
        {0.565000, 0.572000}, {0.632994, 0.570890}, {0.717800, 0.575000},
        {0.313636, 0.653500}, {0.370491, 0.668151}, {0.566586, 0.630000},
        {0.627106, 0.674016}, {0.724000, 0.636508}, {0.304985, 0.717508},
        {0.424000, 0.687000}, {0.478000, 0.692630}, {0.529510, 0.677959},
        {0.668731, 0.749077}, {0.709000, 0.697000}, {0.330289, 0.770158},
        {0.404000, 0.788568}, {0.477000, 0.796556}, {0.547507, 0.795073},
        {0.612492, 0.780688}};
    Two_indices = {
        0,  1,  7,  6,  0,  7,  1,  2,  7,  2,  3,  8,  7,  2,  8,  3,  4,  9,
        8,  3,  9,  4,  5,  10, 9,  4,  10, 10, 5,  11, 6,  7,  12, 7,  8,  13,
        12, 7,  13, 13, 8,  14, 14, 8,  15, 14, 15, 17, 15, 16, 17, 17, 16, 18,
        18, 16, 19, 18, 19, 22, 21, 18, 22, 19, 20, 22, 22, 20, 23, 21, 22, 26,
        26, 22, 27, 22, 23, 28, 27, 22, 28, 24, 25, 29, 26, 27, 32, 27, 28, 34,
        33, 27, 34, 29, 25, 35, 25, 30, 35, 30, 31, 36, 35, 30, 36, 36, 31, 37,
        31, 32, 38, 37, 31, 38, 32, 27, 39, 38, 32, 39, 27, 33, 39};

    // D
    D_vertices = {
        {0.253417, 0.229484}, {0.362504, 0.204991}, {0.475495, 0.206523},
        {0.582000, 0.220608}, {0.674506, 0.261301}, {0.239000, 0.352000},
        {0.400861, 0.345985}, {0.496000, 0.326396}, {0.593131, 0.380990},
        {0.745368, 0.333000}, {0.779205, 0.410000}, {0.239000, 0.473000},
        {0.391000, 0.461000}, {0.630852, 0.486504}, {0.695089, 0.441936},
        {0.789579, 0.487000}, {0.239000, 0.587000}, {0.391000, 0.567000},
        {0.606772, 0.594356}, {0.757584, 0.640493}, {0.783844, 0.563506},
        {0.239000, 0.690000}, {0.410283, 0.661512}, {0.514505, 0.663234},
        {0.626494, 0.759481}, {0.701391, 0.711942}, {0.260413, 0.778677},
        {0.360000, 0.789000}, {0.454000, 0.788511}, {0.542506, 0.781541}};
    D_indices = {0,  1,  6,  5,  0,  6,  1,  2,  6,  2,  3,  7,  6,  2,  7,  3,
                 4,  8,  7,  3,  8,  8,  4,  9,  5,  6,  12, 11, 5,  12, 8,  9,
                 14, 9,  10, 14, 13, 8,  14, 14, 10, 15, 11, 12, 17, 16, 11, 17,
                 13, 14, 20, 14, 15, 20, 18, 13, 20, 19, 18, 20, 16, 17, 22, 21,
                 16, 22, 23, 18, 24, 18, 19, 25, 24, 18, 25, 21, 22, 27, 26, 21,
                 27, 22, 23, 28, 27, 22, 28, 23, 24, 29, 28, 23, 29};

    // O
    O_vertices = {
        {0.285931, 0.286092}, {0.378000, 0.221588}, {0.483000, 0.198444},
        {0.589000, 0.204038}, {0.685000, 0.240136}, {0.231440, 0.379000},
        {0.398374, 0.379794}, {0.499000, 0.313258}, {0.609523, 0.355394},
        {0.761407, 0.312494}, {0.800272, 0.397000}, {0.213340, 0.482000},
        {0.370679, 0.489000}, {0.653599, 0.467000}, {0.717477, 0.456992},
        {0.812609, 0.487000}, {0.221400, 0.585000}, {0.386804, 0.598505},
        {0.643982, 0.584000}, {0.772629, 0.669506}, {0.805782, 0.579000},
        {0.258302, 0.678000}, {0.329494, 0.752759}, {0.467056, 0.682111},
        {0.572194, 0.676621}, {0.699000, 0.750455}, {0.414000, 0.791045},
        {0.508000, 0.802714}, {0.606000, 0.792307}};
    O_indices = {0,  1,  6,  5,  0,  6,  1,  2,  7,  6,  1,  7,  2,  3,  7,
                 3,  4,  8,  7,  3,  8,  8,  4,  9,  5,  6,  12, 11, 5,  12,
                 8,  9,  14, 13, 8,  14, 9,  10, 14, 14, 10, 15, 11, 12, 16,
                 16, 12, 17, 13, 14, 18, 14, 15, 20, 18, 14, 20, 19, 18, 20,
                 16, 17, 21, 21, 17, 22, 22, 17, 23, 18, 19, 25, 24, 18, 25,
                 22, 23, 26, 23, 24, 27, 26, 23, 27, 24, 25, 28, 27, 24, 28};

    float scale = 0.1f;
    for (auto &v : vertices) {
      v = sub(v, vec2(0.5f, 0.5f));
      v = mul(v, scale);
    }
    for (auto &v : T_vertices) {
      v = sub(v, vec2(0.5f, 0.5f));
      v = mul(v, scale);
    }
    for (auto &v : A_vertices) {
      v = sub(v, vec2(0.5f, 0.5f));
      v = mul(v, scale);
    }
    for (auto &v : I_vertices) {
      v = sub(v, vec2(0.5f, 0.5f));
      v = mul(v, scale);
    }
    for (auto &v : C_vertices) {
      v = sub(v, vec2(0.5f, 0.5f));
      v = mul(v, scale);
    }
    for (auto &v : H_vertices) {
      v = sub(v, vec2(0.5f, 0.5f));
      v = mul(v, scale);
    }
    for (auto &v : S_vertices) {
      v = sub(v, vec2(0.5f, 0.5f));
      v = mul(v, scale);
    }
    for (auto &v : O_vertices) {
      v = sub(v, vec2(0.5f, 0.5f));
      v = mul(v, scale);
    }
    for (auto &v : F_vertices) {
      v = sub(v, vec2(0.5f, 0.5f));
      v = mul(v, scale);
    }
    for (auto &v : Two_vertices) {
      v = sub(v, vec2(0.5f, 0.5f));
      v = mul(v, scale);
    }
    for (auto &v : D_vertices) {
      v = sub(v, vec2(0.5f, 0.5f));
      v = mul(v, scale);
    }

    S2Material material =
        make_material(S2_MATERIAL_TYPE_ELASTIC, 1000.0, 0.3, 0.2);
    S2Kinematics kinematics = make_kinematics(
        vec2(0.5, 0.5), 0.0, vec2(0.0, 0.0), 0.0, S2_MOBILITY_DYNAMIC);

    kinematics.center = vec2(0.1f, 0.7f);
    // T
    create_mesh_body_from_vector(world, material, kinematics, T_vertices,
                                 T_indices);
    kinematics.center = vec2(0.25f, 0.7f);
    // A
    create_mesh_body_from_vector(world, material, kinematics, A_vertices,
                                 A_indices);
    kinematics.center = vec2(0.4f, 0.7f);
    // I
    create_mesh_body_from_vector(world, material, kinematics, I_vertices,
                                 I_indices);
    kinematics.center = vec2(0.55f, 0.7f);
    // C
    create_mesh_body_from_vector(world, material, kinematics, C_vertices,
                                 C_indices);
    kinematics.center = vec2(0.7f, 0.7f);
    // H
    create_mesh_body_from_vector(world, material, kinematics, H_vertices,
                                 H_indices);
    kinematics.center = vec2(0.85f, 0.7f);
    // I
    create_mesh_body_from_vector(world, material, kinematics, I_vertices,
                                 I_indices);

    kinematics.center = vec2(0.1f, 0.5f);
    // S
    create_mesh_body_from_vector(world, material, kinematics, S_vertices,
                                 S_indices);
    kinematics.center = vec2(0.25f, 0.5f);
    // O
    create_mesh_body_from_vector(world, material, kinematics, O_vertices,
                                 O_indices);
    kinematics.center = vec2(0.4f, 0.5f);
    // F
    create_mesh_body_from_vector(world, material, kinematics, F_vertices,
                                 F_indices);
    kinematics.center = vec2(0.55f, 0.5f);
    // R
    create_mesh_body_from_vector(world, material, kinematics, T_vertices,
                                 T_indices);
    kinematics.center = vec2(0.7f, 0.5f);
    // 2
    create_mesh_body_from_vector(world, material, kinematics, Two_vertices,
                                 Two_indices);
    kinematics.center = vec2(0.85f, 0.5f);
    // D
    create_mesh_body_from_vector(world, material, kinematics, D_vertices,
                                 D_indices);

    // Add the boundary
    // bottom
    create_collider(world, make_kinematics({0.5f, 0.0f}),
                    make_box_shape(vec2(0.5f, 0.01f)));
    // top
    create_collider(world, make_kinematics({0.5f, 1.0f}),
                    make_box_shape(vec2(0.5f, 0.01f)));
    // left
    create_collider(world, make_kinematics({0.0f, 0.5f}),
                    make_box_shape(vec2(0.01f, 0.5f)));
    // right
    create_collider(world, make_kinematics({1.0f, 0.5f}),
                    make_box_shape(vec2(0.01f, 0.5f)));
    // Soft2D initialization ends

    // Renderer initialization begins
    Renderer &renderer = F.renderer();
    renderer.set_framebuffer_size(win_width, win_height);

    x_ = runtime.allocate_vertex_buffer(config.max_allowed_particle_num, 2);
    collider_texture_ = runtime.allocate_texture2d(
        config.grid_resolution * config.fine_grid_scale,
        config.grid_resolution * config.fine_grid_scale, TI_FORMAT_R32F,
        TI_NULL_HANDLE);
    element_indices_ =
        runtime.allocate_index_buffer(config.max_allowed_element_num * 3, 1);

    draw_points = runtime.draw_points(x_)
                      .point_size(3.0f)
                      .color(glm::vec3(1, 0.5, 0))
                      .build();
    draw_collider_texture = runtime.draw_texture(collider_texture_)
                                .is_single_channel()
                                .color(glm::vec3(0.2, 0.8, 0.0))
                                .build();

    // Set up orthogonal view
    glm::mat4 model2world = glm::mat4(1.0f);
    glm::mat4 camera2view =
        glm::ortho(1.0, -1.0, 1.0 * win_fov, -1.0 * win_fov, -1000.0, 1000.0);
    glm::mat4 world2camera = glm::lookAt(
        glm::vec3(0.0, 0.0, 10), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -1, 0));
    glm::mat4 world2view = camera2view * world2camera;

    auto offset = s2_get_world_config(world).offset;
    auto extent = s2_get_world_config(world).extent;
    draw_mesh =
        runtime
            .draw_mesh(x_, element_indices_, glm::vec2(offset.x, offset.y),
                       glm::vec2(extent.x, extent.y))
            .model2world(model2world)
            .world2view(world2view)
            .color(glm::vec3(1.0, 0.5, 0.0))
            .polygon_mode(VK_POLYGON_MODE_LINE)
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

    // Export collider buffer to texture
    auto texture = collider_texture_.texture();
    s2_export_buffer_to_texture(world, S2_BUFFER_NAME_FINE_GRID_COLLIDER_NUM,
                                true, 0.8f, &texture);

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

    ++frame;
    return true;
  }
  virtual void render() override final {
    Renderer &renderer = F.renderer();
    renderer.enqueue_graphics_task(*draw_mesh);
    renderer.enqueue_graphics_task(*draw_points);
    renderer.enqueue_graphics_task(*draw_collider_texture);
  }
};

std::unique_ptr<App> create_app() {
  return std::unique_ptr<App>(new MeshTaichi);
}
