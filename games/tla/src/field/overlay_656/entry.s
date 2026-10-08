.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_02001abc
	overlay_veneer Func_02000038
	overlay_veneer Scene_GetExits
	overlay_veneer Func_020000b8
	overlay_veneer Func_020004f4
	overlay_veneer Func_02000078
	overlay_veneer Func_02001c38
