.syntax unified
	.thumb
	.set sub_08002064, 0x08002064
	.set sub_08013560, 0x08013560
	.set sub_08013b30, 0x08013b30
	.set sub_081c0cb0, 0x081c0cb0
	.global Func_081c1014
	.thumb_func
Func_081c1014:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	ldr	r3, [pc, #284]
	mov	r9, sp
	mov	r2, r9
	ldmia	r3!, {r0, r1, r4}
	stmia	r2!, {r0, r1, r4}
	movs	r0, #3
	movs	r7, #0
	mov	fp, r0
	ldr	r3, [pc, #272]
	str	r7, [r3, #0]
	ldr	r2, [pc, #272]
	movs	r1, #20
	ldrb	r3, [r2, #2]
	movs	r3, #1
	strb	r3, [r2, #2]
	movs	r2, #0
	mov	sl, r1
	mov	r8, r2
	mov	r6, r9
.L_081c104a:
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_081c1056
	movs	r4, #1
	negs	r4, r4
	add	sl, r4
.L_081c1056:
	ldr	r2, [pc, #248]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_081c1066
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r0, #2
	mov	sl, r0
.L_081c1066:
	ldr	r5, [pc, #236]
	movs	r2, #4
	ldr	r3, [r5, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081c1082
	mov	r0, fp
	adds	r0, #1
	movs	r1, #5
	bl	sub_08002064
	mov	fp, r0
	bl	sub_08013b30
.L_081c1082:
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081c1094
	ldr	r3, [r6, #0]
	adds	r3, #10
	str	r3, [r6, #0]
.L_081c1094:
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081c10a6
	ldr	r3, [r6, #0]
	subs	r3, #10
	str	r3, [r6, #0]
.L_081c10a6:
	ldr	r3, [r5, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081c10b6
	ldr	r3, [r6, #0]
	adds	r3, #1
	str	r3, [r6, #0]
.L_081c10b6:
	ldr	r3, [r5, #12]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081c10c6
	ldr	r3, [r6, #0]
	subs	r3, #1
	str	r3, [r6, #0]
.L_081c10c6:
	ldr	r3, [r5, #12]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081c10de
	cmp	r7, #0
	ble.n	.L_081c10de
	movs	r1, #4
	negs	r1, r1
	subs	r6, #4
	add	r8, r1
	subs	r7, #1
.L_081c10de:
	ldr	r5, [pc, #116]
	movs	r2, #128
	ldr	r3, [r5, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081c10f6
	cmp	r7, #1
	bgt.n	.L_081c10f6
	movs	r2, #4
	adds	r6, #4
	add	r8, r2
	adds	r7, #1
.L_081c10f6:
	ldr	r3, [r5, #12]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081c110a
	mov	r3, r8
	mov	r4, r9
	ldr	r0, [r3, r4]
	bl	sub_081c0cb0
.L_081c110a:
	ldr	r3, [r5, #12]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081c111a
	movs	r0, #0
	bl	sub_081c0cb0
.L_081c111a:
	ldr	r3, [r5, #12]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081c112a
	movs	r0, #78
	bl	sub_081c0cb0
.L_081c112a:
	ldr	r3, [r5, #12]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081c113c
	movs	r0, #195
	lsls	r0, r0, #1
	bl	sub_081c0cb0
.L_081c113c:
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_081c104a
	.4byte 0x081c3430
	.4byte 0x03007804
	.4byte 0x03001138
	.4byte 0x03000ee4
	.2byte 0x1150
	.2byte 0x0300
	push	{lr}
	movs	r3, #128
	ands	r3, r0
	movs	r2, #127
	ands	r0, r2
	cmp	r3, #0
	beq.n	.L_081c1170
	ldr	r2, [pc, #16]
	ldrb	r3, [r2, #0]
	eors	r0, r3
	strb	r0, [r2, #0]
	b.n	.L_081c1174
.L_081c1170:
	ldr	r3, [pc, #4]
	strb	r0, [r3, #0]
.L_081c1174:
	pop	{pc}
	movs	r0, r0
	.2byte 0x5840
	.2byte 0x0200
	ldr	r3, [pc, #4]
	ldrh	r0, [r3, #0]
	bx	lr
	movs	r0, r0
	.4byte 0x02005830
