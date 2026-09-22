.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080048b0, 0x080048b0
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_08007314, 0x08007314
	.set sub_080b5078, 0x080b5078
	.set sub_080b5088, 0x080b5088
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080cef64, 0x080cef64
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e3908, 0x080e3908
	.set sub_080e3980, 0x080e3980
	.set sub_080f9010, 0x080f9010
	.global Overlay_080ecef4
Overlay_080ecef4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #56]
	ldmia	r3!, {r7}
	ldr	r2, [pc, #56]
	ldr	r3, [r3, #0]
	adds	r5, r0, #0
	mov	r8, r3
	adds	r3, r7, r2
	str	r5, [r3, #0]
	movs	r0, #0
	sub	sp, #16
	adds	r6, r1, #0
	bl	sub_080cd594
	ldr	r2, [pc, #36]
	ldr	r3, [pc, #24]
	strh	r3, [r2, #0]
	cmp	r6, #0
	bne.n	.L_080ecf48
	ldr	r0, [pc, #32]
	adds	r1, r7, #0
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [pc, #24]
	b.n	.L_080ecf5a
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000050
	.4byte 0x0000004f
	.2byte 0x0050
	.2byte 0x0000
.L_080ecf48:
	cmp	r6, #1
	bne.n	.L_080ecf66
	ldr	r0, [pc, #380]
	adds	r1, r7, #0
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [pc, #372]
.L_080ecf5a:
	ldr	r1, [pc, #376]
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	b.n	.L_080ecf7e
.L_080ecf66:
	ldr	r0, [pc, #368]
	adds	r1, r7, #0
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [pc, #360]
	ldr	r1, [pc, #348]
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
.L_080ecf7e:
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r4, [pc, #348]
	adds	r2, r7, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r1, #144
	adds	r2, r7, r4
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r0, [pc, #336]
	lsls	r1, r1, #3
	bl	sub_080041d8
	cmp	r6, #1
	bne.n	.L_080ecfb0
	movs	r3, #128
	movs	r2, #36
	ldrsh	r1, [r5, r2]
	ldr	r0, [r5, #8]
	lsls	r3, r3, #12
	movs	r2, #16
	bl	sub_080b5078
	b.n	.L_080ecfbe
.L_080ecfb0:
	movs	r3, #36
	ldrsh	r1, [r5, r3]
	ldr	r0, [r5, #8]
	movs	r2, #16
	movs	r3, #0
	bl	sub_080b5078
.L_080ecfbe:
	movs	r0, #16
	bl	sub_080030f8
	ldr	r4, [pc, #288]
	adds	r3, r7, r4
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080ecfe2
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	bl	.L_080ed408
	b.n	.L_080ecff2
.L_080ecfe2:
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	bl	.L_080ed408
.L_080ecff2:
	ldr	r3, [pc, #248]
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r0, #212
	str	r3, [sp, #8]
	bl	sub_080f9010
	movs	r5, #0
	movs	r6, #120
.L_080ed004:
	cmp	r5, #3
	bgt.n	.L_080ed012
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	mov	r0, r8
	adds	r1, r7, #0
	b.n	.L_080ed030
.L_080ed012:
	cmp	r5, #7
	bgt.n	.L_080ed024
	movs	r2, #225
	lsls	r2, r2, #6
	adds	r1, r7, r2
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	mov	r0, r8
	b.n	.L_080ed030
.L_080ed024:
	cmp	r5, #11
	bgt.n	.L_080ed03c
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	mov	r0, r8
	ldr	r1, [pc, #164]
.L_080ed030:
	movs	r2, #0
	movs	r3, #0
	ldr	r4, [sp, #8]
	bl	sub_080072f4
	b.n	.L_080ed052
.L_080ed03c:
	cmp	r5, #15
	bgt.n	.L_080ed052
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	mov	r0, r8
	ldr	r1, [pc, #168]
	movs	r2, #0
	movs	r3, #0
	ldr	r4, [sp, #8]
	bl	sub_080072f4
.L_080ed052:
	adds	r3, r5, #0
	subs	r3, #16
	cmp	r3, #3
	bhi.n	.L_080ed068
	movs	r1, #128
	ldr	r3, [pc, #148]
	mov	r0, r8
	lsls	r1, r1, #7
	ldr	r2, [pc, #148]
	bl	sub_080072f0
.L_080ed068:
	cmp	r5, #18
	bne.n	.L_080ed072
	movs	r0, #134
	bl	sub_080b50e8
.L_080ed072:
	cmp	r5, #20
	bne.n	.L_080ed08e
	ldr	r3, [pc, #132]
	ldr	r4, [pc, #108]
	adds	r2, r7, r3
	movs	r3, #8
	str	r3, [r2, #0]
	adds	r3, r7, r4
	ldr	r3, [r3, #0]
	movs	r1, #4
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_080b5088
.L_080ed08e:
	movs	r0, #16
	movs	r1, #16
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r3, [pc, #100]
	adds	r2, r7, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	adds	r5, #1
	bl	sub_080030f8
	cmp	r5, #21
	bne.n	.L_080ed004
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r0, [pc, #44]
	bl	sub_08004278
	bl	sub_080cdbc0
	add	sp, #16
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000004d
	.4byte 0x0000004e
	.4byte 0x02010000
	.4byte 0x0000004b
	.4byte 0x0000004c
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007828
	.4byte 0x03001e50
	.4byte 0x02013840
	.4byte 0x03000168
	.4byte 0x3f3f3f3f
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
	ldr	r2, [pc, #96]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r6, [pc, #92]
	ldr	r3, [r3, #0]
	sub	sp, #44
	mov	sl, r1
	str	r3, [sp, #20]
	add	r6, sl
	str	r0, [r6, #0]
	movs	r0, #0
	ldr	r5, [r2, #8]
	bl	sub_080cd594
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #60]
	adds	r1, r5, #0
	strh	r3, [r2, #0]
	ldr	r0, [pc, #72]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [pc, #64]
	mov	r1, sl
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r1, #140
	lsls	r1, r1, #3
	ldr	r0, [pc, #52]
	add	r1, sl
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	ldr	r3, [r6, #0]
	mov	r2, sp
	adds	r2, #24
	ldr	r0, [r3, #4]
	adds	r1, r2, #0
	str	r2, [sp, #16]
	bl	sub_080cef64
	b.n	.L_080ed18c
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x00000073
	.4byte 0x00000051
	.2byte 0x00c0
	.2byte 0x0000
.L_080ed18c:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #576]
	movs	r3, #75
	add	r2, sl
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #568]
	bl	sub_080041d8
	ldr	r3, [r6, #0]
	add	r5, sp, #32
	movs	r4, #36
	ldrsh	r0, [r3, r4]
	adds	r1, r5, #0
	bl	sub_080e3980
	ldr	r2, [r5, #0]
	movs	r3, #64
	ldr	r1, [pc, #548]
	subs	r3, r3, r2
	lsls	r3, r3, #8
	movs	r7, #225
	str	r3, [r1, #0]
	lsls	r7, r7, #7
	movs	r1, #0
	mov	r8, r1
	add	r7, sl
.L_080ed1cc:
	bl	sub_08004458
	ldr	r6, [pc, #528]
	ands	r6, r0
	bl	sub_08004458
	ldr	r3, [pc, #524]
	adds	r5, r0, #0
	ands	r5, r3
	movs	r3, #128
	lsls	r3, r3, #15
	str	r3, [r7, #0]
	movs	r3, #224
	lsls	r3, r3, #15
	str	r3, [r7, #4]
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r6, #128
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #8
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #9
	str	r3, [r7, #16]
	bl	sub_08004458
	movs	r3, #7
	movs	r2, #1
	ands	r3, r0
	add	r8, r2
	str	r3, [r7, #24]
	mov	r3, r8
	adds	r7, #28
	cmp	r3, #16
	bne.n	.L_080ed1cc
	ldr	r2, [pc, #460]
	ldr	r1, [pc, #460]
	movs	r4, #0
	add	r2, sl
	movs	r3, #8
	add	r1, sl
	str	r3, [r2, #0]
	str	r1, [sp, #12]
	str	r4, [sp, #8]
	mov	r9, r4
.L_080ed232:
	mov	r2, r9
	cmp	r2, #10
	bne.n	.L_080ed26a
	ldr	r3, [pc, #432]
	movs	r4, #8
	add	r3, sl
	str	r4, [r3, #0]
	movs	r0, #212
	bl	sub_080b50e8
	ldr	r1, [sp, #12]
	ldr	r3, [r1, #0]
	movs	r1, #0
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_080b5088
	ldr	r4, [sp, #12]
	ldr	r3, [r4, #0]
	movs	r2, #8
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_080d6888
.L_080ed26a:
	mov	r3, r9
	cmp	r3, #7
	ble.n	.L_080ed2f4
	cmp	r3, #31
	bgt.n	.L_080ed280
	lsls	r3, r3, #1
	add	r3, r9
	lsls	r3, r3, #2
	adds	r5, r3, #0
	subs	r5, #96
	b.n	.L_080ed288
.L_080ed280:
	movs	r3, #136
	ldr	r4, [sp, #8]
	lsls	r3, r3, #1
	subs	r5, r3, r4
.L_080ed288:
	cmp	r5, #0
	ble.n	.L_080ed2f4
	cmp	r5, #80
	ble.n	.L_080ed296
	movs	r5, #80
	movs	r7, #2
	b.n	.L_080ed298
.L_080ed296:
	movs	r7, #3
.L_080ed298:
	movs	r1, #0
	movs	r2, #112
	mov	r8, r1
	movs	r6, #50
	mov	fp, r2
.L_080ed2a2:
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L_080ed2b8
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	str	r7, [sp, #0]
	bl	.L_080ed408
	b.n	.L_080ed2c6
.L_080ed2b8:
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	str	r7, [sp, #0]
	bl	.L_080ed408
.L_080ed2c6:
	ldr	r4, [pc, #300]
	ldr	r4, [r4, #0]
	movs	r3, #14
	mov	ip, r4
	str	r4, [sp, #24]
	mov	r4, fp
	adds	r2, r6, #0
	str	r3, [sp, #0]
	mov	r1, sl
	str	r5, [sp, #4]
	ldr	r0, [sp, #20]
	subs	r3, r4, r5
	bl	sub_08007314
	movs	r0, #46
	bl	sub_08002dd8
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r6, #14
	cmp	r2, #2
	bne.n	.L_080ed2a2
.L_080ed2f4:
	ldr	r4, [sp, #12]
	ldr	r3, [r4, #0]
	ldr	r1, [sp, #16]
	ldr	r0, [r3, #4]
	movs	r5, #225
	bl	sub_080cef64
	lsls	r5, r5, #7
	movs	r1, #0
	mov	r8, r1
	add	r5, sl
.L_080ed30a:
	mov	r2, r8
	lsrs	r3, r2, #31
	add	r3, r8
	asrs	r3, r3, #1
	adds	r3, #8
	cmp	r9, r3
	blt.n	.L_080ed366
	ldr	r0, [r5, #24]
	cmp	r0, #28
	bgt.n	.L_080ed366
	movs	r1, #3
	bl	sub_080022ec
	movs	r3, #2
	ldrsh	r4, [r5, r3]
	movs	r1, #6
	ldrsh	r6, [r5, r1]
	cmp	r0, #6
	ble.n	.L_080ed332
	movs	r0, #6
.L_080ed332:
	ldr	r3, [pc, #196]
	lsls	r2, r0, #1
	ldrh	r1, [r3, r2]
	movs	r3, #140
	lsls	r3, r3, #3
	add	r1, sl
	adds	r1, r1, r3
	ldr	r3, [pc, #184]
	ldrh	r0, [r3, r2]
	lsrs	r3, r0, #1
	subs	r2, r4, r3
	str	r0, [sp, #0]
	subs	r3, r6, r3
	str	r0, [sp, #4]
	ldr	r4, [sp, #24]
	ldr	r0, [sp, #20]
	bl	sub_080072f4
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	adds	r0, r5, #0
	movs	r1, #62
	ldr	r2, [pc, #156]
	bl	sub_080e3908
.L_080ed366:
	movs	r4, #1
	add	r8, r4
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #16
	bne.n	.L_080ed30a
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	mov	r2, r9
	cmp	r2, #7
	bgt.n	.L_080ed38e
	movs	r0, #2
	movs	r1, #2
	bl	sub_080e155c
	b.n	.L_080ed396
.L_080ed38e:
	movs	r0, #16
	movs	r1, #16
	bl	sub_080e155c
.L_080ed396:
	bl	sub_080cd52c
	ldr	r2, [pc, #104]
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r4, #1
	ldr	r3, [sp, #8]
	add	r9, r4
	adds	r3, #6
	mov	r1, r9
	str	r3, [sp, #8]
	cmp	r1, #54
	beq.n	.L_080ed3ba
	b.n	.L_080ed232
.L_080ed3ba:
	ldr	r0, [pc, #32]
	bl	sub_08004278
	bl	sub_080cdbc0
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x04000028
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x000077a8
	.4byte 0x00007828
	.4byte 0x03001f08
	.4byte 0x080eef88
	.4byte 0x080eef96
	.4byte 0xffffe000
	.2byte 0x7824
	.2byte 0x0000
.L_080ed408:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	mov	r9, r1
	ldr	r1, [sp, #48]
	mov	sl, r1
	str	r2, [sp, #12]
	mov	r2, sl
	mov	r8, r3
	movs	r1, #3
	cmp	r2, #3
	bne.n	.L_080ed42c
	movs	r1, #6
.L_080ed42c:
	movs	r3, #12
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #4
	bne.n	.L_080ed438
	adds	r1, #3
.L_080ed438:
	cmp	r3, #8
	bne.n	.L_080ed43e
	adds	r1, #4
.L_080ed43e:
	cmp	r3, #12
	bne.n	.L_080ed444
	adds	r1, #3
.L_080ed444:
	cmp	r3, #0
	bne.n	.L_080ed44a
	adds	r1, #1
.L_080ed44a:
	movs	r3, #2
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ed456
	adds	r1, #7
.L_080ed456:
	movs	r3, #1
	mov	r2, r8
	ands	r3, r2
	adds	r1, #2
	cmp	r3, #0
	bne.n	.L_080ed464
	adds	r1, #1
.L_080ed464:
	cmp	r3, #0
	beq.n	.L_080ed46e
	adds	r1, #2
	adds	r1, #2
	adds	r1, #5
.L_080ed46e:
	movs	r3, #1
	mov	r2, r8
	ands	r3, r2
	adds	r1, #9
	cmp	r3, #0
	bne.n	.L_080ed47c
	adds	r1, #1
.L_080ed47c:
	adds	r1, #9
	cmp	r3, #0
	bne.n	.L_080ed484
	adds	r1, #1
.L_080ed484:
	mov	r3, sl
	adds	r1, #3
	cmp	r3, #1
	beq.n	.L_080ed49c
	cmp	r3, #1
	bcc.n	.L_080ed498
	cmp	r3, #2
	beq.n	.L_080ed49c
	cmp	r3, #3
	beq.n	.L_080ed4a0
.L_080ed498:
	adds	r1, #2
	b.n	.L_080ed4a2
.L_080ed49c:
	adds	r1, #4
	b.n	.L_080ed4a2
.L_080ed4a0:
	adds	r1, #6
.L_080ed4a2:
	movs	r3, #1
	mov	r2, r8
	ands	r3, r2
	adds	r1, #3
	cmp	r3, #0
	bne.n	.L_080ed4b0
	adds	r1, #2
.L_080ed4b0:
	mov	r3, sl
	adds	r1, #2
	cmp	r3, #1
	beq.n	.L_080ed4d0
	cmp	r3, #1
	bcc.n	.L_080ed4c4
	cmp	r3, #2
	beq.n	.L_080ed4d4
	cmp	r3, #3
	beq.n	.L_080ed4d8
.L_080ed4c4:
	movs	r3, #0
.L_080ed4c6:
	adds	r3, #1
	adds	r1, #2
	cmp	r3, #7
	ble.n	.L_080ed4c6
	b.n	.L_080ed4da
.L_080ed4d0:
	adds	r1, #25
	b.n	.L_080ed4da
.L_080ed4d4:
	adds	r1, #32
	b.n	.L_080ed4da
.L_080ed4d8:
	adds	r1, #28
.L_080ed4da:
	movs	r3, #1
	mov	r2, r8
	ands	r3, r2
	adds	r1, #1
	cmp	r3, #0
	bne.n	.L_080ed4e8
	adds	r1, #2
.L_080ed4e8:
	mov	r3, sl
	adds	r1, #5
	cmp	r3, #1
	beq.n	.L_080ed500
	cmp	r3, #1
	bcc.n	.L_080ed4fc
	cmp	r3, #2
	beq.n	.L_080ed500
	cmp	r3, #3
	beq.n	.L_080ed504
.L_080ed4fc:
	adds	r1, #2
	b.n	.L_080ed506
.L_080ed500:
	adds	r1, #4
	b.n	.L_080ed506
.L_080ed504:
	adds	r1, #6
.L_080ed506:
	adds	r1, #8
	lsls	r1, r1, #2
	bl	sub_080048b0
	ldr	r6, [pc, #796]
	adds	r5, r0, #0
	ldr	r3, [pc, #796]
	ldr	r2, [pc, #796]
	adds	r0, r6, #0
	adds	r1, r5, #0
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #12
	adds	r6, #12
	mov	r1, sl
	cmp	r1, #3
	bne.n	.L_080ed534
	ldr	r0, [pc, #780]
	adds	r1, r5, #0
	ldr	r2, [pc, #772]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #12
.L_080ed534:
	movs	r4, #12
	mov	r2, r8
	ands	r4, r2
	cmp	r4, #4
	bne.n	.L_080ed54c
	ldr	r3, [pc, #752]
	adds	r0, r6, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #748]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #12
.L_080ed54c:
	adds	r6, #12
	cmp	r4, #8
	bne.n	.L_080ed560
	ldr	r3, [pc, #732]
	adds	r0, r6, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #736]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #16
.L_080ed560:
	adds	r6, #16
	cmp	r4, #12
	bne.n	.L_080ed574
	ldr	r3, [pc, #712]
	adds	r0, r6, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #708]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #12
.L_080ed574:
	adds	r6, #12
	cmp	r4, #0
	bne.n	.L_080ed57e
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
.L_080ed57e:
	movs	r3, #2
	mov	r1, r8
	ands	r3, r1
	adds	r6, #4
	cmp	r3, #0
	beq.n	.L_080ed5bc
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
	ldr	r3, [r6, #4]
	stmia	r5!, {r3}
	movs	r3, #8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ed59e
	ldr	r3, [r6, #8]
	b.n	.L_080ed5a0
.L_080ed59e:
	ldr	r3, [r6, #12]
.L_080ed5a0:
	stmia	r5!, {r3}
	ldr	r3, [r6, #16]
	stmia	r5!, {r3}
	ldr	r3, [r6, #20]
	stmia	r5!, {r3}
	movs	r2, #1
	ldr	r3, [sp, #12]
	lsls	r2, r3
	ldr	r3, [r6, #24]
	adds	r3, r3, r2
	stmia	r5!, {r3}
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	stmia	r5!, {r3}
.L_080ed5bc:
	adds	r6, #32
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	str	r5, [sp, #8]
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	movs	r2, #1
	mov	r1, r8
	ands	r1, r2
	cmp	r1, #0
	bne.n	.L_080ed5de
	mov	r3, r9
	lsls	r2, r3
	ldr	r3, [r6, #0]
	adds	r3, r3, r2
	subs	r3, #1
	stmia	r5!, {r3}
.L_080ed5de:
	adds	r6, #4
	cmp	r1, #0
	beq.n	.L_080ed638
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
	ldr	r3, [r6, #4]
	stmia	r5!, {r3}
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ed5fe
	ldr	r3, [r6, #8]
	stmia	r5!, {r3}
	ldr	r3, [r6, #12]
	b.n	.L_080ed604
.L_080ed5fe:
	ldr	r3, [r6, #16]
	stmia	r5!, {r3}
	ldr	r3, [r6, #20]
.L_080ed604:
	stmia	r5!, {r3}
	ldr	r3, [r6, #24]
	stmia	r5!, {r3}
	ldr	r3, [r6, #28]
	stmia	r5!, {r3}
	ldr	r3, [pc, #560]
	mov	r2, r9
	lsls	r1, r2, #1
	ldrh	r2, [r3, r1]
	ldr	r3, [r6, #32]
	adds	r3, r3, r2
	stmia	r5!, {r3}
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ed62a
	ldr	r3, [r6, #36]
	b.n	.L_080ed62c
.L_080ed62a:
	ldr	r3, [r6, #40]
.L_080ed62c:
	stmia	r5!, {r3}
	ldr	r3, [pc, #528]
	ldrh	r2, [r3, r1]
	ldr	r3, [r6, #44]
	adds	r3, r3, r2
	stmia	r5!, {r3}
.L_080ed638:
	adds	r6, #48
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	mov	fp, r5
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	ldr	r3, [pc, #488]
	ldr	r2, [pc, #508]
	adds	r0, r6, #0
	adds	r1, r5, #0
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #24
	adds	r6, #24
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	movs	r7, #1
	mov	r4, r8
	ands	r4, r7
	cmp	r4, #0
	bne.n	.L_080ed666
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
.L_080ed666:
	adds	r6, #4
	ldr	r3, [pc, #452]
	ldr	r2, [pc, #476]
	adds	r0, r6, #0
	adds	r1, r5, #0
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #20
	adds	r6, #20
	str	r5, [sp, #4]
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
	ldr	r3, [sp, #12]
	subs	r3, #3
	adds	r2, r7, #0
	lsls	r2, r3
	ldr	r3, [r6, #4]
	adds	r3, r3, r2
	subs	r3, #1
	stmia	r5!, {r3}
	mov	r2, r9
	ldr	r3, [r6, #8]
	subs	r2, #3
	lsls	r2, r2, #7
	adds	r3, r3, r2
	stmia	r5!, {r3}
	ldr	r3, [r6, #12]
	stmia	r5!, {r3}
	adds	r6, #16
	cmp	r4, #0
	bne.n	.L_080ed6a8
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
.L_080ed6a8:
	adds	r6, #4
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	mov	ip, r5
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ed6c2
	ldr	r3, [r6, #0]
	b.n	.L_080ed6c4
.L_080ed6c2:
	ldr	r3, [r6, #4]
.L_080ed6c4:
	stmia	r5!, {r3}
	mov	r2, sl
	adds	r6, #8
	adds	r4, r5, #0
	cmp	r2, #1
	beq.n	.L_080ed6fc
	cmp	r2, #1
	bcc.n	.L_080ed6dc
	cmp	r2, #2
	beq.n	.L_080ed712
	cmp	r2, #3
	beq.n	.L_080ed732
.L_080ed6dc:
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ed6ec
	ldr	r3, [pc, #328]
	ldr	r0, [pc, #352]
	b.n	.L_080ed6f0
.L_080ed6ec:
	ldr	r3, [pc, #320]
	ldr	r0, [pc, #352]
.L_080ed6f0:
	adds	r1, r5, #0
	ldr	r2, [pc, #352]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #8
	b.n	.L_080ed750
.L_080ed6fc:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ed70c
	ldr	r3, [pc, #296]
	ldr	r0, [pc, #332]
	b.n	.L_080ed726
.L_080ed70c:
	ldr	r3, [pc, #288]
	ldr	r0, [pc, #332]
	b.n	.L_080ed726
.L_080ed712:
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ed722
	ldr	r3, [pc, #272]
	ldr	r0, [pc, #320]
	b.n	.L_080ed726
.L_080ed722:
	ldr	r3, [pc, #268]
	ldr	r0, [pc, #316]
.L_080ed726:
	adds	r1, r5, #0
	ldr	r2, [pc, #272]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #16
	b.n	.L_080ed750
.L_080ed732:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ed742
	ldr	r3, [pc, #240]
	ldr	r0, [pc, #296]
	b.n	.L_080ed746
.L_080ed742:
	ldr	r3, [pc, #236]
	ldr	r0, [pc, #292]
.L_080ed746:
	adds	r1, r5, #0
	ldr	r2, [pc, #248]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #24
.L_080ed750:
	adds	r6, #16
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	subs	r3, r4, r5
	subs	r3, #8
	ldr	r1, [pc, #276]
	ldmia	r6!, {r2}
	lsrs	r3, r3, #2
	ands	r3, r1
	adds	r2, r2, r3
	stmia	r5!, {r2}
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	movs	r3, #1
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080ed77c
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
	ldr	r3, [r6, #4]
	stmia	r5!, {r3}
.L_080ed77c:
	adds	r6, #8
	mov	r2, ip
	subs	r3, r5, r2
	subs	r3, #8
	ldr	r2, [r2, #0]
	lsrs	r3, r3, #2
	ands	r3, r1
	orrs	r2, r3
	mov	r3, ip
	str	r2, [r3, #0]
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	mov	ip, r5
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	str	r5, [sp, #0]
	mov	r1, sl
	cmp	r1, #1
	beq.n	.L_080ed7de
	cmp	r1, #1
	bcc.n	.L_080ed7ae
	cmp	r1, #2
	beq.n	.L_080ed7fe
	cmp	r1, #3
	beq.n	.L_080ed88c
.L_080ed7ae:
	ldr	r3, [pc, #156]
	ldr	r1, [pc, #156]
	movs	r7, #4
	mov	r2, r8
	movs	r4, #0
	ands	r7, r2
	mov	r9, r3
	mov	lr, r1
.L_080ed7be:
	cmp	r7, #0
	beq.n	.L_080ed7c8
	ldr	r3, [pc, #108]
	mov	r0, r9
	b.n	.L_080ed7cc
.L_080ed7c8:
	ldr	r3, [pc, #100]
	mov	r0, lr
.L_080ed7cc:
	adds	r1, r5, #0
	ldr	r2, [pc, #132]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #8
	adds	r4, #1
	cmp	r4, #7
	ble.n	.L_080ed7be
	b.n	.L_080ed8b6
.L_080ed7de:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ed7ee
	ldr	r3, [pc, #68]
	ldr	r0, [pc, #136]
	b.n	.L_080ed7f2
.L_080ed7ee:
	ldr	r3, [pc, #64]
	ldr	r0, [pc, #132]
.L_080ed7f2:
	adds	r1, r5, #0
	ldr	r2, [pc, #132]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #100
	b.n	.L_080ed8b6
.L_080ed7fe:
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ed80c
	ldr	r4, [pc, #116]
	b.n	.L_080ed80e
.L_080ed80c:
	ldr	r4, [pc, #116]
.L_080ed80e:
	ldr	r3, [pc, #32]
	adds	r0, r4, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #112]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #64
	adds	r0, r4, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #100]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #64
	b.n	.L_080ed8b6
	movs	r0, r0
	.4byte 0x080edcc4
	.4byte 0x040000d4
	.4byte 0x84000003
	.4byte 0x080edcb8
	.4byte 0x84000004
	.4byte 0x080ef034
	.4byte 0x84000006
	.4byte 0x84000005
	.4byte 0x080edaf0
	.4byte 0x080edaf8
	.4byte 0x84000002
	.4byte 0x080edb10
	.4byte 0x080edb00
	.4byte 0x080edbf8
	.4byte 0x080edbe8
	.4byte 0x080edca0
	.4byte 0x080edc88
	.4byte 0x00ffffff
	.4byte 0x080edb84
	.4byte 0x080edb20
	.4byte 0x84000019
	.4byte 0x080edc48
	.4byte 0x080edc08
	.2byte 0x0010
	.2byte 0x8400
.L_080ed88c:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ed89a
	ldr	r4, [pc, #408]
	b.n	.L_080ed89c
.L_080ed89a:
	ldr	r4, [pc, #408]
.L_080ed89c:
	ldr	r3, [pc, #408]
	adds	r0, r4, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #408]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #56
	adds	r0, r4, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #396]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #56
.L_080ed8b6:
	adds	r6, #16
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	movs	r3, #1
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_080ed8ce
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
	ldr	r3, [r6, #4]
	stmia	r5!, {r3}
.L_080ed8ce:
	adds	r6, #8
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	ldr	r2, [sp, #0]
	subs	r3, r2, r5
	ldr	r1, [pc, #356]
	subs	r3, #8
	ldmia	r6!, {r2}
	lsrs	r3, r3, #2
	ands	r3, r1
	adds	r2, r2, r3
	stmia	r5!, {r2}
	mov	r2, ip
	subs	r3, r5, r2
	subs	r3, #8
	ldr	r2, [r2, #0]
	lsrs	r3, r3, #2
	ands	r3, r1
	orrs	r2, r3
	mov	r3, ip
	str	r2, [r3, #0]
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	mov	ip, r5
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ed910
	ldr	r3, [r6, #0]
	b.n	.L_080ed912
.L_080ed910:
	ldr	r3, [r6, #4]
.L_080ed912:
	stmia	r5!, {r3}
	mov	r2, sl
	adds	r6, #8
	adds	r4, r5, #0
	cmp	r2, #1
	beq.n	.L_080ed94a
	cmp	r2, #1
	bcc.n	.L_080ed92a
	cmp	r2, #2
	beq.n	.L_080ed960
	cmp	r2, #3
	beq.n	.L_080ed980
.L_080ed92a:
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ed93a
	ldr	r3, [pc, #256]
	ldr	r0, [pc, #268]
	b.n	.L_080ed93e
.L_080ed93a:
	ldr	r3, [pc, #252]
	ldr	r0, [pc, #264]
.L_080ed93e:
	adds	r1, r5, #0
	ldr	r2, [pc, #264]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #8
	b.n	.L_080ed99e
.L_080ed94a:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ed95a
	ldr	r3, [pc, #224]
	ldr	r0, [pc, #248]
	b.n	.L_080ed974
.L_080ed95a:
	ldr	r3, [pc, #220]
	ldr	r0, [pc, #244]
	b.n	.L_080ed974
.L_080ed960:
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ed970
	ldr	r3, [pc, #204]
	ldr	r0, [pc, #232]
	b.n	.L_080ed974
.L_080ed970:
	ldr	r3, [pc, #196]
	ldr	r0, [pc, #232]
.L_080ed974:
	adds	r1, r5, #0
	ldr	r2, [pc, #232]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #16
	b.n	.L_080ed99e
.L_080ed980:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ed990
	ldr	r3, [pc, #172]
	ldr	r0, [pc, #212]
	b.n	.L_080ed994
.L_080ed990:
	ldr	r3, [pc, #164]
	ldr	r0, [pc, #212]
.L_080ed994:
	adds	r1, r5, #0
	ldr	r2, [pc, #212]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #24
.L_080ed99e:
	adds	r6, #16
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	subs	r3, r4, r5
	subs	r3, #8
	ldr	r4, [pc, #148]
	ldmia	r6!, {r2}
	lsrs	r3, r3, #2
	ands	r3, r4
	adds	r2, r2, r3
	stmia	r5!, {r2}
	mov	r1, ip
	subs	r3, r5, r1
	subs	r3, #8
	ldr	r2, [r1, #0]
	lsrs	r3, r3, #2
	ands	r3, r4
	orrs	r2, r3
	str	r2, [r1, #0]
	ldr	r3, [pc, #112]
	ldr	r2, [pc, #168]
	adds	r0, r6, #0
	adds	r1, r5, #0
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #12
	adds	r6, #12
	ldr	r2, [sp, #4]
	subs	r3, r2, r5
	subs	r3, #8
	ldmia	r6!, {r2}
	lsrs	r3, r3, #2
	ands	r3, r4
	adds	r2, r2, r3
	stmia	r5!, {r2}
	ldr	r1, [sp, #8]
	subs	r3, r5, r1
	subs	r3, #8
	ldr	r2, [r1, #0]
	lsrs	r3, r3, #2
	ands	r3, r4
	orrs	r2, r3
	str	r2, [r1, #0]
	mov	r2, fp
	subs	r3, r5, r2
	subs	r3, #8
	ldr	r2, [r2, #0]
	lsrs	r3, r3, #2
	ands	r3, r4
	orrs	r2, r3
	mov	r3, fp
	str	r2, [r3, #0]
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	ldmia	r6!, {r3}
	str	r3, [r5, #0]
	ldr	r3, [pc, #100]
	eors	r3, r6
	negs	r2, r3
	orrs	r2, r3
	lsrs	r2, r2, #31
	movs	r0, #1
	subs	r0, r0, r2
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x080eefdc
	.4byte 0x080eefa4
	.4byte 0x040000d4
	.4byte 0x8400000e
	.4byte 0x00ffffff
	.4byte 0x080edaf0
	.4byte 0x080edaf8
	.4byte 0x84000002
	.4byte 0x080edb10
	.4byte 0x080edb00
	.4byte 0x080edbf8
	.4byte 0x080edbe8
	.4byte 0x84000004
	.4byte 0x080edca0
	.4byte 0x080edc88
	.4byte 0x84000006
	.4byte 0x84000003
	.4byte 0x080ede48
