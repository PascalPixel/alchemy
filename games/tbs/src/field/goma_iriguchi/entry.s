.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer GomaIriguchi_RestoreEntryState
	overlay_veneer GomaIriguchi_GetEntrances
	overlay_veneer GomaIriguchi_GetExits
	overlay_veneer GomaIriguchi_GetPlacements
	overlay_veneer GomaIriguchi_GetEvents
	overlay_veneer SceneData_ReturnZero
