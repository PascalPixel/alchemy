@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080288a8
	.thumb_func
Func_080288a8:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	ldr	r1, [pc, #104]
	ldr	r5, [r1, #0]
	movs	r1, #144
	adds	r1, r1, r5
	mov	lr, r1
	adds	r2, #2
	mov	r4, lr
	strh	r2, [r4, #0]
	adds	r2, r5, #0
	adds	r2, #146
	strh	r3, [r2, #0]
	adds	r3, r5, #0
	adds	r3, #148
	strh	r6, [r3, #0]
	movs	r7, #142
	adds	r7, r7, r5
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	movs	r1, #0
	sub	sp, #4
	mov	ip, r7
	cmp	r1, r3
	bge.n	.L0
	lsls	r3, r6, #3
	mov	r8, r3
	adds	r2, r5, #0
.L1:
	lsls	r3, r0, #3
	mov	r4, r8
	strh	r4, [r2, #14]
	strh	r3, [r2, #12]
	mov	r4, ip
	movs	r7, #0
	ldrsh	r3, [r4, r7]
	adds	r1, #1
	adds	r0, #3
	adds	r2, #20
	cmp	r1, r3
	blt.n	.L1
.L0:
	mov	r1, lr
	movs	r3, #2
	movs	r7, #0
	ldrsh	r2, [r1, r7]
	str	r3, [sp, #0]
	adds	r1, r6, #0
	movs	r3, #3
	bl	Func_080162d4
	str	r0, [r5, #120]
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f38
