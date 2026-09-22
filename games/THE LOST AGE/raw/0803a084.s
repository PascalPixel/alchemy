.syntax unified
	.thumb
	.set sub_0803a054, 0x0803a054
	.set sub_08041abc, 0x08041abc
	.global Overlay_0803a084
Overlay_0803a084:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r7, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	mov	r8, r2
	mov	fp, r3
	lsls	r3, r1, #6
	lsls	r2, r0, #1
	add	r3, fp
	adds	r6, r3, r2
	adds	r5, r6, #0
	mov	r2, r8
	adds	r5, #8
	cmp	r2, #1
	bhi.n	.L_0803a0b2
	b.n	.L_0803a1ae
.L_0803a0b2:
	cmp	r7, #1
	bhi.n	.L_0803a0b8
	b.n	.L_0803a1ae
.L_0803a0b8:
	cmp	r2, #30
	bls.n	.L_0803a0be
	b.n	.L_0803a1ae
.L_0803a0be:
	cmp	r7, #30
	bls.n	.L_0803a0c4
	b.n	.L_0803a1ae
.L_0803a0c4:
	adds	r3, r7, #0
	bl	sub_08041abc
	mov	r2, fp
	ldrb	r3, [r2, #4]
	cmp	r3, #0
	beq.n	.L_0803a0da
	ldr	r3, [pc, #48]
	strh	r3, [r5, #0]
	adds	r5, #2
	b.n	.L_0803a0e2
.L_0803a0da:
	ldr	r3, [pc, #44]
	strh	r3, [r5, #0]
	adds	r5, r6, #0
	adds	r5, #10
.L_0803a0e2:
	movs	r3, #2
	negs	r3, r3
	add	r3, r8
	mov	sl, r3
	adds	r0, r5, #0
	mov	r2, sl
	ldr	r1, [pc, #32]
	bl	sub_0803a054
	mov	r2, fp
	ldrb	r3, [r2, #4]
	adds	r5, r0, #0
	cmp	r3, #0
	beq.n	.L_0803a114
	ldr	r3, [pc, #12]
	b.n	.L_0803a116
	movs	r0, r0
	.4byte 0x0000f01c
	.4byte 0x0000f010
	.4byte 0x0000f41c
	.2byte 0xf011
	.2byte 0xf011
.L_0803a114:
	ldr	r3, [pc, #52]
.L_0803a116:
	strh	r3, [r5, #0]
	adds	r5, #2
	movs	r3, #32
	mov	r2, r8
	subs	r3, r3, r2
	lsls	r3, r3, #1
	movs	r6, #1
	subs	r7, #1
	adds	r5, r5, r3
	cmp	r6, r7
	bcs.n	.L_0803a164
	mov	r9, r3
.L_0803a12e:
	movs	r3, #240
	lsls	r3, r3, #8
	adds	r3, #22
	mov	r2, r8
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r2, #2
	beq.n	.L_0803a156
	adds	r0, r5, #0
	ldr	r1, [pc, #12]
	mov	r2, sl
	bl	sub_0803a054
	b.n	.L_0803a154
	movs	r0, r0
	.4byte 0x0000f012
	.2byte 0xf020
	.2byte 0xf020
.L_0803a154:
	adds	r5, r0, #0
.L_0803a156:
	ldr	r3, [pc, #24]
	adds	r6, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	add	r5, r9
	cmp	r6, r7
	bcc.n	.L_0803a12e
.L_0803a164:
	mov	r2, fp
	ldrb	r3, [r2, #4]
	cmp	r3, #0
	beq.n	.L_0803a178
	ldr	r3, [pc, #4]
	b.n	.L_0803a17a
	.4byte 0x0000f017
	.2byte 0xf81c
	.2byte 0x0000
.L_0803a178:
	ldr	r3, [pc, #28]
.L_0803a17a:
	strh	r3, [r5, #0]
	adds	r5, #2
	adds	r0, r5, #0
	mov	r2, sl
	ldr	r1, [pc, #28]
	bl	sub_0803a054
	mov	r2, fp
	ldrb	r3, [r2, #4]
	adds	r5, r0, #0
	cmp	r3, #0
	beq.n	.L_0803a1a4
	ldr	r3, [pc, #8]
	b.n	.L_0803a1a6
	movs	r0, r0
	.4byte 0x0000f013
	.4byte 0x0000fc1c
	.2byte 0xf014
	.2byte 0xf014
.L_0803a1a4:
	ldr	r3, [pc, #20]
.L_0803a1a6:
	strh	r3, [r5, #0]
	movs	r3, #1
	mov	r2, fp
	strb	r3, [r2, #3]
.L_0803a1ae:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0xf015
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r8, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	sub	sp, #4
	str	r3, [sp, #0]
	adds	r7, r2, #0
	ldr	r2, [sp, #0]
	lsls	r3, r1, #6
	adds	r3, r2, r3
	lsls	r2, r0, #1
	adds	r6, r3, r2
	adds	r5, r6, #0
	adds	r5, #8
	cmp	r7, #1
	bls.n	.L_0803a2a0
	mov	r3, r8
	cmp	r3, #1
	bls.n	.L_0803a2a0
	cmp	r7, #30
	bhi.n	.L_0803a2a0
	cmp	r3, #30
	bhi.n	.L_0803a2a0
	adds	r2, r7, #0
	bl	sub_08041abc
	ldr	r3, [pc, #48]
	subs	r2, r7, #2
	strh	r3, [r5, #0]
	adds	r5, #2
	adds	r0, r5, #0
	ldr	r1, [pc, #44]
	mov	sl, r2
	bl	sub_0803a054
	ldr	r3, [pc, #32]
	adds	r5, r0, #0
	strh	r3, [r5, #0]
	movs	r2, #1
	movs	r3, #32
	subs	r3, r3, r7
	negs	r2, r2
	adds	r5, #2
	lsls	r3, r3, #1
	movs	r6, #1
	add	r8, r2
	adds	r5, r5, r3
	cmp	r6, r8
	bcs.n	.L_0803a26e
	b.n	.L_0803a240
	movs	r0, r0
	.4byte 0x0000f008
	.4byte 0x0000f00a
	.2byte 0xf009
	.2byte 0xf009
.L_0803a240:
	mov	r9, r3
	movs	r3, #240
	lsls	r3, r3, #8
	adds	r3, #14
	mov	fp, r3
.L_0803a24a:
	mov	r2, fp
	strh	r2, [r5, #0]
	adds	r5, #2
	cmp	r7, #2
	beq.n	.L_0803a260
	adds	r0, r5, #0
	ldr	r1, [pc, #64]
	mov	r2, sl
	bl	sub_0803a054
	adds	r5, r0, #0
.L_0803a260:
	ldr	r3, [pc, #40]
	adds	r6, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	add	r5, r9
	cmp	r6, r8
	bcc.n	.L_0803a24a
.L_0803a26e:
	ldr	r3, [pc, #32]
	mov	r2, sl
	strh	r3, [r5, #0]
	adds	r5, #2
	adds	r0, r5, #0
	ldr	r1, [pc, #32]
	bl	sub_0803a054
	ldr	r3, [pc, #20]
	adds	r5, r0, #0
	strh	r3, [r5, #0]
	ldr	r2, [sp, #0]
	movs	r3, #1
	strb	r3, [r2, #3]
	b.n	.L_0803a2a0
	.4byte 0x0000f00f
	.4byte 0x0000f00b
	.4byte 0x0000f00d
	.4byte 0xf020f020
	.2byte 0xf00c
	.2byte 0xf00c
.L_0803a2a0:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
