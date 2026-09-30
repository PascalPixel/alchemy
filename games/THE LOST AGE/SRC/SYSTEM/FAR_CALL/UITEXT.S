@ Far-call stub table at 0x08038000: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Resource_FarCall004
Resource_FarCall004:
	.global Func_08038000
	.thumb_func
Func_08038000:
	overlay_veneer Func_08038f40
	overlay_veneer Func_08039004
	.global UiWindow_CreateFar
	.thumb_func
UiWindow_CreateFar:
	overlay_veneer UiWindow_Create
	.global UiWork_FinalizeFar
	.thumb_func
UiWork_FinalizeFar:
	overlay_veneer UiWork_Finalize
	.global Func_08038020
	.thumb_func
Func_08038020:
	overlay_veneer Func_0803c9ac
	overlay_veneer Func_0803ca20
	overlay_veneer Func_0803c9bc
	.global UiText_OpenMessageWindowFar
	.thumb_func
UiText_OpenMessageWindowFar:
	overlay_veneer UiText_OpenMessageWindow
	.global Func_08038040
	.thumb_func
Func_08038040:
	overlay_veneer Func_0803a7ac
	.global UiWork_IsCompleteFar
	.thumb_func
UiWork_IsCompleteFar:
	overlay_veneer UiWork_IsComplete
	overlay_veneer Func_0803a3e4
	overlay_veneer Func_0803a668
	.global RenderOutput_PrepareForRedrawFar
	.thumb_func
RenderOutput_PrepareForRedrawFar:
	overlay_veneer RenderOutput_PrepareForRedraw
	overlay_veneer Func_08039454
	.global Func_08038070
	.thumb_func
Func_08038070:
	overlay_veneer Func_08041c54
	.global UiText_DrawResourceFar
	.thumb_func
UiText_DrawResourceFar:
	overlay_veneer UiText_DrawResource
	.global UiText_DrawCharacterAtOffsetFar
	.thumb_func
UiText_DrawCharacterAtOffsetFar:
	overlay_veneer UiText_DrawCharacterAtOffset
	overlay_veneer UiText_DrawString
