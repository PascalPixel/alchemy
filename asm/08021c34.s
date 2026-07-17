@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_08021c34
	.thumb_func
Func_08021c34:
	push	{r5, lr}
	sub	sp, #4
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #6
	movs	r3, #4
	movs	r0, #0
	bl	Func_080162d4
	adds	r5, r0, #0
	adds	r1, r5, #0
	ldr	r0, [pc, #16]
	movs	r2, #0
	movs	r3, #0
	bl	Func_0801e940
	adds	r0, r5, #0
	add	sp, #4
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x08037300
