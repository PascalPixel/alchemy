@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0800ca2c
	.thumb_func
Func_0800ca2c:
	movs	r3, #4
	ldrsh	r2, [r0, r3]
	ldr	r3, [r0, #0]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r3, #4
	str	r3, [r0, #0]
	movs	r3, #0
	strh	r3, [r0, #4]
	movs	r0, #1
	bx	lr
