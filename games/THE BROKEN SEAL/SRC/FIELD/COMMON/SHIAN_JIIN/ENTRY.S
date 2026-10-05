.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer ShianJiin_ApplyEntryState
	overlay_veneer Scene_GetEntrances
	overlay_veneer GetXianMessageData
	overlay_veneer Scene_GetPlacements
	overlay_veneer Scene_GetEvents
	overlay_veneer GetXianInitialState
