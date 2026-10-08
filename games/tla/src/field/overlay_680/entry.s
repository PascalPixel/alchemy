.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_020006ec
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000080
	overlay_veneer Func_02000434
	overlay_veneer Func_02000040
	overlay_veneer Func_02000a68
