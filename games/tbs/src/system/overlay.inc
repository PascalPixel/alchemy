@ The head of an overlay's two stub tables, entry.s and import.s, and of the
@ far-call banks' stub.inc: Thumb code on a word boundary, and the stub.
	.syntax unified
	.thumb
	.balign 4
	.include "games/common/src/system/overlay.s"
@ One stub of an overlay's import table: the overlay's code calls NAME, and
@ the stub goes to TARGET, an entry of the main image.
	.macro import name, target
	.global \name
	.thumb_func
\name:
	overlay_veneer \target
	.endm
