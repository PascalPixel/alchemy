@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080925e0
	.thumb_func
Func_080925e0:
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	ldr	r5, [r6, #48]
	ldr	r3, [r6, #8]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	str	r3, [r6, #56]
	ldr	r7, [r6, #52]
	ldr	r3, [r6, #16]
	adds	r3, r3, r7
	str	r3, [r6, #16]
	str	r3, [r6, #64]
	movs	r2, #128
	ldr	r3, [r6, #12]
	lsls	r2, r2, #3
	adds	r3, r3, r2
	adds	r0, r5, #0
	movs	r1, #18
	str	r3, [r6, #12]
	str	r3, [r6, #60]
	bl	Func_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #48]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L0
	adds	r3, #15
.L0:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #52]
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
