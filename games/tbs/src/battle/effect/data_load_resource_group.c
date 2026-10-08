//--------------------------------------------------------------------------
// data_load_resource_group.c
// シーン表：現在のシーンのコードを読み込む
//--------------------------------------------------------------------------

#include "types.h"
#include "system.h"
#include "fieldrun.h"
#include "game_state.h"

//--------------------------------------------------------------------------
// 現在のシーンのコードをオーバーレイ領域へ読み込む
//--------------------------------------------------------------------------
void BattleFx_LoadResourceGroup(void)
{
	const struct SceneRecord *tbl;

	tbl = Field_SceneTable;
	Resource_LoadCode(tbl[gGameState.scene].resource_id, &gOverlayArea);
}
