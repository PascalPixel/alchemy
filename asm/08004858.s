@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_08004858
	.thumb_func
Func_08004858:
	sub	sp, #4
	ldr	r4, [pc, #28]
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	adds	r1, r4, #0
	ldr	r3, [pc, #20]
	ldr	r2, [pc, #24]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #20]
	str	r3, [r4, #4]
	ldr	r3, [pc, #20]
	add	sp, #4
	str	r3, [r4, #0]
	bx	lr
	.4byte 0x03001e50
	.4byte 0x040000d4
	.4byte 0x85000040
	.4byte 0x03002000
	.4byte 0x02030000
