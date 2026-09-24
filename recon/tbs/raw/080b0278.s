.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08015010, 0x08015010
	.set sub_08015018, 0x08015018
	.set sub_080150c8, 0x080150c8
	.set sub_080150f8, 0x080150f8
	.set sub_08015380, 0x08015380
	.set sub_0808a080, 0x0808a080
	.set sub_080b0070, 0x080b0070
	.set sub_080b010c, 0x080b010c
	.set sub_080b0204, 0x080b0204
	.set sub_080b04dc, 0x080b04dc
	.set sub_080b0a20, 0x080b0a20
	.set sub_080b0aac, 0x080b0aac
	.set sub_080b1a14, 0x080b1a14
	.set sub_080b2110, 0x080b2110
	.set sub_080b26c8, 0x080b26c8
	.set sub_080b26cc, 0x080b26cc
	.set sub_080b2720, 0x080b2720
	.set sub_080b2764, 0x080b2764
	.global Shop_Run
	.global Func_080b0278
	.thumb_func
Shop_Run:
Func_080b0278:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r5, r1, #0
	movs	r1, #0
	mov	r8, r0
	sub	sp, #4
	mov	sl, r1
	bl	sub_080b26c8
	cmp	r8, r0
	bge.n	.L_080b029a
	mov	r2, r8
	cmp	r2, #0
	bge.n	.L_080b029e
.L_080b029a:
	movs	r3, #0
	mov	r8, r3
.L_080b029e:
	mov	r0, r8
	bl	sub_080b26cc
	bl	sub_080b010c
	ldr	r3, [pc, #204]
	mov	r0, r8
	ldr	r7, [r3, #0]
	bl	sub_080b2764
	ldr	r1, [pc, #200]
	mov	r2, r8
	adds	r3, r7, r1
	strb	r0, [r3, #0]
	cmp	r2, #16
	bne.n	.L_080b02c8
	movs	r3, #235
	lsls	r3, r3, #2
	adds	r2, r7, r3
	movs	r3, #1
	strb	r3, [r2, #0]
.L_080b02c8:
	mov	r1, r8
	cmp	r1, #17
	bne.n	.L_080b02d8
	movs	r3, #235
	lsls	r3, r3, #2
	adds	r2, r7, r3
	movs	r3, #1
	strb	r3, [r2, #0]
.L_080b02d8:
	mov	r1, r8
	cmp	r1, #18
	bne.n	.L_080b02e8
	movs	r3, #235
	lsls	r3, r3, #2
	adds	r2, r7, r3
	movs	r3, #1
	strb	r3, [r2, #0]
.L_080b02e8:
	adds	r0, r5, #0
	bl	sub_0808a080
	ldr	r3, [r0, #80]
	ldr	r3, [r3, #40]
	movs	r1, #233
	ldrh	r2, [r3, #0]
	lsls	r1, r1, #2
	adds	r3, r7, r1
	strh	r2, [r3, #0]
	movs	r1, #0
	ldrh	r0, [r3, #0]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080150f8
	mov	r9, r0
	cmp	r0, #0
	bne.n	.L_080b0322
	movs	r3, #2
	movs	r0, #5
	str	r3, [sp, #0]
	negs	r0, r0
	movs	r1, #0
	movs	r2, #5
	movs	r3, #5
	bl	sub_08015010
	mov	r9, r0
.L_080b0322:
	movs	r2, #228
	lsls	r2, r2, #2
	adds	r3, r7, r2
	movs	r1, #128
	ldrh	r0, [r3, #0]
	lsls	r1, r1, #23
	movs	r3, #0
	mov	r2, r9
	str	r3, [sp, #0]
	bl	sub_080150c8
	movs	r3, #1
	adds	r5, r0, #0
	strb	r3, [r5, #5]
	ldr	r3, [pc, #52]
	strb	r3, [r5, #4]
	movs	r3, #224
	lsls	r3, r3, #2
	adds	r6, r7, r3
	movs	r1, #32
	adds	r0, r6, #0
	negs	r1, r1
	movs	r2, #112
	bl	sub_080b0a20
	str	r5, [r6, #0]
	ldr	r0, [pc, #40]
	bl	sub_080b04dc
.L_080b035c:
	mov	r0, sl
	bl	sub_08015380
	ldr	r1, [pc, #32]
	mov	sl, r0
	adds	r3, r7, r1
	mov	r2, sl
	strb	r2, [r3, #0]
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_080b039e
	b.n	.L_080b0388
	.4byte 0x00000000
	.4byte 0x03001f2c
	.4byte 0x000003a9
	.4byte 0x00000c9b
	.2byte 0x03aa
	.2byte 0x0000
.L_080b0388:
	movs	r2, #155
	lsls	r2, r2, #2
	adds	r1, r7, r2
	mov	r0, r8
	bl	sub_080b2720
	ldr	r1, [pc, #140]
	adds	r3, r7, r1
	strb	r0, [r3, #0]
	ldr	r0, [pc, #140]
	b.n	.L_080b03c0
.L_080b039e:
	mov	r2, sl
	cmp	r2, #1
	bne.n	.L_080b03b0
	ldr	r0, [pc, #132]
	bl	sub_080b04dc
	bl	sub_080b1a14
	b.n	.L_080b03e8
.L_080b03b0:
	mov	r3, sl
	cmp	r3, #2
	bne.n	.L_080b03d8
	bl	sub_080b0070
	cmp	r0, #0
	beq.n	.L_080b03ca
	ldr	r0, [pc, #112]
.L_080b03c0:
	bl	sub_080b04dc
	bl	sub_080b0aac
	b.n	.L_080b03e8
.L_080b03ca:
	ldr	r0, [pc, #104]
	bl	sub_080b04dc
	movs	r0, #1
	bl	sub_080030f8
	b.n	.L_080b03e8
.L_080b03d8:
	mov	r1, sl
	cmp	r1, #3
	bne.n	.L_080b0400
	ldr	r0, [pc, #88]
	bl	sub_080b04dc
	bl	sub_080b2110
.L_080b03e8:
	movs	r2, #224
	lsls	r2, r2, #2
	movs	r1, #32
	adds	r0, r7, r2
	negs	r1, r1
	movs	r2, #112
	bl	sub_080b0a20
	ldr	r0, [pc, #64]
	bl	sub_080b04dc
	b.n	.L_080b035c
.L_080b0400:
	ldr	r0, [pc, #60]
	bl	sub_080b04dc
	mov	r0, r9
	movs	r1, #2
	bl	sub_08015018
	bl	sub_080b0204
	movs	r0, #0
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x000003a6
	.4byte 0x00000ca7
	.4byte 0x00000ca9
	.4byte 0x00000cb8
	.4byte 0x00000cb7
	.4byte 0x00000cb9
	.4byte 0x00000ca4
	.4byte 0x00000ca5
