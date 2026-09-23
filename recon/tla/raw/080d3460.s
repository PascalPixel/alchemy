.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_080148e8, 0x080148e8
	.set sub_08020090, 0x08020090
	.set sub_08020148, 0x08020148
	.set sub_08020150, 0x08020150
	.set sub_08020218, 0x08020218
	.set sub_080cad84, 0x080cad84
	.set sub_080d2240, 0x080d2240
	.set sub_080d2c98, 0x080d2c98
	.set sub_080d2d84, 0x080d2d84
	.set sub_080d3378, 0x080d3378
	.set sub_080d3838, 0x080d3838
	.set sub_081c0010, 0x081c0010
	.global Func_080d3460
	.thumb_func
Func_080d3460:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r8, r2
	sub	sp, #4
	mov	sl, r0
	adds	r5, r1, #0
	bl	sub_080d2d84
	adds	r6, r0, #0
	ldr	r2, [r6, #16]
	ldr	r3, [r6, #8]
	mov	fp, r2
	ldr	r2, [pc, #272]
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r2, r2, r3
	mov	r9, r2
	cmp	r6, #0
	beq.n	.L_080d3584
	ldr	r3, [r6, #80]
	adds	r1, r5, #0
	ldrb	r3, [r3, #26]
	adds	r7, r6, #0
	str	r3, [sp, #0]
	bl	sub_08020090
	movs	r0, #10
	bl	sub_08013560
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08020090
	adds	r7, #85
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_08020218
	ldrb	r2, [r7, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r7, #0]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r6, #40]
	movs	r3, #192
	lsls	r3, r3, #12
	ldr	r2, [r6, #12]
	add	r3, fp
	mov	r1, r9
	adds	r0, r6, #0
	bl	sub_08020148
	movs	r0, #6
	bl	sub_08013560
	movs	r0, #217
	bl	sub_081c0010
	movs	r1, #160
	movs	r5, #0
	lsls	r1, r1, #7
	mov	r0, sl
	movs	r2, #0
	bl	sub_080d3838
	strb	r5, [r7, #0]
.L_080d34f2:
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #160]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r6, #12]
	str	r3, [r6, #60]
	bl	sub_08013560
	movs	r3, #1
	negs	r3, r3
	cmp	r8, r3
	beq.n	.L_080d351a
	movs	r3, #1
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_080d351a
	adds	r0, r6, #0
	mov	r1, r8
	bl	sub_080d3378
.L_080d351a:
	adds	r5, #1
	cmp	r5, #13
	bls.n	.L_080d34f2
	movs	r3, #3
	strb	r3, [r7, #0]
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r6, #40]
	movs	r3, #128
	lsls	r3, r3, #13
	ldr	r2, [r6, #12]
	add	r3, fp
	adds	r0, r6, #0
	mov	r1, r9
	bl	sub_08020148
	adds	r0, r6, #0
	bl	sub_08020150
	movs	r3, #128
	lsls	r3, r3, #5
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d3554
	movs	r0, #4
	bl	sub_08013560
	b.n	.L_080d3584
.L_080d3554:
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #20]
	movs	r5, #0
	cmp	r2, r3
	ble.n	.L_080d3572
.L_080d355e:
	movs	r0, #1
	adds	r5, #1
	bl	sub_08013560
	cmp	r5, #179
	bhi.n	.L_080d3572
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #20]
	cmp	r2, r3
	bgt.n	.L_080d355e
.L_080d3572:
	adds	r0, r6, #0
	ldr	r1, [sp, #0]
	bl	sub_08020218
	movs	r0, #2
	bl	sub_08013560
	bl	sub_080d2c98
.L_080d3584:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xfff00000
	.2byte 0x0000
	.2byte 0xfffe
	.2byte 0xb5e0
	adds	r5, r1, #0
	adds	r7, r2, #0
	bl	sub_080cad84
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_080cad84
	adds	r2, r0, #0
	cmp	r6, #0
	beq.n	.L_080d35d0
	cmp	r2, #0
	beq.n	.L_080d35d0
	ldr	r3, [r6, #16]
	ldr	r0, [r2, #16]
	ldr	r1, [r2, #8]
	subs	r0, r0, r3
	ldr	r3, [r6, #8]
	subs	r1, r1, r3
	bl	sub_080148e8
	strh	r0, [r6, #6]
	adds	r0, r7, #0
	bl	sub_080d2240
.L_080d35d0:
	pop	{r5, r6, r7, pc}
	.align 2, 0
