@ Party menus and their interactive selection screens.
@ C2: aligned 8-byte far-call stubs: export and overlay_veneer emit ldr r4, bx r4 and a target word.
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
