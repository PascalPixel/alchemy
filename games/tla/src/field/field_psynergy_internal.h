#ifndef ALCHEMY_FIELD_PSYNERGY_INTERNAL_H
#define ALCHEMY_FIELD_PSYNERGY_INTERNAL_H

/// @file
/// What every part of the field_psynergy object shares: its includes.
/// field_psynergy.c includes it before the parts, and each part includes it
/// too.

// world_map.h first: battle.h names struct WorldMapState, and a struct declared
// before its definition keeps a smaller alignment, which
// field_psynergy_map_screen.c's copies of it show.
#include "field/world_map.h"
#include "window/message_argument.h"
#include "field/perspective.h"
#include "system/iwram_call.h"
#include "system/io_write.h"
#include "field/objdisp.h"
#include "system/system.h"
#include "fixed_math.h"
#include "types.h"
#include "battle/motobj.h"
#include "field/scene_tables.h"
#include "system/field_effect.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "field/actor.h"
#include "system/ram_buffer.h"
#include "field/field_psynergy_state.h"
#include "field/sprite.h"
#include "field/field_event.h"
#include "field/field_particle.h"
#include "field/actor_view.h"
#include "system/object_lookup.h"
#include "system/main_routine_state.h"
#include "field/effect_sprite.h"
#include "system/vector3.h"
#include "graphics/resource.h"
#include "system/task.h"
#include "system/heap_release.h"
#include "system/scene.h"
#include "system/frame.h"
#include "scene_ids.h"
#include "graphics/vram_tab.h"
#include "field/arc_effect.h"
#include "field/canvas_sprite.h"
#include "system/io_reg.h"
#include "field/map_grid.h"
#include "field/field_sequence.h"

/// The shape of a free entry among the map's shape entries
/// (MapShape_FindOrAdd).
#define MAP_SHAPE_FREE 255

void FieldEffect_AdvanceMotion(union FieldObject *object);

/// A spark that circles a centre: the centre in 16.16 fixed-point pixels, the
/// angle and radius it is drawn at, and its age in frames (-1 while unused).
struct WhirlSpark {
  FIXED x;
  FIXED y;
  FIXED z;
  s32 angle;            ///< 1/65536 turns
  s32 unusedAfterAngle; ///< no code names it
  FIXED radius;         ///< 16.16 fixed-point pixels
  s32 age;
};

/// Per-frame update: sets every part of an actor's sprite to colour mode
/// frame % 6, which cycles its colours.
void Actor_CycleSpriteColors(struct Actor *actor);

/// The states of a Retreat particle (Retreat_UpdateSpark).
enum RetreatParticleState {
  RETREAT_PARTICLE_AIM = 0,    ///< placed on the main actor and aimed
  RETREAT_PARTICLE_FLYING = 1, ///< back to RETREAT_PARTICLE_AIM on arrival
  /// Set by FieldPsynergy_Retreat: freed on arrival.
  RETREAT_PARTICLE_LAST_FLIGHT = 2
};

/// The view's sway (ViewMove_Swing) as FieldPsynergy_Run casts Tremor and
/// Grind at no object, in 16.16 pixels: it grows from SWAY_START to SWAY_PEAK
/// by SWAY_RAMP a frame, holds for SWAY_HOLD_FRAMES, and shrinks back to
/// SWAY_START. Tremor's stronger sway at an object starts and ends there too.
#define SWAY_START toFIXED(2)
#define SWAY_PEAK toFIXED(6)
#define SWAY_RAMP (toFIXED(3) / 20) ///< 0.15 pixels a frame
#define SWAY_HOLD_FRAMES 10

/// Cyclone's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the particle burst
/// (ParticleBurstState) at its start, then the leader's ride on the whirlwind.
struct CycloneState {
  u8 particleBurst[6920];
  s16 count;
  u8 unusedAfterCount[2]; ///< no code names it
  struct Actor *follower; ///< the actor carried with the leader
  s16 state;              ///< the stage of the ride; 999 once it ends
  s16 timer;              ///< frames into the ride
  s32 angle;              ///< the whirlwind's turn, 1/65536 turns
  s32 radius;
  FIXED from[3]; ///< where the ride starts, 16.16 pixels
  FIXED to[3];   ///< where it ends, 16.16 pixels
  /// Nonzero runs the particle burst with it: a nearby cell of kind 1
  /// (FieldPsynergy_GatherNearbyCells).
  s8 kind;
};

/// The cycling glow of the Parch disc and the Ring: red stays put while green
/// and blue swing either side of their middles, each colour a step further
/// along the swing.
#define GLOW_RED 28
#define GLOW_GREEN 23
#define GLOW_GREEN_SWING 8
#define GLOW_BLUE 12
#define GLOW_BLUE_SWING 10
/// The cycling shine of Burst's and Pound's canvas: one channel or two swing
/// CANVAS_SHINE_SWING levels either side of CANVAS_SHINE_MIDDLE.
#define CANVAS_SHINE_MIDDLE 16
#define CANVAS_SHINE_SWING 12

/// The map screen's marker tiles (MapScreen_Initialize draws them).
enum WorldMarkerTile {
  WORLD_MARKER_TILE_PARTY = 0, ///< where the party is
  WORLD_MARKER_TILE_SHIP = 2,
  WORLD_MARKER_TILE_NEAREST =
      3 ///< the larger dot on the place nearest the cursor
};

/// A 256-colour picture's size in the 16-colour tiles a sprite counts.
#define OBJ_TILES_PER_8BPP_TILE (TILE_SIZE_8BPP / TILE_SIZE_4BPP)

#endif // ALCHEMY_FIELD_PSYNERGY_INTERNAL_H
