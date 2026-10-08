.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_0200063c
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_020000ac
	overlay_veneer Func_020005ec
	overlay_veneer Scene_GetRegions
	overlay_veneer Scene_PrepareMap
