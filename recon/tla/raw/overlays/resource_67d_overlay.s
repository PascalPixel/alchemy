.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x0200a00d, 0x020080a1, 0x020080ad, 0x020080b5, 0x020080f1, 0x020080a9, 0x0200a191
	overlay_veneer \EntryTarget
	.endr
	push	{lr}
	movs	r0, #10
	bl 0x0200a1dc
	ldr	r2, [pc, #36]
	ldr	r3, [r0, #8]
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	cmp	r3, r2
	bhi.n	.L_02000064
	ldr	r0, [r0, #16]
	movs	r3, #155
	lsls	r3, r3, #18
	cmp	r0, r3
	blt.n	.L_02000064
	movs	r2, #157
	lsls	r2, r2, #18
	cmp	r0, r2
	bgt.n	.L_02000064
	movs	r0, #0
	b.n	.L_02000066
.L_02000064:
	movs	r0, #1
.L_02000066:
	pop	{pc}
	.2byte 0x0000
	.2byte 0xff6c
	.2byte 0xb500
	movs	r0, #9
	bl 0x0200a1dc
	ldr	r2, [pc, #36]
	ldr	r3, [r0, #8]
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	cmp	r3, r2
	bhi.n	.L_02000098
	ldr	r0, [r0, #16]
	movs	r3, #155
	lsls	r3, r3, #18
	cmp	r0, r3
	blt.n	.L_02000098
	movs	r2, #157
	lsls	r2, r2, #18
	cmp	r0, r2
	bgt.n	.L_02000098
	movs	r0, #0
	b.n	.L_0200009a
.L_02000098:
	movs	r0, #1
.L_0200009a:
	pop	{pc}
	.2byte 0x0000
	.2byte 0xff7c
	.2byte 0x4800
	bx	lr
	.2byte 0xaad0
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xab00
	.2byte 0x0200
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x0200a194
	cmp	r0, #0
	beq.n	.L_020000c8
	ldr	r0, [pc, #24]
	b.n	.L_020000de
.L_020000c8:
	ldr	r3, [pc, #24]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #7
	bne.n	.L_020000dc
	ldr	r0, [pc, #12]
	b.n	.L_020000de
.L_020000dc:
	ldr	r0, [pc, #12]
.L_020000de:
	pop	{pc}
	.4byte 0x0200ad80
	.4byte 0x02000240
	.4byte 0x0200acf0
	.2byte 0xab40
	.2byte 0x0200
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x0200a194
	cmp	r0, #0
	beq.n	.L_02000104
	ldr	r0, [pc, #24]
	b.n	.L_0200011a
.L_02000104:
	ldr	r3, [pc, #24]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #7
	bne.n	.L_02000118
	ldr	r0, [pc, #12]
	b.n	.L_0200011a
.L_02000118:
	ldr	r0, [pc, #12]
.L_0200011a:
	pop	{pc}
	.4byte 0x0200b23c
	.4byte 0x02000240
	.4byte 0x0200b164
	.2byte 0xaf78
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200a1dc
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #20]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_02000164
	adds	r0, r5, #0
	bl 0x0200a30c
	b.n	.L_0200019e
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000164:
	bl 0x0200a1c4
	movs	r0, #0
	bl 0x0200a2bc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x0200a194
	cmp	r0, #0
	beq.n	.L_0200018c
	ldr	r0, [pc, #32]
	bl 0x0200a244
	movs	r0, #12
	movs	r1, #0
	bl 0x0200a254
	b.n	.L_0200019a
.L_0200018c:
	ldr	r0, [pc, #20]
	bl 0x0200a244
	movs	r0, #22
	movs	r1, #0
	bl 0x0200a254
.L_0200019a:
	bl 0x0200a1cc
.L_0200019e:
	pop	{r5, pc}
	.4byte 0x000023df
	.2byte 0x22ef
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200a1dc
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_020001e4
	movs	r0, #9
	adds	r1, r5, #0
	bl 0x0200a314
	b.n	.L_02000216
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_020001e4:
	bl 0x0200a1c4
	movs	r0, #0
	bl 0x0200a2bc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x0200a194
	cmp	r0, #0
	beq.n	.L_02000204
	ldr	r0, [pc, #24]
	bl 0x0200a244
	b.n	.L_0200020a
.L_02000204:
	ldr	r0, [pc, #20]
	bl 0x0200a244
.L_0200020a:
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200a254
	bl 0x0200a1cc
.L_02000216:
	pop	{r5, pc}
	.4byte 0x000023db
	.2byte 0x22eb
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200a1dc
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_0200025c
	movs	r0, #24
	adds	r1, r5, #0
	bl 0x0200a304
	b.n	.L_0200028e
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_0200025c:
	bl 0x0200a1c4
	movs	r0, #0
	bl 0x0200a2bc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x0200a194
	cmp	r0, #0
	beq.n	.L_0200027c
	ldr	r0, [pc, #24]
	bl 0x0200a244
	b.n	.L_02000282
.L_0200027c:
	ldr	r0, [pc, #20]
	bl 0x0200a244
.L_02000282:
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200a254
	bl 0x0200a1cc
.L_0200028e:
	pop	{r5, pc}
	.4byte 0x000023d7
	.2byte 0x22e7
	.2byte 0x0000
	push	{lr}
	bl 0x0200a1c4
	movs	r0, #0
	bl 0x0200a2bc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #34
	bl 0x0200a194
	cmp	r0, #0
	bne.n	.L_0200032c
	ldr	r0, [pc, #156]
	bl 0x0200a244
	movs	r1, #0
	movs	r0, #8
	bl 0x0200a254
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #10
	movs	r2, #0
	adds	r1, #255
	movs	r0, #8
	bl 0x0200a26c
	movs	r0, #60
	bl 0x0200a1bc
	movs	r1, #0
	movs	r0, #8
	bl 0x0200a254
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #6
	movs	r2, #0
	adds	r1, #255
	movs	r0, #8
	bl 0x0200a26c
	movs	r0, #60
	bl 0x0200a1bc
	movs	r1, #0
	movs	r0, #8
	bl 0x0200a254
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #1
	movs	r0, #8
	bl 0x0200a22c
	movs	r0, #60
	bl 0x0200a1bc
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a254
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #34
	bl 0x0200a19c
.L_0200032c:
	movs	r1, #4
	movs	r0, #8
	bl 0x0200a21c
	movs	r0, #60
	bl 0x0200a1bc
	ldr	r0, [pc, #24]
	bl 0x0200a244
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a254
	bl 0x0200a1cc
	pop	{pc}
	.2byte 0x0000
	.4byte 0x000022d9
	.2byte 0x22dd
	.2byte 0x0000
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #35
	bl 0x0200a19c
	ldr	r0, [pc, #12]
	bl 0x0200a244
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a254
	pop	{pc}
	.2byte 0x22e2
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	bl 0x0200a1c4
	movs	r0, #0
	bl 0x0200a2bc
	ldr	r3, [pc, #104]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r0, #15
	ldr	r1, [r3, #0]
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #44
	bl 0x0200a194
	cmp	r0, #0
	beq.n	.L_020003ba
	ldr	r0, [pc, #76]
	bl 0x0200a244
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a254
	b.n	.L_020003e2
.L_020003ba:
	ldr	r0, [pc, #64]
	bl 0x0200a244
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a254
	movs	r0, #196
	adds	r0, #255
	bl 0x0200a1ac
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_020003e2
	movs	r3, #173
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #1
	strh	r3, [r2, #0]
.L_020003e2:
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a25c
	bl 0x0200a1cc
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x000023d5
	.2byte 0x23a8
	.2byte 0x0000
	push	{r5, lr}
	bl 0x0200a1c4
	movs	r0, #0
	bl 0x0200a2bc
	ldr	r5, [pc, #68]
	adds	r0, r5, #0
	bl 0x0200a244
	movs	r1, #0
	movs	r0, #16
	bl 0x0200a24c
	bl 0x0200a2dc
	movs	r1, #0
	bl 0x0200a1d4
	cmp	r0, #0
	bne.n	.L_02000438
	movs	r0, #10
	bl 0x0200a1bc
	adds	r0, r5, #1
	bl 0x0200a244
	b.n	.L_02000444
.L_02000438:
	movs	r0, #20
	bl 0x0200a1bc
	adds	r0, r5, #2
	bl 0x0200a244
.L_02000444:
	movs	r0, #16
	movs	r1, #0
	bl 0x0200a254
	bl 0x0200a1cc
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x23c1
	.2byte 0x0000
	push	{r5, lr}
	bl 0x0200a1c4
	movs	r0, #0
	bl 0x0200a2bc
	ldr	r3, [pc, #168]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	movs	r0, #15
	ldr	r1, [r5, #0]
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #43
	bl 0x0200a194
	cmp	r0, #0
	beq.n	.L_02000494
	ldr	r0, [pc, #140]
	bl 0x0200a244
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a254
	b.n	.L_020004fc
.L_02000494:
	ldr	r0, [pc, #128]
	bl 0x0200a244
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a254
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #35
	bl 0x0200a194
	cmp	r0, #0
	beq.n	.L_020004f2
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	ldr	r0, [r5, #0]
	bl 0x0200a26c
	movs	r1, #4
	movs	r2, #30
	adds	r1, #255
	movs	r0, #15
	bl 0x0200a26c
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200a26c
	movs	r1, #4
	movs	r0, #15
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a254
.L_020004f2:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #43
	bl 0x0200a19c
.L_020004fc:
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a25c
	bl 0x0200a1cc
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000023b0
	.2byte 0x23ad
	.2byte 0x0000
	push	{r5, lr}
	bl 0x0200a1c4
	movs	r0, #0
	bl 0x0200a2bc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #37
	bl 0x0200a194
	cmp	r0, #0
	bne.n	.L_020005a8
	ldr	r0, [pc, #192]
	bl 0x0200a244
	movs	r1, #0
	movs	r0, #17
	bl 0x0200a254
	movs	r0, #60
	bl 0x0200a1bc
	movs	r1, #0
	movs	r0, #17
	bl 0x0200a254
	movs	r0, #60
	bl 0x0200a1bc
	movs	r0, #15
	bl 0x0200a1bc
	movs	r1, #0
	movs	r0, #17
	bl 0x0200a254
	movs	r0, #60
	bl 0x0200a1bc
	movs	r0, #15
	bl 0x0200a1bc
	movs	r1, #0
	movs	r0, #17
	bl 0x0200a254
	movs	r0, #60
	bl 0x0200a1bc
	movs	r1, #1
	movs	r0, #17
	bl 0x0200a22c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #17
	movs	r1, #0
	bl 0x0200a254
	movs	r0, #60
	bl 0x0200a1bc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #37
	bl 0x0200a19c
	b.n	.L_020005b6
.L_020005a8:
	ldr	r0, [pc, #80]
	bl 0x0200a244
	movs	r0, #17
	movs	r1, #0
	bl 0x0200a254
.L_020005b6:
	ldr	r5, [pc, #72]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200a1dc
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #0
	movs	r2, #16
	ldr	r0, [r5, #0]
	bl 0x0200a2d4
	ldr	r0, [r5, #0]
	bl 0x0200a20c
	ldr	r0, [r5, #0]
	bl 0x0200a1dc
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	bl 0x0200a1cc
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x000022d3
	.4byte 0x000022d8
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #0
	bl 0x0200861c
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #1
	bl 0x0200861c
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	bl 0x0200a1c4
	movs	r0, #0
	bl 0x0200a2bc
	movs	r0, #143
	movs	r1, #1
	movs	r2, #140
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #18
	bl 0x0200a284
	ldr	r3, [pc, #752]
	cmp	r7, #0
	bne.n	.L_02000654
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #184
	adds	r2, #36
	bl 0x0200a1fc
	b.n	.L_02000664
.L_02000654:
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #104
	adds	r2, #36
	bl 0x0200a1fc
.L_02000664:
	movs	r0, #15
	bl 0x0200a1bc
	ldr	r0, [pc, #712]
	bl 0x0200a244
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #15
	movs	r1, #0
	bl 0x0200a254
	movs	r0, #22
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #22
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #15
	bl 0x0200a26c
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r1, #0
	adds	r0, #15
	bl 0x0200a254
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #3
	movs	r0, #22
	bl 0x0200a21c
	movs	r0, #15
	bl 0x0200a1bc
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #22
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #4
	movs	r2, #30
	adds	r1, #255
	movs	r0, #15
	bl 0x0200a26c
	movs	r0, #22
	movs	r1, #3
	bl 0x0200a22c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #22
	bl 0x0200a274
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #22
	movs	r1, #0
	bl 0x0200a254
	movs	r2, #30
	movs	r0, #20
	movs	r1, #0
	bl 0x0200a25c
	movs	r0, #20
	movs	r1, #4
	bl 0x0200a21c
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r1, #0
	adds	r0, #20
	bl 0x0200a254
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #128
	movs	r2, #30
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200a25c
	movs	r1, #3
	movs	r0, #15
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #15
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #3
	movs	r0, #22
	bl 0x0200a21c
	movs	r0, #15
	bl 0x0200a1bc
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #22
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #2
	movs	r0, #20
	bl 0x0200a22c
	movs	r0, #15
	bl 0x0200a1bc
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #20
	movs	r1, #0
	bl 0x0200a254
	ldr	r3, [pc, #440]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #15
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #0
	movs	r2, #16
	movs	r0, #15
	bl 0x0200a204
	movs	r0, #15
	bl 0x0200a20c
	movs	r0, #15
	bl 0x0200a1bc
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #1
	movs	r2, #30
	bl 0x0200a26c
	cmp	r7, #0
	bne.n	.L_020007c2
	movs	r2, #134
	movs	r0, #15
	movs	r1, #168
	lsls	r2, r2, #2
	bl 0x0200a1fc
	b.n	.L_020007ce
.L_020007c2:
	movs	r2, #134
	movs	r0, #15
	movs	r1, #120
	lsls	r2, r2, #2
	bl 0x0200a1fc
.L_020007ce:
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a25c
	ldr	r5, [pc, #340]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #20
	bl 0x0200a234
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #15
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #8
	movs	r2, #0
	adds	r1, #255
	movs	r0, #15
	bl 0x0200a26c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #15
	movs	r1, #0
	bl 0x0200a254
	movs	r2, #0
	movs	r0, #22
	movs	r1, #4
	bl 0x0200a224
	movs	r1, #2
	adds	r1, #255
	movs	r0, #22
	bl 0x0200a274
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #128
	movs	r0, #20
	lsls	r1, r1, #7
	movs	r2, #30
	bl 0x0200a25c
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #20
	bl 0x0200a26c
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #20
	movs	r1, #0
	bl 0x0200a254
	movs	r2, #0
	movs	r0, #22
	movs	r1, #20
	bl 0x0200a234
	movs	r1, #128
	movs	r0, #22
	lsls	r1, r1, #1
	bl 0x0200a274
	movs	r1, #0
	movs	r0, #22
	bl 0x0200a254
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #15
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #2
	movs	r0, #20
	bl 0x0200a22c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #20
	bl 0x0200a234
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #20
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #132
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #15
	bl 0x0200a26c
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #15
	movs	r1, #0
	bl 0x0200a254
	movs	r2, #130
	movs	r1, #144
	lsls	r2, r2, #2
	movs	r0, #15
	bl 0x0200a1f4
	movs	r0, #15
	bl 0x0200a20c
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a25c
	movs	r1, #128
	movs	r0, #20
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a25c
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	cmp	r7, #0
	bne.n	.L_0200090e
	movs	r2, #142
	movs	r0, #22
	movs	r1, #104
	lsls	r2, r2, #2
	bl 0x0200a1f4
	b.n	.L_0200091a
.L_0200090e:
	movs	r2, #142
	movs	r0, #22
	movs	r1, #184
	lsls	r2, r2, #2
	bl 0x0200a1f4
.L_0200091a:
	movs	r0, #22
	bl 0x0200a20c
	cmp	r7, #0
	bne.n	.L_02000938
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a25c
	b.n	.L_02000944
	.4byte 0x02000240
	.2byte 0x2346
	.2byte 0x0000
.L_02000938:
	movs	r1, #128
	movs	r0, #22
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a25c
.L_02000944:
	ldr	r3, [pc, #1012]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	movs	r1, #144
	adds	r2, #28
	ldr	r0, [r5, #0]
	bl 0x0200a1f4
	ldr	r0, [r5, #0]
	bl 0x0200a20c
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a25c
	movs	r3, #192
	movs	r0, #5
	movs	r1, #8
	movs	r2, #24
	lsls	r3, r3, #8
	bl 0x0200a2cc
	movs	r3, #192
	movs	r0, #6
	movs	r1, #24
	movs	r2, #24
	lsls	r3, r3, #8
	bl 0x0200a2cc
	movs	r1, #8
	movs	r3, #192
	movs	r0, #7
	negs	r1, r1
	movs	r2, #24
	lsls	r3, r3, #8
	bl 0x0200a2cc
	movs	r1, #24
	movs	r3, #192
	lsls	r3, r3, #8
	movs	r2, #24
	negs	r1, r1
	movs	r0, #23
	bl 0x0200a2cc
	movs	r0, #60
	bl 0x0200a1bc
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #20
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #3
	movs	r0, #23
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #23
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #3
	ldr	r0, [r5, #0]
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #15
	movs	r1, #3
	bl 0x0200a21c
	movs	r1, #3
	movs	r0, #20
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r2, #0
	movs	r1, #7
	movs	r0, #23
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #23
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #3
	movs	r0, #7
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a254
	movs	r0, #15
	movs	r1, #3
	bl 0x0200a21c
	movs	r1, #3
	movs	r0, #20
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #3
	movs	r0, #5
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #3
	movs	r0, #6
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #15
	movs	r1, #3
	bl 0x0200a21c
	movs	r1, #3
	movs	r0, #20
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r2, #0
	movs	r0, #5
	movs	r1, #23
	bl 0x0200a234
	movs	r1, #3
	movs	r0, #23
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #23
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #3
	movs	r0, #20
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #20
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #3
	movs	r0, #6
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #15
	bl 0x0200a26c
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #15
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #20
	movs	r2, #0
	movs	r0, #15
	bl 0x0200a23c
	movs	r0, #60
	bl 0x0200a1bc
	ldr	r1, [r5, #0]
	movs	r0, #15
	movs	r2, #0
	bl 0x0200a234
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #20
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #4
	movs	r0, #20
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #20
	movs	r1, #0
	bl 0x0200a254
	ldr	r0, [r5, #0]
	movs	r1, #23
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #5
	movs	r1, #23
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #6
	movs	r1, #23
	movs	r2, #0
	bl 0x0200a234
	movs	r1, #23
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	ldr	r1, [r5, #0]
	movs	r2, #0
	movs	r0, #23
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r2, #0
	movs	r1, #15
	movs	r0, #23
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #23
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #15
	bl 0x0200a26c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #20
	bl 0x0200a26c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #15
	movs	r1, #0
	bl 0x0200a254
	ldr	r0, [r5, #0]
	movs	r1, #15
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #5
	movs	r1, #15
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #6
	movs	r1, #15
	movs	r2, #0
	bl 0x0200a234
	movs	r2, #0
	movs	r1, #15
	movs	r0, #7
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #7
	movs	r1, #3
	bl 0x0200a21c
	movs	r1, #3
	movs	r0, #23
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #15
	bl 0x0200a26c
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r1, #0
	adds	r0, #15
	bl 0x0200a254
	movs	r0, #60
	bl 0x0200a1bc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #15
	bl 0x0200a25c
	movs	r0, #30
	bl 0x0200a1bc
	bl 0x02009d8c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #128
	movs	r2, #0
.L_02000c32:
	lsls	r1, r1, #7
	movs	r0, #15
	bl 0x0200a25c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #15
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200a26c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a26c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #6
	bl 0x0200a26c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a26c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #23
	bl 0x0200a26c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #1
	movs	r0, #5
	bl 0x0200a22c
	movs	r0, #15
	bl 0x0200a1bc
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #20
	movs	r2, #0
	movs	r0, #15
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #15
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #15
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #20
	bl 0x0200a26c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r1, #0
	adds	r0, #20
	bl 0x0200a24c
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200a1d4
	cmp	r0, #1
	bne.n	.L_02000d5e
	ldr	r0, [pc, #68]
	bl 0x0200a244
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #20
	bl 0x0200a26c
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #20
	movs	r1, #0
	bl 0x0200a254
	movs	r0, #6
	movs	r1, #3
	bl 0x0200a21c
	movs	r1, #3
	movs	r0, #5
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #7
	bl 0x0200a234
	movs	r0, #15
	b.n	.L_02000d44
	.4byte 0x02000240
	.2byte 0x2367
	.2byte 0x0000
.L_02000d44:
	bl 0x0200a1bc
	movs	r1, #4
	movs	r0, #7
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a254
.L_02000d5e:
	ldr	r0, [pc, #872]
	bl 0x0200a244
	movs	r1, #3
	movs	r0, #20
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #20
	movs	r1, #0
	bl 0x0200a254
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #7
	movs	r1, #3
	bl 0x0200a21c
	movs	r1, #3
	movs	r0, #23
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #15
	movs	r1, #3
	bl 0x0200a21c
	movs	r1, #3
	movs	r0, #20
	bl 0x0200a21c
	movs	r0, #60
	bl 0x0200a1bc
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #20
	bl 0x0200a26c
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #20
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r1, #0
	adds	r0, #20
	bl 0x0200a254
	movs	r0, #90
	bl 0x0200a1bc
	movs	r0, #21
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #144
	movs	r2, #170
	movs	r0, #21
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	bl 0x0200a214
	ldr	r0, [r5, #0]
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #5
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #6
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #7
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #23
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r2, #0
	movs	r1, #21
	movs	r0, #22
	bl 0x0200a234
	movs	r0, #120
	bl 0x0200a1bc
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r1, #0
	adds	r0, #15
	bl 0x0200a254
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #1
	movs	r0, #21
	bl 0x0200a27c
	movs	r0, #120
	bl 0x0200a1bc
	movs	r1, #153
	movs	r2, #152
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #21
	adds	r1, #153
	adds	r2, #204
	bl 0x0200a1e4
	movs	r1, #153
	movs	r2, #152
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r1, #153
	adds	r2, #204
	movs	r0, #21
	bl 0x0200a1e4
	movs	r0, #21
	bl 0x0200a1dc
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r6, #254
	adds	r3, r6, #0
	ands	r3, r2
	movs	r2, #162
	strb	r3, [r0, #0]
	movs	r1, #144
	movs	r0, #21
	lsls	r2, r2, #2
	bl 0x0200a1f4
	cmp	r7, #0
	beq.n	.L_02000eb0
	b.n	.L_020010cc
.L_02000eb0:
	movs	r2, #162
	movs	r0, #21
	movs	r1, #88
	lsls	r2, r2, #2
	bl 0x0200a1fc
	ldr	r0, [r5, #0]
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #5
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #6
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #7
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #23
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #15
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #20
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #22
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r2, #154
	movs	r0, #21
	movs	r1, #56
	lsls	r2, r2, #2
	bl 0x0200a1fc
	ldr	r0, [r5, #0]
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #5
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #6
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #7
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #23
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #15
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #20
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #22
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r2, #146
	movs	r1, #56
	lsls	r2, r2, #2
	movs	r0, #21
	bl 0x0200a1fc
	movs	r0, #22
	bl 0x0200a1dc
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r6, #0
	ands	r3, r2
	movs	r2, #142
	strb	r3, [r0, #0]
	movs	r1, #88
	lsls	r2, r2, #2
	movs	r0, #22
	bl 0x0200a1fc
	movs	r0, #1
	bl 0x0200a1bc
	movs	r0, #22
	bl 0x0200a1dc
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r2, #0
	movs	r0, #22
	movs	r1, #0
	bl 0x0200a25c
	movs	r0, #21
	movs	r1, #1
	bl 0x0200a264
	movs	r2, #142
	movs	r0, #21
	movs	r1, #104
	lsls	r2, r2, #2
	bl 0x0200a1fc
	ldr	r0, [r5, #0]
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #5
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #6
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #7
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #23
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #15
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #20
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r2, #0
	movs	r0, #22
	movs	r1, #21
	bl 0x0200a234
	movs	r0, #23
	movs	r1, #1
	bl 0x0200a264
	movs	r0, #21
	movs	r1, #2
	bl 0x0200a264
	movs	r2, #142
	lsls	r2, r2, #2
	movs	r0, #21
	movs	r1, #120
	bl 0x0200a1fc
	movs	r0, #21
	movs	r1, #3
	bl 0x0200a264
	movs	r0, #23
	movs	r1, #3
	bl 0x0200a264
	movs	r2, #134
	movs	r0, #21
	movs	r1, #120
	lsls	r2, r2, #2
	bl 0x0200a1fc
	ldr	r0, [r5, #0]
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #5
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #6
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #7
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #23
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #15
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #20
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r2, #0
	movs	r0, #22
	movs	r1, #21
	bl 0x0200a234
	movs	r0, #22
	movs	r1, #3
	bl 0x0200a21c
	movs	r2, #146
	movs	r0, #22
	movs	r1, #56
	lsls	r2, r2, #2
	bl 0x0200a1fc
	movs	r2, #162
	movs	r0, #22
	movs	r1, #88
	lsls	r2, r2, #2
	bl 0x0200a1fc
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a214
	b.n	.L_020012e4
	.2byte 0x2369
	.2byte 0x0000
.L_020010cc:
	movs	r2, #162
	movs	r0, #21
	movs	r1, #200
	lsls	r2, r2, #2
	bl 0x0200a1fc
	ldr	r0, [r5, #0]
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #5
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #6
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #7
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #23
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #15
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #20
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #22
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r2, #154
	movs	r0, #21
	movs	r1, #232
	lsls	r2, r2, #2
	bl 0x0200a1fc
	ldr	r0, [r5, #0]
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #5
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #6
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #7
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #23
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #15
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #20
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #22
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r2, #146
	movs	r1, #232
	lsls	r2, r2, #2
	movs	r0, #21
	bl 0x0200a1fc
	movs	r0, #22
	bl 0x0200a1dc
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r6, #0
	ands	r3, r2
	movs	r2, #142
	strb	r3, [r0, #0]
	movs	r1, #200
	lsls	r2, r2, #2
	movs	r0, #22
	bl 0x0200a1fc
	movs	r0, #1
	bl 0x0200a1bc
	movs	r0, #22
	bl 0x0200a1dc
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
	movs	r1, #128
	strb	r3, [r0, #0]
	movs	r2, #0
	movs	r0, #22
	lsls	r1, r1, #8
	bl 0x0200a25c
	movs	r0, #21
	movs	r1, #1
	bl 0x0200a264
	movs	r2, #142
	movs	r0, #21
	movs	r1, #184
	lsls	r2, r2, #2
	bl 0x0200a1fc
	ldr	r0, [r5, #0]
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #5
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #6
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #7
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #23
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #15
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #20
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r2, #0
	movs	r0, #22
	movs	r1, #21
	bl 0x0200a234
	movs	r0, #21
	movs	r1, #2
	bl 0x0200a264
	movs	r0, #6
	movs	r1, #1
	bl 0x0200a264
	movs	r2, #142
	lsls	r2, r2, #2
	movs	r0, #21
	movs	r1, #168
	bl 0x0200a1fc
	movs	r0, #21
	movs	r1, #3
	bl 0x0200a264
	movs	r0, #6
	movs	r1, #3
	bl 0x0200a264
	movs	r2, #134
	movs	r0, #21
	movs	r1, #168
	lsls	r2, r2, #2
	bl 0x0200a1fc
	ldr	r0, [r5, #0]
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #5
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #6
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #7
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #23
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #15
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #20
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r2, #0
	movs	r0, #22
	movs	r1, #21
	bl 0x0200a234
	movs	r0, #22
	movs	r1, #3
	bl 0x0200a21c
	movs	r2, #146
	movs	r0, #22
	movs	r1, #232
	lsls	r2, r2, #2
	bl 0x0200a1fc
	movs	r2, #162
	movs	r0, #22
	movs	r1, #200
	lsls	r2, r2, #2
	bl 0x0200a1fc
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a214
.L_020012e4:
	movs	r0, #21
	bl 0x0200a1dc
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	ldr	r5, [pc, #900]
	strb	r3, [r0, #0]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a25c
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a25c
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a25c
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a25c
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #23
	bl 0x0200a25c
	movs	r0, #15
	bl 0x0200a1bc
	movs	r1, #0
	movs	r0, #20
	bl 0x0200a254
	movs	r0, #15
	bl 0x0200a1bc
	movs	r1, #132
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #20
	bl 0x0200a26c
	movs	r0, #20
	movs	r1, #0
	bl 0x0200a254
	movs	r2, #0
	movs	r1, #20
	movs	r0, #21
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #21
	movs	r1, #0
	bl 0x0200a254
	ldr	r1, [r5, #0]
	movs	r2, #0
	movs	r0, #21
	bl 0x0200a234
	movs	r0, #60
	bl 0x0200a1bc
	movs	r2, #0
	movs	r1, #20
	movs	r0, #21
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #21
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #20
	movs	r2, #0
	movs	r0, #15
	bl 0x0200a23c
	movs	r0, #30
	bl 0x0200a1bc
	ldr	r1, [r5, #0]
	movs	r0, #15
	movs	r2, #0
	bl 0x0200a234
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #20
	bl 0x0200a234
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a254
	movs	r0, #15
	movs	r1, #3
	bl 0x0200a21c
	movs	r1, #3
	movs	r0, #20
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #7
	movs	r1, #3
	bl 0x0200a21c
	movs	r1, #3
	movs	r0, #23
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #15
	bl 0x0200a26c
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a254
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #21
	bl 0x0200a234
	movs	r0, #21
	movs	r1, #0
	bl 0x0200a254
	ldr	r1, [r5, #0]
	movs	r0, #15
	movs	r2, #0
	bl 0x0200a234
	ldr	r1, [r5, #0]
	movs	r2, #0
	movs	r0, #20
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200a26c
	movs	r1, #3
	movs	r0, #23
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #23
	movs	r1, #0
	bl 0x0200a254
	movs	r0, #15
	movs	r1, #3
	bl 0x0200a21c
	movs	r1, #3
	movs	r0, #20
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #23
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #23
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #15
	bl 0x0200a26c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #20
	bl 0x0200a26c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #21
	bl 0x0200a26c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #22
	bl 0x0200a26c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #23
	bl 0x0200a26c
	movs	r0, #23
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #21
	bl 0x0200a26c
	movs	r0, #21
	movs	r1, #0
	bl 0x0200a254
	movs	r2, #0
	movs	r1, #21
	movs	r0, #23
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #23
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #131
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #20
	bl 0x0200a26c
	movs	r0, #20
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #132
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #23
	bl 0x0200a26c
	movs	r0, #23
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #3
	movs	r0, #23
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #23
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #15
	bl 0x0200a26c
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a254
	movs	r2, #0
	movs	r1, #23
	movs	r0, #7
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a254
	movs	r2, #0
	movs	r0, #23
	movs	r1, #7
	bl 0x0200a234
	movs	r1, #3
	movs	r0, #23
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #15
	bl 0x0200a26c
	movs	r0, #23
	movs	r1, #15
	movs	r2, #0
	bl 0x0200a234
	movs	r2, #0
	movs	r1, #15
	movs	r0, #7
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #132
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #6
	bl 0x0200a26c
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #21
	bl 0x0200a26c
	movs	r0, #21
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #3
	movs	r0, #5
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #3
	movs	r0, #20
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #20
	movs	r1, #0
	bl 0x0200a254
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #7
	movs	r1, #3
	bl 0x0200a21c
	movs	r1, #3
	movs	r0, #23
	bl 0x0200a21c
	movs	r0, #90
	bl 0x0200a1bc
	movs	r0, #15
	movs	r1, #3
	bl 0x0200a21c
	movs	r1, #0
	movs	r0, #15
	bl 0x0200a24c
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200a1d4
	cmp	r0, #0
	bne.n	.L_02001680
	ldr	r0, [pc, #28]
	bl 0x0200a244
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r0, #15
	movs	r2, #30
	bl 0x0200a26c
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a254
	b.n	.L_0200168e
	.4byte 0x02000240
	.2byte 0x2384
	.2byte 0x0000
.L_02001680:
	ldr	r0, [pc, #708]
	bl 0x0200a244
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a254
.L_0200168e:
	ldr	r6, [pc, #700]
	adds	r0, r6, #0
	bl 0x0200a244
	movs	r1, #1
	movs	r0, #23
	bl 0x0200a22c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #23
	movs	r1, #0
	bl 0x0200a254
	movs	r0, #15
	movs	r1, #23
	movs	r2, #0
	bl 0x0200a234
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #15
	bl 0x0200a26c
	movs	r1, #3
	movs	r0, #23
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #23
	movs	r1, #0
	bl 0x0200a254
	movs	r2, #0
	movs	r1, #21
	movs	r0, #23
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #23
	movs	r1, #0
	bl 0x0200a254
	movs	r2, #0
	movs	r1, #20
	movs	r0, #15
	bl 0x0200a23c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #21
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #3
	movs	r0, #7
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a25c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #20
	bl 0x0200a25c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #3
	movs	r0, #6
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #132
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200a26c
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #15
	bl 0x0200a26c
	movs	r0, #15
	bl 0x0200a1bc
	movs	r2, #0
	movs	r0, #15
	movs	r1, #21
	bl 0x0200a234
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #132
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #21
	bl 0x0200a26c
	movs	r0, #21
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #6
	bl 0x0200a26c
	ldr	r5, [pc, #420]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r1, [r5, #0]
	movs	r2, #0
	movs	r0, #6
	bl 0x0200a234
	movs	r1, #0
	movs	r0, #6
	bl 0x0200a24c
	ldr	r1, [r5, #0]
	movs	r0, #5
	movs	r2, #0
	bl 0x0200a234
	ldr	r1, [r5, #0]
	movs	r0, #6
	movs	r2, #0
	bl 0x0200a234
	ldr	r1, [r5, #0]
	movs	r0, #7
	movs	r2, #0
	bl 0x0200a234
	ldr	r1, [r5, #0]
	movs	r0, #23
	movs	r2, #0
	bl 0x0200a234
	ldr	r1, [r5, #0]
	movs	r0, #15
	movs	r2, #0
	bl 0x0200a234
	ldr	r1, [r5, #0]
	movs	r0, #20
	movs	r2, #0
	bl 0x0200a234
	ldr	r1, [r5, #0]
	movs	r0, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200a1d4
	cmp	r0, #0
	bne.n	.L_0200183c
	adds	r0, r6, #0
	adds	r0, #10
	bl 0x0200a244
	movs	r1, #4
	movs	r0, #21
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #21
	movs	r1, #0
	bl 0x0200a254
	b.n	.L_0200185a
.L_0200183c:
	adds	r0, r6, #0
	adds	r0, #11
	bl 0x0200a244
	movs	r1, #3
	movs	r0, #21
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #21
	movs	r1, #0
	bl 0x0200a254
.L_0200185a:
	ldr	r6, [pc, #248]
	adds	r0, r6, #0
	bl 0x0200a244
	movs	r0, #23
	movs	r1, #21
	movs	r2, #0
	bl 0x0200a234
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #23
	bl 0x0200a26c
	movs	r0, #23
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #3
	movs	r0, #23
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #23
	movs	r1, #0
	bl 0x0200a254
	ldr	r5, [pc, #184]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200a21c
	movs	r1, #3
	movs	r0, #7
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r2, #0
	movs	r1, #15
	movs	r0, #23
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #23
	movs	r1, #0
	bl 0x0200a254
	ldr	r1, [r5, #0]
	movs	r0, #15
	movs	r2, #0
	bl 0x0200a234
	ldr	r1, [r5, #0]
	movs	r2, #0
	movs	r0, #23
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #0
	movs	r0, #23
	bl 0x0200a24c
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200a1d4
	cmp	r0, #0
	bne.n	.L_02001958
	adds	r0, r6, #4
	bl 0x0200a244
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200a21c
	movs	r1, #3
	movs	r0, #7
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a254
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a254
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a254
	b.n	.L_02001994
	.4byte 0x00002385
	.4byte 0x00002386
	.4byte 0x02000240
	.2byte 0x2392
	.2byte 0x0000
.L_02001958:
	adds	r0, r6, #7
	bl 0x0200a244
	movs	r0, #5
	movs	r1, #4
	bl 0x0200a21c
	movs	r0, #6
	movs	r1, #4
	bl 0x0200a21c
	movs	r1, #4
	movs	r0, #7
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a254
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a254
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a254
.L_02001994:
	ldr	r0, [pc, #956]
	bl 0x0200a244
	movs	r2, #0
	movs	r1, #15
	movs	r0, #23
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #23
	movs	r1, #0
	bl 0x0200a254
	movs	r2, #0
	movs	r1, #20
	movs	r0, #15
	bl 0x0200a23c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #15
	movs	r1, #3
	bl 0x0200a21c
	movs	r1, #3
	movs	r0, #20
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	ldr	r3, [pc, #892]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r1, [r5, #0]
	movs	r0, #15
	movs	r2, #0
	bl 0x0200a234
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #20
	bl 0x0200a234
	movs	r1, #1
	movs	r0, #21
	bl 0x0200a22c
	movs	r0, #15
	bl 0x0200a1bc
	movs	r1, #6
	adds	r1, #255
	movs	r2, #30
	movs	r0, #21
	bl 0x0200a26c
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #23
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #23
	movs	r1, #0
	bl 0x0200a254
	movs	r2, #0
	movs	r1, #23
	ldr	r0, [r5, #0]
	bl 0x0200a234
	movs	r0, #15
	bl 0x0200a1bc
	movs	r1, #3
	ldr	r0, [r5, #0]
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #3
	movs	r0, #23
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #23
	movs	r1, #0
	bl 0x0200a254
	movs	r0, #5
	movs	r1, #23
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #6
	movs	r1, #23
	movs	r2, #0
	bl 0x0200a234
	movs	r2, #0
	movs	r0, #7
	movs	r1, #23
	bl 0x0200a234
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #7
	movs	r1, #3
	bl 0x0200a21c
	movs	r1, #3
	ldr	r0, [r5, #0]
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #15
	bl 0x0200a26c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #20
	bl 0x0200a26c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #21
	bl 0x0200a26c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #20
	movs	r1, #0
	bl 0x0200a254
	ldr	r0, [r5, #0]
	movs	r1, #15
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #5
	movs	r1, #15
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #6
	movs	r1, #15
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #7
	movs	r1, #15
	movs	r2, #0
	bl 0x0200a234
	movs	r2, #0
	movs	r1, #15
	movs	r0, #23
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #23
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #10
	movs	r2, #30
	adds	r1, #255
	movs	r0, #15
	bl 0x0200a26c
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #3
	movs	r0, #5
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #3
	movs	r0, #20
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #20
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #6
	movs	r2, #0
	adds	r1, #255
	movs	r0, #6
	bl 0x0200a26c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a254
	movs	r2, #0
	movs	r1, #5
	movs	r0, #6
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #1
	movs	r0, #7
	bl 0x0200a22c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a254
	movs	r2, #0
	ldr	r0, [r5, #0]
	movs	r1, #7
	bl 0x0200a23c
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #7
	movs	r1, #3
	bl 0x0200a21c
	movs	r2, #0
	movs	r0, #5
	movs	r1, #6
	bl 0x0200a23c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a21c
	movs	r1, #3
	movs	r0, #6
	bl 0x0200a21c
	movs	r0, #15
	bl 0x0200a1bc
	ldr	r0, [r5, #0]
	movs	r1, #5
	movs	r2, #0
	bl 0x0200a23c
	movs	r2, #0
	movs	r0, #7
	movs	r1, #6
	bl 0x0200a23c
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #7
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a21c
	movs	r1, #3
	movs	r0, #6
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	ldr	r0, [r5, #0]
	movs	r1, #15
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #5
	movs	r1, #15
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #6
	movs	r1, #15
	movs	r2, #0
	bl 0x0200a234
	movs	r0, #7
	movs	r1, #15
	movs	r2, #0
	bl 0x0200a234
	movs	r2, #0
	movs	r0, #23
	movs	r1, #15
	bl 0x0200a234
	movs	r1, #3
	movs	r0, #15
	bl 0x0200a21c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a254
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #7
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200a21c
	movs	r1, #3
	movs	r0, #23
	bl 0x0200a21c
	movs	r0, #15
	bl 0x0200a1bc
	movs	r0, #15
	movs	r1, #3
	bl 0x0200a21c
	movs	r0, #20
	movs	r1, #3
	bl 0x0200a21c
	movs	r1, #3
	movs	r0, #21
	bl 0x0200a21c
	movs	r0, #60
	bl 0x0200a1bc
	movs	r0, #5
	movs	r1, #2
	bl 0x0200a21c
	ldr	r0, [r5, #0]
	bl 0x0200a1dc
	cmp	r0, #0
	beq.n	.L_02001cc0
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x0200a1ec
.L_02001cc0:
	movs	r0, #5
	bl 0x0200a20c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a214
	movs	r0, #6
	movs	r1, #2
	bl 0x0200a21c
	ldr	r0, [r5, #0]
	bl 0x0200a1dc
	cmp	r0, #0
	beq.n	.L_02001cf0
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x0200a1ec
.L_02001cf0:
	movs	r0, #6
	bl 0x0200a20c
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a214
	movs	r0, #7
	movs	r1, #2
	bl 0x0200a21c
	ldr	r0, [r5, #0]
	bl 0x0200a1dc
	cmp	r0, #0
	beq.n	.L_02001d20
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x0200a1ec
.L_02001d20:
	movs	r0, #7
	bl 0x0200a20c
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a214
	movs	r0, #23
	movs	r1, #2
	bl 0x0200a21c
	ldr	r0, [r5, #0]
	bl 0x0200a1dc
	cmp	r0, #0
	beq.n	.L_02001d5c
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #23
	bl 0x0200a1ec
	b.n	.L_02001d5c
	.2byte 0x0000
	.4byte 0x0000239c
	.2byte 0x0240
	.2byte 0x0200
.L_02001d5c:
	movs	r0, #23
	bl 0x0200a20c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #23
	bl 0x0200a214
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #40
	bl 0x0200a19c
	movs	r0, #212
	lsls	r0, r0, #2
	bl 0x0200a19c
	bl 0x0200a1cc
	pop	{r5, r6, r7, pc}
	.4byte 0x42402001
	.2byte 0x4770
	.2byte 0x0000
	push	{lr}
	movs	r0, #140
	movs	r1, #1
	bl 0x0200a294
	movs	r1, #1
	negs	r1, r1
	movs	r0, #15
	bl 0x0200a29c
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #32]
	str	r2, [r3, #36]
	adds	r3, #32
	movs	r2, #1
	strb	r2, [r3, #0]
	bl 0x0200a2b4
	movs	r0, #10
	bl 0x0200a1bc
	movs	r0, #0
	bl 0x0200a28c
	bl 0x0200a2a4
	bl 0x0200a2ac
	pop	{pc}
	.2byte 0x9d85
	.2byte 0x0200
	push	{r5, r6, lr}
	bl 0x0200a1c4
	movs	r0, #0
	bl 0x0200a2bc
	ldr	r6, [pc, #544]
	adds	r0, r6, #0
	bl 0x0200a244
	ldr	r5, [pc, #540]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r2, #134
	ldr	r0, [r5, #0]
	movs	r1, #144
	lsls	r2, r2, #2
	bl 0x0200a1fc
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200a25c
	movs	r0, #15
	bl 0x0200a1bc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #60
	movs	r0, #15
	bl 0x0200a26c
	movs	r1, #0
	movs	r0, #15
	bl 0x0200a24c
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200a1d4
	cmp	r0, #0
	bne.n	.L_02001e3a
	adds	r0, r6, #1
	bl 0x0200a244
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a254
	b.n	.L_02001e48
.L_02001e3a:
	adds	r0, r6, #2
	bl 0x0200a244
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a254
.L_02001e48:
	movs	r1, #10
	movs	r2, #60
	adds	r1, #255
	movs	r0, #15
	bl 0x0200a26c
	ldr	r0, [pc, #432]
	bl 0x0200a244
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a254
	movs	r1, #6
	adds	r1, #255
	movs	r2, #120
	movs	r0, #15
	bl 0x0200a26c
	movs	r1, #131
	movs	r2, #60
	lsls	r1, r1, #1
	movs	r0, #15
	bl 0x0200a26c
	movs	r1, #0
	movs	r0, #15
	bl 0x0200a254
	bl 0x0200a2e4
	movs	r0, #15
	bl 0x0200a1dc
	movs	r1, #2
	bl 0x0200a2fc
	movs	r0, #201
	bl 0x0200a31c
	movs	r0, #60
	bl 0x0200a1bc
	movs	r1, #144
	movs	r2, #130
	movs	r0, #24
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	bl 0x0200a214
	movs	r0, #24
	movs	r1, #6
	movs	r2, #0
	bl 0x0200a224
	movs	r2, #130
	movs	r1, #160
	lsls	r2, r2, #2
	movs	r0, #24
	bl 0x0200a1ec
	movs	r0, #24
	bl 0x0200a20c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #24
	bl 0x0200a25c
	movs	r0, #60
	bl 0x0200a1bc
	movs	r0, #15
	bl 0x0200a1dc
	movs	r1, #0
	bl 0x0200a2fc
	bl 0x0200a2f4
	bl 0x0200a2ec
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #30
	bl 0x0200a1bc
	movs	r2, #0
	movs	r1, #24
	movs	r0, #15
	bl 0x0200a234
	movs	r0, #30
	bl 0x0200a1bc
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a254
	ldr	r5, [pc, #240]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #15
	bl 0x0200a234
	movs	r0, #60
	bl 0x0200a1bc
	movs	r1, #0
	movs	r0, #15
	bl 0x0200a254
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #4
	movs	r0, #15
	bl 0x0200a21c
	movs	r0, #60
	bl 0x0200a1bc
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a254
	movs	r2, #0
	movs	r0, #15
	movs	r1, #24
	bl 0x0200a23c
	movs	r1, #0
	movs	r0, #15
	bl 0x0200a254
	movs	r0, #60
	bl 0x0200a1bc
	movs	r1, #4
	movs	r2, #20
	movs	r0, #24
	bl 0x0200a224
	movs	r0, #60
	bl 0x0200a1bc
	ldr	r1, [r5, #0]
	movs	r0, #15
	movs	r2, #0
	bl 0x0200a234
	ldr	r1, [r5, #0]
	movs	r2, #0
	movs	r0, #24
	bl 0x0200a234
	movs	r0, #60
	bl 0x0200a1bc
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a254
	movs	r0, #24
	movs	r1, #2
	movs	r2, #20
	bl 0x0200a224
	movs	r1, #2
	movs	r2, #11
	movs	r0, #24
	bl 0x0200a2c4
	bl 0x0200a1cc
	bl 0x0200a1c4
	movs	r0, #0
	bl 0x0200a2bc
	movs	r2, #0
	ldr	r0, [r5, #0]
	movs	r1, #15
	bl 0x0200a234
	movs	r0, #60
	bl 0x0200a1bc
	movs	r1, #2
	movs	r0, #15
	bl 0x0200a22c
	movs	r0, #30
	bl 0x0200a1bc
	movs	r1, #0
	movs	r0, #15
	bl 0x0200a254
	movs	r0, #60
	bl 0x0200a1bc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #44
	bl 0x0200a19c
	movs	r0, #196
	adds	r0, #255
	bl 0x0200a1b4
	bl 0x0200a1cc
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x000023cb
	.4byte 0x02000240
	.2byte 0x23ce
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #85
	str	r2, [r3, #0]
	ldr	r3, [pc, #364]
	subs	r2, #31
	adds	r5, r3, r2
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #7
	beq.n	.L_02002034
	movs	r0, #212
	lsls	r0, r0, #2
	bl 0x0200a1a4
.L_02002034:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x0200a194
	cmp	r0, #0
	beq.n	.L_0200209e
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #40
	bl 0x0200a194
	cmp	r0, #0
	bne.n	.L_0200209e
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #7
	bne.n	.L_0200209e
	movs	r0, #7
	bl 0x0200a31c
	movs	r1, #144
	movs	r2, #142
	movs	r0, #22
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	bl 0x0200a214
	movs	r0, #21
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a214
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a214
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a214
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a214
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a214
.L_0200209e:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x0200a194
	cmp	r0, #0
	beq.n	.L_02002104
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #40
	bl 0x0200a194
	cmp	r0, #0
	beq.n	.L_02002104
	ldr	r3, [pc, #208]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #7
	bne.n	.L_02002104
	movs	r0, #212
	lsls	r0, r0, #2
	bl 0x0200a194
	cmp	r0, #0
	beq.n	.L_02002104
	movs	r0, #7
	bl 0x0200a31c
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a214
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a214
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a214
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a214
.L_02002104:
	ldr	r3, [pc, #132]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #5
	bne.n	.L_02002134
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x0200a194
	cmp	r0, #0
	bne.n	.L_02002134
	movs	r0, #17
	movs	r1, #5
	bl 0x0200a21c
	movs	r1, #2
	movs	r0, #17
	adds	r1, #255
	bl 0x0200a274
.L_02002134:
	ldr	r3, [pc, #84]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #7
	bne.n	.L_02002186
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x0200a194
	cmp	r0, #0
	bne.n	.L_02002186
	movs	r0, #9
	movs	r1, #5
	bl 0x0200a21c
	movs	r0, #10
	movs	r1, #5
	bl 0x0200a21c
	movs	r0, #11
	movs	r1, #5
	bl 0x0200a21c
	movs	r1, #5
	movs	r0, #12
	bl 0x0200a21c
	movs	r0, #11
	bl 0x0200a1dc
	adds	r5, r0, #0
	movs	r0, #11
	bl 0x0200a1dc
	ldr	r3, [r0, #24]
	negs	r3, r3
	str	r3, [r5, #24]
.L_02002186:
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	.irp EntryTarget, 0x080003c9, 0x080003d1, 0x080003d9, 0x080ad039, 0x080ad041, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80c1, 0x080c80d1, 0x080c80d9, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8139, 0x080c8141, 0x080c8159, 0x080c8161, 0x080c8181, 0x080c8189, 0x080c81a1, 0x080c81d1, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8229, 0x080c8239, 0x080c8499, 0x080c84a1, 0x080c84a9, 0x080c84b1, 0x080c84b9, 0x080c84c1, 0x080c84e1, 0x080c8581, 0x080c85e9, 0x080c85f1, 0x080c8779, 0x080c8809, 0x080c8811, 0x080c8819, 0x080c8821, 0x08108009, 0x08108011, 0x08108019, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x00000015
	.4byte 0x00006666
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x80010000
	.4byte 0x0000002e
	.4byte 0x02008039
	.4byte 0x00000004
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000004
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0xffffffff
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x00000015
	.4byte 0x00006666
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x80010000
	.4byte 0x0000002e
	.4byte 0x0200806d
	.4byte 0x00000004
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000004
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0xffffffff
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000083
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004ccc
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x80010000
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0xffffffff
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000083
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x0000005a
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x00000015
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00006666
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x0000001e
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
	.4byte 0x000000aa
	.4byte 0x101020ab
	.4byte 0xffffffff
	.4byte 0x102030ab
	.4byte 0xffffffff
	.4byte 0x103040ab
	.4byte 0xffffffff
	.4byte 0x104050ab
	.4byte 0xffffffff
	.4byte 0x105060ab
	.4byte 0xffffffff
	.4byte 0x106070ab
	.4byte 0xffffffff
	.4byte 0x107080ab
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff0078
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00012000
	.4byte 0xffff0078
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0001a000
	.4byte 0xffff0078
	.4byte 0x00000002
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00010000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00014000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00014000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x01800000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00014000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x00012000
	.4byte 0xffff007d
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001a000
	.4byte 0xffff007d
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00012000
	.4byte 0xffff0020
	.4byte 0x0200a8dc
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00014000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x016a0000
	.4byte 0x00014000
	.4byte 0xffff0076
	.4byte 0x00000001
	.4byte 0x006c0000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00012000
	.4byte 0xffff0077
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00014000
	.4byte 0xffff007b
	.4byte 0x00000002
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00010000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00004000
	.4byte 0xffff007f
	.4byte 0x0200a6bc
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00026000
	.4byte 0xffff007f
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x0001a000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00014000
	.4byte 0xffff0078
	.4byte 0x0200a324
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00010000
	.4byte 0xffff0078
	.4byte 0x0200a4f0
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00018000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x005a0000
	.4byte 0x00000000
	.4byte 0x026e0000
	.4byte 0x00018000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x00c60000
	.4byte 0x00000000
	.4byte 0x026e0000
	.4byte 0x00010000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x016a0000
	.4byte 0x00014000
	.4byte 0xffff0076
	.4byte 0x00000001
	.4byte 0x006c0000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00012000
	.4byte 0xffff0077
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00014000
	.4byte 0xffff007b
	.4byte 0x00000002
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00010000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00004000
	.4byte 0xffff007f
	.4byte 0x0200a6bc
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00028000
	.4byte 0xffff007f
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0001e000
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff0078
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00010000
	.4byte 0xffff0078
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00018000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0001e000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0001a000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00014000
	.4byte 0xffff001d
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x0001c000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0x006300f5
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
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
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x0200851d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000022bc
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000022bd
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000022be
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000022c2
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000022c3
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000022c4
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000022c5
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000022c6
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000022c7
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008221
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000022e8
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x020081a9
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x000022ec
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x0200812d
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x000022f0
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x000022f1
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000022bf
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000022c0
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000022c1
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000022c8
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000022c9
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000022ca
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000022cb
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000022cc
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000022cd
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000022e9
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000022ea
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x000022ed
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000022ee
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x000022f2
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x000022f3
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x000022f4
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
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008299
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000022de
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000022df
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000022e0
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000022e1
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x02008359
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000022e3
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000022e4
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000022e5
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000022e6
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
	.4byte 0x00000002
	.4byte 0x09280016
	.4byte 0x02008605
	.4byte 0x00000002
	.4byte 0x09280017
	.4byte 0x02008611
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008221
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000023d8
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020081a9
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000023dc
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0200812d
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000023e0
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000023e1
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008379
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02008401
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000023c4
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x000023c5
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000023c6
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x000023a9
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x000023aa
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000023d9
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000023da
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000023dd
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000023de
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000023e2
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000023e3
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000023e4
	.4byte 0x00008d15
	.4byte 0xffff040f
	.4byte 0x02008459
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000023c7
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000023c8
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000023c9
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000023ca
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x000023ab
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000023ac
	.4byte 0x0001c314
	.4byte 0x092c000f
	.4byte 0x02009dd1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
