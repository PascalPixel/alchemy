//--------------------------------------------------------------------------
// LIST_ALL_UNITS_AND_SUBMIT.C
// バトル：全ユニットのオブジェクト配置
//--------------------------------------------------------------------------

#include "TYPES.H"
#include "BATTLE.H"
#include "BTLPTY.H"

//--------------------------------------------------------------------------
// 戦闘中の全ユニットの絵を用意する（いなくなったユニットの絵は解放）
//--------------------------------------------------------------------------
void BattleParty_ListAllUnitsAndSubmit(void)
{
	s16 list[14];

	BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
	BattleActor_SpawnObjectsForList(list, 0);
}
