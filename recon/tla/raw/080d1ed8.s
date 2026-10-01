.syntax unified
	.thumb
	.global BattleFx_GetFlags
	.thumb_func
BattleFx_GetFlags:
	push {lr}
	bl BattleFx_GetAnimationValue
	bl Func_080d1e60
	ldrb r0, [r0, #3]
	pop {pc}
	.2byte 0x0000
