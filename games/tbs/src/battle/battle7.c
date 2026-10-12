#include "vram_tab.h"
#include "window.h"
#include "unit/battle/battle.h"
#include "unit/battle/battle2.h"
#include "unit/battle/battle5.h"
#include "unit/battle/battle6.h"
#include "unit/battle/battle7.h"
#include "unit/battle/battle8.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object2.h"
#include "unit/battle/effect/object10.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime9.h"
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

s32 BattleUnit_RollCounterEarlyEnd(s32 unit_id, s32 turns, s32 bias)
{
	struct BtlUnit *state = UnitGetFar(unit_id);

	if (turns <= 5)
	{
		s32 threshold = ((state->luck * 3 - turns * 5) + bias) * 0x28f;
		if (threshold >= (BtlRndFar() & 0xffff))
			return 1;
	}
	return 0;
}

s32 BattleUnit_TickAttackModifier(s32 unit_id)
{
	struct BtlUnit *state = UnitGetFar(unit_id);

	if (state->atk_cnt != 0)
	{
		s32 zero;
		state->atk_cnt--;
		zero = 0;
		if (state->atk_cnt == 0)
		{
			state->atk_lv = zero;
			return 1;
		}
		if (state->atk_lv < 0)
		{
			if (BattleUnit_RollCounterEarlyEnd(unit_id, state->atk_cnt, 30) != 0)
			{
				state->atk_lv = zero;
				state->atk_cnt = zero;
				return 1;
			}
		}
	}
	return 0;
}

// 能力変化の残りターン (counter) を 1 減らす。0 になったとき、または下がった能力
// (level が負) が BattleUnit_RollCounterEarlyEnd で早く切れたときに、level を戻して 1 を返す
#define DEFINE_TICK_MODIFIER(name, counter, level)                                   \
	s32 name(s32 unit_id)                                                            \
	{                                                                                \
		struct BtlUnit *state = UnitGetFar(unit_id);                                 \
                                                                                     \
		if (state->counter != 0)                                                     \
		{                                                                            \
			state->counter--;                                                        \
			if (state->counter == 0)                                                 \
			{                                                                        \
				state->level = 0;                                                    \
				return 1;                                                            \
			}                                                                        \
			if (state->level < 0                                                     \
				&& BattleUnit_RollCounterEarlyEnd(unit_id, state->counter, 20) != 0) \
			{                                                                        \
				state->level = 0;                                                    \
				state->counter = 0;                                                  \
				return 1;                                                            \
			}                                                                        \
		}                                                                            \
		return 0;                                                                    \
	}

DEFINE_TICK_MODIFIER(BattleUnit_TickDefenseModifier, def_cnt, def_lv)
DEFINE_TICK_MODIFIER(BattleUnit_TickResistanceModifier, res_cnt, res_lv)

// 状態異常の残りターン (counter) を 1 減らす。0 になったとき、または
// BattleUnit_RollCounterEarlyEnd (bias) で早く切れたときに 1 を返す
#define DEFINE_TICK_STATUS(name, counter, bias)                                     \
	s32 name(s32 unit_id)                                                           \
	{                                                                               \
		struct BtlUnit *state = UnitGetFar(unit_id);                                \
		if (state->counter != 0)                                                    \
		{                                                                           \
			state->counter--;                                                       \
			if (state->counter == 0)                                                \
				return 1;                                                           \
			if (BattleUnit_RollCounterEarlyEnd(unit_id, state->counter, bias) != 0) \
			{                                                                       \
				state->counter = 0;                                                 \
				return 1;                                                           \
			}                                                                       \
		}                                                                           \
		return 0;                                                                   \
	}

DEFINE_TICK_STATUS(BattleUnit_TickDelusion, maboroshi, 30)

s32 BattleUnit_TickConfusion(s32 unit_id)
{
	struct BtlUnit *state = UnitGetFar(unit_id);
	if ((u8)state->konran != 0)
	{
		(*(u8 *)&state->konran)--;
		if (state->konran == 0)
			return 1;
		if (BattleUnit_RollCounterEarlyEnd(unit_id, (u8)state->konran, 60) != 0)
		{
			state->konran = 0;
			return 1;
		}
	}
	return 0;
}

DEFINE_TICK_STATUS(BattleUnit_TickCharm, charm, 70)
DEFINE_TICK_STATUS(BattleUnit_TickStun, shibire, 40)
DEFINE_TICK_STATUS(BattleUnit_TickSleep, nemuri, 50)

