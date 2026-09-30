@ Far-call stub table at 0x08138000: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Resource_FarCall00C
Resource_FarCall00C:
	.global Func_08138000
	.thumb_func
Func_08138000:
	overlay_veneer Func_08196404
	overlay_veneer Func_0815e320
	overlay_veneer Func_0814c994
	overlay_veneer Func_0814cb60
	overlay_veneer Func_0813e3d0
	overlay_veneer Func_0813eb70
	overlay_veneer Func_0813f518
	overlay_veneer Func_0813fa88
	overlay_veneer Func_0813fd40
	overlay_veneer Func_081963ec
	overlay_veneer Func_08142944
