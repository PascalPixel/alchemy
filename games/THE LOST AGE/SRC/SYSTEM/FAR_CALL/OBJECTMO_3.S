@ Far-call stub table at 0x080c88c8: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c88c8
	.thumb_func
Func_080c88c8:
	overlay_veneer Func_080d2d08
	.global Func_080c88d0
	.thumb_func
Func_080c88d0:
	overlay_veneer Func_080e2774
	.global Func_080c88d8
	.thumb_func
Func_080c88d8:
	overlay_veneer Func_080e7238
	.global Func_080c88e0
	.thumb_func
Func_080c88e0:
	overlay_veneer Func_080e7804
	.global Func_080c88e8
	.thumb_func
Func_080c88e8:
	overlay_veneer Func_080e74d8
	.global Func_080c88f0
	.thumb_func
Func_080c88f0:
	overlay_veneer Func_080d4aa8
	.global ObjectMotion_ResetTargetsAndVelocityFar
	.thumb_func
ObjectMotion_ResetTargetsAndVelocityFar:
	overlay_veneer ObjectMotion_ResetTargetsAndVelocity
	.global ObjectEffect_BeginContextEffect26Far
	.thumb_func
ObjectEffect_BeginContextEffect26Far:
	overlay_veneer ObjectEffect_BeginContextEffect26
	.global Func_080c8908
	.thumb_func
Func_080c8908:
	overlay_veneer Func_080d5e64
	.global Func_080c8910
	.thumb_func
Func_080c8910:
	overlay_veneer Func_080d6e58
	.global Func_080c8918
	.thumb_func
Func_080c8918:
	overlay_veneer Func_080d6f34
	.global Func_080c8920
	.thumb_func
Func_080c8920:
	overlay_veneer Func_080d7024
	.global Func_080c8928
	.thumb_func
Func_080c8928:
	overlay_veneer Func_080ea14c
	.global Func_080c8930
	.thumb_func
Func_080c8930:
	overlay_veneer Func_080d2d70
	.global Func_080c8938
	.thumb_func
Func_080c8938:
	overlay_veneer Func_080ca368
	.global Func_080c8940
	.thumb_func
Func_080c8940:
	overlay_veneer Func_080ca3f4
	.global Func_080c8948
	.thumb_func
Func_080c8948:
	overlay_veneer Func_080e28d4