s32 Battle_AdvanceCounterAndCheckChance(s32 id)
{
	s32				next;
	s32				expired;
	u8				remaining;
	s32				turns;
	s32				packed;
	struct BtlUnit *unit;

	unit = UnitGetFar(id);
	packed = unit->fuuji;
	turns = packed & 0xFF;
	if (turns != 0)
	{
		if (turns > 7U)
		{
			turns += 0xF8;
			unit->fuuji = turns;
			packed = turns;
		}
		if (packed & 7)
		{
			next = packed + 0xFF;
			unit->fuuji = next;
			packed = next;
		}
		expired = 1;
		remaining = packed;
		if (remaining != 0)
		{
			if (remaining <= 7U && BattleUnit_RollCounterEarlyEnd(id, unit->fuuji, 0x1E) != 0)
			{
				unit->fuuji = 0U;
				return 1;
			}
			goto block_9;
		}
		return expired;
	}
block_9:
	expired = 0;
	return expired;
}

s32 BattleUnit_TickRefrain(s32 unit_id)
{
	u8 *value = &UnitGetFar(unit_id)->refrain;
	if (*value != 0)
	{
		(*value)--;
		if (*value == 0)
		{
			return 1;
		}
	}
	return 0;
}

s32 BattleUnit_TickReflect(s32 id)
{
	u8 *value = (u8 *)&UnitGetFar(id)->reflect;
	if (*value != 0)
	{
		(*value)--;
		if (*value == 0)
		{
			return 1;
		}
	}
	return 0;
}

s32 BattleUnit_TickAgilityModifier(s32 id)
{
	struct BtlUnit *unit = UnitGetFar(id);
	u8			   *value = &unit->agi_cnt;
	if (*value != 0)
	{
		(*value)--;
		if (*value == 0)
		{
			unit->agi_lv = 0;
			return 1;
		}
	}
	return 0;
}

s32 BattlePlacement_UpdateTimedEntries(void)
{
	struct DjinnRecoveryList  *list;
	struct DjinnRecoveryEntry *timed_entry;
	struct DjinnRecoveryEntry *expired_entry;
	s32						   index;
	s32						   removed;
	s32						   initial_count;

	list = &((struct DjinnRecoveryTable *)Trade_GetOfferStateFar(0))->list;
	initial_count = list->count;
	index = 0;
	removed = 0;
	if (index < initial_count)
	{
		timed_entry = list->entries;
		do
		{
			if (timed_entry->turns > 0 && UnitGetFar(timed_entry->unit_id)->hp != 0)
			{
				timed_entry->turns--;
			}
			index++;
			timed_entry++;
		} while (index < list->count);
	}
	index = 0;
	if (index < list->count)
	{
		expired_entry = list->entries;
		do
		{
			if (expired_entry->turns == 0)
			{
				u8 id = expired_entry->unit_id;

				Djinn_ActivateFar(id, expired_entry->element, expired_entry->index);
				Trade_RemoveOfferFar(id, expired_entry->element, expired_entry->index);
				UnitCalcFar(id);
				removed = 1;
			}
			else
			{
				expired_entry++;
				index++;
			}
		} while (index < list->count);
	}
	return removed;
}

s32 BattlePlacement_UpdateTimedEntriesTwentyTimes(void)
{
	s32 turns;

	turns = 19;
	do
	{
		turns--;
		BattlePlacement_UpdateTimedEntries();
	} while (turns >= 0);
	return 0;
}

void Battle_ReservedNoOpF674(void)
{
}

struct BattleGroupOrder
{
	s32 group[2];
};

