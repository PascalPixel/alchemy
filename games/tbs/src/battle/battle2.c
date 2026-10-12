#include "vram_tab.h"
#include "window.h"
#include "unit/battle/battle.h"
#include "unit/battle/battle2.h"
#include "unit/battle/battle3.h"
#include "unit/battle/battle4.h"
#include "unit/battle/battle5.h"
#include "unit/battle/battle6.h"
#include "unit/battle/battle7.h"
#include "unit/battle/battle8.h"
#include "unit/battle/battle9.h"
#include "unit/battle/battle10.h"
#include "unit/battle/battle11.h"
#include "unit/battle/battle12.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object2.h"
#include "unit/battle/effect/object3.h"
#include "unit/battle/effect/object4.h"
#include "unit/battle/effect/object5.h"
#include "unit/battle/effect/object6.h"
#include "unit/battle/effect/object7.h"
#include "unit/battle/effect/object8.h"
#include "unit/battle/effect/object9.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object11.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime2.h"
#include "unit/system/scheduler/runtime3.h"
#include "unit/system/scheduler/runtime4.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim12.h"
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

s32 BattleParty_PrepareActiveOwners(u16 *owners)
{
	s32 limit;
	s32 count;
	s32 index;

	limit = 4;
	if (BtlWk->two_sided != 0)
		limit = 3;

	count = Party_CountActiveOwnersFar();
	if (count > limit)
		count = limit;

	for (index = 0; index < count; index++)
	{
		s32 owner = gGameState.active_owners[index];

		if (owners != 0)
			*owners++ = owner;
		UnitGetFar(owner)->side = BATTLE_UNIT_PARTY;
	}

	if (owners != 0)
		*owners = BATTLE_UNIT_LIST_END;
	return count;
}

s32 BtlEnmList(s16 *unit_ids)
{
	s16 *output;
	s32	 battle_result;
	s32	 id;
	s32	 entry_limit;
	s32	 entry_count;

	output = unit_ids;
	entry_count = 0;
	entry_limit = 6;
	battle_result = 0;
	if (output != NULL)
	{
		if (GameFlag_TestFar(0x16C) != 0)
		{
			entry_limit = 3;
		}
		id = 0x80;
		entry_limit += 0x80;
		for (; id < entry_limit; id += 1)
		{
			if (UnitGetFar(id)->side != 0)
			{
				*output = id;
				entry_count += 1;
				output += 1;
			}
		}
		*output = BATTLE_UNIT_LIST_END;
		battle_result = entry_count;
	}
	return battle_result;
}

s32 BattleParty_ListLivingUnits(s32 side_mask, u16 *unit_ids)
{
	u16				active_members[8];
	u16			   *base;
	s32				remaining;
	s32				enemy_limit;
	u16			   *output;
	s32				living_count;
	s32				enemy_capacity;
	u16			   *member;
	s32				unit_id;
	s16				hp;
	s32				active_count;
	struct BtlUnit *unit;

	output = unit_ids;
	living_count = 0;
	enemy_capacity = 6;
	if (GameFlag_TestFar(0x16C) != 0)
	{
		enemy_capacity = 3;
	}
	if (side_mask & BATTLE_SIDE_PARTY)
	{
		base = active_members;
		active_count = BattleParty_PrepareActiveOwners(base);
		if (living_count < active_count)
		{
			member = base;
			remaining = active_count;
			do
			{
				unit_id = *member;
				member += 1;
				hp = UnitGetFar(unit_id)->hp;
				if (hp > 0)
				{
					if (output != NULL)
					{
						*output = unit_id;
						output += 1;
					}
					living_count += 1;
				}
				remaining -= 1;
			} while (remaining != 0);
		}
	}
	if (side_mask & BATTLE_SIDE_ENEMIES)
	{
		remaining = 0x80;
		enemy_limit = enemy_capacity + 0x80;
		if (remaining < enemy_limit)
		{
			do
			{
				unit = UnitGetFar(remaining);
				if ((unit->side != 0) && (unit->hp > 0))
				{
					if (output != NULL)
					{
						*output = remaining;
						output += 1;
					}
					living_count += 1;
				}
				remaining += 1;
			} while (remaining < enemy_limit);
		}
	}
	if (output != NULL)
	{
		*output = BATTLE_UNIT_LIST_END;
	}
	return living_count;
}

s32 BattleParty_ListActorIds(s32 groups, u16 *dst)
{
	struct BtlWork *order;
	s32				count;
	s32				i;

	order = BtlWk;
	count = 0;
	if (groups & BATTLE_SIDE_PARTY)
	{
		for (i = 0; order->party_units[i] != BATTLE_UNIT_LIST_END; i++)
		{
			if (order->party_units[i] != BATTLE_UNIT_REMOVED)
			{
				if (dst != NULL)
					*dst++ = order->party_units[i];
				count++;
			}
		}
	}
	if (groups & BATTLE_SIDE_ENEMIES)
	{
		for (i = 0; order->enm_no[i] != BATTLE_UNIT_LIST_END; i++)
		{
			if (order->enm_no[i] != BATTLE_UNIT_REMOVED)
			{
				if (dst != NULL)
					*dst++ = order->enm_no[i];
				count++;
			}
		}
	}
	if (dst != NULL)
		*dst = BATTLE_UNIT_LIST_END;
	return count;
}

//--------------------------------------------------------------------------
// actor_commit_placement.c
// バトル：全ユニットのオブジェクト配置
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 戦闘中の全ユニットの絵を用意し（いなくなったユニットの絵は解放）、
// 全員の表示モードと動作を設定し直す
//--------------------------------------------------------------------------
void BtlObjPut(void)
{
	s16 list[14];

	BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
	BattleActor_SpawnObjectsForList(list, 1);
}

//--------------------------------------------------------------------------
// list_all_units_and_submit.c
// バトル：全ユニットのオブジェクト配置
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 戦闘中の全ユニットの絵を用意する（いなくなったユニットの絵は解放）
//--------------------------------------------------------------------------
void BattleParty_ListAllUnitsAndSubmit(void)
{
	s16 list[14];

	BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
	BattleActor_SpawnObjectsForList(list, 0);
}

