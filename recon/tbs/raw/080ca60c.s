.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080b5088, 0x080b5088
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e38b8, 0x080e38b8
	.set sub_080e3908, 0x080e3908
	.set sub_080e3944, 0x080e3944
	.set sub_080e396c, 0x080e396c
	.set sub_080e3980, 0x080e3980
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global BattleFx_RunTwelveMode
	.global Func_080ca60c
	.thumb_func
BattleFx_RunTwelveMode:
Func_080ca60c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #156
	ldr	r2, [pc, #40]
	str	r1, [sp, #96]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	str	r3, [sp, #92]
	ldr	r3, [r2, #8]
	str	r3, [sp, #80]
	subs	r2, #108
	ldr	r3, [pc, #28]
	mov	fp, r1
	ldr	r2, [r2, #0]
	add	r3, fp
	str	r2, [sp, #76]
	str	r0, [r3, #0]
	ldr	r5, [sp, #96]
	cmp	r5, #8
	bne.n	.L_080ca650
	movs	r0, #0
	bl	sub_080cd594
	b.n	.L_080ca656
	.4byte 0x03001eec
	.2byte 0x7828
	.2byte 0x0000
.L_080ca650:
	movs	r0, #1
	bl	sub_080cd594
.L_080ca656:
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r1, [sp, #80]
	ldr	r0, [pc, #60]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [pc, #52]
	mov	r1, fp
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [sp, #96]
	ldr	r1, [sp, #96]
	lsls	r0, r0, #3
	ldr	r2, [pc, #40]
	str	r0, [sp, #72]
	subs	r3, r0, r1
	ldrb	r3, [r2, r3]
	cmp	r3, #0
	bne.n	.L_080ca6b4
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #28]
	add	r1, fp
	movs	r2, #0
	b.n	.L_080ca6ac
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x04000052
	.4byte 0x00000073
	.4byte 0x000000ce
	.4byte 0x080edf04
	.2byte 0x00c5
	.2byte 0x0000
.L_080ca6ac:
	movs	r3, #0
	bl	sub_080e0524
	b.n	.L_080ca6c4
.L_080ca6b4:
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #844]
	add	r1, fp
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
.L_080ca6c4:
	ldr	r5, [sp, #72]
	ldr	r0, [sp, #96]
	ldr	r2, [pc, #832]
	subs	r3, r5, r0
	adds	r3, #3
	ldrb	r3, [r2, r3]
	cmp	r3, #5
	bhi.n	.L_080ca708
	ldr	r2, [pc, #824]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	add	r6, pc, #976
	lsrs	r4, r1, #32
	add	r6, pc, #992
	lsrs	r4, r1, #32
	add	r6, pc, #1008
	lsrs	r4, r1, #32
	add	r7, pc, #0
	lsrs	r4, r1, #32
	add	r7, pc, #16
	lsrs	r4, r1, #32
	add	r7, pc, #32
	lsrs	r4, r1, #32
	ldr	r0, [pc, #796]
	b.n	.L_080ca70a
	ldr	r0, [pc, #796]
	b.n	.L_080ca70a
	ldr	r0, [pc, #796]
	b.n	.L_080ca70a
	ldr	r0, [pc, #796]
	b.n	.L_080ca70a
	ldr	r0, [pc, #796]
	b.n	.L_080ca70a
.L_080ca708:
	ldr	r0, [pc, #796]
.L_080ca70a:
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #792]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	movs	r1, #200
	lsls	r1, r1, #6
	add	r1, fp
	movs	r2, #1
	movs	r3, #0
	ldr	r0, [pc, #776]
	bl	sub_080e0524
	mov	r2, sp
	movs	r1, #0
	adds	r2, #144
	movs	r3, #36
	str	r1, [sp, #84]
	str	r2, [sp, #68]
	str	r1, [sp, #16]
	str	r3, [sp, #12]
.L_080ca73c:
	ldr	r3, [pc, #756]
	add	r3, fp
	ldr	r5, [sp, #12]
	ldr	r3, [r3, #0]
	ldrsh	r0, [r3, r5]
	bl	sub_080b5098
	movs	r5, #225
	ldr	r0, [r0, #0]
	movs	r2, #0
	lsls	r5, r5, #7
	mov	sl, r0
	mov	r8, r2
	add	r5, fp
.L_080ca758:
	ldr	r3, [pc, #728]
	add	r3, fp
	ldr	r1, [sp, #12]
	ldr	r3, [r3, #0]
	ldrsh	r0, [r3, r1]
	ldr	r1, [sp, #68]
	bl	sub_080e3980
	ldr	r0, [sp, #68]
	ldr	r3, [r0, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	movs	r3, #160
	lsls	r3, r3, #15
	movs	r1, #0
	str	r3, [r5, #4]
	str	r1, [r5, #8]
	bl	sub_08004458
	movs	r2, #255
	ands	r0, r2
	subs	r0, #128
	lsls	r0, r0, #9
	str	r0, [r5, #12]
	bl	sub_08004458
	movs	r3, #255
	ands	r0, r3
	subs	r0, #128
	movs	r1, #1
	lsls	r0, r0, #9
	movs	r3, #1
	add	r8, r1
	str	r0, [r5, #16]
	negs	r3, r3
	movs	r0, #0
	mov	r2, r8
	str	r0, [r5, #20]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #64
	bne.n	.L_080ca758
	mov	r8, r0
	ldr	r3, [sp, #16]
	ldr	r0, [pc, #640]
	movs	r6, #255
	adds	r5, r3, r0
.L_080ca7bc:
	mov	r1, sl
	ldr	r3, [r1, #8]
	str	r3, [r5, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r5, #4]
	ldr	r3, [r1, #16]
	str	r3, [r5, #8]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	sub_08004458
	movs	r3, #1
	ands	r0, r6
	movs	r2, #1
	negs	r3, r3
	subs	r0, #128
	add	r8, r2
	str	r3, [r5, #24]
	lsls	r0, r0, #11
	mov	r3, r8
	str	r0, [r5, #20]
	adds	r5, #28
	cmp	r3, #128
	bne.n	.L_080ca7bc
	ldr	r0, [sp, #72]
	ldr	r1, [sp, #96]
	subs	r0, r0, r1
	str	r0, [sp, #64]
	ldr	r2, [pc, #560]
	adds	r0, #2
	movs	r5, #0
	str	r0, [sp, #60]
	ldr	r6, [pc, #556]
	mov	r8, r5
	movs	r7, #255
	mov	r9, r2
.L_080ca81a:
	mov	r5, sl
	ldr	r3, [r5, #8]
	str	r3, [r6, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	str	r3, [r6, #8]
	ldr	r0, [pc, #480]
	ldr	r1, [sp, #64]
	ldrb	r3, [r0, r1]
	cmp	r3, #1
	bne.n	.L_080ca85a
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r6, #12]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r6, #16]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r6, #20]
	b.n	.L_080ca8be
.L_080ca85a:
	ldr	r3, [pc, #432]
	ldr	r5, [sp, #60]
	ldrb	r2, [r3, r5]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ca890
	bl	sub_08004458
	mov	r1, r9
	ands	r0, r1
	lsls	r0, r0, #11
	str	r0, [r6, #12]
	bl	sub_08004458
	ldr	r5, [pc, #456]
	mov	r2, r9
	ands	r0, r2
	adds	r0, r0, r5
	lsls	r0, r0, #11
	str	r0, [r6, #16]
	bl	sub_08004458
	mov	r3, r9
	ands	r0, r3
	adds	r0, r0, r5
	b.n	.L_080ca8ae
.L_080ca890:
	bl	sub_08004458
	ands	r0, r7
	lsls	r0, r0, #11
	str	r0, [r6, #12]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r6, #16]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #128
.L_080ca8ae:
	lsls	r0, r0, #11
	str	r0, [r6, #20]
	ldr	r3, [r6, #0]
	cmp	r3, #0
	ble.n	.L_080ca8be
	ldr	r3, [r6, #12]
	negs	r3, r3
	str	r3, [r6, #12]
.L_080ca8be:
	movs	r3, #1
	movs	r5, #1
	movs	r0, #128
	negs	r3, r3
	add	r8, r5
	lsls	r0, r0, #2
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r8, r0
	bne.n	.L_080ca81a
	ldr	r1, [sp, #16]
	ldr	r3, [sp, #12]
	ldr	r5, [sp, #84]
	movs	r2, #224
	lsls	r2, r2, #4
	adds	r1, r1, r2
	adds	r3, #2
	adds	r5, #1
	str	r1, [sp, #16]
	str	r3, [sp, #12]
	str	r5, [sp, #84]
	cmp	r5, #1
	beq.n	.L_080ca8ee
	b.n	.L_080ca73c
.L_080ca8ee:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #332]
	movs	r3, #75
	add	r2, fp
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #324]
	bl	sub_080041d8
	ldr	r5, [pc, #296]
	add	r5, fp
	ldr	r3, [r5, #0]
	mov	r2, sp
	adds	r2, #132
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	adds	r1, r2, #0
	str	r2, [sp, #56]
	bl	sub_080e396c
	ldr	r3, [r5, #0]
	movs	r5, #36
	ldrsh	r0, [r3, r5]
	add	r5, sp, #120
	adds	r1, r5, #0
	bl	sub_080e3980
	ldr	r0, [sp, #56]
	ldr	r3, [r5, #4]
	ldr	r1, [r0, #4]
	subs	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r1, r1, r3
	str	r1, [r0, #4]
	ldr	r3, [sp, #96]
	ldr	r2, [sp, #72]
	movs	r1, #0
	ldr	r7, [pc, #196]
	str	r1, [sp, #88]
	subs	r1, r2, r3
	adds	r3, r1, #6
	adds	r5, r7, #0
	ldrb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L_080ca95a
	bl	.L_080cb17a
.L_080ca95a:
	ldr	r0, [sp, #76]
	ldr	r5, [pc, #212]
	adds	r0, #12
	str	r0, [sp, #20]
	mov	r3, sp
	adds	r0, r1, #0
	adds	r3, #100
	add	r5, fp
	str	r1, [sp, #44]
	adds	r0, #4
	adds	r1, #2
	str	r3, [sp, #24]
	str	r5, [sp, #48]
	str	r0, [sp, #40]
	str	r1, [sp, #36]
.L_080ca978:
	ldr	r3, [sp, #96]
	ldr	r5, [pc, #144]
	subs	r2, r2, r3
	adds	r3, r2, #1
	ldrb	r1, [r5, r3]
	adds	r3, r2, #4
	ldrb	r4, [r5, r3]
	muls	r1, r4
	movs	r3, #0
	str	r3, [sp, #52]
	ldrb	r3, [r5, r2]
	lsls	r0, r1, #2
	cmp	r3, #0
	beq.n	.L_080ca996
	b.n	.L_080cabe4
.L_080ca996:
	lsls	r3, r1, #1
	adds	r3, r3, r1
	ldr	r5, [sp, #88]
	lsls	r3, r3, #1
	cmp	r5, r3
	blt.n	.L_080ca9a4
	b.n	.L_080cabd4
.L_080ca9a4:
	adds	r1, r4, #0
	adds	r0, r5, #0
	bl	sub_080022ec
	movs	r1, #6
	bl	sub_080022fc
	adds	r6, r0, #0
	ldr	r0, [sp, #48]
	ldr	r3, [r0, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080caa64
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	movs	r0, #46
	bl	sub_080ed408
	ldr	r2, [pc, #128]
	lsls	r3, r6, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #132]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [pc, #120]
	ldrb	r3, [r3, r6]
	asrs	r2, r2, #1
	lsrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #112]
	ldrb	r5, [r3, r6]
	ldr	r3, [pc, #112]
	ldrb	r4, [r3, r6]
	subs	r2, r2, r5
	str	r5, [sp, #0]
	ldr	r3, [sp, #136]
	str	r4, [sp, #4]
	ldr	r5, [pc, #104]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	add	r1, fp
	adds	r2, #8
	ldr	r4, [r5, #0]
	ldr	r0, [sp, #92]
	bl	sub_080072f4
	b.n	.L_080caaaa
	lsls	r6, r0, #3
	movs	r0, r0
	.2byte 0xdf04
	.2byte 0x080e
	add	r6, pc, #880
	lsrs	r4, r1, #32
	lsls	r4, r1, #3
	movs	r0, r0
	lsls	r0, r4, #2
	movs	r0, r0
	lsls	r1, r4, #2
	movs	r0, r0
	lsls	r4, r6, #2
	movs	r0, r0
	lsls	r5, r1, #2
	movs	r0, r0
	lsls	r4, r0, #3
	movs	r0, r0
	asrs	r0, r1, #14
	lsls	r0, r0, #12
	lsls	r6, r3, #2
	movs	r0, r0
	ldrb	r0, [r5, #0]
	movs	r0, r0
	movs	r0, r0
	lsls	r1, r0, #8
	lsls	r7, r7, #7
	movs	r0, r0
	subs	r0, #0
	lsls	r1, r0, #8
	.2byte 0xff00
	.2byte 0xffff
	strb	r4, [r0, #30]
	movs	r0, r0
	bcs.n	.L_080cab12
	lsrs	r4, r1, #32
	.4byte 0x080edf64
	.4byte 0x080edf70
	.4byte 0x080edf58
	.4byte 0x080edf5e
	.2byte 0x1f08
	.2byte 0x0300
.L_080caa64:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	sub_080ed408
	ldr	r2, [pc, #640]
	lsls	r3, r6, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #132]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [pc, #632]
	ldrb	r3, [r3, r6]
	asrs	r2, r2, #1
	lsrs	r3, r3, #1
	adds	r2, r2, r3
	ldr	r3, [pc, #628]
	ldrb	r4, [r3, r6]
	ldr	r3, [sp, #136]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	ldr	r0, [pc, #620]
	ldrb	r0, [r0, r6]
	str	r4, [sp, #4]
	str	r0, [sp, #0]
	ldr	r0, [pc, #616]
	add	r1, fp
	ldr	r4, [r0, #0]
	subs	r2, #8
	ldr	r0, [sp, #92]
	bl	sub_080072f4
.L_080caaaa:
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r7, [pc, #600]
	ldr	r1, [sp, #40]
	ldrb	r5, [r7, r1]
	lsls	r1, r5, #1
	adds	r1, r1, r5
	lsls	r1, r1, #1
	ldr	r0, [sp, #88]
	bl	sub_080022fc
	lsls	r5, r5, #2
	cmp	r0, r5
	beq.n	.L_080caaca
	b.n	.L_080cabd4
.L_080caaca:
	ldr	r2, [sp, #96]
	cmp	r2, #8
	bne.n	.L_080caad8
	movs	r0, #134
	bl	sub_080b50e8
	b.n	.L_080caae4
.L_080caad8:
	movs	r0, #133
	bl	sub_080f9010
	movs	r0, #133
	bl	sub_080b50e8
.L_080caae4:
	ldr	r3, [pc, #548]
	ldr	r5, [sp, #36]
	ldrb	r2, [r3, r5]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cab40
	ldr	r2, [pc, #540]
	movs	r3, #8
	add	r2, fp
	str	r3, [r2, #0]
	ldr	r0, [sp, #48]
	ldr	r3, [r0, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r3, #12
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_080d6888
	ldr	r2, [sp, #48]
.L_080cab12:
	ldr	r3, [r2, #0]
	movs	r1, #4
	movs	r5, #36
	ldrsh	r0, [r3, r5]
	bl	sub_080b5088
	ldr	r5, [pc, #500]
	movs	r0, #0
	mov	r8, r0
	movs	r6, #15
.L_080cab26:
	bl	sub_08004458
	movs	r1, #1
	ands	r0, r6
	movs	r2, #128
	adds	r0, #15
	add	r8, r1
	lsls	r2, r2, #2
	str	r0, [r5, #0]
	adds	r5, #28
	cmp	r8, r2
	bne.n	.L_080cab26
	b.n	.L_080cab9a
.L_080cab40:
	ldr	r2, [pc, #460]
	movs	r3, #4
	add	r2, fp
	str	r3, [r2, #0]
	ldr	r5, [sp, #48]
	ldr	r3, [r5, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r1, #7
	movs	r3, #0
	bl	sub_080d6888
	ldr	r6, [sp, #44]
	movs	r2, #0
	ldr	r7, [pc, #424]
	mov	r8, r2
	adds	r6, #4
.L_080cab68:
	ldrb	r3, [r7, r6]
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #1
	ldr	r0, [sp, #88]
	bl	sub_080022ec
	lsls	r0, r0, #5
	add	r0, r8
	lsls	r5, r0, #3
	ldr	r3, [pc, #408]
	subs	r5, r5, r0
	lsls	r5, r5, #2
	adds	r5, r5, r3
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r3, #7
	str	r3, [r5, #24]
	movs	r5, #1
	add	r8, r5
	mov	r0, r8
	cmp	r0, #32
	bne.n	.L_080cab68
.L_080cab9a:
	ldr	r3, [pc, #368]
	ldr	r5, [sp, #40]
	ldrb	r2, [r3, r5]
	lsls	r3, r2, #1
	movs	r1, #0
	adds	r3, r3, r2
	mov	r8, r1
	lsls	r5, r3, #1
.L_080cabaa:
	ldr	r0, [sp, #88]
	adds	r1, r5, #0
	bl	sub_080022ec
	lsls	r0, r0, #4
	add	r0, r8
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #2
	movs	r0, #225
	add	r3, fp
	lsls	r0, r0, #7
	movs	r1, #1
	movs	r2, #0
	adds	r3, r3, r0
	add	r8, r1
	str	r2, [r3, #24]
	mov	r2, r8
	cmp	r2, #8
	bne.n	.L_080cabaa
	ldr	r7, [pc, #312]
.L_080cabd4:
	ldr	r3, [sp, #88]
	subs	r3, #12
	cmp	r3, #19
	bls.n	.L_080cabde
	b.n	.L_080cadd8
.L_080cabde:
	movs	r3, #1
	str	r3, [sp, #52]
	b.n	.L_080cadd8
.L_080cabe4:
	ldr	r5, [sp, #88]
	adds	r3, r0, #4
	cmp	r5, r3
	blt.n	.L_080cabee
	b.n	.L_080cadd8
.L_080cabee:
	cmp	r5, r0
	bge.n	.L_080cac0c
	adds	r0, r5, #0
	adds	r1, r4, #0
	bl	sub_080022ec
	cmp	r0, #4
	ble.n	.L_080cac04
.L_080cabfe:
	subs	r0, #4
	cmp	r0, #4
	bgt.n	.L_080cabfe
.L_080cac04:
	ldr	r3, [pc, #276]
	ldrb	r3, [r3, r0]
	str	r3, [sp, #32]
	b.n	.L_080cac10
.L_080cac0c:
	movs	r0, #3
	str	r0, [sp, #32]
.L_080cac10:
	ldr	r2, [sp, #96]
	ldr	r1, [sp, #72]
	subs	r1, r1, r2
	mov	r9, r1
	ldr	r5, [pc, #240]
	movs	r3, #5
	add	r3, r9
	mov	sl, r3
	ldrb	r3, [r5, r3]
	movs	r1, #7
	str	r3, [sp, #0]
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	sub_080ed408
	ldr	r0, [sp, #32]
	lsls	r5, r0, #3
	ldr	r2, [sp, #132]
	subs	r5, r5, r0
	lsls	r5, r5, #2
	subs	r5, r5, r0
	lsrs	r3, r2, #31
	movs	r6, #48
	adds	r2, r2, r3
	lsls	r5, r5, #5
	movs	r3, #18
	movs	r1, #200
	str	r3, [sp, #0]
	str	r6, [sp, #4]
	ldr	r0, [pc, #184]
	lsls	r1, r1, #4
	add	r5, fp
	adds	r5, r5, r1
	asrs	r2, r2, #1
	ldr	r4, [r0, #0]
	adds	r1, r5, #0
	ldr	r0, [sp, #92]
	subs	r2, #18
	mov	r8, r3
	movs	r3, #56
	bl	sub_080072f4
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r1, [pc, #156]
	mov	r2, sl
	ldrb	r3, [r1, r2]
	movs	r0, #46
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	bl	sub_080ed408
	ldr	r2, [sp, #132]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	mov	r3, r8
	str	r3, [sp, #0]
	str	r6, [sp, #4]
	ldr	r0, [pc, #120]
	adds	r1, r5, #0
	ldr	r4, [r0, #0]
	movs	r3, #56
	asrs	r2, r2, #1
	ldr	r0, [sp, #92]
	bl	sub_080072f4
	movs	r0, #46
	bl	sub_08002dd8
	mov	r6, r9
	ldr	r1, [pc, #100]
	adds	r6, #4
	ldrb	r5, [r1, r6]
	ldr	r0, [sp, #88]
	lsls	r1, r5, #2
	bl	sub_080022fc
	lsls	r3, r5, #1
	adds	r3, r3, r5
	cmp	r0, r3
	beq.n	.L_080cacbc
	b.n	.L_080cadcc
.L_080cacbc:
	ldr	r2, [sp, #48]
	ldr	r3, [r2, #0]
	movs	r5, #36
	ldrsh	r0, [r3, r5]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	movs	r1, #7
	bl	sub_080d6888
	ldr	r2, [pc, #60]
	movs	r3, #4
	add	r2, fp
	str	r3, [r2, #0]
	mov	r3, r9
	adds	r3, #1
	ldrb	r3, [r7, r3]
	ldrb	r2, [r7, r6]
	lsls	r3, r3, #2
	subs	r3, #4
	muls	r3, r2
	ldr	r0, [sp, #88]
	cmp	r0, r3
	ble.n	.L_080cad20
	movs	r0, #133
	bl	sub_080b50e8
	b.n	.L_080cad26
	movs	r0, r0
	.4byte 0x080edf64
	.4byte 0x080edf70
	.4byte 0x080edf5e
	.4byte 0x080edf58
	.4byte 0x03001f08
	.4byte 0x080edf04
	.4byte 0x000077a8
	.4byte 0x02013818
	.4byte 0x02013800
	.2byte 0xdf76
	.2byte 0x080e
.L_080cad20:
	movs	r0, #133
	bl	sub_080f9010
.L_080cad26:
	ldr	r6, [sp, #44]
	movs	r1, #0
	ldr	r7, [pc, #852]
	mov	r8, r1
	adds	r6, #4
.L_080cad30:
	ldrb	r3, [r7, r6]
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #1
	ldr	r0, [sp, #88]
	bl	sub_080022ec
	lsls	r0, r0, #6
	add	r0, r8
	lsls	r5, r0, #3
	subs	r5, r5, r0
	ldr	r2, [pc, #828]
	lsls	r5, r5, #2
	adds	r5, r5, r2
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r3, #7
	str	r3, [r5, #24]
	movs	r3, #1
	add	r8, r3
	mov	r5, r8
	cmp	r5, #64
	bne.n	.L_080cad30
	ldr	r3, [pc, #796]
	ldr	r1, [sp, #40]
	ldrb	r2, [r3, r1]
	lsls	r3, r2, #1
	movs	r0, #0
	adds	r3, r3, r2
	mov	r8, r0
	lsls	r5, r3, #1
.L_080cad72:
	ldr	r0, [sp, #88]
	adds	r1, r5, #0
	bl	sub_080022ec
	lsls	r0, r0, #4
	add	r0, r8
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #2
	movs	r2, #225
	lsls	r2, r2, #7
	add	r3, fp
	adds	r3, r3, r2
	movs	r2, #0
	str	r2, [r3, #24]
	movs	r3, #1
	add	r8, r3
	mov	r0, r8
	cmp	r0, #8
	bne.n	.L_080cad72
	ldr	r3, [pc, #740]
	ldr	r1, [sp, #40]
	mov	r8, r2
	ldrb	r2, [r3, r1]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r5, r3, #1
.L_080cada8:
	ldr	r0, [sp, #88]
	adds	r1, r5, #0
	bl	sub_080022ec
	lsls	r0, r0, #4
	add	r0, r8
	lsls	r3, r0, #3
	subs	r3, r3, r0
	ldr	r2, [pc, #716]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	movs	r2, #0
	str	r2, [r3, #24]
	movs	r3, #1
	add	r8, r3
	mov	r0, r8
	cmp	r0, #16
	bne.n	.L_080cada8
.L_080cadcc:
	ldr	r1, [sp, #32]
	cmp	r1, #3
	bne.n	.L_080cadd6
	movs	r2, #1
	str	r2, [sp, #52]
.L_080cadd6:
	ldr	r7, [pc, #680]
.L_080cadd8:
	ldr	r3, [sp, #36]
	ldrb	r2, [r7, r3]
	movs	r3, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cae9c
	ldr	r5, [sp, #52]
	cmp	r5, #0
	beq.n	.L_080cae9c
	ldr	r7, [sp, #88]
	ldr	r2, [sp, #56]
	movs	r1, #3
	movs	r0, #0
	mov	r8, r0
	mov	r9, r1
	ands	r7, r1
	mov	sl, r2
.L_080cadfa:
	bl	sub_08004458
	ldr	r3, [pc, #652]
	adds	r2, r0, #0
	ands	r2, r3
	str	r2, [sp, #8]
	bl	sub_08004458
	ldr	r2, [sp, #8]
	movs	r5, #31
	ands	r5, r0
	adds	r0, r2, #0
	bl	sub_08002322
	mov	r3, sl
	ldr	r6, [r3, #0]
	adds	r5, #4
	lsrs	r3, r6, #31
	adds	r6, r6, r3
	adds	r3, r5, #0
	muls	r3, r0
	ldr	r0, [pc, #616]
	asrs	r3, r3, #17
	asrs	r6, r6, #1
	ldr	r2, [sp, #8]
	adds	r6, r6, r3
	ldrb	r3, [r0, r7]
	adds	r0, r2, #0
	lsrs	r3, r3, #1
	subs	r6, r6, r3
	bl	sub_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	mov	r1, sl
	ldr	r5, [r1, #4]
	ldr	r2, [pc, #592]
	asrs	r3, r3, #17
	subs	r5, r5, r3
	ldrb	r3, [r2, r7]
	lsrs	r3, r3, #1
	subs	r5, r5, r3
	bl	sub_08004458
	ldr	r3, [pc, #580]
	mov	r1, r9
	ands	r0, r1
	ldrb	r2, [r3, r0]
	mov	r3, r9
	str	r1, [sp, #0]
	orrs	r3, r2
	movs	r1, #7
	movs	r2, #7
	movs	r0, #47
	bl	sub_080ed408
	ldr	r2, [pc, #560]
	lsls	r3, r7, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [pc, #540]
	ldrb	r3, [r2, r7]
	ldr	r0, [pc, #540]
	str	r3, [sp, #0]
	ldrb	r3, [r0, r7]
	ldr	r2, [pc, #548]
	str	r3, [sp, #4]
	add	r1, fp
	adds	r3, r5, #0
	ldr	r4, [r2, #0]
	ldr	r0, [sp, #92]
	adds	r2, r6, #0
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
	movs	r3, #1
	add	r8, r3
	mov	r5, r8
	cmp	r5, #3
	bne.n	.L_080cadfa
.L_080cae9c:
	bl	sub_080049ac
	ldr	r0, [sp, #76]
	ldr	r1, [sp, #20]
	bl	sub_080051d8
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	sub_080ed408
	ldr	r0, [pc, #488]
	ldr	r3, [r0, #0]
	movs	r1, #7
	str	r3, [sp, #100]
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	str	r3, [sp, #0]
	bl	sub_080ed408
	ldr	r1, [pc, #464]
	ldr	r2, [sp, #24]
	ldr	r3, [r1, #0]
	str	r3, [r2, #4]
	ldr	r5, [sp, #44]
	movs	r3, #0
	adds	r5, #2
	str	r3, [sp, #84]
	str	r5, [sp, #28]
.L_080caede:
	ldr	r0, [sp, #48]
	ldr	r3, [r0, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5098
	ldr	r7, [pc, #404]
	ldr	r1, [sp, #28]
	ldrb	r2, [r7, r1]
	ldr	r0, [r0, #0]
	movs	r3, #1
	ands	r3, r2
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_080caf64
	movs	r5, #225
	movs	r2, #0
	lsls	r5, r5, #7
	mov	r8, r2
	add	r5, fp
.L_080caf04:
	ldr	r3, [r5, #24]
	cmp	r3, #23
	bhi.n	.L_080caf56
	cmp	r3, #0
	bge.n	.L_080caf10
	adds	r3, #3
.L_080caf10:
	asrs	r3, r3, #2
	lsls	r1, r3, #3
	adds	r1, r1, r3
	lsls	r1, r1, #7
	movs	r2, #200
	mov	r0, r8
	lsls	r2, r2, #6
	movs	r4, #1
	add	r1, fp
	ands	r4, r0
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	movs	r0, #24
	str	r0, [sp, #0]
	movs	r0, #48
	str	r0, [sp, #4]
	ldr	r0, [sp, #24]
	lsls	r4, r4, #2
	subs	r3, #24
	ldr	r4, [r4, r0]
	subs	r2, #12
	ldr	r0, [sp, #92]
	bl	sub_080072f4
	adds	r0, r5, #0
	movs	r1, #62
	ldr	r2, [pc, #348]
	bl	sub_080e3908
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_080caf56:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r5, #28
	cmp	r2, #64
	bne.n	.L_080caf04
	ldr	r7, [pc, #284]
.L_080caf64:
	ldr	r3, [sp, #28]
	ldrb	r2, [r7, r3]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cb066
	ldr	r0, [sp, #96]
	movs	r5, #3
	mov	sl, r5
	cmp	r0, #11
	bne.n	.L_080caf7e
	movs	r1, #8
	mov	sl, r1
.L_080caf7e:
	ldr	r2, [sp, #88]
	cmp	r2, #55
	bne.n	.L_080caf9a
	ldr	r5, [sp, #48]
	ldr	r3, [r5, #0]
	movs	r2, #1
	ldr	r0, [r3, #8]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	subs	r3, #1
	bl	sub_080d6888
.L_080caf9a:
	ldr	r0, [sp, #88]
	cmp	r0, #90
	bne.n	.L_080cafb6
	ldr	r1, [sp, #48]
	ldr	r3, [r1, #0]
	movs	r2, #1
	ldr	r0, [r3, #8]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #0
	negs	r2, r2
	subs	r3, #1
	bl	sub_080d6888
.L_080cafb6:
	movs	r2, #0
	mov	r3, sl
	ldr	r5, [pc, #204]
	mov	r8, r2
	add	r6, sp, #108
	lsls	r7, r3, #1
.L_080cafc2:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_080cb058
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	sub_080e3944
	ldr	r2, [r6, #0]
	ldr	r1, [pc, #216]
	asrs	r2, r2, #1
	str	r2, [r6, #0]
	subs	r3, r7, #2
	ldrh	r1, [r1, r3]
	ldr	r0, [sp, #80]
	adds	r1, r0, r1
	mov	r0, sl
	lsrs	r3, r0, #1
	subs	r2, r2, r3
	ldr	r3, [r6, #4]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r7, [sp, #4]
	ldr	r0, [sp, #24]
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #92]
	bl	sub_080072f4
	adds	r0, r5, #0
	movs	r1, #60
	movs	r2, #0
	bl	sub_080e38b8
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #10
	ble.n	.L_080cb058
	mov	r1, r9
	ldr	r0, [r1, #8]
	ldr	r3, [r5, #0]
	ldr	r1, [r1, #12]
	subs	r0, r0, r3
	ldr	r3, [r5, #4]
	movs	r2, #160
	subs	r1, r1, r3
	lsls	r2, r2, #13
	mov	r3, r9
	adds	r1, r1, r2
	ldr	r2, [r3, #16]
	ldr	r3, [r5, #8]
	subs	r2, r2, r3
	ldr	r3, [r5, #12]
	asrs	r0, r0, #8
	adds	r3, r3, r0
	str	r3, [r5, #12]
	ldr	r3, [r5, #16]
	asrs	r1, r1, #8
	adds	r3, r3, r1
	ldr	r1, [pc, #120]
	str	r3, [r5, #16]
	ldr	r3, [r5, #20]
	asrs	r2, r2, #8
	adds	r0, r0, r1
	ldr	r1, [pc, #112]
	adds	r3, r3, r2
	str	r3, [r5, #20]
	cmp	r0, r1
	bhi.n	.L_080cb058
	ldr	r0, [pc, #100]
	adds	r3, r2, r0
	cmp	r3, r1
	bhi.n	.L_080cb058
	movs	r1, #1
	negs	r1, r1
	str	r1, [r5, #24]
.L_080cb058:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r5, #28
	cmp	r3, #64
	bne.n	.L_080cafc2
	ldr	r7, [pc, #24]
.L_080cb066:
	ldr	r5, [sp, #28]
	ldrb	r2, [r7, r5]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cb114
	ldr	r1, [pc, #56]
	movs	r0, #0
	ldr	r7, [pc, #12]
	mov	r8, r0
	add	r6, sp, #108
	mov	sl, r1
	b.n	.L_080cb0b8
	.4byte 0x080edf04
	.4byte 0x02013800
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0x080edeca
	.4byte 0x080eded0
	.4byte 0x080edf7b
	.4byte 0x080edebe
	.4byte 0x03001f0c
	.4byte 0x03001f08
	.4byte 0xfffffc00
	.4byte 0x080ede48
	.4byte 0x00000fff
	.2byte 0x1ffe
	.2byte 0x0000
.L_080cb0b8:
	ldr	r5, [r7, #24]
	cmp	r5, #0
	blt.n	.L_080cb108
	adds	r1, r6, #0
	adds	r0, r7, #0
	bl	sub_080e3944
	ldr	r3, [r6, #0]
	asrs	r3, r3, #1
	str	r3, [r6, #0]
	adds	r0, r7, #0
	movs	r1, #60
	movs	r2, #0
	bl	sub_080e38b8
	asrs	r5, r5, #3
	adds	r5, #1
	lsls	r0, r5, #1
	subs	r3, r0, #2
	mov	r2, sl
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #80]
	adds	r1, r3, r1
	lsrs	r3, r5, #31
	ldr	r2, [r6, #0]
	adds	r3, r5, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r6, #4]
	str	r5, [sp, #0]
	subs	r3, r3, r5
	str	r0, [sp, #4]
	ldr	r5, [sp, #24]
	ldr	r0, [sp, #92]
	ldr	r4, [r5, #4]
	bl	sub_080072f4
	ldr	r3, [r7, #24]
	subs	r3, #1
	str	r3, [r7, #24]
.L_080cb108:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r7, #28
	cmp	r1, #128
	bne.n	.L_080cb0b8
.L_080cb114:
	ldr	r2, [sp, #84]
	adds	r2, #1
	str	r2, [sp, #84]
	cmp	r2, #1
	beq.n	.L_080cb120
	b.n	.L_080caede
.L_080cb120:
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r3, [pc, #104]
	ldr	r5, [sp, #36]
	ldrb	r2, [r3, r5]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cb144
	movs	r0, #8
	movs	r1, #8
	bl	sub_080e155c
	b.n	.L_080cb14c
.L_080cb144:
	movs	r0, #2
	movs	r1, #4
	bl	sub_080e155c
.L_080cb14c:
	bl	sub_080cd52c
	ldr	r2, [pc, #72]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r0, [sp, #88]
	ldr	r2, [sp, #72]
	ldr	r1, [sp, #96]
	adds	r0, #1
	ldr	r7, [pc, #48]
	str	r0, [sp, #88]
	subs	r3, r2, r1
	adds	r3, #6
	adds	r5, r7, #0
	ldrb	r3, [r5, r3]
	cmp	r0, r3
	beq.n	.L_080cb17a
	bl	.L_080ca978
.L_080cb17a:
	ldr	r0, [pc, #36]
	bl	sub_08004278
	bl	sub_080cdbc0
	add	sp, #156
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080edf04
	.4byte 0x00007824
	.4byte 0x080cd261
