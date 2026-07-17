@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080e6948
	.thumb_func
Func_080e6948:
	push	{r5, r6, lr}
	ldr	r3, [pc, #56]
	ldr	r5, [r3, #0]
	ldr	r3, [pc, #56]
	adds	r6, r0, #0
	adds	r5, r5, r3
	str	r6, [r5, #0]
	movs	r0, #0
	sub	sp, #16
	bl	Func_080cd594
	ldr	r3, [r5, #0]
	ldr	r2, [r3, #4]
	movs	r3, #1
	eors	r2, r3
	add	r3, sp, #12
	str	r3, [sp, #0]
	add	r3, sp, #8
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r1, #7
	movs	r3, #0
	bl	Func_080de2f8
	bl	Func_080cdbc0
	add	sp, #16
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001eec
	.4byte 0x00007828
