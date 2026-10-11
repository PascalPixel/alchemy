#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "fixed_point_position.h"
#include "types.h"
#include "mem.h"
#include "scene.h"
#include "iwram_call.h"
#include "fixed_math.h"
#include "global_cells.h"
#include "btlpty.h"
#include "battle_escape.h"
#include "btlsys.h"
#include "btlwork.h"
#include "btldisp.h"
#include "game_state.h"
#include "project.h"
#include "btltype.h"
#include "battle_command.h"
#include "battle_random.h"
#include "io_reg.h"
#include "serial_runtime.h"
#include "irqlock.h"
#include "edition.h"
#include "battle_unit.h"
#include "owner_state.h"
#include "heap_state.h"
#include "btlmes.h"
#include "btlobj.h"
#include "motobj.h"
#include "animspr.h"
#include "callback_scheduler.h"
#include "transform.h"
#include "btlev.h"
#include "battle_motion.h"
#include "objdisp.h"
#include "inventory.h"
#include "field_event.h"
#include "menu_list.h"
#include "keystate.h"
#include "battle_efx.h"
#include "btlsmn.h"
#include "character.h"
#include "battle_status_icon.h"
#include "io_write_queue.h"
#include "resource.h"
#include "ui.h"
#include "item.h"
#include "btlcalc.h"
#include "dma.h"
#include "irq.h"
#include "vram_block.h"
#include "ownerval.h"
#include "sound_ids.h"
#include "ramcode.h"
#include "ram_buffer.h"
#include "map_scroll.h"
#include "resource_ids.h"
#include "battle_intro.h"
#include "iowrite.h"
#include "effect_step.h"
#include "battle_formation.h"
#include "tbs_edition.h"
#include "b5_context.h"

void BattleMotion_RunValueSequence(s32 id);
s32 BattlePresentation_WaitForAdvance(void);

#define gFrameCount (*(s32 *)&gFrameCount)

#define SceneTransform_ApplyPosition ((void (*)(void *))SceneTransform_ApplyPosition)
#define UnitCalcFar ((void (*)(u16))UnitCalcFar)

u32 Random16(void);

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
	struct BattlePresentationTransition *secondary = slots[32];
	struct FixedPointPosition			 local;
	u32									 result;

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

	count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, (u16 *)ids);
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
	u16 *excluded_units = (u16 *)Runtime_BumpAllocateAlternatePool(17);
	u16 *unit_ids = (u16 *)Runtime_BumpAllocateAlternatePool(9);
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
			action = BtlActGet(Djinn_GetDefinitionHeaderFar(
				(entry->parameter) >> 8 & 15, entry->parameter & 0xff));
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

#define gRandomSeed (*(s32 *)&gRandomSeed)

extern volatile u16 gLinkStatus;

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
			u32			  saved;

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
			action->target = action->target ^ 0x80;
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

extern struct BattlePresentationTransition *gTransitionWork;

void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBitFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);

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

#if GAME_TLA
	work = Ram_HeapSlots->battle_work;
#else
	work = BtlWk;
#endif
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
#if GAME_TLA
	GameFlag_SetBit(363);
#else
	GameFlag_SetBitFar(363);
#endif
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
#if GAME_TLA
	if (Ram_HeapSlots->battle_work->two_sided != 0)
	{
#else
	if (BtlWk->two_sided != 0)
	{
#endif
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
#if GAME_TLA
				unit = UnitGet(act->unit_id);
				unit->guard_lv = act->command == 3 ? 1 : 3;
#else
				unit = UnitGetFar(act->unit_id);
				unit->guard_lv = act->command == 3 ? 1 : 2;
#endif
			}
			act++;
			i--;
		} while (i != 0);
	}

finish:
#if GAME_TLA
	GameFlag_ClearBit(363);
#else
	GameFlag_ClearBitFar(363);
#endif
	Camera_InitDefaultTransform();
#if GAME_TLA
	Ram_HeapSlots->transition_work->target_yaw = 0x2000;
#else
	gTransitionWork->target_yaw = 0x2000;
#endif
	return cnt;
}

//--------------------------------------------------------------------------
// 並びの位置からユニット番号を返す
// slot : 下位 4 ビットが位置、0x80 が立っていれば敵の並び
//--------------------------------------------------------------------------
s16 Battle_GetTaggedSlotValue(s32 slot)
{
#if GAME_TLA
	struct BtlWork *work = Ram_HeapSlots->battle_work;
#else
	struct BtlWork *work = BtlWk;
#endif

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
#if GAME_TLA
	struct BtlWork *work = Ram_HeapSlots->battle_work;
#else
	struct BtlWork *work = BtlWk;
#endif
	s32 i;
	s16 unit;

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

void BattlePres_AdjustCameraByShoulderKeysAlt(void)
{
	struct BattleCamera					*cam = *(gWorkSlot + HEAP_SLOT_CAMERA);
	struct BattlePresentationTransition *trans = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);

	if ((gKeysHeld & KEY_L) != 0)
	{
		cam->yaw += 512;
	}
	if ((gKeysHeld & KEY_R) != 0)
	{
		cam->yaw -= 512;
	}
	if (trans->flag == 0)
	{
		BattleCamera_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
	}
}

void Battle_ReservedNoOp9B2C(void)
{
}

#define Camera_StoreSceneParameters ((void (*)(s32, u32, s32))Camera_StoreSceneParameters)
#define BattleUnit_ProcessTurnEnd ((void (*)(struct BtlPlan *))BattleUnit_ProcessTurnEnd)
#define BattleFx_PlayUnitElementEffect ((s32 (*)(s32, s32, s32, s32))BattleFx_PlayUnitElementEffect)

s32 BattlePresentation_DispatchAction(struct BattleActionRecord *request, s32 delay)
{
	struct BattlePresentationTransition *view;
	struct BtlWork						*battle;
	struct BattleCamera					*camera;
	struct BtlUnit						*unit;
	u16									 actor[2];
	u16									 ids[14];
	s32									 kept;
	s32									 result;
	s32									 count;
	s32									 i;

	kept = 0;
	if (request->unit_id == 0xff)
		return 0;
	unit = UnitGetFar(request->unit_id);
	if (unit->hp == 0)
		return -1;
	if (unit->class_index == 0)
		BattleCommand_SelectAutomatic(request, 1);
	view = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
	view->frames = 60;
	battle = *(gWorkSlot + HEAP_SLOT_BATTLE);
	view->flag = 0;
	battle->brightness = 0x10000;
	camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
	Render_ResetTransformState();
	Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
	Camera_StoreSceneParameters(0x01fe0000, Iwram_RatioMulQ14(0x01fe0000, 0xc000), 0x7fff0000);
	if (delay != 0)
	{
		view->target_yaw = 0x2000;
		WaitFrames(delay);
	}
	actor[0] = request->unit_id;
	actor[1] = 0xff;
	BattlePres_SetActorModes(actor, 1);
	result = BattleCommand_BuildPlan(request, &battle->plan);
	if (result == 0)
	{
		switch (battle->plan.outcome)
		{
		case 1:
			BattlePres_RunActorEntries(&battle->plan, 0);
			break;
		case 2:
			RunBattlePresentation(&battle->plan, 0);
			break;
		case 5:
			BattlePresentation_RunUnitTransition(&battle->plan, 1);
			break;
		case 9:
			BattlePresentation_RunUnitTransition(&battle->plan, 0);
			break;
		case 3:
			BattlePres_RunPsynergyCast(&battle->plan, 0);
			break;
		case 6:
			BattlePres_RunPsynergyCast(&battle->plan, 1);
			break;
		case 8:
			BattlePres_RunPsynergyCast(&battle->plan, 2);
			break;
		case 4:
			BattlePres_RunItem(&battle->plan, 0, request);
			break;
		case 7:
			if (BattlePresentation_RunEncounterOrUnitTrigger(&battle->plan) != 0)
				kept = 1;
			if (kept != 0)
				goto finish;
			break;
		}
	}
	else
	{
		if (result == -1)
		{
			BattlePresentation_WaitForAdvance();
			WaitFrames(3);
		}
		BattlePres_SetActorModes(0, 0);
	}
	BattleMotion_DestroyAllSlotObjects();
	BattleUnit_ProcessTurnEnd(&battle->plan);
	BtlObjPut();
	count = BattleParty_ListActorIds(3, (u16 *)ids);
	for (i = 0; i < count; i++)
		ObjMotRst(ids[i]);
	request->unit_id = 0xff;
finish:
	BattlePresentation_ConfigurePaletteFade(2, BtlWk->background, 0);
	return kept;
}

s32 BattlePres_BuildTargetList(struct BtlPlan *plan, struct BattlePresentationWork *work)
{
	s16 *dst;
	u32	 flags;
	s32	 count;
	s32	 i;
	u8	*target;

	count = 0;
	work->flags = 0;
	flags = plan->presentation_flags;
	work->kind = flags & 0xfff;
	work->variant = (flags & 0x3000) >> 12;
	work->actor = plan->aid;
	i = 0;
	if (i < plan->target_count)
	{
		target = plan->tgt_no;
		dst = work->actors;
		do
		{
			if (UnitGetFar(*target)->hp != 0 || (plan->presentation_flags & 0x10000))
			{
				count++;
				*dst++ = *target;
			}
			i++;
			target++;
		} while (i < plan->target_count);
	}
	if (count == 0)
	{
		work->actors[0] = plan->tgt_no[0];
		count = 1;
	}
	work->first_target = plan->tgt_no[0];
	work->count = count;
	work->initial_value = 1;
}

s32 BattlePresentation_RunEncounterOrUnitTrigger(struct BtlPlan *plan)
{
	struct BattlePresentationTransition *presentation = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
	struct BtlWork						*scene = *(gWorkSlot + HEAP_SLOT_BATTLE);
	/* FAKEMATCH: the halfword initialization preserves the word copy into party_mode instead of folding it into an immediate zero (one instruction differs). */
	union
	{
		s32 value;
		u16 halves[2];
	} completed;
	s32									 party_mode;

	completed.halves[0] = 0;
	completed.halves[1] = 0;
	presentation->target_yaw = 0x2000;
	presentation->active = 1;
	BattlePres_SetActorModes(0, 0);

	if (plan->aid <= 7)
	{
		party_mode = completed.value;

		if (scene->encounter_mode != 2)
		{
			party_mode = 1;
		}

		if (!party_mode)
		{
			UiText_ShowMessageAndWaitCoreFar((s32)MsgNoEscape);
			BattlePresentation_WaitForAdvance();
		}
		else
		{
			s16 unit_ids[14];
			s32 index = BattleParty_ListLivingUnits(1, (u16 *)unit_ids) - 1;

			while (index != -1)
			{
				struct BtlUnit *unit = UnitGetFar(unit_ids[index]);

				if (unit->shibire == 0 && unit->nemuri == 0)
				{
					BattleMotion_SetupEscapeObject(unit_ids[index]);
					WaitFrames(8);
				}
				index--;
			}
			WaitFrames(22);
			completed.value = 1;
		}
	}
	else if (((Random16() * 10) >> 16) <= 6)
	{
		s16 event[2];

		event[0] = plan->aid;
		event[1] = 0xff;
		BattleMotion_SetupEscapeObject(event[0]);
		WaitFrames(8);
		BattleActor_RemoveFromLists(plan->aid);
		ActivateBattleObjectSlot(plan->aid);
	}
	else
	{
		UiText_ShowMessageAndWaitCoreFar((s32)MsgNoEscape);
		BattlePresentation_WaitForAdvance();
	}

	presentation->active = 0;
	return completed.value;
}

void ObjectDispatch_ApplyArgumentToChildrenFar(void *object, s32 mode);
void AudioCommand_PlayFar(s32 cue);

s32 BattlePresentation_RunUnitTransition(struct BtlPlan *plan, s32 mode)
{
	u16							  units[14];
	struct BattlePresentationWork work;
	u32							  actor;
	u32							  target;
	u32							  count;
	s32							  i;
	s32							  n;
	u32							  kept;
	struct MotionObject			 *object;
	void						**slots = (void **)&BtlWk;

	BattlePres_BuildTargetList(plan, &work);
	actor = plan->aid;
	target = plan->tgt_no[0];
	if (plan->presentation_flags & 0x8000)
	{
		struct BattlePresentationTransition *transition;
		s32									 yaw;
		transition = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
		yaw = 0x2000;
		if (actor > 7)
			yaw = 0x5000;
		transition->target_yaw = yaw;
		transition->frames = 60;
	}
	else
	{
		struct BattlePresentationTransition *transition;
		s32									 yaw;
		transition = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
		yaw = -0x2000;
		if (actor <= 7)
			yaw = 0x2000;
		if (transition->target_yaw != yaw)
			transition->target_yaw = yaw;
	}
	BattlePres_SetActorModes(0, 0);
	PtyStDrawFar(BtlWk->pty_st & ~1);
	object = BtlObjGet(actor)->object;
	REG_BLDCNT = 0x3f40;
	count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, units);
	for (i = 0; i != count; i++)
	{
		if (units[i] == BATTLE_UNIT_REMOVED)
			continue;
		if (units[i] != actor)
		{
			if ((target <= 7) != (units[i] <= 7))
				BattlePres_SetActorRecordMode(units[i], 1);
		}
		else
		{
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 3);
		}
	}
	AudioCommand_PlayFar(0x9a);
	BattleFx_PlayUnitElementEffect(work.actor, plan->elm, 0, 0);
	if (mode & 1)
		BattlePres_SetActorRecordMode(actor, 1);
	for (i = 0; i != 16; i++)
	{
		REG_BLDALPHA = (16 - i) | 0x1000;
		WaitFrames(1);
	}
	if (plan->failure != 0)
	{
		if (plan->failure == 1)
		{
			BtlEvPush(0, plan->aid);
			BtlEvPush(4, (s32)MsgPsynergyBlocked);
		}
		else
		{
			BtlEvPush(4, (s32)MsgCastLacksPp);
		}
		BattleEv_DispatchQueued();
		BattlePres_RunWithZeroArguments();
	}
	else
	{
		kept = 0;
		for (i = 0; i < count; i++)
		{
			u32 unit = units[i];
			if (unit == actor)
			{
				if (!(mode & 1))
					units[kept++] = actor;
			}
			else if ((target <= 7) == (unit <= 7))
			{
				units[kept++] = unit;
			}
		}
		units[kept] = BATTLE_UNIT_LIST_END;
		BattleActor_SpawnObjectsForList((s16 *)units, 0);
		for (n = 0; n < plan->target_count; n++)
			units[n] = plan->tgt_no[n];
		units[n] = BATTLE_UNIT_LIST_END;
		for (i = 0; i != work.count; i++)
		{
			struct AnimationObject *animation;
			u32						child;

			animation = GetMotionRecord(BtlObjGet(work.actors[i])->object, 0);
			for (child = 0; child != animation->count - 1; child++)
				work.params[i][child] = animation->entries[child]->param;
		}
		if (plan->presentation_flags & 0x8000)
		{
			if (target > 7)
				work.side = 0;
			else
				work.side = 1;
		}
		else
		{
			if (plan->tgt_no[0] <= 7)
				work.side = 1;
			else
				work.side = 0;
		}
		if (plan->presentation_flags & 0x20000)
			work.side ^= 1;
		Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
		if (plan->presentation_flags & 0x8000)
			BattleFx_InitializeModeFar((s32 *)&work);
		else if (plan->presentation_flags & 0x4000)
			BattleFx_DispatchByIdRangeFar((s32 *)&work);
		else
			BattleFx_DispatchModeFar((s32 *)&work);
		BattleEventRuntime_WaitForReady();
	}
	BtlObjPut();
	count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, units);
	REG_BLDCNT = 0x3f40;
	for (i = 0; i != count; i++)
	{
		if (units[i] == BATTLE_UNIT_REMOVED || units[i] == actor)
			continue;
		if ((target <= 7) != (units[i] <= 7))
			BattlePres_SetActorRecordMode(units[i], 1);
	}
	for (i = 0; i != 16; i++)
	{
		REG_BLDALPHA = i | 0x1000;
		WaitFrames(1);
	}
	for (i = 0; i != count; i++)
		BattlePres_SetActorRecordMode(units[i], 0);
	BattlePres_SetupTransitionScene(0, 0, 0, 100);
	WaitFrames(1);
	return 0;
}

//--------------------------------------------------------------------------

// act_run_entries.c

// バトル表示：行動の効果を対象ごとに出す

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// ハイライトを消し、対象ごとに効果をかけてイベントを流す
// 対象がいなければ、メッセージの送りを待つだけ
// plan : 行動と対象の情報
//--------------------------------------------------------------------------
s32 BattlePres_RunActorEntries(struct BtlPlan *plan, s32 unused)
{
	s32 i;

	BattlePres_SetActorModes(0, 0);
	if (plan->target_count == 0)
	{
		BattlePresentation_WaitForAdvance();
	}
	else
	{
		for (i = 0; i < plan->target_count; i++)
		{
			BtlTgtExec(plan, i);
			BattleEv_DispatchQueued();
		}
	}
	WaitFrames(1);
}

#undef BattleFx_PlayUnitElementEffect

struct ItemDefinition *Item_GetDirectFar(s32 item);

#define ObjectDispatch_ApplyValueToChildrenFar \
	((void (*)(struct MotionObject *, s32))ObjectDispatch_ApplyValueToChildrenFar)
#define BattleFx_DispatchByIdRangeFar \
	((void (*)(struct BattlePresentationWork *))BattleFx_DispatchByIdRangeFar)
#define BattleFx_DispatchModeFar \
	((void (*)(struct BattlePresentationWork *))BattleFx_DispatchModeFar)
#define BattleActor_SpawnObjectsForList ((s32 (*)(void *, s32))BattleActor_SpawnObjectsForList)

s32 ArcTan2(s32 first, s32 second);

s32 RunBattlePresentation(struct BtlPlan *plan, s32 mode)
{
	struct BattlePresentationWork		 work;
	struct MotionObject					*object;
	struct AnimationObject				*animation;
	struct BtlUnit						*unit;
	s16									 position[3];
	s32									 scripted;
	struct BattlePresentationTransition *transition;
	s32									 angle;
	s32									 adjusted;
	s32									 facing_angle;
	s32									 x;
	s32									 z;
	s32									 divisor;
	s32									 direct;
	s32									 index;
	s32									 phase;

	transition = gTransitionWork;
	{
		struct BtlPlan *saved_plan = plan;

		/* FAKEMATCH: the temporary null assignment and restoration end the
		 * native first CSE block, so the first plan read reloads its stack
		 * carrier. Both conditional arms disappear from the generated code.
		 * Without them, 288 of 289 instructions remain and 17 rows differ. */
		if (transition)
			plan = NULL;
		if (transition)
			plan = saved_plan;
	}
	object = BtlObjGet(plan->aid)->object;
	z = object->z;
	x = object->x;
	angle = (u16)ArcTan2(x, z);
	adjusted = angle - 0x2000;
	if (plan->aid > 7)
		adjusted = angle + 0x6000;
	adjusted &= 0x7fff;
	facing_angle = (adjusted - 0x2000) / 2 + 0x2000;

	if (transition->target_yaw == facing_angle)
	{
		transition->target_yaw = facing_angle;
		WaitFrames(5);
	}
	else
	{
		transition->target_yaw = facing_angle;
		WaitFrames(10);
	}

	BattlePres_SetActorModes(0, 0);
	BattlePres_BuildTargetList(plan, &work);
	if (work.kind == 0x87)
		PtyStDrawFar(BtlWk->pty_st & ~1);

	unit = UnitGetFar(work.actor);
	UnitGetFar(work.actors[0]);
	scripted = plan->tgt_crit[0];
	direct = plan->tgt_adj[0] == 0;

	animation = GetMotionRecord(BtlObjGet(plan->aid)->object, 0);
	divisor = ResourceMetadata_SumCommandLengthsFar(animation->entries[0]->anim_id, 2, 1);
	BattleMotion_ApproachTarget(
		work.actor, work.actors[0], divisor, Battle_GetSpriteFlagHighBits(unit->class_id) << 16);
	ObjectDispatch_ApplyValueToChildrenFar(BtlObjGet(work.actor)->object, 16);
	BtlObjGet(work.actors[0]);

	if ((u16)work.actors[0] <= 7)
		work.side = 1;
	else
		work.side = 0;

	REG_WIN0H = 0x00f0;
	REG_WIN0V = 0x1088;
	REG_WIN1H = 0x00f0;
	REG_WIN1V = 0x1088;
	REG_WININ = 0x3537;
	REG_WINOUT = 0x3f21;
	REG_DISPCNT |= 0x6000;

	if (direct != 0)
	{
		WaitFrames(10);
		BattleMotion_ResetObjectAtScaledAnchor(work.actors[0]);
		WaitFrames(2);
		WaitFrames(4);
		WaitFrames(10);
		BtlEvPush(0, plan->tgt_no[0]);
		BtlEvPush(4, (s32)MsgNimblyDodgesBlow);
		BattleEv_DispatchQueued();
		ObjMotRst(work.actors[0]);
	}
	else
	{
		phase = 0;
		work.flags = 0;
		if (plan->presentation_flags != 0)
			work.flags = 1;

		if (scripted != 0)
		{
			work.kind += 200;
			phase = 1;
			transition->flag = 1;
			position[0] = work.actor;
			position[1] = work.first_target;
			position[2] = 0xff;
			BattleActor_SpawnObjectsForList(position, 0);
		}

		divisor -= 8;
		if (divisor <= 0)
			divisor = 1;
		{
			s32 first;
			s32 second;

			for (index = 0; index != divisor; index++)
			{
				if (phase != 0)
				{
					first = work.actor;
					second = work.first_target;
					BattlePres_SetupTransitionAtPairMidpoint(
						first, second, index * 30 / divisor + 100);
				}
				WaitFrames(1);
			}
		}

		Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
		if (work.kind != 0)
		{
			if (plan->presentation_flags & 0x4000)
				BattleFx_DispatchByIdRangeFar(&work);
			else
				BattleFx_DispatchModeFar(&work);
		}
		BattleEventRuntime_WaitForReady();
		if (scripted != 0)
		{
			transition->flag = 0;
			BattleParty_ListAllUnitsAndSubmit();
			BattlePres_SetupTransitionScene(0, 0, 0, 100);
		}
		ObjMotRst(work.actors[0]);
	}
	ObjMotRst(work.actor);
	return 0;
}

s32 BattlePres_RunSimple(struct BtlPlan *input, s32 flags)
{
	struct BattlePresentationWork		 work;
	struct BtlPlan						*saved_input;
	struct MotionObject					*object;
	struct AnimationObject				*animation;
	struct BattlePresentationTransition *transition;
	s32									 angle;
	s32									 adjusted;
	s32									 facing_angle;
	s32									 x;
	s32									 z;
	s32									 divisor;
	s32									 scripted;

	transition = gTransitionWork;
	saved_input = input;
	object = BtlObjGet(saved_input->aid)->object;
	z = object->z;
	x = object->x;
	angle = (u16)ArcTan2(x, z);
	adjusted = angle - 0x2000;
	if (saved_input->aid > 7)
		adjusted = angle + 0x6000;
	adjusted &= 0x7fff;
	facing_angle = (adjusted - 0x2000) / 2 + 0x2000;

	if (transition->target_yaw == facing_angle)
	{
		transition->target_yaw = facing_angle;
		WaitFrames(5);
	}
	else
	{
		transition->target_yaw = facing_angle;
		WaitFrames(20);
	}

	BattlePres_SetActorModes(0, 0);
	BattlePres_BuildTargetList(saved_input, &work);
	UnitGetFar(work.actor);
	UnitGetFar(saved_input->tgt_no[0]);

	scripted = flags & 2;
	animation = GetMotionRecord(BtlObjGet(saved_input->aid)->object, 0);
	divisor = ResourceMetadata_SumCommandLengthsFar(animation->entries[0]->anim_id, 2, 1);
	BattleMotion_ApproachTarget(work.actor, saved_input->tgt_no[0], divisor, 0);
	ObjectDispatch_ApplyValueToChildrenFar(BtlObjGet(work.actor)->object, 16);
	BtlObjGet(saved_input->tgt_no[0]);

	if (saved_input->tgt_no[0] <= 7)
		work.side = 1;
	else
		work.side = 0;
	if (scripted != 0)
	{
		WaitFrames(10);
		BattleMotion_ResetObjectAtScaledAnchor(saved_input->tgt_no[0]);
		WaitFrames(2);
		WaitFrames(4);
		WaitFrames(10);
		ObjMotRst(saved_input->tgt_no[0]);
	}
	else
	{
		BattleFx_DispatchByIdRangeFar(&work);
		BattleEv_DispatchQueued();
		ObjMotRst(saved_input->tgt_no[0]);
	}
	ObjMotRst(work.actor);
	return 0;
}

