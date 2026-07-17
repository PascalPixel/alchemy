@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0876d95a
	.thumb_func
Func_0876d95a:
	push	{r0, r1, r5, r6, r7, lr}
	adds	r2, #174
	movs	r4, #103
	lsls	r6, r7, #8
	ldr	r7, [r3, #76]
	add	r0, sp, #248
	subs	r4, #162
	strh	r5, [r0, #36]
	lsrs	r0, r4, #26
	strb	r2, [r4, #18]
	lsrs	r0, r1, #6
	stmia	r5!, {r1, r3, r4}
	str	r7, [r6, r7]
	subs	r0, r3, #2
	subs	r7, #177
	strb	r7, [r4, #30]
	str	r4, [r0, #116]
	cmp	r6, #124
	str	r6, [sp, #260]
	movs	r7, #147
	str	r3, [sp, #972]
	b.n	Func_0876da78
	lsls	r5, r1, #28
	strh	r4, [r4, #58]
	add	r6, sp, #268
	subs	r1, #167
	stmia	r0!, {r6, r7}
	strb	r7, [r0, #1]
	pop	{r1, r4, r5, pc}
