@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08340e6a
	.thumb_func
Func_08340e6a:
	push	{r2, r4, r5, r6, r7, lr}
	ldrsh	r4, [r6, r0]
	stmia	r5!, {r0, r1, r3, r4, r5}
	stmia	r5!, {r2, r4, r5, r7}
	rors	r1, r3
	stmia	r7!, {r2, r4, r7}
	strh	r2, [r1, #0]
	strh	r6, [r4, r1]
	pop	{r0, r2, r3, pc}
