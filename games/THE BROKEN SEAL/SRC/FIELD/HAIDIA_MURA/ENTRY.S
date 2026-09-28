@ Entry veneers opening the HAIDIA_MURA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — HAIDIA_MURA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer HaidiaMura_ApplyEntryState
	overlay_veneer MapStagedScene_SelectPrimaryData
	overlay_veneer MapStagedScene_SelectSecondaryData
	overlay_veneer MapStagedScene_SelectTertiaryData
	overlay_veneer MapStagedScene_SelectQuaternaryData
	overlay_veneer MapStagedScene_GetEmptyData
