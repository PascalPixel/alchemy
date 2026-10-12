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
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
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

void Inventory_AddAndEquip(s32 owner, s32 target)
{
	struct BtlUnit *state = UnitGet(owner);
	u16			   *entry;
	s32				index;

	Inventory_AddItem(owner, target);
	index = 0;
	entry = state->inventory;
	do
	{
		if (*entry++ == target)
			Inventory_Equip(owner, index);
		index++;
	} while (index < INVENTORY_SLOTS);
}

#define INVENTORY_SNAPSHOT_SENTINEL 0x6774

void InventorySnapshot_Save(void)
{
	u16			   *save;
	struct BtlUnit *unit;
	s16			   *inventory;
	s32				owner;
	s32				i;
	s32				n;
	s16				first;
	s16				second;

	save = gInventorySnapshot;
	if (*save != INVENTORY_SNAPSHOT_SENTINEL)
	{
		*save++ = INVENTORY_SNAPSHOT_SENTINEL;
		first = gGameState.first_shortcut;
		second = gGameState.second_shortcut;
		for (owner = 0; owner < 4; owner++)
		{
			unit = UnitGet(owner);
			for (n = 0; n < 15; n++)
				*save++ = unit->inventory[n];
			for (n = 0; n < 15; n++)
			{
				if (Item_GetDirect(unit->inventory[n])->type != 6)
					unit->inventory[n] = 0;
			}
			n = 0;
			inventory = unit->inventory;
			for (i = 0; i < 15; i++)
			{
				if (inventory[i] != 0)
					inventory[n++] = inventory[i];
			}
			for (; n < 15; n++)
				inventory[n] = 0;
			Owner_RefreshDerivedData(owner);
			UnitCalc(owner);
		}
		*save++ = first;
		*save++ = second;
		inventory = (s16 *)gGameState.active_owners;
		save[0] = inventory[0];
		save[1] = inventory[1];
		Inventory_AddAndEquip(0, 16);
		GameFlag_SetBit(0x952);
	}
	Owner_RefreshActiveRatios(1);
}

void InventorySnapshot_Restore(void)
{
	s32	 owner;
	u16 *source = gInventorySnapshot;

	if (*source++ == INVENTORY_SNAPSHOT_SENTINEL)
	{
		owner = 0;
		do
		{
			u16 *inventory = UnitGet(owner)->inventory;
			s32	 remaining = 14;

			do
			{
				*inventory++ = *source++;
				remaining--;
			} while (remaining >= 0);
			Owner_RefreshDerivedData(owner);
			UnitCalc(owner);
			owner++;
		} while (owner <= 3);

		gGameState.first_shortcut = *source++;
		gGameState.second_shortcut = *source++;
		*(u16 *)gGameState.active_owners = *source++;
		*(u16 *)((u8 *)gGameState.active_owners + 2) = *source;
		source = gInventorySnapshot;
		*source = 0;
		GameFlag_ClearBit(0x952);
	}
}
