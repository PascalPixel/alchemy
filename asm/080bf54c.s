@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080bf54c
	.thumb_func
Func_080bf54c:
	push	{lr}
	bl	Func_08077008
	ldr	r3, [pc, #28]
	adds	r1, r0, r3
	ldrb	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L0
	adds	r3, #255
	strb	r3, [r1, #0]
	lsls	r3, r3, #24
	movs	r0, #1
	cmp	r3, #0
	beq.n	.L1
.L0:
	movs	r0, #0
.L1:
	pop	{r1}
	bx	r1
	.4byte 0x0000013f
