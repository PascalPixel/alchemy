@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080f6148
	.thumb_func
Func_080f6148:
	push	{r5, r6, r7, lr}
	ldr	r5, [pc, #52]
	ldr	r7, [pc, #44]
	movs	r6, #0
.L3:
	ldrh	r2, [r5, #0]
	lsls	r3, r2, #16
	lsrs	r0, r3, #26
	ands	r0, r7
	lsrs	r1, r3, #21
	movs	r4, #31
	ands	r1, r7
	ands	r4, r2
	subs	r0, #1
	subs	r1, #1
	subs	r4, #1
	cmp	r0, #0
	bge.n	.L0
	movs	r0, #0
.L0:
	cmp	r1, #0
	bge.n	.L1
	movs	r1, #0
.L1:
	cmp	r4, #0
	bge.n	.L2
	movs	r4, #0
	b.n	.L2
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x05000140
.L2:
	lsls	r3, r0, #10
	lsls	r2, r1, #5
	orrs	r3, r2
	orrs	r3, r4
	adds	r6, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r6, #16
	bne.n	.L3
	ldr	r5, [pc, #52]
	ldr	r7, [pc, #44]
	movs	r6, #0
.L7:
	ldrh	r2, [r5, #0]
	lsls	r3, r2, #16
	lsrs	r0, r3, #26
	ands	r0, r7
	lsrs	r1, r3, #21
	movs	r4, #31
	ands	r1, r7
	ands	r4, r2
	subs	r0, #1
	subs	r1, #1
	subs	r4, #1
	cmp	r0, #0
	bge.n	.L4
	movs	r0, #0
.L4:
	cmp	r1, #0
	bge.n	.L5
	movs	r1, #0
.L5:
	cmp	r4, #0
	bge.n	.L6
	movs	r4, #0
	b.n	.L6
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x05000202
.L6:
	lsls	r3, r0, #10
	lsls	r2, r1, #5
	orrs	r3, r2
	orrs	r3, r4
	adds	r6, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r6, #239
	bne.n	.L7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
