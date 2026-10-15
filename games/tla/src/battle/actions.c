//--------------------------------------------------------------------------
// actions.c
// バトル：ターンの行動の組み立てと、並びの位置の変換
//--------------------------------------------------------------------------

#include "types.h"
#include "edition.h"
#include "battle.h"
#include "game.h"
#include "system.h"
#include "battle_escape.h"

#include "battle_work.h"
#include "battle_party.h"
#include "battle_unit.h"
#include "battle_system.h"
#include "battle_display.h"
#include "owner_state.h"

extern struct BattlePresentationTransition *gTransitionWork;

void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBitFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);

//--------------------------------------------------------------------------
// BG パレットのバンク 15 を 14 へ、5 を 4 へ、明るさを変えて写す
// offset : RGB 各成分に足す値（0 から 31 におさめる）
//--------------------------------------------------------------------------
void Palette_CopyBanksWithBrightnessOffset(s32 offset)
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
			b += offset;
			g += offset;
			r += offset;
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
	struct BattleUnit *unit;
	struct BattleWork *work;
	u8			   *mode;
	s32				cnt;
	s32				add;
	s32				i;

#if GAME_TLA
	work = Ram_HeapSlots->battle_work;
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
#endif
			}
			act++;
			i--;
		} while (i != 0);
	}

finish:
#if GAME_TLA
	GameFlag_ClearBit(363);
#endif
	Camera_InitDefaultTransform();
#if GAME_TLA
	Ram_HeapSlots->transition_work->target_yaw = 0x2000;
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
	struct BattleWork *work = Ram_HeapSlots->battle_work;
#endif

	if ((slot & 0x80) != 0)
		return work->enemy_ids[slot & 0xf];
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
	struct BattleWork *work = Ram_HeapSlots->battle_work;
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
		unit = work->enemy_ids[i];
		if (unit == BATTLE_UNIT_LIST_END)
			return -1;
		if (unit != BATTLE_UNIT_REMOVED && unit == id)
			return i | 0x180;
	}
}
