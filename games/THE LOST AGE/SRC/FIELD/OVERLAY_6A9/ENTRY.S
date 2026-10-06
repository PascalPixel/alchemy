.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000b20
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000374
	overlay_veneer Func_02000ad0
	overlay_veneer Func_02000344
	overlay_veneer Func_02000b64
