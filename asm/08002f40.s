@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_08002f40
	.thumb_func
Func_08002f40:
	ldr	r3, [pc, #4]
	lsls	r0, r0, #2
	ldr	r0, [r3, r0]
	bx	lr
	.4byte 0x08320000
