@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08096bec
	.thumb_func
Func_08096bec:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	sub	sp, #12
	adds	r0, r1, #0
	adds	r1, r2, #0
	cmp	r6, #0
	beq.n	.L0
	ldr	r3, [r6, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl	Func_0800447c
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	adds	r0, r6, #0
	bl	Func_08009150
.L0:
	add	sp, #12
	pop	{r5, r6}
	pop	{r0}
	bx	r0
