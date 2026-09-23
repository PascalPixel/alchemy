.syntax unified
	.thumb
	.set sub_0808ba1c, 0x0808ba1c
	.set sub_0808bd24, 0x0808bd24
	.set sub_0808d428, 0x0808d428
	.set sub_0808ddec, 0x0808ddec
	.global Func_0808e14c
	.thumb_func
Func_0808e14c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #124]
	ldr	r5, [pc, #124]
	movs	r1, #250
	lsls	r1, r1, #1
	adds	r5, r5, r1
	ldr	r3, [r3, #0]
	mov	r8, r0
	ldr	r0, [r5, #0]
	ldr	r6, [r3, #16]
	sub	sp, #4
	bl	sub_0808ba1c
	ldrh	r0, [r0, #6]
	mov	fp, r0
	ldr	r0, [r5, #0]
	bl	sub_0808ddec
	ldr	r3, [pc, #96]
	mov	r2, r8
	ands	r2, r3
	mov	r9, r0
	mov	r8, r2
	bl	sub_0808bd24
	movs	r3, #1
	ldr	r1, [r6, #0]
	negs	r3, r3
	mov	sl, r0
	cmp	r1, r3
	beq.n	.L_0808e224
.L_0808e196:
	movs	r3, #4
	ldrsh	r5, [r6, r3]
	movs	r3, #240
	lsls	r3, r3, #8
	ldrh	r2, [r6, #4]
	ands	r5, r3
	ldr	r3, [pc, #48]
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r7, r3, #16
	movs	r3, #15
	movs	r4, #255
	ands	r3, r1
	ands	r4, r2
	cmp	r3, #4
	bne.n	.L_0808e216
	movs	r1, #6
	ldrsh	r0, [r6, r1]
	str	r4, [sp, #0]
	bl	sub_0808d428
	ldr	r4, [sp, #0]
	cmp	r0, #0
	beq.n	.L_0808e216
	cmp	r7, #0
	beq.n	.L_0808e1f2
	mov	r2, fp
	ldr	r1, [pc, #20]
	subs	r3, r5, r2
	adds	r3, r3, r1
	b.n	.L_0808e1e8
	.4byte 0x00000800
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x000001ff
	.2byte 0x17ff
	.2byte 0x0000
.L_0808e1e8:
	lsls	r3, r3, #16
	ldr	r2, [pc, #76]
	lsrs	r3, r3, #16
	cmp	r3, r2
	bhi.n	.L_0808e216
.L_0808e1f2:
	mov	r1, r8
	ldr	r2, [r6, #0]
	ldrb	r3, [r6, #1]
	cmp	r1, #0
	beq.n	.L_0808e200
	cmp	r3, r8
	bne.n	.L_0808e216
.L_0808e200:
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0808e210
	cmp	r4, r9
	bne.n	.L_0808e216
	adds	r0, r6, #0
	b.n	.L_0808e226
.L_0808e210:
	adds	r0, r6, #0
	cmp	r4, sl
	beq.n	.L_0808e226
.L_0808e216:
	adds	r6, #12
	ldr	r3, [r6, #0]
	movs	r2, #1
	negs	r2, r2
	adds	r1, r3, #0
	cmp	r3, r2
	bne.n	.L_0808e196
.L_0808e224:
	movs	r0, #0
.L_0808e226:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00002ffe
