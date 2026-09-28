@ Far-call stub table heading the menu code at 0x080a1000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global RunAssetSelectionScreenFar
	.global Func_080a1000
	.thumb_func
RunAssetSelectionScreenFar:
Func_080a1000:
	overlay_veneer 0x080a24d1
	.global Menu_OpenConfirmPromptFar
	.thumb_func
Menu_OpenConfirmPromptFar:
	overlay_veneer 0x080a5b95
	.global Menu_OpenActionFlow
	.thumb_func
Menu_OpenActionFlow:
	overlay_veneer 0x080a7479
	overlay_veneer 0x080a7381
	overlay_veneer 0x080a1779
	.global PsynergyMenu_InitializeEntryObjectsFar
	.global Func_080a1028
	.thumb_func
PsynergyMenu_InitializeEntryObjectsFar:
Func_080a1028:
	overlay_veneer 0x080a1871
	.global Menu_ReleaseEntryObjectsFar
	.thumb_func
Menu_ReleaseEntryObjectsFar:
	overlay_veneer 0x080a195d
	.global UiIcon_PrepareObjectFar
	.thumb_func
UiIcon_PrepareObjectFar:
	overlay_veneer 0x080a17c5
	.global ItemMenu_Open
	.thumb_func
ItemMenu_Open:
	overlay_veneer 0x080aa56d
	overlay_veneer 0x080a4925
