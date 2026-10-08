//--------------------------------------------------------------------------
// DATA_LOAD_RESOURCE_GROUP.C
// シーン表：現在のシーンのコードを読み込む
//--------------------------------------------------------------------------

#include "TYPES.H"
#include "SYSTEM.H"
#include "FIELDRUN.H"
#include "GAME_STATE.H"

//--------------------------------------------------------------------------
// 現在のシーンのコードをオーバーレイ領域へ読み込む
//--------------------------------------------------------------------------
void BattleFx_LoadResourceGroup(void)
{
	const struct SceneRecord *tbl;

	tbl = Field_SceneTable;
	Resource_LoadCode(tbl[gGameState.scene].resource_id, &gOverlayArea);
}
