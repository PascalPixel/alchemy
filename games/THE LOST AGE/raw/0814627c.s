.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0800206c, 0x0800206c
	.set sub_08002074, 0x08002074
	.set sub_08002080, 0x08002080
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_080148e8, 0x080148e8
	.set sub_08014de4, 0x08014de4
	.set sub_08014e38, 0x08014e38
	.set sub_08014ea8, 0x08014ea8
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015128, 0x08015128
	.set sub_080151ac, 0x080151ac
	.set sub_080156e8, 0x080156e8
	.set sub_08020010, 0x08020010
	.set sub_08020048, 0x08020048
	.set sub_08020090, 0x08020090
	.set sub_08020098, 0x08020098
	.set sub_08020138, 0x08020138
	.set sub_08020148, 0x08020148
	.set sub_08118038, 0x08118038
	.set sub_08118040, 0x08118040
	.set sub_08118078, 0x08118078
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_081465aa, 0x081465aa
	.set sub_081471a0, 0x081471a0
	.set sub_081471f6, 0x081471f6
	.set sub_08147268, 0x08147268
	.set sub_081472d4, 0x081472d4
	.set sub_081476be, 0x081476be
	.set sub_08147aec, 0x08147aec
	.set sub_081496c8, 0x081496c8
	.set sub_0814cc4c, 0x0814cc4c
	.set sub_0814cca8, 0x0814cca8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08152404, 0x08152404
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e20c, 0x0815e20c
	.set sub_081963ec, 0x081963ec
	.set sub_081c0010, 0x081c0010
	.global Overlay_0814627c
