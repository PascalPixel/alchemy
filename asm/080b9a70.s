@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b9a70
	.thumb_func
Func_080b9a70:
	push	{r5, r6, lr}
	ldr	r3, [pc, #84]
	adds	r4, r0, #0
	ldr	r3, [r3, #0]
	cmp	r4, #7
	bhi.n	.L0
	movs	r5, #128
	movs	r0, #0
	lsls	r5, r5, #1
	movs	r1, #88
.L4:
	ldrsh	r2, [r1, r3]
	cmp	r2, #255
	beq.n	.L1
	cmp	r2, #254
	beq.n	.L2
	cmp	r2, r4
	beq.n	.L3
.L2:
	adds	r1, #2
	adds	r0, #1
	b.n	.L4
.L0:
	movs	r5, #192
	movs	r0, #0
	adds	r3, #2
	lsls	r5, r5, #1
	movs	r1, #100
.L8:
	ldrsh	r2, [r1, r3]
	cmp	r2, #255
	bne.n	.L5
.L1:
	movs	r0, #1
	negs	r0, r0
	b.n	.L6
.L5:
	cmp	r2, #254
	beq.n	.L7
	cmp	r2, r4
	bne.n	.L7
.L3:
	orrs	r0, r5
	b.n	.L6
.L7:
	adds	r1, #2
	adds	r0, #1
	b.n	.L8
.L6:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e74
