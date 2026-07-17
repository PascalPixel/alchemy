@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08079cbc
	.thumb_func
Func_08079cbc:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r2, #128
	lsls	r2, r2, #2
	sub	sp, #4
	adds	r7, r0, #0
	movs	r6, #0
	movs	r5, #216
	mov	r8, r2
	movs	r1, #14
.L3:
	ldrh	r3, [r5, r7]
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	ldrh	r0, [r5, r7]
	str	r1, [sp, #0]
	bl	Func_08078414
	ldr	r1, [sp, #0]
	adds	r0, #24
	movs	r2, #3
.L2:
	ldrb	r3, [r0, #0]
	cmp	r3, #23
	bne.n	.L1
	movs	r3, #1
	ldrsb	r3, [r0, r3]
	adds	r6, r6, r3
.L1:
	subs	r2, #1
	adds	r0, #4
	cmp	r2, #0
	bge.n	.L2
.L0:
	subs	r1, #1
	adds	r5, #2
	cmp	r1, #0
	bge.n	.L3
	cmp	r6, #0
	bge.n	.L4
	movs	r6, #0
.L4:
	adds	r0, r6, #0
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
