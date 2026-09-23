.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_08005340, 0x08005340
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080b5070, 0x080b5070
	.set sub_080b5088, 0x080b5088
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e155c, 0x080e155c
	.set sub_080e3944, 0x080e3944
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Unnamed_080d33c0
	.global Func_080d33c0
	.thumb_func
Unnamed_080d33c0:
Func_080d33c0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #148]
	adds	r3, r6, #0
	ldmia	r3!, {r7}
	ldr	r1, [pc, #144]
	ldr	r3, [r3, #0]
	sub	sp, #36
	adds	r1, r1, r7
	str	r3, [sp, #20]
	str	r0, [r1, #0]
	movs	r0, #1
	mov	r8, r1
	bl	sub_080cd594
	ldr	r0, [pc, #128]
	bl	sub_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #124]
	adds	r1, r5, #0
	movs	r2, #128
	adds	r5, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	sub_08005340
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r2, [r6, #28]
	movs	r1, #7
	str	r2, [sp, #12]
	movs	r3, #15
	movs	r2, #7
	movs	r0, #47
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #48]
	mov	r4, r8
	strh	r3, [r2, #0]
	ldr	r3, [r4, #0]
	ldr	r6, [r6, #32]
	ldr	r0, [r3, #8]
	str	r6, [sp, #16]
	bl	sub_080b5098
	ldr	r6, [r0, #0]
	mov	r0, r8
	ldr	r3, [r0, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5070
	ldr	r3, [r6, #12]
	adds	r3, r3, r0
	mov	r9, r3
	movs	r3, #225
	movs	r1, #0
	movs	r2, #255
	lsls	r3, r3, #7
	mov	r8, r1
	mov	sl, r2
	adds	r5, r7, r3
	b.n	.L_080d3478
	.4byte 0x00000f0f
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x000000cd
	.4byte 0x03001388
	.2byte 0x0052
	.2byte 0x0400
.L_080d3478:
	ldr	r3, [r6, #8]
	mov	r4, r9
	str	r4, [r5, #4]
	str	r3, [r5, #0]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_08004458
	mov	r1, sl
	ands	r0, r1
	subs	r0, #127
	lsls	r0, r0, #16
	asrs	r0, r0, #5
	str	r0, [r5, #12]
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	subs	r3, #16
	lsls	r3, r3, #16
	asrs	r3, r3, #6
	str	r3, [r5, #16]
	bl	sub_08004458
	mov	r2, sl
	ands	r0, r2
	subs	r0, #127
	lsls	r0, r0, #16
	asrs	r0, r0, #5
	movs	r3, #1
	movs	r1, #1
	str	r0, [r5, #20]
	negs	r3, r3
	mov	r0, r8
	ldr	r4, [pc, #868]
	add	r8, r1
	str	r3, [r5, #24]
	mov	r2, r8
	movs	r3, #0
	strb	r3, [r4, r0]
	adds	r5, #28
	cmp	r2, #30
	bne.n	.L_080d3478
	ldr	r4, [pc, #856]
	movs	r3, #0
	adds	r2, r7, r4
	mov	r8, r3
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L_080d3512
	movs	r0, #232
	lsls	r0, r0, #7
	adds	r6, r2, #0
	movs	r1, #36
	adds	r5, r7, r0
.L_080d34e8:
	ldr	r3, [r6, #0]
	ldrsh	r0, [r3, r1]
	str	r1, [sp, #8]
	bl	sub_080b5098
	ldr	r2, [r0, #0]
	ldr	r3, [r2, #8]
	str	r3, [r5, #0]
	movs	r3, #0
	str	r3, [r5, #4]
	ldr	r3, [r2, #16]
	str	r3, [r5, #8]
	movs	r3, #1
	add	r8, r3
	ldr	r3, [r6, #0]
	ldr	r1, [sp, #8]
	ldr	r3, [r3, #20]
	adds	r1, #2
	adds	r5, #28
	cmp	r8, r3
	bne.n	.L_080d34e8
.L_080d3512:
	ldr	r4, [pc, #792]
	ldr	r0, [pc, #792]
	adds	r3, r7, r4
	movs	r2, #0
	movs	r5, #144
	str	r2, [r3, #0]
	lsls	r5, r5, #3
	adds	r3, r7, r0
	str	r2, [r3, #0]
	adds	r1, r5, #0
	ldr	r0, [pc, #780]
	bl	sub_080041d8
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r7, r1
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r3, [pc, #768]
	adds	r2, r7, r3
	movs	r3, #75
	str	r3, [r2, #0]
	adds	r1, r5, #0
	ldr	r0, [pc, #760]
	bl	sub_080041d8
	movs	r0, #164
	bl	sub_080f9010
	ldr	r0, [pc, #728]
	adds	r3, r7, r0
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #24]
	ldr	r2, [pc, #744]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r2, r3]
	movs	r4, #0
	mov	fp, r4
	cmp	r3, #0
	bne.n	.L_080d3566
	b.n	.L_080d37f4
.L_080d3566:
	ldr	r3, [pc, #732]
	ldr	r6, [r3, #0]
	mov	r3, fp
	subs	r3, #17
	cmp	r3, #46
	bhi.n	.L_080d357c
	ldr	r1, [pc, #696]
	movs	r3, #192
	adds	r2, r7, r1
	lsls	r3, r3, #1
	b.n	.L_080d3582
.L_080d357c:
	ldr	r3, [pc, #684]
	adds	r2, r7, r3
	movs	r3, #0
.L_080d3582:
	str	r3, [r2, #0]
	ldr	r0, [pc, #672]
	adds	r5, r7, r0
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	ldr	r4, [pc, #688]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r4, r3]
	subs	r3, #16
	mov	sl, r4
	cmp	fp, r3
	bne.n	.L_080d35a2
	movs	r0, #132
	bl	sub_080b50e8
.L_080d35a2:
	bl	sub_080049ac
	adds	r1, r6, #0
	adds	r1, #12
	adds	r0, r6, #0
	bl	sub_080051d8
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	mov	r2, sl
	lsls	r3, r3, #1
	ldrb	r3, [r2, r3]
	movs	r1, #0
	mov	r8, r1
	cmp	r3, #0
	bne.n	.L_080d35c4
	b.n	.L_080d37ac
.L_080d35c4:
	movs	r3, #225
	lsls	r3, r3, #7
	mov	sl, r5
	mov	r9, r1
	adds	r6, r7, r3
.L_080d35ce:
	cmp	fp, r9
	ble.n	.L_080d3634
	ldr	r4, [pc, #592]
	mov	r0, r8
	ldrsb	r3, [r4, r0]
	cmp	r3, #0
	bne.n	.L_080d3634
	add	r5, sp, #24
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080e3944
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	ldr	r3, [r5, #8]
	cmp	r3, #159
	bgt.n	.L_080d35f6
	movs	r3, #160
	str	r3, [r5, #8]
.L_080d35f6:
	ldr	r2, [pc, #592]
	cmp	r3, r2
	ble.n	.L_080d35fe
	str	r2, [r5, #8]
.L_080d35fe:
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	movs	r1, #12
	movs	r4, #192
	str	r1, [sp, #0]
	lsls	r4, r4, #4
	movs	r1, #24
	subs	r2, #6
	subs	r3, #12
	str	r1, [sp, #4]
	ldr	r0, [sp, #20]
	adds	r1, r7, r4
	ldr	r4, [sp, #12]
	bl	sub_080072f4
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
.L_080d3634:
	mov	r3, r9
	adds	r3, #48
	cmp	fp, r3
	ble.n	.L_080d3730
	ldr	r0, [pc, #484]
	mov	r1, r8
	ldrsb	r3, [r0, r1]
	cmp	r3, #0
	bne.n	.L_080d3730
	mov	r2, sl
	ldr	r3, [r2, #0]
	mov	r0, r8
	ldr	r1, [r3, #20]
	bl	sub_080022fc
	lsls	r1, r0, #3
	subs	r1, r1, r0
	lsls	r1, r1, #2
	movs	r3, #232
	lsls	r3, r3, #7
	adds	r1, r7, r1
	adds	r1, r1, r3
	ldr	r3, [r1, #0]
	ldr	r2, [r6, #0]
	subs	r3, r3, r2
	ldr	r2, [r6, #12]
	asrs	r3, r3, #9
	adds	r0, r2, r3
	str	r0, [r6, #12]
	ldr	r2, [r6, #4]
	ldr	r3, [r1, #4]
	subs	r3, r3, r2
	ldr	r2, [r6, #16]
	asrs	r3, r3, #9
	adds	r4, r2, r3
	str	r4, [r6, #16]
	ldr	r2, [r6, #8]
	ldr	r3, [r1, #8]
	subs	r3, r3, r2
	ldr	r2, [r6, #20]
	asrs	r3, r3, #9
	adds	r1, r2, r3
	mov	r3, r9
	adds	r3, #85
	str	r1, [r6, #20]
	cmp	fp, r3
	bge.n	.L_080d36c2
	lsls	r3, r0, #4
	subs	r3, r3, r0
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L_080d369e
	adds	r2, #63
.L_080d369e:
	asrs	r3, r2, #6
	str	r3, [r6, #12]
	lsls	r3, r4, #4
	subs	r3, r3, r4
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L_080d36ae
	adds	r2, #63
.L_080d36ae:
	asrs	r3, r2, #6
	str	r3, [r6, #16]
	lsls	r3, r1, #4
	subs	r3, r3, r1
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L_080d36be
	adds	r2, #63
.L_080d36be:
	asrs	r3, r2, #6
	str	r3, [r6, #20]
.L_080d36c2:
	ldr	r3, [r6, #4]
	cmp	r3, #0
	bge.n	.L_080d3730
	ldr	r4, [pc, #344]
	movs	r3, #1
	mov	r0, r8
	strb	r3, [r4, r0]
	movs	r3, #0
	str	r3, [r6, #24]
	add	r5, sp, #24
	ldr	r3, [r5, #0]
	str	r3, [r6, #0]
	bl	sub_08004458
	ldr	r3, [r5, #4]
	movs	r2, #31
	ands	r2, r0
	adds	r3, r3, r2
	subs	r3, #16
	mov	r1, sl
	ldr	r5, [r1, #0]
	str	r3, [r6, #4]
	mov	r0, r8
	ldr	r1, [r5, #20]
	bl	sub_080022fc
	adds	r3, r0, #0
	lsls	r2, r3, #1
	adds	r2, #36
	movs	r1, #4
	ldrsh	r0, [r5, r2]
	str	r1, [sp, #0]
	movs	r2, #5
	movs	r1, #7
	bl	sub_080d6888
	mov	r2, sl
	ldr	r5, [r2, #0]
	mov	r0, r8
	ldr	r1, [r5, #20]
	bl	sub_080022fc
	lsls	r0, r0, #1
	adds	r0, #36
	ldrsh	r0, [r5, r0]
	movs	r1, #0
	bl	sub_080b5088
	ldr	r4, [pc, #296]
	movs	r0, #4
	adds	r3, r7, r4
	str	r0, [r3, #0]
	movs	r0, #132
	bl	sub_080f9010
.L_080d3730:
	ldr	r3, [r6, #24]
	cmp	r3, #15
	bhi.n	.L_080d3790
	lsrs	r0, r3, #31
	adds	r0, r3, r0
	movs	r1, #3
	asrs	r0, r0, #1
	bl	sub_080022fc
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #4]
	adds	r1, r0, #0
	movs	r4, #16
	movs	r0, #64
	lsls	r1, r1, #10
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #12]
	subs	r2, #16
	ldr	r0, [sp, #20]
	adds	r1, r7, r1
	subs	r3, #56
	bl	sub_080072f4
	ldr	r0, [r6, #24]
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	movs	r1, #3
	asrs	r0, r0, #1
	bl	sub_080022fc
	ldr	r3, [r6, #4]
	adds	r1, r0, #0
	movs	r4, #64
	movs	r0, #16
	lsls	r1, r1, #10
	ldr	r2, [r6, #0]
	subs	r3, #56
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	adds	r1, r7, r1
	ldr	r0, [sp, #20]
	ldr	r4, [sp, #16]
	bl	sub_080072f4
	ldr	r3, [r6, #24]
	adds	r3, #1
	str	r3, [r6, #24]
.L_080d3790:
	mov	r4, sl
	ldr	r3, [r4, #0]
	ldr	r3, [r3, #24]
	ldr	r2, [pc, #168]
	lsls	r3, r3, #1
	movs	r1, #1
	ldrb	r3, [r2, r3]
	movs	r0, #2
	add	r8, r1
	add	r9, r0
	adds	r6, #28
	cmp	r8, r3
	beq.n	.L_080d37ac
	b.n	.L_080d35ce
.L_080d37ac:
	ldr	r0, [pc, #120]
	adds	r5, r7, r0
	ldr	r3, [r5, #0]
	ldr	r1, [r3, #24]
	lsls	r1, r1, #1
	adds	r1, #2
	adds	r0, r1, #0
	bl	sub_080e155c
	ldr	r1, [pc, #112]
	adds	r2, r7, r1
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_080d37cc
	movs	r3, #1
	str	r3, [r2, #0]
.L_080d37cc:
	bl	sub_080cd52c
	ldr	r3, [pc, #124]
	adds	r2, r7, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	ldr	r2, [pc, #92]
	lsls	r3, r3, #1
	adds	r3, #1
	movs	r4, #1
	ldrb	r3, [r2, r3]
	add	fp, r4
	cmp	fp, r3
	beq.n	.L_080d37f4
	b.n	.L_080d3566
.L_080d37f4:
	ldr	r0, [pc, #60]
	bl	sub_08004278
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
	.4byte 0x02010000
	.4byte 0x00007828
	.4byte 0x000077ac
	.4byte 0x000077b0
	.4byte 0x080d6505
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x080ee1c4
	.4byte 0x03001e80
	.4byte 0x0000031f
	.4byte 0x000077a8
	.4byte 0x00007824