s32 BattlePres_RunItem(struct BtlPlan *plan, s32 unused, struct BattleActionRecord *selection)
{
	struct BattlePresentationWork		 work;
	struct MotionObject					*object;
	s32									 i;
	s32									 item;
	struct BtlUnit						*unit;
	struct ItemDefinition				*def;
	struct BattlePresentationTransition *transition = gTransitionWork;
	s32									 facing;

	if (plan->aid > 4)
		facing = -0x2000;
	else
		facing = 0x2000;
	if (transition->target_yaw != facing)
		transition->target_yaw = facing;
	BattlePres_BuildTargetList(plan, &work);
	BattlePres_SetActorModes(0, 0);
	object = BtlObjGet(work.actor)->object;
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 3);
	ObjectDispatch_ApplyValueToChildrenFar(object, 16);
	if (plan->tgt_no[0] <= 7)
		work.side = 1;
	else
		work.side = 0;

	for (i = 0; i != work.count; i++)
	{
		struct AnimationObject *animation = GetMotionRecord(BtlObjGet(work.actors[i])->object, 0);
		s32						j;

		for (j = 0; j != animation->count - 1; j++)
			work.params[i][j] = animation->entries[j]->param;
	}

	Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
	if (work.kind != 0)
	{
		s32 fade;

		i = 0;
		fade = 0;
		do
		{
			struct BtlWork *battle = BtlWk;

			if (i < 20)
			{
				u16 *palette = battle->palette;
				s32	 value = 0x10000 - fade;

				battle->brightness = value;
				Graphics_ScaleRgb555Clamped(palette, (u16 *)(BG_PLTT + 6 * 16), value, 128);
			}
			WaitFrames(1);
			i++;
			fade += 0x444;
		} while (i < 20);
		if (plan->presentation_flags & 0x4000)
			BattleFx_DispatchByIdRangeFar(&work);
		else
			BattleFx_DispatchModeFar(&work);
	}
	else
	{
		WaitFrames(60);
	}
	BattleEventRuntime_WaitForReady();
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
	for (i = 0; i != work.count; i++)
		ObjMotRst(work.actors[i]);

	unit = UnitGetFar(selection->unit_id);
	item = unit->inventory[selection->parameter];
	def = Item_GetDirectFar(item);
	if (def->use_type == 1)
	{
		s32 result = Inventory_RemoveFar(selection->unit_id, selection->parameter);
		s32 index = selection->parameter;
		if (result == 2)
		{
			struct BtlWork *battle = BtlWk;
			u32				row;

			for (row = 0; row < 20; row++)
			{
				if (battle->actions[row].command == 2
					&& battle->actions[row].unit_id == selection->unit_id)
				{
					if (battle->actions[row].parameter == index)
						battle->actions[row].parameter = -1;
					else if (battle->actions[row].parameter > index)
						battle->actions[row].parameter--;
				}
			}
		}
	}
	else if (def->use_type == 2)
	{
		if ((BtlRndFar() & 7) == 0)
		{
			BtlEvPush(BEV_ITEM, unit->inventory[selection->parameter]);
			BtlEvPush(BEV_MES, (s32)MsgUsedItemBroke);
			Inventory_BreakFar(selection->unit_id, selection->parameter);
			BattleEv_DispatchQueued();
		}
	}
	else if (def->use_type == 4)
	{
		if ((item & ITEM_ID_MASK) == 0xb8)
			item = 0xb9;
		unit->inventory[selection->parameter] = item;
	}
	return 0;
}

#undef BattleFx_DispatchByIdRangeFar
#undef BattleFx_DispatchModeFar
#undef ObjectDispatch_ApplyValueToChildrenFar

s32 BattleMotion_SetRecordChildValues(struct MotionObject *object, s32 value)
{
	s32						index = 0;
	s32						child_index;
	s32						count;
	struct AnimationObject *record;
	struct AnimationEntry  *child;
	struct AnimationEntry **children;
	/* FAKEMATCH: needed to match: cached compound assignment keeps 94 bytes but changes registers and store order */
	u8 mask = 0xff;
	u8 frame;

	while ((record = GetMotionRecord(object, index)) != NULL)
	{
		children = &record->entries[1];
		child = record->entries[0];
		frame = child->frame;
		child->frame = frame | mask;
		count = record->count;
		child->param = value;
		for (child_index = 1; child_index < count; child_index++)
		{
			child = *children++;
			child->param = 0;
			child->frame = 0xff;
		}
		index++;
	}
}
#define ArcTan2 ((u16 (*)(s32 x, s32 y))ArcTan2)

s32 BattlePres_RunPsynergyCast(struct BtlPlan *plan, s32 mode)
{
	struct BattlePresentationWork		 work;
	struct BattlePresentationTransition *transition = gTransitionWork;
	struct MotionObject					*object;
	s32									 scripted;
	s32									 i;

	if (plan->presentation_flags & 0x40000)
	{
		transition->target_yaw = plan->aid <= 7 ? -0x2000 : 0x5000;
		transition->frames = 60;
	}
	else
	{
		struct MotionObject *actor = BtlObjGet(plan->aid)->object;
		s32					 angle = ArcTan2(actor->x, actor->z);
		s32					 current;

		if (plan->aid <= 7)
			current = angle - 0x1800;
		else
			current = angle + 0x1800;
		current = (s16)current;
		current += ((plan->aid <= 7 ? 0x2000 : -0x2000) - current) * 3 / 4;
		if (!((plan->aid <= 7) ^ (plan->tgt_no[0] <= 7)))
			current = plan->aid <= 7 ? 0x2400 : -0x2400;
		if (transition->target_yaw != current)
			transition->target_yaw = current;
	}
	if (plan->presentation_flags & 0x80000)
	{
		transition->target_yaw = plan->aid <= 7 ? -0x2000 : 0x2000;
		transition->frames = 60;
	}

	BattlePres_BuildTargetList(plan, &work);
	scripted = mode & 1;
	if (scripted)
		work.flags = 1;
	BattlePres_SetActorModes(0, 0);
	PtyStDrawFar(BtlWk->pty_st & ~1);
	object = BtlObjGet(work.actor)->object;
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 3);
	ObjectDispatch_ApplyValueToChildrenFar((struct DispatchObject *)object, 16);
	AudioCommand_PlayFar(0x9a);
	if (mode & 2)
		BattleFx_PlayUnitElementEffect(work.actor, plan->elm, 1, 0);
	else if (!scripted)
		BattleFx_PlayUnitElementEffect(work.actor, plan->elm, 0, 0);
	if (plan->tgt_no[0] <= 7)
		work.side = 1;
	else
		work.side = 0;

	for (i = 0; i != work.count; i++)
	{
		struct AnimationObject *animation = GetMotionRecord(BtlObjGet(work.actors[i])->object, 0);
		s32						j;

		for (j = 0; j != animation->count - 1; j++)
			work.params[i][j] = animation->entries[j]->param;
	}
	if (plan->failure != 0)
	{
		if (plan->failure == 1)
		{
			BtlEvPush(0, plan->aid);
			BtlEvPush(4, (s32)MsgPsynergyBlocked);
		}
		else
		{
			BtlEvPush(4, (s32)MsgCastLacksPp);
		}
		BattleEv_DispatchQueued();
		BattlePres_RunWithZeroArguments();
	}
	else
	{
		Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
		if (work.kind)
		{
			if (plan->presentation_flags & 0x4000)
				BattleFx_DispatchByIdRangeFar((s32 *)&work);
			else
				BattleFx_DispatchModeFar((s32 *)&work);
		}
		else
		{
			BattlePres_RunWithZeroArguments();
		}
		BattleEventRuntime_WaitForReady();
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
		for (i = 0; i != work.count; i++)
			ObjMotRst(work.actors[i]);
	}
	return 0;
}

#undef ArcTan2

#define BattleMotion_SetRecordChildValues ((int (*)())BattleMotion_SetRecordChildValues)

void BattleMotion_RunValueSequence(s32 id)
{
	u16	 selection[2];
	u16 *sel;
	s32	 target;
	s32	 remaining;

	UnitGetFar(id);
	ObjectDispatch_ApplyArgumentToChildrenFar(BtlObjGet(id)->object, 5);

	sel = selection;
	remaining = 1;
	do
	{
		target = 0xff;
		sel[1] = target;
		sel[0] = id;
		BattleLayout_HighlightPartyPanelsFar(sel);
		BattleMotion_SetRecordChildValues(BtlObjGet(id)->object, 7);
		WaitFrames(2);

		sel[0] = id;
		BattleLayout_HighlightPartyPanelsFar(sel);
		BattleMotion_SetRecordChildValues(BtlObjGet(id)->object, BattleMotion_GetSlotPalette(id));
		WaitFrames(2);
		remaining--;
	} while (remaining >= 0);

	PtyStDrawFar(BtlWk->pty_st);
}

#undef BattleMotion_SetRecordChildValues

//--------------------------------------------------------------------------

// actor.c

// バトル：倒れたユニットの絵と並びの後しまつ

//--------------------------------------------------------------------------

#if !GAME_TLA
#include "battle.h"
#include "system.h"
#include "btlpty.h"
#include "btlsys.h"
#include "btltype.h"
#include "btlwork.h"
#include "btlsmn.h"
#include "motobj.h"
#include "animspr.h"

s32	 AnimationObjects_SelectAnimationFar(struct AnimationObject *, s32);

//--------------------------------------------------------------------------
// ユニットを味方か敵の並びから外し、行動の予定からも消す
// id : ユニット番号
//--------------------------------------------------------------------------
void BattleActor_RemoveFromLists(s32 id)
{
	struct BtlWork *work;
	s32				i;
	u32 j;
	s32 unit;

	// 味方の並び
	work = BtlWk;
	UnitGetFar(id)->side = BATTLE_UNIT_ABSENT;
	for (i = 0;; i++)
	{
		if (work->party_units[i] == id)
		{
			work->party_units[i] = BATTLE_UNIT_REMOVED;
			goto removed;
		}
		if (work->party_units[i] == BATTLE_UNIT_LIST_END)
			break;
	}

	// 敵の並び
	/* FAKEMATCH: needed to match: the goto loop avoids the pointer walk and saved 0xfe constant produced by a for loop */
	do
	{
		j = 0;
	again:
		unit = work->enm_no[j];
		if (unit == id)
		{
			work->enm_no[j] = BATTLE_UNIT_REMOVED;
			goto removed;
		}
		j++;
		if (unit == BATTLE_UNIT_LIST_END)
			return;
		goto again;
	} while (0);

removed:
	EnmCntRel(id);
	for (j = 0; j < 20; j++)
	{
		if (work->actions[j].unit_id == id)
			work->actions[j].unit_id = BATTLE_UNIT_LIST_END;
	}
}

//--------------------------------------------------------------------------
// 倒れたユニットの絵：味方はアニメ 4、敵はアニメ 5 にする
// 敵はさらに 4 フレーム待って並びから外し、そのあと絵を 1 ドットずつ消す
// id : ユニット番号
//--------------------------------------------------------------------------
void BattleMotion_InitializeActorRecords(s32 id)
{
	void				   *recs[4];
	struct BtlUnit		   *unit;
	struct AnimationObject *rec;
	struct AnimationEntry *ent;
	s32 n;

	unit = UnitGetFar(id);
	n = 0;
	while ((rec = GetMotionRecord(BtlObjGet(id)->object, n)) != 0)
	{
		if (unit->side != BATTLE_UNIT_ENEMY)
			AnimationObjects_SelectAnimationFar(rec, 4);
		else
			AnimationObjects_SelectAnimationFar(rec, 5);
		n++;
	}

	if (unit->side == BATTLE_UNIT_ENEMY)
	{
		n = 0;
		while ((rec = GetMotionRecord(BtlObjGet(id)->object, n)) != 0)
		{
			ent = rec->entries[0];
			recs[n] = rec;
			ent->param = 6;
			ent->frame = 0xff;
			n++;
		}
		WaitFrames(4);
		BattleActor_RemoveFromLists(id);
		Map_RenderAllAnimatedTileFramesFar(recs, n); // 1 ドットずつ消す
		ActivateBattleObjectSlot(id);
	}
}
#endif

#undef UnitCalcFar
#undef gFrameCount

struct SlotArray
{
	s16 items[64];
};

s32 BattleTarget_SelectRandomPosition(s32 require_living_unit)
{

	u16				  positions[6];
	struct SlotArray *order;
	s16				 *entry;
	u16				 *cursor;
	s32				  value;
	s32				  count;
	s32				  index;
	s32				  slot;
	s32				  tail;
	s32				  offset;

	count = 0;
	order = (struct SlotArray *)BtlWk;

	if (require_living_unit != 0)
	{
		for (;;)
		{
			index = 0;
			slot = (u32) & ((struct BtlWork *)0)->party_units / sizeof(s16);
			if (order->items[slot] != 255)
			{
				entry = order->items;
				do
				{
					value = entry[slot];
					if (value != 254)
					{
						if (UnitGetFar(value)->hp != 0)
						{
							positions[count] = index | 0x100;
							count++;
						}
					}
					slot++;
					index++;
				} while (entry[slot] != 255);
			}
			goto pick;
		}
	}
	else
	{
		index = 0;
		slot = (u32) & ((struct BtlWork *)0)->enm_no / sizeof(s16) - 1;
		tail = (u32) & ((struct BtlWork *)0)->enm_no / sizeof(s16) - 1;
		offset = tail * 2;
		entry = (s16 *)(order->items + 1);
		if (*(s16 *)((char *)entry + offset) != 255)
		{
			cursor = (u16 *)entry;
			do
			{
				if ((s16)cursor[slot] != 254)
				{
					positions[count] = index | 0x180;
					count++;
				}
				slot++;
				tail++;
				index++;
			} while (entry[tail] != 255);
		}
	}

pick:
	if (count == 0)
		return 0;
	return positions[(Random16() * count) >> 16];
}

#define COUNT_PARTIAL_CURES(unit, count) \
	{                                    \
		if ((unit)->maboroshi != 0)      \
			(count)++;                   \
		if ((u8)(unit)->konran != 0)     \
			(count)++;                   \
		if ((unit)->charm != 0)          \
			(count)++;                   \
		if ((unit)->nemuri != 0)         \
			(count)++;                   \
		if ((unit)->fuuji != 0)          \
			(count)++;                   \
		if ((unit)->shiryo != 0)         \
			(count)++;                   \
	}

#define COUNT_ALL_CURES(unit, count)          \
	{                                         \
		COUNT_PARTIAL_CURES((unit), (count)); \
		if ((unit)->akuryo != 0)              \
			(count)++;                        \
		if ((unit)->doku != 0)                \
			(count)++;                        \
	}

#define COUNT_POSITIVE_MODIFIERS(unit, count) \
	{                                         \
		if ((unit)->atk_lv > 0)               \
			(count)++;                        \
		if ((unit)->def_lv > 0)               \
			(count)++;                        \
		if ((unit)->res_lv > 0)               \
			(count)++;                        \
		if ((unit)->elm_lv[0] > 0)            \
			(count)++;                        \
		if ((unit)->elm_lv[1] > 0)            \
			(count)++;                        \
		if ((unit)->elm_lv[2] > 0)            \
			(count)++;                        \
		if ((unit)->elm_lv[3] > 0)            \
			(count)++;                        \
	}

#define CHECK_EFFECT_TARGET(unit, action, applies, damage_class)    \
	{                                                               \
		(applies) = 0;                                              \
		switch ((action)->efx)                                      \
		{                                                           \
		case EFX_ATK_UP2:                                           \
		case EFX_ATK_UP1:                                           \
			if ((unit)->atk_lv + 1 <= 4)                            \
				(applies) = 1;                                      \
			if ((unit)->atk_cnt == 1)                               \
				(applies)++;                                        \
			break;                                                  \
		case EFX_ATK_DOWN2:                                         \
		case EFX_ATK_DOWN1:                                         \
			if ((unit)->atk_lv - 1 >= -4)                           \
				(applies) = 1;                                      \
			if ((unit)->atk_cnt == 1)                               \
				(applies)++;                                        \
			break;                                                  \
		case EFX_DEF_UP2:                                           \
		case EFX_DEF_UP1:                                           \
			if ((unit)->def_lv + 1 <= 4)                            \
				(applies) = 1;                                      \
			if ((unit)->def_cnt == 1)                               \
				(applies)++;                                        \
			break;                                                  \
		case EFX_DEF_DOWN2:                                         \
		case EFX_DEF_DOWN1:                                         \
			if ((unit)->def_lv - 1 >= -4)                           \
				(applies) = 1;                                      \
			if ((unit)->def_cnt == 1)                               \
				(applies)++;                                        \
			break;                                                  \
		case EFX_RES_UP2:                                           \
		case EFX_RES_UP1:                                           \
			if ((unit)->res_lv + 1 <= 4)                            \
				(applies) = 1;                                      \
			if ((unit)->res_cnt == 1)                               \
				(applies)++;                                        \
			break;                                                  \
		case EFX_RES_DOWN2:                                         \
		case EFX_RES_DOWN1:                                         \
			if ((unit)->res_lv - 1 >= -4)                           \
				(applies) = 1;                                      \
			if ((unit)->res_cnt == 1)                               \
				(applies)++;                                        \
			break;                                                  \
		case EFX_CURE_POISON:                                       \
			if ((unit)->doku != 0)                                  \
				(applies) = 1;                                      \
			break;                                                  \
		case EFX_CURE_PART:                                         \
			COUNT_PARTIAL_CURES((unit), (applies));                 \
			break;                                                  \
		case EFX_HEAL_60:                                           \
		case EFX_HEAL_30:                                           \
			if ((unit)->hp < (unit)->max_hp)                        \
				(applies) = 1;                                      \
			break;                                                  \
		case EFX_BUFF_CLEAR:                                        \
			COUNT_POSITIVE_MODIFIERS((unit), (applies));            \
			break;                                                  \
		case EFX_CURE_ALL:                                          \
			COUNT_ALL_CURES((unit), (applies));                     \
			break;                                                  \
		case EFX_DEATH_CURSE:                                       \
			if ((unit)->shiryo == 0)                                \
				(applies) = 1;                                      \
			break;                                                  \
		case EFX_POISON:                                            \
			if ((unit)->doku == 0)                                  \
				(applies) = 1;                                      \
			break;                                                  \
		case EFX_VENOM:                                             \
			if ((unit)->doku <= 1)                                  \
				(applies) = 1;                                      \
			break;                                                  \
		case EFX_STUN:                                              \
			if ((unit)->shibire == 0)                               \
				(applies) = 1;                                      \
			break;                                                  \
		case EFX_SLEEP:                                             \
			if ((unit)->nemuri == 0)                                \
				(applies) = 1;                                      \
			break;                                                  \
		case EFX_EVIL_SPIRIT:                                       \
			if ((unit)->akuryo == 0)                                \
				(applies) = 1;                                      \
			break;                                                  \
		case EFX_REVIVE_FULL:                                       \
		case EFX_REVIVE_HALF:                                       \
		case EFX_REVIVE_80:                                         \
			if ((unit)->hp == 0)                                    \
				(applies) = 100;                                    \
			break;                                                  \
		case 0:                                                     \
		case 1:                                                     \
		case 2:                                                     \
			break;                                                  \
		default:                                                    \
			(applies) = 1;                                          \
			break;                                                  \
		}                                                           \
		if ((unit)->hp == 0 && BtlEfxRevChkFar((action)->efx) == 0) \
			(applies) = 0;                                          \
		if ((applies) == 0)                                         \
		{                                                           \
			(damage_class) = ((action)->tgt_flg & 0x0f) - 1;        \
			switch (damage_class)                                   \
			{                                                       \
			case 2:                                                 \
			case 3:                                                 \
				if ((unit)->hp != 0)                                \
					(applies)++;                                    \
				break;                                              \
			case 7:                                                 \
			case 8:                                                 \
				if ((action)->pow != 0 && (unit)->hp != 0)          \
					(applies)++;                                    \
				break;                                              \
			case 9:                                                 \
				if ((unit)->ep != 0)                                \
					(applies)++;                                    \
				break;                                              \
			case 0:                                                 \
				if ((unit)->hp != 0 && (unit)->hp < (unit)->max_hp) \
					(applies)++;                                    \
				break;                                              \
			case 1:                                                 \
			case 4:                                                 \
			case 5:                                                 \
				if ((action)->pow != 0 && (unit)->hp != 0)          \
					(applies)++;                                    \
				break;                                              \
			}                                                       \
		}                                                           \
	}

s32 BattleTarget_SelectForAction(s32 actor_id, struct BtlAct *action)
{
	u16				order_positions[6];
	s32				unit_ids[6];
	s32				target_positions[6];
	struct BtlWork *turn_order;
	struct BtlUnit *unit;
	struct BtlUnit *next_unit;
	struct BtlUnit *this_unit;
	s32				candidate_count;
	s32				target_index;
	s32				target_count;
	s32				inner_index;
	s32				selected;
	s32				unit_id;
	s32				value;
	s32				next_value;
	s32				temp;
	s32				opposing;
	s32				damage_class;
	s32				applies;
	u32				roll;
	s16			   *slot;

	turn_order = BtlWk;
	target_count = 0;
	candidate_count = 0;

	if (action->target_mode != 0)
	{
		opposing = 0;
		if (action->target_mode == 2 || action->target_mode == 4)
			opposing = 1;

		if (actor_id < 0 || actor_id > 7)
		{
			if (opposing != 0)
				goto scan_mirrored_order;
		}
		else if (opposing == 0)
		{
			goto scan_mirrored_order;
		}

		target_index = 0;
		while (turn_order->party_units[target_index] != 255)
		{
			slot = &turn_order->party_units[target_index];
			unit_id = *slot;
			if (unit_id != 254)
			{
				if (action->target_mode != 4 || unit_id == actor_id)
				{
					unit_ids[candidate_count] = unit_id;
					order_positions[candidate_count] = 0x100;
					order_positions[candidate_count] |= target_index;
					candidate_count++;
				}
			}
			target_index++;
		}
		goto scan_complete;

	scan_mirrored_order:
		target_index = 0;
		while (turn_order->enm_no[target_index] != 255)
		{
			unit_id = turn_order->enm_no[target_index];
			if (unit_id != 254)
			{
				if (action->target_mode != 4 || unit_id == actor_id)
				{
					unit_ids[candidate_count] = unit_id;
					order_positions[candidate_count] = 0x180;
					order_positions[candidate_count] |= target_index;
					candidate_count++;
				}
			}
			target_index++;
		}
	}

scan_complete:

	if (candidate_count == 0)
		return -2;

	for (target_index = 0; target_index < candidate_count; target_index++)
	{
		unit = UnitGetFar(unit_ids[target_index]);
		CHECK_EFFECT_TARGET(unit, action, applies, damage_class);
		if (applies != 0)
		{
			unit_ids[target_count] = unit_ids[target_index];
			target_positions[target_count] = order_positions[target_index];
			target_count++;
		}
	}

	if (target_count == 0)
		return -1;

	if (action->target_mode == 1 && action->rng == 1
		&& Owner_GetRecordFar(UnitGetFar(actor_id)->class_id)->target_strategy != 2
		&& (u32)((action->tgt_flg & 0x0f) - 3) <= 2)
	{
		selected = -1;
		for (target_index = 0; target_index < target_count; target_index++)
		{
			for (inner_index = target_index; inner_index < target_count - 1; inner_index++)
			{
				this_unit = UnitGetFar(unit_ids[inner_index]);
				next_unit = UnitGetFar(unit_ids[inner_index + 1]);
				if (Owner_GetRecordFar(UnitGetFar(actor_id)->class_id)->target_strategy == 0)
				{
					value = this_unit->hp;
					next_value = next_unit->hp;
				}
				else
				{
					value = this_unit->max_hp;
					next_value = next_unit->max_hp;
				}
				if (value < next_value)
				{
					temp = unit_ids[inner_index];
					unit_ids[inner_index] = unit_ids[inner_index + 1];
					unit_ids[inner_index + 1] = temp;
					temp = target_positions[inner_index];
					target_positions[inner_index] = target_positions[inner_index + 1];
					target_positions[inner_index + 1] = temp;
				}
			}
		}

		switch (target_count)
		{
		case 1:
			selected = 0;
			break;
		case 2:
			roll = (11 * Random16()) >> 16;
			selected = 0;
			if (roll > 5)
				selected = 1;
			break;
		case 3:
			selected = (15 * Random16()) >> 16;
			if (selected <= 5)
				selected = 0;
			else if (selected <= 10)
				selected = 1;
			else
				selected = 2;
			break;
		case 4:
			selected = (18 * Random16()) >> 16;
			if (selected <= 5)
				selected = 0;
			else if (selected <= 10)
				selected = 1;
			else if (selected <= 14)
				selected = 2;
			else
				selected = 3;
			break;
		}

		if (selected >= 0)
			return target_positions[selected];
	}
	return target_positions[(Random16() * target_count) >> 16];
}

//--------------------------------------------------------------------------
// actor_reset_runtime_fields.c
// バトル：状態変化の消去と、メッセージ送りの待ち
//--------------------------------------------------------------------------

#define BtlObjGet ((int (*)())BtlObjGet)

//--------------------------------------------------------------------------
// 状態異常と能力の変化をすべて消す
// 戦闘不能になったときに呼ばれる
//--------------------------------------------------------------------------
s32 BattleActor_ResetRuntimeFields(s32 id)
{
	struct BtlUnit *unit;
	s8			   *lv;
	s32				n;
	u8				zero;

	unit = UnitGetFar(id);
	n = 3;
	zero = 0;
	lv = &unit->elm_lv[3];
	do
	{
		n--;
		*lv-- = zero;
	} while (n >= 0);

	unit->doku = 0;
	unit->atk_cnt = 0;
	unit->atk_lv = 0;
	unit->def_cnt = 0;
	unit->def_lv = 0;
	unit->res_cnt = 0;
	unit->res_lv = 0;
	unit->maboroshi = 0;
	unit->konran = 0;
	unit->charm = 0;
	unit->shibire = 0;
	unit->nemuri = 0;
	unit->fuuji = 0;
	unit->refrain = 0;
	unit->reflect = 0;
	unit->akuryo = 0;
	unit->shiryo = 0;
	unit->unknown_142[0] = 0;
	unit->unknown_142[1] = 0;
	unit->sonae = 0;
	unit->miugoki = 0;
	unit->agi_cnt = 0;
	unit->agi_lv = 0;
	unit->chouhatsu = 0;

	UnitCalcFar(id);
	return BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
}

