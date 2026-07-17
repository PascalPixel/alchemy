@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080b2764
	.thumb_func
Func_080b2764:
	lsls	r3, r0, #5
	adds	r3, r3, r0
	ldr	r2, [pc, #8]
	lsls	r3, r3, #1
	adds	r3, #64
	ldrsh	r0, [r2, r3]
	bx	lr
	movs	r0, r0
	.4byte 0x080b41ac
