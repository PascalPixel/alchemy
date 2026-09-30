@ Far-call stub table at 0x08038000: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Resource_FarCall004
Resource_FarCall004:
	.global UiWork_InitializeWithResourceCountersFar
	.thumb_func
UiWork_InitializeWithResourceCountersFar:
	overlay_veneer UiWork_InitializeWithResourceCounters
	overlay_veneer UiWork_Initialize
	.global UiWindow_CreateFar
	.thumb_func
UiWindow_CreateFar:
	overlay_veneer UiWindow_Create
	.global UiWork_FinalizeFar
	.thumb_func
UiWork_FinalizeFar:
	overlay_veneer UiWork_Finalize
	.global Ui_AdjustValueWithoutLimitFar
	.thumb_func
Ui_AdjustValueWithoutLimitFar:
	overlay_veneer Ui_AdjustValueWithoutLimit
	overlay_veneer UiText_DecodeMessage
	overlay_veneer UiText_CopyMessageString
	.global UiText_OpenMessageWindowFar
	.thumb_func
UiText_OpenMessageWindowFar:
	overlay_veneer UiText_OpenMessageWindow
	.global UiText_ShowPositionedMessageAndWaitFar
	.thumb_func
UiText_ShowPositionedMessageAndWaitFar:
	overlay_veneer UiText_ShowPositionedMessageAndWait
	.global UiWork_IsCompleteFar
	.thumb_func
UiWork_IsCompleteFar:
	overlay_veneer UiWork_IsComplete
	overlay_veneer UiWork_IsIdle
	overlay_veneer UiWork_SetBusyFlags
	.global RenderOutput_PrepareForRedrawFar
	.thumb_func
RenderOutput_PrepareForRedrawFar:
	overlay_veneer RenderOutput_PrepareForRedraw
	overlay_veneer UiWindow_ClearInteriorTiles
	.global UiWindow_DrawDividerLineFar
	.thumb_func
UiWindow_DrawDividerLineFar:
	overlay_veneer UiWindow_DrawDividerLine
	.global UiText_DrawResourceFar
	.thumb_func
UiText_DrawResourceFar:
	overlay_veneer UiText_DrawResource
	.global UiText_DrawCharacterAtOffsetFar
	.thumb_func
UiText_DrawCharacterAtOffsetFar:
	overlay_veneer UiText_DrawCharacterAtOffset
	overlay_veneer UiText_DrawString
