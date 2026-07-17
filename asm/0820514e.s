@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0820514e
	.thumb_func
Func_0820514e:
	push	{r0, r1, r2, r3, r4, r5, r6, r7, lr}
	cmp	r2, #23
	subs	r7, r2, #4
	ldrsh	r2, [r3, r4]
	subs	r3, r7, #3
	udf	#163
	movs	r6, #39
	str	r7, [r2, #96]
	asrs	r1, r5, #11
	adds	r7, r7, r3
	asrs	r6, r2, #30
	pop	{r2, r3, r4, r5, r6, pc}