s32 BattleMotion_GetSlotPalette(s32 id)
{
	return BtlObjGet(id)->palette;
}

s32 EnmChk(s32 summon)
{
	struct BtlWork *ptr;
	s32				retval;
	s32				i;

	retval = Summon_IsEntryFlagged(summon);
	ptr = BtlWk;
	for (i = 0; i <= 5; i++)
	{
		if (ptr->sprite_slots[i] != 0)
			continue;
		if (retval != 0)
			break;
		if (i <= 4 && ptr->sprite_slots[i + 1] == 0)
			break;
	}
	return i != 6;
}

s32 SummonSlot_RegisterActorSprites(s32 unit)
{
	s32				pass;
	struct BtlWork *table = BtlWk;
	struct BtlUnit *actor = UnitGetFar(unit);
	s32				single_slot = Summon_IsEntryFlagged(actor->class_id);
	/* FAKEMATCH: initializing the two halves preserves the word copy into pass, mov r4, r10; one s32 zero folds that copy into movs r4, #0 (one instruction differs). */
	union
	{
		s32 value;
		u16 halves[2];
	} result;
	s32 sprite_value;

	result.halves[0] = 0;
	result.halves[1] = 0;
	sprite_value = Summon_GetEntryValue(actor->class_id);
	pass = result.value;
	do
	{
		s32 slot;

		if (actor->class_index != 0)
			continue;

		for (slot = 0; slot <= 5; slot++)
		{
			if (table->sprite_slots[slot] != 0)
				continue;
			if (single_slot)
				break;
			if (slot <= 4 && table->sprite_slots[slot + 1] == 0)
				break;
		}

		if (slot == 6)
			break;

		{
			s32 flag = Summon_GetEntryFlag1Field(actor->class_id);
			s32 buffer_addr = (slot << 14) + (s32)Ram_ActorSpriteSlots;

			if (ResourceSlot_LoadFar(slot, (u32 *)buffer_addr, sprite_value + pass, flag) == 0)
				return 0;
		}

		/* FAKEMATCH: cancelling updates retain sprite_value's allocation
		 * priority; a self-assignment instead differs in 18 rows. */
		sprite_value ^= 1;
		sprite_value ^= 1;
		if (pass == 0)
			result.value = (slot << 12) | sprite_value;

		table->sprite_slots[slot] = unit;
		if (!single_slot)
			table->sprite_slots[slot + 1] = unit;

		if (sprite_value != 476 && sprite_value != 483)
			break;
	} while (++pass <= 1);

	return result.value;
}

s32 BattleMotion_ReleaseObjectSlotByValue(s32 value)
{
	struct BtlWork *work = BtlWk;
	s32				index;

	for (index = 0; index <= 5; index++)
	{
		if (work->sprite_slots[index] == value)
		{
			ResourceSlot_LoadFar(index, 0, 0, 0);
			work->sprite_slots[index] = 0;
		}
	}
}

s32 Resource_FindFreeSlot(s32 key)
{
	struct SlotAssignmentView
	{
		volatile u16 entries[52];
	};
	s32 index;
	s32 entry;
	u32 value;
	s32 result;

	for (index = 0;; index++)
	{
		entry = Resource_SlotAssignments[index];
		if (key == (entry & 0x1ff))
		{
			/* FAKEMATCH: the volatile array member preserves the native second ldrh [table, index]. A scalar pointer needs an extra address add; a nonvolatile member reuses the first load. */
			value = ((struct SlotAssignmentView *)Resource_SlotAssignments)->entries[index];
			result = value >> 9;
			goto done;
		}
		if ((s16)entry == -1)
			break;
	}
	result = 6;
done:
	return result;
}

s32 BattleUnit_LookupWeaponValueByClass(s32 id)
{
	struct BtlUnit *state;
	s32				entry;
	s32				result;

	state = UnitGetFar(id);
	entry = Inventory_FindEquippedFar(id, 1);
	result = 0;
	if (entry >= 0)
	{
		s32 sel;

		sel = Resource_FindFreeSlot(state->inventory[entry] & 0x1FF);
		switch (state->class_id)
		{
		case ROBIN:
			result = BattleUnit_WeaponAnimsClass0[sel];
			break;
		case GERALD:
			result = BattleUnit_WeaponAnimsClass1[sel];
			break;
		case IWAN:
			result = BattleUnit_WeaponAnimsClass2[sel];
			break;
		case MEARI:
			result = BattleUnit_WeaponAnimsClass3[sel];
			break;
		case GARUSHIA:
			break;
		case JASMINE:
			result = BattleUnit_WeaponAnimsClass5[sel];
			break;
		}
	}
	return result;
}

