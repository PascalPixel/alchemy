@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_080fa264
	.thumb_func
Func_080fa264:
	adds	r2, r0, #0
	ldr	r3, [r2, #52]
	ldr	r0, [pc, #12]
	cmp	r3, r0
	bne.n	.L0
	ldr	r0, [r2, #4]
	ldr	r1, [pc, #8]
	ands	r0, r1
	str	r0, [r2, #4]
.L0:
	bx	lr
	.4byte 0x68736d53
	.4byte 0x7fffffff
