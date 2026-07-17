@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080bf400
	.thumb_func
Func_080bf400:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	Func_08077008
	movs	r3, #157
	lsls	r3, r3, #1
	adds	r5, r0, r3
	ldrb	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L0
	adds	r3, #255
	strb	r3, [r5, #0]
	lsls	r3, r3, #24
	movs	r0, #1
	cmp	r3, #0
	beq.n	.L1
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	movs	r2, #70
	bl	Func_080bf208
	cmp	r0, #0
	beq.n	.L0
	movs	r3, #0
	strb	r3, [r5, #0]
	movs	r0, #1
	b.n	.L1
.L0:
	movs	r0, #0
.L1:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
