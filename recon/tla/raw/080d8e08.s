.syntax unified
	.thumb
	.global Func_080d8e08
	.thumb_func
Func_080d8e08:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #52
	str r1, [sp, #44]
	str r2, [sp, #40]
	str r3, [sp, #36]
	str r0, [sp, #48]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #156
	ldr r3, [r3]
	ldr r2, [sp, #84]
	mov r8, r3
	movs r3, #0
	str r3, [sp, #32]
	ldr r3, [sp, #44]
	ldr r1, [sp, #88]
	subs r2, r2, r3
	str r2, [sp, #28]
	asrs r2, r2, #8
	mov r11, r2
	ldr r2, [sp, #40]
	ldr r0, [sp, #92]
	subs r1, r1, r2
	ldr r2, [sp, #36]
	adds r3, r1, #0
	subs r0, r0, r2
	asrs r3, r3, #8
	mov r9, r3
	adds r3, r0, #0
	str r1, [sp, #24]
	str r0, [sp, #20]
	asrs r3, r3, #8
	ldr r7, .L_080d8fa0
	mov r1, r11
	mov r0, r11
	mov r10, r3
	mov lr, r7
	.2byte 0xf800
	mov r1, r9
	adds r5, r0, #0
	mov r0, r9
	mov lr, r7
	.2byte 0xf800
	mov r1, r10
	adds r6, r0, #0
	mov r0, r10
	mov lr, r7
	.2byte 0xf800
	adds r5, r5, r6
	adds r5, r5, r0
	ldr r3, .L_080d8fa4
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	movs r1, #6
	bl Math_Div
	ldr r2, [sp, #48]
	adds r0, #1
	lsls r3, r2, #5
	add r8, r3
	mov r3, r8
	mov r2, r8
	adds r3, #12
	adds r2, #40
	mov r1, r9
	mov r10, r0
	mov r0, r11
	str r3, [sp, #16]
	str r2, [sp, #12]
	bl ArcTan2
	adds r5, r0, #0
	lsls r5, r5, #16
	lsrs r5, r5, #16
	adds r0, r5, #0
	bl Trig_Sin
	str r0, [sp, #8]
	adds r0, r5, #0
	bl Trig_Cos
	ldr r2, [sp, #16]
	mov r3, r10
	str r0, [sp, #4]
	strh r3, [r2, #4]
	movs r3, #0
	mov r8, r3
	mov r11, r3
	mov r9, r3
	str r3, [sp, #0]
	b .L_080d8f56
.L_080d8ecc:
	mov r2, r8
	lsls r0, r2, #16
	mov r1, r10
	bl Math_Div
	bl Trig_Sin
	movs r1, #128
	ldr r3, .L_080d8fa0
	lsls r1, r1, #9
	mov lr, r3
	.2byte 0xf800
	ldr r2, .L_080d8fa0
	ldr r1, [sp, #4]
	adds r6, r0, #0
	mov lr, r2
	.2byte 0xf800
	mov r1, r10
	adds r5, r0, #0
	mov r0, r9
	bl Math_Div
	ldr r3, [sp, #44]
	ldr r2, .L_080d8fa0
	adds r0, r3, r0
	adds r0, r0, r5
	str r0, [r7, #4]
	ldr r1, [sp, #8]
	adds r0, r6, #0
	mov lr, r2
	.2byte 0xf800
	mov r1, r10
	adds r5, r0, #0
	mov r0, r11
	bl Math_Div
	ldr r3, [sp, #40]
	mov r1, r10
	adds r0, r3, r0
	adds r0, r0, r5
	str r0, [r7, #8]
	ldr r0, [sp, #0]
	bl Math_Div
	ldr r2, [sp, #36]
	adds r0, r2, r0
	str r0, [r7, #12]
	ldr r3, [sp, #48]
	adds r3, #1
	strb r3, [r7, #19]
	cmp r8, r10
	bne .L_080d8f38
	movs r3, #2
	b .L_080d8f3a
.L_080d8f38:
	movs r3, #1
.L_080d8f3a:
	strb r3, [r7, #18]
	ldr r3, [sp, #12]
	str r7, [r3]
	ldr r2, [sp, #0]
	ldr r3, [sp, #20]
	str r7, [sp, #12]
	adds r2, r2, r3
	str r2, [sp, #0]
	ldr r2, [sp, #24]
	ldr r3, [sp, #28]
	add r11, r2
	movs r2, #1
	add r9, r3
	add r8, r2
.L_080d8f56:
	cmp r8, r10
	bgt .L_080d8f6a
	bl Func_080d8d40
	adds r7, r0, #0
	cmp r7, #0
	bne .L_080d8ecc
	movs r3, #1
	negs r3, r3
	str r3, [sp, #32]
.L_080d8f6a:
	ldr r2, [sp, #12]
	movs r3, #0
	str r3, [r2]
	ldr r2, [sp, #16]
	movs r3, #1
	strh r3, [r2, #10]
	str r3, [r2, #24]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r2, #12]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r2, #20]
	movs r3, #8
	ldrsh r0, [r2, r3]
	cmp r0, #0
	beq .L_080d8f90
	bl GameFlag_SetBit
.L_080d8f90:
	ldr r0, [sp, #32]
	add sp, #52
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080d8fa0:
	.4byte IwramMulQ16
.L_080d8fa4:
	.4byte IwramFillWords + 0x74
