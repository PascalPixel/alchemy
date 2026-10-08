.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer FuneHobashira_ApplyEntryState
	overlay_veneer FuneHobashira_GetEntrances
	overlay_veneer FuneHobashira_GetExits
	overlay_veneer FuneHobashira_GetPlacements
	overlay_veneer FuneHobashira_GetEvents
	overlay_veneer FuneHobashira_GetRegions
