.syntax unified
	.thumb
	.global Func_08029570
	.thumb_func
Func_08029570:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r4, r0, #0
	ldr r0, [r4, #80]
	movs r3, #128
	sub sp, #40
	movs r2, #0
	lsls r3, r3, #8
	movs r1, #1
	str r1, [sp, #16]
	str r2, [sp, #24]
	str r0, [sp, #20]
	str r3, [r4, #48]
	str r3, [r4, #52]
	adds r3, r4, #0
	adds r3, #85
	strb r2, [r3]
	ldr r3, .L_08029800
	ldr r1, .L_08029804
	ldr r3, [r3]
	movs r2, #15
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r3, [r1, r3]
	lsls r3, r3, #16
	str r3, [sp, #4]
	lsrs r1, r3, #16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	cmp r1, r3
	bne .L_080295d8
	movs r0, #4
	str r0, [sp, #24]
	movs r2, #28
	ldr r3, [r4, #8]
	add r2, sp
	ldr r1, [r4, #104]
	str r3, [r2]
	mov r10, r1
	ldr r3, [r1, #8]
	mov r9, r2
	str r3, [r2, #4]
	ldr r3, [r4, #16]
	str r3, [r2, #8]
	b .L_080296fa
.L_080295d8:
	add r3, sp, #28
	mov r9, r3
	ldr r3, [r4, #8]
	mov r0, r9
	str r3, [r0]
	mov r2, r9
	ldr r3, [r4, #12]
	str r4, [sp, #0]
	str r3, [r0, #4]
	ldr r3, [r4, #16]
	str r3, [r0, #8]
	movs r0, #128
	lsls r0, r0, #12
	bl Vector_AddPolarOffset
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #160
	ldr r3, [r3]
	movs r2, #128
	adds r7, r3, #0
	movs r1, #0
	lsls r2, r2, #21
	str r3, [sp, #12]
	adds r7, #236
	mov r10, r1
	str r2, [sp, #8]
	ldr r4, [sp, #0]
	cmp r3, #0
	bne .L_0802961a
	movs r3, #1
	str r3, [sp, #24]
	b .L_080296fa
.L_0802961a:
	movs r0, #127
	mov r11, r0
.L_0802961e:
	movs r3, #16
	ldrsb r3, [r7, r3]
	cmp r3, #1
	bne .L_0802967a
	ldr r1, [sp, #28]
	ldr r3, [r7, #4]
	ldr r5, [sp, #32]
	subs r1, r1, r3
	ldr r3, [r7, #8]
	ldr r6, [sp, #36]
	subs r5, r5, r3
	ldr r3, [r7, #12]
	asrs r1, r1, #8
	str r4, [sp, #0]
	ldr r2, .L_08029808
	adds r0, r1, #0
	subs r6, r6, r3
	mov lr, r2
	.2byte 0xf800
	asrs r5, r5, #8
	adds r1, r5, #0
	ldr r3, .L_08029808
	mov r8, r0
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	asrs r6, r6, #8
	adds r5, r0, #0
	adds r1, r6, #0
	ldr r2, .L_08029808
	adds r0, r6, #0
	mov lr, r2
	.2byte 0xf800
	add r8, r5
	mov r1, r8
	movs r2, #128
	adds r3, r1, r0
	lsls r2, r2, #3
	ldr r4, [sp, #0]
	cmp r3, r2
	bgt .L_0802967a
	ldr r0, [sp, #8]
	cmp r0, r3
	ble .L_0802967a
	str r3, [sp, #8]
	mov r10, r7
.L_0802967a:
	movs r1, #1
	negs r1, r1
	add r11, r1
	mov r2, r11
	adds r7, #32
	cmp r2, #0
	bge .L_0802961e
	mov r3, r10
	cmp r3, #0
	bne .L_08029698
	ldr r0, [sp, #24]
	movs r3, #1
	orrs r0, r3
	str r0, [sp, #24]
	b .L_080296fa
.L_08029698:
	mov r1, r10
	ldr r2, [r1, #4]
	ldr r3, [r4, #8]
	cmp r2, r3
	bne .L_080296b8
	ldr r2, [r1, #8]
	ldr r3, [r4, #12]
	cmp r2, r3
	bne .L_080296b8
	ldr r2, [r1, #12]
	ldr r3, [r4, #16]
	cmp r2, r3
	bne .L_080296b8
	movs r2, #1
	str r2, [sp, #24]
	b .L_080296fa
.L_080296b8:
	mov r3, r10
	movs r2, #17
	ldrsb r2, [r3, r2]
	cmp r2, #0
	beq .L_080296ec
	ldr r0, [sp, #12]
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, r0, r3
	adds r2, r3, #0
	subs r2, #16
	movs r3, #1
	str r3, [r2, #20]
	ldr r3, [r2, #8]
	movs r1, #128
	lsls r1, r1, #8
	adds r3, r3, r1
	movs r1, #128
	lsls r1, r1, #10
	str r3, [r2, #8]
	cmp r3, r1
	ble .L_080296e8
	str r1, [r2, #8]
.L_080296e8:
	mov r2, r10
	str r2, [r4, #104]
.L_080296ec:
	mov r0, r10
	ldr r3, [r0, #4]
	str r3, [sp, #28]
	ldr r3, [r0, #8]
	str r3, [sp, #32]
	ldr r3, [r0, #12]
	str r3, [sp, #36]
.L_080296fa:
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	cmp r1, #0
	beq .L_08029730
	ldr r3, [sp, #24]
	movs r2, #3
	ands r2, r3
	cmp r2, #0
	beq .L_0802971c
	movs r0, #194
	lsls r0, r0, #1
	adds r2, r1, r0
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_08029724
.L_0802971c:
	movs r0, #194
	lsls r0, r0, #1
	adds r3, r1, r0
	strh r2, [r3]
.L_08029724:
	ldr r3, .L_08029800
	movs r0, #195
	ldr r3, [r3]
	lsls r0, r0, #1
	adds r2, r1, r0
	strh r3, [r2]
.L_08029730:
	ldr r1, [sp, #24]
	cmp r1, #0
	bne .L_0802973a
	movs r2, #2
	str r2, [sp, #16]
.L_0802973a:
	adds r0, r4, #0
	ldr r1, [sp, #16]
	str r4, [sp, #0]
	bl ObjectDispatch_ApplyArgumentToChildren
	ldr r4, [sp, #0]
	movs r1, #8
	adds r0, r4, #0
	bl ObjectDispatch_ApplyValueToChildren
	ldr r0, [sp, #20]
	movs r3, #4
	strb r3, [r0, #23]
	ldr r1, [sp, #24]
	movs r2, #0
	ldr r4, [sp, #0]
	cmp r1, #0
	beq .L_080297a0
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r4, #56]
	str r3, [r4, #60]
	str r3, [r4, #64]
	movs r3, #3
	ands r3, r1
	str r2, [r4, #36]
	str r2, [r4, #40]
	str r2, [r4, #44]
	cmp r3, #0
	beq .L_08029798
	ldr r2, [sp, #4]
	ldrh r1, [r4, #6]
	lsrs r3, r2, #16
	subs r3, r3, r1
	lsls r3, r3, #16
	movs r2, #128
	asrs r3, r3, #16
	lsls r2, r2, #5
	cmp r3, r2
	ble .L_0802978c
	adds r3, r2, #0
.L_0802978c:
	ldr r2, .L_0802980c
	cmp r3, r2
	bge .L_08029794
	adds r3, r2, #0
.L_08029794:
	adds r3, r1, r3
	strh r3, [r4, #6]
.L_08029798:
	mov r0, r9
	ldr r3, [r0, #4]
	str r3, [r4, #12]
	b .L_080297ea
.L_080297a0:
	mov r1, r9
	ldr r2, [r1, #4]
	mov r0, r9
	str r2, [r4, #12]
	str r4, [sp, #0]
	ldr r3, [r0, #8]
	ldr r1, [r1]
	adds r0, r4, #0
	bl Object_SetMoveTarget
	ldr r4, [sp, #0]
	mov r1, r9
	ldr r2, [r4, #8]
	ldr r3, [r1]
	cmp r2, r3
	bne .L_080297ea
	ldr r2, [r4, #16]
	ldr r3, [r1, #8]
	cmp r2, r3
	bne .L_080297ea
	ldr r2, [sp, #4]
	ldrh r1, [r4, #6]
	lsrs r3, r2, #16
	subs r3, r3, r1
	lsls r3, r3, #16
	movs r2, #128
	asrs r3, r3, #16
	lsls r2, r2, #5
	cmp r3, r2
	ble .L_080297de
	adds r3, r2, #0
.L_080297de:
	ldr r2, .L_0802980c
	cmp r3, r2
	bge .L_080297e6
	adds r3, r2, #0
.L_080297e6:
	adds r3, r1, r3
	strh r3, [r4, #6]
.L_080297ea:
	ldrh r3, [r4, #4]
	movs r0, #1
	adds r3, #1
	strh r3, [r4, #4]
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08029800:
	.4byte gInput
.L_08029804:
	.4byte Data_0802ec5c
.L_08029808:
	.4byte IwramMulQ16
.L_0802980c:
	.4byte 0xfffff000
