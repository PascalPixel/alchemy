@ Far-call stub table heading the resource metadata reader at 0x08185000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global Resource_GetMetadataRecordFar
	.global Func_08185000
	.thumb_func
Resource_GetMetadataRecordFar:
Func_08185000:
	.irp target, 0x08185009
	overlay_veneer \target
	.endr
