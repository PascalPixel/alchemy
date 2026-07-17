@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_081064ae
	.thumb_func
Func_081064ae:
	push	{r0, r1, r3, r5, r7, lr}
	stmia	r2!, {r0, r2, r3, r4, r5, r7}
	stmia	r0!, {r1, r6, r7}
	hlt	0x003d
	pop	{r1, r3, r4, r5, r7, pc}
