.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_0800403c, 0x0800403c
	.set sub_080040e8, 0x080040e8
	.set sub_08004760, 0x08004760
	.set sub_0800479c, 0x0800479c
	.set sub_08004858, 0x08004858
	.set sub_08015000, 0x08015000
	.set sub_08015018, 0x08015018
	.set sub_08015020, 0x08015020
	.set sub_08015038, 0x08015038
	.set sub_08015048, 0x08015048
	.set sub_08015118, 0x08015118
	.set sub_08015120, 0x08015120
	.set sub_08015148, 0x08015148
	.set sub_08077008, 0x08077008
	.set sub_08077010, 0x08077010
	.set sub_08077028, 0x08077028
	.set sub_08077050, 0x08077050
	.set sub_08077058, 0x08077058
	.set sub_08077088, 0x08077088
	.set sub_08077098, 0x08077098
	.set sub_080770c8, 0x080770c8
	.set sub_080770d0, 0x080770d0
	.set sub_08077150, 0x08077150
	.set sub_08077168, 0x08077168
	.set sub_080771a8, 0x080771a8
	.set sub_080771b0, 0x080771b0
	.set sub_080771f0, 0x080771f0
	.set sub_080b63c8, 0x080b63c8
	.set sub_080c2a08, 0x080c2a08
	.set sub_080f9010, 0x080f9010
	.global Overlay_080b5368