//--------------------------------------------------------------------------
// メッセージ送りの矢印
//--------------------------------------------------------------------------
#define KEY_OKURI (KEY_A | KEY_B | KEYS_SHOULDERS) // 送りのキー

// OAM の一枚
struct AdvanceSprite
{
	struct AdvanceSprite *next;
	u16					  attr0;
	u16					  attr1;
	u16					  attr2;
	u16					  attr3;
};

extern volatile u32 gFrameCount;

//--------------------------------------------------------------------------
// ウィンドウの位置に矢印を出し、送りのキーを待つ
// B を押し続けるか、送りのキーを押すと進む
// 16 フレームを過ぎれば、押し続けていても進む
//--------------------------------------------------------------------------
s32 BattlePresentation_WaitForAdvance(void)
{
	struct AdvanceSprite	   *spr;
	struct AdvanceSprite		sprite;
	s32							frame;
	s32							slot;
	s32							pat;
	struct UiWindow			   *window;
	struct BattleDisplayOffset *offset;
	s32							attr2;
	s32							attr1;
	s32							x;
	s16							tile;

	while (!UiWork_IsCompleteFar())
		WaitFrames(1);
	spr = &sprite;
	slot = Resource_LoadIntoFreeSlot(128);
	frame = 0;
loop:
	// 4 フレームごとに 8 枚のパターンを回す
	pat = ((gFrameCount >> 2) & 7) * 128 + (s32)BattlePres_AdvanceArrowTiles;
	window = gBattleDisplayWork->window;
	offset = gBattleDisplayWork->offset;
	QueueIoWriteDelay10((u32)&REG_WINOUT, 4);
	QueueIoWriteDelay6((u32)&REG_WINOUT, 16);
	/* FAKEMATCH: needed to match: the one-pass loop keeps the BLDALPHA address load after the value */
	do
	{
		u16 blend = 16;
		REG_BLDALPHA = blend;
	} while (0);
	*(u32 *)&spr->attr0 = 0xa400;
	*(u32 *)&spr->attr2 = 0;
	tile = Resource_GetBuffer(slot, pat) & 0x3ff;
	attr2 = spr->attr2;
	spr->attr2 = (attr2 & ~0x3ff) | tile;
	// ウィンドウの位置はキャラ単位、オフセットは 1/256 ドット
	x = (u16)((window->x * 8 + (offset->x >> 8) + 4) & 0x1ff);
	attr1 = spr->attr1;
	spr->attr1 = (attr1 & ~0x1ff) | x;
	// 上下に 2 ドットまでゆれる
	*(u8 *)&spr->attr0 = Trig_Sin(gFrameCount << 12) / 32768 + window->y * 8 + (offset->y >> 8) + 6;
	Runtime_PushSlotEntry(spr, 240);
	if (!(gKeysHeld & KEY_B) && !(gKeyState & KEY_OKURI)
		&& (frame <= 15 || !(gKeysHeld & KEY_OKURI)))
	{
		WaitFrames(1);
		frame++;
		goto loop;
	}
	AudioCommand_PlayFar(111);
	Resource_ResetEntry(slot);
	WaitFrames(1);
}

//--------------------------------------------------------------------------
// 指定の位置でカーソルを動かし、送りのキーを待つ
// x, y : ドット
//--------------------------------------------------------------------------
s32 BattlePresentation_WaitForPromptAt(s32 x, s32 y)
{
	struct MenuPoint	 pos;
	s32					 pat;
	struct CursorSprite	 sprite;
	struct CursorSprite *spr;
	s32					 slot;

	pat = Ui_GetTableWordZeroFar(0);
	pos.x = x;
	pos.y = y;
	while (!UiWork_IsCompleteFar())
		WaitFrames(1);
	spr = &sprite;
	slot = Resource_LoadIntoFreeSlot(128);
	while (1)
	{
		QueueIoWriteDelay10((u32)&REG_WINOUT, 4);
		QueueIoWriteDelay6((u32)&REG_WINOUT, 16);
		/* FAKEMATCH: needed to match: the one-pass loop keeps the BLDALPHA address load after the value */
		do
		{
			u16 blend = 16;
			REG_BLDALPHA = blend;
		} while (0);
		((s32 *)spr)[1] = MENU_SPRITE_SIZE_16;
		((s32 *)spr)[2] = 0;
		spr->tile = Resource_GetBuffer(slot, pat);
		// 4 フレームごとに右上へゆらす
		spr->x = pos.x + ((gFrameCount & 4) >> 1) - 4;
		spr->y = pos.y - ((gFrameCount & 4) >> 2) - 8;
		Runtime_PushSlotEntry(spr, 240);
		if (gKeyState & KEY_OKURI)
			break;
		WaitFrames(1);
	}
	Resource_ResetEntry(slot);
	WaitFrames(1);
}

#undef BtlObjGet

#define BattlePresentation_WaitForAdvance ((int (*)())BattlePresentation_WaitForAdvance)

void BattleEv_MarkDisplay(void)
{
	gBattleDisplayWork->marked = 1;
}

void		   ObjectDispatch_ReleaseFar(struct DispatchObject *object);
struct BtlObj *BtlObjGet(s32 arg0);
s32			   ActivateBattleObjectSlot(s32 arg0);

s32 BattleActor_DestroyTemporaryObject(s32 arg0)
{
	// 戻り値の型は s32 だが return 文は無い（pop {r1}; bx r1）
	struct BtlUnit *creature;
	struct BtlObj  *runtime;

	creature = UnitGetFar(arg0);
	if (creature->side == 1)
	{
		Owner_UpdateRatioPairFar(creature, 0);
		BattleActor_RemoveFromLists(arg0);
		ActivateBattleObjectSlot(arg0);
		runtime = BtlObjGet(arg0);
		ObjectDispatch_ReleaseFar((struct DispatchObject *)runtime->object);
		runtime->object = 0;
		runtime->active = 0;
	}
}

void Battle_SetRuntimeFlagBit0(struct BattleEventState *runtime, u32 operand)
{
	runtime->flags |= 1;
}

void UiWork_PushValueSlotFar(u32, u32);

u32 BattleEv_DispatchQueued(void)
{
	struct BattleEventState *runtime = &BtlWk->events;
	struct BattleEventQueue *queue = &runtime->queue;
	s32						 i;

	for (i = 0; i < queue->count; i++)
	{
		u8 opcode = queue->opcodes[i];
		if (opcode <= BEV_SCR)
			switch (opcode)
			{
			case BEV_SCR:
				Battle_SetRuntimeFlagBit0(runtime, queue->operands[i]);
				break;
			case BEV_OBJ_EFX:
				BattleActor_DestroyTemporaryObject(queue->operands[i]);
				break;
			case BEV_UNIT:
				UiWork_PushValueSlotFar(queue->operands[i], 1);
				break;
			case BEV_NUM:
				UiWork_PushValueSlotFar(queue->operands[i], 5);
				break;
			case BEV_ITEM:
				UiWork_PushValueSlotFar(queue->operands[i] & ITEM_ID_MASK, 2);
				break;
			case BEV_ACT:
				UiWork_PushValueSlotFar(queue->operands[i] & OWNER_ACTION_ID_MASK, 4);
				break;
			case BEV_MARK:
				gBattleDisplayWork->marked = 1;
				break;
			case BEV_CLR:
				UiWork_ClearValueNameTablesFar();
				break;
			case BEV_MES:
				if ((s32)queue->operands[i] >= 0)
					UiText_ShowMessageAndWaitCoreFar(queue->operands[i]);
				BattlePresentation_WaitForAdvance();
				UiWork_ClearValueNameTablesFar();
				break;
			case BEV_MES_ADD:
				if ((s32)queue->operands[i] >= 0)
					UiText_ShowMessageAndWaitCoreFar(queue->operands[i]);
				UiWork_ClearValueNameTablesFar();
				break;
			case BEV_OBJ_BGN:
				if (runtime->pending_cue > 0)
					AudioCommand_PlayFar(runtime->pending_cue);
				BattleMotion_RunValueSequence(queue->operands[i], 0, 0);
				break;
			case BEV_OBJ_RES:
				BattleEnemy_RecordDefeat(queue->operands[i], runtime->flags);
				BattleActor_ResetRuntimeFields(queue->operands[i]);
				BattleMotion_InitializeActorRecords(queue->operands[i]);
				break;
			case BEV_REDRAW:
				PtyStDrawFar(BtlWk->pty_st);
				break;
			case BEV_OBJ_END:
				BattleUnit_BuildStatusFlags(queue->operands[i], BtlObjGet(queue->operands[i]));
				BattlePres_SetActorModeAndAction(queue->operands[i]);
				break;
			}
	}
	return BattleEventRuntime_Reset();
}

u32 BtlEvPush(u32 opcode, u32 operand)
{
	struct BattleEventQueue *queue = &BtlWk->events.queue;
	u32						 index = queue->count;

	queue->opcodes[index] = opcode;
	queue->operands[index] = operand;
	queue->count = index + 1;
	return opcode;
}

#undef BattlePresentation_WaitForAdvance

//--------------------------------------------------------------------------
// tgtexec.c
// バトル：対象ごとの効果処理
//--------------------------------------------------------------------------

#undef BtlObjGet

//--------------------------------------------------------------------------
// 対象が倒れていても追加効果を出す効果なら 1 を返す
// うばう効果は、与えたダメージを行動側に戻すため
// efx : 効果番号
//--------------------------------------------------------------------------
s32 BtlEfxDeadChk(s32 efx)
{
	switch (efx)
	{
	case 31: // HPをうばう
	case 32: // EPをうばう
	case 60: // HPをうばう（半分）
	case 69: // EPをうばう（1/10）
		return 1;
	default:
		return 0;
	}
}

//--------------------------------------------------------------------------
// 対象の判定・効果計算・メッセージ登録
//--------------------------------------------------------------------------
#define MIKATA 2 // 味方

#define MIKATA_CHK(id) ((id) >= 0 && (id) < 8)

#define STEP_LIM(v)   \
	{                 \
		if ((v) < -4) \
			(v) = -4; \
		if ((v) > 4)  \
			(v) = 4;  \
	}

#define ELM_BONUS()                             \
	{                                           \
		if (elm != 4)                           \
			bonus = pow - target->elm[elm].res; \
	}

#define MES_PUSH(m) BtlEvPush(BEV_MES, (u32)(m))

#define MES_SIDE(m, e)       \
	{                        \
		if (MIKATA_CHK(tid)) \
			MES_PUSH(m);     \
		else                 \
			MES_PUSH(e);     \
	}

#define GUARD_CUT()               \
	{                             \
		guard = target->guard_lv; \
		if (guard != 0)           \
		{                         \
			if (guard == 1)       \
				dmg /= 2;         \
			else                  \
				dmg /= 10;        \
		}                         \
	}

#define RES_STEP(d, v, m)         \
	{                             \
		target->res_lv += (d);    \
		STEP_LIM(target->res_lv); \
		BtlEvPush(BEV_NUM, (v));  \
		MES_PUSH(m);              \
		target->res_cnt = 7;      \
	}

#define STAT_SET(f, m) \
	{                  \
		MES_PUSH(m);   \
		(f) = 7;       \
	}

