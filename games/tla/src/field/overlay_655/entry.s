.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_0200304c
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_020011b8
	overlay_veneer Func_02001240
	overlay_veneer Scene_GetRegions
	overlay_veneer Scene_PrepareMap
