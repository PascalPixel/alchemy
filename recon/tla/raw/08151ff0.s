.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014de4, 0x08014de4
	.set sub_080156e8, 0x080156e8
	.set sub_08020030, 0x08020030
	.set sub_08020040, 0x08020040
	.set sub_08118088, 0x08118088
	.set sub_081180e8, 0x081180e8
	.set sub_08138058, 0x08138058
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_081963ec, 0x081963ec
	.set sub_081c0010, 0x081c0010
	.global Func_08151ff0
	.thumb_func
Func_08151ff0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #56
	str	r1, [sp, #40]
	movs	r3, #192
	lsls	r3, r3, #18
	mov	sl, r0
	ldr	r0, [r3, #92]
	ldr	r3, [r3, #96]
	mov	fp, r0
	movs	r0, #1
	str	r3, [sp, #36]
	bl	sub_081435e0
	ldr	r3, [pc, #40]
	movs	r2, #128
	lsls	r2, r2, #19
	movs	r1, #224
	adds	r2, #82
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	add	r1, fp
	ldr	r0, [pc, #28]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r1, [sp, #40]
	cmp	r1, #1
	bne.n	.L_0815206a
	movs	r2, #0
	movs	r0, #160
	mov	r8, r2
	lsls	r0, r0, #19
	b.n	.L_08152048
	.4byte 0x00001010
	.2byte 0x0184
	.2byte 0x0000
.L_08152048:
	mov	r4, r8
	lsrs	r3, r4, #31
	add	r3, r8
	asrs	r3, r3, #1
	lsls	r1, r3, #5
	lsls	r2, r3, #10
	orrs	r2, r1
	movs	r1, #1
	orrs	r2, r3
	add	r8, r1
	strh	r2, [r0, #0]
	mov	r2, r8
	adds	r0, #2
	cmp	r2, #64
	bne.n	.L_08152048
	str	r1, [sp, #24]
	b.n	.L_08152084
.L_0815206a:
	ldr	r0, [pc, #724]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #716]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4653
	ldr	r3, [r3, #24]
	str	r3, [sp, #24]
.L_08152084:
	movs	r4, #0
	mov	r8, r4
	movs	r7, #0
	movs	r6, #63
	mov	r5, fp
.L_0815208e:
	mov	r0, sl
	ldr	r3, [r0, #4]
	cmp	r3, #1
	bne.n	.L_0815209c
	movs	r3, #200
	lsls	r3, r3, #14
	b.n	.L_0815209e
.L_0815209c:
	ldr	r3, [pc, #680]
.L_0815209e:
	str	r3, [r5, #0]
	str	r7, [r5, #4]
	str	r7, [r5, #8]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #32
	lsls	r0, r0, #13
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #16
	lsls	r0, r0, #12
	str	r0, [r5, #16]
	bl	sub_08014878
	movs	r1, #1
	ands	r0, r6
	subs	r0, #32
	add	r8, r1
	lsls	r0, r0, #13
	mov	r2, r8
	str	r0, [r5, #20]
	str	r7, [r5, #24]
	adds	r5, #28
	cmp	r2, #32
	bne.n	.L_0815208e
	ldr	r5, [pc, #628]
	movs	r3, #0
	mov	r8, r3
	movs	r6, #0
	movs	r7, #63
.L_081520e0:
	mov	r4, sl
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne.n	.L_081520ee
	movs	r3, #200
	lsls	r3, r3, #14
	b.n	.L_081520f0
.L_081520ee:
	ldr	r3, [pc, #600]
.L_081520f0:
	str	r3, [r5, #0]
	str	r6, [r5, #4]
	str	r6, [r5, #8]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #32
	lsls	r0, r0, #13
	str	r0, [r5, #12]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r3, #8
	lsls	r3, r3, #13
	str	r3, [r5, #16]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #32
	lsls	r0, r0, #13
	str	r0, [r5, #20]
	movs	r1, #128
	movs	r0, #1
	add	r8, r0
	lsls	r1, r1, #3
	str	r6, [r5, #24]
	adds	r5, #28
	cmp	r8, r1
	bne.n	.L_081520e0
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r2, #239
	lsls	r2, r2, #7
	str	r3, [sp, #28]
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
	ldr	r0, [pc, #504]
	bl	sub_080145a8
	ldr	r3, [sp, #24]
	ldr	r4, [sp, #24]
	lsls	r3, r3, #1
	movs	r2, #0
	ldr	r0, [pc, #492]
	str	r3, [sp, #12]
	mov	r9, r2
	adds	r2, r3, r4
	adds	r1, r2, #2
	ldrb	r3, [r0, r1]
	cmp	r3, #0
	bne.n	.L_08152176
	b.n	.L_081523de
.L_08152176:
	adds	r0, r2, #0
	adds	r0, #1
	str	r2, [sp, #16]
	str	r0, [sp, #8]
	str	r1, [sp, #20]
.L_08152180:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #48]
	bl	sub_08014de4
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	sub_080156e8
	mov	r1, r9
	cmp	r1, #2
	bne.n	.L_081521a0
	movs	r0, #144
	bl	sub_081c0010
.L_081521a0:
	ldr	r3, [sp, #24]
	ldr	r2, [sp, #12]
	ldr	r6, [pc, #428]
	adds	r5, r2, r3
	adds	r3, r5, #2
	ldrb	r3, [r6, r3]
	subs	r3, #48
	cmp	r9, r3
	bne.n	.L_081521b8
	movs	r0, #133
	bl	sub_081180e8
.L_081521b8:
	ldrb	r3, [r6, r5]
	movs	r4, #0
	mov	r8, r4
	cmp	r3, #0
	beq.n	.L_08152264
	ldr	r6, [pc, #392]
.L_081521c4:
	ldr	r3, [r6, #4]
	cmp	r3, #0
	blt.n	.L_08152254
	add	r5, sp, #44
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0815e1ec
	mov	r0, sl
	ldr	r2, [r5, #0]
	ldr	r3, [r0, #4]
	asrs	r2, r2, #1
	lsls	r3, r3, #5
	adds	r2, r2, r3
	subs	r2, #16
	str	r2, [r5, #0]
	ldr	r2, [r5, #8]
	cmp	r2, #159
	bgt.n	.L_081521f0
	movs	r3, #160
	str	r3, [r5, #8]
	movs	r2, #160
.L_081521f0:
	movs	r3, #136
	lsls	r3, r3, #2
	adds	r3, #255
	cmp	r2, r3
	ble.n	.L_081521fe
	str	r3, [r5, #8]
	adds	r2, r3, #0
.L_081521fe:
	adds	r3, r2, #0
	subs	r3, #160
	cmp	r3, #0
	bge.n	.L_08152208
	adds	r3, #63
.L_08152208:
	asrs	r3, r3, #6
	movs	r0, #9
	subs	r0, r0, r3
	ldr	r2, [pc, #328]
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	mov	r3, r8
	movs	r2, #1
	ands	r2, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #7
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r1, r1, r3
	movs	r2, #228
	lsls	r2, r2, #6
	add	r1, fp
	adds	r1, r1, r2
	lsrs	r3, r0, #31
	ldr	r2, [r5, #0]
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #36]
	ldr	r4, [sp, #28]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c30
	movs	r1, #64
	ldr	r2, [pc, #268]
	bl	sub_08138058
.L_08152254:
	ldr	r1, [pc, #252]
	ldr	r2, [sp, #16]
	movs	r0, #1
	ldrb	r3, [r1, r2]
	add	r8, r0
	adds	r6, #28
	cmp	r8, r3
	bne.n	.L_081521c4
.L_08152264:
	mov	r3, r9
	cmp	r3, #2
	ble.n	.L_081522f6
	ldr	r2, [pc, #232]
	ldr	r0, [sp, #8]
	movs	r4, #0
	ldrb	r3, [r2, r0]
	mov	r8, r4
	cmp	r3, #0
	beq.n	.L_081522f6
	mov	r5, fp
.L_0815227a:
	cmp	r8, r9
	bge.n	.L_081522e8
	ldr	r3, [r5, #4]
	cmp	r3, #0
	blt.n	.L_081522e8
	add	r6, sp, #44
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	sub_0815e1ec
	mov	r1, sl
	ldr	r2, [r6, #0]
	ldr	r3, [r1, #4]
	asrs	r2, r2, #1
	lsls	r3, r3, #5
	adds	r2, r2, r3
	adds	r7, r2, #0
	subs	r7, #16
	str	r7, [r6, #0]
	ldr	r0, [r5, #24]
	cmp	r0, #20
	bhi.n	.L_081522d4
	movs	r1, #3
	bl	sub_08002054
	ldr	r3, [pc, #176]
	lsls	r0, r0, #1
	ldrh	r1, [r3, r0]
	ldr	r3, [pc, #176]
	movs	r2, #224
	ldrh	r4, [r3, r0]
	ldr	r3, [r6, #4]
	lsrs	r0, r4, #1
	lsls	r2, r2, #3
	add	r1, fp
	adds	r1, r1, r2
	subs	r3, r3, r0
	subs	r2, r7, r0
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #36]
	ldr	r4, [sp, #28]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69a8
.L_081522d4:
	cmp	r0, #20
	bgt.n	.L_081522dc
	adds	r3, r0, #1
	str	r3, [r5, #24]
.L_081522dc:
	ldr	r2, [pc, #124]
	adds	r0, r5, #0
	movs	r1, #64
	bl	sub_08138058
	ldr	r2, [pc, #108]
.L_081522e8:
	ldr	r1, [sp, #8]
	movs	r0, #1
	ldrb	r3, [r2, r1]
	add	r8, r0
	adds	r5, #28
	cmp	r8, r3
	bne.n	.L_0815227a
.L_081522f6:
	ldr	r2, [sp, #40]
	cmp	r2, #0
	bne.n	.L_08152368
	mov	r4, sl
	ldr	r2, [r4, #20]
	movs	r3, #0
	mov	r8, r3
	cmp	r2, #0
	beq.n	.L_0815239e
	movs	r5, #36
.L_0815230a:
	mov	r3, r8
	adds	r3, #6
	cmp	r9, r3
	bne.n	.L_08152332
	mov	r1, sl
	movs	r3, #10
	ldrsh	r0, [r5, r1]
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r1, #7
	mov	r3, r8
	bl	sub_0814cd48
	mov	r3, sl
	ldrsh	r0, [r5, r3]
	movs	r1, #2
	bl	sub_08118088
	mov	r0, sl
	ldr	r2, [r0, #20]
.L_08152332:
	movs	r1, #1
	add	r8, r1
	adds	r5, #2
	cmp	r8, r2
	bne.n	.L_0815230a
	b.n	.L_0815239e
	movs	r0, r0
	.4byte 0x00000159
	.4byte 0x03000730
	.4byte 0xffce0000
	.4byte 0x02010000
	.4byte 0x08143001
	.4byte 0x081983a2
	.4byte 0x08197410
	.4byte 0xffffe000
	.4byte 0x081983ac
	.2byte 0x83ba
	.2byte 0x0819
.L_08152368:
	movs	r2, #0
	mov	r3, sl
	mov	r8, r2
	ldr	r2, [r3, #20]
	cmp	r2, #0
	beq.n	.L_0815239e
	movs	r5, #36
.L_08152376:
	mov	r3, r8
	adds	r3, #6
	cmp	r9, r3
	bne.n	.L_08152394
	mov	r4, sl
	movs	r3, #10
	ldrsh	r0, [r5, r4]
	str	r3, [sp, #0]
	movs	r2, #5
	mov	r3, r8
	movs	r1, #7
	bl	sub_0814cd48
	mov	r3, sl
	ldr	r2, [r3, #20]
.L_08152394:
	movs	r4, #1
	add	r8, r4
	adds	r5, #2
	cmp	r8, r2
	bne.n	.L_08152376
.L_0815239e:
	mov	r0, r9
	cmp	r0, #2
	bne.n	.L_081523b0
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, fp
	movs	r3, #6
	str	r3, [r2, #0]
.L_081523b0:
	movs	r1, #16
	movs	r0, #16
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #232
	add	r3, fp
	movs	r2, #1
	str	r2, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [pc, #44]
	ldr	r4, [sp, #20]
	movs	r1, #1
	ldrb	r3, [r2, r4]
	add	r9, r1
	cmp	r9, r3
	beq.n	.L_081523de
	b.n	.L_08152180
.L_081523de:
	ldr	r0, [pc, #32]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x081983a2
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
	mov	r8, r0
	mov	r9, r1
	ldr	r7, [r3, #92]
	movs	r6, #0
	cmp	r0, #0
	beq.n	.L_08152462
	movs	r3, #3
	ands	r2, r3
	movs	r5, #238
	movs	r1, #13
	lsls	r2, r2, #2
	lsls	r5, r5, #7
	negs	r1, r1
	mov	sl, r2
	adds	r5, #220
	mov	fp, r1
.L_08152436:
	mov	r0, r9
	bl	sub_08020040
	str	r0, [r5, r7]
	cmp	r0, #0
	beq.n	.L_0815245a
	movs	r3, #0
	strb	r3, [r0, #26]
	adds	r1, r6, #0
	bl	sub_08020030
	ldr	r2, [r5, r7]
	mov	r1, fp
	ldrb	r3, [r2, #9]
	ands	r3, r1
	mov	r1, sl
	orrs	r3, r1
	strb	r3, [r2, #9]
.L_0815245a:
	adds	r6, #1
	adds	r5, #4
	cmp	r6, r8
	bne.n	.L_08152436
.L_08152462:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
