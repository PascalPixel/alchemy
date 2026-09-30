.syntax unified
	.thumb
	.global Func_08014220
	.thumb_func
Func_08014220:
	push	{lr}
	ldr	r1, [pc, #24]
	movs	r2, #128
	movs	r0, #0
	lsls	r2, r2, #2
.L_0801422a:
	ldrb	r3, [r1, #0]
	adds	r1, #1
	cmp	r3, #255
	bne.n	.L_08014234
	adds	r0, #1
.L_08014234:
	subs	r2, #1
	cmp	r2, #0
	bne.n	.L_0801422a
	pop	{pc}
	.2byte 0x3410
	.2byte 0x0200
	.global Resource_ClearSlotReferences
	.thumb_func
Resource_ClearSlotReferences:
	push	{r5, lr}
	movs	r4, #0
	cmp	r0, #95
	bhi.n	.L_08014266
	ldr	r2, [pc, #36]
	movs	r1, #128
	movs	r5, #255
	lsls	r1, r1, #2
.L_08014250:
	ldrb	r3, [r2, #0]
	cmp	r3, r0
	bne.n	.L_0801425a
	strb	r5, [r2, #0]
	adds	r4, #1
.L_0801425a:
	subs	r1, #1
	adds	r2, #1
	cmp	r1, #0
	bne.n	.L_08014250
	cmp	r4, #0
	beq.n	.L_0801426c
.L_08014266:
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0801426e
.L_0801426c:
	movs	r0, #0
.L_0801426e:
	pop	{r5, pc}
	.2byte 0x3410
	.2byte 0x0200
	.global Func_08014274
	.thumb_func
Func_08014274:
.L_08014274:
	push	{r5, r6, lr}
	ldr	r3, [pc, #48]
	lsls	r2, r0, #2
	adds	r5, r2, r3
	cmp	r0, #95
	bls.n	.L_08014286
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080142a4
.L_08014286:
	movs	r6, #255
	ldrh	r3, [r5, #2]
	lsls	r6, r6, #8
	adds	r6, #255
	cmp	r3, r6
	beq.n	.L_080142a2
	bl	Resource_ClearSlotReferences
	ldrh	r3, [r5, #2]
	adds	r2, r6, #0
	orrs	r2, r3
	movs	r3, #0
	strh	r2, [r5, #2]
	strh	r3, [r5, #0]
.L_080142a2:
	movs	r0, #0
.L_080142a4:
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x36e0
	.2byte 0x0200