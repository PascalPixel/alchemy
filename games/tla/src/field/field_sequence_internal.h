#ifndef ALCHEMY_FIELD_SEQUENCE_INTERNAL_H
#define ALCHEMY_FIELD_SEQUENCE_INTERNAL_H

/// @file
/// What every part of field_sequence.c shares: its includes, and the
/// declarations its parts call before their definitions or once declared
/// differently. field_sequence.c includes it before the parts, and each part
/// includes it too, so that a part read on its own resolves. It places no
/// bytes.

#include "types.h"
#include "field/world_map.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_ids.h"
#include "scene_tables.h"
#include "actor.h"
#include "event_work.h"
#include "map.h"
#include "field_sprite.h"
#include "dma.h"
#include "io_reg.h"
#include "system/vector3.h"
#include "owner_value.h"
#include "battle_unit.h"
#include "owner_state.h"
#include "fixed_math.h"
#include "range_falloff.h"
#include "field_event.h"
#include "input.h"
#include "macros.h"
#include "version.h"
#include "field/event_work.h"
#include "scene.h"
#include "object_effect.h"
#include "item.h"
#include "battle_system.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "actor_script_view.h"
#include "objdisp.h"
#include "heap_release.h"
#include "flag_call.h"
#include "display_transition.h"
#include "task.h"
#include "io_write_queue.h"
#include "irq.h"
#include "frame.h"
#include "palette_buffers.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "sprite.h"
#include "resource.h"
#include "vram_tab.h"
#include "io_write.h"
#include "gamepak/far_call/window.h"
#include "module/field/field.h"
#include "module/field/field_sequence.h"
#include "module/field/object.h"
#include "module/system/system.h"
#include "module/tla.h"

// A row of the field Psynergy target table (gData_080eff28).
struct DescriptorEntry {
  s16 id;
  u8 unk02[2];
  s16 range;
  u8 unk06[2];
};

// The object's functions its parts call before their definitions.
struct TriggerView;
struct PendingEvent;

// Outside routines and data the parts had declared each in its own way, with
// the one type they share.
struct Actor;
struct Actor *Actor_GetById(u32 objectId);
/* VIEW(field_sequence): routine MapGrid_GetTerrainHeightAboveFar as s32 MapGrid_GetTerrainHeightAboveFar(s32 x, s32 z, s32 layer, s32 height); module/tla.h has s32 MapGrid_GetTerrainHeightAboveFar(s32 x, s32 z, s32 layer). */
extern s32 MapGrid_GetTerrainHeightAboveFar__field_sequence(s32 x, s32 z, s32 layer, s32 height) __asm__("MapGrid_GetTerrainHeightAboveFar");
struct ParticleEmitter;

#endif
