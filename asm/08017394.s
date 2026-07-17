@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_08017394
	.thumb_func
Func_08017394:
	push	{lr}
	ldrh	r3, [r0, #22]
	cmp	r3, #0
	bne.n	.L0
	movs	r2, #26
	ldrsh	r3, [r0, r2]
	movs	r0, #1
	cmp	r3, #0
	beq.n	.L1
.L0:
	movs	r0, #0
.L1:
	pop	{r1}
	bx	r1
