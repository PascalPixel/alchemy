.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08014878, 0x08014878
	.global Overlay_08163c2c
Overlay_08163c2c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #136
	str	r1, [sp, #4]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #96]
	ldr	r3, [r3, #92]
	add	r2, sp, #8
	str	r3, [sp, #0]
	mov	fp, r2
	add	r3, sp, #136
	adds	r6, r0, #0
	mov	r9, r1
	movs	r7, #63
	mov	r5, fp
	mov	r8, r3
.L_08163c58:
	bl	sub_08014878
	ands	r0, r7
	strb	r0, [r5, #0]
	adds	r5, #1
	cmp	r5, r8
	bne.n	.L_08163c58
	cmp	r6, #1
	bne.n	.L_08163cec
	movs	r1, #0
	movs	r2, #1
	mov	r8, r1
	mov	sl, r2
	movs	r5, #0
.L_08163c74:
	add	r8, sl
	movs	r3, #1
	add	sl, r3
	cmp	r5, r8
	beq.n	.L_08163cce
	ldr	r1, [sp, #4]
	mov	ip, fp
	movs	r4, #7
	subs	r7, r3, r1
.L_08163c86:
	movs	r6, #0
	mov	r0, ip
.L_08163c8a:
	ldrb	r3, [r0, #0]
	adds	r0, #1
	subs	r1, r5, r3
	cmp	r1, #0
	blt.n	.L_08163cc2
	cmp	r1, #127
	bgt.n	.L_08163cc2
	adds	r2, r6, #0
	cmp	r6, #0
	bge.n	.L_08163ca0
	adds	r2, r6, #7
.L_08163ca0:
	asrs	r2, r2, #3
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_08163caa
	adds	r3, r1, #7
.L_08163caa:
	asrs	r3, r3, #3
	lsls	r2, r2, #4
	adds	r2, r2, r3
	adds	r3, r6, #0
	ands	r3, r4
	lsls	r2, r2, #3
	adds	r2, r2, r3
	ands	r1, r4
	lsls	r2, r2, #3
	adds	r2, r2, r1
	mov	r3, r9
	strb	r7, [r3, r2]
.L_08163cc2:
	adds	r6, #1
	cmp	r6, #128
	bne.n	.L_08163c8a
	adds	r5, #1
	cmp	r5, r8
	bne.n	.L_08163c86
.L_08163cce:
	ldr	r1, [sp, #0]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #232
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #128
	lsls	r1, r1, #1
	cmp	r8, r1
	ble.n	.L_08163c74
	b.n	.L_08163d78
.L_08163cec:
	cmp	r6, #3
	beq.n	.L_08163d78
	movs	r2, #0
	movs	r3, #1
	mov	r8, r2
	mov	sl, r3
	movs	r6, #0
.L_08163cfa:
	mov	r1, sl
	lsrs	r3, r1, #31
	add	r3, sl
	asrs	r3, r3, #1
	movs	r2, #4
	add	r8, r3
	add	sl, r2
	cmp	r6, r8
	beq.n	.L_08163d5e
	ldr	r1, [sp, #4]
	movs	r3, #1
	mov	ip, fp
	movs	r4, #7
	subs	r7, r3, r1
.L_08163d16:
	movs	r5, #0
	mov	r0, ip
.L_08163d1a:
	ldrb	r3, [r0, #0]
	adds	r0, #1
	subs	r1, r6, r3
	cmp	r1, #0
	blt.n	.L_08163d52
	cmp	r1, #127
	bgt.n	.L_08163d52
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_08163d30
	adds	r2, r1, #7
.L_08163d30:
	asrs	r2, r2, #3
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_08163d3a
	adds	r3, r5, #7
.L_08163d3a:
	asrs	r3, r3, #3
	lsls	r2, r2, #4
	adds	r2, r2, r3
	ands	r1, r4
	lsls	r2, r2, #3
	adds	r2, r2, r1
	adds	r3, r5, #0
	ands	r3, r4
	lsls	r2, r2, #3
	adds	r2, r2, r3
	mov	r3, r9
	strb	r7, [r3, r2]
.L_08163d52:
	adds	r5, #1
	cmp	r5, #128
	bne.n	.L_08163d1a
	adds	r6, #1
	cmp	r6, r8
	bne.n	.L_08163d16
.L_08163d5e:
	ldr	r1, [sp, #0]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #232
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	mov	r1, r8
	cmp	r1, #191
	ble.n	.L_08163cfa
.L_08163d78:
	add	sp, #136
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
