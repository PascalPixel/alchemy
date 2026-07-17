@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_080251d4
	.thumb_func
Func_080251d4:
	adds	r3, r0, #0
	ldr	r0, [pc, #28]
	movs	r2, #192
	ands	r1, r0
	ands	r0, r3
	lsls	r2, r2, #19
	lsls	r0, r0, #5
	lsls	r1, r1, #5
	adds	r0, r0, r2
	adds	r1, r1, r2
	ldr	r3, [pc, #12]
	ldr	r2, [pc, #16]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x040000d4
	.4byte 0x84000008
