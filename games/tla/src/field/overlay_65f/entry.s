.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_02000aa8
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000440
	overlay_veneer Func_020009dc
	overlay_veneer Scene_GetRegions
	overlay_veneer Scene_PrepareMap
