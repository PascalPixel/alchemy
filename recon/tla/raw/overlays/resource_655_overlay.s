.syntax unified
	.thumb
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r2, #255
	ldrh	r3, [r0, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	sub	sp, #4
	cmp	r3, r2
	beq.n	.L_0200009c
	adds	r7, r0, #0
.L_0200004e:
	ldrh	r3, [r7, #0]
	mov	r8, r3
	mov	r0, r8
	bl 0x0200bc40
	adds	r5, r0, #0
	adds	r1, r5, #0
	adds	r1, #89
	movs	r2, #2
	ldrsh	r6, [r7, r2]
	ldrb	r2, [r1, #0]
	movs	r3, #4
	ldrsh	r4, [r7, r3]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	movs	r1, #0
	str	r4, [sp, #0]
	bl 0x0200bbf8
	ldr	r4, [sp, #0]
	lsls	r6, r6, #16
	lsls	r4, r4, #16
	lsrs	r4, r4, #16
	lsrs	r6, r6, #16
	adds	r5, #34
	ldrb	r3, [r5, #0]
	adds	r2, r4, #0
	mov	r0, r8
	adds	r1, r6, #0
	adds	r7, #6
	bl 0x02008128
	movs	r2, #255
	ldrh	r3, [r7, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_0200004e
.L_0200009c:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	adds	r5, r0, #0
	mov	r9, r1
	mov	sl, r2
	movs	r1, #255
	ldr	r2, [r3, #0]
	b.n	.L_02000110
.L_020000c0:
	ldrh	r3, [r5, #0]
	movs	r1, #26
	ldrsh	r7, [r2, r1]
	cmp	r7, r3
	bne.n	.L_0200010c
	adds	r0, r7, #0
	bl 0x0200bc40
	adds	r5, #2
	ldrh	r2, [r5, #0]
	mov	r3, sl
	adds	r6, r0, #0
	mov	r8, r2
	ldrh	r5, [r5, #2]
	cmp	r3, #7
	bgt.n	.L_020000e8
	ldr	r3, [r6, #28]
	ldr	r1, [pc, #64]
	adds	r3, r3, r1
	str	r3, [r6, #28]
.L_020000e8:
	mov	r2, r9
	cmp	r2, #1
	bne.n	.L_0200011a
	adds	r0, r5, #0
	bl 0x0200bb90
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r3, [r3, #0]
	adds	r0, r7, #0
	mov	r1, r8
	adds	r2, r5, #0
	bl 0x02008128
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #28]
	b.n	.L_0200011a
.L_0200010c:
	adds	r5, #6
	movs	r1, #255
.L_02000110:
	ldrh	r3, [r5, #0]
	lsls	r1, r1, #8
	adds	r1, #255
	cmp	r3, r1
	bne.n	.L_020000c0
.L_0200011a:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0xe100
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	adds	r5, r3, #0
	mov	r8, r2
	adds	r6, r1, #0
	bl 0x0200bc40
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	lsls	r3, r5, #3
	subs	r3, r3, r5
	movs	r1, #156
	lsls	r1, r1, #1
	lsls	r3, r3, #3
	adds	r3, r3, r1
	ldr	r5, [r2, r3]
	adds	r7, r0, #0
	bl 0x0200bdb0
	lsls	r0, r0, #2
	adds	r5, r5, r0
	mov	r0, r8
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_0200019c
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #0
	strb	r3, [r5, #2]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	cmp	r6, #1
	beq.n	.L_02000188
	cmp	r6, #1
	bcc.n	.L_0200017e
	cmp	r6, #2
	beq.n	.L_02000192
	b.n	.L_020001ca
.L_0200017e:
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200bba0
	b.n	.L_020001ca
.L_02000188:
	adds	r0, r7, #0
	movs	r1, #4
	bl 0x0200bba0
	b.n	.L_020001ca
.L_02000192:
	adds	r0, r7, #0
	movs	r1, #6
	bl 0x0200bba0
	b.n	.L_020001ca
.L_0200019c:
	movs	r3, #255
	strb	r3, [r5, #2]
	cmp	r6, #1
	beq.n	.L_020001b8
	cmp	r6, #1
	bcc.n	.L_020001ae
	cmp	r6, #2
	beq.n	.L_020001c2
	b.n	.L_020001ca
.L_020001ae:
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200bba0
.L_020001b6:
	b.n	.L_020001ca
.L_020001b8:
	adds	r0, r7, #0
	movs	r1, #3
	bl 0x0200bba0
	b.n	.L_020001ca
.L_020001c2:
	adds	r0, r7, #0
	movs	r1, #5
	bl 0x0200bba0
.L_020001ca:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	sub	sp, #20
	str	r3, [sp, #16]
	movs	r2, #255
	ldrh	r3, [r0, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	mov	fp, r0
	cmp	r3, r2
	beq.n	.L_020002b6
.L_020001f6:
	mov	r3, fp
	ldrh	r3, [r3, #0]
	adds	r0, r3, #0
	str	r3, [sp, #12]
	bl 0x0200bc40
	mov	r2, fp
	ldrh	r2, [r2, #2]
	adds	r7, r0, #0
	str	r2, [sp, #8]
	movs	r3, #34
	adds	r3, r3, r7
	adds	r0, r2, #0
	ldrb	r2, [r3, #0]
	mov	r9, r3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	ldr	r2, [sp, #16]
	adds	r0, #1
	ldr	r5, [r2, r3]
	ldr	r2, [pc, #196]
	adds	r3, r5, r2
.L_0200022a:
	ldr	r2, [pc, #196]
	asrs	r3, r3, #2
	adds	r6, r3, r2
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_02000244
	ldr	r0, [sp, #12]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200bc68
	b.n	.L_020002a4
.L_02000244:
	adds	r0, r7, #0
	bl 0x0200bdb0
	mov	r8, r0
	mov	r3, r8
	lsls	r3, r3, #2
	adds	r5, r5, r3
	str	r5, [sp, #4]
	mov	r2, r9
	ldrb	r0, [r2, #0]
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	bl 0x0200bc10
	mov	r3, r9
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	mov	sl, r0
	ldrb	r0, [r3, #0]
	bl 0x0200bbe8
	adds	r5, r0, #0
	ldr	r0, [sp, #12]
	bl 0x0200bcc0
	ldr	r2, [sp, #4]
	movs	r3, #128
	asrs	r5, r5, #19
	strb	r3, [r2, #3]
	adds	r5, #4
	mov	r3, r9
	adds	r2, r5, #0
	ldrb	r0, [r3, #0]
	mov	r1, sl
	bl 0x0200bdd0
	add	r8, r6
	mov	r2, r8
	strb	r0, [r2, #0]
	ldr	r0, [sp, #8]
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_020002a4
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200bba0
.L_020002a4:
	movs	r3, #4
	add	fp, r3
	mov	r2, fp
	ldrh	r3, [r2, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_020001f6
.L_020002b6:
	ldr	r3, [pc, #60]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200bc40
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	bl 0x0200bbe8
	ldr	r3, [r5, #12]
	cmp	r3, r0
	bge.n	.L_020002de
	str	r0, [r5, #20]
	str	r0, [r5, #12]
.L_020002de:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xfdff0000
	.4byte 0x02024000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #12
	adds	r5, r0, #0
	bl 0x0200bd20
	cmp	r0, #0
	beq.n	.L_0200030e
	b.n	.L_0200047e
.L_0200030e:
	ldr	r3, [pc, #376]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200bc40
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r1, #0
	ldr	r3, [r6, #12]
	str	r3, [r0, #4]
	ldr	r3, [r6, #16]
	str	r3, [r0, #8]
	bl 0x0200bdd8
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_0200033a
	b.n	.L_0200047e
.L_0200033a:
	b.n	.L_02000470
.L_0200033c:
	ldrh	r7, [r5, #0]
	adds	r0, r7, #0
	bl 0x0200bc40
	cmp	r0, r8
	beq.n	.L_0200034c
	adds	r5, #4
	b.n	.L_02000470
.L_0200034c:
	ldrh	r5, [r5, #2]
	bl 0x0200bc28
	adds	r0, r5, #0
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_020003ba
	movs	r0, #125
	bl 0x0200bdf8
	adds	r0, r7, #0
	bl 0x0200bc40
	movs	r1, #7
	bl 0x0200bca0
	movs	r0, #2
	bl 0x0200bb40
	movs	r1, #0
	mov	r0, r8
	bl 0x0200bba0
	adds	r0, r7, #0
	bl 0x0200bc40
	movs	r1, #0
	bl 0x0200bca0
	movs	r0, #2
	bl 0x0200bb40
	adds	r0, r7, #0
	bl 0x0200bc40
	movs	r1, #7
	bl 0x0200bca0
	movs	r0, #4
	bl 0x0200bb40
	adds	r0, r7, #0
	bl 0x0200bc40
	movs	r1, #0
	bl 0x0200bca0
	movs	r0, #0
	bl 0x02008974
	adds	r0, r5, #0
	bl 0x0200bb90
	b.n	.L_0200046a
.L_020003ba:
	adds	r5, #1
	mov	sl, r5
	mov	r0, sl
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_0200046a
	adds	r6, #85
	strb	r0, [r6, #0]
	movs	r0, #185
	bl 0x0200bdf8
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #9
	bl 0x0200bc00
	movs	r0, #0
	bl 0x02008974
	movs	r5, #2
	movs	r0, #8
	mov	r7, r8
	bl 0x0200bb40
	negs	r5, r5
	mov	r0, r8
	movs	r1, #2
	adds	r7, #34
	bl 0x0200bba0
	ldrb	r1, [r7, #0]
	adds	r0, r5, #0
	bl 0x02008c4c
	movs	r0, #1
	bl 0x02008974
	movs	r0, #16
	bl 0x0200bb40
	ldrb	r1, [r7, #0]
	adds	r0, r5, #0
	bl 0x02008c4c
	movs	r0, #4
	bl 0x0200bb40
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200bc00
	movs	r0, #8
	bl 0x0200bb40
	movs	r3, #3
	strb	r3, [r6, #0]
	movs	r0, #5
	bl 0x0200bb40
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	mov	r0, r8
	bl 0x0200bbc0
	movs	r0, #2
	bl 0x0200bb40
	movs	r0, #188
	bl 0x0200bdf8
	bl 0x02008aa8
	movs	r0, #20
	bl 0x0200bb40
	mov	r0, sl
	bl 0x0200bb90
.L_0200046a:
	bl 0x0200bc30
	b.n	.L_0200047e
.L_02000470:
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_0200047e
	b.n	.L_0200033c
.L_0200047e:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl 0x0200bc40
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r7, r0, #0
	cmp	r3, r2
	beq.n	.L_02000566
.L_020004aa:
	ldrh	r3, [r5, #0]
	cmp	r3, r6
	beq.n	.L_020004b4
	adds	r5, #4
	b.n	.L_0200055a
.L_020004b4:
	ldrh	r5, [r5, #2]
	bl 0x0200bc28
	adds	r3, r5, #1
	mov	r8, r3
	mov	r0, r8
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_02000554
	movs	r0, #185
	bl 0x0200bdf8
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #9
	bl 0x0200bc00
	movs	r0, #0
	bl 0x02008974
	movs	r0, #8
	bl 0x0200bb40
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200bba0
	adds	r3, r7, #0
	adds	r3, #34
	movs	r0, #4
	ldrb	r1, [r3, #0]
	adds	r2, r6, #0
	negs	r0, r0
	bl 0x02008bc0
	movs	r0, #1
	bl 0x02008974
	movs	r0, #16
	bl 0x0200bb40
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200bc00
	movs	r0, #8
	bl 0x0200bb40
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	adds	r0, r7, #0
	bl 0x0200bbc0
	movs	r0, #2
	bl 0x0200bb40
	movs	r0, #188
	bl 0x0200bdf8
	bl 0x02008aa8
	movs	r0, #20
	bl 0x0200bb40
	adds	r0, r5, #0
	bl 0x0200bb90
	mov	r0, r8
	bl 0x0200bb90
.L_02000554:
	bl 0x0200bc30
	b.n	.L_02000566
.L_0200055a:
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_020004aa
.L_02000566:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200bc40
	movs	r3, #3
	adds	r0, #92
	strb	r3, [r0, #0]
	adds	r0, r5, #0
	bl 0x0200bcc0
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	sub	sp, #16
	ldr	r5, [pc, #324]
	str	r3, [sp, #12]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	mov	fp, r0
	ldr	r1, [r5, #0]
	movs	r0, #8
	bl 0x0200bc78
	ldr	r1, [r5, #0]
	movs	r0, #9
	bl 0x0200bc78
	ldr	r1, [r5, #0]
	movs	r0, #10
	bl 0x0200bc78
	movs	r0, #1
	bl 0x0200bb40
	movs	r0, #8
	bl 0x0200856c
	movs	r0, #9
	bl 0x0200856c
	movs	r0, #10
	bl 0x0200856c
	movs	r1, #0
	movs	r0, #9
	bl 0x0200bc80
	movs	r0, #1
	bl 0x0200bb40
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200bc68
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200bc68
	movs	r2, #0
	movs	r0, #10
	movs	r1, #0
	bl 0x0200bc68
	movs	r0, #1
	bl 0x0200bb40
	b.n	.L_020006be
.L_0200060a:
	mov	r3, fp
	ldrh	r3, [r3, #0]
	mov	r9, r3
	mov	r0, r9
	bl 0x0200bc40
	mov	r2, fp
	ldrh	r2, [r2, #2]
	adds	r5, r0, #0
	str	r2, [sp, #8]
	adds	r7, r5, #0
	adds	r7, #34
	adds	r0, r2, #0
	ldrb	r2, [r7, #0]
	adds	r0, #1
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	ldr	r2, [sp, #12]
	ldr	r6, [r2, r3]
	ldr	r2, [pc, #168]
	adds	r3, r6, r2
	ldr	r2, [pc, #168]
	asrs	r3, r3, #2
	adds	r2, r2, r3
	mov	sl, r2
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #3
	strb	r3, [r2, #0]
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_02000660
	mov	r0, r9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200bc68
	b.n	.L_020006ba
.L_02000660:
	adds	r0, r5, #0
	bl 0x0200bdb0
	mov	r8, r0
	mov	r3, r8
	lsls	r3, r3, #2
	adds	r6, r6, r3
	str	r6, [sp, #4]
	add	r8, sl
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	ldrb	r0, [r7, #0]
	bl 0x0200bc10
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	mov	sl, r0
	ldrb	r0, [r7, #0]
	bl 0x0200bbe8
	adds	r5, r0, #0
	mov	r0, r9
	bl 0x0200bcc0
	ldr	r6, [sp, #4]
	asrs	r5, r5, #19
	movs	r3, #128
	adds	r5, #4
	adds	r2, r5, #0
	strb	r3, [r6, #3]
	ldrb	r0, [r7, #0]
	mov	r1, sl
	bl 0x0200bdd0
	mov	r2, r8
	strb	r0, [r2, #0]
	ldr	r0, [sp, #8]
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_020006ba
	mov	r0, r9
	movs	r1, #9
	bl 0x0200bcd8
.L_020006ba:
	movs	r3, #4
	add	fp, r3
.L_020006be:
	mov	r2, fp
	ldrh	r3, [r2, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_0200060a
	movs	r0, #10
	bl 0x0200bb40
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0xfdff0000
	.2byte 0x4000
	.2byte 0x0202
	push	{r5, lr}
	adds	r5, r1, #0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	bl 0x0200bbc0
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x0200bbc0
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #12
	adds	r6, r0, #0
	bl 0x0200bd20
	cmp	r0, #0
	beq.n	.L_02000720
	b.n	.L_020008e2
.L_02000720:
	ldr	r3, [pc, #460]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200bc40
	adds	r5, r0, #0
	movs	r0, #8
	bl 0x0200bc40
	ldr	r3, [r5, #8]
	adds	r7, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r1, #0
	ldr	r3, [r5, #12]
	str	r3, [r0, #4]
	ldr	r3, [r5, #16]
	str	r3, [r0, #8]
	bl 0x0200bdd8
	mov	sl, r0
	cmp	r0, #0
	bne.n	.L_02000754
	b.n	.L_020008e2
.L_02000754:
	b.n	.L_020008d4
.L_02000756:
	ldrh	r3, [r6, #0]
	mov	r8, r3
	mov	r0, r8
	bl 0x0200bc40
	cmp	r0, sl
	beq.n	.L_02000768
	adds	r6, #4
	b.n	.L_020008d4
.L_02000768:
	ldrh	r6, [r6, #2]
	bl 0x0200bc28
	adds	r0, r6, #0
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_02000802
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200bba0
	mov	r1, sl
	adds	r0, r7, #0
	bl 0x020086ec
	movs	r0, #1
	bl 0x0200bb40
	movs	r0, #125
	bl 0x0200bdf8
	movs	r0, #8
	bl 0x0200bc40
	movs	r1, #7
	bl 0x0200bca0
	movs	r0, #2
	bl 0x0200bb40
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200bba0
	movs	r1, #9
	mov	r0, r8
	bl 0x0200bcd8
	movs	r0, #8
	bl 0x0200bc40
	movs	r1, #0
	bl 0x0200bca0
	movs	r0, #2
	bl 0x0200bb40
	movs	r0, #8
	bl 0x0200bc40
	movs	r1, #7
	bl 0x0200bca0
	movs	r0, #4
	bl 0x0200bb40
	movs	r0, #8
	bl 0x0200bc40
	movs	r1, #0
	bl 0x0200bca0
	movs	r0, #0
	bl 0x02008974
	mov	r0, sl
	adds	r1, r7, #0
	bl 0x020086ec
	movs	r0, #1
	bl 0x0200bb40
	adds	r0, r6, #0
	bl 0x0200bb90
	b.n	.L_020008ce
.L_02000802:
	adds	r6, #1
	mov	r9, r6
	mov	r0, r9
	bl 0x0200bb88
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_020008ce
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200bba0
	mov	r1, sl
	adds	r0, r7, #0
	bl 0x020086ec
	adds	r5, #85
	movs	r0, #1
	bl 0x0200bb40
	strb	r6, [r5, #0]
	movs	r0, #185
	bl 0x0200bdf8
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #9
	bl 0x0200bc00
	movs	r0, #0
	bl 0x02008974
	mov	r8, r5
	movs	r0, #8
	movs	r6, #2
	mov	r5, sl
	bl 0x0200bb40
	negs	r6, r6
	adds	r0, r7, #0
	movs	r1, #2
	adds	r5, #34
	bl 0x0200bba0
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	bl 0x02008c4c
	movs	r0, #1
	bl 0x02008974
	movs	r0, #16
	bl 0x0200bb40
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	bl 0x02008c4c
	movs	r0, #4
	bl 0x0200bb40
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200bc00
	movs	r0, #8
	bl 0x0200bb40
	movs	r3, #3
	mov	r2, r8
	strb	r3, [r2, #0]
	movs	r0, #5
	bl 0x0200bb40
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	adds	r0, r7, #0
	bl 0x0200bbc0
	movs	r0, #2
	bl 0x0200bb40
	movs	r0, #188
	bl 0x0200bdf8
	bl 0x02008aa8
	movs	r0, #20
	bl 0x0200bb40
	mov	r0, r9
	bl 0x0200bb90
.L_020008ce:
	bl 0x0200bc30
	b.n	.L_020008e2
.L_020008d4:
	movs	r2, #255
	ldrh	r3, [r6, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_020008e2
	b.n	.L_02000756
.L_020008e2:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	ldr	r5, [r6, #68]
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, r5, #0
	adds	r3, r3, r7
	movs	r1, #18
	str	r3, [r6, #16]
	bl 0x0200bb38
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02000924
	adds	r3, #15
.L_02000924:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	ldr	r1, [r6, #80]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r1, #18]
	ldrh	r2, [r2, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #18]
	ldr	r3, [pc, #36]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200bc40
	ldr	r3, [r0, #80]
	ldr	r4, [r6, #80]
	ldrb	r3, [r3, #9]
	ldrb	r1, [r4, #9]
	movs	r2, #12
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r4, #9]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #256]
	mov	r8, r0
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl 0x0200bc40
	movs	r2, #0
	adds	r7, r0, #0
	mov	r9, r2
	mov	sl, r2
.L_02000996:
	bl 0x0200bb58
	lsls	r3, r0, #3
	subs	r3, r3, r0
	ldr	r2, [r7, #12]
	lsls	r3, r3, #1
	lsrs	r3, r3, #16
	lsls	r3, r3, #16
	subs	r2, r2, r3
	mov	r3, sl
	lsls	r1, r3, #17
	ldr	r3, [r7, #8]
	ldr	r0, [pc, #212]
	adds	r1, r1, r3
	ldr	r3, [pc, #212]
	adds	r1, r1, r0
	movs	r0, #30
	adds	r2, r2, r3
	adds	r0, #255
	ldr	r3, [r7, #16]
	bl 0x0200bbb0
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02000a6a
	mov	r1, r9
	ldr	r0, [r6, #80]
	bl 0x0200bd80
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	adds	r2, #4
	strb	r3, [r2, #0]
	movs	r1, #0
	mov	r9, r0
	adds	r0, r6, #0
	bl 0x0200bbf8
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200bba0
	adds	r0, r6, #0
	ldr	r1, [pc, #152]
	bl 0x0200bba8
	movs	r3, #179
	lsls	r3, r3, #8
	adds	r3, #51
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	ldr	r1, [r6, #80]
	movs	r0, #13
	ldrb	r3, [r1, #9]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	mov	r2, r8
	strb	r3, [r1, #9]
	cmp	r2, #0
	beq.n	.L_02000a34
	mov	r3, sl
	lsls	r5, r3, #13
	adds	r0, r5, #0
	bl 0x0200bb68
	ldr	r3, [pc, #108]
	ldr	r1, [pc, #108]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x6470
	adds	r0, r5, #0
	bl 0x0200bb60
	b.n	.L_02000a38
.L_02000a34:
	mov	r0, r8
	str	r0, [r6, #68]
.L_02000a38:
	str	r0, [r6, #76]
	bl 0x0200bb58
	movs	r2, #192
	lsls	r0, r0, #14
	lsls	r2, r2, #7
	lsrs	r0, r0, #16
	adds	r0, r0, r2
	negs	r0, r0
	str	r0, [r6, #72]
	bl 0x0200bb58
	ldr	r3, [pc, #68]
	lsls	r0, r0, #9
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r3, r6, #0
	adds	r3, #100
	strh	r0, [r3, #0]
	ldr	r3, [pc, #60]
	str	r3, [r6, #48]
	ldr	r3, [pc, #60]
	str	r3, [r6, #52]
	ldr	r3, [pc, #60]
	str	r3, [r6, #108]
.L_02000a6a:
	movs	r0, #1
	add	sl, r0
	mov	r2, sl
	cmp	r2, #7
	bls.n	.L_02000996
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xfff80000
	.4byte 0xfffe0000
	.4byte 0x0200be00
	.4byte 0x0300021c
	.4byte 0x00013333
	.4byte 0xffffff00
	.4byte 0xfffff800
	.4byte 0xfffffa00
	.2byte 0x88f5
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #244]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200bc40
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02000b9c
	movs	r3, #0
	mov	r9, r3
	mov	sl, r3
.L_02000acc:
	movs	r0, #30
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, #255
	bl 0x0200bbb0
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02000b92
	mov	r1, r9
	ldr	r0, [r7, #80]
	bl 0x0200bd80
	movs	r4, #0
	mov	r8, r4
	adds	r3, r7, #0
	mov	r2, r8
	adds	r3, #85
	strb	r2, [r3, #0]
	adds	r3, #4
	strb	r2, [r3, #0]
	movs	r1, #0
	mov	r9, r0
	adds	r0, r7, #0
	bl 0x0200bbf8
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200bba0
	ldr	r1, [pc, #160]
	adds	r0, r7, #0
	bl 0x0200bba8
	mov	r3, sl
	lsls	r5, r3, #12
	adds	r0, r5, #0
	bl 0x0200bb68
	mov	r4, r8
	str	r4, [r7, #72]
	str	r0, [r7, #68]
	adds	r0, r5, #0
	bl 0x0200bb60
	ldr	r3, [r7, #68]
	str	r0, [r7, #76]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r7, #68]
	bl 0x0200bb58
	lsls	r3, r0, #1
	ldr	r2, [r7, #68]
.L_02000b3a:
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
.L_02000b40:
	adds	r2, r2, r3
	ldr	r3, [pc, #108]
	adds	r2, r2, r3
	str	r2, [r7, #68]
	bl 0x0200bb58
	lsls	r3, r0, #1
	ldr	r2, [r7, #76]
	adds	r3, r3, r0
	ldr	r4, [pc, #96]
	lsls	r3, r3, #13
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	adds	r2, r2, r4
	str	r2, [r7, #76]
	bl 0x0200bb58
	ldr	r2, [pc, #84]
	lsls	r0, r0, #12
	lsrs	r0, r0, #16
	adds	r3, r7, #0
	adds	r0, r0, r2
	adds	r3, #100
	strh	r0, [r3, #0]
	mov	r3, r8
	str	r3, [r7, #48]
	str	r3, [r7, #52]
	ldr	r3, [pc, #68]
	ldr	r0, [r7, #80]
	str	r3, [r7, #108]
	ldr	r3, [r6, #80]
	movs	r1, #12
	ldrb	r3, [r3, #9]
	movs	r4, #13
	ands	r1, r3
	ldrb	r3, [r0, #9]
	negs	r4, r4
	adds	r2, r4, #0
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r0, #9]
.L_02000b92:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_02000acc
.L_02000b9c:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200be30
	.4byte 0xffffa000
	.4byte 0xffffd000
	.4byte 0xfffff800
	.2byte 0x88f5
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	sl, r0
	adds	r0, r2, #0
	adds	r5, r1, #0
	bl 0x0200bc40
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	lsls	r3, r5, #3
	subs	r3, r3, r5
	movs	r1, #156
	lsls	r1, r1, #1
	lsls	r3, r3, #3
	adds	r3, r3, r1
	ldr	r3, [r2, r3]
	ldr	r2, [pc, #88]
	adds	r7, r0, #0
	ldr	r1, [pc, #88]
	adds	r3, r3, r2
	adds	r5, r7, #0
	asrs	r3, r3, #2
	adds	r5, #34
	adds	r6, r3, r1
	ldr	r2, [r7, #16]
	ldr	r1, [r7, #8]
	ldrb	r0, [r5, #0]
	bl 0x0200bc10
	ldr	r2, [r7, #16]
	mov	r8, r0
	ldr	r1, [r7, #8]
	ldrb	r0, [r5, #0]
	bl 0x0200bbe8
	ldr	r3, [r7, #8]
	asrs	r2, r0, #19
	add	r2, sl
	cmp	r3, #0
	bge.n	.L_02000c1a
	ldr	r1, [pc, #48]
	adds	r3, r3, r1
.L_02000c1a:
	ldr	r0, [r7, #16]
	asrs	r1, r3, #20
	cmp	r0, #0
	bge.n	.L_02000c26
	ldr	r3, [pc, #36]
	adds	r0, r0, r3
.L_02000c26:
	asrs	r3, r0, #20
	lsls	r3, r3, #7
	adds	r3, r1, r3
	ldrb	r0, [r5, #0]
	mov	r1, r8
	adds	r6, r6, r3
	bl 0x0200bdd0
	strb	r0, [r6, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0xfdff0000
	.4byte 0x02024000
	.2byte 0xffff
	.2byte 0x000f
	push	{lr}
	ldr	r3, [pc, #16]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r2, [r3, #0]
	bl 0x02008bc0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	sub	sp, #8
	str	r3, [sp, #4]
	movs	r2, #255
	ldrh	r3, [r0, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	mov	fp, r0
	cmp	r3, r2
	beq.n	.L_02000d34
.L_02000c8a:
	mov	r3, fp
	ldrh	r0, [r3, #0]
	bl 0x0200bc40
	mov	r8, r0
	mov	r7, r8
	adds	r7, #34
	mov	r2, r8
	ldr	r1, [r2, #8]
	ldrb	r0, [r7, #0]
	ldr	r2, [r2, #16]
	bl 0x0200bc10
	str	r0, [sp, #0]
	mov	r3, r8
	ldr	r1, [r3, #8]
	ldr	r2, [r3, #16]
	ldrb	r0, [r7, #0]
	bl 0x0200bbe8
	mov	sl, r0
	mov	r2, sl
	asrs	r2, r2, #19
	mov	r0, r8
	mov	sl, r2
	bl 0x0200bdb0
	ldrb	r2, [r7, #0]
	mov	r9, r0
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	ldr	r2, [sp, #4]
	mov	r0, r8
	ldr	r6, [r2, r3]
	ldr	r3, [pc, #148]
	ldr	r2, [pc, #148]
	adds	r5, r6, r3
	asrs	r5, r5, #2
	adds	r5, r5, r2
	mov	r2, r8
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	adds	r2, #4
	strb	r3, [r2, #0]
	movs	r1, #0
	bl 0x0200bbf8
	movs	r3, #4
	add	sl, r3
	mov	r2, sl
	ldrb	r0, [r7, #0]
	ldr	r1, [sp, #0]
	bl 0x0200bdd0
	mov	r2, r9
	lsls	r2, r2, #2
	add	r5, r9
	mov	r9, r2
	add	r6, r9
	ldrb	r3, [r6, #3]
	movs	r2, #192
	orrs	r3, r2
	strb	r3, [r6, #3]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r6, r6, r3
	ldrb	r2, [r6, #3]
	movs	r3, #64
	orrs	r3, r2
	movs	r2, #2
	add	fp, r2
	mov	r2, fp
.L_02000d24:
	strb	r3, [r6, #3]
	ldrh	r3, [r2, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	strb	r0, [r5, #0]
	cmp	r3, r2
	bne.n	.L_02000c8a
.L_02000d34:
	ldr	r3, [pc, #60]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200bc40
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	bl 0x0200bbe8
	ldr	r3, [r5, #12]
	cmp	r3, r0
	bge.n	.L_02000d5c
	str	r0, [r5, #20]
.L_02000d5a:
	str	r0, [r5, #12]
.L_02000d5c:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0xfdff0000
	.4byte 0x02024000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
.L_02000d7e:
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	str	r0, [sp, #4]
	bl 0x0200bdf0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r7, r0, #0
	str	r3, [sp, #0]
	cmp	r7, #0
	bne.n	.L_02000dba
	ldr	r3, [pc, #240]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200bd18
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L_02000dba
	bl 0x0200bc40
	adds	r7, r0, #0
.L_02000dba:
	ldr	r1, [sp, #0]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #188
	adds	r3, r1, r2
	movs	r2, #0
	str	r2, [r3, #0]
	ldr	r1, [sp, #4]
	movs	r2, #255
	ldrh	r3, [r1, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_02000e80
.L_02000dd6:
	ldr	r3, [sp, #4]
	ldrh	r0, [r3, #0]
	bl 0x0200bc40
	cmp	r7, r0
	bne.n	.L_02000e6e
	movs	r1, #34
	adds	r1, r1, r7
	ldrb	r0, [r1, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	lsls	r3, r0, #3
	mov	fp, r1
	subs	r3, r3, r0
	movs	r1, #156
	lsls	r1, r1, #1
	lsls	r3, r3, #3
	adds	r3, r3, r1
	ldr	r2, [r2, r3]
	ldr	r3, [pc, #148]
	mov	sl, r2
	ldr	r2, [pc, #148]
	ldr	r1, [r7, #8]
	add	r2, sl
	asrs	r2, r2, #2
	mov	r8, r2
	ldr	r2, [r7, #16]
	add	r8, r3
	bl 0x0200bc10
	mov	r1, fp
	ldr	r2, [r7, #16]
	mov	r9, r0
	ldrb	r0, [r1, #0]
	ldr	r1, [r7, #8]
	bl 0x0200bbe8
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl 0x0200bdb0
	asrs	r5, r5, #19
	mov	r2, fp
	subs	r5, #4
	adds	r6, r0, #0
	mov	r1, r9
	ldrb	r0, [r2, #0]
	adds	r2, r5, #0
	bl 0x0200bdd0
	add	r8, r6
	lsls	r6, r6, #2
	add	sl, r6
	mov	r1, sl
	ldrb	r2, [r1, #3]
	mov	r3, r8
	strb	r0, [r3, #0]
	movs	r3, #63
	ands	r3, r2
	movs	r2, #128
	strb	r3, [r1, #3]
	lsls	r2, r2, #2
	add	sl, r2
.L_02000e56:
	mov	r3, sl
	ldrb	r2, [r3, #3]
	movs	r3, #191
	ands	r3, r2
	mov	r1, sl
	strb	r3, [r1, #3]
	ldr	r2, [sp, #0]
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #188
	adds	r3, r2, r1
	str	r7, [r3, #0]
.L_02000e6e:
	ldr	r2, [sp, #4]
	movs	r1, #255
.L_02000e72:
	adds	r2, #2
	str	r2, [sp, #4]
	lsls	r1, r1, #8
	ldrh	r3, [r2, #0]
.L_02000e7a:
	adds	r1, #255
	cmp	r3, r1
	bne.n	.L_02000dd6
.L_02000e80:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x02024000
	.2byte 0x0000
	.2byte 0xfdff
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #108]
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #188
	adds	r3, r3, r1
	ldr	r6, [r3, #0]
	cmp	r6, #0
	beq.n	.L_02000f32
	adds	r7, r6, #0
	adds	r7, #34
	ldrb	r0, [r7, #0]
	ldr	r2, [r2, #32]
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r1, #156
	lsls	r1, r1, #1
	lsls	r3, r3, #3
	adds	r3, r3, r1
	ldr	r2, [r2, r3]
	ldr	r3, [pc, #104]
	mov	sl, r2
	ldr	r2, [pc, #104]
	ldr	r1, [r6, #8]
	add	r2, sl
	asrs	r2, r2, #2
	mov	r8, r2
.L_02000ede:
	ldr	r2, [r6, #16]
	add	r8, r3
	bl 0x0200bc10
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	mov	r9, r0
	ldrb	r0, [r7, #0]
	bl 0x0200bbe8
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl 0x0200bdb0
	asrs	r5, r5, #19
	adds	r5, #4
	adds	r6, r0, #0
	mov	r1, r9
	adds	r2, r5, #0
.L_02000f04:
	ldrb	r0, [r7, #0]
	bl 0x0200bdd0
	add	r8, r6
.L_02000f0c:
	lsls	r6, r6, #2
	add	sl, r6
	mov	r3, sl
	ldrb	r2, [r3, #3]
	mov	r1, r8
	movs	r3, #192
	orrs	r3, r2
	strb	r0, [r1, #0]
	movs	r2, #128
	mov	r1, sl
	strb	r3, [r1, #3]
	lsls	r2, r2, #2
	add	sl, r2
	mov	r3, sl
	ldrb	r2, [r3, #3]
	movs	r3, #64
	orrs	r3, r2
	mov	r1, sl
	strb	r3, [r1, #3]
.L_02000f32:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x02024000
	.2byte 0x0000
	.2byte 0xfdff
	.2byte 0xb520
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_02000f88
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000f88
	ldr	r1, [r5, #80]
	movs	r2, #13
	ldrb	r0, [r1, #9]
	movs	r3, #3
	negs	r2, r2
	ands	r4, r3
	adds	r3, r2, #0
	lsls	r4, r4, #2
	ands	r3, r0
	orrs	r3, r4
	strb	r3, [r1, #9]
	adds	r1, #37
	ldrb	r3, [r1, #0]
	ands	r2, r3
	orrs	r2, r4
	strb	r2, [r1, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
.L_02000f88:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x6c426883
	.4byte 0x189b6d01
	.4byte 0x6c826083
	.4byte 0x189b68c3
	.4byte 0x6cc260c3
	.4byte 0x189b6903
	.4byte 0x6b026103
	.4byte 0x189b6983
	.4byte 0x6b426183
	.4byte 0x189b69c3
	.4byte 0x306461c3
	.4byte 0x88028a4b
	.4byte 0x824b189b
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r3
	ldr	r3, [pc, #420]
	sub	sp, #4
	mov	sl, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r1, #0
	ldr	r1, [sp, #44]
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
.L_02000fe8:
	mov	r8, r1
	ldr	r7, [sp, #48]
	bl 0x0200bc40
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_0200100c
	cmp	r7, #0
	beq.n	.L_0200100c
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_02001014
.L_0200100c:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_02001014:
	mov	r3, sl
	bl 0x0200bbb0
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_02001022
	b.n	.L_0200116e
.L_02001022:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl 0x0200bba0
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x0200bba8
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
.L_02001050:
	movs	r1, #0
	bl 0x0200bbf8
	ldr	r3, [pc, #300]
	mov	r1, r9
	str	r3, [r6, #108]
	mov	r3, fp
	str	r3, [r6, #68]
	ldr	r3, [sp, #36]
	adds	r0, r6, #0
.L_02001064:
	str	r3, [r6, #72]
	ldr	r3, [sp, #40]
	str	r3, [r6, #76]
	ldr	r3, [r1, #80]
	ldrb	r1, [r3, #9]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl 0x02008f44
	movs	r2, #100
	adds	r2, r2, r6
	mov	r9, r2
	mov	r3, r9
	str	r5, [r6, #48]
	str	r5, [r6, #52]
	strh	r5, [r3, #0]
	ldr	r3, [pc, #256]
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200116e
	cmp	r7, #0
	beq.n	.L_0200116e
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020010a4
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x0200bca0
.L_020010a4:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020010c4
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
.L_020010c0:
	bl 0x02008f44
.L_020010c4:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_020010d8
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_020010d8:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200111e
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_02001106
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200bb38
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_02001118
.L_02001106:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x0200bb38
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_02001118:
	bl 0x0200bb38
	str	r0, [r6, #52]
.L_0200111e:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200113a
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200bba0
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200bba8
.L_0200113a:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200114c
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_0200114c:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200115e
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_0200115e:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200116e
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_0200116e:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200bf38
	.4byte 0x02008f8d
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	movs	r0, #133
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bb90
	movs	r0, #16
	movs	r1, #47
	bl 0x0200bd08
	pop	{pc}
	.2byte 0x0000
	.global Func_020011a4
	.thumb_func
Func_020011a4:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xc424
	.2byte 0x0200
	.global Func_020011ac
	.thumb_func
Func_020011ac:
	movs	r0, #0
	bx	lr
	.global Func_020011b0
	.thumb_func
Func_020011b0:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xc454
	.2byte 0x0200
	.global Func_020011b8
	.thumb_func
Func_020011b8:
	push	{lr}
.L_020011ba:
	ldr	r3, [pc, #76]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_020011d0
	ldr	r0, [pc, #64]
	b.n	.L_02001204
.L_020011d0:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_020011da
	ldr	r0, [pc, #64]
	b.n	.L_02001204
.L_020011da:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_020011e4
.L_020011e0:
	ldr	r0, [pc, #60]
	b.n	.L_02001204
.L_020011e4:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_020011ee
	ldr	r0, [pc, #60]
	b.n	.L_02001204
.L_020011ee:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_020011f8
	ldr	r0, [pc, #56]
	b.n	.L_02001204
.L_020011f8:
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_02001202
	ldr	r0, [pc, #56]
	b.n	.L_02001204
.L_02001202:
	ldr	r0, [pc, #56]
.L_02001204:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000022
	.4byte 0x0200c5f8
	.4byte 0x00000023
	.4byte 0x0200c7d8
	.4byte 0x00000024
	.4byte 0x0200cac0
	.4byte 0x00000025
	.4byte 0x0200cd78
	.4byte 0x00000026
	.4byte 0x0200cdd8
	.4byte 0x00000027
	.4byte 0x0200ce20
	.2byte 0xc5e0
	.2byte 0x0200
	.global Func_02001240
	.thumb_func
Func_02001240:
	push	{lr}
	ldr	r3, [pc, #76]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_02001258
	ldr	r0, [pc, #64]
	b.n	.L_0200128c
.L_02001258:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_02001262
	ldr	r0, [pc, #64]
	b.n	.L_0200128c
.L_02001262:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_0200126c
	ldr	r0, [pc, #60]
	b.n	.L_0200128c
.L_0200126c:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_02001276
	ldr	r0, [pc, #60]
	b.n	.L_0200128c
.L_02001276:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_02001280
	ldr	r0, [pc, #56]
	b.n	.L_0200128c
.L_02001280:
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_0200128a
	ldr	r0, [pc, #56]
	b.n	.L_0200128c
.L_0200128a:
	ldr	r0, [pc, #56]
.L_0200128c:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000022
	.4byte 0x0200cf1c
	.4byte 0x00000023
	.4byte 0x0200d078
	.4byte 0x00000024
	.4byte 0x0200d1bc
	.4byte 0x00000025
	.4byte 0x0200d3b4
	.4byte 0x00000026
	.4byte 0x0200d450
	.4byte 0x00000027
	.4byte 0x0200d5b8
	.2byte 0xcf10
	.2byte 0x0200
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x020082f8
	pop	{pc}
	.2byte 0x0000
	.2byte 0xbf44
	.2byte 0x0200
	.2byte 0xb500
	movs	r0, #138
	lsls	r0, r0, #1
	adds	r0, #255
	sub	sp, #8
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_020012fe
	movs	r3, #9
	movs	r2, #18
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bbf0
.L_020012fe:
	movs	r0, #139
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_02001320
	movs	r3, #11
	movs	r2, #18
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bbf0
.L_02001320:
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_02001342
	movs	r3, #14
	movs	r2, #28
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bbf0
.L_02001342:
	movs	r0, #146
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_02001364
	movs	r3, #16
	movs	r2, #28
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bbf0
.L_02001364:
	add	sp, #8
	pop	{pc}
.L_02001368:
	push	{r5, lr}
	ldr	r0, [pc, #60]
	bl 0x02008708
	bl 0x020092d8
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl 0x0200bc40
	ldr	r3, [r0, #12]
	movs	r2, #128
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_020013a4
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200bcb8
	ldr	r0, [r5, #0]
	bl 0x0200bc40
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
.L_020013a4:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x0200bf52
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #138
	lsls	r0, r0, #1
	adds	r0, #255
	sub	sp, #8
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_020013d6
	movs	r3, #50
	movs	r2, #20
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #63
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bbf0
.L_020013d6:
	movs	r0, #139
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_020013f8
	movs	r3, #52
	movs	r2, #20
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #63
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bbf0
.L_020013f8:
	add	sp, #8
	pop	{pc}
	push	{r5, lr}
	ldr	r0, [pc, #56]
	bl 0x020082f8
	bl 0x020093b0
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl 0x0200bc40
	ldr	r2, [pc, #40]
	ldr	r3, [r0, #12]
	cmp	r3, r2
	bge.n	.L_02001436
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200bcb8
	ldr	r0, [r5, #0]
	bl 0x0200bc40
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
.L_02001436:
	pop	{r5, pc}
	.4byte 0x0200bf80
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xffe8
	.2byte 0xb500
	ldr	r0, [pc, #36]
	bl 0x020082f8
	ldr	r3, [pc, #32]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200bc40
	ldr	r3, [r0, #12]
	movs	r2, #192
	lsls	r2, r2, #13
	cmp	r3, r2
	bge.n	.L_02001468
	bl 0x0200a180
.L_02001468:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200bf92
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #92]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_02001492
	movs	r1, #13
	movs	r2, #14
	bl 0x02009514
	b.n	.L_020014d0
.L_02001492:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_020014a2
	movs	r1, #25
	movs	r2, #13
	bl 0x02009514
	b.n	.L_020014d0
.L_020014a2:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_020014b2
	movs	r1, #26
	movs	r2, #10
	bl 0x02009514
	b.n	.L_020014d0
.L_020014b2:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_020014c2
	movs	r1, #8
	movs	r2, #3
	bl 0x02009514
	b.n	.L_020014d0
.L_020014c2:
	ldr	r3, [pc, #36]
	cmp	r2, r3
.L_020014c6:
	bne.n	.L_020014d0
	movs	r1, #10
	movs	r2, #6
	bl 0x02009514
.L_020014d0:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000022
	.4byte 0x00000023
	.4byte 0x00000024
	.4byte 0x00000025
	.2byte 0x0027
	.2byte 0x0000
	push	{lr}
	movs	r0, #1
	bl 0x02009474
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200bb90
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #0
	bl 0x02009474
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200bb98
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r2, #0
	movs	r5, #0
	mov	r8, r0
	adds	r7, r1, #0
	cmp	r5, r6
	bcs.n	.L_0200154c
.L_02001526:
	adds	r0, r7, r5
	bl 0x0200bc40
	mov	r3, r8
	adds	r0, #35
	adds	r1, r5, #1
	cmp	r3, #0
	beq.n	.L_0200153e
	ldrb	r2, [r0, #0]
	movs	r3, #239
	ands	r3, r2
	b.n	.L_02001544
.L_0200153e:
	ldrb	r2, [r0, #0]
	movs	r3, #16
	orrs	r3, r2
.L_02001544:
	strb	r3, [r0, #0]
	adds	r5, r1, #0
	cmp	r5, r6
	bcc.n	.L_02001526
.L_0200154c:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{lr}
	adds	r3, r0, #0
	adds	r2, r1, #0
	ldr	r0, [pc, #8]
	adds	r1, r3, #0
	bl 0x020080a4
	pop	{pc}
	.2byte 0xbf98
	.2byte 0x0200
	push	{lr}
	adds	r3, r0, #0
	adds	r2, r1, #0
	ldr	r0, [pc, #8]
	adds	r1, r3, #0
	bl 0x020080a4
	pop	{pc}
	.2byte 0xbfa0
	.2byte 0x0200
	push	{lr}
	adds	r3, r0, #0
	adds	r2, r1, #0
	ldr	r0, [pc, #8]
	adds	r1, r3, #0
	bl 0x020080a4
	pop	{pc}
	.2byte 0xbfa8
	.2byte 0x0200
	push	{lr}
	adds	r3, r0, #0
	adds	r2, r1, #0
	ldr	r0, [pc, #8]
	adds	r1, r3, #0
	bl 0x020080a4
	pop	{pc}
	.2byte 0xbfb0
	.2byte 0x0200
	push	{lr}
	adds	r3, r0, #0
	adds	r2, r1, #0
	ldr	r0, [pc, #8]
	adds	r1, r3, #0
	bl 0x020080a4
	pop	{pc}
	.2byte 0xbfb8
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	adds	r7, r6, #0
	adds	r7, #85
	movs	r3, #3
	strb	r3, [r7, #0]
.L_020015c4:
	movs	r0, #1
.L_020015c6:
	bl 0x0200bb40
	ldr	r5, [r6, #40]
	cmp	r5, #0
	bne.n	.L_020015c4
	movs	r0, #188
	bl 0x0200bdf8
	movs	r0, #10
	bl 0x0200bb40
	strb	r5, [r7, #0]
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	movs	r0, #12
	sub	sp, #8
	bl 0x0200bc40
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	asrs	r6, r3, #20
	cmp	r6, #56
	bne.n	.L_0200162c
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #32
	bne.n	.L_0200162c
	bl 0x0200bc28
	movs	r0, #0
	bl 0x0200bd78
	adds	r0, r5, #0
	bl 0x020095b8
	movs	r3, #64
	str	r3, [sp, #4]
	movs	r0, #54
	movs	r1, #32
	movs	r2, #2
	movs	r3, #1
.L_02001618:
	str	r6, [sp, #0]
	bl 0x0200bbf0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200bb90
	bl 0x0200bc30
.L_0200162c:
	add	sp, #8
	pop	{r5, r6, pc}
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x0200bdc0
	pop	{pc}
	.2byte 0x0000
	.2byte 0xbfc0
	.2byte 0x0200
.L_02001640:
	push	{r5, r6, r7, lr}
	movs	r0, #23
	sub	sp, #8
	bl 0x0200bc40
	adds	r7, r0, #0
	ldr	r3, [r7, #8]
	asrs	r6, r3, #20
	bl 0x0200bdc8
.L_02001654:
	cmp	r6, #52
	bne.n	.L_0200169c
	bl 0x0200bc28
	movs	r0, #0
	bl 0x0200bd78
	movs	r5, #7
	movs	r1, #7
	movs	r2, #1
	movs	r3, #1
	movs	r0, #51
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #1
	bl 0x0200bb40
	adds	r0, r7, #0
	bl 0x020095b8
	movs	r0, #50
	movs	r1, #7
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #132
	lsls	r0, r0, #4
	bl 0x0200bb90
	bl 0x0200bc30
.L_0200169c:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x0200bdc0
	pop	{pc}
	.2byte 0x0000
	.2byte 0xbfc4
	.2byte 0x0200
	.2byte 0xb5e0
	movs	r0, #19
	sub	sp, #8
	bl 0x0200bc40
	adds	r7, r0, #0
	ldr	r3, [r7, #8]
	asrs	r6, r3, #20
	bl 0x0200bdc8
	cmp	r6, #47
	bne.n	.L_0200170e
	bl 0x0200bc28
	movs	r0, #0
	bl 0x0200bd78
	movs	r5, #13
	movs	r1, #12
	movs	r2, #1
	movs	r3, #1
	movs	r0, #47
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #1
	bl 0x0200bb40
	adds	r0, r7, #0
	bl 0x020095b8
	movs	r0, #50
	movs	r1, #16
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
.L_020016fa:
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #19
	movs	r1, #2
	bl 0x0200bcb8
	bl 0x0200bc30
	b.n	.L_02001726
.L_0200170e:
	cmp	r6, #46
	bne.n	.L_02001726
	movs	r3, #48
	movs	r2, #13
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #44
	movs	r1, #13
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bbf0
.L_02001726:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{lr}
	movs	r1, #150
	movs	r2, #134
	movs	r0, #20
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	sub	sp, #8
	bl 0x0200bc68
	movs	r3, #37
	movs	r2, #33
.L_02001742:
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #38
	movs	r1, #32
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bbf0
	movs	r3, #38
	movs	r2, #32
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #30
	movs	r1, #32
	movs	r2, #1
	movs	r3, #2
	bl 0x0200bbf0
	movs	r3, #36
	movs	r2, #64
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #64
	movs	r2, #4
	movs	r3, #3
	movs	r0, #40
	bl 0x0200bbf0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #65
	bl 0x0200bb90
	add	sp, #8
	pop	{pc}
	push	{lr}
	movs	r0, #20
	bl 0x0200bc40
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r3, #37
	bne.n	.L_0200179c
	bl 0x0200972c
.L_0200179c:
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #20
	bl 0x0200bc40
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r3, #38
	bne.n	.L_020017ec
	ldr	r3, [pc, #60]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200bc40
	adds	r5, r0, #0
	bl 0x0200bc28
	movs	r0, #0
	bl 0x0200bd78
	adds	r5, #85
	movs	r3, #0
	strb	r3, [r5, #0]
	movs	r0, #1
	bl 0x0200bb40
	bl 0x0200bde8
	bl 0x0200972c
	movs	r0, #1
	bl 0x0200bb40
	movs	r3, #3
	strb	r3, [r5, #0]
	bl 0x0200bc30
.L_020017ec:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bb98
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r5, #200
	lsls	r5, r5, #2
.L_0200180a:
	adds	r0, r5, #0
	bl 0x0200bb98
	movs	r3, #152
	lsls	r3, r3, #2
	adds	r3, #255
	cmp	r5, r3
	beq.n	.L_0200181e
.L_0200181a:
	adds	r5, #1
	b.n	.L_0200180a
.L_0200181e:
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #116]
	movs	r6, #144
	movs	r3, #0
	sub	sp, #8
	lsls	r6, r6, #2
	mov	r8, r3
	movs	r7, #0
.L_02001834:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_02001868
	adds	r0, r6, #0
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_02001876
	ldr	r3, [pc, #84]
	ldr	r1, [pc, #84]
	adds	r5, r7, r3
	ldrh	r2, [r5, #0]
	adds	r5, #2
	ldrh	r3, [r5, #0]
	ldrh	r0, [r1, #0]
	movs	r4, #1
	ldrh	r1, [r1, #2]
	adds	r5, #2
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	bl 0x0200bbe0
	b.n	.L_02001876
.L_02001868:
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02001874
	adds	r0, r6, #0
	bl 0x0200bb90
.L_02001874:
	adds	r5, #12
.L_02001876:
	movs	r3, #1
	add	r8, r3
	mov	r3, r8
	adds	r6, #1
	adds	r7, #12
	cmp	r3, #13
	bls.n	.L_02001834
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_02001894
	bl 0x02009804
.L_02001894:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0200c066
	.4byte 0x0200c068
	.2byte 0xbfd2
	.2byte 0x0200
	.2byte 0xb5e0
	.2byte 0x4647
	push	{r7}
	ldr	r5, [pc, #116]
	movs	r6, #144
	movs	r3, #0
	sub	sp, #8
	lsls	r6, r6, #2
	mov	r8, r3
	movs	r7, #0
.L_020018bc:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_020018f0
	adds	r0, r6, #0
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_020018fe
	ldr	r3, [pc, #84]
	ldr	r1, [pc, #84]
	adds	r5, r7, r3
	ldrh	r2, [r5, #0]
	adds	r5, #2
	ldrh	r3, [r5, #0]
	ldrh	r0, [r1, #0]
	movs	r4, #1
	ldrh	r1, [r1, #2]
	adds	r5, #2
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	bl 0x0200bbe0
	b.n	.L_020018fe
.L_020018f0:
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_020018fc
	adds	r0, r6, #0
	bl 0x0200bb90
.L_020018fc:
	adds	r5, #12
.L_020018fe:
	movs	r3, #1
	add	r8, r3
	mov	r3, r8
	adds	r6, #1
	adds	r7, #12
	cmp	r3, #7
	bls.n	.L_020018bc
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_0200191c
	bl 0x02009804
.L_0200191c:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0200c10e
	.4byte 0x0200c110
	.2byte 0xbfda
	.2byte 0x0200
	.2byte 0xb5e0
	.2byte 0x4647
	push	{r7}
	ldr	r5, [pc, #116]
	movs	r6, #144
	movs	r3, #0
	sub	sp, #8
	lsls	r6, r6, #2
	mov	r8, r3
	movs	r7, #0
.L_02001944:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_02001978
	adds	r0, r6, #0
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_02001986
	ldr	r3, [pc, #84]
	ldr	r1, [pc, #84]
	adds	r5, r7, r3
	ldrh	r2, [r5, #0]
	adds	r5, #2
	ldrh	r3, [r5, #0]
	ldrh	r0, [r1, #0]
	movs	r4, #1
	ldrh	r1, [r1, #2]
	adds	r5, #2
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	bl 0x0200bbe0
	b.n	.L_02001986
.L_02001978:
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02001984
	adds	r0, r6, #0
	bl 0x0200bb90
.L_02001984:
	adds	r5, #12
.L_02001986:
	movs	r3, #1
	add	r8, r3
	mov	r3, r8
	adds	r6, #1
	adds	r7, #12
	cmp	r3, #5
	bls.n	.L_02001944
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_020019a4
	bl 0x02009804
.L_020019a4:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0200c16e
	.4byte 0x0200c170
	.2byte 0xbfe2
	.2byte 0x0200
	pushal	{r5, r6, r7, lr}
	moval	r7, r8
	push	{r7}
	ldr	r5, [pc, #100]
	movs	r6, #144
	movs	r3, #0
	sub	sp, #8
	lsls	r6, r6, #2
	mov	r8, r3
	movs	r7, #0
.L_020019cc:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_02001a00
	adds	r0, r6, #0
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_02001a0e
	ldr	r3, [pc, #68]
	ldr	r1, [pc, #68]
	adds	r5, r7, r3
	ldrh	r2, [r5, #0]
	adds	r5, #2
	ldrh	r3, [r5, #0]
	ldrh	r0, [r1, #0]
	movs	r4, #1
	ldrh	r1, [r1, #2]
	adds	r5, #2
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	bl 0x0200bbe0
	b.n	.L_02001a0e
.L_02001a00:
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02001a0c
	adds	r0, r6, #0
	bl 0x0200bb90
.L_02001a0c:
	adds	r5, #12
.L_02001a0e:
	movs	r3, #1
	add	r8, r3
	mov	r3, r8
	adds	r6, #1
	adds	r7, #12
	cmp	r3, #15
	bls.n	.L_020019cc
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0200c1b6
	.4byte 0x0200c1b8
	.2byte 0xbfea
	.2byte 0x0200
	.2byte 0xb5e0
	moval	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	sub	sp, #16
	ldr	r5, [pc, #584]
	str	r3, [sp, #12]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r5, r0
	ldr	r0, [r3, #0]
	bl 0x0200bc40
	ldr	r3, [r0, #8]
	movs	r1, #240
	asrs	r3, r3, #20
	lsls	r3, r3, #1
	adds	r3, #1
	mov	r8, r3
	ldr	r3, [r0, #16]
	lsls	r1, r1, #1
	asrs	r3, r3, #20
	lsls	r3, r3, #1
	adds	r3, #1
	str	r3, [sp, #8]
	adds	r3, r5, r1
	mov	r9, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
.L_02001a76:
	ldr	r3, [pc, #540]
	cmp	r2, r3
	bne.n	.L_02001aa2
	ldr	r1, [sp, #12]
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	movs	r1, #155
	adds	r7, r3, #0
	lsls	r1, r1, #1
	subs	r7, #11
	adds	r1, #255
	adds	r1, r1, r3
	ldr	r2, [pc, #512]
	lsls	r3, r7, #1
	adds	r3, r3, r7
	lsls	r3, r3, #2
	mov	fp, r1
	adds	r6, r3, r2
	b.n	.L_02001b36
.L_02001aa2:
	ldr	r3, [pc, #504]
	cmp	r2, r3
	bne.n	.L_02001ace
	ldr	r2, [sp, #12]
	movs	r0, #170
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r7, r3, #0
	adds	r2, #50
	adds	r2, r2, r3
	subs	r7, #14
	mov	fp, r2
	lsls	r3, r7, #1
	ldr	r2, [pc, #472]
	adds	r3, r3, r7
	lsls	r3, r3, #2
	adds	r6, r3, r2
	b.n	.L_02001b36
.L_02001ace:
	ldr	r3, [pc, #468]
	cmp	r2, r3
	bne.n	.L_02001af8
	ldr	r0, [sp, #12]
	movs	r1, #170
	lsls	r1, r1, #1
	adds	r3, r0, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	movs	r0, #141
	adds	r7, r3, #0
	subs	r7, #12
	lsls	r0, r0, #2
	adds	r0, r0, r3
	ldr	r2, [pc, #444]
	lsls	r3, r7, #1
	adds	r3, r3, r7
	lsls	r3, r3, #2
	mov	fp, r0
	adds	r6, r3, r2
	b.n	.L_02001b36
.L_02001af8:
	ldr	r3, [pc, #432]
	cmp	r2, r3
	beq.n	.L_02001b00
	b.n	.L_02001e9e
.L_02001b00:
	ldr	r1, [sp, #12]
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	movs	r1, #128
	adds	r7, r3, #0
	lsls	r1, r1, #2
	subs	r7, #18
	adds	r1, #46
	adds	r1, r1, r3
	ldr	r5, [pc, #404]
	lsls	r3, r7, #1
	movs	r0, #192
	adds	r3, r3, r7
	lsls	r0, r0, #2
	lsls	r3, r3, #2
	adds	r0, #58
	mov	fp, r1
	adds	r6, r3, r5
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_02001b36
	movs	r3, #3
	strh	r3, [r5, #52]
.L_02001b36:
	bl 0x0200bc28
	movs	r0, #0
	bl 0x0200bd78
	ldr	r2, [pc, #332]
	movs	r5, #133
	mov	sl, r2
	lsls	r5, r5, #2
	add	r5, sl
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #11
	lsls	r2, r2, #10
	bl 0x0200bc48
	mov	r3, r8
	lsls	r1, r3, #3
	ldr	r3, [sp, #8]
	ldr	r0, [r5, #0]
	lsls	r2, r3, #3
	bl 0x0200bc58
	mov	r0, r8
	lsls	r1, r0, #19
	ldr	r0, [sp, #8]
	mov	r3, r9
	ldr	r2, [r3, #12]
	lsls	r3, r0, #19
	mov	r0, r9
	bl 0x0200bbc0
	movs	r0, #4
	bl 0x0200bc20
	bl 0x0200bcf8
	ldr	r1, [pc, #304]
	ldr	r3, [r0, #12]
	adds	r5, r6, #2
	adds	r3, r3, r1
	str	r3, [r0, #12]
	mov	r0, fp
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_02001b98
	b.n	.L_02001e00
.L_02001b98:
	movs	r0, #229
	bl 0x0200bdf8
	movs	r3, #240
	lsls	r3, r3, #1
	add	r3, sl
	ldrh	r2, [r6, #0]
	adds	r6, r5, #0
	movs	r0, #0
	ldrsh	r5, [r3, r0]
	ldr	r3, [pc, #228]
	ldrh	r4, [r6, #0]
	adds	r6, #2
	cmp	r5, r3
	bne.n	.L_02001bba
	ldr	r3, [pc, #256]
	b.n	.L_02001bcc
.L_02001bba:
	ldr	r3, [pc, #224]
	cmp	r5, r3
	bne.n	.L_02001bc4
	ldr	r3, [pc, #248]
	b.n	.L_02001bcc
.L_02001bc4:
	ldr	r3, [pc, #220]
	cmp	r5, r3
	bne.n	.L_02001bde
	ldr	r3, [pc, #244]
.L_02001bcc:
	ldrh	r0, [r3, #4]
	ldrh	r1, [r3, #6]
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	adds	r3, r4, #0
	bl 0x0200bbe0
	b.n	.L_02001bf6
.L_02001bde:
	ldr	r3, [pc, #204]
	cmp	r5, r3
	bne.n	.L_02001bf6
	ldr	r3, [pc, #220]
	ldrh	r0, [r3, #4]
	ldrh	r1, [r3, #6]
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	adds	r3, r4, #0
	bl 0x0200bbe0
.L_02001bf6:
	ldrh	r1, [r6, #0]
	ldr	r3, [pc, #148]
	movs	r0, #240
	lsls	r0, r0, #1
	lsls	r2, r1, #1
	adds	r3, r3, r0
	mov	r8, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #136]
	adds	r6, #2
	ldrh	r5, [r6, #0]
	ldrh	r6, [r6, #2]
	cmp	r2, r3
	bne.n	.L_02001c24
	ldr	r2, [pc, #176]
	lsls	r3, r1, #2
	ldrh	r0, [r2, r3]
	adds	r3, #2
	ldrh	r1, [r2, r3]
	movs	r3, #1
	movs	r2, #2
	b.n	.L_02001c82
.L_02001c24:
	ldr	r3, [pc, #116]
	cmp	r2, r3
	bne.n	.L_02001c3a
	ldr	r2, [pc, #160]
	lsls	r3, r1, #2
	ldrh	r0, [r2, r3]
	adds	r3, #2
	ldrh	r1, [r2, r3]
	movs	r3, #1
	movs	r2, #2
	b.n	.L_02001c82
.L_02001c3a:
	ldr	r3, [pc, #104]
	cmp	r2, r3
	bne.n	.L_02001c50
	ldr	r2, [pc, #140]
	lsls	r3, r1, #2
	ldrh	r0, [r2, r3]
	adds	r3, #2
	ldrh	r1, [r2, r3]
	movs	r3, #1
	movs	r2, #2
	b.n	.L_02001c82
.L_02001c50:
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_02001cf4
	cmp	r7, #0
	bne.n	.L_02001c68
	movs	r0, #161
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_02001c78
.L_02001c68:
	cmp	r7, #11
	bne.n	.L_02001cd8
	movs	r0, #147
	lsls	r0, r0, #2
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_02001cd8
.L_02001c78:
	ldr	r3, [pc, #88]
	movs	r2, #2
	ldrh	r0, [r3, #16]
	ldrh	r1, [r3, #18]
	movs	r3, #1
.L_02001c82:
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	adds	r3, r6, #0
	adds	r2, r5, #0
	bl 0x0200bbe0
	b.n	.L_02001cf4
	.4byte 0x02000240
	.4byte 0x00000022
	.4byte 0x0200c068
	.4byte 0x00000023
	.4byte 0x0200c110
	.4byte 0x00000024
	.4byte 0x0200c170
	.4byte 0x00000026
	.4byte 0x0200c1b8
	.4byte 0xfff80000
	.4byte 0x0200bfd2
	.4byte 0x0200bfda
	.4byte 0x0200bfe2
	.4byte 0x0200bfea
	.4byte 0x0200bff2
	.4byte 0x0200c012
	.4byte 0x0200c032
	.2byte 0xc052
	.2byte 0x0200
.L_02001cd8:
	ldr	r2, [pc, #88]
	mov	r1, r8
	lsls	r3, r1, #1
	ldrh	r0, [r2, r3]
	adds	r3, #2
	ldrh	r1, [r2, r3]
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	adds	r3, r6, #0
	adds	r2, r5, #0
	bl 0x0200bbe0
.L_02001cf4:
	movs	r0, #12
	bl 0x0200bc20
	movs	r3, #128
	ldr	r2, [pc, #48]
	lsls	r3, r3, #7
	mov	r0, r9
	strh	r3, [r0, #6]
	mov	r3, r9
	adds	r3, #85
	ldr	r5, [pc, #44]
	strb	r2, [r3, #0]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r6, r5, r1
	ldr	r0, [r6, #0]
	bl 0x0200bc40
	movs	r1, #0
	bl 0x0200bbf8
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_02001d4a
	b.n	.L_02001d40
	.4byte 0x00000000
	.4byte 0x0200c052
	.4byte 0x02000240
	.2byte 0x0022
	.2byte 0x0000
.L_02001d40:
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x0200bcb8
	b.n	.L_02001d80
.L_02001d4a:
	ldr	r3, [pc, #352]
	cmp	r2, r3
	beq.n	.L_02001d56
	ldr	r3, [pc, #348]
	cmp	r2, r3
	bne.n	.L_02001d60
.L_02001d56:
	ldr	r0, [r6, #0]
	movs	r1, #2
	bl 0x0200bcb8
	b.n	.L_02001d80
.L_02001d60:
	ldr	r3, [pc, #336]
	cmp	r2, r3
	bne.n	.L_02001d80
	ldr	r3, [pc, #336]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02001d78
	ldr	r3, [r3, #80]
	movs	r1, #12
	ldrb	r2, [r3, #9]
	orrs	r2, r1
	strb	r2, [r3, #9]
.L_02001d78:
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x0200bcb8
.L_02001d80:
	ldr	r3, [pc, #312]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	movs	r1, #28
	bl 0x0200bc80
	movs	r0, #16
	bl 0x0200bc20
	movs	r2, #0
	mov	r8, r2
.L_02001d9a:
	mov	r3, r8
	cmp	r3, #5
	bne.n	.L_02001da6
	movs	r0, #204
	bl 0x0200bdf8
.L_02001da6:
	mov	r0, r9
	ldr	r3, [r0, #24]
	ldr	r1, [pc, #276]
	ldr	r2, [pc, #276]
	adds	r3, r3, r1
	str	r3, [r0, #24]
	ldr	r3, [r0, #28]
	mov	r1, r9
	adds	r3, r3, r2
	str	r3, [r0, #28]
	ldr	r3, [r0, #12]
	ldr	r0, [pc, #264]
	adds	r3, r3, r0
	str	r3, [r1, #12]
	movs	r0, #1
	bl 0x0200bb40
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #39
	bls.n	.L_02001d9a
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #214
	movs	r2, #133
	lsls	r0, r0, #1
	lsls	r2, r2, #1
	adds	r3, r3, r0
	adds	r2, #255
	str	r2, [r3, #0]
	bl 0x0200bd30
	bl 0x0200bd38
	ldr	r1, [sp, #12]
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl 0x0200bd00
	b.n	.L_02001e9e
.L_02001e00:
	movs	r0, #161
	bl 0x0200bdf8
	movs	r3, #240
	lsls	r3, r3, #1
	add	r3, sl
	ldrh	r4, [r5, #0]
	movs	r0, #0
	ldrsh	r5, [r3, r0]
	ldr	r3, [pc, #184]
	ldrh	r2, [r6, #0]
	cmp	r5, r3
	bne.n	.L_02001e1e
	ldr	r3, [pc, #180]
	b.n	.L_02001e30
.L_02001e1e:
	ldr	r3, [pc, #140]
	cmp	r5, r3
	bne.n	.L_02001e28
	ldr	r3, [pc, #172]
	b.n	.L_02001e30
.L_02001e28:
	ldr	r3, [pc, #132]
	cmp	r5, r3
	bne.n	.L_02001e42
	ldr	r3, [pc, #168]
.L_02001e30:
	ldrh	r0, [r3, #0]
	ldrh	r1, [r3, #2]
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	adds	r3, r4, #0
	bl 0x0200bbe0
	b.n	.L_02001e5a
.L_02001e42:
	ldr	r3, [pc, #112]
	cmp	r5, r3
	bne.n	.L_02001e5a
	ldr	r3, [pc, #144]
	ldrh	r0, [r3, #0]
	ldrh	r1, [r3, #2]
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	adds	r3, r4, #0
	bl 0x0200bbe0
.L_02001e5a:
	movs	r0, #12
	bl 0x0200bc20
	mov	r0, fp
	bl 0x0200bb90
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bb90
	ldr	r3, [pc, #72]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #52]
	cmp	r2, r3
	beq.n	.L_02001e88
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_02001e9a
.L_02001e88:
	mov	r1, r9
	ldr	r3, [r1, #8]
	ldr	r2, [pc, #80]
	asrs	r3, r3, #20
	str	r3, [r2, #0]
	ldr	r2, [pc, #80]
	ldr	r3, [r1, #16]
	asrs	r3, r3, #20
	str	r3, [r2, #0]
.L_02001e9a:
	bl 0x0200bc30
.L_02001e9e:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00000023
	.4byte 0x00000024
	.4byte 0x00000026
	.4byte 0x0200c420
	.4byte 0x02000240
	.4byte 0xfffffc00
	.4byte 0xfffffd00
	.4byte 0xffff6667
	.4byte 0x00000022
	.4byte 0x0200bfd2
	.4byte 0x0200bfda
	.4byte 0x0200bfe2
	.4byte 0x0200bfea
	.4byte 0x0200d67c
	.2byte 0xd678
	.2byte 0x0200
	push	{lr}
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_02001f28
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200bc40
	ldr	r2, [pc, #40]
	ldr	r3, [r0, #8]
	ldr	r2, [r2, #0]
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02001f1e
	ldr	r3, [pc, #32]
	ldr	r2, [r0, #16]
	ldr	r3, [r3, #0]
	asrs	r2, r2, #20
	cmp	r3, r2
	beq.n	.L_02001f28
.L_02001f1e:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bb98
.L_02001f28:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200d67c
	.2byte 0xd678
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200bcc0
	adds	r0, r5, #0
	bl 0x0200bc40
	movs	r1, #15
	bl 0x0200bca0
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200bcb8
	adds	r0, r5, #0
	bl 0x0200bc40
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_02001f88
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_02001f90
	bl 0x02009500
	b.n	.L_02001f90
.L_02001f88:
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200bb90
.L_02001f90:
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r5, #0
.L_02001f98:
	adds	r0, r5, #0
	adds	r0, #13
	adds	r5, #1
	bl 0x02009f38
	cmp	r5, #13
	bls.n	.L_02001f98
	bl 0x02009f68
	pop	{r5, pc}
	push	{r5, lr}
	movs	r5, #0
.L_02001fb0:
	adds	r0, r5, #0
	adds	r0, #25
	adds	r5, #1
	bl 0x02009f38
	cmp	r5, #12
	bls.n	.L_02001fb0
	bl 0x02009f68
	pop	{r5, pc}
	push	{r5, lr}
	movs	r5, #0
.L_02001fc8:
	adds	r0, r5, #0
	adds	r0, #26
	adds	r5, #1
	bl 0x02009f38
	cmp	r5, #9
	bls.n	.L_02001fc8
	bl 0x02009f68
	pop	{r5, pc}
	push	{r5, lr}
	movs	r5, #0
.L_02001fe0:
	adds	r0, r5, #0
	adds	r0, #8
	adds	r5, #1
	bl 0x02009f38
	cmp	r5, #2
	bls.n	.L_02001fe0
	bl 0x02009f68
	pop	{r5, pc}
	push	{r5, lr}
	movs	r5, #0
.L_02001ff8:
	adds	r0, r5, #0
	adds	r0, #10
	adds	r5, #1
	bl 0x02009f38
	cmp	r5, #5
	bls.n	.L_02001ff8
.L_02002006:
	bl 0x02009f68
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	sub	sp, #8
	bl 0x02008e9c
	ldr	r3, [r7, #8]
	asrs	r6, r3, #20
	cmp	r6, #12
	bne.n	.L_0200205c
	bl 0x0200bc28
	movs	r0, #0
	bl 0x0200bd78
	movs	r5, #11
	movs	r1, #32
	movs	r2, #1
	movs	r3, #1
	movs	r0, #0
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #1
	bl 0x0200bb40
	adds	r0, r7, #0
	bl 0x020095b8
	movs	r0, #1
	movs	r1, #32
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	bl 0x0200bc30
	b.n	.L_020020d6
.L_0200205c:
	cmp	r6, #18
	bne.n	.L_02002082
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r0, r7, #0
	ldr	r5, [r3, #0]
	bl 0x0200bdb0
	lsls	r0, r0, #2
	adds	r5, r5, r0
	ldrb	r2, [r5, #3]
	movs	r3, #127
	ands	r3, r2
	strb	r3, [r5, #3]
	b.n	.L_020020d6
.L_02002082:
	cmp	r6, #54
	bne.n	.L_020020ac
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r0, r7, #0
	ldr	r5, [r3, #0]
	bl 0x0200bdb0
	lsls	r0, r0, #2
	adds	r5, r5, r0
	ldrb	r2, [r5, #3]
	movs	r3, #127
	ands	r3, r2
	strb	r3, [r5, #3]
	movs	r3, #11
	strb	r3, [r5, #2]
	b.n	.L_020020d6
.L_020020ac:
	cmp	r6, #55
	bne.n	.L_020020d6
	movs	r3, #54
	movs	r5, #20
	str	r3, [sp, #0]
	movs	r0, #53
	movs	r1, #20
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r3, #56
	str	r3, [sp, #0]
	movs	r0, #57
	movs	r1, #20
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200bbf0
.L_020020d6:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, lr}
	bl 0x0200bde0
	cmp	r0, #0
	beq.n	.L_0200210a
	ldr	r3, [pc, #36]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200bd10
	bl 0x0200bc40
	adds	r5, r0, #0
	ldr	r0, [pc, #20]
	bl 0x02008d78
	bl 0x0200bde8
	adds	r0, r5, #0
	bl 0x0200a00c
.L_0200210a:
	pop	{r5, pc}
	.4byte 0x02000240
	.2byte 0xbfc8
	.2byte 0x0200
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x02008d78
	pop	{pc}
	.2byte 0x0000
	.2byte 0xbfc8
	.2byte 0x0200
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #188
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	cmp	r0, #0
	beq.n	.L_0200213e
	bl 0x0200a00c
.L_0200213e:
	pop	{pc}
	push	{r5, lr}
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	sub	sp, #8
	bl 0x0200bb88
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_0200217c
	movs	r0, #8
	bl 0x0200bc40
	movs	r3, #47
	adds	r0, #85
	movs	r2, #12
	strb	r5, [r0, #0]
	movs	r1, #12
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #51
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bbf0
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200bb90
.L_0200217c:
	add	sp, #8
	pop	{r5, pc}
	push	{lr}
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	sub	sp, #8
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_020021b0
	movs	r3, #47
	movs	r2, #12
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #48
	movs	r1, #11
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bbf0
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200bb98
.L_020021b0:
	add	sp, #8
	pop	{pc}
	push	{r5, r6, lr}
	bl 0x0200bcf8
	adds	r5, r0, #0
	ldr	r3, [r5, #12]
	ldr	r2, [pc, #36]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r5, #12]
	bl 0x0200bb40
	movs	r6, #0
.L_020021cc:
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r5, #12]
	movs	r0, #1
	adds	r6, #1
	bl 0x0200bb40
	cmp	r6, #7
	bls.n	.L_020021cc
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb520
	sub	sp, #8
	bl 0x0200bc28
	movs	r0, #0
	bl 0x0200bd78
	bl 0x0200bcf8
	movs	r3, #0
	adds	r0, #85
	movs	r1, #1
	movs	r2, #164
	strb	r3, [r0, #0]
	negs	r1, r1
	ldr	r0, [pc, #204]
	lsls	r2, r2, #16
	movs	r3, #1
	bl 0x0200bce8
	bl 0x0200bcf0
	ldr	r3, [pc, #192]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r0, #24
	movs	r2, #0
	bl 0x0200bc98
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #24
	bl 0x0200bcc8
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #24
	ldr	r1, [pc, #160]
	adds	r2, #204
	bl 0x0200bc48
	movs	r1, #166
	movs	r0, #24
	lsls	r1, r1, #2
	movs	r2, #136
	bl 0x0200bc60
	movs	r5, #1
	movs	r3, #6
	movs	r1, #33
	movs	r2, #105
	movs	r0, #68
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbe0
	movs	r0, #161
	bl 0x0200bdf8
	bl 0x0200a1b4
	movs	r0, #164
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bb90
	movs	r1, #166
	movs	r0, #24
	lsls	r1, r1, #2
	movs	r2, #104
	bl 0x0200bc60
	movs	r3, #4
	movs	r1, #33
	movs	r2, #105
	movs	r0, #69
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbe0
	movs	r0, #229
	bl 0x0200bdf8
	bl 0x0200a1b4
	movs	r0, #10
	bl 0x0200bc20
	movs	r0, #24
	movs	r1, #6
	movs	r2, #16
	bl 0x0200bc88
	movs	r3, #128
	movs	r1, #222
	movs	r2, #168
	lsls	r3, r3, #7
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	movs	r0, #24
	bl 0x0200bc70
	movs	r0, #20
	bl 0x0200bc20
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #108
	bl 0x0200bb90
	bl 0x0200bc30
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02920000
	.4byte 0x02000240
	.2byte 0x9999
	.2byte 0x0001
	push	{lr}
	bl 0x0200bc28
	movs	r0, #0
	bl 0x0200bd78
	movs	r0, #245
	movs	r1, #1
	movs	r2, #168
	lsls	r0, r0, #18
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	bl 0x0200bce8
	bl 0x0200bcf0
	ldr	r3, [pc, #132]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r0, #24
	movs	r2, #0
	bl 0x0200bc98
	movs	r1, #128
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #24
	bl 0x0200bcc8
	movs	r0, #24
	bl 0x0200bc40
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #1
	movs	r0, #24
	bl 0x0200bcb8
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #24
	ldr	r1, [pc, #80]
	adds	r2, #204
	bl 0x0200bc48
	movs	r1, #235
	movs	r0, #24
	lsls	r1, r1, #2
	movs	r2, #168
	bl 0x0200bc60
	movs	r1, #246
	movs	r0, #24
	lsls	r1, r1, #2
	movs	r2, #152
	bl 0x0200bc60
	movs	r1, #130
	movs	r0, #24
	lsls	r1, r1, #3
	movs	r2, #152
	bl 0x0200bc60
	movs	r1, #0
	movs	r2, #0
	movs	r0, #24
	bl 0x0200bc68
	movs	r0, #20
	bl 0x0200bc20
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #109
	bl 0x0200bb90
	bl 0x0200bc30
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x9999
	.2byte 0x0001
	push	{lr}
	bl 0x0200bc28
	movs	r0, #0
	bl 0x0200bd78
	movs	r0, #144
	movs	r1, #1
	movs	r2, #214
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #15
	movs	r3, #1
	bl 0x0200bce8
	bl 0x0200bcf0
	ldr	r3, [pc, #156]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r0, #25
	movs	r2, #0
	bl 0x0200bc98
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #25
	bl 0x0200bcc8
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #25
	ldr	r1, [pc, #124]
	adds	r2, #204
	bl 0x0200bc48
	movs	r2, #104
	movs	r1, #168
	movs	r0, #25
	bl 0x0200bc60
	movs	r0, #10
	bl 0x0200bc20
	movs	r0, #153
	bl 0x0200bdf8
	movs	r0, #25
	bl 0x0200bc40
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r0, #40]
	movs	r0, #25
	bl 0x0200bc40
	movs	r1, #0
	bl 0x0200bbf8
	movs	r2, #104
	movs	r1, #200
	movs	r0, #25
	bl 0x0200bc60
	movs	r0, #25
	bl 0x0200bc40
	movs	r1, #1
	bl 0x0200bbf8
	movs	r1, #147
	movs	r0, #25
	lsls	r1, r1, #1
	movs	r2, #104
	bl 0x0200bc60
	movs	r3, #128
	movs	r1, #212
	movs	r2, #208
	lsls	r3, r3, #7
	lsls	r1, r1, #17
	lsls	r2, r2, #15
	movs	r0, #25
	bl 0x0200bc70
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #110
	bl 0x0200bb90
	bl 0x0200bc30
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, lr}
	sub	sp, #8
	bl 0x0200bc28
	movs	r0, #0
	bl 0x0200bd78
	ldr	r5, [pc, #260]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #25
	bl 0x0200bd88
	bl 0x0200bcf8
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #1
	movs	r0, #212
	movs	r2, #208
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #15
	bl 0x0200bce8
	bl 0x0200bcf0
	ldr	r1, [r5, #0]
	movs	r0, #25
	movs	r2, #0
	bl 0x0200bc98
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #25
	bl 0x0200bcc8
	movs	r0, #212
	movs	r1, #1
	movs	r2, #152
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200bce8
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #25
	ldr	r1, [pc, #168]
	adds	r2, #204
	bl 0x0200bc48
	movs	r1, #212
	movs	r0, #25
	lsls	r1, r1, #1
	movs	r2, #152
	bl 0x0200bc60
	movs	r0, #25
	movs	r1, #0
	movs	r2, #20
	bl 0x0200bca8
	bl 0x0200a604
	movs	r1, #220
	movs	r0, #25
	lsls	r1, r1, #1
	movs	r2, #152
	bl 0x0200bc60
	movs	r1, #228
	movs	r0, #25
	lsls	r1, r1, #1
	movs	r2, #168
	bl 0x0200bc60
	movs	r1, #228
	lsls	r1, r1, #1
	movs	r2, #184
	movs	r0, #25
	bl 0x0200bc60
	movs	r0, #161
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bb90
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r1, #51
	movs	r3, #11
	movs	r2, #92
	movs	r0, #69
	bl 0x0200bbe0
	movs	r0, #161
	bl 0x0200bdf8
	bl 0x0200a1b4
	movs	r1, #228
	movs	r0, #25
	lsls	r1, r1, #1
	movs	r2, #216
	bl 0x0200bc60
	movs	r1, #146
	movs	r0, #25
	lsls	r1, r1, #2
	movs	r2, #216
	bl 0x0200bc60
	movs	r3, #128
	movs	r1, #154
	movs	r2, #140
	lsls	r3, r3, #7
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	movs	r0, #25
	bl 0x0200bc70
	movs	r0, #238
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200bb90
	bl 0x0200bc30
	add	sp, #8
	pop	{r5, pc}
	.4byte 0x02000240
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	movs	r0, #18
	ldr	r6, [r3, #0]
	sub	sp, #8
	bl 0x0200bc40
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r2, #198
	adds	r3, r6, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	movs	r2, #1
	negs	r2, r2
	adds	r5, r0, #0
	cmp	r3, r2
	beq.n	.L_020025a8
	cmp	r3, #7
	bgt.n	.L_020025a8
	ldr	r3, [r5, #28]
	ldr	r2, [pc, #92]
	adds	r3, r3, r2
	str	r3, [r5, #28]
.L_020025a8:
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r2, #196
	adds	r3, r6, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_020025fa
	movs	r3, #27
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r2, #1
	movs	r0, #27
	movs	r1, #8
	bl 0x0200bbf0
	movs	r0, #18
	movs	r1, #4
	bl 0x0200bc80
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r1, #54
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	movs	r0, #131
	movs	r3, #128
	lsls	r3, r3, #9
	lsls	r0, r0, #1
	str	r3, [r5, #28]
	adds	r0, #255
	bl 0x0200bb90
.L_020025fa:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xe100
	.2byte 0xffff
	.2byte 0xb520
	movs	r0, #134
	movs	r1, #1
	bl 0x0200bd50
	ldr	r5, [pc, #44]
	movs	r1, #144
	adds	r0, r5, #0
	lsls	r1, r1, #3
	bl 0x0200bb48
	movs	r1, #18
	movs	r0, #25
	bl 0x0200bd58
	bl 0x0200bd70
	movs	r0, #1
	bl 0x0200bd48
	bl 0x0200bd60
	adds	r0, r5, #0
	bl 0x0200bb50
	bl 0x0200bd68
	pop	{r5, pc}
	.2byte 0xa575
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	ldr	r5, [r6, #68]
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #72]
.L_0200264a:
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, r5, #0
	adds	r3, r3, r7
	movs	r1, #18
	str	r3, [r6, #16]
	bl 0x0200bb38
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02002670
	adds	r3, #15
.L_02002670:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	ldr	r1, [r6, #80]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r1, #18]
	ldrh	r2, [r2, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #18]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r5, r0, #0
	movs	r0, #125
	mov	r9, r2
	adds	r6, r1, #0
	bl 0x0200bdf8
	adds	r0, r5, #0
	bl 0x0200bc40
	movs	r1, #7
	bl 0x0200bca0
	movs	r0, #2
	bl 0x0200bb40
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x0200bc80
	adds	r0, r5, #0
	bl 0x0200bc40
	movs	r1, #7
	bl 0x0200bca0
	movs	r0, #4
	bl 0x0200bb40
	adds	r0, r5, #0
	bl 0x0200bc40
	movs	r1, #0
	bl 0x0200bca0
	adds	r0, r6, #0
	bl 0x0200bc40
	adds	r7, r0, #0
	movs	r0, #0
	mov	sl, r0
	mov	r8, r0
.L_020026f4:
	bl 0x0200bb58
	lsls	r3, r0, #3
	subs	r3, r3, r0
	ldr	r2, [r7, #12]
	lsls	r3, r3, #1
	lsrs	r3, r3, #16
	lsls	r3, r3, #16
	subs	r2, r2, r3
	mov	r3, r8
	lsls	r1, r3, #17
	ldr	r3, [r7, #8]
	ldr	r0, [pc, #180]
	adds	r1, r1, r3
	ldr	r3, [pc, #180]
	adds	r1, r1, r0
	movs	r0, #30
	adds	r2, r2, r3
	adds	r0, #255
	ldr	r3, [r7, #16]
	bl 0x0200bbb0
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_020027a4
	mov	r1, sl
	ldr	r0, [r6, #80]
	bl 0x0200bd80
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r3, #4
	strb	r5, [r3, #0]
	movs	r1, #0
	mov	sl, r0
	adds	r0, r6, #0
	bl 0x0200bbf8
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200bba0
	adds	r0, r6, #0
	ldr	r1, [pc, #124]
	bl 0x0200bba8
	movs	r3, #179
	lsls	r3, r3, #8
	adds	r3, #51
	ldr	r1, [r6, #80]
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	movs	r0, #13
	ldrb	r3, [r1, #9]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	str	r5, [r6, #68]
	str	r5, [r6, #76]
	bl 0x0200bb58
	movs	r3, #192
	lsls	r0, r0, #14
	lsls	r3, r3, #7
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	negs	r0, r0
	str	r0, [r6, #72]
	bl 0x0200bb58
	ldr	r3, [pc, #68]
	lsls	r0, r0, #9
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r3, r6, #0
	adds	r3, #100
	strh	r0, [r3, #0]
	ldr	r3, [pc, #56]
	str	r3, [r6, #48]
	ldr	r3, [pc, #56]
	str	r3, [r6, #52]
	ldr	r3, [pc, #56]
	str	r3, [r6, #108]
.L_020027a4:
	movs	r0, #1
	add	r8, r0
	mov	r3, r8
	cmp	r3, #7
	bls.n	.L_020026f4
	mov	r0, r9
	bl 0x0200bb90
	movs	r0, #20
.L_020027b6:
	bl 0x0200bc20
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0xfff80000
	.4byte 0xfffe0000
	.4byte 0x0200bf14
	.4byte 0xffffff00
	.4byte 0xfffff800
	.4byte 0xfffffa00
	.2byte 0xa641
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	movs	r0, #25
	sub	sp, #8
	bl 0x0200bc40
	mov	sl, r0
	bl 0x0200bc28
	movs	r0, #0
	bl 0x0200bd78
	ldr	r6, [pc, #592]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r6, r2
	ldr	r0, [r6, #0]
	movs	r1, #25
	bl 0x0200bd88
	bl 0x0200bcf8
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #1
	movs	r0, #154
	movs	r2, #140
	movs	r3, #1
	lsls	r0, r0, #18
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200bce8
	bl 0x0200bcf0
	ldr	r1, [r6, #0]
	movs	r0, #25
	movs	r2, #0
	bl 0x0200bc98
	movs	r1, #128
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #25
	bl 0x0200bcc8
	movs	r0, #25
	movs	r1, #1
	bl 0x0200bcb8
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #25
	ldr	r1, [pc, #512]
	adds	r2, #204
	bl 0x0200bc48
	movs	r1, #158
	movs	r2, #140
	movs	r0, #25
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x0200bc60
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	movs	r0, #25
	bl 0x0200bc48
	movs	r0, #153
	bl 0x0200bdf8
	movs	r3, #128
	lsls	r3, r3, #12
	mov	r2, sl
	str	r3, [r2, #40]
	movs	r0, #25
	mov	r8, r3
	bl 0x0200bc40
	movs	r1, #0
	bl 0x0200bbf8
	movs	r1, #166
	movs	r2, #140
	lsls	r2, r2, #1
	lsls	r1, r1, #2
	movs	r0, #25
	bl 0x0200bc60
	movs	r0, #25
	bl 0x0200bc40
	movs	r1, #1
	bl 0x0200bbf8
	movs	r2, #132
	movs	r1, #25
	lsls	r2, r2, #2
	movs	r0, #21
	bl 0x0200a698
	movs	r0, #153
	bl 0x0200bdf8
	mov	r3, r8
	mov	r2, sl
	str	r3, [r2, #40]
	movs	r0, #25
	bl 0x0200bc40
	movs	r1, #0
	bl 0x0200bbf8
	movs	r1, #174
	movs	r2, #140
	lsls	r2, r2, #1
	lsls	r1, r1, #2
	movs	r0, #25
	bl 0x0200bc60
	movs	r0, #25
	bl 0x0200bc40
	movs	r1, #1
	bl 0x0200bbf8
	movs	r0, #10
	bl 0x0200bc20
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #25
	ldr	r1, [pc, #348]
	adds	r2, #204
	bl 0x0200bc48
	movs	r1, #178
	movs	r2, #140
	lsls	r2, r2, #1
	movs	r0, #25
	lsls	r1, r1, #2
	bl 0x0200bc60
	movs	r1, #152
	lsls	r1, r1, #7
	ldr	r0, [pc, #328]
	adds	r1, #204
	bl 0x0200bce0
	movs	r0, #178
	movs	r1, #1
	movs	r2, #136
	lsls	r0, r0, #18
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	bl 0x0200bce8
	movs	r5, #1
	movs	r3, #4
	movs	r2, #110
	movs	r0, #112
	movs	r1, #6
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbe0
	movs	r0, #25
	movs	r1, #1
	bl 0x0200bcb8
	movs	r1, #178
	movs	r0, #25
	lsls	r1, r1, #2
	movs	r2, #200
	bl 0x0200bc60
	movs	r1, #178
	movs	r2, #168
	lsls	r1, r1, #2
	movs	r0, #25
	bl 0x0200bc60
	ldr	r0, [r6, #0]
	bl 0x0200bc50
	movs	r0, #25
	movs	r1, #0
	bl 0x0200bcb0
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	movs	r0, #25
	bl 0x0200bc48
	movs	r0, #153
	bl 0x0200bdf8
	mov	r3, r8
	mov	r2, sl
	str	r3, [r2, #40]
	movs	r0, #25
	bl 0x0200bc40
	movs	r1, #0
	bl 0x0200bbf8
	movs	r1, #186
	movs	r2, #168
	lsls	r1, r1, #2
	movs	r0, #25
	bl 0x0200bc60
	movs	r0, #25
	bl 0x0200bc40
	movs	r1, #1
.L_020029a2:
	bl 0x0200bbf8
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #22
	movs	r0, #24
	movs	r1, #25
	bl 0x0200a698
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #25
	bl 0x0200bcb0
	movs	r0, #153
	bl 0x0200bdf8
	mov	r3, r8
	mov	r2, sl
	str	r3, [r2, #40]
	movs	r0, #25
	bl 0x0200bc40
	movs	r1, #0
.L_020029d2:
	bl 0x0200bbf8
	movs	r1, #186
	movs	r2, #136
	lsls	r1, r1, #2
	movs	r0, #25
	bl 0x0200bc60
.L_020029e2:
	movs	r0, #25
	bl 0x0200bc40
	movs	r1, #1
	bl 0x0200bbf8
	movs	r0, #10
	bl 0x0200bc20
	movs	r1, #186
	movs	r0, #25
	lsls	r1, r1, #2
	movs	r2, #72
	bl 0x0200bc60
	movs	r3, #4
	movs	r1, #51
	movs	r2, #110
	movs	r0, #66
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbe0
	movs	r0, #229
	bl 0x0200bdf8
	bl 0x0200a1b4
	movs	r0, #10
	bl 0x0200bc20
	movs	r0, #25
	movs	r1, #6
	movs	r2, #16
	bl 0x0200bc88
	movs	r1, #0
	movs	r2, #0
	movs	r0, #25
	bl 0x0200bc68
	movs	r0, #20
	bl 0x0200bc20
	movs	r0, #135
	lsls	r0, r0, #4
	bl 0x0200bb90
	bl 0x0200bc30
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x00019999
	.2byte 0x6666
	.2byte 0x0002
	push	{r5, r6, lr}
	ldr	r5, [pc, #176]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	sub	sp, #8
	bl 0x0200bc40
	adds	r6, r0, #0
	bl 0x0200bc28
	movs	r0, #0
	bl 0x0200bd78
	movs	r3, #192
	movs	r1, #248
	movs	r2, #136
	ldr	r0, [r5, #0]
	lsls	r3, r3, #8
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x0200bc70
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r1, #64
	movs	r3, #8
	movs	r2, #15
	movs	r0, #65
	bl 0x0200bbe0
	movs	r0, #163
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bb90
	movs	r0, #161
	bl 0x0200bdf8
	bl 0x0200a1b4
	ldr	r1, [r5, #0]
	movs	r0, #9
	movs	r2, #0
	bl 0x0200bc98
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #9
	bl 0x0200bcc8
	bl 0x0200abcc
	movs	r0, #20
	bl 0x0200bc20
	movs	r3, #128
	ldr	r2, [pc, #52]
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	movs	r0, #9
	movs	r1, #3
	bl 0x0200bcb8
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200bcb8
	ldr	r0, [r5, #0]
	movs	r1, #28
	bl 0x0200bc80
	movs	r0, #16
	bl 0x0200bc20
	movs	r5, #0
.L_02002b00:
	cmp	r5, #5
	bne.n	.L_02002b14
	movs	r0, #204
	bl 0x0200bdf8
	b.n	.L_02002b14
	.4byte 0x00000000
	.2byte 0x0240
	.2byte 0x0200
.L_02002b14:
	ldr	r3, [r6, #24]
	ldr	r2, [pc, #68]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [pc, #64]
	ldr	r3, [r6, #28]
	adds	r5, #1
	adds	r3, r3, r2
	str	r3, [r6, #28]
	ldr	r2, [pc, #56]
	ldr	r3, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	bl 0x0200bb40
	cmp	r5, #39
	bls.n	.L_02002b00
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200bd30
	bl 0x0200bd38
	movs	r0, #23
	bl 0x0200bd00
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0xfffffc00
	.4byte 0xfffffd00
	.2byte 0x6667
	.2byte 0xffff
	.2byte 0xb520
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r2, #196
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	sub	sp, #8
	cmp	r3, #0
	beq.n	.L_02002bc2
	movs	r0, #229
	bl 0x0200bdf8
	movs	r5, #1
	movs	r0, #64
	movs	r1, #64
	movs	r2, #15
	movs	r3, #8
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbe0
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r1, #66
	movs	r0, #64
	movs	r2, #15
	movs	r3, #72
	str	r5, [sp, #0]
	bl 0x0200bbe0
	ldr	r3, [pc, #24]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200bc40
	movs	r1, #0
	bl 0x0200bbf8
.L_02002bc2:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #134
	movs	r1, #1
	bl 0x0200bd50
	ldr	r5, [pc, #52]
	movs	r1, #144
	adds	r0, r5, #0
	lsls	r1, r1, #3
	bl 0x0200bb48
	ldr	r3, [pc, #44]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r0, #9
	bl 0x0200bd58
	bl 0x0200bd70
	movs	r0, #1
	bl 0x0200bd48
	bl 0x0200bd60
	adds	r0, r5, #0
	bl 0x0200bb50
	bl 0x0200bd68
	pop	{r5, pc}
	.4byte 0x0200ab69
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #424]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r2, r2, r3
	ldr	r0, [r2, #0]
	sub	sp, #8
	mov	r8, r2
	bl 0x0200bc40
	adds	r5, r0, #0
	movs	r0, #9
	bl 0x0200bc40
	adds	r7, r0, #0
	bl 0x0200bc28
	movs	r0, #0
	bl 0x0200bd78
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200bce8
	bl 0x0200bbc8
	movs	r0, #1
	bl 0x0200bb40
	movs	r3, #130
	lsls	r3, r3, #16
	str	r3, [r5, #12]
	movs	r2, #0
	movs	r3, #128
	lsls	r3, r3, #8
	mov	sl, r2
	adds	r6, r5, #0
	str	r3, [r5, #72]
	adds	r6, #85
	mov	r9, r3
	mov	r3, sl
	str	r2, [r5, #68]
	strb	r3, [r6, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	subs	r2, #172
	str	r2, [r3, #0]
	bl 0x0200bd28
	bl 0x0200bd38
	movs	r0, #204
	bl 0x0200bdf8
	movs	r3, #3
	strb	r3, [r6, #0]
	movs	r0, #24
	bl 0x0200bc20
	movs	r0, #176
.L_02002ca6:
	bl 0x0200bdf8
	bl 0x0200a1b4
	ldr	r6, [pc, #284]
	ldr	r0, [r6, #0]
	cmp	r0, #0
	beq.n	.L_02002cd8
	movs	r1, #248
	movs	r3, #240
	movs	r2, #0
	lsls	r3, r3, #15
	lsls	r1, r1, #16
	bl 0x0200bbc0
	movs	r0, #1
	bl 0x0200bb40
	ldr	r3, [r6, #0]
	ldr	r2, [pc, #256]
	str	r2, [r3, #48]
	str	r2, [r3, #52]
	movs	r2, #128
	lsls	r2, r2, #12
	str	r2, [r3, #40]
.L_02002cd8:
	movs	r2, #204
	mov	r3, r8
	lsls	r2, r2, #8
	ldr	r0, [r3, #0]
	ldr	r1, [pc, #236]
	adds	r2, #204
	bl 0x0200bc48
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #9
	ldr	r1, [pc, #224]
	adds	r2, #204
	bl 0x0200bc48
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	movs	r1, #216
	str	r3, [r7, #40]
	movs	r3, #240
	adds	r0, r5, #0
	lsls	r1, r1, #16
	movs	r2, #0
	lsls	r3, r3, #15
	bl 0x0200bbd0
	movs	r1, #140
	movs	r3, #240
	adds	r0, r7, #0
	lsls	r1, r1, #17
	movs	r2, #0
	lsls	r3, r3, #15
	bl 0x0200bbd0
	ldr	r0, [r6, #0]
	cmp	r0, #0
	beq.n	.L_02002d32
	movs	r1, #248
	movs	r3, #176
	lsls	r1, r1, #16
	movs	r2, #0
	lsls	r3, r3, #15
	bl 0x0200bbd0
.L_02002d32:
	adds	r0, r5, #0
	bl 0x0200bbd8
	movs	r0, #20
	bl 0x0200bc20
	movs	r1, #4
	adds	r1, #255
	movs	r2, #0
	movs	r0, #9
	bl 0x0200bcc8
	mov	r2, r8
	ldr	r0, [r2, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200bca8
	movs	r0, #9
	mov	r1, r9
	movs	r2, #60
	bl 0x0200bca8
	movs	r1, #148
	movs	r0, #9
	lsls	r1, r1, #1
	movs	r2, #104
	bl 0x0200bc60
	movs	r1, #148
	movs	r0, #9
	lsls	r1, r1, #1
	movs	r2, #80
	bl 0x0200bc60
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200bc68
	movs	r0, #20
	bl 0x0200bc20
	ldr	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_02002dae
	adds	r3, #85
	mov	r2, sl
	strb	r2, [r3, #0]
	movs	r3, #15
	movs	r2, #5
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bbf0
	movs	r0, #1
	bl 0x0200bb40
.L_02002dae:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #113
	bl 0x0200bb90
	bl 0x0200bc30
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200c420
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r0, #16
	bl 0x0200bc40
	mov	r8, r0
	bl 0x0200bc28
	movs	r0, #0
	bl 0x0200bd78
	ldr	r6, [pc, #276]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r6, r6, r3
	ldr	r0, [r6, #0]
	movs	r1, #16
	bl 0x0200bd88
	bl 0x0200bcf8
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #128
	movs	r0, #190
	movs	r2, #204
	movs	r3, #1
	lsls	r0, r0, #18
	lsls	r1, r1, #14
	lsls	r2, r2, #16
	bl 0x0200bce8
	bl 0x0200bcf0
	ldr	r1, [r6, #0]
	movs	r0, #16
	movs	r2, #0
	bl 0x0200bc98
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #16
	bl 0x0200bcc8
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #10
	movs	r0, #16
	bl 0x0200bc48
	movs	r5, #128
	movs	r0, #153
	bl 0x0200bdf8
	lsls	r5, r5, #12
	mov	r3, r8
	str	r5, [r3, #40]
	movs	r0, #16
	bl 0x0200bc40
	movs	r1, #0
	bl 0x0200bbf8
	movs	r1, #190
	movs	r2, #200
	lsls	r1, r1, #2
	movs	r0, #16
	bl 0x0200bc60
	movs	r0, #16
	bl 0x0200bc40
	movs	r1, #1
	bl 0x0200bbf8
	movs	r0, #20
	bl 0x0200bc20
	movs	r0, #153
	bl 0x0200bdf8
	mov	r3, r8
	str	r5, [r3, #40]
	movs	r0, #16
	bl 0x0200bc40
	movs	r1, #0
	bl 0x0200bbf8
	movs	r1, #198
	movs	r2, #200
.L_02002e92:
	lsls	r1, r1, #2
	movs	r0, #16
	bl 0x0200bc60
	movs	r0, #16
	bl 0x0200bc40
	movs	r1, #1
	bl 0x0200bbf8
	movs	r2, #132
	lsls	r2, r2, #2
	movs	r1, #16
	movs	r0, #9
	bl 0x0200a698
	movs	r0, #153
	bl 0x0200bdf8
	mov	r3, r8
	str	r5, [r3, #40]
	movs	r0, #16
	bl 0x0200bc40
	movs	r1, #0
	bl 0x0200bbf8
	movs	r1, #206
	movs	r2, #200
	lsls	r1, r1, #2
	movs	r0, #16
	bl 0x0200bc60
	movs	r0, #16
	bl 0x0200bc40
	movs	r1, #1
	bl 0x0200bbf8
	movs	r0, #20
	bl 0x0200bc20
	ldr	r1, [r6, #0]
	movs	r0, #16
	bl 0x0200bd88
	ldr	r0, [r6, #0]
	bl 0x0200bc50
	movs	r0, #20
	bl 0x0200bc20
	bl 0x0200bc30
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
.L_02002f08:
	push	{r5, lr}
	bl 0x0200bc28
	movs	r0, #0
	bl 0x0200bd78
	ldr	r5, [pc, #88]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r1, [r5, #0]
	movs	r0, #16
	movs	r2, #0
	bl 0x0200bc98
	movs	r1, #128
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #16
	bl 0x0200bcc8
	movs	r1, #0
	movs	r0, #16
	bl 0x0200bcb0
	movs	r0, #20
	bl 0x0200bc20
	movs	r0, #16
	movs	r1, #3
	bl 0x0200bc90
	movs	r1, #128
	movs	r0, #16
	lsls	r1, r1, #8
	bl 0x0200bcb0
	ldr	r1, [r5, #0]
	movs	r0, #16
	bl 0x0200bd88
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #16
	bl 0x0200bcd0
	movs	r0, #40
	bl 0x0200bc20
	bl 0x0200bc30
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_02002f8a
	bl 0x0200af08
	b.n	.L_02002f8e
.L_02002f8a:
	bl 0x0200add4
.L_02002f8e:
	movs	r0, #133
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bb90
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	sub	sp, #8
	bl 0x0200bc28
	movs	r0, #0
	bl 0x0200bd78
	ldr	r5, [pc, #68]
	ldr	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L_02002fb8
	movs	r1, #3
	bl 0x0200bd40
.L_02002fb8:
	movs	r0, #199
	movs	r1, #0
	bl 0x0200bc38
	ldr	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L_02002fca
	bl 0x0200bbb8
.L_02002fca:
	movs	r3, #15
	movs	r2, #5
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	movs	r0, #1
	bl 0x0200bbf0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #120
	bl 0x0200bb90
	bl 0x0200bc30
	add	sp, #8
	pop	{r5, pc}
	.2byte 0xc420
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	bl 0x0200bc28
	movs	r0, #0
	bl 0x0200bd78
	ldr	r1, [pc, #60]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_02003026
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldr	r0, [r3, #0]
	movs	r1, #3
	bl 0x0200bcb8
.L_02003026:
	movs	r0, #123
	bl 0x0200bdf8
	bl 0x0200bd30
	bl 0x0200bd38
	movs	r0, #170
	lsls	r0, r0, #1
	adds	r3, r5, r0
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200bd00
	pop	{r5, pc}
	.4byte 0x02000240
	.2byte 0x0022
	.2byte 0x0000
	.global Func_0200304c
	.thumb_func
Func_0200304c:
	push	{lr}
	ldr	r3, [pc, #84]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_02003066
	bl 0x0200b0e4
	b.n	.L_020030a0
.L_02003066:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_02003072
	bl 0x0200b164
	b.n	.L_020030a0
.L_02003072:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_0200307e
	bl 0x0200b284
	b.n	.L_020030a0
.L_0200307e:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_0200308a
	bl 0x0200b69c
	b.n	.L_020030a0
.L_0200308a:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_02003096
	bl 0x0200b90c
	b.n	.L_020030a0
.L_02003096:
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_020030a0
	bl 0x0200baa0
.L_020030a0:
	movs	r0, #0
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000022
	.4byte 0x00000023
	.4byte 0x00000024
	.4byte 0x00000025
	.4byte 0x00000026
	.2byte 0x0027
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #28]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200bc40
	ldr	r3, [r0, #80]
	movs	r0, #64
	ldrb	r1, [r3, #9]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl 0x0200bcb8
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r0, [pc, #112]
	sub	sp, #8
	bl 0x020081d0
	movs	r1, #3
	movs	r0, #8
	bl 0x0200bcb8
	ldr	r0, [pc, #100]
	bl 0x02008038
	movs	r0, #12
	bl 0x0200bcc0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_02003140
	movs	r3, #56
	movs	r2, #64
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #32
	movs	r2, #2
	movs	r3, #1
.L_0200311e:
	movs	r0, #54
	bl 0x0200bbf0
	movs	r0, #12
	bl 0x0200bc40
	movs	r1, #226
	movs	r2, #128
	movs	r3, #130
	lsls	r1, r1, #18
.L_02003132:
	lsls	r2, r2, #15
	lsls	r3, r3, #18
	bl 0x0200bbc0
	movs	r0, #10
	bl 0x0200bb40
.L_02003140:
	bl 0x02009820
	bl 0x02009f94
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl 0x0200bb48
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200bf44
	.4byte 0x0200bf98
	.2byte 0xb0c1
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #10
	adds	r0, #255
	sub	sp, #8
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_020031b2
	ldr	r5, [pc, #252]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r5, r2
	ldr	r0, [r6, #0]
	bl 0x0200bc40
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	movs	r2, #241
	lsls	r2, r2, #1
	strb	r3, [r0, #0]
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #5
	bne.n	.L_020031b2
	ldr	r0, [r6, #0]
	movs	r1, #2
	bl 0x0200bcb8
	ldr	r0, [r6, #0]
	bl 0x0200bc40
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
.L_020031b2:
	ldr	r0, [pc, #196]
	bl 0x02008584
	bl 0x020092d8
	ldr	r0, [pc, #188]
	bl 0x02008038
	movs	r0, #132
	lsls	r0, r0, #4
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_020031fe
	movs	r0, #23
	bl 0x0200bcc0
	movs	r3, #52
	movs	r2, #7
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #7
	movs	r2, #1
	movs	r3, #1
	movs	r0, #50
	bl 0x0200bbf0
	movs	r0, #23
	bl 0x0200bc40
	movs	r1, #210
	movs	r3, #240
	lsls	r1, r1, #18
	movs	r2, #0
	lsls	r3, r3, #15
.L_020031f8:
	bl 0x0200bbc0
	b.n	.L_02003204
.L_020031fe:
	ldr	r0, [pc, #128]
	bl 0x0200bdb8
.L_02003204:
	bl 0x020098a8
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #108
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_02003254
	movs	r3, #128
	movs	r1, #222
	movs	r2, #168
	lsls	r3, r3, #7
	movs	r0, #24
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200bc70
	movs	r5, #1
	movs	r0, #68
	movs	r1, #33
	movs	r2, #105
	movs	r3, #6
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbe0
	movs	r0, #69
	movs	r1, #33
	movs	r2, #105
	movs	r3, #4
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbe0
	movs	r0, #164
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bb90
.L_02003254:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #109
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_0200326c
	movs	r0, #24
	movs	r1, #0
	movs	r2, #0
	bl 0x0200bc68
.L_0200326c:
	bl 0x02009fac
	add	sp, #8
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x0200bf52
	.4byte 0x0200bfa0
	.2byte 0xbfc0
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r0, #10
	adds	r0, #255
	sub	sp, #8
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_020032c4
	ldr	r5, [pc, #644]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	bl 0x0200bc40
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #2
	ldr	r0, [r5, #0]
	bl 0x0200bcb8
	ldr	r0, [r5, #0]
	bl 0x0200bc40
	adds	r0, #35
.L_020032bc:
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
.L_020032c4:
	movs	r0, #8
	movs	r1, #1
	bl 0x0200bcb8
	movs	r0, #9
	movs	r1, #1
	bl 0x0200bcb8
	movs	r0, #10
	movs	r1, #1
	bl 0x0200bcb8
	movs	r0, #11
	movs	r1, #1
	bl 0x0200bcb8
	bl 0x0200bd98
	movs	r1, #8
	movs	r2, #9
	movs	r0, #0
	bl 0x0200bda0
	movs	r2, #11
	movs	r1, #10
	movs	r0, #1
	bl 0x0200bda0
	movs	r0, #12
	movs	r1, #1
	bl 0x0200bcb8
	movs	r0, #13
	movs	r1, #1
	bl 0x0200bcb8
	bl 0x0200bd90
	movs	r1, #130
	movs	r2, #12
	movs	r3, #13
	lsls	r1, r1, #2
	movs	r0, #0
	bl 0x0200bda8
	ldr	r0, [pc, #512]
	bl 0x0200bdb8
	movs	r0, #19
	bl 0x0200bc40
	ldr	r2, [pc, #504]
	ldr	r3, [r0, #12]
	cmp	r3, r2
	ble.n	.L_0200333c
	movs	r0, #19
	movs	r1, #1
	bl 0x0200bcb8
	b.n	.L_02003344
.L_0200333c:
	movs	r0, #19
	movs	r1, #2
	bl 0x0200bcb8
.L_02003344:
	movs	r0, #20
	bl 0x0200bcc0
	movs	r0, #20
	bl 0x0200bc40
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #2
	movs	r0, #20
	bl 0x0200bcb8
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #65
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_02003376
	bl 0x0200972c
	b.n	.L_0200338a
.L_02003376:
	movs	r3, #38
	movs	r2, #33
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #38
	movs	r1, #32
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bbf0
.L_0200338a:
	movs	r0, #14
	bl 0x0200bc40
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r6, #254
	adds	r3, r6, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #15
	bl 0x0200bc40
	adds	r0, #35
	ldrb	r2, [r0, #0]
	adds	r3, r6, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #16
	bl 0x0200bc40
	adds	r0, #35
	ldrb	r2, [r0, #0]
	adds	r3, r6, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #17
	bl 0x0200bc40
	adds	r0, #35
	ldrb	r2, [r0, #0]
	adds	r3, r6, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	ldr	r0, [pc, #344]
	bl 0x02008c64
	movs	r0, #17
	bl 0x0200bc40
	ldr	r3, [r0, #8]
	movs	r7, #0
	asrs	r3, r3, #20
	cmp	r3, #54
	bne.n	.L_02003404
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r5, [r3, #0]
	bl 0x0200bdb0
	lsls	r0, r0, #2
	adds	r5, r5, r0
	ldrb	r2, [r5, #3]
	movs	r3, #127
	ands	r3, r2
	strb	r3, [r5, #3]
	movs	r3, #11
	strb	r3, [r5, #2]
.L_02003404:
	movs	r0, #14
	movs	r1, #1
	bl 0x0200bcb8
	movs	r0, #15
	movs	r1, #1
	bl 0x0200bcb8
	bl 0x02009930
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #110
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_02003438
	movs	r3, #128
	movs	r1, #212
	movs	r2, #208
	lsls	r3, r3, #7
	movs	r0, #25
	lsls	r1, r1, #17
	lsls	r2, r2, #15
	bl 0x0200bc70
.L_02003438:
	movs	r0, #18
	movs	r1, #3
	bl 0x0200bc80
	movs	r0, #238
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_020034c0
	movs	r0, #18
	bl 0x0200bc40
	movs	r3, #27
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	adds	r5, r0, #0
	movs	r2, #1
	movs	r3, #1
	movs	r0, #27
	movs	r1, #8
	bl 0x0200bbf0
	movs	r0, #18
	movs	r1, #4
	bl 0x0200bc80
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r5, #89
	ldrb	r2, [r5, #0]
	adds	r3, r6, #0
.L_02003484:
	movs	r0, #131
	ands	r3, r2
	lsls	r0, r0, #1
	strb	r3, [r5, #0]
	adds	r0, #255
	bl 0x0200bb90
	movs	r0, #161
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bb90
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #69
	movs	r1, #51
	movs	r2, #92
	movs	r3, #11
	bl 0x0200bbe0
	movs	r3, #128
	movs	r1, #154
	movs	r2, #140
	lsls	r3, r3, #7
	movs	r0, #25
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200bc70
.L_020034c0:
	movs	r0, #135
	lsls	r0, r0, #4
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_020034e8
	movs	r0, #132
	lsls	r0, r0, #2
	bl 0x0200bb90
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #22
	bl 0x0200bb90
	movs	r0, #25
	movs	r1, #0
	movs	r2, #0
	bl 0x0200bc68
.L_020034e8:
	ldr	r0, [pc, #64]
	bl 0x020081d0
	bl 0x020093b0
	bl 0x02009fc4
	movs	r0, #27
	bl 0x0200bc40
	adds	r3, r0, #0
	adds	r3, #85
	strb	r7, [r3, #0]
	ldr	r3, [pc, #44]
	str	r7, [r0, #12]
	str	r7, [r3, #0]
	ldr	r3, [pc, #40]
	movs	r1, #144
	str	r7, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #36]
	bl 0x0200bb48
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200bfc4
	.4byte 0xfff80000
	.4byte 0x0200bfc8
	.4byte 0x0200bf80
	.4byte 0x0200d67c
	.4byte 0x0200d678
	.2byte 0x9ee9
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r5, #200
	sub	sp, #8
	lsls	r5, r5, #2
	movs	r7, #0
.L_0200354c:
	adds	r0, r5, #0
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_020035ca
	ldr	r6, [pc, #140]
	ldr	r2, [pc, #140]
	adds	r6, r7, r6
	ldrh	r3, [r6, #0]
	adds	r6, #2
	ldrh	r7, [r6, #0]
	mov	r8, r3
	adds	r5, r5, r2
	movs	r2, #1
	adds	r3, r7, #0
	mov	sl, r2
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #3
	mov	r2, r8
	movs	r1, #8
	bl 0x0200bbe0
	adds	r6, #2
	ldrh	r3, [r6, #0]
	ldrh	r7, [r6, #2]
	mov	r8, r3
	mov	r2, sl
	movs	r3, #5
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #67
	movs	r1, #3
	mov	r2, r8
	adds	r3, r7, #0
	bl 0x0200bbe0
	cmp	r5, #1
	bne.n	.L_020035ac
	mov	r3, sl
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #67
	movs	r1, #4
	movs	r2, #69
	movs	r3, #3
	bl 0x0200bbe0
.L_020035ac:
	movs	r2, #64
	negs	r2, r2
	add	r8, r2
	movs	r3, #4
	mov	r2, sl
	adds	r7, #66
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #3
	movs	r1, #69
	mov	r2, r8
	adds	r3, r7, #0
	bl 0x0200bbe0
	b.n	.L_020035da
.L_020035ca:
	movs	r3, #152
	lsls	r3, r3, #2
	adds	r3, #255
	cmp	r5, r3
	beq.n	.L_020035da
	adds	r7, #8
	adds	r5, #1
	b.n	.L_0200354c
.L_020035da:
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x0200c276
	.2byte 0xfce0
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r5, #200
	sub	sp, #8
	lsls	r5, r5, #2
	movs	r7, #0
.L_020035fc:
	adds	r0, r5, #0
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_0200367a
	ldr	r6, [pc, #140]
	ldr	r2, [pc, #140]
	adds	r6, r7, r6
	ldrh	r3, [r6, #0]
	adds	r6, #2
	ldrh	r7, [r6, #0]
	mov	r8, r3
	adds	r5, r5, r2
	movs	r2, #1
	adds	r3, r7, #0
	mov	sl, r2
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #13
	mov	r2, r8
	movs	r1, #5
	bl 0x0200bbe0
	adds	r6, #2
	ldrh	r3, [r6, #0]
	ldrh	r7, [r6, #2]
	mov	r8, r3
	mov	r2, sl
	movs	r3, #5
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #77
	movs	r1, #0
	mov	r2, r8
	adds	r3, r7, #0
	bl 0x0200bbe0
	cmp	r5, #1
	bne.n	.L_0200365c
	mov	r3, sl
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #67
	movs	r1, #4
	movs	r2, #69
	movs	r3, #3
	bl 0x0200bbe0
.L_0200365c:
	movs	r2, #64
	negs	r2, r2
	add	r8, r2
	movs	r3, #4
	mov	r2, sl
	adds	r7, #65
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #13
	movs	r1, #65
	mov	r2, r8
	adds	r3, r7, #0
	bl 0x0200bbe0
	b.n	.L_0200368a
.L_0200367a:
	movs	r3, #152
	lsls	r3, r3, #2
	adds	r3, #255
	cmp	r5, r3
	beq.n	.L_0200368a
	adds	r7, #8
	adds	r5, #1
	b.n	.L_020035fc
.L_0200368a:
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x0200c276
	.2byte 0xfce0
	.2byte 0xffff
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r6, [pc, #180]
	adds	r2, #88
	str	r2, [r3, #0]
	adds	r2, #16
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl 0x0200bc40
.L_020036bc:
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	bl 0x02009fdc
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #108
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_0200370e
	movs	r5, #1
	movs	r0, #3
	movs	r1, #8
	movs	r2, #30
	movs	r3, #33
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbe0
	movs	r3, #5
	str	r3, [sp, #4]
	movs	r0, #67
	movs	r1, #3
	movs	r2, #94
	movs	r3, #28
	str	r5, [sp, #0]
	bl 0x0200bbe0
	movs	r3, #4
	str	r3, [sp, #4]
	movs	r0, #3
	movs	r1, #69
	movs	r2, #30
	movs	r3, #93
	str	r5, [sp, #0]
	bl 0x0200bbe0
.L_0200370e:
	movs	r3, #241
	lsls	r3, r3, #1
	adds	r6, r6, r3
	ldrh	r5, [r6, #0]
	movs	r2, #224
	adds	r3, r5, #0
	subs	r3, #11
	lsls	r3, r3, #16
	lsls	r2, r2, #13
	cmp	r3, r2
	bhi.n	.L_0200373a
	bl 0x02009804
	movs	r3, #192
	lsls	r5, r5, #16
	lsls	r3, r3, #2
	asrs	r5, r5, #16
	adds	r3, #21
	adds	r5, r5, r3
	adds	r0, r5, #0
	bl 0x0200bb90
.L_0200373a:
	bl 0x0200b53c
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_0200375a
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #10
	ble.n	.L_0200375a
	cmp	r3, #39
	bgt.n	.L_0200375a
	bl 0x0200b764
.L_0200375a:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #392]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #68
	bl 0x0200bc40
	adds	r7, r0, #0
	bl 0x0200bc28
	movs	r0, #0
	bl 0x0200bd78
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200bce8
	bl 0x0200bbc8
	movs	r0, #1
	bl 0x0200bb40
	movs	r3, #130
	lsls	r3, r3, #16
	str	r3, [r7, #12]
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r5, r7, #0
	str	r3, [r7, #72]
	adds	r5, #85
	movs	r3, #0
	str	r3, [r7, #68]
	strb	r3, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r4, #214
	lsls	r4, r4, #1
	movs	r2, #128
	adds	r3, r3, r4
	lsls	r2, r2, #1
	str	r2, [r3, #0]
	bl 0x0200bd28
	bl 0x0200bd38
	movs	r0, #204
	bl 0x0200bdf8
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #24
	bl 0x0200bc20
	add	r2, sp, #28
	movs	r3, #7
	str	r3, [r2, #4]
	ldr	r3, [pc, #272]
	mov	r8, r2
	str	r3, [r2, #36]
	movs	r3, #163
	lsls	r3, r3, #8
	adds	r3, #215
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	movs	r3, #0
	mov	sl, r3
.L_020037fe:
	mov	r4, sl
	lsls	r5, r4, #12
	adds	r0, r5, #0
	bl 0x0200bb68
	add	r6, sp, #16
	movs	r3, #0
	str	r0, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r6, #4]
	bl 0x0200bb60
	ldr	r3, [r6, #0]
	str	r0, [r6, #8]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	bl 0x0200bb58
	lsls	r3, r0, #1
	ldr	r2, [r6, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #204]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	bl 0x0200bb58
	lsls	r3, r0, #1
	ldr	r5, [r6, #8]
	adds	r3, r3, r0
	ldr	r4, [pc, #192]
	lsls	r3, r3, #13
	lsrs	r3, r3, #16
	adds	r5, r5, r3
	adds	r5, r5, r4
	ldr	r4, [r6, #4]
	str	r5, [r6, #8]
	ldr	r2, [r7, #16]
	ldr	r3, [r6, #0]
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	str	r4, [sp, #0]
	ldr	r4, [pc, #172]
	str	r5, [sp, #4]
	str	r4, [sp, #8]
	mov	r4, r8
	str	r4, [sp, #12]
	bl 0x02008fc4
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_020037fe
	movs	r0, #188
	bl 0x0200bdf8
	ldr	r5, [pc, #128]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r5, r5, r4
	movs	r1, #2
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl 0x0200bcd0
	ldr	r0, [r5, #0]
	movs	r1, #49
	bl 0x0200bc80
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200bc00
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200bc00
	bl 0x0200bc08
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200bcd0
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #68]
	movs	r0, #10
	bl 0x0200bc20
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200bc80
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #88
	str	r2, [r3, #0]
	bl 0x0200bc30
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200a641
	.4byte 0xffffa000
	.4byte 0xffffd000
	.2byte 0x0001
	.2byte 0x0109
	push	{r5, r6, lr}
	ldr	r3, [pc, #264]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl 0x0200bc40
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #88
	str	r2, [r3, #0]
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #120
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_02003988
	bl 0x0200ba30
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #113
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_02003988
	movs	r3, #15
	movs	r2, #5
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r3, #1
	movs	r1, #0
	movs	r2, #1
	bl 0x0200bbf0
	ldr	r3, [pc, #172]
	ldr	r0, [r3, #0]
	cmp	r0, #0
	beq.n	.L_02003988
	movs	r1, #248
	movs	r3, #176
	lsls	r1, r1, #16
	movs	r2, #0
	lsls	r3, r3, #15
	bl 0x0200bbc0
	movs	r0, #1
	bl 0x0200bb40
.L_02003988:
	ldr	r0, [pc, #148]
	bl 0x02008038
	bl 0x020099b8
	ldr	r3, [pc, #132]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r6, r3, r2
	movs	r3, #0
	ldrsh	r5, [r6, r3]
	cmp	r5, #10
	ble.n	.L_020039b2
	bl 0x02009804
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r2, #21
	adds	r0, r5, r2
	bl 0x0200bb90
.L_020039b2:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #113
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_020039ca
	movs	r0, #9
	movs	r1, #0
.L_020039c4:
	movs	r2, #0
	bl 0x0200bc68
.L_020039ca:
	bl 0x0200b5ec
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_020039fe
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #16
	ble.n	.L_020039fe
	cmp	r3, #37
	bne.n	.L_020039fa
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #113
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_020039fa
	bl 0x0200ac14
	b.n	.L_020039fe
.L_020039fa:
	bl 0x0200b764
.L_020039fe:
	ldr	r3, [pc, #36]
	movs	r2, #0
	str	r2, [r3, #0]
	ldr	r3, [pc, #32]
	movs	r1, #144
	str	r2, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #28]
	bl 0x0200bb48
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200c420
	.4byte 0x0200bfb0
	.4byte 0x0200d67c
	.4byte 0x0200d678
	.2byte 0x9ee9
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r0, #234
	adds	r0, #255
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	ldr	r5, [pc, #92]
	bl 0x0200bbb0
	movs	r7, #0
	str	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L_02003a98
	ldr	r6, [r0, #80]
	movs	r3, #33
	ldrb	r2, [r6, #5]
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r3, r2
	strb	r3, [r6, #9]
	adds	r3, r0, #0
	adds	r3, #85
	adds	r2, r0, #0
	adds	r2, #92
	strb	r7, [r3, #0]
	movs	r1, #193
	movs	r3, #1
	strb	r3, [r2, #0]
	lsls	r1, r1, #3
	strb	r7, [r6, #26]
	strb	r7, [r6, #27]
	movs	r0, #68
	bl 0x0200bb70
	adds	r5, r0, #0
	movs	r0, #199
	bl 0x0200bc18
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r5, r5, r3
	ldrb	r0, [r6, #16]
	movs	r1, #128
	adds	r2, r5, #0
	bl 0x0200bb80
	movs	r0, #68
	bl 0x0200bb78
.L_02003a98:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xc420
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r5, [pc, #120]
	adds	r2, #88
	str	r2, [r3, #0]
	adds	r2, #16
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x0200bc40
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #95
	bl 0x0200bb88
	cmp	r0, #0
	beq.n	.L_02003adc
	movs	r0, #133
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bb90
.L_02003adc:
	ldr	r0, [pc, #76]
	bl 0x02008038
	ldr	r0, [pc, #76]
	bl 0x020081d0
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_02003b00
	bl 0x0200a140
	movs	r0, #10
	bl 0x0200bb40
.L_02003b00:
	bl 0x02009ff4
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bb88
	cmp	r0, #0
	bne.n	.L_02003b26
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r2, #0
.L_02003b18:
	ldrsh	r3, [r3, r2]
	cmp	r3, #5
	ble.n	.L_02003b26
	cmp	r3, #39
	bgt.n	.L_02003b26
	bl 0x0200b764
.L_02003b26:
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x0200bfb8
	.2byte 0xbf92
	.2byte 0x0200
	.global Func_02003b34
	.thumb_func
Func_02003b34:
	.2byte 0x2000
	.2byte 0x4770
	.section .rodata,"a",%progbits
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x0000002c
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x0200be60
	.4byte 0x0200be9c
	.4byte 0x0200bed8
	.4byte 0x02100008
	.4byte 0x02120009
	.4byte 0x0214000a
	.4byte 0x000bffff
	.4byte 0x000c0210
	.4byte 0x000d0212
	.4byte 0x000e0214
	.4byte 0x000f0216
	.4byte 0x00100218
	.4byte 0x0011021a
	.4byte 0x0012021c
	.4byte 0x0013021e
	.4byte 0x00140220
	.4byte 0x00150222
	.4byte 0xffff0224
	.4byte 0x02100015
	.4byte 0x02120016
	.4byte 0x02140017
	.4byte 0x02160018
	.4byte 0x0009ffff
	.4byte 0xffff0210
	.4byte 0x0001000b
	.4byte 0xffff0203
	.4byte 0x00010016
	.4byte 0xffff0204
	.4byte 0x00010012
	.4byte 0xffff0205
	.4byte 0x00010008
	.4byte 0xffff0206
	.4byte 0x00010008
	.4byte 0xffff0207
	.4byte 0xffff0017
	.4byte 0xffff0013
	.4byte 0x000f000e
	.4byte 0x00110010
	.4byte 0x0004ffff
	.4byte 0x00050021
	.4byte 0x00440021
	.4byte 0x00450021
	.4byte 0x00450021
	.4byte 0x00460033
	.4byte 0x00410033
	.4byte 0x00400040
	.4byte 0x00010040
	.4byte 0x00020024
	.4byte 0x00030024
	.4byte 0x00040024
	.4byte 0x00050024
	.4byte 0x00060024
	.4byte 0x00070024
	.4byte 0x00080024
	.4byte 0x00410024
	.4byte 0x00420024
	.4byte 0x00430024
	.4byte 0x00440024
	.4byte 0x00450024
	.4byte 0x00460024
	.4byte 0x00470024
	.4byte 0x00480024
	.4byte 0x00420024
	.4byte 0x00430036
	.4byte 0x00440036
	.4byte 0x00450036
	.4byte 0x00460036
	.4byte 0x00470036
	.4byte 0x00480036
	.4byte 0x00490036
	.4byte 0x00400036
	.4byte 0x00410042
	.4byte 0x00420042
	.4byte 0x00430042
	.4byte 0x00440042
	.4byte 0x00000042
	.4byte 0x000c004c
	.4byte 0x004c0000
	.4byte 0x00000030
	.4byte 0x000d004d
	.4byte 0x004d0000
	.4byte 0x00010031
	.4byte 0x000c004f
	.4byte 0x004f0000
	.4byte 0x00000030
	.4byte 0x000e004f
	.4byte 0x004f0001
	.4byte 0x00000032
	.4byte 0x000c0051
	.4byte 0x00510000
	.4byte 0x00000030
	.4byte 0x00110076
	.4byte 0x00760004
	.4byte 0x00010035
	.4byte 0x00100077
	.4byte 0x00770000
	.4byte 0x00000034
	.4byte 0x00120077
	.4byte 0x00770007
	.4byte 0x00010036
	.4byte 0x000c0078
	.4byte 0x00780007
	.4byte 0x00000030
	.4byte 0x000e0078
	.4byte 0x00780007
	.4byte 0x00000032
	.4byte 0x00110078
	.4byte 0x00780002
	.4byte 0x00000035
	.4byte 0x000c007a
	.4byte 0x007a0007
	.4byte 0x00010030
	.4byte 0x0010007a
	.4byte 0x007a0000
	.4byte 0x00000034
	.4byte 0x0012007a
	.4byte 0x007a0005
	.4byte 0x00010036
	.4byte 0x00040069
	.4byte 0x00290000
	.4byte 0x00000024
	.4byte 0x0011006d
	.4byte 0x002d0000
	.4byte 0x00000031
	.4byte 0x0013006e
	.4byte 0x002e0003
	.4byte 0x00000033
	.4byte 0x0011006f
	.4byte 0x002f0003
	.4byte 0x00000031
	.4byte 0x00150070
	.4byte 0x00300000
	.4byte 0x00000035
	.4byte 0x00140071
	.4byte 0x00310003
	.4byte 0x00000034
	.4byte 0x00120072
	.4byte 0x00320000
	.4byte 0x00000032
	.4byte 0x00060069
	.4byte 0x00290000
	.4byte 0x00010026
	.4byte 0x000b005b
	.4byte 0x001b0000
	.4byte 0x0000002b
	.4byte 0x000b005c
	.4byte 0x001c0000
	.4byte 0x0001002b
	.4byte 0x0009005d
	.4byte 0x001d0000
	.4byte 0x00010029
	.4byte 0x000c005d
	.4byte 0x001d0000
	.4byte 0x0001002c
	.4byte 0x0007006d
	.4byte 0x002d0000
	.4byte 0x00010027
	.4byte 0x00060070
	.4byte 0x00300000
	.4byte 0x00010026
	.4byte 0x0008000d
	.4byte 0x000d0002
	.4byte 0x00000048
	.4byte 0x0009000d
	.4byte 0x000d0000
	.4byte 0x00000049
	.4byte 0x000b000d
	.4byte 0x000d0001
	.4byte 0x0000004b
	.4byte 0x0006000d
	.4byte 0x000d0000
	.4byte 0x00010046
	.4byte 0x0006000f
	.4byte 0x000f0000
	.4byte 0x00000046
	.4byte 0x0008000f
	.4byte 0x000f0000
	.4byte 0x00000048
	.4byte 0x000a000f
	.4byte 0x000f0001
	.4byte 0x0001004a
	.4byte 0x000a000e
	.4byte 0x000e0000
	.4byte 0x0000004a
	.4byte 0x000c000e
	.4byte 0x000e0000
	.4byte 0x0001004c
	.4byte 0x000c000f
	.4byte 0x000f0000
	.4byte 0x0000004c
	.4byte 0x00060010
	.4byte 0x00100000
	.4byte 0x00010046
	.4byte 0x00080010
	.4byte 0x00100002
	.4byte 0x00000048
	.4byte 0x00090010
	.4byte 0x00100000
	.4byte 0x00000049
	.4byte 0x000b0010
	.4byte 0x00100000
	.4byte 0x0001004b
	.4byte 0x00090012
	.4byte 0x00120000
	.4byte 0x00000049
	.4byte 0x000b0012
	.4byte 0x00120000
	.4byte 0x0004004b
	.4byte 0x00440007
	.4byte 0x00050002
	.4byte 0x00450008
	.4byte 0x00070003
	.4byte 0x00470007
	.4byte 0x00070002
	.4byte 0x00470009
	.4byte 0x00090004
	.4byte 0x00490007
	.4byte 0x00280002
	.4byte 0x0068000c
	.4byte 0x00290007
	.4byte 0x0069000b
	.4byte 0x00290006
	.4byte 0x0069000d
	.4byte 0x002a0008
	.4byte 0x006a0005
	.4byte 0x002a0000
	.4byte 0x006a0009
	.4byte 0x002a0004
	.4byte 0x006a000c
	.4byte 0x002c0007
	.4byte 0x006c0007
	.4byte 0x002c0002
	.4byte 0x006c000b
	.4byte 0x002c0006
	.4byte 0x006c000d
	.4byte 0x000b0008
	.4byte 0x004b0017
	.4byte 0x000c0012
	.4byte 0x004c0019
	.4byte 0x000d0014
	.4byte 0x004d0017
	.4byte 0x000e0012
	.4byte 0x004e001b
	.4byte 0x000f0016
	.4byte 0x004f001a
	.4byte 0x00100015
	.4byte 0x00500018
	.4byte 0x001e0013
	.4byte 0x005e0023
	.4byte 0x001a001e
	.4byte 0x005a0009
	.4byte 0x001b0004
	.4byte 0x005b0009
	.4byte 0x001c0004
	.4byte 0x005c0007
	.4byte 0x001c0002
	.4byte 0x005c000a
	.4byte 0x000c0005
	.4byte 0x004c0008
	.4byte 0x000f0003
	.4byte 0x004f0007
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x001e0000
	.4byte 0x005e0021
	.4byte 0x0000001c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000002f0
	.4byte 0x4000015c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x00106002
	.4byte 0x00201025
	.4byte 0x00302025
	.4byte 0x00404025
	.4byte 0x00501023
	.4byte 0x00607022
	.4byte 0x00706022
	.4byte 0x00b0b025
	.4byte 0x00c0c025
	.4byte 0x00d0d025
	.4byte 0x00e0e025
	.4byte 0x00f0f025
	.4byte 0x01010025
	.4byte 0x01111025
	.4byte 0x01212025
	.4byte 0x01313025
	.4byte 0x01414025
	.4byte 0x01515025
	.4byte 0x01616025
	.4byte 0x01717025
	.4byte 0x01818025
	.4byte 0x00000023
	.4byte 0x00105022
	.4byte 0x00206025
	.4byte 0x00305025
	.4byte 0x00401024
	.4byte 0x00506023
	.4byte 0x00605023
	.4byte 0x00f19025
	.4byte 0x0101a025
	.4byte 0x0111b025
	.4byte 0x0121c025
	.4byte 0x0131d025
	.4byte 0x0141e025
	.4byte 0x0151f025
	.4byte 0x00e3c025
	.4byte 0x00000024
	.4byte 0x00104023
	.4byte 0x00203025
	.4byte 0x00301026
	.4byte 0x00404026
	.4byte 0x00505026
	.4byte 0x00607002
	.4byte 0x00708024
	.4byte 0x00807024
	.4byte 0x00c20025
	.4byte 0x00d21025
	.4byte 0x00e22025
	.4byte 0x00f23025
	.4byte 0x01024026
	.4byte 0x01125026
	.4byte 0x00000025
	.4byte 0x00102022
	.4byte 0x00203022
	.4byte 0x00302024
	.4byte 0x00404022
	.4byte 0x00503023
	.4byte 0x00602023
	.4byte 0x0282a025
	.4byte 0x0292b025
	.4byte 0x02a28025
	.4byte 0x02b29025
	.4byte 0x00000026
	.4byte 0x00103024
	.4byte 0x00201027
	.4byte 0x00302027
	.4byte 0x00404024
	.4byte 0x00505024
	.4byte 0x00603027
	.4byte 0x0070a026
	.4byte 0x00809026
	.4byte 0x00908026
	.4byte 0x00a07026
	.4byte 0x01208027
	.4byte 0x01309027
	.4byte 0x0140a027
	.4byte 0x0150b027
	.4byte 0x0160c027
	.4byte 0x0170d027
	.4byte 0x0180e027
	.4byte 0x0190f027
	.4byte 0x01a10027
	.4byte 0x01b11027
	.4byte 0x01c12027
	.4byte 0x01d13027
	.4byte 0x01e14027
	.4byte 0x01f15027
	.4byte 0x02016027
	.4byte 0x02117027
	.4byte 0x00000027
	.4byte 0x00102026
	.4byte 0x00203026
	.4byte 0x00306026
	.4byte 0x02829027
	.4byte 0x02928027
	.4byte 0x02a2b027
	.4byte 0x02b2a027
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x03980000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02900000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x000a0000
	.4byte 0x00000000
	.4byte 0x000a0000
	.4byte 0x00024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x000a0000
	.4byte 0x00000000
	.4byte 0x000a0000
	.4byte 0x00024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x000a0000
	.4byte 0x00000000
	.4byte 0x000a0000
	.4byte 0x00024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff00f5
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff00fa
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff00fa
	.4byte 0x00000001
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff00fa
	.4byte 0x00000001
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff00fa
	.4byte 0x00000001
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x0000000e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x0000000f
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff00f5
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03980000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00024000
	.4byte 0xffff00f5
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01024000
	.4byte 0x005f00f5
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x0200aff5
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff001e
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff001f
	.4byte 0x00000007
	.4byte 0x00000202
	.4byte 0xffff0009
	.4byte 0x020092c9
	.4byte 0x50008615
	.4byte 0x0203000b
	.4byte 0x02009555
	.4byte 0x00000009
	.4byte 0x03010000
	.4byte 0x020095e1
	.4byte 0x00008c15
	.4byte 0x0301000c
	.4byte 0x020095e1
	.4byte 0x00000002
	.4byte 0x02000006
	.4byte 0x020094ed
	.4byte 0x00000002
	.4byte 0x12000007
	.4byte 0x02009501
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x020097f5
	.4byte 0x00000002
	.4byte 0x0201000b
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x0201000c
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x0201000d
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x0201000e
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x0201000f
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010010
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010011
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010012
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010013
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010014
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010015
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010016
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010017
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010018
	.4byte 0x02009a31
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x0200aff5
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x00000006
	.4byte 0x00000002
	.4byte 0x086c001e
	.4byte 0x0200a1e9
	.4byte 0x00000002
	.4byte 0x086d001f
	.4byte 0x0200a2e1
	.4byte 0x00008602
	.4byte 0xffff0008
	.4byte 0x02009369
	.4byte 0x00000202
	.4byte 0xffff0009
	.4byte 0x02009369
	.4byte 0x50008615
	.4byte 0x02040016
	.4byte 0x02009569
	.4byte 0x00000008
	.4byte 0x08400000
	.4byte 0x02009631
	.4byte 0x00000009
	.4byte 0x08400000
	.4byte 0x02009641
	.4byte 0x10008c15
	.4byte 0x08400017
	.4byte 0x02009631
	.4byte 0x00008c15
	.4byte 0x08400017
	.4byte 0x02009641
	.4byte 0x00000002
	.4byte 0x02000006
	.4byte 0x020094ed
	.4byte 0x00000002
	.4byte 0x12000007
	.4byte 0x02009501
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x020097f5
	.4byte 0x00000002
	.4byte 0x0201000e
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x0201000f
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010010
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010011
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010012
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010013
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010014
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010015
	.4byte 0x02009a31
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0018
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0019
	.4byte 0x00000008
	.4byte 0x00000002
	.4byte 0x086e001e
	.4byte 0x0200a391
	.4byte 0x00000002
	.4byte 0x086f001f
	.4byte 0x0200a459
	.4byte 0x00000002
	.4byte 0x08700020
	.4byte 0x0200a7e1
	.4byte 0x00008515
	.4byte 0x0208000c
	.4byte 0x00000000
	.4byte 0x50008615
	.4byte 0x02050012
	.4byte 0x0200957d
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x020096a1
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x020096b1
	.4byte 0x10008c15
	.4byte 0xffff0013
	.4byte 0x020096a1
	.4byte 0x00008c15
	.4byte 0xffff0013
	.4byte 0x020096b1
	.4byte 0x00008c15
	.4byte 0x08410014
	.4byte 0x02009789
	.4byte 0x00008602
	.4byte 0x08410009
	.4byte 0x020097a1
	.4byte 0x00000602
	.4byte 0xffff0014
	.4byte 0x0200a0dd
	.4byte 0x00008602
	.4byte 0xffff0015
	.4byte 0x0200a0dd
	.4byte 0x10008c15
	.4byte 0xffff000e
	.4byte 0x0200a115
	.4byte 0x10008c15
	.4byte 0xffff000f
	.4byte 0x0200a115
	.4byte 0x10008c15
	.4byte 0xffff0010
	.4byte 0x0200a115
	.4byte 0x10008c15
	.4byte 0xffff0011
	.4byte 0x0200a115
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte 0x0200a125
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte 0x0200a125
	.4byte 0x00008c15
	.4byte 0xffff0010
	.4byte 0x0200a125
	.4byte 0x00008c15
	.4byte 0xffff0011
	.4byte 0x0200a125
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x020093fd
	.4byte 0x00008602
	.4byte 0xffff000b
	.4byte 0x020093fd
	.4byte 0x00000602
	.4byte 0xffff001a
	.4byte 0x020093fd
	.4byte 0x00004602
	.4byte 0xffff001b
	.4byte 0x020093fd
	.4byte 0x00000002
	.4byte 0x02000007
	.4byte 0x020094ed
	.4byte 0x00000002
	.4byte 0x12000008
	.4byte 0x02009501
	.4byte 0x00000002
	.4byte 0x0201000c
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x0201000d
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x0201000e
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x0201000f
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010010
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010011
	.4byte 0x02009a31
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000021
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000021
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0028
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0xffff0029
	.4byte 0x00000029
	.4byte 0x00000001
	.4byte 0xffff002a
	.4byte 0x0000002a
	.4byte 0x00000001
	.4byte 0xffff002b
	.4byte 0x0000002b
	.4byte 0x00000002
	.4byte 0x02000008
	.4byte 0x020094ed
	.4byte 0x00000002
	.4byte 0x12000009
	.4byte 0x02009501
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000021
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000021
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000002
	.4byte 0x08710017
	.4byte 0x0200aa5d
	.4byte 0x50008615
	.4byte 0x02060008
	.4byte 0x02009591
	.4byte 0x00000003
	.4byte 0x0878000c
	.4byte 0x0200af9d
	.4byte 0x00000002
	.4byte 0x02010012
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010013
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010014
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010015
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010016
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010017
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010018
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010019
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x0201001a
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x0201001b
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x0201001c
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x0201001d
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x0201001e
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x0201001f
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010020
	.4byte 0x02009a31
	.4byte 0x00000002
	.4byte 0x02010021
	.4byte 0x02009a31
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0028
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0xffff0029
	.4byte 0x00000029
	.4byte 0x00000001
	.4byte 0xffff002a
	.4byte 0x0000002a
	.4byte 0x00000001
	.4byte 0xffff002b
	.4byte 0x0000002b
	.4byte 0x00000202
	.4byte 0xffff0008
	.4byte 0x02009445
	.4byte 0x50008615
	.4byte 0x02070008
	.4byte 0x020095a5
	.4byte 0x00000002
	.4byte 0x02020009
	.4byte 0x0200a141
	.4byte 0x00000002
	.4byte 0x1202000a
	.4byte 0x0200a181
	.4byte 0x00000002
	.4byte 0x02000005
	.4byte 0x020094ed
	.4byte 0x00000002
	.4byte 0x12000006
	.4byte 0x02009501
	.4byte 0x00000002
	.4byte 0x0209000b
	.4byte 0x0200af75
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0200918d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
