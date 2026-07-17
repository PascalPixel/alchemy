@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080b8808
	.thumb_func
Func_080b8808:
	push	{lr}
	cmp	r0, #7
	bhi.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	adds	r3, r0, #0
	subs	r3, #128
	movs	r0, #0
	cmp	r3, #5
	bls.n	.L1
	movs	r0, #1
	negs	r0, r0
.L1:
	pop	{r1}
	bx	r1
