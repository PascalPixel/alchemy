@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0837ea7e
	.thumb_func
Func_0837ea7e:
	push	{r1, r3, r4, r7, lr}
	asrs	r0, r2, #20
	strb	r5, [r0, #7]
	mov	r4, r8
	pop	{r0, r1, r4, r5, r7, pc}
