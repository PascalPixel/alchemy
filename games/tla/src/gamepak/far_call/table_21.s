.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Window_DrawBorderFar
	.thumb_func
Window_DrawBorderFar:
	overlay_veneer Window_DrawBorder
	.global Window_EraseBorderRectFar
	.thumb_func
Window_EraseBorderRectFar:
	overlay_veneer Window_EraseBorderRect
	.global Link_DrawShiftedTilePairFar
	.thumb_func
Link_DrawShiftedTilePairFar:
	overlay_veneer Link_DrawShiftedTilePair
	.global Func_08038400
	.thumb_func
Func_08038400:
	overlay_veneer Func_080430c4
	.global Func_08038408
	.thumb_func
Func_08038408:
	overlay_veneer Save_ToFile
