@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800403c
	.thumb_func
Func_0800403c:
	push	{lr}
	ldr	r0, [pc, #32]
	ldr	r3, [pc, #32]
	movs	r1, #0
	movs	r2, #255
.L0:
	adds	r1, #1
	strb	r2, [r3, #0]
	adds	r3, #1
	cmp	r1, r0
	bls.n	.L0
	ldr	r2, [pc, #20]
	ldr	r4, [pc, #8]
	movs	r1, #0
	movs	r0, #0
	b.n	.L1
	movs	r0, r0
	.4byte 0x0000ffff
	.4byte 0x000001ff
	.4byte 0x03001810
	.4byte 0x03001b10
.L1:
	ldrh	r3, [r2, #2]
	adds	r1, #1
	orrs	r3, r4
	strh	r3, [r2, #2]
	strh	r0, [r2, #0]
	adds	r2, #4
	cmp	r1, #95
	bls.n	.L1
	pop	{r0}
	bx	r0
