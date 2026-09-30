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
	overlay_veneer RenderOutput_CreateFromResource
	overlay_veneer Func_08042450
	overlay_veneer Func_08042508
	overlay_veneer UiWork_SetByte5AndResetHalfwordC
	overlay_veneer Func_0804257c
