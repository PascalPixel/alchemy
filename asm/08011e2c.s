@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_08011e2c
	.thumb_func
Func_08011e2c:
	adds	r3, r0, #0
	movs	r0, #0
	ldrsb	r0, [r3, r0]
	movs	r4, #1
	ldrsb	r4, [r3, r4]
	lsls	r2, r2, #4
	ldr	r3, [pc, #16]
	adds	r1, r1, r2
	lsls	r0, r0, #19
	ldrb	r3, [r3, r1]
	lsls	r4, r4, #19
	subs	r4, r4, r0
	muls	r3, r4
	adds	r0, r0, r3
	bx	lr
	movs	r0, r0
	.4byte 0x080132fc
