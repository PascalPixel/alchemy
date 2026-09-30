@ Far-call stubs: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_081c0080
	.thumb_func
Func_081c0080:
	overlay_veneer Func_081c1000
	.global Func_081c0088
	.thumb_func
Func_081c0088:
	overlay_veneer Func_081c0e30
	.global Func_081c0090
	.thumb_func
Func_081c0090:
	overlay_veneer Func_081c1188
	.global Func_081c0098
	.thumb_func
Func_081c0098:
	overlay_veneer Func_081c11ac
	.global Func_081c00a0
	.thumb_func
Func_081c00a0:
	overlay_veneer Func_081c11cc
	.global Func_081c00a8
	.thumb_func
Func_081c00a8:
	overlay_veneer Func_081c0c18
