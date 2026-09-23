.syntax unified
	.thumb
	.set sub_0802b1a0, 0x0802b1a0
	.global Func_0802b344
	.thumb_func
Func_0802b344:
	push	{r5, r6, lr}
	mov	r6, fp
	mov	r5, sl
	push	{r5, r6}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	sub	sp, #8
	ldr	r5, [sp, #36]
	ldr	r6, [sp, #40]
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	mov	r9, r2
	mov	fp, r3
	adds	r2, r5, #0
	adds	r3, r6, #0
	mov	r8, r0
	mov	sl, r1
	bl	sub_0802b1a0
	mov	r0, r8
	mov	r1, sl
	mov	r2, r9
	mov	r3, fp
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl	.L_0802b38c
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r3}
	mov	fp, r3
	pop	{r5, r6, pc}
.L_0802b38c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	mov	sl, r3
	ldr	r3, [sp, #52]
	mov	r8, r2
	ldr	r2, [sp, #48]
	lsls	r3, r3, #7
	adds	r3, r3, r2
	ldr	r4, [pc, #152]
	ldr	r2, [pc, #156]
	lsls	r1, r1, #7
	adds	r5, r3, r2
	mov	r9, r4
	adds	r1, r1, r0
	lsls	r3, r3, #2
	adds	r2, r1, r2
	add	r3, r9
	lsls	r1, r1, #2
	mov	r7, sl
	str	r5, [sp, #12]
	str	r2, [sp, #8]
	mov	fp, r3
	add	r9, r1
	cmp	r7, #0
	ble.n	.L_0802b436
	movs	r1, #0
	str	r1, [sp, #0]
.L_0802b3ce:
	ldr	r2, [sp, #0]
	mov	r5, fp
	lsrs	r3, r2, #16
	lsls	r2, r3, #9
	adds	r4, r5, r2
	add	r2, r9
	str	r2, [sp, #4]
	ldr	r7, [sp, #12]
	ldr	r2, [sp, #8]
	lsls	r3, r3, #7
	adds	r0, r7, r3
	adds	r1, r2, r3
	mov	r3, r8
	cmp	r3, #0
	ble.n	.L_0802b426
	movs	r5, #240
	ldr	r7, [pc, #92]
	lsls	r5, r5, #4
	adds	r5, #255
	movs	r6, #0
	mov	lr, r5
	mov	ip, r7
.L_0802b3fa:
	ldr	r7, [sp, #4]
	ldr	r2, [r4, #0]
	mov	r3, lr
	ands	r2, r3
	ldmia	r7!, {r3}
	adds	r5, r7, #0
	str	r5, [sp, #4]
	mov	r5, ip
	ands	r3, r5
	orrs	r2, r3
	stmia	r4!, {r2}
	movs	r7, #128
	ldrb	r3, [r1, #0]
	lsls	r7, r7, #9
	strb	r3, [r0, #0]
	adds	r3, r6, r7
	adds	r6, r3, #0
	lsrs	r3, r6, #16
	adds	r0, #1
	adds	r1, #1
	cmp	r3, r8
	blt.n	.L_0802b3fa
.L_0802b426:
	ldr	r1, [sp, #0]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r1, r2
	str	r3, [sp, #0]
	lsrs	r3, r3, #16
	cmp	r3, sl
	blt.n	.L_0802b3ce
.L_0802b436:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02010000
	.4byte 0x02024000
	.2byte 0xf000
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r2
	lsls	r1, r1, #7
	ldr	r2, [pc, #276]
	adds	r1, r1, r0
	sub	sp, #36
	lsls	r1, r1, #2
	adds	r1, r1, r2
	ldr	r6, [sp, #72]
	str	r1, [sp, #8]
	ldr	r1, [sp, #68]
	adds	r4, r3, #0
	lsls	r3, r6, #7
	adds	r3, r3, r1
	lsls	r3, r3, #2
	adds	r3, r3, r2
	str	r3, [sp, #4]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r5, #132
	lsls	r5, r5, #1
	add	r1, sp, #12
	adds	r2, r3, r5
	mov	r9, r1
	movs	r5, #2
.L_0802b490:
	ldr	r3, [r2, #0]
	subs	r5, #1
	asrs	r3, r3, #20
	str	r3, [r1, #0]
	ldr	r3, [r2, #4]
	adds	r2, #56
	asrs	r3, r3, #20
	str	r3, [r1, #4]
	adds	r1, #8
	cmp	r5, #0
	bge.n	.L_0802b490
	adds	r3, r6, r4
	cmp	r6, r3
	bge.n	.L_0802b56a
	str	r3, [sp, #0]
	mov	r1, fp
	movs	r3, #128
	subs	r3, r3, r1
	lsls	r3, r3, #2
	mov	r8, r3
.L_0802b4b8:
	ldr	r4, [sp, #68]
	mov	r2, fp
	adds	r3, r4, r2
	cmp	r4, r3
	bge.n	.L_0802b556
	mov	lr, r3
	adds	r7, r6, #0
	movs	r3, #15
	mov	sl, r3
	ands	r7, r3
.L_0802b4cc:
	ldr	r1, [sp, #8]
	movs	r3, #240
	ldmia	r1!, {r2}
	lsls	r3, r3, #4
	adds	r5, r1, #0
	adds	r3, #255
	ldr	r1, [sp, #4]
	str	r5, [sp, #8]
	mov	ip, r3
	movs	r3, #224
	lsls	r3, r3, #3
	mov	r5, ip
	adds	r3, #255
	ands	r5, r2
	ands	r3, r2
	ldr	r2, [r1, #0]
	ldr	r1, [pc, #140]
	mov	ip, r5
	ldr	r5, [sp, #4]
	ands	r2, r1
	orrs	r3, r2
	stmia	r5!, {r3}
	mov	r3, sl
	adds	r2, r5, #0
	str	r2, [sp, #4]
	adds	r2, r4, #0
	ands	r2, r3
	lsls	r3, r7, #5
	adds	r3, r3, r2
	mov	r1, r9
	movs	r5, #0
	lsls	r0, r3, #2
.L_0802b50c:
	ldr	r3, [r1, #0]
	cmp	r3, r4
	bgt.n	.L_0802b542
	adds	r3, #16
	cmp	r3, r4
	ble.n	.L_0802b542
	ldr	r3, [r1, #4]
	cmp	r3, r6
	bgt.n	.L_0802b542
	adds	r3, #12
	cmp	r3, r6
	ble.n	.L_0802b542
	ldr	r5, [pc, #88]
	mov	r2, ip
	adds	r1, r0, r5
	ldr	r5, [pc, #88]
	lsls	r3, r2, #3
	adds	r2, r3, r5
	ldr	r2, [r2, #0]
	str	r2, [r1, #0]
	ldr	r1, [pc, #80]
	adds	r2, r3, r1
	ldr	r3, [pc, #80]
	adds	r1, r0, r3
	ldr	r3, [r2, #0]
	str	r3, [r1, #0]
	b.n	.L_0802b550
.L_0802b542:
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r5, #1
	adds	r0, r0, r2
	adds	r1, #8
	cmp	r5, #2
	ble.n	.L_0802b50c
.L_0802b550:
	adds	r4, #1
	cmp	r4, lr
	blt.n	.L_0802b4cc
.L_0802b556:
	ldr	r3, [sp, #8]
	ldr	r5, [sp, #4]
	ldr	r1, [sp, #0]
	add	r3, r8
	add	r5, r8
	adds	r6, #1
	str	r3, [sp, #8]
	str	r5, [sp, #4]
	cmp	r6, r1
	blt.n	.L_0802b4b8
.L_0802b56a:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02010000
	.4byte 0xfffff800
	.4byte 0x06002800
	.4byte 0x02020000
	.4byte 0x02020004
	.2byte 0x2840
	.2byte 0x0600
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	ip, r3
	ldr	r3, [sp, #32]
	adds	r7, r2, #0
	ldr	r2, [sp, #28]
	lsls	r3, r3, #7
	adds	r3, r3, r2
	ldr	r4, [pc, #112]
	ldr	r2, [pc, #112]
	lsls	r1, r1, #7
	mov	lr, r4
	adds	r1, r1, r0
	adds	r4, r3, r2
	lsls	r3, r3, #2
	adds	r2, r2, r1
	add	r3, lr
	lsls	r1, r1, #2
	mov	r0, ip
	mov	r9, r4
	mov	sl, r2
	mov	r8, r3
	add	lr, r1
	cmp	r0, #0
	ble.n	.L_0802b60c
	movs	r6, #0
.L_0802b5ca:
	lsrs	r3, r6, #16
	lsls	r2, r3, #9
	mov	r1, r8
	mov	r0, lr
	adds	r5, r1, r2
	adds	r4, r0, r2
	lsls	r3, r3, #7
	mov	r2, r9
	mov	r0, sl
	adds	r1, r2, r3
	adds	r2, r0, r3
	cmp	r7, #0
	ble.n	.L_0802b5fe
	movs	r0, #0
.L_0802b5e6:
	ldmia	r4!, {r3}
	stmia	r5!, {r3}
	ldrb	r3, [r2, #0]
	adds	r2, #1
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #9
	adds	r0, r0, r3
	lsrs	r3, r0, #16
	adds	r1, #1
	cmp	r3, r7
	blt.n	.L_0802b5e6
.L_0802b5fe:
	movs	r4, #128
	lsls	r4, r4, #9
	adds	r3, r6, r4
	adds	r6, r3, #0
	lsrs	r3, r6, #16
	cmp	r3, ip
	blt.n	.L_0802b5ca
.L_0802b60c:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02010000
	.2byte 0x4000
	.2byte 0x0202
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	movs	r3, #241
	ldrh	r1, [r4, #20]
	movs	r2, #224
	lsls	r3, r3, #8
	lsls	r2, r2, #4
	adds	r3, #255
	ands	r2, r0
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r4, #20]
	bx	lr
