@ Party menus and their interactive selection screens.
#include "stub.inc"
	.global FarCall_MenuTable
FarCall_MenuTable:
	export ItemMenu_RunFar, ItemMenu_Run
	export PsynergyMenu_RunFar, PsynergyMenu_Run
	export StatusScreen_RunFar, StatusScreen_Run
	export MemberChooser_OpenFar, MemberChooser_Open
	overlay_veneer Icon_CreateWithResourceVariant
	export PsynergyMenu_InitializeEntryActorsFar, PsynergyMenu_InitializeEntryActors
	export Menu_ReleaseEntryActorsFar, Menu_ReleaseEntryActors
	export Icon_PrepareActorFar, Icon_PrepareActor
	export DjinnMenu_RunFar, DjinnMenu_Run
	export ItemMenu_DrawItemDetailsFar, ItemMenu_DrawItemDetails
