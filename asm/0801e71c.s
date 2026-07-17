@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0801e71c
	.thumb_func
Func_0801e71c:
	ldr	r3, [pc, #16]
	ldr	r2, [pc, #12]
	ldr	r3, [r3, #0]
	ands	r0, r2
	ldr	r2, [pc, #12]
	adds	r3, r3, r2
	strh	r0, [r3, #0]
	bx	lr
	.4byte 0x0000000f
	.4byte 0x03001e8c
	.4byte 0x00000eae
