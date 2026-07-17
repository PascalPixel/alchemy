@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080c23a0
	.thumb_func
Func_080c23a0:
	push	{lr}
	cmp	r0, #171
	bls.n	.L0
	ldr	r3, [pc, #20]
	ldrh	r0, [r3, #0]
	b.n	.L1
.L0:
	ldr	r3, [pc, #12]
	lsls	r2, r0, #3
	adds	r2, r2, r3
	ldrb	r0, [r2, #3]
	lsls	r0, r0, #27
	lsrs	r0, r0, #28
.L1:
	pop	{r1}
	bx	r1
	.4byte 0x080c7420
