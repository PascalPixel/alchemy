@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0800d654
	.thumb_func
Func_0800d654:
	movs	r2, #4
	ldrsh	r3, [r0, r2]
	ldr	r2, [r0, #0]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r3, [r3, #4]
	adds	r2, r0, #0
	subs	r3, #1
	adds	r2, #94
	strh	r3, [r2, #0]
	ldrh	r3, [r0, #4]
	adds	r3, #2
	strh	r3, [r0, #4]
	movs	r0, #0
	bx	lr
