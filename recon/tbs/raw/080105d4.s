.syntax unified
	.thumb
	.global Map_CopyMetatileCellsRect
	.thumb_func
Map_CopyMetatileCellsRect:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #36
	ldr r6, [sp, #72]
	lsls r1, r1, #7
	adds r1, r1, r0
	ldr r0, [sp, #68]
	adds r4, r3, #0
	lsls r3, r6, #7
	adds r3, r3, r0
	mov r11, r2
	ldr r2, .L_080106e8
	lsls r1, r1, #2
	lsls r3, r3, #2
	adds r1, r1, r2
	adds r3, r3, r2
	str r1, [sp, #8]
	str r3, [sp, #4]
	ldr r3, .L_080106ec
	movs r1, #130
	ldr r3, [r3]
	lsls r1, r1, #1
	add r0, sp, #12
	adds r2, r3, r1
	mov r9, r0
	movs r5, #2
.L_08010612:
	ldr r3, [r2]
	asrs r3, r3, #20
	str r3, [r0]
	ldr r3, [r2, #4]
	subs r5, #1
	asrs r3, r3, #20
	str r3, [r0, #4]
	adds r2, #48
	adds r0, #8
	cmp r5, #0
	bge .L_08010612
	adds r3, r6, r4
	cmp r6, r3
	bge .L_080106d4
	str r3, [sp, #0]
	mov r2, r11
	movs r3, #128
	subs r3, r3, r2
	lsls r3, r3, #2
	mov r8, r3
.L_0801063a:
	ldr r1, [sp, #68]
	mov r4, r11
	adds r3, r1, r4
	cmp r1, r3
	bge .L_080106c0
	mov r12, r6
	movs r5, #15
	mov r0, r12
	ands r0, r5
	mov lr, r3
	mov r10, r5
	mov r12, r0
.L_08010652:
	ldr r4, [sp, #8]
	ldmia r4!, {r3}
	ldr r0, [sp, #4]
	ldr r7, .L_080106f0
	adds r2, r4, #0
	str r2, [sp, #8]
	mov r4, r12
	stmia r0!, {r3}
	ands r7, r3
	adds r2, r1, #0
	mov r3, r10
	ands r2, r3
	lsls r3, r4, #5
	adds r5, r0, #0
	adds r3, r3, r2
	str r5, [sp, #4]
	mov r0, r9
	movs r5, #0
	lsls r4, r3, #2
.L_08010678:
	ldr r3, [r0]
	cmp r3, r1
	bgt .L_080106ac
	adds r3, #16
	cmp r3, r1
	ble .L_080106ac
	ldr r3, [r0, #4]
	cmp r3, r6
	bgt .L_080106ac
	adds r3, #12
	cmp r3, r6
	ble .L_080106ac
	ldr r5, .L_080106f4
	adds r0, r4, r5
	ldr r5, .L_080106f8
	lsls r3, r7, #3
	adds r2, r3, r5
	ldr r2, [r2]
	str r2, [r0]
	ldr r0, .L_080106fc
	adds r2, r3, r0
	ldr r3, .L_08010700
	adds r0, r4, r3
	ldr r3, [r2]
	str r3, [r0]
	b .L_080106ba
.L_080106ac:
	movs r2, #128
	lsls r2, r2, #4
	adds r5, #1
	adds r4, r4, r2
	adds r0, #8
	cmp r5, #2
	ble .L_08010678
.L_080106ba:
	adds r1, #1
	cmp r1, lr
	blt .L_08010652
.L_080106c0:
	ldr r3, [sp, #8]
	ldr r4, [sp, #4]
	ldr r5, [sp, #0]
	add r3, r8
	add r4, r8
	adds r6, #1
	str r3, [sp, #8]
	str r4, [sp, #4]
	cmp r6, r5
	blt .L_0801063a
.L_080106d4:
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080106e8:
	.4byte gMapCellBuffer
.L_080106ec:
	.4byte gMapWork
.L_080106f0:
	.4byte 0x00000fff
.L_080106f4:
	.4byte 0x06002800
.L_080106f8:
	.4byte gMapBlocks
.L_080106fc:
	.4byte Data_02020004
.L_08010700:
	.4byte 0x06002840
