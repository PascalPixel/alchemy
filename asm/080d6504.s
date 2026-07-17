@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080d6504
	.thumb_func
Func_080d6504:
	push	{lr}
	ldr	r3, [pc, #72]
	ldr	r2, [r3, #0]
	subs	r3, #108
	ldr	r1, [r3, #0]
	ldr	r3, [pc, #68]
	adds	r0, r2, r3
	ldr	r3, [r0, #0]
	cmp	r3, #1
	bne.n	.L0
	ldr	r4, [pc, #60]
	adds	r3, r2, r4
	ldr	r2, [r3, #0]
	ldrh	r3, [r1, #54]
	adds	r3, r3, r2
	movs	r2, #0
	strh	r3, [r1, #54]
	str	r2, [r0, #0]
	b.n	.L1
.L0:
	ldr	r4, [pc, #44]
	adds	r3, r2, r4
	ldr	r2, [r3, #0]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldrh	r3, [r1, #54]
	asrs	r2, r2, #1
	adds	r3, r3, r2
	strh	r3, [r1, #54]
	ldr	r3, [r0, #0]
	cmp	r3, #2
	bne.n	.L2
	movs	r3, #0
	b.n	.L3
.L2:
	movs	r3, #2
.L3:
	str	r3, [r0, #0]
.L1:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x000077b0
	.4byte 0x000077ac
