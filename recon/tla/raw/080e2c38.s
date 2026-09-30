.syntax unified
	.thumb
	.global Func_080e2c38
	.thumb_func
Func_080e2c38:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #92]
	movs r0, #216
	lsls r0, r0, #5
	mov r8, r3
	movs r5, #132
	adds r0, #2
	movs r7, #192
	sub sp, #12
	lsls r5, r5, #5
	add r0, r8
	lsls r7, r7, #1
	movs r3, #95
	add r5, r8
	mov r9, r0
	mov r6, sp
	add r7, r8
	mov r10, r3
.L_080e2c68:
	ldr r0, [r5, #24]
	cmp r0, #63
	bhi .L_080e2ce2
	movs r1, #6
	asrs r0, r0, #3
	bl __modsi3
	mov r3, r9
	ldrh r1, [r3]
	ldr r3, .L_080e2cb4
	lsls r0, r0, #1
	adds r1, r1, r0
	ands r1, r3
	ldr r2, .L_080e2cb8
	ldrh r3, [r7, #8]
	movs r0, #192
	ands r3, r2
	orrs r3, r1
	strh r3, [r7, #8]
	adds r2, r6, #0
	ldr r3, [r5]
	lsls r0, r0, #14
	str r3, [r6]
	ldr r3, [r5, #4]
	str r3, [r6, #4]
	ldr r3, [r5, #8]
	str r3, [r6, #8]
	ldr r1, [r5, #12]
	bl Func_0801489c
	adds r0, r6, #0
	bl Func_080dc390
	ldr r3, [r6]
	adds r0, r7, #0
	str r3, [r7, #12]
	ldr r3, [r6, #8]
	b .L_080e2cbc
.L_080e2cb4:
	.4byte 0x000003ff
.L_080e2cb8:
	.4byte 0xfffffc00
.L_080e2cbc:
	str r3, [r7, #16]
	bl Func_080eb01c
	ldr r3, [r5, #12]
	ldr r1, [r5, #20]
	ldr r2, [r5, #16]
	adds r3, r3, r1
	str r3, [r5, #12]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
	adds r3, r1, #0
	adds r3, #8
	cmp r1, #0
	bge .L_080e2cde
	adds r3, r1, #0
	subs r3, #8
.L_080e2cde:
	str r3, [r5, #20]
	ldr r0, [r5, #24]
.L_080e2ce2:
	adds r3, r0, #1
	movs r0, #1
	negs r0, r0
	add r10, r0
	str r3, [r5, #24]
	mov r3, r10
	adds r7, #40
	adds r5, #28
	cmp r3, #0
	bge .L_080e2c68
	movs r0, #216
	lsls r0, r0, #5
	adds r0, #4
	add r0, r8
	movs r4, #0
	movs r1, #216
	strh r4, [r0]
	lsls r1, r1, #5
	adds r1, #6
	add r1, r8
	movs r5, #0
	ldrsh r3, [r1, r5]
	ldrh r2, [r1]
	cmp r3, #30
	bne .L_080e2d1a
	movs r3, #1
	strh r3, [r0]
	ldrh r2, [r1]
.L_080e2d1a:
	movs r0, #224
	lsls r3, r2, #16
	lsls r0, r0, #15
	cmp r3, r0
	bne .L_080e2d30
	movs r3, #216
	lsls r3, r3, #5
	adds r3, #8
	add r3, r8
	strh r4, [r3]
	ldrh r2, [r1]
.L_080e2d30:
	adds r3, r2, #1
	strh r3, [r1]
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
