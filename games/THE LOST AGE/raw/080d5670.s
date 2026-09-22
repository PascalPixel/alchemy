.syntax unified
	.thumb
	.set sub_08020090, 0x08020090
	.set sub_08020148, 0x08020148
	.set sub_08020150, 0x08020150
	.set sub_080cad84, 0x080cad84
	.set sub_081c0010, 0x081c0010
	.global Overlay_080d5670
Overlay_080d5670:
.L_080d5670:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	movs	r2, #0
	str	r2, [sp, #4]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #156
	ldr	r3, [r3, #0]
	mov	sl, r0
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_080d570c
	movs	r2, #134
	lsls	r2, r2, #1
	adds	r7, r3, r2
	movs	r3, #128
	ldr	r2, [pc, #124]
	lsls	r3, r3, #21
	str	r3, [sp, #0]
	movs	r3, #127
	mov	fp, r2
	mov	r9, r3
.L_080d56a8:
	movs	r3, #18
	ldrsb	r3, [r7, r3]
	cmp	r3, #1
	bne.n	.L_080d56fc
	mov	r2, sl
	ldr	r1, [r2, #0]
	ldr	r3, [r7, #4]
	ldr	r5, [r2, #4]
	subs	r1, r1, r3
	ldr	r3, [r7, #8]
	ldr	r6, [r2, #8]
	subs	r5, r5, r3
	ldr	r3, [r7, #12]
	asrs	r1, r1, #8
	adds	r0, r1, #0
	subs	r6, r6, r3
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x122d
	adds	r1, r5, #0
	mov	r8, r0
	adds	r0, r5, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x1236
	adds	r5, r0, #0
	adds	r1, r6, #0
	adds	r0, r6, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x44a8
	mov	r2, r8
	adds	r3, r2, r0
	movs	r2, #128
	lsls	r2, r2, #3
	cmp	r3, r2
	bgt.n	.L_080d56fc
	ldr	r2, [sp, #0]
	cmp	r2, r3
	ble.n	.L_080d56fc
	str	r3, [sp, #0]
	str	r7, [sp, #4]
.L_080d56fc:
	movs	r3, #1
	negs	r3, r3
	add	r9, r3
	mov	r2, r9
	adds	r7, #32
	cmp	r2, #0
	bge.n	.L_080d56a8
	ldr	r0, [sp, #4]
.L_080d570c:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #176]
	movs	r3, #133
	mov	r8, r2
	lsls	r3, r3, #2
	add	r3, r8
	ldr	r0, [r3, #0]
	bl	sub_080cad84
	adds	r5, r0, #0
	adds	r0, #8
	ldr	r7, [r5, #80]
	bl	.L_080d5670
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #194
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r6, r0, #0
	movs	r2, #0
	strh	r2, [r3, #0]
	movs	r0, #0
	cmp	r6, #0
	beq.n	.L_080d57dc
	movs	r0, #152
	bl	sub_081c0010
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #52]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	adds	r0, r5, #0
	movs	r1, #2
	str	r6, [r5, #104]
	bl	sub_08020090
	ldr	r1, [r6, #4]
	ldr	r2, [r6, #8]
	ldr	r3, [r6, #12]
	adds	r0, r5, #0
	bl	sub_08020148
	adds	r0, r5, #0
	bl	sub_08020150
	adds	r3, r5, #0
	adds	r3, #85
	ldrb	r2, [r3, #0]
	adds	r3, #15
	strh	r2, [r3, #0]
	ldrb	r1, [r7, #9]
	ldrb	r3, [r7, #26]
	adds	r2, r5, #0
	adds	r2, #102
	strh	r3, [r2, #0]
	lsls	r3, r1, #28
	lsrs	r3, r3, #30
	subs	r2, #4
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	ldr	r3, [pc, #28]
	movs	r2, #4
	strb	r3, [r7, #26]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #18
	strb	r3, [r7, #9]
	add	r2, r8
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r0, #1
	b.n	.L_080d57dc
	.4byte 0x00000000
	.2byte 0x0240
	.2byte 0x0200
.L_080d57dc:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
