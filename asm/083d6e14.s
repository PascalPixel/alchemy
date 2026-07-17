@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_083d6e14
	.thumb_func
Func_083d6e14:
	push	{r0, r4, r5, lr}
	lsrs	r1, r4, #22
	lsls	r2, r2, #18
	asrs	r0, r6, #20
	bge.n	Func_083d6d68
	stmia	r1!, {r0, r1, r2, r3, r5, r7}
	adds	r0, #45
	ldrsh	r1, [r0, r0]
	bcs.n	Func_083d6e9a
	lsls	r4, r7, #5
	lsls	r1, r6, #4
	lsls	r1, r4, #17
	pop	{r1, r5, r6, pc}
