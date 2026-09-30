.syntax unified
	.thumb
	.global Func_08108130
	.thumb_func
Func_08108130:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r0, [r3]
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #220
	adds r0, r0, r3
	bl Func_08108948
	pop {pc}
