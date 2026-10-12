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
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime3.h"
#include "unit/system/scheduler/runtime4.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
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

s32 Party_ComputeEligibleMemberAverage(s32 record_id)
{
	struct BattleFormationRecord *record;
	struct EnemyDefinition		 *enemy;
	u32							  i;
	s32							  id;
	s32							  sum;
	s32							  count;
	/* FAKEMATCH: needed to match: an unread seven-word level table, the native frame's 28 bytes */
	s32 levels[7];

	count = 0;
	sum = 0;
	record = &BattleFormation_Records[record_id];
	for (i = 0; i < 5; i++)
	{
		if (record->minimum_counts[i] != 0)
			break;
	}
	if (i == 5)
		return -1;
	for (i = 0; i < 5; i++)
	{
		if (record->maximum_counts[i] == 0)
			continue;
		id = record->member_ids[i];
		enemy = Owner_GetRecordFar(id + 8);
		if (enemy == 0)
			continue;
		if (enemy->level > 3 && !GameFlag_TestFar(372) && !GameFlag_TestFar(id + 1544))
			return -2;
		sum += enemy->level;
		count++;
	}
	if (count == 0)
		return -3;
	return sum / count;
}

struct FormationCandidate
{
	s16 record_id;
	s16 score;
};

struct BattleBaseStats
{
	s16						  base_hp;
	s16						  base_ep;
	s16						  hp_gauge;
	s16						  ep_gauge;
	u16						  base_attack;
	u16						  base_defense;
	u16						  base_agility;
	u8						  base_luck;
	u8						  action_count;
	u8						  hp_regen;
	u8						  ep_regen;
	u8						  unknown_12[2];
	struct BattleElementStats elm[4];
};

s32 BattleFormation_SelectLevelMatchedCandidate(s32 *out_margin)
{
	s32						   match_count = 0;
	struct FormationCandidate *pool = Runtime_BumpAllocateAlternatePool(128);
	u16						   party[8];
	s32						   level_total = 0;
	s32						   unit_count = BattleParty_PrepareActiveOwners(party);
	s32						   i;
	s32						   j;
	s32						   chance;
	s32						   result;

	if (unit_count > 0)
	{
		u16 *p = party;

		i = unit_count;
		do
		{
			u8 level = UnitGetFar(*p)->level;

			i--;
			p++;
			level_total += level;
		} while (i != 0);
	}

	chance = level_total / unit_count;
	chance += (s8)GameFlag_GetByteFar(1016);
	if (chance <= 0)
		chance = 1;
	if (chance > 99)
		chance = 99;

	for (i = 0; i <= 31; i++)
		pool[i].score = -1;

	for (i = 0; (u32)i <= 19; i++)
	{
		Owner_GetRecordFar(BattleFormation_LevelMatchedEnemies[i]);
		GameFlag_ClearBitFar(BattleFormation_LevelMatchedEnemies[i] + 1536);
	}

	for (i = 0; (u32)i <= 379; i++)
	{
		s32 score = Party_ComputeEligibleMemberAverage(i);

		if (score >= 0 && score <= chance + 3)
		{
			s32 min_index = -1;
			s32 min_value;

			for (j = 0, min_value = 999; j <= 31; j++)
			{
				if (pool[j].score < min_value)
				{
					min_value = pool[j].score;
					min_index = j;
				}
			}

			if (min_index >= 0)
			{
				pool[min_index].score = score;
				pool[min_index].record_id = i;
				match_count++;
			}
		}
	}

	if (match_count > 32)
		match_count = 32;

	if (match_count != 0)
	{
		struct FormationCandidate *chosen = &pool[(match_count * Random16()) >> 16];

		result = chosen->record_id;
		*out_margin = chance - chosen->score;
	}
	else
	{
		*out_margin = match_count;
		result = 1;
	}

	MemFree(pool);
	return result;
}

