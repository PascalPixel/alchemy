@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0815373e
	.thumb_func
Func_0815373e:
	push	{r1, r2, r6, r7, lr}
	strh	r5, [r2, #52]
	strh	r0, [r1, #60]
	ldrh	r0, [r1, #4]
	ldrh	r0, [r1, #4]
	ldrh	r0, [r1, #4]
	ldrh	r0, [r1, #4]
	ldrh	r0, [r1, #4]
	ldrh	r0, [r1, #4]
	ldrh	r0, [r1, #4]
	strh	r0, [r1, #60]
	strh	r0, [r1, #60]
	add	r3, sp, #584
	pop	{r1, r2, r4, r5, r7, pc}
