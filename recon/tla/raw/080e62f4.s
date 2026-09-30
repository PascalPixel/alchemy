.syntax unified
	.thumb
	.global Func_080e62f4
	.thumb_func
Func_080e62f4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r8, r1
	movs r1, #128
	adds r5, r2, #0
	ldr r3, .L_080e637c
	movs r2, #32
	mov r11, r0
	lsls r1, r1, #3
	mov r0, r8
	mov r10, r2
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_080e6380
	movs r7, #32
	adds r2, r5, #0
	muls r2, r3
	movs r6, #0
	mov r9, r2
.L_080e6324:
	lsls r0, r6, #13
	bl Trig_Sin
	movs r3, #128
	lsls r0, r0, #3
	lsls r3, r3, #12
	adds r3, r3, r0
	mov lr, r3
	cmp r7, #0
	beq .L_080e636a
	ldr r4, .L_080e6384
	movs r2, #128
	mov r5, r11
	lsls r2, r2, #11
	adds r0, r6, r5
	add r4, r9
	mov r12, r2
	adds r1, r7, #0
.L_080e6348:
	mov r5, lr
	subs r3, r4, r5
	asrs r3, r3, #16
	cmp r3, #0
	blt .L_080e6360
	cmp r3, r7
	bge .L_080e6360
	ldrb r2, [r0]
	lsls r3, r3, #5
	adds r3, r3, r6
	mov r5, r8
	strb r2, [r5, r3]
.L_080e6360:
	subs r1, #1
	add r0, r10
	add r4, r12
	cmp r1, #0
	bne .L_080e6348
.L_080e636a:
	adds r6, #1
	cmp r6, r10
	blt .L_080e6324
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080e637c:
	.4byte IwramClearWords
.L_080e6380:
	.4byte 0x00019999
.L_080e6384:
	.4byte 0xff840000
