@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b6cdc
	.thumb_func
Func_080b6cdc:
	push	{r5, lr}
	bl	Func_080c23c0
	ldr	r3, [pc, #72]
	ldr	r4, [r3, #0]
	movs	r2, #4
	ldrsh	r3, [r4, r2]
	movs	r1, #0
	cmp	r3, #0
	bne.n	.L0
	cmp	r0, #0
	bne.n	.L1
	movs	r5, #6
	ldrsh	r3, [r4, r5]
	cmp	r3, #0
	beq.n	.L1
.L0:
	adds	r1, #1
	cmp	r1, #5
	bgt.n	.L1
	lsls	r2, r1, #1
	adds	r3, r2, #4
	ldrsh	r3, [r4, r3]
	cmp	r3, #0
	bne.n	.L0
	cmp	r0, #0
	bne.n	.L1
	cmp	r1, #4
	bgt.n	.L0
	adds	r3, r2, #6
	ldrsh	r3, [r4, r3]
	cmp	r3, #0
	bne.n	.L0
.L1:
	movs	r3, #6
	eors	r3, r1
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x03001e74
