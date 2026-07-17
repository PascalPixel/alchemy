@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080040d0
	.thumb_func
Func_080040d0:
	push	{lr}
	adds	r2, r1, #0
	ldr	r1, [pc, #12]
	lsls	r3, r0, #2
	ldrh	r1, [r1, r3]
	bl	Func_08003fa4
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001b10
