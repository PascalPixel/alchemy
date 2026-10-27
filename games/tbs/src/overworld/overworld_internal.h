#ifndef ALCHEMY_OVERWORLD_INTERNAL_H
#define ALCHEMY_OVERWORLD_INTERNAL_H

/// @file
/// What every part of overworld.c shares: its includes and the macros and
/// types more than one part uses. overworld.c includes it before the parts, and
/// each part includes it too, so that a part read on its own resolves. It
/// places no bytes.

#include "unit/overworld/battle_cue.h"
#include "unit/overworld/zonedata.h"
#include "vram_cache.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "unit/runtime/runtime.h"
#include "types.h"
#include "battle_type.h"
#include "save_block.h"
#include "scene_header.h"
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
#include "task.h"
#include "flags.h"
#include "iwram_call.h"
#include "field_work.h"
#include "resource_ids.h"
#include "speaker.h"
#include "resource.h"
#include "scene_tables.h"
#include "map_grid.h"
#include "map_grid_scroll.h"
#include "ram_buffer.h"
#include "heap_slots.h"
#include "actor.h"
#include "animspr.h"
#include "actor.h"
#include "object_dispatch.h"
#include "event_layer.h"
#include "heap.h"
#include "keystate.h"
#include "unit.h"
#include "party_state.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "fixed_math.h"
#include "item_gift.h"
#include "battle_calc.h"
#include "display_transition.h"
#include "text/msg_ids.h"
#include "objdisp.h"
#include "object_effect.h"
#include "item.h"
#include "motobj.h"
#include "static_assert.h"
#include "frame.h"
#include "io_write_queue.h"
#include "palbuf.h"
#include "item_ids.h"
#include "inventory.h"
#include "ui.h"
#include "window.h"
#include "battle_party.h"
#include "edition.h"
#include "script_motion.h"
#include "tbs_edition.h"
#include "metadata_lookup.h"
#include "field_coordinate.h"
#include "scroll.h"
#include "fxblend.h"
#include "fx_work.h"
#include "field_effect_sprite.h"
#include "object_effect_scripts.h"
#include "menu_list.h"
#include "field_effect.h"
#include "fixed_point_position.h"
#include "macros.h"
#include "ability_ids.h"
#include "sprite_ids.h"
#include "field/encounter_zone.h"

/// The speakers' definitions (fxdata.c), read by Speaker_FindDefinition.
extern struct SpeakerDefinition gSpeakerDefinitions[];

/// Map ids above the field maps, which SequenceManager_Run runs without a
/// layout; each returns a result to Scene_ReturnFromSequence instead of an
/// exit. A 16.16 position that Vector_AddPolarOffset moves by a distance and a
/// heading: the routine walks the coordinates as three words, and the callers
/// read the result by name.
union PolarOffsetPosition {
  struct Vector3 vector;
  s32 coordinates[3];
};

STATIC_ASSERT(sizeof(union PolarOffsetPosition) == 12);

/// A map region's bounds, in pixels, as 16.16 fixed point.
#define REGION_FIXED(value) ((s32)((u32)(s32)(value) << 16))

// A field Psynergy or item use, packed into one value (items.c, psynergy.c),
// as work->psynergy and work->item hold it. bits: 13..10 the user (13 is
// ACT_PACKED) | 9..0 the Psynergy or item. Since ACT_PACKED is the user's top
// bit, a packed use names user 8 or above.
#define ABILITY_ID(c) ((c) & 0x3ff)    ///< the Psynergy
#define ACT_UNIT(c) (((c) >> 10) & 15) ///< who uses it
#define ACT_PACKED 0x2000              ///< use it without paying PP
/// No user named: Field_UsePsynergy takes character 0, and Field_UseItem
/// whoever holds the item in the most inventory slots.
#define UNIT_NONE 15
#define ALLY_MAX 7 ///< the last party member; only users up to it pay PP

/// Shows message m with the user's and the Psynergy's names, and waits.
#define PSYNERGY_MESSAGE(who, act, m)                                          \
  (MessageArgument_PushFar((who), MESSAGE_ARGUMENT_ACTOR),                     \
   MessageArgument_PushFar((act), MESSAGE_ARGUMENT_ABILITY),                   \
   FieldMessage_ShowFar((m), FIELD_MESSAGE_ZOOM))

#endif
