.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_02000988
	overlay_veneer Func_02000064
	overlay_veneer Scene_GetExits
	overlay_veneer Func_0200009c
	overlay_veneer Func_0200070c
	overlay_veneer Scene_GetRegions
	overlay_veneer Func_02000bb4
