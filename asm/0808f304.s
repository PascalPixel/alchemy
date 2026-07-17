@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0808f304
	.thumb_func
Func_0808f304:
	push	{lr}
	ldr	r3, [pc, #28]
	ldr	r3, [r3, #0]
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L0
	ldr	r2, [pc, #20]
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
.L0:
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001ebc
	.4byte 0x00000cb8
