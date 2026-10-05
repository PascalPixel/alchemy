.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer KorimaKawa_PrepareWindowTransition
	overlay_veneer MapStagedScene_SelectPrimaryData
	overlay_veneer MapStagedScene_SelectSecondaryData
	overlay_veneer MapStagedScene_SelectTertiaryData
	overlay_veneer KorimaKawa_GetEvents
	overlay_veneer MapStagedScene_GetEmptyData
