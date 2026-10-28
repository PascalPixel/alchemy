.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	overlay_veneer Func_020002f8
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000070
	overlay_veneer Func_0200015c
	overlay_veneer Func_02000040
	overlay_veneer Func_0200030c
