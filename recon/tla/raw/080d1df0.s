.syntax unified
	.thumb
	.global Func_080d1df0
	.thumb_func
Func_080d1df0:
	push {r5, r6, lr}
	sub sp, #12
	mov r5, sp
	adds r6, r0, #0
	adds r0, r5, #0
	str r1, [r5]
	str r2, [r5, #4]
	str r3, [r5, #8]
	bl Camera_WorldToScreen
	ldr r3, [r5]
	adds r0, r6, #0
	str r3, [r6, #12]
	ldr r3, [r5, #8]
	str r3, [r6, #16]
	bl Func_080eb01c
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
