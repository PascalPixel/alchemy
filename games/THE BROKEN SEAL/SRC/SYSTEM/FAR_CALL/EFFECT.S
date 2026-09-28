@ Far-call stub table heading the battle effect code at 0x080c9000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c9000
	.thumb_func
Func_080c9000:
	overlay_veneer 0x080ed409
	.global BattleFx_DispatchByIdRangeFar
	.thumb_func
BattleFx_DispatchByIdRangeFar:
	overlay_veneer 0x080e3a3d
	.global BattleFx_InitializeModeFar
	.thumb_func
BattleFx_InitializeModeFar:
	overlay_veneer 0x080d6579
	overlay_veneer 0x080d6661
	overlay_veneer 0x080cb7f9
	overlay_veneer 0x080cbc0d
	overlay_veneer 0x080cc5d9
	overlay_veneer 0x080ccaed
	overlay_veneer 0x080ccbdd
