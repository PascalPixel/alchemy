@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080fb690
	.thumb_func
Func_080fb690:
	push	{lr}
	ldr	r2, [pc, #12]
	ldr	r2, [r2, #0]
	bl	Func_080072ec
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02004000
