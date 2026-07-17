@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08078a60
	.thumb_func
Func_08078a60:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	Func_08077394
	lsls	r5, r5, #1
	adds	r5, #216
	ldrh	r2, [r0, r5]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L0
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L0:
	ldr	r3, [pc, #12]
	ands	r3, r2
	strh	r3, [r0, r5]
	movs	r0, #0
.L1:
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x0000fbff
