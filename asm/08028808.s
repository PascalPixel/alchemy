@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08028808
	.thumb_func
Func_08028808:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #144]
	ldr	r7, [r3, #0]
	movs	r3, #144
	adds	r3, r3, r7
	mov	r8, r3
	adds	r3, r7, #0
	mov	sl, r0
	adds	r1, #2
	mov	r5, r8
	adds	r3, #146
	strh	r1, [r5, #0]
	mov	r6, sl
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r6, [r3, #0]
	movs	r1, #142
	adds	r1, r1, r7
	movs	r2, #0
	ldrsh	r6, [r1, r2]
	mov	r9, r1
	mov	r1, r8
	movs	r3, #0
	ldrsh	r0, [r1, r3]
	movs	r1, #3
	lsls	r0, r0, #1
	sub	sp, #4
	bl	Func_080022ec
	lsls	r5, r6, #1
	adds	r5, r5, r6
	adds	r5, r5, r0
	lsrs	r3, r5, #31
	adds	r5, r5, r3
	asrs	r5, r5, #1
	movs	r3, #15
	movs	r1, #0
	subs	r0, r3, r5
	cmp	r1, r6
	bge.n	.L0
	mov	r2, sl
	lsls	r4, r2, #3
	mov	ip, r9
	adds	r2, r7, #0
.L1:
	lsls	r3, r0, #3
	strh	r3, [r2, #12]
	strh	r4, [r2, #14]
	mov	r6, ip
	movs	r5, #0
	ldrsh	r3, [r6, r5]
	adds	r1, #1
	adds	r0, #3
	adds	r2, #20
	cmp	r1, r3
	blt.n	.L1
.L0:
	mov	r3, r8
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	movs	r3, #2
	str	r3, [sp, #0]
	mov	r1, sl
	movs	r3, #3
	bl	Func_080162d4
	str	r0, [r7, #120]
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f38
