.syntax unified
	.thumb
	.global Func_080e2718
	.thumb_func
Func_080e2718:
	push {r5, r6, r7, lr}
	ldr r1, .L_080e2770
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #76
	adds r2, r1, r3
	movs r3, #0
	ldrsb r3, [r2, r3]
	movs r6, #0
	sub sp, #8
	cmp r6, r3
	bge .L_080e276c
	movs r3, #184
	lsls r3, r3, #5
	adds r3, #76
	adds r7, r2, #0
	adds r5, r1, r3
.L_080e273a:
	ldrb r3, [r5, #2]
	ldrb r0, [r5]
	ldrb r1, [r5, #1]
	str r3, [sp, #0]
	movs r2, #1
	ldrb r3, [r5, #3]
	adds r6, #1
	str r3, [sp, #4]
	movs r3, #1
	bl Func_080201f0
	ldrb r0, [r5, #4]
	ldrb r1, [r5, #5]
	lsls r0, r0, #20
	lsls r1, r1, #20
	movs r2, #2
	bl Func_080dbb78
	movs r3, #0
	strb r3, [r0, #2]
	movs r3, #0
	ldrsb r3, [r7, r3]
	adds r5, #8
	cmp r6, r3
	blt .L_080e273a
.L_080e276c:
	add sp, #8
	pop {r5, r6, r7, pc}
.L_080e2770:
	.4byte Data_02001000
