.syntax unified
	.thumb
	.set sub_080d2d84, 0x080d2d84
	.set sub_080d8fa8, 0x080d8fa8
	.global Func_080d9978
	.thumb_func
Func_080d9978:
	push	{lr}
	ldr	r2, [r0, #80]
	movs	r3, #253
	strb	r3, [r2, #22]
	movs	r3, #12
	strb	r3, [r2, #23]
	ldr	r3, [pc, #40]
	movs	r2, #128
	ldr	r3, [r3, #12]
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d9998
	ldr	r2, [pc, #32]
	movs	r3, #1
	str	r3, [r2, #0]
.L_080d9998:
	ldr	r3, [pc, #24]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080d99ac
	movs	r0, #0
	movs	r1, #12
	movs	r2, #13
	movs	r3, #0
	bl	sub_080d8fa8
.L_080d99ac:
	pop	{pc}
	movs	r0, r0
	.4byte 0x03001150
	.2byte 0x3944
	.2byte 0x080f
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #176]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	mov	r8, r1
	bl	sub_080d2d84
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_080d2d84
	adds	r6, r0, #0
	mov	r0, r8
	bl	sub_080d2d84
	ldr	r3, [r5, #8]
	ldr	r2, [r6, #12]
	asrs	r4, r3, #20
	ldr	r3, [r5, #12]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	mov	lr, r3
	ldr	r3, [r5, #16]
	asrs	r1, r3, #20
	ldr	r3, [r6, #8]
	asrs	r7, r3, #20
	ldr	r3, [r6, #16]
	asrs	r5, r3, #20
	ldr	r3, [r0, #8]
	asrs	r6, r3, #20
	ldr	r3, [r0, #12]
	asrs	r3, r3, #20
	mov	ip, r3
	ldr	r3, [r0, #16]
	asrs	r0, r3, #20
	cmp	lr, r2
	bne.n	.L_080d9a36
	cmp	r4, r7
	bne.n	.L_080d9a22
	subs	r3, r5, #1
	cmp	r1, r3
	beq.n	.L_080d9a64
	adds	r3, r5, #1
	cmp	r1, r3
	beq.n	.L_080d9a64
	cmp	r1, r5
	bne.n	.L_080d9a36
	b.n	.L_080d9a64
.L_080d9a22:
	cmp	r1, r5
	bne.n	.L_080d9a36
	subs	r3, r7, #1
	cmp	r4, r3
	beq.n	.L_080d9a64
	adds	r3, r7, #1
	cmp	r4, r3
	beq.n	.L_080d9a64
	cmp	r4, r7
	beq.n	.L_080d9a64
.L_080d9a36:
	cmp	lr, ip
	bne.n	.L_080d9a68
	cmp	r4, r6
	bne.n	.L_080d9a50
	subs	r3, r0, #1
	cmp	r1, r3
	beq.n	.L_080d9a64
	adds	r3, r0, #1
	cmp	r1, r3
	beq.n	.L_080d9a64
	cmp	r1, r0
	bne.n	.L_080d9a68
	b.n	.L_080d9a64
.L_080d9a50:
	cmp	r1, r0
	bne.n	.L_080d9a68
	subs	r3, r6, #1
	cmp	r4, r3
	beq.n	.L_080d9a64
	adds	r3, r6, #1
	cmp	r4, r3
	beq.n	.L_080d9a64
	cmp	r4, r6
	bne.n	.L_080d9a68
.L_080d9a64:
	movs	r0, #1
	b.n	.L_080d9a6a
.L_080d9a68:
	movs	r0, #0
.L_080d9a6a:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #156
	ldr	r3, [r3, #0]
	adds	r5, r2, #0
	adds	r4, r3, #0
	adds	r4, #12
	movs	r2, #0
.L_080d9a86:
	ldr	r3, [r4, #28]
	cmp	r3, #0
	beq.n	.L_080d9aa0
	movs	r6, #0
	ldrsh	r3, [r4, r6]
	cmp	r3, r0
	bne.n	.L_080d9aa0
	movs	r0, #2
	ldrsh	r3, [r4, r0]
	movs	r0, #0
	str	r3, [r1, #0]
	str	r2, [r5, #0]
	b.n	.L_080d9aac
.L_080d9aa0:
	adds	r2, #1
	adds	r4, #32
	cmp	r2, #7
	ble.n	.L_080d9a86
	movs	r0, #1
	negs	r0, r0
.L_080d9aac:
	pop	{r5, r6, pc}
	.align 2, 0
