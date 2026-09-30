.syntax unified
	.thumb
	.global Func_0815b510
	.thumb_func
Func_0815b510:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r1, #0
	subs r1, r6, #1
	lsrs r3, r6, #31
	mov r9, r1
	adds r3, r6, r3
	adds r1, r6, #0
	muls r1, r6
	asrs r3, r3, #1
	sub sp, #8
	subs r3, #1
	str r2, [sp, #4]
	str r3, [sp, #0]
	lsls r1, r1, #1
	ldr r3, .L_0815b630
	adds r7, r0, #0
	mov lr, r3
	.2byte 0xf800
	movs r2, #0
	mov r3, r9
	mov r11, r2
	cmp r3, #0
	beq .L_0815b622
.L_0815b54a:
	movs r1, #0
	mov r5, r11
	mov r10, r1
	lsls r0, r5, #14
	mov r1, r9
	bl Math_Div
	bl Trig_Cos
	lsls r3, r0, #6
	ldr r2, [sp, #4]
	subs r3, r3, r0
	asrs r4, r3, #16
	adds r3, r2, #0
	muls r3, r4
	mov r8, r11
	cmp r3, #0
	bge .L_0815b576
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
.L_0815b576:
	asrs r4, r3, #16
	cmp r4, #63
	ble .L_0815b57e
	movs r4, #63
.L_0815b57e:
	mov r2, r11
	cmp r2, #0
	blt .L_0815b61a
	lsls r3, r6, #1
	mov r12, r3
.L_0815b588:
	ldr r1, [sp, #0]
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	subs r0, r1, r3
	mov r3, r9
	mov r1, r10
	subs r2, r3, r1
	adds r1, r6, #0
	muls r1, r2
	adds r3, r1, r0
	strb r4, [r7, r3]
	mov r3, r12
	subs r2, r3, r2
	subs r3, r2, #1
	adds r2, r6, #0
	muls r2, r3
	adds r3, r2, r0
	strb r4, [r7, r3]
	subs r3, r6, r0
	adds r1, r1, r3
	adds r2, r2, r3
	adds r1, r7, r1
	adds r2, r7, r2
	subs r1, #1
	subs r2, #1
	strb r4, [r1]
	strb r4, [r2]
	mov r1, r10
	ldr r2, [sp, #0]
	lsrs r3, r1, #31
	add r3, r10
	asrs r3, r3, #1
	subs r0, r2, r3
	mov r3, r9
	subs r2, r3, r5
	adds r1, r6, #0
	muls r1, r2
	adds r3, r1, r0
	strb r4, [r7, r3]
	mov r3, r12
	subs r2, r3, r2
	subs r3, r2, #1
	adds r2, r6, #0
	muls r2, r3
	adds r3, r2, r0
	strb r4, [r7, r3]
	subs r3, r6, r0
	adds r1, r1, r3
	adds r2, r2, r3
	adds r1, r7, r1
	subs r1, #1
	adds r2, r7, r2
	strb r4, [r1]
	subs r2, #1
	mov r1, r10
	strb r4, [r2]
	lsls r3, r1, #1
	mov r2, r8
	subs r3, r2, r3
	subs r3, #1
	mov r8, r3
	cmp r3, #0
	bge .L_0815b612
	lsls r3, r5, #1
	add r3, r8
	subs r3, #2
	mov r8, r3
	subs r5, #1
.L_0815b612:
	movs r3, #1
	add r10, r3
	cmp r5, r10
	bge .L_0815b588
.L_0815b61a:
	movs r1, #1
	add r11, r1
	cmp r11, r9
	bne .L_0815b54a
.L_0815b622:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0815b630:
	.4byte IwramClearWords