void BtlObjMake(struct BtlObj *actor, s32 unit, s32 x, s32 y)
{
	s32					 fixed_x;
	s32					 fixed_y;
	s32					 actor_flag;
	s32					 sprite;
	s32					 existing_sprite;
	struct BtlUnit		*unit_record;
	struct MotionObject *object;
	s32					 position;
	s32					 anim;
	u8					 class_id;

	fixed_x = x << 16;
	fixed_y = y << 16;
	object = Object_CreateFar(0xf000, fixed_x, 0, fixed_y);
	unit_record = UnitGetFar(unit);
	actor_flag = 0;
	existing_sprite = SummonSlot_RegisterActorSprites(unit);

	if (unit_record->class_index == 0)
	{
		sprite = Summon_GetEntryValue(unit_record->class_id);
		if (existing_sprite == 0)
			actor_flag = Summon_GetEntryFlag1Field(unit_record->class_id);
		else
			sprite = existing_sprite;
	}
	else
	{
		switch (unit_record->class_id)
		{
		case GERALD:
			sprite = BTL_CHARA(GERALD);
			break;
		case MEARI:
			sprite = BTL_CHARA(MEARI);
			break;
		case IWAN:
			sprite = BTL_CHARA(IWAN);
			break;
		case JASMINE:
			sprite = BTL_CHARA(JASMINE);
			break;
		case ROBIN:
		default:
			sprite = BTL_CHARA(ROBIN);
			break;
		}
		if (unit < 0 || unit > 7)
			actor_flag = 1;
	}

	actor->scale = 0x10000;
	switch (unit_record->class_id)
	{
	case 78:
		actor->scale = 0x19999;
		break;
	case 89:
		actor->scale = 0x18ccc;
		break;
	case 130:
		actor->scale = 0x13333;
		break;
	case 131:
		actor->scale = 0x19999;
		break;
	case 138:
		actor->scale = 0x18000;
		break;
	case 147:
		actor->scale = 0x1cccc;
		break;
	case 149:
		actor->scale = 0x1cccc;
		break;
	case 29:
		actor->scale = 0x10000;
		break;
	case 121:
		actor->scale = 0x1b333;
		break;
	case 148:
		actor->scale = 0x18000;
		break;
	case 150:
		actor->scale = 0x18000;
		break;
	case 151:
		actor->scale = 0x18000;
		break;
	case 152:
		actor->scale = 0x18000;
		break;
	case 153:
		actor->scale = 0x18000;
		break;
	case 154:
		actor->scale = 0x18000;
		break;
	case 155:
		actor->scale = 0x18000;
		break;
	case 156:
		actor->scale = 0x18000;
		break;
	case 157:
		actor->scale = 0x18000;
		break;
	case 47:
		actor->scale = 0x13333;
		break;
	case 48:
		actor->scale = 0x13333;
		break;
	case 49:
		actor->scale = 0x16666;
		break;
	case 84:
		actor->scale = 0x10000;
		break;
	case 85:
		actor->scale = 0x14000;
		break;
	case 128:
		actor->scale = 0x16666;
		break;
	case 129:
		actor->scale = 0x16666;
		break;
	case 94:
		actor->scale = 0x18000;
		break;
	case 98:
		actor->scale = 0x14ccc;
		break;
	case 110:
		actor->scale = 0x13333;
		break;
	case 132:
		actor->scale = 0x10ccc;
		break;
	case 133:
		actor->scale = 0x10ccc;
		break;
	case 134:
		actor->scale = 0x11999;
		break;
	case 135:
		actor->scale = 0x11999;
		break;
	case 136:
		actor->scale = 0x13333;
		break;
	case 137:
		actor->scale = 0x13333;
		break;
	case 141:
		actor->scale = 0x18000;
		break;
	case 144:
		actor->scale = 0x13333;
		break;
	case 145:
		actor->scale = 0x18000;
		break;
	case 146:
		actor->scale = 0x18ccc;
		break;
	case 52:
		actor->scale = 0x14000;
		break;
	case 105:
		actor->scale = 0x14000;
		break;
	case 18:
	case 19:
	case 20:
	case 21:
	case 30:
	case 68:
	case 69:
	case 70:
	case 92:
	case 122:
	case 123:
	case 124:
	case 125:
	case 126:
		actor->scale = 0xe666;
		break;
	}

	actor->object = object;
	actor->anchor_x = fixed_x;
	actor->anchor_z = fixed_y;
	actor->palette = actor_flag;
	actor->resource = sprite;
	anim = BattleUnit_LookupWeaponValueByClass(unit);
	actor->animation = 0;
	actor->animation_entry = 0;
	actor->effect_entry = 0;
	actor->active = 0;
	actor->fading = 0;
	actor->effect = 0x1fe;
	class_id = unit_record->class_id;
	actor->overlay = anim;

	if (class_id <= GERALD && Inventory_GetEquippedItemFar(unit_record, 1) == 15)
	{
		if (unit_record->class_id == ROBIN)
		{
			sprite = 480;
			actor->resource = sprite;
		}
		else
		{
			sprite = 482;
			actor->resource = sprite;
		}
		actor->overlay = 0;
	}

	position = ArcTan2(y / 8, x) + 0x8000;
	object->angle = position;
	object->unknown_59 = 3;
	object->motion_flags = 2;
	if (unit_record->class_index == 0)
	{
		object->scale_x = 0x14ccc;
		object->scale_y = 0x14ccc;
	}
	else
	{
		object->scale_x = 0x10000;
		object->scale_y = 0x10000;
	}
	ObjectDispatch_InitializeFar((struct DispatchObject *)object, BattlePres_ActorObjectScript);
}

void BattlePlacement_GetStepPair(s32 index, s32 *x, s32 *y)
{
	index *= 2;
	*x = BattlePlacement_StepPairs[index];
	*y = BattlePlacement_StepPairs[index + 1];
}

void Summon_LayoutPositions(u16 *actor_ids, s32 count, s32 *x_positions, s32 *z_positions)
{
	s32 total_spacing = count <= 4 ? 30 : 27;
	s32 z = ((count - 1) * total_spacing) / 2;
	s32 index;

	for (index = 0; index != count; index++)
	{
		s32 spacing = 0;

		x_positions[index] = -80;
		if (index != 0)
		{
			struct BtlUnit *actor;

			spacing = 25;
			if ((u16)(actor_ids[index] - 254) > 1)
			{
				actor = UnitGetFar(actor_ids[index]);
				spacing = Summon_IsEntryFlagged(actor->class_id) ? 27 : 38;
				if (actor->class_id == 148 || actor->class_id == 121)
				{
					x_positions[index] = -50;
				}
			}
		}
		z -= spacing / 2;
		z_positions[index] = z;
		spacing = 25;
		if ((u16)(actor_ids[index] - 254) > 1)
		{
			struct BtlUnit *actor = UnitGetFar(actor_ids[index]);

			spacing = Summon_IsEntryFlagged(actor->class_id) ? 27 : 38;
		}
		z -= spacing / 2;
	}
}

s32 EnmSlotGet(void)
{
	s32 i;
	s32 id;

	for (i = 0; i <= 5; i++)
	{
		id = i + 0x80;
		if (UnitGetFar(id)->side == 0)
			break;
	}
	if (i == 6)
		return -1;
	return id;
}

s32 EnmUpd(void)
{
	struct BtlWork *work = BtlWk;
	u16				unit_ids[14];
	s32				x[6];
	s32				z[6];
	s32				count;
	s32				i;

	for (i = 0; i < 6 && work->enm_no[i] != 0xff; i++)
		unit_ids[i] = work->enm_no[i];
	count = i;
	Summon_LayoutPositions(unit_ids, count, x, z);
	for (i = 0; i < count; i++)
	{
		s32 id = work->enm_no[i];

		if (id != 0xfe)
		{
			struct BtlObj *object = BtlObjGet(id);

			object->anchor_x = x[i] << 16;
			object->anchor_z = z[i] << 16;
		}
	}
}

