@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_08003810
	.thumb_func
Func_08003810:
	push	{lr}
	ldr	r3, [pc, #24]
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L0
	ldr	r2, [pc, #20]
	ldr	r3, [pc, #20]
	strh	r2, [r3, #0]
	ldr	r2, [pc, #20]
	movs	r3, #1
	strb	r3, [r2, #0]
.L0:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02002000
	.4byte 0x0000c3ff
	.4byte 0x04000132
	.4byte 0x03001cb8
