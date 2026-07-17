@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0807a350
	.thumb_func
Func_0807a350:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	adds	r6, r1, #0
	mov	sl, r2
	bl	Func_08077394
	adds	r1, r6, #0
	adds	r5, r0, #0
	mov	r2, sl
	adds	r0, r7, #0
	bl	Func_0807a2bc
	mov	r8, r0
	cmp	r0, #0
	beq.n	.L0
	movs	r3, #142
	lsls	r3, r3, #1
	adds	r2, r6, r3
	ldrb	r3, [r5, r2]
	adds	r3, #255
	strb	r3, [r5, r2]
	movs	r3, #132
	lsls	r3, r3, #1
	lsls	r1, r6, #2
	adds	r1, r1, r3
	movs	r2, #1
	mov	r3, sl
	lsls	r2, r3
	ldr	r3, [r5, r1]
	bics	r3, r2
	str	r3, [r5, r1]
	adds	r0, r7, #0
	bl	Func_08079ae8
.L0:
	mov	r0, r8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