//--------------------------------------------------------------------------
// わざの効果を対象ひとりにかける
// plan : 行動と対象の情報
// slot : 対象配列の位置
//--------------------------------------------------------------------------
s32 BtlTgtExec(struct BtlPlan *plan, s32 slot)
{
	s32				dist;
	struct BtlAct  *action;
	struct BtlUnit *actor;
	s32				aid;
	s32				act_no;
	s32				bonus;
	struct BtlWork *work;
	s32				half;
	s32				adj;
	s32				dealt;
	s32				crush;
	s32				hit;
	s32				crit;
	s32				skip;
	s32				type;
	s32				aff;
	struct BtlUnit *bak;
	s32				pow;
	s32				hp;
	s32				rate;
	s16			   *cmd;
	s32				tid;
	s32				elm;
	struct BtlUnit *target;
	u32				n;
	s32				value;
	s32				dmg;
	s32				scale;
	s32				pass;
	s32				guard;
	s16				list[8];
	s32				cnt;
	s32				tmp;
	s32				size;

	// 変化量の計算用に、対象の状態を保存する
	bonus = 0;
	work = BtlWk;
	half = 0;
	dealt = 0;
	crush = 0;
	skip = 0;
	aff = 0;
	size = sizeof(struct BtlUnit);
	bak = MemAlloc(size);

	aid = plan->aid;
	tid = plan->tgt_no[slot];
	act_no = plan->act_no;
	elm = plan->elm;
	adj = plan->tgt_adj[slot];
	crit = plan->tgt_crit[slot];

	action = BtlActGet(act_no);
	actor = UnitGetFar(aid);
	target = UnitGetFar(tid);
	Iwram_CopyWords(bak, target, size);

	// 範囲内での距離と属性の相性
	if (action->rng != 255)
	{
		dist = plan->tgt_dist[slot];
		if (dist < 0)
			dist = -dist;
	}
	else
	{
		dist = 0;
	}

	if (elm != 4)
	{
		s32 i;

		value = target->base_elm[elm].res;
		for (i = 0; i < 4 && value >= target->base_elm[i].res; i++)
			;
		if (i == 4)
			aff = -1;

		for (i = 0; i < 4 && value <= target->base_elm[i].res; i++)
			;
		if (i == 4)
			aff = 1;
	}

	if (plan->elm >= 0 && plan->elm <= 3)
	{
		cmd = &plan->cmd;
		if (*cmd != 2)
		{
			pow = actor->elm[plan->elm].pow;
			goto pow_end;
		}
	}
	else
		cmd = &plan->cmd;
	pow = 100;
pow_end:

	if (plan->cmd == 5 && (plan->elm >= 0 && plan->elm <= 3) && aff > 0)
	{
		s32 chance;

		chance = pow - target->elm[plan->elm].res;
		chance += 30;
		chance *= 65536 / 100;
		if (chance > (BtlRndFar() & 0xffff))
			BtlEvPush(BEV_SCR, 5);
	}

	// 命中判定と、効果に応じた事前処理
	type = action->tgt_flg & 15;
	{
		s32 res;

		res = plan->tgt_hit[slot];
		if (res == -1)
			hit = BtlHitChk(aid, tid, elm, action->efx, HitTbl[dist]);
		else
			hit = res;
	}
	if (action->efx == 50 || action->efx == 51)
	{ /* なかまをよぶ */
		s32 cls;
		s32 ent;

		cls = actor->class_id;
		ent = EnmSlotGet();
		if (action->efx == 51)
			cls = BtlGrpRnd(work->enm_grp);
		if (hit != 0 && EnmChk(cls) != 0 && ent >= 0)
		{
			s32 chg;

			chg = EnmCntDec(cls, 1);
			if (chg & 0x8000)
				EnmCntClr(cls);
			BtlUnitSetFar(ent, cls, chg & 0x7fff);
			{
				s32 i;

				for (i = 0; i < 6; i++)
				{
					if (work->enm_no[i] == 254)
					{
						work->enm_no[i] = ent;
						break;
					}
					if (work->enm_no[i] == 255)
					{
						work->enm_no[i] = ent;
						work->enm_no[i + 1] = 255;
						break;
					}
				}
			}
			EnmUpd();
			{
				struct BtlObj *obj;

				obj = BtlObjGet(ent);
				BtlObjMake(obj, ent, obj->anchor_x / 0x10000, obj->anchor_z / 0x10000);
			}
			BtlObjPut();
			{
				s32 num;

				num = BtlEnmList(list);
				for (cnt = 0; cnt < num; cnt++)
					ObjMotRst((u16)list[cnt]);
			}
			BtlEvPush(BEV_UNIT, ent);
			if (act_no != 503)
				MES_PUSH(MsgAppears);
			else
				MES_PUSH(MsgSplitOff);
		}
		else if (act_no == 503)
		{
			MES_PUSH(MsgSplitFail);
		}
		else
		{
			MES_PUSH(MsgNoOneCame);
		}
	}

	if (hit != 0)
	{
		if (action->efx == 53)
		{
			hit = 0;
			for (n = 0; n < 20; n++)
			{
				if (work->actions[n].unit_id == tid)
				{
					hit = 1;
					break;
				}
			}
		}
		else if (action->efx == 35)
		{
			half = 1;
		}
		else if (action->efx == 34)
		{
			crush = 1;
		}
		else if (action->efx == 27)
		{
			skip = 1;
		}
		else if (action->efx == 55)
		{
			if (actor->hp != 0)
				BtlEvPush(BEV_OBJ_EFX, aid);
		}
		else if (action->efx == 32)
		{
			if (target->ep != 0)
				type = 10;
			else
				hit = 0;
		}
	}

	//------------------------------------------------------------------
	// ダメージ・回復
	//------------------------------------------------------------------
	if (skip == 0 && (target->hp != 0 || BtlEfxRevChkFar(action->efx) != 0))
	{
		s32 ep;
		s32 cur;
		s32 ap;

		switch (type)
		{
		case 3: /* こうげき */
		case 4:
		{
			s32 def;

			hp = target->hp;
			def = target->def;
			scale = def;
			if (half != 0)
				scale = (u32)def >> 1;
			pass = 1;
			do
			{
				ELM_BONUS();
				if (pass == 0)
					bonus = 0;
				ap = action->pow;
				if (type == 4)
					dmg = BtlAtkCalc(actor->atk, scale, 0, bonus) * ap / 10;
				else
					dmg = BtlAtkCalc(actor->atk, scale, ap, bonus);
				dmg *= adj;
				if (crit != 0)
				{
					if (crit == 1)
						dmg = dmg * 5 / 4;
					else
						dmg = dmg * 3 / 2;
					dmg += (target->level / 5) + 6;
					if (pass == 0)
					{
						BtlEvPush(BEV_MARK, 0);
						{
							s32 mes;

							mes = (s32)MsgCritical;
							if (MIKATA_CHK(tid))
								mes += 1;
							BtlEvPush(BEV_MES_ADD, mes);
						}
					}
				}
				dmg += BtlRndFar() & 3;
				GUARD_CUT();
				if (dmg <= 0)
					dmg = 1;
				if (crush != 0)
				{
					if (dmg < hp - 1)
					{
						dmg = hp - 1;
						if (dmg <= 0)
							dmg = 1;
					}
				}
				if (GameFlag_TestFar(366) != 0 && *cmd == 5 && hp <= dmg)
				{
					dmg = hp - 1;
				}
				pass++;
			} while (pass <= 1);
			hp -= dmg;
			BtlEvPush(BEV_OBJ_BGN, tid);
			BtlEvPush(BEV_UNIT, tid);
			BtlEvPush(BEV_NUM, dmg);
			MES_PUSH((s32)(MIKATA_CHK(tid) ? MsgDmgEmphP : MsgDmgEmphE) + aff);

			if (hp <= 0)
			{
				BtlEvPush(BEV_OBJ_RES, tid);
				BtlEvPush(BEV_UNIT, tid);
				hp = 0;
				MES_SIDE(MsgGoesDown, MsgFelled);
			}
			else
				BtlEvPush(BEV_OBJ_END, tid);
			dealt = target->hp - hp;
			target->hp = hp;
			UnitRatioFar(tid);
			break;
		}

		case 10: /* EPダメージ */
		{
			if (action->pow == 0)
				break;
			ep = target->ep;
			ELM_BONUS();
			ap = action->pow;
			dmg = BtlPowCalc(ap, bonus, 256);
			dmg = dmg * EpLossTbl[dist] / 100;
			dmg *= adj;
			GUARD_CUT();
			if (action->efx == 32 && dmg > ep)
				dmg = ep;

			BtlEvPush(BEV_OBJ_BGN, tid);
			BtlEvPush(BEV_NUM, dmg);
			BtlEvPush(BEV_UNIT, tid);
			MES_PUSH(MIKATA_CHK(tid) ? MsgEpLossP : MsgEpLossE);
			ep -= dmg;
			if (ep <= 0)
				ep = 0;
			BtlEvPush(BEV_OBJ_END, tid);
			dealt = target->ep - ep;
			target->ep = ep;
			UnitRatioFar(tid);
			break;
		}

		case 1: /* HPかいふく */
		{
			if (action->pow == 0)
				break;
			cur = target->hp;

			ap = action->pow;
			dmg = BtlRcvCalc(ap, elm == 4 ? 100 : pow, 256);
			dmg = dmg * HpRcvTbl[dist] / 100;
			dmg *= adj;
			dmg += BtlRndFar() & 3;
			cur += dmg;
			if (cur > target->max_hp)
			{
				cur = target->max_hp;
				dmg = cur - target->hp;
			}
			BtlEvPush(BEV_UNIT, tid);
			if (cur == target->max_hp)
				MES_PUSH(MsgHpFull);
			else
			{
				BtlEvPush(BEV_NUM, dmg);
				MES_PUSH(MsgHpRecover);
			}
			dealt = target->hp - cur;
			target->hp = cur;
			UnitRatioFar(tid);
			break;
		}

		case -1:
		{
			if (action->pow == 0)
				break;
			ep = target->ep;
			ELM_BONUS();
			ap = action->pow;
			dmg = BtlPowCalc(ap, bonus, 256);
			dmg = dmg * EpDmgTbl[dist] / 100;
			dmg *= adj;
			GUARD_CUT();
			BtlEvPush(BEV_OBJ_BGN, tid);
			BtlEvPush(BEV_NUM, dmg);
			BtlEvPush(BEV_UNIT, tid);
			MES_PUSH(MIKATA_CHK(tid) ? MsgDmgP : MsgDmgE);
			ep -= dmg;
			if (ep <= 0)
				ep = 0;
			BtlEvPush(BEV_OBJ_END, tid);
			target->ep = ep;
			UnitRatioFar(tid);
			break;
		}

		case 5: /* HPダメージ */
		case 6:
		case 8:
		{
			s32 round;

			if (action->pow == 0)
				break;
			cur = target->hp;
			round = 1;
			do
			{
				ELM_BONUS();
				if (round == 0)
					bonus = 0;
				ap = action->pow;
				if (*cmd == 6)
				{
					s32 item;

					item = act_no - 380;
					if (item >= 0 && item <= 21)
					{
						switch (item)
						{
						case 0:
						case 6:
						case 12:
						case 18:
							rate = 3;
							break;
						case 1:
						case 7:
						case 13:
						case 19:
							rate = 6;
							break;
						case 2:
						case 8:
						case 14:
						case 20:
							rate = 9;
							break;
						case 3:
						case 9:
						case 15:
						case 21:
							rate = 12;
							break;
						}
					}
					ap += target->max_hp * rate / 100;
				}
				dmg = BtlPowCalc(ap, bonus, 256);
				dmg *= adj;
				switch (type & 15)
				{
				case 5:
					dmg = HpDmgTbl5[dist] * dmg / 100;
					break;
				case 8:
					dmg = dmg * HpDmgTbl8[dist] / 100;
					break;
				case 6:
					dmg = dmg * HpDmgTbl6[dist] / 100;
					break;
				}
				dmg += BtlRndFar() & 3;
				GUARD_CUT();
				if (GameFlag_TestFar(366) != 0 && *cmd == 6 && cur > dmg)
				{
					dmg = cur;
				}
				round++;
			} while (round <= 1);
			BtlEvPush(BEV_OBJ_BGN, tid);
			BtlEvPush(BEV_NUM, dmg);
			BtlEvPush(BEV_UNIT, tid);
			MES_PUSH((s32)(MIKATA_CHK(tid) ? MsgDmgEmphP : MsgDmgEmphE) + aff);
			cur -= dmg;

			if (cur <= 0)
			{
				BtlEvPush(BEV_OBJ_RES, tid);
				BtlEvPush(BEV_UNIT, tid);
				cur = 0;
				MES_PUSH(MIKATA_CHK(tid) ? MsgGoesDown : MsgFelled);
			}
			else
				BtlEvPush(BEV_OBJ_END, tid);
			dealt = target->hp - cur;
			target->hp = cur;
			UnitRatioFar(tid);
			break;
		}

		case 11: /* EPかいふく */
		{
			if (action->pow == 0)
				break;
			ep = target->ep;
			ap = action->pow;
			dmg = BtlRcvCalc(ap, elm == 4 ? 100 : pow, 256);
			dmg = dmg * EpRcvTbl[dist] / 100;
			dmg *= adj;
			ep += dmg;
			if (ep > target->max_ep)
			{
				ep = target->max_ep;
				dmg = ep - target->ep;
			}
			BtlEvPush(BEV_UNIT, tid);
			if (ep == target->max_ep)
				MES_PUSH(MsgEpFull);
			else
			{
				BtlEvPush(BEV_NUM, dmg);
				MES_PUSH(MsgEpRecover);
			}
			target->ep = ep;
			UnitRatioFar(tid);
			break;
		}

		case 0:
		case 7:
		case 9:
			break;

		case 2:
			if (hit != 0)
			{
				if (action->pow == 0)
					break;
				cur = target->hp;
				ELM_BONUS();
				ap = action->pow;
				dmg = BtlPowCalc(ap, bonus, 256);
				dmg *= adj;
				dmg = dmg * HpDmgTbl[dist] / 100;
				GUARD_CUT();
				BtlEvPush(BEV_OBJ_BGN, tid);
				BtlEvPush(BEV_NUM, dmg);
				BtlEvPush(BEV_UNIT, tid);
				MES_PUSH(MIKATA_CHK(tid) ? MsgDmgP : MsgDmgE);
				cur -= dmg;

				if (cur <= 0)
				{
					BtlEvPush(BEV_OBJ_RES, tid);
					BtlEvPush(BEV_UNIT, tid);
					cur = 0;
					MES_SIDE(MsgGoesDown, MsgFelled);
				}
				else
					BtlEvPush(BEV_OBJ_END, tid);
				dealt = target->hp - cur;
				target->hp = cur;
				UnitRatioFar(tid);
				break;
			}
			BtlEvPush(BEV_OBJ_END, tid);
			BtlEvPush(BEV_UNIT, tid);
			MES_PUSH(MsgNoEffect);
			break;
		}
	}

	//------------------------------------------------------------------
	// 状態異常・能力変化などの追加効果
	//------------------------------------------------------------------
	BtlEvPush(BEV_UNIT, tid);
	if ((BtlEfxRevChkFar(action->efx) != 0 || target->hp != 0 || BtlEfxDeadChk(action->efx) != 0)
		&& hit != 0)
	{
		switch (action->efx)
		{

		case 64: /* 状態異常をすべてなおす */
			if (target->maboroshi != 0)
			{
				target->maboroshi = 0;
				MES_PUSH(MsgCureDelusion);
				BtlEvPush(BEV_UNIT, tid);
			}
			if (target->shibire != 0)
			{
				target->shibire = 0;
				BtlEvPush(BEV_CLR, 0);
				BtlEvPush(BEV_UNIT, tid);
				MES_PUSH(MsgCureStun);
			}
			target->nemuri = 0;
			if (target->fuuji != 0)
			{
				target->fuuji = 0;
				MES_PUSH(MsgCureSeal);
				BtlEvPush(BEV_UNIT, tid);
			}
			if (target->shiryo != 0)
			{
				target->shiryo = 0;
				MES_PUSH(MsgCureCurse);
				BtlEvPush(BEV_UNIT, tid);
			}
			if (target->akuryo != 0)
			{
				target->akuryo = 0;
				MES_PUSH(MsgCureSpirit);
				BtlEvPush(BEV_UNIT, tid);
			}
			if (target->doku != 0)
			{
				MES_PUSH(MsgCurePoison);
				target->doku = 0;
			}
			BtlEvPush(BEV_CLR, 0);
			break;

		case 4: /* 状態異常をなおす */
			if (target->maboroshi != 0)
			{
				target->maboroshi = 0;
				BtlEvPush(BEV_CLR, 0);
				BtlEvPush(BEV_UNIT, tid);
				MES_PUSH(MsgCureDelusion);
			}
			if (target->shibire != 0)
			{
				target->shibire = 0;
				BtlEvPush(BEV_CLR, 0);
				BtlEvPush(BEV_UNIT, tid);
				MES_PUSH(MsgCureStun);
			}
			target->nemuri = 0;
			if (target->fuuji != 0)
			{
				target->fuuji = 0;
				BtlEvPush(BEV_CLR, 0);
				BtlEvPush(BEV_UNIT, tid);
				MES_PUSH(MsgCureSeal);
			}
			if (target->shiryo == 0)
				break;
			target->shiryo = 0;
			BtlEvPush(BEV_CLR, 0);
			BtlEvPush(BEV_UNIT, tid);
			MES_PUSH(MsgCureCurse);
			break;

		case 61: /* HPかいふく */
		case 62: /* HPかいふく */
		{
			s32	 old;
			s32	 maxu;
			s32	 maxv;
			s32	 heal;
			u16 *pt;

			/* FAKEMATCH: walking a pointer down from hp to max_hp reads each stat twice, unsigned and signed, as the ROM does; plain f reads share one load. */
			pt = (u16 *)&target->hp;
			old = *pt--;
			heal = *(s16 *)(pt + 1);
			pt--;
			maxu = *pt--;
			maxv = *(s16 *)(pt + 1);
			if (action->efx == 61)
				heal += maxv * 60 / 100;
			else
				heal += maxv * 30 / 100;
			if (heal > (s16)maxu)
				heal = (s16)maxu;
			tmp = heal - (s16)old;
			if (tmp == 0 && type != 1)
				break;
			if (heal == (s16)maxu)
				MES_PUSH(MsgHpFull);
			else
			{
				BtlEvPush(BEV_NUM, tmp);
				MES_PUSH(MsgHpRecover);
			}
			target->hp = heal;
			UnitRatioFar(tid);
			break;
		}

		case 63: /* EPかいふく */
		{
			s32 old;
			s32 maxv;
			s32 heal;

			heal = target->ep;
			maxv = target->max_ep;
			old = heal;
			heal += maxv * 7 / 100;
			if (heal > maxv)
				heal = maxv;
			tmp = heal - old;
			if (tmp == 0 && type != 11)
				break;
			if (heal == maxv)
				MES_PUSH(MsgEpFull);
			else
			{
				BtlEvPush(BEV_NUM, tmp);
				MES_PUSH(MsgEpRecover);
			}
			target->ep = heal;
			UnitRatioFar(tid);
			break;
		}

		case 59: /* すばやさアップ */
			target->agi_lv = 8;
			target->agi_cnt = 5;
			UnitCalcFar(tid);
			BtlEvPush(BEV_NUM, target->agi - bak->agi);
			MES_PUSH(MsgAgiUp);
			break;

		case 58: /* すばやさダウン */
			target->agi_lv = -4;
			target->agi_cnt = 5;
			UnitCalcFar(tid);
			BtlEvPush(BEV_NUM, bak->agi - target->agi);
			MES_PUSH(MsgAgiDown);
			break;

		case 9: /* こうげき下がる */
			target->atk_lv -= 1;
			STEP_LIM(target->atk_lv);
			UnitCalcFar(tid);
			BtlEvPush(BEV_NUM, bak->atk - target->atk);
			MES_PUSH(MsgAtkDown);
			target->atk_cnt = 7;
			break;

		case 8: /* こうげき下がる */
			target->atk_lv -= 2;
			STEP_LIM(target->atk_lv);
			UnitCalcFar(tid);
			BtlEvPush(BEV_NUM, bak->atk - target->atk);
			MES_PUSH(MsgAtkDown);
			target->atk_cnt = 7;
			break;

		case 7: /* こうげき上がる */
			target->atk_lv += 1;
			STEP_LIM(target->atk_lv);
			UnitCalcFar(tid);
			BtlEvPush(BEV_NUM, target->atk - bak->atk);
			MES_PUSH(MsgAtkUp);
			target->atk_cnt = 7;
			break;

		case 6: /* こうげき上がる */
			target->atk_lv += 2;
			STEP_LIM(target->atk_lv);
			UnitCalcFar(tid);
			BtlEvPush(BEV_NUM, target->atk - bak->atk);
			MES_PUSH(MsgAtkUp);
			target->atk_cnt = 7;
			break;

		case 13: /* ぼうぎょ下がる */
			target->def_lv -= 1;
			STEP_LIM(target->def_lv);
			UnitCalcFar(tid);
			BtlEvPush(BEV_NUM, bak->def - target->def);
			MES_PUSH(MsgDefDown);
			target->def_cnt = 7;
			break;

		case 12: /* ぼうぎょ下がる */
			target->def_lv -= 2;
			STEP_LIM(target->def_lv);
			UnitCalcFar(tid);
			BtlEvPush(BEV_NUM, bak->def - target->def);
			MES_PUSH(MsgDefDown);
			target->def_cnt = 7;
			break;

		case 11: /* ぼうぎょ上がる */
			target->def_lv += 1;
			STEP_LIM(target->def_lv);
			UnitCalcFar(tid);
			BtlEvPush(BEV_NUM, target->def - bak->def);
			MES_PUSH(MsgDefUp);
			target->def_cnt = 7;
			break;

		case 10: /* ぼうぎょ上がる */
			target->def_lv += 2;
			STEP_LIM(target->def_lv);
			UnitCalcFar(tid);
			BtlEvPush(BEV_NUM, target->def - bak->def);
			MES_PUSH(MsgDefUp);
			target->def_cnt = 7;
			break;

		case 5: /* ふっき */
			if (target->hp != 0)
				break;
			MES_PUSH(MsgRevived);
			target->hp = target->max_hp;
			UnitRatioFar(tid);
			break;

		case 56: /* ふっき */
			if (target->hp != 0)
				break;
			MES_PUSH(MsgRevived);
			target->hp = target->max_hp / 2;
			UnitRatioFar(tid);
			break;

		case 57: /* ふっき */
			if (target->hp != 0)
				break;
			MES_PUSH(MsgRevived);
			target->hp = target->max_hp * 8 / 10;
			UnitRatioFar(tid);
			break;

		case 3: /* 毒がきえる */
			if (target->doku != 0)
				MES_PUSH(MsgCurePoison);
			target->doku = 0;
			break;

		case 17: /* たいせいダウン */
			RES_STEP(-1, (bak->res_lv - target->res_lv) * 20, MsgResDown);
			break;

		case 16: /* たいせいダウン */
			RES_STEP(-2, (bak->res_lv - target->res_lv) * 20, MsgResDown);
			break;

		case 15: /* たいせいアップ */
			RES_STEP(1, (target->res_lv - bak->res_lv) * 20, MsgResUp);
			break;

		case 14: /* たいせいアップ */
			RES_STEP(2, (target->res_lv - bak->res_lv) * 20, MsgResUp);
			break;

		case 18: /* 毒 */
			if (target->doku != 0)
				break;
			MES_PUSH(MsgPoisoned);
			target->doku = 1;
			break;

		case 19: /* もう毒 */
			if (target->doku > 1)
				break;
			MES_PUSH(MsgVenom);
			target->doku = 2;
			break;

		case 20: /* まぼろし */
			STAT_SET(target->maboroshi, MsgDelusion);
			break;

		case 21: /* こんらん */
			STAT_SET(target->konran, MsgConfused);
			break;

		case 22: /* こころをうばう */
			STAT_SET(target->charm, MsgCharmed);
			break;

		case 23: /* しびれ */
			STAT_SET(target->shibire, MsgStunned);
			break;

		case 24: /* ねむり */
			STAT_SET(target->nemuri, MsgAsleep);
			break;

		case 25: /* エナジーふうじ */
			MES_SIDE(MsgPsyBlock, MsgPsySeal);
			target->fuuji |= 7;
			break;

		case 67: /* エナジーふうじ */
			MES_SIDE(MsgPsyBlock, MsgPsySeal);
			target->fuuji |= 16;
			break;

		case 27: /* せんとうふのう */
			BtlEvPush(BEV_OBJ_RES, tid);
			if (target->side == MIKATA)
				MES_PUSH(MsgKoDown);
			else if (act_no == 219)
				MES_PUSH(MsgSuffocate);
			else
				MES_PUSH(MsgSpiritDrain);
			target->hp = 0;
			UnitRatioFar(tid);
			break;

		case 29: /* リフレイン */
			STAT_SET(target->refrain, MsgRefrain);
			break;

		case 30: /* リフレクト */
			STAT_SET(target->reflect, MsgReflect);
			break;

		case 31: /* HPをうばう */
		case 60:
		{
			s32 heal;

			heal = actor->hp;
			dmg = dealt;
			if (action->efx == 60)
				dmg /= 2;
			heal += dmg;
			if (heal > actor->max_hp)
			{
				heal = actor->max_hp;
				dmg = heal - actor->hp;
			}
			BtlEvPush(BEV_CLR, 0);
			BtlEvPush(BEV_UNIT, aid);
			if (heal == actor->max_hp)
				MES_PUSH(MsgHpFull);
			else
			{
				BtlEvPush(BEV_NUM, dmg);
				MES_PUSH(MsgHpRecover);
			}
			actor->hp = heal;
			UnitRatioFar(aid);
			break;
		}

		case 32: /* EPをうばう */
		{
			s32 heal;
			s32 amt;

			heal = actor->ep;
			amt = dealt;
			heal += amt;
			if (heal > actor->max_ep)
			{
				heal = actor->max_ep;
				amt = heal - actor->ep;
			}
			BtlEvPush(BEV_CLR, 0);
			BtlEvPush(BEV_UNIT, aid);
			if (heal == actor->max_ep)
				MES_PUSH(MsgEpFull);
			else
			{
				BtlEvPush(BEV_NUM, amt);
				MES_PUSH(MsgEpRecover);
			}
			actor->ep = heal;
			UnitRatioFar(aid);
			break;
		}

		case 69: /* EPをうばう */
			dmg = dealt / 10;
			if (target->ep < dmg)
				dmg = target->ep;
			if (actor->ep + dmg > actor->max_ep)
				dmg = actor->max_ep - actor->ep;
			if (dmg == 0)
				break;
			BtlEvPush(BEV_NUM, dmg);
			MES_SIDE(MsgLeechTake, MsgLeechGain);
			UnitEpAddFar(aid, dmg);
			break;

		case 33: /* つよさがもどる */
			if (target->atk_lv > 0)
			{
				target->atk_lv = 0;
				target->atk_cnt = 0;
			}
			if (target->def_lv > 0)
			{
				target->def_lv = 0;
				target->def_cnt = 0;
			}
			if (target->res_lv > 0)
			{
				target->res_lv = 0;
				target->res_cnt = 0;
			}
			if (target->agi_lv > 0)
				target->agi_lv = 0;
			target->elm_lv[0] = 0;
			target->elm_lv[1] = 0;
			target->elm_lv[2] = 0;
			target->elm_lv[3] = 0;
			MES_PUSH(MsgBuffsReset);
			break;

		case 26: /* あくりょう */
			MES_PUSH(MsgEvilSpirit);
			target->akuryo = 1;
			break;

		case 28: /* しりょう */
			if (target->shiryo == 0)
			{
				MES_PUSH(MsgDeathCurse);
				target->shiryo = 7;
				break;
			}
			if (target->shiryo <= 1)
				break;
			target->shiryo -= 1;
			BtlEvPush(BEV_NUM, target->shiryo);
			MES_PUSH(MsgDeathCount);
			break;

		case 66: /* そなえ */
			MES_PUSH(MsgReadies);
			target->sonae = 2;
			break;

		case 54: /* ちょうはつ */
			MES_PUSH(MsgChallenge);
			target->chouhatsu = 1;
			if (MIKATA_CHK(tid))
				work->chouhatsu_flg |= 2;
			break;

		case 53: /* みうごき */
			MES_PUSH(MsgImmobile);
			target->miugoki = 1;
			break;

		case 46: /* まもりのオーラ */
			MES_PUSH(MsgAura);
			if (target->guard_lv > 0)
				break;
			target->guard_lv = 1;
			break;

		case 47: /* まもりのオーラ */
			MES_PUSH(MsgAura2);
			if (target->guard_lv > 1)
				break;
			target->guard_lv = 2;
			break;

		case 45:
			BtlEvPush(BEV_MES, -1);
			break;

		default:
			break;
		}
	}

	//------------------------------------------------------------------
	// ダメージによる目覚めと、能力・表示の更新
	//------------------------------------------------------------------
	BtlEvPush(BEV_CLR, 0);

	if (target->hp != 0)
	{
		if (target->nemuri != 0)
			if (target->nemuri <= 6 && dealt > 0 && (3 & BtlRndFar()) == 0)
			{
				target->nemuri = 0;
				BtlEvPush(BEV_UNIT, tid);
				MES_PUSH(MsgWakes);
			}
	}
	MemFree(bak);
	UnitCalcFar(tid);
	PtyStDrawFar(BtlWk->pty_st);
	if (target->hp != 0)
		BtlEvPush(BEV_OBJ_END, tid);
	// あくりょうによる反動を行動側に加算する
	if (actor->akuryo != 0 && (BtlRndFar() & 3) == 0 && dealt > 0)
	{
		s32 share;

		share = dealt >> 2;
		if (share == 0)
			share = 1;
		plan->akuryo_dmg += share;
	}
}

//--------------------------------------------------------------------------

// check_status_or_special_id.c

// バトル：EP を使うわざの判定

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// EP を使うわざなら 1 を返す。エナジードレインは EP に関係なく 1
// act_no : わざ番号
//--------------------------------------------------------------------------
u32 Ability_CheckStatusOrSpecialId(s32 act_no)
{
	u8 ep;

	if (act_no == 126) // エナジードレイン
		return 1;
	ep = BtlActGet(act_no)->pp_cost;
	return (u32)((0 - ep) | ep) >> 31; // ep が 0 でなければ 1
}

//--------------------------------------------------------------------------

// cmdplay.c

// バトル：敵の行動の選択と、バトルイベントの再生

//--------------------------------------------------------------------------

#undef BattleMotion_SetRecordChildValues
#undef BtlObjGet

struct BtlAct		  *BtlActGet(s32 action_id);
s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *state, s32 flags);

#define Battle_FindTaggedSlotByValue ((s32 (*)(s32))Battle_FindTaggedSlotByValue)

//--------------------------------------------------------------------------
// 行動の種類
//--------------------------------------------------------------------------
#define CMD_KOUGEKI 0 // こうげき
#define CMD_ENA 1	  // エナジー
#define CMD_ITEM 2	  // アイテム
#define CMD_BOUGYO 3  // ぼうぎょ
#define CMD_WAZA 4	  // 敵のわざ
#define CMD_BOUGYO2 7 // ぼうぎょ (効果 47)
#define CMD_NIGERU 99 // にげる

#define WAZA_KOUGEKI 1 // わざ番号：こうげき
#define MIKATA_MAX 7   // 仲間の番号の上限

// 敵の行動の記録 (BtlUnit の rewards を使う)
struct BattleAutomaticState
{
	unsigned initialized : 1;
	unsigned selection : 3; // 行動表の番号
	unsigned remaining : 28;
};

//--------------------------------------------------------------------------
// 敵の行動を選ぶ
// retry : 0 ならぼうぎょ・にげるだけをここで決め、1 なら残りの行動を決める
//--------------------------------------------------------------------------
s32 BattleCommand_SelectAutomatic(struct BattleActionRecord *req, s32 retry)
{
	register u32				 sel;
	struct BattleAutomaticState *memo;
	struct EnemyDefinition		*enm;
	s32							 item_ok;
	s32							 tries;
	s32							 keep_going;
	s8							*strategy;
	u8							*item_mask;
	s16							 cmd;
	s32							 tgt;
	s32							 tgt2;
	s32							 side;
	s32							 side2;
	struct ItemDefinition		*item;
	s32							 use_item;
	s8							 mode;
	struct BtlAct				*item_act;
	struct BtlAct				*act;
	struct BtlUnit				*unit;
	s32							 act_no;
	s32							 tgt_flg;
	u8							 efx;
	u8							 tgt_mode;

	// 0-255 の乱数で、重みの表から行動表の番号を引く
	s32 WeightRnd(u8 * weights)
	{
		s32 rnd;
		s32 total;
		s32 no;
		s32 n;

		rnd = BtlRndFar() & 0xff;
		total = weights[0];
		no = 0;
		n = 0;
		if (rnd >= total)
		{
		loop:
			n++;
			if (n <= 7)
			{
				total += weights[n];
				if (rnd < total)
					no = n;
				else
					goto loop;
			}
		}
		return no;
	}

	unit = UnitGetFar(req->unit_id);
	keep_going = item_ok = 1;
	tries = 0;
	sel = -1U;
	if (unit->class_index != 0) // 仲間は選ばない
		return;
	if (retry != 0 && req->command != CMD_WAZA)
		return;

	enm = Owner_GetRecordFar(unit->class_id);
	memo = (struct BattleAutomaticState *)&unit->rewards;
	strategy = &enm->action_strategy;
	item_mask = &enm->item_actions;

	// 行動表の番号を選ぶ
select_again:
	mode = *strategy;
	switch (mode)
	{
	case 0: // 重みの表から
		sel = WeightRnd(BattleCommand_SelectionWeights[0]);
		break;
	case 1:
		sel = WeightRnd(BattleCommand_SelectionWeights[1]);
		break;
	case 2:
		sel = WeightRnd(BattleCommand_SelectionWeights[2]);
		break;
	case 3: // 乱数で始め、順に進む
		if (memo->initialized == 0)
		{
			memo->selection = BtlRndFar();
			memo->initialized = 1;
		}
	read_saved_selection:
		sel = memo->selection;
		if (retry != 0)
		{
			memo->selection = sel + 1;
		}
		break;
	case 4: // 記録した番号から順に進む
		goto read_saved_selection;
	case 5: // 選び直すたびに次へ
		sel++;
		break;
	case 6: // 選び直さない
		break;
	}

	// アイテムを使う番号なら、持っている道具を確かめる
	use_item = (*item_mask >> sel) & 1;
	use_item &= item_ok;
	act_no = enm->actions[sel];
	req->command = CMD_WAZA;
	if (use_item != 0 && retry != 0)
	{
		if (!(0x1ff & unit->inventory[0]))
		{
			use_item = 0;
			if (enm->target_strategy == 0)
			{
				req->command = CMD_ITEM;
				req->parameter = 0x1fd;
				return;
			}
		}
		if (use_item != 0)
		{
			item = Item_GetDirectFar(unit->inventory[0]);
			if (item->use_type == 1)
			{
				item_act = BtlActGet(item->action_id);
				act_no = item->action_id;
				req->command = CMD_ITEM;
				tgt_flg = item_act->tgt_flg;
				req->parameter = 0;
				switch (tgt_flg)
				{
				case 1:
				case 2:
					goto item_checked;
				}
			}
			use_item = 0;
		item_checked:
			if (use_item == 0)
				goto disable_item;
		}
		else
		{
		disable_item:
			item_ok = 0;
		}
	}

	if (keep_going == 0)
		goto finish_target;

	// ぼうぎょ・にげるのわざは、自分を対象にする
	act = BtlActGet(act_no);
	efx = act->efx;
	switch (efx)
	{
	case 46:
		req->command = CMD_BOUGYO;
		req->target = Battle_FindTaggedSlotByValue(req->unit_id);
		break;
	case 47:
		req->command = CMD_BOUGYO2;
		req->target = Battle_FindTaggedSlotByValue(req->unit_id);
		break;
	case 49:
		req->command = CMD_NIGERU;
		req->target = Battle_FindTaggedSlotByValue(req->unit_id);
		break;
	}

	if (retry == 0)
	{
		switch (req->command)
		{
		case CMD_BOUGYO:
		case CMD_BOUGYO2:
			break;
		default:
			goto done;
		}
	}

	// エナジーならEPとふうじを確かめる
	// target_strategy が 0 でなければEP不足を、2 ならふうじも避ける
	if (req->command != CMD_ITEM && Ability_CheckStatusOrSpecialId(act_no) != 0)
	{
		req->command = CMD_ENA;
		req->parameter = act_no;
		if (act->pp_cost > unit->ep && enm->target_strategy != 0)
			goto next_candidate;
		if (unit->fuuji != 0 && enm->target_strategy == 2)
			goto next_candidate;
	}
	cmd = req->command;
	if (cmd == CMD_NIGERU && (u8)unit->chouhatsu != 0)
		goto next_candidate;
	if (retry != 0 && (cmd == CMD_BOUGYO || cmd == CMD_BOUGYO2))
		goto next_candidate;

	if (cmd == CMD_WAZA)
	{
		req->parameter = act_no;
		if (act_no == WAZA_KOUGEKI)
			req->command = CMD_KOUGEKI;
	}

	// 対象を選ぶ
	req->rng = act->rng;
	tgt_mode = act->target_mode;
	switch (tgt_mode)
	{
	case 2:
	case 4:
		tgt = BattleTarget_SelectForAction(req->unit_id, act);
		if (tgt == -2)
		{
			side = 0;
			if (req->unit_id <= (u32)MIKATA_MAX)
				side = 1;
			tgt = BattleTarget_SelectRandomPosition(side);
		}
		if (tgt != -1)
		{
			req->target = tgt;
			keep_going = 0;
		}
		break;
	case 1:
		tgt2 = BattleTarget_SelectForAction(req->unit_id, act);
		if (tgt2 == -2)
		{
			side2 = 0;
			if (req->unit_id <= (u32)MIKATA_MAX)
				side2 = 1;
			tgt2 = BattleTarget_SelectRandomPosition(side2);
		}
		if (tgt2 != -1)
		{
			req->target = tgt2;
			keep_going = 0;
		}
		break;
	case 3:
		req->target = Battle_FindTaggedSlotByValue(req->unit_id);
		break;
	default:
		tgt2 = Battle_FindTaggedSlotByValue(req->unit_id);
		req->target = tgt2;
		keep_going = 0;
		break;
	}

finish_target:
	if (retry == 0)
		keep_going = 0;
next_candidate:
	// 18 回選べなければ、ぼうぎょ
	if (keep_going != 0 && tries > 16)
	{
		req->command = CMD_BOUGYO;
		keep_going = 0;
	}
	tries++;
	if (keep_going != 0)
		goto select_again;
done:
	return;
}

