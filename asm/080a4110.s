@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080a4110
	.thumb_func
Func_080a4110:
	push	{lr}
	cmp	r0, #2
	bgt.n	.L0
	cmp	r1, #2
	bgt.n	.L0
	cmp	r0, #0
	blt.n	.L0
	cmp	r1, #0
	bge.n	.L1
.L0:
	movs	r0, #0
	b.n	.L2
.L1:
	lsls	r2, r1, #1
	adds	r2, r2, r1
	adds	r2, r2, r0
	ldr	r3, [pc, #8]
	lsls	r2, r2, #2
	ldr	r0, [r3, r2]
.L2:
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x080af2e4
