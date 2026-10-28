.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	overlay_veneer Func_02001418
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000d44
	overlay_veneer Func_020013d8
	overlay_veneer Scene_GetRegions
	overlay_veneer Scene_PrepareMap
