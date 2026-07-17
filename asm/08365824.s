@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08365824
	.thumb_func
Func_08365824:
	push	{r2, r4, r6, r7, lr}
	adds	r5, #213
	pop	{r0, r2, r6, pc}
