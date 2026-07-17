@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080164ac
	.thumb_func
Func_080164ac:
	push	{r5, lr}
	adds	r3, r0, #0
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L0
	ldr	r0, [r3, #0]
	str	r3, [r3, #4]
	str	r5, [r3, #0]
	cmp	r0, #0
	beq.n	.L0
.L1:
	ldr	r5, [r0, #0]
	bl	Func_08016594
	adds	r0, r5, #0
	cmp	r0, #0
	bne.n	.L1
.L0:
	pop	{r5}
	pop	{r0}
	bx	r0
