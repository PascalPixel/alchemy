.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	overlay_veneer Func_02000ac8
	overlay_veneer Scene_GetEntrances
	overlay_veneer Func_02000044
	overlay_veneer Func_02000070
	overlay_veneer Func_02000128
	overlay_veneer Scene_GetRegions
	overlay_veneer Func_02000b44
