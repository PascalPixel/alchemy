.syntax unified
	.thumb
	.global Func_080eb01c
	.thumb_func
Func_080eb01c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	ldr r0, [r7, #12]
	sub sp, #48
	str r0, [sp, #28]
	ldr r1, [r7, #16]
	str r1, [sp, #24]
	ldr r3, [r7, #24]
	ldr r2, [r7, #20]
	str r3, [sp, #20]
	mov r11, r2
	ldrh r0, [r7, #32]
	str r0, [sp, #16]
	movs r0, #1
	ldrh r1, [r7, #34]
	negs r0, r0
	str r1, [sp, #12]
	movs r2, #30
	ldrsh r3, [r7, r2]
	cmp r3, r0
	bne .L_080eb054
	b .L_080eb288
.L_080eb054:
	mov r1, r11
	cmp r1, #0
	bne .L_080eb05c
	b .L_080eb288
.L_080eb05c:
	ldr r2, [sp, #20]
	cmp r2, #0
	bne .L_080eb064
	b .L_080eb288
.L_080eb064:
	movs r3, #128
	lsls r3, r3, #9
	cmp r11, r3
	bne .L_080eb080
	cmp r2, r11
	bne .L_080eb080
	ldrh r2, [r7, #28]
	adds r3, r2, #0
	cmp r3, #0
	bne .L_080eb082
	movs r0, #0
	str r0, [sp, #32]
	str r0, [sp, #36]
	b .L_080eb0ba
.L_080eb080:
	ldrh r2, [r7, #28]
.L_080eb082:
	add r0, sp, #40
	ldr r3, [r0, #4]
	ldr r1, .L_080eb160
	ands r3, r1
	orrs r3, r2
	str r3, [r0, #4]
	mov r3, r11
	lsls r2, r3, #8
	ldr r3, [sp, #40]
	lsrs r2, r2, #16
	ands r3, r1
	ldr r1, [sp, #20]
	orrs r3, r2
	lsls r2, r1, #8
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	lsrs r2, r2, #16
	lsls r2, r2, #16
	ands r3, r1
	orrs r3, r2
	str r3, [sp, #40]
	bl Func_0801401c
	movs r2, #1
	str r0, [sp, #32]
	str r2, [sp, #36]
	ldrh r2, [r7, #28]
.L_080eb0ba:
	mov r3, r11
	cmp r3, #0
	bge .L_080eb0c2
	negs r3, r3
.L_080eb0c2:
	movs r1, #128
	lsls r1, r1, #9
	cmp r3, r1
	bgt .L_080eb0d6
	ldr r3, [sp, #20]
	cmp r3, #0
	bge .L_080eb0d2
	negs r3, r3
.L_080eb0d2:
	cmp r3, r1
	ble .L_080eb0e6
.L_080eb0d6:
	ldr r0, [sp, #16]
	ldr r1, [sp, #12]
	movs r3, #3
	lsls r0, r0, #1
	lsls r1, r1, #1
	str r3, [sp, #36]
	str r0, [sp, #16]
	str r1, [sp, #12]
.L_080eb0e6:
	adds r3, r2, #0
	cmp r3, #0
	bne .L_080eb164
	movs r2, #128
	lsls r2, r2, #9
	cmp r11, r2
	bne .L_080eb11c
	ldr r3, [sp, #20]
	cmp r3, r11
	bne .L_080eb11c
	ldrh r3, [r7, #32]
	ldr r1, [sp, #16]
	movs r0, #36
	ldrsh r2, [r7, r0]
	lsrs r3, r3, #1
	subs r2, r2, r3
	lsrs r3, r1, #31
	adds r3, r1, r3
	asrs r3, r3, #1
	adds r0, r2, r3
	movs r3, #38
	ldrsh r2, [r7, r3]
	ldrh r3, [r7, #34]
	ldr r1, [sp, #12]
	lsrs r3, r3, #1
	subs r2, r2, r3
	b .L_080eb154
.L_080eb11c:
	movs r3, #36
	ldrsh r2, [r7, r3]
	ldrh r3, [r7, #32]
	movs r1, #255
	lsrs r3, r3, #1
	subs r2, r2, r3
	mov r0, r11
	muls r0, r2
	adds r2, r0, #0
	ldr r0, [sp, #16]
	lsls r1, r1, #8
	adds r1, #255
	lsrs r3, r0, #31
	adds r2, r2, r1
	adds r3, r0, r3
	asrs r3, r3, #1
	asrs r2, r2, #16
	adds r0, r2, r3
	movs r3, #38
	ldrsh r2, [r7, r3]
	ldrh r3, [r7, #34]
	lsrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [sp, #20]
	muls r2, r3
	adds r2, r2, r1
	ldr r1, [sp, #12]
	asrs r2, r2, #16
.L_080eb154:
	lsrs r3, r1, #31
	adds r3, r1, r3
	asrs r3, r3, #1
	adds r2, r2, r3
	b .L_080eb218
	.2byte 0x0000
.L_080eb160:
	.4byte 0xffff0000
.L_080eb164:
	ldrh r0, [r7, #28]
	bl Trig_Sin
	str r0, [sp, #8]
	ldrh r0, [r7, #28]
	bl Trig_Cos
	str r0, [sp, #4]
	ldr r2, [sp, #20]
	ldrh r3, [r7, #32]
	mov r5, r11
	muls r5, r3
	lsrs r3, r5, #31
	adds r5, r5, r3
	ldrh r3, [r7, #34]
	asrs r5, r5, #1
	adds r6, r2, #0
	muls r6, r3
	lsrs r3, r6, #31
	adds r6, r6, r3
	ldr r3, .L_080eb27c
	adds r1, r5, #0
	mov r8, r3
	mov lr, r8
	.2byte 0xf800
	adds r1, r5, #0
	mov r9, r0
	ldr r0, [sp, #8]
	mov lr, r8
	.2byte 0xf800
	mov r1, r9
	asrs r6, r6, #1
	subs r1, r1, r0
	str r1, [sp, #0]
	ldr r0, [sp, #4]
	adds r1, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	mov r9, r0
	ldr r0, [sp, #8]
	mov lr, r8
	.2byte 0xf800
	movs r2, #36
	ldrsh r3, [r7, r2]
	add r9, r0
	mov r0, r11
	muls r0, r3
	ldr r2, [sp, #20]
	movs r1, #38
	ldrsh r3, [r7, r1]
	mov r10, r0
	mov r1, r10
	ldr r0, [sp, #4]
	adds r5, r2, #0
	muls r5, r3
	mov lr, r8
	.2byte 0xf800
	adds r1, r5, #0
	adds r6, r0, #0
	ldr r0, [sp, #8]
	mov lr, r8
	.2byte 0xf800
	adds r1, r5, #0
	subs r6, r6, r0
	ldr r0, [sp, #4]
	mov lr, r8
	.2byte 0xf800
	mov r1, r10
	adds r5, r0, #0
	ldr r0, [sp, #8]
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	ldr r0, [sp, #16]
	ldr r1, [sp, #0]
	ldr r2, [sp, #12]
	lsrs r3, r0, #31
	adds r3, r0, r3
	subs r6, r6, r1
	asrs r3, r3, #1
	asrs r6, r6, #16
	adds r0, r3, r6
	mov r1, r9
	lsrs r3, r2, #31
	adds r3, r2, r3
	subs r5, r5, r1
	asrs r3, r3, #1
	asrs r5, r5, #16
	adds r2, r3, r5
.L_080eb218:
	ldr r1, [sp, #28]
	asrs r3, r1, #16
	subs r5, r3, r0
	ldr r0, [sp, #24]
	ldr r1, [sp, #16]
	asrs r3, r0, #16
	subs r6, r3, r2
	adds r3, r5, r1
	cmp r3, #0
	blt .L_080eb288
	cmp r5, #239
	bgt .L_080eb288
	ldr r2, [sp, #12]
	adds r3, r6, r2
	cmp r3, #0
	ble .L_080eb288
	cmp r6, #159
	bgt .L_080eb288
	ldr r3, .L_080eb278
	ldrh r2, [r7, #6]
	ands r5, r3
	ldr r3, .L_080eb280
	strb r6, [r7, #4]
	ands r3, r2
	orrs r3, r5
	strh r3, [r7, #6]
	ldrb r2, [r7, #5]
	ldr r0, [sp, #36]
	movs r3, #4
	negs r3, r3
	ands r3, r2
	orrs r3, r0
	strb r3, [r7, #5]
	ldr r1, [sp, #32]
	movs r3, #31
	ands r1, r3
	str r1, [sp, #32]
	ldrb r2, [r7, #7]
	movs r3, #63
	negs r3, r3
	lsls r1, r1, #1
	ands r3, r2
	orrs r3, r1
	strb r3, [r7, #7]
	movs r2, #30
	ldrsh r1, [r7, r2]
	adds r0, r7, #0
	b .L_080eb284
.L_080eb278:
	.4byte 0x000001ff
.L_080eb27c:
	.4byte IwramMulQ16
.L_080eb280:
	.4byte 0xfffffe00
.L_080eb284:
	bl Func_080140d8
.L_080eb288:
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
