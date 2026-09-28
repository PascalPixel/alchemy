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
	overlay_veneer 0x080fa319
	.global Audio_InitializeRuntimeDefaultsFar
	.global Func_080f9008
	.thumb_func
Audio_InitializeRuntimeDefaultsFar:
Func_080f9008:
	overlay_veneer 0x080f9439
	.global Audio_PlayCue
	.thumb_func
Audio_PlayCue:
	.global Func_080f9010
	.thumb_func
Func_080f9010:
	overlay_veneer 0x080f9081
	.global Func_080f9018
	.thumb_func
Func_080f9018:
	overlay_veneer 0x080f91e9
	overlay_veneer 0x080f94c9
	overlay_veneer 0x080f94e1
	overlay_veneer 0x080f94f9
	overlay_veneer 0x080f950d
	overlay_veneer 0x080f9539
	.global AudioCommand_GetStateByteFar
	.global Func_080f9048
	.thumb_func
AudioCommand_GetStateByteFar:
Func_080f9048:
	.global Audio_Check
	.thumb_func
Audio_Check:
	overlay_veneer 0x080f954d
	overlay_veneer 0x080f92fd
	overlay_veneer 0x080f9559
	overlay_veneer 0x080f9565
	overlay_veneer 0x080f9571
	.global Func_080f9070
	.thumb_func
Func_080f9070:
	overlay_veneer 0x080f9595
	overlay_veneer 0x080f95a1
