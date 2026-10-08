.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer HaidiaBabi_RestoreEntryState
	overlay_veneer HaidiaBabi_GetEntrances
	overlay_veneer HaidiaBabi_SelectExits
	overlay_veneer HaidiaBabi_SelectPlacements
	overlay_veneer HaidiaBabi_SelectEvents
	overlay_veneer SceneData_ReturnZero
