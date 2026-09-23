.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_08014274, 0x08014274
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_0801489c, 0x0801489c
	.set sub_08014cc0, 0x08014cc0
	.set sub_0801587c, 0x0801587c
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016d18, 0x08016d18
	.set sub_08020090, 0x08020090
	.set sub_080200a8, 0x080200a8
	.set sub_08020138, 0x08020138
	.set sub_080201c0, 0x080201c0
	.set sub_08020218, 0x08020218
	.set sub_08020310, 0x08020310
	.set sub_080cad84, 0x080cad84
	.set sub_080cdf5c, 0x080cdf5c
	.set sub_080d2240, 0x080d2240
	.set sub_080d2d84, 0x080d2d84
	.set sub_080d3838, 0x080d3838
	.set sub_080dbde8, 0x080dbde8
	.set sub_080dbdf4, 0x080dbdf4
	.set sub_080dc1b0, 0x080dc1b0
	.set sub_080dc384, 0x080dc384
	.set sub_080dc390, 0x080dc390
	.set sub_080eaa14, 0x080eaa14
	.set sub_080eab70, 0x080eab70
	.set sub_080eaf98, 0x080eaf98
	.set sub_081c0010, 0x081c0010
	.global Func_080ea14c
	.thumb_func
Func_080ea14c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #235
	lsls	r1, r1, #5
	adds	r5, r0, #0
	movs	r0, #92
	sub	sp, #64
	bl	sub_08014cc0
	mov	r9, r0
	bl	sub_080cdf5c
	str	r0, [sp, #48]
	bl	sub_080cad84
	str	r0, [sp, #44]
	adds	r0, r5, #0
	bl	sub_080cad84
	movs	r1, #0
	str	r0, [sp, #40]
	str	r1, [sp, #8]
	str	r1, [sp, #4]
	ldr	r3, [pc, #496]
	movs	r2, #7
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #496]
	ands	r3, r2
	lsls	r3, r3, #2
	ldr	r1, [r1, r3]
	movs	r0, #8
	bl	sub_080dc1b0
	ldr	r2, [sp, #44]
	add	r5, sp, #52
	ldr	r3, [r2, #8]
	movs	r1, #192
	str	r3, [r5, #0]
	lsls	r1, r1, #13
	ldr	r3, [r2, #12]
	adds	r0, r5, #0
	adds	r3, r3, r1
	str	r3, [r5, #4]
	movs	r7, #132
	ldr	r3, [r2, #16]
	lsls	r7, r7, #5
	str	r3, [r5, #8]
	bl	sub_080dc390
	ldr	r2, [r5, #0]
	adds	r0, r5, #0
	str	r2, [sp, #24]
	movs	r6, #240
	ldr	r3, [r5, #8]
	add	r7, r9
	str	r3, [sp, #20]
	movs	r3, #232
	lsls	r3, r3, #5
	adds	r3, #88
	add	r3, r9
	str	r2, [r3, #0]
	movs	r3, #232
	ldr	r1, [sp, #20]
	lsls	r3, r3, #5
	adds	r3, #92
	add	r3, r9
	str	r1, [r3, #0]
	ldr	r2, [sp, #40]
	lsls	r6, r6, #13
	ldr	r3, [r2, #8]
	str	r3, [r5, #0]
	ldr	r3, [r2, #12]
	str	r3, [r5, #4]
	ldr	r3, [r2, #16]
	str	r3, [r5, #8]
	bl	sub_080dc390
	ldr	r3, [r5, #0]
	ldr	r0, [pc, #396]
	str	r3, [sp, #16]
	ldr	r5, [r5, #8]
	str	r5, [sp, #12]
	bl	sub_08013300
	mov	r1, r9
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r1, #128
	lsls	r1, r1, #4
	mov	r2, r9
	str	r0, [sp, #32]
	bl	sub_080142d4
	movs	r5, #174
	movs	r1, #0
	lsls	r5, r5, #5
	mov	fp, r0
	mov	r8, r1
	add	r5, r9
.L_080ea220:
	bl	sub_08014878
	lsls	r0, r0, #4
	lsrs	r0, r0, #16
	lsls	r0, r0, #1
	add	r0, fp
	str	r0, [sp, #0]
	movs	r1, #4
	movs	r2, #4
	movs	r3, #0
	adds	r0, r5, #0
	bl	sub_080eaf98
	ldrb	r3, [r5, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r5, #9]
	strb	r3, [r5, #5]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #13
	negs	r2, r2
	mov	sl, r2
	mov	r1, sl
	ands	r3, r1
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r5, #9]
	movs	r3, #240
	strh	r3, [r5, #30]
	ldr	r2, [sp, #16]
	mov	r1, r8
	str	r2, [r7, #0]
	ldr	r3, [sp, #12]
	str	r3, [r7, #4]
	cmp	r1, #0
	bge.n	.L_080ea26c
	adds	r1, #15
.L_080ea26c:
	asrs	r1, r1, #4
	mov	r3, r8
	lsls	r2, r1, #4
	subs	r2, r3, r2
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #18
	adds	r3, r3, r6
	str	r3, [r7, #12]
	lsls	r3, r1, #1
	adds	r3, r3, r1
	lsls	r3, r3, #19
	adds	r3, r3, r6
	mov	r1, r8
	movs	r2, #1
	str	r3, [r7, #16]
	add	r8, r2
	negs	r3, r1
	str	r3, [r7, #24]
	mov	r3, r8
	adds	r5, #40
	adds	r7, #28
	cmp	r3, #47
	ble.n	.L_080ea220
	ldr	r0, [pc, #228]
	bl	sub_08013300
	mov	r1, r9
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r5, #128
	lsls	r5, r5, #4
	adds	r1, r5, #0
	mov	r2, r9
	str	r0, [sp, #28]
	bl	sub_080142d4
	movs	r3, #232
	lsls	r3, r3, #5
	adds	r3, #86
	adds	r6, r0, #0
	add	r3, r9
	mov	r1, r9
	adds	r7, r1, r5
	strh	r6, [r3, #0]
	movs	r5, #184
	lsls	r5, r5, #4
	movs	r2, #31
	add	r5, r9
	mov	r8, r2
.L_080ea2d4:
	movs	r3, #128
	adds	r0, r5, #0
	movs	r1, #8
	movs	r2, #8
	lsls	r3, r3, #23
	str	r6, [sp, #0]
	bl	sub_080eaf98
	ldrb	r3, [r5, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r5, #9]
	strb	r3, [r5, #5]
	movs	r3, #15
	ands	r3, r2
	mov	r1, sl
	ands	r3, r1
	strb	r3, [r5, #9]
	movs	r3, #240
	strh	r3, [r5, #30]
	subs	r3, #241
	add	r8, r3
	mov	r2, r8
	str	r3, [r7, #24]
	adds	r5, #40
	adds	r7, #28
	cmp	r2, #0
	bge.n	.L_080ea2d4
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #139
	bl	sub_081c0010
	movs	r1, #2
	ldr	r0, [sp, #40]
	bl	sub_08020090
	movs	r0, #60
	bl	sub_080d2240
	ldr	r2, [sp, #40]
	movs	r3, #0
	adds	r2, #100
	strh	r3, [r2, #0]
	ldr	r1, [sp, #40]
	ldr	r3, [pc, #84]
	movs	r0, #142
	str	r3, [r1, #108]
	bl	sub_081c0010
	movs	r0, #80
	bl	sub_080d2240
	ldr	r1, [pc, #72]
	ldr	r0, [sp, #40]
	bl	sub_080200a8
	movs	r3, #232
	ldr	r5, [pc, #40]
	lsls	r3, r3, #5
	adds	r3, #85
	add	r3, r9
	movs	r1, #144
	strb	r5, [r3, #0]
	ldr	r0, [pc, #56]
	lsls	r1, r1, #3
	bl	sub_080145a8
	movs	r2, #0
	str	r2, [sp, #36]
	mov	sl, r2
.L_080ea364:
	ldr	r3, [sp, #36]
	cmp	r3, #7
	bls.n	.L_080ea36c
	b.n	.L_080ea6c4
.L_080ea36c:
	ldr	r2, [pc, #36]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	b.n	.L_080ea398
	.4byte 0x00000000
	.4byte 0x0300122c
	.4byte 0x080f3954
	.4byte 0x000001f4
	.4byte 0x000001f5
	.4byte 0x080e9e8d
	.4byte 0x080f1040
	.4byte 0x080e9f69
	.2byte 0xa39c
	.2byte 0x080e
.L_080ea398:
	mov	pc, r3
	movs	r0, r0
	add	r3, pc, #752
	lsrs	r6, r1, #32
	add	r4, pc, #72
	lsrs	r6, r1, #32
	add	r4, pc, #1000
	lsrs	r6, r1, #32
	add	r5, pc, #240
	lsrs	r6, r1, #32
	add	r5, pc, #336
	lsrs	r6, r1, #32
	add	r6, pc, #48
	lsrs	r6, r1, #32
	add	r6, pc, #360
	lsrs	r6, r1, #32
	add	r6, pc, #568
	lsrs	r6, r1, #32
	movs	r2, #232
	lsls	r2, r2, #5
	adds	r2, #84
	add	r2, r9
	movs	r3, #0
	mov	r1, sl
	strb	r3, [r2, #0]
	cmp	r1, #39
	bgt.n	.L_080ea3e0
	movs	r3, #7
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_080ea3dc
	movs	r0, #133
	bl	sub_081c0010
.L_080ea3dc:
	movs	r2, #1
	str	r2, [sp, #8]
.L_080ea3e0:
	mov	r3, sl
	cmp	r3, #40
	bne.n	.L_080ea3fc
	movs	r0, #134
	bl	sub_081c0010
	movs	r1, #25
	ldr	r0, [sp, #40]
	str	r1, [sp, #8]
	bl	sub_08020138
	ldr	r2, [sp, #40]
	movs	r3, #0
	str	r3, [r2, #24]
.L_080ea3fc:
	mov	r3, sl
	cmp	r3, #80
	beq.n	.L_080ea404
	b.n	.L_080ea6c4
.L_080ea404:
	ldr	r1, [sp, #36]
	movs	r2, #1
	adds	r1, #1
	negs	r2, r2
	str	r1, [sp, #36]
	mov	sl, r2
	b.n	.L_080ea6c4
	movs	r1, #255
	lsls	r1, r1, #8
	ldr	r6, [pc, #24]
	ldr	r5, [pc, #24]
	movs	r3, #0
	adds	r1, #224
	mov	r8, r3
	mov	ip, r1
	mov	r7, r9
.L_080ea424:
	mov	r2, r8
	cmp	r2, #0
	bge.n	.L_080ea438
	adds	r2, #15
	b.n	.L_080ea438
	movs	r0, r0
	.4byte 0x000003ff
	.2byte 0xfc00
	.2byte 0xffff
.L_080ea438:
	asrs	r2, r2, #4
	lsls	r3, r2, #4
	mov	r1, r8
	subs	r3, r1, r3
	adds	r4, r3, r2
	mov	r2, sl
	movs	r3, #174
	lsls	r3, r3, #5
	subs	r4, r2, r4
	adds	r0, r7, r3
	cmp	r4, #0
	bne.n	.L_080ea462
	ldrh	r1, [r0, #8]
	adds	r3, r5, #0
	lsls	r2, r1, #22
	lsrs	r2, r2, #22
	adds	r2, #32
	ands	r2, r6
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #8]
.L_080ea462:
	cmp	r4, #3
	bne.n	.L_080ea478
	ldrh	r1, [r0, #8]
	adds	r3, r5, #0
	lsls	r2, r1, #22
	lsrs	r2, r2, #22
	add	r2, ip
	ands	r2, r6
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #8]
.L_080ea478:
	cmp	r4, #8
	bne.n	.L_080ea48e
	ldrh	r1, [r0, #8]
	adds	r3, r5, #0
	lsls	r2, r1, #22
	lsrs	r2, r2, #22
	adds	r2, #32
	ands	r2, r6
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #8]
.L_080ea48e:
	cmp	r4, #10
	bne.n	.L_080ea4a4
	ldrh	r1, [r0, #8]
	adds	r3, r5, #0
	lsls	r2, r1, #22
	lsrs	r2, r2, #22
	add	r2, ip
	ands	r2, r6
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #8]
.L_080ea4a4:
	cmp	r4, #16
	bne.n	.L_080ea4ba
	ldrh	r1, [r0, #8]
	adds	r3, r5, #0
	lsls	r2, r1, #22
	lsrs	r2, r2, #22
	adds	r2, #32
	ands	r2, r6
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #8]
.L_080ea4ba:
	cmp	r4, #18
	bne.n	.L_080ea4d0
	ldrh	r1, [r0, #8]
	adds	r3, r5, #0
	lsls	r2, r1, #22
	lsrs	r2, r2, #22
	add	r2, ip
	ands	r2, r6
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #8]
.L_080ea4d0:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r7, #40
	cmp	r2, #47
	ble.n	.L_080ea424
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_080ea4e8
	movs	r0, #246
	bl	sub_081c0010
.L_080ea4e8:
	mov	r1, sl
	cmp	r1, #56
	beq.n	.L_080ea4f0
	b.n	.L_080ea6c4
.L_080ea4f0:
	ldr	r2, [sp, #36]
	movs	r3, #1
	adds	r2, #1
	negs	r3, r3
	b.n	.L_080ea688
	mov	r1, sl
	cmp	r1, #0
	bne.n	.L_080ea50e
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r0, [sp, #48]
	movs	r2, #0
	bl	sub_080d3838
	b.n	.L_080ea6c4
.L_080ea50e:
	ldr	r1, [sp, #44]
	movs	r2, #4
	ldrsh	r3, [r1, r2]
	ldr	r2, [r1, #0]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	cmp	r3, #17
	bne.n	.L_080ea52a
	ldr	r2, [sp, #36]
	movs	r3, #1
	adds	r2, #1
	str	r2, [sp, #36]
	negs	r3, r3
	mov	sl, r3
.L_080ea52a:
	mov	r1, sl
	cmp	r1, #60
	beq.n	.L_080ea532
	b.n	.L_080ea6c4
.L_080ea532:
	ldr	r2, [sp, #36]
	movs	r3, #1
	adds	r2, #1
	negs	r3, r3
	b.n	.L_080ea688
	mov	r1, sl
	cmp	r1, #0
	bne.n	.L_080ea54a
	ldr	r0, [sp, #44]
	movs	r1, #28
	bl	sub_08020090
.L_080ea54a:
	mov	r2, sl
	cmp	r2, #10
	beq.n	.L_080ea552
	b.n	.L_080ea6c4
.L_080ea552:
	b.n	.L_080ea5fe
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L_080ea5e0
	movs	r7, #132
	lsls	r7, r7, #5
	movs	r1, #47
	add	r7, r9
	movs	r3, #0
	mov	r8, r1
.L_080ea566:
	movs	r2, #1
	negs	r2, r2
	add	r8, r2
	mov	r1, r8
	str	r3, [r7, #24]
	subs	r3, #3
	adds	r7, #28
	cmp	r1, #0
	bge.n	.L_080ea566
	movs	r2, #232
	lsls	r2, r2, #5
	adds	r2, #76
	movs	r3, #240
	add	r2, r9
	lsls	r3, r3, #14
	str	r3, [r2, #0]
	movs	r2, #232
	lsls	r2, r2, #5
	adds	r2, #72
	movs	r3, #192
	add	r2, r9
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	movs	r3, #234
	lsls	r3, r3, #5
	movs	r2, #130
	add	r3, r9
	lsls	r2, r2, #16
	str	r2, [r3, #0]
	movs	r3, #232
	lsls	r3, r3, #5
	movs	r2, #232
	adds	r3, #68
	movs	r1, #240
	lsls	r2, r2, #5
	add	r3, r9
	lsls	r1, r1, #13
	adds	r2, #85
	str	r1, [r3, #0]
	add	r2, r9
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r2, #232
	lsls	r2, r2, #5
	adds	r2, #80
	movs	r3, #128
	add	r2, r9
	lsls	r3, r3, #11
	str	r3, [r2, #0]
	movs	r2, #232
	lsls	r2, r2, #5
	adds	r2, #84
	add	r2, r9
	movs	r3, #2
	strb	r3, [r2, #0]
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L_080ea5e0
	movs	r0, #187
	bl	sub_081c0010
.L_080ea5e0:
	mov	r3, sl
	cmp	r3, #48
	bne.n	.L_080ea5ec
	movs	r0, #187
	bl	sub_081c0010
.L_080ea5ec:
	mov	r1, sl
	cmp	r1, #96
	bne.n	.L_080ea5f8
	movs	r0, #187
	bl	sub_081c0010
.L_080ea5f8:
	mov	r2, sl
	cmp	r2, #100
	bne.n	.L_080ea6c4
.L_080ea5fe:
	ldr	r3, [sp, #36]
	movs	r1, #1
	adds	r3, #1
	negs	r1, r1
	str	r3, [sp, #36]
	mov	sl, r1
	b.n	.L_080ea6c4
	ldr	r2, [sp, #24]
	ldr	r1, [pc, #412]
	movs	r5, #234
	adds	r3, r2, r1
	mov	r0, sl
	muls	r0, r3
	movs	r1, #30
	bl	sub_08002054
	movs	r2, #130
	lsls	r2, r2, #16
	lsls	r5, r5, #5
	adds	r0, r0, r2
	add	r5, r9
	str	r0, [r5, #0]
	ldr	r1, [sp, #20]
	ldr	r2, [pc, #384]
	movs	r5, #232
	adds	r3, r1, r2
	mov	r0, sl
	muls	r0, r3
	movs	r1, #30
	bl	sub_08002054
	lsls	r5, r5, #5
	movs	r3, #240
	adds	r5, #68
	lsls	r3, r3, #13
	add	r5, r9
	adds	r0, r0, r3
	mov	r1, sl
	str	r0, [r5, #0]
	cmp	r1, #30
	bne.n	.L_080ea6c4
	ldr	r2, [sp, #36]
	movs	r3, #1
	adds	r2, #1
	negs	r3, r3
	b.n	.L_080ea688
	mov	r1, sl
	cmp	r1, #0
	bne.n	.L_080ea66a
	movs	r3, #232
	lsls	r3, r3, #5
	adds	r3, #80
	add	r3, r9
	str	r1, [r3, #0]
.L_080ea66a:
	movs	r2, #232
	lsls	r2, r2, #5
	adds	r2, #76
	add	r2, r9
	ldr	r3, [r2, #0]
	ldr	r1, [pc, #316]
	adds	r3, r3, r1
	str	r3, [r2, #0]
	cmp	r3, #0
	bge.n	.L_080ea6c4
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r2, [sp, #36]
	subs	r3, #1
	adds	r2, #1
.L_080ea688:
	str	r2, [sp, #36]
	mov	sl, r3
	b.n	.L_080ea6c4
	mov	r1, sl
	cmp	r1, #0
	bne.n	.L_080ea69c
	ldr	r1, [pc, #288]
	ldr	r0, [sp, #44]
	bl	sub_080200a8
.L_080ea69c:
	movs	r5, #3
	mov	r3, sl
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L_080ea6ac
	movs	r0, #167
	bl	sub_081c0010
.L_080ea6ac:
	movs	r3, #232
	lsls	r3, r3, #5
	adds	r3, #84
	add	r3, r9
	mov	r2, sl
	strb	r5, [r3, #0]
	cmp	r2, #60
	bne.n	.L_080ea6c4
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	str	r3, [sp, #36]
.L_080ea6c4:
	ldr	r1, [sp, #8]
	cmp	r1, #0
	beq.n	.L_080ea750
	movs	r2, #0
	mov	fp, r2
	mov	r8, r1
.L_080ea6d0:
	bl	sub_08014878
	ldr	r3, [sp, #4]
	movs	r1, #128
	lsls	r5, r3, #3
	subs	r5, r5, r3
	lsls	r5, r5, #2
	add	r5, r9
	lsls	r1, r1, #4
	adds	r7, r5, r1
	mov	r2, fp
	str	r2, [r7, #24]
	ldr	r1, [sp, #40]
	movs	r2, #192
	ldr	r3, [r1, #8]
	lsls	r2, r2, #12
	str	r3, [r7, #0]
	adds	r6, r0, #0
	ldr	r3, [r1, #12]
	adds	r3, r3, r2
	str	r3, [r7, #4]
	ldr	r3, [r1, #16]
	str	r3, [r7, #8]
	bl	sub_08014878
	movs	r3, #128
	lsls	r3, r3, #10
	lsls	r0, r0, #2
	adds	r0, r0, r3
	adds	r2, r7, #0
	adds	r1, r6, #0
	bl	sub_0801489c
	movs	r3, #144
	mov	r1, fp
	lsls	r3, r3, #11
	str	r1, [r7, #12]
	str	r3, [r7, #16]
	str	r1, [r7, #20]
	bl	sub_08014878
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #12
	movs	r2, #128
	adds	r5, r5, r3
	lsls	r2, r2, #10
	lsls	r0, r0, #1
	adds	r0, r0, r2
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl	sub_0801489c
	ldr	r1, [sp, #4]
	movs	r2, #1
	negs	r2, r2
	movs	r3, #31
	adds	r1, #1
	add	r8, r2
	ands	r1, r3
	mov	r3, r8
	str	r1, [sp, #4]
	cmp	r3, #0
	bne.n	.L_080ea6d0
.L_080ea750:
	movs	r1, #0
	movs	r0, #1
	str	r1, [sp, #8]
	bl	sub_08013560
	movs	r1, #186
	ldr	r3, [sp, #36]
	lsls	r1, r1, #2
	movs	r2, #1
	adds	r1, #255
	add	sl, r2
	cmp	r3, r1
	beq.n	.L_080ea76c
	b.n	.L_080ea364
.L_080ea76c:
	ldr	r0, [sp, #44]
	bl	sub_08020138
	ldr	r2, [sp, #44]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r2, #24]
	str	r3, [r2, #28]
	movs	r0, #10
	bl	sub_080d2240
	ldr	r0, [pc, #56]
	bl	sub_08014644
	bl	sub_080dc384
	ldr	r0, [sp, #32]
	bl	sub_08014274
	ldr	r0, [sp, #28]
	bl	sub_08014274
	movs	r0, #92
	bl	sub_0801314c
	add	sp, #64
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xff7e0000
	.4byte 0xffe20000
	.4byte 0xfffe0000
	.4byte 0x080f3614
	.2byte 0x9f69
	.2byte 0x080e
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	mov	r8, r0
	mov	r9, r8
	mov	r2, r9
	mov	sl, r3
	ldrh	r3, [r2, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_080ea86a
	mov	fp, r2
.L_080ea7ea:
	mov	r3, r8
	ldrh	r0, [r3, #0]
	bl	sub_080d2d84
	mov	r3, r8
	adds	r7, r0, #0
	movs	r2, #2
	ldrsh	r6, [r3, r2]
	bl	sub_080eab70
	adds	r3, r7, #0
	adds	r3, #34
	ldrb	r2, [r3, #0]
	adds	r1, r7, #0
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	mov	r2, sl
	ldr	r5, [r2, r3]
	adds	r2, r7, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, #89
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	lsls	r0, r0, #2
	strb	r3, [r1, #0]
	adds	r5, r5, r0
	movs	r1, #0
	adds	r0, r7, #0
	lsls	r6, r6, #16
	bl	sub_08020218
	lsrs	r6, r6, #16
	movs	r3, #255
	strb	r3, [r5, #2]
	adds	r0, r6, #0
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ea856
	adds	r0, r7, #0
	movs	r1, #0
	bl	sub_08020090
	adds	r0, r6, #0
	bl	sub_08016d18
	b.n	.L_080ea85e
.L_080ea856:
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08020090
.L_080ea85e:
	movs	r3, #4
	add	r8, r3
	mov	r2, r8
	ldrh	r3, [r2, #0]
	cmp	r3, fp
	bne.n	.L_080ea7ea
.L_080ea86a:
	mov	r0, r9
	bl	sub_080eaa14
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_080d2d84
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	bl	sub_080201c0
	ldr	r3, [r5, #12]
	cmp	r3, r0
	bge.n	.L_080ea898
	str	r0, [r5, #20]
	str	r0, [r5, #12]
.L_080ea898:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	adds	r7, r2, #0
	bl	sub_08020310
	cmp	r0, #0
	bne.n	.L_080ea8d0
	adds	r1, r6, #0
	adds	r2, r7, #0
	adds	r0, r5, #0
	bl	sub_080dbde8
	movs	r3, #128
	orrs	r3, r0
	adds	r1, r6, #0
	adds	r0, r5, #0
	adds	r2, r7, #0
	bl	sub_080dbdf4
.L_080ea8d0:
	pop	{r5, r6, r7, pc}
	.align 2, 0
