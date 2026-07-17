@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080784d8
	.thumb_func
Func_080784d8:
	push	{lr}
	bl	Func_08077394
	movs	r3, #216
	ldrh	r3, [r0, r3]
	movs	r2, #0
	cmp	r3, #0
	beq.n	.L0
	adds	r0, #216
.L1:
	adds	r2, #1
	cmp	r2, #14
	bgt.n	.L0
	adds	r0, #2
	ldrh	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L1
.L0:
	adds	r0, r2, #0
	pop	{r1}
	bx	r1
