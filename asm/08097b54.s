@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_08097b54
	.thumb_func
Func_08097b54:
	ldr	r3, [pc, #16]
	ldr	r3, [r3, #0]
	movs	r2, #15
	lsrs	r3, r3, #4
	ldr	r1, [pc, #12]
	ands	r3, r2
	lsls	r3, r3, #1
	ldrh	r0, [r1, r3]
	bx	lr
	movs	r0, r0
	.4byte 0x03001ae8
	.4byte 0x0809f0f8