Overlay_080b5368:
.L_080b5368:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #44
	str	r0, [sp, #24]
	movs	r1, #0
	movs	r0, #0
	str	r1, [sp, #20]
	bl	sub_08077168
	movs	r0, #1
	bl	sub_08077168
	movs	r0, #2
	bl	sub_08077168
	movs	r0, #3
	bl	sub_08077168
	movs	r0, #5
	bl	sub_08077168
	mov	r3, sp
	adds	r3, #28
	str	r3, [sp, #4]
	movs	r2, #0
	ldr	r5, [sp, #4]
	str	r2, [sp, #12]
	add	r3, sp, #40
	mov	ip, r5
.L_080b53ac:
	str	r2, [r3, #0]
	subs	r3, #4
	cmp	r3, ip
	bge.n	.L_080b53ac
	movs	r1, #0
	str	r1, [sp, #16]
	b.n	.L_080b54e8
.L_080b53ba:
	ldr	r2, [sp, #12]
	adds	r2, #1
	str	r2, [sp, #12]
	b.n	.L_080b54e2
.L_080b53c2:
	ldr	r3, [sp, #12]
	ldr	r1, [sp, #24]
	cmp	r3, r1
	beq.n	.L_080b53cc
	b.n	.L_080b54e2
.L_080b53cc:
	mov	r0, sl
	bl	sub_08077150
	ldr	r2, [pc, #108]
	adds	r3, r5, r2
	ldrb	r1, [r3, #1]
	mov	r0, sl
	bl	sub_080771f0
	mov	r0, sl
	bl	sub_08077008
	movs	r5, #140
	adds	r1, r0, #0
	adds	r2, r1, #0
	lsls	r5, r5, #1
	movs	r4, #0
	movs	r0, #0
	adds	r2, #248
	adds	r3, r1, r5
	movs	r7, #3
.L_080b53f6:
	subs	r7, #1
	strb	r4, [r3, #0]
	strb	r4, [r3, #4]
	str	r0, [r2, #0]
	str	r0, [r2, #16]
	adds	r3, #1
	adds	r2, #4
	cmp	r7, #0
	bge.n	.L_080b53f6
	adds	r0, r1, #0
	ldr	r3, [pc, #48]
	movs	r7, #31
	adds	r0, #212
.L_080b5410:
	subs	r7, #1
	strh	r3, [r0, #0]
	subs	r0, #4
	cmp	r7, #0
	bge.n	.L_080b5410
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #16]
	ldr	r5, [pc, #32]
	adds	r3, r1, r2
	lsls	r3, r3, #2
	adds	r3, r5, r3
	adds	r5, r3, #0
	adds	r5, #14
	movs	r7, #1
.L_080b542c:
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080b5444
	adds	r1, r3, #0
	mov	r0, sl
	bl	sub_08077088
	b.n	.L_080b5444
	.4byte 0x00000000
	.2byte 0x3f34
	.2byte 0x080c
.L_080b5444:
	subs	r7, #1
	adds	r5, #2
	cmp	r7, #0
	bge.n	.L_080b542c
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #16]
	adds	r3, r1, r2
	lsls	r4, r3, #2
	movs	r3, #0
	movs	r7, #0
	mov	r9, r3
.L_080b545a:
	ldr	r2, [pc, #212]
	adds	r2, #2
	movs	r5, #0
	ldrsb	r3, [r2, r4]
	mov	r8, r5
	cmp	r8, r3
	bge.n	.L_080b5498
	ldr	r6, [sp, #4]
	mov	r5, r9
	mov	fp, r2
.L_080b546e:
	ldr	r2, [r6, r5]
	adds	r1, r7, #0
	mov	r0, sl
	str	r4, [sp, #0]
	bl	sub_080771a8
	ldr	r2, [r6, r5]
	adds	r1, r7, #0
	mov	r0, sl
	bl	sub_080771b0
	ldr	r3, [r6, r5]
	ldr	r4, [sp, #0]
	adds	r3, #1
	mov	r2, fp
	str	r3, [r6, r5]
	movs	r1, #1
	ldrsb	r3, [r2, r4]
	add	r8, r1
	cmp	r8, r3
	blt.n	.L_080b546e
.L_080b5498:
	movs	r3, #4
	adds	r7, #1
	adds	r4, #1
	add	r9, r3
	cmp	r7, #3
	ble.n	.L_080b545a
	movs	r7, #15
.L_080b54a6:
	mov	r0, sl
	movs	r1, #0
	subs	r7, #1
	bl	sub_08077058
	cmp	r7, #0
	bge.n	.L_080b54a6
	ldr	r5, [sp, #8]
	ldr	r1, [sp, #16]
	ldr	r2, [pc, #116]
	adds	r3, r5, r1
	lsls	r3, r3, #2
	adds	r3, r2, r3
	adds	r5, r3, #6
	movs	r7, #3
.L_080b54c4:
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080b54da
	adds	r1, r3, #0
	mov	r0, sl
	bl	sub_08077028
	adds	r1, r0, #0
	mov	r0, sl
	bl	sub_08077050
.L_080b54da:
	subs	r7, #1
	adds	r5, #2
	cmp	r7, #0
	bge.n	.L_080b54c4
.L_080b54e2:
	ldr	r3, [sp, #16]
	adds	r3, #1
	str	r3, [sp, #16]
.L_080b54e8:
	ldr	r5, [sp, #16]
	ldr	r1, [sp, #16]
	lsls	r5, r5, #2
	str	r5, [sp, #8]
	adds	r3, r5, r1
	ldr	r2, [pc, #60]
	lsls	r5, r3, #2
	ldrsb	r2, [r2, r5]
	movs	r3, #166
	lsls	r3, r3, #1
	mov	sl, r2
	cmp	r1, r3
	bls.n	.L_080b5508
	movs	r5, #1
	str	r5, [sp, #20]
	b.n	.L_080b551c
.L_080b5508:
	movs	r1, #1
	negs	r1, r1
	cmp	sl, r1
	beq.n	.L_080b5512
	b.n	.L_080b53c2
.L_080b5512:
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #24]
	cmp	r2, r3
	beq.n	.L_080b551c
	b.n	.L_080b53ba
.L_080b551c:
	ldr	r0, [sp, #20]
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.2byte 0x3f34
	.2byte 0x080c
.L_080b5534:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r2, #0
	movs	r0, #0
	sub	sp, #128
	mov	sl, r2
	bl	sub_08077008
	mov	r5, sp
	adds	r6, r0, #0
	adds	r1, r5, #0
	ldr	r0, [pc, #356]
	bl	sub_08015020
	mov	r2, sl
	ldrh	r3, [r5, r2]
	strb	r3, [r6, #0]
	lsls	r3, r3, #16
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_080b557a
	adds	r2, r6, #0
	adds	r1, r5, #0
.L_080b5566:
	adds	r0, #1
	cmp	r0, #13
	bgt.n	.L_080b557a
	adds	r1, #2
	ldrh	r3, [r1, #0]
	adds	r2, #1
	strb	r3, [r2, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_080b5566
.L_080b557a:
	movs	r3, #0
	strb	r3, [r6, #14]
	bl	sub_08015000
	movs	r0, #71
	bl	sub_080f9010
	movs	r5, #0
	ldr	r2, [pc, #300]
	movs	r3, #128
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	ldr	r3, [pc, #296]
	movs	r2, #131
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r2, #2
.L_080b559c:
	strb	r2, [r3, #0]
	bl	sub_08015118
	ldr	r0, [pc, #284]
	movs	r1, #5
	bl	sub_08015120
	movs	r0, #0
	movs	r1, #3
	bl	sub_08015120
	movs	r0, #1
	movs	r1, #1
	bl	sub_08015120
	movs	r0, #1
	movs	r1, #2
	bl	sub_08015120
	movs	r0, #2
	movs	r1, #4
	bl	sub_08015120
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_080b55da
	ldr	r3, [pc, #240]
	movs	r1, #2
	adds	r0, r5, r3
	movs	r2, #10
	b.n	.L_080b55e2
.L_080b55da:
	ldr	r3, [pc, #236]
	movs	r1, #2
	adds	r0, r5, r3
	movs	r2, #2
.L_080b55e2:
	movs	r3, #4
	bl	sub_08015038
	adds	r7, r0, #0
	movs	r0, #10
	bl	sub_080030f8
	ldr	r2, [pc, #216]
	ldr	r3, [pc, #208]
	ldr	r6, [pc, #216]
	subs	r2, r2, r3
	mov	r8, r2
	b.n	.L_080b5614
.L_080b55fc:
	bl	sub_08015048
	cmp	r0, #0
	beq.n	.L_080b560e
	ldr	r3, [r6, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080b569a
.L_080b560e:
	movs	r0, #1
	bl	sub_080030f8
.L_080b5614:
	ldr	r3, [r6, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b5630
	mov	r2, sl
	cmp	r2, #0
	beq.n	.L_080b562a
	movs	r3, #0
	mov	sl, r3
	b.n	.L_080b5630
.L_080b562a:
	movs	r2, #1
	adds	r5, #1
	mov	sl, r2
.L_080b5630:
	ldr	r3, [r6, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b563c
	adds	r5, #1
.L_080b563c:
	ldr	r3, [r6, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b5648
	subs	r5, #2
.L_080b5648:
	ldr	r3, [r6, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b5656
	movs	r3, #1
	mov	sl, r3
.L_080b5656:
	ldr	r3, [r6, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b5664
	movs	r2, #0
	mov	sl, r2
.L_080b5664:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b5672
	adds	r5, #10
.L_080b5672:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b5680
	subs	r5, #10
.L_080b5680:
	cmp	r5, #0
	bge.n	.L_080b5686
	movs	r5, #0
.L_080b5686:
	mov	r3, r8
	adds	r3, #5
	cmp	r5, r3
	bcc.n	.L_080b5690
	adds	r5, r3, #0
.L_080b5690:
	ldr	r3, [r6, #0]
	ldr	r2, [pc, #64]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b55fc
.L_080b569a:
	movs	r0, #1
	bl	sub_08015148
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08015018
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #48]
	ldr	r3, [r3, #0]
	adds	r3, r3, r2
	movs	r2, #0
	b.n	.L_080b559c
	.4byte 0x00000903
	.4byte 0x00001341
	.4byte 0x02000240
	.4byte 0x000003e7
	.4byte 0x000026fa
	.4byte 0x00000ad0
	.4byte 0x00002850
	.4byte 0x03001b04
	.4byte 0x000003f2
	.4byte 0x03001e8c
	.2byte 0x12f8
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #0
	mov	sl, r2
	bl	sub_08077098
.L_080b56f6:
	movs	r5, #181
	lsls	r5, r5, #1
	bl	sub_0800479c
	bl	sub_08004760
	bl	sub_080040e8
	bl	sub_08004858
	bl	sub_0800403c
	adds	r0, r5, #0
	bl	sub_080770c8
	ldr	r3, [pc, #312]
	ldr	r3, [r3, #0]
	movs	r2, #128
	ands	r3, r2
	ldr	r6, [pc, #308]
	cmp	r3, #0
	bne.n	.L_080b5724
	b.n	.L_080b583e
.L_080b5724:
	movs	r3, #1
	negs	r3, r3
	adds	r0, r5, #0
	mov	r8, r3
	bl	sub_080770d0
	ldr	r2, [pc, #292]
	movs	r3, #85
	mov	r9, r2
	negs	r3, r3
	add	r3, r9
	ldr	r5, [pc, #288]
	movs	r7, #0
	mov	fp, r3
.L_080b5740:
	movs	r0, #32
	bl	sub_080770d0
	movs	r0, #1
	bl	sub_080030f8
	b.n	.L_080b579a
.L_080b574e:
	ldr	r3, [r5, #0]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b575c
	bl	.L_080b5534
.L_080b575c:
	ldr	r3, [r5, #0]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b576a
	bl	sub_080c2a08
.L_080b576a:
	ldr	r3, [r5, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080b577a
	mov	r2, sl
	cmp	r2, #0
	beq.n	.L_080b5784
.L_080b577a:
	movs	r3, #1
	mov	sl, r3
	movs	r3, #5
	mov	r2, r9
	strb	r3, [r2, #0]
.L_080b5784:
	cmp	r7, r8
	beq.n	.L_080b5794
	bl	sub_08077098
	adds	r0, r7, #0
	bl	.L_080b5368
	mov	r8, r7
.L_080b5794:
	movs	r0, #1
	bl	sub_080030f8
.L_080b579a:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b57a6
	adds	r6, #1
.L_080b57a6:
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b57b2
	subs	r6, #1
.L_080b57b2:
	ldr	r3, [r5, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b57be
	subs	r6, #10
.L_080b57be:
	ldr	r3, [r5, #0]
	movs	r1, #128
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080b57ca
	adds	r6, #10
.L_080b57ca:
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b57d8
	adds	r7, #1
.L_080b57d8:
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b57e6
	subs	r7, #1
.L_080b57e6:
	ldr	r3, [r5, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b574e
	ldr	r3, [pc, #92]
	ldr	r3, [r3, #0]
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080b5802
	movs	r0, #182
	lsls	r0, r0, #1
	bl	sub_080770c8
.L_080b5802:
	movs	r0, #0
	bl	sub_08077010
	ldr	r3, [pc, #84]
	mov	r2, fp
	strh	r3, [r2, #0]
	cmp	r6, #28
	bne.n	.L_080b581a
	movs	r0, #183
	lsls	r0, r0, #1
	bl	sub_080770c8
.L_080b581a:
	movs	r0, #177
	lsls	r0, r0, #1
	bl	sub_080770c8
	adds	r0, r6, #0
	bl	sub_080b63c8
	bl	sub_0800479c
	bl	sub_08004760
	bl	sub_080040e8
	bl	sub_08004858
	bl	sub_0800403c
	b.n	.L_080b5740
.L_080b583e:
	movs	r0, #177
	lsls	r0, r0, #1
	bl	sub_080770c8
	ldr	r0, [pc, #12]
	bl	sub_080b63c8
	b.n	.L_080b56f6
	movs	r0, r0
	.4byte 0x03001ae8
	.4byte 0x00000101
	.4byte 0x0200046b
	.4byte 0x03001b04
	.4byte 0x0000001d
