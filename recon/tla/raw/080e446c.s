.syntax unified
	.thumb
	.global Func_080e446c
	.thumb_func
Func_080e446c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	mov r9, r1
	movs r1, #128
	str r0, [sp, #0]
	mov r11, r2
	ldr r3, .L_080e4508
	movs r2, #32
	mov r0, r9
	lsls r1, r1, #3
	mov r10, r2
	mov r8, r2
	mov lr, r3
	.2byte 0xf800
	movs r6, #0
.L_080e4496:
	lsls r0, r6, #13
	bl Trig_Sin
	movs r3, #128
	lsls r0, r0, #3
	lsls r3, r3, #12
	adds r3, r3, r0
	movs r5, #0
	mov r12, r3
	cmp r5, r8
	bge .L_080e44f4
	movs r3, #166
	lsls r3, r3, #9
	adds r3, #204
	mov r7, r11
	muls r7, r3
	ldr r2, .L_080e450c
	adds r3, r7, #0
	adds r0, r3, r2
	ldr r3, [sp, #0]
	mov r7, r9
	adds r1, r6, r3
	adds r4, r6, r7
.L_080e44c4:
	mov r2, r12
	subs r3, r0, r2
	asrs r3, r3, #16
	cmp r3, #0
	bge .L_080e44d4
	ldrb r3, [r1]
	strb r3, [r4]
	b .L_080e44e4
.L_080e44d4:
	adds r3, r3, r5
	cmp r3, r8
	bge .L_080e44e4
	ldrb r2, [r1]
	lsls r3, r3, #5
	adds r3, r3, r6
	mov r7, r9
	strb r2, [r7, r3]
.L_080e44e4:
	movs r2, #128
	lsls r2, r2, #11
	adds r5, #1
	add r1, r10
	add r4, r10
	adds r0, r0, r2
	cmp r5, r8
	blt .L_080e44c4
.L_080e44f4:
	adds r6, #1
	cmp r6, r10
	blt .L_080e4496
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080e4508:
	.4byte IwramClearWords
.L_080e450c:
	.4byte 0xff840000
