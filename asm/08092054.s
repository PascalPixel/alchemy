@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_08092054
	.thumb_func
Func_08092054:
	push	{lr}
	bl	Func_0808ba1c
	cmp	r0, #0
	bne.n	.L0
	movs	r0, #0
.L0:
	pop	{r1}
	bx	r1
