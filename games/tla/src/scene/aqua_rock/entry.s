.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_020017f4
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_020004a4
	overlay_veneer Func_020013b8
	overlay_veneer Func_02000344
	overlay_veneer Scene_PrepareMap
