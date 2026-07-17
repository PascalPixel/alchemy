@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0809ba34
	.thumb_func
Func_0809ba34:
	push	{lr}
	adds	r3, r0, #0
	adds	r3, #65
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	ldr	r3, [r0, #12]
	movs	r2, #128
	lsls	r2, r2, #24
	eors	r3, r2
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
.L1:
	pop	{r1}
	bx	r1
