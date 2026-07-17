@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080b7e60
	.thumb_func
Func_080b7e60:
	push	{r5, lr}
	adds	r5, r0, #0
	bl	Func_080b6e30
	adds	r0, r5, #0
	bl	Func_080b7dd0
	movs	r3, #1
	strh	r3, [r0, #40]
	movs	r0, #0
	pop	{r5}
	pop	{r1}
	bx	r1
