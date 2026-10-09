//--------------------------------------------------------------------------
// actor.c
// バトル：倒れたユニットの絵と並びの後しまつ
//--------------------------------------------------------------------------

#include "types.h"
#include "edition.h"
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
