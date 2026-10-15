//--------------------------------------------------------------------------
// advance_owner_count_to_target.c
// パーティ：キャラクターを指定のレベルまで上げる
//--------------------------------------------------------------------------

#include "owner_state.h"
#include "party_state.h"
#include "battle_unit.h"

struct LevelUpResult;
struct LevelUpResult *Owner_LevelUp(s32 owner, struct LevelUpResult *result);
void				  UnitCalc(s32);
u32					  Owner_GetLevelThreshold(s32 owner, s32 level);

// Levels the owner up once its experience reaches the next level's threshold;
// returns the result record then, and 0 otherwise.
struct LevelUpResult *Func_080afb80(s32 owner_no, struct LevelUpResult *value)
{
	struct BtlUnit *owner;

	owner = (struct BtlUnit *)UnitGet(owner_no);
	/* FAKEMATCH: the barrier loads the level before the experience field's offset is started; plain, the offset comes first (one reordered pair, differ 2). */
	asm("");
	if ((owner->experience >= Owner_GetLevelThreshold(owner_no, owner->level + 1))
		&& (Owner_LevelUp(owner_no, value) != NULL))
	{
		return value;
	}
	return 0;
}

//--------------------------------------------------------------------------
// 目標のレベルになるまで 1 つずつレベルアップさせ、能力を計算しなおす
// unit : キャラクター番号
// lv   : 目標のレベル
//--------------------------------------------------------------------------
void Party_AdvanceOwnerCountToTarget(s32 unit, s32 lv)
{
	u8	res[16]; // struct LevelUpResult の受け取り（使わない）
	s32 cur = ((struct BtlUnit *)UnitGet(unit))->level;

	while (cur < lv)
	{
		Owner_LevelUp(unit, (struct LevelUpResult *)res);
		cur++;
	}
	UnitCalc(unit);
}
