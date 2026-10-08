.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_080201f0
	.thumb_func
Func_080201f0:
	overlay_veneer Func_0802b450
	.global Func_080201f8
	.thumb_func
Func_080201f8:
	overlay_veneer Func_0802b590
	.global Func_08020200
	.thumb_func
Func_08020200:
	overlay_veneer Map_SetWindowCellTile
	.global Func_08020208
	.thumb_func
Func_08020208:
	overlay_veneer Func_0802bdd0
