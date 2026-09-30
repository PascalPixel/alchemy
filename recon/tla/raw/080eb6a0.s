.syntax unified
	.thumb
	.global Func_080eb6a0
	.thumb_func
Func_080eb6a0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #240
	ldr r5, [r3]
	sub sp, #4
	adds r3, r5, #0
	adds r3, #168
	ldr r3, [r3]
	lsls r3, r3, #16
	mov r9, r3
	adds r3, r5, #0
	adds r3, #172
	ldr r3, [r3]
	lsls r3, r3, #16
	mov r10, r3
	adds r3, r5, #0
	adds r3, #176
	ldr r3, [r3]
	mov r8, r3
	adds r3, r5, #0
	adds r3, #180
	mov r1, r8
	ldr r7, [r3]
	cmp r1, #0
	bne .L_080eb6de
	b .L_080eb80c
.L_080eb6de:
	cmp r7, #0
	bne .L_080eb6e4
	b .L_080eb80c
.L_080eb6e4:
	adds r6, r5, #0
	adds r6, #192
	movs r3, #0
	ldrsb r3, [r6, r3]
	ldrb r2, [r6]
	cmp r3, #1
	bne .L_080eb75a
	adds r2, r5, #0
	adds r2, #188
	ldrh r2, [r2]
	movs r3, #192
	str r2, [sp, #0]
	lsls r3, r3, #24
	adds r0, r5, #0
	movs r1, #32
	movs r2, #32
	bl Func_080eaf98
	ldrb r2, [r5, #5]
	movs r3, #33
	negs r3, r3
	ands r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r5, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #224
	orrs r3, r2
	strb r3, [r5, #9]
	adds r2, r5, #0
	adds r2, #191
	ldrb r2, [r2]
	movs r1, #3
	ands r2, r1
	movs r1, #13
	negs r1, r1
	lsls r2, r2, #2
	ands r3, r1
	orrs r3, r2
	strb r3, [r5, #9]
	adds r3, r5, #0
	adds r3, #190
	ldrb r3, [r3]
	mov r2, r9
	strh r3, [r5, #30]
	mov r1, r8
	mov r3, r10
	str r2, [r5, #12]
	str r3, [r5, #16]
	str r1, [r5, #20]
	str r7, [r5, #24]
	adds r3, r5, #0
	adds r3, #184
	ldr r3, [r3]
	adds r0, r5, #0
	strh r3, [r5, #28]
	bl Func_080eb01c
	ldrb r2, [r6]
.L_080eb75a:
	movs r1, #128
	lsls r3, r2, #24
	lsls r1, r1, #18
	cmp r3, r1
	bne .L_080eb7c6
	adds r2, r5, #0
	adds r2, #188
	ldrh r2, [r2]
	movs r3, #192
	str r2, [sp, #0]
	lsls r3, r3, #24
	adds r0, r5, #0
	movs r1, #32
	movs r2, #32
	bl Func_080eaf98
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	strb r3, [r5, #5]
	ldrb r3, [r5, #9]
	movs r1, #15
	ands r1, r3
	strb r1, [r5, #9]
	adds r2, r5, #0
	adds r2, #191
	ldrb r2, [r2]
	movs r3, #3
	ands r2, r3
	movs r3, #13
	negs r3, r3
	lsls r2, r2, #2
	ands r3, r1
	orrs r3, r2
	strb r3, [r5, #9]
	adds r3, r5, #0
	adds r3, #190
	ldrb r3, [r3]
	mov r2, r9
	strh r3, [r5, #30]
	mov r1, r8
	mov r3, r10
	str r2, [r5, #12]
	str r3, [r5, #16]
	str r1, [r5, #20]
	str r7, [r5, #24]
	adds r3, r5, #0
	adds r3, #184
	ldr r3, [r3]
	adds r0, r5, #0
	strh r3, [r5, #28]
	bl Func_080eb01c
	ldrb r2, [r6]
.L_080eb7c6:
	lsls r3, r2, #24
	movs r2, #192
	lsls r2, r2, #18
	cmp r3, r2
	bne .L_080eb80c
	adds r2, r5, #0
	adds r2, #191
	ldrb r2, [r2]
	ldrb r1, [r5, #9]
	movs r3, #3
	ands r2, r3
	movs r3, #13
	negs r3, r3
	lsls r2, r2, #2
	ands r3, r1
	orrs r3, r2
	strb r3, [r5, #9]
	adds r3, r5, #0
	adds r3, #190
	ldrb r3, [r3]
	mov r1, r10
	strh r3, [r5, #30]
	mov r2, r8
	mov r3, r9
	str r3, [r5, #12]
	str r1, [r5, #16]
	str r2, [r5, #20]
	str r7, [r5, #24]
	adds r3, r5, #0
	adds r3, #184
	ldr r3, [r3]
	adds r0, r5, #0
	strh r3, [r5, #28]
	bl Func_080eb01c
.L_080eb80c:
	adds r2, r5, #0
	adds r2, #160
	ldr r3, [r2]
	add sp, #4
	adds r3, #1
	str r3, [r2]
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
