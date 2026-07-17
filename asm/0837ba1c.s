@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0837ba1c
	.thumb_func
Func_0837ba1c:
	push	{r1, r3, r7, lr}
	str	r1, [r3, #48]
	str	r6, [r7, #40]
	subs	r2, #8
	stmia	r6!, {r0, r1, r4, r7}
	ldr	r7, [r5, #96]
	pop	{r1, r2, r4, r6, pc}
