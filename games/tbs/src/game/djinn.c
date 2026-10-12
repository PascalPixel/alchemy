#include "unit/game/chara.h"
#include "unit/game/item.h"
#include "unit/game/flag.h"
#include "unit/game/party.h"
#include "unit/game/class.h"
#include "unit/game/btlcalc.h"
#include "unit/game/djinn.h"
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

const u16 *Djinn_GetDefinition(u32 group, u32 index)
{
	s32 entry;

	entry = 0;
	if ((group <= 3U) && (index <= 0x13U))
	{
		entry = (group * 0x14) + index;
	}
	return (const u16 *)((u8 *)Djinn_DefinitionTable + entry * 0xC);
}

s32 Djinn_AddToLeastLoadedOwner(s32 index, u8 *state)
{
	void *entry = state + index * 20 + 48;
	s32	  best_no = 0;
	s32	  best_val = 999;
	s32	  count;
	s32	  i;
	s32	  j;

	if (GameFlag_Test(entry) != 0)
		return -1;

	count = Party_CountActiveOwners();
	for (i = 0; i < count; i++)
	{
		struct BtlUnit *unit = UnitGet(gGameState.active_owners[i]);

		if (unit->djinn_owned_counts[index] <= 9)
		{
			s32 value = 0;

			for (j = 0; j < 4; j++)
				value += unit->djinn_owned_counts[j];
			if (best_val > value)
			{
				best_val = value;
				best_no = gGameState.active_owners[i];
			}
		}
	}

	if (best_val == 999)
		return -2;

	Djinn_AddToOwner(best_no, index, (s32)state);
	Trade_AddOffer(best_no, index, (u32)state);
	GameFlag_SetBit((u32)entry);
	return best_no;
}

s32 Djinn_AddToOwner(s32 owner, s32 index, s32 bit)
{
	struct BtlUnit *state = UnitGet(owner);

	if (state->djinn_owned_counts[index] > 9)
		return -1;
	if ((state->djinn_available[index] & (1 << bit)) != 0)
		return -1;
	state->djinn_owned_counts[index]++;
	state->djinn_available[index] |= 1 << bit;
	return 0;
}

s32 Trade_CanOfferDjinn(s32 owner, s32 index, s32 bit)
{
	struct BtlUnit			 *state = UnitGet(owner);
	struct DjinnRecoveryList *table;
	s32						  i;
	s32						  status;

	if (state->djinn_owned_counts[index] == 0)
		return 0;
	if (state->djinn_active_counts[index] > 9)
	{
		state->djinn_active_counts[index] = 10;
		return 0;
	}
	if ((state->djinn_available[index] & (1 << bit)) == 0)
		return 0;
	if ((state->djinn_active[index] & (1 << bit)) != 0)
		return 0;

	table = &((struct DjinnRecoveryTable *)Trade_GetOfferState((owner < 0 || owner > 7)))->list;
	for (i = 0; i < table->count; i++)
	{
		if (index == table->entries[i].element && bit == table->entries[i].index)
			break;
	}
	if (i == table->count || ((status = table->entries[i].turns) <= 0 && status != -2))
		return 1;
	return 0;
}

u32 Djinn_IsActive(s32 owner, s32 index, s32 bit)
{
	s32 value = UnitGet(owner)->djinn_active[index] & (1 << bit);

	return (u32)(-value | value) >> 31;
}

s32 Djinn_Activate(s32 owner, s32 index, s32 bit)
{
	struct BtlUnit *state = UnitGet(owner);
	s32				result = Trade_CanOfferDjinn(owner, index, bit);

	if (result != 0)
	{
		if (state->djinn_available[index] & (1 << bit))
		{
			state->djinn_active[index] |= 1 << bit;
		}
		else
		{
			return 0;
		}
		state->djinn_active_counts[index]++;
		Owner_RefreshDerivedData(owner);
	}
	return result;
}

