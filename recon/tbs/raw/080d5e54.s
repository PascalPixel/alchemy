.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_080049e8, 0x080049e8
	.set sub_08004a5c, 0x08004a5c
	.set sub_08004bd4, 0x08004bd4
	.set sub_08004c1c, 0x08004c1c
	.set sub_08004c6c, 0x08004c6c
	.set sub_08004cb4, 0x08004cb4
	.set sub_080051d8, 0x080051d8
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080b5098, 0x080b5098
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080cef64, 0x080cef64
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e38b8, 0x080e38b8
	.set sub_080e3944, 0x080e3944
	.set sub_080f9010, 0x080f9010
	.global Unnamed_080d5e54
	.global Func_080d5e54
	.thumb_func
Unnamed_080d5e54:
Func_080d5e54:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #576]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	sub	sp, #172
	str	r1, [sp, #76]
	ldr	r3, [r3, #0]
	str	r3, [sp, #72]
	ldr	r2, [r2, #8]
	str	r2, [sp, #64]
	ldr	r2, [pc, #560]
	adds	r5, r1, r2
	str	r0, [r5, #0]
	movs	r0, #0
	bl	sub_080cd594
	ldr	r0, [pc, #552]
	ldr	r1, [sp, #76]
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r2, #0
	ldr	r1, [sp, #64]
	ldr	r0, [pc, #540]
	movs	r3, #0
	bl	sub_080e0524
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #4]
	movs	r3, #1
	eors	r0, r3
	mov	r3, sp
	adds	r3, #80
	adds	r1, r3, #0
	str	r3, [sp, #60]
	bl	sub_080cef64
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5098
	ldr	r3, [r5, #0]
	ldr	r0, [r0, #0]
	mov	r9, r0
	movs	r5, #36
	ldrsh	r0, [r3, r5]
	bl	sub_080b5098
	ldr	r0, [r0, #0]
	ldr	r7, [pc, #492]
	str	r0, [sp, #56]
	movs	r0, #0
	mov	sl, r0
	mov	r8, r0
.L_080d5ece:
	bl	sub_08004458
	ldr	r3, [pc, #484]
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08004458
	mov	r1, r8
	str	r1, [r7, #0]
	movs	r5, #255
	ands	r5, r0
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r3, #20
	lsls	r3, r3, #16
	mov	r2, r8
	str	r3, [r7, #4]
	str	r2, [r7, #8]
	adds	r0, r6, #0
	bl	sub_08002322
	adds	r5, #128
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #12]
	mov	r3, r8
	str	r3, [r7, #16]
	adds	r0, r6, #0
	bl	sub_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	movs	r0, #1
	add	sl, r0
	asrs	r3, r3, #5
	mov	r5, r8
	mov	r1, sl
	str	r3, [r7, #20]
	str	r5, [r7, #24]
	adds	r7, #28
	cmp	r1, #64
	bne.n	.L_080d5ece
	ldr	r3, [sp, #76]
	movs	r5, #239
	lsls	r5, r5, #7
	adds	r2, r3, r5
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r0, [sp, #76]
	ldr	r1, [pc, #388]
	movs	r3, #75
	adds	r2, r0, r1
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #380]
	bl	sub_080041d8
	mov	r5, r9
	movs	r2, #160
	ldr	r3, [r5, #8]
	add	r2, sp
	mov	fp, r2
	str	r3, [r2, #0]
	mov	r0, fp
	movs	r2, #0
	str	r2, [r0, #4]
	ldr	r3, [r5, #16]
	mov	r1, sp
	str	r3, [r0, #8]
	adds	r1, #148
	ldr	r3, [sp, #56]
	str	r1, [sp, #52]
	mov	r5, sp
	ldr	r0, [r3, #8]
	movs	r3, #180
	adds	r5, #136
	lsls	r3, r3, #15
	str	r0, [r1, #0]
	str	r3, [r1, #4]
	str	r2, [r1, #8]
	str	r5, [sp, #48]
	mov	r1, fp
	ldr	r3, [r1, #0]
	movs	r1, #40
	subs	r0, r0, r3
	bl	sub_080022ec
	str	r0, [r5, #0]
	ldr	r2, [sp, #52]
	mov	r5, fp
	ldr	r3, [r5, #4]
	ldr	r0, [r2, #4]
	movs	r1, #40
	subs	r0, r0, r3
	bl	sub_080022ec
	ldr	r1, [sp, #48]
	str	r0, [r1, #4]
	ldr	r2, [sp, #52]
	ldr	r3, [r5, #8]
	ldr	r0, [r2, #8]
	movs	r1, #40
	subs	r0, r0, r3
	bl	sub_080022ec
	ldr	r3, [sp, #48]
	movs	r5, #0
	str	r0, [r3, #8]
	mov	r9, r5
.L_080d5fb0:
	ldr	r3, [pc, #272]
	mov	r0, r9
	ldr	r5, [r3, #0]
	cmp	r0, #8
	bne.n	.L_080d5fc0
	movs	r0, #212
	bl	sub_080f9010
.L_080d5fc0:
	mov	r1, r9
	cmp	r1, #80
	bne.n	.L_080d5fcc
	movs	r0, #142
	bl	sub_080f9010
.L_080d5fcc:
	bl	sub_080049ac
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	sub_080051d8
	mov	r3, r9
	subs	r3, #30
	cmp	r3, #39
	bhi.n	.L_080d6000
	ldr	r5, [sp, #48]
	mov	r2, fp
	ldr	r3, [r2, #0]
	ldr	r2, [r5, #0]
	mov	r0, fp
	adds	r3, r3, r2
	str	r3, [r0, #0]
	ldr	r3, [r0, #4]
	ldr	r2, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r0, #4]
	ldr	r3, [r0, #8]
	ldr	r2, [r5, #8]
	adds	r3, r3, r2
	str	r3, [r0, #8]
.L_080d6000:
	mov	r0, fp
	bl	sub_08004cb4
	mov	r1, r9
	cmp	r1, #0
	bne.n	.L_080d6026
	ldr	r2, [sp, #76]
	ldr	r5, [pc, #152]
	adds	r3, r2, r5
	ldr	r3, [r3, #0]
	movs	r2, #1
	ldr	r0, [r3, #8]
	movs	r3, #1
	str	r1, [sp, #0]
	negs	r2, r2
	movs	r1, #7
	negs	r3, r3
	bl	sub_080d6888
.L_080d6026:
	mov	r0, r9
	cmp	r0, #24
	bne.n	.L_080d6046
	ldr	r1, [sp, #76]
	ldr	r2, [pc, #120]
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	movs	r2, #1
	ldr	r0, [r3, #8]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #0
	negs	r2, r2
	subs	r3, #1
	bl	sub_080d6888
.L_080d6046:
	mov	r0, r9
	negs	r0, r0
	mov	r1, r9
	movs	r5, #0
	lsls	r1, r1, #8
	str	r0, [sp, #44]
	ldr	r6, [pc, #96]
	mov	sl, r5
	lsls	r7, r0, #8
	mov	r8, r1
.L_080d605a:
	cmp	r9, sl
	bgt.n	.L_080d6060
	b.n	.L_080d616a
.L_080d6060:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	beq.n	.L_080d6068
	b.n	.L_080d616a
.L_080d6068:
	bl	sub_080049e8
	movs	r3, #3
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #1
	beq.n	.L_080d6092
	cmp	r3, #1
	bgt.n	.L_080d6080
	cmp	r3, #0
	beq.n	.L_080d608a
	b.n	.L_080d60d4
.L_080d6080:
	cmp	r3, #2
	beq.n	.L_080d609a
	cmp	r3, #3
	beq.n	.L_080d60c8
	b.n	.L_080d60d4
.L_080d608a:
	mov	r0, r8
	bl	sub_08004c1c
	b.n	.L_080d60d4
.L_080d6092:
	adds	r0, r7, #0
	bl	sub_08004bd4
	b.n	.L_080d60d4
.L_080d609a:
	adds	r0, r7, #0
	bl	sub_08004c6c
	b.n	.L_080d60d4
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x00000092
	.4byte 0x00000073
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0x00007784
	.4byte 0x080cd261
	.2byte 0x1e80
	.2byte 0x0300
.L_080d60c8:
	adds	r0, r7, #0
	bl	sub_08004bd4
	adds	r0, r7, #0
	bl	sub_08004c6c
.L_080d60d4:
	add	r5, sp, #112
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_080e3944
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	bl	sub_08004a5c
	ldr	r2, [r5, #8]
	cmp	r2, #249
	bgt.n	.L_080d60f4
	movs	r3, #250
	str	r3, [r5, #8]
	movs	r2, #250
.L_080d60f4:
	ldr	r3, [pc, #936]
	cmp	r2, r3
	ble.n	.L_080d60fe
	str	r3, [r5, #8]
	adds	r2, r3, #0
.L_080d60fe:
	adds	r3, r2, #0
	subs	r3, #250
	cmp	r3, #0
	bge.n	.L_080d6108
	adds	r3, #63
.L_080d6108:
	asrs	r3, r3, #6
	movs	r0, #9
	subs	r0, r0, r3
	lsls	r4, r0, #1
	ldr	r2, [pc, #912]
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #64]
	adds	r1, r3, r1
	lsrs	r3, r0, #31
	ldr	r2, [r5, #0]
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #72]
	ldr	r4, [sp, #80]
	bl	sub_080072f4
	adds	r0, r6, #0
	movs	r1, #60
	movs	r2, #0
	bl	sub_080e38b8
	mov	r3, sl
	adds	r3, #30
	cmp	r9, r3
	ble.n	.L_080d616a
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #12]
	negs	r2, r2
	asrs	r2, r2, #8
	ldr	r1, [r6, #4]
	adds	r3, r3, r2
	negs	r1, r1
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	asrs	r1, r1, #8
	ldr	r0, [r6, #8]
	adds	r3, r3, r1
	negs	r0, r0
	str	r3, [r6, #16]
	ldr	r3, [r6, #20]
	asrs	r0, r0, #8
	adds	r3, r3, r0
	str	r3, [r6, #20]
.L_080d616a:
	ldr	r5, [sp, #44]
	movs	r1, #1
	lsls	r3, r5, #5
	mov	r0, r9
	add	sl, r1
	adds	r7, r7, r3
	mov	r2, sl
	lsls	r3, r0, #5
	add	r8, r3
	adds	r6, #28
	cmp	r2, #32
	beq.n	.L_080d6184
	b.n	.L_080d605a
.L_080d6184:
	cmp	r0, #82
	ble.n	.L_080d61c6
	add	r6, sp, #124
	movs	r3, #0
	str	r3, [r6, #0]
	lsls	r0, r0, #10
	bl	sub_08002322
	movs	r5, #0
	lsls	r0, r0, #2
	str	r5, [r6, #8]
	add	r5, sp, #112
	str	r0, [r6, #4]
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_080e3944
	ldr	r2, [r5, #0]
	movs	r1, #20
	asrs	r2, r2, #1
	str	r2, [r5, #0]
	ldr	r3, [r5, #4]
	str	r1, [sp, #0]
	movs	r1, #34
	str	r1, [sp, #4]
	ldr	r0, [sp, #60]
	subs	r2, #10
	ldr	r4, [r0, #4]
	subs	r3, #17
	ldr	r0, [sp, #72]
	ldr	r1, [sp, #76]
	bl	sub_080072f4
.L_080d61c6:
	ldr	r1, [sp, #76]
	ldr	r3, [pc, #732]
	movs	r5, #1
	adds	r2, r1, r3
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	add	r9, r5
	bl	sub_080030f8
	mov	r0, r9
	cmp	r0, #98
	beq.n	.L_080d61e2
	b.n	.L_080d5fb0
.L_080d61e2:
	movs	r1, #0
	ldr	r5, [pc, #708]
	mov	sl, r1
	movs	r6, #255
.L_080d61ea:
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #0]
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	adds	r3, #64
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	bl	sub_08004458
	movs	r3, #0
	ands	r0, r6
	str	r3, [r5, #24]
	subs	r0, #127
	movs	r2, #1
	movs	r3, #128
	lsls	r0, r0, #15
	add	sl, r2
	lsls	r3, r3, #2
	str	r0, [r5, #8]
	adds	r5, #28
	cmp	sl, r3
	bne.n	.L_080d61ea
	ldr	r0, [pc, #652]
	ldr	r1, [sp, #64]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	ldr	r1, [pc, #644]
	ldr	r0, [sp, #76]
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	movs	r2, #72
	movs	r5, #0
	lsls	r3, r3, #3
	negs	r2, r2
	mov	r9, r5
	cmp	r3, r2
	bne.n	.L_080d6248
	b.n	.L_080d64d6
.L_080d6248:
	mov	r3, sp
	mov	r5, sp
	ldr	r2, [sp, #52]
	adds	r3, #100
	adds	r5, #88
	adds	r1, r0, r1
	str	r3, [sp, #16]
	str	r5, [sp, #20]
	str	r1, [sp, #32]
	str	r2, [sp, #28]
.L_080d625c:
	ldr	r3, [pc, #600]
	ldr	r3, [r3, #0]
	str	r3, [sp, #40]
	bl	sub_080049ac
	ldr	r3, [sp, #40]
	adds	r3, #12
	adds	r1, r3, #0
	ldr	r0, [sp, #40]
	str	r3, [sp, #36]
	bl	sub_080051d8
	ldr	r5, [sp, #32]
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #3
	adds	r3, #40
	cmp	r9, r3
	blt.n	.L_080d628e
	ldr	r0, [sp, #28]
	movs	r1, #128
	ldr	r3, [r0, #4]
	lsls	r1, r1, #11
	adds	r3, r3, r1
	str	r3, [r0, #4]
.L_080d628e:
	ldr	r2, [sp, #28]
	ldr	r5, [sp, #16]
	ldr	r3, [r2, #0]
	str	r3, [r5, #0]
	ldr	r3, [r2, #4]
	mov	r1, r9
	str	r3, [r5, #4]
	lsls	r0, r1, #11
	bl	sub_08002322
	ldr	r5, [sp, #28]
	lsls	r2, r0, #2
	ldr	r3, [r5, #8]
	adds	r2, r2, r0
	lsls	r2, r2, #3
	ldr	r0, [sp, #16]
	adds	r3, r3, r2
	str	r3, [r0, #8]
	add	r2, sp, #88
	adds	r1, r2, #0
	ldr	r0, [sp, #16]
	bl	sub_080e3944
	ldr	r2, [sp, #88]
	ldr	r3, [sp, #92]
	movs	r1, #20
	asrs	r2, r2, #1
	str	r1, [sp, #0]
	movs	r1, #34
	str	r2, [sp, #88]
	subs	r3, #17
	str	r1, [sp, #4]
	subs	r2, #10
	ldr	r4, [sp, #80]
	ldr	r0, [sp, #72]
	ldr	r1, [sp, #76]
	bl	sub_080072f4
	movs	r3, #0
	str	r3, [sp, #68]
	ldr	r5, [sp, #32]
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L_080d62ea
	b.n	.L_080d6484
.L_080d62ea:
	ldr	r1, [pc, #456]
	ldr	r0, [sp, #76]
	movs	r2, #36
	adds	r1, r0, r1
	movs	r3, #0
	str	r1, [sp, #24]
	str	r2, [sp, #12]
	str	r3, [sp, #8]
.L_080d62fa:
	ldr	r5, [sp, #24]
	ldr	r1, [sp, #12]
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r1]
	bl	sub_080b5098
	ldr	r3, [sp, #68]
	lsls	r3, r3, #3
	ldr	r5, [r0, #0]
	mov	r8, r3
	bl	sub_080049ac
	ldr	r0, [sp, #40]
	ldr	r1, [sp, #36]
	bl	sub_080051d8
	ldr	r3, [r5, #8]
	mov	r0, fp
	str	r3, [r0, #0]
	movs	r3, #160
	lsls	r3, r3, #14
	str	r3, [r0, #4]
	ldr	r3, [r5, #16]
	str	r3, [r0, #8]
	bl	sub_08004cb4
	mov	r3, r8
	adds	r3, #30
	cmp	r9, r3
	bne.n	.L_080d633c
	movs	r0, #126
	bl	sub_080f9010
.L_080d633c:
	mov	r3, r8
	adds	r3, #40
	cmp	r9, r3
	bne.n	.L_080d635c
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #12]
	ldr	r3, [r1, #0]
	ldrsh	r0, [r3, r2]
	movs	r3, #0
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	subs	r3, #1
	bl	sub_080d6888
.L_080d635c:
	mov	r3, r8
	adds	r3, #64
	cmp	r9, r3
	bne.n	.L_080d637c
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #12]
	ldr	r3, [r0, #0]
	ldrsh	r0, [r3, r1]
	movs	r3, #0
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #0
	negs	r2, r2
	subs	r3, #1
	bl	sub_080d6888
.L_080d637c:
	cmp	r9, r8
	ble.n	.L_080d6462
	mov	r3, r9
	mov	r5, r8
	subs	r0, r3, r5
	lsls	r0, r0, #9
	bl	sub_08004c1c
	ldr	r2, [pc, #284]
	ldr	r1, [sp, #8]
	movs	r0, #0
	mov	sl, r0
	adds	r6, r1, r2
.L_080d6396:
	mov	r5, sl
	lsrs	r3, r5, #31
	add	r3, sl
	asrs	r3, r3, #1
	add	r3, r8
	cmp	r9, r3
	ble.n	.L_080d6456
	ldr	r3, [r6, #0]
	asrs	r3, r3, #8
	adds	r0, r3, #0
	muls	r0, r3
	ldr	r3, [r6, #4]
	asrs	r3, r3, #8
	adds	r2, r3, #0
	muls	r2, r3
	ldr	r3, [r6, #8]
	asrs	r3, r3, #8
	adds	r1, r3, #0
	muls	r1, r3
	adds	r0, r0, r2
	adds	r3, r1, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #248]
	bl	sub_080072f0
	asrs	r7, r0, #9
	cmp	r7, #0
	beq.n	.L_080d6456
	add	r2, sp, #88
	adds	r0, r6, #0
	adds	r1, r2, #0
	bl	sub_080e3944
	ldr	r3, [sp, #88]
	asrs	r3, r3, #1
	str	r3, [sp, #88]
	ldr	r5, [pc, #224]
	ldr	r3, [sp, #96]
	cmp	r3, r5
	bgt.n	.L_080d63ee
	movs	r3, #157
	ldr	r0, [sp, #20]
	lsls	r3, r3, #1
	str	r3, [r0, #8]
.L_080d63ee:
	ldr	r2, [pc, #176]
	cmp	r3, r2
	ble.n	.L_080d63f8
	ldr	r1, [sp, #20]
	str	r2, [r1, #8]
.L_080d63f8:
	mov	r3, sl
	lsls	r0, r3, #2
	movs	r1, #9
	add	r0, r9
	bl	sub_080022fc
	ldr	r2, [pc, #188]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #188]
	ldr	r5, [sp, #64]
	ldrb	r0, [r3, r0]
	ldr	r3, [sp, #20]
	adds	r1, r5, r1
	ldr	r2, [r3, #0]
	ldr	r5, [sp, #60]
	ldr	r3, [r3, #4]
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	lsrs	r4, r0, #1
	subs	r2, r2, r4
	subs	r3, r3, r4
	ldr	r0, [sp, #72]
	ldr	r4, [r5, #4]
	bl	sub_080072f4
	ldr	r5, [r6, #0]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	sub_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #0]
	ldr	r5, [r6, #4]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	sub_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #4]
	ldr	r5, [r6, #8]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	sub_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #8]
.L_080d6456:
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	adds	r6, #28
	cmp	r1, #64
	bne.n	.L_080d6396
.L_080d6462:
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #8]
	ldr	r0, [sp, #68]
	movs	r5, #224
	lsls	r5, r5, #3
	adds	r3, r3, r5
	adds	r2, #2
	adds	r0, #1
	str	r2, [sp, #12]
	str	r3, [sp, #8]
	str	r0, [sp, #68]
	ldr	r1, [sp, #24]
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #20]
	cmp	r0, r3
	beq.n	.L_080d6484
	b.n	.L_080d62fa
.L_080d6484:
	ldr	r3, [sp, #76]
	ldr	r5, [pc, #32]
	adds	r2, r3, r5
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [sp, #32]
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #20]
	movs	r0, #1
	lsls	r3, r3, #3
	b.n	.L_080d64cc
	.4byte 0x0000027a
	.4byte 0x080ede48
	.4byte 0x00007824
	.4byte 0x02010000
	.4byte 0x000000ba
	.4byte 0x00007828
	.4byte 0x03001e80
	.4byte 0x030001d8
	.4byte 0x00000139
	.4byte 0x080ede84
	.2byte 0xde96
	.2byte 0x080e
.L_080d64cc:
	add	r9, r0
	adds	r3, #72
	cmp	r9, r3
	beq.n	.L_080d64d6
	b.n	.L_080d625c
.L_080d64d6:
	ldr	r0, [pc, #40]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #172
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080cd261
