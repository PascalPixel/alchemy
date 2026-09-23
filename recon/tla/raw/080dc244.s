.syntax unified
	.thumb
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016d18, 0x08016d18
	.set sub_080d170c, 0x080d170c
	.set sub_080d17ac, 0x080d17ac
	.set sub_080dc1b0, 0x080dc1b0
	.global Field_BeginPaletteTransition
	.global Func_080dc244
	.thumb_func
Field_BeginPaletteTransition:
Func_080dc244:
.L_080dc244:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #34
	adds	r6, r5, r3
	adds	r7, r0, #0
	movs	r1, #2
	adds	r0, r6, #0
	bl	sub_080d170c
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #164
	adds	r5, r5, r3
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	beq.n	.L_080dc27c
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #1
	movs	r1, #1
	bl	sub_080d170c
	b.n	.L_080dc284
.L_080dc27c:
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_080d170c
.L_080dc284:
	movs	r0, #132
	lsls	r0, r0, #1
	bl	sub_08016d18
	adds	r0, r7, #0
	bl	sub_080d17ac
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r2, [r3, #0]
	ldr	r3, [pc, #212]
	ldr	r5, [r3, #0]
	movs	r3, #7
	ands	r5, r3
	ldrh	r3, [r2, #30]
	movs	r1, #30
	ldrsh	r2, [r2, r1]
	cmp	r2, #18
	bne.n	.L_080dc2b2
	movs	r5, #0
.L_080dc2b2:
	cmp	r2, #26
	bne.n	.L_080dc2b8
	movs	r5, #2
.L_080dc2b8:
	cmp	r2, #23
	bne.n	.L_080dc2be
	movs	r5, #7
.L_080dc2be:
	cmp	r2, #25
	bne.n	.L_080dc2c4
	movs	r5, #0
.L_080dc2c4:
	cmp	r2, #20
	bne.n	.L_080dc2ca
	movs	r5, #5
.L_080dc2ca:
	cmp	r2, #29
	bne.n	.L_080dc2d0
	movs	r5, #0
.L_080dc2d0:
	cmp	r2, #22
	bne.n	.L_080dc2d6
	movs	r5, #7
.L_080dc2d6:
	cmp	r2, #19
	bne.n	.L_080dc2dc
	movs	r5, #6
.L_080dc2dc:
	cmp	r2, #27
	bne.n	.L_080dc2e2
	movs	r5, #0
.L_080dc2e2:
	cmp	r2, #21
	bne.n	.L_080dc2e8
	movs	r5, #7
.L_080dc2e8:
	movs	r2, #240
	lsls	r3, r3, #16
	lsls	r2, r2, #13
	cmp	r3, r2
	bne.n	.L_080dc2f4
	movs	r5, #6
.L_080dc2f4:
	movs	r0, #164
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080dc302
	movs	r5, #0
.L_080dc302:
	movs	r0, #74
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080dc310
	movs	r5, #1
.L_080dc310:
	movs	r0, #165
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080dc31e
	movs	r5, #2
.L_080dc31e:
	movs	r0, #76
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080dc32c
	movs	r5, #3
.L_080dc32c:
	movs	r0, #166
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080dc33a
	movs	r5, #4
.L_080dc33a:
	movs	r0, #78
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080dc348
	movs	r5, #5
.L_080dc348:
	movs	r0, #167
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080dc356
	movs	r5, #6
.L_080dc356:
	movs	r0, #80
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080dc364
	movs	r5, #7
.L_080dc364:
	ldr	r3, [pc, #16]
	lsls	r2, r5, #2
	ldr	r1, [r3, r2]
	movs	r0, #8
	bl	sub_080dc1b0
	pop	{r5, pc}
	movs	r0, r0
	.4byte 0x0300122c
	.2byte 0x3954
	.2byte 0x080f
	push	{lr}
	bl	.L_080dc244
	pop	{pc}
	push	{lr}
	movs	r0, #8
	bl	.L_080dc244
	pop	{pc}
	.align 2, 0