void BattleUnit_RefreshPlacement(void)
{
	struct BtlWork *work;
	u16				ids[14];
	s32				x[6];
	s32				z[6];
	s32				count;
	s32				i;
	s32				place;
	s32				stand;
	s32				n;
	s32				value;
	u8			   *p;

	place = 0;
	stand = 0;
	work = BtlWk;
	count = BattleParty_PrepareActiveOwners(ids);
	value = 0xff;
	for (i = 13; i >= 0; i--)
		work->placement[i] = value;
	p = &work->placement[13];
	for (n = 5, value = 13; n >= 0; n--, value--)
		*p-- = value;
	for (i = 0; i < count; place++, i++)
	{
		s32 id = ids[i];

		work->placement[id] = place;
		BtlObjMake(BtlObjGet(id), id, BattlePlacement_StepPairs[place * 2],
			BattlePlacement_StepPairs[place * 2 + 1]);
	}
	for (i = 0; i < 6 && work->enm_no[i] != 0xff; i++)
		ids[i] = work->enm_no[i];
	count = i;
	Summon_LayoutPositions(ids, count, x, z);
	for (i = 0; i < count; i++, stand++)
	{
		s32 id = work->enm_no[i];

		if (id != 0xfe)
			BtlObjMake(BtlObjGet(id), id, x[stand], z[stand]);
	}
}

//--------------------------------------------------------------------------
// contains_id.c
// バトル：ユニット番号リストの検索
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 並び番号のユニットがリストにいれば 1 を返す
// list : ユニット番号のリスト（0xff で終わる、最大 14）
// no   : 並び番号（0-7 味方、8-13 敵）
//--------------------------------------------------------------------------
s32 BattlePlacement_ContainsId(s16 *list, s32 no)
{
	u32 i;

	if (no > 7)
		no += 120; // 敵のユニット番号は 128 から
	i = 0;
	do
	{
		u16 id = *list++;

		if (id == 0xff)
			break;
		if (id == no)
			return 1;
	} while (++i < 14);
	return 0;
}

//--------------------------------------------------------------------------
// battle.c
// バトル：キャラの表示の優先度、状態アイコン、ポーズ
//--------------------------------------------------------------------------

#define UNIT_MAX 14 // 敵味方を合わせた人数

//--------------------------------------------------------------------------
// 状態アイコンを進め、カメラの向きで味方と敵の表示の優先度を決める
//--------------------------------------------------------------------------
void BattleActor_UpdateSpritePriorities(void)
{
	u16						 ids[UNIT_MAX];
	s32						 pri[2]; // 味方, 敵
	s32						 i;
	s32						 j;
	s32						 count;
	struct MotionObject		*obj;
	struct AnimationObject	*rec;
	struct AnimationObject **list;

	// 全員の状態アイコン
	BattleParty_ListActorIds(BATTLE_SIDE_BOTH, ids);
	i = 0;
	if (ids[i] != BATTLE_UNIT_LIST_END)
	{
		for (;;)
		{
			struct BtlObj *slot = BtlObjGet(ids[i]);

			if (slot != 0)
			{
				obj = slot->object;

				BattleStatusIcon_Cycle(slot);
				if (slot->effect_entry != 0)
				{
					rec = GetMotionRecord(obj, 0);

					if (rec != 0)
					{
						struct SpriteEntry *efx;
						s32					p = 0;

						if (obj->y != 0) // 高さがあれば
							p = 9;
						efx = slot->effect_entry;
						if (efx->priority != p)
						{
							efx->priority = p;
							rec->dirty = 1;
						}
					}
				}
			}
			i++;
			if (i >= UNIT_MAX || ids[i] == BATTLE_UNIT_LIST_END)
				break;
		}
	}

	// カメラの向きで、味方と敵の優先度を入れ替える
	if ((s16)gCameraWork->yaw >= 0)
	{
		pri[0] = 1;
		pri[1] = 2;
	}
	else
	{
		pri[0] = 2;
		pri[1] = 1;
	}

	// 味方
	{
		count = BattleParty_ListActorIds(BATTLE_SIDE_PARTY, ids);
		for (i = 0; i < count; i++)
		{
			struct BtlObj *slot = BtlObjGet(ids[i]);

			if (slot != 0)
			{
				obj = slot->object;

				switch (obj->record_storage_kind & 15)
				{
				case 1: // 一枚
				{
					rec = obj->records;
					rec->part[0].priority = pri[0];
					break;
				}
				case 2: // 四枚までの表
				{
					list = (struct AnimationObject **)obj->records;
					for (j = 0; j < 4; j++)
					{
						rec = *list++;
						if (rec != 0)
							rec->part[0].priority = pri[0];
					}
					break;
				}
				}
			}
		}
	}

	// 敵
	{
		count = BattleParty_ListActorIds(BATTLE_SIDE_ENEMIES, ids);
		for (i = 0; i < count; i++)
		{
			struct BtlObj *slot = BtlObjGet(ids[i]);

			if (slot != 0)
			{
				obj = slot->object;

				switch (obj->record_storage_kind & 15)
				{
				case 1:
				{
					rec = obj->records;
					rec->part[0].priority = pri[1];
					break;
				}
				case 2:
				{
					list = (struct AnimationObject **)obj->records;
					for (j = 0; j < 4; j++)
					{
						rec = *list++;
						if (rec != 0)
							rec->part[0].priority = pri[1];
					}
					break;
				}
				}
			}
		}
	}
}

//--------------------------------------------------------------------------
// 状態アイコンのビット
//--------------------------------------------------------------------------
#define ICON_DOKU 0x01		// 毒
#define ICON_MOUDOKU 0x02	// もう毒
#define ICON_SHIBIRE 0x04	// しびれ
#define ICON_FUUJI 0x08		// エナジーふうじ
#define ICON_NEMURI 0x10	// ねむり
#define ICON_MABOROSHI 0x20 // まぼろし
#define ICON_AKURYO 0x40	// あくりょう
#define ICON_SHIRYO 6		// しりょう：残りの数 1-7 が bit 7-13

