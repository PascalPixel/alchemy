.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008d55, 0x02008319, 0x02008381, 0x02008389, 0x020083ed, 0x02008359, 0x02008dc9
	overlay_veneer \EntryTarget
	.endr
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
	beq.n	.L_02000108
.L_0200005e:
	mov	r3, fp
	ldrh	r0, [r3, #0]
	bl 0x0200911c
	mov	r8, r0
	mov	r7, r8
	adds	r7, #34
	mov	r2, r8
	ldr	r1, [r2, #8]
	ldrb	r0, [r7, #0]
	ldr	r2, [r2, #16]
	bl 0x020090ec
	str	r0, [sp, #0]
	mov	r3, r8
	ldr	r1, [r3, #8]
	ldr	r2, [r3, #16]
	ldrb	r0, [r7, #0]
	bl 0x020090d4
	mov	sl, r0
	mov	r2, sl
	asrs	r2, r2, #19
	mov	r0, r8
	mov	sl, r2
	bl 0x020091d4
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
	bl 0x020090e4
	movs	r3, #4
	add	sl, r3
	mov	r2, sl
	ldrb	r0, [r7, #0]
	ldr	r1, [sp, #0]
	bl 0x020091f4
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
	strb	r3, [r6, #3]
	ldrh	r3, [r2, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	strb	r0, [r5, #0]
	cmp	r3, r2
	bne.n	.L_0200005e
.L_02000108:
	ldr	r3, [pc, #60]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200911c
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	bl 0x020090d4
	ldr	r3, [r5, #12]
	cmp	r3, r0
	bge.n	.L_02000130
	str	r0, [r5, #20]
	str	r0, [r5, #12]
.L_02000130:
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
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	str	r0, [sp, #4]
	bl 0x0200920c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r7, r0, #0
	str	r3, [sp, #0]
	cmp	r7, #0
	bne.n	.L_0200018e
	ldr	r3, [pc, #240]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200918c
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L_0200018e
	bl 0x0200911c
	adds	r7, r0, #0
.L_0200018e:
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
	beq.n	.L_02000254
.L_020001aa:
	ldr	r3, [sp, #4]
	ldrh	r0, [r3, #0]
	bl 0x0200911c
	cmp	r7, r0
	bne.n	.L_02000242
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
	bl 0x020090ec
	mov	r1, fp
	ldr	r2, [r7, #16]
	mov	r9, r0
	ldrb	r0, [r1, #0]
	ldr	r1, [r7, #8]
	bl 0x020090d4
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl 0x020091d4
	asrs	r5, r5, #19
	mov	r2, fp
	subs	r5, #4
	adds	r6, r0, #0
	mov	r1, r9
	ldrb	r0, [r2, #0]
	adds	r2, r5, #0
	bl 0x020091f4
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
.L_02000242:
	ldr	r2, [sp, #4]
	movs	r1, #255
	adds	r2, #2
	str	r2, [sp, #4]
	lsls	r1, r1, #8
	ldrh	r3, [r2, #0]
	adds	r1, #255
	cmp	r3, r1
	bne.n	.L_020001aa
.L_02000254:
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
	beq.n	.L_02000306
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
	ldr	r2, [r6, #16]
	add	r8, r3
	bl 0x020090ec
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	mov	r9, r0
	ldrb	r0, [r7, #0]
	bl 0x020090d4
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl 0x020091d4
	asrs	r5, r5, #19
	adds	r5, #4
	adds	r6, r0, #0
	mov	r1, r9
	adds	r2, r5, #0
	ldrb	r0, [r7, #0]
	bl 0x020091f4
	add	r8, r6
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
.L_02000306:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x02024000
	.2byte 0x0000
	.2byte 0xfdff
	.2byte 0xb500
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_02000330
	ldr	r0, [pc, #24]
	b.n	.L_0200033c
.L_02000330:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_0200033a
	ldr	r0, [pc, #24]
	b.n	.L_0200033c
.L_0200033a:
	ldr	r0, [pc, #24]
.L_0200033c:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000032
	.4byte 0x02009400
	.4byte 0x00000033
	.4byte 0x02009430
	.2byte 0x93d0
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	movs	r0, #0
	cmp	r2, r3
	bne.n	.L_02000370
	ldr	r0, [pc, #12]
.L_02000370:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000030
	.2byte 0x94a8
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x94d8
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #56]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_020003a0
	ldr	r0, [pc, #44]
	b.n	.L_020003c0
.L_020003a0:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_020003aa
	ldr	r0, [pc, #44]
	b.n	.L_020003c0
.L_020003aa:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_020003b4
	ldr	r0, [pc, #40]
	b.n	.L_020003c0
.L_020003b4:
	ldr	r3, [pc, #40]
	cmp	r2, r3
	bne.n	.L_020003be
	ldr	r0, [pc, #40]
	b.n	.L_020003c0
.L_020003be:
	ldr	r0, [pc, #40]
.L_020003c0:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000002f
	.4byte 0x020095b4
	.4byte 0x00000031
	.4byte 0x0200965c
	.4byte 0x00000033
	.4byte 0x020096bc
	.4byte 0x00000034
	.4byte 0x020096ec
	.2byte 0x959c
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #76]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_02000404
	ldr	r0, [pc, #64]
	b.n	.L_02000438
.L_02000404:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_0200040e
	ldr	r0, [pc, #64]
	b.n	.L_02000438
.L_0200040e:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_02000418
	ldr	r0, [pc, #60]
	b.n	.L_02000438
.L_02000418:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_02000422
	ldr	r0, [pc, #60]
	b.n	.L_02000438
.L_02000422:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_0200042c
	ldr	r0, [pc, #56]
	b.n	.L_02000438
.L_0200042c:
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_02000436
	ldr	r0, [pc, #56]
	b.n	.L_02000438
.L_02000436:
	ldr	r0, [pc, #56]
.L_02000438:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000002f
	.4byte 0x02009920
	.4byte 0x00000030
	.4byte 0x020099f8
	.4byte 0x00000031
	.4byte 0x02009a1c
	.4byte 0x00000032
	.4byte 0x02009aac
	.4byte 0x00000033
	.4byte 0x02009b54
	.4byte 0x00000034
	.4byte 0x02009bcc
	.2byte 0x9914
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	bl 0x0200910c
	movs	r0, #0
	bl 0x020091ac
	ldr	r0, [pc, #24]
	movs	r1, #1
	bl 0x020090f4
	movs	r3, #173
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	bl 0x02009114
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x191d
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	adds	r7, r6, #0
	adds	r7, #85
	movs	r3, #3
	strb	r3, [r7, #0]
.L_020004b0:
	movs	r0, #1
	bl 0x02009094
	ldr	r5, [r6, #40]
	cmp	r5, #0
	bne.n	.L_020004b0
	movs	r0, #188
	bl 0x02009214
	movs	r0, #10
	bl 0x02009094
	strb	r5, [r7, #0]
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	movs	r0, #11
	sub	sp, #8
	bl 0x0200911c
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #40
	bne.n	.L_02000518
	ldr	r3, [r5, #16]
	asrs	r6, r3, #20
	cmp	r6, #13
	bne.n	.L_02000518
	bl 0x0200910c
	movs	r0, #0
	bl 0x020091ac
	adds	r0, r5, #0
	bl 0x020084a4
	movs	r3, #39
	str	r3, [sp, #0]
	movs	r0, #41
	movs	r1, #13
	movs	r2, #2
	movs	r3, #1
	str	r6, [sp, #4]
	bl 0x020090dc
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #200
	bl 0x020090a4
	bl 0x02009114
.L_02000518:
	add	sp, #8
	pop	{r5, r6, pc}
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x020091e4
	pop	{pc}
	.2byte 0x0000
	.2byte 0x93c0
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #12
	bl 0x0200911c
	adds	r5, r0, #0
	bl 0x020091ec
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #44
	bne.n	.L_02000554
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #26
	bne.n	.L_02000554
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #202
	bl 0x020090a4
.L_02000554:
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x020091cc
	pop	{pc}
	.2byte 0x0000
	.2byte 0x93c4
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r0, #8
	bl 0x0200911c
	adds	r7, r0, #0
	movs	r0, #9
	bl 0x0200911c
	adds	r6, r0, #0
	bl 0x0200910c
	movs	r0, #0
	bl 0x020091ac
	movs	r0, #183
	bl 0x02009214
	movs	r5, #0
.L_0200058c:
	ldr	r3, [r7, #8]
	ldr	r2, [pc, #396]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r7, #8]
	movs	r2, #200
	ldr	r3, [r6, #8]
	lsls	r2, r2, #5
	adds	r2, #153
	adds	r3, r3, r2
	str	r3, [r6, #8]
	adds	r5, #1
	bl 0x02009094
	cmp	r5, #119
	bls.n	.L_0200058c
	movs	r1, #204
	lsls	r1, r1, #7
	ldr	r0, [pc, #364]
	adds	r1, #102
	bl 0x02009164
	movs	r0, #160
	movs	r1, #1
	movs	r2, #240
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	bl 0x0200916c
	bl 0x02009174
	movs	r0, #204
	movs	r1, #192
	lsls	r0, r0, #7
	lsls	r1, r1, #4
	adds	r0, #102
	adds	r1, #204
	bl 0x02009164
	movs	r0, #160
	movs	r1, #1
	movs	r2, #186
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #16
	lsls	r0, r0, #16
	bl 0x0200916c
	movs	r0, #168
	bl 0x02009214
	movs	r1, #136
	movs	r2, #129
	movs	r0, #14
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x02009134
	movs	r1, #184
	movs	r2, #129
	lsls	r2, r2, #17
	movs	r0, #19
	lsls	r1, r1, #16
	bl 0x02009134
	ldr	r5, [pc, #272]
	movs	r0, #14
	adds	r1, r5, #0
	bl 0x0200912c
	adds	r1, r5, #0
	movs	r0, #19
	bl 0x0200912c
	movs	r0, #40
	bl 0x02009104
	movs	r0, #168
	bl 0x02009214
	movs	r1, #240
	movs	r2, #226
	movs	r0, #13
	lsls	r1, r1, #15
	lsls	r2, r2, #16
	bl 0x02009134
	movs	r1, #200
	movs	r2, #226
	lsls	r2, r2, #16
	movs	r0, #18
	lsls	r1, r1, #16
	bl 0x02009134
	adds	r1, r5, #0
	movs	r0, #13
	bl 0x0200912c
	adds	r1, r5, #0
	movs	r0, #18
	bl 0x0200912c
	movs	r0, #40
	bl 0x02009104
	movs	r0, #168
	bl 0x02009214
	movs	r1, #240
	movs	r2, #194
	movs	r0, #12
	lsls	r1, r1, #15
	lsls	r2, r2, #16
	bl 0x02009134
	movs	r1, #200
	movs	r2, #194
	lsls	r2, r2, #16
	movs	r0, #17
	lsls	r1, r1, #16
	bl 0x02009134
	adds	r1, r5, #0
	movs	r0, #12
	bl 0x0200912c
	adds	r1, r5, #0
	movs	r0, #17
	bl 0x0200912c
	movs	r0, #60
	bl 0x02009104
	movs	r0, #229
	bl 0x02009214
	movs	r1, #136
	movs	r2, #162
	movs	r0, #11
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x02009134
	movs	r1, #184
	movs	r2, #162
	movs	r0, #16
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x02009134
	movs	r1, #136
	movs	r2, #138
	movs	r0, #10
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x02009134
	movs	r1, #184
	movs	r2, #138
	lsls	r2, r2, #16
	movs	r0, #15
	lsls	r1, r1, #16
	bl 0x02009134
	ldr	r5, [pc, #76]
	movs	r0, #11
	adds	r1, r5, #0
	bl 0x0200912c
	adds	r1, r5, #0
	movs	r0, #16
	bl 0x0200912c
	adds	r1, r5, #0
	movs	r0, #10
	bl 0x0200912c
	adds	r1, r5, #0
	movs	r0, #15
	bl 0x0200912c
	movs	r0, #80
	bl 0x02009104
	bl 0x02008958
	movs	r0, #204
	adds	r0, #255
	bl 0x020090fc
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #201
	bl 0x020090a4
	bl 0x02009114
	pop	{r5, r6, r7, pc}
	.4byte 0xffffe667
	.4byte 0x00033333
	.4byte 0x02009264
	.2byte 0x92f4
	.2byte 0x0200
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #203
	sub	sp, #8
	bl 0x0200909c
	cmp	r0, #0
	beq.n	.L_02000754
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #72
	movs	r1, #63
	movs	r2, #36
	movs	r3, #38
	bl 0x020090cc
	b.n	.L_02000768
.L_02000754:
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #71
	movs	r1, #63
	movs	r2, #36
	movs	r3, #38
	bl 0x020090cc
.L_02000768:
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x020090a4
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #63
	movs	r2, #36
	movs	r3, #38
	movs	r0, #70
	bl 0x020090cc
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x020090ac
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	sub	sp, #8
	bl 0x0200909c
	cmp	r0, #0
	beq.n	.L_02000856
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #203
	bl 0x0200909c
	cmp	r0, #0
	bne.n	.L_02000856
	bl 0x0200910c
	movs	r0, #1
	bl 0x020091ac
	movs	r0, #161
	bl 0x02009214
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r5, #2
	movs	r1, #63
	movs	r2, #36
	movs	r3, #38
	movs	r0, #72
	str	r5, [sp, #4]
	bl 0x020090cc
	movs	r0, #40
	bl 0x02009104
	movs	r0, #188
	bl 0x02009214
	movs	r1, #66
	movs	r2, #97
	movs	r3, #39
	movs	r0, #71
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x020090cc
	movs	r0, #2
	bl 0x02009094
	movs	r1, #66
	movs	r2, #97
	movs	r3, #39
	movs	r0, #73
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x020090cc
	movs	r0, #2
	bl 0x02009094
	movs	r0, #75
	movs	r1, #66
	movs	r2, #97
	movs	r3, #39
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x020090cc
	movs	r3, #33
	movs	r2, #39
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #37
	movs	r2, #2
	movs	r3, #2
	movs	r0, #33
	bl 0x020090dc
	movs	r0, #1
	bl 0x02009094
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #203
	bl 0x020090a4
	bl 0x02009114
	b.n	.L_0200086c
.L_02000856:
	bl 0x0200910c
	movs	r0, #0
	bl 0x020091ac
	ldr	r0, [pc, #12]
	movs	r1, #1
	bl 0x020090f4
	bl 0x02009114
.L_0200086c:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x191c
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #204
	bl 0x0200909c
	cmp	r0, #0
	bne.n	.L_020008cc
	movs	r0, #64
	bl 0x0200911c
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	movs	r2, #186
	lsls	r2, r2, #16
	cmp	r3, r2
	bge.n	.L_020008cc
	bl 0x0200910c
	movs	r0, #0
	bl 0x020091ac
	movs	r0, #204
	bl 0x02009214
	adds	r2, r5, #0
	movs	r3, #3
	adds	r2, #85
	strb	r3, [r2, #0]
	movs	r0, #10
	bl 0x02009094
	movs	r0, #64
	movs	r1, #0
	movs	r2, #0
	bl 0x02009134
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #204
	bl 0x020090a4
	bl 0x02009114
.L_020008cc:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x02008270
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #35
	bne.n	.L_020008f2
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #17
	bne.n	.L_020008f2
	movs	r0, #143
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x020090a4
.L_020008f2:
	pop	{r5, pc}
	push	{r5, lr}
	bl 0x020091fc
	cmp	r0, #0
	beq.n	.L_02000922
	ldr	r3, [pc, #36]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x02009184
	bl 0x0200911c
	adds	r5, r0, #0
	ldr	r0, [pc, #20]
	bl 0x0200814c
	bl 0x02009204
	adds	r0, r5, #0
	bl 0x020088d0
.L_02000922:
	pop	{r5, pc}
	.4byte 0x02000240
	.2byte 0x93ca
	.2byte 0x0200
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x0200814c
	pop	{pc}
	.2byte 0x0000
	.2byte 0x93ca
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
	beq.n	.L_02000956
	bl 0x020088d0
.L_02000956:
	pop	{pc}
	push	{r5, r6, r7, lr}
	movs	r1, #136
	movs	r2, #129
	movs	r0, #14
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	sub	sp, #8
	bl 0x02009134
	movs	r1, #184
	movs	r2, #129
	movs	r0, #19
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x02009134
	movs	r1, #240
	movs	r2, #226
	movs	r0, #13
	lsls	r1, r1, #15
	lsls	r2, r2, #16
	bl 0x02009134
	movs	r1, #200
	movs	r2, #226
	movs	r0, #18
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x02009134
	movs	r1, #240
	movs	r2, #194
	movs	r0, #12
.L_0200099a:
	lsls	r1, r1, #15
	lsls	r2, r2, #16
	bl 0x02009134
	movs	r1, #200
	movs	r2, #194
	movs	r0, #17
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x02009134
	movs	r1, #136
	movs	r2, #162
	movs	r0, #11
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x02009134
	movs	r1, #184
	movs	r2, #162
	movs	r0, #16
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x02009134
	movs	r1, #136
	movs	r2, #138
	movs	r0, #10
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x02009134
	movs	r1, #184
	movs	r2, #138
	movs	r0, #15
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x02009134
	movs	r7, #10
	movs	r6, #20
	movs	r5, #0
.L_020009ee:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02009144
	adds	r5, #1
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl 0x0200913c
	adds	r7, #1
	adds	r6, #1
	cmp	r5, #9
	bls.n	.L_020009ee
	movs	r3, #9
	movs	r2, #22
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #0
	movs	r2, #2
	movs	r3, #2
	movs	r0, #0
	bl 0x020090dc
	movs	r0, #1
	bl 0x02009094
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r0, #8
	bl 0x0200911c
	adds	r5, r0, #0
	movs	r0, #64
	bl 0x0200911c
	ldr	r3, [r5, #80]
	adds	r6, r0, #0
	ldr	r7, [r6, #80]
	ldrh	r3, [r3, #18]
	movs	r0, #129
	strh	r3, [r7, #18]
	lsls	r0, r0, #2
	bl 0x0200909c
	cmp	r0, #0
	beq.n	.L_02000a74
	ldrh	r3, [r7, #18]
	cmp	r3, #0
	bne.n	.L_02000a7a
	movs	r0, #140
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009214
	ldr	r3, [r6, #8]
	ldr	r2, [pc, #32]
.L_02000a62:
	movs	r0, #129
	adds	r3, r3, r2
	str	r3, [r6, #8]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r6, #40]
	lsls	r0, r0, #2
	bl 0x020090ac
.L_02000a74:
	ldrh	r3, [r7, #18]
	cmp	r3, #0
	beq.n	.L_02000a82
.L_02000a7a:
	movs	r0, #129
	lsls	r0, r0, #2
.L_02000a7e:
	bl 0x020090a4
.L_02000a82:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xfffe
	.2byte 0xb5e0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #108]
	ldr	r3, [pc, #156]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	bl 0x0200911c
	adds	r5, r0, #0
	bl 0x0200910c
	movs	r0, #0
	bl 0x020091ac
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r2, r2
	negs	r0, r0
	negs	r1, r1
	bl 0x0200916c
	ldr	r0, [r6, #0]
	movs	r1, #2
	bl 0x02009144
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r6, #0]
	lsls	r2, r2, #7
	lsls	r1, r1, #8
	bl 0x02009124
	ldr	r0, [r5, #8]
	asrs	r2, r0, #16
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_02000ade
	adds	r3, #15
.L_02000ade:
	asrs	r3, r3, #4
	lsls	r3, r3, #4
	subs	r3, r2, r3
	movs	r1, #8
	subs	r1, r1, r3
	lsls	r1, r1, #16
	ldr	r2, [r5, #12]
	adds	r1, r1, r0
	ldr	r3, [r5, #16]
	adds	r0, r5, #0
	bl 0x020090bc
	adds	r0, r5, #0
	bl 0x020090c4
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r5, #6]
	ldr	r1, [pc, #48]
	ldr	r0, [r6, #0]
	bl 0x0200912c
	movs	r0, #12
	bl 0x02009094
	movs	r0, #123
	bl 0x02009214
	bl 0x0200919c
	bl 0x020091a4
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200917c
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x921c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	sl, r3
	movs	r3, #192
.L_02000b46:
	lsls	r3, r3, #18
	sub	sp, #8
	ldr	r3, [r3, #108]
	ldr	r6, [sp, #36]
	adds	r5, r0, #0
	adds	r7, r1, #0
	mov	r8, r2
	mov	r9, r3
	bl 0x0200910c
	movs	r0, #0
	bl 0x020091ac
	cmp	r6, #2
	bne.n	.L_02000b6c
	movs	r0, #188
	bl 0x02009214
	b.n	.L_02000b72
.L_02000b6c:
	movs	r0, #158
	bl 0x02009214
.L_02000b72:
	ldr	r3, [sp, #40]
	adds	r0, r5, #0
	str	r3, [sp, #4]
	adds	r1, r7, #0
	mov	r2, r8
	mov	r3, sl
	str	r6, [sp, #0]
	bl 0x020090cc
	movs	r0, #20
	bl 0x02009104
	ldr	r3, [pc, #144]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x02009124
	cmp	r6, #1
	bne.n	.L_02000bdc
	ldr	r0, [r5, #0]
	bl 0x0200911c
	adds	r5, r0, #0
	ldr	r0, [r5, #8]
	asrs	r2, r0, #16
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_02000bb8
	adds	r3, #15
.L_02000bb8:
	asrs	r3, r3, #4
	lsls	r3, r3, #4
	subs	r3, r2, r3
	movs	r1, #8
	subs	r1, r1, r3
	lsls	r1, r1, #16
	adds	r1, r1, r0
	ldr	r3, [r5, #16]
	ldr	r2, [r5, #12]
	adds	r0, r5, #0
	bl 0x020090bc
	adds	r0, r5, #0
	bl 0x020090c4
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r5, #6]
.L_02000bdc:
	ldr	r3, [pc, #60]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	ldr	r1, [pc, #56]
	bl 0x0200912c
	movs	r0, #12
	bl 0x02009094
	movs	r0, #123
	bl 0x02009214
	bl 0x0200919c
	bl 0x020091a4
	movs	r3, #170
	lsls	r3, r3, #1
	add	r3, r9
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200917c
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x921c
	.2byte 0x0200
	push	{lr}
	sub	sp, #8
	movs	r3, #2
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #78
	movs	r1, #29
	movs	r2, #78
	movs	r3, #31
	bl 0x02008b38
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #109
	movs	r1, #1
	movs	r2, #109
	movs	r3, #3
	bl 0x02008b38
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #118
	movs	r1, #1
	movs	r2, #118
	movs	r3, #3
	bl 0x02008b38
	add	sp, #8
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #92
	movs	r1, #29
	movs	r2, #92
	movs	r3, #31
	bl 0x02008b38
	add	sp, #8
	pop	{pc}
	push	{r5, lr}
	bl 0x0200910c
	movs	r0, #0
	bl 0x020091ac
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	negs	r1, r1
	negs	r0, r0
	movs	r3, #0
	bl 0x0200916c
	ldr	r5, [pc, #60]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200911c
	movs	r1, #0
	bl 0x020090e4
	ldr	r0, [r5, #0]
	bl 0x0200911c
	movs	r1, #15
	bl 0x0200914c
	movs	r0, #1
	bl 0x02009094
	bl 0x02009194
	bl 0x020091a4
	movs	r0, #40
	bl 0x02009104
	movs	r0, #3
	bl 0x0200917c
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	bl 0x0200910c
	movs	r0, #0
	bl 0x020091ac
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	negs	r1, r1
	negs	r0, r0
	movs	r3, #0
	bl 0x0200916c
	ldr	r5, [pc, #60]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200911c
	movs	r1, #0
	bl 0x020090e4
	ldr	r0, [r5, #0]
	bl 0x0200911c
	movs	r1, #15
	bl 0x0200914c
	movs	r0, #1
	bl 0x02009094
	bl 0x02009194
	bl 0x020091a4
	movs	r0, #40
	bl 0x02009104
	movs	r0, #8
	bl 0x0200917c
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	movs	r2, #129
	adds	r3, r3, r1
	lsls	r2, r2, #2
	str	r2, [r3, #0]
	ldr	r3, [pc, #68]
	subs	r2, #36
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_02000d7e
	bl 0x02008dcc
	b.n	.L_02000dac
.L_02000d7e:
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_02000d8a
	bl 0x02008f2c
	b.n	.L_02000dac
.L_02000d8a:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_02000d96
	bl 0x02008fbc
	b.n	.L_02000dac
.L_02000d96:
	ldr	r3, [pc, #40]
	cmp	r2, r3
	bne.n	.L_02000da2
	bl 0x02008fd8
	b.n	.L_02000dac
.L_02000da2:
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_02000dac
	bl 0x0200902c
.L_02000dac:
	movs	r0, #0
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000002f
	.4byte 0x00000031
	.4byte 0x00000032
	.4byte 0x00000033
	.2byte 0x0034
	.2byte 0x0000
	movs	r0, #0
	bx	lr
	push	{lr}
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	sub	sp, #8
	bl 0x020090ac
	movs	r0, #64
	movs	r1, #3
	bl 0x02009154
	bl 0x020091b4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #1
	adds	r1, #255
	movs	r2, #9
	movs	r3, #10
	bl 0x020091bc
	movs	r0, #11
	bl 0x0200915c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #200
	bl 0x0200909c
	cmp	r0, #0
	beq.n	.L_02000e32
	movs	r3, #39
	movs	r2, #13
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #13
	movs	r2, #2
	movs	r3, #1
	movs	r0, #41
	bl 0x020090dc
	movs	r0, #11
	bl 0x0200911c
	movs	r1, #162
	movs	r3, #216
	lsls	r1, r1, #18
	movs	r2, #0
	lsls	r3, r3, #16
	bl 0x020090b4
.L_02000e32:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #202
	bl 0x0200909c
	cmp	r0, #0
	beq.n	.L_02000e4e
	movs	r1, #178
	movs	r2, #212
	movs	r0, #12
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x02009134
.L_02000e4e:
	ldr	r0, [pc, #204]
	bl 0x020091dc
	movs	r0, #143
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200909c
	cmp	r0, #0
	beq.n	.L_02000e70
	movs	r1, #142
	movs	r2, #140
	movs	r0, #8
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x02009134
.L_02000e70:
	movs	r0, #8
	bl 0x0200911c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	ldr	r0, [pc, #156]
	bl 0x02008038
	ldr	r0, [pc, #156]
	bl 0x020091c4
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #203
	bl 0x0200909c
	cmp	r0, #0
	beq.n	.L_02000ec6
	movs	r3, #2
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #75
	movs	r1, #66
	movs	r2, #97
	movs	r3, #39
	bl 0x020090cc
	movs	r3, #33
	movs	r2, #39
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #33
	movs	r1, #37
	movs	r2, #2
	movs	r3, #2
.L_02000ebc:
	bl 0x020090dc
	movs	r0, #1
	bl 0x02009094
.L_02000ec6:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x0200909c
	cmp	r0, #0
	beq.n	.L_02000f18
	movs	r3, #5
	movs	r2, #3
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #82
	movs	r1, #74
	movs	r2, #116
	movs	r3, #16
	bl 0x020090cc
	movs	r3, #52
	movs	r2, #18
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r2, #5
	movs	r0, #52
	movs	r1, #11
	bl 0x020090dc
	ldr	r3, [pc, #40]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L_02000f12
	bl 0x02008c94
	b.n	.L_02000f18
.L_02000f12:
	movs	r0, #10
	bl 0x02009094
.L_02000f18:
	add	sp, #8
	pop	{pc}
	.4byte 0x020093c0
	.4byte 0x020093ca
	.4byte 0x020093c4
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	sub	sp, #8
	bl 0x020091b4
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #2
	movs	r2, #8
	movs	r3, #9
	bl 0x020091bc
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x0200909c
	cmp	r0, #0
	beq.n	.L_02000fb2
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x02009134
	movs	r3, #5
	movs	r2, #3
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #68
	movs	r1, #70
	movs	r2, #113
	movs	r3, #11
	bl 0x020090cc
	movs	r3, #2
	movs	r2, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #50
	movs	r1, #74
	movs	r2, #50
	movs	r3, #75
	bl 0x020090cc
	movs	r3, #49
	movs	r2, #13
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r2, #5
	movs	r0, #49
	movs	r1, #9
	bl 0x020090dc
	ldr	r3, [pc, #32]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #8
	bne.n	.L_02000fac
	bl 0x02008cf4
	b.n	.L_02000fb2
.L_02000fac:
	movs	r0, #1
	bl 0x02009094
.L_02000fb2:
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #204
	bl 0x0200909c
	cmp	r0, #0
	bne.n	.L_02000fd6
	movs	r0, #65
	movs	r1, #0
	movs	r2, #0
	bl 0x02009134
.L_02000fd6:
	pop	{pc}
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #204
	bl 0x0200909c
	cmp	r0, #0
	beq.n	.L_02000ff4
	movs	r0, #64
	movs	r1, #0
	movs	r2, #0
	bl 0x02009134
	b.n	.L_02001024
.L_02000ff4:
	movs	r0, #64
	bl 0x0200911c
	adds	r2, r0, #0
	adds	r1, r2, #0
	adds	r1, #89
	movs	r3, #8
	strb	r3, [r1, #0]
	subs	r1, #4
	movs	r3, #2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r2, #12]
	str	r3, [r2, #20]
	ldr	r3, [pc, #20]
	movs	r0, #8
	str	r3, [r2, #108]
	bl 0x0200911c
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r0, #28]
.L_02001024:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x8a29
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #8
	bl 0x0200911c
	adds	r5, r0, #0
	movs	r0, #9
	bl 0x0200911c
	movs	r3, #160
	lsls	r3, r3, #9
	str	r3, [r5, #24]
	ldr	r3, [pc, #72]
	adds	r6, r0, #0
	adds	r2, r5, #0
	str	r3, [r6, #24]
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r0, #128
	adds	r2, r6, #0
	adds	r2, #85
	lsls	r0, r0, #4
	strb	r3, [r2, #0]
.L_0200105a:
	adds	r0, #201
	str	r3, [r5, #12]
	str	r3, [r6, #12]
	bl 0x0200909c
	cmp	r0, #0
	beq.n	.L_0200108a
	movs	r0, #10
	adds	r0, #255
	bl 0x0200909c
	cmp	r0, #0
	bne.n	.L_02001086
	ldr	r3, [r5, #8]
	ldr	r2, [pc, #24]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	movs	r2, #192
	ldr	r3, [r6, #8]
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r6, #8]
.L_02001086:
	bl 0x02008958
.L_0200108a:
	pop	{r5, r6, pc}
	.4byte 0xfffec000
	.4byte 0xfff40000
	.irp EntryTarget, 0x080000c1, 0x080003c9, 0x080003d1, 0x080003d9, 0x080200e9, 0x08020149, 0x08020151, 0x08020179, 0x080201c1, 0x080201e9, 0x08020219, 0x080202f1, 0x08038041, 0x080ad041, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80f9, 0x080c8111, 0x080c8119, 0x080c8171, 0x080c8201, 0x080c8209, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8279, 0x080c82c1, 0x080c82d9, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c86a9, 0x080c86e9, 0x080c86f9, 0x080c8709, 0x080c8711, 0x080c8719, 0x080c8721, 0x080c8729, 0x080c8731, 0x080c8741, 0x080c8749, 0x080c8751, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000007
	.4byte 0xffff8000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000018
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00004000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0xc0010000
	.4byte 0x80020000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0xc0020000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00008000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0xc0010000
	.4byte 0x80020000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0xc0020000
	.4byte 0x80030000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffc000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0xc0030000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000011
	.4byte 0xffff000c
	.4byte 0x0202000d
	.4byte 0x0008ffff
	.4byte 0x0000ffff
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
	.4byte 0xffff0003
	.4byte 0x00000200
	.4byte 0xc00000e0
	.4byte 0x01880000
	.4byte 0x02780028
	.4byte 0x00000118
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0005
	.4byte 0x000002d8
	.4byte 0x40000058
	.4byte 0x02a80000
	.4byte 0x03980020
	.4byte 0x000000c0
	.4byte 0xffff0006
	.4byte 0x00000368
	.4byte 0x40000058
	.4byte 0x02a80000
	.4byte 0x03980020
	.4byte 0x000000c0
	.4byte 0xffff0007
	.4byte 0x00000368
	.4byte 0x00000078
	.4byte 0x02a80000
	.4byte 0x03980020
	.4byte 0x000000c0
	.4byte 0xffff0008
	.4byte 0x00000318
	.4byte 0xc0000098
	.4byte 0x02a80000
	.4byte 0x03980020
	.4byte 0x000000c0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00300070
	.4byte 0x00800090
	.4byte 0x00a00040
	.4byte 0x0001ffff
	.4byte 0x00300150
	.4byte 0x01600090
	.4byte 0x00a00040
	.4byte 0x0003ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000002f
	.4byte 0x00108033
	.4byte 0x00207032
	.4byte 0x00301031
	.4byte 0x0040d032
	.4byte 0x00506031
	.4byte 0x00000030
	.4byte 0x0010f02c
	.4byte 0x00207031
	.4byte 0x0031002c
	.4byte 0x00404031
	.4byte 0x00000031
	.4byte 0x0010302f
	.4byte 0x00203031
	.4byte 0x00302031
	.4byte 0x00404030
	.4byte 0x00508031
	.4byte 0x0060502f
	.4byte 0x00702030
	.4byte 0x00805031
	.4byte 0x00000032
	.4byte 0x00101033
	.4byte 0x0020b032
	.4byte 0x00309032
	.4byte 0x00402033
	.4byte 0x00504034
	.4byte 0x00601034
	.4byte 0x0070202f
	.4byte 0x0080c032
	.4byte 0x00903032
	.4byte 0x00a07033
	.4byte 0x00b02032
	.4byte 0x00c08032
	.4byte 0x00d0402f
	.4byte 0x00000033
	.4byte 0x00101032
	.4byte 0x00204032
	.4byte 0x00305033
	.4byte 0x00406033
	.4byte 0x00503033
	.4byte 0x00604033
	.4byte 0x0070a032
	.4byte 0x0080102f
	.4byte 0x00000034
	.4byte 0x00106032
	.4byte 0x00203034
	.4byte 0x00302034
	.4byte 0x00405032
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00fa
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x01024000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x031a0000
	.4byte 0x00000000
	.4byte 0x00ec0000
	.4byte 0x00018000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff02a7
	.4byte 0x00000001
	.4byte 0x00ca0000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff014a
	.4byte 0x00000007
	.4byte 0x00960000
	.4byte 0x00000000
	.4byte 0x016e0000
	.4byte 0x00024000
	.4byte 0xffff014a
	.4byte 0x00000007
	.4byte 0x00aa0000
	.4byte 0x00000000
	.4byte 0x016e0000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008a89
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008a89
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008a89
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008a89
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00008515
	.4byte 0x02010009
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x08c8000a
	.4byte 0x020084cd
	.4byte 0x00008c15
	.4byte 0x08c8000b
	.4byte 0x020084cd
	.4byte 0x10008c15
	.4byte 0x08ca000c
	.4byte 0x0200851d
	.4byte 0x00008c15
	.4byte 0x08ca000c
	.4byte 0x0200852d
	.4byte 0x00000602
	.4byte 0x09ef000b
	.4byte 0x020088f5
	.4byte 0x10008c15
	.4byte 0x09ef0008
	.4byte 0x0200892d
	.4byte 0x00008c15
	.4byte 0x09ef0008
	.4byte 0x0200893d
	.4byte 0x00001815
	.4byte 0x0202000d
	.4byte 0x02008559
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x0200872d
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008779
	.4byte 0x0000c403
	.4byte 0xffff000a
	.4byte 0x020087a1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
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
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00008515
	.4byte 0x02000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001873
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001874
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
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008c25
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
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
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008c41
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008c5d
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00008715
	.4byte 0xffff0008
	.4byte 0x02008875
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008c79
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x0001cb04
	.4byte 0xffff000a
	.4byte 0x02008569
	.4byte 0x00000003
	.4byte 0x08c9000a
	.4byte 0x02008475
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
