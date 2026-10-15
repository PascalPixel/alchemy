#include "field.h"
#include "resource_ids.h"
#include "types.h"

//--------------------------------------------------------------------------
// ワールドマップの資源
// World map graphics: a palette and five tile sets for each of its two looks.
//--------------------------------------------------------------------------

// パレットとタイル 5 枚。読み込む位置のセルの種類が 21 なら 2 組目を使う。
// 読む側が u32 * で持つので const にしない。
u32 gWorldMap_GraphicsResources[2][6] =
{
	{
		(u32)&ResourceId_WorldMapPaletteA,
		(u32)&ResourceId_WorldMapTilesA,
		(u32)&ResourceId_WorldMapTilesB,
		(u32)&ResourceId_WorldMapTilesC,
		(u32)&ResourceId_WorldMapTilesD,
		(u32)&ResourceId_WorldMapTilesE,
	},
	{
		(u32)&ResourceId_WorldMapPaletteB,
		(u32)&ResourceId_WorldMapTilesF,
		(u32)&ResourceId_WorldMapTilesG,
		(u32)&ResourceId_WorldMapTilesH,
		(u32)&ResourceId_WorldMapTilesI,
		(u32)&ResourceId_WorldMapTilesJ,
	},
};
