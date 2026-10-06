.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_020010c0
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000080
	overlay_veneer Func_02000cc0
	overlay_veneer Func_02000050
	overlay_veneer Func_020011ec
