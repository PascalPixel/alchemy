.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	overlay_veneer Func_02000e7c
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000058
	overlay_veneer Func_02000e04
	overlay_veneer Scene_GetRegions
	overlay_veneer Func_0200140c
