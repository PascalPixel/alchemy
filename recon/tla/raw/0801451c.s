.syntax unified
	.thumb
	.global Func_0801451c
	.thumb_func
Func_0801451c:
.L_0801451c:
	push	{r5, r6, lr}
	sub	sp, #8
	ldr	r2, [pc, #68]
	movs	r4, #23
	b.n	.L_08014528
.L_08014526:
	ldr	r2, [pc, #64]
.L_08014528:
	adds	r1, r2, #0
	cmp	r4, #0
	ble.n	.L_0801455c
	adds	r0, r4, #0
.L_08014530:
	movs	r3, #12
	ldrsh	r2, [r1, r3]
	movs	r5, #4
	ldrsh	r3, [r1, r5]
	cmp	r2, r3
	ble.n	.L_08014554
	adds	r3, r1, #0
	mov	r2, sp
	ldmia	r3!, {r5, r6}
	stmia	r2!, {r5, r6}
	adds	r2, r1, #0
	adds	r1, r3, #0
	ldmia	r3!, {r5, r6}
	stmia	r2!, {r5, r6}
	mov	r3, sp
	ldmia	r3!, {r5, r6}
	stmia	r2!, {r5, r6}
	b.n	.L_08014556
.L_08014554:
	adds	r1, #8
.L_08014556:
	subs	r0, #1
	cmp	r0, #0
	bne.n	.L_08014530
.L_0801455c:
	subs	r4, #1
	cmp	r4, #1
	bgt.n	.L_08014526
	add	sp, #8
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x3610
	.2byte 0x0200
	push	{r5, lr}
	ldr	r4, [pc, #48]
	movs	r5, #1
	negs	r5, r5
	ldr	r3, [pc, #44]
	ldrh	r2, [r3, #0]
	strh	r3, [r3, #0]
	movs	r1, #0
	ldr	r3, [r4, #0]
	cmp	r3, r0
	bne.n	.L_08014586
	movs	r5, #0
	b.n	.L_08014596
.L_08014586:
	adds	r1, #1
	adds	r4, #8
	cmp	r1, #23
	bgt.n	.L_08014596
	ldr	r3, [r4, #0]
	cmp	r3, r0
	bne.n	.L_08014586
	adds	r5, r1, #0
.L_08014596:
	ldr	r3, [pc, #12]
	strh	r2, [r3, #0]
	adds	r0, r5, #0
	pop	{r5, pc}
	movs	r0, r0
	.4byte 0x02003610
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, lr}
	ldr	r3, [pc, #84]
	ldr	r4, [pc, #84]
	ldrb	r3, [r3, #0]
	movs	r5, #1
	negs	r5, r5
	ldr	r3, [pc, #80]
	ldrh	r2, [r3, #0]
	adds	r6, r2, #0
	strh	r3, [r3, #0]
	movs	r2, #0
	ldr	r3, [r4, #0]
	cmp	r3, r0
	bne.n	.L_080145ca
	strh	r1, [r4, #4]
	movs	r5, #0
	b.n	.L_080145dc
.L_080145ca:
	adds	r2, #1
	adds	r4, #8
	cmp	r2, #23
	bgt.n	.L_080145dc
	ldr	r3, [r4, #0]
	cmp	r3, r0
	bne.n	.L_080145ca
	strh	r1, [r4, #4]
	adds	r5, r2, #0
.L_080145dc:
	movs	r3, #1
	negs	r3, r3
	ldr	r4, [pc, #32]
	cmp	r5, r3
	bne.n	.L_08014624
	ldr	r3, [r4, #0]
	movs	r2, #0
	cmp	r3, #0
	bne.n	.L_0801460c
	ldr	r3, [pc, #12]
	str	r0, [r4, #0]
	strh	r1, [r4, #4]
	strb	r3, [r4, #6]
	movs	r5, #0
	b.n	.L_08014624
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001108
	.4byte 0x02003610
	.2byte 0x0208
	.2byte 0x0400
.L_0801460c:
	adds	r2, #1
	adds	r4, #8
	cmp	r2, #23
	bgt.n	.L_08014624
	ldr	r3, [r4, #0]
	cmp	r3, #0
	bne.n	.L_0801460c
	ldr	r3, [pc, #20]
	str	r0, [r4, #0]
	strh	r1, [r4, #4]
	strb	r3, [r4, #6]
	adds	r5, r2, #0
.L_08014624:
	bl	.L_0801451c
	ldr	r3, [pc, #8]
	strh	r6, [r3, #0]
	adds	r0, r5, #0
	b.n	.L_08014638
	.4byte 0x00000000
	.2byte 0x0208
	.2byte 0x0400
.L_08014638:
	pop	{r5, r6, pc}
	.align 2, 0
