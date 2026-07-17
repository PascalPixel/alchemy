@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0800488c
	.thumb_func
Func_0800488c:
	ldr	r3, [pc, #8]
	ldr	r0, [pc, #12]
	ldr	r3, [r3, #4]
	subs	r0, r0, r3
	bx	lr
	movs	r0, r0
	.4byte 0x03001e50
	.4byte 0x03007800
