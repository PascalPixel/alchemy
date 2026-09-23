.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_08013560, 0x08013560
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08118070, 0x08118070
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_081434f8, 0x081434f8
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_081504b4, 0x081504b4
	.set sub_081504c0, 0x081504c0
	.set sub_081504cc, 0x081504cc
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815f000, 0x0815f000
	.set sub_0816031e, 0x0816031e
	.set sub_08163580, 0x08163580
	.set sub_081c0010, 0x081c0010
	.global Func_0816342a
	.thumb_func
Func_0816342a:
	ldr	r3, [sp, #124]
	cmp	r3, #8
	beq.n	.L_08163432
	b.n	sub_08163580
.L_08163432:
	ldr	r5, [sp, #112]
	cmp	r5, #43
	ble.n	.L_0816343a
	b.n	sub_08163580
.L_0816343a:
	add	r6, sp, #192
	adds	r1, r6, #0
	ldr	r0, [sp, #16]
	bl	sub_0815e1ec
	ldr	r2, [r6, #0]
	movs	r0, #20
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	str	r2, [r6, #0]
	ldr	r3, [r6, #4]
	str	r0, [sp, #0]
	movs	r0, #40
	str	r0, [sp, #4]
	ldr	r0, [sp, #84]
	ldr	r1, [pc, #724]
	ldr	r4, [r0, #4]
	subs	r2, #10
	subs	r3, #4
	ldr	r0, [sp, #116]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2d17
	bne.n	.L_0816347c
	ldr	r1, [sp, #128]
	ldr	r0, [r1, #8]
	bl	sub_08118098
	ldr	r2, [r0, #0]
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r2, #40]
.L_0816347c:
	ldr	r2, [sp, #112]
	cmp	r2, #19
	ble.n	.L_0816348e
	ldr	r5, [sp, #16]
	movs	r0, #128
	ldr	r3, [r5, #4]
	lsls	r0, r0, #11
	adds	r3, r3, r0
	str	r3, [r5, #4]
.L_0816348e:
	ldr	r1, [sp, #112]
	cmp	r1, #23
	bne.n	.L_081634fa
	ldr	r3, [sp, #128]
	movs	r5, #240
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_08118070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	lsls	r5, r5, #13
	adds	r7, r0, r5
	ldr	r5, [sp, #120]
	movs	r0, #0
	mov	fp, r0
	movs	r6, #255
.L_081634b2:
	ldr	r1, [sp, #60]
	ldr	r3, [r1, #8]
	str	r7, [r5, #4]
	str	r3, [r5, #0]
	ldr	r3, [r1, #16]
	str	r3, [r5, #8]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #64
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	sub_08014878
	mov	r2, fp
	lsrs	r3, r2, #31
	add	r3, fp
	asrs	r3, r3, #1
	adds	r3, #32
	ands	r0, r6
	str	r3, [r5, #24]
	subs	r0, #127
	movs	r3, #1
	lsls	r0, r0, #11
	add	fp, r3
	str	r0, [r5, #20]
	mov	r0, fp
	adds	r5, #28
	cmp	r0, #64
	bne.n	.L_081634b2
.L_081634fa:
	ldr	r1, [sp, #112]
	cmp	r1, #24
	bne.n	.L_08163542
	ldr	r3, [sp, #128]
	movs	r1, #1
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #144
	lsls	r3, r3, #12
	str	r3, [sp, #0]
	movs	r3, #100
	str	r3, [sp, #4]
	movs	r2, #0
	movs	r3, #0
	bl	sub_0815f000
	ldr	r1, [sp, #128]
	movs	r3, #4
	movs	r5, #36
	ldrsh	r0, [r1, r5]
	movs	r2, #5
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	bl	sub_0814cd48
	movs	r0, #134
	bl	sub_081180e8
	movs	r5, #238
	ldr	r3, [sp, #120]
	lsls	r5, r5, #7
	adds	r5, #168
	adds	r2, r3, r5
	movs	r3, #8
	str	r3, [r2, #0]
.L_08163542:
	ldr	r2, [sp, #112]
	subs	r2, #24
	cmp	r2, #11
	bhi.n	sub_08163580
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	lsls	r1, r3, #4
	ldr	r0, [sp, #120]
	subs	r1, r1, r3
	ldr	r3, [sp, #12]
	lsls	r1, r1, #6
	movs	r2, #176
	adds	r1, r0, r1
	lsls	r2, r2, #7
	adds	r1, r1, r2
	ldr	r2, [r3, #0]
	ldr	r5, [sp, #84]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	movs	r3, #20
	str	r3, [sp, #0]
	movs	r3, #48
	str	r3, [sp, #4]
	asrs	r2, r2, #1
	subs	r2, #8
	ldr	r4, [r5, #4]
	ldr	r0, [sp, #116]
	movs	r3, #28
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x981f
	cmp	r0, #66
	bne.n	.L_081635c4
	movs	r1, #0
	mov	fp, r1
	movs	r6, #2
	movs	r5, #8
.L_0816358e:
	ldr	r2, [sp, #112]
	cmp	r2, r5
	bne.n	.L_081635b8
	ldr	r1, [sp, #128]
	movs	r2, #5
	movs	r3, #36
	ldrsh	r0, [r1, r3]
	movs	r3, #0
	movs	r1, #7
	str	r6, [sp, #0]
	bl	sub_0814cd48
	movs	r0, #238
	ldr	r2, [sp, #120]
	lsls	r0, r0, #7
	adds	r0, #168
	adds	r3, r2, r0
	str	r6, [r3, #0]
	movs	r0, #133
	bl	sub_081c0010
.L_081635b8:
	movs	r1, #1
	add	fp, r1
	mov	r2, fp
	adds	r5, #4
	cmp	r2, #4
	bne.n	.L_0816358e
.L_081635c4:
	ldr	r3, [sp, #124]
	cmp	r3, #37
	bgt.n	.L_0816361a
	ldr	r5, [sp, #112]
	cmp	r5, #5
	bgt.n	.L_0816361a
	add	r5, sp, #192
	adds	r1, r5, #0
	ldr	r0, [sp, #16]
	bl	sub_0815e1ec
	ldr	r2, [r5, #0]
	movs	r0, #20
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	str	r2, [r5, #0]
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	movs	r0, #40
	str	r0, [sp, #4]
	ldr	r0, [sp, #84]
	ldr	r1, [pc, #316]
	subs	r2, #10
	subs	r3, #4
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #116]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9904
	ldr	r5, [sp, #56]
	ldr	r3, [r1, #0]
	ldr	r2, [r5, #0]
	adds	r3, r3, r2
	str	r3, [r1, #0]
	ldr	r3, [r1, #4]
	ldr	r2, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r1, #4]
	ldr	r3, [r1, #8]
	ldr	r2, [r5, #8]
	adds	r3, r3, r2
	str	r3, [r1, #8]
.L_0816361a:
	ldr	r0, [sp, #124]
	cmp	r0, #8
	beq.n	.L_0816362e
	ldr	r1, [sp, #112]
	cmp	r1, #3
	bne.n	.L_0816362e
	movs	r0, #1
	negs	r0, r0
	bl	sub_081180e8
.L_0816362e:
	ldr	r2, [sp, #112]
	cmp	r2, #4
	bne.n	.L_0816365e
	ldr	r3, [sp, #124]
	cmp	r3, #91
	bne.n	.L_08163642
	movs	r0, #145
	bl	sub_081c0010
	b.n	.L_0816365e
.L_08163642:
	ldr	r5, [sp, #124]
	cmp	r5, #78
	beq.n	.L_08163650
	cmp	r5, #81
	beq.n	.L_08163650
	cmp	r5, #98
	bne.n	.L_08163658
.L_08163650:
	movs	r0, #144
	bl	sub_081c0010
	b.n	.L_0816365e
.L_08163658:
	movs	r0, #134
	bl	sub_081c0010
.L_0816365e:
	ldr	r0, [sp, #112]
	cmp	r0, #6
	beq.n	.L_08163666
	b.n	.L_0816378a
.L_08163666:
	ldr	r1, [sp, #48]
	cmp	r1, #1
	bls.n	.L_081636ae
	ldr	r2, [sp, #124]
	cmp	r2, #7
	beq.n	.L_081636ae
	cmp	r2, #43
	beq.n	.L_081636ae
	cmp	r2, #48
	beq.n	.L_081636ae
	cmp	r2, #73
	beq.n	.L_081636ae
	cmp	r2, #77
	beq.n	.L_081636ae
	cmp	r2, #78
	beq.n	.L_081636ae
	cmp	r2, #49
	beq.n	.L_081636ae
	cmp	r2, #12
	beq.n	.L_081636ae
	cmp	r2, #53
	beq.n	.L_081636ae
	cmp	r2, #64
	beq.n	.L_081636ae
	cmp	r2, #100
	beq.n	.L_081636ae
	cmp	r2, #66
	beq.n	.L_081636ae
	cmp	r2, #81
	beq.n	.L_081636ae
	cmp	r2, #83
	beq.n	.L_081636ae
	cmp	r2, #76
	beq.n	.L_081636ae
	cmp	r2, #98
	bne.n	.L_081636c6
.L_081636ae:
	ldr	r5, [sp, #128]
	movs	r1, #4
	movs	r3, #36
	ldrsh	r0, [r5, r3]
	bl	sub_08118088
	movs	r1, #238
	ldr	r0, [sp, #120]
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r2, r0, r1
	b.n	.L_0816376c
.L_081636c6:
	ldr	r2, [sp, #124]
	cmp	r2, #8
	beq.n	.L_081636d4
	cmp	r2, #79
	beq.n	.L_081636d4
	cmp	r2, #85
	bne.n	.L_081636e2
.L_081636d4:
	ldr	r3, [sp, #120]
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #168
	adds	r2, r3, r5
	movs	r3, #8
	b.n	.L_0816376e
.L_081636e2:
	ldr	r0, [sp, #124]
	cmp	r0, #50
	beq.n	.L_081636f0
	cmp	r0, #44
	beq.n	.L_081636f0
	cmp	r0, #63
	bne.n	.L_0816370a
.L_081636f0:
	ldr	r2, [sp, #128]
	movs	r5, #238
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r1, #1
	bl	sub_08118088
	ldr	r3, [sp, #120]
	lsls	r5, r5, #7
	adds	r5, #168
	adds	r2, r3, r5
	movs	r3, #2
	b.n	.L_0816376e
.L_0816370a:
	ldr	r0, [sp, #124]
	cmp	r0, #60
	beq.n	.L_08163714
	cmp	r0, #38
	bne.n	.L_08163734
.L_08163714:
	ldr	r2, [sp, #128]
	movs	r5, #238
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r1, #3
	bl	sub_08118088
	ldr	r3, [sp, #120]
	lsls	r5, r5, #7
	adds	r5, #168
	adds	r2, r3, r5
	movs	r3, #16
	b.n	.L_0816376e
	movs	r0, r0
	.2byte 0x3c56
	.2byte 0x0201
.L_08163734:
	ldr	r0, [sp, #124]
	cmp	r0, #95
	beq.n	.L_08163756
	cmp	r0, #99
	beq.n	.L_08163756
	cmp	r0, #96
	beq.n	.L_08163756
	cmp	r0, #97
	beq.n	.L_08163756
	cmp	r0, #92
	beq.n	.L_08163756
	cmp	r0, #93
	beq.n	.L_08163756
	cmp	r0, #86
	beq.n	.L_08163756
	cmp	r0, #74
	bne.n	.L_08163770
.L_08163756:
	ldr	r2, [sp, #128]
	movs	r5, #238
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r1, #0
	bl	sub_08118088
	ldr	r3, [sp, #120]
	lsls	r5, r5, #7
	adds	r5, #168
	adds	r2, r3, r5
.L_0816376c:
	movs	r3, #12
.L_0816376e:
	str	r3, [r2, #0]
.L_08163770:
	ldr	r0, [sp, #112]
	cmp	r0, #6
	bne.n	.L_0816378a
	ldr	r2, [sp, #128]
	movs	r3, #4
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
.L_0816378a:
	ldr	r3, [sp, #112]
	cmp	r3, #14
	bne.n	.L_081637a4
	ldr	r1, [sp, #128]
	movs	r3, #4
	movs	r5, #36
	ldrsh	r0, [r1, r5]
	movs	r2, #5
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	bl	sub_0814cd48
.L_081637a4:
	movs	r1, #8
	movs	r0, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r5, #240
	ldr	r3, [sp, #120]
	lsls	r5, r5, #7
	adds	r5, #232
	adds	r2, r3, r5
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	ldr	r0, [sp, #112]
	ldr	r1, [sp, #92]
	adds	r0, #1
	str	r0, [sp, #112]
	cmp	r0, r1
	beq.n	.L_081637d4
	bl	sub_0816031e
.L_081637d4:
	ldr	r2, [sp, #124]
	cmp	r2, #51
	beq.n	.L_081637f6
	cmp	r2, #67
	beq.n	.L_081637f6
	cmp	r2, #9
	beq.n	.L_081637f6
	cmp	r2, #10
	beq.n	.L_081637f6
	cmp	r2, #11
	beq.n	.L_081637f6
	cmp	r2, #70
	beq.n	.L_081637f6
	cmp	r2, #79
	beq.n	.L_081637f6
	cmp	r2, #90
	bne.n	.L_08163868
.L_081637f6:
	movs	r1, #240
	ldr	r5, [pc, #192]
	lsls	r1, r1, #6
	ldr	r0, [pc, #192]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x21f0
	ldr	r0, [sp, #116]
	lsls	r1, r1, #6
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9d20
	movs	r3, #0
	str	r3, [r5, #28]
	ldr	r0, [pc, #176]
	bl	sub_08014644
	ldr	r0, [pc, #172]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [sp, #124]
	cmp	r0, #51
	beq.n	.L_08163840
	cmp	r0, #10
	beq.n	.L_08163840
	cmp	r0, #70
	beq.n	.L_08163840
	cmp	r0, #79
	beq.n	.L_08163840
	cmp	r0, #90
	bne.n	.L_08163848
.L_08163840:
	ldr	r0, [sp, #128]
	bl	sub_081504c0
	b.n	.L_081638ac
.L_08163848:
	ldr	r1, [sp, #124]
	cmp	r1, #9
	beq.n	.L_08163852
	cmp	r1, #67
	bne.n	.L_0816385a
.L_08163852:
	ldr	r0, [sp, #128]
	bl	sub_081504cc
	b.n	.L_081638ac
.L_0816385a:
	ldr	r2, [sp, #124]
	cmp	r2, #11
	bne.n	.L_081638ac
	ldr	r0, [sp, #128]
	bl	sub_081504b4
	b.n	.L_081638ac
.L_08163868:
	ldr	r3, [sp, #52]
	cmp	r3, #1
	bls.n	.L_08163890
	ldr	r5, [sp, #124]
	cmp	r5, #42
	beq.n	.L_08163890
	cmp	r5, #15
	beq.n	.L_08163890
	cmp	r5, #52
	beq.n	.L_08163890
	cmp	r5, #72
	beq.n	.L_08163890
	cmp	r5, #58
	beq.n	.L_08163890
	cmp	r5, #59
	beq.n	.L_08163890
	cmp	r5, #69
	beq.n	.L_08163890
	cmp	r5, #89
	bne.n	.L_08163896
.L_08163890:
	ldr	r0, [pc, #56]
	bl	sub_08014644
.L_08163896:
	ldr	r0, [pc, #48]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
.L_081638ac:
	add	sp, #264
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03000258
	.4byte 0x06004000
	.4byte 0x08143489
	.4byte 0x08143001
	.4byte 0x08152475