//--------------------------------------------------------------------------
// DMA3 を三回設定し、IRQ 復帰命令のワードを呼び出し先として読む
//--------------------------------------------------------------------------
void Dma_StopAllThenRunHook(void)
{
	Dma_Set(0, 0, DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
	Dma_Set(0, 0, DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
	Dma_Set(0, 0, DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
	_call_via_r0(IwramIrqModeWord);
}

//--------------------------------------------------------------------------
// 止まっている再生を、対象の処理から始める
// cue : 当たったときの効果音 (0 なら変えない)
//--------------------------------------------------------------------------
void BattleEventRuntime_BeginPhase(s32 cue)
{
	struct BattleEventState *ev;

	ev = &BtlWk->events;
	if (ev->phase == BATTLE_PLAYBACK_IDLE)
	{
		ev->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
		if (cue != 0)
		{
			ev->pending_cue = cue;
		}
	}
}

//--------------------------------------------------------------------------
// イベントの列を空にして、再生を登録する
// frames : 最初のイベントまでの待ち
//--------------------------------------------------------------------------
s32 BattleEventRuntime_SchedulePhase(s32 frames)
{
	struct BattleEventState *ev;

	ev = &BtlWk->events;
	ev->queue.count = 0;
	ev->event_index = 0;
	ev->timer = frames;
	ev->phase = BATTLE_PLAYBACK_DISPATCH;
	BtlWk->plan.target_count = 0;
	return Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
}

//--------------------------------------------------------------------------
// バトルイベントの再生
//--------------------------------------------------------------------------
#define PLAY_MES 3		// メッセージの表示を待つ
#define PLAY_OKURI 5	// 送りのキーを待つ
#define PLAY_HIT 10		// 当たった者を点滅させる
#define PLAY_TAORE 11	// 倒れた敵を消す
#define PLAY_MES_ADD 13 // 続きのメッセージ (キーを待たない)

#define POSE_DAMAGE 5 // ダメージを受けたポーズ
#define PAL_HIT 7	  // 点滅の色

#define TAORE_FLASH 0x400 // 色を変えている間のタイマーの基準

#define OBJ_VRAM 0x06010000
extern volatile s32 gFrameTick;

//--------------------------------------------------------------------------
// スケジューラから呼ばれ、BtlTgtExec が積んだイベントを順に実行する
//--------------------------------------------------------------------------
void BattleEvent_Playback(void)
{
	u16						 sel[8];
	struct AnimationObject	*recs[4];
	struct BtlWork			*work = BtlWk;
	struct BattleEventState *ev = &work->events;

	// キャラのタイルを消す
	void ClrRecTiles(struct AnimationObject * rec, s32 value)
	{
		/* FAKEMATCH: needed to match: the address held in a local is added before the size is multiplied; written inside the Iwram_ClearWords call the multiply comes first (5 instructions reordered). */
		u8 *tiles = (u8 *)OBJ_VRAM + gVramBlockCache[rec->slot].offset;

		Iwram_ClearWords(tiles, rec->width * rec->height);
	}

	if (work->events.phase == BATTLE_PLAYBACK_IDLE)
		return;
	for (;;)
	{
		if (ev->phase == BATTLE_PLAYBACK_READY)
			return;
		if (ev->phase == BATTLE_PLAYBACK_RESOLVE_TARGET)
		{
			// 次の対象の効果を計算し、イベントを積む
			if (ev->queue.target_index < work->plan.target_count)
			{
				ev->queue.count = 0;
				ev->event_index = 0;
				ev->timer = 0;
				BtlTgtExec(&work->plan, ev->queue.target_index);
				ev->queue.target_index++;
				ev->phase = BATTLE_PLAYBACK_DISPATCH;
			}
			else
			{
				ev->phase = BATTLE_PLAYBACK_READY;
			}
		}
		else if (ev->phase == BATTLE_PLAYBACK_DISPATCH)
		{
			// 積んだイベントを順に実行する
			while (ev->event_index < ev->queue.count)
			{
				{
					s32 i = ev->event_index;

					if (ev->phase != BATTLE_PLAYBACK_DISPATCH)
						break;
					if (ev->timer != 0)
					{
						ev->timer--;
						return;
					}
					switch (ev->queue.opcodes[i])
					{
					case BEV_SE: // 効果音
						AudioCommand_PlayFar(ev->queue.operands[i]);
						break;
					case BEV_SCR:
						Battle_SetRuntimeFlagBit0(ev, ev->queue.operands[i]);
						break;
					case BEV_UNIT: // メッセージに入れる名前
						UiWork_PushValueSlotFar(ev->queue.operands[i], 1);
						break;
					case BEV_NUM: // メッセージに入れる数
						UiWork_PushValueSlotFar(ev->queue.operands[i], 5);
						break;
					case BEV_ITEM: // メッセージに入れるアイテム名
						UiWork_PushValueSlotFar(ev->queue.operands[i] & 0x1ff, 2);
						break;
					case BEV_ACT: // メッセージに入れるわざ名
						UiWork_PushValueSlotFar(ev->queue.operands[i] & 0x3fff, 4);
						break;
					case BEV_MARK:
						gBattleDisplayWork->marked = 1;
						break;
					case BEV_MES: // メッセージを出し、送りを待つ
						if ((s32)ev->queue.operands[i] >= 0)
							UiText_PrepareMessageWorkFar(ev->queue.operands[i]);
						ev->phase = PLAY_MES;
						gKeysPressedLatch = 0;
						break;
					case BEV_MES_ADD: // メッセージを出し、送りを待たない
						if ((s32)ev->queue.operands[i] >= 0)
							UiText_PrepareMessageWorkFar(ev->queue.operands[i]);
						ev->phase = PLAY_MES_ADD;
						break;
					case BEV_CLR: // メッセージに入れたものを消す
						UiWork_ClearValueNameTablesFar();
						break;
					case BEV_OBJ_EFX:
						BattleActor_DestroyTemporaryObject(ev->queue.operands[i]);
						break;
					case BEV_OBJ_BGN: // 当たった
						if (ev->pending_cue > 0)
							AudioCommand_PlayFar(ev->pending_cue);
						ev->actor_id = ev->queue.operands[i];
						ObjectDispatch_ApplyArgumentToChildrenFar(
							BtlObjGet(ev->actor_id)->object, POSE_DAMAGE);
						ev->phase = PLAY_HIT;
						ev->timer = 0;
						break;
					case BEV_OBJ_RES: // 倒れた
					{
						struct BtlUnit		   *unit;
						struct AnimationObject *rec;
						s32						n;

						ev->actor_id = ev->queue.operands[i];
						BattleEnemy_RecordDefeat(ev->actor_id, ev->flags);
						BattleActor_ResetRuntimeFields(ev->actor_id);
						unit = UnitGetFar(ev->actor_id);
						for (n = 0;
							(rec = GetMotionRecord(BtlObjGet(ev->actor_id)->object, n)) != 0; n++)
						{
							if (unit->side != BATTLE_UNIT_ENEMY)
								AnimationObjects_SelectAnimationFar(rec, 4);
							else
								AnimationObjects_SelectAnimationFar(rec, 5);
						}
						if (unit->side == BATTLE_UNIT_ENEMY)
						{
							ev->phase = PLAY_TAORE;
							ev->timer = 0;
						}
						break;
					}
					case BEV_REDRAW: // 仲間の状態を描き直す
						PtyStDrawFar(BtlWk->pty_st);
						break;
					case BEV_OBJ_END: // 状態アイコン・色・ポーズを今の状態に合わせる
					{
						struct BtlObj *slot;

						BattleUnit_BuildStatusFlags(
							ev->queue.operands[i], BtlObjGet(ev->queue.operands[i]));
						slot = BtlObjGet(ev->queue.operands[i]);
						BattleMotion_SetRecordChildValues(
							slot->object, BattleMotion_GetSlotPalette(ev->queue.operands[i]));
						BattlePres_SetActorModeAndAction(ev->queue.operands[i]);
						break;
					}
					}
					ev->event_index++;
				}
			}
			if (ev->phase == BATTLE_PLAYBACK_DISPATCH)
				ev->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
		}
		else if (ev->phase == PLAY_MES || ev->phase == PLAY_MES_ADD)
		{
			// メッセージが出きるまで待つ
			if (UiWork_IsCompleteFar() == 0)
				return;
			if (ev->phase == PLAY_MES_ADD)
			{
				ev->phase = BATTLE_PLAYBACK_DISPATCH;
				ev->timer = 0;
			}
			else
			{
				ev->phase = PLAY_OKURI;
				ev->display_source = -1;
				ev->timer = gFrameTick;
			}
		}
		else if (ev->phase == PLAY_OKURI)
		{
			// 矢印を出して送りのキーを待つ
			// B を押し続けるか送りのキーを押すと進み、10 ティックを過ぎれば押し続けていても進む
			struct BattlePromptSprite  *spr = &ev->display_entry;
			u8						   *arrow = BattlePres_AdvanceArrowTiles;
			s32							pat = (s32)arrow + ((gFrameCount >> 2) & 7) * 128;
			struct BattleDisplayWork   *disp = gBattleDisplayWork;
			struct UiWindow			   *window = disp->window;
			struct BattleDisplayOffset *offset = disp->offset;
			s32							x = 0;

			if (ev->display_source == -1)
				ev->display_source = work->prompt_slot;
			UiWork_ClearValueNameTablesFar();
			QueueIoWriteDelay10((u32)&REG_WINOUT, 4);
			QueueIoWriteDelay6((u32)&REG_WINOUT, 16);
			((u32 *)spr)[1] = 0xa000;
			((u32 *)spr)[2] = x;
			spr->tile = Resource_GetBuffer(ev->display_source, pat);
			x = window->x * 8 + (offset->x >> 8) + 4;
			spr->x = x;
			spr->y = Trig_Sin(gFrameCount << 12) / 32768 + window->y * 8 + (offset->y >> 8) + 6;
			if ((BATTLE_OPTIONS & KEY_B) || (gKeysPressedLatch & KEY_OKURI)
				|| ((u32)(gFrameTick - ev->timer) > 10 && (BATTLE_OPTIONS & KEY_OKURI)))
			{
				AudioCommand_PlayFar(111);
				ev->phase = BATTLE_PLAYBACK_DISPATCH;
				ev->timer = 0;
			}
			else
			{
				Runtime_PushSlotEntry(spr, 240);
				return;
			}
		}
		else if (ev->phase == PLAY_HIT)
		{
			// 2 フレームごとに色を変え、9 フレームで戻る
			if (ev->timer & 1)
			{
				if (ev->timer & 2)
				{
					struct BtlObj *slot;

					sel[0] = 0xff;
					slot = BtlObjGet(ev->actor_id);
					BattleMotion_SetRecordChildValues(
						slot->object, BattleMotion_GetSlotPalette(ev->actor_id));
				}
				else
				{
					sel[0] = ev->actor_id;
					sel[1] = 0xff;
					BattleMotion_SetRecordChildValues(BtlObjGet(ev->actor_id)->object, PAL_HIT);
				}
				BattleLayout_HighlightPartyPanelsFar(sel);
			}
			ev->timer++;
			if (ev->timer <= 8)
				return;
			ev->phase = BATTLE_PLAYBACK_DISPATCH;
			ev->timer = 0;
		}
		else if (ev->phase == PLAY_TAORE)
		{
			// flags があれば、まず 30 フレームのあいだ色を変える
			if (ev->timer == 0 || ev->timer >= TAORE_FLASH)
			{
				s32 frame = 6;

				if (ev->timer == 0 && ev->flags != 0)
				{
					s32 kind = Summon_GetLayoutKind(UnitGetFar(ev->actor_id)->class_id);

					if (kind >= 0)
					{
						kind--;
						if (kind < 0)
							kind = 0;
						AudioCommand_PlayFar(kind + 146);
					}
					ev->timer = TAORE_FLASH;
				}
				if (ev->timer > TAORE_FLASH + 29)
					ev->timer = 0;
				if (ev->timer == 0)
				{
					s32 kind = Summon_GetLayoutKind(UnitGetFar(ev->actor_id)->class_id);

					if (kind >= 0)
						AudioCommand_PlayFar(kind + 146);
				}
				if (ev->timer >= TAORE_FLASH)
					frame = (ev->timer - TAORE_FLASH) / 8 % 5 + 1;
				if (frame == 6 || (ev->timer & 7) == 0)
				{
					struct AnimationObject	*rec;
					s32						 n;
					struct AnimationObject **slot;
					s32						 none;

					for (n = 0, slot = recs, none = 0xff;
						(rec = GetMotionRecord(BtlObjGet(ev->actor_id)->object, n)) != 0; n++)
					{
						struct AnimationEntry *child;
						u8					   old;

						*slot++ = rec;
						child = rec->entries[0];
						old = child->frame;
						child->param = frame;
						child->frame = old | none;
					}
				}
				ev->timer++;
				return;
			}
			if (ev->timer == 4)
			{
				BattleActor_RemoveFromLists(ev->actor_id);
				ev->timer++;
				return;
			}
			if (ev->timer > 4)
			{
				// タイルを順に描き換えて消す
				struct BtlObj		   *slot;
				struct AnimationObject *rec;
				s32						count;
				s32						frame;
				s32						base;

				slot = BtlObjGet(ev->actor_id);
				slot->fading = 1;
				for (count = 0; (rec = GetMotionRecord(slot->object, count)) != 0; count++)
					recs[count] = rec;
				base = ev->timer * 4;
				frame = base - 20;
				if (frame > 127)
				{
					s32 k;

					for (k = 0; k < count; k++)
						ClrRecTiles(recs[k], 0);
					ActivateBattleObjectSlot(ev->actor_id);
					ev->phase = BATTLE_PLAYBACK_DISPATCH;
					ev->timer = 0;
					return;
				}
				{
					s32 k;

					for (k = 0; k < count; k++)
					{
						render_animated_tile_frameFar((u8 *)recs[k], frame);
						render_animated_tile_frameFar((u8 *)recs[k], base - 19);
						render_animated_tile_frameFar((u8 *)recs[k], base - 18);
						render_animated_tile_frameFar((u8 *)recs[k], base - 17);
					}
				}
				ev->timer++;
				return;
			}
			ev->timer++;
			return;
		}
	}
}

u32 BattleEventRuntime_Reset(void)
{
	struct BattleEventState *events;

	events = &BtlWk->events;
	events->phase = BATTLE_PLAYBACK_IDLE;
	events->queue.count = 0;
	events->event_index = 0;
	events->timer = 0;
	events->queue.target_index = 0;
	events->pending_cue = 0x86;
	events->flags = 0;
}

s32 BattleEventRuntime_WaitForReady(void)
{
	s32						 state;
	struct BattleEventState *events;

	events = &BtlWk->events;
	state = events->phase;
	if (state == BATTLE_PLAYBACK_IDLE)
	{
		events->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
		state = BATTLE_PLAYBACK_RESOLVE_TARGET;
	}
	if (state != BATTLE_PLAYBACK_READY)
	{
		do
		{
			WaitFrames(1U);
		} while (events->phase != BATTLE_PLAYBACK_READY);
	}
	Scheduler_RemoveCallback(BattleEvent_Playback);
	return BattleEventRuntime_Reset();
}

//--------------------------------------------------------------------------

// is_unit_listed.c

// バトル：ユニットが戦闘中の並びにいるかの判定

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// ユニットが自分の側の並びにいれば 1 を返す
// id : ユニット番号（0-7 味方、それ以外は敵）
//--------------------------------------------------------------------------
u32 BattleParty_IsUnitListed(u32 id)
{
	u16 list[8];
	s32 side;
	s32 cnt;
	s32 i;

	side = BATTLE_SIDE_PARTY;
	if (id > 7)
		side = BATTLE_SIDE_ENEMIES;
	cnt = BattleParty_ListActorIds(side, list);

	for (i = 0; i < cnt; i++)
	{
		if (list[i] == id)
			break;
	}

	return i != cnt;
}

#undef BattlePresentation_WaitForAdvance
#undef BattleUnit_ProcessTurnEnd

struct DjinnRecoveryTable *Trade_GetOfferStateFar(s32 side);

s32 BattlePlacement_CountValidEntries(u32 unit_id, u8 *counts)
{
	u16						  unit_ids[8];
	struct DjinnRecoveryList *list;
	s32						  total;
	s32						  count;
	s32						  i;
	s32						  j;
	s32						  side_mask;
	s32						  side;

	count = 0;
	side_mask = BATTLE_SIDE_PARTY;
	if (unit_id > 7)
		side_mask = BATTLE_SIDE_ENEMIES;
	total = BattleParty_ListActorIds(side_mask, unit_ids);
	side = 0;
	if (unit_id > 7)
		side = 1;
	list = &Trade_GetOfferStateFar(side)->list;
	if (counts != 0)
		for (j = 3; j >= 0; j--)
			counts[j] = 0;
	i = 0;
	if (list->count != 0)
	{
		do
		{
			if (list->entries[i].turns == -1)
			{
				for (j = 0; j < total; j++)
					if (unit_ids[j] == list->entries[i].unit_id)
						break;
				if (j != total)
				{
					if (counts != 0)
						counts[list->entries[i].element]++;
					count++;
				}
			}
			i++;
		} while (i != list->count);
	}
	return count;
}

//--------------------------------------------------------------------------
// command.c
// バトル：行動の宣言と、対象・効果の計画
//--------------------------------------------------------------------------

s32					   GameFlag_TestFar(s32 flag);

#define Battle_GetTaggedSlotValue ((s32 (*)(s32))Battle_GetTaggedSlotValue)
#define Trade_AddOfferFar ((void (*)(s32, s32, s32))Trade_AddOfferFar)

// ジンの指定：属性 (bit 8-11) と番号 (bit 0-7)
#define DJINN_ELEMENT(v) (((v) >> 8) & 15)
#define DJINN_INDEX(v) ((v) & 255)
#define CMD_DJINN 5	  // ジン
#define CMD_SHOUKAN 6 // しょうかん
#define CMD_NONE 8	  // 何もしない
#define TGT_MAX 14
void UiText_ShowMessageAndWaitCoreFar(s32 message);
s32	 Djinn_ActivateFar(s32 unit_id, s32 element, s32 index);
s32	 Trade_RemoveOfferFar(s32 unit_id, s32 element, s32 index);
s32	 BattleEventRuntime_SchedulePhase(s32 frames);

//--------------------------------------------------------------------------
// 行動を宣言し、対象と効果の計画を作る
// 返り値 : 0 = 実行する、-1 = 失敗 (メッセージ済み)、-2 = 何もしない
//--------------------------------------------------------------------------
s32 BattleCommand_BuildPlan(struct BattleActionRecord *req, struct BtlPlan *plan)
{
	struct BtlUnit *unit = UnitGetFar(req->unit_id);
	struct BtlWork *work = BtlWk;
	s32				tid;
	s32				act_no;

	//----------------------------------------------------------------------
	// わざの範囲にいる者を対象に並べる
	// 誰もいなければ、身を守って -1
	//----------------------------------------------------------------------
	s32 TgtSet(s32 no)
	{
		struct BtlAct *act = BtlActGet(no);
		s32			   mode = act->target_mode;
		s32			   dead_ok = 0;
		s32			   cnt;
		s32			   pty_cnt;
		s32			   enm_cnt;
		s32			   center;
		s32			   rng;
		s32			   pos;
		s32			   id;

		switch (act->efx)
		{
		case 5: // ふっき
		case 56:
		case 57:
			dead_ok = 1;
			break;
		}
		switch (mode)
		{
		case 0: // ひとり
			plan->tgt_dist[0] = mode;
			plan->target_count = 1;
			plan->tgt_no[0] = tid;
			plan->tgt_adj[0] = 1;
			break;
		case 4:
			plan->tgt_dist[0] = 0;
			plan->target_count = 1;
			plan->tgt_no[0] = tid;
			plan->tgt_adj[0] = 1;
			break;
		default:
			// 狙った位置から rng - 1 までの並び
			cnt = 0;
			for (pos = 0; work->party_units[pos] != BATTLE_UNIT_LIST_END; pos++)
				;
			pty_cnt = pos;
			for (pos = 0; work->enm_no[pos] != BATTLE_UNIT_LIST_END; pos++)
				;
			enm_cnt = pos;
			center = req->target & 15;
			rng = req->rng;
			{
				s32 first = center - rng + 1;
				s32 last = center + rng - 1;

				for (pos = first; pos <= last; pos++)
				{
					if (pos < 0)
						continue;
					if (req->target & 128) // 敵の側
					{
						if (pos >= enm_cnt)
							continue;
						id = work->enm_no[pos];
						if (id == BATTLE_UNIT_REMOVED)
							continue;
						if (!dead_ok && UnitGetFar(id)->hp == 0)
							continue;
						plan->tgt_adj[cnt] = 1;
						plan->tgt_dist[cnt] = pos - center;
						plan->tgt_no[cnt] = id;
						cnt++;
					}
					else
					{
						if (pos >= pty_cnt)
							continue;
						id = work->party_units[pos];
						if (id == BATTLE_UNIT_REMOVED)
							continue;
						if (!dead_ok && UnitGetFar(id)->hp == 0)
							continue;
						plan->tgt_adj[cnt] = 1;
						plan->tgt_dist[cnt] = pos - center;
						plan->tgt_no[cnt] = id;
						cnt++;
					}
				}
			}
			plan->target_count = cnt;
			if (cnt <= 0)
			{
				UiWork_PushValueSlotFar(req->unit_id, 1);
				UiText_ShowMessageAndWaitCoreFar((s32)MsgActorDefends);
				if (unit->guard_lv == 0)
					unit->guard_lv = 1;
				return -1;
			}
		}
	}

	tid = Battle_GetTaggedSlotValue(req->target);
	BattleEventRuntime_Reset();
	plan->akuryo_dmg = 0;
	plan->aid = req->unit_id;
	plan->target_count = 0;
	plan->presentation_flags = 0;
	plan->failure = 0;
	plan->elm = 4;
	UiWork_ClearValueNameTablesFar();
	if (unit->hp == 0)
		return -2;

	// デバッグ：R を押していれば敵全員 (SELECT も押せば味方全員) を倒す
	if (BATTLE_AUTO_MODE && GameFlag_TestFar(0x16d) && (BATTLE_OPTIONS & KEY_R))
	{
		s32 teki = 1;
		s32 i;
		s32 id;

		if (BATTLE_OPTIONS & KEY_SELECT)
			teki = 0;
		for (i = 0;; i++)
		{
			if (teki)
				id = work->enm_no[i];
			else
				id = work->party_units[i];
			if (id == BATTLE_UNIT_LIST_END)
				break;
			if (id == BATTLE_UNIT_REMOVED)
				continue;
			if (Owner_AdjustFirstValueFar(id, 0xc0000000) == 0)
			{
				BtlEvPush(BEV_OBJ_BGN, id);
				BtlEvPush(BEV_OBJ_RES, id);
			}
		}
		BattleEv_DispatchQueued();
		return -2;
	}

	// 動けない状態
	UiWork_ClearValueNameTablesFar();
	if (unit->miugoki)
	{
		unit->miugoki = 0;
		UiWork_PushValueSlotFar(req->unit_id, 1);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgUnableToMove);
		return -1;
	}
	if (unit->nemuri)
	{
		UiWork_PushValueSlotFar(req->unit_id, 1);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgIsAsleep);
		return -1;
	}
	if (unit->shibire)
	{
		UiWork_PushValueSlotFar(req->unit_id, 1);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgIsParalyzed);
		return -1;
	}
	// かなしばりは 1/4 で動けない
	if ((unit->restraint & 1) && req->command != CMD_BOUGYO && (BtlRndFar() & 3) == 0)
	{
		UiWork_PushValueSlotFar(req->unit_id, 1);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgIsBound);
		return -1;
	}
	if (req->command == CMD_NONE)
		return -2;
	act_no = WAZA_KOUGEKI;
	{
		s32 i;

		for (i = 0; i < TGT_MAX; i++)
			plan->tgt_crit[i] = 0;
		for (i = 0; i < TGT_MAX; i++)
			plan->tgt_hit[i] = -1;
	}

	//----------------------------------------------------------------------
	// 行動ごとの宣言
	//----------------------------------------------------------------------
	switch (req->command)
	{
	case CMD_NIGERU:
		if ((u16)req->unit_id <= MIKATA_MAX)
		{
			UiText_ShowMessageAndWaitCoreFar((s32)MsgPartyFlees);
		}
		else
		{
			UiWork_PushValueSlotFar(req->unit_id, 1);
			UiText_ShowMessageAndWaitCoreFar((s32)MsgActorRuns);
		}
		BattlePresentation_WaitForAdvance();
		plan->outcome = 7;
		return 0;

	case CMD_KOUGEKI:
		// 武器の力が発動すれば、そのわざになる
		act_no = RollWeaponUnleashFar(unit);
		if (TgtSet(act_no) == -1)
			return -1;
		if (act_no != WAZA_KOUGEKI)
		{
			UiWork_PushValueSlotFar(req->unit_id, 1);
			UiWork_PushValueSlotFar(Inventory_GetEquippedItemFar(unit, 1), 2);
			UiText_ShowMessageAndWaitCoreFar((s32)MsgWeaponHowls);
			BattleEv_MarkDisplay();
			UiWork_PushValueSlotFar(act_no, 4);
			UiText_ShowMessageAndWaitCoreFar((s32)MsgWeaponHowls + 1);
		}
		break;

	case CMD_ENA:
	{
		struct BtlAct *act;
		s32			   ok = 1;

		act_no = req->parameter;
		act = BtlActGet(act_no);
		if (TgtSet(act_no) == -1)
			return -1;
		UiWork_PushValueSlotFar(req->unit_id, 1);
		UiWork_PushValueSlotFar(act_no, 4);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgActorCasts);
		// EPが足りないか、ふうじなら失敗
		if (unit->ep < act->pp_cost)
		{
			plan->failure = 2;
			ok = 0;
		}
		if (unit->fuuji)
		{
			plan->failure = 1;
			ok = 0;
		}
		if (ok)
		{
			plan->failure = 0;
			unit->ep -= act->pp_cost;
			UnitRatioFar(req->unit_id);
			if (unit->ep < 0)
				unit->ep = 0;
			if (unit->ep > unit->max_ep)
				unit->ep = unit->max_ep;
		}
	}
	break;

	case CMD_ITEM:
	{
		struct ItemDefinition *item;

		if (req->parameter < 0)
		{
			UiWork_PushValueSlotFar(req->unit_id, 1);
			UiText_ShowMessageAndWaitCoreFar((s32)MsgItemAlreadyUsed);
			return -1;
		}
		item = Item_GetDirectFar(unit->inventory[req->parameter]);
		act_no = item->action_id;
		if (act_no == 0 || (unit->inventory[req->parameter] & 0x400))
		{
			UiWork_PushValueSlotFar(req->unit_id, 1);
			UiText_ShowMessageAndWaitCoreFar((s32)MsgActorDefends);
			if (unit->guard_lv == 0)
				unit->guard_lv = 1;
			return -1;
		}
		if (TgtSet(act_no) == -1)
			return -1;
		UiWork_PushValueSlotFar(req->unit_id, 1);
		UiWork_PushValueSlotFar(unit->inventory[req->parameter], 2);
		if (item->use_type == 2 || item->use_type == 0)
		{
			switch (item->type)
			{
			case 1:
			case 3:
			case 6:
			case 7:
			case 8:
				UiText_ShowMessageAndWaitCoreFar((s32)MsgActorRaisesItem); // かざした
				break;
			default:
				UiText_ShowMessageAndWaitCoreFar((s32)MsgActorUsesBattleItem); // つかった
				break;
			}
		}
		else
		{
			UiText_ShowMessageAndWaitCoreFar((s32)MsgActorUsesBattleItem);
		}
	}
	break;

	case CMD_BOUGYO:
	case CMD_BOUGYO2:
		UiWork_PushValueSlotFar(req->unit_id, 1);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgActorDefends);
		return -1;

	case CMD_NONE:
		return -2;

	case CMD_WAZA:
	{
		s32 mes;

		act_no = req->parameter;
		if (TgtSet(act_no) == -1)
			return -1;
		UiWork_PushValueSlotFar(req->unit_id, 1);
		UiWork_PushValueSlotFar(act_no, 4);
		if ((BtlActGet(act_no)->tgt_flg & 15) == 6)
			mes = (s32)MsgActorUnleashesAbility;
		else
			mes = (s32)MsgActorUsesAbility;
		// わざごとの言い方
		switch (act_no)
		{
		case 435: // ちょうおんぱ
		case 436:
			mes = (s32)MsgEmitsUltrasonicWaves;
			break;
		case 224: // ヒートミラージュ
			mes = (s32)MsgActorCasts;
			break;
		case 500: // おどし
			mes = (s32)MsgGlowersFerociously;
			break;
		case 501: // てがふるえている
			mes = (s32)MsgIsIntimidated;
			break;
		case 499: // けんせい
			mes = (s32)MsgBalefulGaze;
			break;
		case 494: // ミミズをたべる
			mes = (s32)MsgEatsWorms;
			break;
		case 437: // スクウィールロアー ～ じごくのとおぼえ
		case 438:
		case 439:
		case 440:
		case 441:
			mes = (s32)MsgLetsOutAbility;
			break;
		case 442: // ウイングビート ～ ウイングストローク
		case 443:
		case 444:
			mes = (s32)MsgActorUsesAbility;
			break;
		case 472: // くさったたいえき
			mes = (s32)MsgEnemyUnleashesAbility;
			break;
		case 488: // せつないひとみ
			mes = (s32)MsgGlowersMiserably;
			break;
		case 492: // ふらんしゅう
			mes = (s32)MsgSmellOfDecay;
			break;
		case 495: // きょうぼうか
			mes = (s32)MsgFuriousRage;
			break;
		case 503: // ぞうしょくうんどう
			mes = (s32)MsgAttemptsToDivide;
			break;
		case 504: // なかまをさがす
			mes = (s32)MsgLooksForAllies;
			break;
		case 508: // たすけをさがす
			mes = (s32)MsgLooksForHelp;
			break;
		}
		UiText_ShowMessageAndWaitCoreFar(mes);
	}
	break;

	case CMD_DJINN:
		// セットされていなければ、スタンバイのジンをセットし直す
		act_no = Djinn_GetDefinitionHeaderFar(
			DJINN_ELEMENT(req->parameter), DJINN_INDEX(req->parameter));
		if (!Djinn_IsActiveFar(
				req->unit_id, DJINN_ELEMENT(req->parameter), DJINN_INDEX(req->parameter)))
		{
			if (Trade_CanOfferDjinnFar(
					req->unit_id, DJINN_ELEMENT(req->parameter), DJINN_INDEX(req->parameter)))
			{
				BtlActGet(act_no);
				BattlePres_SetActorModes(0, 0);
				Djinn_ActivateFar(
					req->unit_id, DJINN_ELEMENT(req->parameter), DJINN_INDEX(req->parameter));
				Trade_RemoveOfferFar(
					req->unit_id, DJINN_ELEMENT(req->parameter), DJINN_INDEX(req->parameter));
				UnitCalcFar(req->unit_id);
				BattleEventRuntime_Reset();
				BattleEventRuntime_SchedulePhase(30);
				BtlEvPush(BEV_UNIT, req->unit_id);
				BtlEvPush(BEV_ACT,
					DJINN_ELEMENT(req->parameter) * 20 + DJINN_INDEX(req->parameter) + 300);
				BtlEvPush(BEV_SE, 175);
				BtlEvPush(BEV_REDRAW, 0);
				BtlEvPush(BEV_MES, (s32)MsgDjinnSet);
				BtlEvPush(BEV_OBJ_END, req->unit_id);
				AudioCommand_PlayFar(212);
				ObjectDispatch_ApplyArgumentToChildrenFar(BtlObjGet(req->unit_id)->object, 3);
				ObjectDispatch_ApplyValueToChildrenFar(BtlObjGet(req->unit_id)->object, 32);
				BattleFx_PlayUnitElementEffect(req->unit_id, DJINN_ELEMENT(req->parameter), 3, 0);
				BattleEventRuntime_WaitForReady();
				return -2;
			}
			// リカバリー中
			UiWork_PushValueSlotFar(req->unit_id, 1);
			UiWork_PushValueSlotFar(act_no, 4);
			AudioCommand_PlayFar(114);
			UiText_ShowMessageAndWaitCoreFar((s32)MsgDjinnInRecovery);
			WaitFrames(60);
			return -1;
		}
		// セットされたジンを解き放つ
		{
			struct BtlAct *act;

			if (TgtSet(act_no) == -1)
				return -1;
			Trade_AddOfferFar(
				req->unit_id, DJINN_ELEMENT(req->parameter), DJINN_INDEX(req->parameter));
			act = BtlActGet(act_no);
			UiWork_PushValueSlotFar(req->unit_id, 1);
			UiWork_PushValueSlotFar(act_no, 4);
			UiText_ShowMessageAndWaitCoreFar((s32)MsgActorUnleashesDjinn);
			plan->elm = act->damage_class;
		}
		break;

	case CMD_SHOUKAN:
	{
		const struct SummonDefinition *summon;
		struct DjinnRecoveryList	  *list;
		s32							   side;
		u8							   counts[4];
		/* FAKEMATCH: needed to match: unread: the reference frame keeps 8 bytes below counts */
		u8	unused[8];
		s32 i;

		// 属性ごとのスタンバイの数が足りるか
		summon = SummonDefinition_Get(req->parameter);
		BattlePlacement_CountValidEntries(req->unit_id, counts);
		side = 0;
		if ((u16)req->unit_id > MIKATA_MAX)
			side = 1;
		list = &Trade_GetOfferStateFar(side)->list;
		for (i = 0; i < 4 && counts[i] >= summon->djinn_required[i]; i++)
			counts[i] = summon->djinn_required[i];
		act_no = summon->name_message_id;
		if (TgtSet(act_no) == -1)
			return -1;
		if (i != 4)
		{
			UiWork_PushValueSlotFar(req->unit_id, 1);
			UiWork_PushValueSlotFar(act_no, 4);
			UiText_ShowMessageAndWaitCoreFar((s32)MsgSummonLacksDjinn);
			return -1;
		}
		UiWork_PushValueSlotFar(req->unit_id, 1);
		UiWork_PushValueSlotFar(act_no, 4);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgActorSummons);
		// 使うスタンバイのジンに印 (-2) をつける
		for (i = 0; i != list->count; i++)
		{
			struct DjinnRecoveryEntry *entry = &list->entries[i];

			if (entry->turns == -1 && BattleParty_IsUnitListed(entry->unit_id)
				&& counts[entry->element] != 0)
			{
				entry->turns = -2;
				counts[entry->element]--;
			}
		}
	}
	break;
	}

	//----------------------------------------------------------------------
	// こうげきの計画
	//----------------------------------------------------------------------
	if (act_no == WAZA_KOUGEKI)
	{
		struct BtlUnit *tgt = UnitGetFar(plan->tgt_no[0]);

		plan->act_no = WAZA_KOUGEKI;
		plan->elm = Item_GetEquippedElementFar(req->unit_id);
		plan->outcome = 2;
		if (unit->class_index == 0) // 敵
		{
			plan->presentation_flags = Battle_GetSpriteFlagNibble(unit->class_id) | 0x4000;
		}
		else
		{
			plan->presentation_flags = 0;
			switch (unit->class_id)
			{
			case ROBIN:
				plan->presentation_flags = 0x4001;
				break;
			case GERALD:
				plan->presentation_flags = 0x4001;
				break;
			case IWAN:
				plan->presentation_flags = 0x4004;
				break;
			case MEARI:
				plan->presentation_flags = 0x4004;
				break;
			case JASMINE:
				plan->presentation_flags = 0x4001;
				break;
			}
		}
		UiWork_PushValueSlotFar(req->unit_id, 1);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgActorAttacks);
		// 外れ：まぼろしなら 153/256、ほかに 1/32
		// 相手が動けなければ外れない
		/* FAKEMATCH: needed to match: the one-pass block keeps the native test order */
		do
		{
			if (tgt->hp == 0 || tgt->nemuri != 0 || tgt->shibire != 0 || tgt->miugoki != 0
				|| tgt->charm != 0)
				break;
			if (unit->maboroshi != 0 && (BtlRndFar() & 255) <= 152)
				plan->tgt_adj[0] = 0;
			if ((BtlRndFar() & 31) == 0)
				plan->tgt_adj[0] = 0;
		} while (0);
		if (GameFlag_TestFar(366))
			plan->tgt_adj[0] = 0;
		// 会心：1/32、または装備による確率
		if (tgt->hp != 0)
		{
			if ((BtlRndFar() & 31) == 0)
				plan->tgt_crit[0] = 1;
			else if ((Equipment_GetUnleashRateBonusFar(unit) << 16) / 200 > (BtlRndFar() & 0xffff))
				plan->tgt_crit[0] = 1;
		}
	}

	//----------------------------------------------------------------------
	// わざの計画
	//----------------------------------------------------------------------
	else
	{
		struct BtlAct *act = BtlActGet(act_no);

		plan->elm = act->damage_class;
		plan->presentation_flags = 0;
		plan->act_no = act_no;
		if (act->efx == 65 || act->efx == 41 || act->efx == 42 || act->efx == 43 || act->efx == 44
			|| act->efx == 68)
		{
			// ある確率で、ダメージの倍率を上げる
			s32 chance;
			s32 amount;

			if (act->efx == 65 || act->efx == 68)
				chance = 153;
			else if (act->efx == 41 || act->efx == 43)
				chance = 32;
			else
				chance = 64;
			if (act->efx == 65 || act->efx == 41 || act->efx == 42)
				amount = 1;
			else
				amount = 2;
			if ((BtlRndFar() & 255) < chance)
			{
				s32 i;

				for (i = 0; i < plan->target_count; i++)
					plan->tgt_adj[i] += amount;
			}
		}
		else if (act->efx >= 36 && act->efx <= 40)
		{
			// 会心の確率 1/64 ～ 1/4
			s32 mask;

			switch (act->efx)
			{
			case 36:
				mask = 63;
				break;
			case 37:
				mask = 31;
				break;
			case 38:
				mask = 15;
				break;
			case 39:
				mask = 7;
				break;
			case 40:
			default:
				mask = 3;
				break;
			}
			if ((BtlRndFar() & mask) == 0)
			{
				s32 i;

				for (i = 0; i < plan->target_count; i++)
					plan->tgt_crit[i] = 2;
			}
		}
		else if (act_no == 178) // めっさつのじゅつ：命中を先に決める
		{
			s32 i;

			for (i = 0; i < plan->target_count; i++)
				plan->tgt_hit[i] =
					BtlHitChk(req->unit_id, plan->tgt_no[i], act->damage_class, act->efx, 100);
		}
		if (act_no >= 0 && act_no <= 518)
		{
			plan->presentation_flags = Battle_ActionFlags[act_no];
			if (plan->tgt_adj[0] > 1)
				plan->presentation_flags =
					Battle_ActionFlags[act_no] + (plan->tgt_adj[0] << 12) - 0x1000;
		}
		if ((act_no >= 0 && act_no <= 517) && Battle_ActionStatus[act_no] != 0)
			plan->outcome = Battle_ActionStatus[act_no];
		else if (Ability_CheckStatusOrSpecialId(act_no))
			plan->outcome = 3;
		else if (plan->presentation_flags != 0)
		{
			if (unit->class_index == 0)
				plan->outcome = 8;
			else
				plan->outcome = 3;
		}
		else
			plan->outcome = 1;
		if (BtlEfxRevChkFar(act->efx))
			plan->presentation_flags |= 0x10000;
		if (act_no == 178 && plan->tgt_hit[0] != 0)
			plan->presentation_flags |= 0x1000;
	}
	if (req->command == CMD_ITEM && plan->outcome != 5 && plan->outcome != 9)
		plan->outcome = 4;
	plan->cmd = req->command;
	return 0;
}

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

