@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b5c08
	.thumb_func
Func_080b5c08:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #24
	add	r5, sp, #4
	adds	r0, r5, #0
	bl	Func_080b6a60
	movs	r2, #0
	mov	sl, r0
	mov	r8, r2
	cmp	r8, sl
	bge.n	.L0
	mov	r9, r5
.L7:
	mov	r3, r9
	ldrh	r7, [r3, #0]
	movs	r6, #0
.L6:
	movs	r5, #0
.L5:
	adds	r0, r7, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl	Func_08077210
	cmp	r0, #0
	beq.n	.L1
	movs	r0, #0
	cmp	r7, #7
	bls.n	.L2
	movs	r0, #1
.L2:
	bl	Func_08077000
	movs	r2, #132
	adds	r3, r0, #0
	lsls	r2, r2, #1
	adds	r1, r3, #0
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	movs	r0, #0
	adds	r1, #8
	cmp	r0, r3
	bge.n	.L3
	ldrb	r3, [r1, #0]
	cmp	r6, r3
	bne.n	.L4
	ldrb	r3, [r1, #1]
	cmp	r5, r3
	beq.n	.L3
.L4:
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	adds	r0, #1
	cmp	r0, r3
	bge.n	.L3
	lsls	r2, r0, #2
	ldrb	r3, [r1, r2]
	cmp	r6, r3
	bne.n	.L4
	adds	r3, r1, r2
	ldrb	r3, [r3, #1]
	cmp	r5, r3
	bne.n	.L4
.L3:
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	cmp	r0, r3
	bne.n	.L1
	adds	r0, r7, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl	Func_080771c8
.L1:
	adds	r5, #1
	cmp	r5, #19
	ble.n	.L5
	adds	r6, #1
	cmp	r6, #3
	ble.n	.L6
	movs	r2, #1
	movs	r3, #2
	add	r8, r2
	add	r9, r3
	cmp	r8, sl
	blt.n	.L7
.L0:
	movs	r0, #182
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L8
	movs	r0, #0
	bl	Func_08077000
	movs	r2, #8
	adds	r3, r0, #0
	adds	r2, r2, r3
	mov	sl, r2
	movs	r2, #0
	mov	r8, r2
	movs	r2, #132
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	cmp	r8, r3
	bge.n	.L8
	movs	r3, #1
	negs	r3, r3
	mov	r9, r3
	mov	r7, sl
.L10:
	movs	r3, #3
	ldrsb	r3, [r7, r3]
	cmp	r3, r9
	bne.n	.L9
	ldrb	r0, [r7, #2]
	bl	Func_080b7dd0
	cmp	r0, #0
	bne.n	.L9
	ldrb	r3, [r7, #1]
	ldrb	r5, [r7, #2]
	ldrb	r6, [r7, #0]
	adds	r2, r3, #0
	adds	r1, r6, #0
	adds	r0, r5, #0
	str	r3, [sp, #0]
	bl	Func_080771b0
	ldr	r3, [sp, #0]
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r3, #0
	bl	Func_080771c0
.L9:
	movs	r3, #128
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #1
	ldr	r3, [r3, #0]
	add	r8, r2
	adds	r7, #4
	cmp	r8, r3
	blt.n	.L10
.L8:
	add	sp, #24
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
