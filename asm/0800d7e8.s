@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0800d7e8
	.thumb_func
Func_0800d7e8:
	ldr	r3, [pc, #8]
	str	r3, [r0, #0]
	movs	r3, #0
	strh	r3, [r0, #4]
	movs	r0, #0
	bx	lr
	.4byte 0x08013240
