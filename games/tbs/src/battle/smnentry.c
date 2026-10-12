#include "vram_tab.h"
#include "window.h"
#include "unit/battle/battle.h"
#include "unit/battle/unit.h"
#include "unit/battle/smnentry.h"
#include "unit/battle/effect/objtable.h"
#include "unit/system/scheduler/heap.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "owner_state.h"
#include "edition.h"
#include "io_reg.h"
#include "battle_unit.h"
#include "game_state.h"
#include "btlsys.h"
#include "text/msg_ids.h"
#include "mem.h"
#include "resource_ids.h"
#include "callback_scheduler.h"
#include "fixed_point_position.h"
#include "iwram_call.h"
#include "global_cells.h"
#include "btldisp.h"
#include "btlmes.h"
#include "btlpty.h"
#include "battle_intro.h"
#include "inventory.h"
#include "dma.h"
#include "scene.h"
#include "btlwork.h"
#include "btltype.h"
#include "btlsmn.h"
#include "tbs_edition.h"
#include "serial_runtime.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "battle_command.h"
#include "resource.h"
#include "heap_state.h"
#include "flag_ids.h"
#include "menu_list.h"
#include "audio_engine_symbols.h"
#include "ui.h"
#include "btlcalc.h"
#include "objdisp.h"
#include "motobj.h"
#include "battle_status_icon.h"
#include "animspr.h"
#include "field_event.h"
#include "metadata_lookup.h"
#include "fixed_math.h"
#include "battle_motion.h"
#include "effect_step.h"
#include "battle_escape.h"
#include "battle_target.h"
#include "btlobj.h"
#include "btlev.h"
#include "project.h"
#include "battle_random.h"
#include "irqlock.h"
#include "transform.h"
#include "battle_efx.h"
#include "character.h"
#include "io_write_queue.h"
#include "item.h"
#include "irq.h"
#include "vram_block.h"
#include "ownerval.h"
#include "sound_ids.h"
#include "ramcode.h"
#include "map_scroll.h"
#include "iowrite.h"
#include "battle_formation.h"
#include "b5_context.h"
#include "frame.h"

u32 Summon_GetLayoutKind(s32 index)
{
	u32 kind = Summon_EntryTable[index].layout_kind;

	if ((s32)kind > 4)
		kind = -1U;
	return kind;
}

s32 Summon_GetEntryValue(s32 index)
{
	if (index < 0 || index > 171)
		return Summon_EntryTable[0].animation;
	return Summon_EntryTable[index].animation;
}

s32 Summon_GetEntryFlag1Field(s32 index)
{
	if (index < 0 || index > 171)
		return Summon_EntryTable[0].animation;
	return Summon_EntryTable[index].layout_bits1;
}

s32 Summon_IsEntryFlagged(s32 index)
{
	s32 result;

	if (index < 0 || index > 171)
		return 0;
	result = 0;
	if (Summon_EntryTable[index].sprite_bit0)
		result = 1;
	return result;
}

u32 Battle_GetSpriteFlagNibble(u32 index)
{
	u16 value;

	if (index > 171)
		return 1;
	value = Summon_EntryTable[index].sprite_bits1;
	return value ? value : 1;
}

u32 Battle_GetSpriteFlagHighBits(u32 index)
{
	u16 value;

	if (index > 171)
		return 0;
	value = Summon_EntryTable[index].sprite_bits5;
	return value ? value : 0;
}

s32 Summon_IsEntrySecondaryFlagged(s32 index)
{
	if (index < 0 || index > 171)
		return 0;
	return Summon_EntryTable[index].layout_bit0;
}

s32 Summon_GetEntryByte4(s32 index)
{
	if (index < 0 || index > 171)
		return 0;
	return Summon_EntryTable[index].unknown_04[0];
}
