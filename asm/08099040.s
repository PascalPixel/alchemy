@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_08099040
	.thumb_func
Func_08099040:
	push	{lr}
	cmp	r0, #0
	beq.n	.L0
	ldr	r1, [pc, #32]
	ldr	r2, [r0, #28]
	ldr	r3, [r0, #24]
	adds	r2, r2, r1
	str	r2, [r0, #28]
	movs	r2, #128
	adds	r3, r3, r1
	lsls	r2, r2, #5
	str	r3, [r0, #24]
	cmp	r3, r2
	bgt.n	.L0
	ldr	r1, [pc, #12]
	bl	Func_08009098
.L0:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0xfffff000
	.4byte 0x0809f0b0
