@ Far-call stub table at 0x080f8028: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global UiIcon_PrepareObjectFar
	.thumb_func
UiIcon_PrepareObjectFar:
	overlay_veneer UiIcon_PrepareObject
	.global ItemMenu_Open
	.thumb_func
ItemMenu_Open:
	overlay_veneer Func_08100e7c
	.global ItemMenu_DrawItemDetailsFar
	.thumb_func
ItemMenu_DrawItemDetailsFar:
	overlay_veneer ItemMenu_DrawItemDetails
	.global Func_080f8040
	.thumb_func
Func_080f8040:
	overlay_veneer Func_080fb8b8
	.global Palette_CopyObjectBankToBackground14Far
	.thumb_func
Palette_CopyObjectBankToBackground14Far:
	overlay_veneer Palette_CopyObjectBankToBackground14
	.global Menu_UpdateEntryObjectTransformsFar
	.thumb_func
Menu_UpdateEntryObjectTransformsFar:
	overlay_veneer Menu_UpdateEntryObjectTransforms
