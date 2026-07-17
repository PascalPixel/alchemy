@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_08003c3c
	.thumb_func
Func_08003c3c:
	ldr	r2, [pc, #36]
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r2, [pc, #36]
	ldr	r3, [pc, #24]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #32]
	ldr	r1, [pc, #36]
	ldrb	r3, [r2, #0]
	strb	r3, [r1, #0]
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r3, [pc, #28]
	strb	r0, [r3, #0]
	ldr	r2, [pc, #28]
	ldrb	r3, [r3, #0]
	strb	r3, [r2, #0]
	b.n	.L0
	.4byte 0x0000003e
	.4byte 0x03001cd4
	.4byte 0x03001cf8
	.4byte 0x03001ca8
	.4byte 0x03001aec
	.4byte 0x03001c98
	.4byte 0x03001ac0
.L0:
	bx	lr
