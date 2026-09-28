.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x020082f5, 0x02008045, 0x02008051, 0x02008059, 0x020082cd, 0x0200804d, 0x0200855d
	overlay_veneer \EntryTarget
	.endr
	push	{lr}
	movs	r1, #0
	bl 0x0200c254
	movs	r0, #0
	pop	{pc}
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xca18
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xca48
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.4byte 0x0200ca70
	.4byte 0x049b23c0
	.4byte 0x22d06edb
	.4byte 0x32380112
	.4byte 0x2201189b
	.2byte 0x701a
	.2byte 0x4770
	push	{lr}
	bl 0x020094f4
	pop	{pc}
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	bl 0x0200c2f4
	movs	r1, #5
	mov	r9, r0
	mov	fp, r1
.L_02000098:
	bl 0x0200c12c
	adds	r5, r0, #0
	bl 0x0200c12c
	mov	r2, r9
	ldr	r2, [r2, #8]
	lsls	r5, r5, #4
	mov	r8, r2
	add	r8, r5
	lsls	r0, r0, #4
	mov	r3, r8
	subs	r3, r3, r0
	mov	r8, r3
	bl 0x0200c12c
	adds	r6, r0, #0
	bl 0x0200c12c
	adds	r5, r0, #0
	bl 0x0200c12c
	mov	r1, r9
	ldr	r2, [r1, #12]
	ldr	r3, [r1, #16]
	lsls	r6, r6, #3
	adds	r6, r6, r2
	lsls	r5, r5, #4
	movs	r2, #128
	lsls	r0, r0, #4
	lsls	r2, r2, #11
	adds	r3, r3, r5
	subs	r3, r3, r0
	adds	r6, r6, r2
	movs	r0, #70
	adds	r0, #255
	mov	r1, r8
	adds	r2, r6, #0
	bl 0x0200c1e4
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02000168
	bl 0x0200c12c
	mov	r8, r0
	bl 0x0200c12c
	adds	r6, r0, #0
	bl 0x0200c12c
	movs	r3, #128
	adds	r5, r0, #0
	lsls	r3, r3, #7
	adds	r0, r7, #0
	ldr	r1, [pc, #120]
	lsrs	r5, r5, #1
	adds	r5, r5, r3
	bl 0x0200c1dc
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200c254
	mov	r1, r8
	movs	r2, #128
	lsls	r2, r2, #10
	lsls	r3, r1, #2
	adds	r3, r3, r2
	str	r3, [r7, #40]
	mov	r0, r8
	bl 0x0200c144
	ldr	r3, [pc, #88]
	lsls	r6, r6, #3
	adds	r1, r0, #0
	mov	sl, r3
	adds	r0, r6, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x62f8
	mov	r0, r8
	bl 0x0200c13c
	adds	r1, r0, #0
	adds	r0, r6, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x2300
	str	r3, [r7, #52]
	movs	r3, #168
	lsls	r3, r3, #7
	adds	r3, #122
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r0, [r7, #36]
	str	r5, [r7, #24]
	str	r5, [r7, #28]
	str	r3, [r7, #68]
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200c274
.L_02000168:
	movs	r1, #1
	negs	r1, r1
	add	fp, r1
	mov	r2, fp
	cmp	r2, #0
	bge.n	.L_02000098
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200c474
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, lr}
	movs	r0, #10
	bl 0x0200c2f4
	adds	r5, r0, #0
	movs	r0, #142
	lsls	r0, r0, #2
	bl 0x0200c19c
	cmp	r0, #0
	beq.n	.L_020001ac
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #75
	bl 0x0200c19c
	cmp	r0, #0
	bne.n	.L_020001e6
.L_020001ac:
	movs	r0, #159
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200c46c
	adds	r3, r5, #0
	adds	r3, #100
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	movs	r0, #160
	lsls	r3, r3, #16
	str	r3, [r5, #8]
	adds	r3, r5, #0
	adds	r3, #102
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r0, r0, #4
	lsls	r3, r3, #16
	str	r3, [r5, #16]
	adds	r0, #74
	bl 0x0200c19c
	cmp	r0, #0
	bne.n	.L_020001e6
	movs	r3, #153
	lsls	r3, r3, #8
	adds	r3, #153
	str	r3, [r5, #24]
	str	r3, [r5, #28]
.L_020001e6:
	pop	{r5, pc}
	push	{r5, lr}
	movs	r0, #10
	bl 0x0200c2f4
	adds	r5, r0, #0
	movs	r0, #142
	lsls	r0, r0, #2
	bl 0x0200c19c
	cmp	r0, #0
	beq.n	.L_0200020c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #75
	bl 0x0200c19c
	cmp	r0, #0
	bne.n	.L_0200024a
.L_0200020c:
	movs	r0, #159
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200c46c
	movs	r0, #154
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200c1a4
	adds	r3, r5, #0
	adds	r3, #100
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	movs	r0, #160
	lsls	r3, r3, #16
	str	r3, [r5, #8]
	adds	r3, r5, #0
	adds	r3, #102
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r0, r0, #4
	lsls	r3, r3, #16
	str	r3, [r5, #16]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	adds	r0, #74
	bl 0x0200c1a4
.L_0200024a:
	pop	{r5, pc}
	push	{r5, lr}
	adds	r5, r1, #0
	cmp	r0, #1
	bne.n	.L_02000294
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #62
	bl 0x0200c46c
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200c404
	movs	r0, #60
	bl 0x0200c414
	bl 0x02008d88
	movs	r1, #0
	movs	r2, #0
	movs	r0, #10
	bl 0x0200c32c
	movs	r0, #142
	lsls	r0, r0, #2
	bl 0x0200c1a4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #181
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #20
	strh	r3, [r2, #0]
.L_02000294:
	adds	r3, r5, #0
	subs	r3, #97
	cmp	r3, #174
	bhi.n	.L_020002ae
	adds	r0, r5, #0
	movs	r1, #30
	bl 0x0200c10c
	cmp	r0, #0
	bne.n	.L_020002ae
	movs	r0, #10
	bl 0x02008080
.L_020002ae:
	movs	r3, #138
	lsls	r3, r3, #1
	cmp	r5, r3
	bne.n	.L_020002c8
	movs	r0, #254
	lsls	r0, r0, #7
	adds	r0, #255
	movs	r1, #0
	bl 0x0200c404
	movs	r0, #20
	bl 0x0200c414
.L_020002c8:
	pop	{r5, pc}
	.2byte 0x0000
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xcbc0
	.2byte 0x0200
	push	{r5, lr}
	bl 0x0200c2f4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020002f2
	movs	r1, #0
	bl 0x0200c254
	adds	r3, r5, #0
	adds	r3, #89
	movs	r2, #0
	strb	r2, [r3, #0]
	subs	r3, #4
	strb	r2, [r3, #0]
.L_020002f2:
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r1, [pc, #584]
	movs	r3, #240
	mov	r8, r1
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r5, [r3, r2]
	movs	r3, #241
	lsls	r3, r3, #1
	movs	r2, #192
	lsls	r2, r2, #18
	add	r3, r8
	movs	r1, #0
	ldrsh	r7, [r3, r1]
	ldr	r3, [r2, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	mov	sl, r2
	movs	r2, #129
	adds	r3, r3, r1
	lsls	r2, r2, #2
	str	r2, [r3, #0]
	ldr	r2, [pc, #544]
	movs	r3, #0
.L_0200032c:
	movs	r0, #137
	str	r3, [r2, #0]
	lsls	r0, r0, #1
	sub	sp, #8
	bl 0x0200c1a4
	bl 0x02009d7c
	movs	r3, #88
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x02009df0
	ldr	r3, [pc, #516]
	cmp	r5, r3
	beq.n	.L_02000350
	b.n	.L_0200053a
.L_02000350:
	movs	r0, #0
	bl 0x0200c43c
	movs	r0, #163
	lsls	r0, r0, #4
	bl 0x0200c19c
	cmp	r0, #0
	beq.n	.L_020003ac
	movs	r5, #38
	movs	r6, #24
	movs	r0, #65
	movs	r1, #65
	movs	r2, #9
	movs	r3, #21
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200c23c
	movs	r0, #65
	movs	r1, #65
	movs	r2, #9
	movs	r3, #21
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200c244
	movs	r3, #87
	str	r3, [sp, #4]
	movs	r0, #76
	movs	r1, #64
	movs	r2, #9
	movs	r3, #22
	str	r5, [sp, #0]
	bl 0x0200c244
	movs	r3, #102
	movs	r2, #23
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #76
	movs	r1, #64
	movs	r2, #9
	movs	r3, #22
	bl 0x0200c244
.L_020003ac:
	cmp	r7, #11
	bne.n	.L_0200040a
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, r8
	ldr	r0, [r3, #0]
	bl 0x0200c2f4
	mov	r8, r0
	movs	r0, #208
	mov	r2, sl
	lsls	r0, r0, #2
	ldr	r6, [r2, #108]
	bl 0x0200c1b4
	adds	r5, r0, #0
	movs	r0, #210
	lsls	r0, r0, #2
	bl 0x0200c1b4
	movs	r3, #128
	lsls	r3, r3, #12
	lsls	r5, r5, #20
	adds	r5, r5, r3
	ldr	r2, [pc, #372]
	movs	r3, #230
	lsls	r3, r3, #1
	adds	r6, r6, r3
	lsls	r0, r0, #20
	adds	r0, r0, r2
	ldr	r2, [r6, #0]
	mov	r1, r8
	str	r0, [r1, #16]
	str	r5, [r1, #8]
	str	r5, [r2, #8]
	ldr	r3, [r1, #16]
	str	r3, [r2, #16]
	bl 0x0200c1fc
	movs	r0, #10
	adds	r0, #255
	bl 0x0200c19c
	cmp	r0, #0
	bne.n	.L_0200040a
	bl 0x0200be04
.L_0200040a:
	adds	r3, r7, #0
	subs	r3, #9
	cmp	r3, #1
	bhi.n	.L_02000422
	movs	r0, #10
	adds	r0, #255
	bl 0x0200c19c
	cmp	r0, #0
	bne.n	.L_02000422
	bl 0x0200be04
.L_02000422:
	cmp	r7, #1
	beq.n	.L_0200042c
	cmp	r7, #20
	beq.n	.L_0200042c
	b.n	.L_0200053a
.L_0200042c:
	movs	r0, #163
	lsls	r0, r0, #4
	bl 0x0200c19c
	cmp	r0, #0
	bne.n	.L_02000452
	movs	r0, #11
	movs	r1, #3
	bl 0x0200c3ac
	movs	r0, #12
	movs	r1, #3
	bl 0x0200c3ac
	movs	r0, #12
	bl 0x0200c2f4
	ldr	r3, [pc, #264]
	str	r3, [r0, #24]
.L_02000452:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #75
	bl 0x0200c19c
	cmp	r0, #0
	beq.n	.L_02000470
	bl 0x02008d88
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c32c
	b.n	.L_02000494
.L_02000470:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #74
	bl 0x0200c19c
	cmp	r0, #0
	beq.n	.L_0200048a
	movs	r0, #154
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200c1a4
	b.n	.L_02000494
.L_0200048a:
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c32c
.L_02000494:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #59
	bl 0x0200c19c
	cmp	r0, #0
	beq.n	.L_02000532
	movs	r0, #163
	lsls	r0, r0, #4
	bl 0x0200c19c
	cmp	r0, #0
	bne.n	.L_020004c2
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c32c
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c32c
.L_020004c2:
	bl 0x02008d88
	movs	r0, #10
	bl 0x0200c114
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #58
	bl 0x0200c19c
	cmp	r0, #0
	bne.n	.L_020004f8
	movs	r0, #0
	bl 0x02008ae4
	movs	r0, #10
	adds	r0, #255
	bl 0x0200c19c
	cmp	r0, #0
	bne.n	.L_020004f8
	movs	r0, #13
	bl 0x02008db8
	movs	r0, #14
	bl 0x02008db8
.L_020004f8:
	movs	r0, #13
	bl 0x0200c2f4
	ldr	r3, [r0, #8]
	movs	r1, #200
	lsls	r1, r1, #17
	cmp	r3, r1
	ble.n	.L_0200053a
	movs	r3, #43
	movs	r2, #13
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #39
	movs	r1, #22
	movs	r2, #2
	movs	r3, #1
	bl 0x0200c23c
	movs	r3, #40
	movs	r2, #12
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #31
	movs	r1, #21
	movs	r2, #2
	movs	r3, #2
	bl 0x0200c23c
	b.n	.L_0200053a
.L_02000532:
	cmp	r7, #20
	bne.n	.L_0200053a
	bl 0x02008714
.L_0200053a:
	movs	r0, #0
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200ccec
	.4byte 0x0000011e
	.4byte 0xfed80000
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	movs	r0, #163
	lsls	r0, r0, #4
	bl 0x0200c19c
	cmp	r0, #0
	beq.n	.L_02000586
	ldr	r3, [pc, #56]
	movs	r2, #253
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	ldr	r3, [pc, #48]
	ldr	r2, [pc, #48]
	movs	r1, #160
	subs	r3, r3, r2
	adds	r0, r0, r3
	lsls	r1, r1, #19
	bl 0x0200c294
.L_02000586:
	ldr	r3, [pc, #28]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #20
	bne.n	.L_020005a0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #75
	bl 0x0200c1a4
.L_020005a0:
	movs	r0, #0
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000121
	.2byte 0x010e
	.2byte 0x0000
	push	{r5, r6, lr}
	bl 0x0200c2dc
	movs	r0, #0
	bl 0x0200c44c
	bl 0x02008d88
	movs	r0, #0
	bl 0x0200c46c
	movs	r0, #40
	bl 0x0200c2d4
	ldr	r6, [pc, #308]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r5, r6, r1
	ldr	r0, [r5, #0]
	bl 0x0200c2f4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #129
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200c3bc
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #272]
	adds	r2, #204
	bl 0x0200c2fc
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #8
	bl 0x0200c324
	ldr	r0, [r5, #0]
	movs	r1, #4
	movs	r2, #10
	bl 0x0200c354
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #16
	bl 0x0200c324
	movs	r2, #20
	ldr	r0, [r5, #0]
	movs	r1, #6
	bl 0x0200c354
	movs	r0, #53
	bl 0x0200c46c
	movs	r0, #11
	movs	r1, #2
	bl 0x0200c33c
	movs	r1, #2
	movs	r0, #12
	bl 0x0200c33c
	movs	r0, #24
	bl 0x0200c2d4
	movs	r1, #174
	movs	r2, #232
	movs	r0, #11
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200c32c
	movs	r1, #166
	movs	r2, #232
	lsls	r2, r2, #16
	movs	r0, #12
	lsls	r1, r1, #18
	bl 0x0200c32c
	movs	r0, #11
	movs	r1, #3
	bl 0x0200c33c
	movs	r1, #3
	movs	r0, #12
	bl 0x0200c33c
	movs	r0, #107
	bl 0x0200c46c
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200c264
	movs	r0, #20
	bl 0x0200c2d4
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200c46c
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200c264
	movs	r0, #20
	bl 0x0200c2d4
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r1, r1
	negs	r0, r0
	bl 0x0200c264
	movs	r0, #20
	bl 0x0200c2d4
	movs	r0, #148
	bl 0x0200c46c
	movs	r0, #11
	movs	r1, #4
	bl 0x0200c33c
	movs	r1, #4
	movs	r0, #12
	bl 0x0200c33c
	movs	r0, #60
	bl 0x0200c2d4
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r6, r3
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r0, [pc, #36]
	movs	r1, #20
	bl 0x0200c3ec
	ldr	r3, [pc, #32]
	movs	r1, #251
	lsls	r1, r1, #1
	adds	r2, r6, r1
	strh	r3, [r2, #0]
	movs	r0, #102
	movs	r1, #0
	bl 0x0200c3e4
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00019999
	.4byte 0x0000011e
	.2byte 0x0068
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r0, #11
	bl 0x0200c2f4
	adds	r5, r0, #0
	movs	r0, #15
	bl 0x0200c2f4
	adds	r6, r0, #0
	movs	r0, #16
	bl 0x0200c2f4
	mov	r8, r0
	bl 0x0200c2dc
	movs	r0, #0
	bl 0x0200c44c
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	movs	r3, #0
	negs	r1, r1
	negs	r0, r0
	bl 0x0200c3cc
	bl 0x02008d88
	movs	r0, #13
	bl 0x0200c2f4
	movs	r1, #0
	bl 0x0200c254
	movs	r0, #14
	bl 0x0200c2f4
	movs	r1, #0
	bl 0x0200c254
	movs	r0, #15
	bl 0x0200c2f4
	movs	r1, #7
	bl 0x0200c374
	movs	r0, #16
	bl 0x0200c2f4
	movs	r1, #7
	bl 0x0200c374
	movs	r0, #1
	bl 0x0200c114
	ldr	r3, [r5, #16]
	ldr	r2, [pc, #780]
	ldr	r1, [r5, #8]
	adds	r3, r3, r2
	adds	r0, r5, #0
	movs	r2, #0
	bl 0x0200c1f4
	adds	r3, r6, #0
	movs	r2, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	mov	r1, r8
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r1, #85
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	strb	r2, [r1, #0]
	mov	r2, r8
	str	r3, [r2, #24]
	str	r3, [r2, #28]
	movs	r0, #170
	movs	r1, #1
	movs	r2, #228
	lsls	r0, r0, #18
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #0
	bl 0x0200c3cc
	ldr	r3, [pc, #724]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	movs	r1, #170
	movs	r2, #132
	lsls	r2, r2, #17
	lsls	r1, r1, #18
	ldr	r0, [r6, #0]
	bl 0x0200c32c
	movs	r0, #1
	bl 0x0200c114
	bl 0x0200c1fc
	movs	r0, #1
	bl 0x0200c114
	bl 0x0200c41c
	bl 0x0200c42c
	movs	r0, #78
	bl 0x0200c46c
	movs	r0, #20
	bl 0x0200c2d4
	movs	r0, #151
	bl 0x0200c46c
	movs	r0, #11
	bl 0x0200c2f4
	movs	r1, #7
	bl 0x0200c374
	movs	r0, #12
	bl 0x0200c2f4
	movs	r1, #7
	bl 0x0200c374
	movs	r0, #13
	bl 0x0200c2f4
	movs	r1, #7
	bl 0x0200c374
	movs	r0, #14
	bl 0x0200c2f4
	movs	r1, #7
	bl 0x0200c374
	movs	r0, #20
	bl 0x0200c2d4
	movs	r0, #190
	bl 0x0200c46c
	movs	r0, #15
	movs	r1, #11
	bl 0x0200c334
	movs	r1, #12
	movs	r0, #16
	bl 0x0200c334
	movs	r0, #1
	bl 0x0200c114
	ldr	r1, [pc, #580]
	movs	r0, #11
	bl 0x0200c304
	ldr	r1, [pc, #576]
	movs	r0, #12
	bl 0x0200c304
	ldr	r5, [pc, #572]
	movs	r0, #15
	adds	r1, r5, #0
	bl 0x0200c304
	adds	r1, r5, #0
	movs	r0, #16
	bl 0x0200c30c
	movs	r0, #13
	bl 0x02008db8
	movs	r0, #14
	bl 0x02008db8
	ldr	r5, [pc, #548]
	movs	r0, #15
	adds	r1, r5, #0
	bl 0x0200c304
	adds	r1, r5, #0
	movs	r0, #16
	bl 0x0200c30c
	movs	r0, #10
	bl 0x0200c2d4
	movs	r0, #13
	bl 0x0200c2f4
	movs	r1, #0
	bl 0x0200c374
	movs	r0, #14
	bl 0x0200c2f4
	movs	r1, #0
	bl 0x0200c374
	movs	r0, #80
	bl 0x0200c2d4
	movs	r0, #13
	movs	r1, #2
	bl 0x0200c364
	movs	r1, #5
	movs	r0, #13
	bl 0x0200c33c
	movs	r0, #20
	bl 0x0200c2d4
	ldr	r0, [pc, #476]
	bl 0x0200c37c
	movs	r2, #80
	movs	r0, #13
	movs	r1, #0
	bl 0x0200c38c
	movs	r1, #1
	movs	r0, #13
	bl 0x0200c364
	movs	r0, #20
	bl 0x0200c2d4
	movs	r0, #34
	bl 0x0200c46c
	movs	r2, #20
	movs	r0, #13
	movs	r1, #0
	bl 0x0200c38c
	movs	r0, #14
	movs	r1, #1
	bl 0x0200c364
	movs	r1, #5
	movs	r0, #14
	bl 0x0200c33c
	movs	r0, #20
	bl 0x0200c2d4
	movs	r2, #20
	movs	r0, #14
	movs	r1, #0
	bl 0x0200c38c
	movs	r0, #14
	movs	r1, #0
	bl 0x0200c394
	movs	r0, #13
	movs	r1, #2
	bl 0x0200c364
	movs	r0, #13
	movs	r1, #6
	bl 0x0200c33c
	movs	r0, #13
	movs	r1, #0
	bl 0x0200c394
	movs	r0, #14
	movs	r1, #6
	bl 0x0200c33c
	movs	r0, #14
	movs	r1, #0
	movs	r2, #20
	bl 0x0200c38c
	movs	r1, #6
	movs	r2, #40
	adds	r1, #255
	movs	r0, #13
	bl 0x0200c3b4
	movs	r0, #13
	movs	r1, #0
	bl 0x0200c394
	movs	r1, #6
	movs	r2, #20
	adds	r1, #255
	movs	r0, #14
	bl 0x0200c3b4
	movs	r0, #14
	movs	r1, #0
	bl 0x0200c394
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #13
	bl 0x0200c3b4
	movs	r2, #20
	movs	r0, #13
	movs	r1, #0
	bl 0x0200c38c
	movs	r0, #13
	movs	r1, #0
	bl 0x0200c394
	movs	r0, #14
	movs	r1, #7
	bl 0x0200c33c
	movs	r0, #14
	movs	r1, #0
	bl 0x0200c394
	movs	r0, #13
	movs	r1, #0
	bl 0x0200c394
	movs	r0, #14
	movs	r1, #0
	movs	r2, #40
	bl 0x0200c38c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #13
	bl 0x0200c3b4
	movs	r1, #0
	movs	r0, #13
	bl 0x0200c384
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x0200c2ec
	cmp	r0, #0
	bne.n	.L_02000a04
	ldr	r0, [r6, #0]
	bl 0x0200c34c
	movs	r0, #13
	movs	r1, #7
	bl 0x0200c33c
	movs	r0, #13
	movs	r1, #0
	bl 0x0200c394
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02000a2c
.L_02000a04:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	ldr	r0, [r6, #0]
	bl 0x0200c34c
	movs	r0, #13
	movs	r1, #6
	bl 0x0200c33c
	movs	r0, #13
	movs	r1, #0
	bl 0x0200c394
.L_02000a2c:
	movs	r0, #40
	bl 0x0200c2d4
	movs	r0, #13
	movs	r1, #0
	bl 0x0200c394
	movs	r0, #14
	movs	r1, #2
	bl 0x0200c35c
	movs	r0, #14
	movs	r1, #0
	bl 0x0200c394
	movs	r1, #0
	movs	r0, #13
	bl 0x0200c394
	movs	r0, #1
	bl 0x02008ae4
	movs	r0, #14
	movs	r1, #0
	bl 0x0200c394
	movs	r0, #13
	movs	r1, #7
	bl 0x0200c344
	movs	r0, #13
	movs	r1, #0
	bl 0x0200c394
	movs	r0, #13
	movs	r1, #10
	bl 0x0200c33c
	movs	r1, #9
	movs	r0, #14
	bl 0x0200c33c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #59
	bl 0x0200c1a4
	bl 0x0200c45c
	bl 0x0200c2e4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.4byte 0xfff80000
	.4byte 0x02000240
	.4byte 0x0200c9a0
	.4byte 0x0200c9dc
	.4byte 0x0200c91c
	.4byte 0x0200c958
	.2byte 0x2d6a
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #40]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200c2f4
	ldr	r3, [r0, #80]
	ldr	r4, [r5, #80]
	ldrb	r3, [r3, #9]
	ldrb	r1, [r4, #9]
	movs	r2, #12
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r4, #9]
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #172]
	adds	r7, r0, #0
	mov	r8, r3
	movs	r0, #234
	movs	r3, #224
	lsls	r3, r3, #16
	adds	r0, #255
	ldr	r1, [pc, #160]
	movs	r2, #0
	bl 0x0200c1e4
	mov	r3, r8
	movs	r5, #0
	str	r0, [r3, #0]
	cmp	r0, #0
	beq.n	.L_02000b92
	ldr	r6, [r0, #80]
	movs	r3, #33
	ldrb	r2, [r6, #5]
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #13
	negs	r2, r2
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	adds	r2, r0, #0
	strb	r3, [r6, #9]
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
	adds	r2, #7
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r3, [pc, #104]
	movs	r1, #193
	str	r3, [r0, #108]
	lsls	r1, r1, #3
	strb	r5, [r6, #26]
	strb	r5, [r6, #27]
	movs	r0, #68
	bl 0x0200c154
	adds	r5, r0, #0
	movs	r0, #247
	bl 0x0200c2ac
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r5, r5, r3
	ldrb	r0, [r6, #16]
	movs	r1, #128
	adds	r2, r5, #0
	bl 0x0200c17c
	movs	r0, #68
	bl 0x0200c15c
	cmp	r7, #0
	beq.n	.L_02000b92
	movs	r0, #1
	bl 0x0200c114
	mov	r3, r8
	ldr	r2, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #10
	str	r3, [r2, #40]
	movs	r0, #4
	bl 0x0200c114
	movs	r0, #135
	bl 0x0200c46c
	movs	r0, #8
	bl 0x0200c114
	movs	r0, #135
	bl 0x0200c46c
.L_02000b92:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0200ccec
	.4byte 0x02be0000
	.2byte 0x8ab5
	.2byte 0x0200
	push	{lr}
	bl 0x0200c2dc
	movs	r0, #0
	bl 0x0200c44c
	movs	r1, #5
	movs	r0, #14
	bl 0x0200c33c
	ldr	r0, [pc, #24]
	bl 0x0200c37c
	movs	r0, #14
	movs	r1, #0
	bl 0x0200c394
	movs	r0, #14
	movs	r1, #9
	bl 0x0200c33c
	bl 0x0200c2e4
	pop	{pc}
	.2byte 0x2d80
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	bl 0x0200c2dc
	movs	r0, #0
	bl 0x0200c44c
	movs	r1, #5
	movs	r0, #13
	bl 0x0200c33c
	ldr	r0, [pc, #148]
	bl 0x0200c37c
	movs	r0, #13
	movs	r1, #0
	bl 0x0200c394
	movs	r0, #13
	movs	r1, #10
	bl 0x0200c33c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #58
	bl 0x0200c19c
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02000c14
	b.n	.L_02000d80
.L_02000c14:
	ldr	r5, [pc, #112]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	bl 0x0200c2f4
	movs	r1, #128
	adds	r6, r0, #0
	lsls	r1, r1, #7
	ldr	r0, [r5, #0]
	bl 0x0200c3a4
	ldr	r0, [r5, #0]
	movs	r1, #28
	bl 0x0200c33c
	ldr	r5, [pc, #84]
	ldr	r0, [r5, #0]
	adds	r3, r0, #0
	adds	r3, #85
	strb	r7, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r0, #48]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r0, #52]
.L_02000c4c:
	movs	r3, #128
	ldr	r2, [r6, #12]
	lsls	r3, r3, #14
	adds	r2, r2, r3
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	bl 0x0200c204
	ldr	r0, [r5, #0]
	bl 0x0200c20c
	movs	r0, #83
	bl 0x0200c46c
	movs	r0, #224
	bl 0x0200c2bc
	movs	r1, #222
	movs	r0, #4
	bl 0x0200c2b4
	ldr	r0, [pc, #24]
	movs	r1, #1
	bl 0x0200c29c
	movs	r5, #0
	movs	r6, #216
	b.n	.L_02000c98
	.4byte 0x00002d7f
	.4byte 0x02000240
	.4byte 0x0200ccec
	.2byte 0x2e6c
	.2byte 0x0000
.L_02000c94:
	adds	r6, #2
	adds	r5, #1
.L_02000c98:
	cmp	r5, #14
	bgt.n	.L_02000cb4
	movs	r0, #4
	bl 0x0200c194
	ldr	r3, [pc, #56]
	ldrh	r2, [r0, r6]
	ands	r3, r2
	cmp	r3, #222
	bne.n	.L_02000c94
	movs	r0, #4
	adds	r1, r5, #0
	bl 0x0200c2c4
.L_02000cb4:
	ldr	r3, [pc, #40]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200c33c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #58
	bl 0x0200c1a4
	ldr	r5, [pc, #20]
	movs	r2, #0
	movs	r3, #0
	movs	r1, #0
	ldr	r0, [r5, #0]
	b.n	.L_02000ce8
	.2byte 0x0000
	.4byte 0x000001ff
	.4byte 0x02000240
	.2byte 0xccec
	.2byte 0x0200
.L_02000ce8:
	bl 0x0200c1f4
	movs	r0, #1
	bl 0x0200c114
	ldr	r0, [r5, #0]
	bl 0x0200c1ec
	movs	r0, #1
	bl 0x0200c114
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #112]
	adds	r1, #153
	bl 0x0200c3c4
	movs	r0, #170
	movs	r1, #1
	movs	r2, #146
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #18
	negs	r1, r1
	bl 0x0200c3cc
	bl 0x0200c3d4
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200c40c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #3
	bl 0x0200c404
	movs	r0, #20
	bl 0x0200c414
	movs	r0, #20
	bl 0x0200c114
	ldr	r3, [pc, #52]
	ldr	r2, [pc, #40]
	movs	r0, #1
	strh	r2, [r3, #0]
	bl 0x0200c114
	movs	r2, #0
	ldr	r0, [pc, #40]
	movs	r1, #0
	bl 0x0200c2a4
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200c404
	movs	r0, #20
	bl 0x0200c414
	movs	r0, #20
	bl 0x0200c114
	b.n	.L_02000d80
	.4byte 0x00007fff
	.4byte 0x0004cccc
	.4byte 0x0500021e
	.2byte 0x2d83
	.2byte 0x0000
.L_02000d80:
	bl 0x0200c2e4
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	movs	r3, #5
	movs	r2, #6
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #46
	movs	r1, #68
	movs	r2, #40
	movs	r3, #73
	bl 0x0200c214
	movs	r3, #40
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #104
	movs	r1, #9
	movs	r2, #5
	movs	r3, #6
	bl 0x0200c23c
	add	sp, #8
	pop	{pc}
	push	{r5, r6, lr}
	sub	sp, #8
	adds	r6, r0, #0
	bl 0x0200c2f4
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl 0x0200c2f4
	movs	r1, #0
	bl 0x0200c254
	adds	r0, r6, #0
	movs	r1, #3
	bl 0x0200c3ac
	adds	r3, r5, #0
	adds	r3, #85
	movs	r1, #0
	strb	r1, [r3, #0]
	adds	r0, r5, #0
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	adds	r3, r5, #0
	adds	r3, #89
	strb	r1, [r3, #0]
	cmp	r6, #13
	bne.n	.L_02000e20
	movs	r1, #175
	movs	r2, #224
	lsls	r2, r2, #16
	movs	r0, #13
	lsls	r1, r1, #18
	bl 0x0200c32c
	movs	r0, #13
	movs	r1, #10
	bl 0x0200c33c
	movs	r3, #43
	str	r3, [sp, #0]
	movs	r0, #39
	movs	r1, #22
	movs	r2, #2
	movs	r3, #1
	str	r6, [sp, #4]
	bl 0x0200c23c
	b.n	.L_02000e4a
.L_02000e20:
	movs	r1, #165
	movs	r2, #216
	lsls	r2, r2, #16
	movs	r0, #14
	lsls	r1, r1, #18
	bl 0x0200c32c
	movs	r0, #14
	movs	r1, #9
	bl 0x0200c33c
	movs	r3, #40
	movs	r2, #12
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #31
	movs	r1, #21
	movs	r2, #2
	movs	r3, #2
	bl 0x0200c23c
.L_02000e4a:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	bl 0x0200c2dc
	movs	r0, #0
	bl 0x0200c44c
	ldr	r0, [pc, #24]
	movs	r1, #1
	bl 0x0200c29c
	movs	r3, #173
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	bl 0x0200c2e4
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x2e6b
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #158
	sub	sp, #8
	bl 0x0200c19c
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_02000ea2
	b.n	.L_0200105c
.L_02000ea2:
	bl 0x0200c2dc
	movs	r0, #0
	bl 0x0200c44c
	movs	r1, #1
	ldr	r0, [pc, #412]
	bl 0x0200c29c
	movs	r0, #78
	bl 0x0200c46c
	movs	r0, #1
	bl 0x0200c114
	movs	r0, #182
	bl 0x0200c46c
	movs	r0, #234
	movs	r1, #170
	movs	r2, #128
	movs	r3, #147
	adds	r0, #255
	lsls	r1, r1, #18
	lsls	r2, r2, #12
	lsls	r3, r3, #16
	ldr	r7, [pc, #376]
	bl 0x0200c1e4
	str	r0, [r7, #0]
	cmp	r0, #0
	beq.n	.L_02000f48
	ldr	r6, [r0, #80]
	movs	r3, #33
	ldrb	r2, [r6, #5]
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #13
	negs	r2, r2
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r6, #9]
	adds	r3, r0, #0
	movs	r1, #0
	adds	r3, #85
	adds	r2, r0, #0
	strb	r1, [r3, #0]
	adds	r2, #92
	movs	r3, #1
	strb	r3, [r2, #0]
	strb	r1, [r6, #26]
	strb	r1, [r6, #27]
	adds	r1, r0, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r1, #193
	lsls	r1, r1, #3
	movs	r0, #68
	bl 0x0200c154
	adds	r5, r0, #0
	movs	r0, #247
	bl 0x0200c2ac
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r5, r5, r2
	ldrb	r0, [r6, #16]
	movs	r1, #128
	adds	r2, r5, #0
	bl 0x0200c17c
	movs	r0, #68
	bl 0x0200c15c
.L_02000f48:
	movs	r0, #20
	bl 0x0200c2d4
	ldr	r3, [pc, #260]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r6, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c2fc
	movs	r1, #170
	ldr	r0, [r6, #0]
	lsls	r1, r1, #2
	movs	r2, #186
	bl 0x0200c31c
	movs	r1, #192
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #80
	bl 0x0200c39c
	movs	r1, #2
	ldr	r0, [r6, #0]
	adds	r1, #255
	movs	r2, #40
	bl 0x0200c3b4
	ldr	r0, [r7, #0]
	cmp	r0, #0
	beq.n	.L_02000fe6
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #10
	movs	r5, #128
	str	r3, [r0, #40]
	str	r3, [r0, #48]
	lsls	r5, r5, #9
	movs	r1, #168
	movs	r3, #160
	lsls	r1, r1, #18
	movs	r2, #0
	lsls	r3, r3, #16
	str	r5, [r0, #52]
	bl 0x0200c204
	ldr	r0, [r7, #0]
	bl 0x0200c20c
	movs	r0, #135
	bl 0x0200c46c
.L_02000fc2:
	ldr	r0, [r7, #0]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r0, #52]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r0, #40]
	movs	r1, #166
	movs	r3, #158
	str	r5, [r0, #48]
	lsls	r1, r1, #18
	movs	r2, #0
	lsls	r3, r3, #16
	bl 0x0200c204
	ldr	r0, [r7, #0]
	bl 0x0200c20c
.L_02000fe6:
	movs	r0, #20
	bl 0x0200c2d4
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r1, [pc, #100]
	adds	r2, #204
	ldr	r0, [r6, #0]
	bl 0x0200c2fc
	ldr	r0, [r6, #0]
	bl 0x0200c2f4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #128
	lsls	r1, r1, #2
	movs	r2, #170
	adds	r1, #158
	ldr	r0, [r6, #0]
	bl 0x0200c31c
	movs	r0, #1
	bl 0x0200c2d4
	ldr	r0, [r6, #0]
	bl 0x0200c2f4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	ldr	r0, [r7, #0]
	cmp	r0, #0
	beq.n	.L_02001038
	bl 0x0200c1ec
.L_02001038:
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #7
	bl 0x0200c3a4
	bl 0x0200c45c
	bl 0x0200c2e4
	b.n	.L_020013b0
	.4byte 0x00002e6d
	.4byte 0x0200ccec
	.4byte 0x02000240
	.2byte 0x9999
	.2byte 0x0001
.L_0200105c:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #128
	ldr	r3, [r3, #0]
	mov	fp, r3
	bl 0x0200c2dc
	movs	r0, #0
	bl 0x0200c44c
	movs	r1, #1
	ldr	r0, [pc, #844]
	bl 0x0200c29c
	movs	r0, #78
	bl 0x0200c46c
	movs	r0, #1
	bl 0x0200c114
	movs	r0, #182
	bl 0x0200c46c
	ldr	r3, [pc, #824]
	movs	r0, #234
	mov	r9, r3
	movs	r1, #170
	movs	r2, #128
	movs	r3, #147
	lsls	r2, r2, #12
	adds	r0, #255
	lsls	r1, r1, #18
	lsls	r3, r3, #16
	bl 0x0200c1e4
	mov	r2, r9
	str	r0, [r2, #0]
	cmp	r0, #0
	beq.n	.L_02001112
	ldr	r6, [r0, #80]
	mov	r3, r8
	ldrb	r2, [r6, #5]
	strb	r3, [r6, #26]
	strb	r3, [r6, #27]
	movs	r3, #33
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #13
	negs	r2, r2
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r6, #9]
	adds	r3, r0, #0
	adds	r3, #85
	mov	r2, r8
	strb	r2, [r3, #0]
	adds	r2, r0, #0
	adds	r2, #92
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r1, r0, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r1, #193
	lsls	r1, r1, #3
	movs	r0, #68
	bl 0x0200c154
	adds	r5, r0, #0
	movs	r0, #247
	bl 0x0200c2ac
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r5, r5, r3
	ldrb	r0, [r6, #16]
	movs	r1, #128
	adds	r2, r5, #0
	bl 0x0200c17c
	movs	r0, #68
	bl 0x0200c15c
.L_02001112:
	movs	r0, #20
	bl 0x0200c2d4
	ldr	r2, [pc, #684]
	movs	r7, #133
	mov	sl, r2
	lsls	r7, r7, #2
	movs	r1, #204
	movs	r2, #204
	add	r7, sl
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r7, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200c2fc
	movs	r1, #170
	ldr	r0, [r7, #0]
	lsls	r1, r1, #2
	movs	r2, #186
	bl 0x0200c31c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #40
	ldr	r0, [r7, #0]
	bl 0x0200c39c
	movs	r0, #0
	bl 0x0200c46c
	movs	r0, #141
	bl 0x0200c46c
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #10
	lsls	r0, r0, #10
	bl 0x0200c264
	movs	r0, #20
	bl 0x0200c2d4
	movs	r1, #129
	ldr	r0, [r7, #0]
	lsls	r1, r1, #1
	bl 0x0200c3bc
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #9
	lsls	r0, r0, #9
	bl 0x0200c264
	movs	r0, #20
	bl 0x0200c2d4
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200c40c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #6
	bl 0x0200c404
	movs	r0, #80
	bl 0x0200c414
	movs	r0, #80
	bl 0x0200c2d4
	ldr	r5, [pc, #536]
	movs	r1, #144
	adds	r0, r5, #0
	lsls	r1, r1, #3
	bl 0x0200c11c
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #524]
	adds	r1, #153
	bl 0x0200c3c4
	movs	r0, #170
	movs	r1, #1
	movs	r2, #240
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #18
	bl 0x0200c3cc
	bl 0x0200c3d4
	movs	r0, #80
	bl 0x0200c2d4
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
.L_020011ea:
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r1, r1
	negs	r0, r0
	bl 0x0200c264
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200c46c
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200c404
	movs	r0, #80
	bl 0x0200c414
	movs	r0, #60
	bl 0x0200c114
	adds	r0, r5, #0
	bl 0x0200c124
	movs	r0, #20
	bl 0x0200c114
	movs	r1, #0
	movs	r2, #0
	movs	r0, #0
	bl 0x0200c264
	movs	r0, #20
	bl 0x0200c2d4
	movs	r5, #38
	movs	r6, #24
	movs	r0, #65
	movs	r1, #65
	movs	r2, #9
	movs	r3, #21
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200c23c
	movs	r0, #65
	movs	r1, #65
	movs	r2, #9
.L_0200124c:
	movs	r3, #21
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200c244
	movs	r3, #87
	str	r3, [sp, #4]
	movs	r0, #76
	movs	r1, #64
	movs	r2, #9
	movs	r3, #22
	str	r5, [sp, #0]
	bl 0x0200c244
	movs	r3, #102
	movs	r2, #23
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #76
	movs	r2, #9
	movs	r1, #64
	movs	r3, #22
	bl 0x0200c244
	movs	r3, #253
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	ldr	r2, [pc, #332]
	ldr	r3, [pc, #332]
	mov	r1, fp
	subs	r3, r3, r2
	adds	r0, r0, r3
	bl 0x0200c294
	mov	r3, r8
	mov	r2, fp
	movs	r0, #128
	strh	r3, [r2, #0]
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200c404
	movs	r0, #120
	bl 0x0200c414
	movs	r0, #160
	bl 0x0200c114
	movs	r0, #170
	movs	r1, #1
	movs	r2, #186
	movs	r3, #1
	lsls	r0, r0, #18
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200c3cc
	bl 0x0200c3d4
	movs	r0, #40
	bl 0x0200c2d4
	mov	r3, r9
	ldr	r0, [r3, #0]
	cmp	r0, #0
	beq.n	.L_0200132e
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #10
	movs	r5, #128
	str	r3, [r0, #40]
	str	r3, [r0, #48]
	lsls	r5, r5, #9
	movs	r1, #168
	movs	r3, #160
	lsls	r1, r1, #18
	lsls	r3, r3, #16
	str	r5, [r0, #52]
	movs	r2, #0
	bl 0x0200c204
	mov	r2, r9
	ldr	r0, [r2, #0]
	bl 0x0200c20c
	movs	r0, #135
	bl 0x0200c46c
	mov	r3, r9
	ldr	r0, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r0, #52]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r0, #40]
	movs	r1, #166
	movs	r3, #158
	str	r5, [r0, #48]
	movs	r2, #0
	lsls	r1, r1, #18
	lsls	r3, r3, #16
	bl 0x0200c204
	mov	r2, r9
	ldr	r0, [r2, #0]
	bl 0x0200c20c
.L_0200132e:
	movs	r0, #20
	bl 0x0200c2d4
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r1, [pc, #160]
	adds	r2, #204
	ldr	r0, [r7, #0]
	bl 0x0200c2fc
	ldr	r0, [r7, #0]
	bl 0x0200c2f4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r1, #128
	strb	r3, [r0, #0]
	lsls	r1, r1, #2
	movs	r2, #170
	adds	r1, #158
	ldr	r0, [r7, #0]
	bl 0x0200c31c
	movs	r0, #1
	bl 0x0200c2d4
	ldr	r0, [r7, #0]
	bl 0x0200c2f4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	mov	r3, r9
	ldr	r0, [r3, #0]
	cmp	r0, #0
	beq.n	.L_02001382
	bl 0x0200c1ec
.L_02001382:
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r0, [r7, #0]
	bl 0x0200c3a4
	bl 0x0200c45c
	movs	r0, #163
	lsls	r0, r0, #4
	bl 0x0200c1a4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #158
	bl 0x0200c1a4
	ldr	r3, [pc, #60]
	movs	r2, #251
	lsls	r2, r2, #1
	add	r2, sl
	strh	r3, [r2, #0]
	bl 0x0200c2e4
.L_020013b0:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00002e6d
	.4byte 0x0200ccec
	.4byte 0x02000240
	.4byte 0x02009425
	.4byte 0x0004cccc
	.4byte 0x0000010e
	.4byte 0x00000121
	.4byte 0x00019999
	.2byte 0x0069
	.2byte 0x0000
	push	{lr}
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
	ldr	r3, [r0, #104]
	subs	r3, #1
	str	r3, [r0, #104]
	cmp	r3, #1
	bne.n	.L_02001418
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x0200c1f4
	b.n	.L_02001420
.L_02001418:
	cmp	r3, #0
	bne.n	.L_02001420
	bl 0x0200c1ec
.L_02001420:
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #196]
	movs	r1, #1
	ldr	r2, [r3, #0]
	adds	r3, r2, #0
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_020014e8
	lsrs	r3, r2, #1
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02001454
	bl 0x0200c12c
	lsls	r5, r0, #3
	subs	r5, r5, r0
	bl 0x0200c12c
	lsls	r5, r5, #2
	lsls	r3, r0, #4
	subs	r3, r3, r0
	lsrs	r5, r5, #16
	movs	r2, #155
	b.n	.L_0200146a
.L_02001454:
	bl 0x0200c12c
	lsls	r5, r0, #3
	subs	r5, r5, r0
	bl 0x0200c12c
	lsls	r5, r5, #2
	lsls	r3, r0, #4
	subs	r3, r3, r0
	lsrs	r5, r5, #16
	movs	r2, #178
.L_0200146a:
	lsls	r2, r2, #18
	lsls	r3, r3, #3
	lsls	r5, r5, #16
	adds	r5, r5, r2
	lsrs	r3, r3, #16
	movs	r2, #230
	lsls	r2, r2, #17
	lsls	r3, r3, #16
	adds	r3, r3, r2
	movs	r0, #30
	movs	r2, #128
	adds	r1, r5, #0
	adds	r0, #255
	lsls	r2, r2, #14
	bl 0x0200c1e4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020014e8
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
	movs	r3, #13
	ldr	r1, [r5, #80]
	negs	r3, r3
	ldrb	r2, [r1, #9]
	adds	r0, r5, #0
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r1, #9]
	movs	r1, #0
	bl 0x0200c254
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	movs	r3, #60
	str	r3, [r5, #104]
	ldr	r3, [pc, #24]
	adds	r0, r5, #0
.L_020014d8:
	movs	r1, #5
	str	r3, [r5, #108]
	bl 0x0200c1cc
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200c25c
.L_020014e8:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x0300122c
	.2byte 0x93e5
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #40]
	sub	sp, #4
	str	r3, [sp, #0]
	movs	r1, #18
	movs	r2, #20
	movs	r3, #0
	movs	r0, #9
	bl 0x0200aa58
	ldr	r2, [pc, #24]
	movs	r3, #128
	str	r2, [sp, #0]
	lsls	r3, r3, #8
	movs	r1, #17
	movs	r2, #19
	movs	r0, #9
	bl 0x0200aa58
	add	sp, #4
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200c4b8
	.2byte 0xc5b8
	.2byte 0x0200
	push	{lr}
	ldr	r4, [r0, #8]
	ldr	r3, [r1, #8]
	subs	r2, r4, r3
	cmp	r2, #0
	blt.n	.L_0200153e
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r2, r3
.L_0200153a:
	blt.n	.L_02001548
	b.n	.L_02001588
.L_0200153e:
	movs	r2, #128
	subs	r3, r3, r4
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02001588
.L_02001548:
	ldr	r4, [r0, #12]
	ldr	r3, [r1, #12]
	subs	r2, r4, r3
	cmp	r2, #0
	blt.n	.L_0200155c
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001566
	b.n	.L_02001588
.L_0200155c:
	movs	r2, #128
	subs	r3, r3, r4
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02001588
.L_02001566:
	ldr	r0, [r0, #16]
	ldr	r1, [r1, #16]
	subs	r2, r0, r1
.L_0200156c:
	cmp	r2, #0
	blt.n	.L_0200157a
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001584
	b.n	.L_02001588
.L_0200157a:
	movs	r2, #128
	subs	r3, r1, r0
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02001588
.L_02001584:
	movs	r0, #1
	b.n	.L_0200158a
.L_02001588:
	movs	r0, #0
.L_0200158a:
	pop	{pc}
	push	{lr}
	ldr	r4, [r0, #8]
	ldr	r3, [r1, #8]
	subs	r2, r4, r3
	cmp	r2, #0
	blt.n	.L_020015a2
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_020015ac
	b.n	.L_020015de
.L_020015a2:
	movs	r2, #192
	subs	r3, r3, r4
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_020015de
.L_020015ac:
	ldr	r2, [r1, #12]
	ldr	r3, [r0, #12]
	subs	r3, r3, r2
	ldr	r2, [pc, #48]
	adds	r3, r3, r2
	ldr	r2, [pc, #48]
	cmp	r3, r2
	bhi.n	.L_020015de
	ldr	r0, [r0, #16]
	ldr	r1, [r1, #16]
	subs	r2, r0, r1
	cmp	r2, #0
	blt.n	.L_020015d0
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_020015da
	b.n	.L_020015de
.L_020015d0:
	movs	r2, #192
	subs	r3, r1, r0
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_020015de
.L_020015da:
	movs	r0, #1
	b.n	.L_020015e0
.L_020015de:
	movs	r0, #0
.L_020015e0:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0007ffff
	.2byte 0xfffe
	.2byte 0x001f
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #336]
	ldr	r2, [pc, #336]
	mov	sl, r3
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, sl
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #4
	mov	r8, r2
	bl 0x0200c2f4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	ldr	r5, [r6, #68]
	mov	r9, r3
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r4, r0, #0
	adds	r3, r3, r7
	adds	r0, r5, #0
	movs	r1, #18
	str	r3, [r6, #16]
	str	r4, [sp, #0]
.L_02001636:
	bl 0x0200c104
	subs	r5, r5, r0
.L_0200163c:
	str	r5, [r6, #68]
	adds	r3, r7, #0
	ldr	r4, [sp, #0]
	cmp	r7, #0
	bge.n	.L_02001648
	adds	r3, #15
.L_02001648:
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
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #20]
	cmp	r2, r3
	bne.n	.L_020016da
	adds	r0, r6, #0
	adds	r1, r4, #0
	bl 0x02009528
	cmp	r0, #0
	beq.n	.L_020016da
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	add	r3, sl
	ldrb	r2, [r3, #0]
	cmp	r2, #0
	bne.n	.L_020016da
	ldr	r1, [r6, #76]
	cmp	r1, #0
	beq.n	.L_020016ae
	mov	r3, r8
	adds	r3, #104
	strh	r2, [r3, #0]
	mov	r2, r8
	adds	r2, #106
	cmp	r1, #0
	ble.n	.L_020016a6
	movs	r3, #1
	b.n	.L_020016ac
.L_020016a6:
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
.L_020016ac:
	strh	r3, [r2, #0]
.L_020016ae:
	adds	r2, r6, #0
	adds	r2, #35
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r1, #0
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #16]
	str	r1, [r6, #76]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
	mov	r2, r8
	movs	r3, #1
	strh	r3, [r2, #4]
	ldrh	r2, [r2, #10]
	movs	r3, #170
	lsls	r3, r3, #1
	add	r3, r9
	strh	r2, [r3, #0]
.L_020016da:
	movs	r3, #84
	mov	r2, r8
	ldrh	r0, [r2, r3]
	movs	r7, #0
	cmp	r0, #0
	beq.n	.L_0200173a
	mov	r5, r8
	adds	r5, #84
.L_020016ea:
	bl 0x0200c2f4
	adds	r4, r0, #0
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #20]
	cmp	r2, r3
	bne.n	.L_0200172c
	adds	r0, r6, #0
	adds	r1, r4, #0
	bl 0x0200958c
	cmp	r0, #0
	beq.n	.L_0200172c
	ldrh	r1, [r5, #2]
	cmp	r1, #0
	bne.n	.L_02001726
	adds	r2, r6, #0
	adds	r2, #35
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #16]
	str	r1, [r6, #76]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
	b.n	.L_0200172c
.L_02001726:
	movs	r3, #1
	mov	r2, r8
	strh	r3, [r2, #6]
.L_0200172c:
	adds	r7, #1
	adds	r5, #4
	cmp	r7, #3
	bgt.n	.L_0200173a
	ldrh	r0, [r5, #0]
	cmp	r0, #0
	bne.n	.L_020016ea
.L_0200173a:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r3, r5, #0
	movs	r6, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r3, #4
	strb	r6, [r3, #0]
	movs	r1, #0
	bl 0x0200c254
	ldr	r1, [pc, #20]
	adds	r0, r5, #0
	bl 0x0200c1dc
	adds	r3, r5, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	str	r6, [r5, #48]
	str	r6, [r5, #52]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xc744
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [r0, #8]
	adds	r6, r1, #0
	adds	r7, r2, #0
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #4]
	movs	r0, #183
	lsls	r0, r0, #1
	bl 0x0200c1e4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020017c0
	movs	r3, #0
	str	r3, [r5, #68]
	str	r3, [r5, #72]
	str	r6, [r5, #76]
	movs	r1, #2
	bl 0x02009750
	ldr	r3, [pc, #24]
	str	r3, [r5, #108]
	cmp	r7, #0
	beq.n	.L_020017b8
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
.L_020017b8:
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200c1cc
.L_020017c0:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x95ed
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #332]
	ldr	r2, [pc, #332]
	mov	sl, r3
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, sl
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #4
	mov	r8, r2
	bl 0x0200c2f4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	ldr	r5, [r6, #68]
	mov	r9, r3
	ldr	r3, [r6, #16]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #16]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #8]
	adds	r4, r0, #0
	adds	r3, r3, r7
	adds	r0, r5, #0
	movs	r1, #18
	str	r3, [r6, #8]
	str	r4, [sp, #0]
	bl 0x0200c104
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	ldr	r4, [sp, #0]
	cmp	r7, #0
	bge.n	.L_02001824
	adds	r3, #15
.L_02001824:
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
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #20]
	cmp	r2, r3
	bne.n	.L_020018b4
	adds	r0, r6, #0
	adds	r1, r4, #0
	bl 0x02009528
	cmp	r0, #0
	beq.n	.L_020018b4
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	add	r3, sl
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_020018b4
	ldr	r2, [r6, #76]
	cmp	r2, #0
	beq.n	.L_02001888
	mov	r1, r8
	adds	r1, #106
	strh	r3, [r1, #0]
	subs	r1, #2
	cmp	r2, #0
	ble.n	.L_02001880
	movs	r3, #1
	b.n	.L_02001886
.L_02001880:
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
.L_02001886:
	strh	r3, [r1, #0]
.L_02001888:
	adds	r2, r6, #0
	adds	r2, #35
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r1, #0
	ldr	r3, [r6, #16]
	ldr	r2, [r6, #68]
	str	r1, [r6, #76]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
	mov	r2, r8
	movs	r3, #1
	strh	r3, [r2, #4]
	ldrh	r2, [r2, #10]
	movs	r3, #170
	lsls	r3, r3, #1
	add	r3, r9
	strh	r2, [r3, #0]
.L_020018b4:
	movs	r3, #84
	mov	r2, r8
	ldrh	r0, [r2, r3]
	movs	r7, #0
	cmp	r0, #0
	beq.n	.L_02001914
	mov	r5, r8
	adds	r5, #84
.L_020018c4:
	bl 0x0200c2f4
	adds	r4, r0, #0
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #20]
	cmp	r2, r3
	bne.n	.L_02001906
	adds	r0, r6, #0
	adds	r1, r4, #0
	bl 0x0200958c
	cmp	r0, #0
	beq.n	.L_02001906
	ldrh	r1, [r5, #2]
	cmp	r1, #0
	bne.n	.L_02001900
	adds	r2, r6, #0
	adds	r2, #35
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #16]
	str	r1, [r6, #76]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
	b.n	.L_02001906
.L_02001900:
	movs	r3, #1
	mov	r2, r8
	strh	r3, [r2, #6]
.L_02001906:
	adds	r7, #1
	adds	r5, #4
	cmp	r7, #3
	bgt.n	.L_02001914
	ldrh	r0, [r5, #0]
	cmp	r0, #0
	bne.n	.L_020018c4
.L_02001914:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r3, r5, #0
	movs	r6, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r3, #4
	strb	r6, [r3, #0]
	movs	r3, #3
	ldr	r0, [r5, #80]
	ands	r1, r3
	ldrb	r2, [r0, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	lsls	r1, r1, #2
	orrs	r3, r1
	strb	r3, [r0, #9]
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x0200c254
	ldr	r1, [pc, #16]
	adds	r0, r5, #0
	bl 0x0200c1dc
	adds	r3, r5, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	str	r6, [r5, #48]
	str	r6, [r5, #52]
	pop	{r5, r6, pc}
	.2byte 0xc744
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [r0, #8]
	adds	r6, r1, #0
	adds	r7, r2, #0
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #4]
	movs	r0, #183
	lsls	r0, r0, #1
	bl 0x0200c1e4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020019ae
	movs	r3, #0
	str	r3, [r5, #68]
	str	r3, [r5, #72]
	negs	r3, r6
	str	r3, [r5, #76]
	movs	r1, #3
	bl 0x02009928
	ldr	r3, [pc, #24]
	str	r3, [r5, #108]
	cmp	r7, #0
	beq.n	.L_020019a6
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
.L_020019a6:
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200c1cc
.L_020019ae:
	pop	{r5, r6, r7, pc}
	.2byte 0x95ed
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [r0, #8]
	adds	r6, r1, #0
	adds	r7, r2, #0
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #4]
	movs	r0, #183
	lsls	r0, r0, #1
	bl 0x0200c1e4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020019f4
	movs	r3, #0
	str	r3, [r5, #68]
	str	r3, [r5, #72]
	str	r6, [r5, #76]
	movs	r1, #3
	bl 0x02009928
	ldr	r3, [pc, #24]
	str	r3, [r5, #108]
	cmp	r7, #0
	beq.n	.L_020019ec
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
.L_020019ec:
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200c1cc
.L_020019f4:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x97c9
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [r0, #8]
	adds	r6, r1, #0
	adds	r7, r2, #0
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #4]
	movs	r0, #183
	lsls	r0, r0, #1
	bl 0x0200c1e4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001a3e
	movs	r3, #0
	str	r3, [r5, #68]
	str	r3, [r5, #72]
	negs	r3, r6
	str	r3, [r5, #76]
	movs	r1, #3
	bl 0x02009928
	ldr	r3, [pc, #24]
	str	r3, [r5, #108]
	cmp	r7, #0
	beq.n	.L_02001a36
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
.L_02001a36:
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200c1cc
.L_02001a3e:
	pop	{r5, r6, r7, pc}
	.2byte 0x97c9
	.2byte 0x0200
	push	{lr}
	ldr	r4, [r0, #8]
	ldr	r3, [r1, #8]
	subs	r2, r4, r3
	cmp	r2, #0
	blt.n	.L_02001a5a
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001a64
	b.n	.L_02001a94
.L_02001a5a:
	movs	r2, #192
	subs	r3, r3, r4
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02001a94
.L_02001a64:
	ldr	r2, [r1, #12]
	ldr	r3, [r0, #12]
	subs	r3, r3, r2
	ldr	r2, [pc, #44]
	subs	r3, #1
	cmp	r3, r2
	bhi.n	.L_02001a94
	ldr	r0, [r0, #16]
	ldr	r1, [r1, #16]
	subs	r2, r0, r1
	cmp	r2, #0
	blt.n	.L_02001a86
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001a90
	b.n	.L_02001a94
.L_02001a86:
	movs	r2, #128
	subs	r3, r1, r0
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02001a94
.L_02001a90:
	movs	r0, #1
	b.n	.L_02001a96
.L_02001a94:
	movs	r0, #0
.L_02001a96:
	pop	{pc}
	.2byte 0xfffe
	.2byte 0x000f
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #144]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200c2f4
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
	mov	r8, r0
	adds	r3, r3, r7
	adds	r0, r5, #0
	movs	r1, #18
	str	r3, [r6, #16]
	bl 0x0200c104
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02001ae0
	adds	r3, #15
.L_02001ae0:
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
	mov	r3, r8
	ldr	r2, [r3, #12]
	ldr	r3, [r3, #20]
	cmp	r2, r3
	bne.n	.L_02001b2e
	adds	r0, r6, #0
	mov	r1, r8
	bl 0x02009a44
	cmp	r0, #0
	beq.n	.L_02001b2e
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #16]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
	movs	r3, #0
	str	r3, [r6, #76]
.L_02001b2e:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r3, r5, #0
	movs	r6, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r3, #4
	strb	r6, [r3, #0]
	movs	r1, #0
	bl 0x0200c254
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200c1cc
	adds	r0, r5, #0
	ldr	r1, [pc, #24]
	bl 0x0200c1dc
	adds	r0, r5, #0
	movs	r1, #10
	bl 0x0200c374
	adds	r3, r5, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	str	r6, [r5, #48]
	str	r6, [r5, #52]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xc744
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [r0, #8]
	ldr	r2, [r0, #4]
	adds	r6, r1, #0
	ldr	r1, [r0, #0]
	ldr	r0, [pc, #104]
	adds	r3, r3, r0
	movs	r0, #30
	adds	r0, #255
	bl 0x0200c1e4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001bea
	bl 0x0200c12c
	adds	r3, r0, #0
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #12
	movs	r3, #192
	lsls	r3, r3, #6
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	bl 0x0200c144
	str	r0, [r5, #68]
	bl 0x0200c12c
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	str	r0, [r5, #72]
	bl 0x0200c12c
	lsls	r0, r0, #17
	lsrs	r0, r0, #16
	adds	r0, r0, r6
	str	r0, [r5, #76]
	bl 0x0200c12c
	ldr	r3, [pc, #36]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r3, r5, #0
	adds	r3, #100
	strh	r0, [r3, #0]
	movs	r1, #2
	adds	r0, r5, #0
	bl 0x02009b38
	ldr	r3, [pc, #20]
	adds	r0, r5, #0
	str	r3, [r5, #108]
	movs	r1, #1
	bl 0x0200c25c
.L_02001bea:
	pop	{r5, r6, pc}
	.4byte 0xfffe0000
	.4byte 0xffff8000
	.2byte 0x9a9d
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r0, [pc, #364]
	sub	sp, #8
	mov	r8, r0
	movs	r0, #192
	lsls	r0, r0, #18
	ldr	r1, [r0, #32]
	mov	r7, r8
	adds	r2, r1, #0
	adds	r2, #228
	ldr	r3, [r2, #0]
	ldr	r2, [r2, #4]
	mov	r9, r3
	ldr	r3, [pc, #344]
	mov	r4, r9
	ands	r4, r3
	ands	r2, r3
	ldr	r3, [r1, #0]
	mov	r9, r4
	ldr	r3, [r3, #4]
	adds	r7, #20
	str	r3, [sp, #4]
	mov	sl, r2
	ldr	r0, [r0, #108]
	str	r0, [sp, #0]
	mov	r0, r8
	movs	r4, #6
	ldrsh	r3, [r0, r4]
	cmp	r3, #0
	beq.n	0x02009c52
	movs	r1, #8
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	bne.n	0x02009c52
	ldr	r3, [r0, #16]
	cmp	r3, #0
	beq.n	0x02009c52
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4642
	ldrh	r3, [r2, #6]
	mov	r4, r8
	movs	r2, #0
	mov	r0, r8
	strh	r3, [r4, #8]
	strh	r2, [r0, #6]
	movs	r1, #3
	mov	fp, r1
.L_02001c64:
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	cmp	r3, #0
	beq.n	.L_02001d54
	ldr	r5, [r7, #8]
	cmp	r5, #0
	beq.n	.L_02001d54
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200c19c
	ldr	r4, [sp, #0]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r3, r4, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02001c8e
	movs	r3, #1
	orrs	r0, r3
.L_02001c8e:
	adds	r6, r5, #0
	adds	r6, #91
	strb	r0, [r6, #0]
	mov	r0, r8
	movs	r4, #14
	ldrsh	r3, [r0, r4]
	cmp	r3, #0
	beq.n	.L_02001ca8
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200c19c
	strb	r0, [r6, #0]
.L_02001ca8:
	ldr	r2, [r5, #8]
	mov	r1, r9
	ldr	r3, [r5, #16]
	subs	r2, r2, r1
	ldr	r1, [r5, #12]
	mov	r4, sl
	subs	r3, r3, r4
	subs	r1, r3, r1
	movs	r0, #6
	ldrsh	r4, [r7, r0]
	asrs	r3, r1, #16
	adds	r1, r3, #0
	asrs	r2, r2, #16
	subs	r1, #8
	cmp	r4, #0
	bne.n	.L_02001cde
	adds	r3, r2, #7
	movs	r2, #167
	lsls	r2, r2, #1
	cmp	r3, r2
	bhi.n	.L_02001d54
	movs	r3, #48
	negs	r3, r3
	cmp	r1, r3
	ble.n	.L_02001d54
	cmp	r1, #239
	bgt.n	.L_02001d54
.L_02001cde:
	movs	r0, #2
	ldrsh	r3, [r7, r0]
	ldrh	r1, [r7, #2]
	cmp	r3, #0
	bgt.n	.L_02001d50
	ldrh	r3, [r7, #4]
	movs	r1, #240
	ands	r1, r3
	cmp	r1, #32
	beq.n	.L_02001d24
	cmp	r1, #32
	bgt.n	.L_02001d00
	cmp	r1, #0
	beq.n	.L_02001d40
	cmp	r1, #16
	beq.n	.L_02001d32
	b.n	.L_02001d4c
.L_02001d00:
	cmp	r1, #48
	beq.n	.L_02001d16
	cmp	r1, #128
	bne.n	.L_02001d4c
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	adds	r0, #8
	adds	r2, r4, #0
	bl 0x02009b78
	b.n	.L_02001d4c
.L_02001d16:
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	adds	r0, #8
	adds	r2, r4, #0
	bl 0x0200996c
	b.n	.L_02001d4c
.L_02001d24:
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	adds	r0, #8
	adds	r2, r4, #0
	bl 0x020099fc
	b.n	.L_02001d4c
.L_02001d32:
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	adds	r0, #8
	adds	r2, r4, #0
	bl 0x020099b4
	b.n	.L_02001d4c
.L_02001d40:
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	adds	r0, #8
	adds	r2, r4, #0
	bl 0x02009780
.L_02001d4c:
	movs	r3, #8
	b.n	.L_02001d52
.L_02001d50:
	subs	r3, r1, #1
.L_02001d52:
	strh	r3, [r7, #2]
.L_02001d54:
	movs	r1, #1
	negs	r1, r1
	add	fp, r1
	mov	r2, fp
	adds	r7, #16
	cmp	r2, #0
	blt.n	.L_02001d64
	b.n	.L_02001c64
.L_02001d64:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200254c
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb560
	movs	r0, #10
	adds	r0, #255
	ldr	r6, [pc, #68]
	bl 0x0200c19c
	cmp	r0, #0
	bne.n	0x02009d96
	ldr	r3, [pc, #60]
	adds	r0, r6, #0
	movs	r1, #116
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x206e
	movs	r1, #1
	movs	r2, #0
	movs	r3, #0
	adds	r0, #255
	bl 0x0200c1e4
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r1, #1
	bl 0x0200c1cc
	ldr	r1, [r5, #80]
	movs	r2, #1
	ldrb	r3, [r1, #16]
	str	r5, [r6, #112]
	strh	r3, [r6, #12]
	ldrb	r3, [r1, #17]
	orrs	r3, r2
	strb	r3, [r1, #17]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200254c
	.2byte 0x0258
	.2byte 0x0300
	push	{r5, lr}
	ldr	r5, [pc, #12]
	ldr	r0, [r5, #112]
	bl 0x0200c1ec
	movs	r3, #0
	str	r3, [r5, #112]
	pop	{r5, pc}
	.4byte 0x0200254c
	.4byte 0x81d84b01
	.4byte 0x00004770
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r8, r1
	ldr	r1, [pc, #280]
	sub	sp, #16
	adds	r6, r0, #0
	movs	r0, #10
	str	r1, [sp, #4]
	adds	r0, #255
	adds	r1, #20
	str	r2, [sp, #12]
	str	r3, [sp, #8]
	mov	r9, r1
	bl 0x0200c19c
	cmp	r0, #0
	bne.n	.L_02001ed4
	ldrh	r3, [r6, #0]
	movs	r2, #0
	mov	fp, r2
	mov	sl, r3
	adds	r6, #2
	cmp	r3, #0
	ble.n	.L_02001e92
.L_02001e2a:
	ldrh	r7, [r6, #0]
	movs	r1, #15
	ands	r1, r7
	movs	r3, #240
	mov	r0, sl
	str	r1, [sp, #0]
	ands	r7, r3
	bl 0x0200c2f4
	adds	r5, r0, #0
	adds	r6, #2
	cmp	r5, #0
	beq.n	.L_02001e7e
	movs	r1, #0
	bl 0x0200c254
	adds	r3, r5, #0
	movs	r2, #128
	adds	r3, #98
	movs	r1, #1
	ands	r2, r7
	strb	r1, [r3, #0]
	cmp	r2, #0
	bne.n	.L_02001e5e
	subs	r3, #9
	strb	r2, [r3, #0]
.L_02001e5e:
	mov	r2, r9
	mov	r3, r9
	strh	r1, [r2, #0]
	mov	r0, sl
	strh	r7, [r3, #4]
	bl 0x0200c2f4
	mov	r1, r9
	str	r0, [r1, #8]
	ldr	r2, [sp, #0]
	lsls	r3, r2, #16
	str	r3, [r1, #12]
	mov	r3, fp
	strh	r3, [r1, #2]
	movs	r2, #16
	add	r9, r2
.L_02001e7e:
	movs	r3, #1
	add	fp, r3
	mov	r1, fp
	cmp	r1, #3
	bgt.n	.L_02001e92
	ldrh	r2, [r6, #0]
	adds	r6, #2
	mov	sl, r2
	cmp	r2, #0
	bgt.n	.L_02001e2a
.L_02001e92:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_02001ed4
	movs	r1, #0
	ldrh	r2, [r3, #0]
	mov	fp, r1
	ldr	r1, [sp, #4]
	movs	r3, #2
	add	r8, r3
	movs	r3, #84
	strh	r2, [r1, r3]
	cmp	r2, #0
	ble.n	.L_02001ed4
	adds	r2, r1, #0
	adds	r2, #84
.L_02001eb0:
	mov	r1, r8
	ldrh	r3, [r1, #0]
	movs	r1, #1
	strh	r3, [r2, #2]
	add	fp, r1
	movs	r3, #2
	add	r8, r3
	mov	r3, fp
	adds	r2, #4
	cmp	r3, #3
	bgt.n	.L_02001ed4
	mov	r1, r8
	ldrh	r3, [r1, #0]
	movs	r1, #2
	add	r8, r1
	strh	r3, [r2, #0]
	cmp	r3, #0
	bgt.n	.L_02001eb0
.L_02001ed4:
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #4]
	add	r1, sp, #8
	str	r2, [r3, #16]
	ldrh	r1, [r1, #0]
	ldr	r2, [sp, #4]
	strh	r1, [r2, #10]
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #80
	ldrh	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L_02001f04
	movs	r3, #192
	movs	r2, #128
	lsls	r3, r3, #4
	lsls	r2, r2, #19
	adds	r3, #8
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #16
	strh	r3, [r1, #0]
.L_02001f04:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #20]
	bl 0x0200c11c
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200254c
	.4byte 0x02009bf9
	.4byte 0x01004b02
	.4byte 0x231418c0
	.4byte 0x47705ec0
	.4byte 0x0200254c
	.4byte 0x01004b02
	.4byte 0x828118c0
	.4byte 0x00004770
	.4byte 0x0200254c
	.4byte 0x68184b01
	.4byte 0x00004770
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r0, #130
	lsls	r0, r0, #1
	ldr	r5, [pc, #24]
	bl 0x0200c19c
	cmp	r0, #0
	bne.n	.L_02001f6e
	ldr	r3, [r5, #0]
	adds	r3, #1
	str	r3, [r5, #0]
	cmp	r3, r6
	blt.n	.L_02001f6e
	str	r0, [r5, #0]
.L_02001f6e:
	ldr	r0, [r5, #0]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200254c
	.4byte 0x01004b06
	.4byte 0x230f18c0
	.4byte 0x3014400b
	.4byte 0x60c3041b
	.4byte 0x40194b01
	.4byte 0x47708081
	.4byte 0x000000f0
	.4byte 0x0200254c
	.4byte 0x01004b02
	.4byte 0x834118c0
	.4byte 0x00004770
	.4byte 0x0200254c
	.4byte 0x01004b02
	.4byte 0x231818c0
	.4byte 0x47705ec0
	.2byte 0x254c
	.2byte 0x0200
	push	{lr}
	ldr	r2, [pc, #12]
	cmp	r0, #3
	bhi.n	.L_02001fc6
	lsls	r3, r0, #2
	adds	r3, #84
	strh	r1, [r2, r3]
.L_02001fc6:
	pop	{pc}
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	sub	sp, #16
	ldr	r6, [r3, #108]
	bl 0x0200c464
	mov	r8, r0
	bl 0x0200c2f4
	bl 0x0200c2cc
	movs	r5, #0
	adds	r7, r0, #0
	cmp	r5, r7
	bge.n	.L_0200200e
.L_02001ff2:
	ldr	r2, [pc, #192]
	movs	r1, #134
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldrb	r0, [r2, r3]
	bl 0x0200c194
	ldrh	r3, [r0, #56]
	lsls	r2, r5, #1
	mov	r1, sp
	adds	r5, #1
	strh	r3, [r1, r2]
	cmp	r5, r7
	blt.n	.L_02001ff2
.L_0200200e:
	movs	r0, #10
	negs	r0, r0
	movs	r1, #0
	bl 0x0200c454
	movs	r2, #182
	lsls	r2, r2, #1
	movs	r4, #183
	adds	r3, r6, r2
	lsls	r4, r4, #1
	movs	r2, #0
	strh	r2, [r3, #0]
	movs	r1, #129
	adds	r3, r6, r4
	strh	r2, [r3, #0]
	mov	r0, r8
	lsls	r1, r1, #1
	movs	r5, #0
	bl 0x0200c3bc
	cmp	r5, r7
	bge.n	.L_020020a8
.L_0200203a:
	ldr	r1, [pc, #120]
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r2, r2, r5
	ldrb	r0, [r1, r2]
	mov	sl, r1
	mov	r8, r2
	bl 0x0200c194
	movs	r4, #56
	ldrsh	r3, [r0, r4]
	cmp	r3, #0
	ble.n	.L_02002062
	movs	r1, #183
	lsls	r1, r1, #1
	adds	r2, r6, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020020a2
.L_02002062:
	mov	r3, sp
	lsls	r2, r5, #1
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_020020a2
	movs	r2, #182
	lsls	r2, r2, #1
	adds	r1, r6, r2
	ldrh	r3, [r1, #0]
	movs	r4, #184
	adds	r2, r3, #1
	lsls	r3, r3, #16
	lsls	r4, r4, #1
	asrs	r3, r3, #15
	strh	r2, [r1, #0]
	adds	r3, r3, r4
	mov	r1, sl
	mov	r4, r8
	ldrb	r2, [r1, r4]
	movs	r1, #181
	strh	r2, [r6, r3]
	movs	r3, #255
	lsls	r1, r1, #1
	lsls	r3, r3, #8
	adds	r2, r6, r1
	adds	r3, #255
	strh	r3, [r2, #0]
	movs	r3, #50
	adds	r3, #255
	adds	r2, r0, r3
	movs	r3, #0
	strb	r3, [r2, #0]
.L_020020a2:
	adds	r5, #1
	cmp	r5, r7
	blt.n	.L_0200203a
.L_020020a8:
	add	sp, #16
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #96]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200c2f4
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
	bl 0x0200c104
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_020020f6
	adds	r3, #15
.L_020020f6:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	movs	r1, #128
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	lsls	r1, r1, #5
	adds	r3, r3, r2
	str	r3, [r6, #28]
	ldr	r2, [r6, #80]
	ldrh	r3, [r2, #18]
	adds	r3, r3, r1
	strh	r3, [r2, #18]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #32]
	ldr	r2, [r3, #0]
	movs	r3, #7
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_02002134
	cmp	r2, #4
	beq.n	.L_0200213c
	b.n	.L_02002142
.L_02002134:
	movs	r1, #10
.L_02002136:
	bl 0x0200c274
	b.n	.L_02002142
.L_0200213c:
	movs	r1, #0
	bl 0x0200c274
.L_02002142:
	pop	{pc}
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #484]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	adds	r3, r5, r0
	ldrb	r3, [r3, #0]
	sub	sp, #16
	cmp	r3, #0
	beq.n	.L_0200216a
	b.n	.L_02002328
.L_0200216a:
	movs	r0, #10
	movs	r1, #0
	negs	r0, r0
	bl 0x02009fcc
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldr	r0, [r3, #0]
	bl 0x0200c2f4
	ldr	r2, [pc, #444]
	adds	r6, r0, #0
	str	r2, [sp, #0]
	bl 0x0200c2dc
	movs	r0, #0
	bl 0x0200c44c
	ldr	r3, [pc, #432]
	adds	r0, r6, #0
	str	r3, [r6, #108]
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	movs	r1, #49
	bl 0x0200c1cc
.L_020021a2:
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	bl 0x0200c22c
	ldr	r3, [sp, #0]
	ldr	r1, [sp, #0]
	adds	r3, #104
	adds	r1, #106
	mov	r9, r1
	mov	sl, r3
	add	r1, sp, #4
	cmp	r0, #7
	bne.n	.L_0200221e
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	mov	r1, r9
	lsls	r3, r3, #17
	str	r3, [r6, #36]
	movs	r5, #0
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	lsls	r3, r3, #17
	str	r3, [r6, #44]
	movs	r3, #128
	lsls	r3, r3, #5
	str	r3, [r6, #52]
.L_020021dc:
	mov	r0, sl
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	ldr	r3, [r6, #8]
	lsls	r2, r2, #19
	add	r1, sp, #4
	adds	r3, r3, r2
	str	r3, [r1, #0]
	mov	r0, r9
	ldr	r3, [r6, #12]
	str	r3, [r1, #4]
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	ldr	r3, [r6, #16]
	lsls	r2, r2, #19
	adds	r3, r3, r2
	str	r3, [r1, #8]
	adds	r0, r6, #0
	bl 0x0200c24c
	cmp	r0, #0
	beq.n	.L_02002210
	movs	r3, #0
	str	r3, [r6, #36]
	str	r3, [r6, #44]
	b.n	.L_0200231c
.L_02002210:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200c114
	cmp	r5, #9
	ble.n	.L_020021dc
	b.n	.L_0200231c
.L_0200221e:
	mov	r0, sl
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	ldr	r3, [r6, #8]
	lsls	r2, r2, #19
	adds	r3, r3, r2
	str	r3, [r1, #0]
	mov	r0, r9
	ldr	r3, [r6, #12]
	str	r3, [r1, #4]
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	ldr	r3, [r6, #16]
	lsls	r2, r2, #19
	adds	r3, r3, r2
	str	r3, [r1, #8]
	adds	r0, r6, #0
	bl 0x0200c24c
	cmp	r0, #0
	bgt.n	.L_0200231c
	cmp	r0, #0
	bge.n	.L_02002268
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	ldr	r3, [pc, #232]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	movs	r2, #1
	ldr	r0, [r3, #0]
	movs	r1, #6
	negs	r2, r2
	bl 0x0200c36c
	b.n	.L_0200231c
.L_02002268:
	ldrh	r3, [r6, #32]
	movs	r2, #0
	subs	r3, #2
	mov	fp, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #20]
	mov	r8, r2
	adds	r7, r5, #0
	adds	r7, #89
.L_0200227c:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_020022ac
	ldrb	r2, [r7, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020022ac
	cmp	r5, r6
	beq.n	.L_020022ac
	ldrh	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r0, #8
	subs	r3, #2
	mov	r1, fp
	add	r2, sp, #4
	bl 0x0200c28c
	cmp	r0, #0
	blt.n	.L_020022ac
	movs	r0, #1
	bl 0x0200c114
	b.n	.L_0200231c
.L_020022ac:
	movs	r3, #1
	add	r8, r3
	mov	r0, r8
	adds	r7, #128
	adds	r5, #128
	cmp	r0, #63
	ble.n	.L_0200227c
	mov	r2, sl
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	ldr	r2, [r6, #8]
	lsls	r3, r3, #17
	adds	r1, r2, r3
	str	r1, [r6, #8]
	mov	r2, r9
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	ldr	r2, [r6, #16]
	ldr	r7, [pc, #116]
	lsls	r3, r3, #17
	ldr	r0, [pc, #116]
	adds	r5, r2, r3
	adds	r3, r1, #0
	ands	r3, r7
	movs	r4, #128
	adds	r2, r3, r0
	lsls	r4, r4, #9
	str	r5, [r6, #16]
	cmp	r2, r4
	ble.n	.L_020022ea
	adds	r2, r4, #0
.L_020022ea:
	ldr	r0, [pc, #100]
	cmp	r2, r0
	bge.n	.L_020022f2
	adds	r2, r0, #0
.L_020022f2:
	subs	r3, r1, r2
	ldr	r1, [pc, #84]
	str	r3, [r6, #8]
	adds	r3, r5, #0
	ands	r3, r7
	adds	r2, r3, r1
	cmp	r2, r4
	ble.n	.L_02002304
	adds	r2, r4, #0
.L_02002304:
	cmp	r2, r0
	bge.n	.L_0200230a
	adds	r2, r0, #0
.L_0200230a:
	subs	r3, r5, r2
	str	r3, [r6, #16]
	ldr	r2, [sp, #0]
	movs	r3, #0
	strh	r3, [r2, #4]
	movs	r0, #1
	bl 0x0200c114
	b.n	.L_020021a2
.L_0200231c:
	movs	r3, #0
	str	r3, [r6, #108]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200c274
.L_02002328:
	ldr	r0, [sp, #0]
	movs	r3, #0
	strh	r3, [r0, #4]
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200254c
	.4byte 0x0200a121
	.4byte 0x000fffff
	.4byte 0xfff80000
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	ldr	r3, [pc, #16]
	movs	r2, #4
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02002364
	bl 0x0200a148
.L_02002364:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	movs	r1, #217
	lsls	r1, r1, #1
	adds	r6, r5, r1
	ldrh	r3, [r6, #0]
	sub	sp, #12
	cmp	r3, #0
	bne.n	.L_02002398
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r5, r2
	adds	r1, #2
	ldr	r0, [r3, #0]
	adds	r3, r5, r1
	ldr	r1, [r3, #0]
	bl 0x0200c3fc
	movs	r3, #1
	strh	r3, [r6, #0]
.L_02002398:
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200c1ac
	movs	r2, #179
	lsls	r2, r2, #1
	movs	r1, #173
	adds	r3, r5, r2
	lsls	r1, r1, #1
	movs	r2, #0
	strh	r2, [r3, #0]
	adds	r3, r5, r1
	adds	r1, #4
	strh	r2, [r3, #0]
	adds	r3, r5, r1
	subs	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r5, r1
	adds	r1, #8
	strh	r2, [r3, #0]
	movs	r0, #10
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	movs	r1, #0
	negs	r0, r0
	bl 0x02009fcc
	movs	r0, #224
	movs	r1, #224
	lsls	r1, r1, #8
	lsls	r0, r0, #11
	bl 0x0200c3c4
	ldr	r3, [pc, #192]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c2f4
	adds	r6, r0, #0
	movs	r0, #131
	lsls	r0, r0, #1
	ldr	r7, [pc, #176]
	bl 0x0200c1a4
	bl 0x0200c2dc
	movs	r0, #0
	bl 0x0200c44c
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200c1ac
	ldr	r3, [pc, #156]
	movs	r1, #49
	str	r3, [r6, #108]
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	adds	r0, r6, #0
	bl 0x0200c1cc
	ldr	r3, [r7, #108]
	adds	r3, #100
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_02002484
.L_02002424:
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	movs	r0, #0
	bl 0x0200c22c
	ldr	r1, [r7, #108]
	cmp	r0, #7
	beq.n	.L_02002444
	ldr	r2, [r1, #112]
	ldr	r3, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	ldr	r3, [r6, #16]
	ldr	r2, [r1, #120]
	adds	r3, r3, r2
	b.n	.L_0200246c
.L_02002444:
	ldr	r3, [r1, #112]
	cmp	r3, #0
	beq.n	.L_02002458
	ldr	r3, [r6, #8]
	ldr	r2, [pc, #88]
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r6, #8]
.L_02002458:
	ldr	r3, [r7, #108]
	ldr	r3, [r3, #120]
	cmp	r3, #0
	beq.n	.L_0200246e
	ldr	r3, [r6, #16]
	ldr	r2, [pc, #68]
	movs	r1, #128
	ands	r3, r2
	lsls	r1, r1, #12
	adds	r3, r3, r1
.L_0200246c:
	str	r3, [r6, #16]
.L_0200246e:
	movs	r3, #0
	strh	r3, [r7, #4]
	movs	r0, #1
	bl 0x0200c114
	ldr	r3, [r7, #108]
	adds	r3, #100
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02002424
.L_02002484:
	movs	r5, #0
	movs	r0, #30
	bl 0x0200c2d4
	adds	r0, r6, #0
	str	r5, [r6, #108]
	movs	r1, #0
	bl 0x0200c274
	strh	r5, [r7, #4]
	add	sp, #12
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200254c
	.4byte 0x0200a121
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb500
	ldr	r3, [pc, #20]
	movs	r2, #4
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_020024c0
	bl 0x0200a36c
	movs	r0, #1
	b.n	.L_020024c2
.L_020024c0:
	movs	r0, #0
.L_020024c2:
	pop	{pc}
	.4byte 0x0200254c
	.4byte 0x22044b01
	.4byte 0x47705e98
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r1, [pc, #344]
	sub	sp, #4
	ldr	r3, [r1, #112]
	mov	fp, r0
	cmp	r3, #0
	bne.n	.L_020024f0
	b.n	.L_02002648
.L_020024f0:
	movs	r2, #0
	str	r2, [sp, #0]
.L_020024f4:
	bl 0x0200c12c
	adds	r5, r0, #0
	bl 0x0200c12c
	mov	r3, fp
	ldr	r3, [r3, #8]
	lsls	r5, r5, #4
	mov	r8, r3
	add	r8, r5
	lsls	r0, r0, #4
	mov	r1, r8
	subs	r1, r1, r0
	mov	r8, r1
	bl 0x0200c12c
	adds	r6, r0, #0
	bl 0x0200c12c
	adds	r5, r0, #0
	bl 0x0200c12c
	mov	r2, fp
	ldr	r3, [r2, #16]
	ldr	r2, [r2, #12]
	lsls	r5, r5, #4
	lsls	r6, r6, #3
	movs	r1, #128
	lsls	r0, r0, #4
	adds	r6, r6, r2
	lsls	r1, r1, #11
	adds	r3, r3, r5
	subs	r3, r3, r0
	adds	r6, r6, r1
	movs	r0, #234
	adds	r0, #255
	mov	r1, r8
	adds	r2, r6, #0
	bl 0x0200c1e4
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02002628
	bl 0x0200c12c
	mov	sl, r0
	bl 0x0200c12c
	adds	r6, r0, #0
	bl 0x0200c12c
	adds	r5, r0, #0
	bl 0x0200c12c
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r5, r5, r0
	ldr	r1, [pc, #216]
	adds	r0, r7, #0
	mov	r9, r2
	bl 0x0200c1dc
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200c254
	mov	r1, sl
	movs	r2, #128
	lsls	r2, r2, #10
	lsls	r3, r1, #2
	adds	r3, r3, r2
	str	r3, [r7, #40]
	mov	r0, sl
	bl 0x0200c144
	ldr	r3, [pc, #184]
	lsls	r6, r6, #3
	mov	r8, r3
	adds	r1, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x62f8
	mov	r0, sl
	bl 0x0200c13c
	adds	r1, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x4a24
	movs	r3, #168
	lsls	r3, r3, #7
	adds	r3, #122
	str	r3, [r7, #72]
	lsrs	r5, r5, #2
	ldr	r3, [r2, #112]
	add	r5, r9
	movs	r6, #0
	mov	r1, r9
	str	r5, [r7, #24]
	str	r5, [r7, #28]
	str	r1, [r7, #68]
	ldr	r5, [r7, #80]
	str	r0, [r7, #36]
	str	r6, [r7, #52]
	ldr	r3, [r3, #80]
	ldrb	r0, [r5, #16]
	mov	r8, r3
	bl 0x0200c174
	ldrb	r3, [r5, #17]
	ldr	r1, [pc, #100]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r5, #17]
	ldrh	r3, [r1, #12]
	ldr	r0, [r5, #40]
	strb	r3, [r5, #16]
	bl 0x0200c1c4
	str	r6, [r5, #40]
	strb	r6, [r5, #27]
	mov	r2, r8
	ldrb	r3, [r2, #20]
	ldrb	r0, [r5, #5]
	strb	r3, [r5, #20]
	ldrb	r3, [r2, #21]
	strb	r3, [r5, #21]
	ldrb	r1, [r2, #5]
	movs	r2, #63
	adds	r3, r2, #0
	lsrs	r1, r1, #6
	lsls	r1, r1, #6
	ands	r3, r0
	orrs	r3, r1
	strb	r3, [r5, #5]
	mov	r1, r8
	ldrb	r3, [r1, #7]
	ldrb	r1, [r5, #7]
	lsrs	r3, r3, #6
	lsls	r3, r3, #6
	ands	r2, r1
	orrs	r2, r3
	strb	r2, [r5, #7]
	mov	r3, r8
	ldrh	r2, [r3, #8]
	ldr	r1, [pc, #28]
	ldrh	r3, [r5, #8]
	lsls	r2, r2, #22
	lsrs	r2, r2, #22
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #8]
.L_02002628:
	ldr	r1, [sp, #0]
	subs	r1, #1
	str	r1, [sp, #0]
	cmp	r1, #0
	blt.n	.L_02002634
	b.n	.L_020024f4
.L_02002634:
	b.n	.L_02002648
	.2byte 0x0000
	.4byte 0xfffffc00
	.4byte 0x0200254c
	.4byte 0x0200c774
	.2byte 0x021c
	.2byte 0x0300
.L_02002648:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	str	r3, [sp, #0]
	movs	r3, #1
	mov	fp, r3
.L_02002674:
	movs	r0, #70
	adds	r0, #255
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #4]
	bl 0x0200c1e4
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02002716
	bl 0x0200c12c
	adds	r5, r0, #0
	bl 0x0200c12c
	ldr	r3, [sp, #0]
	lsrs	r5, r5, #4
	adds	r5, r3, r5
	lsrs	r0, r0, #4
	movs	r3, #128
	subs	r5, r5, r0
	lsls	r3, r3, #7
	mov	sl, r3
	adds	r3, r5, #0
	add	r3, sl
	mov	r9, r3
	bl 0x0200c12c
	mov	r8, r0
	mov	r3, r8
	lsls	r3, r3, #3
	mov	r8, r3
	bl 0x0200c12c
	ldr	r1, [pc, #116]
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl 0x0200c1dc
	movs	r1, #0
	adds	r0, r7, #0
	bl 0x0200c254
	movs	r3, #160
	lsls	r3, r3, #9
	adds	r5, r5, r3
	str	r5, [r7, #40]
	mov	r0, r9
	bl 0x0200c144
	ldr	r5, [pc, #88]
	adds	r1, r0, #0
	mov	r0, r8
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x62f8
	mov	r0, r9
	bl 0x0200c13c
	adds	r1, r0, #0
	mov	r0, r8
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2300
	str	r3, [r7, #52]
	movs	r3, #168
	lsls	r3, r3, #7
	adds	r3, #122
	lsrs	r6, r6, #1
	str	r3, [r7, #72]
	movs	r3, #128
	add	r6, sl
	lsls	r3, r3, #8
	str	r0, [r7, #36]
	str	r6, [r7, #24]
	str	r6, [r7, #28]
	str	r3, [r7, #68]
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200c274
.L_02002716:
	movs	r3, #1
	negs	r3, r3
	add	fp, r3
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_02002674
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200c7b8
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200c19c
	adds	r5, #91
	strb	r0, [r5, #0]
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r3, #192
	movs	r0, #100
	lsls	r3, r3, #18
	adds	r0, r0, r5
	ldr	r6, [r3, #108]
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	sub	sp, #56
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L_02002774
	b.n	.L_02002a00
.L_02002774:
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200c19c
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #0
	beq.n	.L_0200278e
	movs	r3, #1
	orrs	r0, r3
.L_0200278e:
	adds	r3, r5, #0
	adds	r3, #91
	strb	r0, [r3, #0]
	add	r7, sp, #44
	ldr	r3, [r5, #8]
	movs	r0, #128
	str	r3, [r7, #0]
	lsls	r0, r0, #12
	ldr	r3, [r5, #12]
	adds	r2, r7, #0
	str	r3, [r7, #4]
	ldr	r3, [r5, #16]
	str	r3, [r7, #8]
	ldrh	r1, [r5, #6]
	bl 0x0200c14c
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
	movs	r0, #0
	bl 0x0200c22c
	cmp	r0, #7
	bne.n	0x0200a804
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r5, #64]
	str	r3, [r5, #60]
	str	r3, [r5, #56]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r5, #52]
	mov	r0, r8
	movs	r3, #1
	strh	r3, [r0, #0]
	movs	r0, #145
	bl 0x0200c46c
	movs	r0, #160
	lsls	r0, r0, #11
	movs	r2, #128
	adds	r1, r0, #0
	lsls	r2, r2, #9
	bl 0x0200c264
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200c264
	ldr	r3, [r5, #104]
	cmp	r3, #0
	beq.n	0x0200a804
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x23c0
	lsls	r3, r3, #18
	ldr	r0, [r3, #32]
	ldr	r3, [r5, #8]
	ldr	r1, [r5, #16]
	asrs	r3, r3, #20
	str	r3, [sp, #32]
	movs	r3, #184
	lsls	r3, r3, #1
	ldr	r4, [sp, #32]
	asrs	r1, r1, #20
	adds	r2, r0, r3
	ldr	r2, [r2, #0]
	lsls	r3, r1, #7
	adds	r3, r4, r3
	lsls	r3, r3, #2
	adds	r2, r2, r3
	str	r2, [sp, #28]
	movs	r4, #212
	lsls	r4, r4, #1
	adds	r2, r0, r4
	ldr	r2, [r2, #0]
	subs	r4, #92
	adds	r2, r2, r3
	str	r2, [sp, #24]
	movs	r2, #164
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldr	r3, [r3, #0]
	asrs	r3, r3, #20
	str	r3, [sp, #20]
	adds	r3, r0, r4
	adds	r4, #52
	ldr	r2, [r3, #0]
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	adds	r4, #4
	asrs	r3, r3, #20
	str	r3, [sp, #16]
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r0, #1
	asrs	r3, r3, #20
	adds	r3, r1, r3
	subs	r3, #2
	asrs	r2, r2, #20
	negs	r0, r0
	str	r3, [sp, #8]
	str	r0, [sp, #40]
	subs	r3, r1, #1
	adds	r1, r1, r2
	subs	r1, #1
	mov	r8, r3
	mov	fp, r1
.L_02002870:
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #20]
	movs	r4, #1
	adds	r3, r0, r1
	subs	r3, #1
	negs	r4, r4
	mov	r9, r3
	str	r4, [sp, #36]
	adds	r3, r0, r2
	adds	r6, r0, #0
	subs	r3, #1
	subs	r6, #1
	mov	sl, r3
.L_0200288c:
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #36]
	lsls	r3, r4, #7
	adds	r3, r3, r0
	lsls	r3, r3, #2
	ldr	r1, [sp, #28]
	str	r3, [sp, #12]
	adds	r2, r3, r1
	ldrb	r3, [r2, #2]
	cmp	r3, #77
	bne.n	.L_020028ec
	movs	r3, #0
	strb	r3, [r2, #2]
	mov	r2, sl
	mov	r3, fp
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #64
	movs	r1, #64
	movs	r2, #1
	movs	r3, #1
	bl 0x0200c244
	mov	r4, r8
	movs	r1, #64
	movs	r2, #1
	movs	r3, #1
	movs	r0, #64
	str	r4, [sp, #4]
	str	r6, [sp, #0]
	bl 0x0200c23c
	movs	r0, #159
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200c46c
	mov	r3, r8
	movs	r4, #128
	lsls	r4, r4, #12
	lsls	r2, r3, #20
	lsls	r0, r6, #20
	adds	r0, r0, r4
	ldr	r1, [r5, #12]
	ldrh	r3, [r5, #6]
	adds	r2, r2, r4
	bl 0x0200a658
.L_020028ec:
	ldr	r4, [sp, #12]
	ldr	r0, [sp, #24]
	adds	r2, r4, r0
	ldrb	r3, [r2, #2]
	cmp	r3, #77
	bne.n	.L_02002940
	movs	r3, #0
	strb	r3, [r2, #2]
	ldr	r2, [sp, #8]
	mov	r1, r9
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #64
	movs	r1, #64
	movs	r2, #1
	movs	r3, #2
	bl 0x0200c244
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #64
	movs	r2, #1
	movs	r3, #1
	movs	r0, #64
	str	r6, [sp, #0]
	bl 0x0200c23c
	movs	r0, #143
	lsls	r0, r0, #2
	bl 0x0200c46c
	mov	r3, r8
	movs	r4, #128
	lsls	r4, r4, #12
	lsls	r2, r3, #20
	lsls	r0, r6, #20
	adds	r0, r0, r4
	ldr	r1, [r5, #12]
	ldrh	r3, [r5, #6]
	adds	r2, r2, r4
	bl 0x0200a658
.L_02002940:
	ldr	r0, [sp, #36]
	movs	r4, #1
	adds	r0, #1
	add	r9, r4
	adds	r6, #1
	add	sl, r4
	str	r0, [sp, #36]
	cmp	r0, #1
	ble.n	.L_0200288c
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #40]
	adds	r1, #1
	adds	r2, #1
	str	r1, [sp, #8]
	add	r8, r4
	add	fp, r4
	str	r2, [sp, #40]
	cmp	r2, #1
	ble.n	.L_02002870
	ldr	r3, [r5, #24]
	movs	r4, #128
	lsls	r4, r4, #9
	cmp	r3, r4
	bge.n	.L_0200297e
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	str	r3, [r5, #24]
	ldr	r3, [r5, #28]
	adds	r3, r3, r2
	str	r3, [r5, #28]
.L_0200297e:
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	adds	r0, r5, #0
	bl 0x0200c204
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #108]
	ldr	r3, [pc, #176]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	ldr	r6, [pc, #172]
	bl 0x0200c2f4
	ldr	r1, [r5, #8]
	ldr	r3, [r0, #8]
	subs	r2, r1, r3
	cmp	r2, #0
	blt.n	.L_020029b4
	movs	r1, #160
	lsls	r1, r1, #13
	cmp	r2, r1
	blt.n	.L_020029be
	b.n	.L_02002a34
.L_020029b4:
	movs	r2, #160
	subs	r3, r3, r1
	lsls	r2, r2, #13
	cmp	r3, r2
	bge.n	.L_02002a34
.L_020029be:
	ldr	r3, [r5, #12]
	ldr	r2, [r0, #12]
	ldr	r4, [pc, #136]
	ldr	r1, [pc, #136]
	subs	r3, r3, r2
	adds	r3, r3, r4
	cmp	r3, r1
	bhi.n	.L_02002a34
	ldr	r3, [r5, #16]
	ldr	r0, [r0, #16]
	subs	r2, r3, r0
	cmp	r2, #0
	blt.n	.L_020029e2
	movs	r3, #160
	lsls	r3, r3, #13
	cmp	r2, r3
	blt.n	.L_020029ec
	b.n	.L_02002a34
.L_020029e2:
	movs	r4, #160
	subs	r3, r0, r3
	lsls	r4, r4, #13
	cmp	r3, r4
	bge.n	.L_02002a34
.L_020029ec:
	movs	r3, #2
	strh	r3, [r6, #4]
	ldrh	r3, [r6, #10]
	movs	r0, #170
	lsls	r0, r0, #1
	adds	r3, #2
	adds	r2, r7, r0
	str	r5, [r6, #108]
	strh	r3, [r2, #0]
	b.n	.L_02002a34
.L_02002a00:
	cmp	r3, #1
	bne.n	.L_02002a34
	adds	r3, r5, #0
	adds	r3, #91
	movs	r2, #0
	strb	r2, [r3, #0]
	ldr	r3, [r5, #24]
	cmp	r3, #0
	ble.n	.L_02002a26
	ldr	r2, [pc, #64]
	adds	r0, r5, #0
	adds	r3, r3, r2
	str	r3, [r5, #24]
	ldr	r3, [r5, #28]
	adds	r3, r3, r2
	str	r3, [r5, #28]
	bl 0x0200a4d4
	b.n	.L_02002a34
.L_02002a26:
	str	r2, [r5, #16]
	str	r2, [r5, #12]
	str	r2, [r5, #8]
	str	r2, [r5, #44]
	str	r2, [r5, #40]
	str	r2, [r5, #36]
	str	r2, [r5, #108]
.L_02002a34:
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200254c
	.4byte 0x0007ffff
	.4byte 0x001ffffe
	.2byte 0xf000
	.2byte 0xffff
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	ldr	r6, [sp, #24]
	adds	r5, r1, #0
	mov	r9, r2
	mov	sl, r3
	bl 0x0200c2f4
	mov	r8, r0
	adds	r0, r5, #0
	bl 0x0200c2f4
	adds	r5, r0, #0
	mov	r0, r8
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #16]
	ldr	r1, [r0, #8]
	adds	r0, r5, #0
	bl 0x0200c1f4
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl 0x0200c1dc
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200c254
	adds	r3, r5, #0
	adds	r3, #85
	movs	r6, #0
	strb	r6, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200c1cc
	adds	r3, r5, #0
	mov	r2, r8
	adds	r3, #100
	str	r2, [r5, #104]
	mov	r0, sl
	strh	r6, [r3, #0]
	adds	r3, #2
	strh	r0, [r3, #0]
	mov	r2, r9
	subs	r3, #4
	strb	r2, [r3, #0]
	ldr	r3, [pc, #12]
	str	r3, [r5, #108]
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	.2byte 0xa739
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r0
	mov	r3, fp
	adds	r3, #98
	ldrb	r0, [r3, #0]
	sub	sp, #12
	bl 0x0200c2f4
	mov	r3, fp
	adds	r7, r0, #0
	adds	r3, #102
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	ldr	r3, [r7, #80]
	mov	r2, fp
	mov	r9, r3
	ldr	r3, [r2, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	movs	r0, #128
	ldr	r3, [r2, #12]
	ldr	r2, [pc, #88]
	adds	r1, r6, #0
	adds	r3, r3, r2
	str	r3, [r5, #4]
	mov	r2, fp
	ldr	r3, [r2, #16]
	lsls	r0, r0, #14
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl 0x0200c14c
	ldr	r3, [pc, #60]
	movs	r2, #0
	mov	sl, r3
	adds	r3, r7, #0
	mov	r8, r2
	adds	r3, #85
	mov	r2, sl
	strh	r6, [r7, #6]
	strb	r2, [r3, #0]
	adds	r0, r7, #0
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	bl 0x0200c1f4
	ldr	r3, [pc, #40]
	mov	r2, sl
	str	r3, [r7, #108]
	adds	r3, r7, #0
	adds	r3, #90
	strb	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r7, #52]
	ldr	r3, [pc, #20]
	mov	r2, r9
	adds	r6, r6, r3
	b.n	.L_02002b6c
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xfff40000
	.4byte 0x0200a74d
	.2byte 0xc000
	.2byte 0xffff
.L_02002b6c:
	.2byte 0x4643
	strh	r6, [r2, #18]
	str	r3, [r7, #24]
	str	r3, [r7, #28]
	adds	r3, r7, #0
	mov	r2, r8
	adds	r3, #100
	strh	r2, [r3, #0]
	mov	r2, fp
	ldr	r3, [r2, #104]
	movs	r0, #104
	str	r3, [r7, #104]
	bl 0x0200c46c
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r7, r5, #0
	adds	r7, #100
	movs	r3, #0
.L_02002ba2:
	ldrsh	r6, [r7, r3]
	cmp	r6, #0
	bne.n	.L_02002bd0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl 0x0200c21c
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #9
	str	r0, [r5, #12]
	movs	r1, #1
	adds	r0, r5, #0
	str	r6, [r5, #24]
	str	r6, [r5, #28]
	str	r3, [r5, #48]
	str	r3, [r5, #52]
	bl 0x0200c27c
	b.n	.L_02002c38
.L_02002bd0:
	cmp	r6, #30
	bgt.n	.L_02002be8
	cmp	r6, #30
	bne.n	.L_02002c38
	movs	r0, #136
	bl 0x0200c46c
	ldr	r0, [r5, #104]
	movs	r1, #2
	bl 0x0200c1cc
	b.n	.L_02002c38
.L_02002be8:
	cmp	r6, #60
	bgt.n	.L_02002c10
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	ldrh	r3, [r7, #0]
	ldr	r0, [r5, #104]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	ldr	r3, [pc, #48]
	asrs	r2, r2, #1
	ands	r2, r3
	lsls	r1, r2, #3
	subs	r1, r1, r2
	bl 0x0200c274
	b.n	.L_02002c38
.L_02002c10:
	movs	r1, #1
	ldr	r0, [r5, #104]
	bl 0x0200c1cc
	movs	r0, #184
	bl 0x0200c46c
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl 0x0200c21c
	movs	r3, #0
	str	r0, [r5, #12]
	str	r3, [r5, #104]
	movs	r0, #0
	b.n	.L_02002c40
	.2byte 0x0000
	.2byte 0x0001
	.2byte 0x0000
.L_02002c38:
	ldrh	r3, [r7, #0]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r7, #0]
.L_02002c40:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r7, r5, #0
	adds	r7, #100
	movs	r3, #0
	ldrsh	r6, [r7, r3]
	cmp	r6, #0
	bne.n	.L_02002c8e
	movs	r0, #136
	bl 0x0200c46c
	ldr	r0, [r5, #104]
	movs	r1, #2
	bl 0x0200c1cc
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl 0x0200c21c
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #48]
	movs	r3, #128
	str	r0, [r5, #12]
	lsls	r3, r3, #8
	adds	r0, r5, #0
	movs	r1, #1
	str	r6, [r5, #24]
	str	r6, [r5, #28]
	str	r3, [r5, #52]
	bl 0x0200c27c
	b.n	.L_02002cdc
.L_02002c8e:
	cmp	r6, #32
	bgt.n	.L_02002cb6
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	ldrh	r3, [r7, #0]
	ldr	r0, [r5, #104]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	ldr	r3, [pc, #48]
	asrs	r2, r2, #1
	ands	r2, r3
	lsls	r1, r2, #3
	subs	r1, r1, r2
	bl 0x0200c274
	b.n	.L_02002cdc
.L_02002cb6:
	movs	r1, #1
	ldr	r0, [r5, #104]
	bl 0x0200c1cc
	movs	r0, #184
	bl 0x0200c46c
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl 0x0200c21c
	movs	r3, #0
	str	r0, [r5, #12]
	str	r3, [r5, #104]
	movs	r0, #0
	b.n	.L_02002ce4
	.2byte 0x0001
	.2byte 0x0000
.L_02002cdc:
	ldrh	r3, [r7, #0]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r7, #0]
.L_02002ce4:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200aad0
	movs	r3, #0
	str	r3, [r5, #8]
	str	r3, [r5, #12]
	str	r3, [r5, #16]
	str	r3, [r5, #36]
	str	r3, [r5, #40]
	str	r3, [r5, #44]
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #100
	movs	r0, #0
	ldrsh	r1, [r2, r0]
	ldrh	r3, [r2, #0]
	cmp	r1, #0
	beq.n	.L_02002d1c
	subs	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02002d82
.L_02002d1c:
	adds	r3, r5, #0
	adds	r3, #90
	movs	r0, #131
	strb	r1, [r3, #0]
	lsls	r0, r0, #1
	bl 0x0200c19c
	movs	r3, #1
	negs	r3, r3
	cmp	r0, #0
	bne.n	.L_02002d42
	ldr	r3, [pc, #80]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #76]
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r3, [r1, r3]
.L_02002d42:
	movs	r0, #1
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_02002d54
	adds	r0, r5, #0
	movs	r1, #9
	bl 0x0200c1cc
	b.n	.L_02002d82
.L_02002d54:
	ldrh	r1, [r5, #6]
	movs	r2, #128
	subs	r3, r3, r1
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	lsls	r2, r2, #5
	cmp	r3, r2
	ble.n	.L_02002d66
	adds	r3, r2, #0
.L_02002d66:
	ldr	r2, [pc, #36]
	cmp	r3, r2
	bge.n	.L_02002d6e
	adds	r3, r2, #0
.L_02002d6e:
	adds	r3, r1, r3
	adds	r0, r5, #0
	movs	r1, #2
	strh	r3, [r5, #6]
	bl 0x0200c1cc
	adds	r0, r5, #0
	movs	r1, #48
.L_02002d7e:
	bl 0x0200c1d4
.L_02002d82:
	pop	{r5, pc}
	.4byte 0x03001150
	.4byte 0x0200c7fc
	.2byte 0xf000
	.2byte 0xffff
	push	{lr}
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
	beq.n	.L_02002dc0
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200c1a4
	bl 0x0200c3f4
	bl 0x0200c444
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200c1ac
.L_02002dc0:
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
	bl 0x0200c434
	adds	r7, r0, #0
.L_02002de4:
	bl 0x0200ad90
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
	bl 0x0200c224
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
.L_02002e64:
	cmp	r5, r0
	bge.n	.L_02002e9c
	ldr	r3, [pc, #36]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #36]
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r2, [r1, r3]
	mov	r9, r2
	lsls	r3, r2, #16
	ldr	r2, [pc, #24]
	cmp	r3, r2
	bne.n	.L_02002ec8
	b.n	.L_0200305e
	.4byte 0x02000240
	.4byte 0xfff00000
	.4byte 0x0300021c
	.4byte 0x03001150
	.4byte 0x0200c83c
	.2byte 0x0000
	.2byte 0xffff
.L_02002e9c:
	.2byte 0x9b04
	ldr	r2, [sp, #20]
	ldr	r0, [r7, #16]
	ldr	r1, [r7, #8]
	subs	r0, r3, r0
	subs	r1, r2, r1
	bl 0x0200c134
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
	b.n	.L_02002ec8
	.2byte 0xc000
	.2byte 0xffff
.L_02002ec8:
	.2byte 0x2080
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x0200c14c
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldr	r1, [r2, #0]
	ldrb	r0, [r3, #0]
	ldr	r2, [r2, #8]
	bl 0x0200c224
	mov	fp, r0
	cmp	r0, #255
	beq.n	.L_02002f4a
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldrb	r0, [r3, #0]
	ldr	r1, [r2, #0]
	ldr	r2, [r2, #8]
	bl 0x0200c21c
.L_02002ef6:
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_02002f4a
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
	bl 0x0200c204
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200c1cc
	adds	r0, r7, #0
	movs	r1, #48
	bl 0x0200c1d4
	adds	r0, r7, #0
	bl 0x0200c20c
	ldr	r3, [pc, #292]
	str	r3, [r7, #108]
	b.n	.L_02002ff4
.L_02002f4a:
	movs	r3, #0
	mov	r0, r9
	strh	r0, [r7, #6]
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r2, [sp, #12]
	str	r2, [r7, #8]
	ldr	r3, [sp, #8]
	str	r3, [r7, #16]
	b.n	.L_02003040
.L_02002f5e:
	ldr	r2, [sp, #4]
	ldr	r1, [r6, #0]
	ldrb	r0, [r2, #0]
	ldr	r2, [r6, #8]
	bl 0x0200c21c
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_02003014
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
.L_02002f8c:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02002fb6
	mov	r3, r8
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02002fb6
	cmp	r5, r7
	beq.n	.L_02002fb6
	ldrh	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r0, #8
	subs	r3, #2
	ldr	r1, [sp, #0]
	add	r2, sp, #28
	bl 0x0200c28c
	cmp	r0, #0
	bge.n	.L_02003014
.L_02002fb6:
	movs	r0, #1
	add	sl, r0
	movs	r2, #128
	mov	r3, sl
	add	r8, r2
	adds	r5, #128
	cmp	r3, #63
	ble.n	.L_02002f8c
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
	bl 0x0200c204
	adds	r0, r7, #0
	bl 0x0200c20c
	ldr	r3, [sp, #24]
	cmp	fp, r3
	bne.n	.L_0200303a
.L_02002ff4:
	movs	r0, #128
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x0200c14c
	ldr	r2, [sp, #4]
	add	r6, sp, #28
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl 0x0200c224
	mov	fp, r0
	cmp	r0, #255
	bne.n	.L_02002f5e
.L_02003014:
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
	bl 0x0200c204
	adds	r0, r7, #0
	bl 0x0200c20c
	movs	r0, #2
	bl 0x0200c114
	b.n	.L_02002de4
.L_0200303a:
	movs	r0, #10
	bl 0x0200c114
.L_02003040:
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
	bl 0x0200c1cc
.L_0200305e:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0xad05
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
	bl 0x0200c434
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #56]
	adds	r7, r0, #0
	strh	r3, [r2, #0]
.L_02003096:
	bl 0x0200ad90
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
	b.n	.L_020030dc
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0x0200ccf0
	.2byte 0x0000
	.2byte 0xfff0
.L_020030dc:
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
	bl 0x0200c224
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
	bge.n	.L_02003158
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
	bne.n	.L_02003184
	b.n	.L_0200334e
	.4byte 0x0300021c
	.4byte 0x03001150
	.4byte 0x0200c83c
	.2byte 0x0000
	.2byte 0xffff
.L_02003158:
	.2byte 0x9b04
	ldr	r2, [sp, #20]
	ldr	r0, [r7, #16]
	ldr	r1, [r7, #8]
	subs	r0, r3, r0
	subs	r1, r2, r1
	bl 0x0200c134
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
	b.n	.L_02003184
	.2byte 0xc000
	.2byte 0xffff
.L_02003184:
	.2byte 0x2080
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x0200c14c
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldr	r1, [r2, #0]
	ldrb	r0, [r3, #0]
	ldr	r2, [r2, #8]
	bl 0x0200c224
	mov	fp, r0
	cmp	r0, #255
	beq.n	.L_020031fe
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldrb	r0, [r3, #0]
	ldr	r1, [r2, #0]
	ldr	r2, [r2, #8]
	bl 0x0200c21c
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_020031fe
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
	bl 0x0200c204
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200c1cc
	adds	r0, r7, #0
	movs	r1, #48
	bl 0x0200c1d4
	movs	r5, #0
	b.n	.L_02003226
.L_020031fe:
	movs	r3, #0
	mov	r0, r9
	strh	r0, [r7, #6]
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r2, [sp, #12]
	str	r2, [r7, #8]
	ldr	r3, [sp, #8]
	str	r3, [r7, #16]
	b.n	.L_0200334e
.L_02003212:
	ldr	r3, [pc, #360]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	beq.n	.L_0200321e
	b.n	.L_0200334e
.L_0200321e:
	movs	r0, #1
	bl 0x0200c114
	adds	r5, #1
.L_02003226:
	cmp	r5, #179
	bgt.n	.L_02003234
	adds	r0, r7, #0
	bl 0x0200c284
	cmp	r0, #0
	beq.n	.L_02003212
.L_02003234:
	ldr	r3, [pc, #328]
	str	r3, [r7, #108]
	b.n	.L_02003302
.L_0200323a:
	ldr	r2, [sp, #4]
	ldr	r1, [r6, #0]
	ldrb	r0, [r2, #0]
	ldr	r2, [r6, #8]
	bl 0x0200c21c
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_02003322
	ldr	r3, [pc, #296]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_0200334e
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
.L_02003272:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_0200329c
	mov	r3, r8
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200329c
	cmp	r5, r7
	beq.n	.L_0200329c
	ldrh	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r0, #8
	subs	r3, #2
	ldr	r1, [sp, #0]
	add	r2, sp, #28
	bl 0x0200c28c
	cmp	r0, #0
	bge.n	.L_02003322
.L_0200329c:
	movs	r0, #1
	add	sl, r0
	movs	r2, #128
	mov	r3, sl
	add	r8, r2
	adds	r5, #128
	cmp	r3, #63
	ble.n	.L_02003272
	ldr	r0, [r6, #0]
	movs	r3, #128
	str	r0, [sp, #20]
	lsls	r3, r3, #10
	ldr	r2, [r6, #8]
	adds	r0, r7, #0
	str	r2, [sp, #16]
	str	r3, [r7, #48]
.L_020032bc:
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r7, #52]
	movs	r5, #0
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	bl 0x0200c204
	b.n	.L_020032da
.L_020032d2:
	movs	r0, #1
	bl 0x0200c114
	adds	r5, #1
.L_020032da:
	cmp	r5, #179
	bgt.n	.L_020032f2
	adds	r0, r7, #0
	bl 0x0200c284
	cmp	r0, #0
	bne.n	.L_020032f2
	ldr	r3, [pc, #144]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	beq.n	.L_020032d2
.L_020032f2:
	ldr	r3, [pc, #136]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_0200334e
	ldr	r3, [sp, #24]
	cmp	fp, r3
	bne.n	.L_02003348
.L_02003302:
	movs	r0, #128
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x0200c14c
	ldr	r2, [sp, #4]
	add	r6, sp, #28
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl 0x0200c224
	mov	fp, r0
	cmp	r0, #255
	bne.n	.L_0200323a
.L_02003322:
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
	bl 0x0200c204
	adds	r0, r7, #0
	bl 0x0200c20c
	movs	r0, #2
	bl 0x0200c114
	b.n	.L_02003096
.L_02003348:
	movs	r0, #10
	bl 0x0200c114
.L_0200334e:
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
	bl 0x0200c1cc
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200ccf0
	.4byte 0x0200ad05
	.4byte 0x80184b01
	.4byte 0x00004770
	.2byte 0xccf0
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
	bl 0x0200c434
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
.L_020033c2:
	bl 0x0200ad90
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
	b.n	.L_02003404
	.2byte 0x0000
	.4byte 0xffffc000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff0
.L_02003404:
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
	bl 0x0200c224
	str	r0, [sp, #12]
	movs	r0, #128
	ldr	r1, [sp, #16]
	lsls	r0, r0, #13
	adds	r2, r6, #0
	bl 0x0200c14c
	mov	r1, fp
	ldrb	r0, [r1, #0]
	ldr	r2, [r6, #8]
	ldr	r1, [r6, #0]
	bl 0x0200c224
	mov	sl, r0
	cmp	r0, #255
	beq.n	.L_02003498
	mov	r2, fp
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl 0x0200c21c
	ldr	r3, [r5, #12]
	subs	r0, r0, r3
	cmp	r0, r9
	bgt.n	.L_02003498
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
	bl 0x0200c1cc
	adds	r0, r5, #0
	movs	r1, #48
	bl 0x0200c1d4
	ldr	r3, [pc, #8]
	str	r3, [r5, #108]
	b.n	.L_02003542
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0xad05
	.2byte 0x0200
.L_02003498:
	add	r1, sp, #16
	ldrh	r1, [r1, #0]
	movs	r3, #0
	mov	r2, r8
	strh	r1, [r5, #6]
	str	r3, [r5, #36]
	str	r3, [r5, #44]
	str	r2, [r5, #8]
	str	r7, [r5, #16]
	b.n	.L_0200358e
.L_020034ac:
	mov	r3, fp
	ldrb	r0, [r3, #0]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
	bl 0x0200c21c
	ldr	r3, [r5, #12]
	movs	r1, #128
	subs	r0, r0, r3
	lsls	r1, r1, #12
	cmp	r0, r1
	bgt.n	.L_02003562
	ldrh	r3, [r5, #32]
	movs	r2, #0
	subs	r3, #2
.L_020034ca:
	str	r3, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #20]
	movs	r3, #89
	adds	r3, r3, r6
	mov	r9, r2
	mov	r8, r3
.L_020034da:
	ldr	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_02003504
	mov	r1, r8
	ldrb	r2, [r1, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003504
	cmp	r6, r5
	beq.n	.L_02003504
	ldrh	r3, [r6, #32]
	adds	r0, r6, #0
	adds	r0, #8
	subs	r3, #2
	ldr	r1, [sp, #0]
	add	r2, sp, #20
	bl 0x0200c28c
	cmp	r0, #0
	bge.n	.L_02003562
.L_02003504:
	movs	r2, #1
	add	r9, r2
	movs	r3, #128
	mov	r1, r9
	add	r8, r3
	adds	r6, #128
	cmp	r1, #63
	ble.n	.L_020034da
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
	bl 0x0200c204
	adds	r0, r5, #0
	bl 0x0200c20c
	ldr	r1, [sp, #12]
	cmp	sl, r1
	bne.n	.L_02003588
.L_02003542:
	movs	r0, #128
	ldr	r1, [sp, #16]
	add	r2, sp, #20
	lsls	r0, r0, #13
	bl 0x0200c14c
	mov	r2, fp
	add	r7, sp, #20
	ldrb	r0, [r2, #0]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
.L_02003558:
	bl 0x0200c224
	mov	sl, r0
	cmp	r0, #255
	bne.n	.L_020034ac
.L_02003562:
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
	bl 0x0200c204
	adds	r0, r5, #0
	bl 0x0200c20c
	movs	r0, #2
	bl 0x0200c114
	b.n	.L_020033c2
.L_02003588:
	movs	r0, #10
	bl 0x0200c114
.L_0200358e:
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
	bl 0x0200c1cc
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_02003600
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003600
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
.L_02003600:
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
	bl 0x0200c2f4
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_02003684
	cmp	r7, #0
	beq.n	.L_02003684
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_0200368c
.L_02003684:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_0200368c:
	mov	r3, sl
	bl 0x0200c1e4
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_0200369a
	b.n	.L_020037e6
.L_0200369a:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl 0x0200c1cc
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x0200c1dc
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200c254
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
	bl 0x0200b5bc
	movs	r2, #100
.L_020036f0:
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
	beq.n	.L_020037e6
	cmp	r7, #0
	beq.n	.L_020037e6
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200371c
.L_02003714:
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x0200c374
.L_0200371c:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200373c
	adds	r1, r6, #0
.L_0200372a:
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl 0x0200b5bc
.L_0200373c:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_02003750
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_02003750:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02003796
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_0200377e
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200c104
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_02003790
.L_0200377e:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x0200c104
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_02003790:
	bl 0x0200c104
	str	r0, [r6, #52]
.L_02003796:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020037b2
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200c1cc
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200c1dc
.L_020037b2:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020037c4
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_020037c4:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020037d6
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_020037d6:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020037e6
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_020037e6:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200cce0
	.4byte 0x0200b605
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r4, [pc, #268]
	movs	r1, #1
	movs	r0, #12
	ldrsh	r3, [r4, r0]
	negs	r1, r1
	sub	sp, #4
	cmp	r3, r1
	beq.n	.L_02003910
	lsls	r3, r3, #3
	adds	r3, r3, r4
	adds	r3, #32
	mov	r8, r3
	ldr	r3, [pc, #248]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	str	r4, [sp, #0]
	bl 0x0200c2f4
	mov	r1, r8
	ldr	r3, [r0, #8]
	movs	r5, #0
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	ldr	r4, [sp, #0]
	cmp	r3, r2
	bne.n	.L_02003850
	ldr	r3, [r0, #16]
	movs	r5, #2
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	cmp	r3, r2
	beq.n	.L_02003858
.L_02003850:
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r4, #12]
.L_02003858:
	movs	r0, #12
	ldrsh	r3, [r4, r0]
	movs	r2, #1
	negs	r2, r2
	ldr	r1, [pc, #192]
	cmp	r3, r2
	beq.n	.L_02003910
	movs	r5, #14
	ldrsh	r3, [r4, r5]
	cmp	r3, #0
	beq.n	.L_02003910
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	str	r4, [sp, #0]
	adds	r3, r2, #0
	adds	r3, #228
	ldr	r0, [r3, #0]
	ldr	r5, [r3, #4]
	ldr	r3, [r2, #0]
	ands	r0, r1
	ands	r5, r1
	ldr	r6, [r3, #4]
	movs	r1, #16
	ldrsh	r3, [r4, r1]
.L_0200388a:
	ldr	r2, [pc, #156]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	lsrs	r3, r3, #5
	mov	sl, r3
	mov	r3, r8
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	lsls	r1, r1, #20
	subs	r7, r1, r0
	movs	r0, #2
	ldrsh	r2, [r3, r0]
	movs	r0, #0
	lsls	r2, r2, #20
	bl 0x0200c21c
	mov	r2, r8
	movs	r1, #2
	ldrsh	r3, [r2, r1]
	subs	r0, r0, r6
	lsls	r3, r3, #20
	subs	r3, r3, r5
	subs	r3, r3, r6
	subs	r2, r3, r0
	asrs	r7, r7, #16
	adds	r0, r0, r3
	asrs	r0, r0, #16
	adds	r3, r7, #0
	movs	r5, #167
	asrs	r2, r2, #16
	adds	r1, r0, #0
	adds	r3, #15
	lsls	r5, r5, #1
	adds	r2, #14
	adds	r1, #58
	ldr	r4, [sp, #0]
	cmp	r3, r5
	bhi.n	.L_02003910
	movs	r0, #15
	negs	r0, r0
	cmp	r2, r0
	blt.n	.L_02003910
	cmp	r2, #239
	bgt.n	.L_02003910
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r7, r3
	movs	r3, #255
	ands	r2, r3
	movs	r3, #0
	str	r3, [r4, #20]
	lsls	r3, r7, #16
	orrs	r2, r3
	ldr	r3, [pc, #48]
	adds	r0, r4, #0
	orrs	r2, r3
	movs	r3, #128
	str	r2, [r4, #24]
	lsls	r3, r3, #3
	mov	r2, sl
	orrs	r2, r3
	str	r2, [r4, #28]
	adds	r0, #20
	bl 0x0200c18c
.L_02003910:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200ccf4
	.4byte 0x02000240
	.4byte 0xffff0000
	.4byte 0x020036e0
	.2byte 0x8800
	.2byte 0x8000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #48
	str	r0, [sp, #44]
	ldr	r0, [pc, #540]
	str	r1, [sp, #40]
	mov	r8, r0
	movs	r1, #32
	add	r1, r8
	mov	r9, r1
	mov	ip, r9
	adds	r5, r2, #0
	mov	r2, ip
	adds	r6, r3, #0
	str	r2, [sp, #8]
	ldr	r3, [pc, #520]
	movs	r1, #4
	ldr	r7, [sp, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xa80b
	ldrh	r0, [r0, #0]
	mov	r1, r8
	strh	r0, [r1, #4]
	add	r1, sp, #40
	ldrh	r1, [r1, #0]
	mov	r3, r8
	strh	r1, [r3, #0]
	strh	r5, [r3, #2]
	movs	r3, #255
	lsls	r3, r3, #8
	mov	r5, r8
	mov	r0, r8
	adds	r3, #255
	mov	r1, r8
	strh	r6, [r5, #6]
	movs	r2, #0
	strh	r7, [r0, #8]
	strh	r3, [r1, #12]
	mov	r3, r8
	strh	r2, [r3, #10]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #132
	mov	ip, r3
	lsls	r2, r2, #1
	mov	r1, ip
	add	r2, ip
	adds	r1, #236
	ldr	r0, [r1, #0]
	ldr	r3, [r2, #8]
	ldr	r5, [r2, #48]
	adds	r3, r3, r0
	asrs	r3, r3, #20
	str	r3, [sp, #32]
	adds	r1, #4
	ldr	r3, [r2, #12]
	ldr	r2, [r1, #0]
	adds	r3, r3, r2
	asrs	r3, r3, #20
	str	r3, [sp, #28]
	mov	r3, ip
	adds	r3, #244
	ldr	r3, [r3, #0]
	subs	r3, r3, r0
	asrs	r3, r3, #20
	str	r3, [sp, #24]
	mov	r3, ip
	adds	r3, #248
	ldr	r3, [r3, #0]
	asrs	r0, r0, #20
	subs	r3, r3, r2
	asrs	r2, r2, #20
	lsls	r2, r2, #7
	adds	r2, r2, r0
	lsls	r2, r2, #2
	asrs	r3, r3, #20
	adds	r5, r5, r2
	movs	r0, #0
	str	r3, [sp, #20]
	str	r5, [sp, #36]
	str	r0, [sp, #12]
	cmp	r0, r3
	bge.n	.L_02003ab4
.L_020039e4:
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #36]
	ldr	r5, [sp, #24]
	lsls	r3, r1, #9
	adds	r2, r2, r3
	movs	r3, #0
	mov	fp, r2
	str	r3, [sp, #16]
	cmp	r3, r5
	bge.n	.L_02003aa8
.L_020039f8:
	mov	r0, fp
	ldrb	r5, [r0, #2]
	cmp	r5, #0
	beq.n	.L_02003a98
	ldr	r1, [sp, #44]
	cmp	r5, r1
	bcc.n	.L_02003a98
	adds	r1, #1
	mov	sl, r1
	cmp	r5, sl
	bhi.n	.L_02003a98
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #32]
	mov	r0, r9
	adds	r7, r2, r3
	strh	r7, [r0, #0]
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #28]
	add	r0, sp, #40
	ldrh	r0, [r0, #0]
	adds	r6, r1, r2
	mov	r3, r9
	mov	r1, r9
	strh	r6, [r3, #2]
	strh	r0, [r1, #4]
	ldr	r1, [sp, #40]
	movs	r0, #10
	adds	r1, #1
	adds	r0, #255
	str	r1, [sp, #40]
	bl 0x0200c19c
	cmp	r0, #0
	bne.n	.L_02003a4c
	cmp	r5, sl
	bne.n	.L_02003a8a
	mov	r3, r9
	movs	r2, #4
	ldrsh	r0, [r3, r2]
	bl 0x0200c1a4
	b.n	.L_02003a8a
.L_02003a4c:
	mov	r1, r9
	movs	r5, #4
	ldrsh	r0, [r1, r5]
	bl 0x0200c19c
	cmp	r0, #0
	beq.n	.L_02003a8a
	mov	r2, r8
	ldrh	r4, [r2, #6]
	ldrh	r5, [r2, #8]
	movs	r3, #8
	ldrsh	r1, [r2, r3]
	movs	r3, #6
	ldrsh	r0, [r2, r3]
	movs	r2, #64
	adds	r3, r2, #0
	ands	r3, r4
	ands	r2, r5
	lsls	r3, r3, #16
	lsls	r2, r2, #16
	asrs	r3, r3, #16
	asrs	r2, r2, #16
	orrs	r7, r3
	orrs	r6, r2
	adds	r1, #1
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200c234
.L_02003a8a:
	mov	r0, r8
	ldrh	r3, [r0, #10]
	mov	r1, r8
	adds	r3, #1
	strh	r3, [r1, #10]
	movs	r5, #8
	add	r9, r5
.L_02003a98:
	ldr	r2, [sp, #16]
	ldr	r5, [sp, #24]
	adds	r2, #1
	movs	r3, #4
	str	r2, [sp, #16]
	add	fp, r3
	cmp	r2, r5
	blt.n	.L_020039f8
.L_02003aa8:
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #20]
	adds	r0, #1
	str	r0, [sp, #12]
	cmp	r0, r1
	blt.n	.L_020039e4
.L_02003ab4:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200c19c
	cmp	r0, #0
	beq.n	.L_02003b0c
	ldr	r3, [pc, #164]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c2f4
	ldr	r3, [r0, #8]
	movs	r2, #0
	asrs	r4, r3, #20
	ldr	r3, [r0, #16]
	mov	r0, r8
	asrs	r1, r3, #20
	ldr	r3, [sp, #8]
	mov	r9, r3
	movs	r5, #10
	ldrsh	r3, [r0, r5]
	cmp	r2, r3
	bge.n	.L_02003b0c
.L_02003ae6:
	mov	r0, r9
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	cmp	r3, r4
	bne.n	.L_02003afc
	movs	r5, #2
	ldrsh	r3, [r0, r5]
	cmp	r3, r1
	bne.n	.L_02003afc
	mov	r0, r8
	strh	r2, [r0, #12]
.L_02003afc:
	movs	r3, #8
	mov	r0, r8
	add	r9, r3
	movs	r5, #10
	ldrsh	r3, [r0, r5]
	adds	r2, #1
	cmp	r2, r3
	blt.n	.L_02003ae6
.L_02003b0c:
	movs	r0, #128
	lsls	r0, r0, #1
	bl 0x0200c164
	adds	r5, r0, #0
	adds	r1, r5, #0
	movs	r2, #63
.L_02003b1a:
	ldr	r3, [pc, #80]
	subs	r2, #1
	stmia	r1!, {r3}
	cmp	r2, #0
	bge.n	.L_02003b1a
	bl 0x0200c184
	mov	r1, r8
	strh	r0, [r1, #16]
	lsls	r0, r0, #16
	movs	r1, #128
	adds	r2, r5, #0
	lsls	r1, r1, #1
	asrs	r0, r0, #16
	bl 0x0200c17c
	adds	r0, r5, #0
	bl 0x0200c16c
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl 0x0200c11c
	mov	r3, r8
	movs	r2, #10
	ldrsh	r0, [r3, r2]
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200ccf4
	.4byte 0x03000258
	.4byte 0x02000240
	.4byte 0x11111111
	.2byte 0xb805
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r1, [pc, #116]
	movs	r2, #133
	mov	r8, r1
	lsls	r2, r2, #2
	add	r8, r2
	mov	r3, r8
	ldr	r0, [r3, #0]
	bl 0x0200c2f4
	mov	r1, r8
	ldr	r5, [r0, #8]
	ldr	r6, [r0, #16]
	mov	sl, r0
	movs	r2, #128
	ldr	r0, [r1, #0]
	movs	r1, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #10
	bl 0x0200c2fc
	asrs	r5, r5, #20
	mov	r2, r8
	asrs	r6, r6, #20
	ldr	r0, [r2, #0]
	lsls	r1, r5, #4
	lsls	r2, r6, #4
	adds	r1, #8
	adds	r2, #8
	bl 0x0200c314
	movs	r0, #1
	bl 0x0200c114
	movs	r3, #128
	lsls	r3, r3, #12
	lsls	r5, r5, #20
	lsls	r6, r6, #20
	adds	r5, r5, r3
	mov	r1, sl
	adds	r6, r6, r3
	ldr	r2, [r1, #12]
	adds	r3, r6, #0
	adds	r1, r5, #0
	mov	r0, sl
	bl 0x0200c1f4
	movs	r0, #4
	bl 0x0200c2d4
	bl 0x0200c3dc
	ldr	r2, [pc, #20]
	ldr	r3, [r0, #12]
	adds	r3, r3, r2
	str	r3, [r0, #12]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb560
	mov	r6, r8
	push	{r6}
	ldr	r3, [pc, #100]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	mov	r8, r0
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl 0x0200c2f4
	mov	r2, r8
	ldrh	r1, [r2, #6]
	movs	r2, #64
	ldr	r6, [r0, #8]
	adds	r3, r2, #0
	ands	r3, r1
	lsls	r3, r3, #16
	mov	r1, r8
	asrs	r3, r3, #16
	asrs	r6, r6, #20
	orrs	r6, r3
	ldrh	r3, [r1, #8]
	ldr	r5, [r0, #16]
	ands	r2, r3
	lsls	r2, r2, #16
	asrs	r2, r2, #16
	asrs	r5, r5, #20
	orrs	r5, r2
	bl 0x0200bb74
	movs	r0, #161
	bl 0x0200c46c
	mov	r3, r8
	movs	r2, #8
	ldrsh	r1, [r3, r2]
	movs	r2, #6
	ldrsh	r0, [r3, r2]
	adds	r1, #1
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200c234
	movs	r0, #12
	bl 0x0200c2d4
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r1, [pc, #164]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r1, r1, r2
	mov	r8, r0
	ldr	r0, [r1, #0]
	sub	sp, #8
	mov	sl, r1
	bl 0x0200c2f4
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	movs	r2, #64
	asrs	r7, r3, #20
	mov	r3, r8
	ldrh	r1, [r3, #6]
	adds	r3, r2, #0
	ands	r3, r1
	lsls	r3, r3, #16
	mov	r1, r8
	asrs	r3, r3, #16
	orrs	r7, r3
	ldrh	r3, [r1, #8]
	ldr	r5, [r6, #16]
	ands	r2, r3
	lsls	r2, r2, #16
	asrs	r2, r2, #16
	asrs	r5, r5, #20
	orrs	r5, r2
	bl 0x0200bb74
	movs	r0, #229
	bl 0x0200c46c
	mov	r3, r8
	movs	r2, #8
	ldrsh	r1, [r3, r2]
	movs	r2, #6
	ldrsh	r0, [r3, r2]
	adds	r1, #2
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200c234
	movs	r0, #12
	bl 0x0200c2d4
	movs	r3, #128
	ldr	r2, [pc, #56]
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	mov	r3, sl
	ldr	r0, [r3, #0]
	bl 0x0200c2f4
	movs	r1, #0
	bl 0x0200c254
	movs	r2, #226
	movs	r3, #128
	lsls	r2, r2, #4
	lsls	r3, r3, #19
	adds	r2, #255
	adds	r3, #74
	strh	r2, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #16]
	movs	r7, #0
	orrs	r3, r2
	strh	r3, [r1, #0]
	mov	r2, sl
	b.n	.L_02003d20
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x00008000
	.2byte 0x0240
	.2byte 0x0200
.L_02003d20:
	movs	r3, #1
	mov	r1, r8
	strh	r3, [r1, #14]
	ldr	r0, [r2, #0]
	movs	r1, #28
	bl 0x0200c33c
	movs	r0, #16
	bl 0x0200c2d4
.L_02003d34:
	cmp	r7, #5
	bne.n	.L_02003d3e
	movs	r0, #204
	bl 0x0200c46c
.L_02003d3e:
	ldr	r3, [r6, #24]
	ldr	r1, [pc, #88]
	ldr	r2, [pc, #92]
	adds	r3, r3, r1
	str	r3, [r6, #24]
	ldr	r3, [r6, #28]
	ldr	r1, [pc, #88]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	ldr	r3, [r6, #12]
	movs	r0, #1
	adds	r3, r3, r1
	str	r3, [r6, #12]
	adds	r7, #1
	bl 0x0200c114
	cmp	r7, #39
	ble.n	.L_02003d34
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200c2f4
	movs	r3, #0
	adds	r0, #84
	strb	r3, [r0, #0]
	mov	r1, r8
	strh	r3, [r1, #14]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200c424
	bl 0x0200c42c
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0xfffffc00
	.4byte 0xfffffd00
	.4byte 0xffff6667
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
	bl 0x0200c104
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02003ddc
	adds	r3, #15
.L_02003ddc:
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
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #376]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #68
	bl 0x0200c2f4
	adds	r7, r0, #0
	bl 0x0200c2dc
	movs	r0, #0
	bl 0x0200c44c
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200c3cc
	bl 0x0200c1fc
	movs	r0, #1
	bl 0x0200c114
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
	bl 0x0200c41c
	bl 0x0200c42c
	movs	r0, #204
	bl 0x0200c46c
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #24
	bl 0x0200c2d4
	add	r2, sp, #28
	movs	r3, #7
	str	r3, [r2, #4]
	ldr	r3, [pc, #256]
	mov	r8, r2
	str	r3, [r2, #36]
	movs	r3, #163
	lsls	r3, r3, #8
	adds	r3, #215
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	movs	r3, #0
	mov	sl, r3
.L_02003e9e:
	mov	r4, sl
	lsls	r5, r4, #12
	adds	r0, r5, #0
	bl 0x0200c144
	add	r6, sp, #16
	movs	r3, #0
	str	r0, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r6, #4]
	bl 0x0200c13c
	ldr	r3, [r6, #0]
	str	r0, [r6, #8]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	bl 0x0200c12c
	lsls	r3, r0, #1
	ldr	r2, [r6, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #188]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	bl 0x0200c12c
	lsls	r3, r0, #1
	ldr	r5, [r6, #8]
	adds	r3, r3, r0
	ldr	r4, [pc, #176]
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
	ldr	r4, [pc, #156]
	str	r5, [sp, #4]
	str	r4, [sp, #8]
	mov	r4, r8
	str	r4, [sp, #12]
	bl 0x0200b63c
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_02003e9e
	movs	r0, #188
	bl 0x0200c46c
	ldr	r5, [pc, #112]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r5, r5, r4
	movs	r1, #2
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl 0x0200c3bc
	ldr	r0, [r5, #0]
	movs	r1, #49
	bl 0x0200c33c
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200c264
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200c264
	bl 0x0200c26c
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200c3bc
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #68]
	movs	r0, #10
	bl 0x0200c2d4
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200c33c
	bl 0x0200c2e4
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200bdad
	.4byte 0xffffa000
	.4byte 0xffffd000
	.2byte 0x0001
	.2byte 0x0109
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #156]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl 0x0200c2f4
	ldr	r3, [r0, #8]
	ldr	r6, [pc, #144]
	asrs	r3, r3, #20
	mov	r8, r3
	ldr	r3, [r0, #16]
	adds	r5, r6, #0
	asrs	r3, r3, #20
	mov	sl, r3
	movs	r1, #10
	ldrsh	r3, [r6, r1]
	movs	r7, #0
	adds	r5, #32
	ldrh	r2, [r6, #10]
	cmp	r7, r3
	bge.n	.L_02004038
.L_02003fd0:
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, r8
	bne.n	.L_0200402c
	movs	r1, #2
	ldrsh	r3, [r5, r1]
	cmp	r3, sl
	bne.n	.L_0200402c
	movs	r2, #4
	ldrsh	r0, [r5, r2]
	bl 0x0200c19c
	cmp	r0, #0
	bne.n	.L_02004000
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl 0x0200bbfc
	movs	r3, #4
	ldrsh	r0, [r5, r3]
	bl 0x0200c1a4
	strh	r7, [r6, #12]
	b.n	.L_02004038
.L_02004000:
	movs	r1, #12
	ldrsh	r3, [r6, r1]
	cmp	r7, r3
	beq.n	.L_02004038
	adds	r0, r6, #0
	adds	r1, r5, #0
	strh	r7, [r6, #12]
	bl 0x0200bc6c
	movs	r2, #2
	ldrsh	r0, [r6, r2]
	mov	r1, r8
	bl 0x0200c1bc
	movs	r3, #2
	ldrsh	r0, [r6, r3]
	mov	r1, sl
	adds	r0, #8
	bl 0x0200c1bc
	movs	r0, #1
	b.n	.L_0200403a
.L_0200402c:
	lsls	r3, r2, #16
	adds	r7, #1
	asrs	r3, r3, #16
	adds	r5, #8
	cmp	r7, r3
	blt.n	.L_02003fd0
.L_02004038:
	movs	r0, #0
.L_0200403a:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xccf4
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	ldr	r3, [pc, #156]
	str	r2, [sp, #0]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r9, r0
	ldr	r0, [r3, #0]
	mov	fp, r1
	bl 0x0200c2f4
	movs	r3, #192
	ldr	r5, [pc, #140]
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r6, r0, #0
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	mov	sl, r3
	bl 0x0200c1b4
	adds	r7, r0, #0
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	adds	r0, #8
	bl 0x0200c1b4
	mov	r8, r0
	cmp	r7, #0
	bne.n	.L_0200409a
	cmp	r0, #0
	beq.n	.L_020040ee
.L_0200409a:
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	movs	r1, #0
	bl 0x0200c1bc
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	movs	r1, #0
	adds	r0, #8
	bl 0x0200c1bc
	mov	r3, r9
	adds	r2, r7, r3
	mov	r3, r8
	movs	r1, #128
	add	r3, fp
	lsls	r1, r1, #12
	lsls	r3, r3, #20
	adds	r3, r3, r1
	str	r3, [r6, #16]
	movs	r3, #230
	lsls	r3, r3, #1
	add	r3, sl
	lsls	r2, r2, #20
	adds	r2, r2, r1
	ldr	r1, [r3, #0]
	str	r2, [r6, #8]
	str	r2, [r1, #8]
	ldr	r3, [r6, #16]
	str	r3, [r1, #16]
	bl 0x0200c1fc
	bl 0x0200be04
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #0]
	str	r2, [r3, #0]
.L_020040ee:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200ccf4
	.irp EntryTarget, 0x03000528, 0x03000508, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000f9, 0x08000101, 0x08000119, 0x08000121, 0x08000129, 0x08000141, 0x08000151, 0x08000169, 0x08000179, 0x080001b9, 0x080001c9, 0x080001d1, 0x080001e9, 0x080003c1, 0x080003c9, 0x080003d1, 0x080003d9, 0x080003e9, 0x080003f1, 0x08020071, 0x08020091, 0x08020099, 0x080200a9, 0x080200c1, 0x080200c9, 0x080200e9, 0x08020121, 0x08020149, 0x08020151, 0x08020179, 0x080201c1, 0x080201c9, 0x080201d9, 0x080201e1, 0x080201e9, 0x080201f1, 0x08020211, 0x08020219, 0x08020221, 0x08020229, 0x08020231, 0x08020279, 0x08020291, 0x080202f9, 0x08020349, 0x08020391, 0x08038041, 0x08038211, 0x08038249, 0x080ad021, 0x080ad041, 0x080ad049, 0x080ad0f1, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80b9, 0x080c80c9, 0x080c80d9, 0x080c80e9, 0x080c80f9, 0x080c8111, 0x080c8119, 0x080c8129, 0x080c8131, 0x080c8139, 0x080c8141, 0x080c8149, 0x080c8151, 0x080c8171, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81a1, 0x080c81d1, 0x080c81d9, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8259, 0x080c8281, 0x080c8291, 0x080c82f9, 0x080c8351, 0x080c8379, 0x080c8381, 0x080c8391, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c8459, 0x080c8481, 0x080c84d9, 0x080c84e1, 0x080c8549, 0x080c8571, 0x080c8779, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.4byte 0x0000002e
	.4byte 0x0200ac45
	.4byte 0x00000004
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02380000
	.4byte 0x00200000
	.4byte 0x00f00000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02380000
	.4byte 0x00200000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02400000
	.4byte 0x00200000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02400000
	.4byte 0x00160000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02500000
	.4byte 0x00160000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000025
	.4byte 0x02008189
	.4byte 0x0000002e
	.4byte 0x0200ace9
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x0200ac45
	.4byte 0x00000004
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03280000
	.4byte 0x00180000
	.4byte 0x00f00000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03180000
	.4byte 0x00180000
	.4byte 0x00f00000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03180000
	.4byte 0x00200000
	.4byte 0x00f00000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03180000
	.4byte 0x00200000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03100000
	.4byte 0x00200000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03100000
	.4byte 0x00160000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03000000
	.4byte 0x00160000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000025
	.4byte 0x020081e9
	.4byte 0x0000002e
	.4byte 0x0200ace9
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x0000001e
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
	.4byte 0x00000014
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xe000c000
	.4byte 0xc000a000
	.4byte 0x20004000
	.4byte 0x40006000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xffffc000
	.4byte 0xc000ffff
	.4byte 0xffff4000
	.4byte 0x4000ffff
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000c000
	.4byte 0xc0008000
	.4byte 0x00004000
	.4byte 0x40008000
	.4byte 0x0000ffff
	.4byte 0xffff8000
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
	.4byte 0x0000002e
	.4byte 0x02008039
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00000400
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00000400
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xffffc000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000028
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffff800
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000018
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000028
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00000400
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000028
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0xffff0000
	.4byte 0x000000c0
	.4byte 0x800000c0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000011e
	.4byte 0x0010211e
	.4byte 0x0020111e
	.4byte 0x0030411e
	.4byte 0x0040311e
	.4byte 0x0050511d
	.4byte 0x0060611c
	.4byte 0x0070711d
	.4byte 0x0080811c
	.4byte 0x000001ff
	.4byte 0xffff01a2
	.4byte 0x0200c910
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte 0x0200c910
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff0123
	.4byte 0x00000007
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00024000
	.4byte 0x0a3000b5
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0x0a3000b5
	.4byte 0x00000007
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01024000
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0017
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff012f
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff012f
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff02a1
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff02a1
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019c
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019c
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
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000051
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000051
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000021
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000021
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000021
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000021
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000002
	.4byte 0x1a4b005a
	.4byte 0x0200a36d
	.4byte 0x10009a15
	.4byte 0xffff0008
	.4byte 0x02008061
	.4byte 0x60009a15
	.4byte 0xffff0009
	.4byte 0x02008075
	.4byte 0x20009a15
	.4byte 0xffff0009
	.4byte 0x0200807d
	.4byte 0x00000006
	.4byte 0xffff0014
	.4byte 0x020085b1
	.4byte 0x50009705
	.4byte 0x12330014
	.4byte 0x0200824d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008ba5
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002d82
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008bd9
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002d81
	.4byte 0x00000003
	.4byte 0xffff0015
	.4byte 0x02008e51
	.4byte 0x0000de04
	.4byte 0xffff0415
	.4byte 0x02008e81
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0200c85c
	.4byte 0x0200c898
	.4byte 0x0200c8d4
