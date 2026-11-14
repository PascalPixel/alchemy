#ifndef ALCHEMY_RADIAL_INTERNAL_H
#define ALCHEMY_RADIAL_INTERNAL_H

/// @file
/// What every part of the radial object shares: its includes. radial.c
/// includes it before the parts, and each part includes it too.

// world_map.h first: battle.h names struct WorldMapWork, and a struct declared
// before its definition keeps a smaller alignment, which world_picture.c's
// copies of it show.
#include "field/world_map.h"
#include "system/iwram_call.h"
#include "field/objdisp.h"
#include "system/system.h"
#include "fixed_math.h"
#include "types.h"
#include "battle/motobj.h"
#include "scene_tables.h"
#include "system/field_effect.h"
#include "battle/battle.h"
#include "debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "actor.h"
#include "system/ram_buffer.h"
#include "field/field_psynergy_work.h"
#include "field/sprite.h"
#include "field_event.h"
#include "field/field_particle.h"
#include "actor_view.h"
#include "system/object_lookup.h"
#include "system/main_routine_work.h"
#include "effect_sprite.h"
#include "system/vector3.h"
#include "graphics/resource.h"
#include "system/task.h"
#include "system/heap_release.h"
#include "system/scene.h"
#include "system/frame.h"
#include "scene_ids.h"
#include "graphics/resource.h"
#include "graphics/vram_tab.h"
#include "arc_effect.h"
#include "system/heap_release.h"
#include "system/task.h"
#include "canvas_sprite.h"
#include "system/io_reg.h"
#include "map.h"

/// The effect scene of the Psynergy these parts run (effect_work): its caster
/// and target, where it takes place, and what each kind keeps there.
struct FxScene {
  u16 facing; ///< 1/65536 turns
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
  struct Actor *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u16 action;
  s16 fieldKind;
  s8 enabled;
  u8 unk21[0x13];
  s8 childOption;
  u8 unk35[0x0c];
  s8 alternate;
  u8 unk42[0x782];
  s16 stage;
  s16 frame;
};

void FieldEffect_AdvanceMotion(union FieldObject *object);

#endif // ALCHEMY_RADIAL_INTERNAL_H
