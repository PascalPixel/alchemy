.syntax unified
	.thumb
	.global Func_080d9498
	.thumb_func
Func_080d9498:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #88
	str r0, [sp, #84]
	str r1, [sp, #80]
	str r2, [sp, #76]
	str r3, [sp, #72]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #156
	ldr r3, [r3]
	ldr r2, [sp, #120]
	str r3, [sp, #68]
	ldr r3, [sp, #80]
	ldr r0, [sp, #128]
	subs r2, r2, r3
	ldr r3, [sp, #72]
	str r2, [sp, #64]
	ldr r1, [sp, #124]
	asrs r6, r2, #8
	ldr r2, [sp, #76]
	subs r0, r0, r3
	subs r1, r1, r2
	str r0, [sp, #56]
	ldr r0, [sp, #84]
	str r1, [sp, #60]
	asrs r2, r1, #8
	ldr r1, [sp, #68]
	lsls r0, r0, #5
	str r0, [sp, #32]
	adds r3, r1, r0
	adds r0, r3, #0
	adds r0, #12
	movs r1, #10
	ldrsh r3, [r0, r1]
	ldr r7, [r0, #28]
	cmp r3, #1
	bne .L_080d94f0
	b .L_080d96fa
.L_080d94f0:
	movs r3, #1
	strh r3, [r0, #10]
	movs r1, #4
	ldrsh r3, [r0, r1]
	movs r0, #128
	lsls r0, r0, #2
	str r3, [sp, #52]
	str r2, [sp, #4]
	bl Runtime_BumpAllocate
	str r0, [sp, #28]
	adds r3, r0, #0
	ldr r0, [sp, #52]
	ldr r2, [sp, #4]
	cmp r0, #0
	blt .L_080d9524
	adds r0, #1
	mov r8, r0
.L_080d9514:
	movs r1, #1
	negs r1, r1
	add r8, r1
	mov r0, r8
	stmia r3!, {r7}
	ldr r7, [r7]
	cmp r0, #0
	bne .L_080d9514
.L_080d9524:
	adds r1, r2, #0
	adds r0, r6, #0
	bl ArcTan2
	adds r5, r0, #0
	lsls r5, r5, #16
	lsrs r5, r5, #16
	adds r0, r5, #0
	bl Trig_Sin
	str r0, [sp, #44]
	adds r0, r5, #0
	bl Trig_Cos
	ldr r2, [sp, #52]
	movs r1, #1
	lsls r2, r2, #1
	str r0, [sp, #40]
	str r1, [sp, #48]
	str r2, [sp, #24]
	cmp r1, r2
	ble .L_080d9552
	b .L_080d96cc
.L_080d9552:
	ldr r3, [sp, #56]
	ldr r0, [sp, #60]
	ldr r1, [sp, #64]
	str r3, [sp, #16]
	str r0, [sp, #12]
	str r1, [sp, #8]
.L_080d955e:
	ldr r2, [sp, #52]
	mov r8, r2
	cmp r2, #0
	bge .L_080d9568
	b .L_080d96a0
.L_080d9568:
	ldr r3, [sp, #84]
	ldr r0, .L_080d9708
	adds r3, #1
	str r3, [sp, #20]
	mov r9, r0
.L_080d9572:
	ldr r2, [sp, #28]
	mov r1, r8
	lsls r3, r1, #2
	ldr r7, [r3, r2]
	add r3, sp, #20
	ldrb r3, [r3]
	strb r3, [r7, #19]
	ldr r0, [sp, #52]
	cmp r8, r0
	bne .L_080d95f6
	ldr r1, [sp, #48]
	lsls r0, r1, #14
	mov r1, r8
	bl Math_Div
	bl Trig_Sin
	movs r1, #128
	lsls r1, r1, #12
	mov lr, r9
	.2byte 0xf800
	ldr r1, [sp, #40]
	adds r6, r0, #0
	mov lr, r9
	.2byte 0xf800
	ldr r2, [sp, #8]
	adds r5, r0, #0
	lsrs r0, r2, #31
	adds r0, r2, r0
	mov r1, r8
	asrs r0, r0, #1
	bl Math_Div
	ldr r3, [sp, #80]
	adds r0, r3, r0
	adds r0, r0, r5
	str r0, [r7, #4]
	ldr r1, [sp, #44]
	str r0, [sp, #36]
	adds r0, r6, #0
	mov lr, r9
	.2byte 0xf800
	ldr r1, [sp, #12]
	adds r5, r0, #0
	lsrs r0, r1, #31
	adds r0, r1, r0
	asrs r0, r0, #1
	mov r1, r8
	bl Math_Div
	ldr r2, [sp, #76]
	mov r1, r8
	adds r0, r2, r0
	adds r0, r0, r5
	str r0, [r7, #8]
	ldr r3, [sp, #16]
	mov r11, r0
	lsrs r0, r3, #31
	adds r0, r3, r0
	asrs r0, r0, #1
	bl Math_Div
	ldr r1, [sp, #72]
	adds r0, r1, r0
	str r0, [r7, #12]
	b .L_080d9690
.L_080d95f6:
	ldr r2, [sp, #36]
	ldr r3, [r7, #4]
	mov r0, r11
	subs r3, r2, r3
	asrs r6, r3, #8
	ldr r3, [r7, #8]
	ldr r5, [r7, #12]
	subs r3, r0, r3
	asrs r2, r3, #8
	mov r1, r10
	str r2, [sp, #4]
	subs r5, r1, r5
	adds r0, r6, #0
	adds r1, r6, #0
	mov lr, r9
	.2byte 0xf800
	ldr r2, [sp, #4]
	adds r6, r0, #0
	adds r1, r2, #0
	adds r0, r2, #0
	mov lr, r9
	.2byte 0xf800
	asrs r5, r5, #8
	adds r3, r0, #0
	str r3, [sp, #0]
	adds r1, r5, #0
	adds r0, r5, #0
	mov lr, r9
	.2byte 0xf800
	ldr r3, [sp, #0]
	adds r6, r6, r3
	adds r6, r6, r0
	adds r0, r6, #0
	ldr r3, .L_080d970c
	mov lr, r3
	.2byte 0xf800
	adds r6, r0, #0
	cmp r6, #6
	ble .L_080d9692
	ldr r2, [sp, #36]
	ldr r3, [r7, #4]
	adds r1, r6, #0
	subs r3, r3, r2
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #1
	bl Math_Div
	ldr r3, [sp, #36]
	adds r1, r6, #0
	adds r0, r3, r0
	str r0, [r7, #4]
	str r0, [sp, #36]
	ldr r3, [r7, #8]
	mov r0, r11
	subs r3, r3, r0
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #1
	bl Math_Div
	ldr r3, [r7, #12]
	mov r1, r10
	subs r3, r3, r1
	add r0, r11
	str r0, [r7, #8]
	mov r11, r0
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #1
	adds r1, r6, #0
	bl Math_Div
	movs r3, #1
	add r0, r10
	str r0, [r7, #12]
	strb r3, [r7, #18]
.L_080d9690:
	mov r10, r0
.L_080d9692:
	movs r2, #1
	negs r2, r2
	add r8, r2
	mov r3, r8
	cmp r3, #0
	blt .L_080d96a0
	b .L_080d9572
.L_080d96a0:
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #16]
	ldr r1, [sp, #56]
	ldr r2, [sp, #12]
	ldr r3, [sp, #60]
	adds r0, r0, r1
	adds r2, r2, r3
	str r0, [sp, #16]
	str r2, [sp, #12]
	ldr r0, [sp, #8]
	ldr r2, [sp, #48]
	ldr r1, [sp, #64]
	ldr r3, [sp, #24]
	adds r0, r0, r1
	adds r2, #1
	str r0, [sp, #8]
	str r2, [sp, #48]
	cmp r2, r3
	bgt .L_080d96cc
	b .L_080d955e
.L_080d96cc:
	ldr r0, [sp, #68]
	ldr r1, [sp, #32]
	adds r3, r0, r1
	adds r0, r3, #0
	adds r0, #12
	movs r3, #1
	str r3, [r0, #24]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r0, #12]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r0, #20]
	movs r2, #8
	ldrsh r0, [r0, r2]
	cmp r0, #0
	beq .L_080d96f2
	bl GameFlag_SetBit
.L_080d96f2:
	ldr r0, [sp, #28]
	bl Sys_Free
	movs r0, #0
.L_080d96fa:
	add sp, #88
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080d9708:
	.4byte IwramMulQ16
.L_080d970c:
	.4byte IwramFillWords + 0x74
