@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_08078ed8
	.thumb_func
Func_08078ed8:
	movs	r3, #180
	muls	r0, r3
	ldr	r2, [pc, #4]
	adds	r0, r0, r2
	bx	lr
	movs	r0, r0
	.4byte 0x080844ec
