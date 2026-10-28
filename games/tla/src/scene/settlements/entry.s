.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	overlay_veneer Func_02000988
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000358
	overlay_veneer Func_02000668
	overlay_veneer Func_02000318
	overlay_veneer Func_02000b18
