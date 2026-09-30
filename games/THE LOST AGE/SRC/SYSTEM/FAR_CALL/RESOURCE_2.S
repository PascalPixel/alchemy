@ Far-call stub table at 0x08020058: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global ResourceMetadata_RegisterFar
	.thumb_func
ResourceMetadata_RegisterFar:
	overlay_veneer ResourceMetadata_Register
	.global Func_08020060
	.thumb_func
Func_08020060:
	overlay_veneer Func_08022a24
	.global Func_08020068
	.thumb_func
Func_08020068:
	overlay_veneer Func_08022a84
	.global Func_08020070
	.thumb_func
Func_08020070:
	overlay_veneer ResourceMetadata_ClearRecord
	.global Func_08020078
	.thumb_func
Func_08020078:
	overlay_veneer Animation_InitWorkFromMetadata
