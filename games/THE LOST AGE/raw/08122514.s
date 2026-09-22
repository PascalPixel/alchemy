.syntax unified
	.thumb
	.set sub_08002064, 0x08002064
	.set sub_08014878, 0x08014878
	.set sub_08016ca4, 0x08016ca4
	.set sub_080ad010, 0x080ad010
	.set sub_080ad078, 0x080ad078
	.set sub_080ad140, 0x080ad140
	.set sub_080ad148, 0x080ad148
	.set sub_080ad180, 0x080ad180
	.set sub_080ad188, 0x080ad188
	.set sub_0811a490, 0x0811a490
	.set sub_0811d748, 0x0811d748
	.set sub_0811f4d4, 0x0811f4d4
	.set sub_0811f66c, 0x0811f66c
	.set sub_081224c0, 0x081224c0
	.set sub_081224d8, 0x081224d8
	.set sub_08123574, 0x08123574
	.global Overlay_08122514
Overlay_08122514:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #72
	str	r1, [sp, #28]
	adds	r7, r0, #0
	movs	r1, #0
	ldrsh	r0, [r7, r1]
	bl	sub_08016ca4
	str	r0, [sp, #24]
	movs	r2, #1
	movs	r3, #0
	ldr	r1, [sp, #24]
	str	r2, [sp, #20]
	str	r3, [sp, #16]
	str	r2, [sp, #12]
	movs	r2, #42
	adds	r2, #255
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	movs	r0, #1
	negs	r0, r0
	mov	fp, r0
	cmp	r3, #0
	beq.n	.L_08122552
	b.n	.L_08122c08
.L_08122552:
	ldr	r1, [sp, #24]
	ldr	r3, [sp, #24]
	movs	r2, #165
	lsls	r2, r2, #1
	movs	r0, #144
	adds	r5, r1, r2
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldrh	r0, [r5, #0]
	mov	r8, r3
	bl	sub_080ad140
	ldrh	r3, [r5, #0]
	mov	sl, r0
	cmp	r3, #101
	bne.n	.L_08122594
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #14
	ldrsh	r1, [r7, r0]
	ldr	r3, [r3, #36]
	lsls	r2, r1, #2
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r2, #108
	adds	r2, r2, r3
	mov	r8, r2
	cmp	r1, #1
	beq.n	.L_08122590
	b.n	.L_081226c0
.L_08122590:
	movs	r0, #102
	b.n	.L_081226ba
.L_08122594:
	movs	r0, #114
	adds	r0, #255
	cmp	r3, r0
	bne.n	.L_081225fe
	movs	r2, #192
	lsls	r2, r2, #18
	movs	r1, #14
	ldrsh	r0, [r7, r1]
	ldr	r4, [r2, #36]
	movs	r2, #128
	lsls	r3, r0, #2
	lsls	r2, r2, #4
	adds	r3, r4, r3
	adds	r2, #108
	adds	r2, r2, r3
	mov	r8, r2
	cmp	r0, #2
	beq.n	.L_081225da
	cmp	r0, #2
	bgt.n	.L_081225c2
	cmp	r0, #1
	beq.n	.L_081225c8
	b.n	.L_081226c0
.L_081225c2:
	cmp	r0, #3
	beq.n	.L_081225ec
	b.n	.L_081226c0
.L_081225c8:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #105
	adds	r3, r4, r0
	movs	r2, #6
	movs	r0, #116
	strb	r2, [r3, #0]
	adds	r0, #255
	b.n	.L_081226ba
.L_081225da:
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r1, #105
	adds	r3, r4, r1
	movs	r2, #3
	movs	r0, #185
	strb	r2, [r3, #0]
	lsls	r0, r0, #1
	b.n	.L_081226ba
.L_081225ec:
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r2, #105
	adds	r3, r4, r2
	movs	r0, #186
	movs	r2, #7
	strb	r2, [r3, #0]
	lsls	r0, r0, #1
	b.n	.L_081226ba
.L_081225fe:
	movs	r0, #118
	adds	r0, #255
	cmp	r3, r0
	bne.n	.L_0812264c
	movs	r2, #192
	lsls	r2, r2, #18
	movs	r1, #14
	ldrsh	r0, [r7, r1]
	ldr	r4, [r2, #36]
	movs	r2, #128
	lsls	r3, r0, #2
	lsls	r2, r2, #4
	adds	r3, r4, r3
	adds	r2, #108
	adds	r2, r2, r3
	mov	r8, r2
	cmp	r0, #1
	beq.n	.L_08122628
	cmp	r0, #2
	beq.n	.L_0812263a
	b.n	.L_081226c0
.L_08122628:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #105
	adds	r3, r4, r0
	movs	r2, #6
	movs	r0, #120
	strb	r2, [r3, #0]
	adds	r0, #255
	b.n	.L_081226ba
.L_0812263a:
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r1, #105
	adds	r3, r4, r1
	movs	r2, #3
	movs	r0, #187
	strb	r2, [r3, #0]
	lsls	r0, r0, #1
	b.n	.L_081226ba
.L_0812264c:
	movs	r2, #188
	lsls	r2, r2, #1
	cmp	r3, r2
	bne.n	.L_08122682
	movs	r3, #192
	movs	r0, #14
	ldrsh	r1, [r7, r0]
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r0, #128
	lsls	r2, r1, #2
	lsls	r0, r0, #4
	adds	r2, r3, r2
	adds	r0, #108
	adds	r0, r0, r2
	mov	r8, r0
	cmp	r1, #1
	bne.n	.L_081226c0
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r1, #105
	adds	r3, r3, r1
	movs	r2, #3
	movs	r0, #122
	strb	r2, [r3, #0]
	adds	r0, #255
	b.n	.L_081226ba
.L_08122682:
	cmp	r3, #218
	bne.n	.L_081226c0
	mov	r3, r8
	ldr	r2, [r3, #0]
	lsls	r3, r2, #31
	cmp	r3, #0
	bne.n	.L_081226b0
	bl	sub_08014878
	lsls	r2, r0, #1
	adds	r2, r2, r0
	mov	r0, r8
	movs	r3, #3
	ldrb	r1, [r0, #0]
	lsrs	r2, r2, #16
	ands	r2, r3
	movs	r3, #49
	negs	r3, r3
	lsls	r2, r2, #4
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r0, #0]
	ldr	r2, [r0, #0]
.L_081226b0:
	ldr	r3, [pc, #776]
	lsls	r2, r2, #26
	lsrs	r2, r2, #30
	lsls	r2, r2, #1
	ldrh	r0, [r3, r2]
.L_081226ba:
	bl	sub_080ad140
	mov	sl, r0
.L_081226c0:
	ldr	r1, [sp, #28]
	cmp	r1, #0
	beq.n	.L_08122702
	movs	r2, #6
	ldrsh	r3, [r7, r2]
	cmp	r3, #4
	beq.n	.L_08122702
	mov	r3, sl
	adds	r3, #44
	movs	r0, #0
	ldrsb	r0, [r3, r0]
	cmp	r0, #4
	ble.n	.L_081226dc
	b.n	.L_08122bc6
.L_081226dc:
	cmp	r0, #3
	bge.n	.L_081226e2
	b.n	.L_08122bc6
.L_081226e2:
	mov	r3, r8
	ldr	r2, [r3, #0]
	mov	r0, r8
	lsls	r2, r2, #28
	lsrs	r2, r2, #29
	movs	r3, #7
	ldrb	r1, [r0, #0]
	adds	r2, #1
	ands	r2, r3
	movs	r3, #15
	negs	r3, r3
	lsls	r2, r2, #1
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r0, #0]
	b.n	.L_08122bc6
.L_08122702:
	mov	r1, sl
	mov	r2, sl
	mov	r3, sl
	adds	r1, #44
	adds	r2, #45
	adds	r3, #2
	str	r1, [sp, #4]
	str	r2, [sp, #0]
	str	r3, [sp, #8]
.L_08122714:
	ldr	r0, [sp, #4]
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	cmp	r3, #6
	bhi.n	.L_081227bc
	ldr	r2, [pc, #672]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x08122744
	.4byte 0x0812274c
	.4byte 0x08122754
	.4byte 0x08122762
	.4byte 0x0812278c
	.4byte 0x081227b8
	.2byte 0x27bc
	.2byte 0x0812
	add	r1, sp, #72
	ldr	r0, [pc, #636]
	mov	r9, r1
	b.n	.L_0812275a
	add	r2, sp, #72
	ldr	r0, [pc, #632]
	mov	r9, r2
	b.n	.L_0812275a
	add	r3, sp, #72
	ldr	r0, [pc, #628]
	mov	r9, r3
.L_0812275a:
	bl	sub_081224d8
	mov	fp, r0
	b.n	.L_081227bc
	mov	r0, r8
	ldr	r2, [r0, #0]
	lsls	r3, r2, #31
	cmp	r3, #0
	bne.n	.L_08122790
	bl	sub_080ad148
	mov	r1, r8
	ldrb	r2, [r1, #0]
	movs	r3, #7
	ands	r0, r3
	movs	r3, #15
	negs	r3, r3
	ands	r3, r2
	lsls	r0, r0, #1
	movs	r2, #1
	orrs	r3, r0
	orrs	r3, r2
	strb	r3, [r1, #0]
	ldr	r2, [r1, #0]
	b.n	.L_08122790
	mov	r3, r8
	ldr	r2, [r3, #0]
.L_08122790:
	ldr	r0, [sp, #28]
	lsls	r3, r2, #28
	lsrs	r3, r3, #29
	mov	fp, r3
	cmp	r0, #0
	beq.n	.L_081227bc
	mov	r2, fp
	movs	r3, #7
	adds	r2, #1
	ands	r2, r3
	mov	r3, r8
	ldrb	r1, [r3, #0]
	movs	r3, #15
	negs	r3, r3
	lsls	r2, r2, #1
	ands	r3, r1
	orrs	r3, r2
	mov	r0, r8
	strb	r3, [r0, #0]
	b.n	.L_081227bc
	movs	r1, #1
	add	fp, r1
.L_081227bc:
	ldr	r2, [sp, #0]
	mov	r3, fp
	ldrb	r6, [r2, #0]
	mov	r1, fp
	asrs	r6, r3
	ldr	r2, [sp, #8]
	movs	r3, #1
	ands	r6, r3
	lsls	r3, r1, #1
	adds	r3, #44
	ldr	r0, [sp, #20]
	ldrh	r3, [r2, r3]
	ands	r6, r0
	mov	r9, r3
	movs	r3, #4
	strh	r3, [r7, #6]
	cmp	r6, #0
	beq.n	.L_08122844
	ldr	r3, [sp, #28]
	cmp	r3, #0
	beq.n	.L_08122844
	ldr	r1, [sp, #24]
	movs	r3, #128
	adds	r1, #216
	ldrh	r2, [r1, #0]
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08122814
	mov	r3, sl
	adds	r3, #43
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r6, #0
	cmp	r3, #0
	bne.n	.L_08122814
	movs	r3, #2
	strh	r3, [r7, #6]
	movs	r3, #254
	adds	r3, #255
	strh	r3, [r7, #8]
	b.n	.L_08122bc6
.L_08122814:
	cmp	r6, #0
	beq.n	.L_08122840
	ldrh	r0, [r1, #0]
	bl	sub_080ad010
	adds	r5, r0, #0
	ldrb	r3, [r5, #12]
	cmp	r3, #1
	beq.n	.L_0812282a
	movs	r6, #0
	b.n	.L_0812283c
.L_0812282a:
	ldrh	r0, [r5, #40]
	bl	sub_080ad078
	ldrh	r5, [r5, #40]
	movs	r2, #0
	movs	r3, #2
	strh	r3, [r7, #6]
	strh	r2, [r7, #8]
	mov	r9, r5
.L_0812283c:
	cmp	r6, #0
	bne.n	.L_08122844
.L_08122840:
	movs	r0, #0
	str	r0, [sp, #20]
.L_08122844:
	ldr	r1, [sp, #12]
	cmp	r1, #0
	bne.n	.L_0812284c
	b.n	.L_08122b9a
.L_0812284c:
	mov	r0, r9
	bl	sub_080ad078
	adds	r5, r0, #0
	ldrb	r3, [r5, #3]
	subs	r3, #46
	cmp	r3, #42
	bhi.n	.L_08122946
	ldr	r2, [pc, #368]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08122910
	.4byte 0x0812291a
	.4byte 0x08122946
	.4byte 0x08122924
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122934
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.4byte 0x08122946
	.2byte 0x2934
	.2byte 0x0812
	movs	r3, #3
	strh	r3, [r7, #6]
	movs	r2, #0
	ldrsh	r0, [r7, r2]
	b.n	.L_0812292c
	movs	r3, #7
	strh	r3, [r7, #6]
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	b.n	.L_0812292c
	movs	r3, #99
	strh	r3, [r7, #6]
	movs	r1, #0
	ldrsh	r0, [r7, r1]
.L_0812292c:
	bl	sub_0811d748
	strh	r0, [r7, #10]
	b.n	.L_08122946
	ldr	r2, [sp, #28]
	cmp	r2, #0
	bne.n	.L_0812295e
	ldrh	r3, [r7, #4]
	movs	r0, #156
	lsls	r0, r0, #6
	adds	r0, #16
	adds	r3, r3, r0
	strh	r3, [r7, #4]
.L_08122946:
	ldr	r1, [sp, #28]
	cmp	r1, #0
	bne.n	.L_0812295e
	movs	r0, #6
	ldrsh	r3, [r7, r0]
	ldrh	r2, [r7, #6]
	cmp	r3, #3
	beq.n	.L_08122960
	cmp	r3, #7
	beq.n	.L_0812295c
	b.n	.L_08122bc6
.L_0812295c:
	b.n	.L_08122960
.L_0812295e:
	ldrh	r2, [r7, #6]
.L_08122960:
	movs	r1, #128
	lsls	r3, r2, #16
	lsls	r1, r1, #10
	cmp	r3, r1
	beq.n	.L_081229d6
	mov	r0, r9
	bl	sub_081224c0
	cmp	r0, #0
	beq.n	.L_081229b8
	movs	r3, #1
	mov	r2, r9
	strh	r3, [r7, #6]
	strh	r2, [r7, #8]
	ldr	r1, [sp, #24]
	ldrb	r2, [r5, #9]
	movs	r0, #58
	ldrsh	r3, [r1, r0]
	cmp	r2, r3
	ble.n	.L_08122998
	mov	r3, sl
	adds	r3, #43
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_08122998
	b.n	.L_08122ba4
.L_08122998:
	ldr	r2, [sp, #24]
	movs	r0, #62
	adds	r0, #255
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_081229d4
	mov	r3, sl
	adds	r3, #43
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #2
	bne.n	.L_081229b6
	b.n	.L_08122ba4
.L_081229b6:
	b.n	.L_081229d4
.L_081229b8:
	ldrh	r2, [r7, #6]
	b.n	.L_081229d6
	.4byte 0x08128960
	.4byte 0x08122728
	.4byte 0x0812cc5c
	.4byte 0x0812cc64
	.4byte 0x0812cc6c
	.2byte 0x2864
	.2byte 0x0812
.L_081229d4:
	ldr	r2, [pc, #44]
.L_081229d6:
	lsls	r3, r2, #16
	asrs	r2, r3, #16
	cmp	r2, #99
	bne.n	.L_081229ee
	ldr	r1, [sp, #24]
	movs	r0, #164
	lsls	r0, r0, #1
	adds	r3, r1, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_081229ee
	b.n	.L_08122ba4
.L_081229ee:
	ldr	r1, [sp, #28]
	cmp	r1, #0
	beq.n	.L_08122a08
	cmp	r2, #3
	bne.n	.L_081229fa
	b.n	.L_08122ba4
.L_081229fa:
	cmp	r2, #7
	bne.n	.L_08122a00
	b.n	.L_08122ba4
.L_08122a00:
	b.n	.L_08122a08
	movs	r0, r0
	.2byte 0x0001
	.2byte 0x0000
.L_08122a08:
	cmp	r2, #4
	bne.n	.L_08122a92
	mov	r2, r9
	mov	r0, r9
	movs	r3, #0
	strh	r2, [r7, #8]
	cmp	r0, #1
	bne.n	.L_08122a1a
	strh	r3, [r7, #6]
.L_08122a1a:
	movs	r1, #237
	lsls	r1, r1, #1
	adds	r1, #255
	cmp	r9, r1
	bne.n	.L_08122a92
	movs	r3, #6
	add	r6, sp, #68
	strh	r3, [r7, #6]
	adds	r1, r6, #0
	movs	r0, #0
	bl	sub_08123574
	add	r2, sp, #32
	mov	r9, r2
	mov	r0, r9
	bl	sub_080ad180
	adds	r5, r0, #0
	subs	r5, #1
	cmp	r5, #0
	bge.n	.L_08122a46
	b.n	.L_08122ba4
.L_08122a46:
	mov	r3, r9
	ldrb	r0, [r3, r5]
	bl	sub_080ad188
	adds	r0, #4
	ldrb	r2, [r0, #0]
	ldrb	r3, [r6, #0]
	movs	r4, #0
	cmp	r2, r3
	bhi.n	.L_08122a6e
	adds	r1, r6, #0
.L_08122a5c:
	adds	r4, #1
	cmp	r4, #3
	bgt.n	.L_08122a6e
	adds	r0, #1
	adds	r1, #1
	ldrb	r2, [r0, #0]
	ldrb	r3, [r1, #0]
	cmp	r2, r3
	bls.n	.L_08122a5c
.L_08122a6e:
	cmp	r4, #4
	beq.n	.L_08122a78
	subs	r5, #1
	cmp	r5, #0
	bge.n	.L_08122a46
.L_08122a78:
	cmp	r5, #0
	bge.n	.L_08122a7e
	b.n	.L_08122ba4
.L_08122a7e:
	mov	r1, r9
	ldrb	r0, [r1, r5]
	strh	r0, [r7, #8]
	bl	sub_080ad188
	ldrh	r0, [r0, #0]
	mov	r9, r0
	bl	sub_080ad078
	adds	r5, r0, #0
.L_08122a92:
	ldrb	r3, [r5, #8]
	strh	r3, [r7, #12]
	ldrb	r3, [r5, #0]
	cmp	r3, #2
	beq.n	.L_08122aae
	cmp	r3, #2
	bgt.n	.L_08122aa6
	cmp	r3, #1
	beq.n	.L_08122af6
	b.n	.L_08122b2c
.L_08122aa6:
	cmp	r3, #3
	beq.n	.L_08122b20
	cmp	r3, #4
	bne.n	.L_08122b2c
.L_08122aae:
	movs	r2, #234
	lsls	r2, r2, #1
	adds	r2, #255
	cmp	r9, r2
	beq.n	.L_08122ac2
	movs	r3, #238
	lsls	r3, r3, #1
	adds	r3, #255
	cmp	r9, r3
	bne.n	.L_08122acc
.L_08122ac2:
	movs	r0, #128
	adds	r1, r5, #0
	bl	sub_0811f66c
	b.n	.L_08122ad6
.L_08122acc:
	movs	r1, #0
	ldrsh	r0, [r7, r1]
	adds	r1, r5, #0
	bl	sub_0811f66c
.L_08122ad6:
	movs	r2, #2
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_08122aec
	ldrh	r3, [r7, #0]
	movs	r0, #0
	cmp	r3, #7
	bhi.n	.L_08122ae8
	movs	r0, #1
.L_08122ae8:
	bl	sub_0811f4d4
.L_08122aec:
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_08122b9a
	b.n	.L_08122b94
.L_08122af6:
	movs	r1, #0
	ldrsh	r0, [r7, r1]
	adds	r1, r5, #0
	bl	sub_0811f66c
	movs	r2, #2
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_08122b16
	ldrh	r3, [r7, #0]
	movs	r0, #0
	cmp	r3, #7
	bhi.n	.L_08122b12
	movs	r0, #1
.L_08122b12:
	bl	sub_0811f4d4
.L_08122b16:
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_08122b9a
	b.n	.L_08122b94
.L_08122b20:
	movs	r1, #0
	ldrsh	r0, [r7, r1]
	bl	sub_0811d748
	strh	r0, [r7, #10]
	b.n	.L_08122b9a
.L_08122b2c:
	ldrb	r2, [r5, #3]
	adds	r3, r2, #0
	cmp	r3, #86
	bne.n	.L_08122b50
	ldr	r2, [sp, #24]
	movs	r0, #165
	lsls	r0, r0, #1
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	adds	r0, #48
	cmp	r3, #164
	beq.n	.L_08122b46
	movs	r0, #81
.L_08122b46:
	bl	sub_0811a490
	cmp	r0, #0
	beq.n	.L_08122b9a
	ldrb	r2, [r5, #3]
.L_08122b50:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #36]
	adds	r3, r2, #0
	cmp	r3, #87
	bne.n	.L_08122b8c
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #107
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_08122b9a
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #106
	adds	r3, r1, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	subs	r2, #7
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r0, [r1, r3]
	bl	sub_0811a490
	cmp	r0, #0
	beq.n	.L_08122b9a
.L_08122b8c:
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	bl	sub_0811d748
.L_08122b94:
	strh	r0, [r7, #10]
	movs	r0, #0
	str	r0, [sp, #12]
.L_08122b9a:
	ldr	r1, [sp, #28]
	cmp	r1, #0
	bne.n	.L_08122ba4
	movs	r2, #0
	str	r2, [sp, #12]
.L_08122ba4:
	ldr	r3, [sp, #12]
	cmp	r3, #0
	beq.n	.L_08122bb8
	ldr	r0, [sp, #16]
	cmp	r0, #16
	ble.n	.L_08122bb8
	movs	r3, #3
	movs	r1, #0
	strh	r3, [r7, #6]
	str	r1, [sp, #12]
.L_08122bb8:
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #12]
	adds	r2, #1
	str	r2, [sp, #16]
	cmp	r3, #0
	beq.n	.L_08122bc6
	b.n	.L_08122714
.L_08122bc6:
	ldr	r0, [sp, #24]
	movs	r1, #165
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldrh	r3, [r3, #0]
	cmp	r3, #218
	bne.n	.L_08122c06
	ldr	r2, [sp, #28]
	cmp	r2, #1
	bne.n	.L_08122c06
	mov	r3, r8
	ldr	r0, [r3, #0]
	lsls	r3, r0, #28
	lsrs	r3, r3, #29
	cmp	r3, #0
	bne.n	.L_08122c06
	lsls	r0, r0, #26
	lsrs	r0, r0, #30
	movs	r1, #3
	adds	r0, #1
	bl	sub_08002064
	mov	r1, r8
	movs	r3, #3
	ldrb	r2, [r1, #0]
	ands	r0, r3
	movs	r3, #49
	negs	r3, r3
	lsls	r0, r0, #4
	ands	r3, r2
	orrs	r3, r0
	strb	r3, [r1, #0]
.L_08122c06:
	movs	r0, #0
.L_08122c08:
	add	sp, #72
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
