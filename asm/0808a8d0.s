@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0808a8d0
	.thumb_func
Func_0808a8d0:
	ldr	r3, [pc, #12]
	lsls	r0, r0, #3
	adds	r0, r0, r3
	ldrb	r0, [r0, #2]
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	bx	lr
	movs	r0, r0
	.4byte 0x0809f1a8