#define ICON_MAX 14 // アイコンの種類

//--------------------------------------------------------------------------
// 状態異常から状態アイコンのビットを作る
//--------------------------------------------------------------------------
s32 BattleUnit_BuildStatusFlags(s32 id, struct BtlObj *slot)
{
	struct BtlUnit *unit = UnitGetFar(id);
	s8				doku = unit->doku;
	u32				flags = 0;

	if (doku == 1)
		flags = ICON_DOKU;
	if (doku == 2)
		flags |= doku; // もう毒 (ICON_MOUDOKU)
	if (unit->maboroshi != 0)
		flags |= ICON_MABOROSHI;
	if (unit->shibire != 0)
	{
		s32 cls;
		flags |= ICON_SHIBIRE;
		cls = unit->class_id;
		if (cls == 121 || cls == 148) // この二体には出さない
			flags &= ~ICON_SHIBIRE;
	}
	if (unit->fuuji != 0)
		flags |= ICON_FUUJI;
	if (unit->akuryo != 0)
		flags |= ICON_AKURYO;
	if (unit->nemuri != 0)
		flags |= ICON_NEMURI;
	if (unit->shiryo != 0)
		flags |= 1 << (unit->shiryo + ICON_SHIRYO);
	slot->active_conditions = flags;
}

//--------------------------------------------------------------------------
// 状態アイコンを 80 フレームごとに次の状態へ切り替える
//--------------------------------------------------------------------------
s32 BattleStatusIcon_Cycle(struct BtlObj *slot)
{
	struct AnimationEntry  *old;
	struct AnimationEntry  *icon;
	struct AnimationObject *rec;
	struct MotionObject	   *owner;
	s32						anim;
	s32						handle;
	s32						prev;
	s32						changed = 0;

	if (slot->cycle_timer >= 0)
		slot->cycle_timer--;

	// 表示中の状態が消えたら、すぐに切り替える
	old = (struct AnimationEntry *)slot->animation_entry;
	if (old == 0)
	{
		if ((s16)slot->active_conditions == 0)
			goto wait;
		goto update;
	}
	if ((((s16)slot->active_conditions >> slot->selected_condition) & 1) == 0)
		goto update;
wait:
	if (slot->cycle_timer != 0)
		goto done;

update:
	anim = -1;
	owner = slot->object;
	if ((s16)slot->active_conditions != 0)
	{
		prev = slot->selected_condition;
		for (anim = prev + 1;; anim++)
		{
			if (anim > ICON_MAX - 1)
				anim = 0;
			if ((((s16)slot->active_conditions >> anim) & 1) != 0)
				break;
		}

		if (prev != anim || old == 0)
		{
			slot->selected_condition = anim;
			changed = 1;
		}
		slot->cycle_timer = 80;
	}
	else
	{
		changed = 1;
	}

	rec = GetMotionRecord(owner, 0);
	if (rec == 0)
		goto done;

	// アイコンのアニメ番号は、幅 32 のキャラと他とで別の並び
	if (anim >= 0)
	{
		if (rec->width == 32)
			anim += 340;
		else
			anim += 355;
	}

	if (slot->animation_entry != 0 && changed != 0)
	{
		ResourceMetadata_UnregisterFar(rec, (s32)slot->animation_entry);
		slot->animation_entry = 0;
	}

	if (anim >= 0 && changed != 0)
	{
		// The handle is stored first and cleared again when registration fails.
		handle = ResourceMetadata_RegisterFar(rec, anim);
		slot->animation_entry = (struct SpriteEntry *)handle;
		if (handle == -1)
			slot->animation_entry = 0;
		icon = (struct AnimationEntry *)slot->animation_entry;
		if (icon != 0)
		{
			icon->priority = 3;
			Animation_SetWorkEntryFar(icon, 0);
		}
	}

	rec->dirty = 1;
	if (anim >= 0)
		slot->animation = anim;
	else
		slot->animation = 0;

done:;
}

//--------------------------------------------------------------------------
// 状態に合わせてキャラのポーズを決める
// 動けなければ味方 4・敵 0、倒れていれば味方 4・敵 5、ほかは 1
//--------------------------------------------------------------------------
s32 BattlePres_SetActorModeAndAction(s32 id)
{
	struct BtlUnit *unit;
	s32				mode;

	unit = UnitGetFar(id);
	mode = 1;
	if (unit->hp != 0)
	{
		if (unit->nemuri != 0 || unit->shibire != 0 || unit->miugoki != 0)
			mode = (unit->side != BATTLE_UNIT_ENEMY) * 4;
	}
	else
	{
		s32 mikata = unit->side ^ mode;
		mode = (u32)(-mikata | mikata) >> 31;
		mode = 5 - mode;
	}

	ObjectDispatch_ApplyArgumentToChildrenFar(BtlObjGet(id)->object, mode);
	ObjectDispatch_ApplyValueToChildrenFar(BtlObjGet(id)->object, (id & 3) + 14);
}

//--------------------------------------------------------------------------
// release_battle_object_records.c
// バトル：オブジェクトの絵の解放
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// オブジェクトが持つ絵をすべて解放し、絵なしにもどす
// no : オブジェクト番号
//--------------------------------------------------------------------------
void ReleaseBattleObjectRecords(s32 no)
{
	struct BtlObj		*slot;
	struct MotionObject *obj;
	s32					 i;
	void				*rec;

	slot = BtlObjGet(no);
	if (slot != NULL)
	{
		obj = slot->object;
		if (obj != NULL)
		{
			slot->animation_entry = 0;
			slot->effect_entry = 0;
			i = 0;
			while ((rec = GetMotionRecord(obj, i)) != NULL)
			{
				ResourceObject_ReleaseFar(rec);
				i += 1;
			}
			obj->record_storage_kind = 0; // 絵なし
			obj->records = NULL;
		}
	}
}

