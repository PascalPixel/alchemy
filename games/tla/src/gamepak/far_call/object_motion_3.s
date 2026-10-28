.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Func_080c88c8
	.thumb_func
Func_080c88c8:
	ldr r4, [pc, #0]
	bx r4
	.4byte Func_080d2d08
	.global Func_080c88d0
	.thumb_func
Func_080c88d0:
	overlay_veneer Func_080e2774
	.global Func_080c88d8
	.thumb_func
Func_080c88d8:
	ldr r4, [pc, #0]
	bx r4
	.4byte Func_080e7238
	.global Func_080c88e0
	.thumb_func
Func_080c88e0:
	overlay_veneer Func_080e7804
	.global Func_080c88e8
	.thumb_func
Func_080c88e8:
	ldr r4, [pc, #0]
	bx r4
	.4byte Func_080e74d8
	.global Func_080c88f0
	.thumb_func
Func_080c88f0:
	overlay_veneer Func_080d4aa8
	.global Actor_ResetTargetsAndVelocityFar
	.thumb_func
Actor_ResetTargetsAndVelocityFar:
	overlay_veneer Actor_ResetTargetsAndVelocity
	.global ObjectEffect_StartContextEffect26Far
	.thumb_func
ObjectEffect_StartContextEffect26Far:
	overlay_veneer ObjectEffect_StartContextEffect26
	.global ObjectEffect_StartContextEffect25Far
	.thumb_func
ObjectEffect_StartContextEffect25Far:
	overlay_veneer ObjectEffect_StartContextEffect25
	.global Func_080c8910
	.thumb_func
Func_080c8910:
	overlay_veneer FieldMotes_Start
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
