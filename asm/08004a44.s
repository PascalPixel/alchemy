@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_08004a44
	.thumb_func
Func_08004a44:
	ldr	r3, [pc, #8]
	ldr	r1, [pc, #12]
	ldr	r2, [pc, #12]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	.4byte 0x040000d4
	.4byte 0x03000350
	.4byte 0x8400000c
