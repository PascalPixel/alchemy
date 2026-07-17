@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_083a227a
	.thumb_func
Func_083a227a:
	push	{r2, r3, r5, lr}
	adds	r1, #113
	pop	{r0, r2, r5, r6, pc}
