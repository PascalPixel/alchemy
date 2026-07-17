@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0837add8
	.thumb_func
Func_0837add8:
	push	{r4, lr}
	ldr	r6, [sp, #500]
	ldrh	r6, [r2, #4]
	pop	{r2, r5, r6, pc}
