.syntax unified
	.thumb
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08004458, 0x08004458
	.set sub_080045d4, 0x080045d4
	.set sub_080072f0, 0x080072f0
	.set sub_08007310, 0x08007310
	.global Overlay_080c11ec
Overlay_080c11ec:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #540]
	mov	ip, r2
	mov	r3, ip
	adds	r3, #160
	ldr	r3, [r3, #0]
	sub	sp, #16
	str	r3, [sp, #12]
	mov	r3, ip
	adds	r3, #156
	ldr	r3, [r3, #0]
	movs	r2, #158
	mov	sl, r3
	lsls	r2, r2, #5
	add	r2, sl
	movs	r3, #0
	str	r3, [r2, #0]
	mov	r3, ip
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r7, #142
	mov	fp, r3
	movs	r2, #15
	ldr	r3, [pc, #500]
	lsls	r7, r7, #5
	str	r2, [sp, #8]
	mov	r9, r3
	add	r7, sl
.L_080c1230:
	ldr	r0, [r7, #24]
	cmp	r0, #0
	beq.n	.L_080c12b4
	ldr	r3, [r7, #0]
	asrs	r3, r3, #8
	adds	r0, r3, #0
	muls	r0, r3
	ldr	r3, [r7, #4]
	asrs	r3, r3, #8
	adds	r2, r3, #0
	muls	r2, r3
	adds	r3, r2, #0
	adds	r0, r0, r3
	ldr	r3, [r7, #8]
	asrs	r3, r3, #8
	adds	r2, r3, #0
	muls	r2, r3
	adds	r3, r2, #0
	adds	r0, r0, r3
	bl	sub_080045d4
	ldr	r3, [pc, #452]
	cmp	r0, r3
	bgt.n	.L_080c1266
	movs	r3, #0
	str	r3, [r7, #24]
	b.n	.L_080c12ae
.L_080c1266:
	movs	r1, #128
	ldr	r3, [pc, #440]
	lsls	r1, r1, #9
	bl	sub_080072f0
	ldr	r3, [r7, #24]
	subs	r3, #1
	movs	r2, #2
	str	r3, [r7, #24]
	ldr	r6, [pc, #416]
	mov	r8, r0
	adds	r5, r7, #0
	mov	lr, r2
.L_080c1280:
	ldr	r4, [r5, #0]
	negs	r0, r4
	asrs	r0, r0, #8
	mov	r1, r8
	mov	ip, pc
	bx	r6
	movs	r1, #152
	lsls	r1, r1, #9
	mov	ip, pc
	bx	r6
	ldr	r3, [r5, #12]
	asrs	r2, r3, #7
	subs	r3, r3, r2
	adds	r3, r3, r0
	str	r3, [r5, #12]
	adds	r4, r4, r3
	movs	r3, #1
	negs	r3, r3
	add	lr, r3
	mov	r2, lr
	stmia	r5!, {r4}
	cmp	r2, #0
	bge.n	.L_080c1280
.L_080c12ae:
	ldr	r0, [r7, #24]
	cmp	r0, #0
	bne.n	.L_080c1348
.L_080c12b4:
	ldr	r3, [pc, #368]
	add	r3, sl
	ldr	r3, [r3, #0]
	cmp	r3, #24
	bgt.n	.L_080c1344
	bl	sub_08004458
	adds	r5, r0, #0
	bl	sub_08004458
	movs	r3, #128
	lsls	r3, r3, #9
	adds	r3, r3, r0
	lsrs	r6, r3, #1
	adds	r0, r5, #0
	mov	r8, r3
	bl	sub_0800231c
	adds	r1, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	r9
	str	r0, [r7, #0]
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r1, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	r9
	ldr	r2, [r7, #0]
	movs	r1, #1
	adds	r3, r2, #0
	ands	r3, r1
	str	r0, [r7, #4]
	cmp	r3, #0
	beq.n	.L_080c1302
	negs	r3, r2
	str	r3, [r7, #0]
.L_080c1302:
	ldr	r2, [r7, #4]
	adds	r3, r2, #0
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080c1310
	negs	r3, r2
	str	r3, [r7, #4]
.L_080c1310:
	bl	sub_08004458
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r0, r0, r2
	lsrs	r0, r0, #2
	ldr	r3, [r7, #4]
	str	r0, [r7, #8]
	ldr	r0, [r7, #0]
	asrs	r2, r3, #8
	negs	r0, r0
	negs	r3, r3
	asrs	r1, r0, #7
	asrs	r3, r3, #7
	asrs	r0, r0, #8
	adds	r3, r3, r0
	adds	r1, r1, r2
	str	r3, [r7, #16]
	mov	r2, r8
	movs	r3, #0
	str	r3, [r7, #20]
	lsrs	r3, r2, #13
	adds	r3, #1
	str	r1, [r7, #12]
	str	r3, [r7, #24]
	adds	r0, r3, #0
.L_080c1344:
	cmp	r0, #0
	beq.n	.L_080c1384
.L_080c1348:
	ldr	r3, [r7, #0]
	asrs	r3, r3, #10
	adds	r5, r3, #0
	ldr	r3, [r7, #4]
	asrs	r3, r3, #10
	adds	r4, r3, #0
	adds	r6, r0, #0
	adds	r5, #64
	adds	r4, #64
	cmp	r6, #0
	bge.n	.L_080c1362
	movs	r6, #0
	b.n	.L_080c1368
.L_080c1362:
	cmp	r6, #6
	ble.n	.L_080c1368
	movs	r6, #6
.L_080c1368:
	ldr	r3, [pc, #192]
	ldr	r2, [pc, #196]
	ldrb	r0, [r3, r6]
	lsls	r3, r6, #2
	ldr	r1, [r2, r3]
	lsrs	r3, r0, #1
	subs	r2, r5, r3
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	add	r1, sl
	subs	r3, r4, r3
	ldr	r0, [sp, #12]
	bl	sub_08007310
.L_080c1384:
	ldr	r3, [sp, #8]
	subs	r3, #1
	adds	r7, #28
	str	r3, [sp, #8]
	cmp	r3, #0
	blt.n	.L_080c1392
	b.n	.L_080c1230
.L_080c1392:
	ldr	r3, [pc, #132]
	adds	r3, #188
	ldr	r3, [r3, #0]
	movs	r5, #156
	lsls	r5, r5, #5
	mov	fp, r3
	add	r5, sl
	movs	r7, #2
.L_080c13a2:
	ldr	r1, [r5, #0]
	ldr	r3, [r5, #8]
	ldr	r2, [r5, #4]
	adds	r1, r1, r3
	ldr	r3, [r5, #12]
	str	r1, [r5, #0]
	asrs	r4, r1, #10
	ldr	r1, [r5, #16]
	adds	r2, r2, r3
	str	r2, [r5, #4]
	asrs	r6, r2, #10
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_080c13c0
	adds	r2, r1, #7
.L_080c13c0:
	asrs	r2, r2, #3
	movs	r3, #3
	subs	r0, r3, r2
	cmp	r0, #0
	blt.n	.L_080c13ea
	adds	r3, r1, #1
	str	r3, [r5, #16]
	ldr	r2, [pc, #100]
	lsls	r3, r0, #2
	ldr	r1, [r2, r3]
	movs	r0, #32
	adds	r2, r4, #0
	adds	r3, r6, #0
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	add	r1, sl
	adds	r2, #48
	adds	r3, #48
	ldr	r0, [sp, #12]
	bl	sub_08007310
.L_080c13ea:
	subs	r7, #1
	adds	r5, #20
	cmp	r7, #0
	bge.n	.L_080c13a2
	ldr	r2, [pc, #52]
	add	r2, sl
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	movs	r2, #158
	lsls	r2, r2, #5
	add	r2, sl
	movs	r3, #1
	str	r3, [r2, #0]
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e50
	.4byte 0x03000118
	.4byte 0x00000fff
	.4byte 0x0300013c
	.4byte 0x000013bc
	.4byte 0x080c3620
	.4byte 0x080c3604
	.4byte 0x080c3628
