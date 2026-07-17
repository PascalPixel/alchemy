@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080793b8
	.thumb_func
Func_080793b8:
	lsls	r3, r0, #20
	lsrs	r0, r3, #23
	ldr	r3, [pc, #4]
	ldrb	r0, [r3, r0]
	bx	lr
	movs	r0, r0
	.4byte 0x02000040
