@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b9604
	.thumb_func
Func_080b9604:
	push	{r5, r6, r7, lr}
	mov	r7, r9
	mov	r6, r8
	push	{r6, r7}
	mov	r1, r9
	sub	sp, #4
	mov	r8, r1
	mov	r3, sp
	mov	r7, r8
	str	r1, [r3, #0]
	subs	r7, #4
	ldr	r0, [r7, #0]
	bl	Func_08006408
	movs	r2, #1
	movs	r5, #150
	negs	r2, r2
	movs	r6, #0
	lsls	r5, r5, #1
	cmp	r0, r2
	bne.n	.L0
	b.n	.L1
.L4:
	ldr	r3, [pc, #232]
	ldrh	r3, [r3, #0]
	cmp	r3, #20
	bhi.n	.L2
	movs	r0, #1
	subs	r5, #1
	bl	Func_080030f8
	cmp	r5, #0
	blt.n	.L2
	ldr	r3, [pc, #216]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L3
	adds	r6, #1
	cmp	r6, #24
	ble.n	.L0
	b.n	.L2
.L3:
	movs	r6, #0
.L0:
	bl	Func_080064f4
	cmp	r0, #0
	bne.n	.L4
	ldr	r3, [pc, #184]
	ldrh	r3, [r3, #0]
	cmp	r3, #20
	bne.n	.L2
	movs	r3, #16
	negs	r3, r3
	add	r3, r8
	mov	r9, r3
	ldr	r3, [r7, #0]
	ldr	r2, [r3, #0]
	mov	r1, r9
	str	r2, [r1, #0]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L5
	mov	r3, r8
	mov	r2, r8
	subs	r3, #20
	subs	r2, #12
	ldr	r3, [r3, #0]
	ldr	r0, [r2, #0]
	lsls	r3, r3, #4
	adds	r0, r0, r3
	bl	Func_08006408
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L6
	b.n	.L1
.L8:
	ldr	r3, [pc, #124]
	ldrh	r3, [r3, #0]
	mov	r8, r3
	mov	r3, r9
	ldr	r0, [r3, #0]
	lsls	r0, r0, #4
	adds	r0, #19
	movs	r1, #20
	bl	Func_080022f4
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #2
	cmp	r8, r3
	bhi.n	.L2
	movs	r0, #1
	subs	r5, #1
	bl	Func_080030f8
	cmp	r5, #0
	blt.n	.L2
	ldr	r3, [pc, #84]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L7
	adds	r6, #1
	cmp	r6, #24
	ble.n	.L6
	b.n	.L2
.L7:
	movs	r6, #0
.L6:
	bl	Func_080064f4
	cmp	r0, #0
	bne.n	.L8
	mov	r1, r9
	ldr	r0, [r1, #0]
	ldr	r3, [pc, #48]
	lsls	r0, r0, #4
	ldrh	r3, [r3, #0]
	adds	r0, #19
	movs	r1, #20
	mov	r8, r3
	bl	Func_080022f4
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #2
	cmp	r8, r3
	beq.n	.L5
.L2:
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L5:
	movs	r0, #0
.L1:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	r9, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02002238
	.4byte 0x03001f64
