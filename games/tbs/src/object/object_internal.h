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
#include "sprite_ids.h"

/// The speakers' definitions (fxdata.c), read by FieldFx_FindDefinition.
extern struct FieldFxDefinition gFieldFx_DefinitionTable[];

/// The exit request whose destination is already in gGameState.scene and
/// .entrance, so that Scene_TakeExit leaves them as they are. Random battles,
/// Retreat and the event commands that set a destination (Event_SetDestination)
/// raise it, and Field_RunScene returns it when the whole party falls;
/// Scene_TakeExit also uses it for "no exit found" (scene.c, run_scene.c).
#define EXIT_NONE 999

/// Scene ids above the field maps, which Game_Run runs without a
/// map; each returns a result to Scene_ReturnFromBattle instead of an exit.
/// A 16.16 position that Vector_AddPolarOffset moves by a distance and a
/// heading: the routine walks the coordinates as three words, and the callers
/// read the result by name.
union PolarOffsetPosition {
  struct EffectVector vector;
  s32 coordinates[3];
};

STATIC_ASSERT(sizeof(union PolarOffsetPosition) == 12);

/// A scene region's bounds, in pixels, as 16.16 fixed point.
#define REGION_FIXED(value) ((s32)((u32)(s32)(value) << 16))

// A field Psynergy or item use, packed into one value (items.c, psynergy.c),
// as work->command and work->item hold it. bits: 13..10 the user (13 is
// ACT_PACKED) | 9..0 the Psynergy or item. Since ACT_PACKED is the user's top
// bit, a packed use names user 8 or above.
#define ABILITY_ID(c) ((c) & 0x3ff)    ///< the Psynergy
#define ACT_UNIT(c) (((c) >> 10) & 15) ///< who uses it
#define ACT_PACKED 0x2000              ///< use it without paying PP
/// No user named: Field_UsePsynergy takes character 0, and Field_UseItem
/// whoever holds the item in the most inventory slots.
#define UNIT_NONE 15
#define ALLY_MAX 7 ///< the last party member; only users up to it pay PP

/// EXIT_NONE's value: Retreat writes its destination itself.
#define EXIT_RETREAT 999

/// Shows message m with the user's and the Psynergy's names, and waits.
#define PSYNERGY_MESSAGE(who, act, m)                                          \
  (UiWork_PushValueSlotFar((who), 1), UiWork_PushValueSlotFar((act), 4),       \
   UiText_ShowPositionedMessageAndWaitFar((m), 1))

#endif
