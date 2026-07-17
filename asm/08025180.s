@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08025180
	.thumb_func
Func_08025180:
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	adds	r7, r0, #0
	movs	r0, #1
	cmp	r6, #0
	beq.n	.L0
	adds	r0, r6, #0
	bl	Func_08077018
	adds	r5, r0, #0
	ldrb	r3, [r5, #12]
	movs	r0, #1
	cmp	r3, #3
	beq.n	.L0
	ldrh	r3, [r5, #40]
	cmp	r3, #0
	beq.n	.L0
	ldrb	r3, [r5, #2]
	cmp	r3, #0
	beq.n	.L1
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl	Func_08077218
	cmp	r0, #0
	bne.n	.L1
	movs	r0, #1
	b.n	.L0
.L1:
	ldrh	r0, [r5, #40]
	bl	Func_08077080
	ldrb	r2, [r0, #1]
	movs	r3, #128
	ands	r3, r2
	movs	r0, #2
	cmp	r3, #0
	beq.n	.L0
	movs	r0, #0
.L0:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
