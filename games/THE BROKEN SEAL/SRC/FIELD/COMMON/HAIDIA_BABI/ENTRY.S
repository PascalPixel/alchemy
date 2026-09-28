@ Entry veneers opening the HAIDIA_BABI overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — HAIDIA_BABI overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_020003f8
	overlay_veneer Func_02000088
	overlay_veneer Func_02000094
	overlay_veneer Func_020000b8
	overlay_veneer Func_02000160
	overlay_veneer Func_02000090
