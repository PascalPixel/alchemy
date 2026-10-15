.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_02003ff8
	overlay_veneer Func_02000290
	overlay_veneer Scene_GetExits
	overlay_veneer Func_020002dc
	overlay_veneer Func_02000c80
	overlay_veneer Scene_GetRegions
	overlay_veneer Scene_PrepareMap
