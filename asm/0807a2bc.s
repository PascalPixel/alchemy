@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0807a2bc
	.thumb_func
Func_0807a2bc:
	push	{r5, r6, lr}
	adds	r6, r2, #0
	adds	r5, r1, #0
	bl	Func_08077394
	movs	r3, #132
	lsls	r3, r3, #1
	lsls	r5, r5, #2
	adds	r5, r5, r3
	ldr	r3, [r0, r5]
	movs	r2, #1
	lsls	r2, r6
	ands	r3, r2
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	pop	{r5, r6}
	pop	{r1}
	bx	r1
