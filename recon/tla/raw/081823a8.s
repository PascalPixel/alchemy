.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014de4, 0x08014de4
	.set sub_08014e38, 0x08014e38
	.set sub_08014ea8, 0x08014ea8
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015128, 0x08015128
	.set sub_080156e8, 0x080156e8
	.set sub_08118098, 0x08118098
	.set sub_08138058, 0x08138058
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_0818253e, 0x0818253e
	.set sub_08182898, 0x08182898
	.set sub_081963ec, 0x081963ec
	.global Func_081823a8
	.thumb_func
Func_081823a8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #84
	str	r0, [sp, #68]
	str	r1, [sp, #64]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r1, [r5, #92]
	movs	r0, #0
	str	r1, [sp, #60]
	ldr	r2, [r5, #96]
	str	r2, [sp, #56]
	ldr	r3, [r5, #100]
	str	r3, [sp, #40]
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r4, [sp, #60]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #180
	adds	r2, r4, r1
	movs	r3, #24
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #184
	adds	r2, r4, r3
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r1, [sp, #40]
	ldr	r0, [pc, #24]
	movs	r2, #0
	bl	sub_08157cf4
	movs	r0, #104
	movs	r1, #7
	bl	sub_081963ec
	ldr	r5, [r5, #104]
	ldr	r4, [sp, #64]
	str	r5, [sp, #48]
	b.n	.L_08182418
	.4byte 0x00001010
	.2byte 0x0134
	.2byte 0x0000
.L_08182418:
	cmp	r4, #0
	bne.n	.L_08182428
	ldr	r1, [sp, #68]
	ldr	r3, [r1, #20]
	lsls	r3, r3, #3
	adds	r3, #38
	str	r3, [sp, #36]
	b.n	.L_0818242c
.L_08182428:
	movs	r2, #72
	str	r2, [sp, #36]
.L_0818242c:
	ldr	r3, [sp, #60]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r1, [sp, #60]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r1, r3
	movs	r3, #75
	movs	r1, #200
	lsls	r1, r1, #4
	str	r3, [r2, #0]
	ldr	r0, [pc, #596]
	bl	sub_080145a8
	ldr	r1, [sp, #36]
	movs	r4, #0
	mov	r9, r4
	cmp	r1, #0
	bne.n	.L_0818245c
	b.n	.L_08182826
.L_0818245c:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #48]
	mov	r2, r9
	str	r3, [sp, #32]
	cmp	r2, #0
	bne.n	sub_0818253e
	ldr	r4, [sp, #68]
	movs	r3, #0
	mov	r8, r3
	ldr	r3, [r4, #20]
	cmp	r3, #0
	beq.n	.L_081824c2
	ldr	r5, [sp, #60]
	movs	r7, #0
	movs	r6, #36
.L_0818247c:
	ldr	r1, [sp, #68]
	ldrsh	r0, [r6, r1]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r3, [r0, #8]
	str	r7, [r5, #4]
	str	r3, [r5, #0]
	ldr	r3, [r0, #16]
	str	r3, [r5, #8]
	ldr	r3, [sp, #64]
	cmp	r3, #1
	bne.n	.L_0818249e
	movs	r3, #128
	lsls	r3, r3, #8
	str	r7, [r5, #12]
	b.n	.L_081824ae
.L_0818249e:
	ldr	r0, [r0, #8]
	movs	r1, #40
	negs	r0, r0
	bl	sub_08002054
	movs	r3, #192
	lsls	r3, r3, #10
	str	r0, [r5, #12]
.L_081824ae:
	str	r3, [r5, #16]
	str	r7, [r5, #20]
	ldr	r1, [sp, #68]
	movs	r4, #1
	ldr	r3, [r1, #20]
	add	r8, r4
	adds	r5, #28
	adds	r6, #2
	cmp	r8, r3
	bne.n	.L_0818247c
.L_081824c2:
	ldr	r7, [pc, #480]
	movs	r2, #0
	mov	r8, r2
	mov	sl, r2
.L_081824ca:
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08014878
	mov	r3, sl
	str	r3, [r7, #0]
	movs	r5, #255
	ands	r5, r0
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r3, #20
	mov	r4, sl
	lsls	r3, r3, #16
	str	r4, [r7, #8]
	str	r3, [r7, #4]
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #128
	adds	r3, r5, #0
	muls	r3, r0
	mov	r1, sl
	asrs	r3, r3, #5
	str	r3, [r7, #12]
	str	r1, [r7, #16]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #20]
	movs	r4, #192
	movs	r3, #1
	mov	r2, sl
	add	r8, r3
	lsls	r4, r4, #1
	str	r2, [r7, #24]
	adds	r7, #28
	cmp	r8, r4
	bne.n	.L_081824ca
	ldr	r0, [pc, #380]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #372]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2100
	mov	r8, r1
.L_08182542:
	mov	r2, r9
	cmp	r2, #7
	ble.n	.L_08182568
	cmp	r2, #19
	bgt.n	.L_08182554
	ldr	r0, [pc, #352]
	bl	sub_0815f0a0
	b.n	.L_08182568
.L_08182554:
	mov	r3, r9
	cmp	r3, #31
	bgt.n	.L_08182562
	ldr	r0, [pc, #344]
	bl	sub_0815f0a0
	b.n	.L_08182568
.L_08182562:
	ldr	r0, [pc, #340]
	bl	sub_0815f0a0
.L_08182568:
	movs	r4, #1
	add	r8, r4
	mov	r1, r8
	cmp	r1, #2
	bne.n	.L_08182542
	movs	r2, #0
	str	r2, [sp, #44]
	ldr	r4, [sp, #68]
	ldr	r3, [r4, #20]
	cmp	r3, #0
	bne.n	.L_08182580
	b.n	.L_081827fa
.L_08182580:
	ldr	r1, [sp, #32]
	ldr	r3, [sp, #60]
	adds	r1, #12
	movs	r2, #36
	movs	r4, #0
	str	r1, [sp, #20]
	str	r2, [sp, #16]
	str	r3, [sp, #12]
	str	r4, [sp, #8]
.L_08182592:
	ldr	r1, [sp, #44]
	ldr	r3, [sp, #64]
	lsls	r1, r1, #3
	movs	r2, #16
	mov	sl, r1
	str	r2, [sp, #28]
	cmp	r3, #0
	beq.n	.L_081825a6
	movs	r4, #48
	str	r4, [sp, #28]
.L_081825a6:
	cmp	r9, sl
	bne.n	.L_081825dc
	ldr	r1, [sp, #64]
	cmp	r1, #0
	bne.n	.L_081825c6
	ldr	r2, [sp, #16]
	ldr	r4, [sp, #68]
	movs	r1, #7
	ldrsh	r0, [r2, r4]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r2, #5
	ldr	r3, [sp, #44]
	bl	sub_0814cd48
	b.n	.L_081825dc
.L_081825c6:
	ldr	r1, [sp, #16]
	ldr	r3, [sp, #68]
	ldrsh	r0, [r1, r3]
	movs	r3, #16
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	ldr	r3, [sp, #44]
	bl	sub_0814cd48
.L_081825dc:
	mov	r3, sl
	adds	r3, #55
	cmp	r9, r3
	bne.n	.L_08182600
	ldr	r4, [sp, #64]
	cmp	r4, #0
	beq.n	.L_08182600
	ldr	r1, [sp, #16]
	ldr	r3, [sp, #68]
	ldrsh	r0, [r1, r3]
	movs	r3, #16
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	ldr	r3, [sp, #44]
	bl	sub_0814cd48
.L_08182600:
	bl	sub_08014de4
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #20]
	bl	sub_080156e8
	mov	r3, sl
	adds	r3, #4
	cmp	r9, r3
	blt.n	.L_08182624
	mov	r4, sl
	cmp	r4, #43
	bgt.n	.L_08182624
	ldr	r0, [sp, #12]
	movs	r1, #64
	movs	r2, #0
	bl	sub_08138058
.L_08182624:
	ldr	r0, [sp, #12]
	bl	sub_08015128
	ldr	r2, [sp, #28]
	movs	r1, #0
	mov	r8, r1
	cmp	r2, #0
	bne.n	.L_08182636
	b.n	.L_081827d2
.L_08182636:
	mov	r3, r9
	negs	r3, r3
	ldr	r1, [sp, #8]
	ldr	r2, [pc, #100]
	mov	r4, r9
	str	r3, [sp, #24]
	lsls	r4, r4, #8
	lsls	r7, r3, #8
	mov	fp, r4
	adds	r6, r1, r2
.L_0818264a:
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_08182652
	adds	r3, #7
.L_08182652:
	asrs	r3, r3, #3
	add	r3, sl
	cmp	r9, r3
	bge.n	.L_0818265c
	b.n	.L_081827b8
.L_0818265c:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	beq.n	.L_08182664
	b.n	.L_081827b8
.L_08182664:
	bl	sub_08014e38
	ldr	r2, [sp, #44]
	movs	r3, #3
	add	r2, r8
	ands	r2, r3
	cmp	r2, #1
	beq.n	.L_08182690
	cmp	r2, #1
	bgt.n	.L_0818267e
	cmp	r2, #0
	beq.n	.L_08182688
	b.n	.L_081826c8
.L_0818267e:
	cmp	r2, #2
	beq.n	.L_08182698
	cmp	r2, #3
	beq.n	.L_081826bc
	b.n	.L_081826c8
.L_08182688:
	mov	r0, fp
	bl	sub_08015068
	b.n	.L_081826c8
.L_08182690:
	adds	r0, r7, #0
	bl	sub_08015024
	b.n	.L_081826c8
.L_08182698:
	adds	r0, r7, #0
	bl	sub_080150e4
	b.n	.L_081826c8
	.4byte 0x08143001
	.4byte 0x02010000
	.4byte 0x00000184
	.4byte 0x03000730
	.4byte 0x00000154
	.4byte 0x00000150
	.2byte 0x0152
	.2byte 0x0000
.L_081826bc:
	adds	r0, r7, #0
	bl	sub_08015024
	adds	r0, r7, #0
	bl	sub_080150e4
.L_081826c8:
	add	r5, sp, #72
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_0815e1ec
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	bl	sub_08014ea8
	ldr	r2, [r5, #8]
	cmp	r2, #249
	bgt.n	.L_081826e8
	movs	r3, #250
	str	r3, [r5, #8]
	movs	r2, #250
.L_081826e8:
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #122
	cmp	r2, r3
	ble.n	.L_081826f6
	str	r3, [r5, #8]
	adds	r2, r3, #0
.L_081826f6:
	adds	r3, r2, #0
	subs	r3, #250
	cmp	r3, #0
	bge.n	.L_08182700
	adds	r3, #63
.L_08182700:
	asrs	r3, r3, #6
	movs	r0, #9
	subs	r0, r0, r3
	ldr	r2, [pc, #316]
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #40]
	ldr	r2, [r5, #0]
	adds	r1, r3, r1
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #56]
	ldr	r4, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c30
	movs	r1, #60
	movs	r2, #0
	bl	sub_08138058
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_0818273e
	adds	r3, #7
.L_0818273e:
	asrs	r3, r3, #3
	adds	r3, #24
	cmp	r9, r3
	blt.n	.L_081827b8
	ldr	r3, [r6, #0]
	ldr	r2, [r6, #4]
	negs	r3, r3
	asrs	r5, r3, #7
	ldr	r3, [r6, #8]
	negs	r2, r2
	negs	r3, r3
	asrs	r4, r3, #7
	ldr	r3, [r6, #16]
	ldr	r1, [r6, #12]
	asrs	r2, r2, #7
	adds	r2, r3, r2
	ldr	r3, [r6, #20]
	adds	r1, r1, r5
	adds	r0, r3, r4
	lsls	r3, r1, #5
	subs	r3, r3, r1
	lsls	r3, r3, #1
	str	r1, [r6, #12]
	str	r2, [r6, #16]
	str	r0, [r6, #20]
	cmp	r3, #0
	bge.n	.L_08182776
	adds	r3, #63
.L_08182776:
	asrs	r3, r3, #6
	str	r3, [r6, #12]
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r3, r3, #1
	cmp	r3, #0
	bge.n	.L_08182786
	adds	r3, #63
.L_08182786:
	asrs	r3, r3, #6
	str	r3, [r6, #16]
	lsls	r3, r0, #5
	subs	r3, r3, r0
	lsls	r3, r3, #1
	cmp	r3, #0
	bge.n	.L_08182796
	adds	r3, #63
.L_08182796:
	movs	r1, #224
	lsls	r1, r1, #3
	movs	r2, #240
	asrs	r3, r3, #6
	adds	r1, #255
	lsls	r2, r2, #4
	str	r3, [r6, #20]
	adds	r2, #254
	adds	r3, r5, r1
	cmp	r3, r2
	bhi.n	.L_081827b8
	adds	r3, r4, r1
	cmp	r3, r2
	bhi.n	.L_081827b8
	movs	r3, #1
	negs	r3, r3
	str	r3, [r6, #24]
.L_081827b8:
	ldr	r2, [sp, #24]
	mov	r4, r9
	lsls	r3, r2, #5
	ldr	r2, [sp, #28]
	movs	r1, #1
	adds	r7, r7, r3
	add	r8, r1
	lsls	r3, r4, #5
	add	fp, r3
	adds	r6, #28
	cmp	r8, r2
	beq.n	.L_081827d2
	b.n	.L_0818264a
.L_081827d2:
	ldr	r3, [sp, #16]
	ldr	r4, [sp, #12]
	adds	r3, #2
	str	r3, [sp, #16]
	ldr	r1, [sp, #8]
	ldr	r3, [sp, #44]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r4, #28
	adds	r1, r1, r2
	adds	r3, #1
	str	r4, [sp, #12]
	str	r1, [sp, #8]
	str	r3, [sp, #44]
	ldr	r4, [sp, #68]
	ldr	r1, [sp, #44]
	ldr	r3, [r4, #20]
	cmp	r1, r3
	beq.n	.L_081827fa
	b.n	.L_08182592
.L_081827fa:
	movs	r1, #16
	movs	r0, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r4, #240
	ldr	r3, [sp, #60]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #36]
	movs	r1, #1
	add	r9, r1
	cmp	r9, r2
	beq.n	.L_08182826
	b.n	.L_0818245c
.L_08182826:
	ldr	r0, [pc, #32]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #84
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x08197410
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, lr}
	movs	r3, #192
	adds	r6, r0, #0
	lsls	r3, r3, #18
	movs	r0, #0
	ldr	r5, [r3, #92]
	bl	sub_081435e0
	ldr	r3, [pc, #44]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #2
	strh	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #180
	adds	r2, r5, r3
	movs	r3, #24
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #184
	adds	r5, r5, r3
	movs	r3, #0
	str	r3, [r5, #0]
	adds	r0, r6, #0
	bl	sub_08182898
	b.n	.L_08182894
	.4byte 0x00003f44
	.2byte 0x1010
	.2byte 0x0000
.L_08182894:
	pop	{r5, r6, pc}
	.2byte 0x0000
