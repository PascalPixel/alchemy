@ Entry veneers opening the OVERLAY_68B overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — OVERLAY_68B overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000bdc
	overlay_veneer Func_020001a0
	overlay_veneer Func_020001ac
	overlay_veneer Func_020001b4
	overlay_veneer Func_02000a64
	overlay_veneer Func_020001a8
	overlay_veneer Func_02000e78
