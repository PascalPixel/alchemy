@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_08015e8c
	.thumb_func
Func_08015e8c:
	push	{lr}
	ldr	r3, [pc, #36]
	ldr	r2, [pc, #36]
	ldr	r3, [r3, #0]
	adds	r1, r3, r2
	ldr	r0, [r1, #0]
	cmp	r0, #0
	beq.n	.L0
	ldr	r2, [r0, #0]
	cmp	r2, #0
	bne.n	.L1
	ldr	r4, [pc, #24]
	adds	r3, r3, r4
	str	r1, [r3, #0]
.L1:
	movs	r3, #0
	str	r2, [r1, #0]
	str	r3, [r0, #0]
.L0:
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x00000d98
	.4byte 0x00000d9c
