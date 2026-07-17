@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080783a4
	.thumb_func
Func_080783a4:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r7, r0, #0
	bl	Func_08077394
	adds	r6, r0, #0
	movs	r1, #56
	ldrsh	r3, [r6, r1]
	movs	r1, #52
	ldrsh	r2, [r6, r1]
	adds	r3, r3, r5
	adds	r1, r2, #0
	cmp	r3, r2
	bgt.n	.L0
	movs	r1, #0
	cmp	r3, #0
	blt.n	.L0
	adds	r1, r3, #0
.L0:
	adds	r0, r7, #0
	strh	r1, [r6, #56]
	bl	Func_0807822c
	movs	r2, #56
	ldrsh	r0, [r6, r2]
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
