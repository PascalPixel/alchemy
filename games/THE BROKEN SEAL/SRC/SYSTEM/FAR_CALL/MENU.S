.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global FarCall_MenuTable
FarCall_MenuTable:
	.global RunAssetSelectionScreenFar
	.thumb_func
RunAssetSelectionScreenFar:
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
	.thumb_func
PsynergyMenu_InitializeEntryObjectsFar:
	overlay_veneer PsynergyMenu_InitializeEntryObjects
	.global Menu_ReleaseEntryObjectsFar
	.thumb_func
Menu_ReleaseEntryObjectsFar:
	overlay_veneer Menu_ReleaseEntryObjects
	.global UiIcon_PrepareObjectFar
	.thumb_func
UiIcon_PrepareObjectFar:
	overlay_veneer UiIcon_PrepareObject
	.global ItemMenu_RunFar
	.thumb_func
ItemMenu_RunFar:
	overlay_veneer ItemMenu_Run
	.global ItemMenu_DrawItemDetailsFar
	.thumb_func
ItemMenu_DrawItemDetailsFar:
	overlay_veneer ItemMenu_DrawItemDetails
