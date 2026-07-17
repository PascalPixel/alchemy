@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b5b18
	.thumb_func
Func_080b5b18:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #20
	mov	r8, sp
	mov	r0, r8
	bl	Func_080b6a60
	movs	r7, #0
	mov	sl, r0
	cmp	r7, sl
	bge.n	.L0
	movs	r0, #0
	movs	r5, #0
	movs	r6, #0
	mov	r9, r0
.L2:
	mov	r1, r8
	ldrh	r0, [r6, r1]
	bl	Func_08077008
	adds	r2, r0, #0
	ldr	r0, [pc, #184]
	movs	r1, #3
	adds	r3, r2, r0
.L1:
	mov	r0, r9
	subs	r1, #1
	strb	r0, [r3, #0]
	subs	r3, #1
	cmp	r1, #0
	bge.n	.L1
	movs	r1, #153
	lsls	r1, r1, #1
	ldr	r0, [pc, #164]
	adds	r3, r2, r1
	strb	r5, [r3, #0]
	adds	r1, #2
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	adds	r0, #2
	adds	r3, r2, r1
	strb	r5, [r3, #0]
	adds	r1, #2
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	adds	r0, #2
	adds	r3, r2, r1
	strb	r5, [r3, #0]
	adds	r1, #2
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	adds	r0, #2
	adds	r3, r2, r1
	strb	r5, [r3, #0]
	adds	r1, #2
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	adds	r0, #2
	adds	r3, r2, r1
	strb	r5, [r3, #0]
	adds	r1, #2
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	adds	r0, #2
	adds	r3, r2, r1
	strb	r5, [r3, #0]
	adds	r1, #2
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	adds	r0, #2
	adds	r3, r2, r1
	strb	r5, [r3, #0]
	adds	r1, #3
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	adds	r0, #3
	adds	r3, r2, r1
	strb	r5, [r3, #0]
	adds	r1, #2
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	adds	r0, #2
	adds	r3, r2, r1
	strb	r5, [r3, #0]
	adds	r1, #2
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	adds	r0, #2
	adds	r3, r2, r1
	strb	r5, [r3, #0]
	adds	r1, #2
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	adds	r0, #2
	adds	r3, r2, r1
	strb	r5, [r3, #0]
	mov	r1, r8
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	ldrh	r0, [r6, r1]
	adds	r7, #1
	bl	Func_08077010
	adds	r6, #2
	cmp	r7, sl
	blt.n	.L2
.L0:
	add	sp, #20
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x0000012f
	.4byte 0x00000133
