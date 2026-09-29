@ Entry veneers opening the WORLD_MAP overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — WORLD_MAP overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_020006ec
	overlay_veneer WorldMap_GetEntrances
	overlay_veneer WorldMap_GetExits
	overlay_veneer StoryScene_SelectPlacementTable
	overlay_veneer WorldMap_GetEvents
	overlay_veneer WorldMap_GetRegions
