@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0820aada
	.thumb_func
Func_0820aada:
	push	{r0, r1, r3, r4, r6, lr}
	pop	{r0, r1, r3, r4, pc}
