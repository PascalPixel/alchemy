.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	overlay_veneer Func_02000d54
	overlay_veneer Func_02000318
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000388
	overlay_veneer Func_020003ec
	overlay_veneer Func_02000358
	overlay_veneer Scene_PrepareMap
