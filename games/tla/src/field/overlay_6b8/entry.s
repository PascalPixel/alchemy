.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_0200079c
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_0200004c
	overlay_veneer Scene_GetEvents
	overlay_veneer Scene_GetRegions
	overlay_veneer Scene_PrepareMap
