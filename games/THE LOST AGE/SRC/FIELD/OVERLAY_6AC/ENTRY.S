@ Entry veneers opening the OVERLAY_6AC overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — OVERLAY_6AC overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000b64
	overlay_veneer Func_02000038
	overlay_veneer Func_02000044
	overlay_veneer Func_02000080
	overlay_veneer Func_02000af4
	overlay_veneer Func_02000040
	overlay_veneer Func_02000e84
