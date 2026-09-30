.syntax unified
	.thumb
	.global Func_080eb298
	.thumb_func
Func_080eb298:
	push {r5, r6, lr}
	adds r6, r0, #0
	sub sp, #12
	cmp r6, #0
	beq .L_080eb2c4
	ldr r3, [r1]
	mov r5, sp
	str r3, [r5]
	adds r0, r5, #0
	ldr r3, [r1, #4]
	str r3, [r5, #4]
	ldr r3, [r1, #8]
	str r3, [r5, #8]
	bl Func_080dc390
	ldr r3, [r5]
	adds r0, r6, #0
	str r3, [r6, #12]
	ldr r3, [r5, #8]
	str r3, [r6, #16]
	bl Func_080eb01c
.L_080eb2c4:
	add sp, #12
	pop {r5, r6, pc}
