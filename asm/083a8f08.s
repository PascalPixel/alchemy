@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_083a8f08
	.thumb_func
Func_083a8f08:
	push	{r0, r4, r5, lr}
	pop	{r1, r6, r7, pc}
