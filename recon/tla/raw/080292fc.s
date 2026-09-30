.syntax unified
	.thumb
	.global Func_080292fc
	.thumb_func
Func_080292fc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r10, r0
	ldr r0, [r0, #80]
	sub sp, #36
	str r0, [sp, #16]
	movs r6, #192
	lsls r6, r6, #18
	adds r6, #156
	ldr r1, [r6]
	movs r2, #128
	lsls r2, r2, #9
	movs r3, #0
	str r1, [sp, #12]
	str r2, [sp, #0]
	str r3, [sp, #20]
	add r1, sp, #20
	movs r3, #128
	lsls r3, r3, #8
	mov r0, r10
	ldrb r1, [r1]
	str r3, [r0, #48]
	str r3, [r0, #52]
	mov r3, r10
	adds r3, #85
	strb r1, [r3]
	ldr r3, .L_08029564
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_08029568
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	movs r2, #255
	ldrh r1, [r1, r3]
	lsls r2, r2, #8
	adds r2, #255
	cmp r1, r2
	bne .L_0802935e
	ldr r3, [r0, #104]
	movs r0, #0
	mov r9, r3
	str r0, [sp, #4]
	b .L_0802940e
.L_0802935e:
	ldr r3, [sp, #20]
	add r5, sp, #24
	movs r2, #16
	movs r0, #128
	str r2, [sp, #4]
	lsls r0, r0, #12
	str r3, [r5]
	str r3, [r5, #4]
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	mov r0, r10
	ldr r2, [r0, #8]
	ldr r3, [r5]
	ldr r6, [r6]
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [r5, #8]
	ldr r3, [r0, #12]
	movs r1, #134
	subs r3, r3, r2
	str r3, [r5, #4]
	lsls r1, r1, #1
	ldr r3, [r0, #16]
	movs r2, #0
	str r3, [r5, #8]
	movs r3, #128
	lsls r3, r3, #21
	str r6, [sp, #12]
	adds r7, r6, r1
	mov r9, r2
	str r3, [sp, #8]
	cmp r6, #0
	beq .L_0802949e
	movs r1, #127
	mov r11, r1
.L_080293a8:
	movs r3, #18
	ldrsb r3, [r7, r3]
	cmp r3, #1
	bne .L_08029400
	ldr r1, [sp, #24]
	ldr r3, [r7, #4]
	ldr r5, [sp, #28]
	subs r1, r1, r3
	ldr r3, [r7, #8]
	ldr r6, [sp, #32]
	subs r5, r5, r3
	ldr r3, [r7, #12]
	asrs r1, r1, #8
	ldr r2, .L_0802956c
	adds r0, r1, #0
	subs r6, r6, r3
	mov lr, r2
	.2byte 0xf800
	asrs r5, r5, #8
	adds r1, r5, #0
	ldr r3, .L_0802956c
	mov r8, r0
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	asrs r6, r6, #8
	adds r5, r0, #0
	adds r1, r6, #0
	ldr r2, .L_0802956c
	adds r0, r6, #0
	mov lr, r2
	.2byte 0xf800
	add r8, r5
	mov r1, r8
	movs r2, #128
	adds r3, r1, r0
	lsls r2, r2, #3
	cmp r3, r2
	bgt .L_08029400
	ldr r0, [sp, #8]
	cmp r0, r3
	ble .L_08029400
	str r3, [sp, #8]
	mov r9, r7
.L_08029400:
	movs r1, #1
	negs r1, r1
	add r11, r1
	mov r2, r11
	adds r7, #32
	cmp r2, #0
	bge .L_080293a8
.L_0802940e:
	mov r3, r9
	cmp r3, #0
	bne .L_0802941e
	ldr r0, [sp, #20]
	movs r3, #1
	orrs r0, r3
	str r0, [sp, #20]
	b .L_080294da
.L_0802941e:
	mov r1, r9
	movs r3, #19
	ldrsb r3, [r1, r3]
	ldr r2, [sp, #12]
	lsls r3, r3, #5
	adds r3, r2, r3
	adds r6, r3, #0
	subs r6, #20
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl Object_GetByIdFar
	adds r5, r0, #0
	movs r1, #2
	ldrsh r0, [r6, r1]
	bl Object_GetByIdFar
	ldr r2, [r5, #8]
	ldr r3, [r0, #8]
	cmp r2, r3
	ble .L_0802944e
	ldr r2, [sp, #0]
	negs r2, r2
	str r2, [sp, #0]
.L_0802944e:
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl Object_GetByIdFar
	adds r5, r0, #0
	movs r1, #2
	ldrsh r0, [r6, r1]
	bl Object_GetByIdFar
	ldr r2, [r5, #12]
	ldr r3, [r0, #12]
	cmp r2, r3
	ble .L_08029474
	ldr r2, [sp, #0]
	movs r1, #192
	negs r2, r2
	str r2, [sp, #0]
	lsls r1, r1, #8
	b .L_08029478
.L_08029474:
	movs r1, #128
	lsls r1, r1, #7
.L_08029478:
	ldr r3, [sp, #0]
	mov r0, r10
	str r3, [r0, #24]
	mov r3, r9
	ldr r2, [r3, #4]
	ldr r3, [r0, #8]
	cmp r2, r3
	bne .L_080294a4
	mov r0, r9
	ldr r2, [r0, #8]
	mov r0, r10
	ldr r3, [r0, #12]
	cmp r2, r3
	bne .L_080294a4
	mov r3, r9
	ldr r2, [r3, #12]
	ldr r3, [r0, #16]
	cmp r2, r3
	bne .L_080294a4
.L_0802949e:
	movs r0, #1
	str r0, [sp, #20]
	b .L_080294da
.L_080294a4:
	movs r3, #1
	str r3, [r6, #24]
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #10
	str r3, [r6, #12]
	cmp r3, r2
	ble .L_080294bc
	str r2, [r6, #12]
.L_080294bc:
	mov r3, r9
	mov r0, r10
	str r3, [r0, #104]
	ldr r3, [r3, #4]
	add r2, sp, #24
	str r3, [r2]
	mov r0, r9
	ldr r3, [r0, #8]
	str r3, [r2, #4]
	ldr r3, [r0, #12]
	str r3, [r2, #8]
	ldrh r3, [r0, #16]
	adds r3, r3, r1
	ldr r1, [sp, #16]
	strh r3, [r1, #18]
.L_080294da:
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	cmp r1, #0
	beq .L_08029510
	ldr r3, [sp, #20]
	movs r2, #3
	ands r2, r3
	cmp r2, #0
	beq .L_080294fc
	movs r0, #194
	lsls r0, r0, #1
	adds r2, r1, r0
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_08029504
.L_080294fc:
	movs r0, #194
	lsls r0, r0, #1
	adds r3, r1, r0
	strh r2, [r3]
.L_08029504:
	ldr r3, .L_08029564
	movs r0, #195
	ldr r3, [r3]
	lsls r0, r0, #1
	adds r2, r1, r0
	strh r3, [r2]
.L_08029510:
	mov r0, r10
	movs r1, #29
	bl ObjectDispatch_ApplyArgumentToChildren
	mov r0, r10
	ldr r1, [sp, #4]
	bl ObjectDispatch_ApplyValueToChildren
	ldr r1, [sp, #20]
	cmp r1, #0
	beq .L_0802953c
	movs r3, #128
	mov r2, r10
	lsls r3, r3, #24
	str r3, [r2, #56]
	str r3, [r2, #60]
	str r3, [r2, #64]
	movs r3, #0
	str r3, [r2, #36]
	str r3, [r2, #40]
	str r3, [r2, #44]
	b .L_0802954a
.L_0802953c:
	add r3, sp, #24
	ldr r1, [r3]
	ldr r2, [r3, #4]
	mov r0, r10
	ldr r3, [r3, #8]
	bl Object_SetMoveTarget
.L_0802954a:
	mov r0, r10
	ldrh r3, [r0, #4]
	mov r1, r10
	adds r3, #1
	movs r0, #1
	strh r3, [r1, #4]
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08029564:
	.4byte gInput
.L_08029568:
	.4byte Data_0802ec5c
.L_0802956c:
	.4byte IwramMulQ16
