@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_080ad5f4
	.thumb_func
Func_080ad5f4:
	ldr	r3, [pc, #12]
	movs	r2, #145
	ldr	r3, [r3, #0]
	lsls	r2, r2, #2
	lsls	r0, r0, #2
	adds	r0, r0, r2
	str	r1, [r3, r0]
	bx	lr
	.4byte 0x03001f2c
