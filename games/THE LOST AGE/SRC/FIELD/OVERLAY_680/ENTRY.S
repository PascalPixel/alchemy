.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_020006ec
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000080
	overlay_veneer Func_02000434
	overlay_veneer Func_02000040
	overlay_veneer Func_02000a68
