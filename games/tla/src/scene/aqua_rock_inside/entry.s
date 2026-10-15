.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_02002020
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_0200035c
	overlay_veneer Func_02001da8
	overlay_veneer Scene_GetRegions
	overlay_veneer Func_02002340
