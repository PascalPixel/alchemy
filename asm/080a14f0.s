@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a14f0
	.thumb_func
Func_080a14f0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #4
	adds	r7, r0, #0
	mov	sl, r1
	adds	r6, r2, #0
	mov	r8, r3
	movs	r5, #1
	b.n	.L0
.L2:
	adds	r5, #1
.L0:
	cmp	r5, #15
	bgt.n	.L1
	movs	r1, #10
	bl	Func_080022ec
	cmp	r0, #9
	bgt.n	.L2
.L1:
	adds	r5, #1
	lsls	r3, r5, #3
	subs	r6, r6, r3
	mov	r3, r8
	str	r3, [sp, #0]
	adds	r0, r7, #0
	adds	r1, r5, #0
	mov	r2, sl
	adds	r3, r6, #0
	bl	Func_080150a8
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
