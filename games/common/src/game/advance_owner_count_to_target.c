//--------------------------------------------------------------------------
// advance_owner_count_to_target.c
// パーティ：キャラクターを指定のレベルまで上げる
//--------------------------------------------------------------------------

#include "owner_state.h"
#include "party_state.h"

struct LevelUpResult;
struct LevelUpResult *Owner_LevelUp(s32 owner, struct LevelUpResult *result);
void				  UnitCalc(s32);

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
