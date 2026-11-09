@ Party menus and their interactive selection screens.
#include "table.inc"
	.global FarCall_MenuTable
FarCall_MenuTable:
	export ItemMenu_RunFar, ItemMenu_Run
	export PsynergyMenu_RunFar, PsynergyMenu_Run
	export StatusScreen_RunFar, StatusScreen_Run
	export PartyMemberChooser_OpenFar, PartyMemberChooser_Open
	overlay_veneer Icon_CreateWithResourceVariant
	export Menu_CreatePartyFiguresFar, Menu_CreatePartyFigures
	export Menu_ReleasePartyFiguresFar, Menu_ReleasePartyFigures
	export Icon_PrepareActorFar, Icon_PrepareActor
	export DjinnMenu_RunFar, DjinnMenu_Run
	export ItemMenu_DrawItemDetailsFar, ItemMenu_DrawItemDetails
