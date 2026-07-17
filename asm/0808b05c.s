@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0808b05c
	.thumb_func
Func_0808b05c:
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #1
	adds	r3, r3, r1
	ldr	r2, [pc, #8]
	lsls	r3, r3, #1
	adds	r3, #4
	ldrh	r0, [r2, r3]
	bx	lr
	movs	r0, r0
	.4byte 0x0809c610
