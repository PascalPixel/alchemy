@ Far-call stub table at 0x08020000: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Resource_FarCall003
Resource_FarCall003:
	.global Resource_GetMetadataRecordFarFar
	.thumb_func
Resource_GetMetadataRecordFarFar:
	overlay_veneer Resource_GetMetadataRecordFar
	.global Func_08020008
	.thumb_func
Func_08020008:
	overlay_veneer Func_08022bd8
