@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08092504
	.thumb_func
Func_08092504:
	push	{r5, r6, r7, lr}
	sub	sp, #4
	bl	Func_0808ba1c
	cmp	r0, #0
	beq.n	.L0
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L0
	ldr	r3, [r0, #80]
	adds	r6, r3, #0
	adds	r6, #36
	ldrb	r3, [r6, #0]
	mov	r7, sp
	movs	r5, #0
	str	r3, [r7, #0]
	b.n	.L1
.L2:
	adds	r5, #1
.L1:
	cmp	r5, #89
	bgt.n	.L0
	movs	r0, #1
	bl	Func_080030f8
	ldrb	r2, [r6, #0]
	ldr	r3, [r7, #0]
	cmp	r3, r2
	beq.n	.L2
.L0:
	add	sp, #4
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
