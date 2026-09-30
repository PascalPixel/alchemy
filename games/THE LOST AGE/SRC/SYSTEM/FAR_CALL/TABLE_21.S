@ Far-call stub table at 0x080383e8: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global UiWindow_DrawFrameFar
	.thumb_func
UiWindow_DrawFrameFar:
	overlay_veneer Func_0803a084
	.global Func_080383f0
	.thumb_func
Func_080383f0:
	overlay_veneer UiWindow_EraseBorderRect
	.global Link_DrawShiftedTilePairFar
	.thumb_func
Link_DrawShiftedTilePairFar:
	overlay_veneer Func_08045330
	.global Func_08038400
	.thumb_func
Func_08038400:
	overlay_veneer Func_080430c4
	.global Func_08038408
	.thumb_func
Func_08038408:
	overlay_veneer Func_080435fc
