.syntax unified
	.thumb
	.global Func_0815b434
	.thumb_func
Func_0815b434:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	lsrs r3, r1, #31
	adds r3, r1, r3
	asrs r3, r3, #1
	mov r10, r3
	mov r9, r0
	mov r3, r10
	muls r3, r1
	movs r0, #1
	sub sp, #8
	negs r0, r0
	str r2, [sp, #4]
	add r0, r10
	subs r2, r1, #1
	mov r8, r2
	adds r1, r3, #0
	mov r11, r0
	ldr r3, .L_0815b50c
	mov r0, r9
	mov lr, r3
	.2byte 0xf800
	mov r0, r8
	movs r4, #0
	cmp r0, #0
	beq .L_0815b4fe
.L_0815b472:
	lsls r0, r4, #14
	mov r1, r8
	adds r5, r4, #0
	adds r6, r4, #0
	str r4, [sp, #0]
	bl __divsi3
	bl Trig_Cos
	lsls r3, r0, #6
	ldr r2, [sp, #4]
	subs r3, r3, r0
	asrs r1, r3, #16
	adds r3, r2, #0
	muls r3, r1
	movs r7, #0
	ldr r4, [sp, #0]
	cmp r3, #0
	bge .L_0815b4a0
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r3, r0
.L_0815b4a0:
	asrs r1, r3, #16
	cmp r1, #63
	ble .L_0815b4a8
	movs r1, #63
.L_0815b4a8:
	cmp r4, #0
	blt .L_0815b4f8
.L_0815b4ac:
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	mov r0, r11
	subs r2, r0, r3
	mov r0, r8
	subs r3, r0, r7
	mov r0, r10
	muls r0, r3
	adds r3, r0, #0
	adds r3, r3, r2
	mov r2, r9
	strb r1, [r2, r3]
	lsrs r3, r7, #31
	adds r3, r7, r3
	asrs r3, r3, #1
	mov r0, r11
	subs r2, r0, r3
	mov r0, r8
	subs r3, r0, r5
	mov r0, r10
	muls r0, r3
	adds r3, r0, #0
	adds r3, r3, r2
	mov r2, r9
	strb r1, [r2, r3]
	lsls r3, r7, #1
	subs r3, r6, r3
	subs r6, r3, #1
	cmp r6, #0
	bge .L_0815b4f2
	lsls r3, r5, #1
	adds r3, r6, r3
	subs r6, r3, #2
	subs r5, #1
.L_0815b4f2:
	adds r7, #1
	cmp r5, r7
	bge .L_0815b4ac
.L_0815b4f8:
	adds r4, #1
	cmp r4, r8
	bne .L_0815b472
.L_0815b4fe:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0815b50c:
	.4byte IwramClearWords
