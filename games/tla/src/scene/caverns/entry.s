.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_02000b20
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000374
	overlay_veneer Func_02000ad0
	overlay_veneer Func_02000344
	overlay_veneer Func_02000b64
