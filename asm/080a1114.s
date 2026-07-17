@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080a1114
	.thumb_func
Func_080a1114:
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L0
	bl	Func_08015018
	movs	r3, #0
	str	r3, [r5, #0]
.L0:
	pop	{r5}
	pop	{r0}
	bx	r0
