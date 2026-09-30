.syntax unified
	.thumb
	.global Func_080db974
	.thumb_func
Func_080db974:
	push {r5, r6, lr}
	adds r6, r0, #0
	sub sp, #12
	adds r0, r1, #0
	adds r1, r2, #0
	cmp r6, #0
	beq .L_080db9a2
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	ldr r3, [r6, #12]
	adds r2, r5, #0
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Func_0801489c
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	adds r0, r6, #0
	bl Object_SetPosition
.L_080db9a2:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
