@ Entry veneers opening the FUNE_HOBASHIRA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — FUNE_HOBASHIRA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer FuneHobashira_ApplyEntryState
	overlay_veneer SceneData_GetTable95c0
	overlay_veneer SceneData_GetTable96a0
	overlay_veneer SceneData_GetTable96C4
	overlay_veneer SceneData_GetTable988c
	overlay_veneer SceneData_GetTable9680
