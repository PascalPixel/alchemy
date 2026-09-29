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
	overlay_veneer RunAssetSelectionScreen
	.global Menu_OpenConfirmPromptFar
	.thumb_func
Menu_OpenConfirmPromptFar:
	overlay_veneer Menu_OpenConfirmPrompt
	.global Menu_OpenActionFlow
	.thumb_func
Menu_OpenActionFlow:
	overlay_veneer ActionMenu_Open
	.global Menu_OpenCharacterSelectorFar
	.thumb_func
Menu_OpenCharacterSelectorFar:
	overlay_veneer Menu_OpenCharacterSelector
	overlay_veneer UiIcon_CreateWithResourceVariant
	.global PsynergyMenu_InitializeEntryObjectsFar
	.global Func_080a1028
	.thumb_func
PsynergyMenu_InitializeEntryObjectsFar:
Func_080a1028:
	overlay_veneer PsynergyMenu_InitializeEntryObjects
	.global Menu_ReleaseEntryObjectsFar
	.thumb_func
Menu_ReleaseEntryObjectsFar:
	overlay_veneer Menu_ReleaseEntryObjects
	.global UiIcon_PrepareObjectFar
	.thumb_func
UiIcon_PrepareObjectFar:
	overlay_veneer UiIcon_PrepareObject
	.global ItemMenu_Open
	.thumb_func
ItemMenu_Open:
	.global Func_080a1040
	.thumb_func
Func_080a1040:
	overlay_veneer ItemMenu_Run
	.global ItemMenu_DrawItemDetailsFar
	.thumb_func
ItemMenu_DrawItemDetailsFar:
	overlay_veneer ItemMenu_DrawItemDetails