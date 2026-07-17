@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_08004760
	.thumb_func
Func_08004760:
	sub	sp, #4
	ldr	r3, [pc, #32]
	mov	r0, sp
	str	r3, [r0, #0]
	ldr	r1, [pc, #28]
	ldr	r3, [pc, #32]
	ldr	r2, [pc, #32]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #32]
	ldr	r3, [pc, #16]
	str	r3, [r2, #0]
	movs	r2, #128
	ldr	r3, [pc, #28]
	lsls	r2, r2, #3
	strh	r2, [r3, #0]
	add	sp, #4
	bx	lr
	.4byte 0xf000f000
	.4byte 0x06002000
	.4byte 0x040000d4
	.4byte 0x85000140
	.4byte 0x03001cbc
	.4byte 0x04000008
