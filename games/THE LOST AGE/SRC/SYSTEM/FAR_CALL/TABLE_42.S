@ Far-call stub table at 0x08108088: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global VramBlock_LoadResourceFar
	.thumb_func
VramBlock_LoadResourceFar:
	overlay_veneer VramBlock_LoadResource
	.global Func_08108090
	.thumb_func
Func_08108090:
	overlay_veneer Func_0810a804
	.global Func_08108098
	.thumb_func
Func_08108098:
	overlay_veneer Func_0810a834
	.global Func_081080a0
	.thumb_func
Func_081080a0:
	overlay_veneer Func_0810a84c
