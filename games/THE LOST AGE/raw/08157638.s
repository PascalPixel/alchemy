.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0800206c, 0x0800206c
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015128, 0x08015128
	.set sub_080151ac, 0x080151ac
	.set sub_080156e8, 0x080156e8
	.set sub_08015768, 0x08015768
	.set sub_0801587c, 0x0801587c
	.set sub_08020048, 0x08020048
	.set sub_08118040, 0x08118040
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138058, 0x08138058
	.set sub_08138086, 0x08138086
	.set sub_0813ba50, 0x0813ba50
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_0814cc4c, 0x0814cc4c
	.set sub_0814cca8, 0x0814cca8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08152404, 0x08152404
	.set sub_0815585c, 0x0815585c
	.set sub_081575ec, 0x081575ec
	.set sub_08157b0e, 0x08157b0e
	.set sub_08157d18, 0x08157d18
	.set sub_08157e66, 0x08157e66
	.set sub_08158206, 0x08158206
	.set sub_08158220, 0x08158220
	.set sub_0815826a, 0x0815826a
	.set sub_08158640, 0x08158640
	.set sub_081589ae, 0x081589ae
	.set sub_081589d0, 0x081589d0
	.set sub_0815981a, 0x0815981a
	.set sub_08159ae0, 0x08159ae0
	.set sub_08159f56, 0x08159f56
	.set sub_0815a110, 0x0815a110
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e20c, 0x0815e20c
	.set sub_0815e21c, 0x0815e21c
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_08163c2c, 0x08163c2c
	.set sub_0816442c, 0x0816442c
	.set sub_0816467c, 0x0816467c
	.set sub_0818caa8, 0x0818caa8
	.set sub_081963ec, 0x081963ec
	.set sub_08196404, 0x08196404
	.set sub_081c0010, 0x081c0010
	.global Overlay_08157638
