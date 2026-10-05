@ Entry veneers opening the KORIMA_MAGARI overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — KORIMA_MAGARI overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer KorimaMagari_SetupBoard
	overlay_veneer KorimaMagari_GetEntrances
	overlay_veneer KorimaMagari_GetExits
	overlay_veneer KorimaMagari_GetPlacements
	overlay_veneer KorimaMagari_GetEvents
	overlay_veneer SceneData_ReturnZero
