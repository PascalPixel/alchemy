.syntax unified
	.thumb
	.set sub_080140d8, 0x080140d8
	.set sub_080201c0, 0x080201c0
	.global Overlay_080d1840
Overlay_080d1840:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #144
	ldr	r3, [r3, #0]
	sub	sp, #16
	mov	fp, r3
	ldr	r3, [r2, #32]
	mov	r5, fp
	adds	r3, #228
	movs	r2, #2
	ldrsh	r1, [r3, r2]
	str	r1, [sp, #8]
	movs	r2, #6
	ldrsh	r1, [r3, r2]
	str	r1, [sp, #4]
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L_080d1876
	b.n	.L_080d1a08
.L_080d1876:
	ldr	r1, [r3, #16]
	ldr	r7, [r3, #8]
	mov	sl, r1
	movs	r1, #22
	ldrsh	r2, [r3, r1]
	adds	r3, #34
	str	r2, [sp, #0]
	movs	r2, #189
	ldrb	r3, [r3, #0]
	lsls	r2, r2, #1
	mov	r8, r3
	ldr	r3, [pc, #148]
	mov	r0, r8
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	movs	r2, #128
	lsrs	r3, r3, #5
	mov	r9, r3
	ldr	r3, [pc, #140]
	lsls	r2, r2, #13
	adds	r7, r7, r3
	add	r2, sl
	adds	r1, r7, #0
	bl	sub_080201c0
	movs	r2, #128
	lsls	r2, r2, #14
	asrs	r6, r0, #16
	add	r2, sl
	mov	r0, r8
	adds	r1, r7, #0
	bl	sub_080201c0
	asrs	r0, r0, #16
	subs	r0, #16
	cmp	r0, r6
	ble.n	.L_080d18c2
	adds	r6, r0, #0
.L_080d18c2:
	cmp	r6, #0
	ble.n	.L_080d1950
	ldr	r1, [sp, #0]
	cmp	r6, r1
	ble.n	.L_080d1950
	ldr	r3, [pc, #92]
	ldrb	r2, [r5, #9]
	str	r3, [r5, #4]
	movs	r1, #13
	movs	r3, #128
	lsls	r3, r3, #3
	negs	r1, r1
	str	r3, [r5, #8]
	adds	r3, r1, #0
	ands	r3, r2
	strb	r3, [r5, #9]
	ldr	r3, [pc, #52]
	mov	r2, r9
	ands	r2, r3
	ldrh	r0, [r5, #8]
	ldr	r3, [pc, #68]
	ands	r3, r0
	orrs	r3, r2
	strh	r3, [r5, #8]
	ldrb	r3, [r5, #5]
	asrs	r2, r7, #16
	ands	r1, r3
	movs	r3, #4
	orrs	r1, r3
	ldr	r3, [pc, #28]
	strb	r1, [r5, #5]
	ands	r2, r3
	ldr	r3, [sp, #8]
	ldrh	r1, [r5, #6]
	subs	r2, r2, r3
	ldr	r3, [pc, #20]
	adds	r0, r5, #0
	ands	r2, r3
	ldr	r3, [pc, #36]
	ands	r3, r1
	orrs	r3, r2
	mov	r1, sl
	b.n	.L_080d1938
	.4byte 0x000003ff
	.4byte 0x0000fff0
	.4byte 0x000001ff
	.4byte 0x020036e0
	.4byte 0xfff80000
	.4byte 0x40000800
	.4byte 0xfffffc00
	.2byte 0xfe00
	.2byte 0xffff
.L_080d1938:
	strh	r3, [r5, #6]
	movs	r2, #240
	asrs	r3, r1, #16
	ands	r3, r2
	ldr	r2, [sp, #4]
	movs	r1, #0
	subs	r3, r3, r2
	subs	r3, r3, r6
	adds	r3, #16
	strb	r3, [r5, #4]
	bl	sub_080140d8
.L_080d1950:
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r7, r7, r2
	adds	r1, r7, #0
	add	r2, sl
	mov	r0, r8
	bl	sub_080201c0
	movs	r2, #128
	lsls	r2, r2, #14
	asrs	r6, r0, #16
	add	r2, sl
	mov	r0, r8
	adds	r1, r7, #0
	bl	sub_080201c0
	asrs	r0, r0, #16
	mov	r5, fp
	subs	r0, #16
	adds	r5, #12
	cmp	r0, r6
	ble.n	.L_080d197e
	adds	r6, r0, #0
.L_080d197e:
	cmp	r6, #0
	ble.n	.L_080d1a08
	ldr	r3, [sp, #0]
	cmp	r6, r3
	ble.n	.L_080d1a08
	ldr	r3, [pc, #84]
	ldrb	r1, [r5, #9]
	movs	r2, #13
	str	r3, [r5, #4]
	negs	r2, r2
	movs	r3, #0
	str	r3, [r5, #8]
	adds	r3, r2, #0
	ands	r3, r1
	strb	r3, [r5, #9]
	ldr	r3, [pc, #52]
	mov	r1, r9
	ands	r1, r3
	mov	r9, r1
	ldr	r3, [pc, #60]
	ldrh	r1, [r5, #8]
	adds	r0, r5, #0
	ands	r3, r1
	mov	r1, r9
	orrs	r3, r1
	strh	r3, [r5, #8]
	ldrb	r3, [r5, #5]
	ldrh	r1, [r5, #6]
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	ldr	r3, [pc, #24]
	strb	r2, [r5, #5]
	asrs	r2, r7, #16
	ands	r2, r3
	ldr	r3, [sp, #8]
	subs	r2, r2, r3
	ldr	r3, [pc, #16]
	ands	r2, r3
	ldr	r3, [pc, #24]
	ands	r3, r1
	b.n	.L_080d19ec
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x0000fff0
	.4byte 0x000001ff
	.4byte 0x40000800
	.4byte 0xfffffc00
	.2byte 0xfe00
	.2byte 0xffff
.L_080d19ec:
	orrs	r3, r2
	mov	r1, sl
	strh	r3, [r5, #6]
	movs	r2, #240
	asrs	r3, r1, #16
	ands	r3, r2
	ldr	r2, [sp, #4]
	movs	r1, #0
	subs	r3, r3, r2
	subs	r3, r3, r6
	adds	r3, #16
	strb	r3, [r5, #4]
	bl	sub_080140d8
.L_080d1a08:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
