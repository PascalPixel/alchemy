@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0800f9cc
	.thumb_func
Func_0800f9cc:
	push	{lr}
	movs	r3, #1
	subs	r1, #1
	negs	r3, r3
	ldr	r2, [pc, #24]
	cmp	r1, r3
	beq.n	.L0
	mov	ip, r3
.L1:
	ldrb	r3, [r0, #0]
	ldrb	r3, [r2, r3]
	subs	r1, #1
	strb	r3, [r0, #0]
	adds	r0, #1
	cmp	r1, ip
	bne.n	.L1
.L0:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080097b8
