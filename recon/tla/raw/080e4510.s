.syntax unified
	.thumb
	.global Func_080e4510
	.thumb_func
Func_080e4510:
	push {r5, r6, r7, lr}
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #224
	ldr r7, [r3]
	sub sp, #16
	movs r3, #128
	ldr r5, [r2, #108]
	lsls r3, r3, #24
	str r1, [sp, #0]
	movs r2, #30
	movs r1, #16
	adds r6, r0, #0
	bl Func_080eaf98
	ldr r0, [r7, #16]
	bl Func_080db9cc
	strh r0, [r6, #30]
	ldr r0, [r7, #16]
	bl Func_080db9c0
	ldrb r2, [r6, #9]
	movs r3, #3
	ands r0, r3
	movs r3, #13
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #5]
	movs r1, #32
	orrs r2, r1
	lsls r0, r0, #2
	orrs r3, r0
	strb r2, [r6, #5]
	movs r2, #15
	ands r3, r2
	movs r2, #197
	lsls r2, r2, #1
	strb r3, [r6, #9]
	adds r3, r5, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080e4574
	movs r3, #192
	lsls r3, r3, #8
	str r3, [r6, #20]
	b .L_080e457c
.L_080e4574:
	ldr r3, [r7, #16]
	ldr r2, [r3, #24]
	str r2, [r6, #20]
	ldr r3, [r3, #28]
.L_080e457c:
	str r3, [r6, #24]
	ldr r2, [r7, #16]
	add r5, sp, #4
	ldr r3, [r2, #8]
	adds r0, r5, #0
	str r3, [r5]
	ldr r3, [r2, #12]
	str r3, [r5, #4]
	ldr r3, [r2, #16]
	str r3, [r5, #8]
	bl Func_080dc390
	ldr r3, [r5]
	add sp, #16
	str r3, [r6, #12]
	ldr r3, [r5, #8]
	str r3, [r6, #16]
	pop {r5, r6, r7, pc}
