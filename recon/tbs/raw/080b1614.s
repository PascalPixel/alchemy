.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_08003fa4, 0x08003fa4
	.set sub_08004080, 0x08004080
	.set sub_080048f4, 0x080048f4
	.set sub_08015010, 0x08015010
	.set sub_08015018, 0x08015018
	.set sub_08015080, 0x08015080
	.set sub_080150b0, 0x080150b0
	.set sub_080150c8, 0x080150c8
	.set sub_080b06c0, 0x080b06c0
	.set sub_080f9010, 0x080f9010
	.global Shop_RunQuantitySelector
	.global Func_080b1614
	.thumb_func
Shop_RunQuantitySelector:
Func_080b1614:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r9, r1
	movs	r1, #128
	sub	sp, #16
	mov	fp, r0
	lsls	r1, r1, #3
	movs	r0, #14
	str	r2, [sp, #12]
	bl	sub_080048f4
	movs	r2, #1
	str	r2, [sp, #8]
	mov	r3, r9
	mov	r2, fp
	subs	r3, r3, r2
	mov	r9, r3
	movs	r3, #2
	str	r3, [sp, #0]
	mov	r8, r0
	movs	r1, #4
	movs	r0, #7
	movs	r2, #23
	movs	r3, #3
	bl	sub_08015010
	movs	r5, #1
	negs	r5, r5
	movs	r7, #0
	mov	sl, r0
	cmp	r0, #0
	bne.n	.L_080b1660
	b.n	.L_080b17b0
.L_080b1660:
	bl	sub_08004080
	str	r0, [sp, #4]
	cmp	r0, #96
	bne.n	.L_080b166c
	b.n	.L_080b17b0
.L_080b166c:
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	bl	sub_08003fa4
	ldr	r5, [pc, #56]
	ldr	r0, [sp, #4]
	adds	r1, r5, #0
	mov	r2, sl
	movs	r3, #0
	str	r7, [sp, #0]
	bl	sub_080150c8
	adds	r1, r5, #0
	mov	r2, sl
	movs	r3, #32
	ldr	r0, [sp, #4]
	str	r7, [sp, #0]
	bl	sub_080150c8
	ldrh	r1, [r0, #24]
	lsls	r2, r1, #22
	ldr	r3, [pc, #16]
	lsrs	r2, r2, #22
	adds	r2, #4
	ands	r2, r3
	ldr	r3, [pc, #16]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #24]
	b.n	.L_080b1776
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x40004000
	.2byte 0xfc00
	.2byte 0xffff
.L_080b16b8:
	ldr	r5, [pc, #272]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b16d0
	movs	r0, #111
	bl	sub_080f9010
	movs	r3, #1
	str	r3, [sp, #8]
	subs	r7, #1
.L_080b16d0:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b16e6
	movs	r0, #111
	bl	sub_080f9010
	movs	r2, #1
	str	r2, [sp, #8]
	adds	r7, #1
.L_080b16e6:
	ldr	r3, [sp, #8]
	cmp	r3, #0
	beq.n	.L_080b1770
	mov	r3, r9
	movs	r2, #0
	adds	r0, r7, r3
	mov	r1, r9
	str	r2, [sp, #8]
	bl	sub_080022fc
	ldr	r3, [pc, #212]
	adds	r7, r0, #0
	mov	r1, r8
	ldr	r0, [pc, #208]
	ldr	r2, [pc, #212]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r2, r8
	movs	r0, #30
	movs	r1, #14
	bl	sub_080b06c0
	mov	r0, fp
	add	r0, r9
	movs	r1, #0
	mov	r2, r8
	bl	sub_080b06c0
	mov	r2, fp
	adds	r0, r2, r7
	adds	r0, #1
	movs	r1, #10
	mov	r2, r8
	bl	sub_080b06c0
	mov	r0, fp
	movs	r1, #2
	mov	r2, r8
	bl	sub_080b06c0
	movs	r1, #128
	ldr	r0, [sp, #4]
	lsls	r1, r1, #1
	mov	r2, r8
	bl	sub_08003fa4
	adds	r5, r7, #1
	adds	r0, r5, #0
	movs	r1, #2
	mov	r2, sl
	movs	r3, #72
	str	r6, [sp, #0]
	bl	sub_080150b0
	ldr	r3, [sp, #12]
	movs	r1, #6
	adds	r0, r5, #0
	muls	r0, r3
	mov	r2, sl
	movs	r3, #88
	str	r6, [sp, #0]
	bl	sub_080150b0
	ldr	r0, [pc, #116]
	mov	r1, sl
	movs	r2, #136
	movs	r3, #0
	bl	sub_08015080
.L_080b1770:
	movs	r0, #1
	bl	sub_080030f8
.L_080b1776:
	ldr	r2, [pc, #104]
	ldr	r3, [r2, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b178c
	movs	r0, #112
	bl	sub_080f9010
	adds	r5, r7, #1
	b.n	.L_080b17a2
.L_080b178c:
	ldr	r3, [pc, #80]
	ldr	r6, [r3, #0]
	movs	r3, #2
	ands	r6, r3
	cmp	r6, #0
	beq.n	.L_080b16b8
	movs	r0, #113
	bl	sub_080f9010
	movs	r5, #1
	negs	r5, r5
.L_080b17a2:
	movs	r0, #1
	bl	sub_080030f8
	mov	r0, sl
	movs	r1, #2
	bl	sub_08015018
.L_080b17b0:
	movs	r0, #14
	bl	sub_08002dd8
	adds	r0, r5, #0
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001b04
	.4byte 0x040000d4
	.4byte 0x080b3f80
	.4byte 0x84000040
	.4byte 0x00000c88
	.4byte 0x03001c94
