@ Entry veneers opening the BABI_FUNE overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — BABI_FUNE overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer BabiFune_SetupScene
	overlay_veneer SceneData_GetTable96c8
	overlay_veneer SceneData_GetTable9710
	overlay_veneer SceneData_GetTable971c
	overlay_veneer SceneData_GetTable97AC
	overlay_veneer BabiFune_GetDefaultResult
