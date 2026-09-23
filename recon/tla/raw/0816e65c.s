.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0800206c, 0x0800206c
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014dac, 0x08014dac
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_0801521c, 0x0801521c
	.set sub_080156e8, 0x080156e8
	.set sub_08020098, 0x08020098
	.set sub_08118078, 0x08118078
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138058, 0x08138058
	.set sub_08138086, 0x08138086
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_0814cd48, 0x0814cd48
	.set sub_081504b4, 0x081504b4
	.set sub_08157530, 0x08157530
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815b434, 0x0815b434
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e21c, 0x0815e21c
	.set sub_0816fca8, 0x0816fca8
	.set sub_0818caa8, 0x0818caa8
	.set sub_081963ec, 0x081963ec
	.set sub_08196404, 0x08196404
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Func_0816e65c
	.thumb_func
Func_0816e65c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #100
	str	r0, [sp, #52]
	str	r1, [sp, #48]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r1, [r5, #92]
	ldr	r2, [r5, #96]
	movs	r0, #1
	str	r2, [sp, #44]
	mov	fp, r1
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r1, #35
	movs	r0, #104
	bl	sub_081963ec
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	ldr	r5, [r5, #104]
	adds	r2, #132
	add	r2, fp
	movs	r3, #0
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #16]
	str	r5, [sp, #36]
	bl	sub_080145a8
	ldr	r3, [sp, #52]
	ldr	r0, [r3, #8]
	b.n	.L_0816e6c8
	movs	r0, r0
	.4byte 0x00000410
	.2byte 0x3001
	.2byte 0x0814
.L_0816e6c8:
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r1, [sp, #48]
	str	r0, [sp, #28]
	cmp	r1, #0
	bne.n	.L_0816e73e
	movs	r1, #224
	movs	r2, #80
	lsls	r1, r1, #3
	movs	r5, #128
	str	r2, [sp, #24]
	ldr	r0, [pc, #840]
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	lsls	r5, r5, #5
	bl	sub_08157cf4
	movs	r4, #1
	mov	ip, r5
.L_0816e6f2:
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #118
	lsls	r2, r4, #1
	adds	r2, r2, r3
	movs	r3, #4
	subs	r3, r3, r4
	movs	r1, #240
	lsls	r1, r1, #6
	lsls	r3, r3, #12
	adds	r3, r3, r1
	strh	r3, [r2, #0]
	movs	r2, #0
	mov	r8, r2
	movs	r2, #224
	adds	r0, r4, #0
	lsls	r2, r2, #3
	adds	r1, r5, #0
	adds	r0, #59
	add	r2, fp
.L_0816e71a:
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_0816e724
	strb	r3, [r2, r1]
	b.n	.L_0816e726
.L_0816e724:
	strb	r0, [r2, r1]
.L_0816e726:
	movs	r3, #1
	add	r8, r3
	adds	r2, #1
	cmp	r8, ip
	bne.n	.L_0816e71a
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r4, #1
	adds	r5, r5, r1
	cmp	r4, #5
	bne.n	.L_0816e6f2
	b.n	.L_0816e752
.L_0816e73e:
	movs	r1, #224
	lsls	r1, r1, #3
	movs	r2, #1
	ldr	r0, [pc, #744]
	add	r1, fp
	movs	r3, #1
	bl	sub_08157cf4
	movs	r2, #64
	str	r2, [sp, #24]
.L_0816e752:
	movs	r0, #212
	bl	sub_081c0010
	ldr	r1, [sp, #24]
	movs	r3, #0
	str	r3, [sp, #32]
	cmp	r1, #0
	bne.n	.L_0816e764
	b.n	.L_0816ec16
.L_0816e764:
	mov	r2, sp
	mov	r3, sp
	mov	r1, sp
	adds	r2, #76
	adds	r3, #64
	adds	r1, #56
	str	r2, [sp, #8]
	str	r3, [sp, #12]
	str	r1, [sp, #16]
.L_0816e776:
	ldr	r2, [sp, #32]
	cmp	r2, #0
	bne.n	.L_0816e7ee
	ldr	r1, [sp, #52]
	add	r5, sp, #88
	movs	r3, #36
	ldrsh	r0, [r1, r3]
	ldr	r7, [pc, #684]
	adds	r1, r5, #0
	bl	sub_0815e21c
	movs	r2, #0
	mov	r8, r2
	mov	sl, r5
.L_0816e792:
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08014878
	mov	r1, sl
	ldr	r3, [r1, #0]
	movs	r5, #255
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	ands	r5, r0
	ldr	r3, [r1, #4]
	adds	r0, r6, #0
	subs	r3, #16
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	bl	sub_08002096
	adds	r5, #128
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08014878
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
	bne.n	.L_0816e792
.L_0816e7ee:
	ldr	r1, [sp, #48]
	cmp	r1, #0
	bne.n	.L_0816e80e
	ldr	r2, [sp, #32]
	cmp	r2, #8
	bne.n	.L_0816e82e
	ldr	r3, [sp, #52]
	movs	r1, #1
	ldr	r0, [r3, #8]
	negs	r1, r1
	movs	r2, #2
	movs	r3, #0
	str	r1, [sp, #0]
	bl	sub_0814cd48
	b.n	.L_0816e82e
.L_0816e80e:
	ldr	r1, [sp, #32]
	cmp	r1, #0
	bne.n	.L_0816e82e
	ldr	r2, [sp, #52]
	movs	r1, #1
	ldr	r0, [r2, #8]
	negs	r1, r1
	movs	r2, #2
	movs	r3, #0
	str	r1, [sp, #0]
	bl	sub_0814cd48
	movs	r1, #32
	ldr	r0, [sp, #28]
	bl	sub_08020098
.L_0816e82e:
	movs	r0, #32
	bl	sub_08014dac
	str	r0, [sp, #20]
	movs	r0, #1
	bl	sub_081969f8
	ldr	r3, [sp, #52]
	ldr	r1, [sp, #8]
	mov	sl, r0
	ldr	r0, [r3, #8]
	bl	sub_0815e21c
	ldr	r2, [sp, #52]
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	ldr	r1, [sp, #12]
	bl	sub_0815e21c
	mov	r1, sl
	movs	r3, #9
	str	r3, [r1, #0]
	ldr	r2, [sp, #16]
	str	r2, [r1, #16]
	ldr	r3, [sp, #20]
	movs	r2, #0
	str	r3, [r1, #12]
	str	r2, [r1, #20]
	ldr	r3, [sp, #48]
	cmp	r3, #0
	beq.n	.L_0816e86e
	b.n	.L_0816e9c6
.L_0816e86e:
	ldr	r1, [sp, #16]
	movs	r3, #5
	strb	r3, [r1, #0]
	movs	r3, #7
	strb	r3, [r1, #1]
	ldr	r3, [pc, #444]
	mov	r2, sl
	str	r3, [r2, #8]
	ldr	r3, [sp, #32]
	cmp	r3, #59
	ble.n	.L_0816e88c
	ldr	r1, [sp, #32]
	movs	r3, #68
	subs	r6, r3, r1
	b.n	.L_0816e890
.L_0816e88c:
	ldr	r6, [sp, #32]
	adds	r6, #2
.L_0816e890:
	cmp	r6, #8
	ble.n	.L_0816e896
	movs	r6, #8
.L_0816e896:
	cmp	r6, #0
	bgt.n	.L_0816e89c
	b.n	.L_0816eace
.L_0816e89c:
	ldr	r2, [sp, #32]
	cmp	r2, #38
	ble.n	.L_0816e8bc
	ldr	r3, [pc, #408]
	lsls	r0, r2, #9
	adds	r0, r0, r3
	bl	sub_08002096
	cmp	r0, #0
	bge.n	.L_0816e8b2
	adds	r0, #15
.L_0816e8b2:
	movs	r3, #128
	asrs	r2, r0, #4
	lsls	r3, r3, #7
	subs	r0, r3, r2
	b.n	.L_0816e8dc
.L_0816e8bc:
	ldr	r1, [sp, #32]
	cmp	r1, #15
	ble.n	.L_0816e8da
	ldr	r2, [pc, #380]
	lsls	r0, r1, #9
	adds	r0, r0, r2
	bl	sub_08002090
	movs	r3, #128
	lsls	r3, r3, #9
	subs	r3, r3, r0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r0, r3, #1
	b.n	.L_0816e8dc
.L_0816e8da:
	movs	r0, #0
.L_0816e8dc:
	movs	r3, #4
	mov	r1, fp
	mov	r8, r3
	adds	r1, #108
.L_0816e8e4:
	ldr	r3, [r1, #0]
	movs	r2, #1
	negs	r2, r2
	add	r8, r2
	str	r3, [r1, #28]
	mov	r3, r8
	subs	r1, #28
	cmp	r3, #0
	bne.n	.L_0816e8e4
	mov	r1, fp
	str	r0, [r1, #24]
	ldr	r3, [sp, #8]
	movs	r2, #4
	mov	r7, fp
	mov	r8, r2
	mov	r9, r3
	adds	r7, #112
.L_0816e906:
	mov	r1, r8
	cmp	r1, #0
	beq.n	.L_0816e916
	ldr	r2, [sp, #32]
	mov	r3, r8
	adds	r3, #38
	cmp	r2, r3
	bgt.n	.L_0816e9b8
.L_0816e916:
	ldr	r2, [sp, #32]
	mov	r1, r8
	lsls	r3, r1, #1
	cmp	r2, r3
	blt.n	.L_0816e9b8
	ldr	r2, [sp, #16]
	lsls	r3, r1, #12
	movs	r1, #224
	lsls	r1, r1, #3
	add	r3, fp
	adds	r3, r3, r1
	str	r3, [r2, #4]
	bl	sub_08014de4
	mov	r3, r9
	ldr	r0, [r3, #0]
	mov	r2, r9
	lsrs	r3, r0, #31
	ldr	r1, [r2, #4]
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #64
	subs	r1, #88
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	movs	r5, #128
	bl	sub_08015160
	lsls	r5, r5, #8
	movs	r1, #128
	lsls	r1, r1, #9
	adds	r0, r5, #0
	adds	r2, r5, #0
	bl	sub_080151e4
	ldr	r1, [sp, #52]
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_0816e970
	ldr	r0, [r7, #24]
	subs	r0, r5, r0
	bl	sub_080150e4
	b.n	.L_0816e97c
.L_0816e970:
	ldr	r0, [r7, #24]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r0, r0, r2
	bl	sub_080150e4
.L_0816e97c:
	movs	r0, #0
	ldr	r1, [pc, #196]
	movs	r2, #0
	bl	sub_08015160
	movs	r3, #204
	lsls	r3, r3, #6
	adds	r3, #136
	adds	r1, r6, #0
	muls	r1, r3
	lsls	r0, r6, #15
	lsls	r2, r6, #13
	bl	sub_080151e4
	ldr	r1, [sp, #52]
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_0816e9a8
	movs	r0, #128
	lsls	r0, r0, #8
	bl	sub_08015068
.L_0816e9a8:
	ldr	r0, [pc, #156]
	ldr	r1, [sp, #20]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_0816e9b8:
	movs	r2, #1
	negs	r2, r2
	add	r8, r2
	subs	r7, #28
	cmp	r8, r2
	bne.n	.L_0816e906
	b.n	.L_0816eace
.L_0816e9c6:
	ldr	r3, [pc, #132]
	mov	r1, sl
	str	r3, [r1, #8]
	ldr	r2, [sp, #16]
	movs	r3, #6
	add	r6, sp, #56
	strb	r3, [r2, #0]
	str	r6, [sp, #16]
	strb	r3, [r6, #1]
	ldr	r3, [sp, #32]
	cmp	r3, #0
	bne.n	.L_0816ea66
	ldr	r4, [sp, #8]
	ldr	r5, [sp, #12]
	movs	r1, #0
	mov	r8, r1
	mov	r0, fp
.L_0816e9e8:
	ldr	r3, [r4, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r0, #0]
	ldr	r3, [r4, #4]
	subs	r3, #32
	lsls	r3, r3, #16
	str	r3, [r0, #4]
	ldr	r2, [r5, #0]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r4, #0]
	asrs	r2, r2, #1
	lsrs	r1, r3, #31
	adds	r3, r3, r1
	asrs	r3, r3, #1
	subs	r2, r2, r3
	lsls	r2, r2, #11
	str	r2, [r0, #12]
	ldr	r2, [r4, #4]
	ldr	r3, [r5, #4]
	subs	r3, r3, r2
	lsls	r3, r3, #11
	movs	r2, #0
	str	r3, [r0, #16]
	str	r2, [r0, #8]
	ldr	r1, [sp, #52]
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_0816ea54
	ldr	r3, [pc, #36]
	b.n	.L_0816ea58
	.4byte 0x000000db
	.4byte 0x00000100
	.4byte 0x02010000
	.4byte 0x0819928c
	.4byte 0xffffb200
	.4byte 0xffffe000
	.4byte 0xfff00000
	.4byte 0x081991c0
	.4byte 0x08199340
	.2byte 0xf000
	.2byte 0xffff
.L_0816ea54:
	movs	r3, #128
	lsls	r3, r3, #5
.L_0816ea58:
	str	r3, [r0, #20]
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r0, #28
	cmp	r3, #5
	bne.n	.L_0816e9e8
.L_0816ea66:
	ldr	r1, [sp, #32]
	cmp	r1, #7
	ble.n	.L_0816eace
	movs	r3, #224
	lsls	r3, r3, #3
	add	r3, fp
	str	r3, [r6, #4]
	bl	sub_08014de4
	mov	r2, fp
	ldr	r3, [pc, #448]
	ldr	r0, [r2, #0]
	ldr	r1, [r2, #4]
	movs	r5, #156
	adds	r0, r0, r3
	adds	r1, r1, r3
	movs	r2, #0
	lsls	r5, r5, #9
	bl	sub_08015160
	adds	r5, #128
	movs	r2, #128
	adds	r1, r5, #0
	asrs	r0, r5, #1
	lsls	r2, r2, #8
	bl	sub_080151e4
	mov	r3, fp
	ldr	r0, [r3, #8]
	bl	sub_080150e4
	ldr	r1, [sp, #52]
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_0816eab4
	movs	r0, #128
	lsls	r0, r0, #8
	bl	sub_08015068
.L_0816eab4:
	ldr	r1, [sp, #20]
	movs	r2, #4
	ldr	r0, [pc, #388]
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
	mov	r0, fp
	movs	r1, #64
	movs	r2, #0
	bl	sub_08138058
.L_0816eace:
	mov	r0, sl
	bl	sub_08013164
	ldr	r0, [sp, #20]
	bl	sub_08013164
	ldr	r6, [pc, #360]
	ldr	r2, [sp, #48]
	ldr	r1, [sp, #32]
	ldrb	r3, [r6, r2]
	cmp	r1, r3
	bne.n	.L_0816eb1a
	movs	r0, #144
	bl	sub_081180e8
	ldr	r3, [sp, #52]
	movs	r1, #4
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_08118088
	ldr	r2, [sp, #52]
	movs	r5, #8
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r3, #0
	movs	r1, #7
	movs	r2, #5
	str	r5, [sp, #0]
	bl	sub_0814cd48
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	add	r3, fp
	str	r5, [r3, #0]
	ldr	r1, [sp, #48]
	ldrb	r3, [r6, r1]
.L_0816eb1a:
	ldr	r2, [sp, #32]
	cmp	r2, r3
	blt.n	.L_0816ebda
	ldr	r2, [pc, #292]
	ldr	r1, [sp, #48]
	movs	r3, #0
	mov	r8, r3
	ldrb	r3, [r2, r1]
	cmp	r3, #0
	beq.n	.L_0816ebda
	ldr	r6, [sp, #36]
	ldr	r5, [pc, #280]
	movs	r3, #1
	movs	r1, #2
	movs	r7, #128
	mov	r9, r3
	mov	sl, r1
	lsls	r7, r7, #5
.L_0816eb3e:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_0816ebcc
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	mov	r1, r9
	str	r1, [sp, #0]
	mov	r1, sl
	str	r1, [sp, #4]
	ldr	r0, [sp, #44]
	subs	r3, #1
	ldr	r1, [pc, #244]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x213e
	adds	r0, r5, #0
	adds	r2, r7, #0
	bl	sub_08138086
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	mov	r1, r9
	str	r1, [sp, #0]
	mov	r1, sl
	str	r1, [sp, #4]
	subs	r3, #1
	ldr	r0, [sp, #44]
	ldr	r1, [pc, #208]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x213e
	adds	r0, r5, #0
	adds	r2, r7, #0
	bl	sub_08138086
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	mov	r1, r9
	str	r1, [sp, #0]
	mov	r1, sl
	subs	r3, #1
	str	r1, [sp, #4]
	ldr	r0, [sp, #44]
	ldr	r1, [pc, #172]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c3a
	adds	r0, r5, #0
	movs	r1, #62
	bl	sub_08138086
	movs	r2, #6
	ldrsh	r3, [r5, r2]
	cmp	r3, #104
	ble.n	.L_0816ebc4
	ldr	r3, [r5, #16]
	negs	r3, r3
	str	r3, [r5, #16]
	ldr	r3, [r5, #24]
	subs	r3, #8
	str	r3, [r5, #24]
.L_0816ebc4:
	ldr	r3, [r5, #24]
	ldr	r2, [pc, #128]
	subs	r3, #1
	str	r3, [r5, #24]
.L_0816ebcc:
	ldr	r1, [sp, #48]
	movs	r3, #1
	add	r8, r3
	ldrb	r3, [r2, r1]
	adds	r5, #28
	cmp	r8, r3
	bne.n	.L_0816eb3e
.L_0816ebda:
	ldr	r2, [sp, #48]
	cmp	r2, #0
	bne.n	.L_0816ebea
	movs	r0, #4
	movs	r1, #16
	bl	sub_08158ce0
	b.n	.L_0816ebf2
.L_0816ebea:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
.L_0816ebf2:
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [sp, #32]
	ldr	r1, [sp, #24]
	adds	r3, #1
	str	r3, [sp, #32]
	cmp	r3, r1
	beq.n	.L_0816ec16
	b.n	.L_0816e776
.L_0816ec16:
	ldr	r0, [sp, #28]
	movs	r1, #16
	bl	sub_08020098
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #44]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #100
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffc00000
	.4byte 0x081991e0
	.4byte 0x08198b96
	.4byte 0x08198b98
	.4byte 0x02010000
	.4byte 0x08198b9a
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #92]
	ldr	r3, [r3, #96]
	sub	sp, #56
	mov	sl, r0
	movs	r0, #1
	str	r3, [sp, #20]
	mov	r9, r1
	bl	sub_081435e0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	movs	r1, #224
	adds	r2, #50
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	ldr	r0, [pc, #44]
	add	r1, r9
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #220
	lsls	r1, r1, #6
	adds	r1, #96
	add	r1, r9
	movs	r2, #1
	movs	r3, #0
	ldr	r0, [pc, #24]
	bl	sub_08157cf4
	ldr	r0, [pc, #24]
	bl	sub_08013300
	adds	r1, r0, #0
	b.n	.L_0816eccc
	.4byte 0x00000100
	.4byte 0x00001010
	.4byte 0x000000ce
	.4byte 0x00000184
	.2byte 0x0130
	.2byte 0x0000
.L_0816eccc:
	movs	r0, #160
	ldr	r3, [pc, #608]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x22ef
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
	ldr	r0, [pc, #576]
	bl	sub_080145a8
	mov	r2, sl
	ldr	r3, [r2, #24]
	add	r5, sp, #32
	lsls	r6, r3, #2
	adds	r3, r6, #0
	adds	r3, #32
	str	r3, [sp, #16]
	adds	r1, r5, #0
	movs	r4, #36
	ldrsh	r0, [r2, r4]
	bl	sub_0815e21c
	mov	r1, sl
	ldr	r3, [r1, #20]
	lsls	r3, r3, #1
	adds	r3, #34
	ldrsh	r0, [r1, r3]
	add	r1, sp, #44
	bl	sub_0815e21c
	ldr	r1, [r5, #0]
	ldr	r3, [r5, #12]
	movs	r4, #54
	subs	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r1, r1, r3
	movs	r2, #128
	movs	r3, #64
	lsls	r2, r2, #19
	subs	r3, r3, r1
	lsls	r3, r3, #8
	adds	r2, #40
	str	r1, [r5, #0]
	negs	r4, r4
	str	r3, [r2, #0]
	movs	r3, #0
	mov	fp, r3
	cmp	r6, r4
	bne.n	.L_0816ed4c
	b.n	.L_0816f058
.L_0816ed4c:
	mov	r1, sp
	adds	r1, #24
	adds	r6, #54
	str	r1, [sp, #8]
	str	r6, [sp, #12]
.L_0816ed56:
	mov	r2, fp
	cmp	r2, #0
	bne.n	.L_0816edbe
	movs	r3, #0
	mov	r8, r3
	mov	r7, r9
.L_0816ed62:
	bl	sub_08014878
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r5, #255
	ands	r5, r0
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	movs	r3, #192
	lsls	r3, r3, #15
	str	r3, [r7, #4]
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #128
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #8
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	movs	r4, #1
	asrs	r3, r3, #8
	add	r8, r4
	str	r3, [r7, #16]
	mov	r1, r8
	movs	r3, #8
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r1, #16
	bne.n	.L_0816ed62
.L_0816edbe:
	mov	r2, fp
	cmp	r2, #32
	bne.n	.L_0816edca
	movs	r0, #134
	bl	sub_081180e8
.L_0816edca:
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_0816edd2
	b.n	.L_0816ef8c
.L_0816edd2:
	movs	r5, #1
	cmp	r3, #23
	bgt.n	.L_0816ede2
	mov	r4, sl
	ldr	r3, [r4, #24]
	cmp	r3, #0
	ble.n	.L_0816ede2
	movs	r5, #0
.L_0816ede2:
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #104
	str	r5, [sp, #0]
	bl	sub_08196404
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r1, #7
	str	r3, [sp, #24]
	movs	r2, #7
	movs	r3, #7
	movs	r0, #188
	str	r5, [sp, #0]
	bl	sub_08196404
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #8]
	movs	r2, #0
	str	r3, [r1, #4]
	mov	r3, sl
	mov	r8, r2
	ldr	r2, [r3, #24]
	movs	r4, #4
	lsls	r3, r2, #1
	adds	r3, r3, r2
	negs	r4, r4
	cmp	r3, r4
	bne.n	.L_0816ee28
	b.n	.L_0816ef80
.L_0816ee28:
	ldr	r3, [pc, #268]
	mov	r1, r8
	ldrb	r3, [r3, r1]
	mov	r2, fp
	subs	r7, r2, r3
	ldr	r2, [pc, #264]
	lsls	r3, r1, #1
	ldrsh	r6, [r2, r3]
	cmp	r7, #2
	bne.n	.L_0816ee78
	mov	r2, sl
	ldr	r1, [r2, #20]
	mov	r0, r8
	bl	sub_08002064
	adds	r3, r0, #0
	lsls	r5, r3, #1
	mov	r4, sl
	adds	r5, #36
	movs	r2, #3
	ldrsh	r0, [r4, r5]
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	bl	sub_0814cd48
	mov	r3, sl
	ldrsh	r0, [r3, r5]
	movs	r1, #0
	bl	sub_08118088
	movs	r0, #212
	bl	sub_081c0010
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	add	r3, r9
	movs	r1, #3
	str	r1, [r3, #0]
.L_0816ee78:
	ldr	r0, [pc, #196]
	mov	r2, r8
	ldrb	r3, [r0, r2]
	movs	r1, #60
	cmp	r3, #1
	bls.n	.L_0816ee86
	movs	r1, #35
.L_0816ee86:
	ldr	r3, [pc, #188]
	mov	r4, r8
	ldrb	r3, [r3, r4]
	muls	r3, r1
	cmp	r3, #0
	bge.n	.L_0816ee94
	adds	r3, #63
.L_0816ee94:
	ldr	r2, [sp, #16]
	asrs	r1, r3, #6
	cmp	fp, r2
	bge.n	.L_0816eea4
	lsls	r3, r7, #2
	adds	r3, r3, r7
	lsls	r4, r3, #1
	b.n	.L_0816eeb2
.L_0816eea4:
	ldr	r4, [sp, #16]
	mov	r3, fp
	subs	r2, r3, r4
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	subs	r4, r1, r3
.L_0816eeb2:
	cmp	r4, r1
	ble.n	.L_0816eeb8
	adds	r4, r1, #0
.L_0816eeb8:
	mov	r1, r8
	ldrb	r3, [r0, r1]
	cmp	r3, #1
	beq.n	.L_0816eede
	cmp	r3, #1
	bgt.n	.L_0816eeca
	cmp	r3, #0
	beq.n	.L_0816eed4
	b.n	.L_0816ef00
.L_0816eeca:
	cmp	r3, #2
	beq.n	.L_0816eee6
	cmp	r3, #3
	beq.n	.L_0816eef4
	b.n	.L_0816ef00
.L_0816eed4:
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	asrs	r3, r3, #1
	subs	r6, r6, r3
	b.n	.L_0816ef00
.L_0816eede:
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	asrs	r3, r3, #1
	b.n	.L_0816eefe
.L_0816eee6:
	adds	r3, r4, #0
	cmp	r4, #0
	bge.n	.L_0816eeee
	adds	r3, r4, #3
.L_0816eeee:
	asrs	r3, r3, #2
	subs	r6, r6, r3
	b.n	.L_0816ef00
.L_0816eef4:
	adds	r3, r4, #0
	cmp	r4, #0
	bge.n	.L_0816eefc
	adds	r3, r4, #3
.L_0816eefc:
	asrs	r3, r3, #2
.L_0816eefe:
	adds	r6, r6, r3
.L_0816ef00:
	mov	r3, r8
	ldrb	r2, [r0, r3]
	adds	r3, r2, #0
	cmp	r3, #1
	bhi.n	.L_0816ef48
	movs	r1, #36
	str	r1, [sp, #0]
	movs	r0, #1
	ldr	r1, [sp, #8]
	str	r4, [sp, #4]
	ands	r0, r2
	lsls	r0, r0, #2
	movs	r3, #112
	subs	r3, r3, r4
	ldr	r4, [r0, r1]
	movs	r1, #224
	adds	r2, r6, #0
	lsls	r1, r1, #3
	subs	r2, #18
	ldr	r0, [sp, #20]
	add	r1, r9
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe01d
	.4byte 0x03000730
	.4byte 0x08143001
	.4byte 0x08198bb0
	.4byte 0x08198b9c
	.4byte 0x08198bc4
	.2byte 0x8bba
	.2byte 0x0819
.L_0816ef48:
	movs	r1, #17
	str	r1, [sp, #0]
	movs	r0, #1
	ldr	r1, [sp, #8]
	str	r4, [sp, #4]
	ands	r0, r2
	lsls	r0, r0, #2
	movs	r3, #108
	subs	r3, r3, r4
	ldr	r4, [r0, r1]
	movs	r1, #247
	adds	r2, r6, #0
	lsls	r1, r1, #4
	subs	r2, #9
	ldr	r0, [sp, #20]
	add	r1, r9
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2201
	mov	r3, sl
	add	r8, r2
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #4
	cmp	r8, r3
	beq.n	.L_0816ef80
	b.n	.L_0816ee28
.L_0816ef80:
	movs	r0, #104
	bl	sub_0801314c
	movs	r0, #188
	bl	sub_0801314c
.L_0816ef8c:
	mov	r4, sl
	ldr	r0, [r4, #4]
	ldr	r1, [sp, #8]
	bl	sub_08144aac
	mov	r3, sl
	ldr	r2, [r3, #24]
	movs	r4, #8
	movs	r1, #0
	lsls	r3, r2, #2
	negs	r4, r4
	mov	r8, r1
	cmp	r3, r4
	beq.n	.L_0816f022
	mov	r5, r9
.L_0816efaa:
	mov	r1, r8
	cmp	r1, #15
	ble.n	.L_0816efb6
	ldr	r3, [sp, #16]
	cmp	fp, r3
	blt.n	.L_0816f014
.L_0816efb6:
	mov	r4, r8
	lsls	r3, r4, #1
	adds	r3, #8
	cmp	fp, r3
	blt.n	.L_0816f014
	ldr	r0, [r5, #24]
	cmp	r0, #28
	bgt.n	.L_0816f014
	movs	r1, #3
	bl	sub_08002054
	movs	r1, #2
	ldrsh	r4, [r5, r1]
	movs	r2, #6
	ldrsh	r6, [r5, r2]
	cmp	r0, #6
	ble.n	.L_0816efda
	movs	r0, #6
.L_0816efda:
	ldr	r3, [pc, #148]
	lsls	r2, r0, #1
	ldrh	r1, [r3, r2]
	movs	r3, #220
	lsls	r3, r3, #6
	adds	r3, #96
	add	r1, r9
	adds	r1, r1, r3
	ldr	r3, [pc, #136]
	ldrh	r0, [r3, r2]
	lsrs	r3, r0, #1
	subs	r2, r4, r3
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #24]
	subs	r3, r6, r3
	ldr	r0, [sp, #20]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69ab
	ldr	r2, [pc, #116]
	adds	r3, #1
	str	r3, [r5, #24]
	adds	r0, r5, #0
	movs	r1, #62
	bl	sub_08138086
	mov	r4, sl
	ldr	r2, [r4, #24]
.L_0816f014:
	movs	r1, #1
	lsls	r3, r2, #2
	add	r8, r1
	adds	r3, #8
	adds	r5, #28
	cmp	r8, r3
	bne.n	.L_0816efaa
.L_0816f022:
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	movs	r0, #4
	movs	r1, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, r9
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [sp, #12]
	movs	r2, #1
	add	fp, r2
	cmp	fp, r3
	beq.n	.L_0816f058
	b.n	.L_0816ed56
.L_0816f058:
	ldr	r0, [pc, #32]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x08198bce
	.4byte 0x08198bdc
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
	sub	sp, #68
	str	r0, [sp, #44]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #96]
	ldr	r0, [r3, #92]
	str	r2, [sp, #40]
	mov	sl, r0
	ldr	r3, [r3, #100]
	movs	r0, #1
	str	r3, [sp, #24]
	bl	sub_081435e0
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r4, [sp, #44]
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_0816f0d4
	movs	r0, #104
	movs	r1, #19
	bl	sub_081963ec
	b.n	.L_0816f0dc
	movs	r0, r0
	.4byte 0x00001010
	.2byte 0x0000
	.2byte 0x0000
.L_0816f0d4:
	movs	r0, #104
	movs	r1, #23
	bl	sub_081963ec
.L_0816f0dc:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r1, #239
	movs	r2, #238
	lsls	r1, r1, #7
	lsls	r2, r2, #7
	str	r3, [sp, #32]
	add	r1, sl
	movs	r3, #2
	adds	r2, #132
	str	r3, [r1, #0]
	add	r2, sl
	movs	r3, #50
	str	r3, [r2, #0]
	movs	r3, #1
	str	r3, [r1, #0]
	movs	r3, #0
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #908]
	bl	sub_080145a8
	ldr	r6, [sp, #44]
	movs	r5, #160
	ldr	r0, [r6, #8]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r3, [pc, #892]
	str	r0, [sp, #20]
	movs	r0, #160
	lsls	r0, r0, #19
	movs	r1, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x04ed
	adds	r5, #2
	movs	r6, #0
.L_0816f12c:
	adds	r0, r6, #0
	movs	r1, #3
	bl	sub_08002054
	adds	r6, #1
	adds	r0, #8
	strh	r0, [r5, #0]
	adds	r5, #2
	cmp	r6, #63
	bne.n	.L_0816f12c
	movs	r0, #16
	mov	fp, r0
	movs	r0, #184
	lsls	r0, r0, #5
	movs	r1, #128
	ldr	r3, [pc, #844]
	lsls	r1, r1, #4
	add	r0, sl
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x20e0
	lsls	r0, r0, #3
	movs	r2, #128
	lsls	r2, r2, #9
	add	r0, sl
	movs	r1, #16
	bl	sub_0815b434
	movs	r2, #0
	mov	r8, r2
	mov	lr, r2
.L_0816f16a:
	movs	r3, #0
	movs	r4, #8
	mov	ip, r3
	cmp	r4, #0
	beq.n	.L_0816f202
	mov	r6, lr
	lsrs	r3, r6, #31
	add	r3, lr
	asrs	r3, r3, #1
	mov	r9, r3
	mov	r3, fp
	movs	r0, #176
	add	r3, lr
	lsls	r0, r0, #5
	lsls	r3, r3, #1
	mov	r6, r8
	adds	r0, #254
	add	r3, sl
	movs	r1, #184
	adds	r7, r3, r0
	lsls	r3, r6, #5
	lsls	r1, r1, #5
	add	r3, sl
	mov	r2, fp
	adds	r5, r3, r1
	movs	r3, #32
	lsls	r4, r2, #1
	subs	r3, r3, r6
	movs	r2, #184
	lsls	r3, r3, #5
	lsls	r2, r2, #5
	subs	r3, #32
	adds	r2, #1
	adds	r2, r3, r2
	adds	r6, r2, #0
	movs	r2, #176
	lsls	r2, r2, #5
	adds	r2, #255
	adds	r2, r3, r2
	adds	r1, r3, r1
	adds	r3, r3, r0
	str	r2, [sp, #8]
	add	r3, sl
	add	r2, sl
	adds	r2, r4, r2
	add	r6, sl
	add	r1, sl
	adds	r4, r4, r3
.L_0816f1ca:
	mov	r3, r9
	movs	r0, #224
	lsls	r0, r0, #3
	add	r3, ip
	adds	r3, r3, r0
	mov	r0, sl
	ldrb	r3, [r0, r3]
	cmp	r3, #0
	beq.n	.L_0816f1ec
	strb	r3, [r5, #0]
	strb	r3, [r5, #1]
	strb	r3, [r7, #0]
	strb	r3, [r7, #1]
	strb	r3, [r1, #0]
	strb	r3, [r6, #0]
	strb	r3, [r4, #0]
	strb	r3, [r2, #0]
.L_0816f1ec:
	movs	r3, #1
	add	ip, r3
	mov	r0, ip
	subs	r2, #2
	subs	r4, #2
	subs	r7, #2
	adds	r5, #2
	adds	r6, #2
	adds	r1, #2
	cmp	r0, #8
	bne.n	.L_0816f1ca
.L_0816f202:
	movs	r2, #1
	add	r8, r2
	add	lr, fp
	cmp	r8, fp
	bne.n	.L_0816f16a
	ldr	r1, [sp, #24]
	ldr	r0, [pc, #652]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #142
	lsls	r1, r1, #7
	movs	r3, #0
	ldr	r0, [pc, #640]
	add	r1, sl
	movs	r2, #0
	bl	sub_08157cf4
	mov	r4, sp
	movs	r3, #0
	adds	r4, #48
	str	r3, [sp, #28]
	str	r4, [sp, #16]
.L_0816f232:
	ldr	r6, [sp, #28]
	cmp	r6, #0
	bne.n	.L_0816f2b0
	ldr	r3, [sp, #44]
	add	r7, sp, #56
	adds	r1, r7, #0
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_0815e21c
	movs	r6, #0
	movs	r1, #0
	mov	r5, sl
.L_0816f24c:
	str	r1, [sp, #12]
	bl	sub_08014878
	ldr	r3, [r7, #0]
	ldr	r1, [sp, #12]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	movs	r2, #15
	ands	r2, r0
	asrs	r3, r3, #1
	adds	r3, r3, r2
	subs	r3, #8
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	movs	r3, #8
	negs	r3, r3
	str	r3, [r5, #4]
	str	r1, [r5, #12]
	str	r1, [r5, #16]
	bl	sub_08014878
	movs	r3, #3
	ands	r3, r0
	adds	r3, #4
	ldr	r1, [sp, #12]
	str	r3, [r5, #8]
	lsls	r2, r6, #4
	movs	r3, #160
	subs	r3, r3, r2
	adds	r6, #1
	str	r3, [r5, #20]
	str	r1, [r5, #24]
	adds	r5, #28
	cmp	r6, #16
	bne.n	.L_0816f24c
	ldr	r3, [r7, #0]
	movs	r1, #192
	lsrs	r2, r3, #31
	lsls	r1, r1, #3
	adds	r3, r3, r2
	adds	r1, #228
	asrs	r3, r3, #1
	add	r1, sl
	lsls	r3, r3, #16
	str	r3, [r1, #0]
	movs	r3, #224
	lsls	r3, r3, #15
	movs	r4, #0
	str	r3, [r1, #4]
	str	r4, [r1, #24]
.L_0816f2b0:
	ldr	r6, [sp, #28]
	cmp	r6, #63
	bgt.n	.L_0816f33e
	movs	r6, #0
	mov	r5, sl
.L_0816f2ba:
	ldr	r0, [sp, #28]
	lsls	r3, r6, #3
	cmp	r0, r3
	ble.n	.L_0816f336
	ldr	r7, [r5, #24]
	cmp	r7, #0
	bne.n	.L_0816f336
	ldr	r0, [r5, #8]
	ldr	r2, [pc, #472]
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #24]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	mov	lr, r2
	movs	r3, #6
	ldrsh	r2, [r5, r3]
	str	r0, [sp, #0]
	subs	r3, r2, r0
	str	r4, [sp, #4]
	mov	r2, lr
	ldr	r0, [sp, #40]
	ldr	r4, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r5, #0
	movs	r1, #64
	lsls	r2, r2, #8
	bl	sub_08138086
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	cmp	r3, #111
	ble.n	.L_0816f336
	movs	r3, #1
	str	r3, [r5, #24]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	subs	r3, #16
	lsls	r3, r3, #12
	str	r3, [r5, #12]
	ldr	r2, [pc, #392]
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L_0816f328
	adds	r3, r6, #3
.L_0816f328:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r6, r3
	ldrsb	r3, [r2, r3]
	str	r7, [r5, #16]
	lsls	r3, r3, #13
	str	r3, [r5, #12]
.L_0816f336:
	adds	r6, #1
	adds	r5, #28
	cmp	r6, #6
	bne.n	.L_0816f2ba
.L_0816f33e:
	movs	r0, #32
	bl	sub_08014dac
	mov	fp, r0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #348]
	ldr	r3, [sp, #48]
	adds	r7, r0, #0
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #340]
	movs	r4, #0
	ands	r3, r2
	movs	r2, #160
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #48]
	ldr	r2, [sp, #16]
	movs	r3, #184
	lsls	r3, r3, #5
	add	r3, sl
	str	r3, [r2, #4]
	movs	r3, #6
	str	r3, [r7, #0]
	ldr	r3, [pc, #316]
	movs	r0, #192
	str	r3, [r7, #8]
	mov	r3, fp
	lsls	r0, r0, #3
	str	r2, [r7, #16]
	str	r3, [r7, #12]
	str	r4, [r7, #20]
	adds	r0, #228
	movs	r6, #0
	add	r0, sl
	mov	r1, sl
.L_0816f38c:
	ldr	r3, [r1, #24]
	cmp	r3, #0
	ble.n	.L_0816f3a4
	ldr	r2, [r1, #20]
	ldr	r3, [r0, #24]
	adds	r3, r3, r2
	str	r3, [r0, #24]
	ldr	r3, [r1, #20]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r1, #20]
.L_0816f3a4:
	adds	r6, #1
	adds	r1, #28
	cmp	r6, #8
	bne.n	.L_0816f38c
	movs	r6, #192
	lsls	r6, r6, #3
	adds	r6, #228
	add	r6, sl
	ldr	r5, [r6, #24]
	cmp	r5, #0
	ble.n	.L_0816f408
	movs	r1, #206
	lsls	r0, r5, #13
	lsls	r1, r1, #2
	bl	sub_08002054
	mov	r8, r0
	ldr	r0, [sp, #28]
	cmp	r0, #63
	ble.n	.L_0816f3d2
	adds	r3, r5, #0
	subs	r3, #24
	str	r3, [r6, #24]
.L_0816f3d2:
	bl	sub_08014de4
	ldr	r3, [pc, #224]
	ldr	r0, [r6, #0]
	ldr	r1, [r6, #4]
	adds	r0, r0, r3
	adds	r1, r1, r3
	movs	r2, #0
	bl	sub_08015160
	mov	r3, r8
	lsls	r2, r3, #1
	adds	r0, r2, #0
	mov	r1, r8
	bl	sub_080151e4
	ldr	r0, [pc, #200]
	bl	sub_08015024
	ldr	r0, [pc, #196]
	mov	r1, fp
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_0816f408:
	ldr	r4, [sp, #16]
	add	r6, sp, #48
	movs	r3, #5
	strb	r3, [r4, #0]
	str	r6, [sp, #16]
	strb	r3, [r6, #1]
	movs	r3, #7
	str	r3, [r7, #0]
	ldr	r3, [pc, #152]
	movs	r0, #194
	lsls	r0, r0, #7
	str	r3, [r7, #8]
	adds	r0, #168
	mov	r8, r6
	mov	r9, r0
	movs	r6, #0
	mov	r5, sl
.L_0816f42a:
	ldr	r2, [sp, #28]
	lsls	r3, r6, #2
	adds	r3, #64
	cmp	r2, r3
	ble.n	.L_0816f4d0
	movs	r3, #0
	str	r3, [r7, #20]
	lsrs	r3, r6, #31
	adds	r3, r6, r3
	asrs	r3, r3, #1
	lsls	r3, r3, #1
	subs	r3, r6, r3
	lsls	r3, r3, #10
	movs	r4, #142
	lsls	r4, r4, #7
	add	r3, sl
	adds	r3, r3, r4
	mov	r0, r8
	str	r3, [r0, #4]
	bl	sub_08014de4
	ldr	r3, [pc, #108]
	ldr	r0, [r5, #0]
	ldr	r2, [pc, #92]
	ldr	r1, [r5, #4]
	adds	r0, r0, r2
	adds	r1, r1, r3
	movs	r2, #0
	bl	sub_08015160
	mov	r4, r9
	asrs	r2, r4, #1
	adds	r0, r2, #0
	mov	r1, r9
	bl	sub_080151e4
	ldr	r0, [pc, #84]
	bl	sub_08015024
	mov	r1, fp
	movs	r2, #4
	ldr	r0, [pc, #64]
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
	adds	r0, r5, #0
	movs	r1, #60
	ldr	r2, [pc, #60]
	bl	sub_08138086
	b.n	.L_0816f4d0
	.4byte 0x08143001
	.4byte 0x03000258
	.4byte 0x00000134
	.4byte 0x000000d8
	.4byte 0x08197410
	.4byte 0x08198bea
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199244
	.4byte 0xffc00000
	.4byte 0xffffe000
	.4byte 0x08199210
	.4byte 0xffd00000
	.4byte 0xffffc000
	.2byte 0x8000
	.2byte 0xffff
.L_0816f4d0:
	.2byte 0x3601
	adds	r5, #28
	cmp	r6, #6
	bne.n	.L_0816f42a
	adds	r0, r7, #0
	bl	sub_08013164
	mov	r0, fp
	bl	sub_08013164
	ldr	r6, [sp, #28]
	cmp	r6, #64
	bne.n	.L_0816f4f0
	movs	r0, #147
	bl	sub_081c0010
.L_0816f4f0:
	movs	r6, #0
	movs	r7, #2
	movs	r5, #80
.L_0816f4f6:
	ldr	r0, [sp, #28]
	cmp	r0, r5
	bne.n	.L_0816f536
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	add	r3, sl
	str	r7, [r3, #0]
	ldr	r3, [sp, #44]
	movs	r1, #0
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_08118088
	ldr	r2, [sp, #44]
	movs	r1, #7
	movs	r4, #36
	ldrsh	r0, [r2, r4]
	movs	r3, #0
	movs	r2, #5
	str	r7, [sp, #0]
	bl	sub_0814cd48
	cmp	r6, #0
	bne.n	.L_0816f530
	movs	r0, #134
	bl	sub_081180e8
	b.n	.L_0816f536
.L_0816f530:
	movs	r0, #133
	bl	sub_081c0010
.L_0816f536:
	adds	r6, #1
	adds	r5, #4
	cmp	r6, #6
	bne.n	.L_0816f4f6
	movs	r0, #4
	movs	r1, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [sp, #28]
	adds	r3, #1
	str	r3, [sp, #28]
	cmp	r3, #120
	beq.n	.L_0816f568
	b.n	.L_0816f232
.L_0816f568:
	ldr	r0, [sp, #20]
	movs	r1, #16
	bl	sub_08020098
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #24]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #68
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
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r1, [r5, #92]
	ldr	r2, [r5, #96]
	sub	sp, #48
	mov	fp, r0
	movs	r0, #0
	str	r2, [sp, #24]
	mov	r9, r1
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	movs	r1, #184
	lsls	r1, r1, #5
	add	r1, r9
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #36]
	bl	sub_08157cf4
	ldr	r0, [pc, #36]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #28]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x22ef
	lsls	r2, r2, #7
	add	r2, r9
	b.n	.L_0816f608
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x000000c2
	.4byte 0x00000148
	.2byte 0x0730
	.2byte 0x0300
.L_0816f608:
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, r9
	movs	r3, #75
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #744]
	bl	sub_080145a8
	ldr	r5, [r5, #48]
	mov	r2, fp
	str	r5, [sp, #16]
	movs	r7, #0
	movs	r4, #54
	ldrsh	r3, [r5, r4]
	movs	r6, #2
	str	r3, [sp, #12]
	movs	r5, #0
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	bl	sub_08118098
	movs	r3, #224
	ldr	r0, [r0, #0]
	lsls	r3, r3, #3
	add	r3, r9
	str	r0, [sp, #8]
	str	r3, [sp, #4]
	movs	r4, #0
	mov	r8, r4
.L_0816f64c:
	ldr	r1, [pc, #700]
	movs	r2, #167
	lsls	r2, r2, #9
	adds	r0, r7, r1
	adds	r2, #32
	adds	r1, r6, #0
	bl	sub_0815b434
	adds	r3, r5, #3
	muls	r3, r6
	ldr	r2, [sp, #4]
	adds	r6, #2
	strh	r7, [r5, r2]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r7, r7, r3
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r5, #2
	cmp	r4, #32
	bne.n	.L_0816f64c
	movs	r1, #0
	str	r1, [sp, #20]
.L_0816f67e:
	ldr	r2, [sp, #20]
	cmp	r2, #0
	bne.n	.L_0816f750
	add	r3, sp, #12
	ldr	r4, [sp, #16]
	ldrh	r3, [r3, #0]
	mov	r7, r9
	strh	r3, [r4, #54]
	bl	sub_08014de4
	ldr	r0, [sp, #16]
	adds	r1, r0, #0
	adds	r1, #12
	bl	sub_080156e8
	movs	r1, #36
	movs	r4, #0
	add	r1, sp
	mov	r8, r4
	mov	sl, r1
.L_0816f6a6:
	bl	sub_08014878
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08014878
	movs	r5, #7
	mov	r2, r8
	lsls	r3, r2, #2
	ands	r5, r0
	adds	r0, r6, #0
	adds	r5, r5, r3
	bl	sub_08002096
	ldr	r4, [sp, #8]
	adds	r2, r5, #0
	muls	r2, r0
	ldr	r3, [r4, #8]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	str	r3, [r7, #0]
	adds	r0, r6, #0
	bl	sub_08002090
	ldr	r1, [sp, #8]
	adds	r2, r5, #0
	muls	r2, r0
	ldr	r3, [r1, #12]
	adds	r0, r7, #0
	adds	r3, r3, r2
	movs	r2, #192
	lsls	r2, r2, #14
	adds	r3, r3, r2
	str	r3, [r7, #4]
	ldr	r3, [r1, #16]
	mov	r1, sl
	str	r3, [r7, #8]
	bl	sub_0815e1ec
	mov	r4, sl
	ldr	r3, [r4, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r7, #0]
	ldr	r3, [r4, #4]
	str	r3, [r7, #4]
	bl	sub_08014878
	mov	r1, r8
	movs	r3, #32
	subs	r3, r3, r1
	str	r3, [r7, #12]
	bl	sub_08014878
	str	r0, [r7, #16]
	bl	sub_08014878
	mov	r2, r8
	negs	r3, r2
	lsls	r3, r3, #2
	str	r3, [r7, #24]
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	str	r0, [r7, #20]
	adds	r7, #28
	cmp	r4, #32
	bne.n	.L_0816f6a6
	ldr	r1, [sp, #20]
	cmp	r1, #0
	bne.n	.L_0816f750
	mov	r2, fp
	movs	r3, #36
	ldrsh	r1, [r2, r3]
	ldr	r0, [r2, #8]
	movs	r3, #80
	movs	r2, #8
	bl	sub_08157530
.L_0816f750:
	ldr	r4, [sp, #20]
	cmp	r4, #8
	bne.n	.L_0816f796
	movs	r1, #240
	ldr	r0, [sp, #24]
	lsls	r1, r1, #6
	ldr	r3, [pc, #432]
	ldr	r2, [pc, #436]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x22ee
	lsls	r2, r2, #7
	adds	r2, #168
	movs	r3, #16
	add	r2, r9
	str	r3, [r2, #0]
	mov	r2, fp
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r1, #0
	bl	sub_08118088
	ldr	r1, [sp, #20]
	mov	r4, fp
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	movs	r2, #5
	str	r1, [sp, #0]
	movs	r3, #0
	movs	r1, #7
	bl	sub_0814cd48
	movs	r0, #145
	bl	sub_081180e8
.L_0816f796:
	ldr	r2, [sp, #20]
	cmp	r2, #7
	bgt.n	.L_0816f79e
	b.n	.L_0816f8c0
.L_0816f79e:
	movs	r3, #0
	mov	r8, r3
	mov	r6, r9
.L_0816f7a4:
	ldr	r3, [r6, #24]
	adds	r5, r3, #1
	str	r5, [r6, #24]
	cmp	r5, #8
	bne.n	.L_0816f7dc
	movs	r0, #144
	bl	sub_081c0010
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	add	r3, r9
	str	r5, [r3, #0]
	mov	r1, fp
	movs	r4, #36
	ldrsh	r0, [r1, r4]
	movs	r1, #0
	bl	sub_08118088
	mov	r3, fp
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	str	r5, [sp, #0]
	bl	sub_0814cd48
.L_0816f7dc:
	ldr	r0, [r6, #24]
	cmp	r0, #31
	bhi.n	.L_0816f806
	lsls	r0, r0, #10
	bl	sub_08002096
	ldr	r3, [r6, #12]
	muls	r3, r0
	asrs	r3, r3, #16
	lsls	r3, r3, #1
	subs	r2, r3, #2
	cmp	r2, #61
	bhi.n	.L_0816f806
	ldr	r4, [sp, #4]
	ldrsh	r0, [r2, r4]
	ldr	r2, [pc, #272]
	ldr	r1, [r6, #0]
	adds	r0, r0, r2
	ldr	r2, [r6, #4]
	bl	sub_0818caa8
.L_0816f806:
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r6, #28
	cmp	r4, #8
	bne.n	.L_0816f7a4
	movs	r0, #32
	bl	sub_08014dac
	mov	sl, r0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #244]
	ldr	r3, [sp, #28]
	adds	r6, r0, #0
	ands	r3, r2
	movs	r2, #7
	orrs	r3, r2
	ldr	r2, [pc, #236]
	mov	r1, sl
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #28]
	movs	r3, #184
	lsls	r3, r3, #5
	add	r2, sp, #28
	add	r3, r9
	str	r3, [r2, #4]
	movs	r3, #6
	str	r3, [r6, #0]
	ldr	r3, [pc, #212]
	str	r1, [r6, #12]
	str	r3, [r6, #8]
	str	r2, [r6, #16]
	movs	r2, #0
	mov	r8, r2
	mov	r5, r9
.L_0816f856:
	ldr	r2, [r5, #24]
	cmp	r2, #23
	bhi.n	.L_0816f8a8
	lsls	r3, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	lsls	r7, r3, #11
	cmp	r2, #15
	ble.n	.L_0816f86e
	movs	r3, #16
	subs	r3, r3, r2
	lsls	r1, r3, #3
.L_0816f86e:
	str	r1, [r6, #20]
	bl	sub_08014de4
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	subs	r0, #64
	subs	r1, #64
	lsls	r1, r1, #16
	movs	r2, #0
	lsls	r0, r0, #16
	bl	sub_08015160
	ldr	r0, [r5, #20]
	bl	sub_080150e4
	ldr	r0, [r5, #16]
	bl	sub_08015024
	adds	r0, r7, #0
	bl	sub_0801521c
	ldr	r0, [pc, #136]
	mov	r1, sl
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_0816f8a8:
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r5, #28
	cmp	r4, #8
	bne.n	.L_0816f856
	adds	r0, r6, #0
	bl	sub_08013164
	mov	r0, sl
	bl	sub_08013164
.L_0816f8c0:
	movs	r1, #4
	movs	r0, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, r9
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [sp, #20]
	adds	r1, #1
	str	r1, [sp, #20]
	cmp	r1, #66
	beq.n	.L_0816f8ea
	b.n	.L_0816f67e
.L_0816f8ea:
	ldr	r0, [pc, #28]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x08143001
	.4byte 0x02010000
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199364
	.2byte 0x91e0
	.2byte 0x0819
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r5, #192
	lsls	r5, r5, #18
	mov	r9, r0
	ldr	r0, [r5, #92]
	ldr	r2, [r5, #96]
	sub	sp, #32
	mov	r8, r0
	movs	r0, #1
	str	r2, [sp, #16]
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	ldr	r5, [r5, #104]
	ldr	r0, [pc, #44]
	str	r5, [sp, #8]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x22ef
	lsls	r2, r2, #7
	add	r2, r8
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	movs	r3, #50
	b.n	.L_0816f998
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x00000166
	.2byte 0x0730
	.2byte 0x0300
.L_0816f998:
	add	r2, r8
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #92]
	bl	sub_080145a8
	mov	r4, r9
	add	r5, sp, #20
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	adds	r1, r5, #0
	bl	sub_0815e21c
	ldr	r2, [r5, #0]
	movs	r3, #64
	subs	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #40
	lsls	r3, r3, #8
	movs	r5, #224
	str	r3, [r2, #0]
	lsls	r5, r5, #3
	mov	r2, r8
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r0, r2, r5
	ldr	r3, [pc, #48]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x20e0
	lsls	r0, r0, #3
	movs	r7, #0
	adds	r0, #3
	movs	r1, #24
.L_0816f9e0:
	mov	r4, r9
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_0816fa08
	lsls	r2, r7, #2
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_0816f9f2
	adds	r3, #15
.L_0816f9f2:
	asrs	r3, r3, #4
	subs	r3, r2, r3
	adds	r3, r3, r0
	mov	r2, r8
	strb	r1, [r2, r3]
	b.n	.L_0816fa1c
	movs	r0, r0
	.4byte 0x08143001
	.2byte 0x0258
	.2byte 0x0300
.L_0816fa08:
	lsls	r2, r7, #2
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_0816fa12
	adds	r3, #15
.L_0816fa12:
	asrs	r3, r3, #4
	adds	r3, r2, r3
	adds	r3, r3, r5
	mov	r4, r8
	strb	r1, [r4, r3]
.L_0816fa1c:
	adds	r7, #1
	cmp	r7, #64
	bne.n	.L_0816f9e0
	movs	r1, #228
	lsls	r1, r1, #6
	ldr	r0, [pc, #72]
	movs	r2, #1
	movs	r3, #0
	add	r1, r8
	bl	sub_08157cf4
	movs	r2, #128
	ldr	r3, [pc, #52]
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	movs	r0, #0
	mov	fp, r0
.L_0816fa40:
	mov	r2, fp
	cmp	r2, #0
	bne.n	.L_0816fac2
	movs	r0, #170
	bl	sub_081c0010
	ldr	r3, [pc, #36]
	movs	r1, #1
	movs	r2, #128
	movs	r7, #0
	negs	r1, r1
	lsls	r2, r2, #1
.L_0816fa58:
	adds	r7, #1
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r7, r2
	bne.n	.L_0816fa58
	movs	r7, #0
	movs	r6, #63
	mov	r5, r8
	b.n	.L_0816fa78
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000161
	.2byte 0x0018
	.2byte 0x0201
.L_0816fa78:
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	subs	r3, #64
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	mov	r4, r9
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_0816fa9c
	bl	sub_08014878
	ldr	r3, [pc, #480]
	ands	r0, r6
	adds	r0, #40
	lsls	r0, r0, #16
	b.n	.L_0816faaa
.L_0816fa9c:
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #24
	movs	r3, #128
	lsls	r0, r0, #16
	lsls	r3, r3, #9
.L_0816faaa:
	str	r0, [r5, #0]
	str	r3, [r5, #12]
	movs	r3, #128
	lsls	r3, r3, #13
	str	r3, [r5, #16]
	movs	r3, #1
	negs	r3, r3
	adds	r7, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #64
	bne.n	.L_0816fa78
.L_0816fac2:
	mov	r0, fp
	cmp	r0, #32
	bne.n	.L_0816fada
	mov	r2, r9
	movs	r3, #36
	ldrsh	r1, [r2, r3]
	movs	r3, #192
	ldr	r0, [r2, #8]
	lsls	r3, r3, #10
	movs	r2, #8
	bl	sub_08118078
.L_0816fada:
	mov	r4, fp
	cmp	r4, #46
	bne.n	.L_0816faf2
	movs	r0, #134
	bl	sub_081180e8
	mov	r3, r9
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r1, #4
	bl	sub_08118088
.L_0816faf2:
	ldr	r5, [pc, #388]
	movs	r7, #0
.L_0816faf6:
	ldr	r3, [r5, #24]
	cmp	r3, #23
	bhi.n	.L_0816fb3e
	cmp	r3, #0
	bge.n	.L_0816fb02
	adds	r3, #3
.L_0816fb02:
	asrs	r3, r3, #2
	lsls	r1, r3, #3
	adds	r1, r1, r3
	lsls	r1, r1, #7
	movs	r4, #228
	lsls	r4, r4, #6
	add	r1, r8
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	adds	r1, r1, r4
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	movs	r0, #24
	str	r0, [sp, #0]
	movs	r0, #48
	subs	r3, #32
	str	r0, [sp, #4]
	subs	r2, #12
	ldr	r0, [sp, #16]
	ldr	r4, [sp, #8]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #56
	ldr	r2, [pc, #328]
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_0816fb3e:
	movs	r0, #128
	adds	r7, #1
	lsls	r0, r0, #1
	adds	r5, #28
	cmp	r7, r0
	bne.n	.L_0816faf6
	movs	r7, #0
	movs	r5, #6
.L_0816fb4e:
	cmp	fp, r5
	bne.n	.L_0816fb6c
	mov	r3, r9
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	movs	r0, #132
	bl	sub_081c0010
.L_0816fb6c:
	adds	r7, #1
	adds	r5, #4
	cmp	r7, #8
	bne.n	.L_0816fb4e
	movs	r4, #0
	movs	r7, #0
	mov	r6, r8
	mov	sl, r4
.L_0816fb7c:
	cmp	fp, r7
	blt.n	.L_0816fbfa
	movs	r0, #6
	ldrsh	r4, [r6, r0]
	cmp	r4, #111
	bgt.n	.L_0816fbfa
	movs	r1, #64
	cmp	r4, #48
	ble.n	.L_0816fb92
	movs	r3, #112
	subs	r1, r3, r4
.L_0816fb92:
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	str	r1, [sp, #4]
	movs	r1, #224
	movs	r3, #4
	lsls	r1, r1, #3
	str	r3, [sp, #0]
	add	r1, r8
	adds	r3, r4, #0
	ldr	r0, [sp, #16]
	ldr	r4, [sp, #8]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x6832
	ldr	r3, [r6, #12]
	adds	r1, r2, r3
	ldr	r3, [r6, #4]
	ldr	r2, [r6, #16]
	str	r1, [r6, #0]
	adds	r3, r3, r2
	str	r3, [r6, #4]
	asrs	r3, r3, #16
	cmp	r3, #48
	ble.n	.L_0816fbfa
	ldr	r5, [pc, #180]
	movs	r0, #1
	add	r5, sl
	ldr	r3, [r5, #24]
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_0816fbfa
	movs	r3, #224
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #24]
	str	r1, [r5, #0]
	bl	sub_08014878
	movs	r1, #12
	bl	sub_0800206c
	subs	r0, #6
	lsls	r0, r0, #15
	str	r0, [r5, #12]
	bl	sub_08014878
	movs	r3, #7
	negs	r0, r0
	ands	r0, r3
	lsls	r0, r0, #10
	str	r0, [r5, #16]
.L_0816fbfa:
	movs	r2, #28
	adds	r7, #1
	adds	r6, #28
	add	sl, r2
	cmp	r7, #32
	bne.n	.L_0816fb7c
	movs	r0, #4
	movs	r1, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	movs	r3, #1
	add	r2, r8
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #1
	add	fp, r3
	mov	r4, fp
	cmp	r4, #68
	beq.n	.L_0816fc30
	b.n	.L_0816fa40
.L_0816fc30:
	ldr	r6, [pc, #76]
	adds	r0, r6, #0
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	movs	r1, #240
	ldr	r5, [pc, #64]
	lsls	r1, r1, #6
	ldr	r0, [pc, #64]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x21f0
	ldr	r0, [sp, #16]
	lsls	r1, r1, #6
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x480d
	bl	sub_08014644
	adds	r0, r6, #0
	bl	sub_08014644
	mov	r0, r9
	bl	sub_081504b4
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffff0000
	.4byte 0x02010000
	.4byte 0xffffc000
	.4byte 0x08143001
	.4byte 0x03000258
	.4byte 0x06004000
	.2byte 0x3489
	.2byte 0x0814
	push	{lr}
	movs	r1, #1
	bl	sub_0816fca8
	pop	{pc}
	.align 2, 0
