@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080915ec
	.thumb_func
Func_080915ec:
	push	{lr}
	ldr	r3, [pc, #64]
	ldr	r1, [r3, #0]
	ldr	r3, [pc, #64]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L0
	ldr	r0, [pc, #60]
	movs	r2, #128
	ldr	r3, [r0, #0]
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	movs	r3, #230
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #0
	str	r3, [r2, #0]
.L1:
	ldr	r3, [r0, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	movs	r3, #230
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #1
	negs	r3, r3
	str	r3, [r2, #0]
.L0:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ebc
	.4byte 0x03001f54
	.4byte 0x03001c94
