@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_08079338
	.thumb_func
Func_08079338:
	movs	r3, #7
	ands	r3, r0
	movs	r2, #1
	lsls	r2, r3
	lsls	r3, r0, #20
	lsrs	r0, r3, #23
	ldr	r3, [pc, #12]
	ldrb	r3, [r3, r0]
	ands	r3, r2
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	bx	lr
	movs	r0, r0
	.4byte 0x02000040
