@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08078948
	.thumb_func
Func_08078948:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	bl	Func_08077394
	lsls	r3, r6, #1
	adds	r3, #216
	ldrh	r7, [r0, r3]
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	Func_080788c4
	movs	r3, #1
	adds	r5, r0, #0
	negs	r3, r3
	cmp	r5, r3
	beq.n	.L0
	movs	r1, #1
	adds	r0, r7, #0
	bl	Func_08078ad0
	bl	Func_0808a548
.L0:
	adds	r0, r5, #0
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
