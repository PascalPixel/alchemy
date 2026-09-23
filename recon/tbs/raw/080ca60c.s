.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_08002304, 0x08002304
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080048b0, 0x080048b0
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080072f8, 0x080072f8
	.set sub_080072fc, 0x080072fc
	.set sub_08009080, 0x08009080
	.set sub_08009140, 0x08009140
	.set sub_08009150, 0x08009150
	.set sub_080b5070, 0x080b5070
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
	.global Func_080ca60c
	.thumb_func
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
	.4byte 0x080ca6f4
	.4byte 0x080ca6f8
	.4byte 0x080ca6fc
	.4byte 0x080ca700
	.4byte 0x080ca704
	.2byte 0xa708
	.2byte 0x080c
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
	.4byte 0x000000c6
	.4byte 0x080edf04
	.4byte 0x080ca6dc
	.4byte 0x000000cc
	.4byte 0x000000a0
	.4byte 0x000000a1
	.4byte 0x000000b4
	.4byte 0x0000008d
	.4byte 0x000000c4
	.4byte 0x03001388
	.4byte 0x0000009e
	.4byte 0x00007828
	.4byte 0x02010000
	.4byte 0x000001ff
	.4byte 0x02013800
	.4byte 0xffffff00
	.4byte 0x00007784
	.4byte 0x080cd261
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
	.2byte 0xd261
	.2byte 0x080c
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r7, [pc, #780]
	adds	r3, r7, #0
	ldmia	r3!, {r1}
	ldr	r2, [pc, #776]
	mov	fp, r1
	ldr	r3, [r3, #0]
	sub	sp, #84
	add	r2, fp
	str	r3, [sp, #56]
	str	r0, [r2, #0]
	movs	r0, #0
	mov	sl, r2
	bl	sub_080cd594
	ldr	r0, [pc, #760]
	mov	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	sub_080ed408
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r2, [pc, #732]
	add	r3, fp
	add	r2, fp
	str	r5, [r3, #0]
	ldr	r7, [r7, #28]
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #716]
	str	r7, [sp, #48]
	bl	sub_080041d8
	mov	r4, sl
	ldr	r3, [r4, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5098
	mov	r1, sl
	ldr	r3, [r1, #0]
	adds	r5, r0, #0
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_080b5098
	ldr	r7, [r5, #0]
	ldr	r0, [r0, #0]
	movs	r3, #90
	ldr	r5, [r7, #8]
	mov	r8, r3
	ldr	r3, [r0, #8]
	subs	r3, r3, r5
	mov	r9, r0
	movs	r1, #100
	mov	r0, r8
	muls	r0, r3
	bl	sub_080022ec
	adds	r5, r5, r0
	str	r5, [sp, #44]
	mov	r4, r9
	ldr	r3, [r4, #16]
	ldr	r5, [r7, #16]
	subs	r3, r3, r5
	mov	r0, r8
	muls	r0, r3
	movs	r1, #100
	bl	sub_080022ec
	mov	r1, sl
	ldr	r3, [r1, #0]
	adds	r5, r5, r0
	ldr	r0, [r3, #8]
	bl	sub_080b5070
	str	r0, [sp, #40]
	mov	r2, sl
	ldr	r3, [r2, #0]
	movs	r4, #36
	ldrsh	r0, [r3, r4]
	bl	sub_080b5070
	str	r0, [sp, #36]
	adds	r0, r7, #0
	bl	sub_08009140
	adds	r3, r5, #0
	ldr	r1, [sp, #44]
	movs	r2, #0
	adds	r0, r7, #0
	bl	sub_08009150
	movs	r1, #2
	adds	r0, r7, #0
	bl	sub_08009080
	adds	r3, r7, #0
	movs	r2, #1
	adds	r3, #88
	strb	r2, [r3, #0]
	adds	r5, r7, #0
	movs	r3, #128
	adds	r5, #90
	lsls	r3, r3, #10
	str	r5, [sp, #32]
	strb	r2, [r5, #0]
	str	r3, [r7, #52]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r7, #48]
	movs	r0, #20
	bl	sub_080030f8
	movs	r5, #46
	mov	r2, sp
	mov	r3, sp
	movs	r4, #56
	adds	r2, #72
	adds	r3, #60
	negs	r4, r4
	negs	r5, r5
	movs	r1, #0
	str	r2, [sp, #20]
	str	r3, [sp, #24]
	str	r4, [sp, #16]
	str	r5, [sp, #12]
	mov	sl, r1
.L_080cb2c4:
	ldr	r3, [pc, #524]
	ldr	r5, [r3, #0]
	adds	r6, r5, #0
	adds	r6, #12
	bl	sub_080049ac
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	sub_080051d8
	mov	r1, sl
	cmp	r1, #0
	bne.n	.L_080cb2f0
	movs	r2, #240
	lsls	r2, r2, #12
	mov	r3, r9
	str	r2, [r3, #40]
	ldr	r3, [pc, #496]
	mov	r4, r9
	str	r3, [r4, #72]
	str	r2, [r7, #40]
	str	r3, [r7, #72]
.L_080cb2f0:
	mov	r1, sl
	cmp	r1, #11
	bne.n	.L_080cb314
	mov	r2, r9
	ldr	r3, [r2, #28]
	negs	r3, r3
	str	r3, [r2, #28]
	ldr	r3, [r7, #28]
	negs	r3, r3
	str	r3, [r7, #28]
	ldr	r4, [sp, #40]
	ldr	r3, [r7, #12]
	adds	r3, r3, r4
	str	r3, [r7, #12]
	ldr	r1, [sp, #36]
	ldr	r3, [r2, #12]
	adds	r3, r3, r1
	str	r3, [r2, #12]
.L_080cb314:
	mov	r2, sl
	cmp	r2, #54
	bne.n	.L_080cb36c
	ldr	r3, [pc, #424]
	add	r3, fp
	ldr	r3, [r3, #0]
	movs	r4, #36
	ldrsh	r0, [r3, r4]
	movs	r3, #10
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_080d6888
	movs	r3, #128
	mov	r1, r9
	lsls	r3, r3, #12
	str	r3, [r1, #40]
	ldr	r3, [pc, #416]
	str	r3, [r1, #72]
	movs	r3, #160
	lsls	r3, r3, #11
	str	r3, [r7, #40]
	ldr	r3, [pc, #408]
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #52]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	ldr	r2, [sp, #32]
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r0, r7, #0
	bl	sub_08009140
	ldr	r3, [r7, #16]
	adds	r0, r7, #0
	movs	r1, #0
	movs	r2, #0
	bl	sub_08009150
.L_080cb36c:
	bl	sub_080049ac
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_080051d8
	ldr	r3, [r7, #8]
	ldr	r4, [sp, #20]
	str	r3, [r4, #0]
	ldr	r3, [r7, #12]
	str	r3, [r4, #4]
	ldr	r3, [r7, #16]
	str	r3, [r4, #8]
	ldr	r0, [sp, #20]
	ldr	r1, [sp, #24]
	bl	sub_080e3944
	ldr	r5, [sp, #24]
	ldr	r3, [r5, #0]
	asrs	r2, r3, #1
	mov	r3, sl
	subs	r3, #54
	str	r2, [r5, #0]
	cmp	r3, #1
	bhi.n	.L_080cb3b6
	ldr	r3, [r5, #4]
	movs	r1, #32
	str	r1, [sp, #0]
	movs	r1, #64
	str	r1, [sp, #4]
	subs	r2, #16
	subs	r3, #16
	ldr	r0, [sp, #56]
	mov	r1, fp
	ldr	r4, [sp, #48]
	bl	sub_080072f4
.L_080cb3b6:
	ldr	r5, [sp, #16]
	cmp	r5, #11
	bhi.n	.L_080cb414
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	lsls	r3, r3, #11
	ldr	r6, [sp, #12]
	movs	r4, #0
	mov	r8, r3
.L_080cb3ca:
	lsls	r1, r4, #12
	adds	r0, r1, #0
	str	r1, [sp, #28]
	str	r4, [sp, #8]
	bl	sub_08002322
	adds	r3, r6, #0
	muls	r3, r0
	ldr	r2, [sp, #24]
	ldr	r5, [r2, #0]
	asrs	r3, r3, #16
	ldr	r0, [sp, #28]
	adds	r5, r5, r3
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	mov	r1, sl
	movs	r2, #32
	asrs	r3, r3, #16
	subs	r3, r3, r1
	subs	r5, #16
	str	r2, [sp, #0]
	mov	r1, fp
	movs	r2, #64
	str	r2, [sp, #4]
	adds	r3, #100
	adds	r2, r5, #0
	ldr	r0, [sp, #56]
	add	r1, r8
	ldr	r5, [sp, #48]
	bl	sub_080072f8
	ldr	r4, [sp, #8]
	adds	r4, #1
	cmp	r4, #16
	bne.n	.L_080cb3ca
.L_080cb414:
	mov	r1, sl
	cmp	r1, #64
	bne.n	.L_080cb440
	mov	r2, r9
	ldr	r3, [r2, #28]
	negs	r3, r3
	str	r3, [r2, #28]
	ldr	r3, [r7, #28]
	negs	r3, r3
	str	r3, [r7, #28]
	ldr	r4, [sp, #40]
	ldr	r3, [r7, #12]
	subs	r3, r3, r4
	str	r3, [r7, #12]
	ldr	r5, [sp, #36]
	ldr	r3, [r2, #12]
	subs	r3, r3, r5
	str	r3, [r2, #12]
	adds	r0, r7, #0
	movs	r1, #0
	bl	sub_08009080
.L_080cb440:
	mov	r1, sl
	cmp	r1, #54
	bne.n	.L_080cb44c
	movs	r0, #134
	bl	sub_080b50e8
.L_080cb44c:
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L_080cb460
	movs	r0, #136
	bl	sub_080f9010
	ldr	r2, [pc, #136]
	movs	r3, #6
	add	r2, fp
	str	r3, [r2, #0]
.L_080cb460:
	mov	r3, sl
	cmp	r3, #53
	bne.n	.L_080cb46e
	ldr	r2, [pc, #124]
	movs	r3, #6
	add	r2, fp
	str	r3, [r2, #0]
.L_080cb46e:
	movs	r1, #16
	movs	r0, #16
	bl	sub_080e155c
	ldr	r2, [pc, #112]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r1, #1
	ldr	r4, [sp, #16]
	ldr	r5, [sp, #12]
	add	sl, r1
	adds	r4, #1
	adds	r5, #1
	mov	r2, sl
	str	r4, [sp, #16]
	str	r5, [sp, #12]
	cmp	r2, #96
	beq.n	.L_080cb49c
	b.n	.L_080cb2c4
.L_080cb49c:
	ldr	r0, [pc, #48]
	bl	sub_08004278
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #84
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x0000007d
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x03001e80
	.4byte 0x000091eb
	.4byte 0x0000ab85
	.4byte 0x00007851
	.4byte 0x000077a8
	.2byte 0x7824
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #96]
	adds	r3, r6, #0
	ldmia	r3!, {r2}
	ldr	r3, [r3, #0]
	sub	sp, #36
	str	r3, [sp, #12]
	ldr	r3, [pc, #88]
	mov	fp, r2
	add	r3, fp
	str	r0, [r3, #0]
	movs	r0, #1
	mov	r8, r3
	bl	sub_080cd594
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r2, #50
	strh	r3, [r2, #0]
	movs	r5, #1
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [r6, #28]
	movs	r1, #7
	str	r3, [sp, #16]
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [r6, #32]
	mov	r0, sp
	adds	r0, #16
	str	r0, [sp, #8]
	mov	r1, fp
	str	r3, [r0, #4]
	b.n	.L_080cb568
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00001000
	.4byte 0x03001eec
	.4byte 0x00007828
	.2byte 0x0020
	.2byte 0x0400
.L_080cb568:
	movs	r2, #1
	ldr	r0, [pc, #152]
	movs	r3, #1
	bl	sub_080e0524
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r2, [pc, #144]
	add	r3, fp
	str	r5, [r3, #0]
	add	r2, fp
	movs	r3, #0
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #132]
	bl	sub_080041d8
	mov	r2, r8
	ldr	r3, [r2, #0]
	add	r5, sp, #24
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	adds	r1, r5, #0
	bl	sub_080e396c
	ldr	r2, [r5, #0]
	movs	r3, #64
	ldr	r1, [pc, #108]
	subs	r3, r3, r2
	lsls	r3, r3, #8
	movs	r5, #225
	str	r3, [r1, #0]
	lsls	r5, r5, #7
	movs	r3, #0
	mov	r9, r3
	add	r5, fp
.L_080cb5b2:
	bl	sub_08004458
	movs	r1, #96
	bl	sub_08002304
	mov	r2, r9
	adds	r0, #16
	str	r0, [r5, #0]
	cmp	r2, #0
	bge.n	.L_080cb5c8
	adds	r2, #3
.L_080cb5c8:
	asrs	r2, r2, #2
	movs	r3, #24
	subs	r3, r3, r2
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	cmp	r0, #43
	bgt.n	.L_080cb5da
	movs	r3, #3
	b.n	.L_080cb618
.L_080cb5da:
	cmp	r0, #51
	bgt.n	.L_080cb5e2
	movs	r3, #2
	b.n	.L_080cb618
.L_080cb5e2:
	cmp	r0, #59
	bgt.n	.L_080cb5ea
	movs	r3, #1
	b.n	.L_080cb618
.L_080cb5ea:
	cmp	r0, #67
	bgt.n	.L_080cb5f2
	movs	r3, #0
	b.n	.L_080cb618
.L_080cb5f2:
	cmp	r0, #75
	bgt.n	.L_080cb5fa
	movs	r3, #1
	b.n	.L_080cb616
.L_080cb5fa:
	cmp	r0, #83
	bgt.n	.L_080cb614
	movs	r3, #2
	b.n	.L_080cb616
	movs	r0, r0
	.4byte 0x00000078
	.4byte 0x00007784
	.4byte 0x080cd261
	.2byte 0x0028
	.2byte 0x0400
.L_080cb614:
	movs	r3, #3
.L_080cb616:
	negs	r3, r3
.L_080cb618:
	str	r3, [r5, #12]
	ldr	r3, [r5, #12]
	lsls	r3, r3, #17
	str	r3, [r5, #12]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r5, #16]
	movs	r0, #1
	ldr	r3, [r5, #0]
	add	r9, r0
	lsls	r3, r3, #16
	mov	r2, r9
	str	r3, [r5, #0]
	adds	r5, #28
	cmp	r2, #64
	bne.n	.L_080cb5b2
	movs	r0, #212
	bl	sub_080f9010
	movs	r3, #0
	mov	sl, r3
.L_080cb642:
	mov	r0, sl
	cmp	r0, #16
	bgt.n	.L_080cb65c
	ldr	r2, [pc, #40]
	mov	r3, sl
	orrs	r3, r2
	ldr	r2, [pc, #48]
	strh	r3, [r2, #0]
	cmp	r0, #16
	bne.n	.L_080cb65c
	ldr	r3, [pc, #32]
	subs	r2, #2
	strh	r3, [r2, #0]
.L_080cb65c:
	mov	r3, sl
	cmp	r3, #103
	ble.n	.L_080cb68e
	ldr	r3, [pc, #24]
	ldr	r2, [pc, #12]
	mov	r0, sl
	subs	r3, r3, r0
	orrs	r3, r2
	ldr	r2, [pc, #16]
	strh	r3, [r2, #0]
	b.n	.L_080cb684
	movs	r0, r0
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000078
	.2byte 0x0052
	.2byte 0x0400
.L_080cb684:
	cmp	r0, #104
	bne.n	.L_080cb68e
	ldr	r3, [pc, #44]
	subs	r2, #2
	strh	r3, [r2, #0]
.L_080cb68e:
	ldr	r7, [pc, #44]
	movs	r3, #15
	mov	r9, r3
	add	r7, fp
.L_080cb696:
	ldr	r6, [r7, #12]
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L_080cb6a0
	negs	r3, r6
.L_080cb6a0:
	mov	r0, r9
	lsls	r0, r0, #2
	mov	r8, r0
	asrs	r4, r3, #17
	mov	r3, r8
	adds	r3, #25
	cmp	sl, r3
	bge.n	.L_080cb70a
	ldr	r2, [pc, #12]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	b.n	.L_080cb6c4
	.4byte 0x00003f44
	.4byte 0x00007224
	.2byte 0xdf88
	.2byte 0x080e
.L_080cb6c4:
	movs	r3, #2
	ldrsh	r2, [r7, r3]
	ldr	r3, [pc, #272]
	ldrb	r5, [r3, r4]
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	movs	r0, #6
	ldrsh	r3, [r7, r0]
	ldr	r0, [pc, #264]
	ldrb	r4, [r0, r4]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r5, [sp, #0]
	ldr	r0, [sp, #8]
	str	r4, [sp, #4]
	lsrs	r6, r6, #31
	lsls	r6, r6, #2
	ldr	r4, [r6, r0]
	add	r1, fp
	ldr	r0, [sp, #12]
	bl	sub_080072f4
	mov	r3, r8
	adds	r3, #16
	cmp	sl, r3
	blt.n	.L_080cb73e
	ldr	r3, [r7, #0]
	ldr	r2, [r7, #12]
	adds	r3, r3, r2
	str	r3, [r7, #0]
	ldr	r2, [r7, #16]
	ldr	r3, [r7, #4]
	adds	r3, r3, r2
	str	r3, [r7, #4]
	b.n	.L_080cb73e
.L_080cb70a:
	ldr	r2, [pc, #216]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	movs	r3, #2
	ldrsh	r2, [r7, r3]
	ldr	r3, [pc, #196]
	ldrb	r5, [r3, r4]
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	movs	r0, #6
	ldrsh	r3, [r7, r0]
	ldr	r0, [pc, #188]
	ldrb	r4, [r0, r4]
	lsrs	r0, r4, #1
	subs	r4, #4
	lsrs	r6, r6, #31
	subs	r3, r3, r0
	str	r5, [sp, #0]
	ldr	r0, [sp, #8]
	str	r4, [sp, #4]
	lsls	r6, r6, #2
	ldr	r4, [r6, r0]
	add	r1, fp
	ldr	r0, [sp, #12]
	bl	sub_080072f4
.L_080cb73e:
	movs	r2, #1
	negs	r2, r2
	add	r9, r2
	subs	r7, #28
	cmp	r9, r2
	bne.n	.L_080cb696
	mov	r3, sl
	subs	r3, #23
	cmp	r3, #64
	bhi.n	.L_080cb78c
	movs	r3, #3
	mov	r0, sl
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_080cb78c
	ldr	r3, [pc, #136]
	add	r3, fp
	ldr	r3, [r3, #0]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	movs	r1, #7
	bl	sub_080d6888
	ldr	r2, [pc, #116]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	mov	r0, sl
	movs	r3, #7
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_080cb78c
	movs	r0, #133
	bl	sub_080f9010
.L_080cb78c:
	movs	r0, #8
	movs	r1, #8
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #84]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #120
	beq.n	.L_080cb7b2
	b.n	.L_080cb642
.L_080cb7b2:
	ldr	r0, [pc, #64]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080edf7f
	.4byte 0x080edf83
	.4byte 0x080edf88
	.4byte 0x00007828
	.4byte 0x000077a8
	.4byte 0x00007824
	.2byte 0xd261
	.2byte 0x080c
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	ldr	r1, [pc, #124]
	movs	r0, #39
	sub	sp, #76
	bl	sub_080048b0
	movs	r1, #128
	mov	r9, r0
	lsls	r1, r1, #7
	movs	r0, #40
	bl	sub_080048b0
	ldr	r1, [pc, #108]
	str	r0, [sp, #36]
	movs	r0, #41
	bl	sub_080048b0
	ldr	r3, [pc, #100]
	str	r0, [sp, #24]
	ldr	r5, [pc, #100]
	ldr	r3, [r3, #0]
	add	r5, r9
	str	r3, [sp, #20]
	movs	r0, #0
	str	r6, [r5, #0]
	bl	sub_080cd594
	ldr	r2, [pc, #88]
	movs	r3, #24
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r2, [pc, #84]
	movs	r3, #0
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r2, [pc, #80]
	ldr	r3, [pc, #48]
	ldr	r6, [pc, #80]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	subs	r2, #50
	strh	r3, [r2, #0]
	adds	r0, r6, #0
	mov	r1, r9
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	movs	r3, #0
	ldr	r0, [pc, #60]
	ldr	r1, [sp, #24]
	movs	r2, #0
	bl	sub_080e0524
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L_080cb8bc
	cmp	r3, #1
	bgt.n	.L_080cb8b2
	b.n	.L_080cb8ac
	.4byte 0x0000100c
	.4byte 0x00000100
	.4byte 0x0000782c
	.4byte 0x0000060e
	.4byte 0x03001e80
	.4byte 0x00007828
	.4byte 0x000077b4
	.4byte 0x000077b8
	.4byte 0x04000052
	.4byte 0x00000057
	.2byte 0x0076
	.2byte 0x0000
.L_080cb8ac:
	cmp	r3, #0
	beq.n	.L_080cb8b8
	b.n	.L_080cb8cc
.L_080cb8b2:
	cmp	r3, #2
	beq.n	.L_080cb8c0
	b.n	.L_080cb8cc
.L_080cb8b8:
	ldr	r0, [pc, #8]
	b.n	.L_080cb8ce
.L_080cb8bc:
	adds	r0, r6, #0
	b.n	.L_080cb8ce
.L_080cb8c0:
	ldr	r0, [pc, #4]
	b.n	.L_080cb8ce
	.4byte 0x00000048
	.2byte 0x0047
	.2byte 0x0000
.L_080cb8cc:
	ldr	r0, [pc, #268]
.L_080cb8ce:
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #264]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	sub_080072f0
	ldr	r5, [pc, #256]
	movs	r1, #0
	mov	r8, r1
.L_080cb8e6:
	movs	r3, #0
	str	r3, [r5, #4]
	bl	sub_08004458
	ldr	r3, [pc, #248]
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08004458
	ldr	r3, [pc, #240]
	mov	r4, r8
	lsls	r2, r4, #1
	ands	r3, r0
	movs	r1, #1
	adds	r3, r3, r2
	add	r8, r1
	str	r3, [r5, #8]
	mov	r2, r8
	negs	r3, r4
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #128
	bne.n	.L_080cb8e6
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #208]
	movs	r3, #75
	add	r2, r9
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #200]
	bl	sub_080041d8
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	movs	r0, #46
	str	r3, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [pc, #184]
	ldr	r1, [pc, #184]
	adds	r3, #184
	add	r1, r9
	ldr	r3, [r3, #0]
	ldr	r2, [r1, #0]
	str	r3, [sp, #28]
	ldr	r3, [r2, #24]
	adds	r3, #1
	str	r3, [r2, #24]
	cmp	r3, #0
	bgt.n	.L_080cb95a
	movs	r3, #1
	str	r3, [r2, #24]
.L_080cb95a:
	ldr	r2, [r1, #0]
	ldr	r3, [r2, #24]
	cmp	r3, #4
	ble.n	.L_080cb966
	movs	r3, #4
	str	r3, [r2, #24]
.L_080cb966:
	movs	r0, #212
	bl	sub_080f9010
	ldr	r2, [pc, #140]
	ldr	r1, [sp, #20]
	mov	r4, sp
	adds	r4, #64
	adds	r1, #12
	add	r2, r9
	movs	r3, #0
	str	r4, [sp, #8]
	str	r1, [sp, #12]
	str	r2, [sp, #16]
	mov	sl, r3
.L_080cb982:
	ldr	r4, [sp, #16]
	ldr	r3, [r4, #0]
	ldr	r1, [sp, #8]
	ldr	r0, [r3, #8]
	bl	sub_080e3980
	ldr	r1, [sp, #8]
	ldr	r3, [r1, #0]
	movs	r4, #64
	ldr	r2, [pc, #104]
	subs	r3, r4, r3
	lsls	r3, r3, #8
	mov	r1, sl
	str	r3, [r2, #0]
	cmp	r1, #49
	ble.n	.L_080cb9b2
	mov	r3, sl
	lsls	r2, r3, #1
	ldr	r3, [pc, #44]
	subs	r3, r3, r2
	ldr	r2, [pc, #44]
	ldr	r1, [pc, #84]
	orrs	r3, r2
	strh	r3, [r1, #0]
.L_080cb9b2:
	mov	r4, sl
	cmp	r4, #16
	bne.n	.L_080cba08
	ldr	r1, [sp, #16]
	ldr	r3, [r1, #0]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #20
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	movs	r3, #0
	bl	sub_080d6888
	b.n	.L_080cba08
	movs	r0, r0
	.4byte 0x00000070
	.4byte 0x00001000
	.4byte 0x00000046
	.4byte 0x03001388
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0x000001ff
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x03001e50
	.4byte 0x00007828
	.4byte 0x04000028
	.2byte 0x0052
	.2byte 0x0400
.L_080cba08:
	mov	r3, sl
	cmp	r3, #55
	bgt.n	.L_080cbaf4
	lsrs	r3, r3, #31
	add	r3, sl
	asrs	r3, r3, #1
	mov	r8, r3
	mov	r0, r8
	cmp	r0, #0
	bge.n	.L_080cba1e
	adds	r0, #3
.L_080cba1e:
	asrs	r7, r0, #2
	mov	fp, r7
	mov	r4, fp
	lsls	r3, r4, #2
	mov	r1, r8
	movs	r2, #2
	subs	r7, r1, r3
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	bl	sub_080ed408
	ldr	r3, [pc, #444]
	ldr	r4, [sp, #8]
	lsls	r1, r7, #4
	ldr	r6, [r3, #0]
	adds	r1, r1, r7
	ldr	r3, [r4, #4]
	movs	r2, #17
	movs	r4, #64
	lsls	r1, r1, #6
	str	r2, [sp, #0]
	str	r6, [sp, #32]
	add	r1, r9
	subs	r3, #64
	str	r4, [sp, #4]
	ldr	r0, [sp, #36]
	movs	r2, #47
	bl	sub_080072fc
	mov	r0, sl
	cmp	r0, #0
	bge.n	.L_080cba66
	adds	r0, #3
.L_080cba66:
	movs	r1, #3
	asrs	r0, r0, #2
	bl	sub_080022fc
	adds	r7, r0, #0
	lsls	r5, r7, #7
	ldr	r2, [sp, #8]
	adds	r5, r5, r7
	lsls	r5, r5, #3
	movs	r1, #136
	ldr	r3, [r2, #4]
	lsls	r1, r1, #5
	add	r5, r9
	movs	r4, #24
	adds	r5, r5, r1
	movs	r1, #43
	str	r4, [sp, #0]
	str	r1, [sp, #4]
	ldr	r0, [sp, #36]
	subs	r3, #36
	adds	r1, r5, #0
	movs	r2, #40
	bl	sub_080072fc
	movs	r0, #47
	bl	sub_08002dd8
	movs	r2, #2
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	bl	sub_080ed408
	ldr	r3, [pc, #328]
	mov	r4, fp
	ldr	r6, [r3, #0]
	mov	r1, r8
	lsls	r3, r4, #2
	ldr	r2, [sp, #8]
	subs	r7, r1, r3
	lsls	r1, r7, #4
	ldr	r3, [r2, #4]
	adds	r1, r1, r7
	movs	r4, #17
	movs	r2, #64
	lsls	r1, r1, #6
	str	r4, [sp, #0]
	str	r2, [sp, #4]
	str	r6, [sp, #32]
	add	r1, r9
	subs	r3, #64
	ldr	r0, [sp, #36]
	bl	sub_080072fc
	ldr	r4, [sp, #8]
	ldr	r3, [r4, #4]
	movs	r1, #24
	movs	r2, #43
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	subs	r3, #36
	ldr	r0, [sp, #36]
	adds	r1, r5, #0
	movs	r2, #64
	bl	sub_080072fc
	movs	r0, #47
	bl	sub_08002dd8
.L_080cbaf4:
	ldr	r4, [sp, #16]
	ldr	r3, [r4, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5098
	bl	sub_080049ac
	ldr	r1, [sp, #12]
	ldr	r0, [sp, #20]
	bl	sub_080051d8
	ldr	r5, [pc, #240]
	movs	r1, #0
	mov	r8, r1
	add	r7, sp, #40
	add	r6, sp, #52
.L_080cbb14:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_080cbb96
	ldr	r0, [r5, #0]
	bl	sub_08002322
	ldr	r3, [r5, #8]
	muls	r3, r0
	asrs	r3, r3, #4
	str	r3, [r7, #0]
	ldr	r0, [r5, #0]
	bl	sub_0800231c
	ldr	r3, [r5, #8]
	muls	r3, r0
	asrs	r3, r3, #4
	negs	r3, r3
	str	r3, [r7, #8]
	ldr	r3, [r5, #4]
	str	r3, [r7, #4]
	movs	r2, #128
	ldr	r3, [r5, #0]
	lsls	r2, r2, #3
	adds	r3, r3, r2
	str	r3, [r5, #0]
	movs	r4, #160
	ldr	r3, [r5, #4]
	lsls	r4, r4, #11
	adds	r3, r3, r4
	str	r3, [r5, #4]
	ldr	r3, [r5, #8]
	adds	r3, #64
	str	r3, [r5, #8]
	adds	r1, r6, #0
	adds	r0, r7, #0
	bl	sub_080e3944
	ldr	r2, [r6, #0]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	str	r2, [r6, #0]
	ldr	r4, [sp, #16]
	ldr	r3, [r4, #0]
	mov	r1, r8
	ldr	r3, [r3, #24]
	movs	r0, #1
	ands	r0, r1
	adds	r0, r0, r3
	lsls	r4, r0, #1
	ldr	r1, [pc, #132]
	subs	r3, r4, #2
	ldrh	r1, [r1, r3]
	ldr	r3, [sp, #24]
	adds	r1, r3, r1
	ldr	r3, [r6, #4]
	subs	r2, r2, r0
	subs	r3, r3, r0
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #36]
	ldr	r4, [sp, #28]
	bl	sub_080072f4
	ldr	r3, [r5, #24]
.L_080cbb96:
	movs	r1, #1
	add	r8, r1
	adds	r3, #1
	mov	r2, r8
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #32
	bne.n	.L_080cbb14
	bl	sub_080cd52c
	ldr	r2, [pc, #88]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	cmp	r4, #56
	beq.n	.L_080cbbc4
	b.n	.L_080cb982
.L_080cbbc4:
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r0, [pc, #60]
	bl	sub_08004278
	bl	sub_080cdbc0
	movs	r0, #41
	bl	sub_08002dd8
	movs	r0, #40
	bl	sub_08002dd8
	movs	r0, #39
	bl	sub_08002dd8
	add	sp, #76
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f0c
	.4byte 0x02010000
	.4byte 0x080ede5c
	.4byte 0x00007824
	.4byte 0x080cd261
