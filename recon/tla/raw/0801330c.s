.syntax unified
	.thumb
	.global Func_0801330c
	.thumb_func
Func_0801330c:
	push	{r5, r6, r7, lr}
	asrs	r7, r1, #1
	movs	r6, #1
	adds	r0, #2
	cmp	r6, r7
	bge.n	.L_08013368
	movs	r3, #248
	lsls	r3, r3, #8
	mov	ip, r3
	movs	r5, #224
	movs	r3, #240
	lsls	r5, r5, #3
	lsls	r3, r3, #8
	adds	r5, #255
	mov	lr, r3
	subs	r0, #4
.L_0801332c:
	ldrh	r2, [r0, #4]
	mov	r3, ip
	adds	r1, r2, #0
	ands	r1, r3
	adds	r0, #2
	cmp	r1, ip
	bne.n	.L_08013362
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	ands	r4, r1
	cmp	r4, lr
	bne.n	.L_08013362
	ands	r3, r5
	ands	r2, r5
	lsls	r2, r2, #1
	lsls	r3, r3, #12
	orrs	r3, r2
	lsls	r2, r6, #1
	subs	r3, r3, r2
	lsrs	r2, r3, #12
	lsrs	r3, r3, #1
	ands	r2, r5
	ands	r3, r5
	orrs	r2, r4
	orrs	r3, r1
	strh	r2, [r0, #0]
	strh	r3, [r0, #2]
.L_08013362:
	adds	r6, #1
	cmp	r6, r7
	blt.n	.L_0801332c
.L_08013368:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
