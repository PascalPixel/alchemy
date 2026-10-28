.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	overlay_veneer Func_02002438
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000600
	overlay_veneer Func_02002338
	overlay_veneer Func_02000344
	overlay_veneer Scene_PrepareMap
