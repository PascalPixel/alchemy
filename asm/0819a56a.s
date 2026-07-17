@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0819a56a
	.thumb_func
Func_0819a56a:
	push	{r6, r7, lr}
	pop	{r1, r2, r4}
	stmia	r0!, {r1, r2, r4}
	adds	r2, #18
	asrs	r7, r3, #27
	ldrb	r2, [r6, #26]
	adds	r5, #18
	orrs	r1, r4
	asrs	r7, r7, #21
	pop	{r0, r1, r6, r7, pc}
