.syntax unified
	.thumb
	.global Object_SetMoveTarget
	.thumb_func
Object_SetMoveTarget:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	str r3, [sp, #4]
	mov r10, r0
	ldr r3, [r0, #8]
	mov r9, r1
	subs r0, r1, r3
	mov r11, r2
	cmp r0, #0
	bge .L_08024760
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r0, r0, r2
.L_08024760:
	mov r2, r10
	ldr r3, [r2, #12]
	mov r2, r11
	asrs r1, r0, #16
	subs r0, r2, r3
	cmp r0, #0
	bge .L_08024776
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r0, r0, r3
.L_08024776:
	mov r2, r10
	ldr r3, [r2, #16]
	ldr r2, [sp, #4]
	asrs r4, r0, #16
	subs r0, r2, r3
	cmp r0, #0
	bge .L_0802478c
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r0, r0, r3
.L_0802478c:
	adds r3, r4, #0
	muls r3, r4
	asrs r7, r0, #16
	adds r0, r1, #0
	muls r0, r1
	adds r2, r7, #0
	muls r2, r7
	adds r0, r0, r3
	adds r0, r0, r2
	ldr r3, .L_08024914
	mov lr, r3
	.2byte 0xf800
	movs r2, #128
	lsls r5, r0, #16
	lsls r2, r2, #13
	cmp r5, r2
	bge .L_080247f2
	mov r2, r10
	ldr r3, [r2, #8]
	mov r2, r9
	subs r1, r2, r3
	mov r2, r10
	ldr r3, [r2, #12]
	mov r2, r11
	subs r4, r2, r3
	mov r2, r10
	ldr r3, [r2, #16]
	ldr r2, [sp, #4]
	str r4, [sp, #0]
	ldr r6, .L_08024918
	adds r0, r1, #0
	subs r7, r2, r3
	mov lr, r6
	.2byte 0xf800
	ldr r4, [sp, #0]
	adds r5, r0, #0
	adds r1, r4, #0
	adds r0, r4, #0
	mov lr, r6
	.2byte 0xf800
	adds r1, r7, #0
	mov r8, r0
	adds r0, r7, #0
	mov lr, r6
	.2byte 0xf800
	add r5, r8
	adds r5, r5, r0
	adds r0, r5, #0
	bl Func_080149e0
	adds r5, r0, #0
.L_080247f2:
	movs r3, #128
	lsls r3, r3, #9
	cmp r5, r3
	bge .L_08024816
	mov r3, r10
	mov r2, r9
	str r2, [r3, #8]
	mov r2, r11
	str r2, [r3, #12]
	ldr r2, [sp, #4]
	str r2, [r3, #16]
	movs r3, #128
	lsls r3, r3, #24
	mov r2, r10
	str r3, [r2, #56]
	str r3, [r2, #60]
	str r3, [r2, #64]
	b .L_08024906
.L_08024816:
	mov r3, r10
	adds r3, #88
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_08024898
	mov r3, r10
	ldr r1, [r3, #48]
	ldr r3, .L_08024918
	adds r0, r1, #0
	mov lr, r3
	.2byte 0xf800
	mov r2, r10
	adds r1, r0, #0
	ldr r3, .L_0802491c
	ldr r0, [r2, #52]
	mov lr, r3
	.2byte 0xf800
	adds r1, r0, #0
	cmp r5, r1
	ble .L_08024848
	lsrs r3, r1, #31
	adds r3, r1, r3
	asrs r3, r3, #1
	subs r1, r5, r3
	b .L_0802484e
.L_08024848:
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r1, r3, #1
.L_0802484e:
	ldr r3, .L_0802491c
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	mov r3, r10
	adds r6, r0, #0
	ldr r0, [r3, #8]
	mov r2, r9
	ldr r5, .L_08024918
	subs r0, r2, r0
	adds r1, r6, #0
	mov lr, r5
	.2byte 0xf800
	mov r2, r10
	ldr r3, [r2, #8]
	adds r1, r6, #0
	adds r3, r3, r0
	ldr r0, [r2, #12]
	mov r9, r3
	mov r3, r11
	subs r0, r3, r0
	mov lr, r5
	.2byte 0xf800
	mov r2, r10
	ldr r3, [r2, #12]
	adds r1, r6, #0
	adds r3, r3, r0
	mov r11, r3
	ldr r0, [r2, #16]
	ldr r3, [sp, #4]
	subs r0, r3, r0
	mov lr, r5
	.2byte 0xf800
	mov r2, r10
	ldr r3, [r2, #16]
	adds r3, r3, r0
	str r3, [sp, #4]
.L_08024898:
	mov r2, r10
	mov r3, r9
	str r3, [r2, #56]
	mov r3, r11
	str r3, [r2, #60]
	ldr r3, [sp, #4]
	str r3, [r2, #64]
	ldr r3, [r2, #8]
	mov r2, r9
	subs r1, r2, r3
	mov r2, r10
	ldr r3, [r2, #12]
	mov r2, r11
	subs r4, r2, r3
	mov r2, r10
	ldr r3, [r2, #16]
	ldr r2, [sp, #4]
	subs r7, r2, r3
	movs r3, #86
	add r3, r10
	mov r12, r3
	mov r2, r12
	movs r3, #16
	strb r3, [r2]
	adds r2, r1, #0
	cmp r1, #0
	bge .L_080248d0
	negs r2, r1
.L_080248d0:
	adds r3, r7, #0
	cmp r7, #0
	bge .L_080248d8
	negs r3, r7
.L_080248d8:
	cmp r2, r3
	bge .L_080248e4
	movs r3, #18
	mov r2, r12
	strb r3, [r2]
	adds r1, r7, #0
.L_080248e4:
	mov r3, r10
	adds r3, #85
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_08024906
	cmp r1, #0
	bge .L_080248f4
	negs r1, r1
.L_080248f4:
	adds r0, r4, #0
	cmp r0, #0
	bge .L_080248fc
	negs r0, r0
.L_080248fc:
	cmp r1, r0
	bge .L_08024906
	movs r3, #17
	mov r2, r12
	strb r3, [r2]
.L_08024906:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08024914:
	.4byte IwramFillWords + 0x74
.L_08024918:
	.4byte IwramMulQ16
.L_0802491c:
	.4byte IwramRatioMulQ14
