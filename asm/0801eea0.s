@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801eea0
	.thumb_func
Func_0801eea0:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #92]
	ldr	r5, [r3, #0]
	subs	r3, #4
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #88]
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	adds	r6, r0, #0
	movs	r7, #4
	cmp	r3, #0
	beq.n	.L0
	movs	r0, #0
	bl	Func_080b50c8
	movs	r7, #3
	b.n	.L1
.L0:
	bl	Func_08077148
.L1:
	movs	r3, #1
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L2
	adds	r7, #1
	b.n	.L3
.L2:
	movs	r3, #3
	negs	r3, r3
	ands	r6, r3
.L3:
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r0, r3, #1
	movs	r3, #2
	ands	r3, r6
	adds	r1, r0, #1
	cmp	r3, #0
	beq.n	.L4
	adds	r1, r0, #6
.L4:
	movs	r3, #30
	subs	r3, r3, r1
	movs	r2, #0
	strh	r3, [r5, #4]
	strh	r2, [r5, #6]
	strh	r1, [r5, #8]
	strh	r7, [r5, #10]
	strh	r6, [r5, #12]
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e90
	.4byte 0x00000ea5
