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

void Palette_UpdatePulseBrightness(void)
{
	u16 *palette;
	s32	 i;
	s32	 mask;

	palette = (u16 *)0x050001C0;
	mask = 31;
	i = 15;
	do
	{
		u32 color;
		s32 amount;
		s32 tick;
		s32 angle;
		s32 red;
		s32 green;
		s32 blue;

		tick = gFrameCount;
		angle = (tick * 3) << 10;
		color = palette[16];
		amount = (0x10000 - Trig_Cos(angle)) / 0x2AAA;

		red = (color >> 10) & mask;
		green = (color >> 5) & mask;
		blue = color & mask;
		red += amount;
		green += amount;
		blue += amount;

		if (red < 0 || red > 31)
			red = 31;
		if (green < 0 || green > 31)
			green = 31;
		if (blue < 0 || blue > 31)
			blue = 31;

		*palette++ = (red << 10) | (green << 5) | blue;
		i--;
	} while (i >= 0);
}

void Camera_ConfigureScene(s32 pos)
{
	void							   **slots = (void **)&gCameraWork;
	struct BattleCamera					*state = slots[0];
	struct BattlePresentationTransition *secondary =
		slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_CAMERA];
	struct FixedPointPosition local;
	u32						  result;

	state->pos[1] = 160 << 11;
	state->pos[0] = 0;
	state->pos[2] = 0;
	secondary->target_yaw = 128 << 7;
	state->yaw = 128 << 7;
	state->pitch = 244 << 8;
	state->follow_pos = 0;
	state->distance = 0x02ee0000;
	state->unknown_18 = 0;

	Render_ResetTransformState();
	SceneTransform_ApplyPosition(&state->pos[0]);
	SceneTransform_ApplyYaw((s16)state->yaw);
	SceneTransform_ApplyPitch((s16)state->pitch);

	local.x = 0;
	local.y = 0;
	local.z = state->distance;
	Iwram_TransformVector((s32 *)&local, (s32 *)state);

	result = Iwram_RatioMulQ14(0x03c90000, 192 << 8);
	Camera_StoreSceneParameters(0, result, 0x07920000);

	gProjection.center_y = pos + 120;
	secondary->active = 1;
	BattleCamera_SetRange(240 << 15, (0x76 - pos) << 16, 0, 128 << 4, 128 << 10);
	secondary->flag = 1;
	secondary->active = 0;
}

void BattleUnit_ResetGuardLevels(void)
{
	u16 ids[14];
	s32 count;
	s32 index;

	count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, ids);
	for (index = 0; index < count; index++)
	{
		struct BtlUnit *actor;

		actor = UnitGetFar(ids[index]);
		actor->guard_lv = 0;
		UnitCalcFar(ids[index]);
	}
}

s32 BattleEscape_CheckSuccess(void)
{
	s32				escaped;
	u8			   *failed_attempts;
	s16				living_units[14];
	s32				living_count;
	s32				level_total;
	s32				unit_index;
	s32				chance;
	struct BtlWork *escape_state;

	escaped = 0;
	escape_state = BtlWk;
	if (escape_state->encounter_mode == 1)
	{
		escaped = 1;
	}
	else
	{
		failed_attempts = &escape_state->escape_failures;
		chance = 0x1388 + (escape_state->escape_failures * 0x7D0);
		living_count = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, living_units);
		level_total = 0;
		for (unit_index = escaped; unit_index < living_count; unit_index++)
		{
			level_total += UnitGetFar(living_units[unit_index])->level;
		}
		chance += level_total * 0x1F4 / living_count;
		living_count = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, living_units);
		level_total = 0;
		for (unit_index = 0; unit_index < living_count; unit_index++)
		{
			level_total += UnitGetFar(living_units[unit_index])->level;
		}
		chance -= level_total * 0x1F4 / living_count;
		if ((chance > 0) && (((0x2710 * Random16()) >> 0x10) < chance))
		{
			escaped = 1;
		}
		*failed_attempts += 1;
	}
	if (gGameState.battle_start == 2)
	{
		escaped = 0;
	}
	return escaped;
}

