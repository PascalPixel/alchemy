@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08012204
	.thumb_func
Func_08012204:
	push	{r5, lr}
	ldr	r4, [r0, #8]
	ldr	r3, [r0, #0]
	asrs	r2, r4, #17
	asrs	r1, r3, #17
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L0
	adds	r3, r2, #7
.L0:
	asrs	r3, r3, #3
	movs	r5, #63
	ands	r3, r5
	lsls	r0, r3, #6
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L1
	adds	r3, r1, #7
.L1:
	asrs	r3, r3, #3
	ands	r3, r5
	adds	r5, r0, r3
	lsrs	r3, r4, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	movs	r2, #3
	ands	r3, r2
	lsls	r0, r3, #1
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L2
	adds	r3, r1, #3
.L2:
	movs	r2, #1
	asrs	r3, r3, #2
	ands	r3, r2
	ldr	r2, [pc, #84]
	adds	r4, r0, r3
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	ldr	r2, [pc, #80]
	lsls	r3, r3, #3
	adds	r3, r3, r4
	adds	r3, r3, r2
	ldrb	r2, [r3, #0]
	cmp	r2, #0
	beq.n	.L3
	movs	r3, #2
	ands	r3, r1
	lsrs	r0, r2, #4
	cmp	r3, #0
	bne.n	.L4
	movs	r0, #15
	ands	r0, r2
.L4:
	cmp	r0, #0
	bne.n	.L5
.L3:
	ldr	r2, [pc, #52]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	ldr	r2, [pc, #48]
	lsls	r3, r3, #3
	adds	r3, r3, r4
	adds	r3, r3, r2
	ldrb	r2, [r3, #0]
	cmp	r2, #0
	beq.n	.L6
	movs	r3, #2
	ands	r3, r1
	lsrs	r0, r2, #4
	cmp	r3, #0
	bne.n	.L7
	movs	r0, #15
	ands	r0, r2
.L7:
	cmp	r0, #0
	bne.n	.L5
.L6:
	movs	r0, #7
.L5:
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x06005000
	.4byte 0x0202c800
	.4byte 0x06004000
	.4byte 0x0202c000
