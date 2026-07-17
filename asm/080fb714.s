@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080fb714
	.thumb_func
Func_080fb714:
	ldr	r0, [r1, #64]
	ldrb	r0, [r0, #0]
	adds	r2, r1, #0
	adds	r2, #45
	strb	r0, [r2, #0]
	ldr	r0, [r1, #64]
	adds	r0, #1
	str	r0, [r1, #64]
	bx	lr
