@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_08079c5c
	.thumb_func
Func_08079c5c:
	push	{r5, r6, lr}
	lsls	r1, r1, #1
	subs	r1, #200
	adds	r6, r0, #0
	adds	r0, r1, #0
	movs	r1, #0
	adds	r5, r2, #0
	bl	Func_08079b24
	adds	r3, r6, #0
	muls	r3, r0
	adds	r0, r5, #0
	muls	r0, r3
	cmp	r0, #0
	bge.n	.L0
	ldr	r3, [pc, #12]
	adds	r0, r0, r3
.L0:
	asrs	r0, r0, #16
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x0000ffff