void BattleActor_SpawnObjectsForList(s16 *list, s32 refresh)
{
	s32						 i;
	s32						 id;
	struct BtlObj			*slot;
	struct MotionObject		*object;
	struct AnimationObject	*res;
	struct SpriteEntry		*entry;
	struct AnimationObject **objects;
	s32						 resource;
	s32						 object_id;

	for (i = 0; i <= 13; i++)
	{
		if (!BattlePlacement_ContainsId(list, i))
		{
			object_id = i + 120;
			if (i <= 7)
				object_id = i;
			ReleaseBattleObjectRecords(object_id);
		}
	}
	if (gSchedulerStatus == 0)
		WaitFrames(1);
	for (i = 0; i <= 13 && (id = list[i]) != 255; i++)
	{
		if (id == 254)
			continue;
		slot = BtlObjGet(id);
		if (slot == 0)
			continue;
		BattleUnit_BuildStatusFlags(id, slot);
		object = slot->object;
		if (object == 0)
			continue;
		if (object->record_storage_kind != 0)
			continue;
		if ((slot->resource & 0xfff) == 476 || (slot->resource & 0xfff) == 483)
		{
			objects = (struct AnimationObject **)gMenuCtrlWork->unknown_08 + gMenuCtrlWork->count;
			resource = slot->resource;
			object->record_storage_kind = 2;
			object->records = objects;
			Iwram_ClearWords(objects, 16);
			res = ResourceObject_CreateFar(resource);
			if (res != 0)
			{
				res->scale = Iwram_MulQ16(res->scale, slot->scale);
				object->height = Resource_GetMetadataRecordFar(resource)->box_y >> 1;
				*objects = res;
				objects++;
			}
			res->flags = 0;
			res = ResourceObject_CreateFar(resource + 0x2001);
			if (res != 0)
			{
				res->scale = Iwram_MulQ16(res->scale, slot->scale);
				*objects = res;
			}
			res->flags = 0;
		}
		else
		{
			res = ResourceObject_CreateFar(slot->resource);
			if (res != 0)
			{
				object->record_storage_kind = 1;
				object->records = res;
				res->scale = Iwram_MulQ16(res->scale, slot->scale);
				entry = (struct SpriteEntry *)res->entries[0];
				entry->priority = 1;
				entry->palette = slot->palette;
				if ((resource = slot->overlay) != 0)
				{
					entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(res, resource);
					entry->priority = 1;
				}
				if ((resource = slot->animation) != 0)
				{
					entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(res, resource);
					slot->animation_entry = entry;
					Animation_SetWorkEntryFar((struct AnimationEntry *)entry, 0);
					entry->priority = 3;
				}
				if ((resource = slot->effect) != 0)
				{
					if (res->width == 32)
						resource = 0x1ff;
					entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(res, resource);
					slot->effect_entry = entry;
					entry->priority = 0;
					res->flags = 0;
				}
			}
		}
		BattlePres_SetActorModeAndAction(id);
	}
	if (refresh)
	{
		i = 0;
		if ((resource = list[0]) != 255)
		{
			for (;;)
			{
				if (list[i] != 254 && (slot = BtlObjGet(resource)) != 0
					&& (object = slot->object) != 0)
					BattlePres_SetActorModeAndAction(resource);
				i++;
				if (i >= 14 || (resource = list[i]) == 255)
					break;
			}
		}
	}
}

struct BtlObj *BtlObjGet(s32 object_id)
{
	struct BtlWork *work = BtlWk;
	struct BtlObj  *slots = work->slots;
	s32				index;
	if (object_id > 7)
	{
		object_id -= 0x78;
	}
	index = object_id;
	if (work->placement[index] == 0xFF)
	{
		return NULL;
	}
	return &slots[work->placement[index]];
}

void ResetMotionRecordGroup(void *owner)
{
	struct AnimationObject *object = owner;
	s32						i;

	if (object != NULL)
	{
		for (i = 0; i < 4; i++)
		{
			if (object->entries[i] != NULL)
				object->entries[i]->script = NULL;
		}
	}
}

void ResetBattleObjectRecordGroups(struct MotionObject *object)
{
	s32	   storage_kind;
	s32	   remaining;
	void **record_groups;
	void  *record_group;

	if (object != NULL)
	{
		storage_kind = object->record_storage_kind & 0xF;
		switch (storage_kind)
		{
		case 1:
			ResetMotionRecordGroup(object->records);
			return;
		case 2:
			record_groups = object->records;
			remaining = 3;
			do
			{
				record_group = *record_groups++;
				if (record_group != NULL)
				{
					ResetMotionRecordGroup(record_group);
				}
				remaining -= 1;
			} while (remaining >= 0);
			break;
		}
	}
}

s32 ActivateBattleObjectSlot(s32 object_id)
{
	struct BtlObj *slot;
	BattleMotion_ReleaseObjectSlotByValue(object_id);
	slot = BtlObjGet(object_id);
	slot->active = 1;
	return 0;
}

void *BattleMotion_DestroyAllSlotObjects(void)
{
	// 戻り値の型はポインタだが return 文は無い（pop {r1}; bx r1）
	s32			   no;
	s32			   i;
	struct BtlObj *slot;

	i = 0;
	do
	{
		no = i + 0x78;
		if (i <= 7)
		{
			no = i;
		}
		slot = BtlObjGet(no);
		if ((slot != NULL) && (slot->active != 0))
		{
			ObjectDispatch_ReleaseFar((s32)slot->object);
			slot->object = NULL;
			slot->active = 0;
		}
		i += 1;
	} while (i <= 0xD);
}

s32 Battle_GetWorkEntryPair(s32 no, struct FixedPointPosition *out)
{
	struct BtlWork *state = BtlWk;

	out->x = state->slots[no].anchor_x;
	out->y = 0;
	out->z = state->slots[no].anchor_z;
	return 0;
}

s32 Camera_ApplyTransformByFlag(void)
{
	struct BattleCamera *state = gCameraWork;
	Render_ResetTransformState();
	if (GameFlag_TestFar(0x16B) != 0)
	{
		Iwram_TransformMatrix((s32 *)Camera_FlagTransformWork);
		Graphics_PrepareTransferAndRun(state->eye, state->pos);
	}
	else
	{
		Graphics_PrepareTransferInIwramWork(state->eye, state->pos);
	}
}

