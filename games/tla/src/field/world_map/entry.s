.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_02000dc4
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_020003f8
	overlay_veneer Scene_GetEvents
	overlay_veneer Scene_GetRegions
	overlay_veneer Scene_PrepareMap
