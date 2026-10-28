.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	overlay_veneer Func_020051f8
	overlay_veneer Func_020025cc
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02002604
	overlay_veneer Func_02004c00
	overlay_veneer Scene_GetRegions
	overlay_veneer Func_02005cb8
