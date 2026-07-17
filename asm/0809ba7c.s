@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_0809ba7c
	.thumb_func
Func_0809ba7c:
	movs	r3, #0
	ldr	r2, [pc, #12]
	str	r1, [r0, #52]
	strh	r3, [r0, #58]
	strh	r3, [r0, #56]
	adds	r0, #64
	strb	r2, [r0, #0]
	bx	lr
	.4byte 0x00000000
