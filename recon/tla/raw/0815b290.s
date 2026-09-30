.syntax unified
	.thumb
	.global Func_0815b290
	.thumb_func
Func_0815b290:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	str r1, [sp, #0]
	movs r1, #0
	mov r10, r2
	adds r7, r3, #0
	mov r11, r0
	mov r8, r1
	bl Resource_FindFreeEntry
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #16]
	adds r5, r0, #0
	ldrb r3, [r6, #20]
	movs r2, #0
	b .L_0815b2c8
.L_0815b2be:
	adds r2, #1
	adds r6, #56
	cmp r2, #63
	bgt .L_0815b2ce
	ldrb r3, [r6, #20]
.L_0815b2c8:
	cmp r3, #0
	bne .L_0815b2be
	mov r8, r6
.L_0815b2ce:
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	bl VramBlock_LoadCached
	mov r2, r8
	strb r5, [r2, #16]
	ldr r5, .L_0815b31c
	mov r3, r8
	ldrb r2, [r3, #17]
	movs r1, #0
	strh r1, [r3, #18]
	strb r5, [r3, #26]
	movs r3, #2
	negs r3, r3
	ands r3, r2
	mov r2, r8
	strb r3, [r2, #17]
	mov r3, r8
	stmia r3!, {r1}
	mov r2, r10
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #4
	orrs r7, r0
	orrs r7, r2
	str r7, [r3]
	mov r3, r8
	adds r3, #28
	str r1, [r3]
	movs r3, #192
	mov r1, r8
	lsls r3, r3, #7
	str r3, [r1, #32]
	ldr r3, .L_0815b320
	mov r9, r3
	movs r3, #187
	b .L_0815b324
	.2byte 0x0000
.L_0815b31c:
	.4byte 0x00000000
.L_0815b320:
	.4byte ResourceTableEntries
.L_0815b324:
	lsls r3, r3, #1
	add r3, r9
	ldrh r3, [r3]
	lsrs r3, r3, #5
	orrs r3, r2
	str r3, [r1, #36]
	bl Func_0815b24c
	mov r2, sp
	ldrb r2, [r2]
	movs r3, #128
	lsls r3, r3, #9
	mov r1, r11
	str r3, [r6, #12]
	movs r3, #1
	str r0, [r6, #40]
	strb r1, [r6, #20]
	strb r2, [r6, #21]
	strb r5, [r6, #23]
	strb r5, [r6, #22]
	strb r3, [r6, #27]
	strb r5, [r6, #26]
	movs r1, #0
	adds r0, r6, #0
	bl Animation_ApplyChildArgumentFar
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #24]
	ldrb r2, [r6, #20]
	ldrb r3, [r6, #21]
	mov r1, r10
	adds r5, r3, #0
	muls r5, r2
	movs r3, #128
	lsls r3, r3, #6
	ands r3, r1
	cmp r3, #0
	bne .L_0815b374
	lsrs r5, r5, #1
.L_0815b374:
	adds r1, r5, #0
	ldrb r0, [r6, #16]
	movs r2, #0
	bl VramBlock_LoadCached
	ldrh r3, [r7]
	ldrh r1, [r6, #8]
	adds r3, r3, r5
	strh r3, [r7]
	ldrb r3, [r6, #16]
	mov r0, r8
	lsls r3, r3, #2
	add r3, r9
	ldrh r2, [r3, #2]
	ldr r3, .L_0815b3ac
	lsls r2, r2, #17
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r6, #8]
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0815b3ac:
	.4byte 0xfffffc00
