.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer KorimaKi_PrepareActors
	overlay_veneer KorimaKi_GetEntrances
	overlay_veneer KorimaKi_GetExits
	overlay_veneer KorimaKi_GetPlacements
	overlay_veneer KorimaKi_GetEvents
	overlay_veneer KorimaKi_GetRegions
