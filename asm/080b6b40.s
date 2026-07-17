@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b6b40
	.thumb_func
Func_080b6b40:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	sl, r0
	movs	r0, #182
	adds	r6, r1, #0
	movs	r2, #6
	movs	r1, #0
	lsls	r0, r0, #1
	sub	sp, #20
	mov	r8, r1
	mov	r9, r2
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L0
	movs	r3, #3
	mov	r9, r3
.L0:
	movs	r3, #1
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L1
	add	r5, sp, #4
	adds	r0, r5, #0
	bl	Func_080b6a60
	cmp	r8, r0
	bge.n	.L1
	adds	r2, r5, #0
	adds	r5, r0, #0
.L4:
	ldrh	r7, [r2, #0]
	adds	r2, #2
	adds	r0, r7, #0
	str	r2, [sp, #0]
	bl	Func_08077008
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	ldr	r2, [sp, #0]
	cmp	r3, #0
	ble.n	.L2
	cmp	r6, #0
	beq.n	.L3
	strh	r7, [r6, #0]
	adds	r6, #2
.L3:
	movs	r3, #1
	add	r8, r3
.L2:
	subs	r5, #1
	cmp	r5, #0
	bne.n	.L4
.L1:
	movs	r3, #2
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L5
	mov	r7, r9
	movs	r5, #128
	adds	r7, #128
	cmp	r5, r7
	bge.n	.L5
.L8:
	adds	r0, r5, #0
	bl	Func_08077008
	movs	r2, #149
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L6
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	ble.n	.L6
	cmp	r6, #0
	beq.n	.L7
	strh	r5, [r6, #0]
	adds	r6, #2
.L7:
	movs	r2, #1
	add	r8, r2
.L6:
	adds	r5, #1
	cmp	r5, r7
	blt.n	.L8
.L5:
	cmp	r6, #0
	beq.n	.L9
	ldr	r3, [pc, #20]
	strh	r3, [r6, #0]
.L9:
	mov	r0, r8
	add	sp, #20
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x000000ff
