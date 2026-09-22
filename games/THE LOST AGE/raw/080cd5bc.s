.syntax unified
	.thumb
	.set sub_080ccd78, 0x080ccd78
	.set sub_080cdea8, 0x080cdea8
	.set sub_080d5e50, 0x080d5e50
	.set sub_080d5e64, 0x080d5e64
	.set sub_080d5fd4, 0x080d5fd4
	.set sub_081c0010, 0x081c0010
	.global Overlay_080cd5bc
Overlay_080cd5bc:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	adds	r1, r6, #0
	movs	r0, #2
	bl	sub_080ccd78
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r7, #1
	adds	r5, r0, #0
	negs	r7, r7
	mov	r8, r3
	cmp	r5, #0
	beq.n	.L_080cd614
	ldr	r4, [r5, #8]
	cmp	r4, #0
	beq.n	.L_080cd614
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cd5fa
	subs	r2, #124
	add	r2, r8
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r4, [r5, #8]
.L_080cd5fa:
	movs	r3, #128
	adds	r1, r4, #0
	lsls	r3, r3, #9
	cmp	r1, r3
	bge.n	.L_080cd60c
	adds	r0, r7, #0
	bl	sub_080cdea8
	b.n	.L_080cd674
.L_080cd60c:
	adds	r0, r6, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe02f
.L_080cd614:
	movs	r0, #1
	adds	r1, r6, #0
	bl	sub_080ccd78
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080cd676
	ldr	r2, [r5, #0]
	movs	r3, #112
	ands	r2, r3
	cmp	r2, #48
	beq.n	.L_080cd654
	cmp	r2, #48
	bgt.n	.L_080cd63a
	cmp	r2, #0
	beq.n	.L_080cd640
	cmp	r2, #32
	beq.n	.L_080cd648
	b.n	.L_080cd66a
.L_080cd63a:
	cmp	r2, #80
	beq.n	.L_080cd660
	b.n	.L_080cd66a
.L_080cd640:
	movs	r0, #123
	bl	sub_081c0010
	b.n	.L_080cd66a
.L_080cd648:
	movs	r0, #128
	bl	sub_081c0010
	bl	sub_080d5e50
	b.n	.L_080cd66a
.L_080cd654:
	movs	r0, #129
	bl	sub_081c0010
	bl	sub_080d5e64
	b.n	.L_080cd66a
.L_080cd660:
	movs	r0, #123
	bl	sub_081c0010
	bl	sub_080d5fd4
.L_080cd66a:
	ldr	r2, [r5, #8]
	movs	r3, #172
	lsls	r3, r3, #1
	add	r3, r8
	strh	r2, [r3, #0]
.L_080cd674:
	movs	r7, #0
.L_080cd676:
	adds	r0, r7, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
