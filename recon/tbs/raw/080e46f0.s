.syntax unified
	.thumb
	.set sub_08002f40, 0x08002f40
	.set sub_080072f0, 0x080072f0
	.global Unnamed_080e46f0
	.global Func_080e46f0
	.thumb_func
Unnamed_080e46f0:
Func_080e46f0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r1, #160
	lsls	r1, r1, #19
	sub	sp, #128
	mov	r8, r1
	bl	sub_08002f40
	ldr	r3, [pc, #32]
	adds	r1, r0, #0
	movs	r2, #128
	mov	r0, sp
	bl	sub_080072f0
	movs	r3, #0
	mov	r2, sp
	strh	r3, [r2, #0]
	mov	ip, r3
	ldr	r7, [pc, #8]
	movs	r3, #31
	mov	sl, r3
	mov	lr, r2
	movs	r6, #0
	b.n	.L_080e472c
	.4byte 0x0000001f
	.2byte 0x1388
	.2byte 0x0300
.L_080e472c:
	mov	r1, r8
	ldrh	r3, [r1, #0]
	mov	r5, sl
	mov	r1, lr
	ands	r5, r3
	lsls	r3, r3, #16
	lsrs	r2, r3, #21
	lsrs	r0, r3, #26
	ldrh	r3, [r6, r1]
	mov	r1, sl
	ands	r1, r3
	lsls	r3, r3, #16
	lsrs	r4, r3, #21
	lsrs	r3, r3, #26
	ands	r2, r7
	ands	r0, r7
	ands	r4, r7
	ands	r3, r7
	cmp	r5, r1
	bge.n	.L_080e4758
	adds	r5, #1
	b.n	.L_080e475e
.L_080e4758:
	cmp	r5, r1
	ble.n	.L_080e475e
	subs	r5, #1
.L_080e475e:
	cmp	r2, r4
	bge.n	.L_080e4766
	adds	r2, #1
	b.n	.L_080e476c
.L_080e4766:
	cmp	r2, r4
	ble.n	.L_080e476c
	subs	r2, #1
.L_080e476c:
	cmp	r0, r3
	bge.n	.L_080e4774
	adds	r0, #1
	b.n	.L_080e477a
.L_080e4774:
	cmp	r0, r3
	ble.n	.L_080e477a
	subs	r0, #1
.L_080e477a:
	lsls	r2, r2, #5
	lsls	r3, r0, #10
	orrs	r3, r2
	orrs	r3, r5
	mov	r2, lr
	strh	r3, [r6, r2]
	movs	r3, #1
	add	ip, r3
	movs	r1, #2
	mov	r2, ip
	adds	r6, #2
	add	r8, r1
	cmp	r2, #64
	bne.n	.L_080e472c
	movs	r0, #160
	mov	r1, sp
	ldr	r3, [pc, #24]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	add	sp, #128
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001388
