@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08317742
	.thumb_func
Func_08317742:
	push	{r2, r4, r5, r7, lr}
	asrs	r0, r2, #2
	ldrsh	r6, [r3, r6]
	lsrs	r6, r2, #1
	cmp	r1, #12
	subs	r2, r4, #6
	strb	r2, [r7, r0]
	ldrsb	r3, [r0, r6]
	lsls	r3, r1, #12
	stmia	r0!, {r2, r7}
	add	r7, pc, #972
	subs	r3, #129
	pop	{r1, r2, r3, r4, r5, r6, pc}
