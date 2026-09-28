@ Entry veneers opening the KORIMA_HASHI overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — KORIMA_HASHI overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000c68
	overlay_veneer Func_02000a4c
	overlay_veneer Func_02000a58
	overlay_veneer Func_02000a60
	overlay_veneer Func_02000c60
	overlay_veneer Func_02000a54
