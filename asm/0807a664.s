@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0807a664
	.thumb_func
Func_0807a664:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r0, [pc, #132]
	ldr	r2, [pc, #132]
	ldrh	r3, [r0, #0]
	sub	sp, #4
	mov	r8, r0
	cmp	r3, r2
	bne.n	.L0
	b.n	.L1
.L0:
	mov	r1, r8
	ldr	r3, [pc, #120]
	movs	r0, #136
	strh	r2, [r1, #0]
	lsls	r0, r0, #2
	movs	r2, #2
	add	r8, r2
	adds	r2, r3, r0
	movs	r0, #0
	ldrsh	r1, [r2, r0]
	mov	r9, r1
	ldr	r1, [pc, #104]
	adds	r3, r3, r1
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	movs	r1, #0
	mov	fp, r2
	mov	sl, r1
.L9:
	mov	r0, sl
	bl	Func_08077394
	adds	r7, r0, #0
	adds	r2, r7, #0
	adds	r2, #216
	movs	r5, #14
.L2:
	ldrh	r3, [r2, #0]
	mov	r0, r8
	movs	r1, #2
	subs	r5, #1
	adds	r2, #2
	strh	r3, [r0, #0]
	add	r8, r1
	cmp	r5, #0
	bge.n	.L2
	ldr	r2, [pc, #44]
	movs	r6, #216
	movs	r5, #14
.L4:
	ldrh	r0, [r6, r7]
	str	r2, [sp, #0]
	bl	Func_08078414
	ldrb	r3, [r0, #2]
	ldr	r2, [sp, #0]
	cmp	r3, #6
	beq.n	.L3
	strh	r2, [r6, r7]
.L3:
	subs	r5, #1
	adds	r6, #2
	cmp	r5, #0
	bge.n	.L4
	adds	r0, r7, #0
	adds	r0, #216
	movs	r5, #0
	adds	r4, r0, #0
	adds	r1, r0, #0
	movs	r6, #14
	b.n	.L5
	.4byte 0x00000000
	.4byte 0x02001078
	.4byte 0x00006774
	.4byte 0x02000240
	.4byte 0x00000222
.L5:
	ldrh	r2, [r4, #0]
	lsls	r3, r2, #16
	adds	r4, #2
	cmp	r3, #0
	beq.n	.L6
	strh	r2, [r1, #0]
	adds	r5, #1
	adds	r1, #2
.L6:
	subs	r6, #1
	cmp	r6, #0
	bge.n	.L5
	cmp	r5, #14
	bgt.n	.L7
	lsls	r3, r5, #1
	adds	r0, r3, r0
	ldr	r2, [pc, #16]
	movs	r3, #15
	subs	r5, r3, r5
.L8:
	subs	r5, #1
	strh	r2, [r0, #0]
	adds	r0, #2
	cmp	r5, #0
	bne.n	.L8
	b.n	.L7
	.4byte 0x00000000
.L7:
	mov	r0, sl
	bl	Func_08079ae8
	mov	r0, sl
	bl	Func_08077428
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #3
	ble.n	.L9
	movs	r2, #2
	mov	r1, r8
	mov	r0, r9
	add	r8, r2
	strh	r0, [r1, #0]
	mov	r3, fp
	mov	r0, r8
	strh	r3, [r0, #0]
	ldr	r0, [pc, #52]
	add	r8, r2
	ldrh	r3, [r0, #0]
	mov	r1, r8
	strh	r3, [r1, #0]
	ldrh	r3, [r0, #2]
	mov	r2, r8
	movs	r0, #0
	strh	r3, [r2, #2]
	movs	r1, #16
	bl	Func_0807a628
	ldr	r0, [pc, #32]
	bl	Func_08079358
.L1:
	movs	r0, #1
	bl	Func_0807808c
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02000438
	.4byte 0x00000952
