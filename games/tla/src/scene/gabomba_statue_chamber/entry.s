.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_02001aa4
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000070
	overlay_veneer Func_020000b0
	overlay_veneer Func_02000040
	overlay_veneer Scene_PrepareMap
