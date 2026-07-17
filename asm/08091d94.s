@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_08091d94
	.thumb_func
Func_08091d94:
	push	{r5, r6, lr}
	adds	r6, r1, #0
	adds	r5, r0, #0
	bl	Func_08077088
	movs	r0, #83
	bl	Func_080f9010
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015120
	adds	r0, r6, #0
	movs	r1, #4
	bl	Func_08015120
	ldr	r0, [pc, #12]
	movs	r1, #3
	bl	Func_08015040
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000001e
