.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_08002304, 0x08002304
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f3c, 0x08002f3c
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080040e8, 0x080040e8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080048b0, 0x080048b0
	.set sub_080048f4, 0x080048f4
	.set sub_08005340, 0x08005340
	.set sub_0800562c, 0x0800562c
	.set sub_0800567c, 0x0800567c
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_08015000, 0x08015000
	.set sub_08015010, 0x08015010
	.set sub_08015018, 0x08015018
	.set sub_08015080, 0x08015080
	.set sub_080150b0, 0x080150b0
	.set sub_08077048, 0x08077048
	.set sub_080772e0, 0x080772e0
	.set sub_080c9000, 0x080c9000
	.set sub_080f6038, 0x080f6038
	.set sub_080f9010, 0x080f9010
	.global Overlay_080f60a0
Overlay_080f60a0:
	push	{r5, lr}
	ldr	r0, [pc, #132]
	ldr	r2, [pc, #132]
	ldr	r5, [r0, #0]
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080f6116
	subs	r2, #164
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	ldr	r4, [r0, #4]
	cmp	r3, #1
	beq.n	.L_080f60c2
	cmp	r3, #2
	beq.n	.L_080f60e2
	b.n	.L_080f6106
.L_080f60c2:
	ldr	r3, [pc, #108]
	adds	r0, r4, #0
	ldr	r1, [pc, #108]
	ldr	r2, [pc, #108]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #108]
	movs	r1, #128
	adds	r3, r5, r2
	ldr	r2, [r3, #0]
	adds	r0, r4, #0
	ldr	r3, [pc, #100]
	lsls	r1, r1, #8
	bl	sub_080072f0
	b.n	.L_080f6106
.L_080f60e2:
	ldr	r2, [pc, #88]
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	cmp	r3, #50
	bne.n	.L_080f60fa
	movs	r2, #128
	ldr	r1, [pc, #68]
	lsls	r2, r2, #8
	adds	r0, r4, #0
	bl	sub_0800567c
	b.n	.L_080f6106
.L_080f60fa:
	movs	r2, #128
	ldr	r1, [pc, #52]
	lsls	r2, r2, #8
	adds	r0, r4, #0
	bl	sub_0800562c
.L_080f6106:
	ldr	r3, [pc, #36]
	adds	r2, r5, r3
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, r5, r3
	movs	r3, #1
	b.n	.L_080f611e
.L_080f6116:
	ldr	r3, [pc, #44]
	adds	r2, r5, r3
	ldr	r3, [r2, #0]
	adds	r3, #1
.L_080f611e:
	str	r3, [r2, #0]
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x00007824
	.4byte 0x040000d4
	.4byte 0x06003500
	.4byte 0x84002000
	.4byte 0x00007784
	.4byte 0x03000168
	.2byte 0x7820
	.2byte 0x0000
.L_080f6148:
	push	{r5, r6, r7, lr}
	ldr	r5, [pc, #52]
	ldr	r7, [pc, #44]
	movs	r6, #0
.L_080f6150:
	ldrh	r2, [r5, #0]
	lsls	r3, r2, #16
	lsrs	r0, r3, #26
	ands	r0, r7
	lsrs	r1, r3, #21
	movs	r4, #31
	ands	r1, r7
	ands	r4, r2
	subs	r0, #1
	subs	r1, #1
	subs	r4, #1
	cmp	r0, #0
	bge.n	.L_080f616c
	movs	r0, #0
.L_080f616c:
	cmp	r1, #0
	bge.n	.L_080f6172
	movs	r1, #0
.L_080f6172:
	cmp	r4, #0
	bge.n	.L_080f6184
	movs	r4, #0
	b.n	.L_080f6184
	movs	r0, r0
	.4byte 0x0000001f
	.2byte 0x0140
	.2byte 0x0500
.L_080f6184:
	lsls	r3, r0, #10
	lsls	r2, r1, #5
	orrs	r3, r2
	orrs	r3, r4
	adds	r6, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r6, #16
	bne.n	.L_080f6150
	ldr	r5, [pc, #52]
	ldr	r7, [pc, #44]
	movs	r6, #0
.L_080f619c:
	ldrh	r2, [r5, #0]
	lsls	r3, r2, #16
	lsrs	r0, r3, #26
	ands	r0, r7
	lsrs	r1, r3, #21
	movs	r4, #31
	ands	r1, r7
	ands	r4, r2
	subs	r0, #1
	subs	r1, #1
	subs	r4, #1
	cmp	r0, #0
	bge.n	.L_080f61b8
	movs	r0, #0
.L_080f61b8:
	cmp	r1, #0
	bge.n	.L_080f61be
	movs	r1, #0
.L_080f61be:
	cmp	r4, #0
	bge.n	.L_080f61d0
	movs	r4, #0
	b.n	.L_080f61d0
	movs	r0, r0
	.4byte 0x0000001f
	.2byte 0x0202
	.2byte 0x0500
.L_080f61d0:
	lsls	r3, r0, #10
	lsls	r2, r1, #5
	orrs	r3, r2
	orrs	r3, r4
	adds	r6, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r6, #239
	bne.n	.L_080f619c
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
.L_080f61e8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r1, #160
	lsls	r1, r1, #19
	sub	sp, #128
	mov	r8, r1
	bl	sub_08002f40
	ldr	r3, [pc, #28]
	mov	r1, sp
	ldr	r2, [pc, #28]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #0
	movs	r3, #31
	ldr	r7, [pc, #12]
	mov	lr, r2
	mov	sl, r3
	mov	ip, sp
	movs	r6, #0
	b.n	.L_080f6224
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x040000d4
	.2byte 0x0020
	.2byte 0x8400
.L_080f6224:
	mov	r1, r8
	ldrh	r3, [r1, #0]
	mov	r5, sl
	mov	r1, ip
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
	bge.n	.L_080f6250
	adds	r5, #1
	b.n	.L_080f6256
.L_080f6250:
	cmp	r5, r1
	ble.n	.L_080f6256
	subs	r5, #1
.L_080f6256:
	cmp	r2, r4
	bge.n	.L_080f625e
	adds	r2, #1
	b.n	.L_080f6264
.L_080f625e:
	cmp	r2, r4
	ble.n	.L_080f6264
	subs	r2, #1
.L_080f6264:
	cmp	r0, r3
	bge.n	.L_080f626c
	adds	r0, #1
	b.n	.L_080f6272
.L_080f626c:
	cmp	r0, r3
	ble.n	.L_080f6272
	subs	r0, #1
.L_080f6272:
	lsls	r2, r2, #5
	lsls	r3, r0, #10
	orrs	r3, r2
	orrs	r3, r5
	mov	r2, ip
	strh	r3, [r6, r2]
	movs	r3, #1
	add	lr, r3
	movs	r1, #2
	mov	r2, lr
	adds	r6, #2
	add	r8, r1
	cmp	r2, #64
	bne.n	.L_080f6224
	mov	r0, sp
	ldr	r3, [pc, #24]
	adds	r0, #2
	ldr	r1, [pc, #24]
	ldr	r2, [pc, #28]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	add	sp, #128
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x040000d4
	.4byte 0x05000002
	.2byte 0x003f
	.2byte 0x8000
.L_080f62b8:
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
	.2byte 0xfeff
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #156]
	ldr	r7, [r3, #0]
	subs	r3, #24
	ldr	r3, [r3, #0]
	movs	r1, #128
	sub	sp, #40
	lsls	r1, r1, #3
	movs	r0, #0
	str	r3, [sp, #36]
	str	r1, [sp, #28]
	str	r0, [sp, #32]
	bl	sub_08004458
	ldr	r3, [pc, #132]
	ldr	r2, [pc, #136]
	ldrh	r1, [r3, #10]
	ands	r2, r1
	strh	r2, [r3, #10]
	ldr	r2, [pc, #132]
	ldrh	r1, [r3, #10]
	ands	r2, r1
	strh	r2, [r3, #10]
	ldrh	r2, [r3, #10]
	movs	r2, #155
	lsls	r2, r2, #3
	adds	r0, r7, r2
	ldr	r1, [pc, #120]
	ldr	r2, [pc, #120]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #120]
	adds	r5, r7, #0
	ldr	r4, [r3, #0]
	adds	r5, #156
	ldrh	r0, [r5, #0]
	lsls	r4, r4, #16
	ldr	r1, [r3, #0]
	adds	r6, r7, #0
	lsrs	r3, r4, #16
	bics	r3, r0
	adds	r6, #160
	strh	r3, [r6, #0]
	movs	r3, #158
	adds	r3, r3, r7
	movs	r2, #240
	mov	r8, r3
	ands	r1, r2
	mov	r0, r8
	strh	r1, [r0, #0]
	ldrh	r3, [r5, #0]
	ands	r2, r3
	cmp	r2, r1
	bne.n	.L_080f650c
	adds	r1, r7, #0
	adds	r1, #162
	ldrh	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #12
	bls.n	.L_080f64ca
	movs	r3, #12
	strh	r3, [r1, #0]
	ldr	r2, [pc, #28]
.L_080f64ca:
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L_080f64d6
	movs	r3, #4
	strh	r3, [r1, #0]
	b.n	.L_080f6514
.L_080f64d6:
	ldr	r0, [pc, #48]
	adds	r3, r2, r0
	strh	r3, [r1, #0]
	add	r1, sp, #32
	ldrh	r1, [r1, #0]
	mov	r2, r8
	strh	r1, [r2, #0]
	b.n	.L_080f6514
	movs	r0, r0
	.4byte 0x0000000c
	.4byte 0x03001f04
	.4byte 0x040000b0
	.4byte 0x0000c5ff
	.4byte 0x00007fff
	.4byte 0x04000054
	.4byte 0xa2600001
	.4byte 0x03001ae8
	.2byte 0xffff
	.2byte 0x0000
.L_080f650c:
	adds	r2, r7, #0
	adds	r2, #162
	movs	r3, #12
	strh	r3, [r2, #0]
.L_080f6514:
	lsrs	r3, r4, #16
	strh	r3, [r5, #0]
	ldr	r3, [pc, #204]
	ldrb	r3, [r3, #0]
	mov	sl, r3
	cmp	r3, #0
	beq.n	.L_080f6526
	bl	.L_080f6dc2
.L_080f6526:
	adds	r2, r7, #0
	adds	r2, #140
	str	r2, [sp, #12]
	str	r2, [sp, #24]
	ldr	r5, [r2, #0]
	cmp	r5, #0
	beq.n	.L_080f6536
	b.n	.L_080f6670
.L_080f6536:
	movs	r0, #228
	bl	sub_080772e0
	movs	r3, #152
	adds	r3, r3, r7
	ldr	r1, [pc, #168]
	mov	r9, r0
	ldr	r0, [r3, #0]
	mov	fp, r3
	mov	r4, r9
	adds	r5, r7, r1
	mov	r3, sl
	subs	r0, r4, r0
	ldr	r2, [r5, #0]
	movs	r1, #2
	str	r3, [sp, #0]
	movs	r3, #64
	bl	sub_080150b0
	mov	r4, fp
	movs	r3, #8
	ldr	r2, [r5, #0]
	ldr	r0, [r4, #0]
	movs	r1, #2
	str	r3, [sp, #0]
	movs	r3, #64
	bl	sub_080150b0
	ldrh	r2, [r6, #0]
	movs	r3, #2
	ands	r3, r2
	mov	r8, fp
	cmp	r3, #0
	beq.n	.L_080f65a4
	ldr	r0, [sp, #12]
	ldr	r2, [pc, #112]
	movs	r1, #144
	movs	r3, #10
	lsls	r1, r1, #1
	str	r3, [r0, #0]
	adds	r2, r2, r1
	movs	r3, #254
	strb	r3, [r2, #0]
	movs	r2, #153
	lsls	r2, r2, #3
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	sub_08015018
	movs	r3, #148
	adds	r3, r3, r7
	mov	r8, r3
	bl	.L_080f6e26
.L_080f65a4:
	movs	r3, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f65ca
	mov	r4, fp
	ldr	r3, [r4, #0]
	cmp	r3, #3
	bgt.n	.L_080f65c4
	cmp	r9, r3
	ble.n	.L_080f65c4
	adds	r3, #1
	str	r3, [r4, #0]
	movs	r0, #111
	bl	sub_080f9010
	b.n	.L_080f65ca
.L_080f65c4:
	movs	r0, #113
	bl	sub_080f9010
.L_080f65ca:
	ldrh	r2, [r6, #0]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f65fa
	mov	r0, r8
	ldr	r3, [r0, #0]
	cmp	r3, #1
	ble.n	.L_080f65f4
	subs	r3, #1
	str	r3, [r0, #0]
	movs	r0, #111
	bl	sub_080f9010
	b.n	.L_080f65fa
	.4byte 0x03001d20
	.4byte 0x000004cc
	.2byte 0x024c
	.2byte 0x0200
.L_080f65f4:
	movs	r0, #113
	bl	sub_080f9010
.L_080f65fa:
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #52]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, #2
	strh	r3, [r2, #0]
	movs	r1, #1
	ldrh	r2, [r6, #0]
	adds	r3, r1, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080f6614
	b.n	.L_080f6dd2
.L_080f6614:
	ldr	r2, [sp, #24]
	str	r1, [r2, #0]
	ldr	r3, [sp, #36]
	ldr	r4, [pc, #36]
	movs	r0, #153
	adds	r2, r3, r4
	lsls	r0, r0, #3
	movs	r3, #0
	str	r3, [r2, #0]
	adds	r3, r7, r0
	ldr	r0, [r3, #0]
	bl	sub_08015018
	mov	r1, r8
	ldr	r3, [r1, #0]
	b.n	.L_080f6644
	.4byte 0x00003fd0
	.4byte 0x00000010
	.4byte 0x04000050
	.2byte 0x778c
	.2byte 0x0000
.L_080f6644:
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_080f665a
.L_080f664a:
	movs	r0, #228
	bl	sub_08077048
	mov	r2, r8
	ldr	r3, [r2, #0]
	adds	r5, #1
	cmp	r5, r3
	bne.n	.L_080f664a
.L_080f665a:
	ldr	r4, [pc, #788]
	adds	r3, r7, r4
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	sub_08015018
	movs	r0, #152
	lsls	r0, r0, #1
	bl	sub_080f9010
	b.n	.L_080f6dd2
.L_080f6670:
	cmp	r5, #5
	beq.n	.L_080f6676
	b.n	.L_080f691c
.L_080f6676:
	adds	r4, r7, #0
	movs	r1, #0
	adds	r4, #168
	mov	r9, r1
	adds	r1, r4, #0
	ldr	r3, [r1, #0]
	adds	r3, #1
	str	r3, [r1, #0]
	ldrb	r3, [r7, #25]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_080f66a0
	adds	r2, r7, #0
	adds	r2, #25
.L_080f6692:
	adds	r5, #1
	cmp	r5, #5
	beq.n	.L_080f66a4
	adds	r2, #28
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_080f6692
.L_080f66a0:
	cmp	r5, #5
	bne.n	.L_080f66a8
.L_080f66a4:
	movs	r2, #1
	mov	r9, r2
.L_080f66a8:
	ldrh	r2, [r6, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f675a
	movs	r2, #0
	str	r2, [r1, #0]
	ldr	r4, [sp, #36]
	ldr	r0, [pc, #696]
	adds	r3, r4, r0
	str	r2, [r3, #0]
	movs	r1, #148
	adds	r1, r1, r7
	ldr	r3, [r1, #0]
	mov	r8, r1
	mov	sl, r8
	cmp	r3, #4
	bne.n	.L_080f66f4
	adds	r3, r7, #0
	adds	r3, #144
	str	r2, [r1, #0]
	str	r2, [r3, #0]
	ldr	r3, [sp, #24]
	str	r2, [r3, #0]
	adds	r2, r7, #0
	movs	r6, #0
	movs	r0, #0
	movs	r1, #255
	adds	r2, #24
.L_080f66e2:
	ldrb	r3, [r2, #2]
	adds	r6, #1
	orrs	r3, r1
	strb	r0, [r2, #1]
	strb	r3, [r2, #2]
	adds	r2, #28
	cmp	r6, #5
	bne.n	.L_080f66e2
	b.n	.L_080f67ac
.L_080f66f4:
	adds	r5, r7, #0
	adds	r5, #144
	ldr	r3, [r5, #0]
	cmp	r3, #4
	bgt.n	.L_080f671a
	ldr	r0, [pc, #632]
	bl	sub_080f9010
	ldr	r2, [r5, #0]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, #24
	adds	r3, r7, r3
	ldrb	r2, [r3, #1]
	movs	r1, #1
	eors	r2, r1
	strb	r2, [r3, #1]
	b.n	.L_080f67ac
.L_080f671a:
	mov	r4, r9
	cmp	r4, #0
	bne.n	.L_080f6752
	movs	r0, #152
	lsls	r0, r0, #1
	bl	sub_080f9010
	ldr	r0, [sp, #24]
	mov	r1, r9
	movs	r3, #1
	adds	r2, r7, #0
	str	r3, [r0, #0]
	movs	r6, #0
	str	r1, [r5, #0]
	adds	r2, #24
	movs	r1, #255
.L_080f673a:
	ldrb	r3, [r2, #2]
	adds	r6, #1
	orrs	r3, r1
	strb	r3, [r2, #2]
	adds	r2, #28
	cmp	r6, #5
	bne.n	.L_080f673a
	mov	r2, sl
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	b.n	.L_080f67ac
.L_080f6752:
	movs	r0, #113
	bl	sub_080f9010
	b.n	.L_080f67ac
.L_080f675a:
	mov	r3, r8
	ldrh	r2, [r3, #0]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f6780
	adds	r5, r7, #0
	adds	r5, #144
	ldr	r0, [r5, #0]
	movs	r1, #6
	adds	r0, #1
	bl	sub_080022fc
	str	r0, [r5, #0]
	movs	r0, #111
	bl	sub_080f9010
	mov	r4, r8
	ldrh	r2, [r4, #0]
.L_080f6780:
	movs	r3, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f67a6
	adds	r5, r7, #0
	adds	r5, #144
	ldr	r0, [r5, #0]
	movs	r1, #6
	adds	r0, #5
	bl	sub_080022fc
	str	r0, [r5, #0]
	movs	r0, #111
	bl	sub_080f9010
	movs	r0, #148
	adds	r0, r0, r7
	mov	r8, r0
	b.n	.L_080f67ac
.L_080f67a6:
	movs	r1, #148
	adds	r1, r1, r7
	mov	r8, r1
.L_080f67ac:
	ldr	r2, [sp, #24]
	ldr	r2, [r2, #0]
	mov	sl, r2
	cmp	r2, #5
	beq.n	.L_080f67b8
	b.n	.L_080f6906
.L_080f67b8:
	adds	r3, r7, #0
	adds	r3, #144
	ldr	r2, [r3, #0]
	cmp	r2, #5
	bne.n	.L_080f6870
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L_080f682c
	movs	r4, #195
	lsls	r4, r4, #3
	adds	r6, r7, r4
	ldr	r2, [r6, #0]
	subs	r3, r2, #1
	cmp	r3, #1
	bls.n	.L_080f6808
	movs	r0, #153
	lsls	r0, r0, #3
	adds	r5, r7, r0
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl	sub_08015018
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #19
	movs	r3, #4
	movs	r0, #11
	bl	sub_08015010
	adds	r1, r0, #0
	str	r1, [r5, #0]
	movs	r3, #0
	ldr	r0, [pc, #384]
	movs	r2, #0
	bl	sub_08015080
	movs	r1, #152
	movs	r3, #1
	b.n	.L_080f6868
.L_080f6808:
	cmp	r2, #1
	beq.n	.L_080f680e
	b.n	.L_080f6dda
.L_080f680e:
	movs	r2, #153
	lsls	r2, r2, #3
	adds	r3, r7, r2
	ldr	r1, [r3, #0]
	ldr	r0, [pc, #360]
	movs	r3, #8
	movs	r2, #0
	bl	sub_08015080
	movs	r3, #2
	str	r3, [r6, #0]
	movs	r3, #152
	adds	r3, r3, r7
	mov	fp, r3
	b.n	.L_080f6e26
.L_080f682c:
	movs	r4, #195
	lsls	r4, r4, #3
	adds	r6, r7, r4
	ldr	r3, [r6, #0]
	cmp	r3, #3
	beq.n	.L_080f6864
	movs	r0, #153
	lsls	r0, r0, #3
	adds	r5, r7, r0
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl	sub_08015018
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #14
	movs	r3, #3
	movs	r0, #16
	bl	sub_08015010
	adds	r1, r0, #0
	str	r1, [r5, #0]
	ldr	r0, [pc, #296]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08015080
.L_080f6864:
	movs	r1, #152
	movs	r3, #3
.L_080f6868:
	adds	r1, r1, r7
	str	r3, [r6, #0]
	mov	fp, r1
	b.n	.L_080f6e26
.L_080f6870:
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r7
	ldrb	r3, [r3, #25]
	cmp	r3, #0
	bne.n	.L_080f68c2
	movs	r2, #195
	lsls	r2, r2, #3
	adds	r6, r7, r2
	ldr	r3, [r6, #0]
	cmp	r3, #4
	beq.n	.L_080f68b6
	movs	r3, #153
	lsls	r3, r3, #3
	adds	r5, r7, r3
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl	sub_08015018
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #7
	movs	r3, #3
	movs	r0, #23
	bl	sub_08015010
	adds	r1, r0, #0
	str	r1, [r5, #0]
	ldr	r0, [pc, #216]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08015080
.L_080f68b6:
	movs	r4, #152
	movs	r3, #4
	adds	r4, r4, r7
	str	r3, [r6, #0]
	mov	fp, r4
	b.n	.L_080f6e26
.L_080f68c2:
	movs	r0, #195
	lsls	r0, r0, #3
	adds	r6, r7, r0
	ldr	r3, [r6, #0]
	cmp	r3, #5
	beq.n	.L_080f68fa
	movs	r1, #153
	lsls	r1, r1, #3
	adds	r5, r7, r1
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl	sub_08015018
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #7
	movs	r3, #3
	movs	r0, #23
	bl	sub_08015010
	adds	r1, r0, #0
	str	r1, [r5, #0]
	ldr	r0, [pc, #152]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08015080
.L_080f68fa:
	movs	r3, #152
	mov	r2, sl
	adds	r3, r3, r7
	str	r2, [r6, #0]
	mov	fp, r3
	b.n	.L_080f6e26
.L_080f6906:
	movs	r4, #153
	lsls	r4, r4, #3
	adds	r3, r7, r4
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	sub_08015018
	movs	r0, #152
	adds	r0, r0, r7
	mov	fp, r0
	b.n	.L_080f6e26
.L_080f691c:
	cmp	r5, #2
	bne.n	.L_080f6998
	adds	r4, r7, #0
	adds	r4, #168
	ldr	r3, [r4, #0]
	movs	r1, #0
	adds	r3, #1
	str	r3, [r4, #0]
	str	r1, [sp, #28]
	cmp	r3, #60
	beq.n	.L_080f6934
	b.n	.L_080f6de2
.L_080f6934:
	ldr	r2, [sp, #12]
	movs	r3, #3
	str	r3, [r2, #0]
	movs	r0, #93
	str	r4, [sp, #8]
	bl	sub_080f9010
	ldr	r3, [sp, #28]
	ldr	r4, [sp, #8]
	ldr	r2, [pc, #72]
	str	r3, [r4, #0]
	ldr	r3, [pc, #28]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r4, [sp, #36]
	movs	r0, #239
	lsls	r0, r0, #7
	ldr	r1, [pc, #56]
	adds	r3, r4, r0
	str	r5, [r3, #0]
	adds	r2, r4, r1
	movs	r3, #75
	str	r3, [r2, #0]
	b.n	.L_080f6de2
	.4byte 0x00003f44
	.4byte 0x00001010
	.4byte 0x000004cc
	.4byte 0x0000778c
	.4byte 0x00000131
	.4byte 0x00000912
	.4byte 0x00000913
	.4byte 0x0000090f
	.4byte 0x0000090d
	.4byte 0x0000090e
	.4byte 0x04000050
	.2byte 0x7784
	.2byte 0x0000
.L_080f6998:
	cmp	r5, #3
	bne.n	.L_080f69c4
	adds	r4, r7, #0
	adds	r4, #168
	ldr	r3, [r4, #0]
	adds	r3, #1
	str	r3, [r4, #0]
	movs	r4, #0
	str	r4, [sp, #28]
	ldrh	r2, [r6, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080f69b6
	b.n	.L_080f6df0
.L_080f69b6:
	ldr	r0, [sp, #12]
	movs	r3, #10
	str	r3, [r0, #0]
	movs	r0, #112
	bl	sub_080f9010
	b.n	.L_080f6dfe
.L_080f69c4:
	cmp	r5, #11
	bne.n	.L_080f6a16
	movs	r3, #195
	lsls	r3, r3, #3
	adds	r5, r7, r3
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_080f69ea
	movs	r4, #153
	movs	r3, #1
	lsls	r4, r4, #3
	str	r3, [r5, #0]
	adds	r3, r7, r4
	ldr	r1, [r3, #0]
	ldr	r0, [pc, #844]
	movs	r2, #0
	movs	r3, #8
	bl	sub_08015080
.L_080f69ea:
	ldrh	r2, [r6, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080f69f6
	b.n	.L_080f6dfe
.L_080f69f6:
	ldr	r0, [sp, #12]
	movs	r3, #5
	mov	r1, sl
	str	r3, [r0, #0]
	str	r1, [r5, #0]
	movs	r0, #112
	bl	sub_080f9010
	movs	r2, #153
	lsls	r2, r2, #3
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	sub_08015018
	b.n	.L_080f6e0c
.L_080f6a16:
	cmp	r5, #20
	bne.n	.L_080f6a4e
	adds	r4, r7, #0
	adds	r4, #168
	ldr	r3, [r4, #0]
	adds	r3, #1
	str	r3, [r4, #0]
	cmp	r3, #45
	beq.n	.L_080f6a2a
	b.n	.L_080f6e0c
.L_080f6a2a:
	ldr	r0, [sp, #12]
	movs	r1, #152
	movs	r2, #148
	movs	r3, #10
	adds	r1, r1, r7
	adds	r2, r2, r7
	str	r3, [r0, #0]
	b.n	.L_080f6e06
.L_080f6a3a:
	bl	sub_08004458
	movs	r3, #3
	ands	r0, r3
	adds	r0, #4
	strb	r0, [r5, #2]
	ldr	r0, [pc, #748]
	bl	sub_080f9010
	b.n	.L_080f6adc
.L_080f6a4e:
	cmp	r5, #10
	bne.n	.L_080f6a54
	b.n	.L_080f6e1a
.L_080f6a54:
	adds	r4, r7, #0
	adds	r4, #168
	str	r4, [sp, #20]
	ldr	r3, [r4, #0]
	cmp	r3, #4
	bne.n	.L_080f6a8a
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #17
	movs	r2, #12
	movs	r3, #3
	movs	r0, #18
	str	r4, [sp, #8]
	bl	sub_08015010
	adds	r1, r0, #0
	movs	r0, #153
	lsls	r0, r0, #3
	adds	r3, r7, r0
	str	r1, [r3, #0]
	ldr	r0, [pc, #696]
	movs	r3, #0
	movs	r2, #0
	bl	sub_08015080
	ldr	r4, [sp, #8]
	ldr	r3, [r4, #0]
.L_080f6a8a:
	cmp	r3, #16
	bne.n	.L_080f6a9c
	movs	r0, #153
	lsls	r0, r0, #1
	str	r4, [sp, #8]
	bl	sub_080f9010
	ldr	r4, [sp, #8]
	ldr	r3, [r4, #0]
.L_080f6a9c:
	cmp	r3, #56
	ble.n	.L_080f6adc
	ldr	r3, [pc, #664]
	ldr	r2, [sp, #36]
	adds	r1, r2, r3
	ldr	r3, [r1, #0]
	cmp	r3, #31
	bgt.n	.L_080f6ab8
	ldrh	r2, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f6adc
.L_080f6ab8:
	mov	r4, sl
	movs	r2, #1
	adds	r5, r7, #0
	str	r4, [r1, #0]
	movs	r6, #0
	negs	r2, r2
	adds	r5, #24
.L_080f6ac6:
	ldrb	r3, [r5, #1]
	cmp	r3, #0
	bne.n	.L_080f6ad4
	movs	r3, #2
	ldrsb	r3, [r5, r3]
	cmp	r3, r2
	beq.n	.L_080f6a3a
.L_080f6ad4:
	adds	r6, #1
	adds	r5, #28
	cmp	r6, #5
	bne.n	.L_080f6ac6
.L_080f6adc:
	adds	r2, r7, #0
	movs	r6, #0
	adds	r2, #24
.L_080f6ae2:
	movs	r3, #2
	ldrsb	r3, [r2, r3]
	ldrb	r1, [r2, #2]
	cmp	r3, #0
	ble.n	.L_080f6af0
	subs	r3, r1, #1
	strb	r3, [r2, #2]
.L_080f6af0:
	adds	r6, #1
	adds	r2, #28
	cmp	r6, #5
	bne.n	.L_080f6ae2
	adds	r2, r7, #0
	movs	r1, #0
	movs	r6, #0
	movs	r4, #15
	adds	r2, #24
	movs	r0, #0
.L_080f6b04:
	ldrb	r3, [r2, #1]
	cmp	r3, #1
	beq.n	.L_080f6b1a
	movs	r3, #2
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	bne.n	.L_080f6b1c
	ldr	r3, [r0, r7]
	ands	r3, r4
	cmp	r3, #8
	bne.n	.L_080f6b1c
.L_080f6b1a:
	adds	r1, #1
.L_080f6b1c:
	adds	r6, #1
	adds	r2, #28
	adds	r0, #28
	cmp	r6, #5
	bne.n	.L_080f6b04
	cmp	r1, #5
	beq.n	.L_080f6b2c
	b.n	.L_080f6d5c
.L_080f6b2c:
	movs	r0, #0
	movs	r1, #152
	movs	r2, #172
	adds	r1, r1, r7
	adds	r2, r2, r7
	str	r0, [sp, #16]
	mov	sl, r0
	mov	fp, r1
	mov	r8, r2
.L_080f6b3e:
	movs	r3, #0
	mov	r0, r8
	str	r3, [r0, #0]
	mov	r1, fp
	ldr	r2, [r1, #0]
	mov	r9, r3
	movs	r3, #3
	movs	r4, #1
	subs	r3, r3, r2
	negs	r4, r4
	cmp	sl, r3
	ble.n	.L_080f6bf0
	adds	r3, r2, #3
	cmp	sl, r3
	bge.n	.L_080f6bf0
	movs	r6, #0
	movs	r5, #0
.L_080f6b60:
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L_080f6b76
	ldr	r0, [r5, r7]
	cmp	r0, #0
	bge.n	.L_080f6b6e
	adds	r0, #15
.L_080f6b6e:
	asrs	r0, r0, #4
	subs	r0, r6, r0
	adds	r0, #22
	b.n	.L_080f6b9e
.L_080f6b76:
	mov	r3, sl
	cmp	r3, #6
	bne.n	.L_080f6b8e
	ldr	r0, [r5, r7]
	negs	r3, r6
	cmp	r0, #0
	bge.n	.L_080f6b86
	adds	r0, #15
.L_080f6b86:
	asrs	r0, r0, #4
	subs	r0, r3, r0
	adds	r0, #26
	b.n	.L_080f6b9e
.L_080f6b8e:
	ldr	r0, [r5, r7]
	cmp	r0, #0
	bge.n	.L_080f6b96
	adds	r0, #15
.L_080f6b96:
	mov	r1, sl
	asrs	r0, r0, #4
	subs	r0, r1, r0
	adds	r0, #21
.L_080f6b9e:
	movs	r1, #21
	str	r4, [sp, #8]
	bl	sub_080022fc
	adds	r0, r0, r5
	adds	r0, #4
	ldrb	r3, [r7, r0]
	ldr	r4, [sp, #8]
	cmp	r3, #5
	beq.n	.L_080f6bc6
	movs	r2, #1
	negs	r2, r2
	cmp	r4, r2
	bne.n	.L_080f6bbe
	adds	r4, r3, #0
	b.n	.L_080f6bc6
.L_080f6bbe:
	cmp	r4, r3
	beq.n	.L_080f6bc6
	movs	r3, #1
	mov	r9, r3
.L_080f6bc6:
	adds	r6, #1
	adds	r5, #28
	cmp	r6, #5
	bne.n	.L_080f6b60
	mov	r0, r9
	cmp	r0, #0
	bne.n	.L_080f6bf0
	movs	r3, #1
	mov	r1, r8
	str	r3, [r1, #0]
	ldr	r3, [sp, #16]
	movs	r0, #144
	lsls	r0, r0, #1
	adds	r2, r3, r0
	ldr	r3, [pc, #348]
	ldr	r1, [pc, #348]
	ldrb	r3, [r3, r4]
	strb	r3, [r1, r2]
	ldr	r2, [sp, #16]
	adds	r2, #1
	str	r2, [sp, #16]
.L_080f6bf0:
	movs	r4, #1
	add	sl, r4
	movs	r3, #4
	mov	r0, sl
	add	r8, r3
	cmp	r0, #7
	bne.n	.L_080f6b3e
	ldr	r1, [sp, #20]
	movs	r5, #0
	str	r5, [r1, #0]
	ldr	r2, [sp, #16]
	cmp	r2, #0
	beq.n	.L_080f6c52
	movs	r3, #144
	subs	r4, #2
	ldr	r0, [pc, #308]
	lsls	r3, r3, #1
	adds	r2, r2, r3
	adds	r3, r4, #0
	strb	r3, [r0, r2]
	ldr	r1, [sp, #24]
	movs	r3, #2
	str	r3, [r1, #0]
	movs	r0, #171
	bl	sub_080f9010
	movs	r4, #239
	ldr	r3, [sp, #36]
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r0, [sp, #36]
	ldr	r1, [pc, #276]
	adds	r3, r0, r1
	str	r5, [r3, #0]
	movs	r2, #153
	ldr	r3, [pc, #272]
	lsls	r2, r2, #3
	strh	r5, [r3, #0]
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	sub_08015018
	movs	r3, #148
	adds	r3, r3, r7
	mov	r8, r3
	b.n	.L_080f6d68
.L_080f6c52:
	ldr	r4, [sp, #24]
	movs	r3, #11
	str	r3, [r4, #0]
	movs	r0, #195
	movs	r2, #153
	ldr	r1, [sp, #16]
	lsls	r2, r2, #3
	lsls	r0, r0, #3
	adds	r5, r7, r2
	adds	r3, r7, r0
	str	r1, [r3, #0]
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl	sub_08015018
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #16
	movs	r2, #24
	movs	r3, #4
	movs	r0, #3
	bl	sub_08015010
	adds	r1, r0, #0
	str	r1, [r5, #0]
	movs	r3, #0
	ldr	r0, [pc, #200]
	movs	r2, #0
	bl	sub_08015080
	movs	r3, #148
	adds	r3, r3, r7
	mov	r8, r3
	mov	r5, r8
	ldr	r3, [r5, #0]
	cmp	r3, #4
	bne.n	.L_080f6d68
	movs	r0, #228
	bl	sub_080772e0
	cmp	r0, #0
	ble.n	.L_080f6cae
	ldr	r4, [sp, #24]
	movs	r3, #20
	str	r3, [r4, #0]
	b.n	.L_080f6cb4
.L_080f6cae:
	ldr	r1, [sp, #24]
	movs	r3, #20
	str	r3, [r1, #0]
.L_080f6cb4:
	mov	r2, fp
	ldr	r3, [r2, #0]
	cmp	r3, r0
	ble.n	.L_080f6cbe
	str	r0, [r2, #0]
.L_080f6cbe:
	adds	r3, r7, #0
	movs	r2, #0
	adds	r3, #144
	str	r2, [r5, #0]
	str	r2, [r3, #0]
	adds	r2, r7, #0
	movs	r6, #0
	movs	r0, #0
	movs	r1, #255
	adds	r2, #24
.L_080f6cd2:
	ldrb	r3, [r2, #2]
	adds	r6, #1
	orrs	r3, r1
	strb	r0, [r2, #1]
	strb	r3, [r2, #2]
	adds	r2, #28
	cmp	r6, #5
	bne.n	.L_080f6cd2
	ldr	r3, [sp, #36]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r0, [sp, #36]
	ldr	r1, [pc, #84]
	movs	r2, #0
	adds	r3, r0, r1
	str	r2, [r3, #0]
	ldr	r3, [pc, #80]
	strh	r2, [r3, #0]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #12
	movs	r3, #4
	movs	r0, #18
	bl	sub_08015010
	ldr	r2, [pc, #68]
	ldr	r5, [pc, #72]
	adds	r1, r0, #0
	adds	r6, r7, r2
	adds	r0, r5, #0
	str	r1, [r6, #0]
	movs	r2, #0
	movs	r3, #8
	subs	r5, #1
	bl	sub_08015080
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08015080
	b.n	.L_080f6d68
	.4byte 0x0000090c
	.4byte 0x00000133
	.4byte 0x0000090a
	.4byte 0x0000778c
	.4byte 0x080f870c
	.4byte 0x0200024c
	.4byte 0x00007784
	.4byte 0x04000050
	.4byte 0x0000090b
	.4byte 0x000004cc
	.2byte 0x0905
	.2byte 0x0000
.L_080f6d5c:
	movs	r3, #152
	movs	r4, #148
	adds	r3, r3, r7
	adds	r4, r4, r7
	mov	fp, r3
	mov	r8, r4
.L_080f6d68:
	ldr	r0, [sp, #24]
	ldr	r3, [r0, #0]
	cmp	r3, #1
	bne.n	.L_080f6dac
	movs	r6, #0
	adds	r1, r7, #0
.L_080f6d74:
	ldrb	r3, [r1, #25]
	cmp	r3, #0
	bne.n	.L_080f6da4
	movs	r3, #26
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	beq.n	.L_080f6d86
	ldr	r3, [r1, #0]
	b.n	.L_080f6d92
.L_080f6d86:
	ldr	r2, [r1, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #8
	beq.n	.L_080f6d98
	adds	r3, r2, #0
.L_080f6d92:
	adds	r3, #8
	str	r3, [r1, #0]
	adds	r2, r3, #0
.L_080f6d98:
	movs	r3, #168
	lsls	r3, r3, #1
	cmp	r2, r3
	bne.n	.L_080f6da4
	movs	r3, #0
	str	r3, [r1, #0]
.L_080f6da4:
	adds	r6, #1
	adds	r1, #28
	cmp	r6, #5
	bne.n	.L_080f6d74
.L_080f6dac:
	ldr	r4, [sp, #36]
	ldr	r0, [pc, #856]
	adds	r3, r4, r0
	ldr	r2, [r3, #0]
	adds	r2, #1
	str	r2, [r3, #0]
	ldr	r1, [sp, #20]
	ldr	r3, [r1, #0]
	adds	r3, #1
	str	r3, [r1, #0]
	b.n	.L_080f6e26
.L_080f6dc2:
	adds	r2, r7, #0
	movs	r3, #152
	movs	r4, #148
	adds	r2, #140
	adds	r3, r3, r7
	adds	r4, r4, r7
	str	r2, [sp, #12]
	b.n	.L_080f6e14
.L_080f6dd2:
	movs	r0, #148
	adds	r0, r0, r7
	mov	r8, r0
	b.n	.L_080f6e26
.L_080f6dda:
	movs	r1, #152
	adds	r1, r1, r7
	mov	fp, r1
	b.n	.L_080f6e26
.L_080f6de2:
	movs	r2, #152
	movs	r3, #148
	adds	r2, r2, r7
	adds	r3, r3, r7
	mov	fp, r2
	mov	r8, r3
	b.n	.L_080f6e26
.L_080f6df0:
	movs	r4, #152
	movs	r0, #148
	adds	r4, r4, r7
	adds	r0, r0, r7
	mov	fp, r4
	mov	r8, r0
	b.n	.L_080f6e26
.L_080f6dfe:
	movs	r1, #152
	movs	r2, #148
	adds	r1, r1, r7
	adds	r2, r2, r7
.L_080f6e06:
	mov	fp, r1
	mov	r8, r2
	b.n	.L_080f6e26
.L_080f6e0c:
	movs	r3, #152
	movs	r4, #148
	adds	r3, r3, r7
	adds	r4, r4, r7
.L_080f6e14:
	mov	fp, r3
	mov	r8, r4
	b.n	.L_080f6e26
.L_080f6e1a:
	movs	r0, #152
	movs	r1, #148
	adds	r0, r0, r7
	adds	r1, r1, r7
	mov	fp, r0
	mov	r8, r1
.L_080f6e26:
	ldr	r2, [sp, #12]
	ldr	r3, [r2, #0]
	cmp	r3, #5
	bne.n	.L_080f6eb2
	adds	r3, r7, #0
	adds	r3, #144
	ldr	r1, [r3, #0]
	lsls	r3, r1, #3
	adds	r3, r3, r1
	adds	r4, r7, #0
	lsls	r3, r3, #2
	adds	r4, #168
	adds	r5, r3, #0
	ldr	r3, [r4, #0]
	movs	r2, #15
	ands	r3, r2
	adds	r5, #36
	movs	r6, #128
	movs	r0, #0
	cmp	r3, #7
	bgt.n	.L_080f6e52
	movs	r0, #1
.L_080f6e52:
	cmp	r1, #5
	bne.n	.L_080f6e5a
	movs	r5, #208
	movs	r6, #32
.L_080f6e5a:
	adds	r3, r5, #0
	ldr	r4, [sp, #28]
	subs	r3, #12
	lsls	r3, r3, #16
	orrs	r3, r4
	adds	r4, r6, #0
	adds	r4, #8
	ldr	r2, [pc, #672]
	orrs	r3, r4
	orrs	r3, r2
	movs	r1, #200
	str	r3, [r7, r1]
	lsls	r1, r0, #4
	movs	r0, #172
	lsls	r0, r0, #2
	adds	r1, r1, r0
	movs	r3, #204
	str	r1, [r7, r3]
	adds	r3, r5, #0
	ldr	r2, [sp, #28]
	adds	r3, #12
	lsls	r3, r3, #16
	orrs	r3, r2
	ldr	r2, [pc, #644]
	orrs	r3, r4
	orrs	r3, r2
	movs	r0, #208
	str	r3, [r7, r0]
	movs	r3, #212
	str	r1, [r7, r3]
	ldr	r4, [sp, #28]
	lsls	r3, r5, #16
	ldr	r2, [pc, #632]
	orrs	r3, r4
	orrs	r3, r6
	orrs	r3, r2
	movs	r1, #216
	str	r3, [r7, r1]
	movs	r3, #248
	movs	r2, #220
	lsls	r3, r3, #1
	movs	r0, #3
	str	r3, [r7, r2]
	str	r0, [sp, #32]
.L_080f6eb2:
	ldr	r1, [sp, #12]
	ldr	r3, [r1, #0]
	cmp	r3, #3
	bne.n	.L_080f6f7e
	ldr	r3, [sp, #36]
	ldr	r0, [sp, #32]
	ldr	r2, [pc, #600]
	mov	lr, r3
	movs	r1, #204
	lsls	r3, r0, #3
	adds	r4, r7, #0
	adds	r1, r1, r3
	adds	r5, r3, #0
	movs	r6, #0
	adds	r4, #168
	mov	sl, r2
	mov	ip, r1
	adds	r5, #200
.L_080f6ed6:
	movs	r0, #225
	lsls	r0, r0, #7
	add	r0, lr
	movs	r3, #2
	ldrsh	r2, [r0, r3]
	ldr	r1, [sp, #28]
	lsls	r2, r2, #16
	orrs	r2, r1
	movs	r1, #6
	ldrsh	r3, [r0, r1]
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #255
	ands	r3, r1
	orrs	r2, r3
	movs	r3, #128
	lsls	r3, r3, #24
	orrs	r2, r3
	str	r2, [r7, r5]
	mov	r2, sl
	ldrb	r3, [r2, r6]
	movs	r1, #220
	lsls	r3, r3, #4
	lsls	r1, r1, #2
	movs	r2, #240
	lsls	r2, r2, #8
	adds	r3, r3, r1
	orrs	r3, r2
	mov	r2, ip
	str	r3, [r7, r2]
	ldr	r2, [r0, #16]
	ldr	r3, [r0, #4]
	adds	r3, r3, r2
	str	r3, [r0, #4]
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r2, r2, r3
	str	r2, [r0, #16]
	ldr	r3, [r4, #0]
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L_080f6f2e
	adds	r2, #255
.L_080f6f2e:
	asrs	r2, r2, #8
	lsls	r2, r2, #8
	subs	r2, r3, r2
	lsls	r3, r6, #2
	adds	r3, #200
	cmp	r2, r3
	bne.n	.L_080f6f46
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r0, #16]
	movs	r3, #0
	str	r3, [r0, #24]
.L_080f6f46:
	movs	r2, #128
	ldr	r3, [r0, #4]
	lsls	r2, r2, #15
	cmp	r3, r2
	ble.n	.L_080f6f68
	ldr	r1, [r0, #24]
	str	r2, [r0, #4]
	cmp	r1, #1
	bgt.n	.L_080f6f64
	ldr	r3, [r0, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r0, #16]
.L_080f6f64:
	adds	r3, r1, #1
	str	r3, [r0, #24]
.L_080f6f68:
	ldr	r1, [sp, #32]
	movs	r0, #8
	adds	r1, #1
	movs	r2, #28
	adds	r6, #1
	add	ip, r0
	adds	r5, #8
	str	r1, [sp, #32]
	add	lr, r2
	cmp	r6, #8
	bne.n	.L_080f6ed6
.L_080f6f7e:
	ldr	r3, [pc, #396]
	ldr	r0, [sp, #32]
	mov	r9, r3
	lsls	r3, r0, #3
	ldr	r4, [pc, #404]
	adds	r1, r3, #0
	adds	r0, r3, #0
	movs	r2, #156
	movs	r3, #157
	adds	r1, #204
	lsls	r2, r2, #3
	lsls	r3, r3, #3
	ldr	r6, [pc, #392]
	mov	ip, r4
	movs	r5, #0
	adds	r4, r1, #0
	adds	r0, #200
	mov	sl, r2
	mov	lr, r3
.L_080f6fa4:
	ldrb	r3, [r6, #0]
	ldr	r2, [sp, #28]
	lsls	r3, r3, #16
	orrs	r3, r2
	mov	r2, ip
	ldrb	r2, [r2, #0]
	str	r2, [sp, #4]
	movs	r2, #1
	add	ip, r2
	ldr	r2, [sp, #4]
	orrs	r3, r2
	mov	r2, r9
	orrs	r3, r2
	adds	r6, #1
	str	r3, [r7, r0]
	cmp	r5, #3
	bgt.n	.L_080f6fcc
	mov	r3, sl
	str	r3, [r7, r4]
	b.n	.L_080f6fd0
.L_080f6fcc:
	mov	r2, lr
	str	r2, [r7, r1]
.L_080f6fd0:
	ldr	r3, [sp, #32]
	adds	r5, #1
	adds	r3, #1
	adds	r1, #8
	adds	r4, #8
	adds	r0, #8
	str	r3, [sp, #32]
	cmp	r5, #14
	bne.n	.L_080f6fa4
	lsls	r3, r3, #3
	ldr	r4, [pc, #316]
	movs	r0, #25
	adds	r1, r3, #0
	adds	r2, r3, #0
	movs	r5, #128
	adds	r0, r0, r7
	adds	r1, #204
	adds	r2, #200
	mov	lr, r4
	movs	r6, #0
	lsls	r5, r5, #14
	mov	ip, r0
	adds	r4, r1, #0
	mov	r9, r2
.L_080f7000:
	mov	r3, ip
	ldrb	r3, [r3, #0]
	mov	sl, r3
	movs	r0, #28
	mov	r3, sl
	add	ip, r0
	cmp	r3, #0
	bne.n	.L_080f7024
	ldr	r3, [sp, #28]
	mov	r0, lr
	orrs	r3, r5
	orrs	r3, r0
	mov	r0, r9
	str	r3, [r7, r0]
	movs	r3, #140
	lsls	r3, r3, #3
	str	r3, [r7, r4]
	b.n	.L_080f7034
.L_080f7024:
	ldr	r3, [sp, #28]
	mov	r0, lr
	orrs	r3, r5
	orrs	r3, r0
	str	r3, [r7, r2]
	movs	r3, #144
	lsls	r3, r3, #3
	str	r3, [r7, r1]
.L_080f7034:
	movs	r3, #8
	ldr	r0, [sp, #32]
	add	r9, r3
	movs	r3, #144
	adds	r0, #1
	lsls	r3, r3, #14
	adds	r6, #1
	adds	r1, #8
	adds	r2, #8
	adds	r4, #8
	str	r0, [sp, #32]
	adds	r5, r5, r3
	cmp	r6, #5
	bne.n	.L_080f7000
	ldr	r1, [sp, #32]
	ldr	r4, [pc, #212]
	lsls	r3, r1, #3
	adds	r2, r3, #0
	movs	r0, #128
	lsls	r0, r0, #3
	mov	sl, r4
	movs	r5, #128
	adds	r2, #204
	adds	r3, #200
	movs	r4, #132
	mov	ip, r0
	movs	r6, #0
	lsls	r5, r5, #14
	adds	r0, r2, #0
	mov	r9, r3
	mov	lr, r8
	lsls	r4, r4, #2
.L_080f7074:
	ldr	r3, [sp, #28]
	mov	r1, sl
	orrs	r3, r5
	orrs	r3, r1
	mov	r1, r9
	str	r3, [r7, r1]
	mov	r1, lr
	ldr	r3, [r1, #0]
	cmp	r6, r3
	bne.n	.L_080f7092
	adds	r3, r4, #0
	mov	r1, ip
	orrs	r3, r1
	str	r3, [r7, r0]
	b.n	.L_080f70a0
.L_080f7092:
	movs	r1, #136
	lsls	r1, r1, #2
	lsls	r3, r6, #5
	adds	r3, r3, r1
	mov	r1, ip
	orrs	r3, r1
	str	r3, [r7, r2]
.L_080f70a0:
	movs	r3, #8
	ldr	r1, [sp, #32]
	add	r9, r3
	movs	r3, #128
	adds	r1, #1
	lsls	r3, r3, #13
	adds	r6, #1
	adds	r2, #8
	adds	r0, #8
	str	r1, [sp, #32]
	adds	r5, r5, r3
	adds	r4, #32
	cmp	r6, #5
	bne.n	.L_080f7074
	lsls	r3, r1, #3
	movs	r4, #0
	mov	sl, r4
	adds	r0, r3, #0
	adds	r4, r3, #0
	movs	r5, #5
	adds	r4, #204
	adds	r0, #200
	adds	r1, r3, #0
.L_080f70ce:
	mov	r3, sl
	movs	r2, #1
	ands	r2, r3
	movs	r3, #129
	lsls	r2, r2, #3
	lsls	r3, r3, #2
	subs	r3, r3, r2
	ldr	r2, [pc, #76]
	ands	r3, r2
	ldr	r2, [sp, #28]
	lsls	r3, r3, #16
	orrs	r3, r2
	ldr	r2, [pc, #44]
	orrs	r3, r5
	orrs	r3, r2
	str	r3, [r7, r0]
	mov	r3, fp
	ldr	r2, [r3, #0]
	movs	r3, #3
	subs	r3, r3, r2
	cmp	sl, r3
	ble.n	.L_080f7130
	adds	r3, r2, #3
	cmp	sl, r3
	bge.n	.L_080f7130
	movs	r3, #186
	lsls	r3, r3, #3
	str	r3, [r7, r4]
	b.n	.L_080f713a
	.4byte 0x0000778c
	.4byte 0x80006000
	.4byte 0x90006000
	.4byte 0x80002000
	.4byte 0x080f8712
	.4byte 0x080f8728
	.4byte 0x080f871a
	.4byte 0x8000207c
	.4byte 0x80006003
	.2byte 0x01ff
	.2byte 0x0000
.L_080f7130:
	adds	r2, r1, #0
	movs	r3, #162
	adds	r2, #204
	lsls	r3, r3, #3
	str	r3, [r7, r2]
.L_080f713a:
	ldr	r2, [sp, #32]
	movs	r3, #1
	adds	r2, #1
	add	sl, r3
	str	r2, [sp, #32]
	mov	r2, sl
	adds	r4, #8
	adds	r0, #8
	adds	r1, #8
	adds	r5, #16
	cmp	r2, #7
	bne.n	.L_080f70ce
	movs	r3, #160
	lsls	r3, r3, #14
	movs	r6, #0
	mov	fp, r3
	adds	r5, r7, #0
.L_080f715c:
	ldr	r0, [sp, #32]
	movs	r4, #0
	lsls	r3, r0, #3
	movs	r1, #204
	mov	sl, r4
	adds	r1, r1, r3
	adds	r4, r3, #0
	mov	r9, fp
	mov	r8, r1
	adds	r4, #200
.L_080f7170:
	mov	r2, sl
	lsls	r1, r2, #4
	ldr	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080f717e
	adds	r3, #15
.L_080f717e:
	asrs	r3, r3, #4
	lsls	r3, r3, #4
	subs	r3, r2, r3
	adds	r3, r1, r3
	mov	r0, r9
	adds	r3, #4
	ldr	r2, [pc, #372]
	orrs	r3, r0
	orrs	r3, r2
	str	r3, [r7, r4]
	ldr	r0, [r5, #0]
	cmp	r0, #0
	bge.n	.L_080f719a
	adds	r0, #15
.L_080f719a:
	mov	r1, sl
	asrs	r0, r0, #4
	subs	r0, r1, r0
	adds	r0, #21
	movs	r1, #21
	str	r4, [sp, #8]
	bl	sub_080022fc
	adds	r0, #4
	ldrb	r3, [r5, r0]
	movs	r2, #128
	lsls	r2, r2, #4
	lsls	r3, r3, #4
	orrs	r3, r2
	mov	r2, r8
	str	r3, [r7, r2]
	ldr	r0, [sp, #32]
	movs	r1, #1
	ldr	r4, [sp, #8]
	add	sl, r1
	movs	r3, #8
	adds	r0, #1
	mov	r2, sl
	add	r8, r3
	adds	r4, #8
	str	r0, [sp, #32]
	cmp	r2, #7
	bne.n	.L_080f7170
	movs	r3, #128
	lsls	r3, r3, #14
	adds	r6, #1
	add	fp, r3
	adds	r5, #28
	cmp	r6, #5
	bne.n	.L_080f715c
	ldr	r0, [sp, #12]
	ldr	r3, [r0, #0]
	movs	r4, #40
	cmp	r3, #1
	bne.n	.L_080f7216
	adds	r3, r7, #0
	adds	r3, #168
	ldr	r0, [r3, #0]
	cmp	r0, #47
	bgt.n	.L_080f7200
	ldr	r3, [pc, #268]
	muls	r0, r3
	bl	sub_08002322
	lsls	r0, r0, #6
	b.n	.L_080f7210
.L_080f7200:
	cmp	r0, #55
	bgt.n	.L_080f7216
	ldr	r1, [pc, #256]
	lsls	r0, r0, #12
	adds	r0, r0, r1
	bl	sub_08002322
	lsls	r0, r0, #2
.L_080f7210:
	asrs	r0, r0, #16
	adds	r4, r0, #0
	adds	r4, #40
.L_080f7216:
	ldr	r3, [sp, #32]
	ldr	r0, [sp, #28]
	lsls	r2, r3, #3
	ldr	r3, [pc, #236]
	orrs	r4, r0
	orrs	r4, r3
	adds	r1, r2, #0
	movs	r3, #160
	adds	r1, #200
	adds	r2, #204
	lsls	r3, r3, #3
	str	r4, [r7, r1]
	str	r3, [r7, r2]
	ldr	r1, [sp, #32]
	adds	r1, #1
	lsls	r3, r1, #3
	adds	r6, r3, #0
	adds	r6, #204
	adds	r4, r6, #0
	adds	r0, r4, #0
	str	r1, [sp, #32]
	movs	r2, #12
	adds	r1, r0, #0
	adds	r3, #200
	movs	r5, #0
	mov	r8, r2
	mov	lr, r1
	mov	ip, r3
.L_080f724e:
	ldr	r3, [sp, #28]
	mov	r2, r8
	orrs	r3, r2
	ldr	r2, [pc, #184]
	orrs	r3, r2
	mov	r2, ip
	str	r3, [r7, r2]
	cmp	r5, #0
	bne.n	.L_080f726a
	movs	r3, #168
	lsls	r3, r3, #3
	mov	r2, lr
	str	r3, [r7, r2]
	b.n	.L_080f7294
.L_080f726a:
	cmp	r5, #1
	bne.n	.L_080f7276
	movs	r3, #170
	lsls	r3, r3, #3
	str	r3, [r7, r1]
	b.n	.L_080f7294
.L_080f7276:
	cmp	r5, #6
	bne.n	.L_080f7282
	movs	r3, #174
	lsls	r3, r3, #3
	str	r3, [r7, r0]
	b.n	.L_080f7294
.L_080f7282:
	cmp	r5, #7
	bne.n	.L_080f728e
	movs	r3, #176
	lsls	r3, r3, #3
	str	r3, [r7, r4]
	b.n	.L_080f7294
.L_080f728e:
	movs	r3, #172
	lsls	r3, r3, #3
	str	r3, [r7, r6]
.L_080f7294:
	ldr	r2, [sp, #32]
	movs	r3, #8
	add	lr, r3
	add	ip, r3
	adds	r2, #1
	movs	r3, #16
	adds	r5, #1
	adds	r6, #8
	adds	r4, #8
	adds	r0, #8
	adds	r1, #8
	str	r2, [sp, #32]
	add	r8, r3
	cmp	r5, #8
	bne.n	.L_080f724e
	cmp	r2, #128
	beq.n	.L_080f72d4
	lsls	r3, r2, #3
	adds	r2, r3, #0
	ldr	r0, [pc, #88]
	movs	r1, #0
	adds	r2, #204
	adds	r3, #200
.L_080f72c2:
	str	r0, [r7, r3]
	str	r1, [r7, r2]
	ldr	r4, [sp, #32]
	adds	r4, #1
	adds	r2, #8
	adds	r3, #8
	str	r4, [sp, #32]
	cmp	r4, #128
	bne.n	.L_080f72c2
.L_080f72d4:
	ldr	r0, [sp, #32]
	movs	r4, #132
	lsls	r2, r0, #1
	lsls	r4, r4, #24
	adds	r0, r7, #0
	movs	r1, #224
	ldr	r3, [pc, #52]
	adds	r0, #200
	lsls	r1, r1, #19
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x80006000
	.4byte 0x000002aa
	.4byte 0xfffd0000
	.4byte 0x80d06000
	.4byte 0x80ce6000
	.4byte 0x40f02000
	.2byte 0x00d4
	.2byte 0x0400
.L_080f731c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #144]
	ldr	r1, [r3, #0]
	subs	r3, #24
	ldr	r3, [r3, #0]
	movs	r2, #128
	mov	sl, r3
	ldr	r3, [pc, #136]
	mov	r8, r1
	sub	sp, #4
	movs	r5, #0
	movs	r6, #0
	movs	r1, #0
	lsls	r2, r2, #4
.L_080f733e:
	adds	r6, #1
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r6, r2
	bne.n	.L_080f733e
	movs	r2, #225
	lsls	r2, r2, #7
	ldr	r0, [pc, #112]
	ldr	r1, [pc, #116]
	ldr	r7, [pc, #116]
	movs	r6, #0
	movs	r4, #0
	add	r2, sl
.L_080f7358:
	adds	r3, r5, #0
	adds	r3, #24
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	ldrb	r3, [r1, #0]
	adds	r6, #1
	str	r0, [r2, #4]
	str	r4, [r2, #16]
	str	r4, [r2, #24]
	adds	r1, #1
	adds	r5, r5, r3
	adds	r0, r0, r7
	adds	r2, #28
	cmp	r6, #8
	bne.n	.L_080f7358
	movs	r3, #155
	lsls	r3, r3, #3
	ldr	r2, [pc, #56]
	movs	r6, #0
	add	r3, r8
.L_080f7380:
	adds	r6, #1
	strh	r2, [r3, #0]
	adds	r3, #2
	cmp	r6, #160
	bne.n	.L_080f7380
	ldr	r7, [pc, #64]
	movs	r6, #0
	add	r7, r8
.L_080f7390:
	ldr	r3, [pc, #60]
	adds	r5, r6, #0
	muls	r5, r3
	adds	r0, r5, #0
	bl	sub_0800231c
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsrs	r3, r3, #15
	strh	r3, [r7, #0]
	adds	r0, r5, #0
	bl	sub_0800231c
	movs	r2, #110
	subs	r2, r2, r6
	movs	r3, #155
	lsls	r3, r3, #3
	b.n	.L_080f73d4
	.4byte 0x00000000
	.4byte 0x03001f04
	.4byte 0x02010018
	.4byte 0xffe00000
	.4byte 0x080f8736
	.4byte 0xfff80000
	.4byte 0x00000506
	.2byte 0x0199
	.2byte 0x0000
.L_080f73d4:
	lsls	r2, r2, #1
	adds	r2, r2, r3
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsrs	r3, r3, #15
	mov	r1, r8
	adds	r6, #1
	adds	r7, #2
	strh	r3, [r1, r2]
	cmp	r6, #40
	bne.n	.L_080f7390
	mov	r3, r8
	movs	r2, #0
	adds	r3, #148
	str	r2, [r3, #0]
	movs	r1, #239
	subs	r3, #8
	str	r2, [r3, #0]
	lsls	r1, r1, #7
	adds	r3, #4
	str	r2, [r3, #0]
	add	r1, sl
	movs	r3, #1
	str	r3, [r1, #0]
	ldr	r3, [pc, #72]
	add	r3, sl
	str	r2, [r3, #0]
	ldr	r3, [pc, #72]
	strh	r2, [r3, #0]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #12
	movs	r3, #4
	movs	r0, #18
	bl	sub_08015010
	ldr	r6, [pc, #56]
	ldr	r5, [pc, #56]
	adds	r1, r0, #0
	add	r6, r8
	str	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #8
	subs	r5, #1
	bl	sub_08015080
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08015080
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00007784
	.4byte 0x04000050
	.4byte 0x000004cc
	.2byte 0x0905
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r1, [pc, #120]
	movs	r0, #41
	sub	sp, #116
	bl	sub_080048b0
	movs	r1, #128
	str	r0, [sp, #44]
	lsls	r1, r1, #8
	movs	r0, #40
	bl	sub_080048b0
	ldr	r1, [pc, #100]
	str	r0, [sp, #40]
	movs	r0, #39
	bl	sub_080048f4
	ldr	r1, [pc, #96]
	str	r0, [sp, #36]
	movs	r0, #45
	bl	sub_080048f4
	str	r0, [sp, #32]
	ldr	r0, [pc, #88]
	ldr	r5, [pc, #88]
	bl	sub_08002f3c
	movs	r0, #144
	lsls	r0, r0, #1
	adds	r5, r5, r0
	movs	r3, #255
	strb	r3, [r5, #0]
	ldr	r2, [sp, #32]
	movs	r3, #0
	adds	r2, #162
	strh	r3, [r2, #0]
	ldr	r1, [sp, #32]
	movs	r3, #1
	adds	r1, #152
	str	r1, [sp, #28]
	str	r3, [r1, #0]
	bl	sub_080040e8
	ldr	r5, [pc, #24]
	ldr	r3, [pc, #52]
	movs	r0, #0
	strb	r5, [r3, #0]
	ldr	r1, [pc, #52]
	ldr	r7, [pc, #16]
	ldr	r4, [pc, #20]
	mov	r8, r0
	movs	r6, #0
.L_080f74d4:
	movs	r2, #0
	mov	sl, r2
	b.n	.L_080f7504
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x0000a1a6
	.4byte 0x0000a1a8
	.4byte 0x0000060e
	.4byte 0x0000782c
	.4byte 0x0000061c
	.4byte 0x0000000c
	.4byte 0x0200024c
	.4byte 0x03001d18
	.2byte 0x2800
	.2byte 0x0600
.L_080f7504:
	mov	r3, sl
	subs	r3, #5
	cmp	r3, #19
	bhi.n	.L_080f751c
	cmp	r6, #2
	ble.n	.L_080f751c
	cmp	r6, #13
	bgt.n	.L_080f751c
	mov	r2, r8
	adds	r3, r2, r1
	strh	r7, [r3, #0]
	b.n	.L_080f7534
.L_080f751c:
	mov	r3, sl
	cmp	r3, #29
	ble.n	.L_080f752a
	mov	r2, r8
	adds	r3, r2, r1
	strh	r5, [r3, #0]
	b.n	.L_080f7534
.L_080f752a:
	mov	r3, r8
	adds	r2, r3, r1
	adds	r3, r0, r4
	strh	r3, [r2, #0]
	adds	r0, #1
.L_080f7534:
	movs	r2, #1
	add	sl, r2
	movs	r3, #2
	mov	r2, sl
	add	r8, r3
	cmp	r2, #32
	bne.n	.L_080f7504
	adds	r6, #1
	cmp	r6, #20
	bne.n	.L_080f74d4
	ldr	r0, [pc, #172]
	bl	sub_08002f40
	ldr	r1, [sp, #44]
	bl	sub_08005340
	ldr	r0, [pc, #164]
	bl	sub_08002f40
	ldr	r3, [pc, #164]
	adds	r4, r0, #0
	ldr	r1, [pc, #164]
	ldr	r2, [pc, #164]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r4, #32
	adds	r0, r4, #0
	ldr	r1, [pc, #160]
	bl	sub_08005340
	ldr	r0, [pc, #152]
	movs	r3, #0
	ldr	r7, [pc, #152]
	mov	r8, r3
	movs	r6, #0
	movs	r5, #0
	mov	ip, r0
.L_080f757e:
	movs	r1, #0
	lsls	r3, r5, #6
	mov	r2, ip
	mov	sl, r1
	adds	r4, r3, r2
.L_080f7588:
	mov	r3, sl
	subs	r3, #5
	cmp	r3, #19
	bhi.n	.L_080f7598
	cmp	r6, #2
	ble.n	.L_080f7598
	cmp	r6, #13
	ble.n	.L_080f75aa
.L_080f7598:
	mov	r2, r8
	adds	r1, r2, r7
	ldr	r3, [pc, #96]
	adds	r0, r4, #0
	ldr	r2, [pc, #100]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #32
	add	r8, r3
.L_080f75aa:
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	adds	r4, #32
	cmp	r1, #30
	bne.n	.L_080f7588
	adds	r6, #1
	adds	r5, #15
	cmp	r6, #20
	bne.n	.L_080f757e
	movs	r1, #192
	ldr	r3, [pc, #80]
	ldr	r0, [pc, #84]
	lsls	r1, r1, #2
	bl	sub_080072f0
	ldr	r3, [pc, #40]
	movs	r2, #0
	movs	r4, #2
	ldr	r5, [pc, #72]
	mov	r8, r2
	movs	r6, #0
	mov	ip, r3
	negs	r4, r4
	movs	r0, #0
.L_080f75dc:
	movs	r7, #0
	adds	r2, r0, #0
	mov	sl, r7
	adds	r1, r4, #0
	adds	r2, #148
.L_080f75e6:
	cmp	r1, #14
	bls.n	.L_080f7620
	mov	r7, r8
	adds	r3, r7, r5
	mov	r7, ip
	strh	r7, [r3, #0]
	b.n	.L_080f7626
	.4byte 0x000000bf
	.4byte 0x00000076
	.4byte 0x0000003f
	.4byte 0x040000d4
	.4byte 0x05000140
	.4byte 0x84000008
	.2byte 0x0000
	.2byte 0x0201
	push	{lr}
	lsls	r0, r0, #24
	lsls	r4, r4, #5
	lsls	r0, r0, #12
	cmp	r5, #0
	lsls	r0, r0, #24
	adds	r0, #0
	lsls	r0, r0, #24
.L_080f7620:
	mov	r7, r8
	adds	r3, r7, r5
	strh	r2, [r3, #0]
.L_080f7626:
	movs	r3, #1
	add	sl, r3
	movs	r7, #2
	mov	r3, sl
	adds	r2, #1
	add	r8, r7
	cmp	r3, #32
	bne.n	.L_080f75e6
	adds	r6, #1
	adds	r4, #1
	adds	r0, #32
	cmp	r6, #20
	bne.n	.L_080f75dc
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #64]
	movs	r1, #0
	strh	r1, [r3, #0]
	strh	r1, [r3, #2]
	strh	r1, [r3, #4]
	strh	r1, [r3, #6]
	strh	r1, [r3, #8]
	strh	r1, [r3, #10]
	ldr	r3, [pc, #28]
	adds	r2, #60
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #24]
	adds	r2, #6
	strh	r3, [r2, #0]
	ldr	r3, [pc, #24]
	adds	r2, #2
	b.n	.L_080f7694
	.4byte 0x00000509
	.4byte 0x00000680
	.4byte 0x00003737
	.4byte 0x00002727
	.4byte 0x00003f44
	.4byte 0x00001010
	.4byte 0x0400000a
	.2byte 0x1ad0
	.2byte 0x0300
.L_080f7694:
	strh	r3, [r2, #0]
	ldr	r3, [pc, #76]
	ldr	r2, [pc, #52]
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r1, [r3, #0]
	subs	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	ldr	r0, [pc, #60]
	ldr	r3, [pc, #40]
	ldr	r4, [pc, #60]
	strh	r3, [r0, #0]
	ldr	r2, [pc, #36]
	ldr	r3, [pc, #40]
	strh	r3, [r4, #0]
	strh	r2, [r0, #0]
	ldr	r3, [pc, #52]
	ldr	r0, [pc, #36]
	strh	r0, [r4, #0]
	strh	r2, [r3, #0]
	adds	r3, #4
	strh	r0, [r3, #0]
	ldr	r7, [sp, #36]
	ldr	r2, [sp, #32]
	movs	r0, #128
	lsls	r0, r0, #2
	b.n	.L_080f76f4
	movs	r0, r0
	.4byte 0x0000ff60
	.4byte 0x000028c8
	.4byte 0x000000f0
	.4byte 0x00001878
	.4byte 0x000000a0
	.4byte 0x04000014
	.4byte 0x04000040
	.4byte 0x04000044
	.2byte 0x0042
	.2byte 0x0400
.L_080f76f4:
	adds	r0, r7, r0
	adds	r2, #140
	str	r0, [sp, #24]
	str	r2, [sp, #20]
	str	r1, [r2, #0]
	ldr	r3, [sp, #32]
	adds	r3, #144
	str	r1, [r3, #0]
	ldr	r3, [sp, #32]
	ldr	r0, [pc, #96]
	adds	r3, #148
	str	r1, [r3, #0]
	adds	r3, r7, r0
	str	r1, [r3, #0]
	ldr	r2, [sp, #32]
	adds	r2, #168
	str	r2, [sp, #16]
	str	r1, [r2, #0]
	ldr	r0, [pc, #80]
	bl	sub_08002f40
	movs	r1, #160
	ldr	r3, [pc, #76]
	lsls	r1, r1, #19
	ldr	r2, [pc, #76]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r0, [pc, #68]
	bl	sub_08002f40
	ldr	r3, [pc, #48]
	adds	r4, r0, #0
	ldr	r1, [pc, #60]
	ldr	r2, [pc, #64]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #240
	lsls	r3, r3, #1
	adds	r4, r4, r3
	adds	r0, r4, #0
	ldr	r1, [pc, #52]
	bl	sub_08005340
	ldr	r3, [pc, #24]
	ldr	r0, [pc, #44]
	ldr	r1, [pc, #48]
	ldr	r2, [pc, #48]
	b.n	.L_080f7794
	.4byte 0x00002f8b
	.4byte 0x00005bf6
	.4byte 0x0000778c
	.4byte 0x0000008f
	.4byte 0x040000d4
	.4byte 0x84000020
	.4byte 0x05000080
	.4byte 0x00000040
	.4byte 0x05000200
	.4byte 0x84000078
	.4byte 0x02010000
	.4byte 0x06010000
	.2byte 0x1b30
	.2byte 0x8400
.L_080f7794:
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #156]
	bl	sub_08002f40
	ldr	r3, [pc, #156]
	adds	r4, r0, #0
	ldr	r1, [pc, #156]
	ldr	r2, [pc, #156]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r4, #32
	adds	r0, r4, #0
	ldr	r1, [pc, #152]
	bl	sub_08005340
	ldr	r3, [pc, #132]
	ldr	r0, [pc, #144]
	ldr	r1, [pc, #144]
	ldr	r2, [pc, #148]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	sub_08015000
	bl	.L_080f731c
	movs	r7, #0
	mov	sl, r7
	ldr	r7, [sp, #32]
.L_080f77ce:
	movs	r3, #8
	str	r3, [r7, #0]
	movs	r3, #0
	strb	r3, [r7, #25]
	movs	r3, #255
	strb	r3, [r7, #26]
	movs	r6, #0
	adds	r5, r7, #4
.L_080f77de:
	bl	sub_08004458
	movs	r1, #5
	bl	sub_08002304
	adds	r6, #1
	strb	r0, [r5, #0]
	adds	r5, #1
	cmp	r6, #21
	bne.n	.L_080f77de
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	adds	r7, #28
	cmp	r1, #5
	bne.n	.L_080f77ce
	ldr	r3, [sp, #32]
	movs	r2, #0
	add	r7, sp, #84
	mov	sl, r2
	adds	r4, r7, #0
	mov	r9, r3
.L_080f780a:
	movs	r0, #0
	mov	r8, r0
	movs	r5, #0
.L_080f7810:
	str	r4, [sp, #8]
	bl	sub_08004458
	movs	r1, #21
	bl	sub_08002304
	mov	r1, r8
	str	r0, [r5, r7]
	movs	r6, #0
	ldr	r4, [sp, #8]
	cmp	r1, #0
	beq.n	.L_080f786c
	ldr	r3, [r7, #0]
	cmp	r0, r3
	bne.n	.L_080f7854
	movs	r2, #1
	negs	r2, r2
	subs	r5, #4
	add	r8, r2
	b.n	.L_080f786c
	.4byte 0x00000041
	.4byte 0x040000d4
	.4byte 0x050003e0
	.4byte 0x84000008
	.4byte 0x02010000
	.4byte 0x06016e00
	.2byte 0x0480
	.2byte 0x8400
.L_080f7854:
	adds	r6, #1
	cmp	r6, r8
	beq.n	.L_080f786c
	lsls	r3, r6, #2
	ldr	r2, [r5, r4]
	ldr	r3, [r4, r3]
	cmp	r2, r3
	bne.n	.L_080f7854
	movs	r3, #1
	negs	r3, r3
	subs	r5, #4
	add	r8, r3
.L_080f786c:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #4
	cmp	r1, #8
	bne.n	.L_080f7810
	movs	r6, #0
	adds	r1, r7, #0
.L_080f787c:
	adds	r2, r6, #0
	cmp	r6, #5
	ble.n	.L_080f7884
	movs	r2, #5
.L_080f7884:
	ldmia	r1!, {r3}
	mov	r0, r9
	adds	r3, #4
	adds	r6, #1
	strb	r2, [r0, r3]
	cmp	r6, #8
	bne.n	.L_080f787c
	movs	r2, #1
	add	sl, r2
	movs	r1, #28
	mov	r3, sl
	add	r9, r1
	cmp	r3, #5
	bne.n	.L_080f780a
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	sub_080c9000
	ldr	r5, [pc, #172]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r1, #8
	str	r3, [sp, #48]
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	str	r3, [sp, #0]
	bl	sub_080c9000
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r7, sp
	adds	r7, #48
	str	r7, [sp, #12]
	movs	r1, #128
	str	r3, [r7, #4]
	ldr	r0, [sp, #40]
	ldr	r3, [pc, #136]
	lsls	r1, r1, #8
	movs	r2, #0
	bl	sub_080072f0
	ldr	r3, [pc, #132]
	ldr	r0, [sp, #40]
	ldr	r1, [pc, #132]
	ldr	r2, [pc, #132]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #160
	lsls	r0, r0, #19
	ldr	r1, [sp, #36]
	ldr	r2, [pc, #124]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r1, [sp, #24]
	ldr	r0, [pc, #120]
	ldr	r2, [pc, #116]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r5, #128
	lsls	r5, r5, #1
	ldr	r1, [pc, #108]
	ldr	r0, [sp, #24]
	movs	r2, #0
	adds	r3, r5, #0
	bl	sub_080f6038
	movs	r1, #160
	movs	r2, #0
	adds	r3, r5, #0
	lsls	r1, r1, #19
	ldr	r0, [sp, #36]
	bl	sub_080f6038
	movs	r2, #128
	ldr	r3, [pc, #52]
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r0, #228
	bl	sub_080772e0
	cmp	r0, #1
	bne.n	.L_080f7982
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #16
	movs	r2, #18
	movs	r3, #3
	movs	r0, #6
	bl	sub_08015010
	movs	r2, #153
	adds	r1, r0, #0
	ldr	r0, [sp, #32]
	lsls	r2, r2, #3
	adds	r3, r0, r2
	str	r1, [r3, #0]
	ldr	r0, [pc, #40]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08015080
	b.n	.L_080f7980
	.4byte 0x00003740
	.4byte 0x03001e50
	.4byte 0x03000168
	.4byte 0x040000d4
	.4byte 0x06003500
	.4byte 0x84002000
	.4byte 0x84000080
	.4byte 0x05000200
	.2byte 0x0909
	.2byte 0x0000
.L_080f7980:
	b.n	.L_080f79b8
.L_080f7982:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #16
	movs	r2, #26
	movs	r3, #4
	movs	r0, #2
	bl	sub_08015010
	movs	r7, #153
	ldr	r3, [sp, #32]
	ldr	r5, [pc, #596]
	lsls	r7, r7, #3
	adds	r1, r0, #0
	adds	r6, r3, r7
	adds	r0, r5, #0
	str	r1, [r6, #0]
	movs	r2, #0
	movs	r3, #0
	adds	r5, #1
	bl	sub_08015080
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #8
	bl	sub_08015080
.L_080f79b8:
	ldr	r0, [sp, #36]
	ldr	r1, [pc, #564]
	movs	r5, #144
	adds	r2, r0, r1
	movs	r3, #0
	lsls	r5, r5, #3
	str	r3, [r2, #0]
	adds	r1, r5, #0
	ldr	r0, [pc, #552]
	bl	sub_080041d8
	ldr	r0, [pc, #552]
	adds	r1, r5, #0
	bl	sub_080041d8
	ldr	r7, [sp, #20]
	ldr	r3, [r7, #0]
	movs	r2, #0
	mov	fp, r2
	cmp	r3, #10
	bne.n	.L_080f79e4
	b.n	.L_080f7d26
.L_080f79e4:
	mov	r0, fp
	cmp	r0, #16
	bgt.n	.L_080f7a0a
	movs	r6, #128
	lsls	r5, r0, #12
	lsls	r6, r6, #1
	ldr	r1, [pc, #520]
	adds	r2, r5, #0
	adds	r3, r6, #0
	ldr	r0, [sp, #24]
	bl	sub_080f6038
	movs	r1, #160
	ldr	r0, [sp, #36]
	lsls	r1, r1, #19
	adds	r2, r5, #0
	adds	r3, r6, #0
	bl	sub_080f6038
.L_080f7a0a:
	ldr	r1, [sp, #20]
	ldr	r3, [r1, #0]
	cmp	r3, #3
	beq.n	.L_080f7a14
	b.n	.L_080f7b98
.L_080f7a14:
	mov	r0, fp
	movs	r1, #80
	bl	sub_080022fc
	cmp	r0, #15
	bgt.n	.L_080f7a28
	ldr	r0, [pc, #476]
	bl	.L_080f61e8
	b.n	.L_080f7a52
.L_080f7a28:
	cmp	r0, #31
	bgt.n	.L_080f7a34
	ldr	r0, [pc, #468]
	bl	.L_080f61e8
	b.n	.L_080f7a52
.L_080f7a34:
	cmp	r0, #47
	bgt.n	.L_080f7a40
	ldr	r0, [pc, #460]
	bl	.L_080f61e8
	b.n	.L_080f7a52
.L_080f7a40:
	cmp	r0, #63
	bgt.n	.L_080f7a4c
	ldr	r0, [pc, #452]
	bl	.L_080f61e8
	b.n	.L_080f7a52
.L_080f7a4c:
	ldr	r0, [pc, #448]
	bl	.L_080f61e8
.L_080f7a52:
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #0]
	cmp	r3, #15
	bgt.n	.L_080f7a62
	bl	.L_080f6148
	ldr	r7, [sp, #16]
	ldr	r3, [r7, #0]
.L_080f7a62:
	cmp	r3, #16
	ble.n	.L_080f7b00
	movs	r3, #7
	mov	r0, fp
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_080f7b00
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	adds	r5, r3, #0
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r1, r3, #0
	mov	r2, fp
	adds	r5, #56
	adds	r1, #48
	cmp	r2, #0
	bge.n	.L_080f7a90
	adds	r2, #7
.L_080f7a90:
	movs	r3, #3
	asrs	r2, r2, #3
	ands	r2, r3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r2, [pc, #376]
	lsls	r3, r3, #10
	adds	r7, r3, r2
	lsls	r5, r5, #16
	movs	r3, #0
	lsls	r1, r1, #16
	mov	r8, r3
	mov	r9, r5
	mov	sl, r1
.L_080f7aac:
	bl	sub_08004458
	movs	r5, #255
	ands	r5, r0
	bl	sub_08004458
	ldr	r3, [pc, #348]
	adds	r6, r0, #0
	ands	r6, r3
	mov	r1, sl
	mov	r0, r9
	str	r1, [r7, #4]
	str	r0, [r7, #0]
	adds	r0, r6, #0
	bl	sub_08002322
	adds	r5, #64
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	negs	r3, r3
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r3, #16
	str	r3, [r7, #24]
	movs	r2, #1
	movs	r3, #128
	add	r8, r2
	lsls	r3, r3, #1
	adds	r7, #28
	cmp	r8, r3
	bne.n	.L_080f7aac
.L_080f7b00:
	movs	r7, #0
	mov	r8, r7
	ldr	r7, [pc, #268]
.L_080f7b06:
	ldr	r0, [r7, #24]
	cmp	r0, #0
	ble.n	.L_080f7b8a
	ldr	r3, [r7, #0]
	ldr	r1, [pc, #268]
	subs	r0, #1
	str	r0, [r7, #24]
	cmp	r3, r1
	bhi.n	.L_080f7b5c
	ldr	r6, [r7, #4]
	ldr	r2, [pc, #260]
	cmp	r6, r2
	bgt.n	.L_080f7b5c
	cmp	r6, #0
	blt.n	.L_080f7b5c
	movs	r1, #12
	asrs	r5, r3, #16
	bl	sub_080022ec
	adds	r0, #1
	lsls	r4, r0, #1
	mov	r3, r8
	ldr	r1, [pc, #240]
	movs	r2, #1
	ands	r2, r3
	asrs	r6, r6, #16
	subs	r3, r4, #2
	ldrh	r1, [r1, r3]
	subs	r5, r5, r0
	ldr	r3, [sp, #44]
	subs	r6, r6, r0
	str	r4, [sp, #0]
	ldr	r0, [sp, #12]
	str	r4, [sp, #4]
	lsls	r2, r2, #2
	ldr	r4, [r2, r0]
	adds	r1, r3, r1
	ldr	r0, [sp, #40]
	adds	r3, r6, #0
	adds	r2, r5, #0
	bl	sub_080072f4
	ldr	r3, [r7, #0]
.L_080f7b5c:
	ldr	r2, [r7, #12]
	adds	r3, r3, r2
	str	r3, [r7, #0]
	ldr	r1, [r7, #16]
	ldr	r3, [r7, #4]
	adds	r3, r3, r1
	str	r3, [r7, #4]
	lsls	r3, r2, #4
	subs	r3, r3, r2
	lsls	r3, r3, #2
	cmp	r3, #0
	bge.n	.L_080f7b76
	adds	r3, #63
.L_080f7b76:
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	lsls	r3, r1, #4
	subs	r3, r3, r1
	lsls	r3, r3, #2
	cmp	r3, #0
	bge.n	.L_080f7b86
	adds	r3, #63
.L_080f7b86:
	asrs	r3, r3, #6
	str	r3, [r7, #16]
.L_080f7b8a:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #3
	adds	r7, #28
	cmp	r8, r2
	bne.n	.L_080f7b06
.L_080f7b98:
	ldr	r7, [sp, #20]
	ldr	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_080f7ba6
	cmp	r3, #2
	beq.n	.L_080f7ba6
	b.n	.L_080f7d08
.L_080f7ba6:
	movs	r0, #0
	add	r5, sp, #56
	mov	r8, r0
	movs	r2, #0
	adds	r3, r5, #0
.L_080f7bb0:
	movs	r1, #1
	add	r8, r1
	mov	r7, r8
	stmia	r3!, {r2}
	cmp	r7, #7
	bne.n	.L_080f7bb0
	ldr	r0, [sp, #20]
	ldr	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L_080f7c28
	movs	r2, #1
	str	r2, [r5, #12]
	ldr	r1, [sp, #28]
	ldr	r3, [r1, #0]
	cmp	r3, #1
	ble.n	.L_080f7bd6
	str	r2, [r5, #16]
	str	r2, [r5, #8]
	ldr	r3, [r1, #0]
.L_080f7bd6:
	cmp	r3, #2
	ble.n	.L_080f7be2
	str	r2, [r5, #20]
	str	r2, [r5, #4]
	ldr	r7, [sp, #28]
	ldr	r3, [r7, #0]
.L_080f7be2:
	cmp	r3, #3
	ble.n	.L_080f7c4c
	str	r2, [r5, #24]
	str	r2, [r5, #0]
	b.n	.L_080f7c4c
	.4byte 0x00000908
	.4byte 0x00007824
	.4byte 0x080f6441
	.4byte 0x080f60a1
	.4byte 0x05000200
	.4byte 0x00000091
	.4byte 0x00000093
	.4byte 0x000000b4
	.4byte 0x000000a0
	.4byte 0x0000008f
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0x00ffffff
	.4byte 0x007fffff
	.2byte 0x86f8
	.2byte 0x080f
.L_080f7c28:
	mov	r3, fp
	mov	r0, r8
	ands	r3, r0
	cmp	r3, #3
	bgt.n	.L_080f7c4c
	ldr	r2, [sp, #32]
	movs	r1, #0
	mov	r8, r1
	adds	r0, r5, #0
	adds	r2, #172
.L_080f7c3c:
	ldmia	r2!, {r3}
	str	r3, [r1, r0]
	movs	r3, #1
	add	r8, r3
	mov	r7, r8
	adds	r1, #4
	cmp	r7, #7
	bne.n	.L_080f7c3c
.L_080f7c4c:
	movs	r0, #0
	mov	r8, r0
.L_080f7c50:
	movs	r2, #1
	mov	r1, r8
	eors	r2, r1
	negs	r3, r2
	orrs	r3, r2
	lsrs	r6, r3, #31
	movs	r3, #65
	subs	r6, r3, r6
	ldr	r3, [r5, #4]
	cmp	r3, #0
	beq.n	.L_080f7c76
	mov	r3, r8
	adds	r3, #19
	movs	r0, #20
	adds	r1, r3, #0
	movs	r2, #200
	str	r6, [sp, #0]
	bl	.L_080f62b8
.L_080f7c76:
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq.n	.L_080f7c8c
	mov	r3, r8
	adds	r3, #35
	movs	r0, #28
	adds	r1, r3, #0
	movs	r2, #200
	str	r6, [sp, #0]
	bl	.L_080f62b8
.L_080f7c8c:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	beq.n	.L_080f7ca2
	mov	r3, r8
	adds	r3, #51
	movs	r0, #20
	adds	r1, r3, #0
	movs	r2, #200
	str	r6, [sp, #0]
	bl	.L_080f62b8
.L_080f7ca2:
	ldr	r3, [r5, #16]
	cmp	r3, #0
	beq.n	.L_080f7cb8
	mov	r3, r8
	adds	r3, #67
	movs	r0, #28
	adds	r1, r3, #0
	movs	r2, #200
	str	r6, [sp, #0]
	bl	.L_080f62b8
.L_080f7cb8:
	ldr	r3, [r5, #20]
	cmp	r3, #0
	beq.n	.L_080f7cce
	mov	r3, r8
	adds	r3, #83
	movs	r0, #20
	adds	r1, r3, #0
	movs	r2, #200
	str	r6, [sp, #0]
	bl	.L_080f62b8
.L_080f7cce:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080f7ce6
	mov	r1, r8
	mov	r3, r8
	adds	r1, #5
	adds	r3, #91
	movs	r0, #28
	movs	r2, #200
	str	r6, [sp, #0]
	bl	.L_080f62b8
.L_080f7ce6:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	beq.n	.L_080f7cfe
	mov	r1, r8
	mov	r3, r8
	adds	r1, #97
	adds	r3, #11
	movs	r0, #28
	movs	r2, #200
	str	r6, [sp, #0]
	bl	.L_080f62b8
.L_080f7cfe:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #3
	bne.n	.L_080f7c50
.L_080f7d08:
	ldr	r7, [sp, #36]
	ldr	r0, [pc, #152]
	movs	r2, #1
	adds	r3, r7, r0
	str	r2, [r3, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [sp, #20]
	ldr	r3, [r2, #0]
	movs	r1, #1
	add	fp, r1
	cmp	r3, #10
	beq.n	.L_080f7d26
	b.n	.L_080f79e4
.L_080f7d26:
	movs	r3, #0
	movs	r6, #128
	mov	r8, r3
	lsls	r6, r6, #1
.L_080f7d2e:
	mov	r7, r8
	movs	r5, #128
	lsls	r3, r7, #12
	lsls	r5, r5, #9
	subs	r5, r5, r3
	ldr	r0, [sp, #24]
	ldr	r1, [pc, #108]
	adds	r2, r5, #0
	adds	r3, r6, #0
	bl	sub_080f6038
	movs	r1, #160
	lsls	r1, r1, #19
	ldr	r0, [sp, #36]
	adds	r2, r5, #0
	adds	r3, r6, #0
	bl	sub_080f6038
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	cmp	r1, #17
	bne.n	.L_080f7d2e
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r0, [pc, #60]
	bl	sub_08004278
	ldr	r0, [pc, #56]
	bl	sub_08004278
	movs	r0, #45
	bl	sub_08002dd8
	movs	r0, #40
	bl	sub_08002dd8
	movs	r0, #39
	bl	sub_08002dd8
	movs	r0, #41
	bl	sub_08002dd8
	add	sp, #116
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00007824
	.4byte 0x05000200
	.4byte 0x080f60a1
	.4byte 0x080f6441
