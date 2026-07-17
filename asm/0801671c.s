@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0801671c
	.thumb_func
Func_0801671c:
	push	{lr}
	movs	r1, #240
	ldr	r3, [pc, #12]
	lsls	r1, r1, #4
	movs	r2, #0
	ldr	r0, [pc, #12]
	bl	Func_080072f0
	pop	{r1}
	bx	r1
	.4byte 0x03000168
	.4byte 0x06002500