s32 Owner_ApplyLevelGains(s32 owner, s32 levels)
{
	struct BattleBaseStats *base;
	struct BtlUnit		   *state;
	s32						value;
	s32						floor;
	s32						i;

	base = Runtime_BumpAllocateAlternatePool(sizeof(struct BattleBaseStats));
	state = UnitGetFar(owner);
	Iwram_CopyWords(base, &state->base_hp, sizeof(struct BattleBaseStats));

	value = state->base_hp;
	value += levels * 97 / 10;
	floor = base->base_hp * 7 / 10;
	if (value < floor)
		value = floor;
	if (value > 9999)
		value = 9999;
	state->base_hp = value;

	value = state->base_ep;
	value += levels * 15 / 10;
	floor = base->base_ep * 7 / 10;
	if (value < floor)
		value = floor;
	if (value > 9999)
		value = 9999;
	state->base_ep = value;

	floor = levels * 123 / 10;
	value = state->base_attack;
	value += floor;
	floor = base->base_attack * 7 / 10;
	if (value < floor)
		value = floor;
	if (value > 999)
		value = 999;
	state->base_attack = value;

	floor = levels * 33 / 10;
	value = state->base_defense;
	value += floor;
	floor = base->base_defense * 7 / 10;
	if (value < floor)
		value = floor;
	if (value > 999)
		value = 999;
	state->base_defense = value;

	floor = levels * 51 / 10;
	value = state->base_agility;
	value += floor;
	floor = base->base_agility * 7 / 10;
	if (value < floor)
		value = floor;
	if (value > 999)
		value = 999;
	state->base_agility = value;

	for (i = 0; i < 4; i++)
	{
		value = state->base_elm[i].pow + levels * 15;
		floor = base->elm[i].pow * 7 / 10;
		if (value < floor)
			value = floor;
		if (value > 200)
			value = 200;
		state->base_elm[i].pow = value;
	}

	state->level += levels;
	UnitCalcFar(owner);
	MemFree(base);
}

#if EDITION_INTERNATIONAL
#define CH_CNT 9
#else
#define CH_CNT 26
#endif

s32 EnmCntDec(s32 no, s32 n)
{
	struct BtlWork *w;
	s32				num;
	s32				i;
	s32				retry;
	s32				ch;

	w = BtlWk;
	num = w->summon_count;
	for (i = 0; i < num; i++)
	{
		if (w->enemy_classes[i] == no)
			break;
	}
	if (i != num)
	{
		retry = 0;
		if (w->summon_channels[i] < 0)
		{
			w->summon_channels[i] = 1;
			w->summon_masks[i] = 3;
			return 0x8001;
		}
		for (; retry <= 31; retry++)
		{
			ch = (w->summon_channels[i] + 1) % CH_CNT;
			w->summon_channels[i] = ch;
			if ((w->summon_masks[i] & (1 << (s8)ch)) == 0)
				break;
		}
		w->summon_masks[i] |= 1 << w->summon_channels[i];
		return w->summon_channels[i];
	}
	if (num <= 4)
	{
		w->summon_channels[num] = -1;
		w->enemy_classes[num] = no;
		w->summon_masks[num] = 0;
		w->summon_count = num + 1;
		return CH_CNT;
	}
	return -1;
}

s32 EnmCntRel(s32 actor_id)
{
	struct BtlWork *state = BtlWk;
	struct BtlUnit *actor;
	s32				count = state->summon_count;
	s32				index;
	s32				name_length;
	s32				bit;
	s32				class_id;

	actor = UnitGetFar(actor_id);
	if (actor->class_index != 0)
		return;

	class_id = actor->class_id;
	for (index = 0; index < count; index++)
	{
		if (state->enemy_classes[index] == class_id)
			break;
	}
	if (index == count || state->summon_masks[index] == 0)
		return;

	for (name_length = 0; name_length <= 13; name_length++)
	{
		if (actor->name[name_length] == 0)
			break;
	}

	bit = 32;
	if (name_length > 0)
		bit = actor->name[name_length - 1] - LIST_MARKER_CHAR;
	state->summon_masks[index] &= ~(1 << bit);
}

s32 EnmCntClr(s32 class_id)
{
	struct BtlUnit *summon;
	s32				object_index;
	s32				slot;
	s32				next_slot;
	u8				occupied;
	u8				marker;

	object_index = 0;
	marker = LIST_MARKER_CHAR;
scan_objects:
	summon = UnitGetFar(object_index + 0x80);
	occupied = summon->side;
	if (occupied != 1)
		goto next_object;
	if (summon->class_id != class_id)
		goto next_object;
	slot = 0;
	if (summon->name[0] != 0)
		goto scan_slots;
	summon->name[0] = marker;
	summon->name[occupied] = slot;
	return;
scan_slots:
	slot++;
	if (slot > 13)
		return;
	occupied = summon->name[slot];
	if (occupied != 0)
		goto scan_slots;
	next_slot = slot + 1;
	summon->name[slot] = marker;
	summon->name[next_slot] = occupied;
	return;
next_object:
	object_index++;
	if (object_index <= 5)
		goto scan_objects;
}

s32 BtlGrpRnd(s32 record_id)
{
	s32							  choices[5];
	struct BattleFormationRecord *record;
	s32							 *choices_base;
	s32							 *choice_cursor;
	u8							 *maximum_count;
	u8							 *member_id;
	u32							  count;
	s32							  remaining;

	count = 0;
	if (record_id < 0 || record_id >= 380)
		record_id = 1;

	choices_base = choices;
	record = &BattleFormation_Records[record_id];
	member_id = record->member_ids;
	maximum_count = record->maximum_counts;
	choice_cursor = choices_base;
	remaining = 4;
	do
	{
		if (*maximum_count++ != 0)
		{
			*choice_cursor++ = *member_id + 8;
			count++;
		}
		member_id++;
		remaining--;
	} while (remaining >= 0);

	return choices[(Random16() * count) >> 16];
}

