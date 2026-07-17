@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b9724
	.thumb_func
Func_080b9724:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #332]
	ldr	r3, [r3, #0]
	sub	sp, #20
	mov	r9, r3
	add	r3, sp, #4
	add	r2, sp, #8
	mov	r8, r3
	mov	r7, sp
	str	r0, [r2, #0]
	movs	r3, #0
	mov	r0, r8
	str	r1, [r7, #0]
	str	r3, [r0, #0]
	ldr	r0, [r7, #0]
	lsls	r0, r0, #4
	movs	r1, #20
	adds	r0, #19
	mov	fp, r2
	bl	Func_080022f4
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #2
	movs	r0, #40
	str	r3, [sp, #12]
	bl	Func_08004970
	ldr	r3, [r7, #0]
	add	r5, sp, #16
	mov	sl, r5
	str	r0, [r5, #0]
	cmp	r3, #0
	ble.n	.L0
	mov	r2, fp
	mov	r6, r9
	ldr	r1, [r2, #0]
	movs	r0, #1
	adds	r6, #80
	adds	r4, r3, #0
.L4:
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	mov	r2, r9
	adds	r3, #72
	ldrb	r3, [r2, r3]
	strh	r3, [r1, #2]
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L1
	ldrh	r2, [r1, #4]
	adds	r3, r0, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L2
	adds	r3, r2, #1
	b.n	.L3
.L1:
	ldrh	r2, [r1, #4]
	adds	r3, r0, #0
	orrs	r3, r2
.L3:
	strh	r3, [r1, #4]
.L2:
	subs	r4, #1
	adds	r1, #16
	cmp	r4, #0
	bne.n	.L4
.L0:
	mov	r3, r9
	adds	r3, #82
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L5
	mov	r3, r9
	adds	r3, #80
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L6
	mov	r3, sl
	ldr	r2, [r3, #0]
	ldr	r3, [r7, #0]
	str	r3, [r2, #0]
	bl	Func_080771a0
	mov	r1, sl
	ldr	r3, [r1, #0]
	str	r0, [r3, #4]
	ldr	r2, [pc, #172]
	ldrh	r1, [r2, #0]
	strh	r2, [r2, #0]
	ldr	r4, [pc, #168]
	ldr	r3, [pc, #172]
	add	r5, sp, #16
	ldr	r3, [r3, #0]
	ldr	r0, [r5, #0]
	str	r3, [r0, #8]
	str	r3, [r4, #0]
	strh	r1, [r2, #0]
	add	r2, sp, #20
	mov	r9, r2
	bl	Func_080b9554
	cmp	r0, #0
	blt.n	.L5
	add	r3, sp, #20
	mov	r9, r3
	bl	Func_080b9604
	cmp	r0, #0
	blt.n	.L5
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #0]
	mov	r0, r8
	str	r3, [r0, #0]
	b.n	.L7
.L6:
	add	r1, sp, #20
	mov	r9, r1
	bl	Func_080b9604
	cmp	r0, #0
	blt.n	.L5
	mov	r3, sl
	ldr	r2, [r3, #0]
	ldr	r3, [r2, #0]
	mov	r0, r8
	str	r3, [r0, #0]
	ldr	r3, [r7, #0]
	add	r1, sp, #20
	str	r3, [r2, #0]
	mov	r9, r1
	bl	Func_080b9554
	cmp	r0, #0
	blt.n	.L5
	bl	Func_080771a0
	mov	r2, sl
	ldr	r1, [r2, #0]
	ldr	r3, [r1, #4]
	cmp	r0, r3
	bne.n	.L5
	ldr	r2, [pc, #68]
	ldr	r3, [r1, #8]
	str	r3, [r2, #0]
.L7:
	mov	r3, r8
	ldr	r1, [r3, #0]
	cmp	r1, #0
	ble.n	.L8
	mov	r0, fp
	ldr	r3, [r7, #0]
	ldr	r2, [r0, #0]
	lsls	r3, r3, #4
	ldr	r6, [pc, #32]
	adds	r0, r3, r2
	adds	r4, r1, #0
.L9:
	ldrh	r3, [r0, #2]
	strh	r3, [r0, #0]
	ldrh	r3, [r0, #10]
	subs	r4, #1
	eors	r3, r6
	strh	r3, [r0, #10]
	adds	r0, #16
	cmp	r4, #0
	bne.n	.L9
.L8:
	ldr	r0, [r5, #0]
	bl	Func_08002df0
	mov	r1, r8
	ldr	r0, [r1, #0]
	b.n	.L10
	.4byte 0x00000080
	.4byte 0x03001e74
	.4byte 0x04000208
	.4byte 0x020023a8
	.4byte 0x03001cb4
.L5:
	bl	Func_0800651c
	bl	Func_08006358
	ldr	r0, [r5, #0]
	bl	Func_08002df0
	movs	r0, #1
	negs	r0, r0
.L10:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
