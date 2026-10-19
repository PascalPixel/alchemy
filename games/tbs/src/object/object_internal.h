#ifndef ALCHEMY_OBJECT_INTERNAL_H
#define ALCHEMY_OBJECT_INTERNAL_H

/// @file
/// What every part of object.c shares: its includes and the macros and
/// types more than one part uses. object.c includes it before the parts, and
/// each part includes it too, so that a part read on its own resolves. It
/// places no bytes.

#include "unit/object/battle_cue.h"
#include "unit/object/zonedata.h"
#include "vram_tab.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"
#include "types.h"
#include "scene.h"
#include "battle_type.h"
#include "game_state.h"
#include "field_runtime.h"
#include "battle_system.h"
#include "battle.h"
#include "battle_unit.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "scene_ids.h"
#include "io_reg.h"
#include "iowrite.h"
#include "irq.h"
#include "callback_scheduler.h"
#include "flags.h"
#include "iwram_call.h"
#include "event_work.h"
#include "resource_ids.h"
#include "battle_effect_runtime.h"
#include "resource.h"
#include "field_scene.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "global_cells.h"
#include "object_runtime.h"
#include "script_object_runtime.h"
#include "animspr.h"
#include "field_obj.h"
#include "object_dispatch.h"
#include "field_event.h"
#include "heap_state.h"
#include "keystate.h"
#include "owner_value.h"
#include "party_state.h"
#include "songs.h"
#include "fixed_math.h"
#include "item_gift.h"
#include "battle_calc.h"
#include "display_transition.h"
#include "text/msg_ids.h"
#include "object_lookup.h"
#include "objdisp.h"
#include "object_effect.h"
#include "item.h"
#include "motobj.h"
#include "static_assert.h"
#include "frame.h"
#include "io_write_queue.h"
#include "palbuf.h"
#include "vram_block.h"
#include "item_ids.h"
#include "inventory.h"
#include "ui.h"
#include "window.h"
#include "battle_party.h"
#include "b5_context.h"
#include "edition.h"
#include "facing_object.h"
#include "script_motion.h"
#include "tbs_edition.h"
#include "metadata_lookup.h"
#include "field_coordinate.h"
#include "scroll.h"
#include "fxblend.h"
#include "fx_scene.h"
#include "effect_slot.h"
#include "object_effect_scripts.h"
#include "menu_list.h"
#include "field_effect.h"
#include "fixed_point_position.h"
#include "macros.h"
#include "ability_ids.h"

/// Field_RunScene's result when the scene was not left (scene.c, run_scene.c).
#define EXIT_NONE 999

/// Scene ids above the field maps, which Game_ResetForNewGame runs without a
/// map.
enum {
  SCENE_BLANK = 0x1fb,
  SCENE_REEL_GAME = 0x1fc,
  SCENE_LUCKY_DICE = 0x1fd,
  SCENE_BATTLE = 0x1fe
};

/// The polar-offset routine walks three array elements. Named coordinates
/// remain useful to callers; the union gives that array real storage.
union PolarOffsetPosition {
  struct EffectVector vector;
  s32 coordinates[3];
};

STATIC_ASSERT(sizeof(union PolarOffsetPosition) == 12);

/// A scene region's bounds, in pixels, as 16.16 fixed point.
#define REGION_FIXED(value) ((s32)((u32)(s32)(value) << 16))

// A field Psynergy or item use, packed into one value (items.c, psynergy.c).
#define ABILITY_ID(c) ((c) & 0x3ff)    // the Psynergy
#define ACT_UNIT(c) (((c) >> 10) & 15) // who uses it
#define ACT_PACKED 0x2000              // use it without paying PP
#define UNIT_NONE 15
#define ALLY_MAX 7 // the last party member

#define EXIT_RETREAT 999 // the exit Retreat takes

#define FLAG_PSYNERGY_EFFECT 0x140 // play the usual effect
#define FLAG_PSYNERGY_TARGET 0x141 // aim the effect at the target

/// The flags of a Psynergy event (kind 5) in the given phase.
#define PSYNERGY_EV(phase) ((phase) << 28 | EVENT_PSYNERGY)
#define PSYNERGY_EV_ANY PSYNERGY_EV(7) // any phase

/// Shows message m with the user's and the Psynergy's names, and waits.
#define PSYNERGY_MESSAGE(who, act, m)                                          \
  (UiWork_PushValueSlotFar((who), 1), UiWork_PushValueSlotFar((act), 4),       \
   UiText_ShowPositionedMessageAndWaitFar((m), 1))

#endif
