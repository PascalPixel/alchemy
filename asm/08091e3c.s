@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_08091e3c
	.thumb_func
Func_08091e3c:
	ldr	r3, [pc, #32]
	movs	r2, #184
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [pc, #28]
	movs	r4, #224
	strh	r2, [r3, #0]
	ldr	r2, [pc, #24]
	lsls	r4, r4, #1
	adds	r3, r2, r4
	strh	r0, [r3, #0]
	movs	r0, #225
	lsls	r0, r0, #1
	adds	r3, r2, r0
	strh	r1, [r3, #0]
	bx	lr
	movs	r0, r0
	.4byte 0x03001ebc
	.4byte 0x000003e7
	.4byte 0x02000240
