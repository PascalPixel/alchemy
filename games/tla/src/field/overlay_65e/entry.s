.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_02000340
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_0200004c
	overlay_veneer Func_020000c8
	overlay_veneer Scene_GetRegions
	overlay_veneer Scene_PrepareMap
