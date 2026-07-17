@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_08019d0c
	.thumb_func
Func_08019d0c:
	ldr	r3, [pc, #20]
	ldr	r0, [pc, #24]
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #12]
	adds	r1, r3, r0
	adds	r0, #2
	strh	r2, [r1, #0]
	adds	r1, r3, r0
	strh	r2, [r1, #0]
	bx	lr
	.4byte 0x000003e7
	.4byte 0x03001e8c
	.4byte 0x000012ec