s32 BattleUnit_TickDefenseModifier(s32 unit_id)
{
	struct BtlUnit *state = UnitGetFar(unit_id);

	if (state->def_cnt != 0)
	{
		state->def_cnt--;
		if (state->def_cnt == 0)
		{
			state->def_lv = 0;
			return 1;
		}
		if (state->def_lv < 0 && BattleUnit_RollCounterEarlyEnd(unit_id, state->def_cnt, 20) != 0)
		{
			state->def_lv = 0;
			state->def_cnt = 0;
			return 1;
		}
	}
	return 0;
}

s32 BattleUnit_TickResistanceModifier(s32 unit_id)
{
	struct BtlUnit *state = UnitGetFar(unit_id);

	if (state->res_cnt != 0)
	{
		state->res_cnt--;
		if (state->res_cnt == 0)
		{
			state->res_lv = 0;
			return 1;
		}
		if (state->res_lv < 0 && BattleUnit_RollCounterEarlyEnd(unit_id, state->res_cnt, 20) != 0)
		{
			state->res_lv = 0;
			state->res_cnt = 0;
			return 1;
		}
	}
	return 0;
}

s32 BattleUnit_TickDelusion(s32 unit_id)
{
	struct BtlUnit *state = UnitGetFar(unit_id);
	if (state->maboroshi != 0)
	{
		state->maboroshi--;
		if (state->maboroshi == 0)
			return 1;
		if (BattleUnit_RollCounterEarlyEnd(unit_id, state->maboroshi, 30) != 0)
		{
			state->maboroshi = 0;
			return 1;
		}
	}
	return 0;
}

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

s32 BattleUnit_TickCharm(s32 unit_id)
{
	struct BtlUnit *state = UnitGetFar(unit_id);
	if (state->charm != 0)
	{
		state->charm--;
		if (state->charm == 0)
			return 1;
		if (BattleUnit_RollCounterEarlyEnd(unit_id, state->charm, 70) != 0)
		{
			state->charm = 0;
			return 1;
		}
	}
	return 0;
}

s32 BattleUnit_TickStun(s32 unit_id)
{
	struct BtlUnit *state = UnitGetFar(unit_id);
	if (state->shibire != 0)
	{
		state->shibire--;
		if (state->shibire == 0)
			return 1;
		if (BattleUnit_RollCounterEarlyEnd(unit_id, state->shibire, 40) != 0)
		{
			state->shibire = 0;
			return 1;
		}
	}
	return 0;
}

s32 BattleUnit_TickSleep(s32 unit_id)
{
	struct BtlUnit *state = UnitGetFar(unit_id);
	if (state->nemuri != 0)
	{
		state->nemuri--;
		if (state->nemuri == 0)
			return 1;
		if (BattleUnit_RollCounterEarlyEnd(unit_id, state->nemuri, 50) != 0)
		{
			state->nemuri = 0;
			return 1;
		}
	}
	return 0;
}

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

struct DjinnRecoveryTable *Trade_GetOfferStateFar(s32 owner);

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

	list = &Trade_GetOfferStateFar(0)->list;
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

void UnitCalcFar(s32 unit_id);
#define BattlePres_SetActorModeAndAction ((void (*)(s32))BattlePres_SetActorModeAndAction)
#define BattlePresentation_ConfigurePaletteFade \
	((void (*)(s32, s32, s32))BattlePresentation_ConfigurePaletteFade)
#define BattleFx_PlayUnitElementEffect ((s32 (*)(s32, s32, s32, s32))BattleFx_PlayUnitElementEffect)

struct BattleGroupOrder
{
	s32 group[2];
};
void UiText_ShowMessageAndWaitCoreFar(s32 message_id);

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
		list = &Trade_GetOfferStateFar(side)->list;
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
						ObjectDispatch_ApplyValueToChildrenFar(BtlObjGet(id)->object, 32);
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
	list = &Trade_GetOfferStateFar((id < 0 || id > 7))->list;
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
		pending = &Trade_GetOfferStateFar((id < 0 || id > 7))->list;
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
				ObjectDispatch_ApplyValueToChildrenFar(BtlObjGet(id)->object, 32);
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

s32 BattlePres_RunMosaicFadeOut(void)
{
	volatile u16  bg0cnt;
	volatile u16  bg1cnt;
	volatile u16  bg2cnt;
	volatile u16  bg3cnt;
	s32			  i;
	volatile u16 *reg;

	bg0cnt = REG_BG0CNT;
	REG_BG0CNT = bg0cnt | 0x40;
	reg = &REG_BG0CNT;
	bg1cnt = *++reg;
	*reg = bg1cnt | 0x40;
	bg2cnt = *++reg;
	*reg = bg2cnt | 0x40;
	bg3cnt = *++reg;
	*reg = bg3cnt | 0x40;
	reg += 0x21;
	*reg = 0x3eee;

	Blend_SetDarkenTarget16(16);
	for (i = 0; i <= 15; i++)
	{
		Random16();
		Random16();
		Random16();
		Random16();
		REG_MOSAIC = (i << 8) | i;
		WaitFrames(1);
	}

	REG_DISPCNT = 1;
	WaitFrames(4);
	reg = &REG_BG0CNT;
	*reg++ = bg0cnt;
	*reg++ = bg1cnt;
	*reg++ = bg2cnt;
	*reg = bg3cnt;
	return 0;
}

//--------------------------------------------------------------------------

// build_sequential_table.c

// タイル：1 バイトずつ連番のマップを作る

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// 0〜255 と 0〜223 の連番のバイトを続けて書き、
// 残りの 544 バイトを 0xff で埋める（あわせて 1024 バイト）
//--------------------------------------------------------------------------
void Graphics_BuildSequentialTileTable(s32 *dst)
{
	s32 v;
	s32 n;
	s32 (*fill)(void *, s32, u32);

	// 4 バイトずつ書くので 1 回に 4 つ進める
	v = 0x03020100;
	n = 0;
	do
	{
		n += 1;
		*dst++ = v;
		v += 0x04040404;
	} while (n >= 0 && n <= 63);

	v = 0x03020100;
	n = 0;
	do
	{
		n += 1;
		*dst++ = v;
		v += 0x04040404;
	} while (n >= 0 && n <= 55);

	/* FAKEMATCH: the routine is called through a local pointer to its fixed entry; a plain call matches The Broken Seal through its IWRAM call layer but not The Lost Age, whose header calls the entry directly (all six of its editions differ). */
	fill = Iwram_FillWordsEntry;
	fill(dst, 544, -1);
}

#undef BattleActor_SpawnObjectsForList
#undef BattlePresentation_ConfigurePaletteFade
#undef SceneTransform_ApplyPosition

extern void *gWorkSlot[64];

extern u8 gDisp[];

void BattlePresentation_BuildTilemap(s32 *destination)
{
	s32 entry;
	u32 index;

	Iwram_FillWords(destination, 0x100, -1);
	destination += 0x40;
	Iwram_FillWords(destination, 0x80, 0x03ff03ff);
	entry = (0x0201 << 16) | 0x0200;
	destination += 0x20;
	index = 0;
	do
	{
		index++;
		*destination++ = entry;
		entry += 0x00020002;
	} while (index <= 239);
	Iwram_FillWords(destination, 0x280, 0x03ff03ff);
}

void BattlePres_UpdateHBlankScroll(void)
{
	struct BattleAffineHdma *records;
	u16						*record;
	s32						 control;

	if (((struct BattleBackgroundView *)*(
			 void **)((u8 *)gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND * sizeof(void *)))
			->mode
		== 2)
	{
		records = *(struct BattleAffineHdma **)&gWorkSlot[10];
		record = records->lines[records->page];
		control = record[0];
		/* FAKEMATCH: needed to match: the do-while keeps the BG2CNT address load after the record read */
		do
		{
			REG_BG2CNT = control;
		} while (0);
		Dma_Set(record + 1, (void *)0x0400000c, 0xa2600001, &REG_DMA0SAD);
		Dma_Set(&records->affine, (void *)0x04000020, 0x84000004, REG_DMA3);
	}
}

void BattlePresentation_UploadTileVariant(void)
{
	u32 variant = **(u32 **)gDisp - 1;
	if (variant <= 31)
		Dma_Set(BattlePres_TileVariants + (variant >> 2) * 32, (void *)0x06005000, 0x84000008,
			REG_DMA3);
}

void BattlePres_AdvanceTransitionTimer(void)
{
	s32					 scroll;
	struct BattleCamera *camera;
	u32					*timer;
	struct BgScroll		*offset;
	u32					 frame;
	u32					 next;

	timer = *(u32 **)Ram_Disp;
	frame = *timer;
	camera = *(struct BattleCamera **)Ram_CameraWork;
	scroll = 0x34 - frame;
	if (scroll > 0x20)
		scroll = 0x20;
	offset = (struct BgScroll *)Ram_BgScroll;
	if (scroll < 0)
		scroll = 0;
	offset->y = scroll;
	if (frame <= 0x50U)
		camera->yaw = 45 * frame * 8 + 0xaf80;
	next = (*timer = *timer + 1);
	if (next <= 0x50U)
	{
		BattlePres_SetupTransitionScene(0, 0, 0, 0xb4 - next);
		return;
	}
	BattlePres_SetupTransitionScene(0, 0, 0, 0x64);
}

void BattlePres_DrawTransitionRows(void)
{
	u32	 i;
	s32	 frame;
	s32	 aligned;
	s32	 tile;
	u32	 row;
	u16 *tiles;

	frame = **(s32 **)gDisp;
	if (frame >= 0 && frame <= 79)
	{
		tile = (7 & frame) + 0xf081;
		if (frame >= 0)
		{
			aligned = frame;
		}
		else
		{
			aligned = frame + 7;
		}
		row = 13 - (aligned >> 3);
		i = 0;
		tiles = (u16 *)((row << 6) + 0x06006000);
		do
		{
			i++;
			*tiles = tile;
			tiles++;
		} while (i != 32);

		tile = tile | 0x800;
		aligned = frame;
		if (frame < 0)
		{
			aligned = aligned + 7;
		}
		row = (aligned >> 3) + 13;
		if (row <= 20)
		{
			i = 0;
			tiles = (u16 *)((row << 6) + 0x06006000);
			do
			{
				i++;
				*tiles = tile;
				tiles++;
			} while (i != 32);
		}
	}
}

void Graphics_ClearBg0Vofs(void)
{
	u32 zero = 0;

	REG_BG0VOFS = zero;
}

#define BattleActor_SpawnObjectsForList ((void (*)(u16 *, s32))BattleActor_SpawnObjectsForList)
#define Graphics_BuildSequentialTileTable ((void (*)(void *))Graphics_BuildSequentialTileTable)
#define BattlePresentation_BuildTilemap ((void (*)(void *))BattlePresentation_BuildTilemap)

#define gTransitionWork ((u8(*)) & gTransitionWork)

struct DisplayRegisters
{
	u16 control;
	u16 green_swap;
};

struct TransitionActorList
{
	u8	unknown_00[20];
	s32 count;
	u8	unknown_18[12];
	u16 ids[24];
};

