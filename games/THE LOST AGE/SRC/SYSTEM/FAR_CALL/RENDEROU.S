@ Far-call stub table at 0x080380c8: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global RenderOutput_CreateFar
	.thumb_func
RenderOutput_CreateFar:
	overlay_veneer RenderOutput_Create
	.global RenderOutput_CreateFromResourceFar
	.thumb_func
RenderOutput_CreateFromResourceFar:
	overlay_veneer RenderOutput_CreateFromResource
	.global Func_080380d8
	.thumb_func
Func_080380d8:
	overlay_veneer Func_08042450
	.global Func_080380e0
	.thumb_func
Func_080380e0:
	overlay_veneer Func_08042508
	.global UiWork_SetByte5AndResetHalfwordCFar
	.thumb_func
UiWork_SetByte5AndResetHalfwordCFar:
	overlay_veneer UiWork_SetByte5AndResetHalfwordC
	.global Func_080380f0
	.thumb_func
Func_080380f0:
	overlay_veneer Func_0804257c
