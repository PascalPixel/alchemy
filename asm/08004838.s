@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_08004838
	.thumb_func
Func_08004838:
	ldr	r3, [pc, #12]
	ldr	r0, [pc, #16]
	ldr	r1, [pc, #16]
	ldr	r2, [pc, #20]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	movs	r0, r0
	.4byte 0x040000d4
	.4byte 0x0800779c
	.4byte 0x05000200
	.4byte 0x800000e0
