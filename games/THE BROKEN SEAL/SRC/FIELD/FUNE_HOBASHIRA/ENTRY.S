.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer FuneHobashira_ApplyEntryState
	overlay_veneer FuneHobashira_GetEntrances
	overlay_veneer FuneHobashira_GetExits
	overlay_veneer FuneHobashira_GetPlacements
	overlay_veneer FuneHobashira_GetEvents
	overlay_veneer FuneHobashira_GetRegions
