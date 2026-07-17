@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_081a2da0
	.thumb_func
Func_081a2da0:
	push	{r2, r3, r4, lr}
	pop	{r0, r4, pc}
