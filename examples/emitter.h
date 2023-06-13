// #include "common.h"
#include <unordered_map>
#include <vector>

struct Emitter {
  S2World world;
  S2Material material;
  S2Kinematics kinematics;
  S2Shape shape;
  uint32_t tag;

  // -1 means infinite lifetime
  int lifetime{-1};
  int frame_begin_emit{0};
  int frame_end_emit{1000};
  int frequency{50};
  std::unordered_map<S2Body, int> lifetimes_;
  Emitter(){};
  Emitter(S2World world, S2Material material, S2Kinematics kinematics,
          S2Shape shape, uint32_t tag = 0)
      : world(world), material(material), kinematics(kinematics), shape(shape),
        tag(tag) {}

  void Emit() {
    S2Body body = make_body(world, material, kinematics, shape, tag);
    lifetimes_[body] = lifetime;
  }

  void SetEmittingBeginFrame(int frame) { frame_begin_emit = frame; }

  void SetEmittingEndFrame(int frame) { frame_end_emit = frame; }

  void SetLifetime(int lifetime) { this->lifetime = lifetime; }

  void SetFrequency(int frequency) { this->frequency = frequency; }

  void Update(int frame) {
    if (frame_begin_emit <= frame && frame < frame_end_emit) {
      if ((frame - frame_begin_emit) % frequency == 0) {
        Emit();
      }
    }
    std::vector<S2Body> bodies_to_be_removed;
    for (auto it = lifetimes_.begin(); it != lifetimes_.end();) {
      --it->second;
      if (it->second == 0) {
        auto &handle = it->first;
        bodies_to_be_removed.push_back(handle);
        it = lifetimes_.erase(it);
      } else {
        ++it;
      }
    }
    for (auto &body : bodies_to_be_removed) {
      s2_destroy_body(body);
    }
  }
};
