@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer VaultRevisit_Initialize
	overlay_veneer VaultRevisit_GetEntrances
	overlay_veneer VaultRevisit_GetExits
	overlay_veneer VaultRevisit_GetPlacements
	overlay_veneer VaultRevisit_GetEvents
	overlay_veneer VaultRevisit_GetRegions
