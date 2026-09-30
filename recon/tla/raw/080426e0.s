.syntax unified
	.thumb
	.global Func_080426e0
	.thumb_func
Func_080426e0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	adds r5, r0, #0
	mov r12, r3
	movs r3, #0
	mov r11, r3
	ldrh r3, [r5, #8]
	movs r2, #1
	subs r3, #1
	mov r9, r3
	adds r3, r1, #0
	mov r7, r12
	ands r3, r2
	sub sp, #4
	ldrb r4, [r7, #5]
	ldrh r6, [r5, #10]
	cmp r3, #0
	bne .L_0804271a
	movs r3, #3
	negs r3, r3
	ands r1, r3
.L_0804271a:
	movs r3, #2
	ands r3, r1
	cmp r3, #0
	beq .L_08042728
	movs r1, #5
	mov r11, r1
	movs r2, #0
.L_08042728:
	ldr r7, .L_08042788
	adds r0, r2, #0
	adds r2, r7, #0
	ldrsb r3, [r2, r0]
	cmp r3, #0
	blt .L_080427a6
.L_08042734:
	ldrsb r3, [r2, r0]
	mov r2, r11
	adds r4, r3, r2
	cmp r4, r9
	bcs .L_08042798
	movs r1, #0
	cmp r6, #0
	beq .L_08042798
	movs r3, #240
	lsls r3, r3, #8
	subs r7, r6, #1
	movs r2, #240
	adds r3, #24
	lsls r2, r2, #8
	str r7, [sp, #0]
	mov r10, r3
	adds r2, #25
	subs r3, #9
	mov r8, r2
	mov lr, r3
.L_0804275c:
	movs r7, #14
	ldrsh r2, [r5, r7]
	movs r7, #12
	ldrsh r3, [r5, r7]
	adds r2, r2, r1
	lsls r2, r2, #6
	adds r3, r3, r4
	add r2, r12
	lsls r3, r3, #1
	adds r2, r2, r3
	adds r2, #8
	cmp r1, #0
	bne .L_0804277c
	mov r3, r10
	strh r3, [r2]
	b .L_08042790
.L_0804277c:
	ldr r7, [sp, #0]
	cmp r1, r7
	bne .L_0804278c
	mov r3, r8
	strh r3, [r2]
	b .L_08042790
.L_08042788:
	.4byte Data_0805f574
.L_0804278c:
	mov r7, lr
	strh r7, [r2]
.L_08042790:
	adds r1, #1
	cmp r1, r6
	bne .L_0804275c
	ldr r7, .L_080427f0
.L_08042798:
	adds r0, #1
	adds r2, r7, #0
	ldrsb r3, [r2, r0]
	cmp r3, #0
	bge .L_08042734
	mov r1, r12
	ldrb r4, [r1, #5]
.L_080427a6:
	adds r3, r4, #0
	cmp r3, #0
	beq .L_080427f4
	movs r3, #14
	ldrsh r2, [r5, r3]
	ldrh r3, [r5, #10]
	movs r4, #1
	adds r2, r2, r3
	movs r7, #12
	ldrsh r3, [r5, r7]
	lsls r2, r2, #6
	add r2, r12
	lsls r3, r3, #1
	adds r1, r2, r3
	ldr r3, .L_080427e4
	adds r2, r1, #0
	subs r2, #56
	strh r3, [r2]
	adds r2, #2
	cmp r4, r9
	bcs .L_080427dc
	ldr r3, .L_080427e8
.L_080427d2:
	adds r4, #1
	strh r3, [r2]
	adds r2, #2
	cmp r4, r9
	bcc .L_080427d2
.L_080427dc:
	ldr r3, .L_080427ec
	strh r3, [r2]
	b .L_080427f4
	.2byte 0x0000
.L_080427e4:
	.4byte 0x0000f080
.L_080427e8:
	.4byte 0x0000f081
.L_080427ec:
	.4byte 0x0000f082
.L_080427f0:
	.4byte Data_0805f574
.L_080427f4:
	movs r3, #1
	mov r1, r12
	strb r3, [r1, #3]
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