s32 BattleFormation_BuildEnemyList(s32 record_id)
{
	u16							 *list;
	struct BtlWork				 *work;
	struct BattleFormationRecord *record;
	s32							  count;
	s32							  margin;
	u16							  list_buffer[14];
	s32							  extra[5];
	s32							  counts[5];
	s32							  order[5];
	s32							  budget;
	s32							  changed;
	s32							  room;
	s32							  size;
	s32							  fit;
	s32							  id;
	s32							  k;
	s32							  n;
	s32							  total;
	u32							  i;
	s32							  j;

	work = BtlWk;
	margin = 0;
	work->summon_count = 0;
	list = list_buffer;
	if (GameFlag_TestFar(0x173))
		record_id = BattleFormation_SelectLevelMatchedCandidate(&margin);
	if (record_id < 0 || record_id >= 380)
		record_id = 1;
	record = &BattleFormation_Records[record_id];
	for (j = 0; j <= 4 && record->minimum_counts[j] == 0; j++)
		;
	if (j == 5)
		record = &BattleFormation_Records[1];

	budget = 6;
	count = 0;
	for (i = 0; i <= 4; i++)
	{
		if (record->minimum_counts[i] != 0)
		{
			s32 size1 = 2 - (Summon_IsEntryFlagged(record->member_ids[i] + 8) != 0);

			budget -= size1 * record->minimum_counts[i];
		}
	}

	for (i = 0; i <= 4; i++)
	{
		s32 minimum;
		s32 maximum;

		counts[i] = minimum = record->minimum_counts[i];
		maximum = record->maximum_counts[i];
		room = maximum - minimum;
		if (room > 0)
		{
			s32 size2 = 2 - (Summon_IsEntryFlagged(record->member_ids[i] + 8) != 0);

			fit = budget / size2;
			if (fit < room)
				room = fit;
			extra[i] = ((room + 1) * Random16()) >> 16;
		}
		else
		{
			extra[i] = 0;
		}
	}

	do
	{
		changed = 0;
		for (i = 0; i <= 4; i++)
		{
			id = record->member_ids[i] + 8;
			if (extra[i] != 0)
			{
				size = 2 - (Summon_IsEntryFlagged(id) != 0);
				if (size > budget)
				{
					extra[i] = 0;
				}
				else
				{
					counts[i]++;
					extra[i]--;
					budget -= size;
					changed = 1;
				}
			}
		}
	} while (changed);

	work->battle_type = record->battle_type;
	switch (work->battle_type)
	{
	case 0:
		for (i = 0; i <= 4; i++)
			order[i] = i;
		for (i = 0; i <= 9; i++)
		{
			s32 first = (Random16() * 5) >> 16;
			s32 second = (Random16() * 5) >> 16;
			s32 swap = order[first];

			order[first] = order[second];
			order[second] = swap;
		}
		for (i = 0; i <= 4; i++)
		{
			s32 m;
			s32 slot = order[i];

			for (m = 0; m < counts[slot]; m++)
				list[count++] = record->member_ids[slot] + 8;
		}
		break;
	case 1:
		for (;;)
		{
			n = 0;
			for (i = 0; i <= 4; i++)
			{
				if (counts[i] != 0)
					order[n++] = i;
			}
			if (n == 0)
				break;
			k = order[(Random16() * n) >> 16];
			list[count++] = record->member_ids[k] + 8;
			counts[k]--;
		}
		break;
	default:
		for (i = 0; i <= 4; i++)
		{
			total = counts[i];
			for (k = 0; k < total; k++)
				list[count++] = record->member_ids[i] + 8;
		}
		break;
	}
	list[count] = 0;
	work->first_defeated = 6;
	work->defeat_state = 0;
	for (i = 128; i <= 133; i++)
		Iwram_ClearWords(UnitGetFar(i), BATTLE_UNIT_SIZE);

	i = 0;
	if (list[0] == 0)
		goto assigned;
assign_enemy:
	{
		s32 charge = EnmCntDec(list[i], 1);

		if (charge & 0x8000)
			EnmCntClr(list[i]);
		BtlUnitSetFar(i + 128, list[i], charge & 0x7fff);
		UnitGetFar(i + 128);
		if (margin != 0)
			Owner_ApplyLevelGains(i + 128, margin);
	}
	i++;
	if (i > 5)
		goto assigned;
	if (list[i] != 0)
		goto assign_enemy;
assigned:
	return i;
}
