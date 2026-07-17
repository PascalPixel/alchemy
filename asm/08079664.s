@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08079664
	.thumb_func
Func_08079664:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	bl	Func_080795fc
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	Func_08079374
	movs	r1, #0
	cmp	r1, r6
	bge.n	.L0
	ldr	r0, [pc, #68]
	movs	r2, #252
	lsls	r2, r2, #1
	ldrb	r3, [r0, r2]
	cmp	r3, r5
	beq.n	.L0
	adds	r2, r0, r2
.L1:
	adds	r1, #1
	cmp	r1, r6
	bge.n	.L0
	adds	r2, #1
	ldrb	r3, [r2, #0]
	cmp	r3, r5
	bne.n	.L1
.L0:
	subs	r0, r6, #1
	cmp	r1, r0
	bge.n	.L2
	ldr	r3, [pc, #32]
	movs	r4, #252
	adds	r3, r1, r3
	lsls	r4, r4, #1
	adds	r2, r3, r4
	subs	r1, r0, r1
.L3:
	ldrb	r3, [r2, #1]
	subs	r1, #1
	strb	r3, [r2, #0]
	adds	r2, #1
	cmp	r1, #0
	bne.n	.L3
.L2:
	bl	Func_080795fc
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02000240
