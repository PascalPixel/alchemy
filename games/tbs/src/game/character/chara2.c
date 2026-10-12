#include "unit/game/character/starteq.h"
#include "unit/game/character/chara.h"
#include "unit/game/character/chara2.h"
#include "unit/game/character/chara4.h"
#include "unit/game/character/chara5.h"
#include "unit/game/character/chara6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime9.h"
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

//--------------------------------------------------------------------------
// アイテムの定義データを返す
// item : アイテム番号（持ち物の値なら上位の装備・個数ビットを無視する）
//--------------------------------------------------------------------------
struct ItemDefinition *Item_GetDirect(s32 item)
{
	return Item_DefinitionTable + (item & ITEM_ID_MASK);
}

//--------------------------------------------------------------------------
// キャラクターがアイテムを装備できるか
// equip_mask の class_id 番目のビットで決まり、class_id が 8 以上なら 0
//--------------------------------------------------------------------------
s32 Item_CanOwnerEquipDirect(s32 unit, s32 item)
{
	struct BtlUnit		  *st = UnitGet(unit);
	struct ItemDefinition *def = Item_GetDirect(item);
	s32					   mask = def->equip_mask;

	if (st->class_id >= 8)
	{
		return 0;
	}
	return (mask >> st->class_id) & 1;
}

//--------------------------------------------------------------------------
// 装備品でなければ 1、装備品ならキャラクターが装備できるかを返す
//--------------------------------------------------------------------------
s32 Item_IsCompatibleWithOwner(s32 unit, s32 item)
{
	if (Item_GetEquipmentGroup(item) == 0)
	{
		return 1;
	}
	return Item_CanOwnerEquipDirect(unit, item);
}

s32 Item_GetEquipmentGroup(s32 item_id)
{
	s32					   group;
	struct ItemDefinition *item;

	item = Item_GetDirect(item_id);
	group = 0;
	if (item->type == 1)
		group = 1;
	else if (item->type == 2)
		group = 2;
	else if (item->type == 3)
		group = 2;
	else if (item->type == 4)
		group = 2;
	else if (item->type == 5)
		group = 2;
	else if (item->type == 9)
		group = 2;
	return group;
}

s32 Inventory_GetQuantity(s32 owner, s32 slot)
{
	s32 item_id;

	owner = UnitGet(owner)->inventory[slot];
	item_id = ITEM_ID_MASK;
	item_id &= owner;
	owner = (u32)owner >> INVENTORY_QUANTITY_SHIFT;
	owner++;
	if (item_id == 0)
	{
		owner = 0;
	}
	return owner;
}

s32 Inventory_Count(s32 owner)
{
	struct BtlUnit *inv = UnitGet(owner);
	s32				count = 0;

	if (inv->inventory[count] != 0)
	{
		do
		{
			count++;
			if (count > 14)
				break;
		} while (inv->inventory[count] != 0);
	}
	return count;
}

s32 PartyInventory_HasSpace(void)
{
	s16	 owners[10];
	s32	 owner_count;
	s32	 owner_index;
	s16 *owner_cursor;

	if (Inventory_Count(gGameState.selected_actor) != 15)
		return 1;
	owner_count = Party_ListActiveOwners(owners);
	owner_cursor = owners;
	owner_index = 0;
	if (owner_index < owner_count)
	{
		do
		{
			if (Inventory_Count(*owner_cursor++) != 15)
				return 1;
			owner_index++;
		} while (owner_index < owner_count);
	}
	return 0;
}

s32 PartyInventory_CountFreeSlots(void)
{
	s16	 owners[10];
	s32	 owner_count = Party_ListActiveOwners(owners);
	s32	 cnt = 0;
	s16 *owner_cursor = owners;

	if (cnt < owner_count)
	{
		s32 n = owner_count;

		do
		{
			cnt = cnt - Inventory_Count(*owner_cursor++) + 15;
			n--;
		} while (n != 0);
	}
	return cnt;
}

