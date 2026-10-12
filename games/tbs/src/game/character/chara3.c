#include "unit/game/character/starteq.h"
#include "unit/game/character/chara.h"
#include "unit/game/character/chara2.h"
#include "unit/game/character/chara3.h"
#include "unit/game/character/chara4.h"
#include "unit/game/character/chara5.h"
#include "unit/game/character/chara6.h"
#include "unit/game/character/chara7.h"
#include "unit/game/character/chara8.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "mem.h"
#include "scene.h"
#include "owner_state.h"
#include "character.h"
#include "party_state.h"
#include "runtime_interfaces.h"
#include "item.h"
#include "game_flags.h"
#include "inventory.h"
#include "resource.h"
#include "resource_ids.h"
#include "dma.h"
#include "game_state.h"
#include "frame.h"
#include "save_state.h"
#include "io_reg.h"
#include "fixed_math.h"
#include "battle_unit.h"
#include "lib.h"
#include "edition.h"
#include "btlsys.h"
#include "text/msg_ids.h"
#include "btlsmn.h"
#include "preset_table.h"
#include "iwram_call.h"
#include "btlcalc.h"
#include "battle_random.h"
#include "btltype.h"
#include "battle_effect_chance.h"
#include "btlpty.h"
#include "field_event.h"

s32 GameFlag_Test(s32 flag)
{
	s32 flag_mask;
	u8 *flag_bytes;
	s32 flag_value;

	flag_mask = 1 << (7 & flag);
	flag = ((u32)flag << 0x14) >> 0x17;
	flag_bytes = GameFlagBytes;
	flag_value = flag_bytes[flag] & flag_mask;
	return (u32)((0 - flag_value) | flag_value) >> 0x1F;
}

s32 GameFlag_SetBit(s32 flag)
{
	s32 flag_mask;
	u8 *flag_bytes;

	flag_mask = 1 << (7 & flag);
	flag_bytes = GameFlagBytes;
	flag = ((u32)flag << 0x14) >> 0x17;
	flag_bytes[flag] |= flag_mask;
	return flag;
}

void GameFlag_ClearBit(s32 flag)
{
	s32 flag_mask;
	u8 *flag_bytes;

	flag_mask = ~(1 << (7 & flag));
	flag_bytes = GameFlagBytes;
	flag = ((u32)flag << 0x14) >> 0x17;
	flag_bytes[flag] &= flag_mask;
}

u32 GameFlag_Toggle(s32 flag)
{
	s32 flag_mask;
	u8 *flag_bytes;
	s32 flag_value;

	flag_mask = 1 << (7 & flag);
	flag_bytes = GameFlagBytes;
	flag = ((u32)flag << 0x14) >> 0x17;
	flag_bytes[flag] ^= flag_mask;
	flag_value = flag_bytes[flag] & flag_mask;
	return (u32)((0 - flag_value) | flag_value) >> 31;
}

s32 GameFlag_GetByte(s32 flag)
{
	u32 byte_index;

	byte_index = flag << 20;
	flag = byte_index >> 23;
	return GameFlagBytes[flag];
}

void GameFlag_SetByte(s32 flag, s32 value)
{
	u32 shifted = flag << 20;
	flag = shifted >> 23;
	GameFlagBytes[flag] = value;
}

u8 GameFlag_IncrementByte(s32 flag)
{
	u8 *flag_bytes;

	flag_bytes = GameFlagBytes;
	flag = ((u32)flag << 0x14) >> 0x17;
	if (flag_bytes[flag] <= 0xFE)
	{
		flag_bytes[flag]++;
	}
	return flag_bytes[flag];
}

u8 GameFlag_DecrementByte(s32 flag)
{
	u8 *flag_bytes;

	flag_bytes = GameFlagBytes;
	flag = ((u32)flag << 0x14) >> 0x17;
	if (flag_bytes[flag] != 0)
	{
		flag_bytes[flag] += 0xFF;
	}
	return flag_bytes[flag];
}

s32 GameFlag_GetNibble(u32 flag)
{
	u32 shifted = flag << 20;
	u32 shift;
	u32 mask;
	s32 result;

	shift = flag & 4;
	mask = 15;
	flag = shifted >> 23;
	result = GameFlagBytes[flag];
	result &= mask << shift;
	return result >> shift;
}

void GameFlag_SetNibble(s32 flag, s32 value)
{
	s32 field_mask = 0xF;
	s32 shift = 4 & flag;
	s32 mask = field_mask << shift;
	u8 *bytes = GameFlagBytes;

	flag = ((u32)flag << 0x14) >> 0x17;
	bytes[flag] = (bytes[flag] & ~mask) | ((value & field_mask) << shift);
}
