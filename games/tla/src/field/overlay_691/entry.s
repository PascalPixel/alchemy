.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_02002618
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_020008d4
	overlay_veneer Func_020021e8
	overlay_veneer Func_0200088c
	overlay_veneer Scene_PrepareMap