s32 BattlePres_BuildUnitEntries(struct BattleActionRecord *entries)
{
	u16 *excluded_units = Runtime_BumpAllocateAlternatePool(17);
	u16 *unit_ids = Runtime_BumpAllocateAlternatePool(9);
	s32	 unit_count = BattleParty_ListLivingUnits(1, unit_ids);
	s32	 excluded_count = 0;
	s32	 entry_count = 0;
	s32	 unit_index;

	for (unit_index = 0; unit_index < unit_count; unit_index++)
	{
		struct BtlUnit *unit = UnitGetFar(unit_ids[unit_index]);
		s32				copy_index;

		for (copy_index = 0; copy_index < unit->action_entry_count; copy_index++)
		{
			if (unit->nemuri != 0 || (*(u32 *)&unit->maboroshi & 0xffffff00))
			{
				struct BattleActionRecord *entry = &entries[entry_count];
				entry->unit_id = unit_ids[unit_index];
				entry->priority = unit->agi;
				entry->command = 8;
				entry->parameter = 0;
				entry->target = 0x180;
				entry_count++;
			}
			else
			{
				excluded_units[excluded_count++] = unit_ids[unit_index];
			}
		}
	}

	{
		s32 appended;

		entries += entry_count;
		appended = Battle_CollectPartyCommandsFar(entries, excluded_units, excluded_count);
		if (appended < 0)
		{
			unit_count = -1;
		}
		else
		{
			unit_count = entry_count + appended;
		}
		MemFree(unit_ids);
		MemFree(excluded_units);
		return unit_count;
	}
}

s32 BattlePres_BuildOpponentEntries(struct BattleActionRecord *entries)
{
	u16				unit_ids[14];
	s32				entry_count = 0;
	struct BtlWork *battle = BtlWk;
	s32				unit_count;
	s32				i;

	if (battle->encounter_mode == 1)
	{
		return 0;
	}

	unit_count = BattleParty_ListLivingUnits(2, unit_ids);
	if (unit_count == 0)
	{
		return 0;
	}

	for (i = 31; i >= 0; i--)
	{
		u32 first = (unit_count * Random16()) >> 16;
		u32 second = (unit_count * Random16()) >> 16;
		s32 swap = unit_ids[first];
		unit_ids[first] = unit_ids[second];
		unit_ids[second] = swap;
	}

	if (battle->encounter_mode == 2)
	{
		s32 limit = ((Random16() * 5) >> 16) + 1;

		if (limit <= 1)
		{
			limit = 2;
		}
		if (limit < unit_count)
		{
			unit_count = limit;
		}
	}

	for (i = 0; i < unit_count; i++)
	{
		s32				unit_id = unit_ids[i];
		struct BtlUnit *unit = UnitGetFar(unit_id);
		s32				copy_index;

		for (copy_index = 0; copy_index < unit->action_entry_count; copy_index++)
		{
			struct BattleActionRecord *entry = &entries[entry_count];
			s32						   value;

			entry->unit_id = unit_id;
			value = unit->agi;
			entry->priority = value;
			if (copy_index != 0)
			{
				entry->priority = (s16)value / 2;
			}

			if (unit->nemuri != 0 || unit->shibire != 0)
			{
				entry->command = 8;
				entry->parameter = 0;
				entry->target = 0x100;
			}
			else
			{
				BattleCommand_SelectAutomatic(entry, 0);
			}

			entry_count++;

			if (battle->encounter_mode == 2)
			{
				break;
			}
		}
	}

	return entry_count;
}

void BattleQueue_SortByPriority(struct BattleActionRecord *entries, s32 count)
{
	s32 i;
	s32 j;
	s32 swapped;

	for (i = 0; i < count; i++)
	{
		struct BattleActionRecord *entry = &entries[i];

		if (entry->command == 5)
		{
			struct BtlAct *action;

			UnitGetFar(entry->unit_id);
			action = BtlActGet(
				Djinn_GetDefinitionHeaderFar(entry->parameter >> 8 & 15, entry->parameter & 0xff));
			if (action->efx == 46 || action->efx == 47 || action->efx == 53)
			{
				entry->priority += 10000;
			}
		}
	}

	do
	{
		swapped = 0;
		for (j = count - 1; j > 0; j--)
		{
			if ((s16)entries[j].priority > (s16)entries[j - 1].priority)
			{
				struct BattleActionRecord temporary;

				Iwram_CopyWords(&temporary, &entries[j], sizeof(struct BattleActionRecord));
				Iwram_CopyWords(&entries[j], &entries[j - 1], sizeof(struct BattleActionRecord));
				Iwram_CopyWords(&entries[j - 1], &temporary, sizeof(struct BattleActionRecord));
				swapped++;
			}
		}
	} while (swapped != 0);
}

struct BattleLinkedActionState
{
	s32 count;
	s32 check;
	s32 seed;
	u8	unknown_0c[28];
};

