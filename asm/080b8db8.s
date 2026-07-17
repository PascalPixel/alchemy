@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b8db8
	.thumb_func
Func_080b8db8:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	adds	r5, r0, #0
	adds	r7, r1, #0
	mov	r8, r2
	cmp	r3, #0
	bne.n	.L0
	mov	r2, sp
	movs	r3, #0
	strb	r3, [r2, #0]
	strb	r3, [r2, #1]
	strb	r3, [r2, #2]
	strb	r3, [r2, #3]
.L0:
	adds	r0, r5, #0
	bl	Func_08077008
	adds	r6, r0, #0
	ldrh	r3, [r6, #56]
	subs	r3, r3, r7
	strh	r3, [r6, #56]
	lsls	r3, r3, #16
	cmp	r3, #0
	bge.n	.L1
	movs	r3, #0
	strh	r3, [r6, #56]
.L1:
	adds	r0, r5, #0
	bl	Func_080b7dd0
	movs	r1, #5
	ldr	r0, [r0, #0]
	bl	Func_08009080
	movs	r0, #0
	bl	Func_08015130
	bl	Func_08015118
	cmp	r5, #7
	bhi.n	.L2
	mov	r2, r8
	cmp	r2, #0
	beq.n	.L3
	ldr	r0, [pc, #152]
	bl	Func_080151c8
.L3:
	adds	r0, r7, #0
	movs	r1, #5
	bl	Func_08015120
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015120
	ldr	r0, [pc, #136]
	bl	Func_080151c8
	b.n	.L4
.L2:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L5
	ldr	r0, [pc, #124]
	bl	Func_080151c8
.L5:
	adds	r0, r7, #0
	movs	r1, #5
	bl	Func_08015120
	movs	r1, #1
	adds	r0, r5, #0
	bl	Func_08015120
	ldr	r0, [pc, #108]
	bl	Func_080151c8
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015120
.L4:
	adds	r0, r5, #0
	bl	Func_080b8ec4
	cmp	r5, #7
	bhi.n	.L6
	movs	r2, #56
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	bgt.n	.L7
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015120
	ldr	r0, [pc, #72]
	bl	Func_080151c8
	b.n	.L7
.L6:
	movs	r2, #56
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	bgt.n	.L7
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015120
	ldr	r0, [pc, #52]
	bl	Func_080151c8
.L7:
	adds	r0, r5, #0
	bl	Func_080b7dd0
	movs	r1, #1
	ldr	r0, [r0, #0]
	bl	Func_08009080
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000823
	.4byte 0x00000827
	.4byte 0x00000822
	.4byte 0x00000826
	.4byte 0x00000825
	.4byte 0x00000838
