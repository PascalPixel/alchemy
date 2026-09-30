.syntax unified
	.thumb
	.global Func_080da3bc
	.thumb_func
Func_080da3bc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #88
	str r1, [sp, #80]
	str r2, [sp, #76]
	str r3, [sp, #72]
	str r0, [sp, #84]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #160
	ldr r3, [r3]
	ldr r2, [sp, #120]
	str r3, [sp, #68]
	ldr r3, [sp, #80]
	ldr r1, [sp, #124]
	subs r2, r2, r3
	str r2, [sp, #64]
	asrs r2, r2, #8
	mov r9, r2
	ldr r2, [sp, #76]
	ldr r3, [sp, #72]
	ldr r0, [sp, #128]
	subs r1, r1, r2
	str r1, [sp, #60]
	subs r0, r0, r3
	asrs r1, r1, #8
	mov r11, r1
	adds r1, r0, #0
	asrs r1, r1, #8
	str r0, [sp, #56]
	mov r10, r1
	ldr r6, .L_080da658
	mov r1, r9
	mov r0, r9
	mov lr, r6
	.2byte 0xf800
	mov r1, r11
	adds r5, r0, #0
	mov r0, r11
	mov lr, r6
	.2byte 0xf800
	mov r1, r10
	mov r8, r0
	mov r0, r10
	mov lr, r6
	.2byte 0xf800
	add r5, r8
	adds r5, r5, r0
	ldr r3, .L_080da65c
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	movs r1, #6
	bl Math_Div
	ldr r2, [sp, #84]
	ldr r1, [sp, #84]
	lsls r2, r2, #3
	adds r0, #1
	subs r3, r2, r1
	str r0, [sp, #52]
	str r2, [sp, #44]
	ldr r2, [sp, #68]
	lsls r3, r3, #2
	movs r0, #128
	adds r3, #36
	lsls r0, r0, #2
	ldr r7, [r2, r3]
	bl Runtime_BumpAllocate
	ldr r1, [sp, #52]
	str r0, [sp, #20]
	adds r3, r0, #0
	cmp r1, #0
	blt .L_080da470
	adds r1, #1
	mov r8, r1
.L_080da460:
	movs r2, #1
	negs r2, r2
	add r8, r2
	mov r1, r8
	stmia r3!, {r7}
	ldr r7, [r7]
	cmp r1, #0
	bne .L_080da460
.L_080da470:
	mov r1, r11
	mov r0, r9
	bl ArcTan2
	adds r5, r0, #0
	lsls r5, r5, #16
	lsrs r5, r5, #16
	adds r0, r5, #0
	bl Trig_Sin
	str r0, [sp, #40]
	adds r0, r5, #0
	bl Trig_Cos
	ldr r3, [sp, #52]
	movs r2, #1
	lsls r3, r3, #1
	str r0, [sp, #36]
	str r2, [sp, #48]
	str r3, [sp, #16]
	cmp r2, r3
	ble .L_080da49e
	b .L_080da624
.L_080da49e:
	ldr r1, [sp, #56]
	ldr r2, [sp, #60]
	ldr r3, [sp, #64]
	str r1, [sp, #8]
	str r2, [sp, #4]
	str r3, [sp, #0]
.L_080da4aa:
	ldr r1, [sp, #52]
	mov r8, r1
	cmp r1, #0
	bge .L_080da4b4
	b .L_080da5f8
.L_080da4b4:
	ldr r2, [sp, #84]
	adds r2, #1
	str r2, [sp, #12]
.L_080da4ba:
	ldr r2, [sp, #20]
	mov r1, r8
	lsls r3, r1, #2
	ldr r7, [r3, r2]
	add r3, sp, #12
	ldrb r3, [r3]
	strb r3, [r7, #17]
	ldr r1, [sp, #52]
	cmp r8, r1
	bne .L_080da546
	ldr r2, [sp, #48]
	mov r1, r8
	lsls r0, r2, #14
	bl Math_Div
	bl Trig_Sin
	movs r1, #128
	ldr r3, .L_080da658
	lsls r1, r1, #12
	mov lr, r3
	.2byte 0xf800
	ldr r2, .L_080da658
	ldr r1, [sp, #36]
	adds r6, r0, #0
	mov lr, r2
	.2byte 0xf800
	ldr r3, [sp, #0]
	adds r5, r0, #0
	lsrs r0, r3, #31
	adds r0, r3, r0
	mov r1, r8
	asrs r0, r0, #1
	bl Math_Div
	ldr r1, [sp, #80]
	ldr r2, .L_080da658
	adds r0, r1, r0
	adds r0, r0, r5
	str r0, [r7, #4]
	ldr r1, [sp, #40]
	str r0, [sp, #32]
	adds r0, r6, #0
	mov lr, r2
	.2byte 0xf800
	ldr r3, [sp, #4]
	adds r5, r0, #0
	lsrs r0, r3, #31
	adds r0, r3, r0
	mov r1, r8
	asrs r0, r0, #1
	bl Math_Div
	ldr r1, [sp, #76]
	adds r0, r1, r0
	adds r0, r0, r5
	str r0, [r7, #8]
	ldr r2, [sp, #8]
	str r0, [sp, #28]
	lsrs r0, r2, #31
	adds r0, r2, r0
	asrs r0, r0, #1
	mov r1, r8
	bl Math_Div
	ldr r3, [sp, #72]
	adds r0, r3, r0
	str r0, [r7, #12]
	str r0, [sp, #24]
	b .L_080da5ea
.L_080da546:
	ldr r1, [sp, #32]
	ldr r3, [r7, #4]
	ldr r2, [sp, #28]
	subs r3, r1, r3
	asrs r3, r3, #8
	mov r9, r3
	ldr r3, [r7, #8]
	ldr r1, [sp, #24]
	subs r3, r2, r3
	asrs r3, r3, #8
	mov r11, r3
	ldr r3, [r7, #12]
	ldr r2, .L_080da658
	subs r3, r1, r3
	asrs r3, r3, #8
	mov r1, r9
	mov r0, r9
	mov r10, r3
	mov lr, r2
	.2byte 0xf800
	ldr r3, .L_080da658
	adds r5, r0, #0
	mov r1, r11
	mov r0, r11
	mov lr, r3
	.2byte 0xf800
	mov r1, r10
	adds r6, r0, #0
	ldr r2, .L_080da658
	mov r0, r10
	mov lr, r2
	.2byte 0xf800
	adds r5, r5, r6
	adds r5, r5, r0
	ldr r3, .L_080da65c
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	adds r6, r0, #0
	cmp r6, #6
	ble .L_080da5f8
	ldr r1, [sp, #32]
	ldr r3, [r7, #4]
	subs r3, r3, r1
	lsls r0, r3, #1
	adds r0, r0, r3
	adds r1, r6, #0
	lsls r0, r0, #1
	bl Math_Div
	ldr r2, [sp, #32]
	adds r0, r2, r0
	str r0, [r7, #4]
	str r0, [sp, #32]
	ldr r1, [sp, #28]
	ldr r3, [r7, #8]
	subs r3, r3, r1
	lsls r0, r3, #1
	adds r0, r0, r3
	adds r1, r6, #0
	lsls r0, r0, #1
	bl Math_Div
	ldr r2, [sp, #28]
	adds r0, r2, r0
	str r0, [r7, #8]
	str r0, [sp, #28]
	ldr r1, [sp, #24]
	ldr r3, [r7, #12]
	subs r3, r3, r1
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #1
	adds r1, r6, #0
	bl Math_Div
	ldr r2, [sp, #24]
	movs r3, #1
	adds r0, r2, r0
	str r0, [r7, #12]
	str r0, [sp, #24]
	strb r3, [r7, #16]
.L_080da5ea:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r1, r8
	cmp r1, #0
	blt .L_080da5f8
	b .L_080da4ba
.L_080da5f8:
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #8]
	ldr r3, [sp, #56]
	ldr r1, [sp, #4]
	adds r2, r2, r3
	str r2, [sp, #8]
	ldr r2, [sp, #60]
	ldr r3, [sp, #0]
	adds r1, r1, r2
	str r1, [sp, #4]
	ldr r1, [sp, #64]
	ldr r2, [sp, #48]
	adds r3, r3, r1
	str r3, [sp, #0]
	ldr r3, [sp, #16]
	adds r2, #1
	str r2, [sp, #48]
	cmp r2, r3
	bgt .L_080da624
	b .L_080da4aa
.L_080da624:
	ldr r1, [sp, #44]
	ldr r2, [sp, #84]
	subs r3, r1, r2
	ldr r1, [sp, #68]
	lsls r3, r3, #2
	adds r3, r1, r3
	adds r3, #12
	movs r2, #1
	str r2, [r3, #20]
	movs r2, #128
	lsls r2, r2, #11
	str r2, [r3, #8]
	movs r2, #128
	lsls r2, r2, #7
	str r2, [r3, #16]
	ldr r0, [sp, #20]
	bl Sys_Free
	movs r0, #0
	add sp, #88
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080da658:
	.4byte IwramMulQ16
.L_080da65c:
	.4byte IwramFillWords + 0x74