s32 Inventory_AddItem(s32 owner_id, s32 item_id)
{
	struct BtlUnit		  *inv = UnitGet(owner_id);
	struct ItemDefinition *item = Item_GetDirect(item_id);
	s32					   slot;

	if ((item->flags & ITEM_STACKABLE) != 0)
	{
		slot = 0;
		if (((inv->inventory[slot] ^ item_id) & ITEM_ID_MASK) != 0)
		{
			do
			{
				slot++;
				if (slot >= INVENTORY_SLOTS)
					break;
			} while (((inv->inventory[slot] ^ item_id) & ITEM_ID_MASK) != 0);
		}
		if (slot != INVENTORY_SLOTS)
		{
			s32 entry = inv->inventory[slot];
			u32 count = ((u32)entry >> INVENTORY_QUANTITY_SHIFT) + 1;

			if (count > 29)
				return -1;
			{
				s32 value = INVENTORY_ENTRY_MASK;

				value &= entry;
				value |= count << INVENTORY_QUANTITY_SHIFT;
				inv->inventory[slot] = value;
			}
			return slot;
		}
	}

	slot = 0;
	do
	{
		if (inv->inventory[slot] == 0)
		{
			inv->inventory[slot] = item_id;
			return slot;
		}
		slot++;
	} while (slot < INVENTORY_SLOTS);
	return -1;
}

s32 PartyInventory_Add(s32 item_id)
{
	s16	 owners[10];
	s32	 owner_count;
	s32	 owner_index;
	s16 *owner_cursor;

	owner_count = Party_ListActiveOwners(owners);
	owner_cursor = owners;
	owner_index = 0;
	if (owner_index < owner_count)
	{
		do
		{
			s16 owner_id = *owner_cursor++;

			if (Inventory_AddItem(owner_id, item_id) >= 0)
				return owner_id;
			owner_index++;
		} while (owner_index < owner_count);
	}
	return -1;
}

s32 Inventory_Find(s32 owner, s32 item_id)
{
	struct BtlUnit *inv = UnitGet(owner);
	s32				slot = 0;
	u16			   *entry = inv->inventory;

	do
	{
		if (((*entry++) & ITEM_ID_MASK) == item_id)
		{
			return slot;
		}
		slot++;
	} while (slot < INVENTORY_SLOTS);
	return -1;
}

s32 PartyInventory_FindOwner(s32 item_id)
{
	s16	 owners[10];
	s32	 owner_count;
	s32	 owner_index;
	s16 *owner_cursor;
	s16	 owner;

	if (Inventory_Find(gGameState.selected_actor, item_id) != -1)
		return gGameState.selected_actor;
	owner_count = Party_ListActiveOwners(owners);
	owner_cursor = owners;
	owner_index = 0;
	if (owner_index < owner_count)
	{
		do
		{
			owner = *owner_cursor++;
			if (Inventory_Find(owner, item_id) != -1)
				return owner;
			owner_index++;
		} while (owner_index < owner_count);
	}
	return -1;
}

s32 Inventory_Equip(s32 owner, s32 slot)
{
	struct BtlUnit		  *inv = UnitGet(owner);
	u32					   item_id = inv->inventory[slot];
	struct ItemDefinition *item;
	u8					   type;
	s32					   other;

	if (Item_CanOwnerEquipDirect(owner, item_id) == 0)
		return -1;
	if (item_id & INVENTORY_EQUIPPED)
		return 0;

	item = Item_GetDirect(item_id);
	type = item->type;
	if (type != 6)
	{
		for (other = 0; other <= 14; other++)
		{
			if ((inv->inventory[other] & INVENTORY_EQUIPPED) == 0)
				continue;
			if (Item_GetDirect(inv->inventory[other])->type == type)
				break;
		}

		if (other != 15)
		{
			item = Item_GetDirect(inv->inventory[other]);
			if (item->flags & ITEM_CANNOT_UNEQUIP)
				return -2;
			inv->inventory[other] &= 0xfdff;
		}
	}

	inv->inventory[slot] |= INVENTORY_EQUIPPED;
	Owner_RefreshClassActions(owner);
	UnitCalc(owner);
	return 0;
}

s32 Inventory_FindEquipped(s32 owner, s32 type)
{
	struct BtlUnit		  *base = UnitGet(owner);
	s32					   index;
	struct ItemDefinition *item;

	for (index = 0; index < INVENTORY_SLOTS; index++)
	{
		if (base->inventory[index] & INVENTORY_EQUIPPED)
		{
			item = Item_GetDirect(base->inventory[index]);
			if (item->type == type)
				break;
		}
	}
	if (index == 15)
		index = -1;
	return index;
}

struct ItemDefinition *Inventory_GetEquippedDefinition(struct BtlUnit *inv, s32 type)
{
	s32					   slot;
	struct ItemDefinition *item;

