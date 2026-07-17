@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080ae99c
	.thumb_func
Func_080ae99c:
	push	{r5, r6, lr}
	adds	r5, r3, #0
	ldr	r3, [pc, #68]
	sub	sp, #4
	adds	r4, r0, #0
	adds	r6, r1, #0
	ldr	r3, [r3, #0]
	cmp	r5, #0
	bne.n	.L0
	ldr	r1, [pc, #60]
	b.n	.L1
.L0:
	movs	r1, #229
	lsls	r1, r1, #2
.L1:
	adds	r3, r3, r1
	ldrh	r0, [r3, #0]
	movs	r1, #128
	str	r2, [sp, #0]
	lsls	r1, r1, #23
	adds	r2, r4, #0
	adds	r3, r6, #0
	bl	Func_080150c8
	cmp	r0, #0
	bne.n	.L2
	movs	r0, #1
	negs	r0, r0
	b.n	.L3
.L2:
	movs	r3, #0
	strb	r3, [r0, #4]
	strh	r3, [r0, #12]
	movs	r3, #1
	strb	r3, [r0, #5]
	movs	r0, #1
.L3:
	add	sp, #4
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x00000392