Overlay_0814627c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #132
	str	r0, [sp, #80]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #92]
	movs	r0, #0
	str	r1, [sp, #76]
	ldr	r2, [r3, #96]
	str	r2, [sp, #72]
	ldr	r5, [r3, #100]
	ldr	r3, [r3, #48]
	str	r3, [sp, #52]
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [sp, #76]
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r1, r3, r4
	ldr	r0, [pc, #44]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	adds	r1, r5, #0
	ldr	r0, [pc, #36]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r5, [sp, #76]
	movs	r2, #184
	lsls	r2, r2, #5
	adds	r1, r5, r2
	ldr	r0, [pc, #24]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r3, #239
	b.n	.L_081462f4
	.4byte 0x00000100
	.4byte 0x0000013a
	.4byte 0x00000134
	.2byte 0x0137
	.2byte 0x0000
.L_081462f4:
	lsls	r3, r3, #7
	adds	r2, r5, r3
	movs	r4, #238
	movs	r3, #3
	str	r3, [r2, #0]
	lsls	r4, r4, #7
	ldr	r3, [pc, #796]
	adds	r4, #132
	adds	r2, r5, r4
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #788]
	bl	sub_080145a8
	ldr	r1, [sp, #80]
	mov	r2, sp
	adds	r2, #120
	movs	r5, #36
	ldrsh	r0, [r1, r5]
	adds	r1, r2, #0
	str	r2, [sp, #44]
	bl	sub_0815e20c
	ldr	r3, [sp, #44]
	movs	r0, #142
	ldr	r2, [r3, #0]
	movs	r3, #64
	subs	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #19
	str	r3, [sp, #48]
	adds	r2, #40
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	bl	sub_081c0010
	movs	r4, #0
	str	r4, [sp, #68]
	ldr	r5, [sp, #80]
	movs	r1, #72
	ldr	r2, [r5, #20]
	negs	r1, r1
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #2
	cmp	r3, r1
	bne.n	.L_08146356
	b.n	.L_08146608
.L_08146356:
	ldr	r2, [sp, #68]
	cmp	r2, #64
	bne.n	.L_08146362
	movs	r0, #0
	bl	sub_081180e8
.L_08146362:
	ldr	r3, [sp, #68]
	cmp	r3, #46
	bne.n	.L_08146378
	ldr	r4, [sp, #80]
	movs	r2, #16
	ldr	r0, [r4, #8]
	movs	r5, #36
	ldrsh	r1, [r4, r5]
	movs	r3, #0
	bl	sub_08118078
.L_08146378:
	movs	r1, #170
	movs	r2, #170
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [sp, #68]
	adds	r2, #85
	movs	r3, #0
	adds	r1, #171
	bl	sub_081496c8
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r1, #23
	movs	r0, #188
	str	r3, [sp, #60]
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #68]
	str	r3, [sp, #64]
	cmp	r1, #16
	ble.n	.L_081463ce
	movs	r3, #15
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_081463ce
	ldr	r3, [sp, #76]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #132
	adds	r2, r3, r4
	ldr	r3, [r2, #0]
	ldr	r5, [pc, #604]
	adds	r3, r3, r5
	str	r3, [r2, #0]
.L_081463ce:
	ldr	r2, [sp, #76]
	ldr	r4, [sp, #68]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r3, r2, r3
	str	r3, [sp, #28]
	lsls	r3, r4, #1
	adds	r3, r3, r4
	movs	r1, #0
	lsls	r3, r3, #9
	movs	r5, #36
	str	r1, [sp, #56]
	str	r1, [sp, #20]
	str	r3, [sp, #16]
	str	r5, [sp, #12]
	mov	r9, r4
.L_081463ee:
	ldr	r1, [sp, #12]
	ldr	r3, [sp, #80]
	ldrsh	r0, [r1, r3]
	bl	sub_08118098
	mov	r4, r9
	ldr	r6, [r0, #0]
	cmp	r4, #95
	bls.n	.L_08146402
	b.n	sub_081465aa
.L_08146402:
	bl	sub_08014de4
	ldr	r0, [sp, #52]
	adds	r1, r0, #0
	adds	r1, #12
	bl	sub_080156e8
	ldr	r3, [r6, #8]
	add	r5, sp, #108
	str	r3, [r5, #0]
	movs	r1, #96
	ldr	r3, [r6, #12]
	add	r1, sp
	str	r3, [r5, #4]
	adds	r0, r5, #0
	ldr	r3, [r6, #16]
	mov	sl, r1
	str	r3, [r5, #8]
	bl	sub_0815e1ec
	ldr	r2, [sp, #44]
	ldr	r4, [sp, #48]
	ldr	r3, [r2, #0]
	mov	r1, sl
	adds	r3, r3, r4
	str	r3, [r1, #0]
	ldr	r3, [r1, #4]
	mov	r2, r9
	subs	r3, #24
	str	r3, [r1, #4]
	cmp	r2, #67
	ble.n	.L_08146444
	b.n	.L_08146570
.L_08146444:
	ldr	r5, [sp, #20]
	ldr	r1, [sp, #76]
	lsls	r2, r5, #3
	subs	r2, r2, r5
	lsls	r2, r2, #2
	adds	r5, r2, r1
	ldr	r2, [sp, #16]
	movs	r3, #168
	lsls	r3, r3, #10
	movs	r4, #0
	subs	r2, r3, r2
	str	r4, [sp, #24]
	movs	r3, #64
	mov	r4, r9
	subs	r3, r3, r4
	lsls	r3, r3, #9
	movs	r7, #0
	add	r6, sp, #84
	mov	r8, r2
	mov	fp, r3
.L_0814646c:
	bl	sub_08014de4
	mov	r1, r9
	cmp	r1, #63
	bgt.n	.L_08146490
	mov	r2, r8
	str	r2, [r6, #0]
	str	r2, [r6, #4]
	str	r2, [r6, #8]
	adds	r0, r6, #0
	bl	sub_080151ac
	mov	r0, fp
	bl	sub_080150e4
	mov	r0, fp
	bl	sub_08015068
.L_08146490:
	ldr	r0, [sp, #24]
	bl	sub_080150e4
	add	r3, sp, #108
	adds	r1, r3, #0
	ldr	r0, [pc, #400]
	bl	sub_0815e1ec
	mov	r4, sl
	ldr	r3, [r4, #0]
	ldr	r2, [sp, #108]
	adds	r7, #1
	adds	r2, r2, r3
	str	r2, [r5, #12]
	ldr	r3, [sp, #112]
	ldr	r2, [r4, #4]
	adds	r3, r3, r2
	adds	r3, #16
	str	r3, [r5, #16]
	ldr	r1, [sp, #24]
	movs	r2, #170
	lsls	r2, r2, #7
	adds	r2, #85
	adds	r1, r1, r2
	str	r1, [sp, #24]
	adds	r5, #28
	cmp	r7, #3
	bne.n	.L_0814646c
	ldr	r3, [sp, #20]
	movs	r7, #0
	str	r3, [sp, #36]
.L_081464ce:
	ldr	r4, [sp, #36]
	ldr	r5, [sp, #76]
	adds	r2, r7, r4
	lsls	r3, r2, #3
	subs	r3, r3, r2
	adds	r7, #1
	lsls	r3, r3, #2
	adds	r3, r5, r3
	movs	r1, #3
	adds	r0, r7, #0
	str	r3, [sp, #40]
	str	r7, [sp, #32]
	bl	sub_08002064
	ldr	r1, [sp, #36]
	mov	r2, r9
	adds	r0, r0, r1
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #2
	adds	r5, r5, r3
	mov	fp, r5
	cmp	r2, #0
	bge.n	.L_08146500
	adds	r2, #15
.L_08146500:
	asrs	r2, r2, #4
	movs	r3, #5
	subs	r4, r3, r2
	movs	r2, #0
	mov	r8, r2
	lsls	r7, r4, #1
.L_0814650c:
	ldr	r1, [sp, #40]
	mov	r5, fp
	ldr	r6, [r1, #12]
	ldr	r3, [r5, #12]
	movs	r1, #24
	subs	r3, r3, r6
	mov	r0, r8
	muls	r0, r3
	str	r4, [sp, #8]
	bl	sub_08002054
	ldr	r2, [sp, #40]
	ldr	r3, [r5, #16]
	ldr	r5, [r2, #16]
	adds	r6, r6, r0
	subs	r3, r3, r5
	mov	r0, r8
	muls	r0, r3
	movs	r1, #24
	bl	sub_08002054
	ldr	r2, [pc, #248]
	subs	r3, r7, #2
	ldrh	r1, [r2, r3]
	ldr	r4, [sp, #8]
	ldr	r3, [sp, #76]
	adds	r5, r5, r0
	movs	r2, #184
	subs	r5, r5, r4
	subs	r6, r6, r4
	adds	r1, r3, r1
	lsls	r2, r2, #5
	adds	r1, r1, r2
	adds	r3, r5, #0
	adds	r2, r6, #0
	str	r7, [sp, #0]
	str	r7, [sp, #4]
	ldr	r0, [sp, #72]
	ldr	r5, [sp, #60]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2101
	add	r8, r1
	mov	r2, r8
	ldr	r4, [sp, #8]
	cmp	r2, #24
	bne.n	.L_0814650c
	ldr	r7, [sp, #32]
	cmp	r7, #3
	bne.n	.L_081464ce
.L_08146570:
	mov	r3, r9
	cmp	r3, #63
	ble.n	sub_081465aa
	mov	r4, sl
	ldr	r2, [r4, #0]
	ldr	r3, [r4, #4]
	movs	r5, #24
	str	r5, [sp, #0]
	movs	r5, #48
	subs	r2, #24
	subs	r3, #24
	str	r5, [sp, #4]
	ldr	r1, [sp, #28]
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #72]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4651
	ldr	r3, [r1, #4]
	movs	r4, #24
	ldr	r2, [r1, #0]
	subs	r3, #24
	str	r5, [sp, #4]
	str	r4, [sp, #0]
	ldr	r0, [sp, #72]
	ldr	r1, [sp, #28]
	ldr	r5, [sp, #64]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9905
	ldr	r2, [sp, #16]
	adds	r1, #32
	ldr	r5, [sp, #12]
	str	r1, [sp, #20]
	ldr	r3, [pc, #124]
	ldr	r1, [sp, #56]
	movs	r4, #8
	adds	r2, r2, r3
	negs	r4, r4
	adds	r5, #2
	adds	r1, #1
	str	r2, [sp, #16]
	add	r9, r4
	str	r5, [sp, #12]
	str	r1, [sp, #56]
	cmp	r1, #1
	beq.n	.L_081465d0
	b.n	.L_081463ee
.L_081465d0:
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	movs	r4, #240
	ldr	r2, [sp, #76]
	ldr	r5, [sp, #56]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r3, r2, r4
	str	r5, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [sp, #68]
	ldr	r3, [sp, #80]
	adds	r1, #1
	str	r1, [sp, #68]
	ldr	r2, [r3, #20]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, #72
	cmp	r1, r3
	beq.n	.L_08146608
	b.n	.L_08146356
.L_08146608:
	ldr	r0, [pc, #24]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #132
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x04040404
	.4byte 0x08143001
	.4byte 0x01010101
	.4byte 0x08197918
	.4byte 0x08197424
	.2byte 0xd000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #120
	str	r0, [sp, #80]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #92]
	ldr	r4, [sp, #80]
	str	r0, [sp, #76]
	ldr	r1, [r5, #96]
	str	r1, [sp, #72]
	ldr	r2, [r5, #100]
	str	r2, [sp, #52]
	ldr	r3, [r5, #48]
	str	r3, [sp, #48]
	ldr	r0, [r4, #8]
	bl	sub_08118098
	ldr	r6, [r0, #0]
	movs	r0, #1
	bl	sub_081435e0
	ldr	r0, [pc, #828]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #820]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2200
	ldr	r1, [sp, #52]
	movs	r3, #0
	ldr	r0, [pc, #808]
	bl	sub_08157cf4
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_08020090
	adds	r0, r6, #0
	movs	r1, #48
	bl	sub_08020098
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	ldr	r0, [sp, #76]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r0, r1
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	ldr	r5, [r5, #104]
	adds	r3, #132
	adds	r2, r0, r3
	movs	r1, #200
	movs	r3, #75
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #752]
	str	r5, [sp, #60]
	bl	sub_080145a8
	ldr	r4, [sp, #80]
	ldr	r0, [r4, #8]
	bl	sub_08118098
	ldr	r2, [sp, #80]
	ldr	r6, [r0, #0]
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	bl	sub_08118098
	movs	r3, #0
	ldr	r0, [r0, #0]
	ldr	r5, [sp, #76]
	str	r3, [sp, #56]
	mov	fp, r0
.L_081466ea:
	ldr	r2, [r6, #8]
	movs	r4, #160
	str	r2, [r5, #0]
	lsls	r4, r4, #14
	ldr	r3, [r6, #12]
	adds	r3, r3, r4
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	ldr	r0, [sp, #56]
	cmp	r0, #0
	bne.n	.L_0814670a
	mov	r1, fp
	ldr	r0, [r1, #8]
	movs	r1, #12
	b.n	.L_08146712
.L_0814670a:
	mov	r3, fp
	ldr	r0, [r3, #8]
	movs	r1, #12
	lsls	r0, r0, #1
.L_08146712:
	subs	r0, r0, r2
	bl	sub_08002054
	str	r0, [r5, #12]
	mov	r4, fp
	ldr	r0, [r4, #12]
	ldr	r3, [r5, #4]
	movs	r1, #160
	subs	r0, r0, r3
	lsls	r1, r1, #14
	adds	r0, r0, r1
	movs	r1, #12
	bl	sub_08002054
	str	r0, [r5, #16]
	mov	r2, fp
	ldr	r0, [r2, #16]
	ldr	r3, [r5, #8]
	movs	r1, #12
	subs	r0, r0, r3
	bl	sub_08002054
	movs	r3, #0
	str	r0, [r5, #20]
	str	r3, [r5, #24]
	ldr	r3, [sp, #56]
	adds	r5, #28
	adds	r3, #1
	str	r3, [sp, #56]
	cmp	r3, #3
	bne.n	.L_081466ea
	movs	r4, #0
	str	r4, [sp, #68]
.L_08146754:
	ldr	r0, [sp, #68]
	cmp	r0, #47
	bgt.n	.L_08146788
	movs	r2, #128
	cmp	r0, #39
	ble.n	.L_0814676a
	ldr	r1, [sp, #68]
	movs	r3, #192
	lsls	r2, r1, #4
	lsls	r3, r3, #2
	subs	r2, r3, r2
.L_0814676a:
	ldr	r4, [sp, #80]
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_0814677e
	ldr	r0, [sp, #48]
	ldrh	r3, [r0, #54]
	adds	r1, r0, #0
	subs	r3, r3, r2
	strh	r3, [r1, #54]
	b.n	.L_08146788
.L_0814677e:
	ldr	r4, [sp, #48]
	ldrh	r3, [r4, #54]
	adds	r0, r4, #0
	adds	r3, r3, r2
	strh	r3, [r0, #54]
.L_08146788:
	ldr	r2, [sp, #76]
	movs	r1, #0
	str	r1, [sp, #56]
	str	r1, [sp, #24]
	str	r2, [sp, #20]
	str	r1, [sp, #16]
.L_08146794:
	ldr	r3, [sp, #68]
	ldr	r4, [sp, #16]
	cmp	r3, r4
	bge.n	.L_0814679e
	b.n	.L_081469f8
.L_0814679e:
	ldr	r0, [sp, #20]
	subs	r5, r3, r4
	mov	sl, r0
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_081467ac
	adds	r3, r5, #3
.L_081467ac:
	asrs	r3, r3, #2
	adds	r3, #2
	mov	r8, r3
	cmp	r3, #10
	ble.n	.L_081467ba
	movs	r1, #10
	mov	r8, r1
.L_081467ba:
	bl	sub_08014de4
	ldr	r0, [sp, #48]
	adds	r1, r0, #0
	adds	r1, #12
	bl	sub_080156e8
	ldr	r0, [sp, #20]
	bl	sub_08015128
	movs	r2, #0
	str	r2, [sp, #44]
	movs	r0, #128
	ldr	r2, [sp, #24]
	lsls	r3, r5, #12
	lsls	r0, r0, #5
	adds	r0, r3, r0
	str	r0, [sp, #36]
	lsls	r3, r2, #3
	ldr	r0, [pc, #472]
	movs	r1, #96
	subs	r3, r3, r2
	add	r1, sp
	lsls	r3, r3, #2
	str	r5, [sp, #40]
	movs	r4, #0
	add	r7, sp, #84
	add	r6, sp, #108
	mov	r9, r1
	adds	r5, r3, r0
.L_081467f6:
	str	r4, [sp, #12]
	bl	sub_08014e38
	ldr	r1, [sp, #40]
	lsls	r0, r1, #10
	bl	sub_080150e4
	movs	r0, #128
	lsls	r0, r0, #7
	bl	sub_08015068
	ldr	r2, [sp, #36]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r2, [r7, #0]
	ldr	r4, [sp, #12]
	cmp	r2, r3
	ble.n	.L_0814681c
	str	r3, [r7, #0]
.L_0814681c:
	ldr	r3, [r7, #0]
	adds	r0, r7, #0
	str	r3, [r7, #4]
	str	r3, [r7, #8]
	str	r4, [sp, #12]
	bl	sub_080151ac
	movs	r3, #200
	ldr	r4, [sp, #12]
	lsls	r3, r3, #5
	adds	r3, #154
	adds	r0, r4, #0
	muls	r0, r3
	bl	sub_080150e4
	ldr	r4, [sp, #12]
	movs	r3, #1
	ands	r3, r4
	lsls	r0, r3, #1
	adds	r0, r0, r3
	ldr	r3, [pc, #376]
	lsls	r0, r0, #2
	adds	r0, r0, r3
	adds	r1, r6, #0
	bl	sub_0815e1ec
	ldr	r3, [sp, #44]
	ldr	r4, [sp, #12]
	cmp	r3, r0
	bge.n	.L_0814685a
	str	r0, [sp, #44]
.L_0814685a:
	ldr	r2, [r6, #0]
	mov	r0, r9
	asrs	r2, r2, #1
	str	r2, [r6, #0]
	str	r4, [sp, #12]
	ldr	r3, [r0, #0]
	adds	r2, r2, r3
	str	r2, [r5, #12]
	ldr	r3, [r0, #4]
	ldr	r2, [r6, #4]
	adds	r2, r2, r3
	str	r2, [r5, #16]
	ldr	r3, [r6, #0]
	str	r3, [r5, #12]
	ldr	r3, [r6, #4]
	str	r3, [r5, #16]
	bl	sub_08014ea8
	ldr	r4, [sp, #12]
	adds	r5, #28
	adds	r4, #1
	cmp	r4, #10
	bne.n	.L_081467f6
	ldr	r1, [sp, #44]
	ldr	r2, [pc, #312]
	cmp	r1, r2
	bgt.n	.L_08146926
	ldr	r3, [sp, #24]
	mov	r1, r8
	str	r3, [sp, #32]
	lsrs	r3, r1, #31
	add	r3, r8
	asrs	r3, r3, #1
	mov	r0, r8
	str	r3, [sp, #28]
	lsls	r0, r0, #1
	movs	r4, #0
	mov	r9, r0
.L_081468a6:
	ldr	r3, [sp, #32]
	ldr	r0, [pc, #272]
	adds	r2, r4, r3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r4, #1
	adds	r7, r3, r0
	movs	r1, #10
	adds	r0, r4, #0
	str	r4, [sp, #8]
	bl	sub_08002064
	ldr	r1, [sp, #32]
	ldr	r2, [pc, #248]
	adds	r0, r0, r1
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #2
	adds	r6, r3, r2
	movs	r5, #0
.L_081468d0:
	ldr	r3, [r6, #12]
	ldr	r2, [r7, #12]
	subs	r3, r3, r2
	muls	r3, r5
	cmp	r3, #0
	bge.n	.L_081468de
	adds	r3, #15
.L_081468de:
	asrs	r3, r3, #4
	adds	r0, r2, r3
	ldr	r3, [r6, #16]
	ldr	r2, [r7, #16]
	subs	r3, r3, r2
	muls	r3, r5
	cmp	r3, #0
	bge.n	.L_081468f0
	adds	r3, #15
.L_081468f0:
	asrs	r3, r3, #4
	adds	r2, r2, r3
	ldr	r3, [pc, #208]
	mov	ip, r2
	mov	r2, r9
	subs	r2, #2
	ldrh	r1, [r3, r2]
	ldr	r4, [sp, #52]
	ldr	r3, [sp, #28]
	adds	r1, r4, r1
	subs	r2, r0, r3
	mov	r4, ip
	mov	r0, r8
	subs	r3, r4, r0
	mov	r4, r9
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #72]
	ldr	r4, [sp, #60]
	adds	r5, #1
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2d10
	bne.n	.L_081468d0
	ldr	r4, [sp, #8]
	cmp	r4, #10
	bne.n	.L_081468a6
.L_08146926:
	mov	r0, sl
	ldr	r3, [r0, #0]
	ldr	r2, [r0, #12]
	adds	r3, r3, r2
	str	r3, [r0, #0]
	ldr	r2, [r0, #16]
	ldr	r3, [r0, #4]
	adds	r3, r3, r2
	str	r3, [r0, #4]
	ldr	r2, [r0, #20]
	ldr	r3, [r0, #8]
	adds	r3, r3, r2
	str	r3, [r0, #8]
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #56]
	ldr	r4, [sp, #68]
	adds	r3, r1, r2
	adds	r3, #10
	cmp	r4, r3
	bne.n	.L_081469f8
	movs	r3, #128
	mov	r0, fp
	lsls	r3, r3, #10
	str	r3, [r0, #52]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r0, #48]
	movs	r3, #160
	lsls	r3, r3, #11
	str	r3, [r0, #40]
	movs	r3, #171
	lsls	r3, r3, #8
	adds	r3, #133
	mov	r2, fp
	str	r3, [r0, #72]
	adds	r2, #90
	movs	r3, #0
	strb	r3, [r2, #0]
	bl	sub_08020138
	mov	r2, fp
	ldr	r1, [r2, #8]
	cmp	r1, #0
	bge.n	.L_0814698e
	ldr	r3, [pc, #76]
	mov	r0, fp
	adds	r1, r1, r3
	ldr	r3, [r2, #16]
	movs	r2, #0
	bl	sub_08020148
	b.n	.L_0814699e
.L_0814698e:
	movs	r4, #160
	lsls	r4, r4, #14
	mov	r0, fp
	adds	r1, r1, r4
	ldr	r3, [r0, #16]
	movs	r2, #0
	bl	sub_08020148
.L_0814699e:
	ldr	r1, [sp, #56]
	cmp	r1, #2
	bne.n	.L_081469d0
	movs	r0, #134
	bl	sub_081180e8
	b.n	.L_081469ea
	.4byte 0x0000013a
	.4byte 0x03000730
	.4byte 0x00000134
	.4byte 0x08143001
	.4byte 0x02010000
	.4byte 0x08197924
	.4byte 0x00061a7f
	.4byte 0x08197410
	.2byte 0x0000
	.2byte 0xffd8
.L_081469d0:
	.2byte 0x2086
	bl	sub_081c0010
	ldr	r3, [sp, #80]
	movs	r1, #7
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
.L_081469ea:
	ldr	r4, [sp, #76]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #168
	adds	r2, r4, r0
	movs	r3, #4
	str	r3, [r2, #0]
.L_081469f8:
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #20]
	ldr	r3, [sp, #16]
	ldr	r4, [sp, #56]
	adds	r1, #10
	adds	r2, #28
	adds	r3, #12
	adds	r4, #1
	str	r1, [sp, #24]
	str	r2, [sp, #20]
	str	r3, [sp, #16]
	str	r4, [sp, #56]
	cmp	r4, #3
	beq.n	.L_08146a16
	b.n	.L_08146794
.L_08146a16:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r1, #240
	ldr	r0, [sp, #76]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r0, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #68]
	adds	r2, #1
	str	r2, [sp, #68]
	cmp	r2, #60
	beq.n	.L_08146a42
	b.n	.L_08146754
.L_08146a42:
	ldr	r0, [pc, #36]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #120
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #68
	str	r0, [sp, #52]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #92]
	str	r0, [sp, #48]
	movs	r0, #1
	ldr	r1, [r5, #96]
	str	r1, [sp, #44]
	ldr	r2, [r5, #100]
	str	r2, [sp, #24]
	ldr	r3, [r5, #48]
	str	r3, [sp, #20]
	bl	sub_081435e0
	ldr	r0, [pc, #836]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #832]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2200
	ldr	r1, [sp, #24]
	movs	r3, #0
	ldr	r0, [pc, #820]
	bl	sub_08157cf4
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	ldr	r5, [r5, #104]
	movs	r0, #239
	str	r5, [sp, #32]
	ldr	r5, [sp, #48]
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	adds	r2, r5, r0
	movs	r3, #2
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r2, r5, r1
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #776]
	bl	sub_080145a8
	ldr	r2, [sp, #52]
	ldr	r0, [r2, #8]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r5, [sp, #52]
	mov	r8, r0
	movs	r3, #36
	ldrsh	r0, [r5, r3]
	bl	sub_08118098
	ldr	r7, [r0, #0]
	movs	r0, #0
	str	r0, [sp, #28]
	ldr	r6, [sp, #48]
.L_08146b04:
	mov	r2, r8
	ldr	r3, [r2, #8]
	ldr	r1, [sp, #28]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #0]
	mov	r0, r8
	ldr	r3, [r0, #12]
	lsls	r5, r1, #3
	movs	r1, #240
	lsls	r1, r1, #15
	adds	r3, r3, r1
	str	r3, [r6, #4]
	ldr	r3, [r0, #16]
	str	r3, [r6, #8]
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	ldr	r0, [r7, #8]
	subs	r3, #64
	lsls	r3, r3, #16
	adds	r0, r0, r3
	ldr	r3, [r6, #0]
	movs	r1, #12
	subs	r0, r0, r3
	bl	sub_08002054
	str	r0, [r6, #12]
	ldr	r3, [r6, #4]
	ldr	r0, [r7, #12]
	movs	r2, #160
	lsls	r2, r2, #13
	subs	r0, r0, r3
	adds	r0, r0, r2
	movs	r1, #12
	bl	sub_08002054
	str	r0, [r6, #16]
	ldr	r3, [r6, #8]
	ldr	r0, [r7, #16]
	movs	r1, #12
	subs	r0, r0, r3
	bl	sub_08002054
	str	r0, [r6, #20]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, r3, r5
	str	r3, [r6, #24]
	ldr	r3, [sp, #28]
	adds	r6, #28
	adds	r3, #1
	str	r3, [sp, #28]
	cmp	r3, #8
	bne.n	.L_08146b04
	movs	r5, #0
	str	r5, [sp, #40]
.L_08146b7e:
	movs	r1, #170
	movs	r2, #170
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [sp, #40]
	adds	r1, #171
	adds	r2, #85
	movs	r3, #0
	bl	sub_081496c8
	ldr	r0, [sp, #40]
	cmp	r0, #96
	bne.n	.L_08146b9e
	movs	r0, #134
	bl	sub_081180e8
.L_08146b9e:
	movs	r1, #0
	str	r1, [sp, #28]
	str	r1, [sp, #16]
	ldr	r7, [sp, #48]
.L_08146ba6:
	ldr	r3, [r7, #24]
	ldr	r2, [sp, #40]
	cmp	r2, r3
	bge.n	.L_08146bb0
	b.n	.L_08146d7a
.L_08146bb0:
	bl	sub_08014de4
	ldr	r0, [sp, #20]
	adds	r1, r0, #0
	adds	r1, #12
	bl	sub_080156e8
	add	r5, sp, #56
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0815e1ec
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	adds	r3, #8
	cmp	r3, #135
	bls.n	.L_08146bd6
	b.n	.L_08146cfe
.L_08146bd6:
	ldr	r3, [r5, #4]
	cmp	r3, #127
	ble.n	.L_08146bde
	b.n	.L_08146cfe
.L_08146bde:
	movs	r0, #8
	negs	r0, r0
	cmp	r3, r0
	bge.n	.L_08146be8
	b.n	.L_08146cfe
.L_08146be8:
	ldr	r1, [sp, #28]
	ldr	r3, [pc, #516]
	lsls	r2, r1, #2
	adds	r2, r2, r1
	mov	sl, r5
	mov	r8, r3
	ldr	r5, [pc, #508]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #3
	movs	r4, #0
	adds	r1, r3, r5
.L_08146c00:
	movs	r3, #200
	ldr	r2, [sp, #40]
	ldr	r0, [r7, #24]
	lsls	r3, r3, #5
	adds	r3, #154
	adds	r6, r4, #0
	muls	r6, r3
	subs	r0, r2, r0
	lsls	r0, r0, #11
	subs	r0, r6, r0
	str	r1, [sp, #12]
	str	r4, [sp, #8]
	bl	sub_08002096
	ldr	r4, [sp, #8]
	movs	r5, #1
	ands	r5, r4
	mov	r2, r8
	ldrb	r3, [r2, r5]
	ldr	r1, [sp, #12]
	adds	r2, r3, #0
	muls	r2, r0
	mov	r0, sl
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r0, #0]
	asrs	r2, r2, #17
	adds	r3, r3, r2
	str	r3, [r1, #12]
	ldr	r2, [sp, #40]
	ldr	r3, [r7, #24]
	subs	r3, r2, r3
	lsls	r3, r3, #11
	subs	r6, r6, r3
	adds	r0, r6, #0
	bl	sub_08002090
	mov	r2, r8
	ldrb	r3, [r2, r5]
	mov	r5, sl
	adds	r2, r3, #0
	muls	r2, r0
	ldr	r4, [sp, #8]
	ldr	r3, [r5, #4]
	ldr	r1, [sp, #12]
	asrs	r2, r2, #16
	subs	r3, r3, r2
	adds	r4, #1
	str	r3, [r1, #16]
	adds	r1, #28
	cmp	r4, #10
	bne.n	.L_08146c00
	ldr	r0, [sp, #16]
	movs	r4, #0
	mov	fp, r0
.L_08146c6e:
	mov	r1, fp
	adds	r2, r4, r1
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r2, [pc, #380]
	adds	r4, #1
	lsls	r3, r3, #2
	mov	r9, r4
	adds	r2, r2, r3
	mov	r0, r9
	movs	r1, #10
	mov	sl, r2
	bl	sub_08002064
	add	r0, fp
	lsls	r3, r0, #3
	ldr	r5, [pc, #356]
	subs	r3, r3, r0
	lsls	r3, r3, #2
	adds	r5, r5, r3
	mov	r8, r5
	movs	r4, #0
.L_08146c9a:
	mov	r0, r8
	mov	r1, sl
	ldr	r6, [r1, #12]
	ldr	r3, [r0, #12]
	movs	r1, #12
	subs	r3, r3, r6
	adds	r0, r4, #0
	muls	r0, r3
	str	r4, [sp, #8]
	bl	sub_08002054
	mov	r2, r8
	adds	r6, r6, r0
	mov	r0, sl
	ldr	r3, [r2, #16]
	ldr	r5, [r0, #16]
	ldr	r4, [sp, #8]
	subs	r3, r3, r5
	adds	r0, r4, #0
	muls	r0, r3
	movs	r1, #12
	bl	sub_08002054
	ldr	r2, [pc, #300]
	movs	r3, #4
	subs	r3, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #24]
	adds	r5, r5, r0
	movs	r0, #1
	adds	r1, r3, r1
	subs	r6, r6, r0
	subs	r5, #2
	movs	r2, #2
	movs	r3, #4
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	ldr	r0, [sp, #44]
	adds	r3, r5, #0
	adds	r2, r6, #0
	ldr	r5, [sp, #32]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9c02
	adds	r4, #1
	cmp	r4, #12
	bne.n	.L_08146c9a
	mov	r4, r9
	cmp	r4, #10
	bne.n	.L_08146c6e
.L_08146cfe:
	ldr	r3, [r7, #4]
	ldr	r0, [pc, #248]
	cmp	r3, r0
	bgt.n	.L_08146d62
	ldr	r3, [r7, #16]
	movs	r0, #134
	negs	r3, r3
	str	r3, [r7, #16]
	ldr	r3, [r7, #12]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r7, #12]
	ldr	r3, [r7, #20]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r7, #20]
	ldr	r1, [sp, #48]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	adds	r2, r1, r3
	movs	r3, #4
	str	r3, [r2, #0]
	bl	sub_081c0010
	ldr	r5, [sp, #52]
	movs	r4, #0
	ldr	r3, [r5, #20]
	cmp	r3, #0
	beq.n	.L_08146d62
	movs	r6, #8
	movs	r5, #36
.L_08146d42:
	ldr	r1, [sp, #52]
	adds	r3, r4, #0
	ldrsh	r0, [r5, r1]
	movs	r1, #7
	movs	r2, #5
	str	r4, [sp, #8]
	str	r6, [sp, #0]
	bl	sub_0814cd48
	ldr	r0, [sp, #52]
	ldr	r4, [sp, #8]
	ldr	r3, [r0, #20]
	adds	r4, #1
	adds	r5, #2
	cmp	r4, r3
	bne.n	.L_08146d42
.L_08146d62:
	ldr	r3, [r7, #0]
	ldr	r2, [r7, #12]
	adds	r3, r3, r2
	str	r3, [r7, #0]
	ldr	r2, [r7, #16]
	ldr	r3, [r7, #4]
	adds	r3, r3, r2
	str	r3, [r7, #4]
	ldr	r2, [r7, #20]
	ldr	r3, [r7, #8]
	adds	r3, r3, r2
	str	r3, [r7, #8]
.L_08146d7a:
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #28]
	adds	r1, #10
	adds	r2, #1
	str	r1, [sp, #16]
	adds	r7, #28
	str	r2, [sp, #28]
	cmp	r2, #8
	beq.n	.L_08146d8e
	b.n	.L_08146ba6
.L_08146d8e:
	movs	r0, #4
	movs	r1, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r5, #240
	ldr	r3, [sp, #48]
	lsls	r5, r5, #7
	adds	r5, #232
	adds	r2, r3, r5
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	ldr	r0, [sp, #40]
	adds	r0, #1
	str	r0, [sp, #40]
	cmp	r0, #128
	beq.n	.L_08146dba
	b.n	.L_08146b7e
.L_08146dba:
	ldr	r0, [pc, #48]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0000013a
	.4byte 0x03000730
	.4byte 0x00000134
	.4byte 0x08143001
	.4byte 0x0819793c
	.4byte 0x02010000
	.4byte 0x08197410
	.2byte 0xffff
	.2byte 0x001d
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #364
	str	r0, [sp, #100]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #96]
	movs	r6, #224
	str	r0, [sp, #96]
	movs	r0, #0
	ldr	r1, [r3, #48]
	lsls	r6, r6, #3
	str	r1, [sp, #92]
	movs	r7, #239
	ldr	r2, [r3, #92]
	lsls	r7, r7, #7
	str	r2, [sp, #88]
	ldr	r3, [r3, #100]
	str	r3, [sp, #80]
	bl	sub_081435e0
	mov	r3, sp
	adds	r3, #112
	adds	r1, r3, #0
	movs	r0, #0
	str	r3, [sp, #76]
	bl	sub_08144aac
	ldr	r5, [sp, #88]
	ldr	r0, [pc, #716]
	adds	r1, r5, r6
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r0, [pc, #708]
	ldr	r1, [sp, #80]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r2, r5, r7
	movs	r3, #2
	adds	r0, #132
	str	r3, [r2, #0]
	movs	r1, #200
	adds	r2, r5, r0
	movs	r3, #50
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #680]
	bl	sub_080145a8
	ldr	r5, [sp, #100]
	movs	r3, #0
	str	r3, [sp, #60]
	str	r3, [sp, #64]
	mov	r9, r3
	ldr	r3, [r5, #20]
	movs	r1, #128
	movs	r2, #160
	lsls	r1, r1, #16
	lsls	r2, r2, #14
	str	r1, [sp, #68]
	str	r2, [sp, #72]
	cmp	r3, #0
	beq.n	.L_08146f1c
	movs	r7, #166
	movs	r6, #172
	lsls	r7, r7, #1
	add	r6, sp
	add	r7, sp
	movs	r0, #36
	mov	fp, r6
	mov	sl, r7
	add	r2, sp, #300
	add	r7, sp, #268
	movs	r4, #0
	add	r6, sp, #204
	mov	r8, r0
.L_08146eae:
	ldr	r5, [sp, #100]
	mov	r1, r8
	ldrsh	r0, [r1, r5]
	str	r2, [sp, #12]
	str	r4, [sp, #8]
	bl	sub_08118098
	ldr	r5, [r0, #0]
	ldr	r4, [sp, #8]
	ldr	r3, [r5, #8]
	mov	r0, fp
	str	r3, [r6, #0]
	ldr	r3, [r5, #16]
	str	r3, [r6, #4]
	adds	r6, #8
	ldrh	r3, [r5, #6]
	str	r3, [r4, r0]
	ldr	r1, [r5, #16]
	ldr	r0, [r5, #8]
	bl	sub_080148e8
	ldr	r4, [sp, #8]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	mov	r1, sl
	str	r0, [r4, r1]
	ldr	r3, [r5, #8]
	asrs	r3, r3, #8
	adds	r0, r3, #0
	muls	r0, r3
	ldr	r3, [r5, #16]
	asrs	r3, r3, #8
	adds	r1, r3, #0
	muls	r1, r3
	adds	r3, r1, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #552]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c02
	ldr	r2, [sp, #12]
	movs	r3, #0
	asrs	r0, r0, #7
	str	r0, [r4, r2]
	str	r3, [r4, r7]
	str	r3, [r5, #72]
	ldr	r0, [sp, #100]
	movs	r3, #2
	add	r8, r3
	ldr	r3, [r0, #20]
	movs	r5, #1
	add	r9, r5
	adds	r4, #4
	cmp	r9, r3
	bne.n	.L_08146eae
.L_08146f1c:
	ldr	r5, [sp, #88]
	movs	r1, #0
	mov	r9, r1
	movs	r6, #0
.L_08146f24:
	movs	r3, #120
	str	r3, [r5, #8]
	str	r6, [r5, #4]
	bl	sub_08014878
	str	r6, [r5, #16]
	str	r6, [r5, #12]
	bl	sub_08014878
	movs	r3, #63
	movs	r2, #1
	ands	r3, r0
	add	r9, r2
	str	r3, [r5, #24]
	mov	r3, r9
	adds	r5, #28
	cmp	r3, #64
	bne.n	.L_08146f24
	ldr	r6, [sp, #92]
	movs	r5, #0
	adds	r6, #12
	str	r5, [sp, #84]
	str	r6, [sp, #40]
.L_08146f52:
	ldr	r3, [pc, #464]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08146f7a
	ldr	r7, [sp, #84]
	cmp	r7, #159
	bgt.n	.L_08146f6a
	movs	r0, #160
	str	r0, [sp, #84]
	b.n	.L_08146f7a
.L_08146f6a:
	ldr	r1, [sp, #84]
	movs	r2, #197
	lsls	r2, r2, #1
	cmp	r1, r2
	bgt.n	.L_08146f7a
	movs	r3, #140
	adds	r3, #255
	str	r3, [sp, #84]
.L_08146f7a:
	bl	sub_08014de4
	ldr	r0, [sp, #92]
	ldr	r1, [sp, #40]
	bl	sub_080156e8
	ldr	r5, [sp, #84]
	cmp	r5, #16
	bne.n	.L_08146f92
	movs	r0, #141
	bl	sub_081c0010
.L_08146f92:
	ldr	r6, [sp, #84]
	movs	r7, #128
	lsls	r7, r7, #1
	cmp	r6, r7
	bne.n	.L_08146fa2
	movs	r0, #140
	bl	sub_081c0010
.L_08146fa2:
	ldr	r0, [sp, #84]
	movs	r1, #167
	lsls	r1, r1, #1
	cmp	r0, r1
	bne.n	.L_08146fb2
	movs	r0, #212
	bl	sub_081c0010
.L_08146fb2:
	ldr	r2, [sp, #84]
	movs	r3, #92
	adds	r3, #255
	cmp	r2, r3
	bne.n	.L_08146fc2
	movs	r0, #212
	bl	sub_081c0010
.L_08146fc2:
	ldr	r5, [sp, #84]
	movs	r6, #104
	adds	r6, #255
	cmp	r5, r6
	bne.n	.L_08146fd2
	movs	r0, #212
	bl	sub_081c0010
.L_08146fd2:
	ldr	r7, [sp, #84]
	movs	r0, #186
	lsls	r0, r0, #1
	cmp	r7, r0
	bne.n	.L_08146fe2
	movs	r0, #212
	bl	sub_081c0010
.L_08146fe2:
	ldr	r2, [sp, #100]
	movs	r1, #0
	ldr	r3, [r2, #20]
	mov	r9, r1
	cmp	r3, #0
	bne.n	.L_08146ff0
	b.n	.L_08147146
.L_08146ff0:
	movs	r5, #36
	str	r5, [sp, #16]
	movs	r4, #0
.L_08146ff6:
	ldr	r0, [sp, #84]
	mov	r6, r9
	lsls	r7, r6, #4
	cmp	r0, r7
	bgt.n	.L_08147002
	b.n	.L_08147134
.L_08147002:
	lsls	r6, r6, #1
	str	r6, [sp, #56]
	ldr	r3, [sp, #100]
	ldr	r1, [sp, #16]
	str	r4, [sp, #8]
	ldrsh	r0, [r1, r3]
	bl	sub_08118098
	ldr	r4, [sp, #8]
	add	r6, sp, #332
	ldr	r5, [r0, #0]
	ldr	r0, [r6, r4]
	mov	r8, r4
	bl	sub_08002096
	ldr	r4, [sp, #8]
	add	r2, sp, #300
	ldr	r3, [r2, r4]
	mov	sl, r2
	muls	r3, r0
	asrs	r3, r3, #1
	str	r3, [r5, #8]
	str	r2, [sp, #12]
	ldr	r0, [r6, r4]
	bl	sub_08002090
	ldr	r2, [sp, #12]
	ldr	r4, [sp, #8]
	ldr	r3, [r2, r4]
	muls	r3, r0
	asrs	r3, r3, #1
	str	r3, [r5, #16]
	ldr	r0, [sp, #84]
	cmp	r0, #159
	bgt.n	.L_0814707e
	adds	r3, r7, #0
	adds	r3, #16
	cmp	r0, r3
	ble.n	.L_08147058
	add	r7, sp, #268
	ldr	r3, [r7, r4]
	adds	r3, #48
	str	r3, [r7, r4]
.L_08147058:
	ldr	r3, [r2, r4]
	cmp	r3, #31
	bgt.n	.L_08147068
	ldr	r3, [r5, #12]
	movs	r1, #192
	lsls	r1, r1, #11
	adds	r3, r3, r1
	b.n	.L_08147070
.L_08147068:
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r2
.L_08147070:
	str	r3, [r5, #12]
	ldr	r3, [r5, #12]
	movs	r2, #248
	lsls	r2, r2, #15
	cmp	r3, r2
	ble.n	.L_0814707e
	str	r2, [r5, #12]
.L_0814707e:
	ldr	r3, [sp, #84]
	movs	r7, #245
	lsls	r7, r7, #1
	cmp	r3, r7
	bgt.n	.L_081470b4
	add	r7, sp, #268
	ldr	r2, [r6, r4]
	ldr	r3, [r7, r4]
	movs	r0, #128
	adds	r2, r2, r3
	lsls	r0, r0, #9
	str	r2, [r6, r4]
	cmp	r2, r0
	ble.n	.L_081470a2
	ldr	r1, [pc, #140]
	adds	r3, r2, r1
	mov	r2, r8
	str	r3, [r6, r2]
.L_081470a2:
	mov	r3, r8
	ldr	r2, [r7, r3]
	cmp	r2, #0
	bge.n	.L_081470ac
	adds	r2, #3
.L_081470ac:
	ldrh	r3, [r5, #6]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	strh	r3, [r5, #6]
.L_081470b4:
	ldr	r6, [sp, #84]
	movs	r7, #140
	adds	r7, #255
	cmp	r6, r7
	bne.n	.L_081470c6
	movs	r3, #171
	lsls	r3, r3, #8
	adds	r3, #133
	str	r3, [r5, #72]
.L_081470c6:
	ldr	r6, [pc, #100]
	movs	r0, #0
	mov	fp, r0
.L_081470cc:
	ldrh	r3, [r6, #0]
	ldr	r1, [sp, #84]
	adds	r6, #2
	cmp	r1, r3
	bne.n	.L_081470f4
	movs	r3, #0
	str	r3, [r5, #40]
	ldr	r3, [sp, #56]
	ldr	r2, [sp, #100]
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r3, r9
	str	r4, [sp, #8]
	bl	sub_0814cd48
	ldr	r4, [sp, #8]
.L_081470f4:
	movs	r0, #1
	add	fp, r0
	mov	r1, fp
	cmp	r1, #4
	bne.n	.L_081470cc
	mov	r2, sl
	ldr	r3, [r2, r4]
	cmp	r3, #16
	ble.n	.L_08147130
	subs	r3, #2
	mov	r5, r8
	str	r3, [r2, r5]
	ldr	r6, [sp, #100]
	ldr	r3, [r6, #20]
	b.n	.L_08147134
	movs	r0, r0
	.4byte 0x00000143
	.4byte 0x00000134
	.4byte 0x08143001
	.4byte 0x030002d4
	.4byte 0x03001150
	.4byte 0xffff0000
	.2byte 0x7950
	.2byte 0x0819
.L_08147130:
	ldr	r7, [sp, #100]
	ldr	r3, [r7, #20]
.L_08147134:
	ldr	r0, [sp, #16]
	movs	r1, #1
	adds	r0, #2
	add	r9, r1
	adds	r4, #4
	str	r0, [sp, #16]
	cmp	r9, r3
	beq.n	.L_08147146
	b.n	.L_08146ff6
.L_08147146:
	ldr	r3, [sp, #84]
	subs	r3, #16
	cmp	r3, #143
	bhi.n	sub_081471a0
	ldr	r2, [sp, #84]
	lsls	r3, r2, #1
	adds	r5, r3, #0
	subs	r5, #32
	cmp	r5, #48
	ble.n	.L_0814715c
	movs	r5, #48
.L_0814715c:
	ldr	r0, [sp, #84]
	cmp	r0, #0
	bge.n	.L_08147164
	adds	r0, #3
.L_08147164:
	movs	r2, #3
	ldr	r1, [pc, #456]
	negs	r2, r2
	asrs	r0, r0, #2
	bl	sub_08002080
	movs	r3, #48
	subs	r2, r3, r5
	lsls	r1, r0, #1
	lsls	r3, r2, #1
	adds	r1, r1, r0
	adds	r3, r3, r2
	ldr	r6, [sp, #88]
	lsls	r3, r3, #4
	lsls	r1, r1, #10
	adds	r1, r1, r3
	movs	r7, #224
	movs	r0, #48
	adds	r1, r6, r1
	lsls	r7, r7, #3
	movs	r3, #112
	str	r0, [sp, #0]
	adds	r1, r1, r7
	subs	r3, r3, r5
	str	r5, [sp, #4]
	ldr	r4, [sp, #112]
	ldr	r0, [sp, #96]
	movs	r2, #32
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9b15
	subs	r3, #48
	cmp	r3, #111
	bhi.n	sub_081471f6
	ldr	r1, [sp, #84]
	lsls	r3, r1, #1
	adds	r5, r3, #0
	subs	r5, #96
	cmp	r5, #64
	ble.n	.L_081471b6
	movs	r5, #64
.L_081471b6:
	ldr	r0, [sp, #84]
	cmp	r0, #0
	bge.n	.L_081471be
	adds	r0, #3
.L_081471be:
	movs	r2, #3
	ldr	r1, [pc, #364]
	negs	r2, r2
	asrs	r0, r0, #2
	bl	sub_08002080
	movs	r3, #64
	subs	r3, r3, r5
	lsls	r1, r0, #1
	lsls	r2, r3, #1
	adds	r1, r1, r0
	adds	r2, r2, r3
	lsls	r2, r2, #4
	lsls	r1, r1, #10
	adds	r1, r1, r2
	ldr	r2, [sp, #88]
	movs	r6, #224
	adds	r1, r2, r1
	lsls	r6, r6, #3
	movs	r7, #48
	adds	r1, r1, r6
	str	r7, [sp, #0]
	str	r5, [sp, #4]
	ldr	r4, [sp, #112]
	ldr	r0, [sp, #96]
	movs	r2, #32
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9815
	subs	r0, #160
	mov	fp, r0
	cmp	r0, #239
	bhi.n	sub_08147268
	ldr	r3, [sp, #84]
	cmp	r3, #0
	bge.n	.L_08147208
	adds	r3, #3
.L_08147208:
	asrs	r3, r3, #2
	ldr	r1, [pc, #292]
	mov	r8, r3
	movs	r2, #3
	negs	r2, r2
	mov	r0, r8
	mov	sl, r1
	mov	r9, r2
	bl	sub_08002080
	ldr	r3, [sp, #88]
	lsls	r1, r0, #1
	adds	r1, r1, r0
	movs	r5, #48
	lsls	r1, r1, #10
	movs	r6, #224
	str	r5, [sp, #0]
	adds	r1, r3, r1
	lsls	r6, r6, #3
	movs	r5, #64
	ldr	r4, [sp, #112]
	movs	r3, #0
	adds	r1, r1, r6
	str	r5, [sp, #4]
	ldr	r0, [sp, #96]
	movs	r2, #8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4651
	mov	r2, r9
	mov	r0, r8
	bl	sub_08002080
	lsls	r1, r0, #1
	ldr	r7, [sp, #88]
	adds	r1, r1, r0
	lsls	r1, r1, #10
	movs	r0, #48
	adds	r1, r7, r1
	str	r0, [sp, #0]
	adds	r1, r1, r6
	str	r5, [sp, #4]
	ldr	r4, [sp, #112]
	ldr	r0, [sp, #96]
	movs	r2, #8
	movs	r3, #64
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9915
	cmp	r1, #159
	bgt.n	.L_0814734e
	movs	r3, #160
	ldr	r6, [sp, #88]
	movs	r2, #0
	add	r3, sp
	mov	r9, r2
	mov	sl, r3
.L_0814727a:
	ldr	r5, [r6, #24]
	mov	r8, r5
	cmp	r5, #0
	bne.n	.L_0814733c
	ldr	r0, [r6, #16]
	bl	sub_08002096
	ldr	r3, [r6, #8]
	mov	r5, sl
	muls	r3, r0
	str	r3, [r5, #0]
	ldr	r3, [r6, #4]
	str	r3, [r5, #4]
	ldr	r0, [r6, #16]
	bl	sub_08002090
	ldr	r3, [r6, #8]
	add	r7, sp, #148
	muls	r3, r0
	adds	r1, r7, #0
	str	r3, [r5, #8]
	adds	r0, r5, #0
	bl	sub_0815e1ec
	ldr	r3, [r7, #0]
	ldr	r0, [pc, #132]
	asrs	r2, r3, #1
	str	r2, [r7, #0]
	ldr	r3, [r6, #4]
	cmp	r3, r0
	bgt.n	sub_081472d4
	ldr	r3, [r7, #4]
	ldr	r5, [sp, #88]
	movs	r7, #172
	movs	r1, #16
	lsls	r7, r7, #6
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	subs	r2, #12
	subs	r3, #12
	ldr	r4, [sp, #112]
	ldr	r0, [sp, #96]
	adds	r1, r5, r7
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x68b3
	cmp	r3, #24
	ble.n	.L_081472de
	subs	r3, #4
	str	r3, [r6, #8]
.L_081472de:
	ldr	r3, [r6, #12]
	ldr	r2, [r6, #16]
	lsls	r3, r3, #1
	movs	r0, #128
	adds	r2, r2, r3
	lsls	r0, r0, #9
	str	r2, [r6, #16]
	cmp	r2, r0
	ble.n	.L_081472f6
	ldr	r1, [pc, #68]
	adds	r3, r2, r1
	str	r3, [r6, #16]
.L_081472f6:
	ldr	r3, [r6, #12]
	movs	r2, #128
	adds	r3, #50
	lsls	r2, r2, #5
	str	r3, [r6, #12]
	cmp	r3, r2
	ble.n	.L_08147306
	str	r2, [r6, #12]
.L_08147306:
	ldr	r3, [r6, #16]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldr	r2, [r6, #4]
	str	r3, [r6, #16]
	lsls	r3, r3, #1
	adds	r2, r2, r3
	movs	r3, #192
	lsls	r3, r3, #14
	str	r2, [r6, #4]
	cmp	r2, r3
	ble.n	.L_08147342
	mov	r5, r8
	movs	r3, #100
	str	r5, [r6, #4]
	str	r3, [r6, #8]
	str	r5, [r6, #16]
	str	r5, [r6, #12]
	b.n	.L_08147342
	movs	r0, r0
	.4byte 0x55555556
	.4byte 0x003fffff
	.2byte 0x0000
	.2byte 0xffff
.L_0814733c:
	.2byte 0x4643
	subs	r3, #1
	str	r3, [r6, #24]
.L_08147342:
	movs	r7, #1
	add	r9, r7
	mov	r0, r9
	adds	r6, #28
	cmp	r0, #24
	bne.n	.L_0814727a
.L_0814734e:
	ldr	r1, [sp, #84]
	cmp	r1, #160
	beq.n	.L_08147356
	b.n	.L_08147474
.L_08147356:
	ldr	r5, [pc, #748]
	movs	r0, #1
	adds	r1, r5, #0
	movs	r2, #0
	bl	sub_08118040
	adds	r1, r5, #0
	movs	r0, #1
	movs	r2, #8
	bl	sub_08118038
	movs	r5, #240
	ldr	r2, [sp, #88]
	lsls	r5, r5, #7
	adds	r5, #240
	adds	r3, r2, r5
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	movs	r1, #128
	lsls	r1, r1, #2
	movs	r0, #9
	adds	r1, #134
	movs	r2, #2
	bl	sub_08152404
	ldr	r6, [sp, #88]
	movs	r7, #244
	lsls	r7, r7, #6
	ldr	r0, [pc, #692]
	adds	r1, r6, r7
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r3, [pc, #684]
	movs	r0, #0
	movs	r2, #128
	mov	r9, r0
	movs	r1, #0
	lsls	r2, r2, #2
.L_081473a8:
	movs	r5, #1
	add	r9, r5
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r9, r2
	bne.n	.L_081473a8
	ldr	r7, [sp, #100]
	movs	r6, #0
	ldr	r3, [r7, #20]
	mov	r9, r6
	cmp	r3, #0
	beq.n	.L_081473e8
	movs	r6, #36
.L_081473c2:
	ldr	r1, [sp, #100]
	ldrsh	r0, [r6, r1]
	bl	sub_08118098
	ldr	r5, [r0, #0]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #8
	lsls	r3, r3, #16
	str	r3, [r5, #12]
	ldr	r5, [sp, #100]
	movs	r3, #1
	add	r9, r3
	ldr	r3, [r5, #20]
	adds	r6, #2
	cmp	r9, r3
	bne.n	.L_081473c2
.L_081473e8:
	ldr	r2, [pc, #612]
	movs	r3, #72
	str	r3, [r2, #12]
	ldr	r7, [sp, #88]
	movs	r6, #0
	mov	r9, r6
.L_081473f4:
	bl	sub_08014878
	movs	r3, #127
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08014878
	movs	r5, #254
	lsls	r5, r5, #7
	adds	r5, #255
	ands	r5, r0
	movs	r0, #159
	lsls	r0, r0, #8
	adds	r0, #255
	adds	r5, r5, r0
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	adds	r0, r5, #0
	str	r3, [r7, #0]
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	str	r3, [r7, #4]
	bl	sub_08014878
	movs	r1, #200
	bl	sub_0800206c
	movs	r1, #1
	subs	r0, #100
	add	r9, r1
	lsls	r0, r0, #16
	movs	r3, #0
	mov	r2, r9
	str	r0, [r7, #8]
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r2, #32
	bne.n	.L_081473f4
	ldr	r5, [sp, #100]
	mov	r9, r3
	ldr	r3, [r5, #20]
	cmp	r3, #0
	beq.n	.L_08147474
	movs	r4, #184
	lsls	r4, r4, #5
	add	r0, sp, #300
	movs	r5, #16
	add	r1, sp, #268
	adds	r4, #112
	movs	r2, #0
.L_08147462:
	str	r5, [r2, r0]
	str	r4, [r2, r1]
	ldr	r7, [sp, #100]
	movs	r6, #1
	ldr	r3, [r7, #20]
	add	r9, r6
	adds	r2, #4
	cmp	r9, r3
	bne.n	.L_08147462
.L_08147474:
	ldr	r0, [sp, #84]
	cmp	r0, #159
	bgt.n	.L_0814747c
	b.n	.L_08147a18
.L_0814747c:
	ldr	r3, [pc, #468]
	mov	r1, fp
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	movs	r5, #16
	str	r3, [sp, #104]
	str	r4, [sp, #108]
	cmp	r1, #64
	bgt.n	.L_08147490
	movs	r5, #32
.L_08147490:
	mov	r2, fp
	lsls	r0, r2, #7
	bl	sub_08002096
	ldr	r3, [sp, #68]
	lsls	r0, r0, #5
	asrs	r0, r0, #6
	subs	r0, r3, r0
	mov	r6, fp
	str	r0, [sp, #68]
	lsls	r0, r6, #9
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	ldr	r7, [sp, #72]
	ldr	r5, [sp, #60]
	ldr	r2, [sp, #64]
	asrs	r3, r3, #6
	adds	r3, r7, r3
	ldr	r6, [sp, #64]
	adds	r2, r2, r3
	str	r3, [sp, #72]
	lsrs	r3, r5, #31
	adds	r3, r3, r5
	asrs	r3, r3, #1
	ldr	r0, [sp, #60]
	str	r3, [sp, #60]
	lsrs	r3, r6, #31
	adds	r3, r3, r6
	asrs	r3, r3, #1
	ldr	r1, [sp, #68]
	str	r3, [sp, #64]
	movs	r3, #128
	lsls	r3, r3, #9
	add	r4, sp, #104
	str	r3, [sp, #104]
	str	r2, [sp, #72]
	str	r3, [r4, #4]
	add	r2, sp, #120
	movs	r3, #0
	adds	r0, r0, r1
	str	r3, [r2, #12]
	ldr	r7, [sp, #88]
	str	r0, [sp, #68]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #220
	adds	r5, r7, r0
	movs	r7, #255
	mov	r9, r3
	adds	r6, r2, #0
	lsls	r7, r7, #16
.L_081474fa:
	ldr	r3, [pc, #348]
	mov	r1, r9
	ldrb	r3, [r3, r1]
	ldr	r2, [sp, #68]
	lsls	r3, r3, #16
	movs	r0, #160
	adds	r3, r3, r2
	lsls	r0, r0, #14
	adds	r3, r3, r0
	str	r3, [r6, #0]
	ldr	r3, [pc, #332]
	lsls	r2, r1, #6
	ldrb	r3, [r3, r1]
	ldr	r1, [sp, #72]
	adds	r3, r3, r2
	lsls	r3, r3, #16
	adds	r3, r3, r1
	adds	r2, r4, #0
	str	r7, [r6, #4]
	str	r3, [r6, #8]
	ldmia	r5!, {r0}
	movs	r3, #0
	adds	r1, r6, #0
	str	r4, [sp, #8]
	bl	sub_08020010
	movs	r3, #1
	movs	r2, #128
	add	r9, r3
	lsls	r2, r2, #15
	mov	r0, r9
	adds	r7, r7, r2
	ldr	r4, [sp, #8]
	cmp	r0, #9
	bne.n	.L_081474fa
	ldr	r1, [sp, #84]
	cmp	r1, #255
	bgt.n	.L_08147548
	b.n	.L_08147a18
.L_08147548:
	movs	r3, #128
	add	r5, sp, #136
	movs	r2, #0
	lsls	r3, r3, #17
	str	r3, [r5, #8]
	str	r2, [r5, #0]
	str	r2, [r5, #4]
	mov	sl, r2
	bl	sub_08014de4
	adds	r0, r5, #0
	bl	sub_08015128
	ldr	r3, [sp, #84]
	movs	r5, #74
	adds	r5, #255
	cmp	r3, r5
	ble.n	.L_0814756e
	b.n	sub_081476be
.L_0814756e:
	movs	r6, #0
	mov	r9, r6
	ldr	r6, [sp, #88]
.L_08147574:
	ldr	r3, [r6, #0]
	asrs	r3, r3, #8
	adds	r0, r3, #0
	muls	r0, r3
	ldr	r3, [r6, #4]
	asrs	r3, r3, #8
	adds	r2, r3, #0
	muls	r2, r3
	ldr	r3, [r6, #8]
	adds	r0, r0, r2
	asrs	r3, r3, #8
	adds	r7, r3, #0
	muls	r7, r3
	adds	r3, r7, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #204]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1240
	mov	r8, r0
	cmp	r0, #0
	beq.n	.L_0814766c
	add	r7, sp, #148
	adds	r1, r7, #0
	adds	r0, r6, #0
	bl	sub_0815e1ec
	ldr	r3, [r7, #0]
	ldr	r0, [sp, #68]
	asrs	r3, r3, #17
	asrs	r2, r0, #17
	adds	r3, r3, r2
	adds	r3, #32
	ldr	r5, [sp, #72]
	str	r3, [r7, #0]
	movs	r1, #6
	ldrsh	r3, [r7, r1]
	asrs	r2, r5, #16
	adds	r3, r3, r2
	subs	r3, #4
	str	r3, [r7, #4]
	movs	r0, #10
	ldrsh	r3, [r7, r0]
	str	r3, [r7, #8]
	cmp	r3, #169
	bgt.n	.L_081475d4
	movs	r3, #170
	str	r3, [r7, #8]
.L_081475d4:
	ldr	r0, [r7, #8]
	movs	r3, #175
	lsls	r3, r3, #1
	cmp	r0, r3
	ble.n	.L_081475e2
	str	r3, [r7, #8]
	adds	r0, r3, #0
.L_081475e2:
	ldr	r1, [pc, #128]
	subs	r0, #170
	bl	sub_08002074
	movs	r3, #6
	subs	r4, r3, r0
	ldr	r2, [pc, #120]
	lsls	r0, r4, #1
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #80]
	lsrs	r3, r4, #31
	adds	r1, r2, r1
	ldr	r2, [r7, #0]
	adds	r3, r4, r3
	asrs	r3, r3, #1
	ldr	r5, [sp, #76]
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	str	r0, [sp, #4]
	str	r4, [sp, #0]
	subs	r3, r3, r4
	ldr	r0, [sp, #96]
	ldr	r4, [r5, #4]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x6835
	mov	r1, r8
	adds	r0, r5, #0
	bl	sub_08002054
	subs	r5, r5, r0
	str	r5, [r6, #0]
	ldr	r5, [r6, #4]
	mov	r1, r8
	adds	r0, r5, #0
	bl	sub_08002054
	subs	r5, r5, r0
	str	r5, [r6, #4]
	ldr	r5, [r6, #8]
	mov	r1, r8
	adds	r0, r5, #0
	bl	sub_08002054
	subs	r5, r5, r0
	str	r5, [r6, #8]
	b.n	.L_08147670
	movs	r0, r0
	.4byte 0x00000045
	.4byte 0x0000014b
	.4byte 0x02010018
	.4byte 0x030011e0
	.4byte 0x08196df8
	.4byte 0x0819793e
	.4byte 0x08197947
	.4byte 0x030002d4
	.4byte 0x071c71c8
	.2byte 0x7410
	.2byte 0x0819
.L_0814766c:
	movs	r7, #1
	add	sl, r7
.L_08147670:
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	adds	r6, #28
	cmp	r1, #32
	beq.n	.L_0814767e
	b.n	.L_08147574
.L_0814767e:
	mov	r2, sl
	cmp	r2, #0
	ble.n	sub_081476be
	movs	r1, #10
	mov	r0, sl
	bl	sub_08002054
	ldr	r2, [pc, #852]
	adds	r4, r0, #1
	lsls	r0, r4, #1
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #80]
	ldr	r5, [sp, #68]
	adds	r1, r3, r1
	ldr	r6, [sp, #72]
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	asrs	r3, r3, #1
	asrs	r2, r5, #17
	ldr	r7, [sp, #76]
	subs	r2, r2, r3
	asrs	r3, r6, #16
	subs	r3, r3, r4
	str	r0, [sp, #4]
	str	r4, [sp, #0]
	adds	r2, #32
	subs	r3, #4
	ldr	r4, [r7, #4]
	ldr	r0, [sp, #96]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2000
	mov	fp, r0
	mov	sl, r0
.L_081476c4:
	ldr	r2, [pc, #800]
	mov	r1, fp
	lsls	r3, r1, #1
	ldrh	r3, [r2, r3]
	ldr	r5, [sp, #84]
	cmp	r5, r3
	bne.n	.L_08147742
	ldr	r7, [pc, #792]
	movs	r6, #0
	mov	r9, r6
	add	r7, sl
	mov	r8, r6
.L_081476dc:
	bl	sub_08014878
	movs	r6, #127
	ands	r6, r0
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r6, #16
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	negs	r3, r3
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	subs	r3, #128
	mov	r0, r8
	lsls	r3, r3, #10
	str	r3, [r7, #16]
	str	r0, [r7, #0]
	str	r0, [r7, #4]
	str	r0, [r7, #8]
	bl	sub_08014878
	movs	r3, #15
	movs	r1, #1
	ands	r3, r0
	add	r9, r1
	adds	r3, #64
	mov	r2, r9
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r2, #96
	bne.n	.L_081476dc
	ldr	r2, [pc, #676]
.L_08147742:
	ldrh	r3, [r2, #0]
	ldr	r5, [sp, #84]
	cmp	r5, r3
	blt.n	.L_081477ea
	ldr	r5, [pc, #672]
	movs	r6, #0
	mov	r9, r6
.L_08147750:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	ble.n	.L_081477dc
	ldr	r3, [pc, #664]
	add	r1, sp, #148
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xaf25
	ldr	r3, [r7, #0]
	asrs	r3, r3, #17
	adds	r3, #32
	str	r3, [r7, #0]
	movs	r0, #6
	ldrsh	r3, [r7, r0]
	adds	r3, #56
	str	r3, [r7, #4]
	movs	r1, #10
	ldrsh	r3, [r7, r1]
	str	r3, [r7, #8]
	cmp	r3, #169
	bgt.n	.L_08147780
	movs	r3, #170
	str	r3, [r7, #8]
.L_08147780:
	ldr	r0, [r7, #8]
	movs	r3, #175
	lsls	r3, r3, #1
	cmp	r0, r3
	ble.n	.L_0814778e
	str	r3, [r7, #8]
	adds	r0, r3, #0
.L_0814778e:
	ldr	r1, [pc, #612]
	subs	r0, #170
	bl	sub_08002074
	movs	r3, #3
	subs	r4, r3, r0
	ldr	r2, [pc, #584]
	lsls	r0, r4, #1
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #80]
	lsrs	r3, r4, #31
	adds	r1, r2, r1
	ldr	r2, [r7, #0]
	adds	r3, r4, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	str	r4, [sp, #0]
	subs	r3, r3, r4
	str	r0, [sp, #4]
	ldr	r4, [sp, #112]
	ldr	r0, [sp, #96]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x682a
	ldr	r3, [r5, #12]
	adds	r2, r2, r3
	str	r2, [r5, #0]
	ldr	r3, [r5, #4]
	ldr	r2, [r5, #16]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r5, #8]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_081477dc:
	movs	r3, #1
	movs	r6, #128
	add	r9, r3
	lsls	r6, r6, #2
	adds	r5, #28
	cmp	r9, r6
	bne.n	.L_08147750
.L_081477ea:
	movs	r0, #1
	movs	r7, #224
	add	fp, r0
	lsls	r7, r7, #4
	mov	r1, fp
	add	sl, r7
	cmp	r1, #4
	beq.n	.L_081477fc
	b.n	.L_081476c4
.L_081477fc:
	movs	r2, #0
	mov	r9, r2
	str	r2, [sp, #28]
	ldr	r2, [sp, #68]
	movs	r3, #212
	lsls	r3, r3, #3
	str	r3, [sp, #32]
	ldr	r6, [pc, #492]
	ldr	r5, [sp, #84]
	movs	r7, #165
	asrs	r3, r2, #17
	lsls	r7, r7, #1
	ldr	r0, [sp, #88]
	adds	r3, #32
	movs	r1, #210
	str	r7, [sp, #20]
	str	r3, [sp, #52]
	adds	r6, r5, r6
	lsls	r1, r1, #3
	str	r6, [sp, #24]
	adds	r6, r0, r1
.L_08147826:
	ldr	r3, [sp, #84]
	ldr	r5, [sp, #20]
	cmp	r3, r5
	bne.n	.L_08147872
	ldr	r0, [sp, #72]
	ldr	r7, [sp, #52]
	asrs	r3, r0, #16
	subs	r3, #4
	str	r7, [r6, #0]
	str	r3, [r6, #4]
	str	r7, [r6, #12]
	str	r3, [r6, #16]
	ldr	r5, [sp, #88]
	movs	r7, #230
	movs	r1, #0
	lsls	r7, r7, #2
	mov	fp, r1
	movs	r2, #4
	adds	r3, r5, r7
.L_0814784c:
	movs	r0, #1
	add	fp, r0
	mov	r1, fp
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r1, #28
	bne.n	.L_0814784c
	ldr	r3, [pc, #416]
	ldr	r5, [sp, #88]
	movs	r7, #238
	movs	r2, #128
	lsls	r7, r7, #7
	lsls	r2, r2, #12
	adds	r7, #168
	str	r2, [sp, #60]
	str	r3, [sp, #64]
	adds	r2, r5, r7
	movs	r3, #8
	str	r3, [r2, #0]
.L_08147872:
	ldr	r0, [sp, #84]
	ldr	r1, [sp, #20]
	cmp	r0, r1
	bge.n	.L_0814787c
	b.n	.L_081479bc
.L_0814787c:
	ldr	r2, [sp, #24]
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r4, r3, #1
	cmp	r4, #2
	ble.n	.L_0814788a
	movs	r4, #2
.L_0814788a:
	ldr	r2, [pc, #372]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #88]
	ldr	r2, [r6, #0]
	adds	r1, r3, r1
	ldr	r3, [pc, #364]
	ldr	r7, [sp, #76]
	ldrb	r0, [r3, r4]
	ldr	r3, [r6, #4]
	movs	r5, #224
	subs	r3, r3, r0
	ldr	r0, [pc, #356]
	lsls	r5, r5, #3
	ldrb	r0, [r0, r4]
	adds	r1, r1, r5
	str	r0, [sp, #0]
	ldr	r0, [pc, #348]
	ldrb	r0, [r0, r4]
	str	r0, [sp, #4]
	ldr	r4, [r7, #4]
	ldr	r0, [sp, #96]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x6833
	subs	r3, #8
	str	r3, [r6, #0]
	ldr	r3, [r6, #4]
	adds	r3, #2
	str	r3, [r6, #4]
	ldr	r3, [sp, #20]
	ldr	r0, [sp, #84]
	adds	r3, #8
	cmp	r0, r3
	bge.n	.L_081479bc
	ldr	r0, [pc, #316]
	bl	sub_080150e4
	ldr	r0, [pc, #316]
	bl	sub_08015068
	ldr	r3, [sp, #28]
	movs	r1, #0
	ldr	r5, [sp, #32]
	ldr	r0, [sp, #88]
	mov	fp, r1
	movs	r2, #160
	str	r1, [sp, #36]
	str	r3, [sp, #48]
	movs	r1, #224
	add	r7, sp, #148
	add	r2, sp
	lsls	r1, r1, #2
	str	r5, [sp, #44]
	mov	r8, r2
	mov	sl, r7
	adds	r5, r0, r1
.L_081478fc:
	mov	r2, r8
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r0, [sp, #36]
	bl	sub_08002090
	ldr	r3, [r5, #24]
	muls	r3, r0
	mov	r0, r8
	str	r3, [r0, #4]
	ldr	r0, [sp, #36]
	bl	sub_08002096
	ldr	r3, [r5, #24]
	mov	r1, r8
	muls	r3, r0
	str	r3, [r1, #8]
	mov	r0, r8
	ldr	r3, [r5, #24]
	mov	r1, sl
	adds	r3, #2
	str	r3, [r5, #24]
	bl	sub_0815e1ec
	movs	r1, #192
	ldr	r0, [sp, #48]
	lsls	r1, r1, #3
	mov	r3, sl
	adds	r1, #156
	ldr	r2, [r3, #0]
	adds	r3, r0, r1
	ldr	r0, [sp, #88]
	asrs	r2, r2, #17
	ldr	r3, [r0, r3]
	mov	r1, sl
	adds	r2, r2, r3
	str	r2, [r1, #0]
	movs	r3, #6
	ldrsh	r2, [r1, r3]
	ldr	r1, [sp, #44]
	ldr	r3, [r0, r1]
	mov	r1, sl
	adds	r2, r2, r3
	mov	r3, sl
	str	r2, [r3, #4]
	movs	r0, #10
	ldrsh	r3, [r3, r0]
	str	r3, [r1, #8]
	cmp	r3, #169
	bgt.n	.L_08147964
	movs	r3, #170
	str	r3, [r7, #8]
.L_08147964:
	ldr	r0, [r7, #8]
	movs	r3, #175
	lsls	r3, r3, #1
	cmp	r0, r3
	ble.n	.L_08147972
	str	r3, [r7, #8]
	adds	r0, r3, #0
.L_08147972:
	ldr	r1, [pc, #128]
	subs	r0, #170
	bl	sub_08002074
	movs	r3, #3
	subs	r4, r3, r0
	ldr	r2, [pc, #100]
	lsls	r0, r4, #1
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #80]
	lsrs	r3, r4, #31
	adds	r1, r2, r1
	ldr	r2, [r7, #0]
	adds	r3, r4, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	str	r4, [sp, #0]
	subs	r3, r3, r4
	str	r0, [sp, #4]
	ldr	r4, [sp, #112]
	ldr	r0, [sp, #96]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2090
	ldr	r3, [sp, #36]
	lsls	r0, r0, #4
	movs	r1, #1
	adds	r0, #36
	add	fp, r1
	adds	r3, r3, r0
	mov	r2, fp
	str	r3, [sp, #36]
	adds	r5, #28
	cmp	r2, #28
	bne.n	.L_081478fc
.L_081479bc:
	ldr	r3, [sp, #32]
	ldr	r5, [sp, #28]
	ldr	r7, [sp, #24]
	ldr	r0, [sp, #20]
	movs	r1, #1
	add	r9, r1
	adds	r3, #28
	adds	r5, #28
	subs	r7, #12
	adds	r0, #12
	mov	r2, r9
	str	r3, [sp, #32]
	str	r5, [sp, #28]
	str	r7, [sp, #24]
	str	r0, [sp, #20]
	adds	r6, #28
	cmp	r2, #4
	beq.n	.L_081479e2
	b.n	.L_08147826
.L_081479e2:
	b.n	.L_08147a18
	.4byte 0x08197410
	.4byte 0x08197950
	.4byte 0x02010000
	.4byte 0x03000354
	.4byte 0x02d82d83
	.4byte 0xfffffeb6
	.4byte 0xfffe0000
	.4byte 0x08197962
	.4byte 0x0819795e
	.4byte 0x08197958
	.4byte 0x0819795b
	.4byte 0xfffff800
	.2byte 0xf000
	.2byte 0xffff
.L_08147a18:
	bl	sub_081434f8
	movs	r5, #240
	ldr	r3, [sp, #88]
	lsls	r5, r5, #7
	adds	r5, #232
	adds	r2, r3, r5
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r6, [sp, #84]
	movs	r7, #200
	adds	r6, #1
	lsls	r7, r7, #1
	str	r6, [sp, #84]
	cmp	r6, r7
	beq.n	.L_08147a42
	bl	.L_08146f52
.L_08147a42:
	movs	r0, #134
	bl	sub_081180e8
	ldr	r1, [sp, #100]
	movs	r0, #0
	ldr	r3, [r1, #20]
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_08147a80
	add	r6, sp, #172
	add	r5, sp, #204
	movs	r7, #36
.L_08147a5a:
	ldr	r2, [sp, #100]
	ldrsh	r0, [r7, r2]
	bl	sub_08118098
	ldr	r3, [r5, #0]
	ldr	r2, [r0, #0]
	movs	r0, #1
	str	r3, [r2, #8]
	add	r9, r0
	ldr	r3, [r5, #4]
	adds	r7, #2
	str	r3, [r2, #16]
	adds	r5, #8
	ldmia	r6!, {r3}
	strh	r3, [r2, #6]
	ldr	r1, [sp, #100]
	ldr	r3, [r1, #20]
	cmp	r9, r3
	bne.n	.L_08147a5a
.L_08147a80:
	ldr	r2, [pc, #72]
	movs	r3, #120
	str	r3, [r2, #12]
	bl	sub_0814cca8
	movs	r6, #238
	ldr	r3, [sp, #88]
	lsls	r6, r6, #7
	movs	r2, #0
	adds	r6, #220
	mov	r9, r2
	adds	r5, r3, r6
.L_08147a98:
	movs	r7, #1
	ldmia	r5!, {r0}
	add	r9, r7
	bl	sub_08020048
	mov	r0, r9
	cmp	r0, #9
	bne.n	.L_08147a98
	ldr	r0, [pc, #36]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #364
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x030011e0
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #1
	bl	sub_08147aec
	pop	{pc}
