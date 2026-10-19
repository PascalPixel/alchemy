@ Party menus and their interactive selection screens.
@ Ordered 8-byte jumps into this bank. Named exports are callable symbols;
@ bare veneers preserve entries that have no exported name.
#include "stub.inc"
	.global FarCall_MenuTable
FarCall_MenuTable:
	export RunAssetSelectionScreenFar, RunAssetSelectionScreen
	export PsynergyMenu_RunFar, PsynergyMenu_Run
	export Menu_OpenActionFlow, ActionMenu_Open
	export Menu_OpenCharacterSelectorFar, Menu_OpenCharacterSelector
	overlay_veneer UiIcon_CreateWithResourceVariant
	export PsynergyMenu_InitializeEntryObjectsFar, PsynergyMenu_InitializeEntryObjects
	export Menu_ReleaseEntryObjectsFar, Menu_ReleaseEntryObjects
	export UiIcon_PrepareObjectFar, UiIcon_PrepareObject
	export DjinnMenu_RunFar, DjinnMenu_Run
	export ItemMenu_DrawItemDetailsFar, ItemMenu_DrawItemDetails
