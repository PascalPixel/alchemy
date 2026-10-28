.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global UiWindow_DrawBorderFar
	.thumb_func
UiWindow_DrawBorderFar:
	overlay_veneer UiWindow_DrawBorder
	.global UiWindow_EraseBorderRectFar
	.thumb_func
UiWindow_EraseBorderRectFar:
	overlay_veneer UiWindow_EraseBorderRect
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
	overlay_veneer SaveState_SaveToSlot
