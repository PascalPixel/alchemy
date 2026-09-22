.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_080148e8, 0x080148e8
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014de4, 0x08014de4
	.set sub_08014e1c, 0x08014e1c
	.set sub_080156e8, 0x080156e8
	.set sub_080156f8, 0x080156f8
	.set sub_08016ce4, 0x08016ce4
	.set sub_0802254c, 0x0802254c
	.global Overlay_08023d80
Overlay_08023d80:
.L_08023d80:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	movs	r3, #24
	movs	r0, #108
	adds	r3, r3, r5
	adds	r0, #255
	sub	sp, #4
	mov	sl, r1
	mov	r8, r3
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_08023da4
	ldr	r3, [pc, #112]
	mov	r8, r3
.L_08023da4:
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	movs	r2, #15
	ands	r2, r3
	cmp	r2, #1
	beq.n	.L_08023dbc
	cmp	r2, #1
	ble.n	.L_08023e0a
	cmp	r2, #2
	beq.n	.L_08023de4
	b.n	.L_08023e0a
.L_08023dbc:
	adds	r3, r5, #0
	adds	r3, #35
	ldrb	r2, [r3, #0]
	movs	r3, #16
	adds	r1, r5, #0
	ands	r3, r2
	adds	r1, #8
	ldr	r0, [r5, #80]
	cmp	r3, #0
	bne.n	.L_08023e0a
	adds	r2, r5, #0
	adds	r2, #34
	ldrb	r2, [r2, #0]
	ldrh	r3, [r5, #6]
	str	r2, [sp, #0]
	add	r3, sl
	mov	r2, r8
	bl	sub_0802254c
	b.n	.L_08023e0a
.L_08023de4:
	ldr	r6, [r5, #80]
	movs	r7, #3
.L_08023de8:
	ldmia	r6!, {r0}
	cmp	r0, #0
	beq.n	.L_08023e04
	adds	r2, r5, #0
	adds	r2, #34
	ldrb	r2, [r2, #0]
	ldrh	r3, [r5, #6]
	adds	r1, r5, #0
	str	r2, [sp, #0]
	add	r3, sl
	adds	r1, #8
	mov	r2, r8
	bl	sub_0802254c
.L_08023e04:
	subs	r7, #1
	cmp	r7, #0
	bge.n	.L_08023de8
.L_08023e0a:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0xeb90
	.2byte 0x0802
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #48]
	ldr	r3, [r3, #24]
	sub	sp, #12
	mov	r8, r3
	ldr	r1, [pc, #184]
	movs	r0, #80
	bl	sub_08014cc0
	ldr	r2, [pc, #180]
	adds	r1, r0, #0
	ldr	r0, [pc, #180]
	movs	r4, #132
	subs	r2, r2, r0
	movs	r3, #128
	lsls	r4, r4, #24
	lsrs	r2, r2, #2
	lsls	r3, r3, #19
	adds	r3, #212
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, r7, #0
	ldr	r3, [r5, #24]
	adds	r6, r5, #0
	adds	r6, #12
	cmp	r3, #0
	beq.n	.L_08023e5a
	adds	r5, r3, #0
.L_08023e5a:
	ldr	r3, [r7, #28]
	cmp	r3, #0
	beq.n	.L_08023e62
	adds	r6, r3, #0
.L_08023e62:
	ldr	r3, [r6, #0]
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #8]
	subs	r0, r0, r3
	ldr	r3, [r6, #8]
	asrs	r0, r0, #16
	subs	r1, r1, r3
	asrs	r1, r1, #16
	bl	sub_080148e8
	lsls	r0, r0, #16
	asrs	r7, r0, #16
	movs	r3, #0
	mov	r2, r8
	movs	r0, #108
	strh	r3, [r2, #0]
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_08023ea6
	bl	sub_08014de4
	ldr	r3, [pc, #92]
	ldr	r0, [pc, #96]
	adds	r7, r7, r3
	ldr	r3, [pc, #96]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c28
	adds	r1, r6, #0
	bl	sub_080156f8
	b.n	.L_08023eb2
.L_08023ea6:
	bl	sub_08014e1c
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_080156e8
.L_08023eb2:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #20]
	movs	r2, #252
	lsls	r2, r2, #5
	adds	r5, r5, r2
	movs	r6, #63
.L_08023ec0:
	ldrh	r3, [r5, #2]
	cmp	r3, #0
	beq.n	.L_08023ece
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	.L_08023d80
.L_08023ece:
	subs	r6, #1
	subs	r5, #128
	cmp	r6, #0
	bge.n	.L_08023ec0
	movs	r0, #80
	bl	sub_0801314c
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x000002c8
	.4byte 0x08020f90
	.4byte 0x08020da8
	.4byte 0xffffe000
	.4byte 0x0802eb98
	.2byte 0x03c8
	.2byte 0x0300
	movs	r2, #4
	ldrsh	r3, [r0, r2]
	ldr	r2, [r0, #0]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r3, [r3, #4]
	str	r3, [r0, #0]
	movs	r3, #0
	strh	r3, [r0, #4]
	movs	r0, #1
	bx	lr
