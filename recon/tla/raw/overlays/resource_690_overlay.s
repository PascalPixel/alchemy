.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008989, 0x02008311, 0x02008351, 0x02008359, 0x02008669, 0x02008319, 0x02008b19
	overlay_veneer \EntryTarget
	.endr
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_0200007c
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200007c
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
.L_0200007c:
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
	bl 0x0200a088
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_02000100
	cmp	r7, #0
	beq.n	.L_02000100
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_02000108
.L_02000100:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_02000108:
	mov	r3, sl
	bl 0x02009ff8
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_02000116
	b.n	.L_02000262
.L_02000116:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl 0x02009fe0
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x02009ff0
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200a038
	ldr	r3, [pc, #300]
	mov	r1, r9
	str	r3, [r6, #108]
	mov	r3, fp
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
	bl 0x02008038
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
	beq.n	.L_02000262
	cmp	r7, #0
	beq.n	.L_02000262
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000198
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x0200a0d8
.L_02000198:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
.L_020001a0:
	cmp	r3, #0
	beq.n	.L_020001b8
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl 0x02008038
.L_020001b8:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_020001cc
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_020001cc:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000212
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_020001fa
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x02009f68
.L_020001ee:
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_0200020c
.L_020001fa:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x02009f68
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_0200020c:
	bl 0x02009f68
	str	r0, [r6, #52]
.L_02000212:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200022e
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x02009fe0
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x02009ff0
.L_0200022e:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000240
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_02000240:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000252
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_02000252:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000262
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_02000262:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200a2d8
	.4byte 0x02008081
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	movs	r0, #10
	movs	r1, #1
	movs	r2, #14
	bl 0x0200a158
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #11
	movs	r1, #2
	movs	r2, #14
	bl 0x0200a158
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #23
	movs	r1, #3
	movs	r2, #19
	bl 0x0200a158
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r1, #0
	bl 0x0200a038
	movs	r0, #0
	pop	{pc}
	push	{r5, r6, lr}
	adds	r2, r0, #0
	adds	r5, r2, #0
	adds	r5, #98
	ldrb	r3, [r5, #0]
	movs	r0, #63
	adds	r1, r2, #0
	ands	r0, r3
	adds	r1, #85
	movs	r3, #3
	ldr	r6, [r2, #80]
	strb	r3, [r1, #0]
	cmp	r0, #0
	bne.n	.L_020002de
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r2, #40]
.L_020002de:
	cmp	r0, #16
	bne.n	.L_020002e8
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r2, #40]
.L_020002e8:
	cmp	r0, #24
	bne.n	.L_020002f2
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r2, #40]
.L_020002f2:
	lsls	r0, r0, #12
	bl 0x02009f98
	cmp	r0, #0
	bge.n	.L_020002fe
	adds	r0, #63
.L_020002fe:
	asrs	r3, r0, #6
	strh	r3, [r6, #18]
	ldrb	r3, [r5, #0]
	movs	r0, #1
	adds	r3, #1
	strb	r3, [r5, #0]
	negs	r0, r0
	pop	{r5, r6, pc}
	.2byte 0x0000
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xa338
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
	bne.n	.L_02000330
	ldr	r0, [pc, #20]
	b.n	.L_0200033a
.L_02000330:
	ldr	r3, [pc, #20]
	movs	r0, #0
	cmp	r2, r3
	bne.n	.L_0200033a
	ldr	r0, [pc, #16]
.L_0200033a:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000d9
	.4byte 0x0200a380
	.4byte 0x000000d7
	.2byte 0xa3a0
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xa3c0
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #96]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_02000370
	ldr	r0, [pc, #84]
	b.n	.L_020003b8
.L_02000370:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_0200037a
	ldr	r0, [pc, #84]
	b.n	.L_020003b8
.L_0200037a:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_02000384
	ldr	r0, [pc, #80]
	b.n	.L_020003b8
.L_02000384:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_0200038e
	ldr	r0, [pc, #80]
	b.n	.L_020003b8
.L_0200038e:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_02000398
	ldr	r0, [pc, #76]
	b.n	.L_020003b8
.L_02000398:
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_020003a2
	ldr	r0, [pc, #76]
	b.n	.L_020003b8
.L_020003a2:
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_020003ac
	ldr	r0, [pc, #72]
	b.n	.L_020003b8
.L_020003ac:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_020003b6
	ldr	r0, [pc, #72]
	b.n	.L_020003b8
.L_020003b6:
	ldr	r0, [pc, #72]
.L_020003b8:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000d5
	.4byte 0x0200a4dc
	.4byte 0x000000d6
	.4byte 0x0200a524
	.4byte 0x000000d8
	.4byte 0x0200a554
	.4byte 0x000000d9
	.4byte 0x0200a5cc
	.4byte 0x000000db
	.4byte 0x0200a7dc
	.4byte 0x000000dc
	.4byte 0x0200a854
	.4byte 0x000000dd
	.4byte 0x0200a614
	.4byte 0x000000de
	.4byte 0x0200a7ac
	.2byte 0xa4ac
	.2byte 0x0200
	push	{lr}
	bl 0x0200a068
	movs	r0, #0
	bl 0x0200a150
	movs	r2, #0
	movs	r1, #0
	movs	r0, #11
	bl 0x0200a0c0
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #93
	bl 0x02009fd0
	movs	r0, #193
	movs	r1, #3
	bl 0x0200a130
	movs	r1, #0
	movs	r0, #193
	bl 0x0200a078
	bl 0x0200a070
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r5, [pc, #68]
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200a0e0
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200a0e8
	bl 0x0200a1a8
	movs	r1, #0
	bl 0x0200a080
	cmp	r0, #0
	bne.n	.L_0200046c
	movs	r0, #10
	bl 0x0200a060
	adds	r0, r5, #1
	bl 0x0200a0e0
	b.n	.L_02000478
.L_0200046c:
	movs	r0, #20
	bl 0x0200a060
	adds	r0, r5, #2
	bl 0x0200a0e0
.L_02000478:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200a0f0
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x28ae
	.2byte 0x0000
	push	{lr}
	sub	sp, #12
	movs	r3, #5
	movs	r2, #43
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #5
	movs	r1, #70
	movs	r2, #9
	movs	r3, #7
	bl 0x0200a1a0
	add	sp, #12
	pop	{pc}
	push	{r5, r6, lr}
	ldr	r3, [pc, #124]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	sub	sp, #12
	bl 0x0200a088
	ldr	r3, [r0, #8]
	ldr	r2, [r0, #16]
	asrs	r3, r3, #20
	asrs	r2, r2, #20
	cmp	r3, #5
	bne.n	.L_020004e0
	cmp	r2, #17
	bne.n	.L_020004e0
	ldr	r0, [r5, #0]
	movs	r1, #2
	movs	r2, #0
	bl 0x0200a0d0
	movs	r2, #148
	ldr	r0, [r5, #0]
	movs	r1, #88
	lsls	r2, r2, #1
	bl 0x0200a0a0
.L_020004e0:
	movs	r3, #43
	str	r3, [sp, #4]
	movs	r5, #1
	movs	r6, #5
	movs	r0, #5
	movs	r1, #70
	movs	r2, #9
	movs	r3, #7
	str	r6, [sp, #0]
	str	r5, [sp, #8]
	bl 0x0200a1a0
	movs	r0, #4
	movs	r1, #25
	movs	r2, #5
	movs	r3, #17
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200a018
	movs	r3, #17
	str	r3, [sp, #4]
	movs	r1, #25
	movs	r2, #1
	movs	r3, #1
	movs	r0, #4
	str	r6, [sp, #0]
	bl 0x0200a030
	movs	r0, #166
	lsls	r0, r0, #4
	bl 0x02009fd0
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #192
	movs	r2, #0
	ldrsh	r5, [r3, r2]
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	sub	sp, #8
	bl 0x0200a0f8
	movs	r0, #10
	bl 0x0200a060
	movs	r0, #4
	bl 0x0200a088
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r2, #1
	movs	r3, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #14
	movs	r0, #96
	movs	r1, #14
	movs	r2, #72
	bl 0x0200a018
	movs	r2, #4
	movs	r1, #0
	movs	r0, #4
	bl 0x0200a0b0
	movs	r0, #4
	bl 0x0200a0b8
	movs	r0, #4
	bl 0x0200a088
	movs	r1, #0
	bl 0x0200a038
	movs	r0, #4
	movs	r1, #13
	bl 0x0200a0c8
	movs	r2, #16
	movs	r1, #0
	movs	r0, #4
	bl 0x0200a0b0
	movs	r0, #4
	bl 0x0200a0b8
	movs	r1, #10
	movs	r0, #4
	bl 0x0200a0c8
	movs	r0, #10
	bl 0x0200a060
	movs	r0, #123
	bl 0x0200a1b0
	adds	r0, r5, #0
	bl 0x0200a110
	bl 0x0200a070
	add	sp, #8
	pop	{r5, pc}
	push	{lr}
	bl 0x0200a068
	movs	r0, #0
	bl 0x0200a150
	movs	r2, #14
	movs	r0, #4
	movs	r1, #0
	negs	r2, r2
	bl 0x0200a160
	movs	r0, #4
	movs	r1, #16
	movs	r2, #0
	bl 0x0200a160
	bl 0x0200852c
	pop	{pc}
	push	{lr}
	bl 0x0200a068
	movs	r0, #0
	bl 0x0200a150
	movs	r1, #2
	movs	r2, #6
	movs	r0, #4
	negs	r1, r1
	negs	r2, r2
	bl 0x0200a160
	movs	r1, #14
	movs	r2, #8
	movs	r0, #4
	negs	r1, r1
	negs	r2, r2
	bl 0x0200a160
	bl 0x0200852c
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	bl 0x0200a068
	movs	r0, #0
	bl 0x0200a150
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a0f8
	bl 0x0200852c
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	movs	r3, #3
	movs	r2, #15
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #3
	movs	r2, #1
	movs	r3, #1
	movs	r0, #0
	bl 0x0200a030
	movs	r0, #140
	lsls	r0, r0, #2
	bl 0x02009fd0
	add	sp, #8
	pop	{pc}
	push	{lr}
	ldr	r3, [pc, #96]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_02000680
	ldr	r0, [pc, #84]
	b.n	.L_020006c8
.L_02000680:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_0200068a
	ldr	r0, [pc, #84]
	b.n	.L_020006c8
.L_0200068a:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_02000694
	ldr	r0, [pc, #80]
	b.n	.L_020006c8
.L_02000694:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_0200069e
	ldr	r0, [pc, #80]
	b.n	.L_020006c8
.L_0200069e:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_020006a8
	ldr	r0, [pc, #76]
	b.n	.L_020006c8
.L_020006a8:
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_020006b2
	ldr	r0, [pc, #76]
	b.n	.L_020006c8
.L_020006b2:
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_020006bc
	ldr	r0, [pc, #72]
	b.n	.L_020006c8
.L_020006bc:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_020006c6
	ldr	r0, [pc, #72]
	b.n	.L_020006c8
.L_020006c6:
	ldr	r0, [pc, #72]
.L_020006c8:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000d5
	.4byte 0x0200a8f0
	.4byte 0x000000d6
	.4byte 0x0200a974
	.4byte 0x000000d8
	.4byte 0x0200a9a4
	.4byte 0x000000d9
	.4byte 0x0200aa70
	.4byte 0x000000db
	.4byte 0x0200aad0
	.4byte 0x000000dc
	.4byte 0x0200ab3c
	.4byte 0x000000dd
	.4byte 0x0200ab90
	.4byte 0x000000de
	.4byte 0x0200ac2c
	.2byte 0xa89c
	.2byte 0x0200
	push	{lr}
	adds	r1, r0, #0
	adds	r1, #100
	movs	r3, #0
	ldrsh	r2, [r1, r3]
	ldr	r3, [r0, #8]
	lsls	r2, r2, #8
	adds	r3, r3, r2
	str	r3, [r0, #8]
	ldr	r3, [r0, #12]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r2
	str	r3, [r0, #12]
	movs	r2, #160
	ldr	r3, [r0, #24]
	lsls	r2, r2, #3
	adds	r2, #30
	adds	r3, r3, r2
	str	r3, [r0, #24]
	ldr	r3, [r0, #28]
	adds	r3, r3, r2
	str	r3, [r0, #28]
	ldrh	r3, [r1, #0]
	adds	r3, #2
	strh	r3, [r1, #0]
	ldr	r3, [r0, #104]
	subs	r3, #1
	str	r3, [r0, #104]
	cmp	r3, #0
	bne.n	.L_02000756
	bl 0x0200a000
.L_02000756:
	pop	{pc}
	push	{r5, lr}
	adds	r4, r0, #0
	adds	r5, r1, #0
	movs	r0, #30
	adds	r3, r2, #0
	adds	r0, #255
	adds	r2, r5, #0
	adds	r1, r4, #0
	bl 0x02009ff8
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020007d0
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
	adds	r3, r5, #0
	movs	r1, #0
	adds	r3, #85
	strb	r1, [r3, #0]
	adds	r3, #15
	strh	r1, [r3, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	movs	r1, #7
	bl 0x0200a0d8
	ldr	r1, [r5, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200a038
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	movs	r3, #60
	str	r3, [r5, #104]
	ldr	r3, [pc, #20]
	adds	r0, r5, #0
	movs	r1, #5
	str	r3, [r5, #108]
	bl 0x02009fe0
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200a040
.L_020007d0:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x8715
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #28]
	movs	r2, #63
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_020007f6
	movs	r0, #248
	movs	r1, #192
	movs	r2, #128
	lsls	r0, r0, #16
	lsls	r1, r1, #14
	lsls	r2, r2, #17
	bl 0x02008758
.L_020007f6:
	pop	{pc}
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	ldr	r0, [r6, #48]
	ldr	r7, [r6, #80]
	bl 0x02009f90
	lsls	r5, r0, #1
	cmp	r5, #0
	ble.n	.L_02000810
	negs	r5, r5
.L_02000810:
	ldr	r0, [r6, #48]
	bl 0x02009f98
	ldr	r3, [r6, #56]
	lsls	r0, r0, #1
	adds	r3, r3, r0
	str	r3, [r6, #8]
	ldr	r0, [r6, #48]
	ldr	r3, [r6, #60]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r5
	adds	r0, r0, r2
	str	r3, [r6, #12]
	bl 0x02009f98
	cmp	r0, #0
	bge.n	.L_02000836
	adds	r0, #7
.L_02000836:
	asrs	r3, r0, #3
	strh	r3, [r7, #18]
	bl 0x02009f80
	adds	r5, r0, #0
	bl 0x02009f80
	lsls	r5, r5, #9
	ldr	r3, [r6, #48]
	lsls	r0, r0, #9
	lsrs	r0, r0, #16
	lsrs	r5, r5, #16
	adds	r5, r5, r0
	movs	r2, #128
	adds	r3, r3, r5
	lsls	r2, r2, #3
	adds	r3, r3, r2
	str	r3, [r6, #48]
	movs	r0, #0
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	subs	r2, #2
	movs	r1, #144
	strh	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #44]
	bl 0x02009f78
	bl 0x0200a170
	bl 0x0200a170
	movs	r1, #130
	lsls	r1, r1, #1
	movs	r0, #1
	adds	r1, #255
	movs	r2, #8
	movs	r3, #9
	bl 0x0200a180
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #93
	b.n	.L_020008b0
	.4byte 0x00000c08
	.4byte 0x00003f10
	.2byte 0x87d9
	.2byte 0x0200
.L_020008b0:
	bl 0x02009fc8
	mov	r8, r0
	cmp	r0, #0
	beq.n	.L_020008c6
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a0c0
	b.n	.L_0200097a
.L_020008c6:
	movs	r0, #11
	bl 0x0200a088
	adds	r7, r0, #0
	ldr	r6, [r7, #80]
	movs	r2, #13
	ldrb	r3, [r6, #9]
	negs	r2, r2
	ldrb	r1, [r6, #5]
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	movs	r3, #33
	negs	r3, r3
	ands	r3, r1
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r2, r3
	strb	r2, [r6, #9]
	mov	r2, r8
	strb	r2, [r6, #27]
	movs	r1, #0
	bl 0x0200a038
	movs	r3, #92
	adds	r3, r3, r7
	mov	r2, r8
	strb	r2, [r3, #0]
	mov	sl, r3
	adds	r3, r7, #0
	adds	r3, #85
	movs	r0, #10
	strb	r2, [r3, #0]
	adds	r0, #255
	bl 0x02009fc8
	cmp	r0, #0
	bne.n	.L_0200091c
	ldr	r3, [r7, #12]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r7, #12]
.L_0200091c:
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r2, #1
	strb	r3, [r1, #0]
	mov	r9, r2
	adds	r3, r7, #0
	adds	r3, #97
	mov	r2, r9
	movs	r1, #193
	strb	r2, [r3, #0]
	lsls	r1, r1, #3
	movs	r0, #68
	bl 0x02009fb0
	adds	r5, r0, #0
	movs	r0, #193
	bl 0x0200a058
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r5, r5, r3
	adds	r2, r5, #0
	movs	r1, #128
	ldrb	r0, [r6, #16]
	bl 0x02009fc0
	movs	r0, #68
	bl 0x02009fb8
	ldr	r3, [r7, #8]
	mov	r2, r8
	str	r3, [r7, #56]
	ldr	r3, [r7, #12]
	str	r2, [r7, #48]
	str	r3, [r7, #60]
	mov	r2, sl
	mov	r3, r9
	strb	r3, [r2, #0]
	ldr	r3, [pc, #20]
	mov	r2, r8
	str	r3, [r7, #108]
	adds	r3, r7, #0
	adds	r3, #86
	strb	r2, [r3, #0]
.L_0200097a:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x87fd
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	movs	r2, #128
	adds	r3, r3, r1
	lsls	r2, r2, #1
	str	r2, [r3, #0]
	ldr	r3, [pc, #344]
	adds	r2, #224
	adds	r5, r3, r2
	movs	r3, #0
	ldrsh	r2, [r5, r3]
	ldr	r3, [pc, #340]
	sub	sp, #8
	cmp	r2, r3
	bne.n	.L_020009b2
	bl 0x02008860
.L_020009b2:
	movs	r1, #0
	ldrsh	r2, [r5, r1]
	ldr	r3, [pc, #328]
	cmp	r2, r3
	bne.n	.L_02000a66
	movs	r0, #0
	bl 0x0200a140
	ldr	r0, [pc, #320]
	bl 0x0200a188
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x02009fc8
	cmp	r0, #0
	beq.n	.L_020009e0
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a0c0
.L_020009e0:
	movs	r0, #10
	bl 0x0200a088
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #2
	movs	r0, #18
	bl 0x0200a0c8
	movs	r0, #19
	movs	r1, #2
	bl 0x0200a0c8
	movs	r0, #21
	movs	r1, #2
	bl 0x0200a0c8
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009fc8
	cmp	r0, #0
	beq.n	.L_02000a1e
	movs	r0, #10
	movs	r1, #2
	bl 0x0200a0c8
.L_02000a1e:
	bl 0x02008e04
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x02009fc8
	cmp	r0, #0
	beq.n	.L_02000a66
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a0c0
	movs	r1, #184
	movs	r2, #184
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	movs	r0, #21
	bl 0x0200a0c0
	movs	r0, #21
	bl 0x0200a088
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r2, #12
	movs	r3, #11
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #9
	movs	r1, #11
	movs	r2, #1
	movs	r3, #1
	bl 0x0200a030
.L_02000a66:
	ldr	r3, [pc, #144]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #148]
	cmp	r2, r3
	bne.n	.L_02000aca
	movs	r0, #140
	lsls	r0, r0, #2
	bl 0x02009fc8
	cmp	r0, #0
	beq.n	.L_02000aa0
	movs	r3, #3
	movs	r2, #15
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #3
	movs	r2, #1
	movs	r3, #1
	bl 0x0200a030
	movs	r0, #9
	movs	r1, #4
	bl 0x0200a0c8
.L_02000aa0:
	movs	r0, #10
	adds	r0, #255
	bl 0x02009fc8
	cmp	r0, #0
	beq.n	.L_02000ac4
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x02009fc8
	cmp	r0, #0
	beq.n	.L_02000ac4
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x02009fd8
	bl 0x0200925c
.L_02000ac4:
	ldr	r0, [pc, #68]
	bl 0x0200a188
.L_02000aca:
	ldr	r3, [pc, #44]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r5, r3, r2
	movs	r3, #0
	ldrsh	r2, [r5, r3]
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_02000ae2
	movs	r0, #0
	bl 0x0200a140
.L_02000ae2:
	movs	r1, #0
	ldrsh	r2, [r5, r1]
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_02000af2
	movs	r0, #0
	bl 0x0200a140
.L_02000af2:
	movs	r0, #0
	add	sp, #8
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x000000db
	.4byte 0x000000dd
	.4byte 0x0200a26c
	.4byte 0x000000d8
	.4byte 0x0200a272
	.4byte 0x000000d7
	.2byte 0x00da
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #244]
.L_02000b1c:
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #236]
	sub	sp, #8
	cmp	r2, r3
	bne.n	.L_02000bb6
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009fc8
	cmp	r0, #0
	beq.n	.L_02000b72
	movs	r3, #11
	movs	r2, #12
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #9
	movs	r1, #11
	movs	r2, #1
.L_02000b4a:
	movs	r3, #1
	bl 0x0200a030
	movs	r5, #1
	movs	r0, #12
	movs	r1, #32
	movs	r2, #11
	movs	r3, #15
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200a018
	movs	r0, #77
	movs	r1, #15
	movs	r2, #75
	movs	r3, #15
.L_02000b6a:
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200a018
.L_02000b72:
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x02009fc8
	cmp	r0, #0
	beq.n	.L_02000bb6
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a0c0
	movs	r1, #184
	movs	r2, #184
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	movs	r0, #21
	bl 0x0200a0c0
	movs	r0, #21
	bl 0x0200a088
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r2, #12
	movs	r3, #11
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #9
	movs	r1, #11
	movs	r2, #1
	movs	r3, #1
	bl 0x0200a030
.L_02000bb6:
	ldr	r3, [pc, #88]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_02000c0a
	movs	r0, #166
	lsls	r0, r0, #4
	bl 0x02009fc8
	cmp	r0, #0
	beq.n	.L_02000c0a
	movs	r5, #1
	movs	r0, #5
	movs	r1, #46
	movs	r2, #5
	movs	r3, #47
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200a018
	movs	r0, #4
	movs	r1, #25
	movs	r2, #5
	movs	r3, #17
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200a018
	movs	r3, #5
	movs	r2, #17
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #4
	movs	r1, #25
	movs	r2, #1
	movs	r3, #1
	bl 0x0200a030
.L_02000c0a:
	movs	r0, #0
	add	sp, #8
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x000000dd
	.2byte 0x00d5
	.2byte 0x0000
	push	{lr}
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009fd0
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009fd8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	bl 0x02009770
	pop	{pc}
	push	{r5, lr}
	sub	sp, #8
	bl 0x0200a068
	movs	r0, #0
	bl 0x0200a150
	movs	r5, #8
.L_02000c54:
	adds	r0, r5, #0
	bl 0x0200a088
	cmp	r0, #0
	beq.n	.L_02000c66
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
.L_02000c66:
	adds	r5, #1
	cmp	r5, #63
	bls.n	.L_02000c54
	ldr	r3, [pc, #132]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #124]
	cmp	r2, r3
	bne.n	.L_02000c94
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #33
	movs	r1, #30
	movs	r2, #23
	movs	r3, #14
	bl 0x0200a018
	b.n	.L_02000ca8
.L_02000c94:
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #27
	movs	r1, #29
	movs	r2, #40
	movs	r3, #16
	bl 0x0200a018
.L_02000ca8:
	movs	r0, #158
	bl 0x0200a1b0
	ldr	r5, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200a0c8
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x0200a090
	movs	r2, #4
	movs	r1, #2
	negs	r2, r2
	ldr	r0, [r5, #0]
	bl 0x0200a0a8
	movs	r0, #6
	bl 0x0200a060
	movs	r0, #2
	bl 0x0200a110
	bl 0x0200a120
	bl 0x0200a128
	bl 0x0200a070
	add	sp, #8
	pop	{r5, pc}
	.4byte 0x02000240
	.2byte 0x00d8
	.2byte 0x0000
	push	{lr}
	movs	r1, #8
	movs	r2, #9
	movs	r0, #1
	bl 0x0200a178
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009fd0
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_02000de4
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #164
	adds	r3, r2, r1
	movs	r6, #0
	ldrsb	r6, [r3, r6]
	cmp	r6, #0
	bne.n	.L_02000de4
	ldr	r7, [pc, #172]
	movs	r2, #1
	ldr	r3, [r7, #0]
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_02000d6a
	ldr	r5, [pc, #164]
	ldr	r3, [pc, #164]
	movs	r1, #160
	lsls	r1, r1, #19
	mov	r8, r3
	adds	r1, #96
	movs	r2, #32
	adds	r0, r5, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x4826
	adds	r1, r5, #0
	movs	r2, #32
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x603e
.L_02000d6a:
	ldr	r3, [r7, #0]
	cmp	r3, #0
	bge.n	.L_02000d72
	adds	r3, #7
.L_02000d72:
	asrs	r2, r3, #3
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_02000d7c
	adds	r3, r2, #3
.L_02000d7c:
	asrs	r1, r3, #2
	ldr	r6, [pc, #120]
	ldr	r5, [pc, #108]
	lsls	r3, r1, #2
	subs	r1, r2, r3
	movs	r0, #0
	movs	r4, #24
.L_02000d8a:
	adds	r2, r1, r0
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_02000d94
	adds	r3, r2, #3
.L_02000d94:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	ldrh	r2, [r5, r4]
	lsls	r3, r3, #1
	adds	r3, #24
	adds	r0, #1
	strh	r2, [r6, r3]
	adds	r4, #2
	cmp	r0, #3
	ble.n	.L_02000d8a
	ldr	r1, [pc, #80]
	ldr	r0, [pc, #80]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_02000dda
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r1
	adds	r3, #4
	adds	r2, #1
	stmia	r3!, {r6}
	strh	r2, [r1, #0]
	movs	r2, #160
	lsls	r2, r2, #19
	adds	r2, #96
	stmia	r3!, {r2}
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #8
	str	r2, [r3, #0]
.L_02000dda:
	strh	r4, [r0, #0]
	ldr	r2, [pc, #12]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
.L_02000de4:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200ac5c
	.4byte 0x0200ac60
	.4byte 0x03000730
	.4byte 0x0200ac80
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #144
	adds	r2, #86
	str	r2, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl 0x02009f78
	movs	r0, #170
	bl 0x0200a168
	bl 0x02008ed0
	pop	{pc}
	.2byte 0x8d15
	.2byte 0x0200
	push	{lr}
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #6
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #32]
	ldr	r3, [r3, #0]
	cmp	r2, r3
	bge.n	.L_02000e64
	ldr	r3, [pc, #28]
	ldrh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #20
	strh	r2, [r3, #0]
	movs	r2, #128
	ldr	r3, [pc, #4]
	lsls	r2, r2, #19
	adds	r2, #80
	b.n	.L_02000e7e
	.4byte 0x00000000
	.4byte 0x0200ace0
	.2byte 0xace4
	.2byte 0x0200
.L_02000e64:
	ldr	r3, [pc, #36]
	ldrh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #20
	strh	r2, [r3, #0]
	movs	r2, #128
	ldr	r3, [pc, #16]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #12]
	subs	r2, #2
.L_02000e7e:
	strh	r3, [r2, #0]
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0000100c
	.4byte 0x00003f42
	.4byte 0x0200ace6
	.4byte 0x049b23c0
	.4byte 0x21bc6a1a
	.4byte 0x18520049
	.4byte 0x5ed12306
	.4byte 0x23b54807
	.4byte 0x1a5b00db
	.4byte 0x4b066003
	.4byte 0x5e522102
	.4byte 0x801a4905
	.4byte 0x681b4b05
	.4byte 0x1ad2089b
	.4byte 0x4770800a
	.4byte 0x0200ace0
	.4byte 0x0200ace4
	.4byte 0x0200ace6
	.2byte 0x122c
	.2byte 0x0300
	push	{lr}
	ldr	r2, [pc, #20]
	movs	r0, #1
	movs	r1, #0
	bl 0x02009fa8
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #8]
	bl 0x02009f78
	pop	{pc}
	.4byte 0x02008e31
	.2byte 0x8e91
	.2byte 0x0200
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x0200a190
	pop	{pc}
	.2byte 0x0000
	.2byte 0xa26c
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200a088
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	ldr	r0, [pc, #12]
	bl 0x0200a190
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xa26c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r0, #9
	sub	sp, #8
	bl 0x0200a088
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	movs	r0, #129
	asrs	r7, r3, #20
	ldr	r3, [r5, #16]
	lsls	r0, r0, #2
	asrs	r6, r3, #20
	bl 0x02009fc8
	cmp	r0, #0
	bne.n	.L_02000fc4
.L_02000f4c:
	cmp	r7, #11
	bne.n	.L_02000fc4
	cmp	r6, #12
	bne.n	.L_02000fc4
	adds	r3, r5, #0
	adds	r3, #85
	strb	r0, [r3, #0]
	movs	r6, #10
.L_02000f5c:
	ldr	r3, [r5, #12]
	ldr	r2, [pc, #104]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r5, #12]
	subs	r6, #1
	bl 0x0200a060
	cmp	r6, #0
	bgt.n	.L_02000f5c
	movs	r0, #9
	bl 0x0200956c
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a0c0
	movs	r1, #184
	movs	r2, #184
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	movs	r0, #21
	bl 0x0200a0c0
	movs	r0, #21
	bl 0x0200a088
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #21
	bl 0x0200a088
	ldr	r2, [pc, #40]
	ldr	r3, [r0, #12]
	movs	r1, #11
	adds	r3, r3, r2
	str	r3, [r0, #12]
	movs	r2, #12
	movs	r3, #11
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #9
	movs	r2, #1
	movs	r3, #1
	bl 0x0200a030
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x02009fd0
.L_02000fc4:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.4byte 0xfffe0000
	.2byte 0x0000
	.2byte 0xffc0
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	movs	r0, #10
	sub	sp, #8
	bl 0x0200a088
	adds	r7, r0, #0
	ldr	r3, [r7, #8]
	asrs	r6, r3, #20
	ldr	r3, [r7, #16]
	asrs	r5, r3, #20
	cmp	r6, #11
	beq.n	.L_02000fee
	b.n	.L_0200115c
.L_02000fee:
	cmp	r5, #9
	beq.n	.L_02000ff4
	b.n	.L_0200115c
.L_02000ff4:
	adds	r2, r7, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r6, #8
.L_02000ffe:
	movs	r0, #1
	bl 0x0200a060
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #420]
	subs	r6, #1
	adds	r3, r3, r2
	str	r3, [r7, #12]
	cmp	r6, #0
	bgt.n	.L_02000ffe
	movs	r0, #10
	bl 0x0200956c
	movs	r0, #20
	bl 0x0200a060
	movs	r0, #192
	movs	r1, #1
	movs	r2, #224
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200a108
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #10
	adds	r1, #204
	adds	r2, #102
	bl 0x0200a090
	movs	r1, #0
	movs	r2, #8
	movs	r0, #10
	bl 0x0200a0b0
	movs	r0, #10
	bl 0x0200a0b8
	movs	r0, #10
	bl 0x0200a060
	ldr	r3, [pc, #340]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r1, #128
	ldr	r0, [r3, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a0f8
	movs	r0, #10
	ldr	r1, [pc, #324]
	ldr	r2, [pc, #324]
	bl 0x0200a090
	movs	r1, #0
	movs	r2, #80
	movs	r0, #10
	bl 0x0200a0b0
	movs	r0, #10
	bl 0x0200a0b8
	movs	r0, #10
	bl 0x0200956c
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x02009fc8
	cmp	r0, #0
	bne.n	.L_020010f4
	movs	r0, #10
	bl 0x0200a060
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #10
	adds	r1, #204
	adds	r2, #102
	bl 0x0200a090
	movs	r1, #0
	movs	r2, #80
	movs	r0, #10
	bl 0x0200a0b0
	movs	r0, #10
	bl 0x0200a0b8
	movs	r1, #160
	movs	r2, #160
	movs	r0, #10
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200a090
	movs	r1, #0
	movs	r2, #80
	movs	r0, #10
	bl 0x0200a0b0
	movs	r0, #5
	bl 0x0200a060
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a0c0
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x02009fd0
	b.n	.L_02001156
.L_020010f4:
	movs	r5, #1
	movs	r0, #12
	movs	r1, #32
	movs	r2, #11
	movs	r3, #15
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200a018
	movs	r3, #15
	movs	r0, #77
	movs	r1, #15
	movs	r2, #75
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200a018
	movs	r1, #0
	movs	r2, #12
	movs	r0, #10
	bl 0x0200a0b0
	movs	r0, #10
	bl 0x0200a0b8
	movs	r0, #20
	bl 0x0200a060
	movs	r3, #11
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r2, #1
	movs	r3, #1
	movs	r0, #9
	movs	r1, #11
	bl 0x0200a030
	movs	r0, #10
	movs	r1, #2
	bl 0x0200a0c8
	movs	r0, #30
	bl 0x0200a060
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009fd0
.L_02001156:
	bl 0x0200a070
	b.n	.L_020011a4
.L_0200115c:
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009fc8
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_020011a4
	cmp	r6, #16
	bne.n	.L_020011a4
	cmp	r5, #12
	bne.n	.L_020011a4
	str	r5, [sp, #4]
	movs	r1, #12
	movs	r2, #1
	movs	r0, #17
	movs	r3, #1
	adds	r5, r7, #0
	str	r6, [sp, #0]
	adds	r5, #85
	bl 0x0200a030
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #30
	bl 0x0200a060
	movs	r0, #132
	mov	r3, r8
	lsls	r0, r0, #1
	strb	r3, [r5, #0]
	adds	r0, #255
	bl 0x02009fd0
	bl 0x0200a070
.L_020011a4:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0xfffe0000
	.4byte 0x02000240
	.4byte 0x00026666
	.2byte 0x3333
	.2byte 0x0001
	push	{lr}
	bl 0x0200a198
	bl 0x02008f2c
	pop	{pc}
	push	{lr}
	bl 0x0200a198
	bl 0x02008fd0
	pop	{pc}
	push	{lr}
	bl 0x0200a198
	ldr	r3, [pc, #32]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200a088
	adds	r0, #35
	ldrb	r3, [r0, #0]
	adds	r3, #254
	strb	r3, [r0, #0]
	bl 0x02008f2c
	bl 0x02008fd0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	sub	sp, #8
	movs	r3, #0
	str	r3, [sp, #4]
	movs	r5, #64
	movs	r0, #11
	movs	r1, #73
	movs	r2, #10
.L_02001210:
	movs	r3, #10
	str	r5, [sp, #0]
	bl 0x0200a030
	movs	r3, #10
	movs	r0, #42
	movs	r1, #73
	movs	r2, #10
	str	r5, [sp, #0]
	str	r3, [sp, #4]
	bl 0x0200a030
	add	sp, #8
	pop	{r5, pc}
	push	{r5, lr}
	sub	sp, #8
	movs	r3, #11
	str	r3, [sp, #0]
	movs	r5, #73
	movs	r0, #64
	movs	r1, #0
	movs	r2, #10
	movs	r3, #10
	str	r5, [sp, #4]
	bl 0x0200a030
	movs	r3, #42
	str	r3, [sp, #0]
	movs	r0, #64
	movs	r1, #10
	movs	r2, #10
	movs	r3, #10
	str	r5, [sp, #4]
	bl 0x0200a030
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	sub	sp, #8
	bl 0x02009fc8
	cmp	r0, #0
	bne.n	.L_020012bc
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x02009fc8
	cmp	r0, #0
	bne.n	.L_020012bc
	movs	r3, #50
	str	r3, [sp, #4]
	movs	r5, #64
	movs	r0, #11
	movs	r1, #73
	movs	r2, #10
	movs	r3, #10
	str	r5, [sp, #0]
	bl 0x0200a030
	movs	r3, #60
	str	r3, [sp, #4]
	movs	r0, #42
	movs	r1, #73
	movs	r2, #10
	movs	r3, #10
	str	r5, [sp, #0]
	bl 0x0200a030
	movs	r3, #11
	movs	r2, #75
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #42
	movs	r1, #11
	movs	r2, #10
	movs	r3, #4
	bl 0x0200a030
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x02009fd0
.L_020012bc:
	add	sp, #8
	pop	{r5, pc}
	push	{r5, lr}
	movs	r0, #136
	lsls	r0, r0, #2
	sub	sp, #8
	bl 0x02009fc8
	cmp	r0, #0
	beq.n	.L_020012fe
	movs	r3, #11
	str	r3, [sp, #0]
	movs	r5, #73
	movs	r0, #64
	movs	r1, #50
	movs	r2, #10
	movs	r3, #10
	str	r5, [sp, #4]
	bl 0x0200a030
	movs	r3, #42
	str	r3, [sp, #0]
	movs	r0, #64
	movs	r1, #60
	movs	r2, #10
	movs	r3, #10
	str	r5, [sp, #4]
	bl 0x0200a030
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x02009fd8
.L_020012fe:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #10
	sub	sp, #8
	bl 0x0200a088
	ldr	r3, [r0, #8]
	asrs	r6, r3, #20
	ldr	r3, [r0, #16]
	asrs	r5, r3, #20
	bl 0x02009200
	ldr	r0, [pc, #108]
	bl 0x0200a190
	cmp	r6, #12
	bne.n	.L_02001336
	cmp	r5, #15
	bne.n	.L_02001336
	movs	r3, #42
	movs	r2, #14
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #42
	movs	r1, #3
	b.n	.L_02001352
.L_02001336:
	cmp	r6, #13
	bne.n	.L_0200133e
	cmp	r5, #15
	beq.n	.L_02001346
.L_0200133e:
	cmp	r6, #16
	bne.n	.L_02001370
	cmp	r5, #15
	bne.n	.L_02001370
.L_02001346:
	movs	r3, #42
	movs	r2, #14
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #42
	movs	r1, #0
.L_02001352:
	movs	r2, #7
	movs	r3, #3
	bl 0x0200a030
	movs	r3, #11
	movs	r2, #73
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #42
	movs	r1, #9
	movs	r2, #10
	movs	r3, #10
	bl 0x0200a030
	b.n	.L_02001384
.L_02001370:
	movs	r3, #11
	movs	r2, #73
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #42
	movs	r1, #9
	movs	r2, #10
	movs	r3, #10
	bl 0x0200a030
.L_02001384:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0xa272
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	bl 0x0200a068
	movs	r0, #0
	bl 0x0200a150
	movs	r0, #10
	movs	r1, #1
	bl 0x0200a100
	movs	r0, #10
	bl 0x0200a088
	adds	r7, r0, #0
	ldr	r3, [r7, #8]
	asrs	r6, r3, #20
	ldr	r3, [r7, #16]
	asrs	r5, r3, #20
	cmp	r6, #12
	bne.n	.L_020013ee
	cmp	r5, #15
	bne.n	.L_020013ee
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009fd0
	adds	r2, r7, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r5, #32
.L_020013d2:
	movs	r0, #1
	bl 0x0200a060
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #376]
	subs	r5, #1
	adds	r3, r3, r2
	str	r3, [r7, #12]
	cmp	r5, #0
	bgt.n	.L_020013d2
	movs	r0, #10
	bl 0x02009620
	b.n	.L_020014aa
.L_020013ee:
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009fc8
	cmp	r0, #0
	bne.n	.L_02001442
	cmp	r6, #16
	bne.n	.L_02001442
	cmp	r5, #15
	bne.n	.L_02001442
	adds	r3, r7, #0
	adds	r3, #85
	strb	r0, [r3, #0]
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009fd0
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009fd0
	movs	r5, #48
.L_02001420:
	movs	r0, #1
	bl 0x0200a060
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #296]
	subs	r5, #1
	adds	r3, r3, r2
	str	r3, [r7, #12]
	cmp	r5, #0
	bgt.n	.L_02001420
	movs	r0, #10
	bl 0x02009620
	movs	r0, #30
	bl 0x0200a060
	b.n	.L_02001548
.L_02001442:
	cmp	r6, #19
	bne.n	.L_020014b2
	cmp	r5, #15
	bne.n	.L_020014b2
	movs	r0, #18
	movs	r1, #16
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	str	r6, [sp, #0]
	bl 0x0200a030
	adds	r2, r7, #0
	movs	r0, #145
	adds	r2, #85
	movs	r3, #0
	lsls	r0, r0, #1
	strb	r3, [r2, #0]
	adds	r0, #255
	bl 0x02009fd0
	movs	r5, #32
.L_0200146e:
	movs	r0, #1
	bl 0x0200a060
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #220]
	subs	r5, #1
	adds	r3, r3, r2
	str	r3, [r7, #12]
	cmp	r5, #0
	bgt.n	.L_0200146e
	movs	r0, #10
	bl 0x02009620
	movs	r0, #102
	bl 0x02009fc8
	cmp	r0, #0
	bne.n	.L_020014aa
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a0c0
	movs	r0, #10
	ldr	r1, [pc, #184]
	bl 0x0200a098
	movs	r0, #20
	bl 0x0200a060
.L_020014aa:
	movs	r0, #20
	bl 0x0200a060
	b.n	.L_02001548
.L_020014b2:
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x02009fc8
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_0200151e
	cmp	r6, #13
	bne.n	.L_0200151e
	cmp	r5, #15
	bne.n	.L_0200151e
	movs	r3, #12
	str	r3, [sp, #0]
	movs	r0, #1
	movs	r1, #3
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200a030
	movs	r2, #1
	movs	r3, #1
	movs	r0, #1
	movs	r1, #3
	str	r5, [sp, #4]
	str	r6, [sp, #0]
	bl 0x0200a030
	adds	r3, r7, #0
	adds	r3, #85
	mov	r2, r8
	strb	r2, [r3, #0]
	movs	r5, #16
.L_020014f4:
	movs	r0, #1
	bl 0x0200a060
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #84]
	subs	r5, #1
	adds	r3, r3, r2
	str	r3, [r7, #12]
	cmp	r5, #0
	bgt.n	.L_020014f4
	movs	r0, #10
	bl 0x02009620
	movs	r0, #20
	bl 0x0200a060
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x02009fd0
	b.n	.L_02001548
.L_0200151e:
	cmp	r6, #14
	bne.n	.L_02001548
	cmp	r5, #15
	bne.n	.L_02001548
	movs	r3, #12
	str	r3, [sp, #0]
	movs	r0, #1
	movs	r1, #3
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200a030
	movs	r0, #1
	movs	r1, #3
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200a030
.L_02001548:
	bl 0x0200a070
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0xfffe0000
	.2byte 0xa32c
	.2byte 0x0200
	push	{lr}
	bl 0x0200a198
	bl 0x0200922c
	bl 0x0200938c
	pop	{pc}
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	sub	sp, #68
	bl 0x0200a088
	movs	r3, #0
	mov	r8, r3
	movs	r3, #22
	add	r5, sp, #16
	adds	r3, #255
	strh	r3, [r5, #24]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r5, #8]
	str	r3, [r5, #12]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #16]
	str	r3, [r5, #20]
	ldr	r3, [pc, #44]
	adds	r6, r0, #0
	movs	r0, #154
	str	r3, [r5, #28]
	bl 0x0200a1b0
	mov	r3, r8
	ldr	r0, [r6, #8]
	ldr	r1, [r6, #12]
	ldr	r2, [r6, #16]
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r3, #224
	lsls	r3, r3, #13
	str	r3, [sp, #8]
	movs	r3, #0
	str	r5, [sp, #12]
	bl 0x020080b8
	add	sp, #68
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0xa310
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
	bl 0x02009f68
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_020015f8
	adds	r3, #15
.L_020015f8:
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
	sub	sp, #68
	bl 0x0200a088
	ldr	r3, [pc, #108]
	add	r2, sp, #16
	str	r3, [r2, #36]
	movs	r3, #0
	adds	r7, r0, #0
	mov	r9, r2
	mov	sl, r3
.L_0200163e:
	mov	r2, sl
	lsls	r6, r2, #12
	adds	r0, r6, #0
	bl 0x02009f98
	add	r5, sp, #56
	movs	r3, #0
	str	r0, [r5, #0]
	adds	r0, r6, #0
	str	r3, [r5, #4]
	bl 0x02009f90
	ldr	r6, [r5, #0]
	mov	r8, r0
	str	r0, [r5, #8]
	movs	r1, #3
	adds	r0, r6, #0
	bl 0x02009f68
	ldr	r3, [r5, #4]
	adds	r6, r6, r0
	str	r6, [r5, #0]
	ldr	r2, [r7, #16]
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	str	r3, [sp, #0]
.L_02001672:
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r3, #128
	lsls	r3, r3, #17
	adds	r3, #1
	str	r3, [sp, #8]
	mov	r3, r9
	str	r3, [sp, #12]
	adds	r3, r6, #0
	bl 0x020080b8
	movs	r2, #2
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_0200163e
	add	sp, #68
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x95c9
	.2byte 0x0200
	push	{lr}
	movs	r0, #130
	lsls	r0, r0, #2
	bl 0x02009fd0
	pop	{pc}
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #100
	movs	r0, #0
	ldrsh	r1, [r2, r0]
	ldrh	r3, [r2, #0]
	cmp	r1, #0
	beq.n	.L_020016c8
	subs	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_0200172e
.L_020016c8:
	adds	r3, r5, #0
	adds	r3, #90
	movs	r0, #131
	strb	r1, [r3, #0]
	lsls	r0, r0, #1
	bl 0x02009fc8
	movs	r3, #1
	negs	r3, r3
	cmp	r0, #0
	bne.n	.L_020016ee
	ldr	r3, [pc, #80]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #76]
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r3, [r1, r3]
.L_020016ee:
	movs	r0, #1
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_02001700
	adds	r0, r5, #0
	movs	r1, #9
	bl 0x02009fe0
	b.n	.L_0200172e
.L_02001700:
	ldrh	r1, [r5, #6]
	movs	r2, #128
	subs	r3, r3, r1
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	lsls	r2, r2, #5
	cmp	r3, r2
	ble.n	.L_02001712
	adds	r3, r2, #0
.L_02001712:
	ldr	r2, [pc, #36]
	cmp	r3, r2
	bge.n	.L_0200171a
	adds	r3, r2, #0
.L_0200171a:
	adds	r3, r1, r3
	adds	r0, r5, #0
	movs	r1, #2
	strh	r3, [r5, #6]
	bl 0x02009fe0
	adds	r0, r5, #0
	movs	r1, #48
	bl 0x02009fe8
.L_0200172e:
	pop	{r5, pc}
	.4byte 0x03001150
	.4byte 0x0200a276
	.2byte 0xf000
	.2byte 0xffff
	push	{lr}
.L_0200173e:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #162
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_0200176c
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x02009fd0
	bl 0x0200a118
	bl 0x0200a148
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x02009fd8
.L_0200176c:
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #176]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	sub	sp, #40
	bl 0x0200a138
	adds	r7, r0, #0
.L_02001790:
	bl 0x0200973c
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #52]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	str	r3, [r7, #64]
	movs	r3, #0
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r1, [pc, #132]
	ldr	r3, [r7, #8]
	add	r2, sp, #28
	mov	sl, r2
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r1
	adds	r3, r3, r2
	mov	r0, sl
	str	r3, [sp, #12]
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	ands	r3, r1
	adds	r3, r3, r2
	str	r3, [sp, #8]
	str	r3, [r0, #8]
	ldr	r2, [sp, #12]
	str	r3, [sp, #16]
	adds	r3, r7, #0
	adds	r3, #34
	str	r2, [sp, #20]
	str	r3, [sp, #4]
	adds	r1, r2, #0
	ldrb	r0, [r3, #0]
	ldr	r2, [sp, #16]
	bl 0x0200a028
	str	r0, [sp, #24]
	ldr	r2, [sp, #20]
	ldr	r1, [r7, #8]
	ldr	r0, [sp, #16]
	ldr	r3, [pc, #68]
	ldr	r6, [r7, #16]
	subs	r1, r2, r1
	subs	r6, r0, r6
	mov	r8, r3
	adds	r0, r1, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c31
	adds	r5, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x182d
	movs	r0, #128
	lsls	r0, r0, #11
	cmp	r5, r0
	bge.n	.L_02001848
	ldr	r3, [pc, #36]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #36]
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r2, [r1, r3]
.L_02001824:
	mov	r9, r2
	lsls	r3, r2, #16
	ldr	r2, [pc, #24]
	cmp	r3, r2
	bne.n	.L_02001874
	b.n	.L_02001a0a
	.4byte 0x02000240
	.4byte 0xfff00000
	.4byte 0x0300021c
	.4byte 0x03001150
	.4byte 0x0200a2b6
	.2byte 0x0000
	.2byte 0xffff
.L_02001848:
	.2byte 0x9b04
	ldr	r2, [sp, #20]
	ldr	r0, [r7, #16]
	ldr	r1, [r7, #8]
	subs	r0, r3, r0
	subs	r1, r2, r1
	bl 0x02009f88
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	movs	r3, #128
	ldr	r2, [pc, #16]
	mov	r9, r0
	lsls	r3, r3, #6
	add	r3, r9
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	mov	r9, r3
	b.n	.L_02001874
	.2byte 0xc000
	.2byte 0xffff
.L_02001874:
	.2byte 0x2080
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x02009fa0
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldr	r1, [r2, #0]
	ldrb	r0, [r3, #0]
	ldr	r2, [r2, #8]
	bl 0x0200a028
	mov	fp, r0
	cmp	r0, #255
	beq.n	.L_020018f6
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldrb	r0, [r3, #0]
	ldr	r1, [r2, #0]
	ldr	r2, [r2, #8]
	bl 0x0200a020
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_020018f6
	ldr	r0, [sp, #12]
	mov	r2, sl
	str	r0, [r2, #0]
	ldr	r3, [sp, #8]
	adds	r0, r7, #0
	str	r3, [r2, #8]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	adds	r2, r7, #0
	str	r3, [r7, #52]
	adds	r2, #100
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r1, [sp, #12]
	ldr	r3, [sp, #8]
	ldr	r2, [r7, #12]
	bl 0x0200a008
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x02009fe0
	adds	r0, r7, #0
	movs	r1, #48
	bl 0x02009fe8
	adds	r0, r7, #0
	bl 0x0200a010
	ldr	r3, [pc, #292]
	str	r3, [r7, #108]
	b.n	.L_020019a0
.L_020018f6:
	movs	r3, #0
	mov	r0, r9
	strh	r0, [r7, #6]
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r2, [sp, #12]
	str	r2, [r7, #8]
	ldr	r3, [sp, #8]
	str	r3, [r7, #16]
	b.n	.L_020019ec
.L_0200190a:
	ldr	r2, [sp, #4]
	ldr	r1, [r6, #0]
	ldrb	r0, [r2, #0]
	ldr	r2, [r6, #8]
	bl 0x0200a020
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_020019c0
	ldrh	r3, [r7, #32]
	movs	r2, #89
	subs	r3, #2
	str	r3, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #20]
	movs	r0, #0
	adds	r2, r2, r5
	mov	sl, r0
	mov	r8, r2
.L_02001938:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02001962
	mov	r3, r8
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001962
	cmp	r5, r7
	beq.n	.L_02001962
	ldrh	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r0, #8
	subs	r3, #2
	ldr	r1, [sp, #0]
	add	r2, sp, #28
	bl 0x0200a050
	cmp	r0, #0
	bge.n	.L_020019c0
.L_02001962:
	movs	r0, #1
	add	sl, r0
	movs	r2, #128
	mov	r3, sl
	add	r8, r2
	adds	r5, #128
	cmp	r3, #63
	ble.n	.L_02001938
	ldr	r0, [r6, #0]
	movs	r3, #128
	str	r0, [sp, #20]
	lsls	r3, r3, #10
	ldr	r2, [r6, #8]
	adds	r0, r7, #0
	str	r2, [sp, #16]
	str	r3, [r7, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r7, #52]
	ldr	r3, [r6, #8]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	bl 0x0200a008
	adds	r0, r7, #0
	bl 0x0200a010
	ldr	r3, [sp, #24]
	cmp	fp, r3
	bne.n	.L_020019e6
.L_020019a0:
	movs	r0, #128
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x02009fa0
	ldr	r2, [sp, #4]
	add	r6, sp, #28
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl 0x0200a028
	mov	fp, r0
	cmp	r0, #255
	bne.n	.L_0200190a
.L_020019c0:
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #52]
	ldr	r2, [r7, #12]
	adds	r0, r7, #0
	ldr	r1, [sp, #20]
	ldr	r3, [sp, #16]
	bl 0x0200a008
	adds	r0, r7, #0
	bl 0x0200a010
	movs	r0, #2
	bl 0x02009f70
	b.n	.L_02001790
.L_020019e6:
	movs	r0, #10
	bl 0x02009f70
.L_020019ec:
	movs	r3, #0
	str	r3, [r7, #108]
	adds	r1, r7, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #52]
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x02009fe0
.L_02001a0a:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x96b1
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #80]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	sub	sp, #40
	bl 0x0200a138
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #56]
	adds	r7, r0, #0
	strh	r3, [r2, #0]
.L_02001a42:
	bl 0x0200973c
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #52]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	str	r3, [r7, #64]
	movs	r3, #0
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r1, [pc, #32]
	ldr	r3, [r7, #8]
	add	r2, sp, #28
	mov	sl, r2
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r1
	adds	r3, r3, r2
	mov	r0, sl
	str	r3, [sp, #12]
	str	r3, [r0, #0]
	b.n	.L_02001a88
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0x0200ace8
	.2byte 0x0000
	.2byte 0xfff0
.L_02001a88:
	.2byte 0x68fb
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	ands	r3, r1
	adds	r3, r3, r2
	str	r3, [sp, #8]
	str	r3, [r0, #8]
	ldr	r2, [sp, #12]
	str	r3, [sp, #16]
	adds	r3, r7, #0
	adds	r3, #34
	str	r2, [sp, #20]
	str	r3, [sp, #4]
	adds	r1, r2, #0
	ldrb	r0, [r3, #0]
	ldr	r2, [sp, #16]
	bl 0x0200a028
	str	r0, [sp, #24]
	ldr	r2, [sp, #20]
	ldr	r1, [r7, #8]
	ldr	r0, [sp, #16]
	ldr	r3, [pc, #60]
	ldr	r6, [r7, #16]
	subs	r1, r2, r1
	subs	r6, r0, r6
	mov	r8, r3
	adds	r0, r1, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c31
	adds	r5, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x182d
	movs	r0, #128
	lsls	r0, r0, #11
	cmp	r5, r0
	bge.n	.L_02001b04
	ldr	r3, [pc, #28]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #28]
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r2, [r1, r3]
	mov	r9, r2
	lsls	r3, r2, #16
	ldr	r2, [pc, #16]
	cmp	r3, r2
	bne.n	.L_02001b30
	b.n	.L_02001cfa
	.4byte 0x0300021c
	.4byte 0x03001150
	.4byte 0x0200a2b6
	.2byte 0x0000
	.2byte 0xffff
.L_02001b04:
	.2byte 0x9b04
	ldr	r2, [sp, #20]
	ldr	r0, [r7, #16]
	ldr	r1, [r7, #8]
	subs	r0, r3, r0
	subs	r1, r2, r1
	bl 0x02009f88
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	movs	r3, #128
	ldr	r2, [pc, #16]
	mov	r9, r0
	lsls	r3, r3, #6
	add	r3, r9
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	mov	r9, r3
	b.n	.L_02001b30
	.2byte 0xc000
	.2byte 0xffff
.L_02001b30:
	.2byte 0x2080
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x02009fa0
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldr	r1, [r2, #0]
	ldrb	r0, [r3, #0]
	ldr	r2, [r2, #8]
	bl 0x0200a028
	mov	fp, r0
	cmp	r0, #255
	beq.n	.L_02001baa
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldrb	r0, [r3, #0]
	ldr	r1, [r2, #0]
	ldr	r2, [r2, #8]
	bl 0x0200a020
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_02001baa
	ldr	r0, [sp, #12]
	mov	r2, sl
	str	r0, [r2, #0]
	ldr	r3, [sp, #8]
	adds	r0, r7, #0
	str	r3, [r2, #8]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	adds	r2, r7, #0
	str	r3, [r7, #52]
	adds	r2, #100
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r1, [sp, #12]
	ldr	r2, [r7, #12]
	ldr	r3, [sp, #8]
	bl 0x0200a008
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x02009fe0
	adds	r0, r7, #0
	movs	r1, #48
	bl 0x02009fe8
	movs	r5, #0
	b.n	.L_02001bd2
.L_02001baa:
	movs	r3, #0
	mov	r0, r9
	strh	r0, [r7, #6]
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r2, [sp, #12]
	str	r2, [r7, #8]
	ldr	r3, [sp, #8]
	str	r3, [r7, #16]
	b.n	.L_02001cfa
.L_02001bbe:
	ldr	r3, [pc, #360]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	beq.n	.L_02001bca
	b.n	.L_02001cfa
.L_02001bca:
	movs	r0, #1
	bl 0x02009f70
	adds	r5, #1
.L_02001bd2:
	cmp	r5, #179
	bgt.n	.L_02001be0
	adds	r0, r7, #0
	bl 0x0200a048
	cmp	r0, #0
	beq.n	.L_02001bbe
.L_02001be0:
	ldr	r3, [pc, #328]
	str	r3, [r7, #108]
	b.n	.L_02001cae
.L_02001be6:
	ldr	r2, [sp, #4]
	ldr	r1, [r6, #0]
	ldrb	r0, [r2, #0]
	ldr	r2, [r6, #8]
	bl 0x0200a020
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_02001cce
	ldr	r3, [pc, #296]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_02001cfa
	ldrh	r3, [r7, #32]
	movs	r2, #89
	subs	r3, #2
	str	r3, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #20]
	movs	r0, #0
	adds	r2, r2, r5
	mov	sl, r0
	mov	r8, r2
.L_02001c1e:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02001c48
	mov	r3, r8
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001c48
	cmp	r5, r7
	beq.n	.L_02001c48
	ldrh	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r0, #8
	subs	r3, #2
	ldr	r1, [sp, #0]
	add	r2, sp, #28
	bl 0x0200a050
	cmp	r0, #0
	bge.n	.L_02001cce
.L_02001c48:
	movs	r0, #1
	add	sl, r0
	movs	r2, #128
	mov	r3, sl
	add	r8, r2
	adds	r5, #128
	cmp	r3, #63
	ble.n	.L_02001c1e
	ldr	r0, [r6, #0]
	movs	r3, #128
	str	r0, [sp, #20]
	lsls	r3, r3, #10
	ldr	r2, [r6, #8]
	adds	r0, r7, #0
	str	r2, [sp, #16]
	str	r3, [r7, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r7, #52]
	movs	r5, #0
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	bl 0x0200a008
	b.n	.L_02001c86
.L_02001c7e:
	movs	r0, #1
	bl 0x02009f70
	adds	r5, #1
.L_02001c86:
	cmp	r5, #179
	bgt.n	.L_02001c9e
	adds	r0, r7, #0
	bl 0x0200a048
	cmp	r0, #0
	bne.n	.L_02001c9e
	ldr	r3, [pc, #144]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	beq.n	.L_02001c7e
.L_02001c9e:
	ldr	r3, [pc, #136]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_02001cfa
	ldr	r3, [sp, #24]
	cmp	fp, r3
	bne.n	.L_02001cf4
.L_02001cae:
	movs	r0, #128
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x02009fa0
	ldr	r2, [sp, #4]
	add	r6, sp, #28
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl 0x0200a028
	mov	fp, r0
	cmp	r0, #255
	bne.n	.L_02001be6
.L_02001cce:
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #52]
	ldr	r2, [r7, #12]
	adds	r0, r7, #0
	ldr	r1, [sp, #20]
	ldr	r3, [sp, #16]
	bl 0x0200a008
	adds	r0, r7, #0
	bl 0x0200a010
	movs	r0, #2
	bl 0x02009f70
	b.n	.L_02001a42
.L_02001cf4:
	movs	r0, #10
	bl 0x02009f70
.L_02001cfa:
	movs	r3, #0
	str	r3, [r7, #108]
	adds	r1, r7, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #52]
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x02009fe0
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
.L_02001d1e:
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200ace8
	.4byte 0x020096b1
	.4byte 0x80184b01
	.4byte 0x00004770
	.2byte 0xace8
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #92]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	sub	sp, #32
	bl 0x0200a138
	adds	r5, r0, #0
	ldrh	r3, [r5, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #60]
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	str	r3, [sp, #16]
.L_02001d6e:
	bl 0x0200973c
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #52]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r5, #56]
	str	r3, [r5, #64]
	movs	r3, #0
	str	r3, [r5, #36]
	str	r3, [r5, #44]
	ldr	r2, [pc, #28]
	ldr	r3, [r5, #8]
	movs	r1, #128
	lsls	r1, r1, #12
	ands	r3, r2
	mov	r9, r1
	add	r6, sp, #20
	add	r3, r9
	str	r3, [r6, #0]
	mov	r8, r3
	b.n	.L_02001db0
	.2byte 0x0000
	.4byte 0xffffc000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff0
.L_02001db0:
	.2byte 0x68eb
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	ands	r3, r2
	adds	r7, r3, r1
	mov	r2, r8
	str	r7, [r6, #8]
	str	r2, [sp, #8]
	str	r7, [sp, #4]
	movs	r3, #34
	adds	r3, r3, r5
	ldrb	r0, [r3, #0]
	adds	r1, r2, #0
	adds	r2, r7, #0
	mov	fp, r3
	bl 0x0200a028
	str	r0, [sp, #12]
	movs	r0, #128
	ldr	r1, [sp, #16]
	lsls	r0, r0, #13
	adds	r2, r6, #0
	bl 0x02009fa0
	mov	r1, fp
	ldrb	r0, [r1, #0]
	ldr	r2, [r6, #8]
	ldr	r1, [r6, #0]
	bl 0x0200a028
	mov	sl, r0
	cmp	r0, #255
	beq.n	.L_02001e44
	mov	r2, fp
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl 0x0200a020
	ldr	r3, [r5, #12]
	subs	r0, r0, r3
	cmp	r0, r9
	bgt.n	.L_02001e44
	ldr	r3, [sp, #8]
	ldr	r2, [pc, #48]
	str	r3, [r6, #0]
	ldr	r1, [sp, #4]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r1, [r6, #8]
	str	r3, [r5, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r5, #52]
	adds	r3, r5, #0
	adds	r3, #100
	strh	r2, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x02009fe0
	adds	r0, r5, #0
	movs	r1, #48
	bl 0x02009fe8
	ldr	r3, [pc, #8]
	str	r3, [r5, #108]
	b.n	.L_02001eee
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0x96b1
	.2byte 0x0200
.L_02001e44:
	add	r1, sp, #16
	ldrh	r1, [r1, #0]
	movs	r3, #0
	mov	r2, r8
	strh	r1, [r5, #6]
	str	r3, [r5, #36]
	str	r3, [r5, #44]
	str	r2, [r5, #8]
	str	r7, [r5, #16]
	b.n	.L_02001f3a
.L_02001e58:
	mov	r3, fp
	ldrb	r0, [r3, #0]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
	bl 0x0200a020
	ldr	r3, [r5, #12]
	movs	r1, #128
	subs	r0, r0, r3
	lsls	r1, r1, #12
	cmp	r0, r1
	bgt.n	.L_02001f0e
	ldrh	r3, [r5, #32]
	movs	r2, #0
	subs	r3, #2
	str	r3, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #20]
	movs	r3, #89
	adds	r3, r3, r6
	mov	r9, r2
	mov	r8, r3
.L_02001e86:
	ldr	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_02001eb0
	mov	r1, r8
	ldrb	r2, [r1, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001eb0
	cmp	r6, r5
	beq.n	.L_02001eb0
	ldrh	r3, [r6, #32]
	adds	r0, r6, #0
	adds	r0, #8
	subs	r3, #2
	ldr	r1, [sp, #0]
	add	r2, sp, #20
	bl 0x0200a050
	cmp	r0, #0
	bge.n	.L_02001f0e
.L_02001eb0:
	movs	r2, #1
	add	r9, r2
	movs	r3, #128
	mov	r1, r9
	add	r8, r3
	adds	r6, #128
	cmp	r1, #63
	ble.n	.L_02001e86
	ldr	r2, [r7, #0]
	adds	r0, r5, #0
	str	r2, [sp, #8]
	ldr	r3, [r7, #8]
	str	r3, [sp, #4]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r5, #52]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	bl 0x0200a008
	adds	r0, r5, #0
	bl 0x0200a010
	ldr	r1, [sp, #12]
	cmp	sl, r1
	bne.n	.L_02001f34
.L_02001eee:
	movs	r0, #128
	ldr	r1, [sp, #16]
	add	r2, sp, #20
	lsls	r0, r0, #13
	bl 0x02009fa0
	mov	r2, fp
	add	r7, sp, #20
	ldrb	r0, [r2, #0]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
	bl 0x0200a028
	mov	sl, r0
	cmp	r0, #255
	bne.n	.L_02001e58
.L_02001f0e:
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #52]
	ldr	r2, [r5, #12]
	adds	r0, r5, #0
	ldr	r1, [sp, #8]
	ldr	r3, [sp, #4]
	bl 0x0200a008
	adds	r0, r5, #0
	bl 0x0200a010
	movs	r0, #2
	bl 0x02009f70
	b.n	.L_02001d6e
.L_02001f34:
	movs	r0, #10
	bl 0x02009f70
.L_02001f3a:
	movs	r3, #0
	str	r3, [r5, #108]
	adds	r1, r5, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r5, #52]
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x02009fe0
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.irp EntryTarget, 0x03000528, 0x080000c1, 0x080000d1, 0x080000f9, 0x08000101, 0x08000119, 0x08000121, 0x08000129, 0x08000131, 0x08000141, 0x08000151, 0x080001c9, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020091, 0x08020099, 0x080200a9, 0x080200c1, 0x080200c9, 0x08020149, 0x08020151, 0x08020179, 0x080201c1, 0x080201c9, 0x080201e9, 0x08020219, 0x08020221, 0x080202f9, 0x08020349, 0x08038249, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8061, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80c9, 0x080c80e1, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8139, 0x080c8171, 0x080c8181, 0x080c8189, 0x080c81a1, 0x080c81d1, 0x080c8229, 0x080c8239, 0x080c8279, 0x080c82f9, 0x080c83b1, 0x080c83b9, 0x080c83e1, 0x080c8459, 0x080c8481, 0x080c84d9, 0x080c84e1, 0x080c8581, 0x080c85f9, 0x080c8689, 0x080c86a9, 0x080c86c1, 0x080c86e9, 0x080c8719, 0x080c8721, 0x080c8729, 0x080c8761, 0x080c8779, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
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
	.4byte 0x000a0009
	.4byte 0x000affff
	.4byte 0xffffffff
	.4byte 0x80000000
	.4byte 0xc000ffff
	.4byte 0xa000e000
	.4byte 0x4000c000
	.4byte 0x60002000
	.4byte 0xffff4000
	.4byte 0x80000000
	.4byte 0xffffffff
	.4byte 0x80000000
	.4byte 0xc000ffff
	.4byte 0xffffffff
	.4byte 0x4000c000
	.4byte 0xffffffff
	.4byte 0xffff4000
	.4byte 0x80000000
	.4byte 0xffffffff
	.4byte 0x80000000
	.4byte 0xc000ffff
	.4byte 0x80000000
	.4byte 0x4000c000
	.4byte 0x80000000
	.4byte 0xffff4000
	.4byte 0x80000000
	.4byte 0x0000ffff
	.4byte 0x0200a1b8
	.4byte 0x0200a1f4
	.4byte 0x0200a230
	.4byte 0x00000027
	.4byte 0x00000003
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xfff00000
	.4byte 0x0000002e
	.4byte 0x020082b1
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x020082bd
	.4byte 0x00000011
	.4byte 0xffff0000
	.4byte 0x000000d8
	.4byte 0x400000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0063
	.4byte 0x00000140
	.4byte 0xc000033e
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x002e02a0
	.4byte 0x02b00190
	.4byte 0x01a0003e
	.4byte 0x000effff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000c00d4
	.4byte 0x00dc0084
	.4byte 0x008c0014
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000d5
	.4byte 0x10130002
	.4byte 0xffffffff
	.4byte 0x1020b0d6
	.4byte 0xffffffff
	.4byte 0x103010d7
	.4byte 0xffffffff
	.4byte 0x104010d7
	.4byte 0xffffffff
	.4byte 0x000000d6
	.4byte 0x101020d5
	.4byte 0xffffffff
	.4byte 0x000000d7
	.4byte 0x101030d5
	.4byte 0xffffffff
	.4byte 0x102040d8
	.4byte 0xffffffff
	.4byte 0x000000d8
	.4byte 0x10131002
	.4byte 0xffffffff
	.4byte 0x1020b0d9
	.4byte 0xffffffff
	.4byte 0x1030e0d9
	.4byte 0xffffffff
	.4byte 0x1040e0d9
	.4byte 0xffffffff
	.4byte 0x1050e0d9
	.4byte 0xffffffff
	.4byte 0x106020da
	.4byte 0xffffffff
	.4byte 0x000000da
	.4byte 0x102040d8
	.4byte 0xffffffff
	.4byte 0x000000d9
	.4byte 0x10b020d8
	.4byte 0xffffffff
	.4byte 0x10c0d0d9
	.4byte 0xffffffff
	.4byte 0x10d0c0d9
	.4byte 0xffffffff
	.4byte 0x10e030d8
	.4byte 0xffffffff
	.4byte 0x000000db
	.4byte 0x10132002
	.4byte 0xffffffff
	.4byte 0x102010dc
	.4byte 0xffffffff
	.4byte 0x000000dc
	.4byte 0x101020db
	.4byte 0xffffffff
	.4byte 0x000000dd
	.4byte 0x1012f002
	.4byte 0xffffffff
	.4byte 0x1020b0de
	.4byte 0xffffffff
	.4byte 0x000000de
	.4byte 0x101020dd
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x0000c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x0001c000
	.4byte 0xffff006a
	.4byte 0x00000002
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x0001c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x00000002
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0001c000
	.4byte 0xffff012b
	.4byte 0x00000007
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x0002c000
	.4byte 0xffff011f
	.4byte 0x0200a31c
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0x006600f5
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00008000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00014000
	.4byte 0xffff006a
	.4byte 0x00000003
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff005d
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00014000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00014000
	.4byte 0xffff0120
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00014000
	.4byte 0xffff0112
	.4byte 0x0200a2e4
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00014000
	.4byte 0xffff0112
	.4byte 0x0200a2e4
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01014000
	.4byte 0xffff0112
	.4byte 0x0200a2e4
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01014000
	.4byte 0xffff0112
	.4byte 0x0200a2e4
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01014000
	.4byte 0xffff0112
	.4byte 0x0200a2e4
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01014000
	.4byte 0xffff0112
	.4byte 0x0200a2e4
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01014000
	.4byte 0xffff0112
	.4byte 0x0200a2e4
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x01014000
	.4byte 0xffff0112
	.4byte 0x0200a2e4
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00014000
	.4byte 0xffff0112
	.4byte 0x0200a2e4
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01014000
	.4byte 0xffff0112
	.4byte 0x0200a2e4
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00014000
	.4byte 0xffff0112
	.4byte 0x0200a2e4
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00014000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x005c0000
	.4byte 0x00014000
	.4byte 0x007f00f6
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00008000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff005d
	.4byte 0x00000001
	.4byte 0x00600000
	.4byte 0x00000000
	.4byte 0x03300000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0002c000
	.4byte 0x005200f4
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00008000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00014000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001c000
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
	.4byte 0xffff000b
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x00000009
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00004602
	.4byte 0xffff0003
	.4byte 0x02008625
	.4byte 0x00000602
	.4byte 0xffff0004
	.4byte 0x020085cd
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0200843d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000028b2
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000028b3
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000028b5
	.4byte 0x50008905
	.4byte 0xffff000a
	.4byte 0x02008489
	.4byte 0x50008905
	.4byte 0xffff000b
	.4byte 0x020084a9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000028b1
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000028b4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008c45
	.4byte 0x00004602
	.4byte 0xffff0003
	.4byte 0x02008625
	.4byte 0x00000602
	.4byte 0xffff0004
	.4byte 0x020085cd
	.4byte 0x00008602
	.4byte 0xffff0005
	.4byte 0x020085f5
	.4byte 0x0000c401
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000002
	.4byte 0x02200022
	.4byte 0x0200925d
	.4byte 0x00000002
	.4byte 0x12200023
	.4byte 0x020092c1
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000028b6
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000028b9
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008291
	.4byte 0x10008c15
	.4byte 0xffff000a
	.4byte 0x02009305
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x0200955d
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02009305
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x0200955d
	.4byte 0x00000c15
	.4byte 0x02300009
	.4byte 0x02008645
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000021
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000031
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000028b7
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000028b8
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000028ba
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000028bb
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008c45
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x02008c1d
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x02008c2d
	.4byte 0x00000002
	.4byte 0x02010014
	.4byte 0x02008c3d
	.4byte 0x00008515
	.4byte 0x02030008
	.4byte 0x02008cfd
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008281
	.4byte 0x00009415
	.4byte 0x0f5d000b
	.4byte 0x02008405
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000028bc
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000028bd
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000028be
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000028bf
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x0040305e
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c401
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c401
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000028c1
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x020082a1
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000028c3
	.4byte 0x10008c15
	.4byte 0x02040009
	.4byte 0x02008ef1
	.4byte 0x00008c15
	.4byte 0x02040009
	.4byte 0x020091bd
	.4byte 0x10008c15
	.4byte 0x0205000a
	.4byte 0x02008ef1
	.4byte 0x00008c15
	.4byte 0x0205000a
	.4byte 0x020091c9
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008f01
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x020091d5
	.4byte 0x00008f15
	.4byte 0x02080016
	.4byte 0x020096a5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000028c0
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000028c2
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
