#ifndef ALCHEMY_FIELD_PSYNERGY_INTERNAL_H
#define ALCHEMY_FIELD_PSYNERGY_INTERNAL_H

/// @file
/// What every part of the field_psynergy object shares: its includes.
/// field_psynergy.c includes it before the parts, and each part includes it
/// too.

// world_map.h first: battle.h names struct WorldMapWork, and a struct declared
// before its definition keeps a smaller alignment, which world_picture.c's
// copies of it show.
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
#include "field/field_psynergy_work.h"
#include "field/sprite.h"
#include "field/field_event.h"
#include "field/field_particle.h"
#include "field/actor_view.h"
#include "system/object_lookup.h"
#include "system/main_routine_work.h"
#include "field/effect_sprite.h"
#include "system/vector3.h"
#include "graphics/resource.h"
#include "system/task.h"
#include "system/heap_release.h"
#include "system/scene.h"
#include "system/frame.h"
#include "scene_ids.h"
#include "graphics/resource.h"
#include "graphics/vram_tab.h"
#include "field/arc_effect.h"
#include "system/heap_release.h"
#include "system/task.h"
#include "field/canvas_sprite.h"
#include "system/io_reg.h"
#include "field/map_grid.h"
#include "field/field_sequence.h"

void FieldEffect_AdvanceMotion(union FieldObject *object);

/// A spark that circles a centre: the centre in 16.16 fixed-point pixels, the
/// angle and radius it is drawn at, and its age in frames (-1 while unused).
struct WhirlSpark {
  s32 x;
  s32 y;
  s32 z;
  s32 angle;            ///< 1/65536 turns
  s32 unusedAfterAngle; ///< no code names it
  s32 radius;           ///< 16.16 fixed-point pixels
  s32 age;
};

/// Per-frame update: sets every part of an actor's sprite to colour mode
/// frame % 6, which cycles its colours.
void Actor_ApplyRandomChildValues(struct Actor *actor);

/// The effect sprites a field Psynergy has (FieldEffectWork.particles).
#define PSYNERGY_EFFECT_SLOT_COUNT 24

/// The states of a Retreat particle (FieldEffect_UpdateRadialLaunch).
enum RetreatParticleState {
  RETREAT_PARTICLE_AIM = 0,    ///< placed on the main actor and aimed
  RETREAT_PARTICLE_FLYING = 1, ///< back to RETREAT_PARTICLE_AIM on arrival
  /// Set by FieldPsynergy_Retreat: freed on arrival.
  RETREAT_PARTICLE_LAST_FLIGHT = 2
};

/// The phase or state a field Psynergy's work is left in when it has
/// finished, which the cast waits for.
#define FIELD_PSYNERGY_FINISHED 999

#endif // ALCHEMY_FIELD_PSYNERGY_INTERNAL_H
