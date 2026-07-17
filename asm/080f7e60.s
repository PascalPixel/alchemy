@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080f7e60
	.thumb_func
Func_080f7e60:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r8, r1
	movs	r6, #0
	mov	sl, r0
	mov	fp, r2
	cmp	r6, r8
	bge.n	.L0
	ldr	r0, [pc, #160]
	movs	r7, #146
	lsls	r7, r7, #1
	mov	r9, r0
	add	r7, sl
.L2:
	mov	r3, r9
	adds	r0, r7, #0
	mov	r2, sl
	ands	r0, r3
	adds	r5, r2, r6
	bl	Func_080f7e34
	ldr	r3, [pc, #140]
	ldr	r0, [pc, #140]
	ldr	r1, [r3, #0]
	adds	r3, r1, r0
	ldr	r2, [r3, #0]
	mov	r0, fp
	ldrb	r4, [r0, r2]
	adds	r2, #1
	ldr	r0, [pc, #132]
	str	r2, [r3, #0]
	adds	r3, r1, r0
	ldr	r3, [r3, #0]
	cmp	r2, r3
	bne.n	.L1
	mov	r2, r9
	ands	r5, r2
	ldr	r0, [pc, #120]
	lsls	r3, r5, #2
	movs	r2, #1
	adds	r3, r3, r0
	negs	r2, r2
	str	r2, [r1, r3]
	b.n	.L0
.L1:
	mov	r0, r9
	ands	r0, r5
	ldr	r2, [pc, #100]
	lsls	r3, r0, #2
	adds	r3, r3, r2
	str	r4, [r1, r3]
	adds	r6, #1
	bl	Func_080f7df0
	adds	r7, #1
	cmp	r6, r8
	blt.n	.L2
.L0:
	adds	r6, #1
	cmp	r6, r8
	bge.n	.L3
	ldr	r3, [pc, #60]
	ldr	r0, [pc, #60]
	movs	r7, #1
	mov	fp, r3
	mov	r9, r0
	negs	r7, r7
.L4:
	mov	r2, sl
	adds	r5, r2, r6
	mov	r3, fp
	ands	r5, r3
	adds	r0, r5, #0
	bl	Func_080f7e34
	mov	r0, r9
	ldr	r2, [pc, #48]
	ldr	r3, [r0, #0]
	lsls	r5, r5, #2
	adds	r5, r5, r2
	adds	r6, #1
	str	r7, [r3, r5]
	cmp	r6, r8
	blt.n	.L4
.L3:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x02004c00
	.4byte 0x00004438
	.4byte 0x00004440
	.4byte 0x00003404
