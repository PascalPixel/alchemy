.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x020092dd, 0x02009069, 0x020090a9, 0x020090b1, 0x020092b1, 0x02009071, 0x020093ed
	overlay_veneer \EntryTarget
	.endr
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
	bl 0x02009498
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
	bl 0x02009468
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
	bl 0x02009498
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
	bl 0x02009420
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
	bl 0x02009498
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
	bl 0x02009508
	lsls	r0, r0, #2
	adds	r5, r5, r0
	mov	r0, r8
	bl 0x02009418
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
	bl 0x02009428
	b.n	.L_020001ca
.L_02000188:
	adds	r0, r7, #0
	movs	r1, #4
	bl 0x02009428
	b.n	.L_020001ca
.L_02000192:
	adds	r0, r7, #0
	movs	r1, #6
	bl 0x02009428
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
	bl 0x02009428
.L_020001b6:
	b.n	.L_020001ca
.L_020001b8:
	adds	r0, r7, #0
	movs	r1, #3
	bl 0x02009428
	b.n	.L_020001ca
.L_020001c2:
	adds	r0, r7, #0
	movs	r1, #5
	bl 0x02009428
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
	bl 0x02009498
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
	bl 0x02009418
	cmp	r0, #0
	beq.n	.L_02000244
	ldr	r0, [sp, #12]
	movs	r1, #0
	movs	r2, #0
	bl 0x020094a0
	b.n	.L_020002a4
.L_02000244:
	adds	r0, r7, #0
	bl 0x02009508
	mov	r8, r0
	mov	r3, r8
	lsls	r3, r3, #2
	adds	r5, r5, r3
	str	r5, [sp, #4]
	mov	r2, r9
	ldrb	r0, [r2, #0]
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	bl 0x02009478
	mov	r3, r9
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	mov	sl, r0
	ldrb	r0, [r3, #0]
	bl 0x02009458
	adds	r5, r0, #0
	ldr	r0, [sp, #12]
	bl 0x020094c0
	ldr	r2, [sp, #4]
	movs	r3, #128
	asrs	r5, r5, #19
	strb	r3, [r2, #3]
	adds	r5, #4
	mov	r3, r9
	adds	r2, r5, #0
	ldrb	r0, [r3, #0]
	mov	r1, sl
	bl 0x02009528
	add	r8, r6
	mov	r2, r8
	strb	r0, [r2, #0]
	ldr	r0, [sp, #8]
	bl 0x02009418
	cmp	r0, #0
	beq.n	.L_020002a4
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x02009428
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
	bl 0x02009498
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	bl 0x02009458
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
	bl 0x020094d8
	cmp	r0, #0
	beq.n	.L_0200030e
	b.n	.L_0200047e
.L_0200030e:
	ldr	r3, [pc, #376]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x02009498
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r1, #0
	ldr	r3, [r6, #12]
	str	r3, [r0, #4]
	ldr	r3, [r6, #16]
	str	r3, [r0, #8]
	bl 0x02009530
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_0200033a
	b.n	.L_0200047e
.L_0200033a:
	b.n	.L_02000470
.L_0200033c:
	ldrh	r7, [r5, #0]
	adds	r0, r7, #0
	bl 0x02009498
	cmp	r0, r8
	beq.n	.L_0200034c
	adds	r5, #4
	b.n	.L_02000470
.L_0200034c:
	ldrh	r5, [r5, #2]
	bl 0x02009488
	adds	r0, r5, #0
	bl 0x02009418
	cmp	r0, #0
	bne.n	.L_020003ba
	movs	r0, #125
	bl 0x02009538
	adds	r0, r7, #0
	bl 0x02009498
	movs	r1, #7
	bl 0x020094b8
	movs	r0, #2
	bl 0x020093f8
	movs	r1, #0
	mov	r0, r8
	bl 0x02009428
	adds	r0, r7, #0
	bl 0x02009498
	movs	r1, #0
	bl 0x020094b8
	movs	r0, #2
	bl 0x020093f8
	adds	r0, r7, #0
	bl 0x02009498
	movs	r1, #7
	bl 0x020094b8
	movs	r0, #4
	bl 0x020093f8
	adds	r0, r7, #0
	bl 0x02009498
	movs	r1, #0
	bl 0x020094b8
	movs	r0, #0
	bl 0x02008974
	adds	r0, r5, #0
	bl 0x02009420
	b.n	.L_0200046a
.L_020003ba:
	adds	r5, #1
	mov	sl, r5
	mov	r0, sl
	bl 0x02009418
	cmp	r0, #0
	bne.n	.L_0200046a
	adds	r6, #85
	strb	r0, [r6, #0]
	movs	r0, #185
	bl 0x02009538
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #9
	bl 0x02009470
	movs	r0, #0
	bl 0x02008974
	movs	r5, #2
	movs	r0, #8
	mov	r7, r8
	bl 0x020093f8
	negs	r5, r5
	mov	r0, r8
	movs	r1, #2
	adds	r7, #34
	bl 0x02009428
	ldrb	r1, [r7, #0]
	adds	r0, r5, #0
	bl 0x02008c4c
	movs	r0, #1
	bl 0x02008974
	movs	r0, #16
	bl 0x020093f8
	ldrb	r1, [r7, #0]
	adds	r0, r5, #0
	bl 0x02008c4c
	movs	r0, #4
	bl 0x020093f8
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x02009470
	movs	r0, #8
	bl 0x020093f8
	movs	r3, #3
	strb	r3, [r6, #0]
	movs	r0, #5
	bl 0x020093f8
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	mov	r0, r8
	bl 0x02009440
	movs	r0, #2
	bl 0x020093f8
	movs	r0, #188
	bl 0x02009538
	bl 0x02008aa8
	movs	r0, #20
	bl 0x020093f8
	mov	r0, sl
	bl 0x02009420
.L_0200046a:
	bl 0x02009490
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
	bl 0x02009498
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
	bl 0x02009488
	adds	r3, r5, #1
	mov	r8, r3
	mov	r0, r8
	bl 0x02009418
	cmp	r0, #0
	bne.n	.L_02000554
	movs	r0, #185
	bl 0x02009538
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #9
	bl 0x02009470
	movs	r0, #0
	bl 0x02008974
	movs	r0, #8
	bl 0x020093f8
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x02009428
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
	bl 0x020093f8
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x02009470
	movs	r0, #8
	bl 0x020093f8
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	adds	r0, r7, #0
	bl 0x02009440
	movs	r0, #2
	bl 0x020093f8
	movs	r0, #188
	bl 0x02009538
	bl 0x02008aa8
	movs	r0, #20
	bl 0x020093f8
	adds	r0, r5, #0
	bl 0x02009420
	mov	r0, r8
	bl 0x02009420
.L_02000554:
	bl 0x02009490
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
	bl 0x02009498
	movs	r3, #3
	adds	r0, #92
	strb	r3, [r0, #0]
	adds	r0, r5, #0
	bl 0x020094c0
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
	bl 0x020094a8
	ldr	r1, [r5, #0]
	movs	r0, #9
	bl 0x020094a8
	ldr	r1, [r5, #0]
	movs	r0, #10
	bl 0x020094a8
	movs	r0, #1
	bl 0x020093f8
	movs	r0, #8
	bl 0x0200856c
	movs	r0, #9
	bl 0x0200856c
	movs	r0, #10
	bl 0x0200856c
	movs	r1, #0
	movs	r0, #9
	bl 0x020094b0
	movs	r0, #1
	bl 0x020093f8
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x020094a0
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x020094a0
	movs	r2, #0
	movs	r0, #10
	movs	r1, #0
	bl 0x020094a0
	movs	r0, #1
	bl 0x020093f8
	b.n	.L_020006be
.L_0200060a:
	mov	r3, fp
	ldrh	r3, [r3, #0]
	mov	r9, r3
	mov	r0, r9
	bl 0x02009498
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
	bl 0x02009418
	cmp	r0, #0
	beq.n	.L_02000660
	mov	r0, r9
	movs	r1, #0
	movs	r2, #0
	bl 0x020094a0
	b.n	.L_020006ba
.L_02000660:
	adds	r0, r5, #0
	bl 0x02009508
	mov	r8, r0
	mov	r3, r8
	lsls	r3, r3, #2
	adds	r6, r6, r3
	str	r6, [sp, #4]
	add	r8, sl
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	ldrb	r0, [r7, #0]
	bl 0x02009478
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	mov	sl, r0
	ldrb	r0, [r7, #0]
	bl 0x02009458
	adds	r5, r0, #0
	mov	r0, r9
	bl 0x020094c0
	ldr	r6, [sp, #4]
	asrs	r5, r5, #19
	movs	r3, #128
	adds	r5, #4
	adds	r2, r5, #0
	strb	r3, [r6, #3]
	ldrb	r0, [r7, #0]
	mov	r1, sl
	bl 0x02009528
	mov	r2, r8
	strb	r0, [r2, #0]
	ldr	r0, [sp, #8]
	bl 0x02009418
	cmp	r0, #0
	beq.n	.L_020006ba
	mov	r0, r9
	movs	r1, #9
	bl 0x020094c8
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
	bl 0x020093f8
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
	bl 0x02009440
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x02009440
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #12
	adds	r6, r0, #0
	bl 0x020094d8
	cmp	r0, #0
	beq.n	.L_02000720
	b.n	.L_020008e2
.L_02000720:
	ldr	r3, [pc, #460]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x02009498
	adds	r5, r0, #0
	movs	r0, #8
	bl 0x02009498
	ldr	r3, [r5, #8]
	adds	r7, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r1, #0
	ldr	r3, [r5, #12]
	str	r3, [r0, #4]
	ldr	r3, [r5, #16]
	str	r3, [r0, #8]
	bl 0x02009530
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
	bl 0x02009498
	cmp	r0, sl
	beq.n	.L_02000768
	adds	r6, #4
	b.n	.L_020008d4
.L_02000768:
	ldrh	r6, [r6, #2]
	bl 0x02009488
	adds	r0, r6, #0
	bl 0x02009418
	cmp	r0, #0
	bne.n	.L_02000802
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x02009428
	mov	r1, sl
	adds	r0, r7, #0
	bl 0x020086ec
	movs	r0, #1
	bl 0x020093f8
	movs	r0, #125
	bl 0x02009538
	movs	r0, #8
	bl 0x02009498
	movs	r1, #7
	bl 0x020094b8
	movs	r0, #2
	bl 0x020093f8
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x02009428
	movs	r1, #9
	mov	r0, r8
	bl 0x020094c8
	movs	r0, #8
	bl 0x02009498
	movs	r1, #0
	bl 0x020094b8
	movs	r0, #2
	bl 0x020093f8
	movs	r0, #8
	bl 0x02009498
	movs	r1, #7
	bl 0x020094b8
	movs	r0, #4
	bl 0x020093f8
	movs	r0, #8
	bl 0x02009498
	movs	r1, #0
	bl 0x020094b8
	movs	r0, #0
	bl 0x02008974
	mov	r0, sl
	adds	r1, r7, #0
	bl 0x020086ec
	movs	r0, #1
	bl 0x020093f8
	adds	r0, r6, #0
	bl 0x02009420
	b.n	.L_020008ce
.L_02000802:
	adds	r6, #1
	mov	r9, r6
	mov	r0, r9
	bl 0x02009418
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_020008ce
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x02009428
	mov	r1, sl
	adds	r0, r7, #0
	bl 0x020086ec
	adds	r5, #85
	movs	r0, #1
	bl 0x020093f8
	strb	r6, [r5, #0]
	movs	r0, #185
	bl 0x02009538
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #9
	bl 0x02009470
	movs	r0, #0
	bl 0x02008974
	mov	r8, r5
	movs	r0, #8
	movs	r6, #2
	mov	r5, sl
	bl 0x020093f8
	negs	r6, r6
	adds	r0, r7, #0
	movs	r1, #2
	adds	r5, #34
	bl 0x02009428
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	bl 0x02008c4c
	movs	r0, #1
	bl 0x02008974
	movs	r0, #16
	bl 0x020093f8
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	bl 0x02008c4c
	movs	r0, #4
	bl 0x020093f8
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x02009470
	movs	r0, #8
	bl 0x020093f8
	movs	r3, #3
	mov	r2, r8
	strb	r3, [r2, #0]
	movs	r0, #5
	bl 0x020093f8
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	adds	r0, r7, #0
	bl 0x02009440
	movs	r0, #2
	bl 0x020093f8
	movs	r0, #188
	bl 0x02009538
	bl 0x02008aa8
	movs	r0, #20
	bl 0x020093f8
	mov	r0, r9
	bl 0x02009420
.L_020008ce:
	bl 0x02009490
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
	bl 0x020093f0
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
	bl 0x02009498
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
	bl 0x02009498
	movs	r2, #0
	adds	r7, r0, #0
	mov	r9, r2
	mov	sl, r2
.L_02000996:
	bl 0x02009400
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
	bl 0x02009438
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02000a6a
	mov	r1, r9
	ldr	r0, [r6, #80]
	bl 0x020094f0
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	adds	r2, #4
	strb	r3, [r2, #0]
	movs	r1, #0
	mov	r9, r0
	adds	r0, r6, #0
	bl 0x02009468
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x02009428
	adds	r0, r6, #0
	ldr	r1, [pc, #152]
	bl 0x02009430
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
	bl 0x02009410
	ldr	r3, [pc, #108]
	ldr	r1, [pc, #108]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x6470
	adds	r0, r5, #0
	bl 0x02009408
	b.n	.L_02000a38
.L_02000a34:
	mov	r0, r8
	str	r0, [r6, #68]
.L_02000a38:
	str	r0, [r6, #76]
	bl 0x02009400
	movs	r2, #192
	lsls	r0, r0, #14
	lsls	r2, r2, #7
	lsrs	r0, r0, #16
	adds	r0, r0, r2
	negs	r0, r0
	str	r0, [r6, #72]
	bl 0x02009400
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
	.4byte 0x02009540
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
	bl 0x02009498
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
	bl 0x02009438
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02000b92
	mov	r1, r9
	ldr	r0, [r7, #80]
	bl 0x020094f0
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
	bl 0x02009468
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x02009428
	ldr	r1, [pc, #160]
	adds	r0, r7, #0
	bl 0x02009430
	mov	r3, sl
	lsls	r5, r3, #12
	adds	r0, r5, #0
	bl 0x02009410
	mov	r4, r8
	str	r4, [r7, #72]
	str	r0, [r7, #68]
	adds	r0, r5, #0
	bl 0x02009408
	ldr	r3, [r7, #68]
	str	r0, [r7, #76]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r7, #68]
	bl 0x02009400
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
	bl 0x02009400
	lsls	r3, r0, #1
	ldr	r2, [r7, #76]
	adds	r3, r3, r0
	ldr	r4, [pc, #96]
	lsls	r3, r3, #13
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	adds	r2, r2, r4
	str	r2, [r7, #76]
	bl 0x02009400
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
	.4byte 0x02009570
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
	bl 0x02009498
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
	bl 0x02009478
	ldr	r2, [r7, #16]
	mov	r8, r0
	ldr	r1, [r7, #8]
	ldrb	r0, [r5, #0]
	bl 0x02009458
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
	bl 0x02009528
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
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_02000ca8
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000ca8
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
.L_02000ca8:
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
	mov	r8, r1
	ldr	r7, [sp, #48]
	bl 0x02009498
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_02000d2c
	cmp	r7, #0
	beq.n	.L_02000d2c
	movs	r2, #24
.L_02000d24:
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_02000d34
.L_02000d2c:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_02000d34:
	mov	r3, sl
	bl 0x02009438
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_02000d42
	b.n	.L_02000e8e
.L_02000d42:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl 0x02009428
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
.L_02000d5a:
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x02009430
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02009468
	ldr	r3, [pc, #300]
	mov	r1, r9
	str	r3, [r6, #108]
	mov	r3, fp
.L_02000d7e:
	str	r3, [r6, #68]
	ldr	r3, [sp, #36]
	adds	r0, r6, #0
	str	r3, [r6, #72]
	ldr	r3, [sp, #40]
	str	r3, [r6, #76]
	ldr	r3, [r1, #80]
	ldrb	r1, [r3, #9]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl 0x02008c64
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
	beq.n	.L_02000e8e
	cmp	r7, #0
	beq.n	.L_02000e8e
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000dc4
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x020094b8
.L_02000dc4:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000de4
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl 0x02008c64
.L_02000de4:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_02000df8
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_02000df8:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000e3e
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_02000e26
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x020093f0
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_02000e38
.L_02000e26:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x020093f0
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_02000e38:
	bl 0x020093f0
	str	r0, [r6, #52]
.L_02000e3e:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000e5a
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x02009428
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
.L_02000e56:
	bl 0x02009430
.L_02000e5a:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000e6c
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_02000e6c:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
.L_02000e72:
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000e7e
	ldrh	r3, [r7, #34]
.L_02000e7a:
	mov	r1, r9
	strh	r3, [r1, #0]
.L_02000e7e:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000e8e
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_02000e8e:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x02009668
	.4byte 0x02008cad
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	cmp	r4, #0
	bne.n	.L_02000ebc
	movs	r0, #0
	b.n	.L_02000ee2
.L_02000ebc:
	cmp	r0, #2
	bhi.n	.L_02000ed0
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r0, #156
	lsls	r3, r3, #3
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r4, [r4, r3]
	b.n	.L_02000ed2
.L_02000ed0:
	ldr	r4, [pc, #16]
.L_02000ed2:
	lsls	r3, r2, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r4, r4, r3
	ldrb	r0, [r4, #2]
	ldrb	r3, [r4, #3]
.L_02000ede:
	lsls	r0, r0, #8
	orrs	r0, r3
.L_02000ee2:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0201
	push	{r5, lr}
	adds	r5, r1, #0
	adds	r1, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	cmp	r4, #0
	bne.n	.L_02000efc
	movs	r0, #0
	b.n	.L_02000f28
.L_02000efc:
	cmp	r0, #2
	bhi.n	.L_02000f10
	lsls	r3, r0, #3
	subs	r3, r3, r0
.L_02000f04:
	movs	r0, #156
	lsls	r3, r3, #3
	lsls	r0, r0, #1
	adds	r3, r3, r0
.L_02000f0c:
	ldr	r4, [r4, r3]
	b.n	.L_02000f12
.L_02000f10:
	ldr	r4, [pc, #24]
.L_02000f12:
	lsls	r3, r2, #7
	adds	r3, r5, r3
	lsls	r3, r3, #2
	adds	r4, r4, r3
	ldrb	r0, [r4, #2]
	ldrb	r3, [r4, #3]
	lsls	r0, r0, #8
	orrs	r0, r3
	asrs	r3, r1, #8
	strb	r3, [r4, #2]
	strb	r1, [r4, #3]
.L_02000f28:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0201
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	movs	r0, #0
	asrs	r3, r3, #20
	mov	r9, r3
	ldr	r3, [r6, #16]
	mov	r1, r9
	asrs	r3, r3, #20
	mov	sl, r3
	mov	r2, sl
	bl 0x02008eac
	mov	r1, r9
	mov	r2, sl
	mov	r8, r0
	movs	r0, #2
	bl 0x02008eac
	movs	r2, #34
	adds	r2, r2, r6
	adds	r5, r0, #0
	mov	fp, r2
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	bl 0x02009458
	adds	r3, r6, #0
	adds	r3, #100
	asrs	r7, r0, #19
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x02009418
	cmp	r0, #0
	beq.n	.L_02000faa
	movs	r3, #255
	lsls	r3, r3, #8
	orrs	r5, r3
	movs	r3, #129
	negs	r3, r3
	mov	r2, r8
	ands	r2, r3
	ldr	r3, [r6, #20]
	mov	r8, r2
	asrs	r3, r3, #19
	cmp	r3, r7
	beq.n	.L_02000fa0
	subs	r7, #4
.L_02000fa0:
	adds	r0, r6, #0
	movs	r1, #3
	bl 0x02008c64
	b.n	.L_02000fd4
.L_02000faa:
	mov	r2, r8
	asrs	r3, r2, #8
	cmp	r3, #232
	beq.n	.L_02000fb8
	movs	r3, #255
	ands	r5, r3
	b.n	.L_02000fbe
.L_02000fb8:
	movs	r3, #255
	ands	r5, r3
	movs	r3, #232
.L_02000fbe:
	lsls	r3, r3, #8
	orrs	r5, r3
	mov	r2, r8
	movs	r3, #128
	orrs	r2, r3
	adds	r0, r6, #0
	movs	r1, #2
	mov	r8, r2
	adds	r7, #4
	bl 0x02008c64
.L_02000fd4:
	mov	r1, r9
	mov	r2, sl
	mov	r3, r8
	movs	r0, #0
	bl 0x02008ee8
	mov	r1, r9
	mov	r2, sl
	adds	r3, r5, #0
	movs	r0, #2
.L_02000fe8:
	bl 0x02008ee8
	mov	r3, r9
	mov	r2, sl
	lsls	r0, r3, #20
	mov	r3, fp
	lsls	r1, r2, #20
	ldrb	r2, [r3, #0]
	adds	r3, r7, #0
	bl 0x02009480
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	b.n	.L_02001058
.L_02001012:
	ldrh	r0, [r7, #0]
	bl 0x02009498
	movs	r3, #4
	ldrsh	r6, [r7, r3]
	movs	r1, #0
	adds	r5, r0, #0
	bl 0x02009468
	lsls	r0, r6, #16
	lsrs	r0, r0, #16
	bl 0x02009418
	adds	r1, r0, #0
	adds	r1, #1
	adds	r0, r5, #0
	bl 0x02009428
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r5, #0
	adds	r1, #89
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r5, #0
	adds	r3, #100
	strh	r6, [r3, #0]
.L_02001050:
	adds	r0, r5, #0
	adds	r7, #6
	bl 0x02008f30
.L_02001058:
	movs	r2, #255
	ldrh	r3, [r7, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02001012
.L_02001064:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x96a0
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_02001088
	ldr	r0, [pc, #20]
	b.n	.L_02001092
.L_02001088:
	ldr	r3, [pc, #20]
	movs	r0, #0
	cmp	r2, r3
	bne.n	.L_02001092
	ldr	r0, [pc, #16]
.L_02001092:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000ac
	.4byte 0x020096d0
	.4byte 0x000000ad
	.2byte 0x96f0
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9710
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
.L_020010c0:
	cmp	r2, r3
	bne.n	.L_020010c8
	ldr	r0, [pc, #24]
	b.n	.L_020010d4
.L_020010c8:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_020010d2
	ldr	r0, [pc, #24]
	b.n	.L_020010d4
.L_020010d2:
	ldr	r0, [pc, #24]
.L_020010d4:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000ac
	.4byte 0x0200976c
	.4byte 0x000000ad
	.4byte 0x02009814
	.2byte 0x9754
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r6, [r3, #0]
	ldr	r3, [pc, #60]
	adds	r4, r1, #0
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #48]
	adds	r5, r0, #0
	cmp	r2, r3
	bne.n	.L_0200111c
	ldr	r0, [pc, #44]
	adds	r1, r5, #0
	adds	r2, r4, #0
	bl 0x020080a4
	b.n	.L_02001126
.L_0200111c:
	ldr	r0, [pc, #36]
	adds	r1, r5, #0
	adds	r2, r4, #0
	bl 0x020080a4
.L_02001126:
	cmp	r5, #1
	bne.n	.L_02001136
	movs	r2, #26
	ldrsh	r0, [r6, r2]
	bl 0x02009498
	bl 0x02008f30
.L_02001136:
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x000000ac
	.4byte 0x02009674
	.2byte 0x9688
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	bl 0x02009488
	movs	r0, #0
	bl 0x020094e8
	ldr	r0, [pc, #36]
	bl 0x02009500
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #188
	adds	r5, r5, r3
	ldr	r1, [r5, #0]
	movs	r3, #1
	adds	r1, #35
	ldrb	r2, [r1, #0]
	orrs	r3, r2
	movs	r2, #253
	ands	r3, r2
	strb	r3, [r1, #0]
	bl 0x02009490
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x9696
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #44]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x02009498
	adds	r5, r0, #0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	movs	r0, #2
	bl 0x02008eac
	asrs	r0, r0, #8
	cmp	r0, #232
	bne.n	.L_020011b2
	adds	r2, r5, #0
	adds	r2, #34
	movs	r3, #2
	strb	r3, [r2, #0]
.L_020011b2:
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
.L_020011ba:
	ldr	r3, [pc, #20]
.L_020011bc:
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x02009498
.L_020011c8:
	movs	r3, #0
	adds	r0, #34
	strb	r3, [r0, #0]
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #64]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
.L_020011e0:
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_020011f0
	ldr	r0, [pc, #52]
	bl 0x02009518
	b.n	.L_02001214
.L_020011f0:
	movs	r1, #50
	movs	r2, #13
	movs	r0, #2
	bl 0x02008eac
	movs	r3, #255
	movs	r2, #255
	ands	r3, r0
	lsls	r2, r2, #8
	orrs	r3, r2
	movs	r0, #2
	movs	r1, #50
	movs	r2, #13
	bl 0x02008ee8
	ldr	r0, [pc, #20]
	bl 0x02009518
.L_02001214:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000ac
	.4byte 0x02009654
	.2byte 0x9658
	.2byte 0x0200
	push	{lr}
	bl 0x02009488
	movs	r0, #0
	bl 0x020094e8
	bl 0x02009520
	ldr	r3, [pc, #44]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_02001262
	movs	r1, #50
	movs	r2, #13
	movs	r0, #2
	bl 0x02008eac
	movs	r3, #255
	ands	r3, r0
	movs	r1, #50
	movs	r0, #2
	movs	r2, #13
	bl 0x02008ee8
.L_02001262:
	bl 0x02009490
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x00ad
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	bl 0x02009488
	movs	r2, #12
	str	r2, [sp, #4]
	movs	r3, #7
	movs	r0, #7
	movs	r1, #2
	movs	r2, #8
	str	r3, [sp, #0]
	bl 0x02009460
	movs	r3, #8
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #66
	movs	r2, #7
	movs	r3, #76
	movs	r0, #7
	bl 0x02009450
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #65
	bl 0x02009420
	bl 0x02009490
	add	sp, #8
	pop	{pc}
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_020012c8
	ldr	r0, [pc, #12]
	b.n	.L_020012ca
.L_020012c8:
	ldr	r0, [pc, #12]
.L_020012ca:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000ac
	.4byte 0x020098ec
	.2byte 0x99ac
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	ldr	r5, [pc, #228]
	movs	r2, #129
	adds	r3, r3, r1
	lsls	r2, r2, #2
	str	r2, [r3, #0]
	adds	r2, #16
	adds	r7, r5, r2
	ldr	r0, [r7, #0]
	sub	sp, #8
	bl 0x02009498
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r6, #32
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #0
	bl 0x020094e0
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r5, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #184]
	cmp	r2, r3
	bne.n	.L_02001332
	ldr	r0, [pc, #184]
	bl 0x02009510
	ldr	r0, [pc, #180]
	bl 0x0200900c
	ldr	r0, [pc, #180]
	bl 0x020094f8
	b.n	.L_020013ca
.L_02001332:
	movs	r0, #10
	bl 0x02009498
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #11
	bl 0x02009498
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r6
	strb	r3, [r0, #0]
	ldr	r0, [pc, #148]
	bl 0x02009510
	ldr	r0, [pc, #144]
	bl 0x0200900c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #65
	bl 0x02009418
	cmp	r0, #0
	beq.n	.L_020013ca
.L_02001368:
	movs	r2, #12
	str	r2, [sp, #4]
	movs	r3, #7
	movs	r0, #7
	movs	r1, #2
	movs	r2, #8
	str	r3, [sp, #0]
	bl 0x02009460
	movs	r3, #8
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #7
	movs	r1, #66
	movs	r2, #7
	movs	r3, #76
	bl 0x02009450
	bl 0x02009448
	movs	r0, #1
	bl 0x020093f8
	movs	r0, #10
	adds	r0, #255
	bl 0x02009418
	cmp	r0, #0
	beq.n	.L_020013ca
	ldr	r0, [r7, #0]
	bl 0x02009498
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #12]
	ldr	r1, [r5, #8]
	subs	r2, r2, r3
	bl 0x02009458
	str	r0, [r5, #20]
	str	r0, [r5, #12]
	ldr	r0, [r7, #0]
	movs	r1, #0
	bl 0x020094d0
.L_020013ca:
	movs	r0, #0
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x000000ac
	.4byte 0x02009654
	.4byte 0x02009674
	.4byte 0x02009696
	.4byte 0x02009658
	.2byte 0x9688
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	.irp EntryTarget, 0x03000528, 0x080000c1, 0x080000f9, 0x08000119, 0x08000121, 0x080003c9, 0x080003d1, 0x08020091, 0x080200a9, 0x080200c1, 0x080200e9, 0x08020121, 0x08020179, 0x080201c1, 0x080201e9, 0x08020219, 0x08020229, 0x080202f1, 0x08020361, 0x080c8019, 0x080c8021, 0x080c8089, 0x080c80f9, 0x080c8111, 0x080c8119, 0x080c8171, 0x080c8209, 0x080c8221, 0x080c8229, 0x080c82e1, 0x080c8481, 0x080c84e1, 0x080c8519, 0x080c86f9, 0x080c8709, 0x080c8711, 0x080c8719, 0x080c8721, 0x080c8729, 0x080c8731, 0x080c8739, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
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
	.4byte 0xffff000d
	.4byte 0x000b000a
	.4byte 0x000d000c
	.4byte 0x000f000e
	.4byte 0x0000ffff
	.4byte 0x020095a0
	.4byte 0x020095dc
	.4byte 0x02009618
	.4byte 0x00000008
	.4byte 0x00090200
	.4byte 0x02010000
	.4byte 0x0000000c
	.4byte 0xffff0202
	.4byte 0x00000008
	.4byte 0x00090200
	.4byte 0x02010000
	.4byte 0x000affff
	.4byte 0x000b0203
	.4byte 0xffff0204
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x002c00d0
	.4byte 0x00e002f0
	.4byte 0x0300003c
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00100190
	.4byte 0x01a00160
	.4byte 0x01700020
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000ac
	.4byte 0x1010a0ab
	.4byte 0xffffffff
	.4byte 0x102030ac
	.4byte 0xffffffff
	.4byte 0x103020ac
	.4byte 0xffffffff
	.4byte 0x104010ad
	.4byte 0xffffffff
	.4byte 0x000000ad
	.4byte 0x101040ac
	.4byte 0xffffffff
	.4byte 0x102030ad
	.4byte 0xffffffff
	.4byte 0x103020ad
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x0002c000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x0002c000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x0002c000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x0002c000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x0002c000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x50008615
	.4byte 0x02000008
	.4byte 0x020090f1
	.4byte 0x50008615
	.4byte 0x02010009
	.4byte 0x020090f1
	.4byte 0x50008615
	.4byte 0x0202000c
	.4byte 0x020090f1
	.4byte 0x00001815
	.4byte 0x0203000a
	.4byte 0x02009149
	.4byte 0x00001815
	.4byte 0x0204000b
	.4byte 0x02009149
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x02009185
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x020091b9
	.4byte 0x10008c15
	.4byte 0xffff000d
	.4byte 0x020091d5
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte 0x02009229
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x020091d5
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02009229
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x50008615
	.4byte 0x02000008
	.4byte 0x020090f1
	.4byte 0x50008615
	.4byte 0x02010009
	.4byte 0x020090f1
	.4byte 0x50008a05
	.4byte 0xffff0032
	.4byte 0x02009271
	.4byte 0x10008c15
	.4byte 0xffff000a
	.4byte 0x020091d5
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x02009229
	.4byte 0x10008c15
	.4byte 0xffff000b
	.4byte 0x020091d5
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x02009229
	.4byte 0x10008c15
	.4byte 0xffff000c
	.4byte 0x020091d5
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x02009229
	.4byte 0x10008c15
	.4byte 0xffff000d
	.4byte 0x020091d5
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte 0x02009229
	.4byte 0x10008c15
	.4byte 0xffff000e
	.4byte 0x020091d5
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte 0x02009229
	.4byte 0x10008c15
	.4byte 0xffff000f
	.4byte 0x020091d5
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte 0x02009229
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
