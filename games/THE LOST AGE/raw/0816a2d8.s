.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08020090, 0x08020090
	.set sub_08020098, 0x08020098
	.set sub_08118078, 0x08118078
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138086, 0x08138086
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_0814cd48, 0x0814cd48
	.set sub_081504cc, 0x081504cc
	.set sub_08157530, 0x08157530
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e21c, 0x0815e21c
	.set sub_0816a620, 0x0816a620
	.set sub_0816a636, 0x0816a636
	.set sub_0816ac62, 0x0816ac62
	.set sub_0816acfc, 0x0816acfc
	.set sub_0816ad54, 0x0816ad54
	.set sub_0816adbc, 0x0816adbc
	.set sub_081963ec, 0x081963ec
	.set sub_08196404, 0x08196404
	.set sub_081c0010, 0x081c0010
	.global Overlay_0816a2d8
Overlay_0816a2d8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	str	r0, [sp, #16]
	str	r1, [sp, #12]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #92]
	movs	r0, #0
	str	r1, [sp, #8]
	ldr	r3, [r3, #96]
	mov	r9, r3
	bl	sub_081435e0
	ldr	r3, [sp, #12]
	subs	r3, #3
	cmp	r3, #2
	bls.n	.L_0816a310
	movs	r2, #128
	ldr	r3, [pc, #44]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
.L_0816a310:
	movs	r2, #128
	ldr	r3, [pc, #40]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r2, [sp, #12]
	cmp	r2, #0
	bne.n	.L_0816a348
	ldr	r3, [sp, #8]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r3, r2
	ldr	r0, [pc, #20]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #16]
	b.n	.L_0816a3f8
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00001010
	.4byte 0x00000111
	.2byte 0x0112
	.2byte 0x0000
.L_0816a348:
	ldr	r3, [sp, #12]
	cmp	r3, #1
	bne.n	.L_0816a364
	ldr	r2, [sp, #8]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #104]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #96]
	b.n	.L_0816a3f8
.L_0816a364:
	ldr	r1, [sp, #12]
	cmp	r1, #4
	bne.n	.L_0816a3d4
	ldr	r2, [sp, #8]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #84]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r1, [pc, #76]
	movs	r2, #1
	ldr	r0, [pc, #76]
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r1, [pc, #48]
	movs	r4, #160
	movs	r2, #31
	movs	r5, #0
	mov	ip, r1
	lsls	r4, r4, #19
	mov	lr, r2
.L_0816a396:
	ldrh	r0, [r4, #0]
	mov	r3, ip
	lsls	r1, r0, #16
	lsrs	r2, r1, #26
	lsrs	r1, r1, #21
	ands	r2, r3
	ands	r1, r3
	mov	r3, lr
	ands	r3, r0
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r1
	adds	r5, #1
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r5, #64
	bne.n	.L_0816a396
	b.n	.L_0816a422
	.4byte 0x0000001f
	.4byte 0x0000010f
	.4byte 0x00000110
	.4byte 0x0000010d
	.4byte 0x02010000
	.2byte 0x010e
	.2byte 0x0000
.L_0816a3d4:
	ldr	r1, [sp, #12]
	cmp	r1, #3
	bne.n	.L_0816a3f0
	ldr	r2, [sp, #8]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #784]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #776]
	b.n	.L_0816a3f8
.L_0816a3f0:
	ldr	r1, [sp, #12]
	cmp	r1, #5
	bne.n	.L_0816a404
	ldr	r0, [pc, #772]
.L_0816a3f8:
	ldr	r1, [pc, #772]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	b.n	.L_0816a422
.L_0816a404:
	ldr	r2, [sp, #8]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #756]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #752]
	ldr	r1, [pc, #740]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
.L_0816a422:
	ldr	r1, [sp, #8]
	movs	r3, #239
	lsls	r3, r3, #7
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r1, r3
	movs	r3, #0
	movs	r1, #200
	lsls	r1, r1, #4
	str	r3, [r2, #0]
	ldr	r0, [pc, #716]
	bl	sub_080145a8
	ldr	r1, [sp, #12]
	cmp	r1, #1
	beq.n	.L_0816a44e
	cmp	r1, #5
	bne.n	.L_0816a45c
.L_0816a44e:
	ldr	r2, [sp, #16]
	movs	r3, #36
	ldrsh	r1, [r2, r3]
	movs	r3, #128
	ldr	r0, [r2, #8]
	lsls	r3, r3, #12
	b.n	.L_0816a46c
.L_0816a45c:
	ldr	r1, [sp, #12]
	cmp	r1, #3
	bne.n	.L_0816a474
	ldr	r2, [sp, #16]
	movs	r3, #160
	ldr	r1, [r2, #8]
	lsls	r3, r3, #12
	adds	r0, r1, #0
.L_0816a46c:
	movs	r2, #16
	bl	sub_08118078
	b.n	.L_0816a4a6
.L_0816a474:
	ldr	r3, [sp, #12]
	cmp	r3, #4
	bne.n	.L_0816a496
	ldr	r1, [sp, #16]
	ldr	r0, [r1, #8]
	bl	sub_08118098
	ldr	r5, [r0, #0]
	movs	r1, #2
	adds	r0, r5, #0
	bl	sub_08020090
	adds	r0, r5, #0
	movs	r1, #48
	bl	sub_08020098
	b.n	.L_0816a4a6
.L_0816a496:
	ldr	r2, [sp, #16]
	movs	r3, #36
	ldrsh	r1, [r2, r3]
	ldr	r0, [r2, #8]
	movs	r3, #0
	movs	r2, #16
	bl	sub_08118078
.L_0816a4a6:
	ldr	r1, [sp, #12]
	cmp	r1, #4
	bne.n	.L_0816a4b4
	movs	r0, #8
	bl	sub_08013560
	b.n	.L_0816a4ba
.L_0816a4b4:
	movs	r0, #16
	bl	sub_08013560
.L_0816a4ba:
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L_0816a4cc
	movs	r0, #104
	movs	r1, #7
	bl	sub_081963ec
	b.n	.L_0816a4d4
.L_0816a4cc:
	movs	r0, #104
	movs	r1, #3
	bl	sub_081963ec
.L_0816a4d4:
	ldr	r1, [sp, #16]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #104]
	ldr	r3, [r1, #4]
	cmp	r3, #1
	bne.n	.L_0816a4ec
	movs	r0, #188
	movs	r1, #15
	bl	sub_081963ec
	b.n	.L_0816a4f4
.L_0816a4ec:
	movs	r0, #188
	movs	r1, #11
	bl	sub_081963ec
.L_0816a4f4:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	movs	r0, #212
	ldr	r7, [r3, #0]
	bl	sub_081c0010
	movs	r2, #0
	movs	r3, #120
	movs	r1, #60
	mov	sl, r2
	mov	r8, r3
	mov	fp, r1
.L_0816a50e:
	ldr	r2, [sp, #12]
	cmp	r2, #5
	bne.n	.L_0816a5be
	mov	r3, sl
	cmp	r3, #3
	bgt.n	.L_0816a53c
	mov	r1, r8
	mov	r2, fp
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	ldr	r1, [pc, #476]
	movs	r2, #0
	movs	r3, #0
	mov	r0, r9
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4643
	mov	r1, fp
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	mov	r0, r9
	ldr	r1, [pc, #452]
	b.n	.L_0816a5b4
.L_0816a53c:
	mov	r2, sl
	cmp	r2, #7
	bgt.n	.L_0816a564
	mov	r3, r8
	mov	r1, fp
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	movs	r2, #0
	ldr	r1, [pc, #448]
	movs	r3, #0
	mov	r0, r9
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4642
	mov	r3, fp
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	mov	r0, r9
	ldr	r1, [pc, #428]
	b.n	.L_0816a5b4
.L_0816a564:
	mov	r1, sl
	cmp	r1, #11
	bgt.n	.L_0816a58c
	mov	r2, r8
	mov	r3, fp
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	ldr	r1, [pc, #416]
	movs	r2, #0
	movs	r3, #0
	mov	r0, r9
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4641
	mov	r2, fp
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	mov	r0, r9
	ldr	r1, [pc, #392]
	b.n	.L_0816a5b4
.L_0816a58c:
	mov	r3, sl
	cmp	r3, #15
	bgt.n	sub_0816a620
	ldr	r5, [pc, #388]
	mov	r1, r8
	mov	r2, fp
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	mov	r0, r9
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4643
	mov	r1, fp
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	mov	r0, r9
	adds	r1, r5, #0
.L_0816a5b4:
	movs	r2, #0
	movs	r3, #60
	mov	lr, r7
	.2byte 0xf800
	.2byte 0xe030
.L_0816a5be:
	mov	r2, sl
	cmp	r2, #3
	bgt.n	.L_0816a5d6
	mov	r3, r8
	ldr	r2, [sp, #8]
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	mov	r0, r9
	b.n	.L_0816a5fe
.L_0816a5d6:
	mov	r1, sl
	cmp	r1, #7
	bgt.n	.L_0816a5ee
	mov	r2, r8
	ldr	r3, [sp, #8]
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	movs	r2, #253
	lsls	r2, r2, #6
	adds	r1, r3, r2
	mov	r0, r9
	b.n	.L_0816a5fe
.L_0816a5ee:
	mov	r3, sl
	cmp	r3, #11
	bgt.n	.L_0816a608
	mov	r1, r8
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	mov	r0, r9
	ldr	r1, [pc, #256]
.L_0816a5fe:
	movs	r2, #0
	movs	r3, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0xe00b
.L_0816a608:
	mov	r2, sl
	cmp	r2, #15
	bgt.n	sub_0816a620
	mov	r3, r8
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	mov	r0, r9
	ldr	r1, [pc, #252]
	movs	r2, #0
	movs	r3, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4653
	subs	r3, #16
	cmp	r3, #3
	bhi.n	sub_0816a636
	movs	r1, #128
	ldr	r3, [pc, #240]
	mov	r0, r9
	lsls	r1, r1, #7
	ldr	r2, [pc, #236]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4651
	cmp	r1, #18
	bne.n	.L_0816a642
	movs	r0, #134
	bl	sub_081180e8
.L_0816a642:
	mov	r2, sl
	cmp	r2, #20
	bne.n	.L_0816a662
	ldr	r1, [sp, #8]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	adds	r3, r1, r2
	movs	r2, #8
	str	r2, [r3, #0]
	ldr	r1, [sp, #16]
	movs	r3, #36
	ldrsh	r0, [r1, r3]
	movs	r1, #4
	bl	sub_08118088
.L_0816a662:
	movs	r0, #16
	movs	r1, #16
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r1, #240
	ldr	r3, [sp, #8]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r3, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #21
	beq.n	.L_0816a68e
	b.n	.L_0816a50e
.L_0816a68e:
	movs	r0, #104
	bl	sub_0801314c
	movs	r0, #188
	bl	sub_0801314c
	ldr	r0, [pc, #112]
	bl	sub_08014644
	ldr	r1, [sp, #12]
	cmp	r1, #3
	bne.n	.L_0816a6ca
	movs	r1, #240
	ldr	r5, [pc, #120]
	lsls	r1, r1, #6
	ldr	r0, [pc, #120]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x21f0
	lsls	r1, r1, #6
	mov	r0, r9
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x481b
	bl	sub_08014644
	ldr	r0, [sp, #16]
	bl	sub_081504cc
	b.n	.L_0816a6e4
.L_0816a6ca:
	ldr	r2, [sp, #12]
	cmp	r2, #4
	bne.n	.L_0816a6e0
	ldr	r3, [sp, #16]
	ldr	r0, [r3, #8]
	bl	sub_08118098
	movs	r1, #16
	ldr	r0, [r0, #0]
	bl	sub_08020098
.L_0816a6e0:
	bl	sub_08143bb8
.L_0816a6e4:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0000010f
	.4byte 0x00000110
	.4byte 0x000000fe
	.4byte 0x02010000
	.4byte 0x0000010d
	.4byte 0x0000010e
	.4byte 0x08143001
	.4byte 0x02011c20
	.4byte 0x02013840
	.4byte 0x02015460
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x03000258
	.4byte 0x06004000
	.2byte 0x3489
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #44
	str	r0, [sp, #20]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #96]
	ldr	r2, [r3, #92]
	str	r4, [sp, #16]
	movs	r0, #0
	mov	sl, r2
	ldr	r5, [r3, #100]
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	adds	r1, r5, #0
	ldr	r0, [pc, #52]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	add	r1, sl
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r1, #182
	lsls	r1, r1, #4
	ldr	r0, [pc, #28]
	add	r1, sl
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	b.n	.L_0816a7a4
	.4byte 0x00000100
	.4byte 0x00000134
	.4byte 0x00000113
	.2byte 0x0184
	.2byte 0x0000
.L_0816a7a4:
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, sl
	movs	r3, #75
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #572]
	bl	sub_080145a8
	ldr	r3, [sp, #20]
	add	r5, sp, #32
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	adds	r1, r5, #0
	bl	sub_0815e21c
	ldr	r2, [r5, #0]
	movs	r1, #128
	movs	r3, #64
	lsls	r1, r1, #19
	subs	r3, r3, r2
	adds	r1, #40
	lsls	r3, r3, #8
	str	r3, [r1, #0]
	movs	r4, #0
	mov	r8, r4
	mov	r7, sl
.L_0816a7e2:
	bl	sub_08014878
	movs	r6, #128
	lsls	r6, r6, #1
	adds	r6, #255
	ands	r6, r0
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r5, r0, #0
	adds	r3, #255
	ands	r5, r3
	movs	r3, #128
	lsls	r3, r3, #15
	str	r3, [r7, #0]
	movs	r3, #224
	lsls	r3, r3, #15
	str	r3, [r7, #4]
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r6, #128
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #8
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #9
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #7
	movs	r2, #1
	ands	r3, r0
	add	r8, r2
	str	r3, [r7, #24]
	mov	r3, r8
	adds	r7, #28
	cmp	r3, #16
	bne.n	.L_0816a7e2
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, sl
	movs	r3, #8
	str	r3, [r2, #0]
	mov	r2, sp
	movs	r4, #0
	adds	r2, #24
	str	r2, [sp, #12]
	str	r4, [sp, #8]
	mov	r9, r4
.L_0816a854:
	mov	r3, r9
	cmp	r3, #10
	bne.n	.L_0816a88c
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	movs	r4, #8
	add	r3, sl
	str	r4, [r3, #0]
	movs	r0, #212
	bl	sub_081180e8
	ldr	r3, [sp, #20]
	movs	r1, #0
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_08118088
	ldr	r2, [sp, #20]
	movs	r3, #8
	movs	r4, #36
	ldrsh	r0, [r2, r4]
	movs	r1, #7
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
.L_0816a88c:
	mov	r4, r9
	cmp	r4, #7
	ble.n	.L_0816a91c
	cmp	r4, #31
	bgt.n	.L_0816a8a2
	lsls	r3, r4, #1
	add	r3, r9
	lsls	r3, r3, #2
	adds	r5, r3, #0
	subs	r5, #96
	b.n	.L_0816a8aa
.L_0816a8a2:
	ldr	r2, [sp, #8]
	movs	r3, #136
	lsls	r3, r3, #1
	subs	r5, r3, r2
.L_0816a8aa:
	cmp	r5, #0
	ble.n	.L_0816a91c
	cmp	r5, #80
	ble.n	.L_0816a8b8
	movs	r5, #80
	movs	r7, #1
	b.n	.L_0816a8ba
.L_0816a8b8:
	movs	r7, #0
.L_0816a8ba:
	movs	r3, #0
	movs	r4, #112
	mov	r8, r3
	movs	r6, #50
	mov	fp, r4
.L_0816a8c4:
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_0816a8da
	movs	r0, #104
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	str	r7, [sp, #0]
	bl	sub_08196404
	b.n	.L_0816a8e8
.L_0816a8da:
	movs	r0, #104
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	str	r7, [sp, #0]
	bl	sub_08196404
.L_0816a8e8:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r1, #224
	mov	ip, r3
	str	r3, [sp, #24]
	mov	r4, fp
	movs	r3, #14
	lsls	r1, r1, #3
	adds	r2, r6, #0
	str	r3, [sp, #0]
	str	r5, [sp, #4]
	subs	r3, r4, r5
	ldr	r0, [sp, #16]
	add	r1, sl
	mov	lr, ip
	.2byte 0xf800
	.2byte 0x2068
	bl	sub_0801314c
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r6, #14
	cmp	r3, #2
	bne.n	.L_0816a8c4
.L_0816a91c:
	ldr	r4, [sp, #20]
	ldr	r1, [sp, #12]
	ldr	r0, [r4, #4]
	bl	sub_08144aac
	movs	r2, #0
	mov	r8, r2
	mov	r5, sl
.L_0816a92c:
	mov	r4, r8
	lsrs	r3, r4, #31
	add	r3, r8
	asrs	r3, r3, #1
	adds	r3, #8
	cmp	r9, r3
	blt.n	.L_0816a988
	ldr	r0, [r5, #24]
	cmp	r0, #28
	bgt.n	.L_0816a988
	movs	r1, #3
	bl	sub_08002054
	movs	r2, #2
	ldrsh	r4, [r5, r2]
	movs	r3, #6
	ldrsh	r6, [r5, r3]
	cmp	r0, #6
	ble.n	.L_0816a954
	movs	r0, #6
.L_0816a954:
	ldr	r3, [pc, #164]
	lsls	r2, r0, #1
	ldrh	r1, [r3, r2]
	movs	r3, #182
	lsls	r3, r3, #4
	add	r1, sl
	adds	r1, r1, r3
	ldr	r3, [pc, #156]
	ldrh	r0, [r3, r2]
	lsrs	r3, r0, #1
	subs	r2, r4, r3
	str	r0, [sp, #0]
	subs	r3, r6, r3
	str	r0, [sp, #4]
	ldr	r4, [sp, #24]
	ldr	r0, [sp, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69ab
	adds	r0, r5, #0
	adds	r3, #1
	str	r3, [r5, #24]
	movs	r1, #62
	ldr	r2, [pc, #128]
	bl	sub_08138086
.L_0816a988:
	movs	r4, #1
	add	r8, r4
	mov	r2, r8
	adds	r5, #28
	cmp	r2, #16
	bne.n	.L_0816a92c
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	mov	r3, r9
	cmp	r3, #7
	bgt.n	.L_0816a9b0
	movs	r0, #4
	movs	r1, #4
	bl	sub_08158ce0
	b.n	.L_0816a9b8
.L_0816a9b0:
	movs	r0, #16
	movs	r1, #16
	bl	sub_08158ce0
.L_0816a9b8:
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, sl
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r4, [sp, #8]
	movs	r2, #1
	add	r9, r2
	adds	r4, #6
	mov	r3, r9
	str	r4, [sp, #8]
	cmp	r3, #54
	beq.n	.L_0816a9e0
	b.n	.L_0816a854
.L_0816a9e0:
	ldr	r0, [pc, #20]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x08143001
	.4byte 0x08198afc
	.4byte 0x08198b0a
	.2byte 0xe000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #96]
	sub	sp, #56
	mov	fp, r0
	ldr	r0, [r3, #92]
	str	r1, [sp, #32]
	mov	r9, r0
	ldr	r3, [r3, #100]
	movs	r0, #0
	str	r3, [sp, #24]
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	add	r1, sp, #36
	strh	r3, [r2, #0]
	movs	r0, #0
	bl	sub_08144aac
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, r9
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #20]
	bl	sub_080145a8
	mov	r3, fp
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_08118098
	b.n	.L_0816aa78
	movs	r0, r0
	.4byte 0x00001010
	.2byte 0x3001
	.2byte 0x0814
.L_0816aa78:
	movs	r1, #156
	ldr	r0, [r0, #0]
	lsls	r1, r1, #7
	adds	r1, #16
	str	r0, [sp, #20]
	movs	r4, #0
	add	r1, r9
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #888]
	str	r4, [sp, #16]
	bl	sub_08157cf4
	ldr	r0, [pc, #884]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #876]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x48da
	ldr	r1, [sp, #24]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #860]
	add	r1, r9
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #156
	lsls	r1, r1, #6
	ldr	r0, [pc, #848]
	add	r1, r9
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r3, [pc, #840]
	movs	r0, #0
	movs	r1, #1
	movs	r2, #128
	mov	r8, r0
	negs	r1, r1
	lsls	r2, r2, #3
.L_0816aae0:
	movs	r4, #1
	add	r8, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_0816aae0
	movs	r2, #224
	mov	r1, sp
	lsls	r2, r2, #3
	movs	r0, #0
	adds	r1, #44
	add	r2, r9
	str	r0, [sp, #28]
	str	r1, [sp, #8]
	str	r2, [sp, #12]
.L_0816aafe:
	mov	r4, fp
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	ldr	r1, [sp, #8]
	bl	sub_0815e21c
	ldr	r0, [sp, #28]
	cmp	r0, #0
	bne.n	.L_0816abb0
	ldr	r2, [sp, #8]
	movs	r1, #0
	mov	r8, r1
	mov	sl, r2
	mov	r7, r9
.L_0816ab1a:
	bl	sub_08014878
	movs	r6, #192
	lsls	r6, r6, #2
	adds	r6, #255
	ands	r6, r0
	bl	sub_08014878
	movs	r5, #254
	ldr	r3, [pc, #752]
	lsls	r5, r5, #7
	adds	r5, #255
	mov	r4, sl
	ands	r5, r0
	adds	r5, r5, r3
	ldr	r3, [r4, #0]
	adds	r0, r5, #0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	adds	r6, #32
	ldr	r3, [r4, #4]
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #8
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	movs	r1, #1
	asrs	r3, r3, #8
	mov	r0, r8
	add	r8, r1
	str	r3, [r7, #16]
	mov	r2, r8
	negs	r3, r0
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r2, #64
	bne.n	.L_0816ab1a
	ldr	r4, [sp, #20]
	movs	r3, #0
	str	r3, [r4, #72]
	str	r3, [r4, #12]
	ldr	r0, [sp, #8]
	mov	r2, fp
	ldr	r0, [r0, #4]
	str	r0, [sp, #16]
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r1, #1
	negs	r1, r1
	movs	r2, #5
	str	r1, [sp, #0]
	bl	sub_0814cd48
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, r9
	movs	r3, #4
	str	r3, [r2, #0]
	movs	r0, #144
	bl	sub_081c0010
.L_0816abb0:
	ldr	r3, [sp, #28]
	cmp	r3, #32
	bne.n	sub_0816ac62
	ldr	r0, [sp, #8]
	ldr	r7, [pc, #616]
	movs	r4, #0
	mov	r8, r4
	mov	sl, r0
.L_0816abc0:
	bl	sub_08014878
	movs	r5, #192
	lsls	r5, r5, #2
	adds	r5, #255
	ands	r5, r0
	bl	sub_08014878
	mov	r1, sl
	ldr	r3, [r1, #0]
	adds	r6, r0, #0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	adds	r5, #32
	ldr	r3, [r1, #4]
	subs	r3, #16
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #7
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #48
	str	r3, [r7, #24]
	movs	r2, #1
	movs	r3, #128
	add	r8, r2
	lsls	r3, r3, #1
	adds	r7, #28
	cmp	r8, r3
	bne.n	.L_0816abc0
	mov	r1, fp
	movs	r4, #36
	ldrsh	r0, [r1, r4]
	movs	r1, #4
	bl	sub_08118088
	mov	r3, fp
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, r9
	movs	r3, #8
	str	r3, [r2, #0]
	movs	r0, #145
	bl	sub_081180e8
	movs	r1, #128
	ldr	r3, [pc, #464]
	ldr	r0, [sp, #32]
	lsls	r1, r1, #7
	ldr	r2, [pc, #460]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c07
	cmp	r4, #23
	bhi.n	.L_0816ac74
	ldr	r0, [sp, #20]
	movs	r1, #128
	ldr	r3, [r0, #12]
	lsls	r1, r1, #10
	adds	r3, r3, r1
	str	r3, [r0, #12]
.L_0816ac74:
	ldr	r2, [sp, #28]
	cmp	r2, #18
	bne.n	.L_0816ac8a
	mov	r3, fp
	ldr	r0, [r3, #8]
	movs	r4, #36
	ldrsh	r1, [r3, r4]
	movs	r2, #8
	movs	r3, #80
	bl	sub_08157530
.L_0816ac8a:
	ldr	r0, [sp, #28]
	cmp	r0, #21
	bne.n	.L_0816aca2
	mov	r1, fp
	movs	r3, #128
	ldr	r0, [r1, #8]
	lsls	r3, r3, #12
	movs	r2, #36
	ldrsh	r1, [r1, r2]
	movs	r2, #12
	bl	sub_08118078
.L_0816aca2:
	ldr	r4, [pc, #396]
	ldr	r7, [pc, #396]
	movs	r3, #0
	mov	r8, r3
	mov	sl, r4
.L_0816acac:
	ldr	r6, [r7, #24]
	cmp	r6, #0
	blt.n	sub_0816acfc
	adds	r0, r6, #0
	movs	r1, #6
	bl	sub_08002054
	adds	r5, r0, #0
	cmp	r5, #1
	bgt.n	.L_0816acc2
	movs	r5, #2
.L_0816acc2:
	subs	r3, r6, #1
	movs	r2, #128
	str	r3, [r7, #24]
	adds	r0, r7, #0
	movs	r1, #60
	lsls	r2, r2, #6
	bl	sub_08138086
	lsls	r0, r5, #1
	subs	r3, r0, #2
	mov	r2, sl
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #24]
	movs	r4, #2
	ldrsh	r2, [r7, r4]
	adds	r1, r3, r1
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r4, #6
	ldrsh	r3, [r7, r4]
	str	r0, [sp, #4]
	subs	r3, r3, r5
	str	r5, [sp, #0]
	ldr	r4, [sp, #36]
	ldr	r0, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	movs	r1, #128
	add	r8, r0
	lsls	r1, r1, #2
	adds	r7, #28
	cmp	r8, r1
	bne.n	.L_0816acac
	movs	r2, #0
	mov	r8, r2
	mov	r6, r9
.L_0816ad10:
	ldr	r3, [r6, #24]
	adds	r0, r3, #1
	str	r0, [r6, #24]
	cmp	r0, #17
	bhi.n	sub_0816ad54
	movs	r1, #3
	bl	sub_08002054
	movs	r1, #60
	adds	r5, r0, #0
	ldr	r2, [pc, #272]
	adds	r0, r6, #0
	bl	sub_08138086
	lsls	r5, r5, #11
	movs	r3, #156
	lsls	r3, r3, #6
	add	r5, r9
	movs	r4, #2
	ldrsh	r2, [r6, r4]
	adds	r5, r5, r3
	movs	r0, #6
	ldrsh	r3, [r6, r0]
	movs	r1, #32
	str	r1, [sp, #0]
	movs	r1, #64
	str	r1, [sp, #4]
	subs	r2, #16
	subs	r3, #32
	ldr	r4, [sp, #36]
	ldr	r0, [sp, #32]
	adds	r1, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2101
	add	r8, r1
	mov	r2, r8
	adds	r6, #28
	cmp	r2, #32
	bne.n	.L_0816ad10
	ldr	r3, [sp, #28]
	cmp	r3, #31
	bhi.n	sub_0816adbc
	movs	r4, #104
	lsls	r0, r3, #3
	movs	r1, #104
	mov	r8, r4
	bl	sub_08002064
	mov	sl, r0
	ldr	r0, [sp, #8]
	ldr	r1, [sp, #16]
	ldr	r2, [r0, #0]
	movs	r6, #34
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	mov	r3, sl
	subs	r5, r1, r3
	adds	r3, r5, #0
	mov	r4, r8
	asrs	r2, r2, #1
	subs	r2, #17
	subs	r3, #96
	str	r4, [sp, #4]
	ldr	r1, [sp, #12]
	ldr	r4, [sp, #36]
	str	r6, [sp, #0]
	ldr	r0, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9802
	mov	r1, sl
	ldr	r2, [r0, #0]
	adds	r5, #8
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	str	r1, [sp, #4]
	subs	r2, #17
	str	r6, [sp, #0]
	ldr	r4, [sp, #36]
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #12]
	adds	r3, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xf7d8
	.2byte 0xfb9c
	.2byte 0x22f0
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, r9
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #28]
	adds	r2, #1
	str	r2, [sp, #28]
	cmp	r2, #80
	beq.n	.L_0816adde
	b.n	.L_0816aafe
.L_0816adde:
	movs	r0, #104
	bl	sub_0801314c
	movs	r0, #188
	bl	sub_0801314c
	ldr	r0, [pc, #80]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x000000da
	.4byte 0x00000155
	.4byte 0x03000730
	.4byte 0x00000134
	.4byte 0x00000146
	.4byte 0x0000013e
	.4byte 0x02010018
	.4byte 0xffffc000
	.4byte 0x02011c00
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x08197410
	.4byte 0x02010000
	.4byte 0xffffe000
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	adds	r7, r1, #0
	movs	r1, #0
	str	r0, [sp, #4]
	adds	r6, r2, #0
	mov	sl, r3
	mov	fp, r1
	cmp	r3, #0
	beq.n	.L_0816aec4
	lsls	r2, r6, #1
	lsls	r3, r3, #2
	str	r2, [sp, #0]
	subs	r3, #2
	mov	r9, r1
	mov	r8, r1
	mov	lr, r3
.L_0816ae6e:
	movs	r3, #0
	mov	ip, r3
	cmp	r6, #0
	beq.n	.L_0816aeb0
	mov	r3, lr
	muls	r3, r6
	mov	r1, r8
	lsls	r2, r1, #1
	adds	r0, r2, r7
	ldr	r1, [sp, #4]
	lsls	r2, r6, #1
	adds	r5, r3, r7
	adds	r3, r2, r3
	add	r2, r9
	adds	r3, r3, r7
	adds	r2, r2, r7
	add	r1, r8
	subs	r4, r3, #1
	subs	r2, #1
.L_0816ae94:
	ldrb	r3, [r1, #0]
	adds	r1, #1
	strb	r3, [r0, #0]
	strb	r3, [r2, #0]
	strb	r3, [r5, #0]
	strb	r3, [r4, #0]
	movs	r3, #1
	add	ip, r3
	adds	r0, #1
	subs	r2, #1
	adds	r5, #1
	subs	r4, #1
	cmp	ip, r6
	bne.n	.L_0816ae94
.L_0816aeb0:
	ldr	r1, [sp, #0]
	movs	r2, #2
	movs	r3, #1
	negs	r2, r2
	add	fp, r3
	add	r9, r1
	add	r8, r6
	add	lr, r2
	cmp	fp, sl
	bne.n	.L_0816ae6e
.L_0816aec4:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
