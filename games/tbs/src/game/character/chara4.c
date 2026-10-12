#include "unit/game/character/chara.h"
#include "unit/game/character/chara2.h"
#include "unit/game/character/chara3.h"
#include "unit/game/character/chara4.h"
#include "unit/game/character/chara5.h"
#include "unit/game/character/chara6.h"
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

#if EDITION_INTERNATIONAL
#define ENEMY_SUFFIX_FIRST '1'
#define ENEMY_SUFFIX_LAST 8
#else
#define ENEMY_SUFFIX_FIRST 'A'
#define ENEMY_SUFFIX_LAST 25
#endif

s32 BtlUnitSet(s32 unit_id, s32 enemy_id, s32 suffix)
{
	struct BtlUnit				 *unit;
	const struct EnemyDefinition *enemy;
	u16							  name[15];
	s32							  i;
	u32							  index;
	s32							  q;
	s32							  count;

	index = enemy_id - 8;
	if (unit_id <= 127)
		return 0;
	if (unit_id > 134)
		return 0;
	if (index > 242)
		return 0;
	unit = UnitGet(unit_id);
	Iwram_ClearWords(unit, BATTLE_UNIT_SIZE);
	if (index > 164)
		index = 0;
	enemy = &Enemy_Definitions[index];
	unit->level = enemy->level;
	unit->base_hp = enemy->hp;
	unit->hp = enemy->hp;
	unit->max_hp = enemy->hp;
	unit->base_ep = enemy->ep;
	unit->ep = enemy->ep;
	unit->max_ep = enemy->ep;
	unit->hp_gauge = 0x4000;
	unit->ep_gauge = 0x4000;
	unit->base_attack = enemy->atk;
	unit->base_defense = enemy->def;
	unit->base_agility = enemy->agi;
	unit->base_luck = enemy->luck;
	unit->base_action_count = enemy->turns;
	unit->base_hp_regen = enemy->hp_regen;
	unit->base_ep_regen = enemy->ep_regen;
	UiText_DecodeMessageFar(index + (s32)MsgEnemyName, name, 15);
	for (i = 0; i < 14 && name[i] != 0; i++)
		unit->name[i] = name[i];
	if (suffix <= ENEMY_SUFFIX_LAST)
	{
		unit->name[i] = ENEMY_SUFFIX_FIRST + suffix;
		i++;
	}
	unit->name[i] = 0;
	unit->name[14] = 0;
	count = 0;
	for (i = 0; i < 4; i++)
	{
		if (enemy->items[i] != 0)
		{
			for (q = 0; q < enemy->counts[i]; q++)
			{
				if (count < 15)
					unit->inventory[count++] = enemy->items[i];
			}
		}
	}
	unit->rewards = enemy->rewards;
	unit->class_index = 0;
	unit->class_id = enemy_id;
	Owner_CalculateBaseElements(unit_id, (s16(*)[2])unit->base_elm);
	UnitCalc(unit_id);
	unit->side = 1;
	switch (unit->class_id)
	{
	case 158:
	case 159:
	case 160:
	case 161:
	case 162:
	case 163:
	case 164:
	case 165:
	case 166:
	case 167:
	case 168:
	case 169:
	case 170:
	case 171:
		unit->side = 2;
		break;
	}
	return 1;
}

s32 Party_CountActiveOwners(void)
{
	s32 owner;
	s32 count;

	count = 0;
	owner = 0;
	do
	{
		if (GameFlag_Test(owner) != 0)
			count++;
		owner++;
	} while (owner <= 7);
	return count;
}

s32 Party_AddActiveOwner(s32 value)
{
	s32 count = Party_CountActiveOwners();
	s32 index;

	GameFlag_SetBit(value);
	index = 0;
	while (index < count)
	{
		if (gGameState.active_owners[index] == value)
			return count;
		index++;
	}
	gGameState.active_owners[index] = value;
	return count + 1;
}

s32 Party_RemoveActiveOwner(s32 value)
{
	s32 count = Party_CountActiveOwners();
	s32 i;
	s32 j;

	GameFlag_ClearBit(value);
	for (i = 0; i < count; i++)
	{
		if (gGameState.active_owners[i] == value)
			break;
	}
	for (j = i; j < count - 1; j++)
		gGameState.active_owners[j] = gGameState.active_owners[j + 1];
	return Party_CountActiveOwners();
}

s32 Party_ListActiveOwners(s16 *owners)
{
	s32 count = 0;

	if (owners != NULL)
	{
		s32 index;

		count = Party_CountActiveOwners();
		index = 0;
		if (count != 0)
		{
			do
			{
				*owners++ = gGameState.active_owners[index];
				index++;
			} while (index != count);
		}
		*owners = 0xff;
	}
	return count;
}

s32 Party_AdjustCoins(s32 amount)
{
	s32				  value;
	struct GameState *work;
	struct GameState *store;

	work = &gGameState;
	value = work->coins;
	value += amount;
	store = work;
	if (value > 0xF423F)
	{
		value = 0xF423F;
	}
	if (value < 0)
	{
		value = 0;
	}
	work = store;
	work->coins = value;
	return value;
}
