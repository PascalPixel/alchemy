@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080792fc
	.thumb_func
Func_080792fc:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	adds	r6, r0, #0
	mov	r8, r1
	bl	Func_08077394
	ldrb	r5, [r0, #15]
	cmp	r5, r8
	bge.n	.L0
	mov	r3, r8
	mov	r7, sp
	subs	r5, r3, r5
.L1:
	adds	r0, r6, #0
	adds	r1, r7, #0
	subs	r5, #1
	bl	Func_0807905c
	cmp	r5, #0
	bne.n	.L1
.L0:
	adds	r0, r6, #0
	bl	Func_08077428
	add	sp, #16
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
