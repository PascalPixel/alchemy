.syntax unified
	.thumb
	.global Func_0815b3b0
	.thumb_func
Func_0815b3b0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #16]
	adds r7, r0, #0
	ldrb r3, [r5, #20]
	mov r8, r1
	movs r6, #0
	movs r2, #0
	b .L_0815b3d2
.L_0815b3c8:
	adds r2, #1
	adds r5, #56
	cmp r2, #63
	bgt .L_0815b3d8
	ldrb r3, [r5, #20]
.L_0815b3d2:
	cmp r3, #0
	bne .L_0815b3c8
	adds r6, r5, #0
.L_0815b3d8:
	bl Func_0815b24c
	mov r3, r8
	strb r3, [r5, #21]
	movs r3, #128
	ldrb r2, [r5, #17]
	lsls r3, r3, #9
	str r3, [r5, #12]
	movs r3, #2
	negs r3, r3
	ands r3, r2
	movs r1, #0
	strb r3, [r5, #17]
	movs r3, #1
	str r0, [r5, #40]
	strb r7, [r5, #20]
	strb r1, [r5, #23]
	strb r1, [r5, #22]
	strb r3, [r5, #27]
	strb r1, [r5, #26]
	strh r1, [r5, #18]
	adds r0, r5, #0
	bl Animation_ApplyChildArgumentFar
	adds r0, r6, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
