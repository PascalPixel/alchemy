.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_02003698
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_020007cc
	overlay_veneer Func_02000874
	overlay_veneer Scene_GetRegions
	overlay_veneer Scene_PrepareMap
