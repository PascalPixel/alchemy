@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08078a08
	.thumb_func
Func_08078a08:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	Func_08078698
	movs	r3, #1
	adds	r5, r0, #0
	negs	r3, r3
	movs	r0, #0
	cmp	r5, r3
	beq.n	.L0
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	Func_08078664
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_08078948
	movs	r0, #0
.L0:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
