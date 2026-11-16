#ifndef ALCHEMY_FIELD_FIELD_PSYNERGY_H
#define ALCHEMY_FIELD_FIELD_PSYNERGY_H

/// @file
/// What games/tla/src/field/field_psynergy.c declares for the files that
/// use it, written once.

#include "types.h"
#include "field/effect_sprite.h"
#include "field/field_particle.h"
#include "field/field_psynergy_work.h"
#include "field/sprite.h"

struct Actor;
struct SpriteAnimation;

/// The scatter's work (heap slot HEAP_SLOT_BATTLE_EFFECT): its tiles, 64
/// sprites and particles, the actor it scatters from, and its stage.
struct ScatterWork {
  u8 tiles[4096];
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

enum OrbKind {
  ORB_PLAIN,    // the scene is enabled: the orb only comes and goes
  ORB_ORBITING, // motes circle the orb while the target shrinks
  ORB_WANDER,   // target 372: motes wander off, the target is handed on
  ORB_HANDOFF,  // target 373: the target is handed on
};

/// One moving point of the Arrow: a spark on the canvas (x and y only, counting
/// delay) or a sprite's place in the world.
struct BowMotion {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 delay;
};

/// The Arrow's work (FieldPsynergy_Arrow, in heap slot
/// HEAP_SLOT_BATTLE_EFFECT): its 32 sparks, the arrow and the bow with their
/// motions, the points the arrow flies between, and the phase of its task.
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

/// FieldWork as FieldPsynergy_GatherNearbyCells writes it: the cells a Psynergy
/// touches and the Cyclone's two spots, here unsigned.
struct PointWork {
  u8 unused[3264]; ///< no code names it
  s16 points[62];
  u16 cycloneCarryX;
  u16 cycloneCarryZ;
  u16 cycloneLiftX;
  u16 cycloneLiftZ;
};

/// A falling arc's sprite (struct Sprite), seen as the image attributes, VRAM
/// cache entry and first layer that Avoid's arcs set on it, under struct
/// Sprite's names; its image bit fields are read here as bytes.
struct DescendingArcSprite {
  u8 link[4];
  u8 y;
  u8 affine : 2;
  u8 objectMode : 2;
  u8 mosaic : 1;
  u8 fullColor : 1; ///< 0 for 16 colours
  u8 shape : 2;     ///< 1 wide
  u8 xLow;
  u8 xHigh : 1;
  u8 affineIndex : 5;
  u8 size : 2;
  union {
    struct {
      u16 tile : 10; ///< the first sprite tile in VRAM
      u16 priority : 2;
      u16 palette : 4;
    } half;
    struct {
      u8 tileLow;
      u8 tileHigh : 2;
      u8 priority : 2;
      u8 palette : 4;
    } byte;
  } attr;
  s32 scale;
  u8 slot;                 ///< the VRAM cache entry holding its tiles
  u8 retainCacheEntry : 1; ///< the borrowed tiles stay loaded
  u8 useOwnScale : 1;
  u8 unusedFlags : 6;
  u16 rotation;
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 lastNo;
  u8 dirty;
  u8 flags; ///< 0 drops the shadow
  u8 count;
  struct AnimationSpritePart shadow;
  struct SpriteAnimation *firstEntry; ///< entries[0], its first layer
};

/// The arc sparks' tiles, a copy of The Broken Seal's
/// gBattleEffect_ArcSparkTiles (field_effects.s).
extern const u8 gFieldEffect_ArcSparkTiles[];

void Actor_WaitForTrackEndFar(struct Actor *object);

extern const s32 gArrow_WobbleAngles[8];
extern const s32 gArrow_WobbleFades[4];

/// Three 8 by 8 glints the Arrow draws into its canvas, a byte a pixel
/// (arrow_glint.s).
extern const u8 gArrow_GlintPixels[];

extern const s8 gFieldPsynergy_NearbyCellOffsets[];

#endif