s32 BattleMotion_ProjectPosition(s32 id, s32 *projected)
{
	struct MotionObject	   *object = BtlObjGet(id)->object;
	struct AnimationObject *record = GetMotionRecord(object, 0);
	s32						position[3];
	s32						scaled;

	Camera_ApplyTransformByFlag();
	position[0] = object->x;
	position[1] = object->y;
	position[2] = object->z;
	scaled = Render_ProjectPoint(position, projected);
	(void)Iwram_MulQ16(scaled, record->scale);
	return 0;
}

void *GetMotionRecord(struct MotionObject *object, s32 record_index)
{
	s32 storage_kind = object->record_storage_kind & 0xF;
	if (storage_kind == 1)
	{
		if (record_index == 0)
		{
			return object->records;
		}
	}
	else if (storage_kind == 2)
	{
		return ((void **)object->records)[record_index];
	}
	return NULL;
}

void Camera_InitDefaultTransform(void)
{
	struct BattleCamera		 *state = gCameraWork;
	struct FixedPointPosition transfer;

	state->yaw = 192 << 6;
	state->pitch = 254 << 8;
	state->distance = 255 << 17;
	state->pos[0] = 0;
	state->pos[1] = 0;
	state->pos[2] = 0;
	state->follow_pos = 0;
	state->unknown_18 = 0;

	Render_ResetTransformState();
	SceneTransform_ApplyPosition(&state->pos[0]);
	SceneTransform_ApplyYaw((s16)state->yaw);
	SceneTransform_ApplyPitch((s16)state->pitch);

	transfer.x = 0;
	transfer.y = 0;
	transfer.z = state->distance;
	Iwram_TransformVector((s32 *)&transfer, (s32 *)state);
}

void ObjMotRst(s32 slot_id)
{
	s32					 z;
	s32					 zero;
	struct BtlObj		*slot;
	struct MotionObject *object;

	slot = BtlObjGet(slot_id);
	object = slot->object;
	zero = 0;
	object->acceleration = 0x20000;
	object->speed_limit = 0x80000;
	object->vertical_motion_strength = 0xab85;
	object->velocity_y = zero;
	object->vertical_motion_phase = zero;
	object->auto_face_motion = zero;
	object->snap_to_target = 1;
	ObjectDispatch_InitFromTable6Far(object);
	Object_SetMoveTargetFar(object, slot->anchor_x, 0, slot->anchor_z);
	z = slot->anchor_z;
	if (z < 0)
		z += 7;
	object->angle = ArcTan2(z >> 3, slot->anchor_x) - 0x8000;
}

void BattleMotion_SetupEscapeObject(s32 object_id)
{
	struct BtlObj		*slot;
	struct MotionObject *object;

	slot = BtlObjGet(object_id);
	object = slot->object;
	object->acceleration = 0x20000;
	object->speed_limit = 0x80000;
	object->velocity_y = 0x50000;
	object->vertical_motion_strength = 0x7851;
	object->vertical_motion_phase = 0;
	object->auto_face_motion = 0;
	ObjectDispatch_InitFromTable6Far(object);
	Object_SetMoveTargetFar(object, slot->anchor_x * 3, 0, slot->anchor_z);
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
}

void BattleMotion_InterpolatePosition(
	struct BtlObj *start_slot, struct BtlObj *end_slot, s32 progress)
{
	s32					 z_step;
	s32					 start_x;
	s32					 x;
	s32					 start_z;
	s32					 end_z;
	struct MotionObject *start;
	struct MotionObject *end;

	start = start_slot->object;
	end = end_slot->object;
	start_x = start->x;
	x = start_x + progress * (end->x - start_x) / 100;
	end_z = end->z;
	start_z = start->z;
	z_step = progress * (end_z - start_z) / 100;
	REG_BLDCNT = 0;
	start->acceleration = 0x20000;
	start->speed_limit = 0x80000;
	start->velocity_y = 0x40000;
	start->vertical_motion_strength = 0xab85;
	start->vertical_motion_phase = 0;
	start->auto_face_motion = 1;
	Object_SetMoveTargetFar(start, x, 0, start_z + z_step);
	ObjectDispatch_ApplyArgumentToChildrenFar(start, 2);
}

void BattleMotion_SetObjectPosition(struct BtlObj *slot)
{
	struct MotionObject *object = slot->object;

	object->acceleration = 0x20000;
	object->speed_limit = 0x80000;
	object->vertical_motion_strength = 0xab85;
	object->vertical_motion_phase = 0;
	object->auto_face_motion = 0;
	Object_SetMoveTargetFar(object, slot->anchor_x, 0, slot->anchor_z);
}

void BattleMotion_ResetObjectAtScaledAnchor(s32 object_id)
{
	u32					 scaled_x;
	struct BtlObj		*slot;
	struct MotionObject *object;

	slot = BtlObjGet(object_id);
	object = slot->object;
	object->acceleration = 0x10000;
	object->speed_limit = 0x40000;
	object->velocity_y = 0x30000;
	object->vertical_motion_strength = 0x9999;
	object->vertical_motion_phase = 0;
	object->auto_face_motion = 0;
	ObjectDispatch_InitFromTable6Far(object);
	scaled_x = slot->anchor_x * 3;
	Object_SetMoveTargetFar(object, (s32)(scaled_x + (scaled_x >> 0x1F)) >> 1, 0, slot->anchor_z);
}

void BattleMotion_InitializeObject(s32 id)
{
	struct BtlObj		*slot = BtlObjGet(id);
	struct MotionObject *object = slot->object;

	object->acceleration = 128 << 9;
	object->speed_limit = 128 << 11;
	object->velocity_y = 128 << 11;
	object->vertical_motion_strength = 0x9999;
	object->vertical_motion_phase = 0;
	object->auto_face_motion = 0;
	ObjectDispatch_InitFromTable6Far(object);
	Object_SetMoveTargetFar(object, Iwram_MulQ16(slot->anchor_x, 0x14ccc), 0, slot->anchor_z);
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
}

