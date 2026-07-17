@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080b10cc
	.thumb_func
Func_080b10cc:
	push	{r5, lr}
	ldr	r3, [pc, #48]
	ldr	r3, [r3, #0]
	ldr	r5, [r3, #12]
	sub	sp, #4
	cmp	r5, #0
	beq.n	.L0
	ldr	r0, [pc, #40]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
	ldr	r3, [pc, #32]
	ldr	r0, [r3, #16]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #6
	adds	r2, r5, #0
	movs	r3, #32
	bl	Func_080150b0
.L0:
	add	sp, #4
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
	.4byte 0x00000c8a
	.4byte 0x02000240
