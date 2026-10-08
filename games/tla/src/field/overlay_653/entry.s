.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_02002854
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_020003d0
	overlay_veneer Func_02000580
	overlay_veneer Func_02000390
	overlay_veneer Scene_PrepareMap
