@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0803ff3e
	.thumb_func
Func_0803ff3e:
	push	{r0, r3, r7, lr}
	bmi.n	Func_0803ffd0
	pop	{r0, r2, r6, pc}
