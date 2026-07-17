@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08092b54
	.thumb_func
Func_08092b54:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	mov	r8, r0
	adds	r0, r1, #0
	bl	Func_08092054
	ldr	r0, [r0, #80]
	ldrb	r6, [r0, #28]
	ldrh	r5, [r0, #8]
	mov	r0, r8
	bl	Func_08092054
	ldr	r0, [r0, #80]
	ldr	r3, [pc, #24]
	ldrh	r2, [r0, #8]
	lsls	r5, r5, #22
	lsrs	r5, r5, #22
	ands	r3, r2
	orrs	r3, r5
	strb	r6, [r0, #28]
	strh	r3, [r0, #8]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0xfffffc00
	bx	lr
