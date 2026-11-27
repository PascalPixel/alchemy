#ifndef ALCHEMY_FIELD_FIELD_PSYNERGY_H
#define ALCHEMY_FIELD_FIELD_PSYNERGY_H

// The states of the field Psynergy's effects: the scatter, Whirlwind's orb, the Arrow's bow, and
// Avoid's falling arcs.

#include "types.h"
#include "field/effect_sprite.h"
#include "field/field_particle.h"
#include "field/field_psynergy_state.h"
#include "field/sprite.h"

struct ACTOR;
struct SPR_ANIMATION;

// The particles of a scatter, a power of two: the next one wraps by a mask.
#define SCATTER_PARTICLE_COUNT 64

// The scatter's state (heap slot HEAP_SLOT_BATTLE_EFFECT): its tiles, 64 sprites and particles, the
// actor it scatters from, and its stage.
typedef struct SCATTER_STATE
{
	Uint8                   tiles[4096];
	EFFECT_SPR              sprites[SCATTER_PARTICLE_COUNT];
	FIELD_PSYNERGY_PARTICLE particles[SCATTER_PARTICLE_COUNT];
	struct ACTOR           *actor;
	Sint16                  phase;
	Sint16                  timer;
	Sint16                  spawnCount;
	Sint16                  next;
	Uint16                  tileA;
	Uint16                  tileB;
	Sint16                  flash;
	Sint16                  hit;
} SCATTER_STATE;

// What Whirlwind's orb does once it reaches its target (fdPsynergyWhirlwind).
typedef enum ORB_KIND
{
	ORB_PLAIN,    // the scene is enabled: the orb only comes and goes
	ORB_ORBITING, // leaves circle the orb while the target shrinks
	// sprouting leaves: leaves wander off, the target is handed on
	ORB_WANDER,
	ORB_HANDOFF, // a seedling: the target is handed on
} ORB_KIND;

// One moving point of the Arrow: a spark on the canvas (x and y only, counting delay) or a sprite's
// place in the world.
typedef struct BOW_MOTION
{
	FIXED  x;         // 16.16 fixed-point pixels
	FIXED  y;
	FIXED  z;
	FIXED  velocityX; // 16.16 fixed-point pixels a tick
	FIXED  velocityY;
	FIXED  velocityZ;
	Sint32 delay;
} BOW_MOTION;

// Tiles in one of the bow's drawing poses, which follow each other from BOW_STATE.bowTile.
#define BOW_POSE_TILES  32

// The glints the Arrow throws off the bow while it draws.
#define BOW_SPARK_COUNT 32

// The bow's six drawing poses, 32 by 32 pixels in 256 colours.
#define BOW_TILE_BYTES  (6 * 32 * 32)

// BOW_STATE.frame while the bow holds its pose and is not drawing.
#define BOW_TICK_IDLE   999

// The Arrow's state (fdPsynergyArrow, in heap slot HEAP_SLOT_BATTLE_EFFECT): its 32 sparks, the
// arrow and the bow with their motions, the points the arrow flies between, and the phase of its
// event.
typedef struct BOW_STATE
{
	Uint8     *tiles;
	BOW_MOTION sparks[BOW_SPARK_COUNT];
	BOW_MOTION arrowMotion;
	EFFECT_SPR arrow;
	BOW_MOTION bowMotion;
	EFFECT_SPR bow;
	Sint32     start[3];
	Sint32     target[3];
	Sint16     phase;
	Sint16     timer;
	Uint16     bowTile;
	Sint16     frame;
	Sint8      bowGrowing;
	Sint8      bowShrinking;
	Sint8      sparking;
	Sint8      sparksRepeat;
	Sint16     result;
	Sint8      done;
	Sint8      arrowMoving;
} BOW_STATE;

// FIELD_STATE as fdPsynergyGatherNearbyMetatiles writes it: the metatiles a Psynergy touches and
// the Cyclone's two spots, here unsigned.
typedef struct POINT_STATE
{
	Uint8  unused[3264]; // no code names it
	Sint16 points[62];
	Uint16 cycloneCarryX;
	Uint16 cycloneCarryZ;
	Uint16 cycloneLiftX;
	Uint16 cycloneLiftZ;
} POINT_STATE;

// A falling arc's sprite (SPR), seen as the image attributes, VRAM cache entry and first layer that
// Avoid's arcs set on it, under struct Sprite's names; its image bit fields are read here as bytes.
typedef struct DESCENDING_ARC_SPR
{
	Uint8 link[4];
	Uint8 y;
	Uint8 affine     : 2;
	Uint8 objectMode : 2;
	Uint8 mosaic     : 1;
	Uint8 fullColor  : 1; // 0 for 16 colours
	Uint8 shape      : 2; // 1 wide
	Uint8 xLow;
	Uint8 xHigh       : 1;
	Uint8 affineIndex : 5;
	Uint8 size        : 2;

	union
	{
		struct
		{
			Uint16 tile     : 10; // the first sprite tile in VRAM
			Uint16 priority : 2;
			Uint16 palette  : 4;
		} half;

		struct
		{
			Uint8 tileLow;
			Uint8 tileHigh : 2;
			Uint8 priority : 2;
			Uint8 palette  : 4;
		} byte;
	} attribute;

	FIXED              scale;                // 16.16 fixed point, toFIXED(1) full size
	Uint8              slot;                 // the VRAM cache entry holding its tiles
	Uint8              retainCacheEntry : 1; // the borrowed tiles stay loaded
	Uint8              useOwnScale      : 1;
	Uint8              unusedFlags      : 6;
	Uint16             rotation;
	Uint8              width;
	Uint8              height;
	Sint8              offsetX;
	Sint8              offsetY;
	Uint8              lastNo;
	Uint8              dirty;
	Uint8              flags;      // 0 drops the shadow
	Uint8              count;
	ANIMATION_SPR_PART shadow;
	SPR_ANIMATION     *firstEntry; // entries[0], its first layer
} DESCENDING_ARC_SPR;

// The arc sparks' tiles.
extern const Uint8  FieldEffectArcSparkTiles[];

void                fdActorWaitForTrackEndStub(struct ACTOR *actor);

extern const Sint32 ArrowWobbleAngles[8];
extern const Sint32 ArrowWobbleFades[4];

// Three 8 by 8 glints the Arrow draws into its canvas, a byte a pixel (arrow_glint.s).
extern const Uint8  ArrowGlintPixels[];

// The metatiles around the caster Cyclone looks at.
#define FIELD_PSYNERGY_NEARBY_METATILES 21

// The tile offsets of those metatiles from the caster, signed x, z pairs.
extern const Sint8 FieldPsynergyNearbyMetatileOffsets[FIELD_PSYNERGY_NEARBY_METATILES * 2];

#endif
