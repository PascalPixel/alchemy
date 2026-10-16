@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer VaultInteriors_Initialize
	overlay_veneer VaultInteriors_GetEntrances
	overlay_veneer VaultInteriors_GetExits
	overlay_veneer VaultInteriors_GetPlacements
	overlay_veneer VaultInteriors_GetEvents
	overlay_veneer VaultInteriors_GetRegions