	for (slot = 0; slot < INVENTORY_SLOTS; slot++)
	{
		if (inv->inventory[slot] & INVENTORY_EQUIPPED)
		{
			item = Item_GetDirect(inv->inventory[slot]);
			if (item->type == type)
			{
				return item;
			}
		}
	}
	return 0;
}

s32 Inventory_GetEquippedItem(struct BtlUnit *inv, s32 type)
{
	s32 slot;

	for (slot = 0; slot < INVENTORY_SLOTS; slot++)
	{
		if (inv->inventory[slot] & INVENTORY_EQUIPPED)
		{
			struct ItemDefinition *item = Item_GetDirect(inv->inventory[slot]);

			if (item->type == type)
			{
				return inv->inventory[slot] & ITEM_ID_MASK;
			}
		}
	}
	return 0;
}

s32 Inventory_Remove(s32 owner, s32 slot)
{
	struct BtlUnit *inv = UnitGet(owner);
	u16				item = inv->inventory[slot];
	s32				result = -1;

	if (item != 0)
	{
		if (item & INVENTORY_QUANTITY_MASK)
		{
			inv->inventory[slot] = item - INVENTORY_QUANTITY_STEP;
			result = 1;
		}
		else
		{
			s16 *list;
			s32	 count;
			s32	 i;

			inv->inventory[slot] = 0;
			list = inv->inventory;
			count = 0;
			for (i = 0; i < 15; i++)
			{
				if (list[i] != 0)
					list[count++] = list[i];
			}
			for (; count < 15; count++)
				list[count] = 0;
			result = 2;
		}
	}
	UnitCalc(owner);
	return result;
}

s32 Inventory_Discard(s32 owner, s32 slot)
{
	s32 item = UnitGet(owner)->inventory[slot];
	s32 removed_slot = Inventory_Remove(owner, slot);

	if (removed_slot != -1)
	{
		Item_AdjustCounter(item, 1);
		Event_ClearInvalidPackedValuesFar();
	}
	return removed_slot;
}

s32 Inventory_CheckDiscard(s32 owner, s32 slot)
{
	struct BtlUnit		  *inv = UnitGet(owner);
	s32					   item_id = inv->inventory[slot] & ITEM_ID_MASK;
	struct ItemDefinition *item = Item_GetDirect(item_id);

	if (item_id == 0)
	{
		return -1;
	}
	if ((item->flags & ITEM_INDISPENSABLE) != 0)
	{
		return -4;
	}
	if ((inv->inventory[slot] & INVENTORY_EQUIPPED) != 0
		&& (item->flags & ITEM_CANNOT_UNEQUIP) != 0)
	{
		return -3;
	}
	return 0;
}

s32 PartyInventory_Remove(s32 item_id)
{
	s32 owner = PartyInventory_FindOwner(item_id);

	if (owner == -1)
		return 0;
	Inventory_Remove(owner, Inventory_Find(owner, item_id));
	return 0;
}

s32 PartyInventory_Discard(s32 item_id)
{
	s32 owner = PartyInventory_FindOwner(item_id);

	if (owner == -1)
		return 0;
	Inventory_Discard(owner, Inventory_Find(owner, item_id));
	return 0;
}

s32 Inventory_Break(s32 owner, s32 slot)
{
	struct BtlUnit *inv = UnitGet(owner);
	if (inv->inventory[slot] == 0)
	{
		return -1;
	}
	inv->inventory[slot] |= INVENTORY_BROKEN;
	return 0;
}

s32 Inventory_Repair(s32 owner, s32 slot)
{
	struct BtlUnit *inv = UnitGet(owner);
	if (inv->inventory[slot] == 0)
	{
		return -1;
	}
	inv->inventory[slot] &= ~INVENTORY_BROKEN;
	return 0;
}

u8 Item_GetTargetMode(s32 item_id)
{
	return BattleAction_GetDirect(Item_GetDirect(item_id)->action_id)->target_mode;
}

s32 ItemCounter_Adjust(s32 index, s32 delta)
{
	s32 counter_slot = index;
	u8 *data = gItemCounters;

	index = 0;
	if (counter_slot <= 127)
	{
		s32 value = data[counter_slot];

		value += delta;

		if (value < 0)
		{
			value = 0;
		}
		else if (value > 99)
		{
			value = 99;
			index = 99;
		}
		else
		{
			index = value;
		}
		data[counter_slot] = value;
	}
	return index;
}

