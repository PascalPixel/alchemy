.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_020020c4
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000088
	overlay_veneer Func_02001890
	overlay_veneer Scene_GetRegions
	overlay_veneer Func_0200242c