void BattleMotion_ApplyVariantMotion(s32 id, s32 variant)
{
	struct BtlObj		*slot;
	struct MotionObject *object;
	s32					 x;
	s32					 scale;

	slot = BtlObjGet(id);
	object = slot->object;
	if (UnitGetFar(id)->class_id != 0x94)
	{
		object->acceleration = BattleMotion_VariantAcceleration[variant];
		object->speed_limit = BattleMotion_VariantSpeedLimit[variant];
		if (object->y == 0 || variant > 4)
			object->velocity_y = BattleMotion_VariantVelocityY[variant];
		object->vertical_motion_strength = 0x9999;
		object->vertical_motion_phase = 0;
		object->auto_face_motion = 0;
		ObjectDispatch_InitFromTable6Far(object);
		scale = slot->anchor_x;
		x = (scale * BattleMotion_VariantDistancePercent[variant]) / 100;
		Object_SetMoveTargetFar(object, x, 0, slot->anchor_z);
	}
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
}

void BattleMotion_ApproachTarget(
	s32 actor_id, s32 target_id, s32 travel_divisor, s32 initial_velocity_y)
{
	struct BtlObj		*actor_slot = BtlObjGet(actor_id);
	struct BtlObj		*target_slot = BtlObjGet(target_id);
	struct MotionObject *object = actor_slot->object;
	struct MotionObject *target = target_slot->object;
	s32					 scale = 75;
	s32					 dx = target->x - object->x;
	s32					 start_x = object->x;
	s32					 step_x = scale * dx / 100;
	s32					 dz = target->z - object->z;
	s32					 start_z = object->z;
	s32					 step_z = scale * dz / 100;
	s32					 x = start_x + step_x;
	s32					 z = start_z + step_z;
	s32					 cell_x = step_x >> 8;
	s32					 cell_z = step_z >> 8;
	s32					 dist;

	dist = Iwram_Sqrt(cell_x * cell_x + cell_z * cell_z);
	dist = (dist << 8) / travel_divisor;
	object->acceleration = dist;
	object->speed_limit = dist;
	object->snap_to_target = 1;

	if (object->motion_flags & 4)
		object->velocity_y = initial_velocity_y;
	object->velocity_y = initial_velocity_y;
	object->vertical_motion_strength = 0xab85;
	object->auto_face_motion = 1;
	ObjectDispatch_InitFromTable6Far(object);
	Object_SetMoveTargetFar(object, x, 0, z);
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
}

void BattleMotion_ResetSlotObjectMode2(s32 id)
{
	struct MotionObject *object;

	object = BtlObjGet(id)->object;
	ObjectDispatch_InitFromTable6Far(object);
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
}

void BattleMotion_ReservedNoOp83B0()
{
}

void BattleMotion_ApplyPairMidpoint(s32 first_id, s32 second_id)
{
	struct BtlObj			 *second_slot;
	struct MotionObject		 *first;
	struct MotionObject		 *second;
	s32						  x1;
	s32						  x2;
	s32						  z1;
	s32						  z2;
	struct FixedPointPosition pos;

	first = BtlObjGet(first_id)->object;
	second_slot = BtlObjGet(second_id);
	second = second_slot->object;

	x1 = first->target_x;
	if (x1 == 0x80000000)
	{
		x1 = first->x;
	}
	z1 = first->target_z;
	if (z1 == 0x80000000)
	{
		z1 = first->z;
	}
	x2 = second->target_x;
	if (x2 == 0x80000000)
	{
		x2 = second->x;
	}
	z2 = second->target_z;
	if (z2 == 0x80000000)
	{
		z2 = second->z;
	}

	pos.x = (x1 + x2) / 2;
	pos.y = 0;
	pos.z = (z1 + z2) / 2;

	BattleMotion_ReservedNoOp83B0(&pos, 0x1000);
}

void BattleMotion_SpawnSlotEffectAndWait(s32 id)
{
	struct MotionObject	  *object;
	struct AnimationEntry *effect;
	s32					   entry;

	object = BtlObjGet(id)->object;
	if ((object != NULL) && ((0xF & object->record_storage_kind) == 1))
	{
		entry = ResourceMetadata_RegisterFar(object->records, 0x11b);
		if (entry != 0)
		{
			effect = (struct AnimationEntry *)entry;
			Animation_SetWorkEntryFar(effect, 1);
			effect->priority = 3;
		}
		WaitFrames(0xA);
	}
}

s32 BattleMotion_ProjectScaledPosition(s32 id, s32 *projected)
{
	struct MotionObject	   *object = BtlObjGet(id)->object;
	struct AnimationObject *record = GetMotionRecord(object, 0);
	s32						scaled;
	s32						factor;
	/* FAKEMATCH: needed to match: the input position BattleMotion_ProjectPosition fills, unused here */
	struct EffectPosition input;

	Camera_ApplyTransformByFlag();
	scaled = Render_ProjectPoint(&object->x, projected);
	factor = Iwram_MulQ16(scaled, record->scale);
	scaled = Iwram_MulQ16(factor, (s32)Battle_GetObjectTableValue(id) >> 16);
	projected[1] -= scaled;
	return 0;
}

s32 BattleMotion_ProjectConditionalPosition(s32 id, s32 *projected)
{
	struct MotionObject	   *object = BtlObjGet(id)->object;
	struct AnimationObject *record = GetMotionRecord(object, 0);
	s32						scaled;
	s32						factor;

	Camera_ApplyTransformByFlag();
	scaled = Render_ProjectPoint(&object->x, projected);
	factor = Iwram_MulQ16(scaled, record->scale);
	if (Summon_IsEntryFlagged(UnitGetFar(id)->class_id) != 0)
		scaled = Iwram_MulQ16(factor, 24);
	else
		scaled = Iwram_MulQ16(factor, 48);
	projected[1] -= scaled;
	return 0;
}

s32 Battle_GetObjectTableValue(s32 id)
{
	u32 value;
	u8	no;

	no = UnitGetFar(id)->class_id;
	value = (u8)Summon_GetEntryByte4(no) << 16;
	if (value == 0)
	{
		no = UnitGetFar(id)->class_id;
		if (Summon_IsEntryFlagged(no) != 0)
		{
			value = 0x180000;
		}
		else
		{
			value = 0x300000;
		}
	}
	return value;
}
