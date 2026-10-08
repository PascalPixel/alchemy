.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_02000d08
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000064
	overlay_veneer Func_020000e0
	overlay_veneer Scene_GetRegions
	overlay_veneer Func_02000d3c
