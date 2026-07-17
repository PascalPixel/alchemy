@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0808d458
	.thumb_func
Func_0808d458:
	push	{lr}
	movs	r3, #15
	ands	r3, r0
	cmp	r3, #3
	beq.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	ldr	r3, [pc, #28]
	ands	r3, r0
	movs	r0, #0
	cmp	r3, #3
	beq.n	.L1
	ldr	r3, [pc, #20]
	movs	r2, #160
	ands	r3, r1
	lsls	r2, r2, #15
	eors	r3, r2
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
.L1:
	pop	{r1}
	bx	r1
	.4byte 0x000001ff
	.4byte 0xfff00000
