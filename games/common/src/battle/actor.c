//--------------------------------------------------------------------------
// actor.c
// バトル：倒れたユニットの絵と並びの後しまつ
//--------------------------------------------------------------------------

#include "types.h"
#include "edition.h"
#include "battle.h"
#include "system.h"
#include "btlpty.h"
#include "btlsys.h"
#include "btltype.h"
#include "btlwork.h"
#include "btlsmn.h"
#include "motobj.h"
#include "animspr.h"

struct BtlUnit *UnitGet(s32);

void Animation_ApplyChildArgumentFar(void *, s32);
s32	 AnimationObjects_SelectAnimationFar(struct AnimationObject *, s32);

//--------------------------------------------------------------------------
// ユニットを味方か敵の並びから外し、行動の予定からも消す
// GAME_TLA では、外せたら 0、どちらの並びにもいなければ -1 を返す
// id : ユニット番号
//--------------------------------------------------------------------------
#if GAME_TLA
s32 BattleActor_RemoveFromLists(s32 id)
#else
void BattleActor_RemoveFromLists(s32 id)
#endif
{
	struct BtlWork *work;
	s32				i;
#if GAME_TLA
	/* FAKEMATCH: retain r2 and integer-load/pointer-store operands; natural loops split the removal exit. */
	register s32 slot asm("r2");
#endif
	u32 j;
	s32 unit;

	// 味方の並び
#if GAME_TLA
	work = Ram_HeapSlots->battle_work;
	UnitGet(id)->side = BATTLE_UNIT_ABSENT;
	i = (u32)(&((struct BtlWork *)0)->party_units);
	slot = i;
	unit = *(s16 *)(slot + (u32)work);
	if (unit == id)
		goto party_removed;
	if (unit != BATTLE_UNIT_LIST_END)
	{
		do
		{
			i += sizeof(s16);
			slot = i;
			unit = *(s16 *)(slot + (u32)work);
			if (unit == id)
				goto party_removed;
		} while (unit != BATTLE_UNIT_LIST_END);
	}
#else
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
#endif

	// 敵の並び
	/* FAKEMATCH: needed to match: the goto loop avoids the pointer walk and saved 0xfe constant produced by a for loop */
	do
	{
		j = 0;
	again:
		unit = work->enm_no[j];
#if GAME_TLA
		if (unit == id)
			goto enemy_removed;
#else
		if (unit == id)
		{
			work->enm_no[j] = BATTLE_UNIT_REMOVED;
			goto removed;
		}
#endif
		j++;
		if (unit == BATTLE_UNIT_LIST_END)
#if GAME_TLA
			break;
#else
			return;
#endif
		goto again;
	} while (0);
#if GAME_TLA
	return -1;
enemy_removed:
	work->enm_no[j] = BATTLE_UNIT_REMOVED;
	goto removed;
party_removed:
	*(s16 *)((u8 *)work + slot) = BATTLE_UNIT_REMOVED;
#endif

removed:
	EnmCntRel(id);
	for (j = 0; j < 20; j++)
	{
		if (work->actions[j].unit_id == id)
			work->actions[j].unit_id = BATTLE_UNIT_LIST_END;
	}
#if GAME_TLA
	return 0;
#endif
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

#if GAME_TLA
	unit = UnitGet(id);
#else
	unit = UnitGetFar(id);
#endif
	n = 0;
	while ((rec = GetMotionRecord(BtlObjGet(id)->object, n)) != 0)
	{
		if (unit->side != BATTLE_UNIT_ENEMY)
#if GAME_TLA
			Animation_ApplyChildArgumentFar(rec, 4);
#else
			AnimationObjects_SelectAnimationFar(rec, 4);
#endif
		else
#if GAME_TLA
			Animation_ApplyChildArgumentFar(rec, 5);
#else
			AnimationObjects_SelectAnimationFar(rec, 5);
#endif
		n++;
	}

	if (unit->side == BATTLE_UNIT_ENEMY)
	{
		n = 0;
		while ((rec = GetMotionRecord(BtlObjGet(id)->object, n)) != 0)
		{
#if GAME_TLA
			/* FAKEMATCH: the native CSE block ends after the child lookup; a one-pass block
			   keeps its load before the index shift, which a straight assignment reverses. */
			do
			{
				ent = rec->entries[0];
			} while (0);
#else
			ent = rec->entries[0];
#endif
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