Overlay_08157638:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #96]
	sub	sp, #40
	mov	fp, r0
	ldr	r0, [r3, #92]
	str	r1, [sp, #28]
	mov	r9, r0
	ldr	r3, [r3, #100]
	movs	r0, #0
	str	r3, [sp, #20]
	bl	sub_081435e0
	mov	r2, sp
	adds	r2, #32
	adds	r1, r2, #0
	movs	r0, #0
	str	r2, [sp, #16]
	bl	sub_08144aac
	ldr	r0, [pc, #832]
	ldr	r1, [sp, #20]
	movs	r2, #0
	movs	r3, #0
	bl	.L_08157cf4
	ldr	r5, [pc, #824]
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #820]
	add	r1, r9
	movs	r2, #1
	movs	r3, #0
	bl	.L_08157cf4
	adds	r1, r5, #0
	ldr	r0, [pc, #812]
	movs	r2, #1
	movs	r3, #1
	bl	.L_08157cf4
	movs	r1, #144
	lsls	r1, r1, #4
	adds	r1, #168
	adds	r0, r5, #0
	add	r1, r9
	movs	r2, #17
	movs	r3, #104
	bl	sub_081575ec
	movs	r3, #221
	movs	r1, #128
	lsls	r3, r3, #3
	lsls	r1, r1, #5
	adds	r5, r5, r3
	adds	r1, #144
	add	r1, r9
	adds	r0, r5, #0
	movs	r2, #34
	movs	r3, #65
	bl	sub_081575ec
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, r9
	movs	r3, #75
	movs	r1, #200
	str	r3, [r2, #0]
	ldr	r0, [pc, #740]
	lsls	r1, r1, #4
	bl	sub_080145a8
	movs	r7, #0
	movs	r2, #1
	mov	r3, r9
	mov	sl, r7
	negs	r2, r2
	adds	r3, #24
.L_081576ee:
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r1, #8
	bne.n	.L_081576ee
	movs	r2, #0
	ldr	r3, [pc, #708]
	mov	sl, r2
	movs	r1, #1
	movs	r2, #128
	negs	r1, r1
	lsls	r2, r2, #2
.L_0815770a:
	movs	r7, #1
	add	sl, r7
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	sl, r2
	bne.n	.L_0815770a
	movs	r0, #162
	bl	sub_081c0010
	movs	r0, #0
	str	r0, [sp, #24]
.L_08157720:
	ldr	r1, [sp, #24]
	cmp	r1, #56
	bne.n	.L_0815772c
	movs	r0, #133
	bl	sub_081180e8
.L_0815772c:
	movs	r2, #0
	str	r2, [sp, #12]
	mov	sl, r2
	mov	r8, r9
.L_08157734:
	mov	r7, r8
	ldr	r3, [r7, #24]
	movs	r0, #1
	negs	r0, r0
	cmp	r3, r0
	beq.n	.L_081577ec
	movs	r1, #65
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	str	r1, [sp, #0]
	movs	r1, #34
	str	r1, [sp, #4]
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r1, #144
	subs	r3, #17
	subs	r2, #16
	ldr	r4, [sp, #32]
	ldr	r0, [sp, #28]
	add	r1, r9
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x683b
	subs	r3, #12
	str	r3, [r7, #0]
	ldr	r3, [r7, #24]
	adds	r3, #1
	str	r3, [r7, #24]
	cmp	r3, #5
	bne.n	.L_081577ec
	movs	r0, #133
	bl	sub_081c0010
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, r9
	movs	r3, #4
	str	r3, [r2, #0]
	ldr	r1, [sp, #12]
	ldr	r2, [pc, #556]
	movs	r4, #0
	adds	r7, r1, r2
.L_0815778a:
	str	r4, [sp, #8]
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08014878
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r5, #255
	movs	r3, #128
	ands	r5, r0
	lsls	r3, r3, #1
	mov	r0, r8
	adds	r5, r5, r3
	ldr	r3, [r0, #0]
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	ldr	r3, [r0, #4]
	adds	r0, r6, #0
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #8
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #16]
	bl	sub_08014878
	ldr	r4, [sp, #8]
	movs	r3, #15
	ands	r3, r0
	adds	r3, #32
	adds	r4, #1
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r4, #32
	bne.n	.L_0815778a
.L_081577ec:
	ldr	r2, [sp, #12]
	movs	r3, #224
	movs	r7, #1
	lsls	r3, r3, #2
	add	sl, r7
	movs	r1, #28
	adds	r2, r2, r3
	mov	r0, sl
	add	r8, r1
	str	r2, [sp, #12]
	cmp	r0, #5
	bne.n	.L_08157734
	ldr	r1, [sp, #24]
	cmp	r1, #95
	bgt.n	.L_081578fa
	lsls	r6, r1, #11
	adds	r0, r6, #0
	bl	sub_08002096
	ldr	r2, [sp, #24]
	movs	r5, #64
	lsls	r3, r2, #1
	subs	r5, r5, r3
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r7, r3, #17
	movs	r3, #96
	adds	r3, r3, r7
	adds	r0, r6, #0
	mov	r8, r3
	bl	sub_08002090
	movs	r1, #20
	str	r1, [sp, #0]
	adds	r3, r5, #0
	muls	r3, r0
	movs	r1, #34
	str	r1, [sp, #4]
	ldr	r0, [sp, #16]
	movs	r1, #224
	asrs	r3, r3, #16
	adds	r2, r7, #0
	lsls	r1, r1, #3
	adds	r6, r3, #0
	adds	r2, #86
	adds	r3, #43
	ldr	r4, [r0, #4]
	add	r1, r9
	ldr	r0, [sp, #28]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2100
	mov	sl, r1
	ldr	r1, [pc, #368]
	mov	r2, sl
	ldrb	r3, [r1, r2]
	ldr	r0, [sp, #24]
	adds	r6, #60
	cmp	r0, r3
	bne.n	.L_0815787c
	mov	r2, r9
	ldr	r3, [r2, #24]
	movs	r0, #1
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_0815787c
	adds	r3, r7, #0
	adds	r3, #88
	str	r3, [r2, #0]
	mov	r1, r8
	mov	r3, sl
	b.n	.L_081578f4
.L_0815787c:
	mov	r7, sl
	ldrb	r3, [r1, r7]
	ldr	r0, [sp, #24]
	adds	r3, #6
	cmp	r0, r3
	bne.n	.L_081578be
	mov	r1, fp
	ldr	r3, [r1, #20]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_081578be
	movs	r5, #36
.L_08157894:
	mov	r2, fp
	ldrsh	r0, [r5, r2]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #7
	adds	r3, r4, #0
	movs	r2, #5
	mov	r7, fp
	str	r4, [sp, #8]
	bl	sub_0814cd48
	ldrsh	r0, [r5, r7]
	movs	r1, #6
	bl	sub_08118088
	ldr	r4, [sp, #8]
	ldr	r3, [r7, #20]
	adds	r4, #1
	adds	r5, #2
	cmp	r4, r3
	bne.n	.L_08157894
.L_081578be:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #5
	beq.n	.L_081578fa
	ldr	r3, [pc, #252]
	mov	r7, sl
	adds	r1, r3, #0
	ldrb	r3, [r1, r7]
	ldr	r0, [sp, #24]
	cmp	r0, r3
	bne.n	.L_0815787c
	lsls	r3, r7, #3
	subs	r3, r3, r7
	lsls	r3, r3, #2
	mov	r7, r9
	adds	r2, r7, r3
	ldr	r3, [r2, #24]
	movs	r0, #1
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_0815787c
	mov	r3, r8
	subs	r3, #8
	str	r3, [r2, #0]
	mov	r1, r8
	movs	r3, #0
.L_081578f4:
	str	r1, [r2, #12]
	str	r6, [r2, #4]
	str	r3, [r2, #24]
.L_081578fa:
	ldr	r5, [pc, #184]
	ldr	r6, [pc, #204]
	movs	r2, #0
	mov	sl, r2
.L_08157902:
	ldr	r0, [r5, #24]
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_08157952
	cmp	r0, #0
	bge.n	.L_08157912
	adds	r0, #15
.L_08157912:
	asrs	r0, r0, #4
	adds	r0, #2
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r7, [sp, #20]
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	adds	r1, r7, r1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #16]
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #28]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r5, #0
	movs	r1, #62
	lsls	r2, r2, #6
	bl	sub_08138086
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_08157952:
	movs	r1, #1
	movs	r2, #128
	add	sl, r1
	lsls	r2, r2, #1
	adds	r5, #28
	cmp	sl, r2
	bne.n	.L_08157902
	movs	r0, #4
	movs	r1, #4
	bl	.L_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [sp, #24]
	adds	r3, #1
	str	r3, [sp, #24]
	cmp	r3, #96
	beq.n	.L_0815798a
	b.n	.L_08157720
.L_0815798a:
	ldr	r0, [pc, #52]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00000134
	.4byte 0x02010000
	.4byte 0x00000155
	.4byte 0x00000130
	.4byte 0x08143001
	.4byte 0x02010018
	.4byte 0x0819850e
	.2byte 0x7410
	.2byte 0x0819
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #96]
	sub	sp, #44
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
	strh	r3, [r2, #0]
	mov	r2, sp
	adds	r2, #36
	adds	r1, r2, #0
	movs	r0, #0
	str	r2, [sp, #20]
	bl	sub_08144aac
	ldr	r0, [pc, #40]
	ldr	r1, [sp, #24]
	movs	r2, #0
	movs	r3, #0
	bl	.L_08157cf4
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #28]
	add	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	.L_08157cf4
	movs	r1, #162
	lsls	r1, r1, #4
	ldr	r0, [pc, #16]
	add	r1, r9
	b.n	.L_08157a48
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x00000134
	.4byte 0x00000153
	.2byte 0x014c
	.2byte 0x0000
.L_08157a48:
	movs	r2, #1
	movs	r3, #0
	bl	.L_08157cf4
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	movs	r3, #75
	add	r2, r9
	movs	r1, #200
	str	r3, [r2, #0]
	ldr	r0, [pc, #628]
	lsls	r1, r1, #4
	bl	sub_080145a8
	movs	r3, #0
	mov	sl, r3
	movs	r6, #63
	mov	r5, r9
.L_08157a78:
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #64
	str	r0, [r5, #0]
	bl	sub_08014878
	movs	r7, #1
	ands	r0, r6
	subs	r0, #80
	add	sl, r7
	str	r0, [r5, #4]
	mov	r0, sl
	adds	r5, #28
	cmp	r0, #32
	bne.n	.L_08157a78
	ldr	r3, [pc, #584]
	movs	r1, #0
	movs	r2, #128
	mov	sl, r1
	lsls	r2, r2, #2
	subs	r1, #1
.L_08157aa4:
	movs	r7, #1
	add	sl, r7
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	sl, r2
	bne.n	.L_08157aa4
	movs	r0, #171
	bl	sub_081c0010
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #8
	str	r0, [sp, #28]
	str	r1, [sp, #16]
.L_08157ac0:
	ldr	r2, [sp, #28]
	cmp	r2, #56
	bne.n	.L_08157acc
	movs	r0, #133
	bl	sub_081180e8
.L_08157acc:
	ldr	r3, [sp, #28]
	cmp	r3, #95
	bgt.n	sub_08157b0e
	ldr	r0, [sp, #16]
	bl	sub_08002096
	ldr	r7, [sp, #28]
	movs	r5, #64
	lsls	r3, r7, #1
	subs	r5, r5, r3
	adds	r6, r5, #0
	muls	r6, r0
	ldr	r0, [sp, #16]
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	movs	r2, #20
	asrs	r6, r6, #17
	movs	r1, #224
	asrs	r3, r3, #16
	adds	r6, #86
	str	r2, [sp, #0]
	lsls	r1, r1, #3
	movs	r2, #40
	str	r2, [sp, #4]
	adds	r3, #28
	ldr	r4, [sp, #36]
	ldr	r0, [sp, #32]
	add	r1, r9
	adds	r2, r6, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2000
	str	r0, [sp, #12]
	mov	sl, r0
	mov	r8, r9
.L_08157b16:
	mov	r1, sl
	ldr	r2, [sp, #28]
	lsls	r3, r1, #2
	adds	r3, #8
	cmp	r2, r3
	blt.n	.L_08157c08
	mov	r7, r8
	ldr	r3, [r7, #4]
	cmp	r3, #95
	bgt.n	.L_08157c08
	movs	r1, #40
	ldr	r2, [r7, #0]
	str	r1, [sp, #0]
	movs	r1, #64
	str	r1, [sp, #4]
	movs	r1, #162
	lsls	r1, r1, #4
	subs	r3, #32
	subs	r2, #20
	ldr	r4, [sp, #36]
	ldr	r0, [sp, #32]
	add	r1, r9
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x683b
	subs	r3, #6
	str	r3, [r7, #0]
	ldr	r3, [r7, #4]
	adds	r3, #12
	str	r3, [r7, #4]
	cmp	r3, #95
	ble.n	.L_08157c08
	ldr	r0, [sp, #12]
	ldr	r1, [pc, #396]
	movs	r4, #0
	adds	r7, r0, r1
.L_08157b5e:
	str	r4, [sp, #8]
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08014878
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r5, #255
	ands	r5, r0
	mov	r0, r8
	ldr	r3, [r0, #0]
	movs	r2, #128
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	lsls	r2, r2, #1
	ldr	r3, [r0, #4]
	adds	r0, r6, #0
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	adds	r5, r5, r2
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08014878
	ldr	r4, [sp, #8]
	movs	r3, #15
	ands	r3, r0
	adds	r3, #32
	adds	r4, #1
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r4, #32
	bne.n	.L_08157b5e
	movs	r0, #133
	bl	sub_081c0010
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	add	r3, r9
	movs	r2, #4
	str	r2, [r3, #0]
	mov	r1, fp
	ldr	r3, [r1, #20]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_08157c08
	movs	r5, #36
.L_08157bde:
	mov	r2, fp
	ldrsh	r0, [r5, r2]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #7
	adds	r3, r4, #0
	movs	r2, #5
	mov	r7, fp
	str	r4, [sp, #8]
	bl	sub_0814cd48
	ldrsh	r0, [r5, r7]
	movs	r1, #6
	bl	sub_08118088
	ldr	r4, [sp, #8]
	ldr	r3, [r7, #20]
	adds	r4, #1
	adds	r5, #2
	cmp	r4, r3
	bne.n	.L_08157bde
.L_08157c08:
	ldr	r3, [sp, #12]
	movs	r7, #224
	movs	r0, #1
	lsls	r7, r7, #2
	add	sl, r0
	movs	r2, #28
	adds	r3, r3, r7
	mov	r1, sl
	add	r8, r2
	str	r3, [sp, #12]
	cmp	r1, #8
	beq.n	.L_08157c22
	b.n	.L_08157b16
.L_08157c22:
	ldr	r5, [pc, #196]
	ldr	r6, [pc, #196]
	movs	r2, #0
	mov	sl, r2
.L_08157c2a:
	ldr	r0, [r5, #24]
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_08157c7a
	cmp	r0, #0
	bge.n	.L_08157c3a
	adds	r0, #15
.L_08157c3a:
	asrs	r0, r0, #4
	adds	r0, #1
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r7, [sp, #24]
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	adds	r1, r7, r1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #20]
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r5, #0
	movs	r1, #62
	lsls	r2, r2, #6
	bl	sub_08138086
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_08157c7a:
	movs	r1, #1
	movs	r2, #128
	add	sl, r1
	lsls	r2, r2, #2
	adds	r5, #28
	cmp	sl, r2
	bne.n	.L_08157c2a
	movs	r0, #4
	movs	r1, #4
	bl	.L_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r7, [pc, #72]
	ldr	r3, [sp, #16]
	ldr	r0, [sp, #28]
	adds	r3, r3, r7
	adds	r0, #1
	str	r3, [sp, #16]
	str	r0, [sp, #28]
	cmp	r0, #96
	beq.n	.L_08157cba
	b.n	.L_08157ac0
.L_08157cba:
	ldr	r0, [pc, #36]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x08143001
	.4byte 0x02010018
	.4byte 0x02010000
	.4byte 0x08197410
	.2byte 0xf800
	.2byte 0xffff
.L_08157cf4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r1
	adds	r7, r2, #0
	adds	r5, r3, #0
	bl	sub_08013300
	adds	r6, r0, #0
	cmp	r5, #0
	beq.n	sub_08157d18
	movs	r0, #160
	ldr	r3, [pc, #28]
	lsls	r0, r0, #19
	adds	r1, r6, #0
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2f00
	beq.n	.L_08157d1e
	adds	r6, #128
.L_08157d1e:
	adds	r0, r6, #0
	mov	r1, r8
	bl	sub_0801587c
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0730
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r1, [r5, #96]
	sub	sp, #44
	mov	fp, r0
	ldr	r0, [r5, #92]
	str	r1, [sp, #40]
	mov	r9, r0
	ldr	r2, [r5, #100]
	movs	r0, #0
	str	r2, [sp, #24]
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r1, #27
	movs	r0, #104
	bl	sub_081963ec
	movs	r1, #3
	movs	r0, #188
	bl	sub_081963ec
	ldr	r3, [r5, #104]
	adds	r5, #188
	ldr	r5, [r5, #0]
	str	r3, [sp, #28]
	ldr	r1, [sp, #24]
	ldr	r0, [pc, #28]
	movs	r2, #0
	movs	r3, #0
	str	r5, [sp, #32]
	bl	.L_08157cf4
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	add	r1, r9
	movs	r2, #1
	movs	r3, #1
	b.n	.L_08157da4
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x00000134
	.2byte 0x0157
	.2byte 0x0000
.L_08157da4:
	bl	.L_08157cf4
	movs	r1, #144
	lsls	r1, r1, #4
	adds	r1, #248
	ldr	r0, [pc, #676]
	add	r1, r9
	movs	r2, #1
	movs	r3, #0
	bl	.L_08157cf4
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, r9
	movs	r3, #75
	movs	r1, #200
	str	r3, [r2, #0]
	ldr	r0, [pc, #644]
	lsls	r1, r1, #4
	bl	sub_080145a8
	movs	r4, #0
	mov	sl, r4
	movs	r7, #63
	mov	r5, r9
	movs	r6, #104
.L_08157de4:
	bl	sub_08014878
	ands	r0, r7
	str	r0, [r5, #0]
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	str	r6, [r5, #4]
	adds	r5, #28
	cmp	r1, #32
	bne.n	.L_08157de4
	movs	r2, #0
	ldr	r3, [pc, #604]
	mov	sl, r2
	movs	r1, #1
	movs	r2, #128
	negs	r1, r1
	lsls	r2, r2, #2
.L_08157e08:
	movs	r4, #1
	add	sl, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	sl, r2
	bne.n	.L_08157e08
	movs	r0, #141
	bl	sub_081c0010
	movs	r0, #128
	movs	r7, #0
	lsls	r0, r0, #8
	str	r7, [sp, #36]
	str	r0, [sp, #16]
.L_08157e24:
	ldr	r1, [sp, #36]
	cmp	r1, #79
	bgt.n	sub_08157e66
	ldr	r0, [sp, #16]
	bl	sub_08002096
	lsls	r5, r0, #1
	adds	r5, r5, r0
	ldr	r0, [sp, #16]
	bl	sub_08002090
	ldr	r3, [sp, #36]
	lsls	r5, r5, #3
	lsls	r2, r3, #1
	movs	r3, #64
	subs	r3, r3, r2
	muls	r3, r0
	movs	r2, #20
	asrs	r5, r5, #16
	movs	r1, #224
	asrs	r3, r3, #16
	adds	r5, #22
	str	r2, [sp, #0]
	lsls	r1, r1, #3
	movs	r2, #38
	str	r2, [sp, #4]
	adds	r3, #29
	ldr	r0, [sp, #40]
	add	r1, r9
	adds	r2, r5, #0
	ldr	r4, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9f09
	cmp	r7, #56
	bne.n	.L_08157e72
	movs	r0, #133
	bl	sub_081180e8
.L_08157e72:
	movs	r0, #0
	movs	r1, #16
	str	r1, [sp, #20]
	str	r0, [sp, #12]
	mov	sl, r0
	mov	r8, r9
.L_08157e7e:
	ldr	r2, [sp, #36]
	ldr	r3, [sp, #20]
	cmp	r2, r3
	blt.n	.L_08157f7a
	mov	r4, r8
	movs	r1, #34
	ldr	r2, [r4, #0]
	ldr	r3, [r4, #4]
	str	r1, [sp, #0]
	movs	r1, #65
	str	r1, [sp, #4]
	movs	r1, #135
	lsls	r1, r1, #5
	add	r1, r9
	subs	r2, #17
	subs	r3, #32
	ldr	r0, [sp, #40]
	ldr	r7, [sp, #28]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x9809
	ldr	r1, [sp, #20]
	cmp	r0, r1
	bne.n	.L_08157f72
	ldr	r2, [sp, #12]
	ldr	r3, [pc, #428]
	movs	r4, #0
	adds	r7, r2, r3
.L_08157eb6:
	str	r4, [sp, #8]
	bl	sub_08014878
	movs	r6, #254
	lsls	r6, r6, #7
	adds	r6, #255
	ands	r6, r0
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r6, r6, r0
	bl	sub_08014878
	mov	r2, r8
	ldr	r3, [r2, #0]
	movs	r5, #128
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	lsls	r5, r5, #1
	ldr	r3, [r2, #4]
	adds	r5, #255
	adds	r3, #16
	lsls	r3, r3, #16
	movs	r1, #128
	lsls	r1, r1, #1
	str	r3, [r7, #4]
	ands	r5, r0
	adds	r0, r6, #0
	adds	r5, r5, r1
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08014878
	ldr	r4, [sp, #8]
	movs	r3, #15
	ands	r3, r0
	adds	r3, #32
	adds	r4, #1
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r4, #16
	bne.n	.L_08157eb6
	movs	r3, #1
	mov	r4, sl
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08157f2e
	movs	r0, #133
	bl	sub_081c0010
.L_08157f2e:
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	add	r3, r9
	movs	r2, #4
	str	r2, [r3, #0]
	mov	r7, fp
	ldr	r3, [r7, #20]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_08157f72
	movs	r5, #36
.L_08157f46:
	mov	r1, fp
	movs	r3, #6
	ldrsh	r0, [r5, r1]
	str	r3, [sp, #0]
	movs	r1, #7
	adds	r3, r4, #0
	movs	r2, #5
	str	r4, [sp, #8]
	bl	sub_0814cd48
	mov	r3, fp
	ldrsh	r0, [r5, r3]
	movs	r1, #6
	bl	sub_08118088
	mov	r0, fp
	ldr	r4, [sp, #8]
	ldr	r3, [r0, #20]
	adds	r4, #1
	adds	r5, #2
	cmp	r4, r3
	bne.n	.L_08157f46
.L_08157f72:
	mov	r1, r8
	ldr	r3, [r1, #4]
	subs	r3, #12
	str	r3, [r1, #4]
.L_08157f7a:
	ldr	r2, [sp, #20]
	ldr	r4, [sp, #12]
	movs	r7, #224
	movs	r0, #1
	lsls	r7, r7, #2
	add	sl, r0
	adds	r2, #4
	movs	r3, #28
	adds	r4, r4, r7
	mov	r1, sl
	str	r2, [sp, #20]
	add	r8, r3
	str	r4, [sp, #12]
	cmp	r1, #10
	beq.n	.L_08157f9a
	b.n	.L_08157e7e
.L_08157f9a:
	ldr	r5, [pc, #196]
	ldr	r6, [pc, #196]
	movs	r2, #0
	mov	sl, r2
.L_08157fa2:
	ldr	r0, [r5, #24]
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_08157ff0
	cmp	r0, #0
	bge.n	.L_08157fb2
	adds	r0, #15
.L_08157fb2:
	asrs	r0, r0, #4
	adds	r0, #2
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r7, [sp, #24]
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	adds	r1, r7, r1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #40]
	ldr	r4, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r5, #0
	movs	r1, #62
	lsls	r2, r2, #6
	bl	sub_08138086
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_08157ff0:
	movs	r7, #1
	movs	r0, #128
	add	sl, r7
	lsls	r0, r0, #2
	adds	r5, #28
	cmp	sl, r0
	bne.n	.L_08157fa2
	movs	r1, #4
	movs	r0, #4
	bl	.L_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, r9
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [pc, #72]
	ldr	r1, [sp, #16]
	ldr	r3, [sp, #36]
	adds	r1, r1, r2
	adds	r3, #1
	str	r1, [sp, #16]
	str	r3, [sp, #36]
	cmp	r3, #96
	beq.n	.L_08158030
	b.n	.L_08157e24
.L_08158030:
	ldr	r0, [pc, #36]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00000130
	.4byte 0x08143001
	.4byte 0x02010018
	.4byte 0x02010000
	.4byte 0x08197410
	.2byte 0xf800
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #36
	str	r0, [sp, #24]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #96]
	ldr	r0, [r3, #92]
	str	r1, [sp, #20]
	mov	fp, r0
	ldr	r3, [r3, #100]
	movs	r0, #0
	str	r3, [sp, #16]
	bl	sub_081435e0
	mov	r2, sp
	adds	r2, #28
	adds	r1, r2, #0
	movs	r0, #0
	str	r2, [sp, #12]
	bl	sub_08144aac
	ldr	r0, [pc, #848]
	ldr	r1, [sp, #16]
	movs	r2, #0
	movs	r3, #0
	bl	.L_08157cf4
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #836]
	add	r1, fp
	movs	r2, #1
	movs	r3, #0
	bl	.L_08157cf4
	movs	r1, #162
	lsls	r1, r1, #4
	ldr	r0, [pc, #824]
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	.L_08157cf4
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	movs	r3, #75
	add	r2, fp
	movs	r1, #200
	str	r3, [r2, #0]
	ldr	r0, [pc, #792]
	lsls	r1, r1, #4
	bl	sub_080145a8
	movs	r3, #0
	str	r3, [sp, #8]
	movs	r7, #0
	mov	r6, fp
.L_081580f6:
	lsls	r5, r7, #11
	adds	r0, r5, #0
	bl	sub_08002096
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
	asrs	r3, r3, #16
	str	r3, [r6, #0]
	adds	r0, r5, #0
	bl	sub_08002090
	lsls	r0, r0, #2
	asrs	r0, r0, #16
	movs	r3, #1
	adds	r0, #52
	ands	r3, r7
	str	r0, [r6, #4]
	cmp	r3, #0
	beq.n	.L_08158126
	ldr	r2, [r6, #0]
	movs	r3, #32
	subs	r3, r3, r2
	b.n	.L_0815812a
.L_08158126:
	ldr	r3, [r6, #0]
	adds	r3, #32
.L_0815812a:
	str	r3, [r6, #0]
	lsls	r3, r7, #1
	negs	r3, r3
	str	r3, [r6, #24]
	movs	r1, #255
	ldr	r2, [sp, #8]
	ldr	r3, [pc, #716]
	lsls	r1, r1, #8
	movs	r4, #0
	movs	r0, #127
	adds	r1, #255
	mov	r8, r4
	mov	r9, r0
	mov	sl, r1
	adds	r5, r2, r3
.L_08158148:
	bl	sub_08014878
	ldr	r2, [r6, #0]
	movs	r3, #15
	ands	r3, r0
	adds	r3, r3, r2
	subs	r3, #8
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #96
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	mov	r4, r9
	ands	r0, r4
	subs	r0, #64
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08014878
	mov	r1, r9
	ands	r0, r1
	subs	r0, #64
	lsls	r0, r0, #10
	str	r0, [r5, #16]
	bl	sub_08014878
	mov	r2, sl
	ands	r0, r2
	str	r0, [r5, #8]
	bl	sub_08014878
	mov	r3, sl
	movs	r4, #1
	ands	r0, r3
	add	r8, r4
	str	r0, [r5, #20]
	mov	r0, r8
	adds	r5, #28
	cmp	r0, #16
	bne.n	.L_08158148
	ldr	r1, [sp, #8]
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r1, r1, r2
	adds	r7, #1
	adds	r6, #28
	str	r1, [sp, #8]
	cmp	r7, #9
	bne.n	.L_081580f6
	movs	r0, #136
	bl	sub_081c0010
	movs	r4, #172
	movs	r3, #0
	negs	r4, r4
	mov	r8, r3
	mov	sl, r4
.L_081581c6:
	mov	r0, r8
	cmp	r0, #56
	bne.n	.L_081581d2
	movs	r0, #133
	bl	sub_081180e8
.L_081581d2:
	mov	r1, r8
	cmp	r1, #23
	bgt.n	sub_08158206
	mov	r3, r8
	cmp	r1, #0
	bge.n	.L_081581e0
	adds	r3, #3
.L_081581e0:
	asrs	r3, r3, #2
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #3
	adds	r1, r1, r3
	lsls	r1, r1, #6
	movs	r2, #162
	movs	r3, #40
	lsls	r2, r2, #4
	add	r1, fp
	adds	r1, r1, r2
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	ldr	r4, [sp, #28]
	ldr	r0, [sp, #20]
	movs	r2, #40
	movs	r3, #20
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4643
	cmp	r3, #20
	bne.n	sub_08158220
	ldr	r0, [pc, #488]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #496]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4643
	subs	r3, #20
	cmp	r3, #11
	bhi.n	sub_0815826a
	mov	r4, r8
	cmp	r4, #23
	ble.n	.L_08158250
	lsls	r3, r4, #2
	movs	r2, #146
	subs	r2, r2, r3
	movs	r3, #20
	str	r3, [sp, #0]
	movs	r3, #40
	str	r3, [sp, #4]
	ldr	r0, [sp, #12]
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r4, [r0, #4]
	add	r1, fp
	ldr	r0, [sp, #20]
	mov	r3, sl
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe00c
.L_08158250:
	movs	r3, #20
	movs	r1, #224
	str	r3, [sp, #0]
	lsls	r1, r1, #3
	movs	r3, #40
	str	r3, [sp, #4]
	ldr	r4, [sp, #28]
	ldr	r0, [sp, #20]
	add	r1, fp
	movs	r2, #50
	movs	r3, #20
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4641
	cmp	r1, #32
	bne.n	.L_08158292
	movs	r0, #145
	bl	sub_081c0010
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	movs	r1, #224
	add	r2, fp
	movs	r3, #8
	lsls	r1, r1, #3
	str	r3, [r2, #0]
	ldr	r0, [pc, #388]
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	.L_08157cf4
.L_08158292:
	mov	r2, r8
	cmp	r2, #31
	ble.n	.L_081582ea
	movs	r7, #0
	mov	r6, fp
.L_0815829c:
	ldr	r3, [r6, #24]
	cmp	r3, #47
	bhi.n	.L_081582de
	adds	r5, r3, #0
	cmp	r3, #0
	bge.n	.L_081582aa
	adds	r5, r3, #7
.L_081582aa:
	ldr	r2, [pc, #356]
	asrs	r5, r5, #3
	lsls	r3, r5, #1
	ldrh	r1, [r2, r3]
	movs	r3, #224
	lsls	r3, r3, #3
	add	r1, fp
	adds	r1, r1, r3
	ldr	r3, [pc, #344]
	ldr	r2, [r6, #0]
	ldrb	r4, [r3, r5]
	lsrs	r3, r4, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #336]
	ldrb	r0, [r3, r5]
	ldr	r3, [r6, #4]
	str	r4, [sp, #0]
	adds	r3, r3, r0
	ldr	r0, [pc, #332]
	ldr	r4, [sp, #28]
	ldrb	r0, [r0, r5]
	str	r0, [sp, #4]
	ldr	r0, [sp, #20]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69b3
.L_081582de:
	adds	r3, #1
	adds	r7, #1
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r7, #9
	bne.n	.L_0815829c
.L_081582ea:
	ldr	r6, [pc, #280]
	movs	r7, #0
.L_081582ee:
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_081582f6
	adds	r3, #15
.L_081582f6:
	asrs	r3, r3, #4
	lsls	r3, r3, #1
	adds	r3, #40
	cmp	r8, r3
	blt.n	.L_0815835e
	ldr	r0, [r6, #8]
	bl	sub_08002096
	movs	r5, #1
	movs	r4, #2
	ldrsh	r2, [r6, r4]
	ands	r5, r7
	lsls	r0, r0, #2
	adds	r5, #3
	asrs	r0, r0, #16
	ldr	r1, [pc, #264]
	adds	r2, r2, r0
	lsls	r0, r5, #1
	subs	r3, r0, #2
	ldrh	r1, [r1, r3]
	ldr	r3, [sp, #16]
	adds	r1, r3, r1
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	movs	r4, #6
	ldrsh	r3, [r6, r4]
	str	r0, [sp, #4]
	str	r5, [sp, #0]
	ldr	r0, [sp, #12]
	subs	r3, r3, r5
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #20]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2140
	ldr	r2, [pc, #228]
	adds	r0, r6, #0
	bl	sub_08138086
	ldr	r2, [r6, #8]
	movs	r1, #128
	movs	r4, #255
	lsls	r1, r1, #4
	lsls	r4, r4, #8
	adds	r3, r2, r1
	adds	r4, #255
	str	r3, [r6, #8]
	cmp	r3, r4
	ble.n	.L_0815835e
	ldr	r0, [pc, #204]
	adds	r3, r2, r0
	str	r3, [r6, #8]
.L_0815835e:
	adds	r7, #1
	adds	r6, #28
	cmp	r7, #144
	bne.n	.L_081582ee
	mov	r1, r8
	cmp	r1, #38
	bne.n	.L_081583a0
	ldr	r2, [sp, #24]
	movs	r7, #0
	ldr	r3, [r2, #20]
	cmp	r3, #0
	beq.n	.L_081583a0
	movs	r5, #36
.L_08158378:
	ldr	r3, [sp, #24]
	movs	r2, #5
	ldrsh	r0, [r5, r3]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r1, #7
	adds	r3, r7, #0
	bl	sub_0814cd48
	ldr	r1, [sp, #24]
	adds	r7, #1
	ldrsh	r0, [r5, r1]
	movs	r1, #6
	bl	sub_08118088
	ldr	r4, [sp, #24]
	adds	r5, #2
	ldr	r3, [r4, #20]
	cmp	r7, r3
	bne.n	.L_08158378
.L_081583a0:
	movs	r1, #8
	movs	r0, #8
	bl	.L_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #1
	add	r8, r1
	movs	r0, #8
	mov	r2, r8
	add	sl, r0
	cmp	r2, #112
	beq.n	.L_081583ce
	b.n	.L_081581c6
.L_081583ce:
	ldr	r0, [pc, #48]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00000134
	.4byte 0x00000151
	.4byte 0x0000017b
	.4byte 0x08143001
	.4byte 0x02010000
	.4byte 0x03000730
	.4byte 0x00000178
	.4byte 0x0819747a
	.4byte 0x08197467
	.4byte 0x08197473
	.4byte 0x0819746d
	.4byte 0x08197410
	.4byte 0xffffe000
	.2byte 0x0801
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #68
	str	r0, [sp, #20]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	ldr	r3, [r3, #96]
	mov	fp, r0
	movs	r0, #0
	str	r3, [sp, #16]
	bl	sub_081435e0
	ldr	r2, [sp, #20]
	movs	r3, #65
	ldr	r1, [r2, #4]
	adds	r0, r2, #0
	lsls	r1, r1, #4
	orrs	r1, r3
	mov	r3, sp
	adds	r3, #56
	str	r3, [sp, #12]
	ldr	r2, [sp, #12]
	add	r3, sp, #44
	bl	sub_0815585c
	ldr	r4, [sp, #20]
	mov	r1, sp
	ldr	r0, [r4, #4]
	adds	r1, #24
	str	r1, [sp, #8]
	bl	sub_08144aac
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #288]
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	.L_08157cf4
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, fp
	movs	r3, #75
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #256]
	bl	sub_080145a8
	ldr	r3, [sp, #20]
	add	r5, sp, #32
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	adds	r1, r5, #0
	bl	sub_0815e21c
	ldr	r0, [pc, #240]
	movs	r4, #15
	movs	r1, #127
	movs	r7, #0
	mov	r9, r4
	mov	r8, r0
	mov	sl, r1
.L_081584c4:
	bl	sub_08014878
	movs	r6, #254
	lsls	r6, r6, #7
	adds	r6, #255
	movs	r2, #128
	lsls	r2, r2, #7
	ands	r6, r0
	adds	r6, r6, r2
	bl	sub_08014878
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r5, #255
	ands	r5, r0
	bl	sub_08014878
	ldr	r3, [sp, #32]
	mov	r4, r9
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	ands	r0, r4
	asrs	r3, r3, #1
	adds	r3, r3, r0
	subs	r3, #8
	mov	r0, r8
	lsls	r3, r3, #16
	str	r3, [r0, #0]
	ldr	r3, [sp, #36]
	adds	r5, #128
	adds	r3, #8
	lsls	r3, r3, #16
	str	r3, [r0, #4]
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	mov	r1, r8
	asrs	r3, r3, #9
	str	r3, [r1, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	mov	r2, r8
	asrs	r3, r3, #6
	str	r3, [r2, #16]
	bl	sub_08014878
	mov	r3, sl
	mov	r4, r8
	ands	r0, r3
	str	r0, [r4, #8]
	bl	sub_08014878
	mov	r1, sl
	mov	r2, r8
	ands	r0, r1
	str	r0, [r2, #20]
	bl	sub_08014878
	mov	r3, r9
	ands	r0, r3
	adds	r0, #32
	mov	r4, r8
	str	r0, [r4, #24]
	adds	r7, #1
	movs	r0, #28
	add	r8, r0
	cmp	r7, #64
	bne.n	.L_081584c4
	movs	r1, #0
	mov	sl, r1
.L_0815855a:
	mov	r2, sl
	cmp	r2, #47
	ble.n	.L_08158572
	ldr	r2, [pc, #52]
	ldr	r1, [pc, #56]
	movs	r3, #128
	mov	r4, sl
	lsls	r3, r3, #19
	subs	r2, r2, r4
	adds	r3, #82
	orrs	r2, r1
	strh	r2, [r3, #0]
.L_08158572:
	mov	r0, sl
	cmp	r0, #1
	bne.n	.L_081585b8
	movs	r1, #176
	lsls	r1, r1, #4
	ldr	r0, [pc, #44]
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	.L_08157cf4
	movs	r1, #216
	lsls	r1, r1, #7
	adds	r1, #192
	ldr	r0, [pc, #32]
	add	r1, fp
	movs	r2, #1
	movs	r3, #0
	b.n	.L_081585b4
	.4byte 0x00000040
	.4byte 0x00001000
	.4byte 0x0000012f
	.4byte 0x08143001
	.4byte 0x02010000
	.4byte 0x0000017c
	.2byte 0x0155
	.2byte 0x0000
.L_081585b4:
	bl	.L_08157cf4
.L_081585b8:
	ldr	r1, [sp, #20]
	ldr	r3, [r1, #28]
	cmp	r3, #1
	bne.n	sub_08158640
	mov	r2, sl
	lsls	r5, r2, #11
	adds	r0, r5, #0
	bl	sub_08002096
	ldr	r4, [sp, #12]
	negs	r0, r0
	ldr	r3, [r4, #0]
	lsls	r0, r0, #2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	asrs	r0, r0, #16
	adds	r0, r0, r3
	subs	r0, #10
	mov	r9, r0
	adds	r0, r5, #0
	bl	sub_08002090
	ldr	r1, [sp, #12]
	lsls	r0, r0, #1
	ldr	r3, [r1, #4]
	asrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r5, r0, #0
	mov	r2, sl
	subs	r5, #22
	cmp	r2, #69
	ble.n	.L_08158602
	lsls	r3, r2, #1
	subs	r3, r5, r3
	adds	r5, r3, #0
	adds	r5, #138
.L_08158602:
	movs	r3, #20
	movs	r6, #216
	movs	r7, #40
	str	r3, [sp, #0]
	ldr	r0, [sp, #8]
	str	r7, [sp, #4]
	lsls	r6, r6, #7
	adds	r6, #192
	add	r6, fp
	ldr	r4, [r0, #4]
	adds	r1, r6, #0
	mov	r8, r3
	ldr	r0, [sp, #16]
	mov	r2, r9
	adds	r3, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4651
	cmp	r1, #3
	bgt.n	sub_08158640
	mov	r2, r8
	str	r2, [sp, #0]
	ldr	r3, [sp, #8]
	str	r7, [sp, #4]
	ldr	r0, [sp, #16]
	ldr	r4, [r3, #4]
	adds	r1, r6, #0
	mov	r2, r9
	adds	r3, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4c63
	movs	r7, #0
	mov	r8, r4
.L_08158646:
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_0815864e
	adds	r3, r7, #3
.L_0815864e:
	asrs	r3, r3, #2
	adds	r3, #4
	cmp	sl, r3
	blt.n	.L_081586b2
	mov	r0, r8
	ldr	r4, [r0, #8]
	cmp	r4, #0
	bge.n	.L_08158660
	adds	r4, #127
.L_08158660:
	ldr	r2, [pc, #368]
	movs	r3, #3
	asrs	r4, r4, #7
	ands	r4, r3
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	movs	r2, #176
	lsls	r2, r2, #4
	mov	r0, r8
	add	r1, fp
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r0, r3]
	ldr	r3, [pc, #348]
	movs	r5, #1
	ldrb	r6, [r3, r4]
	ands	r5, r7
	lsrs	r3, r6, #1
	subs	r2, r2, r3
	movs	r3, #6
	ldrsh	r0, [r0, r3]
	lsls	r5, r5, #2
	mov	ip, r0
	ldr	r0, [pc, #332]
	mov	r3, ip
	ldrb	r4, [r0, r4]
	str	r6, [sp, #0]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #8]
	ldr	r4, [r5, r0]
	ldr	r0, [sp, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	mov	r0, r8
	movs	r1, #63
	lsls	r2, r2, #5
	bl	sub_08138058
.L_081586b2:
	movs	r1, #28
	adds	r7, #1
	add	r8, r1
	cmp	r7, #64
	bne.n	.L_08158646
	mov	r2, sl
	cmp	r2, #8
	bne.n	.L_081586f2
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	add	r3, fp
	str	r2, [r3, #0]
	movs	r0, #134
	bl	sub_081180e8
	ldr	r4, [sp, #20]
	movs	r1, #7
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	ldr	r2, [sp, #20]
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r1, #3
	bl	sub_08118088
.L_081586f2:
	mov	r3, sl
	lsls	r5, r3, #2
	cmp	r5, #32
	ble.n	.L_081586fc
	movs	r5, #32
.L_081586fc:
	ldr	r4, [sp, #20]
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_08158742
	movs	r0, #224
	lsls	r0, r0, #3
	add	r0, fp
	movs	r1, #120
	movs	r7, #0
	mov	r8, r0
	movs	r6, #32
	mov	r9, r1
.L_08158714:
	mov	r2, sl
	lsls	r1, r7, #5
	cmp	r2, #0
	bge.n	.L_0815871e
	adds	r2, #3
.L_0815871e:
	movs	r3, #31
	asrs	r2, r2, #2
	ands	r2, r3
	ldr	r3, [sp, #24]
	mov	r4, r9
	subs	r2, r1, r2
	mov	ip, r3
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	ldr	r0, [sp, #16]
	mov	r1, r8
	subs	r3, r4, r5
	adds	r7, #1
	mov	lr, ip
	.2byte 0xf800
	.2byte 0x2f05
	bne.n	.L_08158714
	b.n	.L_08158780
.L_08158742:
	movs	r0, #224
	lsls	r0, r0, #3
	add	r0, fp
	movs	r1, #120
	movs	r7, #0
	mov	r8, r0
	movs	r6, #32
	mov	r9, r1
.L_08158752:
	mov	r2, sl
	lsls	r1, r7, #5
	cmp	r2, #0
	bge.n	.L_0815875c
	adds	r2, #3
.L_0815875c:
	movs	r3, #31
	asrs	r2, r2, #2
	ands	r2, r3
	ldr	r3, [sp, #24]
	adds	r2, r1, r2
	mov	r4, r9
	mov	ip, r3
	subs	r2, #32
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	ldr	r0, [sp, #16]
	mov	r1, r8
	subs	r3, r4, r5
	adds	r7, #1
	mov	lr, ip
	.2byte 0xf800
	.2byte 0x2f05
	bne.n	.L_08158752
.L_08158780:
	movs	r1, #8
	movs	r0, #4
	bl	.L_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, fp
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	cmp	r1, #64
	beq.n	.L_081587aa
	b.n	.L_0815855a
.L_081587aa:
	ldr	r0, [pc, #52]
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
	.4byte 0x02010000
	.4byte 0x0819851c
	.4byte 0x08198513
	.4byte 0x08198517
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #96
	str	r0, [sp, #56]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #96]
	ldr	r0, [r3, #92]
	str	r1, [sp, #52]
	mov	r9, r0
	ldr	r3, [r3, #48]
	movs	r0, #0
	str	r3, [sp, #32]
	bl	sub_081435e0
	ldr	r2, [sp, #56]
	movs	r3, #64
	ldr	r1, [r2, #4]
	adds	r0, r2, #0
	lsls	r1, r1, #4
	orrs	r1, r3
	mov	r3, sp
	adds	r3, #84
	str	r3, [sp, #28]
	ldr	r2, [sp, #28]
	add	r3, sp, #72
	bl	sub_0815585c
	ldr	r3, [pc, #28]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r4, [sp, #56]
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne.n	.L_08158848
	movs	r0, #104
	movs	r1, #23
	bl	sub_081963ec
	b.n	.L_08158850
	movs	r0, r0
	.2byte 0x1010
	.2byte 0x0000
.L_08158848:
	movs	r0, #104
	movs	r1, #19
	bl	sub_081963ec
.L_08158850:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r1, #224
	lsls	r1, r1, #3
	str	r3, [sp, #36]
	ldr	r0, [pc, #732]
	add	r1, r9
	movs	r2, #1
	movs	r3, #0
	bl	.L_08157cf4
	movs	r1, #216
	lsls	r1, r1, #7
	adds	r1, #192
	ldr	r0, [pc, #720]
	add	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	.L_08157cf4
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
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
	ldr	r0, [pc, #684]
	bl	sub_080145a8
	ldr	r5, [sp, #56]
	movs	r7, #0
	ldr	r0, [r5, #8]
	bl	sub_08118098
	ldr	r6, [r0, #0]
	movs	r1, #36
	ldrsh	r0, [r5, r1]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	movs	r2, #0
	str	r0, [sp, #24]
	str	r2, [sp, #48]
	mov	r5, r9
.L_081588b8:
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	movs	r3, #132
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	asrs	r3, r7, #5
	str	r3, [r5, #12]
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	subs	r3, #64
	lsls	r3, r3, #16
	asrs	r3, r3, #6
	str	r3, [r5, #16]
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	subs	r3, #127
	lsls	r3, r3, #16
	asrs	r3, r3, #5
	str	r3, [r5, #20]
	ldr	r3, [r5, #0]
	cmp	r3, #0
	ble.n	.L_081588f6
	ldr	r3, [r5, #12]
	negs	r3, r3
	str	r3, [r5, #12]
.L_081588f6:
	movs	r3, #1
	str	r3, [r5, #24]
	ldr	r4, [sp, #48]
	movs	r3, #160
	lsls	r3, r3, #15
	adds	r4, #1
	adds	r7, r7, r3
	adds	r5, #28
	str	r4, [sp, #48]
	cmp	r4, #8
	bne.n	.L_081588b8
	ldr	r0, [sp, #32]
	movs	r5, #0
	adds	r0, #12
	str	r5, [sp, #44]
	str	r0, [sp, #16]
.L_08158916:
	ldr	r1, [sp, #44]
	cmp	r1, #16
	ble.n	.L_08158922
	ldr	r0, [pc, #540]
	bl	sub_0815f0a0
.L_08158922:
	ldr	r2, [sp, #56]
	ldr	r3, [r2, #28]
	cmp	r3, #1
	bne.n	sub_081589d0
	ldr	r3, [sp, #44]
	lsls	r5, r3, #11
	adds	r0, r5, #0
	bl	sub_08002096
	ldr	r4, [sp, #28]
	negs	r0, r0
	ldr	r3, [r4, #0]
	lsls	r0, r0, #2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	asrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_08002090
	ldr	r5, [sp, #28]
	lsls	r0, r0, #1
	ldr	r3, [r5, #4]
	asrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r5, r0, #0
	ldr	r0, [sp, #44]
	subs	r6, #10
	subs	r5, #22
	cmp	r0, #16
	ble.n	.L_0815896c
	lsls	r3, r0, #1
	subs	r3, r5, r3
	adds	r5, r3, #0
	adds	r5, #32
.L_0815896c:
	ldr	r1, [sp, #56]
	ldr	r3, [r1, #4]
	cmp	r3, #1
	bne.n	.L_0815897e
	movs	r0, #188
	movs	r1, #7
	bl	sub_081963ec
	b.n	.L_08158986
.L_0815897e:
	movs	r0, #188
	movs	r1, #3
	bl	sub_081963ec
.L_08158986:
	ldr	r2, [sp, #44]
	cmp	r2, #3
	bgt.n	sub_081589ae
	movs	r3, #20
	str	r3, [sp, #0]
	movs	r3, #40
	str	r3, [sp, #4]
	movs	r2, #192
	movs	r1, #216
	lsls	r2, r2, #18
	lsls	r1, r1, #7
	adds	r2, #188
	adds	r1, #192
	ldr	r4, [r2, #0]
	ldr	r0, [sp, #52]
	add	r1, r9
	adds	r2, r6, #0
	adds	r3, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r1, #216
	movs	r3, #20
	lsls	r1, r1, #7
	str	r3, [sp, #0]
	adds	r1, #192
	movs	r3, #40
	str	r3, [sp, #4]
	ldr	r0, [sp, #52]
	add	r1, r9
	adds	r2, r6, #0
	adds	r3, r5, #0
	ldr	r4, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9d0b
	movs	r3, #1
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L_08158a0c
	movs	r0, #0
	movs	r5, #224
	str	r0, [sp, #48]
	ldr	r6, [pc, #356]
	lsls	r5, r5, #2
	add	r5, r9
.L_081589e6:
	bl	sub_08014878
	movs	r1, #6
	bl	sub_0800206c
	adds	r0, #3
	str	r0, [r5, #12]
	bl	sub_08014878
	movs	r3, #3
	ands	r3, r0
	ldrb	r3, [r6, r3]
	str	r3, [r5, #16]
	ldr	r1, [sp, #48]
	adds	r5, #28
	adds	r1, #1
	str	r1, [sp, #48]
	cmp	r1, #32
	bne.n	.L_081589e6
.L_08158a0c:
	bl	sub_08014de4
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #16]
	bl	sub_080156e8
	mov	ip, r9
	movs	r2, #0
	mov	r3, ip
	str	r2, [sp, #48]
	str	r2, [sp, #12]
	str	r3, [sp, #8]
	mov	r6, r9
.L_08158a26:
	ldr	r3, [r6, #24]
	cmp	r3, #1
	beq.n	.L_08158a2e
	b.n	.L_08158c68
.L_08158a2e:
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #44]
	str	r0, [sp, #20]
	cmp	r1, r0
	bgt.n	.L_08158a3a
	b.n	.L_08158bb8
.L_08158a3a:
	add	r5, sp, #60
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_0815e1ec
	ldr	r3, [r5, #0]
	movs	r1, #224
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	subs	r3, #12
	mov	sl, r3
	ldr	r3, [r5, #4]
	lsls	r1, r1, #3
	subs	r3, #24
	mov	r8, r3
	movs	r3, #24
	str	r3, [sp, #0]
	movs	r3, #48
	str	r3, [sp, #4]
	mov	r2, sl
	mov	r3, r8
	ldr	r5, [sp, #36]
	ldr	r0, [sp, #52]
	add	r1, r9
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9a0b
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #1
	bgt.n	.L_08158aa4
	ldr	r3, [pc, #208]
	ldr	r4, [pc, #212]
	ldrh	r1, [r3, #2]
	movs	r3, #224
	lsls	r3, r3, #3
	add	r1, r9
	adds	r1, r1, r3
	ldr	r3, [pc, #204]
	ldrb	r2, [r3, #1]
	ldrb	r3, [r4, #1]
	ldr	r4, [pc, #200]
	add	r2, sl
	ldrb	r0, [r4, #1]
	add	r3, r8
	str	r0, [sp, #0]
	ldr	r0, [pc, #196]
	ldrb	r0, [r0, #1]
	str	r0, [sp, #4]
	ldr	r0, [sp, #52]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0xe014
.L_08158aa4:
	ldr	r3, [pc, #164]
	ldr	r4, [pc, #168]
	ldrh	r1, [r3, #4]
	ldr	r3, [pc, #168]
	movs	r0, #224
	ldrb	r2, [r3, #2]
	ldrb	r3, [r4, #2]
	ldr	r4, [pc, #164]
	lsls	r0, r0, #3
	add	r1, r9
	adds	r1, r1, r0
	ldrb	r0, [r4, #2]
	add	r2, sl
	str	r0, [sp, #0]
	ldr	r0, [pc, #152]
	add	r3, r8
	ldrb	r0, [r0, #2]
	str	r0, [sp, #4]
	ldr	r0, [sp, #52]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9802
	movs	r1, #224
	movs	r5, #0
	lsls	r1, r1, #2
	mov	fp, r5
	adds	r7, r0, r1
.L_08158ada:
	movs	r2, #1
	ldr	r3, [r7, #16]
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r0, #188
	movs	r2, #7
	bl	sub_08196404
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r3, [r3, #0]
	ldr	r1, [r7, #16]
	str	r3, [sp, #40]
	movs	r3, #4
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08158b12
	ldr	r0, [r7, #12]
	ldr	r4, [pc, #84]
	ldr	r5, [pc, #80]
	ldrb	r3, [r4, r0]
	mov	r2, sl
	subs	r3, r2, r3
	ldrb	r2, [r5, r0]
	subs	r3, r3, r2
	adds	r3, #24
	b.n	.L_08158b1c
.L_08158b12:
	ldr	r0, [r7, #12]
	ldr	r2, [pc, #60]
	ldr	r4, [pc, #64]
	ldrb	r3, [r2, r0]
	add	r3, sl
.L_08158b1c:
	mov	ip, r3
	movs	r3, #8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08158b60
	ldr	r5, [pc, #52]
	mov	r1, r8
	ldrb	r3, [r5, r0]
	ldr	r5, [pc, #32]
	subs	r3, r1, r3
	ldrb	r2, [r5, r0]
	subs	r3, r3, r2
	adds	r5, r3, #0
	adds	r5, #48
	b.n	.L_08158b68
	movs	r0, r0
	.4byte 0x0000016c
	.4byte 0x00000157
	.4byte 0x08143001
	.4byte 0x08198524
	.4byte 0x0819853a
	.4byte 0x08198555
	.4byte 0x0819854c
	.4byte 0x08198528
	.2byte 0x8531
	.2byte 0x0819
.L_08158b60:
	ldr	r1, [pc, #360]
	mov	r2, r8
	ldrb	r3, [r1, r0]
	adds	r5, r2, r3
.L_08158b68:
	ldr	r2, [pc, #356]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	movs	r3, #224
	lsls	r3, r3, #3
	add	r1, r9
	adds	r1, r1, r3
	ldrb	r3, [r4, r0]
	ldr	r4, [pc, #344]
	str	r3, [sp, #0]
	ldr	r3, [r7, #12]
	ldr	r0, [sp, #52]
	ldrb	r3, [r4, r3]
	mov	r2, ip
	str	r3, [sp, #4]
	adds	r3, r5, #0
	ldr	r5, [sp, #40]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r0, #1
	add	fp, r0
	mov	r1, fp
	adds	r7, #28
	cmp	r1, #4
	bne.n	.L_08158ada
	ldr	r3, [r6, #0]
	ldr	r2, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #0]
	ldr	r2, [r6, #16]
	ldr	r3, [r6, #4]
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r2, [r6, #20]
	ldr	r3, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #8]
.L_08158bb8:
	ldr	r3, [sp, #12]
	ldr	r2, [sp, #44]
	adds	r3, #16
	cmp	r2, r3
	ble.n	.L_08158c68
	ldr	r4, [sp, #24]
	ldr	r2, [r6, #0]
	ldr	r3, [r4, #8]
	subs	r3, r3, r2
	ldr	r2, [r6, #12]
	asrs	r3, r3, #8
	adds	r1, r2, r3
	ldr	r2, [r6, #4]
	movs	r3, #160
	lsls	r3, r3, #13
	subs	r3, r3, r2
	ldr	r2, [r6, #16]
	asrs	r3, r3, #8
	adds	r0, r2, r3
	str	r1, [r6, #12]
	str	r0, [r6, #16]
	ldr	r2, [r6, #8]
	ldr	r3, [r4, #16]
	subs	r3, r3, r2
	ldr	r2, [r6, #20]
	asrs	r3, r3, #8
	adds	r4, r2, r3
	str	r4, [r6, #20]
	ldr	r3, [sp, #20]
	ldr	r5, [sp, #44]
	adds	r3, #85
	cmp	r5, r3
	bge.n	.L_08158c2a
	lsls	r3, r1, #4
	subs	r3, r3, r1
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L_08158c06
	adds	r2, #63
.L_08158c06:
	asrs	r3, r2, #6
	str	r3, [r6, #12]
	lsls	r3, r0, #4
	subs	r3, r3, r0
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L_08158c16
	adds	r2, #63
.L_08158c16:
	asrs	r3, r2, #6
	str	r3, [r6, #16]
	lsls	r3, r4, #4
	subs	r3, r3, r4
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L_08158c26
	adds	r2, #63
.L_08158c26:
	asrs	r3, r2, #6
	str	r3, [r6, #20]
.L_08158c2a:
	ldr	r3, [r6, #4]
	ldr	r0, [pc, #168]
	cmp	r3, r0
	bgt.n	.L_08158c68
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, r9
	movs	r3, #8
	str	r3, [r2, #0]
	movs	r3, #0
	str	r3, [r6, #24]
	movs	r0, #134
	bl	sub_081c0010
	ldr	r2, [sp, #56]
	movs	r3, #4
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	movs	r2, #5
	bl	sub_0814cd48
	ldr	r4, [sp, #56]
	movs	r1, #4
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	bl	sub_08118088
.L_08158c68:
	ldr	r5, [sp, #12]
	ldr	r0, [sp, #8]
	ldr	r1, [sp, #48]
	adds	r5, #2
	adds	r0, #112
	adds	r1, #1
	str	r5, [sp, #12]
	adds	r6, #28
	str	r0, [sp, #8]
	str	r1, [sp, #48]
	cmp	r1, #6
	beq.n	.L_08158c82
	b.n	.L_08158a26
.L_08158c82:
	movs	r0, #16
	movs	r1, #16
	bl	.L_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, r9
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #44]
	adds	r2, #1
	str	r2, [sp, #44]
	cmp	r2, #96
	beq.n	.L_08158cac
	b.n	.L_08158916
.L_08158cac:
	ldr	r0, [pc, #44]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #96
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x08198555
	.4byte 0x0819853a
	.4byte 0x08198531
	.4byte 0x0013ffff
	.2byte 0x3001
	.2byte 0x0814
.L_08158ce0:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r6, r0, #0
	adds	r7, r1, #0
	movs	r0, #238
	ldr	r1, [r3, #92]
	lsls	r0, r0, #7
	adds	r0, #168
	adds	r0, r0, r1
	ldr	r3, [r0, #0]
	mov	r8, r0
	cmp	r3, #0
	ble.n	.L_08158d3a
	bl	sub_08014878
	subs	r6, #1
	ands	r6, r0
	bl	sub_08014878
	lsrs	r5, r7, #31
	adds	r5, r7, r5
	subs	r2, r7, #1
	asrs	r5, r5, #1
	ands	r2, r0
	subs	r2, r2, r5
	ldr	r1, [pc, #68]
	adds	r3, r2, #0
	subs	r6, r6, r5
	adds	r3, #32
	strh	r6, [r1, #4]
	strh	r3, [r1, #6]
	ldr	r1, [pc, #60]
	movs	r3, #120
	subs	r6, r3, r6
	subs	r3, r3, r2
	str	r6, [r1, #12]
	str	r3, [r1, #16]
	mov	r2, r8
	ldr	r3, [r2, #0]
	subs	r3, #1
	str	r3, [r2, #0]
	b.n	.L_08158d58
.L_08158d3a:
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #160
	adds	r3, r1, r0
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #24]
	adds	r0, #4
	strh	r3, [r2, #4]
	adds	r3, r1, r0
	ldr	r3, [r3, #0]
	strh	r3, [r2, #6]
	ldr	r2, [pc, #16]
	movs	r3, #120
	str	r3, [r2, #12]
	str	r3, [r2, #16]
.L_08158d58:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03001120
	.2byte 0x11e0
	.2byte 0x0300
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #92]
	adds	r5, r0, #0
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #168
	adds	r6, r1, r0
	ldr	r3, [r6, #0]
	cmp	r3, #0
	ble.n	.L_08158da8
	bl	sub_08014878
	lsrs	r3, r5, #31
	subs	r2, r5, #1
	adds	r3, r5, r3
	asrs	r3, r3, #1
	ands	r2, r0
	subs	r2, r2, r3
	ldr	r1, [pc, #44]
	adds	r3, r2, #0
	adds	r3, #32
	strh	r3, [r1, #6]
	ldr	r1, [pc, #40]
	movs	r3, #120
	subs	r3, r3, r2
	str	r3, [r1, #16]
	ldr	r3, [r6, #0]
	subs	r3, #1
	str	r3, [r6, #0]
	b.n	.L_08158dbc
.L_08158da8:
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #164
	adds	r3, r1, r0
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #12]
	strh	r3, [r2, #6]
	ldr	r2, [pc, #12]
	movs	r3, #120
	str	r3, [r2, #16]
.L_08158dbc:
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x03001120
	.2byte 0x11e0
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #324
	movs	r3, #192
	str	r0, [sp, #120]
	lsls	r3, r3, #18
	adds	r2, r3, #0
	adds	r2, #176
	ldr	r2, [r2, #0]
	movs	r5, #248
	str	r2, [sp, #116]
	lsls	r5, r5, #5
	ldr	r0, [r3, #92]
	movs	r6, #0
	str	r0, [sp, #112]
	movs	r0, #128
	ldr	r1, [r3, #96]
	lsls	r0, r0, #6
	str	r1, [sp, #108]
	mov	r9, r6
	ldr	r3, [r3, #100]
	movs	r6, #192
	str	r3, [sp, #104]
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r2, [sp, #112]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #48]
	movs	r2, #1
	movs	r3, #1
	bl	.L_08157cf4
	ldr	r4, [sp, #112]
	ldr	r0, [pc, #40]
	adds	r1, r4, r5
	movs	r2, #0
	movs	r3, #0
	bl	.L_08157cf4
	lsls	r6, r6, #2
	ldr	r0, [pc, #28]
	ldr	r1, [sp, #104]
	movs	r2, #0
	movs	r3, #0
	bl	.L_08157cf4
	movs	r5, #0
	movs	r0, #64
	b.n	.L_08158e54
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000180
	.4byte 0x00000136
	.2byte 0x0134
	.2byte 0x0000
.L_08158e54:
	adds	r6, #2
.L_08158e56:
	ldr	r2, [sp, #112]
	ldr	r1, [sp, #104]
	adds	r3, r5, r2
	movs	r2, #184
	lsls	r2, r2, #6
	adds	r2, #16
	movs	r4, #0
	mov	ip, r0
	adds	r3, r3, r2
.L_08158e68:
	ldrb	r2, [r1, #0]
	adds	r1, #1
	cmp	r2, ip
	ble.n	.L_08158e72
	mov	r2, ip
.L_08158e72:
	cmp	r2, #0
	bge.n	.L_08158e78
	movs	r2, #0
.L_08158e78:
	adds	r4, #1
	strb	r2, [r3, #0]
	adds	r3, #1
	cmp	r4, r6
	bne.n	.L_08158e68
	movs	r3, #1
	add	r9, r3
	adds	r5, r5, r4
	mov	r4, r9
	subs	r0, #7
	cmp	r4, #8
	bne.n	.L_08158e56
	bl	sub_0813ba50
	ldr	r2, [pc, #56]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r5, [sp, #112]
	movs	r6, #238
	lsls	r6, r6, #7
	movs	r0, #238
	adds	r6, #144
	lsls	r0, r0, #7
	movs	r2, #238
	adds	r3, r5, r6
	adds	r0, #148
	movs	r6, #0
	lsls	r2, r2, #7
	movs	r4, #238
	str	r6, [r3, #0]
	movs	r1, #2
	adds	r3, r5, r0
	adds	r2, #152
	lsls	r4, r4, #7
	adds	r4, #156
	b.n	.L_08158ed8
	.4byte 0x00000000
	.2byte 0x2784
	.2byte 0x0000
.L_08158ed8:
	str	r1, [r3, #0]
	adds	r3, r5, r2
	movs	r2, #1
	str	r2, [r3, #0]
	adds	r3, r5, r4
	str	r6, [r3, #0]
	ldr	r5, [sp, #116]
	mov	sl, r1
	str	r2, [r5, #16]
	movs	r5, #200
	lsls	r5, r5, #4
	adds	r1, r5, #0
	ldr	r0, [pc, #108]
	bl	sub_080145a8
	adds	r1, r5, #0
	ldr	r0, [pc, #104]
	bl	sub_080145a8
	ldr	r0, [sp, #112]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r0, r0, r1
	str	r6, [r0, #0]
	mov	r8, r0
	movs	r1, #0
	movs	r0, #0
	bl	sub_08163c2c
	ldr	r1, [pc, #84]
	movs	r0, #1
	movs	r2, #0
	bl	sub_08118040
	ldr	r2, [pc, #76]
	ldr	r5, [pc, #52]
	movs	r6, #128
	movs	r3, #240
	lsls	r6, r6, #19
	str	r3, [r2, #16]
	adds	r6, #32
	movs	r0, #0
	movs	r1, #1
	bl	sub_08163c2c
	strh	r5, [r6, #0]
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r2, #128
	str	r3, [sp, #92]
	ldr	r3, [pc, #16]
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #12]
	adds	r2, #82
	strh	r5, [r6, #0]
	b.n	.L_08158f70
	.4byte 0x00000080
	.4byte 0x00007741
	.4byte 0x0000100f
	.4byte 0x0813baed
	.4byte 0x08143001
	.4byte 0x00000074
	.2byte 0x11e0
	.2byte 0x0300
.L_08158f70:
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	subs	r2, #2
	strh	r3, [r2, #0]
	mov	r2, sl
	mov	r3, r8
	str	r2, [r3, #0]
	ldr	r4, [sp, #112]
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #132
	movs	r0, #146
	adds	r2, r4, r5
	movs	r3, #75
	lsls	r0, r0, #1
	str	r3, [r2, #0]
	add	r0, sp
	movs	r2, #68
	adds	r2, #255
	str	r0, [sp, #44]
	movs	r6, #0
	add	r2, sp
	mov	r9, r6
	adds	r5, r0, #0
	movs	r6, #63
	movs	r1, #3
	adds	r4, r2, #0
	b.n	.L_08158fac
	.2byte 0x3f44
	.2byte 0x0000
.L_08158fac:
	strb	r1, [r0, #0]
	strb	r1, [r4, #0]
	subs	r4, #1
	ldrb	r3, [r0, #0]
	cmp	r3, #63
	bls.n	.L_08158fba
	strb	r6, [r5, #0]
.L_08158fba:
	ldrb	r3, [r2, #0]
	cmp	r3, #63
	bls.n	.L_08158fc2
	strb	r6, [r2, #0]
.L_08158fc2:
	movs	r3, #1
	add	r9, r3
	mov	r3, r9
	adds	r5, #1
	adds	r0, #1
	subs	r2, #1
	adds	r1, #8
	cmp	r3, #16
	bne.n	.L_08158fac
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r0, [pc, #44]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #36]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4e08
	mov	r5, sp
	movs	r4, #0
	adds	r5, #124
	str	r4, [sp, #100]
	str	r5, [sp, #80]
	str	r6, [sp, #24]
	str	r4, [sp, #20]
	str	r4, [sp, #16]
	b.n	.L_081594e2
	movs	r0, r0
	.4byte 0x00000784
	.4byte 0x00000184
	.4byte 0x03000730
	.2byte 0xf460
	.2byte 0xffff
.L_08159018:
	ldr	r1, [sp, #100]
	movs	r0, #0
	movs	r5, #63
	mov	r9, r0
	ands	r5, r1
.L_08159022:
	cmp	r5, #15
	bgt.n	.L_0815902e
	ldr	r0, [pc, #656]
	bl	sub_0815f0a0
	b.n	.L_0815904c
.L_0815902e:
	cmp	r5, #31
	bgt.n	.L_0815903a
	ldr	r0, [pc, #648]
	bl	sub_0815f0a0
	b.n	.L_0815904c
.L_0815903a:
	cmp	r5, #47
	bgt.n	.L_08159046
	ldr	r0, [pc, #640]
	bl	sub_0815f0a0
	b.n	.L_0815904c
.L_08159046:
	ldr	r0, [pc, #636]
	bl	sub_0815f0a0
.L_0815904c:
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	cmp	r3, #2
	bne.n	.L_08159022
	ldr	r4, [sp, #100]
	cmp	r4, #150
	bne.n	.L_08159074
	ldr	r5, [sp, #112]
	movs	r6, #239
	lsls	r6, r6, #7
	adds	r2, r5, r6
	movs	r3, #1
	movs	r0, #238
	str	r3, [r2, #0]
	lsls	r0, r0, #7
	ldr	r3, [pc, #600]
	adds	r0, #132
	adds	r2, r5, r0
	b.n	.L_0815908a
.L_08159074:
	ldr	r1, [sp, #112]
	movs	r2, #239
	movs	r5, #238
	lsls	r2, r2, #7
	lsls	r5, r5, #7
	adds	r3, r1, r2
	mov	r4, r9
	adds	r5, #132
	str	r4, [r3, #0]
	adds	r2, r1, r5
	movs	r3, #75
.L_0815908a:
	str	r3, [r2, #0]
	movs	r0, #255
	movs	r1, #192
	ldr	r3, [pc, #568]
	lsls	r1, r1, #8
	lsls	r0, r0, #17
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c01
	movs	r0, #255
	lsls	r0, r0, #17
	ldr	r2, [pc, #556]
	bl	sub_08015768
	bl	sub_08014de4
	ldr	r6, [sp, #100]
	cmp	r6, #128
	ble.n	.L_08159130
	subs	r6, #128
	cmp	r6, #22
	ble.n	.L_081590b8
	movs	r6, #20
.L_081590b8:
	add	r0, sp, #136
	movs	r3, #0
	str	r3, [r0, #0]
	negs	r3, r6
	lsls	r3, r3, #17
	str	r3, [r0, #4]
	movs	r3, #128
	lsls	r3, r3, #18
	str	r3, [r0, #8]
	asrs	r3, r6, #2
	adds	r3, #2
	str	r3, [sp, #88]
	cmp	r3, #8
	ble.n	.L_081590d8
	movs	r1, #8
	str	r1, [sp, #88]
.L_081590d8:
	movs	r5, #128
	bl	sub_08015128
	lsls	r5, r5, #8
	ldr	r0, [pc, #496]
	bl	sub_08015024
	adds	r0, r5, #0
	bl	sub_080150e4
	adds	r0, r5, #0
	bl	sub_08015024
	lsls	r0, r6, #12
	bl	sub_08015068
	ldr	r2, [sp, #100]
	cmp	r2, #150
	ble.n	.L_08159116
	ldr	r4, [sp, #16]
	ldr	r5, [pc, #468]
	ldr	r0, [sp, #80]
	adds	r3, r4, r2
	lsls	r3, r3, #11
	adds	r3, r3, r5
	movs	r6, #5
	str	r3, [r0, #0]
	str	r3, [r0, #4]
	str	r3, [r0, #8]
	str	r6, [sp, #88]
	b.n	.L_0815912a
.L_08159116:
	lsls	r3, r6, #1
	ldr	r0, [sp, #80]
	adds	r3, r3, r6
	movs	r2, #128
	lsls	r3, r3, #10
	lsls	r2, r2, #9
	subs	r2, r2, r3
	str	r2, [r0, #0]
	str	r2, [r0, #4]
	str	r2, [r0, #8]
.L_0815912a:
	bl	sub_080151ac
	b.n	.L_08159158
.L_08159130:
	add	r0, sp, #136
	movs	r3, #0
	str	r3, [r0, #0]
	str	r3, [r0, #4]
	movs	r3, #128
	lsls	r3, r3, #17
	str	r3, [r0, #8]
	bl	sub_08015128
	ldr	r0, [pc, #400]
	bl	sub_08015024
	ldr	r0, [sp, #100]
	lsls	r5, r0, #8
	adds	r0, r5, #0
	bl	sub_080150e4
	adds	r0, r5, #0
	bl	sub_08015024
.L_08159158:
	ldr	r1, [sp, #100]
	cmp	r1, #149
	ble.n	.L_08159160
	b.n	.L_08159284
.L_08159160:
	mov	r3, sp
	movs	r2, #0
	adds	r3, #148
	mov	r9, r2
	str	r3, [sp, #48]
	ldr	r2, [pc, #368]
	add	r6, sp, #280
	adds	r5, r3, #0
	movs	r7, #0
.L_08159172:
	ldrsh	r3, [r7, r2]
	adds	r1, r5, #0
	subs	r3, #96
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	movs	r3, #0
	str	r3, [r6, #4]
	adds	r3, r7, #2
	ldrsh	r3, [r3, r2]
	adds	r0, r6, #0
	subs	r3, #96
	lsls	r3, r3, #16
	str	r3, [r6, #8]
	str	r2, [sp, #12]
	bl	sub_0815e1ec
	ldr	r3, [r5, #0]
	adds	r7, #4
	asrs	r3, r3, #17
	adds	r3, #64
	str	r3, [r5, #0]
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	ldr	r2, [sp, #12]
	adds	r3, #60
	str	r3, [r5, #4]
	movs	r3, #1
	add	r9, r3
	mov	r4, r9
	adds	r5, #12
	cmp	r4, #6
	bne.n	.L_08159172
	ldr	r0, [sp, #20]
	movs	r5, #0
	movs	r6, #4
	str	r6, [sp, #36]
	str	r5, [sp, #32]
	str	r0, [sp, #28]
	mov	r9, r5
.L_081591c0:
	ldr	r1, [sp, #28]
	ldr	r2, [pc, #284]
	adds	r1, r1, r2
	mov	r8, r1
	cmp	r1, #48
	ble.n	.L_081591d0
	movs	r3, #48
	mov	r8, r3
.L_081591d0:
	mov	r4, r8
	cmp	r4, #0
	blt.n	.L_08159268
	mov	r5, r8
	movs	r4, #0
	cmp	r5, #0
	beq.n	.L_08159268
	ldr	r6, [sp, #88]
	ldr	r1, [sp, #88]
	asrs	r6, r6, #31
	ldr	r0, [sp, #36]
	str	r6, [sp, #40]
	ldr	r7, [sp, #32]
	ldr	r6, [sp, #48]
	lsls	r1, r1, #1
	mov	fp, r0
	mov	sl, r1
.L_081591f2:
	adds	r3, r7, #0
	adds	r3, #12
	ldr	r2, [r6, r3]
	ldr	r3, [r6, r7]
	movs	r1, #160
	subs	r2, r2, r3
	lsls	r1, r1, #3
	str	r4, [sp, #8]
	adds	r0, r4, #0
	muls	r0, r2
	adds	r1, #85
	ldr	r2, [pc, #216]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x59f5
	adds	r3, r7, #0
	adds	r5, r5, r0
	adds	r3, #16
	mov	r0, fp
	ldr	r2, [r6, r3]
	ldr	r3, [r6, r0]
	ldr	r4, [sp, #8]
	movs	r1, #160
	subs	r2, r2, r3
	lsls	r1, r1, #3
	adds	r0, r4, #0
	muls	r0, r2
	adds	r1, #85
	ldr	r2, [pc, #184]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x4659
	ldr	r3, [r6, r1]
	mov	r2, sl
	adds	r3, r3, r0
	ldr	r0, [pc, #172]
	subs	r2, #2
	ldrh	r1, [r0, r2]
	ldr	r2, [sp, #104]
	ldr	r0, [sp, #40]
	adds	r1, r2, r1
	lsrs	r2, r0, #31
	ldr	r0, [sp, #88]
	adds	r2, r0, r2
	asrs	r2, r2, #1
	subs	r5, r5, r2
	mov	r2, sl
	subs	r3, r3, r0
	str	r0, [sp, #0]
	str	r2, [sp, #4]
	ldr	r0, [sp, #108]
	adds	r2, r5, #0
	ldr	r5, [sp, #92]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9c02
	adds	r4, #1
	cmp	r4, r8
	bne.n	.L_081591f2
.L_08159268:
	ldr	r6, [sp, #36]
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #28]
	movs	r2, #1
	add	r9, r2
	adds	r6, #24
	adds	r0, #24
	subs	r1, #48
	mov	r3, r9
	str	r6, [sp, #36]
	str	r0, [sp, #32]
	str	r1, [sp, #28]
	cmp	r3, #3
	bne.n	.L_081591c0
.L_08159284:
	ldr	r4, [sp, #100]
	cmp	r4, #179
	ble.n	.L_0815928c
	b.n	.L_081593ae
.L_0815928c:
	movs	r5, #0
	cmp	r4, #155
	ble.n	.L_08159296
	adds	r5, r4, #0
	subs	r5, #156
.L_08159296:
	cmp	r5, #7
	ble.n	.L_0815929c
	movs	r5, #7
.L_0815929c:
	ldr	r6, [sp, #100]
	cmp	r6, #139
	bgt.n	.L_081592ec
	movs	r0, #188
	movs	r1, #3
	bl	sub_081963ec
	movs	r0, #192
	lsls	r0, r0, #18
	adds	r0, #188
	ldr	r0, [r0, #0]
	str	r0, [sp, #76]
	b.n	.L_081592fe
	movs	r0, r0
	.4byte 0x00000184
	.4byte 0x00000154
	.4byte 0x00000150
	.4byte 0x00000152
	.4byte 0x1a1a1a1a
	.4byte 0x03000230
	.4byte 0x7fff0000
	.4byte 0xfffff000
	.4byte 0xfff26c00
	.4byte 0x08196e2c
	.4byte 0xffffff00
	.4byte 0x0300021c
	.2byte 0x7410
	.2byte 0x0819
.L_081592ec:
	movs	r1, #19
	movs	r0, #188
	bl	sub_081963ec
	movs	r1, #192
	lsls	r1, r1, #18
	adds	r1, #188
	ldr	r1, [r1, #0]
	str	r1, [sp, #76]
.L_081592fe:
	ldr	r2, [sp, #84]
	mov	r8, r2
	cmp	r2, #128
	ble.n	.L_0815930a
	movs	r3, #128
	mov	r8, r3
.L_0815930a:
	movs	r4, #0
	mov	r6, r8
	mov	r9, r4
	cmp	r6, #0
	beq.n	.L_081593a8
	lsls	r3, r5, #1
	adds	r3, r3, r5
	ldr	r7, [sp, #88]
	lsls	r3, r3, #7
	adds	r3, r3, r5
	movs	r0, #140
	lsls	r3, r3, #1
	adds	r7, #1
	lsls	r0, r0, #1
	str	r3, [sp, #72]
	add	r0, sp
	lsls	r1, r7, #1
	mov	sl, r0
	add	r6, sp, #148
	mov	fp, r1
.L_08159332:
	mov	r2, r9
	lsls	r5, r2, #9
	adds	r0, r5, #0
	bl	sub_08002096
	lsls	r3, r0, #1
	adds	r3, r3, r0
	mov	r4, sl
	lsls	r3, r3, #5
	str	r3, [r4, #0]
	adds	r0, r5, #0
	bl	sub_08002090
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #5
	negs	r3, r3
	mov	r5, sl
	str	r3, [r5, #8]
	adds	r1, r6, #0
	mov	r0, sl
	bl	sub_0815e1ec
	ldr	r2, [r6, #0]
	movs	r0, #6
	ldrsh	r3, [r6, r0]
	asrs	r2, r2, #17
	ldr	r0, [pc, #608]
	adds	r2, #64
	adds	r3, #60
	mov	r1, fp
	str	r2, [r6, #0]
	str	r3, [r6, #4]
	subs	r1, #2
	ldrh	r1, [r0, r1]
	ldr	r4, [sp, #72]
	ldr	r5, [sp, #112]
	movs	r0, #184
	adds	r1, r4, r1
	lsls	r0, r0, #6
	adds	r1, r5, r1
	adds	r0, #16
	adds	r1, r1, r0
	lsrs	r0, r7, #31
	adds	r0, r7, r0
	asrs	r0, r0, #1
	mov	r4, fp
	subs	r2, r2, r0
	subs	r3, r3, r7
	str	r7, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #108]
	ldr	r5, [sp, #76]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2001
	add	r9, r0
	cmp	r9, r8
	bne.n	.L_08159332
.L_081593a8:
	movs	r0, #188
	bl	sub_0801314c
.L_081593ae:
	ldr	r3, [sp, #100]
	subs	r3, #151
	cmp	r3, #16
	bhi.n	.L_081594b6
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #100]
	movs	r4, #0
	mov	r8, r1
	cmp	r2, #151
	ble.n	.L_081593c6
	adds	r4, r2, #0
	subs	r4, #152
.L_081593c6:
	cmp	r4, #15
	ble.n	.L_081593cc
	movs	r4, #15
.L_081593cc:
	movs	r3, #1
	ldr	r5, [sp, #44]
	mov	r9, r3
	lsls	r3, r4, #1
	negs	r3, r3
	adds	r7, r3, #0
	adds	r3, r4, r5
	adds	r6, r4, #0
	adds	r3, #1
	adds	r7, #30
	adds	r6, #49
	mov	sl, r3
.L_081593e4:
	mov	r0, r9
	adds	r3, r4, r0
	cmp	r3, #15
	bgt.n	.L_08159406
	movs	r2, #1
	movs	r3, #16
	subs	r3, r3, r0
	str	r2, [sp, #4]
	str	r4, [sp, #8]
	str	r7, [sp, #0]
	ldr	r0, [sp, #108]
	mov	r1, sl
	adds	r2, r6, #0
	ldr	r5, [sp, #92]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9c02
.L_08159406:
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	subs	r7, #2
	adds	r6, #1
	add	sl, r0
	cmp	r1, #10
	bne.n	.L_081593e4
	movs	r2, #0
	mov	r3, r8
	mov	r9, r2
	cmp	r3, #0
	beq.n	.L_08159452
	lsls	r2, r4, #1
	movs	r3, #32
	adds	r6, r4, #0
	subs	r3, r3, r2
	adds	r6, #48
	mov	sl, r3
	movs	r7, #1
.L_0815942e:
	ldr	r2, [sp, #44]
	mov	r3, r9
	mov	r5, sl
	adds	r1, r2, r4
	str	r5, [sp, #0]
	str	r4, [sp, #8]
	adds	r3, #16
	str	r7, [sp, #4]
	ldr	r0, [sp, #108]
	adds	r2, r6, #0
	ldr	r5, [sp, #92]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2001
	add	r9, r0
	ldr	r4, [sp, #8]
	cmp	r9, r8
	bne.n	.L_0815942e
.L_08159452:
	movs	r1, #19
	movs	r0, #188
	bl	sub_081963ec
	movs	r6, #96
	movs	r1, #32
	str	r1, [sp, #0]
	str	r6, [sp, #4]
	movs	r2, #192
	ldr	r5, [sp, #24]
	lsls	r2, r2, #18
	ldr	r3, [sp, #112]
	adds	r2, #188
	ldr	r4, [r2, #0]
	movs	r2, #224
	subs	r5, #56
	lsls	r2, r2, #3
	ldr	r0, [sp, #108]
	mov	r8, r1
	adds	r1, r3, r2
	movs	r2, #32
	adds	r3, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r1, #23
	movs	r0, #188
	bl	sub_081963ec
	mov	r3, r8
	str	r3, [sp, #0]
	str	r6, [sp, #4]
	ldr	r2, [sp, #112]
	movs	r6, #192
	lsls	r6, r6, #18
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r6, #188
	adds	r1, r2, r3
	ldr	r4, [r6, #0]
	ldr	r0, [sp, #108]
	movs	r2, #64
	adds	r3, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
.L_081594b6:
	ldr	r4, [sp, #112]
	movs	r5, #240
	lsls	r5, r5, #7
	adds	r5, #232
	adds	r2, r4, r5
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r6, [sp, #24]
	ldr	r0, [sp, #20]
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #100]
	adds	r6, #20
	adds	r0, #4
	adds	r1, #2
	adds	r2, #1
	str	r6, [sp, #24]
	str	r0, [sp, #20]
	str	r1, [sp, #16]
	str	r2, [sp, #100]
.L_081594e2:
	ldr	r3, [sp, #100]
	cmp	r3, #170
	beq.n	.L_08159520
	ldr	r5, [sp, #16]
	movs	r4, #2
	str	r4, [sp, #88]
	str	r5, [sp, #84]
	cmp	r3, #16
	bne.n	.L_081594fa
	movs	r0, #140
	bl	sub_081c0010
.L_081594fa:
	ldr	r6, [sp, #100]
	cmp	r6, #132
	bne.n	.L_08159506
	movs	r0, #131
	bl	sub_081c0010
.L_08159506:
	ldr	r0, [sp, #100]
	cmp	r0, #151
	bne.n	.L_08159512
	movs	r0, #145
	bl	sub_081c0010
.L_08159512:
	ldr	r3, [pc, #188]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08159520
	b.n	.L_08159018
.L_08159520:
	ldr	r1, [sp, #116]
	movs	r3, #0
	str	r3, [r1, #16]
	ldr	r0, [pc, #172]
	bl	sub_08014644
	bl	sub_0814cca8
	movs	r4, #240
	ldr	r2, [sp, #112]
	lsls	r4, r4, #7
	adds	r4, #240
	adds	r3, r2, r4
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #130
	movs	r0, #9
	movs	r2, #1
	bl	sub_08152404
	ldr	r0, [pc, #136]
	ldr	r1, [pc, #136]
	movs	r2, #1
	movs	r3, #0
	bl	.L_08157cf4
	ldr	r5, [sp, #112]
	movs	r6, #224
	lsls	r6, r6, #3
	adds	r1, r5, r6
	ldr	r0, [pc, #124]
	movs	r2, #1
	movs	r3, #1
	bl	.L_08157cf4
	movs	r2, #206
	lsls	r2, r2, #7
	adds	r1, r5, r2
	movs	r3, #0
	movs	r2, #1
	ldr	r0, [pc, #108]
	bl	.L_08157cf4
	ldr	r0, [pc, #104]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #100]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2200
	ldr	r1, [sp, #104]
	movs	r3, #0
	ldr	r0, [pc, #88]
	bl	.L_08157cf4
	ldr	r3, [pc, #40]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r4, #128
	movs	r3, #160
	movs	r5, #0
	lsls	r3, r3, #15
	lsls	r4, r4, #15
	str	r3, [sp, #68]
	movs	r2, #192
	str	r4, [sp, #64]
	str	r5, [sp, #60]
	str	r5, [sp, #56]
	ldr	r3, [pc, #56]
	lsls	r2, r2, #2
	mov	r9, r5
	movs	r1, #0
	adds	r2, #142
	b.n	.L_081595f8
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x08197410
	.4byte 0x03001150
	.4byte 0x0813baed
	.4byte 0x00000192
	.4byte 0x02010000
	.4byte 0x00000194
	.4byte 0x00000128
	.4byte 0x00000193
	.4byte 0x03000730
	.4byte 0x00000135
	.2byte 0x0c70
	.2byte 0x0201
.L_081595f8:
	movs	r6, #1
	add	r9, r6
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r9, r2
	bne.n	.L_081595f8
	ldr	r5, [sp, #112]
	movs	r0, #0
	mov	r9, r0
.L_0815960a:
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	subs	r3, #16
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #15
	movs	r1, #1
	ands	r3, r0
	add	r9, r1
	negs	r3, r3
	mov	r2, r9
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #16
	bne.n	.L_0815960a
	ldr	r3, [sp, #112]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r5, [sp, #112]
	movs	r6, #238
	lsls	r6, r6, #7
	adds	r6, #132
	adds	r2, r5, r6
	movs	r3, #50
	movs	r0, #145
	str	r3, [r2, #0]
	bl	sub_081c0010
	movs	r0, #0
	str	r0, [sp, #100]
.L_0815965c:
	ldr	r1, [sp, #100]
	cmp	r1, #20
	bne.n	.L_08159674
	ldr	r2, [sp, #112]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #876]
	movs	r2, #1
	movs	r3, #0
	bl	.L_08157cf4
.L_08159674:
	ldr	r4, [sp, #100]
	subs	r4, #80
	str	r4, [sp, #52]
	cmp	r4, #59
	bhi.n	.L_0815968e
	ldr	r5, [sp, #100]
	movs	r3, #7
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L_0815968e
	movs	r0, #134
	bl	sub_081c0010
.L_0815968e:
	ldr	r6, [sp, #100]
	cmp	r6, #120
	bne.n	.L_0815969a
	movs	r0, #134
	bl	sub_081180e8
.L_0815969a:
	movs	r0, #8
	movs	r4, #0
	mov	sl, r0
.L_081596a0:
	ldr	r1, [sp, #100]
	lsls	r2, r4, #7
	cmp	r1, sl
	bge.n	.L_081596aa
	b.n	.L_081597b2
.L_081596aa:
	adds	r3, r2, #0
	adds	r3, #17
	cmp	r1, r3
	bge.n	.L_081597b2
	movs	r3, #112
	mov	fp, r3
	adds	r3, r2, #0
	adds	r5, r2, #0
	adds	r3, #9
	adds	r5, #12
	cmp	r1, r3
	blt.n	.L_081596e6
	cmp	r1, r5
	bge.n	.L_081596ec
	ldr	r2, [sp, #112]
	movs	r3, #48
	str	r3, [sp, #0]
	movs	r3, #224
	mov	r6, fp
	lsls	r3, r3, #3
	adds	r1, r2, r3
	str	r6, [sp, #4]
	str	r4, [sp, #8]
	ldr	r0, [sp, #108]
	movs	r2, #36
	movs	r3, #0
	ldr	r6, [sp, #92]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x9c02
.L_081596e6:
	ldr	r0, [sp, #100]
	cmp	r0, r5
	blt.n	.L_08159716
.L_081596ec:
	ldr	r1, [sp, #100]
	mov	r3, sl
	adds	r3, #8
	cmp	r1, r3
	bge.n	.L_08159716
	movs	r3, #48
	ldr	r2, [sp, #112]
	str	r3, [sp, #0]
	movs	r3, #112
	str	r3, [sp, #4]
	movs	r3, #224
	lsls	r3, r3, #5
	adds	r1, r2, r3
	str	r4, [sp, #8]
	ldr	r0, [sp, #108]
	movs	r2, #36
	movs	r3, #0
	ldr	r5, [sp, #92]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9c02
.L_08159716:
	ldr	r6, [sp, #100]
	mov	r3, sl
	adds	r3, #2
	cmp	r6, r3
	bne.n	.L_081597b2
	ldr	r7, [pc, #696]
	movs	r0, #0
	mov	r8, r0
	mov	r9, r0
.L_08159728:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	bne.n	.L_08159794
	str	r4, [sp, #8]
	bl	sub_08014878
	movs	r6, #192
	lsls	r6, r6, #2
	adds	r6, #255
	ands	r6, r0
	bl	sub_08014878
	movs	r5, #254
	ldr	r1, [pc, #668]
	lsls	r5, r5, #7
	movs	r2, #60
	adds	r5, #255
	lsls	r3, r2, #16
	ands	r5, r0
	mov	r0, fp
	adds	r5, r5, r1
	str	r3, [r7, #0]
	lsls	r3, r0, #16
	str	r3, [r7, #4]
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r6, #32
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #7
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	movs	r1, #1
	movs	r2, #128
	adds	r3, #32
	add	r8, r1
	lsls	r2, r2, #2
	str	r3, [r7, #24]
	ldr	r4, [sp, #8]
	cmp	r8, r2
	beq.n	.L_081597a4
.L_08159794:
	movs	r5, #192
	movs	r3, #1
	lsls	r5, r5, #2
	add	r9, r3
	adds	r5, #142
	adds	r7, #28
	cmp	r9, r5
	bne.n	.L_08159728
.L_081597a4:
	ldr	r6, [sp, #112]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #168
	adds	r2, r6, r0
	movs	r3, #8
	str	r3, [r2, #0]
.L_081597b2:
	movs	r1, #128
	adds	r4, #1
	add	sl, r1
	cmp	r4, #1
	beq.n	.L_081597be
	b.n	.L_081596a0
.L_081597be:
	ldr	r2, [sp, #100]
	cmp	r2, #10
	bne.n	.L_081597f8
	ldr	r4, [sp, #120]
	movs	r3, #0
	mov	r9, r3
	ldr	r3, [r4, #20]
	cmp	r3, #0
	beq.n	.L_081597f8
	movs	r5, #36
.L_081597d2:
	ldr	r6, [sp, #120]
	movs	r3, #8
	ldrsh	r0, [r5, r6]
	movs	r2, #5
	str	r3, [sp, #0]
	movs	r1, #7
	mov	r3, r9
	bl	sub_0814cd48
	ldrsh	r0, [r5, r6]
	movs	r1, #4
	bl	sub_08118088
	movs	r3, #1
	add	r9, r3
	ldr	r3, [r6, #20]
	adds	r5, #2
	cmp	r9, r3
	bne.n	.L_081597d2
.L_081597f8:
	ldr	r4, [sp, #52]
	cmp	r4, #63
	bls.n	.L_08159800
	b.n	.L_08159a60
.L_08159800:
	ldr	r6, [pc, #480]
	movs	r5, #0
	mov	r9, r5
	movs	r5, #80
.L_08159808:
	ldr	r0, [sp, #100]
	cmp	r0, r5
	bne.n	sub_0815981a
	movs	r1, #128
	ldr	r0, [sp, #108]
	lsls	r1, r1, #7
	ldr	r2, [pc, #464]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x2101
	add	r9, r1
	mov	r2, r9
	adds	r5, #8
	cmp	r2, #6
	bne.n	.L_08159808
	movs	r3, #0
	movs	r4, #3
	movs	r5, #80
	mov	r9, r3
	mov	fp, r4
	mov	sl, r5
.L_08159832:
	ldr	r0, [sp, #100]
	mov	r6, r9
	lsls	r3, r6, #1
	cmp	r0, sl
	bge.n	.L_0815983e
	b.n	.L_08159974
.L_0815983e:
	adds	r3, #82
	cmp	r0, r3
	bge.n	.L_0815989e
	bl	sub_08014878
	mov	r1, fp
	adds	r6, r0, #0
	ands	r6, r1
	bl	sub_08014878
	mov	r2, fp
	adds	r5, r0, #0
	movs	r1, #19
	movs	r0, #188
	ands	r5, r2
	bl	sub_081963ec
	movs	r1, #3
	mov	r0, r9
	bl	sub_08002064
	lsls	r1, r0, #2
	adds	r1, r1, r0
	ldr	r3, [sp, #112]
	lsls	r1, r1, #4
	adds	r1, r1, r0
	lsls	r1, r1, #6
	adds	r1, r3, r1
	movs	r3, #72
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #192
	movs	r4, #224
	lsls	r0, r0, #18
	lsls	r4, r4, #3
	adds	r0, #188
	subs	r6, #3
	adds	r5, #32
	adds	r1, r1, r4
	adds	r2, r6, #0
	ldr	r4, [r0, #0]
	adds	r3, r5, #0
	ldr	r0, [sp, #108]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
.L_0815989e:
	ldr	r1, [sp, #100]
	cmp	r1, sl
	bne.n	.L_08159974
	ldr	r7, [pc, #308]
	movs	r2, #0
	mov	r8, r2
	movs	r4, #0
.L_081598ac:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	bne.n	.L_08159914
	str	r4, [sp, #8]
	bl	sub_08014878
	movs	r6, #192
	lsls	r6, r6, #2
	adds	r6, #255
	ands	r6, r0
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r5, r0, #0
	adds	r3, #255
	ands	r5, r3
	movs	r3, #128
	lsls	r3, r3, #13
	str	r3, [r7, #0]
	movs	r3, #160
	lsls	r3, r3, #15
	str	r3, [r7, #4]
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r6, #32
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #7
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #32
	str	r3, [r7, #24]
	movs	r3, #1
	add	r8, r3
	mov	r5, r8
	ldr	r4, [sp, #8]
	cmp	r5, #16
	beq.n	.L_08159922
.L_08159914:
	movs	r6, #192
	lsls	r6, r6, #2
	adds	r4, #1
	adds	r6, #142
	adds	r7, #28
	cmp	r4, r6
	bne.n	.L_081598ac
.L_08159922:
	ldr	r0, [sp, #112]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r3, r0, r1
	movs	r2, #8
	str	r2, [r3, #0]
	ldr	r2, [sp, #120]
	movs	r4, #0
	ldr	r3, [r2, #20]
	cmp	r3, #0
	beq.n	.L_08159974
	ldr	r6, [pc, #176]
	movs	r5, #36
.L_0815993e:
	str	r4, [sp, #8]
	bl	sub_08014878
	ldr	r2, [sp, #120]
	ldr	r4, [sp, #8]
	mov	r3, fp
	ands	r0, r3
	ldrb	r1, [r6, r0]
	ldrsh	r0, [r5, r2]
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r2, #5
	adds	r3, r4, #0
	bl	sub_0814cd48
	ldr	r1, [sp, #120]
	ldrsh	r0, [r5, r1]
	movs	r1, #4
	bl	sub_08118088
	ldr	r0, [sp, #120]
	ldr	r4, [sp, #8]
	ldr	r3, [r0, #20]
	adds	r4, #1
	adds	r5, #2
	cmp	r4, r3
	bne.n	.L_0815993e
.L_08159974:
	movs	r2, #1
	add	r9, r2
	movs	r1, #2
	mov	r3, r9
	add	sl, r1
	cmp	r3, #18
	beq.n	.L_08159984
	b.n	.L_08159832
.L_08159984:
	bl	sub_08014878
	movs	r6, #7
	ands	r6, r0
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r6, #8
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #16
	adds	r3, #72
	adds	r0, r5, #0
	mov	fp, r3
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	ldr	r4, [sp, #112]
	movs	r5, #206
	asrs	r3, r3, #16
	movs	r2, #32
	lsls	r5, r5, #7
	subs	r2, r2, r3
	adds	r0, r4, r5
	mov	r1, fp
	movs	r3, #24
	mov	sl, r2
	bl	sub_0818caa8
	ldr	r7, [pc, #12]
	movs	r6, #0
	mov	r8, r6
	mov	r9, r6
	b.n	.L_081599f0
	.4byte 0x00000124
	.4byte 0x02010c58
	.4byte 0xffffc000
	.4byte 0x03000260
	.4byte 0x10101010
	.2byte 0x855e
	.2byte 0x0819
.L_081599f0:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	bne.n	.L_08159a50
	bl	sub_08014878
	movs	r5, #63
	ands	r5, r0
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r6, r0, #0
	adds	r3, #255
	mov	r0, fp
	mov	r1, sl
	ands	r6, r3
	lsls	r3, r0, #16
	str	r3, [r7, #0]
	lsls	r3, r1, #16
	str	r3, [r7, #4]
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #64
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
	movs	r2, #1
	adds	r3, #16
	add	r8, r2
	str	r3, [r7, #24]
	mov	r3, r8
	cmp	r3, #4
	beq.n	.L_08159a60
.L_08159a50:
	movs	r5, #192
	movs	r4, #1
	lsls	r5, r5, #2
	add	r9, r4
	adds	r5, #142
	adds	r7, #28
	cmp	r9, r5
	bne.n	.L_081599f0
.L_08159a60:
	movs	r0, #188
	movs	r1, #31
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r3, [r3, #0]
	ldr	r5, [pc, #528]
	movs	r6, #0
	mov	r8, r3
	mov	r9, r6
.L_08159a78:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	ble.n	sub_08159ae0
	subs	r3, #1
	str	r3, [r5, #24]
	adds	r0, r5, #0
	movs	r1, #60
	movs	r2, #0
	bl	sub_08138086
	ldr	r3, [r5, #4]
	movs	r0, #240
	lsls	r0, r0, #15
	cmp	r3, r0
	ble.n	.L_08159aa4
	ldr	r3, [r5, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r5, #16]
	b.n	sub_08159ae0
.L_08159aa4:
	ldr	r2, [r5, #0]
	ldr	r1, [pc, #480]
	cmp	r2, r1
	bhi.n	sub_08159ae0
	cmp	r3, #0
	blt.n	sub_08159ae0
	ldr	r0, [r5, #24]
	asrs	r6, r2, #16
	asrs	r7, r3, #16
	cmp	r0, #0
	bge.n	.L_08159abc
	adds	r0, #15
.L_08159abc:
	asrs	r0, r0, #4
	adds	r0, #1
	ldr	r2, [pc, #456]
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #104]
	subs	r3, r7, r0
	adds	r1, r2, r1
	lsrs	r2, r0, #31
	adds	r2, r0, r2
	asrs	r2, r2, #1
	str	r0, [sp, #0]
	subs	r2, r6, r2
	str	r4, [sp, #4]
	ldr	r0, [sp, #108]
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x24c0
	movs	r3, #1
	lsls	r4, r4, #2
	add	r9, r3
	adds	r4, #142
	adds	r5, #28
	cmp	r9, r4
	bne.n	.L_08159a78
	movs	r0, #188
	bl	sub_0801314c
	ldr	r5, [sp, #100]
	cmp	r5, #15
	ble.n	.L_08159b4e
	cmp	r5, #32
	bne.n	.L_08159b0a
	ldr	r0, [pc, #396]
	movs	r6, #128
	lsls	r6, r6, #11
	str	r6, [sp, #60]
	str	r0, [sp, #56]
.L_08159b0a:
	ldr	r1, [sp, #100]
	cmp	r1, #31
	ble.n	.L_08159b44
	ldr	r2, [sp, #68]
	ldr	r3, [sp, #60]
	ldr	r6, [sp, #60]
	ldr	r4, [sp, #64]
	ldr	r5, [sp, #56]
	adds	r2, r2, r3
	lsls	r3, r3, #4
	subs	r3, r3, r6
	adds	r4, r4, r5
	lsls	r3, r3, #2
	str	r2, [sp, #68]
	str	r4, [sp, #64]
	cmp	r3, #0
	bge.n	.L_08159b2e
	adds	r3, #63
.L_08159b2e:
	ldr	r0, [sp, #56]
	asrs	r3, r3, #6
	str	r3, [sp, #60]
	lsls	r3, r0, #4
	subs	r3, r3, r0
	lsls	r3, r3, #2
	cmp	r3, #0
	bge.n	.L_08159b40
	adds	r3, #63
.L_08159b40:
	asrs	r3, r3, #6
	str	r3, [sp, #56]
.L_08159b44:
	movs	r0, #0
	ldr	r1, [sp, #68]
	ldr	r2, [sp, #64]
	bl	sub_0816442c
.L_08159b4e:
	ldr	r3, [sp, #100]
	subs	r3, #16
	cmp	r3, #127
	bhi.n	.L_08159c02
	ldr	r2, [sp, #68]
	movs	r1, #0
	asrs	r2, r2, #17
	mov	r9, r1
	mov	fp, r2
	movs	r7, #3
.L_08159b62:
	mov	r4, r9
	ands	r4, r7
	str	r4, [sp, #8]
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	ldr	r3, [pc, #276]
	ldr	r4, [sp, #8]
	adds	r6, r0, #0
	mov	sl, r3
	ldrb	r3, [r3, r4]
	lsls	r6, r6, #3
	asrs	r6, r6, #16
	lsrs	r3, r3, #1
	adds	r0, r5, #0
	add	r6, fp
	subs	r6, r6, r3
	bl	sub_08002090
	lsls	r5, r0, #2
	ldr	r4, [sp, #8]
	adds	r5, r5, r0
	ldr	r0, [pc, #248]
	lsls	r5, r5, #3
	ldrb	r3, [r0, r4]
	asrs	r5, r5, #16
	lsrs	r3, r3, #1
	subs	r5, r5, r3
	mov	r8, r0
	bl	sub_08014878
	ldr	r3, [pc, #236]
	ands	r0, r7
	ldrb	r2, [r3, r0]
	adds	r3, r7, #0
	orrs	r3, r2
	movs	r2, #1
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r0, #188
	bl	sub_08196404
	ldr	r4, [sp, #8]
	ldr	r2, [pc, #212]
	lsls	r3, r4, #1
	mov	r0, sl
	ldrh	r1, [r2, r3]
	ldrb	r3, [r0, r4]
	ldr	r2, [pc, #208]
	str	r3, [sp, #0]
	adds	r1, r1, r2
	mov	r2, r8
	ldrb	r3, [r2, r4]
	adds	r5, #56
	str	r3, [sp, #4]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r4, [r3, #0]
	ldr	r0, [sp, #108]
	adds	r3, r5, #0
	adds	r2, r6, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r4, #1
	add	r9, r4
	mov	r5, r9
	cmp	r5, #3
	bne.n	.L_08159b62
.L_08159c02:
	ldr	r6, [sp, #100]
	cmp	r6, #31
	bgt.n	.L_08159c12
	movs	r0, #8
	movs	r1, #16
	bl	.L_08158ce0
	b.n	.L_08159c1a
.L_08159c12:
	movs	r0, #4
	movs	r1, #4
	bl	.L_08158ce0
.L_08159c1a:
	bl	sub_081434f8
	movs	r1, #240
	ldr	r0, [sp, #112]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r0, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #100]
	adds	r2, #1
	str	r2, [sp, #100]
	cmp	r2, #128
	beq.n	.L_08159c3e
	b.n	.L_0815965c
.L_08159c3e:
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #96]
	bl	sub_08014644
	movs	r0, #0
	ldr	r1, [sp, #68]
	ldr	r2, [sp, #64]
	bl	sub_0816467c
	movs	r6, #238
	ldr	r4, [sp, #112]
	lsls	r6, r6, #7
	movs	r3, #0
	adds	r6, #220
	mov	r9, r3
	adds	r5, r4, r6
.L_08159c62:
	ldmia	r5!, {r0}
	bl	sub_08020048
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	cmp	r1, #9
	bne.n	.L_08159c62
	bl	sub_08143bb8
	add	sp, #324
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02010c58
	.4byte 0x007effff
	.4byte 0x08197410
	.4byte 0xffffc000
	.4byte 0x08197492
	.4byte 0x08197498
	.4byte 0x08198562
	.4byte 0x08197486
	.4byte 0x02010000
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #60
	str	r0, [sp, #32]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #92]
	str	r0, [sp, #28]
	movs	r0, #1
	ldr	r1, [r5, #96]
	str	r1, [sp, #24]
	bl	sub_081435e0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, #48
	strh	r3, [r2, #0]
	ldr	r2, [sp, #28]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #40]
	movs	r2, #1
	movs	r3, #1
	bl	.L_08157cf4
	movs	r1, #35
	movs	r0, #104
	bl	sub_081963ec
	ldr	r5, [r5, #104]
	ldr	r6, [sp, #32]
	str	r5, [sp, #20]
	movs	r4, #36
	ldrsh	r0, [r6, r4]
	add	r6, sp, #48
	adds	r1, r6, #0
	b.n	.L_08159d18
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000000
	.2byte 0x014d
	.2byte 0x0000
.L_08159d18:
	bl	sub_0815e20c
	ldr	r0, [sp, #32]
	add	r5, sp, #36
	ldr	r3, [r0, #20]
	lsls	r3, r3, #1
	adds	r3, #34
	ldrsh	r0, [r0, r3]
	adds	r1, r5, #0
	bl	sub_0815e20c
	ldr	r1, [r6, #0]
	ldr	r3, [r5, #0]
	movs	r4, #239
	subs	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r1, r1, r3
	movs	r2, #128
	movs	r3, #64
	subs	r3, r3, r1
	lsls	r2, r2, #19
	adds	r2, #40
	lsls	r3, r3, #8
	str	r1, [r6, #0]
	str	r3, [r2, #0]
	ldr	r3, [sp, #28]
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r6, [sp, #28]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #132
	adds	r2, r6, r0
	movs	r3, #0
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #688]
	bl	sub_080145a8
	ldr	r1, [sp, #32]
	ldr	r2, [pc, #684]
	ldr	r3, [r1, #24]
	ldrb	r2, [r2, r3]
	str	r2, [sp, #16]
	ldr	r3, [sp, #16]
	movs	r2, #0
	mov	sl, r2
	cmp	r3, #0
	beq.n	.L_08159e12
	ldr	r1, [pc, #668]
	ldr	r3, [sp, #28]
.L_08159d88:
	str	r1, [r3, #4]
	str	r2, [r3, #16]
	ldr	r6, [sp, #16]
	movs	r4, #1
	add	sl, r4
	adds	r3, #28
	cmp	sl, r6
	bne.n	.L_08159d88
	movs	r0, #0
	mov	sl, r0
	cmp	r6, #0
	beq.n	.L_08159e12
	ldr	r1, [pc, #644]
	movs	r7, #0
	mov	r9, r1
.L_08159da6:
	movs	r3, #140
	mov	r4, sl
	muls	r4, r3
	ldr	r0, [pc, #636]
	adds	r3, r4, #0
	movs	r2, #0
	adds	r3, r7, r3
	mov	r8, r2
	mov	r6, r9
	adds	r5, r3, r0
.L_08159dba:
	ldr	r2, [pc, #628]
	mov	r1, sl
	ldrsb	r2, [r2, r1]
	ldrb	r3, [r6, #0]
	adds	r3, r3, r2
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldrb	r3, [r6, #1]
	adds	r6, #2
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r1, #96
	bl	sub_0800206c
	subs	r0, #48
	lsls	r0, r0, #10
	str	r0, [r5, #12]
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	adds	r3, #32
	negs	r3, r3
	lsls	r3, r3, #11
	str	r3, [r5, #16]
	movs	r2, #1
	movs	r3, #32
	str	r3, [r5, #8]
	add	r8, r2
	movs	r3, #0
	str	r3, [r5, #24]
	mov	r3, r8
	adds	r5, #28
	cmp	r3, #21
	bne.n	.L_08159dba
	ldr	r6, [sp, #16]
	movs	r4, #224
	lsls	r4, r4, #1
	add	sl, r2
	adds	r7, r7, r4
	cmp	sl, r6
	bne.n	.L_08159da6
.L_08159e12:
	ldr	r2, [sp, #16]
	ldr	r1, [pc, #540]
	subs	r2, #1
	str	r2, [sp, #12]
	movs	r4, #65
	ldrb	r3, [r1, r2]
	movs	r0, #0
	negs	r4, r4
	mov	fp, r0
	cmp	r3, r4
	bne.n	.L_08159e2a
	b.n	.L_08159ffc
.L_08159e2a:
	ldrb	r3, [r1, r2]
	adds	r3, #48
	cmp	fp, r3
	bne.n	.L_08159e38
	movs	r0, #133
	bl	sub_081180e8
.L_08159e38:
	ldr	r0, [sp, #16]
	movs	r6, #0
	mov	sl, r6
	cmp	r0, #0
	bne.n	.L_08159e44
	b.n	.L_08159fca
.L_08159e44:
	ldr	r1, [sp, #28]
	mov	r9, r1
.L_08159e48:
	ldr	r5, [pc, #488]
	mov	r2, sl
	ldrb	r3, [r5, r2]
	adds	r3, #18
	cmp	fp, r3
	bne.n	.L_08159e76
	cmp	r2, #0
	bne.n	.L_08159e60
	movs	r0, #134
	bl	sub_081180e8
	b.n	.L_08159e66
.L_08159e60:
	movs	r0, #134
	bl	sub_081c0010
.L_08159e66:
	ldr	r3, [sp, #28]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	movs	r3, #4
	str	r3, [r2, #0]
	ldr	r5, [pc, #444]
.L_08159e76:
	mov	r6, sl
	ldrb	r3, [r5, r6]
	adds	r3, #18
	cmp	fp, r3
	blt.n	.L_08159f2a
	lsls	r6, r6, #2
	str	r6, [sp, #8]
	movs	r0, #0
	mov	r8, r0
.L_08159e88:
	ldr	r3, [sp, #8]
	ldr	r1, [pc, #416]
	add	r3, sl
	lsls	r3, r3, #2
	add	r3, sl
	add	r3, r8
	lsls	r2, r3, #3
	subs	r2, r2, r3
	lsls	r2, r2, #2
	adds	r7, r2, r1
	mov	r0, r8
	movs	r1, #5
	bl	sub_08002064
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r1, #96
	ldr	r0, [r7, #24]
	bl	sub_08002054
	movs	r1, #3
	bl	sub_08002064
	ldr	r2, [pc, #384]
	adds	r5, r5, r0
	lsls	r3, r5, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #28]
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r1, r2, r1
	adds	r3, #60
	adds	r1, r1, r3
	ldr	r3, [pc, #368]
	movs	r4, #2
	ldrsh	r2, [r7, r4]
	ldrb	r6, [r3, r5]
	lsrs	r3, r6, #1
	subs	r2, r2, r3
	movs	r0, #6
	ldrsh	r3, [r7, r0]
	ldr	r0, [pc, #356]
	ldrb	r4, [r0, r5]
	str	r6, [sp, #0]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #24]
	ldr	r4, [sp, #20]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	lsls	r2, r2, #7
	adds	r0, r7, #0
	movs	r1, #64
	bl	sub_08138086
	ldr	r3, [r7, #24]
	ldr	r2, [r7, #8]
	adds	r3, r3, r2
	str	r3, [r7, #24]
	cmp	r2, #1
	ble.n	.L_08159f14
	movs	r3, #1
	mov	r6, fp
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_08159f14
	subs	r3, r2, #1
	str	r3, [r7, #8]
.L_08159f14:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	cmp	r1, #21
	bne.n	.L_08159e88
	ldr	r5, [pc, #276]
	mov	r2, sl
	ldrb	r3, [r5, r2]
	adds	r3, #18
	cmp	fp, r3
	bge.n	.L_08159f82
.L_08159f2a:
	mov	r4, sl
	ldrb	r3, [r5, r4]
	cmp	fp, r3
	blt.n	sub_08159f56
	ldr	r3, [pc, #252]
	mov	r0, r9
	ldrsb	r2, [r3, r4]
	ldr	r4, [sp, #28]
	movs	r6, #6
	ldrsh	r3, [r0, r6]
	movs	r1, #34
	movs	r6, #224
	str	r1, [sp, #0]
	lsls	r6, r6, #3
	movs	r1, #62
	str	r1, [sp, #4]
	adds	r2, #47
	adds	r1, r4, r6
	ldr	r0, [sp, #24]
	ldr	r4, [sp, #20]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x464e
	ldr	r3, [r6, #4]
	ldr	r2, [r6, #16]
	mov	r0, sl
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldrb	r3, [r5, r0]
	cmp	fp, r3
	ble.n	.L_08159f70
	movs	r1, #128
	lsls	r1, r1, #9
	adds	r3, r2, r1
	str	r3, [r6, #16]
.L_08159f70:
	mov	r2, r9
	ldr	r3, [r2, #4]
	movs	r2, #200
	lsls	r2, r2, #14
	cmp	r3, r2
	ble.n	.L_08159f80
	mov	r3, r9
	str	r2, [r3, #4]
.L_08159f80:
	ldr	r5, [pc, #176]
.L_08159f82:
	mov	r4, sl
	ldrb	r3, [r5, r4]
	adds	r3, #18
	cmp	fp, r3
	bne.n	.L_08159fba
	ldr	r0, [sp, #32]
	movs	r6, #0
	ldr	r3, [r0, #20]
	mov	r8, r6
	cmp	r3, #0
	beq.n	.L_08159fba
	movs	r6, #8
	movs	r5, #36
.L_08159f9c:
	ldr	r1, [sp, #32]
	mov	r3, r8
	ldrsh	r0, [r5, r1]
	movs	r1, #7
	movs	r2, #5
	str	r6, [sp, #0]
	bl	sub_0814cd48
	ldr	r4, [sp, #32]
	movs	r3, #1
	add	r8, r3
	ldr	r3, [r4, #20]
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_08159f9c
.L_08159fba:
	ldr	r1, [sp, #16]
	movs	r0, #1
	movs	r6, #28
	add	sl, r0
	add	r9, r6
	cmp	sl, r1
	beq.n	.L_08159fca
	b.n	.L_08159e48
.L_08159fca:
	movs	r1, #4
	movs	r0, #2
	bl	.L_08158ce0
	bl	sub_081434f8
	movs	r4, #240
	ldr	r3, [sp, #28]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [pc, #72]
	ldr	r2, [sp, #12]
	movs	r6, #1
	ldrb	r3, [r1, r2]
	add	fp, r6
	adds	r3, #65
	cmp	fp, r3
	beq.n	.L_08159ffc
	b.n	.L_08159e2a
.L_08159ffc:
	ldr	r0, [pc, #28]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #60
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x08143001
	.4byte 0x081985b0
	.4byte 0xffc00000
	.4byte 0x08198566
	.4byte 0x02010000
	.4byte 0x081985a6
	.4byte 0x081985ab
	.4byte 0x081974bc
	.4byte 0x0819749e
	.2byte 0x74ad
	.2byte 0x0819
	push	{lr}
	ldr	r3, [r0, #24]
	cmp	r3, #0
	bne.n	.L_0815a054
	movs	r1, #6
	bl	sub_0815a110
	b.n	.L_0815a066
.L_0815a054:
	cmp	r3, #1
	bne.n	.L_0815a060
	movs	r1, #7
	bl	sub_0815a110
	b.n	.L_0815a066
.L_0815a060:
	movs	r1, #8
	bl	sub_0815a110
.L_0815a066:
	pop	{pc}
	push	{lr}
	movs	r1, #0
	bl	sub_0815a110
	pop	{pc}
