@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a40ac
	.thumb_func
Func_080a40ac:
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	bl	Func_08077008
	movs	r3, #216
	ldrh	r3, [r0, r3]
	movs	r1, #0
	movs	r6, #0
	adds	r0, #216
	b.n	.L0
.L8:
	ldrh	r2, [r0, #0]
	ldr	r3, [pc, #40]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L1
	lsrs	r3, r2, #11
	adds	r2, r3, #1
	cmp	r3, #0
	bne.n	.L2
	movs	r2, #1
.L2:
	cmp	r2, #0
	beq.n	.L3
	adds	r5, r2, #0
.L4:
	adds	r1, r6, #0
	adds	r0, r7, #0
	bl	Func_08077058
	subs	r5, #1
	adds	r1, r0, #0
	cmp	r5, #0
	bne.n	.L4
	b.n	.L3
	.4byte 0x00000200
.L3:
	movs	r0, #0
	cmp	r1, #2
	bne.n	.L5
	b.n	.L6
.L1:
	adds	r6, #1
	adds	r0, #2
	cmp	r6, #14
	bgt.n	.L7
	ldrh	r3, [r0, #0]
.L0:
	cmp	r3, #0
	bne.n	.L8
.L6:
	movs	r1, #1
.L7:
	adds	r0, r1, #0
.L5:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
