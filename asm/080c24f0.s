@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c24f0
	.thumb_func
Func_080c24f0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r1
	adds	r5, r0, #0
	bl	Func_08077008
	ldr	r3, [pc, #524]
	mov	r8, r0
	movs	r1, #166
	ldr	r0, [r3, #0]
	lsls	r1, r1, #3
	adds	r7, r0, r1
	cmp	r5, #7
	bhi.n	.L0
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L0:
	ldr	r3, [pc, #504]
	add	r3, r8
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L2
	movs	r0, #2
	negs	r0, r0
	b.n	.L1
.L2:
	movs	r1, #148
	lsls	r1, r1, #1
	add	r1, r8
	ldrh	r2, [r0, #16]
	ldrb	r3, [r1, #0]
	movs	r6, #0
	movs	r5, #0
	cmp	r2, r3
	beq.n	.L3
	adds	r4, r1, #0
	adds	r1, r0, #0
	adds	r1, #16
.L4:
	adds	r5, #1
	cmp	r5, #5
	bgt.n	.L3
	adds	r1, #2
	ldrh	r2, [r1, #0]
	ldrb	r3, [r4, #0]
	cmp	r2, r3
	bne.n	.L4
.L3:
	cmp	r5, #6
	beq.n	.L5
	adds	r6, r5, #0
.L5:
	ldrh	r3, [r0, #62]
	cmp	r3, #2
	beq.n	.L6
	ldrh	r3, [r0, #60]
	cmp	r6, r3
	bge.n	.L7
	strh	r6, [r0, #60]
.L7:
	ldr	r3, [r7, #8]
	cmp	r3, #0
	beq.n	.L8
	movs	r3, #1
	strh	r3, [r0, #62]
.L6:
	ldr	r3, [r7, #8]
.L8:
	adds	r3, #1
	str	r3, [r7, #8]
	ldr	r0, [pc, #416]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L9
	b.n	.L10
.L9:
	movs	r5, #148
	lsls	r5, r5, #1
	add	r5, r8
	ldrb	r0, [r5, #0]
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r0, r0, r2
	bl	Func_080770c8
	ldrb	r0, [r5, #0]
	bl	Func_08077198
	mov	r3, fp
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L11
	movs	r6, #76
	add	r6, sl
	ldrh	r3, [r6, #0]
	mov	r9, r6
	cmp	r3, #0
	beq.n	.L12
	movs	r6, #0
	movs	r5, #0
	b.n	.L13
.L14:
	bl	Func_08004458
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #1
	lsrs	r3, r3, #16
	adds	r3, r6, r3
	adds	r6, r3, #1
	adds	r5, #1
.L13:
	mov	r1, r8
	ldrb	r0, [r1, #15]
	movs	r1, #10
	bl	Func_080022f4
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	adds	r0, #1
	cmp	r5, r0
	blt.n	.L14
	mov	r2, r9
	ldrh	r5, [r2, #0]
	lsls	r0, r5, #1
	adds	r0, r0, r5
	movs	r1, #10
	bl	Func_080022ec
	cmp	r6, r0
	bge.n	.L15
	adds	r6, r0, #0
.L15:
	ldr	r3, [r7, #0]
	adds	r2, r6, r5
	adds	r3, r3, r2
	str	r3, [r7, #0]
.L12:
	movs	r3, #82
	add	r3, sl
	mov	r9, r3
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L16
	movs	r6, #0
	movs	r5, #0
	b.n	.L17
.L18:
	bl	Func_08004458
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	adds	r0, r6, r0
	adds	r6, r0, #1
	adds	r5, #1
.L17:
	mov	r1, r8
	ldrb	r0, [r1, #15]
	movs	r1, #10
	bl	Func_080022f4
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	adds	r0, #1
	cmp	r5, r0
	blt.n	.L18
	mov	r2, r9
	ldrh	r5, [r2, #0]
	lsls	r0, r5, #1
	adds	r0, r0, r5
	movs	r1, #10
	bl	Func_080022ec
	cmp	r6, r0
	bge.n	.L19
	adds	r6, r0, #0
.L19:
	ldr	r3, [r7, #4]
	adds	r2, r6, r5
	b.n	.L20
.L11:
	mov	r3, sl
	adds	r3, #76
	ldrh	r2, [r3, #0]
	ldr	r3, [r7, #0]
	adds	r3, r3, r2
	str	r3, [r7, #0]
	mov	r3, sl
	adds	r3, #82
	ldrh	r2, [r3, #0]
	ldr	r3, [r7, #4]
.L20:
	adds	r3, r3, r2
	str	r3, [r7, #4]
.L16:
	movs	r3, #78
	add	r3, sl
	movs	r6, #0
	ldrsh	r2, [r3, r6]
	mov	r9, r3
	cmp	r2, #0
	beq.n	.L10
	mov	r0, sl
	adds	r0, #80
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	beq.n	.L10
	ldrh	r3, [r7, #12]
	movs	r5, #0
	cmp	r3, r2
	beq.n	.L21
	adds	r1, r7, #0
	mov	r4, r9
	adds	r1, #12
.L22:
	adds	r5, #1
	cmp	r5, #3
	bgt.n	.L21
	adds	r1, #2
	ldrh	r2, [r1, #0]
	movs	r6, #0
	ldrsh	r3, [r4, r6]
	cmp	r2, r3
	bne.n	.L22
.L21:
	cmp	r5, #4
	bne.n	.L10
	mov	r2, fp
	movs	r1, #0
	ldrsh	r0, [r0, r1]
	cmp	r2, #0
	beq.n	.L23
	subs	r0, #2
.L23:
	cmp	r0, #0
	bge.n	.L24
	movs	r0, #0
.L24:
	movs	r5, #128
	lsls	r5, r5, #10
	asrs	r5, r0
	bl	Func_080771a0
	ldr	r3, [pc, #100]
	ands	r0, r3
	cmp	r5, r0
	ble.n	.L10
	movs	r6, #128
	lsls	r6, r6, #23
	movs	r3, #1
	negs	r3, r3
	mov	r8, r6
	adds	r6, r7, #0
	mov	sl, r3
	movs	r5, #0
	adds	r6, #12
.L26:
	ldrh	r0, [r6, #0]
	adds	r6, #2
	bl	Func_080c2470
	cmp	r0, r8
	bge.n	.L25
	mov	r8, r0
	mov	sl, r5
.L25:
	adds	r5, #1
	cmp	r5, #3
	ble.n	.L26
	mov	r2, r9
	movs	r1, #0
	ldrsh	r0, [r2, r1]
	bl	Func_080c2470
	cmp	r0, r8
	ble.n	.L10
	mov	r6, sl
	mov	r1, r9
	lsls	r3, r6, #1
	ldrh	r2, [r1, #0]
	adds	r3, #12
	strh	r2, [r7, r3]
.L10:
	movs	r0, #0
.L1:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e74
	.4byte 0x00000129
	.4byte 0x00000173
	.4byte 0x0000ffff
