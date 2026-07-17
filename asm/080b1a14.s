@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b1a14
	.thumb_func
Func_080b1a14:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #408]
	movs	r0, #1
	movs	r2, #0
	sub	sp, #4
	ldr	r6, [r3, #0]
	movs	r5, #2
	movs	r1, #9
	movs	r3, #4
	mov	r9, r0
	mov	sl, r2
	movs	r0, #0
	movs	r2, #12
	str	r5, [sp, #0]
	bl	Func_08015010
	str	r0, [r6, #12]
	bl	Func_080b10cc
	movs	r1, #12
	movs	r2, #14
	movs	r3, #8
	movs	r0, #16
	str	r5, [sp, #0]
	bl	Func_08015010
	movs	r1, #14
	str	r0, [r6, #32]
	movs	r2, #13
	movs	r3, #3
	movs	r0, #0
	str	r5, [sp, #0]
	bl	Func_08015010
	movs	r4, #224
	lsls	r4, r4, #2
	adds	r3, r6, r4
	ldr	r2, [r3, #0]
	mov	r8, r0
	movs	r0, #234
	movs	r3, #4
	lsls	r0, r0, #2
	strb	r3, [r2, #5]
	adds	r2, r6, r0
	movs	r3, #12
	strb	r3, [r2, #0]
	movs	r7, #0
	movs	r2, #0
	mov	r0, r8
	movs	r1, #2
	movs	r3, #8
	str	r7, [sp, #0]
	bl	Func_080a1028
	movs	r2, #234
	lsls	r2, r2, #2
	adds	r2, r2, r6
	mov	fp, r2
.L3:
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L0
	ldr	r0, [pc, #292]
	adds	r3, r6, r0
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	movs	r4, #0
	adds	r0, r7, r1
	mov	r9, r4
	bl	Func_080022fc
	movs	r3, #219
	adds	r7, r0, #0
	lsls	r1, r7, #1
	lsls	r3, r3, #2
	adds	r2, r1, r3
	adds	r3, r6, #2
	adds	r1, r1, r7
	ldrsh	r4, [r3, r2]
	lsls	r1, r1, #3
	subs	r1, #12
	mov	r0, r8
	movs	r2, #0
	mov	sl, r4
	bl	Func_080b0a6c
	movs	r3, #3
	mov	r2, fp
	strb	r3, [r2, #0]
	mov	r0, r8
	adds	r1, r7, #0
	movs	r2, #0
	bl	Func_080b11c4
	ldr	r0, [r6, #32]
	mov	r1, sl
	bl	Func_080b1dec
.L0:
	ldr	r1, [pc, #224]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L1
	movs	r0, #1
	bl	Func_080030f8
	mov	r0, sl
	bl	Func_08077248
	cmp	r0, #0
	bne.n	.L2
	movs	r0, #113
	bl	Func_080f9010
	b.n	.L3
.L2:
	movs	r0, #112
	bl	Func_080f9010
	ldr	r4, [pc, #184]
	adds	r3, r6, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #1
	bne.n	.L4
	mov	r0, sl
	bl	Func_080b1bd0
	b.n	.L5
.L4:
	mov	r0, sl
	bl	Func_080b211c
.L5:
	movs	r0, #224
	lsls	r0, r0, #2
	adds	r3, r6, r0
	ldr	r2, [r3, #0]
	movs	r3, #4
	strb	r3, [r2, #5]
	movs	r3, #12
	mov	r2, fp
	strb	r3, [r2, #0]
	movs	r3, #1
	mov	r9, r3
	b.n	.L3
.L1:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L6
	movs	r0, #113
	bl	Func_080f9010
	bl	Func_080a1030
	mov	r0, r8
	movs	r1, #2
	bl	Func_08015018
	ldr	r0, [r6, #32]
	movs	r1, #2
	bl	Func_08015018
	ldr	r0, [r6, #12]
	movs	r1, #2
	bl	Func_08015018
	movs	r0, #1
	bl	Func_080030f8
	movs	r0, #0
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
.L6:
	ldr	r5, [pc, #68]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L7
	movs	r0, #111
	bl	Func_080f9010
	movs	r4, #1
	subs	r7, #1
	mov	r9, r4
.L7:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L8
	movs	r0, #111
	bl	Func_080f9010
	movs	r0, #1
	adds	r7, #1
	mov	r9, r0
.L8:
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L3
	.4byte 0x03001f2c
	.4byte 0x000003a7
	.4byte 0x03001c94
	.4byte 0x000003aa
	.4byte 0x03001b04
