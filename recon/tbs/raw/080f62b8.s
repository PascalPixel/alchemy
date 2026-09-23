.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.global Unnamed_080f62b8
	.global Func_080f62b8
	.thumb_func
Unnamed_080f62b8:
Func_080f62b8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r4, r3, #0
	ldr	r3, [pc, #364]
	ldr	r3, [r3, #0]
	adds	r7, r1, #0
	subs	r6, r2, r0
	movs	r1, #128
	mov	sl, r2
	sub	sp, #4
	mov	r8, r0
	subs	r5, r4, r7
	mov	r9, r1
	mov	fp, r3
	adds	r2, r6, #0
	cmp	r6, #0
	bge.n	.L_080f62e6
	negs	r2, r6
.L_080f62e6:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_080f62ee
	negs	r3, r5
.L_080f62ee:
	cmp	r2, r3
	bge.n	.L_080f6396
	cmp	r5, #0
	bge.n	.L_080f630a
	mov	ip, r8
	mov	r8, sl
	mov	sl, ip
	mov	ip, r7
	mov	r2, sl
	adds	r7, r4, #0
	mov	r3, r8
	mov	r4, ip
	subs	r6, r2, r3
	subs	r5, r4, r7
.L_080f630a:
	lsls	r0, r6, #8
	cmp	r6, #0
	bge.n	.L_080f6318
	mov	r1, r8
	mov	r2, sl
	subs	r3, r1, r2
	lsls	r0, r3, #8
.L_080f6318:
	cmp	r5, #0
	blt.n	.L_080f632a
	adds	r1, r5, #0
	str	r4, [sp, #0]
	bl	sub_080022ec
	mov	ip, r0
	ldr	r4, [sp, #0]
	b.n	.L_080f6336
.L_080f632a:
	subs	r1, r7, r4
	str	r4, [sp, #0]
	bl	sub_080022ec
	ldr	r4, [sp, #0]
	mov	ip, r0
.L_080f6336:
	adds	r0, r7, #0
	mov	r1, r8
	cmp	r0, r4
	beq.n	.L_080f6426
	movs	r3, #128
	ldr	r7, [pc, #248]
	lsls	r3, r3, #1
	movs	r5, #7
	mov	lr, r3
	mov	r8, r7
.L_080f634a:
	lsrs	r2, r0, #3
	lsrs	r3, r1, #3
	lsls	r2, r2, #5
	adds	r2, r2, r3
	adds	r3, r0, #0
	ands	r3, r5
	lsls	r2, r2, #3
	adds	r2, r2, r3
	adds	r3, r1, #0
	ands	r3, r5
	lsls	r2, r2, #3
	mov	r7, fp
	adds	r2, r2, r3
	ldrb	r3, [r7, r2]
	ldr	r7, [sp, #36]
	cmp	r3, r7
	bge.n	.L_080f6370
	mov	r3, fp
	strb	r7, [r3, r2]
.L_080f6370:
	add	r9, ip
	mov	r3, r9
	mov	r7, lr
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_080f638e
	cmp	r6, #0
	ble.n	.L_080f6384
	adds	r1, #1
	b.n	.L_080f6386
.L_080f6384:
	subs	r1, #1
.L_080f6386:
	mov	r2, r9
	mov	r3, r8
	ands	r2, r3
	mov	r9, r2
.L_080f638e:
	adds	r0, #1
	cmp	r0, r4
	bne.n	.L_080f634a
	b.n	.L_080f6426
.L_080f6396:
	cmp	r6, #0
	bge.n	.L_080f63ae
	mov	ip, r8
	mov	r8, sl
	mov	sl, ip
	mov	ip, r7
	mov	r1, sl
	adds	r7, r4, #0
	mov	r2, r8
	mov	r4, ip
	subs	r6, r1, r2
	subs	r5, r4, r7
.L_080f63ae:
	lsls	r0, r5, #8
	cmp	r5, #0
	bge.n	.L_080f63b8
	subs	r3, r7, r4
	lsls	r0, r3, #8
.L_080f63b8:
	cmp	r6, #0
	blt.n	.L_080f63c0
	adds	r1, r6, #0
	b.n	.L_080f63c6
.L_080f63c0:
	mov	r3, r8
	mov	r6, sl
	subs	r1, r3, r6
.L_080f63c6:
	bl	sub_080022ec
	mov	ip, r0
	mov	r0, r8
	adds	r1, r7, #0
	cmp	r0, sl
	beq.n	.L_080f6426
	ldr	r2, [pc, #100]
	movs	r7, #128
	movs	r4, #7
	lsls	r7, r7, #1
	mov	lr, r2
.L_080f63de:
	lsrs	r2, r1, #3
	lsrs	r3, r0, #3
	lsls	r2, r2, #5
	adds	r2, r2, r3
	adds	r3, r1, #0
	ands	r3, r4
	lsls	r2, r2, #3
	adds	r2, r2, r3
	adds	r3, r0, #0
	ands	r3, r4
	lsls	r2, r2, #3
	mov	r6, fp
	adds	r2, r2, r3
	ldrb	r3, [r6, r2]
	ldr	r6, [sp, #36]
	cmp	r3, r6
	bge.n	.L_080f6404
	mov	r3, fp
	strb	r6, [r3, r2]
.L_080f6404:
	add	r9, ip
	mov	r3, r9
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_080f6420
	cmp	r5, #0
	ble.n	.L_080f6416
	adds	r1, #1
	b.n	.L_080f6418
.L_080f6416:
	subs	r1, #1
.L_080f6418:
	mov	r6, r9
	mov	r2, lr
	ands	r6, r2
	mov	r9, r6
.L_080f6420:
	adds	r0, #1
	cmp	r0, sl
	bne.n	.L_080f63de
.L_080f6426:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001ef0
	.4byte 0xfffffeff