void BattlePres_RunEntryTransition(s32 enemy_count, s32 kind)
{
	volatile u32				 fill;
	struct TransitionActorList	 list;
	u16							 party[14];
	u16							 actors[14];
	struct BattleBackgroundView *work;
	s32							*timer;
	u16							*map;
	s32							 tile;
	u32							 i;
	u32							 j;
	s32							 count;
	s32							 id;
	struct BtlObj				*object;
	struct BtlWork				*session;
	u16							*ids;
	s32							 n;

	work = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
	timer = Runtime_AllocateHeapBlock(42, 4);
	if (kind != 0x15b)
	{
		{

			volatile u32 *dma;

			Dma_Set(BattlePres_CurtainTiles, (void *)0x06005020, 0x84000008, REG_DMA3);
			Dma_Set(BattlePres_CurtainTiles + 32, (void *)0x06005040, 0x84000008, REG_DMA3);
			Dma_Set(BattlePres_CurtainTiles + 64, (void *)0x06005060, 0x84000008, REG_DMA3);
			Dma_Set(BattlePres_CurtainTiles + 96, (void *)0x06005080, 0x84000008, REG_DMA3);
			Dma_Set(BattlePres_CurtainTiles + 128, (void *)0x060050a0, 0x84000008, REG_DMA3);
			Dma_Set(BattlePres_CurtainTiles + 160, (void *)0x060050c0, 0x84000008, REG_DMA3);
			Dma_Set(BattlePres_CurtainTiles + 192, (void *)0x060050e0, 0x84000008, REG_DMA3);
			dma = REG_DMA3;
			/* FAKEMATCH: introducing the pointer after the transfers avoids a preserved-register copy; the volatile output orders its subtraction before the control value, differing by two instructions without volatile. 2026-10-11: the native subs r3, #212 is reload_cse_move2add on a constant address that happened to land in r3; plain pointer arithmetic is CSE-reused by the later Dma_Sets (differ 45), REG_DISPCNT = 1 takes a pool constant (63), and a constant struct-field store gets r2 from local-alloc, so no move2add (31). */
			asm volatile("" : "+l"(dma));
			((struct DisplayRegisters *)((u8 *)dma - 212))->control = 1;
		}
		work->second_mode = 1;
		work->mode = 1;
		work->busy = 0;
		fill = 0x33333333;
		Dma_Set((void *)&fill, (void *)0x06005000, 0x85000008, REG_DMA3);
		fill = 0;
		Dma_Set((void *)&fill, (void *)0x06005100, 0x85000008, REG_DMA3);
		REG_BG1CNT = 0xc04;
		REG_BG0CNT |= 2;
		work->mode = 2;
		map = (u16 *)0x06006000;
		for (i = 0; i <= 31; i++)
		{
			if (i <= 20)
				tile = 0xf080;
			else
				tile = 0xf088;
			for (j = 0; j <= 31; j++)
				*map++ = tile;
		}
		{
			s32 start = 0;

			gBgScroll[0].y = 32;
			gBgScroll[1].y = 32;
			gBgScroll[1].x = 8;
			WaitFrames(1);
			REG_WIN0H = 0xf0;
			REG_WIN0V = 0x88;
			REG_WIN1H = 0xf0;
			REG_WIN1V = 0x88;
			REG_WININ = 0x3537;
			REG_WINOUT = 0x3f21;
			QueueIoWriteDelay2(0x04000000, 0x7741);
			BattlePres_SetupTransitionScene(0, 0, 0, 180);
			*timer = start;
			Scheduler_AddOrUpdateCallback(BattlePres_AdvanceTransitionTimer, 3200);
			Scheduler_AddOrUpdateCallback(BattlePres_DrawTransitionRows, 0x480);
			Runtime_SetIrqHandler(2, 32, Graphics_ClearBg0Vofs);
			gBgScroll[0].y = 32;
			WaitFrames(1);
			UiWindow_CreateWithLayoutBoundsFar(BtlWk->pty_st);
			WaitFrames(20);
			QueueIoWriteDelay10(0x04000008, 2);
			QueueIoWriteDelay6(0x04000008, 0);
			BattleIntro_AnnounceEncounter(enemy_count);
			Scheduler_RemoveCallback(BattlePres_AdvanceTransitionTimer);
			Scheduler_RemoveCallback(BattlePres_DrawTransitionRows);
			gBgScroll[0].y = start;
			Runtime_SetIrqHandler(2, 0, 0);
		}
	}
	else
	{
		session = *(gWorkSlot + HEAP_SLOT_BATTLE);
		work->second_mode = 1;
		work->busy = 0;
		count = BattleParty_ListActorIds(3, (u16 *)party);
		for (i = 0; i != count; i++)
		{
			id = i + 120;
			if ((s32)i <= 7)
				id = i;
			object = BtlObjGet(id);
			if (UnitGetFar(id)->class_id != 148)
				object->scale = 0xb333;
		}
		IO_WRITE_QUEUE_PUSH(0x04000000, 0x6041, 0x20000);
		WaitFrames(1);
		{
			s32 start = 0;

			session->background = (u32)&ResourceId_VinasuChojoStormBackdrop;
			list.count = BattleParty_ListActorIds(2, list.ids);
			list.ids[list.count] = 0xff;
			BattleActor_SpawnObjectsForList(list.ids, 0);
			BattleEffect_RunTileAndPaletteAnimationFar(&list);
			BattlePres_SetupTransitionScene(0, 0, 0, 100);
			*timer = start;
		}
		Runtime_SetIrqHandler(2, 32, Graphics_ClearBg0Vofs);
		WaitFrames(1);
		WaitFrames(20);
		UiWindow_CreateWithLayoutBoundsFar(BtlWk->pty_st);
		QueueIoWriteDelay10(0x04000008, 2);
		QueueIoWriteDelay6(0x04000008, 0);
		REG_BLDCNT = 0x3f40;
		ids = actors;
		n = BattleParty_ListActorIds(3, (u16 *)ids);
		actors[n] = 0xff;
		BattleActor_SpawnObjectsForList(ids, 0);
		n = BattleParty_ListActorIds(1, (u16 *)ids);
		for (i = 0; i != n; i++)
			BattlePres_SetActorRecordMode(ids[i], 1);
		for (i = 0; i != 16; i++)
		{
			REG_BLDALPHA = i | 0x1000;
			WaitFrames(1);
		}
		for (i = 0; i != n; i++)
			BattlePres_SetActorRecordMode(ids[i], 0);
		BattleIntro_AnnounceEncounter(enemy_count);
		gBgScroll[0].y = 0;
		WaitFrames(1);
		Runtime_SetIrqHandler(2, 0, 0);
	}
	Runtime_SetIrqHandler(2, 0, 0);
	REG_BG1CNT = 0x1f83;
	WaitFrames(1);
	REG_BG1CNT = 0x1f83;
	REG_BG0CNT &= 0xfffd;
	gBgScroll[1].x = 8;
	REG_DISPCNT = 0x1541;
	Runtime_ReleaseHeapBlock(42);
}

struct Half
{
	u16 v;
};

void BattlePresentation_SetPaletteLevel(s32 unused, s32 level)
{
	struct BtlWork *screen = BtlWk;
	u16			   *palette = screen->palette;
	u32				ime;
	u32			   *slot = &ime;

	/* FAKEMATCH: needed to match: the one-pass block and the loop on a zero local keep the IME save through slot's register */
	do
	{
		u16 *ime_reg = (u16 *)0x04000208;
		s32	 once = 0;

		do
		{
			*slot = *ime_reg;
			*ime_reg = (u32)ime_reg;
		} while (once);
	} while (0);

	if (level == 0)
	{
		Dma_Set(palette, (void *)0x050000c0, 0x80000080, REG_DMA3);
	}
	else
	{
		Graphics_ScaleRgb555Clamped(
			palette, (u16 *)0x050000c0, screen->brightness = 0x10000 - level * 1092, 128);
	}
	{
		u32 saved = *slot;

		REG_IME = saved;
	}
}

void BattlePresentation_ConfigurePaletteFade(s32 mode, u16 value, s32 fade)
{
	/* FAKEMATCH: needed to match: a one-halfword struct mask held across the fade loop */

	struct BattleBackgroundView *transition = *(struct BattleBackgroundView **)gTransitionWork;

	if (transition->mode == 0)
	{
		Scheduler_AddOrUpdateCallback(BattlePres_UpdateHBlankScroll, 0x4ff);
	}
	transition->mode = mode;

	if (mode == 1)
	{
		IO_WRITE_QUEUE_PUSH(0x0400000a, 0x1f83, 0x20000);
	}

	Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, REG_DMA3);
	*(u16 *)0x050000bc = *(u16 *)0x050001e8;

	if (fade == 0x80)
	{
		Dma_Set(BtlWk->palette, (void *)0x050000c0, 0x80000080, REG_DMA3);
	}
	else if (fade != 0)
	{
		u16		   *source = BtlWk->palette;
		u16		   *destination = (u16 *)0x050000c0;
		s32			i;
		struct Half mask;

		mask.v = 0x1f;
		for (i = 0; i != 128; i++)
		{
			s32 red = source[i] & 31;
			s32 green = (source[i] >> 5) & mask.v;
			s32 blue = (source[i] >> 10) & mask.v;

			if (red > fade)
			{
				red -= fade;
			}
			else
			{
				red = 0;
			}
			if (green > fade)
			{
				green -= fade;
			}
			else
			{
				green = 0;
			}
			if (blue > fade)
			{
				blue -= fade;
			}
			else
			{
				blue = 0;
			}
			destination[i] = (blue << 10) | (green << 5) | red;
		}
	}

	Graphics_BuildSequentialTileTable((void *)0x06003800);
	BattlePresentation_BuildTilemap((void *)0x0600f800);
}

#undef gTransitionWork

#undef gTransitionWork
#define gTransitionWork (*(struct BattleBackgroundView **)&gTransitionWork)

void BattlePresentation_InitializeWorkAndResetState(void)
{
	struct BattleAffineHdma		*work;
	struct BattleBackgroundView *state;
	volatile u32				 zero;
	work = Runtime_AllocateBlock(10, sizeof *work);
	state = gTransitionWork;
	zero = 0;
	Dma_Set(&zero, work, 0x850000a8, REG_DMA3);
	state->mode = 0;
}

void Runtime_ReleaseHeapBlock10(void)
{
	Runtime_ReleaseHeapBlock(10);
}

typedef void (*BitDecoder)(const u8 *source, void *destination);

void BattleBackground_Load(s32 mode, s32 resource, s32 level)
{
	struct BattleBackgroundView *view = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
	u8							*data = Resource_GetTableEntry(resource);
	struct BtlWork				*session = *(gWorkSlot + HEAP_SLOT_BATTLE);
	u16							*palette;

	RamCode_LoadSlot(49, BitDecoder_DecodeImage, BitDecoder_Size);
	{
		u32	  offset = 0x100;
		void *vram;

		/* FAKEMATCH: conditional cancelling updates end CSE before the
		 * decoder arguments, keeping the VRAM load before the data add.
		 * They disappear from all 103 instructions in every edition;
		 * a reassignment or XOR pair instead differs by four rows. */
		if (session)
		{
			offset++;
			offset--;
		}
		vram = (void *)0x06008000;
		((BitDecoder) * (gWorkSlot + 49))(data + offset, vram);
	}
	Runtime_ReleaseHeapBlock(49);
	palette = session->palette;
	Dma_Set(data, palette, 0x84000040, REG_DMA3);
	if (level >= 0)
		Graphics_ScaleRgb555Clamped(
			palette, (u16 *)0x050000c0, session->brightness = 0x10000 - level * 1092, 128);
	Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, REG_DMA3);
	*(u16 *)0x050000bc = *(u16 *)0x050001e8;
	Graphics_BuildSequentialTileTable((void *)0x06003800);
	BattlePresentation_BuildTilemap((void *)0x0600f800);
	Iwram_ClearWords((void *)0x0600ffc0, 64);
	if (view->mode == 0)
		Scheduler_AddOrUpdateCallback(BattlePres_UpdateHBlankScroll, 0x4ff);
	view->mode = mode;
	if (mode == 1)
		REG_BG1CNT = 0x1f83;
}
#define SceneTransform_ApplyPosition ((void (*)(s32 *))SceneTransform_ApplyPosition)

void BattleCamera_SetRange(s32 cx, s32 cy, s32 ox, s32 oy, s32 scale)
{
	struct BattleBackgroundView *view = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
	struct BattleAffineHdma		*work = *(gWorkSlot + 10);
	struct BattleCamera			*camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
	u16							 wrap;
	s32							 horizon;
	u16							*line;
	struct BattleAffine			*affine;
	s32							 ratio;
	s32							 step;
	s32							 x;
	s32							 y;
	u32							 i;
	u32							 count;

	horizon = 0x800;
	wrap = 0;
	if (scale >= 0x10000)
	{
		wrap = 0x2000;
		horizon = 0x6800 + -(s16)camera->yaw * 3;
	}
	if (work == NULL)
		return;
	if ((view->mode == 1 || view->second_mode == 1) && view->busy == 0)
		gBgScroll[1].x = horizon >> 8;
	if (view->mode != 2)
		return;
	line = work->lines[work->page ^ 1];
	ratio = Iwram_RatioMulQ14(scale, 0x10000);
	step = ratio >> 8;
	affine = &work->affine;
	affine->pa = step;
	affine->pb = 0;
	affine->pc = 0;
	affine->pd = step;
	x = Iwram_MulQ16(ratio, Iwram_MulQ16(cx, scale - 0x10000));
	y = Iwram_MulQ16(ratio, Iwram_MulQ16(cy, scale - 0x10000));
	affine->x = ((x + 0x7fff) >> 8) + ox + horizon;
	y = ((y + 0x7fff) >> 8) + oy - 0x1000;
	affine->y = y;
	count = (Iwram_RatioMulQ14((s16)step, 0x4000 - y) >> 16) + 1;
	i = 0;
	if (!GameFlag_TestFar(0x16b))
	{
		for (; i < 16; i++)
			*line++ = 0x3f8e;
	}
	if (count > 136)
		count = 136;
	for (; i < count; i++)
		*line++ = wrap | 0x478a;
	for (; i < 136; i++)
		*line++ = wrap | 0x478e;
	for (; i < 160; i++)
		*line++ = 0x3f8e;
	work->page ^= 1;
}

void BattlePres_SetupTransitionSceneAtDepth(s32 x, s32 depth, s32 y)
{
	s32 span = 0x01fe0000;
	// 元のバグ：mode は初期化されず、入口の r4 をそのまま読む（lsls r0, r4, #16）
	s32					 mode;
	struct BattleCamera *scene = gCameraWork;
	s32					*pos = scene->pos;
	s32					 scale = (mode << 16) / 100;
	s32					 render_bounds[3];
	s32					 measured_bounds[3];
	s32					 source_bounds[3];
	s32 (*blend)(s32, s32);
	s32 alpha;
	s32 half;
	s32 width;
	u32 result;

	pos[0] = x;
	pos[1] = depth;
	pos[2] = y;
	source_bounds[0] = 0;
	alpha = 0xc000;
	source_bounds[1] = 0;
	source_bounds[2] = 0;

	result = Iwram_RatioMulQ14(span, alpha);
	Camera_StoreSceneParameters(span, result, span * 2);
	Render_ResetTransformState();
	SceneTransform_ApplyPosition(pos);
	SceneTransform_ApplyYaw((s16)scene->yaw);
	SceneTransform_ApplyPitch((s16)scene->pitch);
	render_bounds[0] = 0;
	render_bounds[1] = 0;
	render_bounds[2] = scene->distance;
	Iwram_TransformVector(render_bounds, (s32 *)scene);
	gProjection.center_x = 120;
	gProjection.center_y = 120;
	Render_ResetTransformState();
	Graphics_PrepareTransferInIwramWork(scene->eye, pos);
	Render_ProjectPoint(source_bounds, measured_bounds);

	BattleCamera_SetRange(0x780000, 0x780000, (120 - measured_bounds[0]) << 8,
		(120 - measured_bounds[1]) << 8, scale);
	half = scale * 0xff;
	width = half * 2;
	result = Iwram_RatioMulQ14(width, alpha);
	Camera_StoreSceneParameters(width, result, half * 4);
}

#undef Camera_StoreSceneParameters
#undef SceneTransform_ApplyPosition

#undef Camera_StoreSceneParameters
#define Camera_StoreSceneParameters ((int (*)())Camera_StoreSceneParameters)
#define Render_ResetTransformState ((int (*)())Render_ResetTransformState)
#define SceneTransform_ApplyPitch ((int (*)())SceneTransform_ApplyPitch)
#undef SceneTransform_ApplyPosition
#define SceneTransform_ApplyPosition ((int (*)())SceneTransform_ApplyPosition)
#define SceneTransform_ApplyYaw ((int (*)())SceneTransform_ApplyYaw)

void BattlePres_SetupTransitionScene(s32 x, s32 depth, s32 y, s32 zoom)
{
	struct BattleCamera *scene = gCameraWork;
	s32					*pos = scene->pos;
	s32					 scale = (zoom << 16) / 100;
	s32					 render_bounds[3];
	s32					 measured_bounds[3];
	s32					 source_bounds[3];
	s32					 span = 0x01fe0000;
	s32 (*blend)(s32, s32);
	s32 alpha;
	s32 half;
	s32 width;
	u32 result;

	pos[0] = x;
	pos[1] = depth;
	pos[2] = y;
	source_bounds[0] = 0;
	alpha = 0xc000;
	source_bounds[1] = 0;
	source_bounds[2] = 0;

	result = Iwram_RatioMulQ14(span, alpha);
	Camera_StoreSceneParameters(span, result, span * 2);
	Render_ResetTransformState();
	SceneTransform_ApplyPosition(pos);
	SceneTransform_ApplyYaw((s16)scene->yaw);
	SceneTransform_ApplyPitch((s16)scene->pitch);
	render_bounds[0] = 0;
	render_bounds[1] = 0;
	render_bounds[2] = span;
	Iwram_TransformVector(render_bounds, (s32 *)scene);
	gProjection.center_x = 120;
	gProjection.center_y = 120;
	Render_ResetTransformState();
	Graphics_PrepareTransferInIwramWork(scene->eye, pos);
	Render_ProjectPoint(source_bounds, measured_bounds);

	BattleCamera_SetRange(0x780000, 0x780000, (120 - measured_bounds[0]) << 8,
		(120 - measured_bounds[1]) << 8, scale);
	half = scale * 0xff;
	width = half * 2;
	result = Iwram_RatioMulQ14(width, alpha);
	Camera_StoreSceneParameters(width, result, half * 4);
}

#undef Camera_StoreSceneParameters
#undef Render_ResetTransformState
#undef SceneTransform_ApplyPitch
#undef SceneTransform_ApplyPosition
#undef SceneTransform_ApplyYaw

//--------------------------------------------------------------------------

// setup_transition_at_pair_midpoint.c

// バトル表示：ふたりの中間へのカメラ合わせ

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// ふたつのオブジェクトの中間（x, z）にカメラを合わせる
// a, b : オブジェクト番号
// zoom : 拡大率（%）
//--------------------------------------------------------------------------
void BattlePres_SetupTransitionAtPairMidpoint(s32 a, s32 b, s32 zoom)
{
	struct MotionObject *obj_a = BtlObjGet(a)->object;
	struct MotionObject *obj_b = BtlObjGet(b)->object;
	s32					 ax = obj_a->x;
	s32					 bx = obj_b->x;
	s32					 az = obj_a->z;
	s32					 bz = obj_b->z;
	s32					 x = (bx + ax) / 2;
	s32					 z = (bz + az) / 2;

	BattlePres_SetupTransitionScene(x, 0, z, zoom);
}

void Graphics_FadeAlphaOut(void)
{
    s32 alpha_step;

    REG_BLDCNT = 0x2044;
    for (alpha_step = 1; alpha_step <= 16; alpha_step += 2)
    {
        REG_BLDALPHA = 0x1010 - alpha_step;
        WaitFrames(1);
    }
}

void Graphics_FadeAlphaIn(void)
{
    s32 step;

    REG_BLDCNT = 0x2044;
    for (step = 1; step <= 16; step += 2) {
        REG_BLDALPHA = step + 0x1000;
        WaitFrames(1);
    }
}

struct BattleTransitionEntry
{
	u32 value;
	u32 sum;
	u32 field8;
	u32 fieldc;
};

void Graphics_SetBlendControl(void)
{
	REG_BLDCNT = 0xbf;
}

void BattlePresentation_InitializeTransitionEntries(struct BattleTransitionEntry *entries)
{
	u32 previous = entries[0].value;

	CAMELOT_ASM("the fixed-register identity store of transform.h")
	Transform_SetIdentity(entries);
	entries[0].sum = previous + entries[0].value;
}

s32 BattlePres_DivideBy16(s32 value)
{
	return value / 16;
}

void WaitFrames(s32);

#if EDITION_INTERNATIONAL
#define DEBUG_SELECT_VALUE 0x18f
#else
#define DEBUG_SELECT_VALUE 480
#endif

s32 Runtime_AdjustDebugValueWithButtons(s32 ret)
{
	struct BtlWork *battle;
	volatile u32   *keys;

	if (gKeysHeld & KEY_START)
	{
		keys = &gKeysRepeat;
	loop:
		battle = BtlWk;
		if (*keys & 0x20)
			battle->debug_value -= 1;
		if (*keys & 0x10)
			battle->debug_value += 1;
		if (*keys & 0x40)
			battle->debug_value -= 100;
		if (*keys & 0x80)
			battle->debug_value += 100;
		if (gKeyState & KEY_A)
		{
			ret = battle->debug_value;
			goto done;
		}
		WaitFrames(1);
		goto loop;
	}
done:
	if (gKeysHeld & KEY_SELECT)
		ret = DEBUG_SELECT_VALUE;
	return ret;
}

void BattlePres_SetActorRecordMode(s32 actor_id, s32 mode)
{
	struct BtlObj			*slot = BtlObjGet(actor_id);
	struct MotionObject		*object;
	struct AnimationObject	*record;
	struct AnimationObject **records;
	s32						 i;

	if (slot == 0)
		return;
	object = slot->object;
	if (object == 0)
		return;
	switch (object->record_storage_kind & 15)
	{
	case 1:
	{
		record = object->records;
		record->part[0].object_mode = mode;
		record->part[1].object_mode = mode;
		break;
	}
	case 2:
	{
		records = (struct AnimationObject **)object->records;
		i = 0;
		do
		{
			record = *records++;
			if (record == 0)
				break;
			record->part[0].object_mode = mode;
			record->part[1].object_mode = mode;
			i++;
		} while (i <= 3);
		break;
	}
	}
}

//--------------------------------------------------------------------------

// present.c

// バトル表示：ユニットの絵の半透明の切りかえ

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// except 以外の全ユニットの絵を半透明（OBJ モード 1）にする
// except : そのままにするユニット番号
//--------------------------------------------------------------------------
void BattlePres_SetOtherActorRecordModes(s32 except)
{
	s16 list[14];
	s32 cnt = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
	s32 i;

	for (i = 0; i < cnt; i++)
	{
		if (list[i] != except)
			BattlePres_SetActorRecordMode(list[i], 1);
	}
}

//--------------------------------------------------------------------------
// 全ユニットの絵をふつう（OBJ モード 0）にもどす
//--------------------------------------------------------------------------
void BattlePres_ClearAllActorRecordModes(void)
{
	s16 list[14];
	s32 cnt = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
	s32 i;

	for (i = 0; i < cnt; i++)
		BattlePres_SetActorRecordMode(list[i], 0);
}

#undef BattleFx_PlayUnitElementEffect
#undef gTransitionWork

void QueueIoWriteDelay2(u32, u32);

struct Spark
{
	s32 pos[6];
	s32 life;
};

struct SparkWork
{
	u8			 sheet[0x11c0];
	struct Spark sparks[16];
	struct Ring	 rings[3];
	s32			 frames;
	s32			 ready;
	s32			 origin_x;
	s32			 origin_y;
	s32			 origin_pending;
};

typedef void (*DrawFunc)(void *dest, u8 *src, s32 x, s32 y, s32 width, s32 height);

#define ColorBuffer_BackupAndScaleThreeQuarters \
	((u32 (*)(void *, void *, u32))ColorBuffer_BackupAndScaleThreeQuarters)

void Graphics_AdvancePaletteCycle(void)
{
	struct BtlWork *battle = BtlWk;
	s8			   *table;
	u16				step;
	s32				next;

	if (battle != NULL && battle->highlight_mode != 0)
	{
		REG_BLDCNT = 0x3f90;
		REG_BLDALPHA = 0x10;
		table = BattleFx_HighlightBlendRamp;
		REG_BLDY = table[battle->highlight_step];
		step = battle->highlight_step;
		next = (step + 1) & 0xf;
		if (step > 14)
			next |= 0x10;
		battle->highlight_step = next;
	}
}

void BattlePres_SetActorModes(u16 *actors, s32 mode)
{
	s16				active_actors[14];
	struct BtlWork *battle = BtlWk;
	u32				count;
	u32				i;
	volatile u16   *blend_y;

	if (mode == 0)
	{
		Scheduler_RemoveCallback(Graphics_AdvancePaletteCycle);
		REG_BLDY = mode;
		BattlePres_ClearAllActorRecordModes();
		WaitFrames(1);
		QueueIoWriteDelay2(0x04000050, 0);
	}
	if (battle != 0 && mode != 0)
	{
		u32 zero = 0;
		u32 sixteen;

		battle->highlight_mode = mode;
		battle->highlight_step = zero;
		blend_y = &REG_BLDY;
		*blend_y = zero;
		sixteen = 16;

		/* FAKEMATCH: needed to match: removing this one-pass block changes instruction scheduling */
		do
		{
			blend_y[-1] = sixteen;
		} while (0);

		count = BattleParty_ListActorIds(3, (u16 *)active_actors);
		for (i = 0; i < count; i++)
			BattlePres_SetActorRecordMode(active_actors[i], mode & 1);

		if (actors != 0)
		{
			u32 actor = *actors;
			i = 0;
			actors++;
			if (actor != 0xff)
			{
				do
				{
					BattlePres_SetActorRecordMode(actor, (mode & 1) ^ 1);
					i++;
					if (i > 13)
						break;
					actor = *actors++;
				} while (actor != 0xff);
			}
		}
		WaitFrames(1);
		QueueIoWriteDelay2(0x04000050, 0);
		Scheduler_AddOrUpdateCallback(Graphics_AdvancePaletteCycle, 0x480);
	}
}

