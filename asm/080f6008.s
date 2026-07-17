@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080f6008
	.thumb_func
Func_080f6008:
	push	{lr}
	ldr	r3, [pc, #28]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #24]
	ldr	r2, [pc, #24]
	ldr	r3, [r3, #4]
	movs	r0, #9
	str	r3, [r2, #0]
	bl	Func_080f9010
	bl	Func_080f7460
	movs	r0, #0
	b.n	.L0
	.4byte 0x00000040
	.4byte 0x02000240
	.4byte 0x03001cb4
.L0:
	pop	{r1}
	bx	r1
