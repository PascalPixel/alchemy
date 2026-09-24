@ Far-call stub table heading the sound command code at 0x080f9000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080f9000
	.thumb_func
Func_080f9000:
	.irp target, 0x080fa319
	overlay_veneer \target
	.endr
	.global Audio_InitializeRuntimeDefaultsFar
	.global Func_080f9008
	.thumb_func
Audio_InitializeRuntimeDefaultsFar:
Func_080f9008:
	.irp target, 0x080f9439, 0x080f9081, 0x080f91e9, 0x080f94c9
	overlay_veneer \target
	.endr
	.irp target, 0x080f94e1, 0x080f94f9, 0x080f950d, 0x080f9539
	overlay_veneer \target
	.endr
	.global AudioCommand_GetStateByteFar
	.global Func_080f9048
	.thumb_func
AudioCommand_GetStateByteFar:
Func_080f9048:
	.irp target, 0x080f954d, 0x080f92fd, 0x080f9559, 0x080f9565
	overlay_veneer \target
	.endr
	.irp target, 0x080f9571
	overlay_veneer \target
	.endr
	.global Func_080f9070
	.thumb_func
Func_080f9070:
	.irp target, 0x080f9595, 0x080f95a1
	overlay_veneer \target
	.endr
