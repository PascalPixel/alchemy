#ifndef ALCHEMY_FIELD_FIELD_PSYNERGY_H
#define ALCHEMY_FIELD_FIELD_PSYNERGY_H

/// @file
/// What games/tla/src/field/field_psynergy.c declares for the files that
/// use it, written once.

#include "types.h"
#include "field/effect_sprite.h"
#include "field/field_particle.h"
#include "field/field_psynergy_work.h"

struct Actor;
struct SpriteAnimation;
struct ArcEffectObject__launch;

struct ArcEntry {
  u8 unused[5]; ///< no code names it
  u8 param;
};

struct ArcGroup {
  u8 unused[25]; ///< no code names it
  u8 dirty;
  u8 flags;
  u8 unk1b[13];
  struct ArcEntry *entries[1];
};

struct ArcScene {
  u8 unused[0x10]; ///< no code names it
  struct ArcEffectObject__launch *mainActor;
  u8 unk14[0x6fc];
  s16 arcTileSlot;
};

/// The scatter's work (heap slot HEAP_SLOT_BATTLE_EFFECT): its tiles, 64
/// sprites and particles, the actor it scatters from, and its stage.
struct ScatterWork {
  u8 tiles[0x1000];
  struct EffectSprite sprites[64];
  struct FieldPsynergyParticle particles[64];
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

struct LaunchVector {
  s32 x;
  s32 y;
  s32 z;
};

struct LaunchObject {
  u8 unused[8]; ///< no code names it
  s32 x;
  s32 y;
  s32 z;
};

struct LaunchScene {
  u8 unused[0x10]; ///< no code names it
  struct LaunchObject *mainActor;
};

/// A composite sprite, as far as this reads it.
struct OrbTargetSprite {
  u8 unused[0x28]; ///< no code names it
  struct SpriteAnimation *entries[4];
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

struct PointWork {
  u8 unused[0xcc0]; ///< no code names it
  s16 points[62];
  u16 unk_d3c;
  u16 unk_d3e;
  u16 unk_d40;
  u16 unk_d42;
};

struct TouchOwner {
  u8 unused[34]; ///< no code names it
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
  u8 unused[0x64]; ///< no code names it
  s16 age;
  u8 unusedAfterAge[6]; ///< no code names it
  void *update;
};

/// The effect work, its slots 8 bytes earlier than FieldEffectWork has them.
struct FallbackScene {
  u8 unused[0x50]; ///< no code names it
  struct FieldParticle slots[16];
};

struct FxPosition13 {
  s32 x;
  s32 y;
  s32 z;
};

struct SpinObject {
  u8 unused[8]; ///< no code names it
  s32 x;
  s32 y;
  s32 z;
  s32 floor;
  s32 fall;
  u8 unusedAfterFall[0x1c]; ///< no code names it
  s32 targetX;
  u8 unusedAfterTargetX[0x19]; ///< no code names it
  u8 motionFlags;
  u8 unusedAfterMotionFlags[0xe]; ///< no code names it
  s16 timer;
  u8 unusedAfterTimer[6]; ///< no code names it
  void (*update)(struct SpinObject *);
};

struct SpinVector {
  s32 x;
  s32 y;
  s32 z;
};

struct ArcPicture {
  u8 unused[22]; ///< no code names them
  u8 visible;
};

struct ArcSprite {
  u8 unused[5]; ///< no code names them
  u8 flags05_0 : 5;
  u8 flags05_5 : 1;
  u8 layer : 2;
  u8 unusedAfterLayer; ///< no code names it
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
  u8 unusedAfterAttr[4]; ///< no code names them
  u8 resource;
  u8 active : 1;
  u8 flags11_1 : 7;
  u8 unusedB[8]; ///< no code names them
  u8 animation;
  u8 unusedAfterAnimation[13]; ///< no code names them
  struct ArcPicture *picture;
};

struct ArcObject {
  u8 unused[8]; ///< no code names them
  s32 x;
  s32 y;
  s32 z;
  s32 unk14;
  s32 scaleX;
  s32 scaleY;
  u8 unusedAfterScaleY[0x30]; ///< no code names them
  struct ArcSprite *sprite;
  u8 unusedAfterSprite; ///< no code names it
  u8 unk55;
  u8 unusedB[0x0e]; ///< no code names them
  u16 unk64;
  u8 unusedC[2]; ///< no code names them
  struct ArcObject *source;
  void *callback;
};

extern const u8 gFieldEffect_ArcSparkTiles[];

void Actor_WaitForTrackEndFar(struct Actor *object);

extern const s32 Data_080f0f34[8];
extern const s32 Data_080f0f54[4];

extern const u8 Data_080eda0c[];

extern const s8 gFieldPsynergy_NearbyCellOffsets[];

#endif
