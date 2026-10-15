//--------------------------------------------------------------------------
// actor.c
// バトル：倒れたユニットの絵と並びの後しまつ
//--------------------------------------------------------------------------

#include "types.h"
#include "edition.h"
#include "battle.h"
#include "system.h"
#include "battle_party.h"
#include "battle_system.h"
#include "battle_type.h"
#include "battle_work.h"
#include "battle_summon.h"
#include "motobj.h"
#include "animspr.h"

struct BattleUnit *UnitGet(s32);

void Animation_ApplyChildArgumentFar(void *, s32);

//--------------------------------------------------------------------------
// ユニットを味方か敵の並びから外し、行動の予定からも消す
// 外せたら 0、どちらの並びにもいなければ -1 を返す
// id : ユニット番号
//--------------------------------------------------------------------------
s32 BattleActor_RemoveFromLists(s32 id)
{
	struct BattleWork *work;
	s32				i;
	u32				j;
	s32				unit;

	// 味方の並び
	work = Ram_HeapSlots->battle_work;
	UnitGet(id)->side = BATTLE_UNIT_ABSENT;
	for (i = 0;; i++)
	{
		unit = work->party_units[i];
		if (unit == id)
		{
			work->party_units[i] = BATTLE_UNIT_REMOVED;
			goto removed;
		}
		if (unit == BATTLE_UNIT_LIST_END)
			break;
	}

	// 敵の並び
	for (j = 0;; j++)
	{
		unit = work->enemy_ids[j];
		if (unit == id)
		{
			work->enemy_ids[j] = BATTLE_UNIT_REMOVED;
			goto removed;
		}
		if (unit == BATTLE_UNIT_LIST_END)
			break;
	}
	return -1;

removed:
	EnemyCountRelease(id);
	for (j = 0; j < 20; j++)
	{
		if (work->actions[j].unit_id == id)
			work->actions[j].unit_id = BATTLE_UNIT_LIST_END;
	}
	return 0;
}

//--------------------------------------------------------------------------
// 倒れたユニットの絵：味方はアニメ 4、敵はアニメ 5 にする
// 敵はさらに 4 フレーム待って並びから外し、そのあと絵を 1 ドットずつ消す
// id : ユニット番号
//--------------------------------------------------------------------------
void BattleMotion_InitializeActorRecords(s32 id)
{
	void				   *recs[4];
	struct BattleUnit		   *unit;
	struct AnimationObject *rec;
	struct AnimationEntry *ent;
	s32 n;

	unit = UnitGet(id);
	n = 0;
	while ((rec = GetMotionRecord(BattleObjGet(id)->object, n)) != 0)
	{
		if (unit->side != BATTLE_UNIT_ENEMY)
			Animation_ApplyChildArgumentFar(rec, 4);
		else
			Animation_ApplyChildArgumentFar(rec, 5);
		n++;
	}

	if (unit->side == BATTLE_UNIT_ENEMY)
	{
		n = 0;
		while ((rec = GetMotionRecord(BattleObjGet(id)->object, n)) != 0)
		{
			/* FAKEMATCH: the native CSE block ends after the child lookup; a one-pass block
			   keeps its load before the index shift, which a straight assignment reverses. */
			do
			{
				ent = rec->entries[0];
			} while (0);
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
