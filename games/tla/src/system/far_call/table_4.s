.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Map_GetTerrainHeightFar
	.thumb_func
Map_GetTerrainHeightFar:
	overlay_veneer Func_0802d45c
	.global GetMapCellCollisionFar
	.thumb_func
GetMapCellCollisionFar:
	overlay_veneer GetMapCellCollision
	.global Func_080201d0
	.thumb_func
Func_080201d0:
	overlay_veneer Func_0802d4d8
	.global Func_080201d8
	.thumb_func
Func_080201d8:
	overlay_veneer Func_0802d530
	.global Map_CopyCellsRectFar
	.thumb_func
Map_CopyCellsRectFar:
	overlay_veneer Map_CopyCellsRect
	.global Map_CopyCellAttributeRectFar
	.thumb_func
Map_CopyCellAttributeRectFar:
	overlay_veneer Map_CopyCellAttributeRect
