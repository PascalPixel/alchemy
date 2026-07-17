@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08064278
	.thumb_func
Func_08064278:
	push	{r1, r2, r3, r5, lr}
	strb	r7, [r6, #12]
	strh	r1, [r1, #54]
	add	r2, sp, #808
	ldr	r2, [r6, #116]
	pop	{r0, r1, r2, r3, r4, r5, r7, pc}
