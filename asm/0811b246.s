@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0811b246
	.thumb_func
Func_0811b246:
	push	{r0, r3, r6, r7, lr}
	add	r4, sp, #688
	pop	{r2, r4, r5, r7, pc}
