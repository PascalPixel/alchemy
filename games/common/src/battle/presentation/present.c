//--------------------------------------------------------------------------
// present.c
// バトル表示：ユニットの絵の半透明の切りかえ
//--------------------------------------------------------------------------

#include "types.h"
#include "btlpty.h"
#include "btldisp.h"

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
