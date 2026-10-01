/* NONMATCHING: Japanese pillar imports, 2026-10-01.
 * Japanese now uses its direction-step table, so its polar-offset veneer
 * is absent. Retaining this complete eight-byte veneer shifts later imports.
 */
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.section .text,"ax",%progbits
	.balign 4
	.global Vector_AddPolarOffset
	.thumb_func
Vector_AddPolarOffset:
	overlay_veneer Vector_AddPolarOffsetFar
