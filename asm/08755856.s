@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08755856
	.thumb_func
Func_08755856:
	push	{r0, r1, r3, r4, r5, r6, r7, lr}
	ldrb	r7, [r6, #27]
	pop	{r0, r2, r4, r5, r7, pc}
