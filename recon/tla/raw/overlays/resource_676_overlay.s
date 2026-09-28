.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x0200a279, 0x02008205, 0x02008211, 0x02008219, 0x020084ed, 0x0200820d, 0x0200a2b1
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x23806d02
	.4byte 0x8253021b
	.2byte 0x2000
	.2byte 0x4770
	push	{lr}
	movs	r1, #0
	bl 0x0200a478
	movs	r0, #0
	pop	{pc}
	push	{r5, lr}
	ldmia	r0!, {r5}
	ldmia	r1!, {r3}
	ldmia	r0!, {r4}
	subs	r5, r5, r3
	ldmia	r1!, {r3}
	asrs	r5, r5, #16
	ldr	r2, [r1, #0]
	subs	r4, r4, r3
	ldr	r3, [r0, #0]
	asrs	r4, r4, #16
	subs	r3, r3, r2
	asrs	r3, r3, #16
	adds	r0, r5, #0
	muls	r0, r5
	adds	r2, r4, #0
	muls	r2, r4
	adds	r1, r3, #0
	muls	r1, r3
	adds	r0, r0, r2
	adds	r3, r1, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #4]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd20
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	mov	fp, r3
	adds	r3, r5, #0
	adds	r3, #99
	ldrb	r3, [r3, #0]
	mov	sl, r2
	movs	r2, #0
	mov	r8, r1
	mov	r9, r2
	cmp	r3, #0
	beq.n	.L_020000b8
	adds	r2, r5, #0
	adds	r2, #91
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r0, #1
	b.n	.L_0200013a
.L_020000b8:
	mov	r6, r8
	adds	r7, r5, #0
	adds	r6, #8
	adds	r7, #8
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl 0x02008050
	cmp	r0, sl
	blt.n	.L_020000d2
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L_02000128
.L_020000d2:
	mov	r2, r8
	ldr	r0, [r2, #16]
	ldr	r3, [r5, #16]
	ldr	r1, [r6, #0]
	subs	r0, r0, r3
	ldr	r3, [r7, #0]
	subs	r1, r1, r3
	bl 0x0200a430
	ldr	r3, [pc, #96]
	lsls	r0, r0, #16
	movs	r2, #128
	lsrs	r0, r0, #16
	lsls	r2, r2, #5
	adds	r1, r0, r2
	ldrh	r2, [r5, #6]
	adds	r4, r0, r3
	movs	r3, #240
	lsls	r3, r3, #8
	ands	r4, r3
	ands	r1, r3
	ands	r0, r3
	ands	r3, r2
	cmp	r0, r3
	beq.n	.L_02000112
	cmp	r1, r3
	beq.n	.L_02000112
	cmp	r4, r3
	beq.n	.L_02000112
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L_02000138
.L_02000112:
	adds	r2, r5, #0
	adds	r2, #91
.L_02000116:
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200a448
	movs	r2, #1
	mov	r9, r2
	b.n	.L_02000138
.L_02000128:
	adds	r3, r5, #0
	adds	r3, #91
	mov	r2, r9
	strb	r2, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200a448
.L_02000138:
	mov	r0, r9
.L_0200013a:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xf000
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #60]
	ldr	r3, [r3, #108]
	mov	sl, r2
	mov	r8, r3
	ldr	r3, [pc, #156]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200a4a0
	adds	r1, r5, #0
	adds	r0, #8
	adds	r1, #8
	movs	r7, #0
	bl 0x02008050
	cmp	r0, #11
	bgt.n	.L_02000190
	adds	r3, r5, #0
	adds	r3, #91
	adds	r0, r5, #0
	strb	r7, [r3, #0]
	movs	r1, #2
	bl 0x0200a448
	b.n	.L_020001f4
.L_02000190:
	adds	r6, r5, #0
	adds	r6, #100
	ldrh	r2, [r6, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020001a2
	movs	r0, #10
	b.n	.L_020001a4
.L_020001a2:
	movs	r0, #9
.L_020001a4:
	bl 0x0200a4a0
	adds	r1, r0, #0
	adds	r0, r5, #0
	movs	r2, #24
	movs	r3, #0
	bl 0x02008088
	cmp	r0, #0
	bne.n	.L_020001f4
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200a4a0
	movs	r3, #176
	lsls	r3, r3, #1
.L_020001ca:
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	adds	r1, r0, #0
	cmp	r3, #0
	bne.n	.L_020001de
	mov	r2, sl
	ldrb	r3, [r2, #4]
	cmp	r3, #0
	beq.n	.L_020001ea
.L_020001de:
	ldrh	r2, [r6, #0]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020001ea
	movs	r7, #1
.L_020001ea:
	adds	r0, r5, #0
	movs	r2, #24
	adds	r3, r7, #0
	bl 0x02008088
.L_020001f4:
	movs	r0, #0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xaa48
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xaa78
	.2byte 0x0200
	push	{lr}
	ldr	r1, [pc, #60]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_0200023e
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #15
	bne.n	.L_0200023e
.L_0200023a:
	ldr	r0, [pc, #36]
	b.n	.L_02000254
.L_0200023e:
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_02000252
	ldr	r0, [pc, #24]
	b.n	.L_02000254
.L_02000252:
	ldr	r0, [pc, #24]
.L_02000254:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000008b
	.4byte 0x0200abfc
	.4byte 0x0000008c
	.4byte 0x0200abe4
	.2byte 0xaac4
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #108]
	bl 0x0200a488
	movs	r0, #0
	bl 0x0200a590
	movs	r5, #8
.L_02000284:
	adds	r0, r5, #0
	bl 0x0200a4a0
	cmp	r0, #0
	beq.n	.L_02000296
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
.L_02000296:
	adds	r5, #1
	cmp	r5, #63
	bls.n	.L_02000284
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	movs	r0, #158
	bl 0x0200a5a0
	subs	r6, #1
	ldr	r0, [pc, #112]
	lsls	r4, r6, #3
	adds	r3, r4, #4
	ldrh	r1, [r0, r3]
	adds	r3, r3, r0
	ldrh	r2, [r3, #2]
	ldr	r0, [r0, r4]
	bl 0x0200a460
	ldr	r5, [pc, #96]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200a500
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200a4a8
	ldr	r0, [r5, #0]
	cmp	r6, #7
	bne.n	.L_020002f4
	movs	r2, #4
	movs	r1, #0
	negs	r2, r2
	bl 0x0200a4e8
	b.n	.L_020002fe
.L_020002f4:
	movs	r2, #4
	movs	r1, #2
	negs	r2, r2
	bl 0x0200a4e0
.L_020002fe:
	movs	r0, #10
	bl 0x0200a480
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200a578
	bl 0x0200a580
	bl 0x0200a588
	bl 0x0200a490
	pop	{r5, r6, r7, pc}
	.4byte 0x0200ace4
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	sub	sp, #12
	movs	r3, #67
	str	r3, [sp, #0]
	movs	r5, #1
	movs	r6, #5
	movs	r0, #67
	movs	r1, #25
	movs	r2, #5
	movs	r3, #5
	str	r6, [sp, #4]
	str	r5, [sp, #8]
	bl 0x0200a598
	movs	r0, #23
	movs	r1, #29
	movs	r2, #5
	movs	r3, #6
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200a468
	movs	r3, #8
	str	r3, [sp, #4]
	movs	r0, #23
	movs	r1, #29
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	bl 0x0200a470
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #12
	movs	r3, #79
	movs	r2, #22
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #67
	movs	r1, #25
	movs	r2, #2
	movs	r3, #1
	bl 0x0200a598
	add	sp, #12
	pop	{pc}
	push	{r5, r6, lr}
	ldr	r3, [pc, #136]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #12
	bl 0x0200a4a0
	movs	r3, #26
	movs	r2, #76
	movs	r1, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	adds	r5, r0, #0
	movs	r1, #95
	movs	r0, #26
	movs	r2, #2
	movs	r3, #1
	bl 0x0200a598
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #98
	bl 0x0200a438
	cmp	r0, #0
	bne.n	.L_02000412
	movs	r1, #212
	movs	r2, #232
	movs	r0, #66
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200a4f8
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #26
	bne.n	.L_02000412
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #14
	bne.n	.L_02000412
	bl 0x0200a488
	movs	r0, #0
	bl 0x0200a590
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x0200a550
	ldr	r0, [r6, #0]
	movs	r1, #16
	movs	r2, #0
	bl 0x0200a4e8
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r0, [r6, #0]
	bl 0x0200a4f0
	bl 0x0200a490
.L_02000412:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	sub	sp, #12
	movs	r3, #106
	movs	r2, #20
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #67
	movs	r1, #25
	movs	r2, #3
	movs	r3, #3
	bl 0x0200a598
	add	sp, #12
	pop	{pc}
	push	{r5, r6, lr}
	ldr	r3, [pc, #136]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #12
	bl 0x0200a4a0
	movs	r3, #106
	movs	r2, #20
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	adds	r5, r0, #0
	movs	r1, #25
	movs	r0, #67
	movs	r2, #3
	movs	r3, #3
	bl 0x0200a598
	movs	r0, #246
	lsls	r0, r0, #4
	bl 0x0200a438
	cmp	r0, #0
	bne.n	.L_020004c2
	movs	r1, #170
	movs	r2, #172
	movs	r0, #64
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200a4f8
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #42
	bne.n	.L_020004c2
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #21
	bne.n	.L_020004c2
	bl 0x0200a488
	movs	r0, #0
	bl 0x0200a590
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x0200a550
	movs	r2, #16
	ldr	r0, [r6, #0]
	movs	r1, #0
	negs	r2, r2
	bl 0x0200a4e8
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r0, [r6, #0]
	bl 0x0200a4f0
	bl 0x0200a490
.L_020004c2:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	sub	sp, #12
	movs	r3, #87
	movs	r2, #8
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #67
	movs	r1, #25
	movs	r2, #2
	movs	r3, #2
	bl 0x0200a598
	add	sp, #12
	pop	{pc}
	push	{lr}
	ldr	r1, [pc, #60]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_02000512
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #15
	bne.n	.L_02000512
	ldr	r0, [pc, #36]
	b.n	.L_02000528
.L_02000512:
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_02000526
	ldr	r0, [pc, #24]
	b.n	.L_02000528
.L_02000526:
	ldr	r0, [pc, #24]
.L_02000528:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000008b
	.4byte 0x0200af10
	.4byte 0x0000008c
	.4byte 0x0200aeec
	.2byte 0xad24
	.2byte 0x0200
	push	{lr}
	bl 0x0200a488
	movs	r0, #0
	bl 0x0200a590
	ldr	r0, [pc, #44]
	bl 0x0200a520
	movs	r1, #0
	movs	r0, #8
	bl 0x0200a528
	movs	r0, #0
	movs	r1, #0
	bl 0x0200a498
	cmp	r0, #0
	bne.n	.L_02000570
	bl 0x02009944
	b.n	.L_02000578
.L_02000570:
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a538
.L_02000578:
	bl 0x0200a490
	pop	{pc}
	.2byte 0x0000
	.2byte 0x223f
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #9
	ldr	r7, [r3, #108]
	bl 0x0200a4a0
	adds	r5, r0, #0
	movs	r3, #6
	ldrsh	r2, [r5, r3]
	adds	r6, r5, #0
	mov	r8, r2
	bl 0x0200a488
	movs	r0, #0
	bl 0x0200a590
	movs	r2, #179
	movs	r3, #1
	lsls	r2, r2, #1
	adds	r6, #99
	strb	r3, [r6, #0]
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_020005d8
	ldr	r3, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r0, #9
	ldr	r1, [r3, #0]
	movs	r2, #0
	bl 0x0200a518
	ldr	r0, [pc, #56]
	bl 0x0200a520
	b.n	.L_020005de
.L_020005d8:
	ldr	r0, [pc, #52]
	bl 0x0200a520
.L_020005de:
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a538
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_020005f8
	mov	r3, r8
	strh	r3, [r5, #6]
.L_020005f8:
	movs	r3, #0
	strb	r3, [r6, #0]
	bl 0x0200a490
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00002241
	.2byte 0x2245
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #10
	ldr	r7, [r3, #108]
	bl 0x0200a4a0
	adds	r5, r0, #0
	movs	r3, #6
	ldrsh	r2, [r5, r3]
	adds	r6, r5, #0
	mov	r8, r2
	bl 0x0200a488
	movs	r0, #0
	bl 0x0200a590
	movs	r2, #179
	movs	r3, #1
	lsls	r2, r2, #1
	adds	r6, #99
	strb	r3, [r6, #0]
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02000668
	ldr	r3, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r0, #10
	ldr	r1, [r3, #0]
	movs	r2, #0
	bl 0x0200a518
	ldr	r0, [pc, #56]
	bl 0x0200a520
	b.n	.L_0200066e
.L_02000668:
	ldr	r0, [pc, #52]
	bl 0x0200a520
.L_0200066e:
	movs	r0, #10
	movs	r1, #0
	bl 0x0200a538
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02000688
	mov	r3, r8
	strh	r3, [r5, #6]
.L_02000688:
	movs	r3, #0
	strb	r3, [r6, #0]
	bl 0x0200a490
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00002242
	.2byte 0x2246
	.2byte 0x0000
	push	{lr}
	ldr	r2, [r0, #56]
	movs	r3, #128
	lsls	r3, r3, #24
	cmp	r2, r3
	bne.n	.L_020006b8
	ldr	r3, [r0, #64]
	movs	r0, #1
	cmp	r3, r2
	beq.n	.L_020006ba
.L_020006b8:
	movs	r0, #0
.L_020006ba:
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	bl 0x0200a4a0
	movs	r2, #98
	adds	r5, r0, #0
	adds	r2, r2, r5
	ldrb	r3, [r2, #0]
	mov	r8, r2
	cmp	r3, #1
	bne.n	.L_020006da
	bl 0x0200967e
.L_020006da:
	adds	r7, r5, #0
	adds	r7, #100
	cmp	r3, #1
	bgt.n	.L_020006e8
	cmp	r3, #0
	beq.n	.L_020006f2
	b.n	.L_02000724
.L_020006e8:
	cmp	r3, #2
	beq.n	.L_02000704
	cmp	r3, #3
	beq.n	.L_020006fc
	b.n	.L_02000724
.L_020006f2:
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200a448
	b.n	.L_02000724
.L_020006fc:
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200a448
.L_02000704:
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	ldr	r1, [r5, #8]
	adds	r0, r5, #0
	bl 0x0200a450
	ldrh	r2, [r7, #0]
	movs	r3, #2
	negs	r3, r3
	ands	r3, r2
	strh	r3, [r7, #0]
	mov	r2, r8
	movs	r3, #1
	strb	r3, [r2, #0]
	bl 0x0200967e
.L_02000724:
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	movs	r2, #162
	lsls	r2, r2, #1
	cmp	r3, r2
	bcc.n	.L_02000734
	bl 0x0200967e
.L_02000734:
	ldr	r2, [pc, #4]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	b.n	.L_02000740
	.2byte 0x8744
	.2byte 0x0200
.L_02000740:
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x02008c54
	.4byte 0x0200966e
	.4byte 0x02008c66
	.4byte 0x0200966e
	.4byte 0x02008c78
	.4byte 0x0200966e
	.4byte 0x02008c8a
	.4byte 0x0200966e
	.4byte 0x02008c9c
	.4byte 0x0200966e
	.4byte 0x02008cae
	.4byte 0x0200966e
	.4byte 0x02008cc0
	.4byte 0x0200966e
	.4byte 0x02008cd2
	.4byte 0x0200966e
	.4byte 0x02008ce4
	.4byte 0x0200966e
	.4byte 0x02008cf6
	.4byte 0x0200966e
	.4byte 0x02008d08
	.4byte 0x0200966e
	.4byte 0x02008d1a
	.4byte 0x0200966e
	.4byte 0x02008d2c
	.4byte 0x0200966e
	.4byte 0x02008d3e
	.4byte 0x0200966e
	.4byte 0x02008d50
	.4byte 0x0200966e
	.4byte 0x02008d62
	.4byte 0x0200966e
	.4byte 0x02008d74
	.4byte 0x0200966e
	.4byte 0x02008d86
	.4byte 0x0200966e
	.4byte 0x02008daa
	.4byte 0x0200966e
	.4byte 0x02008dbc
	.4byte 0x0200966e
	.4byte 0x02008dce
	.4byte 0x0200966e
	.4byte 0x02008de0
	.4byte 0x0200966e
	.4byte 0x02008dfc
	.4byte 0x0200966e
	.4byte 0x02008e0e
	.4byte 0x0200966e
	.4byte 0x02008e20
	.4byte 0x0200966e
	.4byte 0x02008e32
	.4byte 0x0200966e
	.4byte 0x02008e44
	.4byte 0x0200966e
	.4byte 0x02008e56
	.4byte 0x0200966e
	.4byte 0x02008e68
	.4byte 0x0200966e
	.4byte 0x02008e7a
	.4byte 0x0200966e
	.4byte 0x02008e8a
	.4byte 0x0200966e
	.4byte 0x02008e9a
	.4byte 0x0200966e
	.4byte 0x02008eaa
	.4byte 0x0200966e
	.4byte 0x02008eba
	.4byte 0x0200966e
	.4byte 0x02008eca
	.4byte 0x0200966e
	.4byte 0x02008eda
	.4byte 0x0200966e
	.4byte 0x02008eea
	.4byte 0x0200966e
	.4byte 0x02008efa
	.4byte 0x0200966e
	.4byte 0x02008f0a
	.4byte 0x0200966e
	.4byte 0x02008f1a
	.4byte 0x0200966e
	.4byte 0x02008f2a
	.4byte 0x0200966e
	.4byte 0x02008f3a
	.4byte 0x0200966e
	.4byte 0x02008f4a
	.4byte 0x0200966e
	.4byte 0x02008f5a
	.4byte 0x0200966e
	.4byte 0x02008f6a
	.4byte 0x0200966e
	.4byte 0x02008f7a
	.4byte 0x0200966e
	.4byte 0x02008f8a
	.4byte 0x0200966e
	.4byte 0x02008f9a
	.4byte 0x0200966e
	.4byte 0x02008faa
	.4byte 0x0200966e
	.4byte 0x02008fba
	.4byte 0x0200966e
	.4byte 0x02008fca
	.4byte 0x0200966e
	.4byte 0x02008fda
	.4byte 0x0200966e
	.4byte 0x02008fea
	.4byte 0x0200966e
	.4byte 0x02008ffa
	.4byte 0x0200966e
	.4byte 0x0200900a
	.4byte 0x0200966e
	.4byte 0x0200901a
	.4byte 0x0200966e
	.4byte 0x0200902a
	.4byte 0x0200966e
	.4byte 0x0200903a
	.4byte 0x0200966e
	.4byte 0x0200904a
	.4byte 0x0200966e
	.4byte 0x0200905a
	.4byte 0x0200966e
	.4byte 0x0200906a
	.4byte 0x0200966e
	.4byte 0x0200907a
	.4byte 0x0200966e
	.4byte 0x0200908a
	.4byte 0x0200966e
	.4byte 0x0200909a
	.4byte 0x0200966e
	.4byte 0x020090aa
	.4byte 0x0200966e
	.4byte 0x020090ba
	.4byte 0x0200966e
	.4byte 0x020090ca
	.4byte 0x0200966e
	.4byte 0x020090da
	.4byte 0x0200966e
	.4byte 0x020090ea
	.4byte 0x0200966e
	.4byte 0x020090fa
	.4byte 0x0200966e
	.4byte 0x0200910a
	.4byte 0x0200966e
	.4byte 0x0200911a
	.4byte 0x0200966e
	.4byte 0x0200912a
	.4byte 0x0200966e
	.4byte 0x0200913a
	.4byte 0x0200966e
	.4byte 0x0200914a
	.4byte 0x0200966e
	.4byte 0x0200915a
	.4byte 0x0200966e
	.4byte 0x0200916a
	.4byte 0x0200966e
	.4byte 0x02009180
	.4byte 0x0200966e
	.4byte 0x02009190
	.4byte 0x0200966e
	.4byte 0x0200919e
	.4byte 0x0200966e
	.4byte 0x020091ac
	.4byte 0x0200966e
	.4byte 0x020091ba
	.4byte 0x0200966e
	.4byte 0x020091c8
	.4byte 0x0200966e
	.4byte 0x020091d6
	.4byte 0x0200966e
	.4byte 0x020091e6
	.4byte 0x0200966e
	.4byte 0x020091f6
	.4byte 0x0200966e
	.4byte 0x02009206
	.4byte 0x0200966e
	.4byte 0x02009214
	.4byte 0x0200966e
	.4byte 0x02009222
	.4byte 0x0200966e
	.4byte 0x02009230
	.4byte 0x0200966e
	.4byte 0x0200923e
	.4byte 0x0200966e
	.4byte 0x0200924c
	.4byte 0x0200966e
	.4byte 0x0200925c
	.4byte 0x0200966e
	.4byte 0x0200926c
	.4byte 0x0200966e
	.4byte 0x0200927c
	.4byte 0x0200966e
	.4byte 0x0200928a
	.4byte 0x0200966e
	.4byte 0x02009298
	.4byte 0x0200966e
	.4byte 0x020092a6
	.4byte 0x0200966e
	.4byte 0x020092b4
	.4byte 0x0200966e
	.4byte 0x020092c2
	.4byte 0x0200966e
	.4byte 0x020092d2
	.4byte 0x0200966e
	.4byte 0x020092e2
	.4byte 0x0200966e
	.4byte 0x020092f2
	.4byte 0x0200966e
	.4byte 0x02009300
	.4byte 0x0200966e
	.4byte 0x0200930e
	.4byte 0x0200966e
	.4byte 0x0200931c
	.4byte 0x0200966e
	.4byte 0x0200932a
	.4byte 0x0200966e
	.4byte 0x02009338
	.4byte 0x0200966e
	.4byte 0x02009346
	.4byte 0x0200966e
	.4byte 0x02009354
	.4byte 0x0200966e
	.4byte 0x02009362
	.4byte 0x0200966e
	.4byte 0x02009370
	.4byte 0x020093e6
	.4byte 0x020093f6
	.4byte 0x0200966e
	.4byte 0x0200937e
	.4byte 0x0200966e
	.4byte 0x0200938e
	.4byte 0x0200966e
	.4byte 0x0200939e
	.4byte 0x0200966e
	.4byte 0x020093ae
	.4byte 0x0200966e
	.4byte 0x020093bc
	.4byte 0x0200966e
	.4byte 0x020093ca
	.4byte 0x0200966e
	.4byte 0x020093d8
	.4byte 0x020093e6
	.4byte 0x020093f6
	.4byte 0x0200966e
	.4byte 0x0200940e
	.4byte 0x0200966e
	.4byte 0x0200941e
	.4byte 0x0200966e
	.4byte 0x0200942e
	.4byte 0x0200966e
	.4byte 0x0200943e
	.4byte 0x0200966e
	.4byte 0x0200944c
	.4byte 0x0200966e
	.4byte 0x0200945a
	.4byte 0x0200966e
	.4byte 0x02009468
	.4byte 0x0200966e
	.4byte 0x02009488
	.4byte 0x0200966e
	.4byte 0x02009496
	.4byte 0x0200966e
	.4byte 0x020094ae
	.4byte 0x0200966e
	.4byte 0x020094bc
	.4byte 0x0200966e
	.4byte 0x020094ca
	.4byte 0x0200966e
	.4byte 0x020094d8
	.4byte 0x0200966e
	.4byte 0x020094e6
	.4byte 0x0200966e
	.4byte 0x020094f4
	.4byte 0x0200966e
	.4byte 0x02009502
	.4byte 0x0200966e
	.4byte 0x02009510
	.4byte 0x0200966e
	.4byte 0x0200951e
	.4byte 0x0200966e
	.4byte 0x0200952c
	.4byte 0x0200966e
	.4byte 0x0200953a
	.4byte 0x0200966e
	.4byte 0x02009548
	.4byte 0x0200966e
	.4byte 0x02009556
	.4byte 0x0200966e
	.4byte 0x02009564
	.4byte 0x0200966e
	.4byte 0x02009572
	.4byte 0x0200966e
	.4byte 0x02009580
	.4byte 0x0200966e
	.4byte 0x0200958e
	.4byte 0x0200966e
	.4byte 0x0200959c
	.4byte 0x0200966e
	.4byte 0x020095aa
	.4byte 0x0200966e
	.4byte 0x020095b8
	.4byte 0x0200966e
	.4byte 0x020095c6
	.4byte 0x0200966e
	.4byte 0x020095d4
	.4byte 0x0200966e
	.4byte 0x020095e2
	.4byte 0x0200966e
	.4byte 0x020095f0
	.4byte 0x0200966e
	.4byte 0x020095fe
	.4byte 0x0200966e
	.4byte 0x0200960c
	.4byte 0x0200966e
	.4byte 0x0200961a
	.4byte 0x0200966e
	.4byte 0x02009628
	.4byte 0x0200966e
	.4byte 0x02009636
	.4byte 0x0200966e
	.4byte 0x02009644
	.4byte 0x0200966e
	.4byte 0x02009652
	.4byte 0x0200966e
	.4byte 0x02009660
	.4byte 0x0200966e
	.4byte 0x22ce21ac
	.4byte 0x00491c30
	.4byte 0xf0010092
	.4byte 0xf000fc33
	.4byte 0x21a2fd09
	.4byte 0x1c3022cd
	.4byte 0x00920049
	.4byte 0xfc2af001
	.4byte 0xfd00f000
	.4byte 0x22ca21a0
	.4byte 0x00491c30
	.4byte 0xf0010092
	.4byte 0xf000fc21
	.4byte 0x21a2fcf7
	.4byte 0x1c3022c7
	.4byte 0x00920049
	.4byte 0xfc18f001
	.4byte 0xfceef000
	.4byte 0x22c621ac
	.4byte 0x00491c30
	.4byte 0xf0010092
	.4byte 0xf000fc0f
	.4byte 0x21b6fce5
	.4byte 0x1c3022c7
	.4byte 0x00920049
	.4byte 0xfc06f001
	.4byte 0xfcdcf000
	.4byte 0x22ca21b8
	.4byte 0x00491c30
	.4byte 0xf0010092
	.4byte 0xf000fbfd
	.4byte 0x21b6fcd3
	.4byte 0x1c3022cd
	.4byte 0x00920049
	.4byte 0xfbf4f001
	.4byte 0xfccaf000
	.4byte 0x22ce21ac
	.4byte 0x00491c30
	.4byte 0xf0010092
	.4byte 0xf000fbeb
	.4byte 0x21a2fcc1
	.4byte 0x1c3022cd
	.4byte 0x00920049
	.4byte 0xfbe2f001
	.4byte 0xfcb8f000
	.4byte 0x22ca21a0
	.4byte 0x00491c30
	.4byte 0xf0010092
	.4byte 0xf000fbd9
	.4byte 0x21a2fcaf
	.4byte 0x1c3022c7
	.4byte 0x00920049
	.4byte 0xfbd0f001
	.4byte 0xfca6f000
	.4byte 0x22c621ac
	.4byte 0x00491c30
	.4byte 0xf0010092
	.4byte 0xf000fbc7
	.4byte 0x21b6fc9d
	.4byte 0x1c3022c7
	.4byte 0x00920049
	.4byte 0xfbbef001
	.4byte 0xfc94f000
	.4byte 0x22ca21b8
	.4byte 0x00491c30
	.4byte 0xf0010092
	.4byte 0xf000fbb5
	.4byte 0x21b6fc8b
	.4byte 0x1c3022cd
	.4byte 0x00920049
	.4byte 0xfbacf001
	.4byte 0xfc82f000
	.4byte 0x22ce21ac
	.4byte 0x00491c30
	.4byte 0xf0010092
	.4byte 0xf000fba3
	.4byte 0x21e6fc79
	.4byte 0x024922e6
	.4byte 0x1c300252
	.4byte 0x32cc31cc
	.4byte 0xfb88f001
	.4byte 0x22cd21b6
	.4byte 0x00491c30
	.4byte 0xf0010092
	.4byte 0xf000fb91
	.4byte 0x21b8fc67
	.4byte 0x1c3022ca
	.4byte 0x00920049
	.4byte 0xfb88f001
	.4byte 0xfc5ef000
	.4byte 0x22ca21c4
	.4byte 0x00491c30
	.4byte 0xf0010092
	.4byte 0xf000fb7f
	.4byte 0x21c4fc55
	.4byte 0x1c3022ba
	.4byte 0x00920049
	.4byte 0xfb76f001
	.4byte 0xfc4cf000
	.4byte 0x49e61c30
	.4byte 0xf0014ae5
	.4byte 0x21bafb5f
	.4byte 0x1c3022b9
	.4byte 0x00920049
	.4byte 0xfb68f001
	.4byte 0xfc3ef000
	.4byte 0x22b621b8
	.4byte 0x00491c30
	.4byte 0xf0010092
	.4byte 0xf000fb5f
	.4byte 0x21bafc35
	.4byte 0x1c3022b3
	.4byte 0x00920049
	.4byte 0xfb56f001
	.4byte 0xfc2cf000
	.4byte 0x22b221c4
	.4byte 0x00491c30
	.4byte 0xf0010092
	.4byte 0xf000fb4d
	.4byte 0x21cefc23
	.4byte 0x1c3022b3
	.4byte 0x00920049
	.4byte 0xfb44f001
	.4byte 0xfc1af000
	.4byte 0x22b621d0
	.4byte 0x00491c30
	.4byte 0xf0010092
	.4byte 0xf000fb3b
	.4byte 0x21cefc11
	.4byte 0x1c3022b9
	.4byte 0x00920049
	.4byte 0xfb32f001
	.4byte 0xfc08f000
	.4byte 0x22ba21c4
	.4byte 0x00491c30
	.4byte 0xf0010092
	.4byte 0xf000fb29
	.4byte 0x21bafbff
	.4byte 0x1c3022b9
	.4byte 0x00920049
	.4byte 0xfb20f001
	.4byte 0x21b8e3f6
	.4byte 0x1c3022b6
	.4byte 0x00920049
	.4byte 0xfb18f001
	.4byte 0x21bae3ee
	.4byte 0x1c3022b3
	.4byte 0x00920049
	.4byte 0xfb10f001
	.4byte 0x21c4e3e6
	.4byte 0x1c3022b2
	.4byte 0x00920049
	.4byte 0xfb08f001
	.4byte 0x21cee3de
	.4byte 0x1c3022b3
	.4byte 0x00920049
	.4byte 0xfb00f001
	.4byte 0x21d0e3d6
	.4byte 0x1c3022b6
	.4byte 0x00920049
	.4byte 0xfaf8f001
	.4byte 0x21cee3ce
	.4byte 0x1c3022b9
	.4byte 0x00920049
	.4byte 0xfaf0f001
	.4byte 0x21c4e3c6
	.4byte 0x1c3022ba
	.4byte 0x00920049
	.4byte 0xfae8f001
	.4byte 0x21bae3be
	.4byte 0x1c3022b9
	.4byte 0x00920049
	.4byte 0xfae0f001
	.4byte 0x21b8e3b6
	.4byte 0x1c3022b6
	.4byte 0x00920049
	.4byte 0xfad8f001
	.4byte 0x21bae3ae
	.4byte 0x1c3022b3
	.4byte 0x00920049
	.4byte 0xfad0f001
	.4byte 0x21c4e3a6
	.4byte 0x1c3022b2
	.4byte 0x00920049
	.4byte 0xfac8f001
	.4byte 0x21cee39e
	.4byte 0x1c3022b3
	.4byte 0x00920049
	.4byte 0xfac0f001
	.4byte 0x21d0e396
	.4byte 0x1c3022b6
	.4byte 0x00920049
	.4byte 0xfab8f001
	.4byte 0x21cee38e
	.4byte 0x1c3022b9
	.4byte 0x00920049
	.4byte 0xfab0f001
	.4byte 0x21c4e386
	.4byte 0x1c3022ba
	.4byte 0x00920049
	.4byte 0xfaa8f001
	.4byte 0x21bae37e
	.4byte 0x1c3022b9
	.4byte 0x00920049
	.4byte 0xfaa0f001
	.4byte 0x21b8e376
	.4byte 0x1c3022b6
	.4byte 0x00920049
	.4byte 0xfa98f001
	.4byte 0x21bae36e
	.4byte 0x1c3022b3
	.4byte 0x00920049
	.4byte 0xfa90f001
	.4byte 0x21c4e366
	.4byte 0x1c3022b2
	.4byte 0x00920049
	.4byte 0xfa88f001
	.4byte 0x21c4e35e
	.4byte 0x1c3022ae
	.4byte 0x00920049
	.4byte 0xfa80f001
	.4byte 0x21cee356
	.4byte 0x1c3022ad
	.4byte 0x00920049
	.4byte 0xfa78f001
	.4byte 0x21d0e34e
	.4byte 0x1c3022aa
	.4byte 0x00920049
	.4byte 0xfa70f001
	.4byte 0x21cee346
	.4byte 0x1c3022a7
	.4byte 0x00920049
	.4byte 0xfa68f001
	.4byte 0x21c4e33e
	.4byte 0x1c3022a6
	.4byte 0x00920049
	.4byte 0xfa60f001
	.4byte 0x21bae336
	.4byte 0x1c3022a7
	.4byte 0x00920049
	.4byte 0xfa58f001
	.4byte 0x21b8e32e
	.4byte 0x1c3022aa
	.4byte 0x00920049
	.4byte 0xfa50f001
	.4byte 0x21a0e326
	.4byte 0x1c3022aa
	.4byte 0x00920049
	.4byte 0xfa48f001
	.4byte 0x219ee31e
	.4byte 0x1c3022ad
	.4byte 0x00920049
	.4byte 0xfa40f001
	.4byte 0x2194e316
	.4byte 0x1c3022ae
	.4byte 0x00920049
	.4byte 0xfa38f001
	.4byte 0x218ae30e
	.4byte 0x1c3022ad
	.4byte 0x00920049
	.4byte 0xfa30f001
	.4byte 0x2188e306
	.4byte 0x1c3022aa
	.4byte 0x00920049
	.4byte 0xfa28f001
	.4byte 0x218ae2fe
	.4byte 0x1c3022a7
	.4byte 0x00920049
	.4byte 0xfa20f001
	.4byte 0x2194e2f6
	.4byte 0x1c3022a6
	.4byte 0x00920049
	.4byte 0xfa18f001
	.4byte 0x219ee2ee
	.4byte 0x1c3022a7
	.4byte 0x00920049
	.4byte 0xfa10f001
	.4byte 0x21a0e2e6
	.4byte 0x1c3022aa
	.4byte 0x00920049
	.4byte 0xfa08f001
	.4byte 0x219ee2de
	.4byte 0x1c3022ad
	.4byte 0x00920049
	.4byte 0xfa00f001
	.4byte 0x2194e2d6
	.4byte 0x1c3022ae
	.4byte 0x00920049
	.4byte 0xf9f8f001
	.4byte 0x218ae2ce
	.4byte 0x1c3022ad
	.4byte 0x00920049
	.4byte 0xf9f0f001
	.4byte 0x2188e2c6
	.4byte 0x1c3022aa
	.4byte 0x00920049
	.4byte 0xf9e8f001
	.4byte 0x218ae2be
	.4byte 0x1c3022a7
	.4byte 0x00920049
	.4byte 0xf9e0f001
	.4byte 0x2194e2b6
	.4byte 0x1c3022a6
	.4byte 0x00920049
	.4byte 0xf9d8f001
	.4byte 0x219ee2ae
	.4byte 0x1c3022a7
	.4byte 0x00920049
	.4byte 0xf9d0f001
	.4byte 0x21a0e2a6
	.4byte 0x1c3022aa
	.4byte 0x00920049
	.4byte 0xf9c8f001
	.4byte 0x219ee29e
	.4byte 0x1c3022ad
	.4byte 0x00920049
	.4byte 0xf9c0f001
	.4byte 0x2194e296
	.4byte 0x1c3022ae
	.4byte 0x00920049
	.4byte 0xf9b8f001
	.4byte 0x2194e28e
	.4byte 0x1c3022b6
	.4byte 0x00920049
	.4byte 0xf9b0f001
	.4byte 0x2188e286
	.4byte 0x1c3022b6
	.4byte 0x00920049
	.4byte 0xf9a8f001
	.4byte 0x0000e27e
	.4byte 0x00013333
	.4byte 0x22b92186
	.4byte 0x00491c30
	.4byte 0xf0010092
	.4byte 0xe273f99d
	.4byte 0x1c3022ba
	.4byte 0x009221f8
	.4byte 0xf996f001
	.4byte 0x22b9e26c
	.4byte 0x21e41c30
	.4byte 0xf0010092
	.4byte 0xe265f98f
	.4byte 0x1c3022b6
	.4byte 0x009221e0
	.4byte 0xf988f001
	.4byte 0x22b3e25e
	.4byte 0x21e41c30
	.4byte 0xf0010092
	.4byte 0xe257f981
	.4byte 0x1c3022b2
	.4byte 0x009221f8
	.4byte 0xf97af001
	.4byte 0x2186e250
	.4byte 0x1c3022b3
	.4byte 0x00920049
	.4byte 0xf972f001
	.4byte 0x2188e248
	.4byte 0x1c3022b6
	.4byte 0x00920049
	.4byte 0xf96af001
	.4byte 0x2186e240
	.4byte 0x1c3022b9
	.4byte 0x00920049
	.4byte 0xf962f001
	.4byte 0x22bae238
	.4byte 0x21f81c30
	.4byte 0xf0010092
	.4byte 0xe231f95b
	.4byte 0x1c3022b9
	.4byte 0x009221e4
	.4byte 0xf954f001
	.4byte 0x22b6e22a
	.4byte 0x21e01c30
	.4byte 0xf0010092
	.4byte 0xe223f94d
	.4byte 0x1c3022b3
	.4byte 0x009221e4
	.4byte 0xf946f001
	.4byte 0x22b2e21c
	.4byte 0x21f81c30
	.4byte 0xf0010092
	.4byte 0xe215f93f
	.4byte 0x22b32186
	.4byte 0x00491c30
	.4byte 0xf0010092
	.4byte 0xe20df937
	.4byte 0x22b62188
	.4byte 0x00491c30
	.4byte 0xf0010092
	.4byte 0xe205f92f
	.4byte 0x22b92186
	.4byte 0x00491c30
	.4byte 0xf0010092
	.4byte 0xe1fdf927
	.4byte 0x1c3022ba
	.4byte 0x009221f8
	.4byte 0xf920f001
	.4byte 0x22b9e1f6
	.4byte 0x21e41c30
	.4byte 0xf0010092
	.4byte 0xe1eff919
	.4byte 0x1c3022b6
	.4byte 0x009221e0
	.4byte 0xf912f001
	.4byte 0x22b3e1e8
	.4byte 0x21e41c30
	.4byte 0xf0010092
	.4byte 0xe1e1f90b
	.4byte 0x1c3022b2
	.4byte 0x009221f8
	.4byte 0xf904f001
	.4byte 0x2186e1da
	.4byte 0x1c3022b3
	.4byte 0x00920049
	.4byte 0xf8fcf001
	.4byte 0x2188e1d2
	.4byte 0x1c3022b6
	.4byte 0x00920049
	.4byte 0xf8f4f001
	.4byte 0x2186e1ca
	.4byte 0x1c3022b9
	.4byte 0x00920049
	.4byte 0xf8ecf001
	.4byte 0x22bae1c2
	.4byte 0x21f81c30
	.4byte 0xf0010092
	.4byte 0xe1bbf8e5
	.4byte 0x1c3022be
	.4byte 0x009221f8
	.4byte 0xf8def001
	.4byte 0x22bfe1b4
	.4byte 0x21e41c30
	.4byte 0xf0010092
	.4byte 0xe1adf8d7
	.4byte 0x1c3022c2
	.4byte 0x009221e0
	.4byte 0xf8d0f001
	.4byte 0x22c5e1a6
	.4byte 0x21e41c30
	.4byte 0xf0010092
	.4byte 0xe19ff8c9
	.4byte 0x1c3022c6
	.4byte 0x009221f8
	.4byte 0xf8c2f001
	.4byte 0x22d2e198
	.4byte 0x21f81c30
	.4byte 0xf0010092
	.4byte 0xe191f8bb
	.4byte 0x1c3022d3
	.4byte 0x009221e4
	.4byte 0xf8b4f001
	.4byte 0x22d6e18a
	.4byte 0x21e01c30
	.4byte 0xf0010092
	.4byte 0xe183f8ad
	.4byte 0x1c3022d9
	.4byte 0x009221e4
	.4byte 0xf8a6f001
	.4byte 0x2186e17c
	.4byte 0x1c3022d9
	.4byte 0x00920049
	.4byte 0xf89ef001
	.4byte 0x2188e174
	.4byte 0x1c3022d6
	.4byte 0x00920049
	.4byte 0xf896f001
	.4byte 0x2186e16c
	.4byte 0x1c3022d3
	.4byte 0x00920049
	.4byte 0xf88ef001
	.4byte 0x22d2e164
	.4byte 0x21f81c30
	.4byte 0xf0010092
	.4byte 0xe15df887
	.4byte 0x1c3022d3
	.4byte 0x009221e4
	.4byte 0xf880f001
	.4byte 0x22d6e156
	.4byte 0x21e01c30
	.4byte 0xf0010092
	.4byte 0xe14ff879
	.4byte 0x1c3022d9
	.4byte 0x009221e4
	.4byte 0xf872f001
	.4byte 0x1c28e148
	.4byte 0xf95cf7ff
	.4byte 0xd0022800
	.4byte 0x3301883b
	.4byte 0x22da803b
	.4byte 0x1c300092
	.4byte 0xf00121f8
	.4byte 0x1c2af863
	.4byte 0x88133264
	.4byte 0x80133301
	.4byte 0x2186e137
	.4byte 0x1c3022d9
	.4byte 0x00920049
	.4byte 0xf856f001
	.4byte 0x2188e12c
	.4byte 0x1c3022d6
	.4byte 0x00920049
	.4byte 0xf84ef001
	.4byte 0x2186e124
	.4byte 0x1c3022d3
	.4byte 0x00920049
	.4byte 0xf846f001
	.4byte 0x22d2e11c
	.4byte 0x21f81c30
	.4byte 0xf0010092
	.4byte 0xe115f83f
	.4byte 0x1c3022d3
	.4byte 0x009221e4
	.4byte 0xf838f001
	.4byte 0x22d6e10e
	.4byte 0x21e01c30
	.4byte 0xf0010092
	.4byte 0xe107f831
	.4byte 0x22e621e6
	.4byte 0x02520249
	.4byte 0x31cc1c30
	.4byte 0xf00132cc
	.4byte 0x22d6f817
	.4byte 0x21c81c30
	.4byte 0xf0010092
	.4byte 0xe0f7f821
	.4byte 0x1c3022ba
	.4byte 0x009221c8
	.4byte 0xf81af001
	.4byte 0x1c30e0f0
	.4byte 0x4a7a497a
	.4byte 0xf804f001
	.4byte 0x1c3022b9
	.4byte 0x009221b4
	.4byte 0xf80ef001
	.4byte 0x22b6e0e4
	.4byte 0x21b01c30
	.4byte 0xf0010092
	.4byte 0xe0ddf807
	.4byte 0x1c3022b3
	.4byte 0x009221b4
	.4byte 0xf800f001
	.4byte 0x22b2e0d6
	.4byte 0x21c81c30
	.4byte 0xf0000092
	.4byte 0xe0cffff9
	.4byte 0x1c3022b3
	.4byte 0x009221dc
	.4byte 0xfff2f000
	.4byte 0x22b6e0c8
	.4byte 0x21e01c30
	.4byte 0xf0000092
	.4byte 0xe0c1ffeb
	.4byte 0x1c3022b9
	.4byte 0x009221dc
	.4byte 0xffe4f000
	.4byte 0x22bae0ba
	.4byte 0x21c81c30
	.4byte 0xf0000092
	.4byte 0xe0b3ffdd
	.4byte 0x1c3022b9
	.4byte 0x009221b4
	.4byte 0xffd6f000
	.4byte 0x22b6e0ac
	.4byte 0x21b01c30
	.4byte 0xf0000092
	.4byte 0xe0a5ffcf
	.4byte 0x1c3022b3
	.4byte 0x009221b4
	.4byte 0xffc8f000
	.4byte 0x22b2e09e
	.4byte 0x21c81c30
	.4byte 0xf0000092
	.4byte 0xe097ffc1
	.4byte 0x1c3022b3
	.4byte 0x009221dc
	.4byte 0xffbaf000
	.4byte 0x22b6e090
	.4byte 0x21e01c30
	.4byte 0xf0000092
	.4byte 0xe089ffb3
	.4byte 0x1c3022b9
	.4byte 0x009221dc
	.4byte 0xffacf000
	.4byte 0x22bae082
	.4byte 0x21c81c30
	.4byte 0xf0000092
	.4byte 0xe07bffa5
	.4byte 0x1c3022b9
	.4byte 0x009221b4
	.4byte 0xff9ef000
	.4byte 0x22b6e074
	.4byte 0x21b01c30
	.4byte 0xf0000092
	.4byte 0xe06dff97
	.4byte 0x1c3022b3
	.4byte 0x009221b4
	.4byte 0xff90f000
	.4byte 0x22b2e066
	.4byte 0x21c81c30
	.4byte 0xf0000092
	.4byte 0xe05fff89
	.4byte 0x1c3022b3
	.4byte 0x009221dc
	.4byte 0xff82f000
	.4byte 0x22b6e058
	.4byte 0x21e01c30
	.4byte 0xf0000092
	.4byte 0xe051ff7b
	.4byte 0x1c3022b9
	.4byte 0x009221dc
	.4byte 0xff74f000
	.4byte 0x22bae04a
	.4byte 0x21c81c30
	.4byte 0xf0000092
	.4byte 0xe043ff6d
	.4byte 0x1c3022b9
	.4byte 0x009221b4
	.4byte 0xff66f000
	.4byte 0x22b6e03c
	.4byte 0x21b01c30
	.4byte 0xf0000092
	.4byte 0xe035ff5f
	.4byte 0x1c3022b3
	.4byte 0x009221b4
	.4byte 0xff58f000
	.4byte 0x22b2e02e
	.4byte 0x21c81c30
	.4byte 0xf0000092
	.4byte 0xe027ff51
	.4byte 0x1c3022aa
	.4byte 0x009221c8
	.4byte 0xff4af000
	.4byte 0x22a9e020
	.4byte 0x21d41c30
	.4byte 0xf0000092
	.4byte 0xe019ff43
	.4byte 0x1c3022a6
	.4byte 0x009221d8
	.4byte 0xff3cf000
	.4byte 0x22a3e012
	.4byte 0x21d41c30
	.4byte 0xf0000092
	.4byte 0xe00bff35
	.4byte 0x1c3022a2
	.4byte 0x009221c8
	.4byte 0xff2ef000
	.4byte 0x1c28e004
	.4byte 0xf818f7ff
	.4byte 0xd0022800
	.4byte 0x3301883b
	.4byte 0xbc08803b
	.4byte 0xbde04698
	.2byte 0x3333
	.2byte 0x0001
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #8
	bl 0x0200a4a0
	adds	r5, r0, #0
	movs	r0, #10
	bl 0x0200a4a0
	adds	r6, r0, #0
	movs	r0, #9
	bl 0x0200a4a0
	adds	r7, r0, #0
	movs	r0, #11
	bl 0x0200a4a0
	ldr	r3, [pc, #124]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r8, r0
	ldr	r0, [r3, #0]
	bl 0x0200a4a0
	adds	r1, r5, #0
	adds	r1, #102
	movs	r3, #0
	ldrsh	r2, [r1, r3]
	cmp	r2, #2
	beq.n	.L_020016de
	cmp	r2, #2
	bgt.n	.L_020016d2
	cmp	r2, #1
	beq.n	.L_020016d8
	b.n	.L_020016fc
.L_020016d2:
	cmp	r2, #3
	beq.n	.L_020016de
	b.n	.L_020016fc
.L_020016d8:
	adds	r3, r5, #0
	movs	r2, #0
	b.n	.L_020016e0
.L_020016de:
	adds	r3, r5, #0
.L_020016e0:
	adds	r3, #98
	strb	r2, [r3, #0]
	adds	r3, r6, #0
	adds	r3, #98
	strb	r2, [r3, #0]
	adds	r3, r7, #0
	adds	r3, #98
	strb	r2, [r3, #0]
	mov	r3, r8
	adds	r3, #98
	strb	r2, [r3, #0]
	adds	r3, r0, #0
	adds	r3, #98
	strb	r2, [r3, #0]
.L_020016fc:
	movs	r3, #0
	strh	r3, [r1, #0]
	movs	r0, #8
	bl 0x020086bc
	movs	r0, #10
	bl 0x020086bc
	movs	r0, #9
	bl 0x020086bc
	movs	r0, #11
	bl 0x020086bc
	ldr	r3, [pc, #16]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x020086bc
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #8
	bl 0x0200a4a0
	adds	r2, r0, #0
	adds	r3, r2, #0
	adds	r3, #100
	ldr	r5, [pc, #224]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r0, #10
	str	r3, [r5, #0]
	adds	r3, r2, #0
	adds	r3, #102
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	str	r3, [r5, #4]
	adds	r3, r2, #0
	adds	r3, #98
	ldrb	r3, [r3, #0]
	str	r3, [r5, #8]
	adds	r3, r2, #0
	adds	r3, #99
	ldrb	r3, [r3, #0]
	str	r3, [r5, #12]
	bl 0x0200a4a0
	adds	r2, r0, #0
	adds	r3, r2, #0
	adds	r3, #100
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r0, #9
	str	r3, [r5, #16]
	adds	r3, r2, #0
	adds	r3, #102
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	str	r3, [r5, #20]
	adds	r3, r2, #0
	adds	r3, #98
	ldrb	r3, [r3, #0]
	str	r3, [r5, #24]
	adds	r3, r2, #0
	adds	r3, #99
	ldrb	r3, [r3, #0]
	str	r3, [r5, #28]
	bl 0x0200a4a0
	adds	r2, r0, #0
	adds	r3, r2, #0
	adds	r3, #100
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r0, #11
	str	r3, [r5, #32]
	adds	r3, r2, #0
	adds	r3, #102
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	str	r3, [r5, #36]
	adds	r3, r2, #0
	adds	r3, #98
	ldrb	r3, [r3, #0]
	str	r3, [r5, #40]
	adds	r3, r2, #0
	adds	r3, #99
	ldrb	r3, [r3, #0]
	str	r3, [r5, #44]
	bl 0x0200a4a0
	adds	r2, r0, #0
	adds	r3, r2, #0
	adds	r3, #100
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	str	r3, [r5, #48]
	adds	r3, r2, #0
	adds	r3, #102
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	str	r3, [r5, #52]
	adds	r3, r2, #0
	adds	r3, #98
	ldrb	r3, [r3, #0]
	str	r3, [r5, #56]
	adds	r3, r2, #0
	adds	r3, #99
	ldrb	r3, [r3, #0]
	movs	r2, #133
	str	r3, [r5, #60]
	ldr	r3, [pc, #60]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200a4a0
	adds	r2, r0, #0
	adds	r3, r2, #0
	adds	r3, #100
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	ldr	r0, [pc, #40]
	str	r3, [r5, #64]
	adds	r3, r2, #0
	adds	r3, #102
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	str	r3, [r5, #68]
	adds	r3, r2, #0
	adds	r3, #98
	ldrb	r3, [r3, #0]
	str	r3, [r5, #72]
	adds	r3, r2, #0
	adds	r3, #99
	ldrb	r3, [r3, #0]
	str	r3, [r5, #76]
	bl 0x0200a428
	pop	{r5, pc}
	.4byte 0x0200afc0
	.4byte 0x02000240
	.2byte 0x9689
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #8
	bl 0x0200a4a0
	ldr	r5, [pc, #192]
	adds	r1, r0, #0
	ldr	r2, [r5, #0]
	adds	r3, r1, #0
	adds	r3, #100
	strh	r2, [r3, #0]
	adds	r2, r1, #0
	ldr	r3, [r5, #4]
	adds	r2, #102
	strh	r3, [r2, #0]
	subs	r2, #4
	ldr	r3, [r5, #8]
	movs	r0, #10
	strb	r3, [r2, #0]
	adds	r2, #1
	ldr	r3, [r5, #12]
	strb	r3, [r2, #0]
	bl 0x0200a4a0
	ldr	r2, [r5, #16]
	adds	r1, r0, #0
	adds	r3, r1, #0
	adds	r3, #100
	strh	r2, [r3, #0]
	adds	r2, r1, #0
	ldr	r3, [r5, #20]
	adds	r2, #102
	strh	r3, [r2, #0]
	subs	r2, #4
	ldr	r3, [r5, #24]
	movs	r0, #9
	strb	r3, [r2, #0]
	adds	r2, #1
	ldr	r3, [r5, #28]
	strb	r3, [r2, #0]
	bl 0x0200a4a0
	ldr	r2, [r5, #32]
	adds	r1, r0, #0
	adds	r3, r1, #0
	adds	r3, #100
	strh	r2, [r3, #0]
	adds	r2, r1, #0
	ldr	r3, [r5, #36]
	adds	r2, #102
	strh	r3, [r2, #0]
	subs	r2, #4
	ldr	r3, [r5, #40]
	movs	r0, #11
	strb	r3, [r2, #0]
	adds	r2, #1
	ldr	r3, [r5, #44]
	strb	r3, [r2, #0]
	bl 0x0200a4a0
	ldr	r2, [r5, #48]
	adds	r1, r0, #0
	adds	r3, r1, #0
	adds	r3, #100
	strh	r2, [r3, #0]
	adds	r2, r1, #0
	ldr	r3, [r5, #52]
	adds	r2, #102
	strh	r3, [r2, #0]
	subs	r2, #4
	ldr	r3, [r5, #56]
	strb	r3, [r2, #0]
	adds	r2, #1
	ldr	r3, [r5, #60]
	strb	r3, [r2, #0]
	ldr	r3, [pc, #56]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200a4a0
	ldr	r2, [r5, #64]
	adds	r1, r0, #0
	adds	r3, r1, #0
	adds	r3, #100
	strh	r2, [r3, #0]
	adds	r2, r1, #0
	ldr	r3, [r5, #68]
	adds	r2, #102
	strh	r3, [r2, #0]
	subs	r2, #4
	ldr	r3, [r5, #72]
	movs	r1, #144
	strb	r3, [r2, #0]
	adds	r2, #1
	ldr	r3, [r5, #76]
	lsls	r1, r1, #3
	strb	r3, [r2, #0]
	ldr	r0, [pc, #12]
	bl 0x0200a420
	pop	{r5, pc}
	.4byte 0x0200afc0
	.4byte 0x02000240
	.2byte 0x9689
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	bl 0x0200a4a0
	ldr	r2, [pc, #48]
	adds	r6, r0, #0
	ldr	r1, [pc, #44]
	adds	r0, r5, #0
	bl 0x0200a4a8
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200a500
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #100
	strh	r3, [r2, #0]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r1, [pc, #12]
	subs	r2, #4
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r3, r6, #0
	adds	r3, #99
	strb	r1, [r3, #0]
	pop	{r5, r6, pc}
	.4byte 0x00000000
	.2byte 0x3333
	.2byte 0x0001
	push	{r5, r6, r7, lr}
	movs	r0, #8
	bl 0x0200a4a0
	adds	r7, r0, #0
	ldr	r0, [pc, #904]
	bl 0x0200a520
	movs	r1, #0
	movs	r0, #8
	bl 0x0200a538
	movs	r0, #9
	bl 0x0200a4b8
	movs	r0, #10
	bl 0x0200a4b8
	movs	r0, #9
	bl 0x0200a4a0
	movs	r5, #0
	str	r5, [r0, #108]
	movs	r0, #10
	bl 0x0200a4a0
	str	r5, [r0, #108]
	movs	r0, #1
	bl 0x0200a418
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #9
	ldr	r1, [pc, #852]
	adds	r2, #204
	bl 0x0200a4a8
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #10
	ldr	r1, [pc, #836]
	bl 0x0200a4a8
	ldr	r1, [pc, #832]
	movs	r0, #9
	bl 0x0200a4b0
	ldr	r1, [pc, #828]
	movs	r0, #10
	bl 0x0200a4b0
	bl 0x0200a570
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r1, #200
	movs	r0, #204
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200a560
	movs	r0, #152
	movs	r1, #1
	lsls	r0, r0, #17
	negs	r1, r1
	ldr	r2, [pc, #792]
	movs	r3, #1
	bl 0x0200a568
	ldr	r3, [pc, #788]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200a4a8
	movs	r1, #148
	movs	r2, #218
	lsls	r2, r2, #2
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200a4d8
	movs	r1, #0
	ldr	r0, [r5, #0]
	bl 0x0200a548
	movs	r0, #78
	bl 0x0200a5a0
	movs	r1, #176
	movs	r2, #20
	movs	r0, #8
	lsls	r1, r1, #8
	bl 0x0200a540
	movs	r1, #208
	movs	r0, #8
	lsls	r1, r1, #8
	bl 0x0200a548
	movs	r0, #8
	movs	r1, #2
	movs	r2, #10
	bl 0x0200a510
	movs	r2, #10
	movs	r0, #8
	movs	r1, #4
	bl 0x0200a510
	movs	r0, #8
	movs	r1, #3
	bl 0x0200a508
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #8
	ldr	r1, [pc, #664]
	adds	r2, #204
	bl 0x0200a4a8
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #11
	ldr	r1, [pc, #648]
	bl 0x0200a4a8
	ldr	r1, [pc, #660]
	movs	r0, #8
	bl 0x0200a4b0
	ldr	r1, [pc, #656]
	movs	r0, #9
	bl 0x0200a4b0
	ldr	r1, [pc, #652]
	movs	r0, #10
	bl 0x0200a4b0
	ldr	r1, [pc, #648]
	movs	r0, #11
	bl 0x0200a4c0
	movs	r1, #148
	movs	r2, #218
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	movs	r0, #5
	bl 0x0200a4f8
	movs	r0, #1
	bl 0x0200a418
	movs	r1, #224
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a540
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #608]
	adds	r2, #153
	bl 0x0200a4a8
	movs	r2, #192
	movs	r1, #156
	lsls	r2, r2, #2
	adds	r2, #86
	movs	r0, #5
	lsls	r1, r1, #1
	bl 0x0200a4d8
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x0200a548
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a508
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200a508
	movs	r0, #5
	movs	r1, #2
	bl 0x0200a500
	ldr	r0, [r5, #0]
	bl 0x0200a4a0
	cmp	r0, #0
	beq.n	.L_02001af0
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x0200a4c8
.L_02001af0:
	movs	r0, #5
	bl 0x0200a4f0
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a4f8
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #468]
	adds	r2, #204
	bl 0x0200a4a8
	movs	r1, #158
	movs	r2, #224
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200a4d8
	movs	r1, #172
	movs	r2, #228
	lsls	r2, r2, #2
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200a4d8
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200a548
	movs	r0, #17
	bl 0x0200a5a0
	movs	r1, #192
	movs	r0, #8
	lsls	r1, r1, #8
	bl 0x0200a548
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a538
	movs	r0, #8
	movs	r1, #1
	bl 0x0200a558
	ldr	r1, [pc, #428]
	movs	r0, #8
	bl 0x0200a4b0
	ldr	r1, [pc, #424]
	movs	r0, #10
	bl 0x0200a4b0
	ldr	r1, [pc, #420]
	movs	r0, #9
	bl 0x0200a4c0
	movs	r2, #192
	movs	r1, #172
	lsls	r2, r2, #2
	adds	r2, #122
	movs	r0, #11
	lsls	r1, r1, #1
	bl 0x0200a4d8
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #11
	bl 0x0200a548
	movs	r0, #8
	bl 0x02009904
	movs	r0, #10
	bl 0x02009904
	movs	r0, #9
	bl 0x02009904
	movs	r0, #11
	bl 0x02009904
	ldr	r0, [r5, #0]
	bl 0x02009904
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #356]
	bl 0x0200a420
	adds	r5, r7, #0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a538
	adds	r5, #102
	movs	r3, #1
	strh	r3, [r5, #0]
.L_02001bbe:
	adds	r6, r7, #0
	movs	r0, #1
	adds	r6, #100
	bl 0x0200a418
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #34
	bne.n	.L_02001bbe
	movs	r3, #2
	strh	r3, [r5, #0]
	movs	r0, #1
	bl 0x0200a418
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a538
	movs	r3, #1
	strh	r3, [r5, #0]
.L_02001be6:
	movs	r0, #1
	bl 0x0200a418
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #42
	bne.n	.L_02001be6
	movs	r3, #2
	strh	r3, [r5, #0]
	movs	r0, #1
	bl 0x0200a418
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a538
	movs	r3, #1
	strh	r3, [r5, #0]
.L_02001c0a:
	movs	r0, #1
	bl 0x0200a418
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #108
	bne.n	.L_02001c0a
	movs	r3, #3
	strh	r3, [r5, #0]
	movs	r0, #1
	bl 0x0200a418
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a538
	bl 0x02009730
	movs	r0, #8
	movs	r1, #4
	movs	r2, #0
	bl 0x0200a510
	movs	r0, #10
	movs	r1, #4
	movs	r2, #0
	bl 0x0200a510
	movs	r0, #9
	movs	r1, #4
	movs	r2, #0
	bl 0x0200a510
	movs	r0, #11
	movs	r1, #4
	movs	r2, #40
	bl 0x0200a510
	ldr	r5, [pc, #148]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a540
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a540
	movs	r1, #160
	movs	r0, #10
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a540
	movs	r1, #128
	movs	r2, #0
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200a540
	movs	r1, #128
	movs	r0, #11
	lsls	r1, r1, #8
	bl 0x0200a548
	movs	r1, #0
	movs	r0, #8
	bl 0x0200a528
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200a498
	cmp	r0, #0
	bne.n	.L_02001cb2
	bl 0x02009d14
	b.n	.L_02001cd6
.L_02001cb2:
	movs	r1, #0
	movs	r2, #20
	movs	r0, #8
	bl 0x0200a530
	bl 0x0200a580
	bl 0x0200a588
	movs	r0, #123
	bl 0x0200a5a0
	movs	r0, #40
	bl 0x0200a480
	movs	r0, #15
	bl 0x0200a578
.L_02001cd6:
	pop	{r5, r6, r7, pc}
	.4byte 0x00002254
	.4byte 0x00019999
	.4byte 0x0200a77c
	.4byte 0x0200a7c0
	.4byte 0x036e0000
	.4byte 0x02000240
	.4byte 0x0200a804
	.4byte 0x0200a870
	.4byte 0x0200a8dc
	.4byte 0x0200a948
	.4byte 0x00013333
	.4byte 0x0200a998
	.4byte 0x0200a9c0
	.4byte 0x0200aa04
	.2byte 0x9689
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r0, #8
	bl 0x0200a4a0
	movs	r1, #128
	adds	r7, r0, #0
	lsls	r1, r1, #8
	movs	r0, #8
	movs	r2, #0
	bl 0x0200a540
	movs	r1, #128
	movs	r0, #10
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a540
	movs	r1, #176
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a540
	movs	r1, #176
	movs	r0, #11
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a540
	ldr	r3, [pc, #360]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
.L_02001d56:
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x0200a548
	ldr	r0, [pc, #348]
	bl 0x0200a520
	adds	r5, r7, #0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a538
	adds	r5, #102
	bl 0x0200982c
	movs	r3, #1
	strh	r3, [r5, #0]
.L_02001d78:
	adds	r6, r7, #0
	movs	r0, #1
	adds	r6, #100
	bl 0x0200a418
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #114
	bne.n	.L_02001d78
	movs	r3, #2
	strh	r3, [r5, #0]
	movs	r0, #1
	bl 0x0200a418
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a538
	movs	r3, #1
	strh	r3, [r5, #0]
.L_02001da0:
	movs	r0, #1
	bl 0x0200a418
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #150
	bne.n	.L_02001da0
	movs	r3, #2
.L_02001db0:
	strh	r3, [r5, #0]
	movs	r0, #1
	bl 0x0200a418
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a538
.L_02001dc0:
	movs	r3, #1
	strh	r3, [r5, #0]
.L_02001dc4:
	movs	r0, #1
.L_02001dc6:
	bl 0x0200a418
	movs	r2, #0
.L_02001dcc:
	ldrsh	r3, [r6, r2]
	cmp	r3, #154
	bne.n	.L_02001dc4
.L_02001dd2:
	movs	r3, #2
	strh	r3, [r5, #0]
	movs	r0, #1
	bl 0x0200a418
	movs	r0, #8
	movs	r1, #0
.L_02001de0:
	bl 0x0200a538
	movs	r3, #1
	strh	r3, [r5, #0]
.L_02001de8:
	movs	r0, #1
	bl 0x0200a418
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #216
	bne.n	.L_02001de8
	movs	r3, #3
	strh	r3, [r5, #0]
	movs	r0, #1
	bl 0x0200a418
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a538
	bl 0x02009730
	movs	r0, #8
	movs	r1, #4
	movs	r2, #0
	bl 0x0200a510
	movs	r0, #10
	movs	r1, #4
	movs	r2, #0
	bl 0x0200a510
	movs	r0, #9
	movs	r1, #4
	movs	r2, #0
	bl 0x0200a510
	movs	r0, #11
	movs	r1, #4
	movs	r2, #40
	bl 0x0200a510
	ldr	r5, [pc, #128]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a540
	movs	r1, #208
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a540
	movs	r1, #208
	movs	r0, #10
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a540
	movs	r1, #208
	movs	r2, #0
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200a540
	movs	r1, #208
	movs	r0, #11
	lsls	r1, r1, #8
	bl 0x0200a548
	movs	r1, #0
	movs	r0, #8
	bl 0x0200a528
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200a498
	cmp	r0, #0
	bne.n	.L_02001e90
	bl 0x02009ec0
	b.n	.L_02001eb4
.L_02001e90:
	movs	r1, #0
	movs	r2, #20
	movs	r0, #8
	bl 0x0200a530
	bl 0x0200a580
	bl 0x0200a588
	movs	r0, #123
	bl 0x0200a5a0
	movs	r0, #40
	bl 0x0200a480
	movs	r0, #15
.L_02001eb0:
	bl 0x0200a578
.L_02001eb4:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x225c
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #8
	bl 0x0200a4a0
	movs	r1, #192
	adds	r6, r0, #0
	lsls	r1, r1, #6
	movs	r0, #8
	movs	r2, #0
	bl 0x0200a540
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a540
	movs	r1, #192
	movs	r0, #9
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a540
	movs	r1, #160
	movs	r0, #11
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a540
	ldr	r3, [pc, #820]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r1, #192
	ldr	r0, [r3, #0]
	lsls	r1, r1, #7
	bl 0x0200a548
	ldr	r0, [pc, #808]
	bl 0x0200a520
	adds	r5, r6, #0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a538
	adds	r5, #102
	bl 0x0200982c
	movs	r3, #1
	strh	r3, [r5, #0]
.L_02001f28:
	adds	r7, r6, #0
	movs	r0, #1
	adds	r7, #100
	bl 0x0200a418
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	cmp	r3, #218
	bne.n	.L_02001f28
	movs	r3, #2
	strh	r3, [r5, #0]
	movs	r0, #1
	bl 0x0200a418
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a538
	movs	r3, #1
	strh	r3, [r5, #0]
.L_02001f50:
	movs	r0, #1
	bl 0x0200a418
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	cmp	r3, #254
	bne.n	.L_02001f50
	movs	r3, #2
	strh	r3, [r5, #0]
	movs	r0, #1
	bl 0x0200a418
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a538
	movs	r3, #1
	strh	r3, [r5, #0]
.L_02001f74:
	movs	r0, #1
	bl 0x0200a418
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	movs	r2, #129
	lsls	r2, r2, #1
	cmp	r3, r2
	bne.n	.L_02001f74
	movs	r3, #2
	strh	r3, [r5, #0]
	movs	r0, #1
	bl 0x0200a418
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a538
	movs	r3, #1
	strh	r3, [r5, #0]
.L_02001f9c:
	movs	r0, #1
	bl 0x0200a418
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	movs	r2, #162
	lsls	r2, r2, #1
	cmp	r3, r2
	bne.n	.L_02001f9c
	movs	r3, #3
	strh	r3, [r5, #0]
	movs	r0, #1
	bl 0x0200a418
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a538
	bl 0x02009730
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #8
	bl 0x0200a548
	movs	r0, #8
	bl 0x0200a4a0
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #8
	bl 0x0200a4a0
	adds	r6, r0, #0
	ldr	r3, [r6, #16]
	movs	r0, #8
	ldr	r1, [pc, #588]
	ldr	r2, [pc, #592]
	mov	r8, r3
	ldr	r7, [r6, #8]
	bl 0x0200a4a8
	movs	r0, #8
	movs	r1, #6
	movs	r2, #0
	bl 0x0200a510
	movs	r2, #128
	lsls	r2, r2, #2
	movs	r0, #8
	movs	r1, #200
	adds	r2, #158
	bl 0x0200a4d0
	movs	r0, #8
	movs	r1, #4
	movs	r2, #0
	bl 0x0200a510
	mov	r3, r8
	movs	r2, #0
	adds	r1, r7, #0
	adds	r0, r6, #0
	bl 0x0200a450
	movs	r0, #5
	bl 0x0200a480
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #10
	bl 0x0200a548
	movs	r0, #10
	bl 0x0200a4a0
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x0200a4a0
	adds	r6, r0, #0
	ldr	r2, [r6, #16]
	movs	r0, #10
	mov	r8, r2
	ldr	r1, [pc, #484]
	ldr	r2, [pc, #488]
	ldr	r7, [r6, #8]
	bl 0x0200a4a8
	movs	r0, #10
	movs	r1, #6
	movs	r2, #0
	bl 0x0200a510
	movs	r2, #128
	lsls	r2, r2, #2
	movs	r0, #10
	movs	r1, #200
	adds	r2, #158
	bl 0x0200a4d0
	movs	r0, #10
	movs	r1, #4
	movs	r2, #0
	bl 0x0200a510
	mov	r3, r8
	movs	r2, #0
	adds	r1, r7, #0
	adds	r0, r6, #0
	bl 0x0200a450
	movs	r0, #5
	bl 0x0200a480
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x0200a548
	movs	r0, #9
	bl 0x0200a4a0
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #9
	bl 0x0200a4a0
	adds	r6, r0, #0
	ldr	r3, [r6, #16]
	movs	r0, #9
	ldr	r1, [pc, #384]
	ldr	r2, [pc, #388]
	mov	r8, r3
	ldr	r7, [r6, #8]
	bl 0x0200a4a8
	movs	r0, #9
	movs	r1, #6
	movs	r2, #0
	bl 0x0200a510
	movs	r2, #128
	lsls	r2, r2, #2
	movs	r0, #9
	movs	r1, #200
	adds	r2, #158
	bl 0x0200a4d0
	movs	r0, #9
	movs	r1, #4
	movs	r2, #0
	bl 0x0200a510
	movs	r2, #0
	mov	r3, r8
	adds	r1, r7, #0
	adds	r0, r6, #0
	bl 0x0200a450
	movs	r0, #10
	bl 0x0200a480
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r0, #11
	bl 0x0200a548
	movs	r0, #11
	bl 0x0200a4a0
	adds	r0, #90
	ldrb	r3, [r0, #0]
	ands	r5, r3
	strb	r5, [r0, #0]
	movs	r0, #11
	bl 0x0200a4a0
	adds	r6, r0, #0
	ldr	r2, [r6, #16]
	movs	r0, #11
	mov	r8, r2
	ldr	r1, [pc, #284]
	ldr	r2, [pc, #284]
	ldr	r7, [r6, #8]
	bl 0x0200a4a8
	movs	r0, #11
	movs	r1, #6
	movs	r2, #0
	bl 0x0200a510
	movs	r2, #128
	lsls	r2, r2, #2
	movs	r0, #11
	movs	r1, #200
	adds	r2, #158
	bl 0x0200a4d0
	movs	r0, #11
	movs	r1, #4
	movs	r2, #0
	bl 0x0200a510
	adds	r1, r7, #0
	mov	r3, r8
	adds	r0, r6, #0
	movs	r2, #0
	bl 0x0200a450
	movs	r0, #11
	bl 0x0200a4f0
	ldr	r5, [pc, #212]
	movs	r3, #133
	lsls	r3, r3, #2
	movs	r1, #204
	movs	r2, #204
	adds	r5, r5, r3
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200a4a8
	movs	r2, #128
	lsls	r2, r2, #2
	ldr	r0, [r5, #0]
	movs	r1, #184
	adds	r2, #174
	bl 0x0200a4d8
	movs	r2, #128
	lsls	r2, r2, #2
	ldr	r0, [r5, #0]
	movs	r1, #186
	adds	r2, #162
	bl 0x0200a4d8
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200a540
	movs	r0, #8
	bl 0x0200a4a0
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r5, #1
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x0200a4a0
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #9
	bl 0x0200a4a0
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #11
	bl 0x0200a4a0
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #160
	orrs	r5, r3
	strb	r5, [r0, #0]
	lsls	r1, r1, #7
	movs	r0, #8
	movs	r2, #0
	bl 0x0200a540
	movs	r1, #128
	movs	r0, #10
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a540
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a540
	movs	r1, #128
	movs	r0, #11
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200a540
	movs	r1, #0
	movs	r2, #20
	movs	r0, #8
	bl 0x0200a530
	bl 0x0200a580
	bl 0x0200a588
	movs	r0, #123
	bl 0x0200a5a0
	movs	r0, #40
	bl 0x0200a480
	movs	r0, #15
	bl 0x0200a578
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00002263
	.4byte 0x00026666
	.2byte 0x3333
	.2byte 0x0001
	push	{lr}
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200a438
	cmp	r0, #0
	beq.n	.L_02002274
	movs	r0, #9
	bl 0x0200a4a0
	movs	r3, #168
	lsls	r3, r3, #16
	str	r3, [r0, #8]
	movs	r0, #9
	bl 0x0200a4a0
	movs	r3, #0
	str	r3, [r0, #12]
	movs	r0, #9
	bl 0x0200a4a0
	movs	r3, #192
	lsls	r3, r3, #16
	str	r3, [r0, #16]
.L_02002274:
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	ldr	r1, [pc, #44]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_020022a4
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #15
	bne.n	.L_020022a0
	bl 0x0200a3c8
	b.n	.L_020022a4
.L_020022a0:
	bl 0x0200a244
.L_020022a4:
	movs	r0, #0
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x008b
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #56]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #48]
	sub	sp, #8
	cmp	r2, r3
	bne.n	.L_020022e6
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200a438
	cmp	r0, #0
	beq.n	.L_020022e6
	movs	r3, #10
	movs	r2, #12
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #8
	movs	r1, #12
	movs	r2, #1
	movs	r3, #1
	bl 0x0200a470
.L_020022e6:
	movs	r0, #0
	add	sp, #8
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000008b
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #9
	sub	sp, #8
	bl 0x0200a4a0
	adds	r6, r0, #0
	ldr	r2, [r6, #80]
	movs	r0, #192
	lsls	r0, r0, #2
	mov	r8, r2
	bl 0x0200a440
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r6, #52]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r6, #48]
	movs	r0, #227
	bl 0x0200a5a0
	movs	r1, #176
	movs	r2, #160
	movs	r3, #184
	adds	r0, r6, #0
	lsls	r1, r1, #16
	lsls	r2, r2, #14
	lsls	r3, r3, #16
	bl 0x0200a450
	movs	r7, #0
	movs	r5, #15
.L_0200233c:
	mov	r2, r8
	ldrh	r3, [r2, #18]
	movs	r0, #1
	subs	r3, r3, r7
	strh	r3, [r2, #18]
	subs	r5, #1
	bl 0x0200a418
	adds	r7, #40
	cmp	r5, #0
	bge.n	.L_0200233c
	movs	r1, #168
	movs	r2, #128
	movs	r3, #184
	adds	r0, r6, #0
	lsls	r1, r1, #16
	lsls	r2, r2, #13
	lsls	r3, r3, #16
	bl 0x0200a450
	movs	r7, #200
	lsls	r7, r7, #1
	movs	r5, #25
.L_0200236a:
	mov	r2, r8
	ldrh	r3, [r2, #18]
	movs	r0, #1
	subs	r3, r3, r7
	strh	r3, [r2, #18]
	subs	r5, #1
	bl 0x0200a418
	adds	r7, #40
	cmp	r5, #0
	bge.n	.L_0200236a
	adds	r0, r6, #0
	bl 0x0200a458
	movs	r0, #2
	bl 0x0200a480
	movs	r0, #240
	bl 0x0200a5a0
	movs	r5, #0
	mov	r3, r8
	strh	r5, [r3, #18]
	movs	r3, #168
	lsls	r3, r3, #16
	str	r3, [r6, #8]
	movs	r3, #192
	lsls	r3, r3, #16
	str	r3, [r6, #16]
	movs	r2, #12
	movs	r3, #10
	str	r5, [r6, #12]
	str	r5, [r6, #40]
	str	r5, [r6, #36]
	movs	r0, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #12
	movs	r2, #1
	movs	r3, #1
	bl 0x0200a470
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r0, #9
	bl 0x0200a4a0
	adds	r1, r0, #0
	adds	r2, r1, #0
	movs	r3, #0
	ldr	r5, [pc, #48]
	adds	r2, #100
	ldr	r6, [pc, #40]
	mov	r8, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	adds	r3, r1, #0
	adds	r3, #99
	strb	r6, [r3, #0]
	movs	r0, #10
	str	r5, [r1, #108]
	bl 0x0200a4a0
	adds	r2, r0, #0
	adds	r3, r2, #0
	adds	r3, #100
	mov	r1, r8
	strh	r1, [r3, #0]
	subs	r3, #1
	strb	r6, [r3, #0]
	str	r5, [r2, #108]
	b.n	.L_02002410
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0x814d
	.2byte 0x0200
.L_02002410:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	.irp EntryTarget, 0x080000c1, 0x080000d1, 0x080000d9, 0x08000101, 0x080003c9, 0x080003d1, 0x08020091, 0x08020149, 0x08020151, 0x08020171, 0x08020179, 0x080201e9, 0x08020219, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80b1, 0x080c80b9, 0x080c80c1, 0x080c80c9, 0x080c80d9, 0x080c80e1, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8129, 0x080c8139, 0x080c8159, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81a1, 0x080c81d1, 0x080c81d9, 0x080c8219, 0x080c8229, 0x080c8231, 0x080c8239, 0x080c8259, 0x080c8279, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c8761, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
	.4byte 0x00000027
	.4byte 0x00000003
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00020000
	.4byte 0x0000002e
	.4byte 0x02008045
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xfff00000
	.4byte 0x0000002e
	.4byte 0x02008045
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xfff00000
	.4byte 0x0000002e
	.4byte 0x02008045
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00006666
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x02f00000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x02f00000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00006666
	.4byte 0x00000004
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x02f00000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x02f00000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x016a0000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01800000
	.4byte 0x00000000
	.4byte 0x03500000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x017e0000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x016c0000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01760000
	.4byte 0x00000000
	.4byte 0x036e0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01120000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x03580000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x036a0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
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
	.4byte 0x0000008b
	.4byte 0x0010108d
	.4byte 0x0020208d
	.4byte 0x0030108e
	.4byte 0x0040408d
	.4byte 0x0050308e
	.4byte 0x0060208e
	.4byte 0x0070308d
	.4byte 0x0080403a
	.4byte 0x0090608c
	.4byte 0x00a22002
	.4byte 0x00b21002
	.4byte 0x00c0f08b
	.4byte 0x00d0708c
	.4byte 0x00f0c08b
	.4byte 0x0000008c
	.4byte 0x0060908b
	.4byte 0x0070d08b
	.4byte 0x000001ff
	.4byte 0xffff0146
	.4byte 0x0200a5d4
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00008000
	.4byte 0xffff0146
	.4byte 0x0200a5a8
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00008000
	.4byte 0xffff0147
	.4byte 0x0200a5f8
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00008000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00032000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00014000
	.4byte 0xffff0072
	.4byte 0x00000003
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00016000
	.4byte 0xffff0080
	.4byte 0x00000002
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x0001a000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00012000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0001e000
	.4byte 0xffff00e2
	.4byte 0x00000001
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00016000
	.4byte 0xffff00e2
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x00018000
	.4byte 0xffff0088
	.4byte 0x0200a634
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x0002c000
	.4byte 0xffff008a
	.4byte 0x0200a6d8
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00024000
	.4byte 0xffff008a
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x0000b000
	.4byte 0xffff0005
	.4byte 0x00000001
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
	.4byte 0x001c005d
	.4byte 0x00020001
	.4byte 0x005c0006
	.4byte 0x0001001c
	.4byte 0x00060002
	.4byte 0x005fffff
	.4byte 0x0001001c
	.4byte 0x00060002
	.4byte 0x001c005e
	.4byte 0x00020001
	.4byte 0xffff0006
	.4byte 0x001c0059
	.4byte 0x00020001
	.4byte 0x005a0006
	.4byte 0x0001001c
	.4byte 0x00060002
	.4byte 0x005cffff
	.4byte 0x0002001e
	.4byte 0x00060002
	.4byte 0x001e005e
	.4byte 0x00020002
	.4byte 0xffff0006
	.4byte 0x0200ac8c
	.4byte 0x00130062
	.4byte 0x0200aca2
	.4byte 0x0014005c
	.4byte 0x0200acb8
	.4byte 0x00120054
	.4byte 0x0200ac8c
	.4byte 0x0007004f
	.4byte 0x0200acb8
	.4byte 0x00100050
	.4byte 0x0200acb8
	.4byte 0x0011004d
	.4byte 0x0200aca2
	.4byte 0x00080063
	.4byte 0x0200acce
	.4byte 0x0006005a
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008271
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008271
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008271
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008271
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008271
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008271
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008271
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008271
	.4byte 0x0000c401
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00004401
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x50008905
	.4byte 0xffff0014
	.4byte 0x02008329
	.4byte 0x50008905
	.4byte 0xffff0015
	.4byte 0x0200836d
	.4byte 0x50008905
	.4byte 0xffff0016
	.4byte 0x0200838d
	.4byte 0x50008905
	.4byte 0xffff0017
	.4byte 0x0200841d
	.4byte 0x50008905
	.4byte 0xffff0018
	.4byte 0x0200843d
	.4byte 0x50008905
	.4byte 0xffff0019
	.4byte 0x020084cd
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000021f9
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000021fa
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000021fb
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000021fc
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000021fd
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000021fe
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000021ff
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00002200
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002201
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002202
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002203
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002204
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002205
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002206
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002207
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00002208
	.4byte 0x10008e15
	.4byte 0x03000008
	.4byte 0x0200a2f5
	.4byte 0x00008e15
	.4byte 0x03000008
	.4byte 0x0200a2f9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x0000c401
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000400
	.4byte 0xffff0008
	.4byte 0x02008545
	.4byte 0x00002400
	.4byte 0xffff0008
	.4byte 0x02008545
	.4byte 0x0000e400
	.4byte 0xffff0008
	.4byte 0x02008545
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte 0x02008545
	.4byte 0x00004400
	.4byte 0xffff0008
	.4byte 0x02008545
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002244
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008585
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x02008585
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008615
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x02008615
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002243
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002247
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
