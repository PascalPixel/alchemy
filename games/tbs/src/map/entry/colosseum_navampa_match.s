@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ColosseumNavampaMatch_Initialize
	overlay_veneer ColosseumNavampaMatch_GetEntrances
	overlay_veneer ColosseumNavampaMatch_GetExits
	overlay_veneer ColosseumNavampaMatch_GetPlacements
	overlay_veneer ColosseumNavampaMatch_GetEvents
	overlay_veneer ColosseumNavampaMatch_GetRegions
