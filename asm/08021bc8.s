@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_08021bc8
	.thumb_func
Func_08021bc8:
	push	{lr}
	cmp	r0, #0
	beq.n	.L0
	movs	r0, #0
.L0:
	ldr	r3, [pc, #8]
	lsls	r2, r0, #2
	ldr	r0, [r3, r2]
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x08073968
