@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c2410
	.thumb_func
Func_080c2410:
	push	{lr}
	cmp	r0, #171
	bls.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	ldr	r3, [pc, #20]
	lsls	r2, r0, #3
	adds	r2, r2, r3
	ldrb	r3, [r2, #2]
	lsrs	r3, r3, #5
	adds	r0, r3, #0
	cmp	r3, #0
	bne.n	.L1
	movs	r0, #0
.L1:
	pop	{r1}
	bx	r1
	.4byte 0x080c7420
