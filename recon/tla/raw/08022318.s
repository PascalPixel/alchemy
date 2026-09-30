.syntax unified
	.thumb
	.global Func_08022318
	.thumb_func
Func_08022318:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	ldrb r0, [r7, #20]
	sub sp, #64
	lsrs r0, r0, #1
	str r0, [sp, #24]
	movs r4, #4
	ldrb r0, [r7, #21]
	mov r11, r4
	lsrs r0, r0, #1
	str r0, [sp, #20]
	movs r0, #8
	str r0, [sp, #16]
	ldmia r1!, {r0}
	ldmia r2!, {r6}
	ldr r5, [r2]
	str r0, [sp, #8]
	adds r0, r7, #0
	ldmia r1!, {r2}
	str r2, [sp, #4]
	ldmia r1!, {r4}
	str r4, [sp, #0]
	ldr r1, [r1]
	mov r8, r1
	adds r1, r3, #0
	bl Func_08021a84
	mov r12, r0
	cmp r0, #0
	bne .L_0802237c
	movs r0, #128
	lsls r0, r0, #9
	cmp r6, r0
	bne .L_0802237c
	cmp r5, r6
	bne .L_0802237c
	ldrh r2, [r7, #18]
	adds r3, r2, #0
	cmp r3, #0
	bne .L_0802237e
	movs r1, #0
	str r1, [sp, #12]
	str r1, [sp, #28]
	b .L_080223c6
.L_0802237c:
	ldrh r2, [r7, #18]
.L_0802237e:
	movs r3, #1
	str r3, [sp, #12]
	add r0, sp, #32
	ldr r3, [r0, #4]
	ldr r4, .L_08022438
	ands r3, r4
	orrs r3, r2
	str r3, [r0, #4]
	ldr r1, [sp, #32]
	lsls r3, r6, #8
	lsrs r3, r3, #16
	ands r1, r4
	movs r2, #255
	orrs r1, r3
	lsls r2, r2, #8
	lsls r3, r5, #8
	adds r2, #255
	lsrs r3, r3, #16
	ands r1, r2
	lsls r3, r3, #16
	orrs r1, r3
	mov r2, r12
	str r1, [sp, #32]
	cmp r2, #0
	beq .L_080223c0
	ldrh r3, [r0]
	adds r2, r4, #0
	negs r3, r3
	lsls r3, r3, #16
	lsrs r3, r3, #16
	ands r2, r1
	orrs r2, r3
	str r2, [sp, #32]
.L_080223c0:
	bl Func_0801401c
	str r0, [sp, #28]
.L_080223c6:
	movs r3, #128
	lsls r3, r3, #9
	cmp r6, r3
	bgt .L_080223d2
	cmp r5, r3
	ble .L_080223ea
.L_080223d2:
	ldr r4, [sp, #24]
	ldr r0, [sp, #20]
	movs r3, #3
	lsls r4, r4, #1
	lsls r0, r0, #1
	movs r1, #16
	str r3, [sp, #12]
	str r4, [sp, #24]
	str r0, [sp, #20]
	str r1, [sp, #16]
	movs r2, #8
	mov r11, r2
.L_080223ea:
	ldr r4, [sp, #0]
	mov r0, r8
	subs r3, r4, r0
	ldrb r2, [r7, #26]
	asrs r3, r3, #16
	mov r1, r11
	subs r4, r3, r1
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08022440
	cmp r4, #159
	bgt .L_08022440
	adds r0, r7, #0
	adds r0, #28
	ldrb r2, [r0, #5]
	movs r3, #4
	negs r3, r3
	ands r3, r2
	strb r3, [r0, #5]
	ldr r3, [sp, #8]
	ldr r1, [sp, #16]
	asrs r2, r3, #16
	ldr r3, .L_08022434
	subs r2, r2, r1
	ands r2, r3
	ldrh r1, [r0, #6]
	ldr r3, .L_0802243c
	strb r4, [r0, #4]
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #6]
	ldr r1, [sp, #96]
	bl Func_08014128
	b .L_08022440
	.2byte 0x0000
.L_08022434:
	.4byte 0x000001ff
.L_08022438:
	.4byte 0xffff0000
.L_0802243c:
	.4byte 0xfffffe00
.L_08022440:
	cmp r6, #0
	bne .L_08022446
	b .L_0802253c
.L_08022446:
	cmp r5, #0
	bne .L_0802244c
	b .L_0802253c
.L_0802244c:
	ldrh r3, [r7, #18]
	cmp r3, #0
	bne .L_0802246c
	movs r3, #22
	ldrsb r3, [r7, r3]
	adds r2, r6, #0
	muls r2, r3
	ldrb r3, [r7, #21]
	mov r9, r2
	movs r2, #23
	ldrsb r2, [r7, r2]
	lsrs r3, r3, #1
	subs r3, r3, r2
	adds r0, r5, #0
	muls r0, r3
	b .L_080224c2
.L_0802246c:
	ldrh r0, [r7, #18]
	bl Trig_Sin
	mov r10, r0
	ldrh r0, [r7, #18]
	bl Trig_Cos
	movs r3, #22
	ldrsb r3, [r7, r3]
	mov r8, r0
	adds r4, r6, #0
	muls r4, r3
	str r4, [sp, #16]
	adds r1, r4, #0
	ldrb r3, [r7, #21]
	movs r2, #23
	ldrsb r2, [r7, r2]
	lsrs r3, r3, #1
	subs r3, r3, r2
	adds r0, r5, #0
	muls r0, r3
	ldr r5, .L_08022530
	mov r11, r0
	mov r0, r8
	mov lr, r5
	.2byte 0xf800
	mov r1, r11
	adds r6, r0, #0
	mov r0, r10
	mov lr, r5
	.2byte 0xf800
	mov r1, r11
	adds r6, r6, r0
	mov r0, r8
	mov lr, r5
	.2byte 0xf800
	mov r9, r6
	ldr r1, [sp, #16]
	adds r6, r0, #0
	mov r0, r10
	mov lr, r5
	.2byte 0xf800
	subs r0, r6, r0
.L_080224c2:
	ldr r1, [sp, #8]
	ldr r3, [sp, #24]
	asrs r2, r1, #16
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	mov r4, r9
	subs r2, r2, r3
	adds r3, r4, r1
	asrs r3, r3, #16
	adds r6, r2, r3
	ldr r4, [sp, #4]
	ldr r2, [sp, #0]
	adds r1, r0, r1
	subs r3, r2, r4
	ldr r2, [sp, #20]
	asrs r3, r3, #16
	subs r3, r3, r2
	asrs r1, r1, #16
	subs r4, r3, r1
	cmp r6, #239
	bgt .L_0802253c
	cmp r4, #159
	bgt .L_0802253c
	ldr r3, .L_0802252c
	adds r0, r7, #0
	ldrh r2, [r0, #6]
	ands r6, r3
	ldr r3, .L_08022534
	strb r4, [r0, #4]
	ands r3, r2
	orrs r3, r6
	strh r3, [r0, #6]
	ldrb r2, [r0, #5]
	ldr r4, [sp, #12]
	movs r3, #4
	negs r3, r3
	ands r3, r2
	orrs r3, r4
	strb r3, [r0, #5]
	ldr r1, [sp, #28]
	movs r3, #31
	ands r1, r3
	str r1, [sp, #28]
	movs r3, #63
	ldrb r2, [r0, #7]
	negs r3, r3
	lsls r1, r1, #1
	ands r3, r2
	orrs r3, r1
	strb r3, [r0, #7]
	ldr r1, [sp, #96]
	b .L_08022538
.L_0802252c:
	.4byte 0x000001ff
.L_08022530:
	.4byte IwramMulQ16
.L_08022534:
	.4byte 0xfffffe00
.L_08022538:
	bl Func_08014128
.L_0802253c:
	add sp, #64
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
