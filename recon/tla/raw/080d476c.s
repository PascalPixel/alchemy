.syntax unified
	.thumb
	.set sub_08020000, 0x08020000
	.set sub_08020090, 0x08020090
	.set sub_080200a8, 0x080200a8
	.set sub_080200c0, 0x080200c0
	.set sub_080cad84, 0x080cad84
	.set sub_080cccb8, 0x080cccb8
	.set sub_080d2240, 0x080d2240
	.set sub_081c0010, 0x081c0010
	.global Func_080d476c
	.thumb_func
Func_080d476c:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	ldr	r6, [r5, #104]
	cmp	r6, #0
	beq.n	.L_080d47b0
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r3, [r6, #8]
	str	r3, [r5, #8]
	adds	r3, r5, #0
	adds	r3, #102
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	sub_080cccb8
	movs	r3, #0
	ldrsh	r0, [r0, r3]
	bl	sub_08020000
	movs	r2, #8
	ldrsb	r2, [r0, r2]
	ldr	r3, [r6, #12]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r3, [r6, #20]
	str	r3, [r5, #20]
	ldr	r3, [r6, #16]
	str	r3, [r5, #16]
.L_080d47b0:
	movs	r0, #0
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r1, #0
	movs	r3, #255
	ands	r3, r7
	mov	r8, r0
	mov	sl, r2
	cmp	r3, #6
	bne.n	.L_080d47d0
	movs	r0, #110
	bl	sub_081c0010
.L_080d47d0:
	mov	r0, r8
	bl	sub_080cad84
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080d485e
	movs	r0, #244
	lsls	r0, r0, #1
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	bl	sub_080200c0
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080d4858
	ldr	r1, [pc, #72]
	bl	sub_080200a8
	movs	r1, #15
	ands	r1, r7
	adds	r0, r5, #0
	bl	sub_08020090
	adds	r3, r5, #0
	movs	r2, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	adds	r3, #15
	strh	r2, [r3, #0]
	adds	r3, #2
	mov	r2, r8
	strh	r2, [r3, #0]
	ldr	r3, [pc, #44]
	ldr	r1, [pc, #32]
	str	r3, [r5, #108]
	ldr	r0, [r5, #80]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r7
	str	r6, [r5, #104]
	strb	r1, [r0, #26]
	cmp	r3, #0
	beq.n	.L_080d4844
	ldrb	r3, [r0, #9]
	movs	r2, #13
	negs	r2, r2
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	strb	r2, [r0, #9]
	b.n	.L_080d4858
	.4byte 0x00000000
	.4byte 0x080f3310
	.2byte 0x476d
	.2byte 0x080d
.L_080d4844:
	ldr	r3, [r6, #80]
	ldrb	r1, [r0, #9]
	ldrb	r3, [r3, #9]
	movs	r2, #12
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r0, #9]
.L_080d4858:
	mov	r0, sl
	bl	sub_080d2240
.L_080d485e:
	adds	r0, r5, #0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{lr}
	cmp	r0, #0
	beq.n	.L_080d4884
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080d4884
	ldr	r0, [r0, #80]
	cmp	r0, #0
	beq.n	.L_080d4884
	ldr	r0, [r0, #40]
	cmp	r0, #0
	bne.n	.L_080d4888
.L_080d4884:
	movs	r0, #1
	b.n	.L_080d4898
.L_080d4888:
	movs	r2, #0
	ldrsh	r3, [r0, r2]
	movs	r2, #132
	lsls	r2, r2, #1
	eors	r3, r2
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
.L_080d4898:
	pop	{pc}
	.2byte 0x0000
