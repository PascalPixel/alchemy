#ifndef ALCHEMY_FIELD_PSYNERGY_INTERNAL_H
#define ALCHEMY_FIELD_PSYNERGY_INTERNAL_H

// What every part of the field_psynergy object shares: its includes. field_psynergy.c includes it
// before the parts, and each part includes it too.

// world_map.h first: battle.h names WORLD_MAP_STATE, and a struct declared before its definition
// keeps a smaller alignment, which field_psynergy_map_screen.c's copies of it show.
#include "field/world_map.h"
#include "window/message_argument.h"
#include "field/perspective.h"
#include "system.h"
#include "game/main_routine_state.h"
#include "system/field_effect.h"
#include "field/objdisp.h"
#include "fixed_math.h"
#include "types.h"
#include "battle/motobj.h"
#include "field/scene_tables.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/field.h"
#include "game/game.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "field/actor.h"
#include "field/field_psynergy_state.h"
#include "field/sprite.h"
#include "field/field_script.h"
#include "field/field_particle.h"
#include "field/actor_view.h"
#include "field/effect_sprite.h"
#include "graphics/resource.h"
#include "scene_ids.h"
#include "graphics/vram_tab.h"
#include "field/arc_effect.h"
#include "field/canvas_sprite.h"
#include "field/map_grid.h"
#include "field/field_mode.h"

// The shape of a free entry among the map's shape entries (fdMapShapeFindOrAdd).
#define MAP_SHAPE_FREE 255

void fdEffectAdvanceMotion(FIELD_OBJECT *object);

// A spark that circles a centre: the centre in 16.16 fixed-point pixels, the angle and radius it is
// drawn at, and its age in ticks (-1 while unused).
typedef struct WHIRL_SPARK
{
	FIXED  x;
	FIXED  y;
	FIXED  z;
	Sint32 angle;            // 1/65536 turns
	Sint32 unusedAfterAngle; // no code names it
	FIXED  radius;           // 16.16 fixed-point pixels
	Sint32 age;
} WHIRL_SPARK;

// The states of a Retreat particle (fdRetreatUpdateSpark).
typedef enum RETREAT_PARTICLE_STATE
{
	RETREAT_PARTICLE_AIM         = 0, // placed on the main actor and aimed
	RETREAT_PARTICLE_FLYING      = 1, // back to RETREAT_PARTICLE_AIM on arrival
	// Set by fdPsynergyRetreat: freed on arrival.
	RETREAT_PARTICLE_LAST_FLIGHT = 2
} RETREAT_PARTICLE_STATE;

// The view's sway (fdViewMoveSwing) as fdPsynergyRun casts Tremor and Grind at no object, in 16.16
// pixels: it grows from SWAY_START to SWAY_PEAK by SWAY_RAMP a tick, holds for SWAY_HOLD_TICKS, and
// shrinks back to SWAY_START. Tremor's stronger sway at an object starts and ends there too.
#define SWAY_START      toFIXED(2)
#define SWAY_PEAK       toFIXED(6)
#define SWAY_RAMP       (toFIXED(3) / 20) // 0.15 pixels a tick
#define SWAY_HOLD_TICKS 10

// Cyclone's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the particle burst (PARTICLE_BURST_STATE) at
// its start, then the leader's ride on the whirlwind.
typedef struct CYCLONE_STATE
{
	Uint8  particleBurst[6920];
	Sint16 count;
	Uint8  unusedAfterCount[2]; // no code names it
	ACTOR *follower;            // the actor carried with the leader
	Sint16 state;               // the stage of the ride; 999 once it ends
	Sint16 timer;               // ticks into the ride
	Sint32 angle;               // the whirlwind's turn, 1/65536 turns
	Sint32 radius;
	FIXED  from[3];             // where the ride starts, 16.16 pixels
	FIXED  to[3];               // where it ends, 16.16 pixels
	// Nonzero runs the particle burst with it: a nearby metatile of kind 1
	// (fdPsynergyGatherNearbyMetatiles).
	Sint8  kind;
} CYCLONE_STATE;

// The cycling glow of the Parch disc and the Ring: red stays put while green and blue swing either
// side of their middles, each colour a step further along the swing.
#define GLOW_RED            28
#define GLOW_GREEN          23
#define GLOW_GREEN_SWING    8
#define GLOW_BLUE           12
#define GLOW_BLUE_SWING     10
// The cycling shine of Burst's and Pound's canvas: one channel or two swing CANVAS_SHINE_SWING
// levels either side of CANVAS_SHINE_MIDDLE.
#define CANVAS_SHINE_MIDDLE 16
#define CANVAS_SHINE_SWING  12

// The map screen's marker tiles (fdMapScreenInit draws them).
typedef enum WORLD_MARKER_TILE
{
	WORLD_MARKER_TILE_PARTY   = 0, // where the party is
	WORLD_MARKER_TILE_SHIP    = 2,
	// the larger dot on the place nearest the cursor
	WORLD_MARKER_TILE_NEAREST = 3
} WORLD_MARKER_TILE;

// A 256-colour picture's size in the 16-colour tiles a sprite counts.
#define OBJ_TILES_PER_8BPP_TILE (TILE_SIZE_8BPP / TILE_SIZE_4BPP)

#endif // ALCHEMY_FIELD_PSYNERGY_INTERNAL_H
