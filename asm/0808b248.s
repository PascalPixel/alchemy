@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0808b248
	.thumb_func
Func_0808b248:
	ldr	r3, [pc, #12]
	movs	r2, #235
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bx	lr
	movs	r0, r0
	.4byte 0x02000240