s32 BattlePresentation_AppendLinkedActions(struct BattleActionRecord *actions, s32 count)
{
	struct BtlWork				   *battle = BtlWk;
	s32								result = 0;
	s32								allocation_size = (u32)(count * 16 + 19) / 20 * 20;
	struct BattleLinkedActionState *state =
		(struct BattleLinkedActionState *)Runtime_BumpAllocateAlternatePool(40);
	s32 index;

	s32 BattleLink_SendActions(void)
	{
		s32 timeout;
		s32 disconnected;
		s32 status = SerialRuntime_BeginTransferA(state, 20);

		disconnected = 0;
		timeout = 300;
		if (status == -1)
			return status;
		while (SerialRuntime_GetActiveTransfers())
		{
			WaitFrames(1);
			if (--timeout < 0)
				return -1;
			if ((gLinkStatus & 3) != 3)
			{
				if (++disconnected > 24)
					return -1;
			}
			else
			{
				disconnected = 0;
			}
		}
		if (allocation_size)
		{
			status = SerialRuntime_BeginTransferA(actions, allocation_size);
			if (status == -1)
				return status;
			while (SerialRuntime_GetActiveTransfers())
			{
				WaitFrames(1);
				if (--timeout < 0)
					return -1;
				if ((gLinkStatus & 3) != 3)
				{
					if (++disconnected > 24)
						return -1;
				}
				else
				{
					disconnected = 0;
				}
			}
		}
		return 0;
	}

	s32 BattleLink_ReceiveActions(void)
	{
		s32 timeout;
		s32 disconnected;
		s32 status = SerialRuntime_BeginTransferB(state);

		disconnected = 0;
		timeout = 300;
		if (status == -1)
			return status;
		while (SerialRuntime_GetActiveTransfers())
		{
			if (gSerialReceivedSize > 20)
				return -1;
			WaitFrames(1);
			if (--timeout < 0)
				return -1;
			if ((gLinkStatus & 3) != 3)
			{
				if (++disconnected > 24)
					return -1;
			}
			else
			{
				disconnected = 0;
			}
		}
		if (gSerialReceivedSize != 20)
			return -1;
		result = state->count;
		if (state->count)
		{
			status = SerialRuntime_BeginTransferB(actions + count);
			if (status == -1)
				return status;
			while (SerialRuntime_GetActiveTransfers())
			{
				if (gSerialReceivedSize > (u32)(result * 16 + 19) / 20 * 20)
					return -1;
				WaitFrames(1);
				if (--timeout < 0)
					return -1;
				if ((gLinkStatus & 3) != 3)
				{
					if (++disconnected > 24)
						return -1;
				}
				else
				{
					disconnected = 0;
				}
			}
			if (gSerialReceivedSize != (u32)(result * 16 + 19) / 20 * 20)
				return -1;
		}
		return 0;
	}

	for (index = 0; index < count; index++)
	{
		struct BattleActionRecord *action = &actions[index];

		action->slot = battle->owner_slots[action->unit_id];

		if (battle->link_side == 0)
		{
			if (action->priority & 1)
				action->priority++;
		}
		else
		{
			action->priority |= 1;
		}
	}

	if (battle->link_paused == 0)
	{
		if (battle->link_side == 0)
		{
			u32 saved;

			state->count = count;
			state->check = BtlRndFar();
			IRQ_LOCK(saved);
			gBattleRandomSeed = state->seed = gRandomSeed;
			IRQ_UNLOCK(saved);
			if (BattleLink_SendActions() < 0 || BattleLink_ReceiveActions() < 0)
				goto fail;
			result = state->count;
		}
		else
		{
			if (BattleLink_ReceiveActions() < 0)
				goto fail;
			result = state->count;
			state->count = count;
			if (BattleLink_SendActions() < 0)
				goto fail;
			if (BtlRndFar() != state->check)
				goto fail;
			gBattleRandomSeed = state->seed;
		}

		for (index = 0; index < result; index++)
		{
			struct BattleActionRecord *action = &actions[count + index];

			action->unit_id = action->slot;
			action->target ^= 0x80;
		}

		MemFree(state);
		return result;
	}

fail:
	BattleLink_ResetTransferState();
	SerialRuntime_RemoveIrqHandlers();
	MemFree(state);
	return -1;
}

//--------------------------------------------------------------------------
// actions.c
// バトル：ターンの行動の組み立てと、並びの位置の変換
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// BG パレットのバンク 15 を 14 へ、5 を 4 へ、明るさを変えて写す
// ofs : RGB 各成分に足す値（0 から 31 におさめる）
//--------------------------------------------------------------------------
void Palette_CopyBanksWithBrightnessOffset(s32 ofs)
{
	s32 n;
	s32 bank;
	s32 col;
	s32 mask;
	s32 idx;

	bank = 15;
	n = 0;
	mask = 31;
	do
	{
		for (col = 0; col <= 15; col++)
		{
			u32 color;
			s32 b;
			s32 g;
			s32 r;
			s32 val;

			idx = bank * 16 + col;
			color = ((u16 *)0x05000000)[idx];
			b = (color >> 10) & mask;
			g = (color >> 5) & mask;
			r = color & mask;
			b += ofs;
			g += ofs;
			r += ofs;
			if (b > 31)
				b = 31;
			if (g > 31)
				g = 31;
			if (r > 31)
				r = 31;
			if (b < 0)
				b = 0;
			if (g < 0)
				g = 0;
			if (r < 0)
				r = 0;
			val = b << 10;
			val |= g << 5;
			val |= r;
			((s16 *)0x04ffffe0)[idx] = val; // ひとつ前のバンクへ
		}
		n++;
		bank = 5;
	} while (n <= 1);
}

