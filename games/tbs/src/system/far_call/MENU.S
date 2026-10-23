@ Party menus and their interactive selection screens.
@ Ordered 8-byte jumps into this bank. Named exports are callable symbols;
@ bare veneers preserve entries that have no exported name.
#include "stub.inc"
	.global FarCall_MenuTable
FarCall_MenuTable:
	export ItemMenu_RunFar, ItemMenu_Run
	export PsynergyMenu_RunFar, PsynergyMenu_Run
	export SummaryScreen_RunFar, SummaryScreen_Run
	export MemberChooser_OpenFar, MemberChooser_Open
	overlay_veneer Icon_CreateWithResourceVariant
	export PsynergyMenu_InitializeEntryObjectsFar, PsynergyMenu_InitializeEntryObjects
	export Menu_ReleaseEntryObjectsFar, Menu_ReleaseEntryObjects
	export Icon_PrepareObjectFar, Icon_PrepareObject
	export DjinnMenu_RunFar, DjinnMenu_Run
	export ItemMenu_DrawItemDetailsFar, ItemMenu_DrawItemDetails
