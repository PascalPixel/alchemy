@ Entry veneers opening the SORU_IRIGUCHI overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — SORU_IRIGUCHI overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_020008f4
	overlay_veneer Func_02000030
	overlay_veneer Scene_GetExits
	overlay_veneer Func_0200007c
	overlay_veneer Func_020000ec
	overlay_veneer Scene_GetRegions
