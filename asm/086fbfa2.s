@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_086fbfa2
	.thumb_func
Func_086fbfa2:
	push	{r2, r5, r6, r7, lr}
	bne.n	Func_086fc032
	strh	r2, [r2, #12]
	adds	r3, #196
	ldr	r4, [r1, r4]
	ldrb	r5, [r7, #9]
	strh	r4, [r2, #36]
	str	r2, [r5, #108]
	strb	r1, [r4, r6]
	cmp	r1, #211
	pop	{r3, pc}
