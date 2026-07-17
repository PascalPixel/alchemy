@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801b010
	.thumb_func
Func_0801b010:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #288]
	ldr	r6, [r3, #0]
	mov	sl, r0
	adds	r0, r6, #0
	sub	sp, #4
	adds	r5, r1, #0
	bl	Func_0801b36c
	movs	r2, #212
	lsls	r2, r2, #2
	adds	r2, r2, r6
	mov	r9, r0
	ldr	r0, [r2, #0]
	mov	r8, r2
	cmp	r0, #0
	bne.n	.L0
	mov	r3, sl
	cmp	r3, #6
	bne.n	.L1
	movs	r2, #238
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L2
	mov	r3, sl
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r0, #17
	movs	r1, #17
	b.n	.L3
.L2:
	mov	r3, sl
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r0, #17
	movs	r1, #0
.L3:
	movs	r3, #3
	bl	Func_080162d4
	mov	r2, r8
	str	r0, [r2, #0]
	movs	r3, #232
	lsls	r3, r3, #2
	adds	r2, r6, r3
	movs	r3, #0
	strh	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #2
	adds	r2, r6, r3
	adds	r3, #47
	strh	r3, [r2, #0]
	b.n	.L4
.L1:
	movs	r0, #9
	subs	r0, r0, r5
	movs	r3, #6
	lsrs	r0, r0, #1
	adds	r2, r5, #2
	str	r3, [sp, #0]
	adds	r0, #19
	movs	r1, #17
	movs	r3, #3
	bl	Func_080162d4
	mov	r2, r8
	str	r0, [r2, #0]
.L4:
	movs	r2, #212
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	bl	Func_08016478
	b.n	.L5
.L0:
	cmp	r5, #0
	beq.n	.L6
	ldrh	r3, [r0, #8]
	adds	r7, r5, #2
	cmp	r3, r7
	beq.n	.L6
	movs	r1, #2
	bl	Func_08016418
	movs	r0, #9
	subs	r0, r0, r5
	movs	r3, #6
	lsrs	r0, r0, #1
	str	r3, [sp, #0]
	adds	r0, #19
	movs	r3, #3
	movs	r1, #17
	adds	r2, r7, #0
	bl	Func_080162d4
	mov	r3, r8
	str	r0, [r3, #0]
.L6:
	movs	r2, #212
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	bl	Func_08016478
.L5:
	movs	r2, #229
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L7
	mov	r3, r9
	subs	r2, #68
	ldrh	r0, [r3, #32]
	adds	r3, r6, r2
	ldr	r1, [r3, #0]
	b.n	.L8
.L7:
	mov	r3, sl
	cmp	r3, #2
	beq.n	.L9
	cmp	r3, #4
	bne.n	.L10
	movs	r2, #212
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r1, [r3, #0]
	ldr	r0, [pc, #48]
.L8:
	movs	r2, #0
	movs	r3, #0
	bl	Func_0801e7c0
	b.n	.L10
.L9:
	movs	r2, #212
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r1, [r3, #0]
	ldr	r0, [pc, #32]
	movs	r2, #0
	movs	r3, #0
	bl	Func_0801e7c0
.L10:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e98
	.4byte 0x00000051
	.4byte 0x00000050
