.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08016180, 0x08016180
	.set sub_080167d8, 0x080167d8
	.set sub_0801680c, 0x0801680c
	.set sub_08016854, 0x08016854
	.set sub_08016bd8, 0x08016bd8
	.set sub_08016df8, 0x08016df8
	.set sub_081c0010, 0x081c0010
	.global Func_08016bdc
	.thumb_func
Func_08016bdc:
	push	{r5, r6, lr}
	movs	r0, #3
	sub	sp, #4
	bl	sub_081c0010
	bl	sub_08016180
	ldr	r2, [pc, #152]
	ldr	r3, [pc, #152]
	movs	r1, #19
.L_08016bf0:
	subs	r1, #1
	strh	r3, [r2, #0]
	subs	r2, #2
	subs	r3, #1
	cmp	r1, #0
	bge.n	.L_08016bf0
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	ldr	r1, [pc, #136]
	ldr	r2, [pc, #136]
	bl	sub_08016df8
	movs	r0, #3
	bl	sub_080167d8
.L_08016c10:
	ldr	r0, [pc, #120]
	bl	sub_08016854
	ldr	r6, [pc, #124]
.L_08016c18:
	ldr	r3, [r6, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08016c2e
	movs	r0, #128
	movs	r1, #160
	lsls	r0, r0, #20
	lsls	r1, r1, #2
	bl	sub_0801680c
.L_08016c2e:
	ldr	r3, [r6, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08016c42
	movs	r1, #160
	ldr	r0, [pc, #92]
	lsls	r1, r1, #2
	bl	sub_0801680c
.L_08016c42:
	ldr	r3, [r6, #0]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08016c5c
	movs	r5, #156
	lsls	r5, r5, #6
	adds	r5, #15
.L_08016c52:
	subs	r5, #1
	bl	sub_08016bd8
	cmp	r5, #0
	bge.n	.L_08016c52
.L_08016c5c:
	ldr	r3, [pc, #60]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_08016c7a
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #28]
	ldr	r1, [pc, #44]
	adds	r2, #160
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	b.n	.L_08016c10
.L_08016c7a:
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_08016c18
	movs	r0, r0
	.4byte 0x06002426
	.4byte 0xfffff093
	.4byte 0x02010000
	.4byte 0x05000100
	.4byte 0x03001150
	.4byte 0x08001000
	.4byte 0x020055d0
	.2byte 0x1000
	.2byte 0x0600
	push	{lr}
	cmp	r0, #7
	bhi.n	.L_08016cb6
	movs	r3, #166
	lsls	r3, r3, #1
	ldr	r2, [pc, #44]
	muls	r3, r0
	adds	r0, r3, r2
	b.n	.L_08016cda
.L_08016cb6:
	adds	r3, r0, #0
	subs	r3, #128
	cmp	r3, #5
	bhi.n	.L_08016cd8
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #216
	ldr	r2, [r3, #0]
	cmp	r2, #0
	beq.n	.L_08016cd8
	movs	r3, #166
	lsls	r3, r3, #1
	muls	r3, r0
	adds	r3, r2, r3
	ldr	r2, [pc, #12]
	adds	r0, r3, r2
	b.n	.L_08016cda
.L_08016cd8:
	movs	r0, #0
.L_08016cda:
	pop	{pc}
	.4byte 0x02000520
	.2byte 0x5a00
	.2byte 0xffff
	.2byte 0x0503
	movs	r2, #7
	ands	r2, r0
	lsrs	r0, r3, #23
	ldr	r3, [pc, #8]
	ldrb	r0, [r3, r0]
	movs	r3, #1
	asrs	r0, r2
	ands	r0, r3
	bx	lr
	.2byte 0x0040
	.2byte 0x0200
	movs	r3, #7
	ands	r3, r0
	ldr	r1, [pc, #16]
	movs	r2, #1
	lsls	r2, r3
	lsls	r3, r0, #20
	lsrs	r0, r3, #23
	ldrb	r3, [r1, r0]
	orrs	r2, r3
	strb	r2, [r1, r0]
	bx	lr
	movs	r0, r0
	.2byte 0x0040
	.2byte 0x0200
	movs	r3, #7
	ands	r3, r0
	ldr	r1, [pc, #16]
	movs	r2, #1
	lsls	r2, r3
	lsls	r3, r0, #20
	lsrs	r0, r3, #23
	ldrb	r3, [r1, r0]
	bics	r3, r2
	strb	r3, [r1, r0]
	bx	lr
	movs	r0, r0
	.2byte 0x0040
	.2byte 0x0200
	adds	r4, r0, #0
	movs	r3, #7
	ldr	r2, [pc, #28]
	ands	r3, r4
	movs	r1, #1
	lsls	r1, r3
	lsls	r3, r4, #20
	lsrs	r4, r3, #23
	ldrb	r0, [r2, r4]
	adds	r3, r1, #0
	eors	r3, r0
	strb	r3, [r2, r4]
	ldrb	r3, [r2, r4]
	ands	r3, r1
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	bx	lr
	.4byte 0x02000040
