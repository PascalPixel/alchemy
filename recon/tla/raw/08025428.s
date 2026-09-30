.syntax unified
	.thumb
	.global Func_08025428
	.thumb_func
Func_08025428:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	mov r8, r0
	adds r3, r2, #0
	adds r3, #236
	ldr r3, [r3]
	ldr r1, [r0, #104]
	movs r0, #240
	lsls r0, r0, #15
	adds r7, r3, r0
	adds r3, r2, #0
	adds r3, #240
	ldr r3, [r3]
	movs r0, #192
	lsls r0, r0, #15
	adds r6, r3, r0
	adds r3, r2, #0
	adds r3, #244
	ldr r3, [r3]
	ldr r0, .L_080255f4
	sub sp, #8
	adds r4, r3, r0
	adds r3, r2, #0
	adds r3, #248
	ldr r3, [r3]
	ldr r2, .L_080255f8
	adds r0, r3, r2
	mov r2, r8
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	cmp r1, #0
	bne .L_0802547c
	b .L_080255d8
.L_0802547c:
	ldr r3, [r1]
	cmp r3, #0
	bne .L_08025484
	b .L_080255d8
.L_08025484:
	ldr r3, [r1, #8]
	ldr r5, [r1, #12]
	ldr r1, [r1, #16]
	mov r11, r3
	movs r3, #128
	lsls r3, r3, #24
	mov r2, r8
	str r1, [sp, #4]
	str r3, [r2, #56]
	str r3, [r2, #60]
	str r3, [r2, #64]
	cmp r11, r7
	bge .L_080254a0
	mov r11, r7
.L_080254a0:
	ldr r3, [sp, #4]
	cmp r3, r6
	bge .L_080254a8
	str r6, [sp, #4]
.L_080254a8:
	cmp r11, r4
	ble .L_080254ae
	mov r11, r4
.L_080254ae:
	ldr r2, [sp, #4]
	cmp r2, r0
	ble .L_080254b6
	str r0, [sp, #4]
.L_080254b6:
	mov r3, r8
	adds r3, #100
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_080254d0
	mov r3, r8
	mov r2, r11
	str r2, [r3, #8]
	str r5, [r3, #12]
	ldr r0, [sp, #4]
	str r0, [r3, #16]
	b .L_080255d8
.L_080254d0:
	mov r2, r8
	ldr r3, [r2, #8]
	mov r2, r11
	subs r0, r2, r3
	cmp r0, #0
	bge .L_080254e4
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r0, r0, r3
.L_080254e4:
	mov r2, r8
	ldr r3, [r2, #16]
	ldr r2, [sp, #4]
	asrs r0, r0, #16
	mov r10, r0
	subs r0, r2, r3
	cmp r0, #0
	bge .L_080254fc
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r0, r0, r3
.L_080254fc:
	mov r2, r10
	asrs r6, r0, #16
	adds r3, r6, #0
	muls r3, r6
	mov r0, r10
	muls r0, r2
	adds r0, r0, r3
	ldr r3, .L_080255fc
	mov lr, r3
	.2byte 0xf800
	lsls r7, r0, #16
	mov r0, r8
	ldr r3, [r0, #8]
	mov r2, r11
	subs r2, r2, r3
	ldr r3, [r0, #12]
	mov r10, r2
	subs r5, r5, r3
	ldr r3, [r0, #16]
	ldr r0, [sp, #4]
	movs r2, #128
	lsls r2, r2, #15
	mov r9, r5
	subs r6, r0, r3
	cmp r7, r2
	bge .L_08025552
	ldr r3, .L_08025600
	mov r1, r10
	mov r0, r10
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	adds r7, r0, #0
	ldr r2, .L_08025600
	adds r0, r6, #0
	mov lr, r2
	.2byte 0xf800
	adds r7, r7, r0
	adds r0, r7, #0
	str r7, [sp, #0]
	bl Func_080149e0
	adds r7, r0, #0
.L_08025552:
	adds r1, r7, #0
	cmp r7, #0
	bge .L_0802555a
	adds r1, r7, #7
.L_0802555a:
	mov r0, r8
	ldr r3, [r0, #48]
	asrs r5, r1, #3
	cmp r5, r3
	ble .L_08025566
	adds r5, r3, #0
.L_08025566:
	movs r2, #128
	lsls r2, r2, #7
	cmp r7, r2
	bge .L_0802557a
	mov r0, r8
	mov r3, r11
	str r3, [r0, #8]
	ldr r2, [sp, #4]
	str r2, [r0, #16]
	b .L_080255b4
.L_0802557a:
	cmp r7, r5
	ble .L_080255a6
	mov r1, r10
	ldr r3, .L_08025604
	adds r0, r7, #0
	mov lr, r3
	.2byte 0xf800
	ldr r2, .L_08025600
	adds r1, r5, #0
	mov r11, r2
	mov lr, r11
	.2byte 0xf800
	adds r1, r6, #0
	ldr r3, .L_08025604
	mov r10, r0
	adds r0, r7, #0
	mov lr, r3
	.2byte 0xf800
	adds r1, r5, #0
	mov lr, r11
	.2byte 0xf800
	adds r6, r0, #0
.L_080255a6:
	mov r0, r8
	ldr r3, [r0, #8]
	add r3, r10
	str r3, [r0, #8]
	ldr r3, [r0, #16]
	adds r3, r3, r6
	str r3, [r0, #16]
.L_080255b4:
	mov r3, r9
	cmp r3, #0
	bge .L_080255bc
	negs r3, r3
.L_080255bc:
	movs r2, #128
	lsls r2, r2, #8
	cmp r3, r2
	ble .L_080255d0
	mov r3, r9
	cmp r3, #0
	bge .L_080255cc
	adds r3, #3
.L_080255cc:
	asrs r3, r3, #2
	mov r9, r3
.L_080255d0:
	mov r0, r8
	ldr r3, [r0, #12]
	add r3, r9
	str r3, [r0, #12]
.L_080255d8:
	mov r2, r8
	ldrh r3, [r2, #4]
	mov r0, r8
	adds r3, #1
	strh r3, [r0, #4]
	add sp, #8
	movs r0, #1
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080255f4:
	.4byte 0xff880000
.L_080255f8:
	.4byte 0xffc00000
.L_080255fc:
	.4byte IwramFillWords + 0x74
.L_08025600:
	.4byte IwramMulQ16
.L_08025604:
	.4byte IwramRatioMulQ14
