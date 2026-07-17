@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0808ddb8
	.thumb_func
Func_0808ddb8:
	push	{lr}
	ldr	r2, [pc, #44]
	movs	r4, #0
	ldrsh	r3, [r2, r4]
	movs	r4, #1
	negs	r4, r4
	movs	r1, #16
	b.n	.L0
.L2:
	adds	r2, #2
	movs	r4, #0
	ldrsh	r3, [r2, r4]
	movs	r4, #1
	negs	r4, r4
.L0:
	cmp	r3, r4
	beq.n	.L1
	adds	r2, #2
	cmp	r0, r3
	bne.n	.L2
	movs	r3, #0
	ldrsh	r1, [r2, r3]
.L1:
	adds	r0, r1, #0
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x0809e686
