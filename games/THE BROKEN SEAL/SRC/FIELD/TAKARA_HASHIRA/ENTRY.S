@ Entry veneers opening the TAKARA_HASHIRA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — TAKARA_HASHIRA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_0200274c
	overlay_veneer Func_02001440
	overlay_veneer Func_020014bc
	overlay_veneer Func_020014c4
	overlay_veneer Func_02002580
	overlay_veneer Func_020014b8
