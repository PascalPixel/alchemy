.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08016ca4, 0x08016ca4
	.set sub_080af094, 0x080af094
	.set sub_080af4e4, 0x080af4e4
	.set sub_080b0084, 0x080b0084
	.set sub_080b0144, 0x080b0144
	.global Func_080b0298
	.thumb_func
Func_080b0298:
.L_080b0298:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	bl	sub_08016ca4
	movs	r1, #10
	adds	r6, r0, #0
	bl	sub_080af094
	movs	r1, #165
	lsls	r1, r1, #1
	adds	r3, r6, r1
	adds	r1, r6, #0
	adds	r2, r0, #0
	adds	r1, #248
	ldrh	r0, [r3, #0]
	bl	sub_080b0144
	movs	r2, #42
	adds	r2, #255
	adds	r3, r6, r2
	strb	r0, [r3, #0]
	adds	r0, r5, #0
	bl	sub_080af4e4
	adds	r1, r6, #0
	adds	r1, #36
	adds	r0, r5, #0
	bl	sub_080b0084
	pop	{r5, r6, pc}
	push	{lr}
	bl	.L_080b0298
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #144]
	mov	r8, r1
	movs	r1, #0
	ldrsh	r2, [r5, r1]
	adds	r6, r0, #0
	movs	r4, #16
	ldrsh	r3, [r5, r4]
	movs	r0, #5
	cmp	r6, r2
	ble.n	.L_080b02fa
	adds	r6, r2, #0
	b.n	.L_080b0300
.L_080b02fa:
	cmp	r6, r3
	bge.n	.L_080b0300
	adds	r6, r3, #0
.L_080b0300:
	movs	r1, #0
	movs	r4, #0
	cmp	r1, r0
	bge.n	.L_080b0330
	movs	r7, #0
	ldrsh	r3, [r5, r7]
	cmp	r6, r3
	bgt.n	.L_080b032a
	mov	ip, r5
	movs	r2, #0
.L_080b0314:
	adds	r1, #1
	adds	r2, #4
	cmp	r1, r0
	bge.n	.L_080b032e
	adds	r4, r2, #0
	mov	r3, ip
	ldrsh	r3, [r4, r3]
	mov	lr, r3
	cmp	r6, lr
	ble.n	.L_080b0314
	b.n	.L_080b0330
.L_080b032a:
	movs	r4, #0
	b.n	.L_080b0330
.L_080b032e:
	lsls	r4, r1, #2
.L_080b0330:
	cmp	r1, r0
	bne.n	.L_080b033a
	subs	r3, r4, #2
	ldrsh	r0, [r5, r3]
	b.n	.L_080b0356
.L_080b033a:
	subs	r3, r4, #4
	ldrsh	r1, [r5, r3]
	ldrsh	r0, [r5, r4]
	subs	r3, r4, #2
	ldrsh	r2, [r5, r3]
	adds	r3, r4, #2
	ldrsh	r5, [r5, r3]
	subs	r1, r1, r0
	subs	r2, r2, r5
	subs	r0, r6, r0
	muls	r0, r2
	bl	sub_08002054
	adds	r0, r0, r5
.L_080b0356:
	mov	r7, r8
	cmp	r7, #0
	beq.n	.L_080b0366
	cmp	r7, #1
	bne.n	.L_080b0366
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r0, r3, #1
.L_080b0366:
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r0, r0, r1
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x6b04
	.2byte 0x080c
.L_080b0378:
	ldr	r1, [pc, #24]
	ldr	r3, [pc, #28]
	ldr	r2, [r1, #0]
	adds	r0, r2, #0
	muls	r0, r3
	movs	r3, #192
	lsls	r3, r3, #6
	adds	r3, #57
	adds	r0, r0, r3
	str	r0, [r1, #0]
	lsls	r0, r0, #8
	lsrs	r0, r0, #16
	bx	lr
	movs	r0, r0
	.4byte 0x020054c8
	.2byte 0x4e6d
	.2byte 0x41c6
	push	{lr}
	bl	.L_080b0378
	movs	r3, #100
	muls	r0, r3
	lsrs	r0, r0, #16
	pop	{pc}
	.align 2, 0
