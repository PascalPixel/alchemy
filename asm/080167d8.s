@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080167d8
	.thumb_func
Func_080167d8:
	movs	r3, #2
	strh	r3, [r0, #28]
	bx	lr
