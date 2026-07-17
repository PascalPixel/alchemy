@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c1afc
	.thumb_func
Func_080c1afc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #0
	mov	fp, r0
	movs	r0, #128
	sub	sp, #16
	mov	r9, r2
	bl	Func_08004970
	mov	r5, sp
	movs	r3, #0
	mov	sl, r0
	adds	r0, r5, #0
	mov	r8, r3
	bl	Func_080b6a60
	adds	r7, r0, #0
	cmp	r7, #0
	ble.n	.L0
	adds	r6, r5, #0
	adds	r5, r7, #0
.L1:
	ldrh	r0, [r6, #0]
	bl	Func_08077008
	ldrb	r3, [r0, #15]
	subs	r5, #1
	adds	r6, #2
	add	r8, r3
	cmp	r5, #0
	bne.n	.L1
.L0:
	adds	r1, r7, #0
	mov	r0, r8
	bl	Func_080022ec
	mov	r8, r0
	movs	r0, #254
	lsls	r0, r0, #2
	bl	Func_080770e0
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	add	r8, r0
	mov	r7, r8
	cmp	r7, #0
	bgt.n	.L2
	movs	r2, #1
	mov	r8, r2
.L2:
	mov	r3, r8
	cmp	r3, #99
	ble.n	.L3
	movs	r7, #99
	mov	r8, r7
.L3:
	ldr	r1, [pc, #28]
	mov	r2, sl
	movs	r5, #31
.L4:
	ldrh	r3, [r2, #2]
	subs	r5, #1
	orrs	r3, r1
	strh	r3, [r2, #2]
	adds	r2, #4
	cmp	r5, #0
	bge.n	.L4
	ldr	r7, [pc, #12]
	movs	r5, #0
	movs	r6, #0
	b.n	.L5
	movs	r0, r0
	.4byte 0x0000ffff
	.4byte 0x080c73f8
.L5:
	ldrh	r0, [r6, r7]
	bl	Func_08077198
	ldrh	r0, [r6, r7]
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r0, r0, r2
	adds	r5, #1
	bl	Func_080770d0
	adds	r6, #2
	cmp	r5, #19
	bls.n	.L5
	movs	r5, #0
.L9:
	adds	r0, r5, #0
	bl	Func_080c1a34
	cmp	r0, #0
	blt.n	.L6
	mov	r3, r8
	adds	r3, #3
	cmp	r0, r3
	bgt.n	.L6
	movs	r6, #1
	ldr	r3, [pc, #132]
	negs	r6, r6
	movs	r4, #0
	mov	r1, sl
.L8:
	movs	r7, #2
	ldrsh	r2, [r1, r7]
	cmp	r2, r3
	bge.n	.L7
	adds	r3, r2, #0
	adds	r6, r4, #0
.L7:
	adds	r4, #1
	adds	r1, #4
	cmp	r4, #31
	ble.n	.L8
	cmp	r6, #0
	blt.n	.L6
	lsls	r3, r6, #2
	add	r3, sl
	movs	r2, #1
	strh	r0, [r3, #2]
	strh	r5, [r3, #0]
	add	r9, r2
.L6:
	ldr	r3, [pc, #92]
	adds	r5, #1
	cmp	r5, r3
	bls.n	.L9
	mov	r7, r9
	cmp	r7, #32
	ble.n	.L10
	movs	r2, #32
	mov	r9, r2
.L10:
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L11
	bl	Func_08004458
	mov	r3, r9
	muls	r3, r0
	lsrs	r3, r3, #16
	lsls	r3, r3, #2
	add	r3, sl
	movs	r7, #0
	ldrsh	r5, [r3, r7]
	movs	r2, #2
	ldrsh	r3, [r3, r2]
	mov	r7, r8
	subs	r3, r7, r3
	mov	r2, fp
	str	r3, [r2, #0]
	b.n	.L12
.L11:
	mov	r3, r9
	mov	r7, fp
	str	r3, [r7, #0]
	movs	r5, #1
.L12:
	mov	r0, sl
	bl	Func_08002df0
	adds	r0, r5, #0
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000003e7
	.4byte 0x0000017b
