@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080ece7c
	.thumb_func
Func_080ece7c:
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r3, #36
	ldrsh	r1, [r5, r3]
	ldr	r0, [r5, #8]
	ldr	r3, [pc, #32]
	movs	r2, #24
	bl	Func_080b5078
	movs	r0, #29
	bl	Func_080030f8
	movs	r3, #4
	adds	r0, r5, #0
	movs	r1, #2
	str	r3, [r5, #24]
	bl	Func_080d4604
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x000c3333
