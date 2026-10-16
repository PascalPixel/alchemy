@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer VaultInteriorsRevisit_Initialize
	overlay_veneer VaultInteriorsRevisit_GetEntrances
	overlay_veneer VaultInteriorsRevisit_GetExits
	overlay_veneer VaultInteriorsRevisit_GetPlacements
	overlay_veneer VaultInteriorsRevisit_GetEvents
	overlay_veneer VaultInteriorsRevisit_GetRegions
