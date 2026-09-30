.syntax unified
	.thumb
	.global Func_0802b450
	.thumb_func
Func_0802b450:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r2
	lsls r1, r1, #7
	ldr r2, .L_0802b578
	adds r1, r1, r0
	sub sp, #36
	lsls r1, r1, #2
	adds r1, r1, r2
	ldr r6, [sp, #72]
	str r1, [sp, #8]
	ldr r1, [sp, #68]
	adds r4, r3, #0
	lsls r3, r6, #7
	adds r3, r3, r1
	lsls r3, r3, #2
	adds r3, r3, r2
	str r3, [sp, #4]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r5, #132
	lsls r5, r5, #1
	add r1, sp, #12
	adds r2, r3, r5
	mov r9, r1
	movs r5, #2
.L_0802b490:
	ldr r3, [r2]
	subs r5, #1
	asrs r3, r3, #20
	str r3, [r1]
	ldr r3, [r2, #4]
	adds r2, #56
	asrs r3, r3, #20
	str r3, [r1, #4]
	adds r1, #8
	cmp r5, #0
	bge .L_0802b490
	adds r3, r6, r4
	cmp r6, r3
	bge .L_0802b56a
	str r3, [sp, #0]
	mov r1, r11
	movs r3, #128
	subs r3, r3, r1
	lsls r3, r3, #2
	mov r8, r3
.L_0802b4b8:
	ldr r4, [sp, #68]
	mov r2, r11
	adds r3, r4, r2
	cmp r4, r3
	bge .L_0802b556
	mov lr, r3
	adds r7, r6, #0
	movs r3, #15
	mov r10, r3
	ands r7, r3
.L_0802b4cc:
	ldr r1, [sp, #8]
	movs r3, #240
	ldmia r1!, {r2}
	lsls r3, r3, #4
	adds r5, r1, #0
	adds r3, #255
	ldr r1, [sp, #4]
	str r5, [sp, #8]
	mov r12, r3
	movs r3, #224
	lsls r3, r3, #3
	mov r5, r12
	adds r3, #255
	ands r5, r2
	ands r3, r2
	ldr r2, [r1]
	ldr r1, .L_0802b57c
	mov r12, r5
	ldr r5, [sp, #4]
	ands r2, r1
	orrs r3, r2
	stmia r5!, {r3}
	mov r3, r10
	adds r2, r5, #0
	str r2, [sp, #4]
	adds r2, r4, #0
	ands r2, r3
	lsls r3, r7, #5
	adds r3, r3, r2
	mov r1, r9
	movs r5, #0
	lsls r0, r3, #2
.L_0802b50c:
	ldr r3, [r1]
	cmp r3, r4
	bgt .L_0802b542
	adds r3, #16
	cmp r3, r4
	ble .L_0802b542
	ldr r3, [r1, #4]
	cmp r3, r6
	bgt .L_0802b542
	adds r3, #12
	cmp r3, r6
	ble .L_0802b542
	ldr r5, .L_0802b580
	mov r2, r12
	adds r1, r0, r5
	ldr r5, .L_0802b584
	lsls r3, r2, #3
	adds r2, r3, r5
	ldr r2, [r2]
	str r2, [r1]
	ldr r1, .L_0802b588
	adds r2, r3, r1
	ldr r3, .L_0802b58c
	adds r1, r0, r3
	ldr r3, [r2]
	str r3, [r1]
	b .L_0802b550
.L_0802b542:
	movs r2, #128
	lsls r2, r2, #4
	adds r5, #1
	adds r0, r0, r2
	adds r1, #8
	cmp r5, #2
	ble .L_0802b50c
.L_0802b550:
	adds r4, #1
	cmp r4, lr
	blt .L_0802b4cc
.L_0802b556:
	ldr r3, [sp, #8]
	ldr r5, [sp, #4]
	ldr r1, [sp, #0]
	add r3, r8
	add r5, r8
	adds r6, #1
	str r3, [sp, #8]
	str r5, [sp, #4]
	cmp r6, r1
	blt .L_0802b4b8
.L_0802b56a:
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0802b578:
	.4byte gMapCellBuffer
.L_0802b57c:
	.4byte 0xfffff800
.L_0802b580:
	.4byte 0x06002800
.L_0802b584:
	.4byte gMapBlocks
.L_0802b588:
	.4byte Data_02020004
.L_0802b58c:
	.4byte 0x06002840
