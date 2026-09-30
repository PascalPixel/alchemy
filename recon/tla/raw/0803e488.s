.syntax unified
	.thumb
	.global Func_0803e488
	.thumb_func
Func_0803e488:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0803e4ec
	mov r8, r1
	ldr r3, [r3]
	adds r6, r0, #0
	lsrs r4, r3, #2
	movs r3, #7
	ands r4, r3
	movs r3, #52
	mov r2, r8
	muls r2, r3
	adds r3, r2, r6
	ldrh r3, [r3, #10]
	cmp r3, #0
	bne .L_0803e4ac
	b .L_0803e5a0
.L_0803e4ac:
	movs r0, #128
	adds r3, r6, r2
	lsls r0, r0, #1
	adds r2, #16
	ldrh r1, [r6, r2]
	adds r0, #255
	adds r5, r3, #0
	mov r12, r0
	adds r5, #40
	mov r3, r12
	ldr r7, .L_0803e4f0
	ands r3, r1
	ldrh r1, [r5, #6]
	adds r0, r7, #0
	ands r0, r1
	orrs r0, r3
	strh r0, [r5, #6]
	adds r2, r6, r2
	ldrh r3, [r2, #2]
	mov r1, r8
	strb r3, [r5, #4]
	cmp r1, #0
	beq .L_0803e4f8
	ldrh r2, [r6, #60]
	ldr r1, .L_0803e4f4
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0803e512
	lsls r3, r0, #23
	lsrs r3, r3, #23
	adds r3, r3, r2
	b .L_0803e508
.L_0803e4ec:
	.4byte gFrameTick
.L_0803e4f0:
	.4byte 0xfffffe00
.L_0803e4f4:
	.4byte Data_0805c5c4
.L_0803e4f8:
	ldrh r2, [r6, #8]
	ldr r1, .L_0803e558
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0803e512
	lsls r3, r0, #23
	lsrs r3, r3, #23
	subs r3, r3, r2
.L_0803e508:
	mov r2, r12
	ands r3, r2
	ands r0, r7
	orrs r0, r3
	strh r0, [r5, #6]
.L_0803e512:
	movs r3, #52
	mov r0, r8
	muls r0, r3
	adds r3, r0, #0
	adds r3, #12
	lsls r2, r4, #7
	adds r2, r1, r2
	ldrh r0, [r6, r3]
	movs r1, #128
	bl VramBlock_LoadCached
	ldr r3, .L_0803e554
	ldrh r2, [r5, #8]
	ands r0, r3
	ldr r3, .L_0803e55c
	ands r3, r2
	orrs r3, r0
	movs r0, #4
	strh r3, [r5, #8]
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0803e57a
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #226
	adds r3, r6, r1
	ldrh r3, [r3]
	cmp r3, #1
	bne .L_0803e570
	b .L_0803e560
	.2byte 0x0000
.L_0803e554:
	.4byte 0x000003ff
.L_0803e558:
	.4byte Data_0805c1c4
.L_0803e55c:
	.4byte 0xfffffc00
.L_0803e560:
	ldrb r3, [r5, #5]
	movs r2, #13
	negs r2, r2
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	strb r2, [r5, #5]
	b .L_0803e57a
.L_0803e570:
	ldrb r2, [r5, #5]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	strb r3, [r5, #5]
.L_0803e57a:
	movs r1, #238
	adds r0, r5, #0
	bl Func_08014128
	movs r3, #52
	mov r2, r8
	muls r2, r3
	adds r3, r2, #0
	adds r1, r3, #0
	adds r1, #8
	ldrh r2, [r6, r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0803e5a0
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r2, r0
	strh r3, [r6, r1]
.L_0803e5a0:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