s32 Item_AdjustCounter(s32 item_id, s32 delta)
{
	s32 item_id_mask = ITEM_ID_MASK;
	u8	counter;
	s32 result = 0;

	counter = Item_ArtifactSlotTable[item_id & item_id_mask];
	if (counter != 0)
	{
		result = ItemCounter_Adjust(counter - 1, delta);
	}
	return result;
}

s32 Inventory_CountItem(s32 owner, s32 item_id)
{
	struct BtlUnit *base = UnitGet(owner);
	s32				count = 0;
	s32				target = item_id & ITEM_ID_MASK;
	s32				index = 0;

	do
	{
		if ((base->inventory[index] & ITEM_ID_MASK) == target)
		{
			struct ItemDefinition *item = Item_GetDirect(target);

			if (item->flags & ITEM_STACKABLE)
			{
				count = (base->inventory[index] >> INVENTORY_QUANTITY_SHIFT) + 1;
				break;
			}
			count++;
		}
		index++;
	} while (index < INVENTORY_SLOTS);
	return count;
}

s32 PartyInventory_CountItem(s32 item_id)
{
	u16 owners[16];
	s32 item_count = 0;
	s32 owner_count = Party_ListActiveOwners(owners);

	if (item_count < owner_count)
	{
		u16 *owner_cursor = owners;
		s32	 n = owner_count;

		do
		{
			item_count += Inventory_CountItem(*owner_cursor++, item_id);
			n--;
		} while (n != 0);
	}
	return item_count;
}

struct BtlAct *BattleAction_GetDirect(s32 action_id)
{
	u32 entry_index;

	entry_index = action_id & 0x3fff;
	if (entry_index >= 0x208U)
	{
		entry_index = 0;
	}
	return &BattleAction_DefinitionTable[entry_index];
}

s32 Equipment_HasValue(s32 owner, s32 value)
{
	struct BtlUnit		   *unit = UnitGet(owner);
	struct OwnerActionSlot *entry;
	s32						mask = 0x3fff;
	s32						index = 0;

	entry = unit->action_slots;
	do
	{
		s32 current = entry->encoded_action;

		current &= mask;
		entry++;
		if (current == value)
		{
			return 1;
		}
		index++;
	} while (index <= 31);
	return 0;
}

enum
{
	OWNER_ACTION_CLASS = 0x8000,
	OWNER_ACTION_ITEM = 0x4000
};

s32 Owner_RefreshClassActions(s32 owner)
{
	struct BtlUnit		   *st;
	u16					   *abil;
	struct ClassDefinition *cls;
	struct ItemDefinition  *def;
	s32						i;
	s32						j;
	u16						mask;
	struct ClassAbility	   *cg;

	st = UnitGet(owner);
	abil = (u16 *)st->action_slots;
	cls = Owner_GetRecordStride84(st->class_index);
	if (st->class_index == 0)
		return 0;
	mask = 0x8000;
	for (j = 0; j < 32; j++)
	{
		if (abil[j * 2] & mask)
			abil[j * 2] = 0;
	}
	mask = 0x4000;
	for (j = 0; j < 32; j++)
	{
		if (abil[j * 2] & mask)
			abil[j * 2] = 0;
	}
	for (i = 31, j = 31; i >= 0;)
	{
		if (abil[i * 2] == 0)
		{
			i--;
		}
		else
		{
			abil[j * 2] = abil[i * 2];
			i--;
			j--;
		}
	}
	for (; j >= 0; j--)
		abil[j * 2] = 0;

	mask = OWNER_ACTION_CLASS;
	for (j = 0; j < 16; j++)
	{
		cg = &cls->abilities[j];
		if (cls->abilities[j].ability == 0)
			continue;
		if (st->level < cls->abilities[j].level)
			continue;
		i = 0;
		while (i < 32 && abil[i * 2] != cls->abilities[j].ability)
			i++;
		if (i != 32)
			continue;
		for (i = 0; i < 32; i++)
		{
			if (abil[i * 2] == 0)
			{
				abil[i * 2] = cg->ability | mask;
				break;
			}
		}
		if (i == 32)
			break;
	}

	for (j = 0; j < 15; j++)
	{
		if (st->inventory[j] == 0)
			continue;
		if (!(st->inventory[j] & INVENTORY_EQUIPPED))
			continue;
		def = Item_GetDirect(st->inventory[j]);
		if (def->use_type != 3)
			continue;
		for (i = 0; i < 32; i++)
		{
			if ((abil[i * 2] & OWNER_ACTION_ID_MASK) == def->action_id)
				break;
		}
		if (i != 32)
			continue;
		for (i = 0; i < 32; i++)
		{
			if (abil[i * 2] == 0)
			{
				abil[i * 2] = def->action_id | OWNER_ACTION_ITEM;
				break;
			}
		}
		if (i == 32)
			break;
	}

	for (i = 0, j = 0; i < 32;)
	{
		if (abil[i * 2] == 0)
		{
			i++;
		}
		else
		{
			abil[j * 2] = abil[i * 2];
			i++;
			j++;
		}
	}
	for (; j < 32; j++)
		abil[j * 2] = 0;
	return 0;
}

