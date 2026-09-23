.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08004760, 0x08004760
	.set sub_0800479c, 0x0800479c
	.set sub_080052f4, 0x080052f4
	.set sub_080072e4, 0x080072e4
	.set sub_080770c0, 0x080770c0
	.global Func_0808cf78
	.thumb_func
Func_0808cf78:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #300]
	movs	r0, #225
	ldr	r7, [r3, #0]
	ldr	r3, [pc, #300]
	lsls	r0, r0, #1
	adds	r3, r3, r0
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	ldr	r3, [pc, #292]
	ldr	r0, [r3, #12]
	mov	r8, r1
	bl	sub_080072e4
	movs	r6, #1
	movs	r3, #0
	adds	r5, r0, #0
	mov	sl, r3
	negs	r6, r6
	b.n	.L_0808cfa8
.L_0808cfa6:
	adds	r5, #24
.L_0808cfa8:
	movs	r4, #0
	ldrsh	r3, [r5, r4]
	cmp	r3, r6
	beq.n	.L_0808cfc8
	cmp	r3, r8
	bne.n	.L_0808cfa6
	movs	r1, #2
	ldrsh	r0, [r5, r1]
	cmp	r0, r6
	beq.n	.L_0808cfc4
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_0808cfa6
.L_0808cfc4:
	movs	r2, #1
	mov	sl, r2
.L_0808cfc8:
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_0808cfd8
	ldr	r3, [pc, #232]
	ldr	r0, [r3, #12]
	bl	sub_080072e4
	adds	r5, r0, #0
.L_0808cfd8:
	ldr	r0, [pc, #224]
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_0808d020
	ldr	r1, [pc, #208]
	movs	r4, #238
	lsls	r4, r4, #1
	adds	r2, r1, r4
	movs	r4, #4
	ldrsh	r3, [r5, r4]
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	movs	r3, #240
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	movs	r3, #242
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r4, #8
	ldrsh	r3, [r5, r4]
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	movs	r3, #244
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r4, #246
	ldrh	r3, [r5, #10]
	lsls	r4, r4, #1
	str	r3, [r2, #0]
	adds	r3, r1, r4
	strh	r0, [r3, #0]
.L_0808d020:
	movs	r1, #1
	movs	r0, #14
	ldrsh	r3, [r5, r0]
	negs	r1, r1
	cmp	r3, r1
	beq.n	.L_0808d036
	adds	r6, r7, #0
	adds	r6, #236
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	b.n	.L_0808d03a
.L_0808d036:
	adds	r6, r7, #0
	adds	r6, #236
.L_0808d03a:
	movs	r2, #16
	ldrsh	r3, [r5, r2]
	cmp	r3, r1
	beq.n	.L_0808d04c
	adds	r4, r7, #0
	adds	r4, #240
	lsls	r3, r3, #16
	str	r3, [r4, #0]
	b.n	.L_0808d050
.L_0808d04c:
	adds	r4, r7, #0
	adds	r4, #240
.L_0808d050:
	movs	r0, #18
	ldrsh	r3, [r5, r0]
	cmp	r3, r1
	beq.n	.L_0808d062
	adds	r2, r7, #0
	adds	r2, #244
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	b.n	.L_0808d066
.L_0808d062:
	adds	r2, r7, #0
	adds	r2, #244
.L_0808d066:
	movs	r3, #20
	ldrsh	r0, [r5, r3]
	cmp	r0, r1
	beq.n	.L_0808d078
	adds	r1, r7, #0
	adds	r1, #248
	lsls	r3, r0, #16
	str	r3, [r1, #0]
	b.n	.L_0808d07c
.L_0808d078:
	adds	r1, r7, #0
	adds	r1, #248
.L_0808d07c:
	ldr	r3, [r6, #0]
	movs	r0, #240
	lsls	r0, r0, #16
	ldr	r2, [r2, #0]
	adds	r3, r3, r0
	cmp	r3, r2
	ble.n	.L_0808d090
	ldr	r0, [pc, #52]
	adds	r3, r2, r0
	str	r3, [r6, #0]
.L_0808d090:
	ldr	r3, [r4, #0]
	movs	r2, #160
	lsls	r2, r2, #16
	adds	r3, r3, r2
	ldr	r2, [r1, #0]
	cmp	r3, r2
	ble.n	.L_0808d0a4
	ldr	r0, [pc, #36]
	adds	r3, r2, r0
	str	r3, [r4, #0]
.L_0808d0a4:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e70
	.4byte 0x02000240
	.4byte 0x02008000
	.4byte 0x00000109
	.4byte 0xff100000
	.2byte 0x0000
	.2byte 0xff60
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #0
	movs	r2, #1
	mov	r9, r1
	ldr	r0, [pc, #32]
	ldr	r1, [pc, #36]
	sub	sp, #12
	mov	sl, r2
	mov	r8, r2
	bl	sub_080052f4
	movs	r3, #0
	str	r3, [sp, #8]
	mov	fp, r3
	b.n	.L_0808d114
.L_0808d0f2:
	mov	r1, r9
	mov	r2, r9
	lsls	r1, r1, #12
	lsls	r2, r2, #5
	str	r1, [sp, #8]
	mov	fp, r2
	b.n	.L_0808d114
	.4byte 0x0809e4ce
	.2byte 0x1a00
	.2byte 0x0600
.L_0808d108:
	mov	r3, r9
	lsls	r3, r3, #12
	mov	r1, r9
.L_0808d10e:
	lsls	r1, r1, #5
	str	r3, [sp, #8]
.L_0808d112:
	mov	fp, r1
.L_0808d114:
	ldr	r3, [pc, #64]
	ldr	r1, [pc, #68]
	add	r3, r9
	ldr	r0, [sp, #8]
	adds	r2, r1, #0
	strh	r3, [r1, #0]
	ldr	r3, [pc, #32]
	adds	r2, #64
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	adds	r2, #64
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	adds	r2, #64
	ldr	r7, [pc, #44]
	strh	r3, [r2, #0]
	ldr	r5, [pc, #24]
	movs	r2, #1
	ldr	r4, [pc, #24]
	adds	r0, #209
	add	r7, fp
	adds	r1, #2
	mov	ip, r2
	b.n	.L_0808d164
	.4byte 0x0000f052
	.4byte 0x0000f047
	.4byte 0x0000f042
	.4byte 0x0000001f
	.4byte 0x0000f0e0
	.4byte 0xfffff0e0
	.4byte 0x0600205a
	.2byte 0x0002
	.2byte 0x0500
.L_0808d164:
	strh	r0, [r1, #0]
	ldrh	r6, [r7, #0]
	adds	r3, r6, #0
	adds	r2, r1, #0
	ands	r3, r5
	adds	r2, #64
	adds	r3, r3, r4
	strh	r3, [r2, #0]
	lsrs	r3, r6, #5
	ands	r3, r5
	adds	r2, #64
	adds	r3, r3, r4
	strh	r3, [r2, #0]
	lsrs	r3, r6, #10
	ands	r3, r5
	adds	r2, #64
	adds	r3, r3, r4
	strh	r3, [r2, #0]
	movs	r3, #1
	add	ip, r3
	mov	r2, ip
	adds	r0, #1
	adds	r7, #2
	adds	r1, #2
	cmp	r2, #15
	bls.n	.L_0808d164
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [pc, #356]
	movs	r4, #31
.L_0808d1a2:
	ldr	r2, [r1, #0]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0808d1ba
	subs	r3, #65
	add	sl, r3
	mov	r2, sl
	cmp	r2, #0
	bgt.n	.L_0808d1ba
	movs	r3, #3
	mov	sl, r3
.L_0808d1ba:
	ldr	r2, [r1, #0]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0808d1d0
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #3
	ble.n	.L_0808d1d0
	mov	sl, r2
.L_0808d1d0:
	ldr	r2, [r1, #0]
	movs	r3, #32
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0808d1ea
	movs	r2, #1
	negs	r2, r2
	add	r8, r2
	mov	r3, r8
	cmp	r3, #0
	bgt.n	.L_0808d1ea
	movs	r2, #15
	mov	r8, r2
.L_0808d1ea:
	ldr	r2, [r1, #0]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0808d200
	movs	r3, #1
	add	r8, r3
	mov	r2, r8
	cmp	r2, #15
	ble.n	.L_0808d200
	mov	r8, r3
.L_0808d200:
	ldr	r2, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0808d226
	movs	r3, #1
	negs	r3, r3
	add	r9, r3
	mov	r1, r9
	cmp	r1, #0
	blt.n	.L_0808d21a
	b.n	.L_0808d0f2
.L_0808d21a:
	movs	r2, #13
	mov	r9, r2
	mov	r3, r9
	mov	r1, r9
	lsls	r3, r3, #12
	b.n	.L_0808d10e
.L_0808d226:
	ldr	r2, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0808d246
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	cmp	r3, #13
	bgt.n	.L_0808d23e
	b.n	.L_0808d108
.L_0808d23e:
	movs	r1, #0
	mov	r9, r1
	str	r1, [sp, #8]
	b.n	.L_0808d112
.L_0808d246:
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0808d292
	mov	r2, r9
	lsls	r3, r2, #4
	add	r3, r8
	movs	r1, #160
	lsls	r3, r3, #1
	lsls	r1, r1, #19
	adds	r7, r3, r1
	ldrh	r6, [r7, #0]
	mov	r1, sl
	adds	r5, r6, #0
	lsrs	r2, r6, #5
	lsrs	r3, r6, #10
	ands	r5, r4
	ands	r2, r4
	ands	r3, r4
	cmp	r1, #1
	bne.n	.L_0808d278
	cmp	r5, #30
	bhi.n	.L_0808d278
	adds	r5, #1
.L_0808d278:
	mov	r1, sl
	cmp	r1, #2
	bne.n	.L_0808d284
	cmp	r2, #30
	bhi.n	.L_0808d284
	adds	r2, #1
.L_0808d284:
	mov	r1, sl
	cmp	r1, #3
	bne.n	.L_0808d2dc
	cmp	r3, #30
	bhi.n	.L_0808d2dc
	adds	r3, #1
	b.n	.L_0808d2dc
.L_0808d292:
	ldr	r2, [r1, #0]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0808d2e8
	mov	r2, r9
	lsls	r3, r2, #4
	add	r3, r8
	movs	r1, #160
	lsls	r3, r3, #1
	lsls	r1, r1, #19
	adds	r7, r3, r1
	ldrh	r6, [r7, #0]
	mov	r1, sl
	adds	r5, r6, #0
	lsrs	r2, r6, #5
	lsrs	r3, r6, #10
	ands	r5, r4
	ands	r2, r4
	ands	r3, r4
	cmp	r1, #1
	bne.n	.L_0808d2c4
	cmp	r5, #0
	beq.n	.L_0808d2c4
	subs	r5, #1
.L_0808d2c4:
	mov	r1, sl
	cmp	r1, #2
	bne.n	.L_0808d2d0
	cmp	r2, #0
	beq.n	.L_0808d2d0
	subs	r2, #1
.L_0808d2d0:
	mov	r1, sl
	cmp	r1, #3
	bne.n	.L_0808d2dc
	cmp	r3, #0
	beq.n	.L_0808d2dc
	subs	r3, #1
.L_0808d2dc:
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r5
	strh	r3, [r7, #0]
	b.n	.L_0808d114
.L_0808d2e8:
	ldr	r2, [r1, #0]
	movs	r3, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0808d352
	mov	r2, r9
	lsls	r3, r2, #4
	add	r3, r8
	movs	r2, #160
	lsls	r3, r3, #1
	lsls	r2, r2, #19
	adds	r7, r3, r2
	ldrh	r6, [r7, #0]
	b.n	.L_0808d334
	.2byte 0x1b04
	.2byte 0x0300
.L_0808d308:
	cmp	r5, #0
	bne.n	.L_0808d310
	ldr	r3, [pc, #28]
	strh	r3, [r7, #0]
.L_0808d310:
	cmp	r5, #10
	bne.n	.L_0808d316
	strh	r6, [r7, #0]
.L_0808d316:
	cmp	r5, #20
	bne.n	.L_0808d31e
	ldr	r3, [pc, #20]
	strh	r3, [r7, #0]
.L_0808d31e:
	cmp	r5, #30
	bne.n	.L_0808d324
	strh	r6, [r7, #0]
.L_0808d324:
	adds	r5, #1
	cmp	r5, #39
	bls.n	.L_0808d336
	b.n	.L_0808d334
	.4byte 0x00007fff
	.2byte 0x0000
	.2byte 0x0000
.L_0808d334:
	movs	r5, #0
.L_0808d336:
	movs	r0, #1
	str	r1, [sp, #4]
	str	r4, [sp, #0]
	bl	sub_080030f8
	ldr	r3, [pc, #72]
	ldr	r2, [r3, #0]
	movs	r3, #8
	ands	r2, r3
	ldr	r1, [sp, #4]
	ldr	r4, [sp, #0]
	cmp	r2, #0
	bne.n	.L_0808d308
	strh	r6, [r7, #0]
.L_0808d352:
	ldr	r2, [r1, #0]
	movs	r3, #4
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_0808d370
	ldr	r3, [pc, #48]
	movs	r0, #1
	str	r1, [sp, #4]
	str	r4, [sp, #0]
	ldr	r3, [r3, #0]
	bl	sub_080030f8
	ldr	r1, [sp, #4]
	ldr	r4, [sp, #0]
	b.n	.L_0808d1a2
.L_0808d370:
	bl	sub_0800479c
	bl	sub_08004760
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ae8
	.4byte 0x03001e40
