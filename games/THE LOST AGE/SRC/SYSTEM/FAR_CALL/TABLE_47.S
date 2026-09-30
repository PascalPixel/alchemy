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
	.global Func_08138008
	.thumb_func
Func_08138008:
	overlay_veneer Func_0815e320
	.global BattleFx_InitializeModeFar
	.thumb_func
BattleFx_InitializeModeFar:
	overlay_veneer Func_0814c994
	.global Func_08138018
	.thumb_func
Func_08138018:
	overlay_veneer Func_0814cb60
	.global Func_08138020
	.thumb_func
Func_08138020:
	overlay_veneer Func_0813e3d0
	.global Func_08138028
	.thumb_func
Func_08138028:
	overlay_veneer Func_0813eb70
	.global Func_08138030
	.thumb_func
Func_08138030:
	overlay_veneer Func_0813f518
	.global Func_08138038
	.thumb_func
Func_08138038:
	overlay_veneer Func_0813fa88
	.global Func_08138040
	.thumb_func
Func_08138040:
	overlay_veneer Func_0813fd40
	.global Func_08138048
	.thumb_func
Func_08138048:
	overlay_veneer Func_081963ec
	.global Func_08138050
	.thumb_func
Func_08138050:
	overlay_veneer Func_08142944