//--------------------------------------------------------------------------
// このターンの行動を組み立て、優先度の高い順にならべる
// 味方のコマンドと、通信対戦の相手または敵の行動を集める
// 返り値は行動の数。負の値なら中止（通信の失敗など）
// acts : 行動の並び（20 まで）
//--------------------------------------------------------------------------
s32 BattlePresentation_BuildActions(struct BattleActionRecord *acts)
{
	struct BtlUnit *unit;
	struct BtlWork *work;
	u8			   *mode;
	s32				cnt;
	s32				add;
	s32				i;

	work = BtlWk;
	{
		struct BattleActionRecord *rec = work->actions;
		u32						   n;

		for (n = 0; n < 20; n++)
		{
			rec[n].unit_id = BATTLE_UNIT_LIST_END;
			rec[n].priority = 0x8000; // いちばん低い優先度
		}
	}

	BattleUnit_ResetGuardLevels();
	Palette_CopyBanksWithBrightnessOffset(8);
	// コマンドを選ぶあいだ立てるフラグ
	GameFlag_SetBitFar(363);
	mode = &work->encounter_mode;
	Camera_ConfigureScene(0);
	UiWork_FinalizeSharedSlotFar();

	// 味方のコマンド（敵の先手なら選べない）
	if (*mode != 2)
	{
		cnt = BattlePres_BuildUnitEntries(acts);
		if (cnt < 0)
			goto finish;
		// にげるに失敗したら、このターンは敵の先手と同じあつかい
		if (cnt != 0 && acts[0].command == 99 && BattleEscape_CheckSuccess() == 0)
			*mode = 2;
	}
	else
	{
		cnt = 0;
	}

	// 通信対戦の相手か、敵の行動
	if (BtlWk->two_sided != 0)
	{
		add = BattlePresentation_AppendLinkedActions(acts, cnt);
		if (BattlePres_WaitSync() < 0)
		{
			cnt = -1;
			goto finish;
		}
		cnt += add;
		if (add < 0)
		{
			cnt = -1;
			goto finish;
		}
	}
	else
	{
		cnt += BattlePres_BuildOpponentEntries(acts + cnt);
	}

	// 優先度の順にならべ、ぼうぎょ（3 と 7）のガードを先に立てる
	BattleQueue_SortByPriority(acts, cnt);
	if (cnt > 0)
	{
		struct BattleActionRecord *act = acts;

		i = cnt;
		do
		{
			if (act->command == 3 || act->command == 7)
			{
				unit = UnitGetFar(act->unit_id);
				unit->guard_lv = act->command == 3 ? 1 : 2;
			}
			act++;
			i--;
		} while (i != 0);
	}

finish:
	GameFlag_ClearBitFar(363);
	Camera_InitDefaultTransform();
	gTransitionWork->target_yaw = 0x2000;
	return cnt;
}

//--------------------------------------------------------------------------
// 並びの位置からユニット番号を返す
// slot : 下位 4 ビットが位置、0x80 が立っていれば敵の並び
//--------------------------------------------------------------------------
s32 Battle_GetTaggedSlotValue(s32 slot)
{
	struct BtlWork *work = BtlWk;

	if ((slot & 0x80) != 0)
		return work->enm_no[slot & 0xf];
	return work->party_units[slot & 0xf];
}

//--------------------------------------------------------------------------
// ユニット番号から並びの位置を返す（いなければ -1）
// 味方は 0x100 | 位置、敵は 0x180 | 位置
// id : ユニット番号（0-7 味方、それ以外は敵）
//--------------------------------------------------------------------------
s32 Battle_FindTaggedSlotByValue(u32 id)
{
	struct BtlWork *work = BtlWk;
	s32				i;
	s16				unit;

	if (id <= 7)
	{
		for (i = 0;; i++)
		{
			unit = work->party_units[i];
			if (unit == BATTLE_UNIT_LIST_END)
				return -1;
			if (unit != BATTLE_UNIT_REMOVED && unit == id)
				return i | 0x100;
		}
	}
	for (i = 0;; i++)
	{
		unit = work->enm_no[i];
		if (unit == BATTLE_UNIT_LIST_END)
			return -1;
		if (unit != BATTLE_UNIT_REMOVED && unit == id)
			return i | 0x180;
	}
}

DEFINE_ADJUST_CAMERA(BattlePres_AdjustCameraByShoulderKeysAlt)

void Battle_ReservedNoOp9B2C(void)
{
}