s32 BattleFx_UpdateStarField(void)
{
	// 戻り値の型は s32 だが return 文は無い（pop {r1}; bx r1）
	s32				  j;
	s32				  frame;
	u32				  size;
	s32				  ring_x;
	s32				  ring_y;
	s32				  phase;
	s32				  age;
	void			 *dest;
	s32				  i;
	struct SparkWork *work;
	DrawFunc		  draw;
	struct Spark	 *spark;
	struct Ring		 *ring;
	s32				 *pos;
	s32				  k;
	s32				  dist;
	s32				  scale;
	s32				  value;
	s32				  step;
	u32				  radius;
	s32				  angle;
	u32				  half;
	s32				  x;
	s32				  y;
	s32				  life;

	dest = gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
	work = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
	work->ready = 0;
	draw = (DrawFunc)gWorkSlot[HEAP_SLOT_BLITTER];
	for (i = 0; i < 16; i++)
	{
		spark = &work->sparks[i];
		life = spark->life;
		if (life != 0)
		{
			dist = FixedSqrt((spark->pos[0] >> 8) * (spark->pos[0] >> 8)
							 + (spark->pos[1] >> 8) * (spark->pos[1] >> 8)
							 + (spark->pos[2] >> 8) * (spark->pos[2] >> 8));
			if (dist <= 0xfff)
			{
				spark->life = 0;
			}
			else
			{
				scale = Iwram_RatioMulQ14(dist, 0x10000);
				spark->life--;
				pos = spark->pos;
				for (k = 2; k >= 0; k--)
				{
					value = *pos;
					step = Iwram_MulQ16(Iwram_MulQ16(-value >> 8, scale), 0x13000);
					pos[3] = pos[3] - (pos[3] >> 7) + step;
					*pos = value + pos[3];
					pos++;
				}
			}
			life = spark->life;
			if (life != 0)
				goto draw_spark;
		}
		if (work->frames <= 24)
		{
			angle = Random16();
			radius = Random16() + 0x10000;
			half = radius >> 1;
			spark->pos[0] = Iwram_MulQ16(Trig_Cos(angle), half);
			spark->pos[1] = Iwram_MulQ16(Trig_Sin(angle), half);
			if (spark->pos[0] & 1)
				spark->pos[0] = -spark->pos[0];
			if (spark->pos[1] & 1)
				spark->pos[1] = -spark->pos[1];
			spark->pos[2] = (Random16() + 0x8000) >> 2;
			spark->pos[3] = (-spark->pos[0] >> 7) + (spark->pos[1] >> 8);
			spark->pos[4] = (-spark->pos[1] >> 7) + (-spark->pos[0] >> 8);
			spark->pos[5] = 0;
			life = spark->life = (radius >> 13) + 1;
		}
		if (life != 0)
		{
		draw_spark:
			x = (spark->pos[0] >> 10) + 64;
			y = (spark->pos[1] >> 10) + 64;
			frame = life;
			if (frame < 0)
				frame = 0;
			else if (frame > 6)
				frame = 6;
			size = BattleFx_StarSizes[frame];
			draw(dest, (u8 *)work + BattleFx_StarWorkOffsets[frame], x - (size >> 1),
				y - (size >> 1), size, size);
		}
	}
	draw = (DrawFunc)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	for (ring = work->rings, j = 2; j >= 0; j--, ring++)
	{
		ring->x += ring->vel_x;
		ring->y += ring->vel_y;
		ring_x = ring->x >> 10;
		ring_y = ring->y >> 10;
		phase = 3 - ring->age / 8;
		if (phase >= 0)
		{
			ring->age++;
			draw(dest, (u8 *)work + BattleFx_RingFrameOffsets[phase], ring_x + 48, ring_y + 48, 32,
				32);
		}
	}
	work->frames++;
	work->ready = 1;
}

u32 Graphics_UploadVramBlock(void)
{
	void *source = gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
	u8	 *context = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];

	if (source != 0)
	{
		u32 *active = (u32 *)(context + 0x13C0);

		if (*active != 0)
		{
			*active = 0;
			return ColorBuffer_BackupAndScaleThreeQuarters(source, (void *)0x06004000, 0x4000);
		}
	}
	return (u32)source;
}

s32 BattleEffect_LoadWorkFar(s32 slot, s32 width_shift, s32 height_shift, s32 flags, u32 mode);

#define BattleEffect_LoadWorkFar ((s32 (*)(s32, s32, s32, s32, s32))BattleEffect_LoadWorkFar)
#define BattleFx_RunIceFleckBurstFar ((void (*)(struct EffectLaunch *))BattleFx_RunIceFleckBurstFar)

#define gTransitionWork (*(s32 **)&gTransitionWork)

s32 BattleFx_InitializeStarField(s32 mode)
{
	struct SparkWork *work;
	struct Spark	 *spark;
	struct Ring		 *ring;
	s32				  i;
	u8				 *data;
	s32				  scale;
	s32				  zero;
	/* FAKEMATCH: needed to match: an unused Spark, the 28 bytes the native frame reserves. */
	struct Spark unused;

	gTransitionWork[2] = 1;
	work = Runtime_AllocateBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
	/* FAKEMATCH: needed to match: the empty loop ends a scheduling region, keeping the multiply routine's address load ahead of the spark cursor. */
	do
	{
	} while (0);
	for (i = 0; i < 16; i++)
	{
		s32 angle = Random16();
		u32 radius = Random16() + 0x10000;
		u32 half = radius >> 1;

		spark = &work->sparks[i];
		spark->pos[0] = Iwram_MulQ16(Trig_Cos(angle), half);
		spark->pos[1] = Iwram_MulQ16(Trig_Sin(angle), half);
		if (spark->pos[0] & 1)
			spark->pos[0] = -spark->pos[0];
		if (spark->pos[1] & 1)
			spark->pos[1] = -spark->pos[1];
		spark->pos[2] = (Random16() + 0x8000) >> 2;
		spark->pos[3] = (-spark->pos[0] >> 7) + (spark->pos[1] >> 8);
		spark->pos[4] = (-spark->pos[1] >> 7) + (-spark->pos[0] >> 8);
		spark->pos[5] = 0;
		spark->life = (radius >> 13) + 1;
	}
	for (i = 0; i < 3; i++)
	{
		s32 radius = 0x1000;
		s32 angle;

		ring = &work->rings[i];
		angle = i * 0x5555;
		ring->x = Iwram_MulQ16(Trig_Cos(angle), radius);
		ring->y = Iwram_MulQ16(Trig_Sin(angle), radius);
		ring->vel_x = Iwram_MulQ16(Trig_Cos(angle), 0x200);
		ring->vel_y = Iwram_MulQ16(Trig_Sin(angle), 0x200);
		ring->age = 0;
	}
	work->frames = 0;
	work->ready = 0;
	work->origin_pending = 0;
	Iwram_ClearWords(gWorkSlot[HEAP_SLOT_BATTLE_CANVAS], 0x4000);
	data = Resource_GetTableEntry((s32)&ResourceId_BlueRingSheet);
	Iwram_CopyWords(BG_PLTT, data, 128);
	data += 128;
	Resource_DecodeType01(data, work);
	switch (mode)
	{
	case 0:
		data = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteB);
		break;
	case 1:
		data = Resource_GetTableEntry((s32)&ResourceId_BlueRingSheet);
		break;
	case 2:
		data = Resource_GetTableEntry((s32)&ResourceId_RedPaletteC);
		break;
	default:
		data = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteD);
		break;
	}
	Dma_Set(data, BG_PLTT, DMA_ENABLE32 | DMA_32BIT | 32, REG_DMA3);
	/* FAKEMATCH: needed to match: the two one-pass blocks keep the BG2 origin and matrix writes in source order, the zero loaded first. */
	do
	{
		zero = 0;
		REG_BG2X = zero;
		REG_BG2Y = zero;
	} while (0);
	do
	{
		scale = 0x100;
		REG_BG2PA = scale;
		REG_BG2PB = zero;
		REG_BG2PC = zero;
		REG_BG2PD = scale;
	} while (0);
	BattleEffect_LoadWorkFar(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
	BattleEffect_LoadWorkFar(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
	Scheduler_AddOrUpdateCallback((SchedulerCallback)BattleFx_UpdateStarField, 0xc80);
	return Scheduler_AddOrUpdateCallback((SchedulerCallback)Graphics_UploadVramBlock, 0xc80);
}

s32 Graphics_ResetVramBlockAndReleaseHeapBlocks(s32 unused)
{
	Iwram_ClearWords((void *)0x06004000, 0x4000);
	Runtime_ReleaseHeapBlock(47);
	Runtime_ReleaseHeapBlock(46);
	Runtime_ReleaseHeapBlock(40);
	Runtime_ReleaseHeapBlock(39);
	REG_DISPCNT = 0x1341;
	Scheduler_RemoveCallback((SchedulerCallback)Graphics_UploadVramBlock);
	return Scheduler_RemoveCallback((SchedulerCallback)BattleFx_UpdateStarField);
}

s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale, s32 count)
{
	s32 remaining;
	u32 mask_red;
	u32 mask_green;
	u32 mask_blue;
	u32 pixel;
	u32 red;
	u32 green;
	u32 blue;

	if (scale > 0x10000)
		scale = 0x10000;
	if (count > 0)
	{
		mask_red = 0x1f;
		mask_green = 0x3e0;
		mask_blue = 0x7c00;
		remaining = count;
		do
		{
			pixel = *source;
			red = pixel & mask_red;
			green = pixel & mask_green;
			blue = mask_blue & pixel;
			red *= scale;
			green *= scale;
			blue *= scale;
			pixel = ((red >> 16) & mask_red) | ((green >> 16) & mask_green);
			pixel |= (blue >> 16) & mask_blue;
			*destination = pixel;
			source++;
			destination++;
			remaining--;
		} while (remaining != 0);
	}
	return 0;
}

struct Bg2OriginWork
{
	u8	unknown_0000[0x13c4];
	s32 origin_x;
	s32 origin_y;
	s32 origin_pending;
};

struct EffectLaunch
{
	s32 kind;
	s32 side;
	s32 actor;
	s32 target;
	s32 unknown_0010;
	s32 count;
	s32 variant;
	s32 prelude;
	s32 unknown_0020;
	s16 actors[24];
};

void BattleFx_PlayUnitElementEffect(s32 unit, s32 kind, s32 mode, s32 variant)
{
	/* FAKEMATCH: needed to match: an unread ten-word launch tail, the native frame's 40 extra bytes. */
	s32					  spare[10];
	struct EffectPosition position;
	struct EffectPosition position2;
	struct EffectLaunch	  launch;
	struct EffectLaunch	  launch2;
	/* FAKEMATCH: needed to match: one variable, assigned twice, holds slot 12's address and later the IME address, as the bytes do; the session is slot 9, three words below. */
	volatile u16		  *ime = (volatile u16 *)&gWorkSlot[12];
	struct BtlWork		  *session = *(struct BtlWork **)((u8 *)ime - 12);
	struct Bg2OriginWork  *work;
	s32					   frame;
	s32					  *brightness;
	struct EffectPosition *pos;

	WaitFrames(1);
	BattlePresentation_ConfigurePaletteFade(1, session->background, 0);
	Iwram_ClearWords((void *)0x06004000, 0x4000);
	QueueIoWriteDelay2(0x04000000, 0x3741);
	QueueIoWriteDelay2(0x0400000c, 0x784);
	QueueIoWriteDelay2(0x04000050, 0x3f44);
	WaitFrames(1);
	Io_Write16(240, &REG_WIN0H);
	Io_Write16(0x1088, &REG_WIN0V);
	Io_Write16(63, &REG_WININ);
	Io_Write16(17, &REG_WINOUT);
	if (mode == 0)
	{
		QueueIoWriteDelay2(0x04000052, 0x100e);
		BattleFx_InitializeStarField(kind);
		for (frame = 0, pos = &position; frame <= 44; frame++)
		{
			brightness = &session->brightness;
			ime = &REG_IME;
			work = gWorkSlot[39];
			if (frame <= 24)
				Graphics_ScaleRgb555Clamped(
					session->palette, (u16 *)0x050000c0, *brightness = 0x10000 - frame * 1092, 128);
			BattleMotion_ProjectScaledPosition(unit, (s32 *)pos);
			work->origin_x = (64 - pos->x) << 8;
			work->origin_y = (64 - pos->y) << 8;
			{
				/* FAKEMATCH: needed to match: IO_WRITE_QUEUE_PUSH written out with the IME address in the function's own pointer, set at the top of the loop; the one-pass blocks keep the saved copy ahead of the IME store and the restore ahead of the next statement. With the macro the IME address and pos trade r6 and r7 (differ 20). */
				struct IoWriteQueue *q;
				u32					 saved;
				s32					 count;

				q = &gIoWriteQueue;
				do
				{
					saved = *ime;
				} while (0);
				*ime = (u16)(u32)ime;
				count = q->count;
				if (count <= 31)
				{
					u32 *entry = (u32 *)((u8 *)q + count * 12 + 4);

					*(u16 *)&q->count = count + 1;
					*entry++ = (u32)&work->origin_x;
					*entry++ = 0x04000028;
					*entry = 0x84000002;
				}
				do
				{
					*ime = saved;
				} while (0);
			}
			work->origin_pending = 1;
			WaitFrames(1);
		}
		Graphics_ResetVramBlockAndReleaseHeapBlocks(kind);
	}
	else if (mode == 1)
	{
		BattlePresentation_PrepareSceneFar(kind);
		for (frame = 0; frame < 40; frame++)
		{
			work = gWorkSlot[39];
			BattleMotion_ProjectScaledPosition(unit, (s32 *)&position2);
			work->origin_x = (64 - position2.x) << 8;
			work->origin_y = (64 - position2.y) << 8;
			IO_WRITE_QUEUE_PUSH(0x04000028, (u32)&work->origin_x, 0x84000002);
			work->origin_pending = 1;
			WaitFrames(1);
		}
		BattleFx_ScheduleCallbacksAndReleaseBlocksFar();
	}
	else if (mode == 2)
	{
		launch.prelude = 0;
		launch.kind = kind;
		launch.variant = variant;
		launch.actor = unit;
		launch.actors[0] = unit;
		launch.target = unit;
		launch.count = 1;
		launch.unknown_0010 = 1;
		BattleFx_RunIceFleckBurstFar(&launch);
	}
	else
	{
		launch2.kind = kind;
		launch2.prelude = 0;
		launch2.variant = 0;
		launch2.actor = unit;
		launch2.actors[0] = unit;
		launch2.target = unit;
		launch2.count = 1;
		launch2.unknown_0010 = 1;
		BattleFx_RunGatheringBurstFar(&launch2);
	}
}
void BattlePresentation_SetPaletteLevel(s32 unused, s32 level);

void BattlePres_RunWithZeroArguments(void)
{
	BattlePresentation_SetPaletteLevel(0, 0);
}

typedef void (*InterruptHandler)(void);
void Runtime_SetIrqHandler(u32, s32, InterruptHandler);

void Runtime_RemoveIrqHandlerSlot2(void)
{
	Runtime_SetIrqHandler(2, 0, 0);
}

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
	struct FormationCandidate *pool =
		(struct FormationCandidate *)Runtime_BumpAllocateAlternatePool(128);
	u16 party[8];
	s32 level_total = 0;
	s32 unit_count = BattleParty_PrepareActiveOwners(party);
	s32 i;
	s32 j;
	s32 chance;
	s32 result;

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

	base =
		(struct BattleBaseStats *)Runtime_BumpAllocateAlternatePool(sizeof(struct BattleBaseStats));
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

u32 Summon_GetLayoutKind(s32 index)
{
	u32 kind = Summon_EntryTable[index].layout_kind;

	if ((s32)kind > 4)
		kind = -1U;
	return kind;
}

s32 Summon_GetEntryValue(s32 index)
{
	if (index < 0 || index > 171)
		return Summon_EntryTable[0].animation;
	return Summon_EntryTable[index].animation;
}

s32 Summon_GetEntryFlag1Field(s32 index)
{
	if (index < 0 || index > 171)
		return Summon_EntryTable[0].animation;
	return Summon_EntryTable[index].layout_bits1;
}

s32 Summon_IsEntryFlagged(s32 index)
{
	s32 result;

	if (index < 0 || index > 171)
		return 0;
	result = 0;
	if (Summon_EntryTable[index].sprite_bit0)
		result = 1;
	return result;
}

u32 Battle_GetSpriteFlagNibble(u32 index)
{
	u16 value;

	if (index > 171)
		return 1;
	value = Summon_EntryTable[index].sprite_bits1;
	return value ? value : 1;
}

u32 Battle_GetSpriteFlagHighBits(u32 index)
{
	u16 value;

	if (index > 171)
		return 0;
	value = Summon_EntryTable[index].sprite_bits5;
	return value ? value : 0;
}

s32 Summon_IsEntrySecondaryFlagged(s32 index)
{
	if (index < 0 || index > 171)
		return 0;
	return Summon_EntryTable[index].layout_bit0;
}

s32 Summon_GetEntryByte4(s32 index)
{
	if (index < 0 || index > 171)
		return 0;
	return Summon_EntryTable[index].unknown_04[0];
}

u32 Item_EncodeBankedId(u32 value)
{
	u32 bank = 0;
	u32 base = value & 0x1ff;
	if (base == 0)
		return 0;
	{
		u8 flags = Item_GetDirectFar(base)->flags;

		if (flags & 8)
			bank = 1;
		bank <<= 1;
		if (flags & 4)
			bank++;
		bank <<= 9;
		bank += base;
	}
	return bank;
}

void Summon_ClearWorkFields(void)
{
	struct BtlWork		*work = BtlWk;
	struct BattleSpoils *spoils = &work->spoils;
	u16					*items;
	s32					 index;

	gGameState.pending_item = 0;
	spoils->coins = 0;
	spoils->experience = 0;
	spoils->defeated = 0;
	items = work->spoils.items;
	for (index = 3; index >= 0; index--)
		items[index] = 0;
}

#define Item_EncodeBankedId ((s32 (*)(s32))Item_EncodeBankedId)

static __inline__ s32 BattleEnemy_CalculateCoinReward(struct BtlUnit *unit, u16 *reward)
{
	s32 i;
	s32 bonus = 0;
	s32 base;
	s32 floor;

	for (i = 0; i < (u8)(unit->level / 10U) + 1; i++)
		bonus += ((Random16() * 6) >> 16) + 1;
	base = *reward;
	floor = base * 3 / 10;
	if (bonus < floor)
		bonus = floor;
	return bonus + base;
}

static __inline__ s32 BattleEnemy_CalculateExperienceReward(struct BtlUnit *unit, u16 *reward)
{
	s32 i;
	s32 bonus = 0;
	s32 base;
	s32 floor;

	for (i = 0; i < (u8)(unit->level / 10U) + 1; i++)
		bonus += ((Random16() * 4) >> 16) + 1;
	base = *reward;
	floor = base * 3 / 10;
	if (bonus < floor)
		bonus = floor;
	return bonus + base;
}

s32 BattleEnemy_RecordDefeat(s32 unit_id, s32 earned)
{
	struct BtlUnit		   *unit;
	struct EnemyDefinition *rec;
	struct BtlWork		   *formation;
	struct BattleSpoils	   *spoils;
	s32						i;
	s32						chance;
	s32						slot;
	s32						lowest;
	s32						lowest_slot;
	s32						value;

	unit = UnitGetFar(unit_id);
	formation = BtlWk;
	spoils = &formation->spoils;
	if (unit_id >= 0 && unit_id < 8)
		return -1;
	if (unit->class_index != 0)
		return -2;
	slot = 0;
	i = 0;
	if (formation->enemy_classes[0] != unit->class_id)
	{
		do
		{
			if (++i > 5)
				break;
		} while (formation->enemy_classes[i] != unit->class_id);
	}
	if (i != 6)
		slot = i;
	if (formation->defeat_state != 2)
	{
		if (slot < formation->first_defeated)
			formation->first_defeated = slot;
		if (spoils->defeated)
			formation->defeat_state = 1;
	}
	spoils->defeated++;
	if (GameFlag_TestFar(0x173))
		return 0;
	GameFlag_SetBitFar(unit->class_id + 0x600);
	rec = Owner_GetRecordFar(unit->class_id);
	if (earned != 0)
	{
		if (rec->coins != 0)
			spoils->coins += BattleEnemy_CalculateCoinReward(unit, &rec->coins);
		if (rec->experience != 0)
			spoils->experience += BattleEnemy_CalculateExperienceReward(unit, &rec->experience);
	}
	else
	{
		spoils->coins += rec->coins;
		spoils->experience += rec->experience;
	}
	if (rec->item == 0 || rec->item_chance == 0)
		return 0;
	i = 0;
	if (spoils->items[0] != rec->item)
	{
		do
		{
			if (++i > 3)
				break;
		} while (spoils->items[i] != rec->item);
	}
	if (i != 4)
		return 0;
	chance = rec->item_chance;
	if (earned != 0)
		chance -= 2;
	if (chance < 0)
		chance = 0;
	if ((0x20000 >> chance) > (BtlRndFar() & 0xffff))
	{
		lowest_slot = -1;
		lowest = 0x40000000;
		for (i = 0; i <= 3; i++)
		{
			value = Item_EncodeBankedId(spoils->items[i]);
			if (value < lowest)
			{
				lowest = value;
				lowest_slot = i;
			}
		}
		if (Item_EncodeBankedId(rec->item) > lowest)
			spoils->items[lowest_slot] = rec->item;
	}
	return 0;
}

void Battle_AwardSpoils(void)
{
	struct BattleSpoils *spoils;
	s32					 count;
	u16					*list;
	s32					 i;
	s32					 unit_id;
	struct BtlUnit		*unit;
	struct BtlUnit		*backup;
	s32					 cnt;
	u32					 learned;
	s32					 j;
	s32					 best;
	s32					 best_slot;
	s32					 slot;
	u16					*found;
	s32					 value;
	s16					 gains[8];
	u16					 units[8];

	spoils = &BtlWk->spoils;
	if (spoils->experience != 0)
	{
		UiWork_PushValueSlotFar(spoils->experience, 5);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgExpGained);
		BattlePresentation_WaitForAdvance();
	}
	list = units;
	count = BattleParty_ListLivingUnits(1, list);
	backup = (struct BtlUnit *)Runtime_BumpAllocateAlternatePool(sizeof(struct BtlUnit));
	for (i = 0; i < count; i++)
	{
		unit_id = list[i];
		unit = (struct BtlUnit *)UnitGetFar(unit_id);
		unit->experience += spoils->experience;
		while (Iwram_CopyWords(backup, unit, sizeof(struct BtlUnit)),
			Owner_GetValueIfLevelThresholdReachedFar(unit_id, gains) != 0)
		{
			AudioCommand_PlayFar(0x59);
			UiWork_ClearValueNameTablesFar();
			UiWork_PushValueSlotFar(unit->class_index, 3);
			UiWork_PushValueSlotFar(list[i], 1);
			UiWork_PushValueSlotFar(unit->level, 5);
			UiText_ShowMessageAndWaitCoreFar((s32)MsgLevelUp);
			BattlePresentation_WaitForAdvance();
			for (cnt = 0; cnt < 32; cnt++)
			{
				learned = unit->action_slots[cnt].encoded_action;
				if ((learned & OWNER_ACTION_ID_MASK) && (learned >> 15))
				{
					for (j = 0; j < 32; j++)
					{
						if (learned == backup->action_slots[j].encoded_action)
							break;
					}
					if (j == 32)
					{
						UiWork_ClearValueNameTablesFar();
						UiWork_PushValueSlotFar(unit->class_index, 3);
						UiWork_PushValueSlotFar(unit_id, 1);
						UiWork_PushValueSlotFar(learned & OWNER_ACTION_ID_MASK, 4);
						AudioCommand_PlayFar(0x9a);
						UiText_ShowMessageAndWaitCoreFar((s32)MsgAbilityMastered);
						BattlePresentation_WaitForAdvance();
					}
				}
			}
			if (gains[2] != 0)
			{
				UiWork_PushValueSlotFar(gains[2], 5);
				UiText_ShowMessageAndWaitCoreFar((s32)MsgMaxHpRises);
				BattlePresentation_WaitForAdvance();
			}
			if (gains[3] != 0)
			{
				UiWork_PushValueSlotFar(gains[3], 5);
				UiText_ShowMessageAndWaitCoreFar((s32)MsgMaxPpRises);
				BattlePresentation_WaitForAdvance();
			}
			if (gains[4] != 0)
			{
				UiWork_PushValueSlotFar(gains[4], 5);
				UiText_ShowMessageAndWaitCoreFar((s32)MsgAttackRises);
				BattlePresentation_WaitForAdvance();
			}
			if (gains[5] != 0)
			{
				UiWork_PushValueSlotFar(gains[5], 5);
				UiText_ShowMessageAndWaitCoreFar((s32)MsgDefenseRises);
				BattlePresentation_WaitForAdvance();
			}
			if (gains[6] != 0)
			{
				UiWork_PushValueSlotFar(gains[6], 5);
				UiText_ShowMessageAndWaitCoreFar((s32)MsgAgilityRises);
				BattlePresentation_WaitForAdvance();
			}
			if (gains[7] != 0)
			{
				UiWork_PushValueSlotFar(gains[7], 5);
				UiText_ShowMessageAndWaitCoreFar((s32)MsgLuckRises);
				BattlePresentation_WaitForAdvance();
			}
		}
	}
	MemFree(backup);
	if (spoils->coins != 0)
	{
		UiWork_PushValueSlotFar(spoils->coins, 5);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgCoinsGained);
		Party_AdjustCoinsFar(spoils->coins);
		BattlePresentation_WaitForAdvance();
	}
	found = &gOverflowItem;
	for (;;)
	{
		best_slot = -1;
		best = -1;
		for (slot = 0; slot <= 3; slot++)
		{
			if (spoils->items[slot] != 0)
			{
				value = Item_EncodeBankedId(spoils->items[slot]);
				if (value >= best)
				{
					best = value;
					best_slot = slot;
				}
			}
		}
		if (best_slot == -1)
			break;
		UiWork_PushValueSlotFar(spoils->items[best_slot], 2);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgItemGained);
		BattlePresentation_WaitForAdvance();
		if (PartyInventory_AddFar(spoils->items[best_slot]) == -1)
		{
			*found = spoils->items[best_slot];
			break;
		}
		spoils->items[best_slot] = 0;
	}
}

void Battle_ReservedNoOp2A08(void)
{
}
