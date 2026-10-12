@ The head of an overlay's two stub tables, entry.s and import.s, and of the
@ far-call banks' stub.inc: Thumb code on a word boundary, and the stub.
	.syntax unified
	.thumb
	.balign 4
	.include "games/common/src/system/overlay.s"
