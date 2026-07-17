@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_086b4430
	.thumb_func
Func_086b4430:
	push	{r1, r3, r4, r5, r7, lr}
	lsls	r1, r3, #9
	str	r6, [r7, #44]
	strh	r2, [r4, #8]
	strb	r7, [r3, #12]
	adds	r3, r1, r4
	ldr	r2, [r6, #56]
	ldrh	r5, [r5, #4]
	asrs	r0, r2
	pop	{r0, r1, r3, r4, r5, r6, pc}
