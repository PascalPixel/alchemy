#include "unit/game/character/starteq.h"
#include "unit/game/character/chara.h"
#include "unit/game/character/chara3.h"
#include "unit/game/character/chara4.h"
#include "unit/game/character/chara5.h"
#include "unit/game/character/chara6.h"
#include "unit/game/character/chara7.h"
#include "unit/game/character/chara8.h"
#include "unit/system/scheduler/runtime.h"
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

struct TradeFlags
{
	u32 flags;
};

struct ElementLevelBonus
{
	u16 pow;
	u16 res;
};

extern struct ElementLevelBonus Element_PowerResistByLevel[16];

// gGameState の field に amount を足し、0 から max までにおさめて返す
#define DEFINE_PARTY_ADJUST(name, field, max)     \
	s32 name(s32 amount)                          \
	{                                             \
		s32 value;                                \
		struct GameState *progress = &gGameState; \
                                                  \
		value = progress->field;                  \
		value += amount;                          \
		if (value > max)                          \
			value = max;                          \
		if (value < 0)                            \
			value = 0;                            \
		progress->field = value;                  \
		return value;                             \
	}

DEFINE_PARTY_ADJUST(Party_AdjustSixDigitCounterB, shop_credit, 0xf423f)
DEFINE_PARTY_ADJUST(Party_AdjustCounterCappedAt28, shop_gifts, 28)

s32 Trade_ListFlaggedEntries(u8 *output)
{
	u8 *source = Summon_OrderList;
	u8 *end = Summon_OrderList + 15;
	s32 count = 0;

	do
	{
		u8 value = *source++;
		if ((((struct TradeFlags *)Trade_GetOfferState(0))->flags & (1 << value)) != 0)
		{
			*output++ = value;
			count++;
		}
	} while (source <= end);
	*output = 32;
	return count;
}

const struct SummonDefinition *SummonDefinition_GetNear(u32 summon_id)
{
	if (summon_id > 15)
		return NULL;
	return &Summon_DefinitionTable[summon_id];
}

s32 Owner_LookupFourColumnTable(s32 row, s32 column)
{
	return Owner_FourColumnValues[(row * 4) + column];
}

s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 output[4])
{
	s32	 i;
	s32 *cursor;

	if (record > 7)
	{
		u32 index;

		index = ((const struct EnemyDefinition *)Owner_GetRecord(record))->element_preset;
		if (index > 43)
			index = 0;

		i = 0;
		cursor = output;
		for (; i <= 3; i++)
			*cursor++ = Enemy_ElementPresetTable[index].digits[i] * 10;
	}
	else
	{
		cursor = output;
		source += (u8 *)&((struct BtlUnit *)0)->djinn_active_counts
				  - (u8 *)&((struct BtlUnit *)0)->djinn_available;
		for (i = 3; i >= 0; i--)
		{
			u32 value = *source;
			source++;
			*cursor++ = value * 10;
		}

		if (record <= 7)
		{
			for (i = 0; i <= 3; i++)
			{
				*output += Character_GetDefinition(record)->element_bonus[i];
				output++;
			}
		}
	}

	return 0;
}

s32 Owner_GetResistanceValue(s32 owner, s32 index)
{
	struct BtlUnit *state = UnitGet(owner);
	s32				values[4];
	s32				result = 0;

	if (index <= 3)
	{
		Owner_GetDigitValues(state->class_id, (u8 *)state->djinn_available, values);
		result = values[index] / 10;
	}
	return result;
}

s32 Owner_GetDefaultElement(struct BtlUnit *state)
{
	const struct EnemyDefinition *record =
		(const struct EnemyDefinition *)Owner_GetRecord(state->class_id);
	u8 value = record->element_preset;

	if (value > 43)
		value = 0;
	return Enemy_ElementPresetTable[value].first;
}

u32 Owner_CalculateBaseElements(s32 owner, s16 destination[4][2])
{
	struct BtlUnit *state = UnitGet(owner);
	u32				index;
	s32				values[4];
	s32				i;

	if (state->class_index == 0)
	{
		index = ((const struct EnemyDefinition *)Owner_GetRecord(state->class_id))->element_preset;
		if (index > 43)
			index = 0;

		i = 0;
		for (;;)
		{
			((s32 *)destination)[i] = Enemy_ElementPresetTable[index].values[i];
			i++;
			if (i > 3)
				goto copied;
		}
	copied:
		return index;
	}

	Owner_GetDigitValues(state->class_id, (u8 *)state->djinn_available, values);
	for (i = 0; i < 4; i++)
	{
		s32 value;
		s32 ones;
		s32 tens;

		value = values[i];
		ones = value % 10;
		tens = value / 10;

		if (tens > 15)
			tens = 15;
		if (tens < 0)
			tens = 0;

		destination[i][0] = Element_PowerResistByLevel[tens].pow + ones;
		destination[i][1] = Element_PowerResistByLevel[tens].res + ones;
	}
	// 戻り値なし（r0 には表のアドレスが残る）
}

s32 Owner_DetermineClass(s32 character, const u8 *djinn)
{
	s32 levels[4];
	s32 primary;
	s32 best;
	s32 series;
	s32 result;
	s32 j;
	s32 i;

	result = -1;
	if (character > 7)
		return 0;
	Owner_GetDigitValues(character, djinn, levels);
	if (GameFlag_Test(32))
	{
		if (character == 0)
			return 200;
		if (character == 1)
			return 201;
	}
	if (character == 5)
		return 202;
	if (result != -1)
		return result;
	best = result;
	primary = result;
	for (i = 0; i < 4; i++)
	{
		if (best < levels[i])
		{
			best = levels[i];
			primary = i;
		}
	}
	j = -1;
	best = -1;
	for (i = 0; i < 4; i++)
	{
		if (i != primary && best < levels[i])
		{
			best = levels[i];
			j = i;
		}
	}
	series = Owner_LookupFourColumnTable(primary, levels[j] > 9 ? j : primary);
	for (i = 202; i >= 0; i--)
	{
		if (Class_DefinitionTable[i].series != series)
			continue;
		for (j = 0; j < 4; j++)
		{
			if (levels[j] < Class_DefinitionTable[i].required[j] * 10)
				break;
		}
		if (j == 4)
		{
			result = i;
			break;
		}
	}
	if (result == -1)
		result = 0;
	return result;
}

struct ClassDefinition *Owner_GetRecordStride84(s32 arg0)
{
	return &Class_DefinitionTable[arg0];
}

void Owner_RefreshDerivedData(s32 owner_no)
{
	struct BtlUnit *owner = UnitGet(owner_no);

	owner->class_index = Owner_DetermineClass(owner->class_id, (u8 *)owner->djinn_available);
	Owner_RefreshClassActions(owner_no);
	Owner_CalculateBaseElements(owner_no, (s16(*)[2])owner->base_elm);
}
