@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0802b6d0
	.thumb_func
Func_0802b6d0:
	push	{lr}
	pop	{r0, r1, r3, r4, r5, r7, pc}
