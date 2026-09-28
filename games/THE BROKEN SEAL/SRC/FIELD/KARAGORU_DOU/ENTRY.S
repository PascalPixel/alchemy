@ Entry veneers opening the KARAGORU_DOU overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — KARAGORU_DOU overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02001394
	overlay_veneer Func_02000cc0
	overlay_veneer StagedActorPairScene_GetMessageData
	overlay_veneer Func_02000d20
	overlay_veneer Func_02000d88
	overlay_veneer StagedActorPairScene_GetInitialValue
