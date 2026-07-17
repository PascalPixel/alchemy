@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c23e8
	.thumb_func
Func_080c23e8:
	push	{lr}
	cmp	r0, #171
	bls.n	.L0
	movs	r0, #1
	b.n	.L1
.L0:
	ldr	r3, [pc, #24]
	lsls	r2, r0, #3
	adds	r2, r2, r3
	ldrb	r3, [r2, #2]
	lsls	r3, r3, #27
	lsrs	r3, r3, #28
	adds	r0, r3, #0
	cmp	r3, #0
	bne.n	.L1
	movs	r0, #1
.L1:
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x080c7420
