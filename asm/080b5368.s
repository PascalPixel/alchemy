@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b5368
	.thumb_func
Func_080b5368:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #44
	str	r0, [sp, #24]
	movs	r1, #0
	movs	r0, #0
	str	r1, [sp, #20]
	bl	Func_08077168
	movs	r0, #1
	bl	Func_08077168
	movs	r0, #2
	bl	Func_08077168
	movs	r0, #3
	bl	Func_08077168
	movs	r0, #5
	bl	Func_08077168
	mov	r3, sp
	adds	r3, #28
	str	r3, [sp, #4]
	movs	r2, #0
	ldr	r5, [sp, #4]
	str	r2, [sp, #12]
	add	r3, sp, #40
	mov	ip, r5
.L0:
	str	r2, [r3, #0]
	subs	r3, #4
	cmp	r3, ip
	bge.n	.L0
	movs	r1, #0
	str	r1, [sp, #16]
	b.n	.L1
.L18:
	ldr	r2, [sp, #12]
	adds	r2, #1
	str	r2, [sp, #12]
	b.n	.L2
.L17:
	ldr	r3, [sp, #12]
	ldr	r1, [sp, #24]
	cmp	r3, r1
	beq.n	.L3
	b.n	.L2
.L3:
	mov	r0, sl
	bl	Func_08077150
	ldr	r2, [pc, #108]
	adds	r3, r5, r2
	ldrb	r1, [r3, #1]
	mov	r0, sl
	bl	Func_080771f0
	mov	r0, sl
	bl	Func_08077008
	movs	r5, #140
	adds	r1, r0, #0
	adds	r2, r1, #0
	lsls	r5, r5, #1
	movs	r4, #0
	movs	r0, #0
	adds	r2, #248
	adds	r3, r1, r5
	movs	r7, #3
.L4:
	subs	r7, #1
	strb	r4, [r3, #0]
	strb	r4, [r3, #4]
	str	r0, [r2, #0]
	str	r0, [r2, #16]
	adds	r3, #1
	adds	r2, #4
	cmp	r7, #0
	bge.n	.L4
	adds	r0, r1, #0
	ldr	r3, [pc, #48]
	movs	r7, #31
	adds	r0, #212
.L5:
	subs	r7, #1
	strh	r3, [r0, #0]
	subs	r0, #4
	cmp	r7, #0
	bge.n	.L5
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #16]
	ldr	r5, [pc, #32]
	adds	r3, r1, r2
	lsls	r3, r3, #2
	adds	r3, r5, r3
	adds	r5, r3, #0
	adds	r5, #14
	movs	r7, #1
.L7:
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L6
	adds	r1, r3, #0
	mov	r0, sl
	bl	Func_08077088
	b.n	.L6
	.4byte 0x00000000
	.4byte 0x080c3f34
.L6:
	subs	r7, #1
	adds	r5, #2
	cmp	r7, #0
	bge.n	.L7
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #16]
	adds	r3, r1, r2
	lsls	r4, r3, #2
	movs	r3, #0
	movs	r7, #0
	mov	r9, r3
.L10:
	ldr	r2, [pc, #212]
	adds	r2, #2
	movs	r5, #0
	ldrsb	r3, [r2, r4]
	mov	r8, r5
	cmp	r8, r3
	bge.n	.L8
	ldr	r6, [sp, #4]
	mov	r5, r9
	mov	fp, r2
.L9:
	ldr	r2, [r6, r5]
	adds	r1, r7, #0
	mov	r0, sl
	str	r4, [sp, #0]
	bl	Func_080771a8
	ldr	r2, [r6, r5]
	adds	r1, r7, #0
	mov	r0, sl
	bl	Func_080771b0
	ldr	r3, [r6, r5]
	ldr	r4, [sp, #0]
	adds	r3, #1
	mov	r2, fp
	str	r3, [r6, r5]
	movs	r1, #1
	ldrsb	r3, [r2, r4]
	add	r8, r1
	cmp	r8, r3
	blt.n	.L9
.L8:
	movs	r3, #4
	adds	r7, #1
	adds	r4, #1
	add	r9, r3
	cmp	r7, #3
	ble.n	.L10
	movs	r7, #15
.L11:
	mov	r0, sl
	movs	r1, #0
	subs	r7, #1
	bl	Func_08077058
	cmp	r7, #0
	bge.n	.L11
	ldr	r5, [sp, #8]
	ldr	r1, [sp, #16]
	ldr	r2, [pc, #116]
	adds	r3, r5, r1
	lsls	r3, r3, #2
	adds	r3, r2, r3
	adds	r5, r3, #6
	movs	r7, #3
.L13:
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L12
	adds	r1, r3, #0
	mov	r0, sl
	bl	Func_08077028
	adds	r1, r0, #0
	mov	r0, sl
	bl	Func_08077050
.L12:
	subs	r7, #1
	adds	r5, #2
	cmp	r7, #0
	bge.n	.L13
.L2:
	ldr	r3, [sp, #16]
	adds	r3, #1
	str	r3, [sp, #16]
.L1:
	ldr	r5, [sp, #16]
	ldr	r1, [sp, #16]
	lsls	r5, r5, #2
	str	r5, [sp, #8]
	adds	r3, r5, r1
	ldr	r2, [pc, #60]
	lsls	r5, r3, #2
	ldrsb	r2, [r2, r5]
	movs	r3, #166
	lsls	r3, r3, #1
	mov	sl, r2
	cmp	r1, r3
	bls.n	.L14
	movs	r5, #1
	str	r5, [sp, #20]
	b.n	.L15
.L14:
	movs	r1, #1
	negs	r1, r1
	cmp	sl, r1
	beq.n	.L16
	b.n	.L17
.L16:
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #24]
	cmp	r2, r3
	beq.n	.L15
	b.n	.L18
.L15:
	ldr	r0, [sp, #20]
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x080c3f34