s32 Battle_ProcessRoundEnd(void)
{
	struct BtlWork			  *work;
	s32						   side;
	s32						   sides;
	s32						   count;
	s32						   i;
	struct DjinnRecoveryList  *list;
	struct DjinnRecoveryEntry *timed_entry;
	struct DjinnRecoveryEntry *expired_entry;

	work = BtlWk;
	sides = (work->two_sided != 0) + 1;
	for (side = 0; side < sides; side++)
	{
		list = &((struct DjinnRecoveryTable *)Trade_GetOfferStateFar(side))->list;
		i = 0;
		if (i < list->count)
		{
			timed_entry = list->entries;
			do
			{
				if (timed_entry->turns > 0 && BtlObjGet(timed_entry->unit_id) != 0
					&& UnitGetFar(timed_entry->unit_id)->hp != 0)
				{
					timed_entry->turns--;
				}
				i++;
				timed_entry++;
			} while (i < list->count);
		}
		i = 0;
		if (i < list->count)
		{
			expired_entry = list->entries;
			do
			{
				if (expired_entry->turns == 0)
				{
					u8	id = expired_entry->unit_id;
					s32 element;

					if (BtlObjGet(id) != 0)
					{
						BattleEventRuntime_Reset();
						BattleEventRuntime_SchedulePhase(30);
						BtlEvPush(BEV_UNIT, id);
						BtlEvPush(
							BEV_ACT, expired_entry->element * 20 + expired_entry->index + 300);
						BtlEvPush(BEV_SE, 175);
						BtlEvPush(BEV_REDRAW, 0);
						MES_PUSH(MsgDjinnSet);
						BtlEvPush(BEV_OBJ_END, id);
						AudioCommand_PlayFar(212);
						ObjectDispatch_ApplyArgumentToChildrenFar(BtlObjGet(id)->object, 3);
						ObjectDispatch_ApplyValueToChildrenFar(
							(struct DispatchObject *)BtlObjGet(id)->object, 32);
						element = expired_entry->element;
						Djinn_ActivateFar(id, element, expired_entry->index);
						Trade_RemoveOfferFar(id, expired_entry->element, expired_entry->index);
						UnitCalcFar(id);
						BattleFx_PlayUnitElementEffect(id, element, 3, 0);
						BattleEventRuntime_WaitForReady();
					}
				}
				else
				{
					expired_entry++;
					i++;
				}
			} while (i < list->count);
		}
	}
	BattlePresentation_ConfigurePaletteFade(2, BtlWk->background, 0);
	{
		s32					   *group_list;
		struct BattleGroupOrder order = BattleParty_RoundEndGroupOrder;
		u16						ids[14];
		s32						group;
		s32						j;

		if (work->two_sided != 0 && work->link_side != 0)
		{
			order.group[0] = 2;
			order.group[1] = 1;
		}
		group_list = order.group;
		for (group = 0; group <= 1; group++)
		{
			count = BattleParty_ListActorIds(group_list[group], ids);
			for (j = 0; j < count; j++)
			{
				s32				hp_amount;
				s32				pp_amount;
				s32				id = ids[j];
				struct BtlUnit *unit = UnitGetFar(id);

				if (unit->sonae != 0)
				{
					unit->sonae--;
				}
				if (unit->hp != 0)
				{
					if (unit->hp_regen != 0 && unit->hp != unit->max_hp)
					{
						hp_amount = unit->hp_regen;
						if (unit->hp + hp_amount > unit->max_hp)
						{
							hp_amount = unit->max_hp - unit->hp;
						}
						Owner_AdjustFirstValueFar(id, hp_amount);
						UiWork_PushValueSlotFar(id, 1);
						UiWork_PushValueSlotFar(hp_amount, 5);
						if (unit->hp == unit->max_hp)
						{
							UiText_ShowMessageAndWaitCoreFar((s32)MsgHpFull);
						}
						else
						{
							UiText_ShowMessageAndWaitCoreFar((s32)MsgHpRecover);
						}
						AudioCommand_PlayFar(175);
						BattlePresentation_WaitForAdvance();
					}
					if (unit->ep_regen != 0 && unit->ep != unit->max_ep)
					{
						pp_amount = unit->ep_regen;
						if (unit->ep + pp_amount > unit->max_ep)
						{
							pp_amount = unit->max_ep - unit->ep;
						}
						UnitEpAddFar(id, pp_amount);
						UiWork_PushValueSlotFar(id, 1);
						UiWork_PushValueSlotFar(pp_amount, 5);
						if (unit->ep == unit->max_ep)
						{
							UiText_ShowMessageAndWaitCoreFar((s32)MsgEpFull);
						}
						else
						{
							UiText_ShowMessageAndWaitCoreFar((s32)MsgEpRecover);
						}
						AudioCommand_PlayFar(175);
						BattlePresentation_WaitForAdvance();
					}
				}
				if (BattleUnit_TickAgilityModifier(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					UiText_ShowMessageAndWaitCoreFar((s32)MsgAgiNormal);
					BattlePresentation_WaitForAdvance();
				}
				if (BattleUnit_TickAttackModifier(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					UiText_ShowMessageAndWaitCoreFar((s32)MsgAtkNormal);
					BattlePresentation_WaitForAdvance();
				}
				if (BattleUnit_TickDefenseModifier(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					UiText_ShowMessageAndWaitCoreFar((s32)MsgDefNormal);
					BattlePresentation_WaitForAdvance();
				}
				if (BattleUnit_TickResistanceModifier(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					UiText_ShowMessageAndWaitCoreFar((s32)MsgResNormal);
					BattlePresentation_WaitForAdvance();
				}
				if (BattleUnit_TickDelusion(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					UiText_ShowMessageAndWaitCoreFar((s32)MsgCureDelusion);
					BattlePresentation_WaitForAdvance();
				}
				if (BattleUnit_TickConfusion(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					UiText_ShowMessageAndWaitCoreFar((s32)MsgConscious);
					BattlePresentation_WaitForAdvance();
				}
				if (BattleUnit_TickCharm(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					UiText_ShowMessageAndWaitCoreFar((s32)MsgConscious2);
					BattlePresentation_WaitForAdvance();
				}
				if (BattleUnit_TickStun(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					BattlePres_SetActorModeAndAction(id);
					UiText_ShowMessageAndWaitCoreFar((s32)MsgCureStun);
					BattlePresentation_WaitForAdvance();
				}
				if (BattleUnit_TickSleep(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					BattlePres_SetActorModeAndAction(id);
					UiText_ShowMessageAndWaitCoreFar((s32)MsgWakes);
					BattlePresentation_WaitForAdvance();
				}
				if (Battle_AdvanceCounterAndCheckChance(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					UiText_ShowMessageAndWaitCoreFar((s32)MsgCureSeal);
					BattlePresentation_WaitForAdvance();
				}
				if (BattleUnit_TickRefrain(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					UiText_ShowMessageAndWaitCoreFar((s32)MsgRefrainEnd);
					BattlePresentation_WaitForAdvance();
				}
				if (BattleUnit_TickReflect(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					UiText_ShowMessageAndWaitCoreFar((s32)MsgReflectEnd);
					BattlePresentation_WaitForAdvance();
				}
			}
		}
	}
}

s32 BattleUnit_ProcessTurnEnd(struct BtlPlan *plan)
{
	s32						  id;
	struct BtlUnit			 *unit;
	s32						  both_sides;
	s32						  counts[4];
	struct DjinnRecoveryList *list;
	s32						  best;
	s32						  order;

	id = plan->aid;
	both_sides = 0;
	unit = UnitGetFar(id);
	list = &((struct DjinnRecoveryTable *)Trade_GetOfferStateFar((id < 0 || id > 7)))->list;
	for (best = 0; best < list->count; best++)
	{
		if (list->entries[best].unit_id == id && list->entries[best].turns == -1)
		{
			Djinn_DeactivateFar(id, list->entries[best].element, list->entries[best].index);
		}
	}
	if (BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, 0) != 0
		&& BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, 0) != 0)
	{
		both_sides = 1;
	}
	{
		s32						  i;
		s32						  owner;
		struct DjinnRecoveryList *pending;
		pending = &((struct DjinnRecoveryTable *)Trade_GetOfferStateFar((id < 0 || id > 7)))->list;
		for (i = 0; i < 4; i++)
		{
			counts[i] = 0;
		}
		for (;;)
		{
			owner = -1;
			for (i = 0; i < pending->count; i++)
			{
				if (pending->entries[i].turns == -2)
				{
					owner = pending->entries[i].unit_id;
					break;
				}
			}
			if (owner == -1)
			{
				break;
			}
			order = -1;
			for (i = 0; i < pending->count; i++)
			{
				if (pending->entries[i].unit_id == owner && pending->entries[i].turns > order)
				{
					order = pending->entries[i].turns;
				}
			}
			order++;
			if (order <= 1)
			{
				order = 2;
			}
			for (i = 0; i < pending->count; i++)
			{
				if (pending->entries[i].unit_id == owner && pending->entries[i].turns == -2)
				{
					pending->entries[i].turns = order;
					counts[pending->entries[i].element]++;
					order++;
				}
			}
		}
	}
	if (both_sides != 0)
	{
		struct BtlUnit *before;
		s32				most;
		s32				i;
		most = 0;
		before = Runtime_BumpAllocateAlternatePool(sizeof(struct BtlUnit));
		Iwram_CopyWords(before, unit, sizeof(struct BtlUnit));
		best = -1;
		for (i = 0; i <= 3; i++)
		{
			if (counts[i] > most)
			{
				most = counts[i];
				best = i;
			}
		}
		if (best >= 0 && unit->elm_lv[best] < most)
		{
			unit->elm_lv[best] = most;
		}
		UnitCalcFar(id);
		for (i = 0; i <= 3; i++)
		{
			s32 gain = unit->elm[i].pow - before->elm[i].pow;

			if (gain > 0)
			{
				BattleEventRuntime_Reset();
				BattleEventRuntime_SchedulePhase(25);
				BtlEvPush(BEV_UNIT, id);
				BtlEvPush(BEV_NUM, gain);
				BtlEvPush(BEV_SE, 175);
				MES_PUSH(MsgEarthPowerUp + i);
				BtlEvPush(BEV_OBJ_END, id);
				AudioCommand_PlayFar(212);
				ObjectDispatch_ApplyArgumentToChildrenFar(BtlObjGet(id)->object, 3);
				ObjectDispatch_ApplyValueToChildrenFar(
					(struct DispatchObject *)BtlObjGet(id)->object, 32);
				BattleFx_PlayUnitElementEffect(id, i, 2, most - 1);
				BattleEventRuntime_WaitForReady();
			}
		}
		MemFree(before);
	}
	if (both_sides != 0)
	{
		s8 *poison;
		BattleEventRuntime_Reset();
		if (plan->akuryo_dmg != 0)
		{
			BtlEvPush(BEV_OBJ_BGN, id);
			BtlEvPush(BEV_UNIT, id);
			BtlEvPush(BEV_NUM, plan->akuryo_dmg);
			MES_PUSH(MsgCurseDamage);
			if (Owner_AdjustFirstValueFar(id, -plan->akuryo_dmg) == 0)
			{
				s32 text;
				BtlEvPush(BEV_OBJ_RES, id);
				BtlEvPush(BEV_UNIT, id);
				if (id >= 0 && id <= 7)
				{
					text = (s32)MsgGoesDown;
				}
				else
				{
					text = (s32)MsgStrengthExhausted;
				}
				MES_PUSH(text);
			}
			else
			{
				BtlEvPush(BEV_OBJ_END, id);
			}
		}
		BattleEv_DispatchQueued();
		BattleEventRuntime_Reset();
		poison = &unit->doku;
		if (*poison != 0)
		{
			s32				damage = unit->max_hp * *poison / 10;
			struct BtlWork *state = BtlWk;
			BtlEvPush(BEV_OBJ_BGN, id);
			BtlEvPush(BEV_UNIT, id);
			BtlEvPush(BEV_NUM, damage);
			MES_PUSH(MsgPoisonDamage);
			if (*poison != 0)
			{
				state->events.pending_cue = SOUND_HEAVY_IMPACT;
			}
			else
			{
				state->events.pending_cue = 133;
			}
			if (Owner_AdjustFirstValueFar(id, -damage) == 0)
			{
				s32 text;
				BtlEvPush(BEV_OBJ_RES, id);
				BtlEvPush(BEV_UNIT, id);
				if (id >= 0 && id <= 7)
				{
					text = (s32)MsgGoesDown;
				}
				else
				{
					text = (s32)MsgStrengthExhausted;
				}
				MES_PUSH(text);
			}
			else
			{
				BtlEvPush(BEV_OBJ_END, id);
			}
		}
		BattleEv_DispatchQueued();
		BattleEventRuntime_Reset();
		if (unit->shiryo != 0)
		{
			if (--unit->shiryo == 0 && Owner_AdjustFirstValueFar(id, 0xc0000000) == 0)
			{
				BtlEvPush(BEV_UNIT, id);
				MES_PUSH(MsgReaperCalls);
				BtlEvPush(BEV_OBJ_BGN, id);
				BtlEvPush(BEV_OBJ_RES, id);
				BtlEvPush(BEV_UNIT, id);
				if (id >= 0 && id <= 7)
				{
					MES_PUSH(MsgReaperCalls - 3);
				}
				else
				{
					MES_PUSH(MsgReaperCalls + 3);
				}
			}
		}
		BattleEv_DispatchQueued();
	}
	UnitCalcFar(id);
}
