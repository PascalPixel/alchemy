.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	overlay_veneer Func_02000fb8
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000360
	overlay_veneer Func_02000d90
	overlay_veneer Scene_GetRegions
	overlay_veneer Scene_PrepareMap
