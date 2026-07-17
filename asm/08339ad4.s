@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08339ad4
	.thumb_func
Func_08339ad4:
	push	{r4, r6, lr}
	pop	{r2, r4, r6, pc}
