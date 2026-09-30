@ Far-call stub table at 0x08108030: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08108030
	.thumb_func
Func_08108030:
	overlay_veneer Func_08108948
	overlay_veneer Func_081089e4
	.global Func_08108040
	.thumb_func
Func_08108040:
	overlay_veneer Func_08108a88
	.global Func_08108048
	.thumb_func
Func_08108048:
	overlay_veneer Func_08108aa8
	overlay_veneer Func_0810875c
	overlay_veneer Func_081088d8
	overlay_veneer Func_08108928
	.global Func_08108068
	.thumb_func
Func_08108068:
	overlay_veneer Func_0810b7a8
	.global Func_08108070
	.thumb_func
Func_08108070:
	overlay_veneer Func_0810b79c
	.global Func_08108078
	.thumb_func
Func_08108078:
	overlay_veneer Func_081084e0
	.global Func_08108080
	.thumb_func
Func_08108080:
	overlay_veneer Func_0810b520
