@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_080173ac
	.thumb_func
Func_080173ac:
	ldr	r3, [pc, #44]
	ldr	r2, [r3, #0]
	ldr	r3, [pc, #44]
	adds	r1, r2, r3
	movs	r3, #15
	strh	r3, [r1, #0]
	ldr	r3, [pc, #40]
	adds	r1, r2, r3
	movs	r3, #10
	strh	r3, [r1, #0]
	ldr	r3, [pc, #36]
	adds	r1, r2, r3
	movs	r3, #9
	strh	r3, [r1, #0]
	ldr	r1, [pc, #32]
	movs	r0, #0
	adds	r3, r2, r1
	strh	r0, [r3, #0]
	ldr	r3, [pc, #28]
	adds	r2, r2, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	bx	lr
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x00000eae
	.4byte 0x00000ea8
	.4byte 0x000012b0
	.4byte 0x00000eac
	.4byte 0x00000eaa
