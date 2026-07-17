@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_080a1090
	.thumb_func
Func_080a1090:
	ldr	r3, [pc, #48]
	sub	sp, #4
	ldr	r4, [r3, #0]
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	adds	r1, r4, #0
	ldr	r3, [pc, #40]
	ldr	r2, [pc, #40]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #137
	movs	r3, #255
	lsls	r1, r1, #1
	strb	r3, [r4, #28]
	adds	r2, r4, r1
	movs	r3, #1
	adds	r1, #1
	strb	r3, [r4, #30]
	strb	r3, [r4, #31]
	strb	r3, [r2, #0]
	adds	r2, r4, r1
	strb	r3, [r2, #0]
	add	sp, #4
	bx	lr
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x040000d4
	.4byte 0x8500029c
