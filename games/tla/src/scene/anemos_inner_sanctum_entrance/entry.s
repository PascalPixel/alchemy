.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	overlay_veneer Func_02000740
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_0200035c
	overlay_veneer Func_02000714
	overlay_veneer Scene_GetRegions
	overlay_veneer Scene_PrepareMap
