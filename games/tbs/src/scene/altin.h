#ifndef ALCHEMY_ALTIN_H
#define ALCHEMY_ALTIN_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

// A field actor as Altin's water effects use it: the water beast hopping in
// the flood and the spray drifting off its spout reuse the actor's own
// fields after the motion state.
struct WaterActor {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  s32 ground; // the terrain height under the actor
  s32 scaleX;
  s32 scaleY;
  u8 unk20[8];
  s32 velocityY; // upward speed of a hop, zero on the ground
  u8 unk2c[47];
  u8 onGround;
  u8 unk5c[8];
  s16 timer;
  s16 shakeFrames; // frames until a landing's screen shake is calmed
  s32 counter;     // the beast: 1 while airborne; a spray: frames to live
  void (*update)(struct WaterActor *);
};

// The few fields of a spray particle set up when it spawns.
struct SprayActor {
  u8 head[18];
  u8 gap18[6];
  s32 scaleX;
  s32 scaleY;
  u8 gap32[48];
  struct FieldSpritePriority *sprite;
  u8 gap84;
  u8 motionFlags;
};

#endif
