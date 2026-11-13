#ifndef ALCHEMY_MODULE_FIELD_RADIAL_H
#define ALCHEMY_MODULE_FIELD_RADIAL_H

/// @file
/// What games/tla/src/field/event_commands/radial.c declares for the files that
/// use it, written once.

#include "types.h"
#include "effect_sprite.h"
#include "field/field_particle.h"

struct Actor;
struct AnimationEntry;
struct ArcEffectObject__launch;

struct ArcEntry {
  u8 unk00[5];
  u8 param;
};

struct ArcGroup {
  u8 unk00[25];
  u8 dirty;
  u8 flags;
  u8 unk1b[13];
  struct ArcEntry *entries[1];
};

struct ArcScene {
  u8 unk00[0x10];
  struct ArcEffectObject__launch *mainObject;
  u8 unk14[0x6fc];
  s16 arcTileSlot;
};

struct ScatterParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

struct ScatterWork {
  u8 tiles[0x1000];
  struct EffectSprite sprites[64];
  struct ScatterParticle particles[64];
  struct Actor *object;
  s16 phase;
  s16 timer;
  s16 spawnCount;
  s16 next;
  u16 tileA;
  u16 tileB;
  s16 flash;
  s16 hit;
};

struct ScatterEvent {
  u8 unk000[0xd31];
  s8 target;
  s8 targetY;
  s8 targetZ;
};

struct EffectSceneState {
  u8 unk000[0xd31];
  u8 valueD31;
  u8 valueD32;
  u8 valueD33;
};

struct LaunchVector {
  s32 x;
  s32 y;
  s32 z;
};

struct LaunchObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

struct LaunchScene {
  u8 unk00[0x10];
  struct LaunchObject *mainObject;
};

// ⚓️'s composite sprite, as far as this reads it.
struct OrbTargetSprite {
  u8 unk00[0x28];
  struct AnimationEntry *entries[4];
};

enum OrbKind {
  ORB_PLAIN,    // the scene is enabled: the orb only comes and goes
  ORB_ORBITING, // motes circle the orb while the target shrinks
  ORB_WANDER,   // target 372: motes wander off, the target is handed on
  ORB_HANDOFF,  // target 373: the target is handed on
};

// One moving point: a spark on the canvas (x and y only, counting delay) or
// a sprite's place in the world.
struct BowMotion {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 delay;
};

struct BowWork {
  u8 *tiles;
  struct BowMotion sparks[32];
  struct BowMotion arrowMotion;
  struct EffectSprite arrow;
  struct BowMotion bowMotion;
  struct EffectSprite bow;
  s32 start[3];
  s32 target[3];
  s16 phase;
  s16 timer;
  u16 bowTile;
  s16 frame;
  s8 bowGrowing;
  s8 bowShrinking;
  s8 sparking;
  s8 sparksRepeat;
  s16 result;
  s8 done;
  s8 arrowMoving;
};

struct BowScene {
  u16 angleFraction;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
};

struct BowEvent {
  u8 unk00[20];
  struct Actor *objects[80];
};

struct PointWork {
  u8 unk_0[0xcc0];
  s16 points[62];
  u16 unk_d3c;
  u16 unk_d3e;
  u16 unk_d40;
  u16 unk_d42;
};

struct TouchOwner {
  u8 unk00[34];
  u8 layer;
};

struct ArcWork {
  struct EffectSprite sprites[2][2];
  s16 age[2];
  s16 resource;
  s16 group;
  struct Actor *target;
};

struct TimedObject {
  u8 unk00[0x64];
  s16 age;
  u8 unk66[6];
  void *update;
};

// ⚓️'s effect work holds its slots 8 bytes earlier than ☀️'s.
struct FallbackScene {
  u8 unk00[0x50];
  struct FieldParticle slots[16];
};

struct FxScene13 {
  u16 angle;
  u8 unk02[2];
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
  struct Actor *child;
  u8 unk18[8];
  s8 enabled;
};

struct FxPosition13 {
  s32 x;
  s32 y;
  s32 z;
};

struct SpinObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  s32 floor;
  s32 fall;
  u8 unk1c[0x1c];
  s32 targetX;
  u8 unk3c[0x19];
  u8 motionFlags;
  u8 unk56[0xe];
  s16 timer;
  u8 unk66[6];
  void (*update)(struct SpinObject *);
};

struct SpinVector {
  s32 x;
  s32 y;
  s32 z;
};

struct ArcPicture {
  u8 padding00[22];
  u8 visible;
};

struct ArcSprite {
  u8 padding00[5];
  u8 flags05_0 : 5;
  u8 flags05_5 : 1;
  u8 layer : 2;
  u8 padding06;
  u8 flags07_0 : 6;
  u8 size : 2;
  union {
    struct {
      u16 tile : 10;
      u16 flags08_10 : 6;
    } half;
    struct {
      u8 tileLow;
      u8 flags09_0 : 2;
      u8 variant : 2;
      u8 flags09_4 : 4;
    } byte;
  } attr;
  u8 padding0c[4];
  u8 resource;
  u8 active : 1;
  u8 flags11_1 : 7;
  u8 padding12[8];
  u8 animation;
  u8 padding1b[13];
  struct ArcPicture *picture;
};

struct ArcObject {
  u8 padding00[8];
  s32 x;
  s32 y;
  s32 z;
  s32 unk14;
  s32 scaleX;
  s32 scaleY;
  u8 padding20[0x30];
  struct ArcSprite *sprite;
  u8 padding54;
  u8 unk55;
  u8 padding56[0x0e];
  u16 unk64;
  u8 padding66[2];
  struct ArcObject *source;
  void *callback;
};

extern const u8 gFieldEffect_ArcSparkTiles[];

void Actor_WaitForTrackEndFar(struct Actor *object);

extern const s32 Data_080f0f34[8];
extern const s32 Data_080f0f54[4];

extern const u8 Data_080eda0c[];

extern const s8 Data_080f0f64[];

#endif
