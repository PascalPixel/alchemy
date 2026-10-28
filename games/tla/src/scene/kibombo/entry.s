.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	overlay_veneer Func_02005034
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_0200012c
	overlay_veneer Func_020001b4
	overlay_veneer Func_020000fc
	overlay_veneer Scene_PrepareMap