struct LevelUpWork
{
	s32							class_index;
	s32							level;
	struct CharacterDefinition *growth;
	u8							unknown_0c[0x20];
};

s32 OwnerAction_Add(s32 state_index, s32 value)
{
	struct BtlUnit *state = UnitGet(state_index);
	s32				key = value & OWNER_ACTION_ID_MASK;
	s32				found = -1;
	s32				index;

	for (index = 0; index <= 30; index++)
	{
		s32 masked = state->action_slots[index].encoded_action & OWNER_ACTION_ID_MASK;

		if ((masked ^ key) == 0)
		{
			state->action_slots[index].encoded_action = masked;
			found = index;
			break;
		}
	}

	if (found < 0)
	{
		/* FAKEMATCH: the scalar member-address walk preserves halfword operand order; typed indexing shortens the loop by four bytes. */
		for (index = 0; index <= 30; index++)
		{
			s32 offset = index * sizeof(state->action_slots[0]) + (s32)
						 & ((struct BtlUnit *)0)->action_slots;

			if (*(u16 *)((u8 *)state + offset) == 0)
			{
				*(u16 *)((u8 *)state + offset) = key;
				found = index;
				break;
			}
		}
		if (found < 0)
		{
			return -1;
		}
	}

	Owner_RefreshClassActions(state_index);
	for (index = 0; index <= 31; index++)
	{
		if (state->action_slots[index].encoded_action == key)
		{
			break;
		}
	}
	return index;
}

struct LevelUpResult *OwnerAction_CheckLevelThreshold(s32 owner, struct LevelUpResult *value)
{
	return Owner_GetValueIfLevelThresholdReached(owner, value);
}

struct CharacterDefinition *Character_GetDefinition(s32 index)
{
	struct CharacterDefinition *base;

	base = Character_DefinitionTable;
	return &base[index];
}

void Owner_InitRecords(void)
{
	struct BtlUnit			   *state;
	struct CharacterDefinition *tmpl;
	u16							name_buf[16];
	s32							owner;
	s32						   *remote = Character_StartingEquipOwnerIds;
	s32							i;
	s32							slot;
	u8						   *name;

	for (owner = 0; owner <= 7; owner++)
	{
		state = UnitGet(owner);
		Ui_AdjustValueWithoutLimitFar(owner + (s32)MsgCharacterName, name_buf);
		name = state->name;
		name[0] = name_buf[0];
		i = 0;
		if (name_buf[0] != 0)
		{
			do
			{
				i++;
				if (i > 13)
					break;
				name[i] = name_buf[i];
			} while (name_buf[i] != 0);
		}
		state->name[14] = 0;
	}

	if (*remote != -1)
	{
		do
		{
			state = UnitGet(*remote);
			if (state != 0)
			{
				state->class_id = *remote;
				tmpl = Character_GetDefinition(state->class_id);

				for (i = 14; i >= 0; i--)
					state->inventory[i] = 0;

				for (i = 0; i < sizeof(tmpl->items) / sizeof(tmpl->items[0]); i++)
				{
					slot = Inventory_AddItem(*remote, tmpl->items[i] & 0x1ff);
					Inventory_Equip(*remote, slot);
				}

				Owner_RefreshDerivedData(*remote);
				state->ep_gauge = 0x4000;
				state->hp_gauge = 0x4000;
				Party_AdvanceOwnerCountToTarget(*remote, tmpl->starting_level);
				UnitCalc(*remote);
			}
			remote++;
		} while (*remote != -1);
	}
}