u32 Djinn_Deactivate(s32 owner, s32 index, s32 bit)
{
	struct BtlUnit *state = UnitGet(owner);
	u32				present = Djinn_IsActive(owner, index, bit);

	if (present != 0)
	{
		state->djinn_active_counts[index]--;
		state->djinn_active[index] &= ~(1 << bit);
		Owner_RefreshDerivedData(owner);
	}
	return present;
}

s32 Trade_RemoveOffer(s32 owner, s32 index, s32 bit)
{
	struct DjinnRecoveryList *table;
	s32						  found = 0;
	s32						  i;

	table = &((struct DjinnRecoveryTable *)Trade_GetOfferState((owner < 0 || owner > 7)))->list;
	for (i = 0; i < table->count; i++)
	{
		if (index == table->entries[i].element && bit == table->entries[i].index)
		{
			table->count--;
			found = 1;
			break;
		}
	}
	for (; i < table->count; i++)
	{
		table->entries[i] = table->entries[i + 1];
	}
	return found;
}

u32 *Trade_AddOffer(u32 kind, u32 first, u32 second)
{
	struct DjinnRecoveryList *table;
	s32						  count;

	Trade_RemoveOffer(kind, first, second);
	table = &((struct DjinnRecoveryTable *)Trade_GetOfferState(kind > 7))->list;
	count = table->count;
	table->entries[count].element = first;
	table->entries[count].index = second;
	table->entries[count].unit_id = kind;
	table->entries[count].turns = -1;
	table->count = count + 1;
	return (u32 *)&table->count;
}

s32 Djinn_Transfer(s32 source, s32 index, s32 bit, s32 target)
{
	struct BtlUnit *state = UnitGet(source);
	u32				present;

	if ((state->djinn_available[index] & (1 << bit)) != 0)
	{
		present = Djinn_IsActive(source, index, bit);
		if (Djinn_AddToOwner(target, index, bit) == 0)
		{
			Djinn_Deactivate(source, index, bit);
			state->djinn_available[index] &= ~(1 << bit);
			state->djinn_owned_counts[index]--;

			if (present != 0)
			{
				Djinn_Activate(target, index, bit);
			}
			else
			{
				Trade_RemoveOffer(source, index, bit);
				Trade_AddOffer(target, index, bit);
			}
			return 0;
		}
	}
	return -1;
}

s32 Trade_CountPendingOffers(u8 *counts)
{
	s32						   found = 0;
	struct DjinnRecoveryTable *base = Trade_GetOfferState(0);
	struct DjinnRecoveryEntry *entry = base->list.entries;
	s32						   index;

	if (counts != 0)
	{
		u8 *slot;

		slot = counts + 3;
		*slot = found;
		slot = counts + 2;
		*slot = found;
		slot = counts + 1;
		*slot = found;
		counts[0] = found;
	}
	index = 0;
	if (base->list.count != 0)
	{
		do
		{
			if (entry->turns == -1)
			{
				if (counts != 0)
					counts[entry->element]++;
				found++;
			}
			index++;
			entry++;
		} while (index != base->list.count);
	}
	return found;
}

s32 Djinn_GetDefinitionHeader(u32 group, u32 index)
{
	return *Djinn_GetDefinition(group, index);
}

s32 Party_SumDjinnCounts(s32 index)
{
	u16 owners[16];
	s32 result = 0;
	s32 count = Party_ListActiveOwners(owners);

	if (result < count)
	{
		u16 *owner = owners;
		s32	 remaining = count;

		do
		{
			struct BtlUnit *state = UnitGet(*owner++);

			if (index == -1)
			{
				result += state->djinn_owned_counts[0];
				result += state->djinn_owned_counts[1];
				result += state->djinn_owned_counts[2];
				result += state->djinn_owned_counts[3];
			}
			else
			{
				result += state->djinn_owned_counts[index];
			}
			remaining--;
		} while (remaining != 0);
	}
	return result;
}
