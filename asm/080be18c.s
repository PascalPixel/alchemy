@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080be18c
	.thumb_func
Func_080be18c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	add	r3, sp, #24
	mov	r1, r9
	str	r1, [r3, #0]
	mov	sl, r1
	bl	Func_08077080
	movs	r3, #0
	ldrb	r2, [r0, #0]
	str	r3, [sp, #20]
	ldrb	r3, [r0, #3]
	cmp	r3, #5
	beq.n	.L0
	cmp	r3, #5
	blt.n	.L1
	cmp	r3, #57
	bgt.n	.L1
	cmp	r3, #56
	blt.n	.L1
.L0:
	movs	r0, #1
	str	r0, [sp, #20]
.L1:
	cmp	r2, #0
	beq.n	.L2
	cmp	r2, #4
	beq.n	.L3
	movs	r2, #12
	negs	r2, r2
	movs	r1, #0
	add	r2, sl
	mov	r9, r1
	movs	r6, #0
	mov	fp, r2
	b.n	.L4
.L2:
	mov	r3, sl
	subs	r3, #4
	ldr	r3, [r3, #0]
	movs	r1, #1
	strb	r2, [r3, #16]
	strb	r1, [r3, #1]
	mov	r2, sl
	subs	r2, #8
	ldr	r2, [r2, #0]
	strb	r1, [r3, #30]
	strb	r2, [r3, #2]
	b.n	.L5
.L3:
	mov	r3, sl
	subs	r3, #4
	ldr	r2, [r3, #0]
	movs	r1, #1
	movs	r3, #0
	strb	r3, [r2, #16]
	strb	r1, [r2, #1]
	mov	r3, sl
	subs	r3, #8
	ldr	r3, [r3, #0]
	strb	r1, [r2, #30]
	strb	r3, [r2, #2]
	b.n	.L5
.L6:
	adds	r6, #1
.L4:
	mov	r3, fp
	ldr	r1, [r3, #0]
	lsls	r3, r6, #1
	adds	r3, #88
	ldrsh	r3, [r1, r3]
	cmp	r3, #255
	bne.n	.L6
	str	r6, [sp, #16]
	movs	r3, #100
	adds	r2, r1, #2
	ldrsh	r3, [r2, r3]
	movs	r6, #0
	cmp	r3, #255
	beq.n	.L7
	adds	r2, #100
.L8:
	adds	r2, #2
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	adds	r6, #1
	cmp	r3, #255
	bne.n	.L8
.L7:
	mov	r2, sl
	subs	r2, #16
	str	r6, [sp, #12]
	str	r2, [sp, #8]
	ldr	r2, [r2, #0]
	ldrh	r3, [r2, #10]
	movs	r4, #15
	ands	r4, r3
	movs	r0, #12
	ldrsh	r3, [r2, r0]
	subs	r2, r4, r3
	adds	r3, r4, r3
	subs	r3, #1
	adds	r6, r2, #1
	str	r3, [sp, #4]
	cmp	r6, r3
	bgt.n	.L9
	movs	r1, #4
	negs	r1, r1
	lsls	r3, r6, #1
	add	r1, sl
	adds	r7, r3, #0
	mov	r8, r1
	adds	r7, #100
.L14:
	cmp	r6, #0
	blt.n	.L10
	ldr	r2, [sp, #8]
	ldr	r3, [r2, #0]
	ldrh	r2, [r3, #10]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L11
	ldr	r3, [sp, #12]
	cmp	r6, r3
	bge.n	.L10
	mov	r0, fp
	ldr	r3, [r0, #0]
	adds	r3, #2
	ldrsh	r5, [r3, r7]
	cmp	r5, #254
	beq.n	.L10
	ldr	r2, [sp, #20]
	cmp	r2, #0
	bne.n	.L12
	adds	r0, r5, #0
	str	r4, [sp, #0]
	bl	Func_08077008
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	ldr	r4, [sp, #0]
	cmp	r3, #0
	beq.n	.L10
.L12:
	mov	r2, r8
	ldr	r0, [r2, #0]
	mov	r2, r9
	adds	r1, r0, #2
	adds	r2, #28
	movs	r3, #1
	strb	r3, [r1, r2]
	subs	r3, r6, r4
	subs	r2, #12
	strb	r3, [r0, r2]
	mov	r3, r9
	movs	r0, #1
	strb	r5, [r1, r3]
	add	r9, r0
	b.n	.L10
.L11:
	ldr	r1, [sp, #16]
	cmp	r6, r1
	bge.n	.L10
	mov	r3, fp
	ldr	r2, [r3, #0]
	lsls	r3, r6, #1
	adds	r3, #88
	ldrsh	r5, [r2, r3]
	cmp	r5, #254
	beq.n	.L10
	ldr	r1, [sp, #20]
	cmp	r1, #0
	bne.n	.L13
	adds	r0, r5, #0
	str	r4, [sp, #0]
	bl	Func_08077008
	movs	r2, #56
	ldrsh	r3, [r0, r2]
	ldr	r4, [sp, #0]
	cmp	r3, #0
	beq.n	.L10
.L13:
	mov	r3, r8
	ldr	r0, [r3, #0]
	mov	r2, r9
	adds	r1, r0, #2
	adds	r2, #28
	movs	r3, #1
	strb	r3, [r1, r2]
	subs	r2, #12
	subs	r3, r6, r4
	strb	r3, [r0, r2]
	mov	r0, r9
	strb	r5, [r1, r0]
	movs	r1, #1
	add	r9, r1
.L10:
	ldr	r2, [sp, #4]
	adds	r6, #1
	adds	r7, #2
	cmp	r6, r2
	ble.n	.L14
	b.n	.L15
.L9:
	movs	r3, #4
	negs	r3, r3
	add	r3, sl
	mov	r8, r3
.L15:
	mov	r0, r8
	ldr	r3, [r0, #0]
	mov	r1, r9
	mov	r2, r9
	strb	r1, [r3, #1]
	cmp	r2, #0
	bgt.n	.L5
	ldr	r0, [sp, #8]
	ldr	r3, [r0, #0]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	Func_08015120
	ldr	r0, [pc, #48]
	bl	Func_080151c8
	mov	r3, sl
	subs	r3, #20
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #40]
	adds	r2, r3, r0
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	bne.n	.L16
	movs	r3, #1
	strb	r3, [r2, #0]
.L16:
	movs	r0, #1
	negs	r0, r0
.L5:
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00000816
	.4byte 0x0000012b
