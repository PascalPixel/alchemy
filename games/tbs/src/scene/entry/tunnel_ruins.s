@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer TunnelRuins_Initialize
	overlay_veneer TunnelRuins_GetEntrances
	overlay_veneer TunnelRuins_GetExits
	overlay_veneer TunnelRuins_GetPlacements
	overlay_veneer TunnelRuins_GetEvents
	overlay_veneer TunnelRuins_GetRegions
