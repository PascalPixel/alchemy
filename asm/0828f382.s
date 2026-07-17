@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0828f382
	.thumb_func
Func_0828f382:
	push	{r1, r4, lr}
	ldmia	r1!, {r2, r3, r4}
	add	r1, sp, #72
	subs	r2, r5, #3
	adds	r0, r7, #3
	adds	r4, r2, #2
	pop	{r1, r4, r5, r6, pc}
