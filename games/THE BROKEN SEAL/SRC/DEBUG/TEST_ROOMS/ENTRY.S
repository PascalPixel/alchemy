@ Entry veneers opening the TEST_ROOMS overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — TEST_ROOMS overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Scene_RunEmptyScene
	overlay_veneer TestRooms_GetEntrances
	overlay_veneer TestRooms_GetExits
	overlay_veneer TestRooms_GetPlacements
	overlay_veneer TestRooms_GetEvents
	overlay_veneer SceneData_ReturnZero
