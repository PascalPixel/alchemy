@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800b868
	.thumb_func
Func_0800b868:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L0
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl	Func_08185000
	adds	r6, r0, #0
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L0
	ldr	r0, [r6, #12]
	cmp	r0, #0
	bne.n	.L1
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl	Func_0800b798
.L1:
	ldrb	r3, [r6, #4]
	strb	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r0, [r5, #8]
	str	r3, [r5, #12]
	ldrb	r3, [r6, #10]
	movs	r2, #0
	strb	r3, [r5, #7]
	movs	r3, #255
	strb	r3, [r5, #22]
	str	r2, [r5, #16]
	strb	r2, [r5, #20]
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
