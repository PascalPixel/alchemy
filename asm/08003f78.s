@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08003f78
	.thumb_func
Func_08003f78:
	push	{r5, lr}
	ldr	r3, [pc, #36]
	lsls	r2, r0, #2
	adds	r5, r2, r3
	cmp	r0, #95
	bls.n	.L0
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L0:
	ldrh	r3, [r5, #0]
	cmp	r3, #16
	bls.n	.L2
	bl	Func_08003f04
	movs	r3, #1
	strh	r3, [r5, #0]
.L2:
	movs	r0, #0
.L1:
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x03001b10
