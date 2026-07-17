@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_086fc9a8
	.thumb_func
Func_086fc9a8:
	push	{r2, r3, r4, lr}
	strh	r0, [r4, #6]
	str	r2, [r7, #56]
	lsls	r5, r0, #17
	str	r6, [r1, #16]
	ldrsh	r1, [r1, r5]
	ldr	r4, [sp, #260]
	strh	r6, [r5, #26]
	bge.n	Func_086fca36
	strb	r5, [r2, #14]
	asrs	r7, r0, #24
	bx	ip