void Owner_LevelNoOp(void)
{
}

u32 Owner_GetLevelThreshold(s32 owner, s32 level)
{
	struct BtlUnit *state = UnitGet(owner);

	if (state->class_index != 0)
	{
		if (level <= 0)
		{
			return 0;
		}
		if (level <= 99 && state->class_id <= 7)
		{
			return Character_LevelExpTable[state->class_id * 99 + level - 1];
		}
	}
	return -1;
}

struct LevelUpResult *Owner_LevelUp(s32 owner, struct LevelUpResult *res)
{
	struct BtlUnit	   *st;
	struct LevelUpWork *work;
	u32					threshold;
	s16					band;
	s32					diff;
	s32					level;

	st = UnitGet(owner);
	work = Runtime_BumpAllocateAlternatePool(sizeof(struct LevelUpWork));
	work->class_index = st->class_index;
	level = st->level;
	work->level = level;
	res->level = level;
	res->ability = 0xffff;
	res->hp = 0;
	res->ep = 0;
	res->atk = 0;
	res->def = 0;
	res->agi = 0;
	res->luck = 0;
	if (level < 99)
	{
		st->level++;
		res->level = level + 1;
		threshold = Owner_GetLevelThreshold(owner, st->level);
		if (threshold != -1 && st->experience < threshold)
			st->experience = threshold;
		work->growth = Character_GetDefinition(owner);
		if (res->level == 1)
		{
			res->hp += work->growth->hp[0];
			res->ep += work->growth->ep[0];
			res->atk += work->growth->atk[0];
			res->def += work->growth->def[0];
			res->agi += work->growth->agi[0];
			res->luck += work->growth->luck[0];
		}
		band = res->level / 20;
		if (band < 0)
			band = 0;
		if (band > 4)
			band = 4;
		diff = work->growth->hp[band + 1] - work->growth->hp[band];
		res->hp += ((Random16() * 20 >> 16) + diff) / 20;
		diff = work->growth->ep[band + 1] - work->growth->ep[band];
		res->ep += ((Random16() * 20 >> 16) + diff) / 20;
		diff = work->growth->atk[band + 1] - work->growth->atk[band];
		res->atk += ((Random16() * 20 >> 16) + diff) / 20;
		diff = work->growth->def[band + 1] - work->growth->def[band];
		res->def += ((Random16() * 20 >> 16) + diff) / 20;
		diff = work->growth->agi[band + 1] - work->growth->agi[band];
		res->agi += ((Random16() * 20 >> 16) + diff) / 20;
		diff = work->growth->luck[band + 1] - work->growth->luck[band];
		res->luck += ((Random16() * 20 >> 16) + diff) / 20;
		st->base_hp += res->hp;
		st->base_ep += res->ep;
		st->base_attack += res->atk;
		st->base_defense += res->def;
		st->base_agility += res->agi;
		st->base_luck += res->luck;
		st->base_action_count = 1;
		st->base_hp_regen = 0;
		st->base_ep_regen = 0;
		Owner_RefreshClassActions(owner);
		UnitCalc(owner);
	}
	MemFree(work);
	return res;
}

struct LevelUpResult *Owner_GetValueIfLevelThresholdReached(
	s32 owner_no, struct LevelUpResult *value)
{
	struct BtlUnit *owner;

	owner = UnitGet(owner_no);
	if ((owner->experience >= Owner_GetLevelThreshold(owner_no, owner->level + 1))
		&& (Owner_LevelUp(owner_no, value) != NULL))
	{
		return value;
	}
	return 0;
}

//--------------------------------------------------------------------------

// advance_owner_count_to_target.c

// パーティ：キャラクターを指定のレベルまで上げる

//--------------------------------------------------------------------------
// 目標のレベルになるまで 1 つずつレベルアップさせ、能力を計算しなおす
// unit : キャラクター番号
// lv   : 目標のレベル
//--------------------------------------------------------------------------
void Party_AdvanceOwnerCountToTarget(s32 unit, s32 lv)
{
	u8	res[16]; // struct LevelUpResult の受け取り（使わない）
	s32 cur = UnitGet(unit)->level;

	while (cur < lv)
	{
		Owner_LevelUp(unit, (struct LevelUpResult *)res);
		cur++;
	}
	UnitCalc(unit);
}
