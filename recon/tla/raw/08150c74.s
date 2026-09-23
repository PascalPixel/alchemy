.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0800206c, 0x0800206c
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014de4, 0x08014de4
	.set sub_08015128, 0x08015128
	.set sub_080156e8, 0x080156e8
	.set sub_08118028, 0x08118028
	.set sub_08118070, 0x08118070
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138086, 0x08138086
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143a88, 0x08143a88
	.set sub_08143b20, 0x08143b20
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_0814cd48, 0x0814cd48
	.set sub_08151734, 0x08151734
	.set sub_08151d18, 0x08151d18
	.set sub_08151f2e, 0x08151f2e
	.set sub_08151ff0, 0x08151ff0
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e22c, 0x0815e22c
	.set sub_081963ec, 0x081963ec
	.set sub_081c0010, 0x081c0010
	.global Func_08150c74
	.thumb_func
Func_08150c74:
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
	ldr	r0, [r3, #92]
	sub	sp, #112
	str	r0, [sp, #68]
	movs	r0, #1
	ldr	r1, [r3, #96]
	ldr	r5, [pc, #80]
	str	r1, [sp, #64]
	movs	r6, #0
	ldr	r3, [r3, #48]
	movs	r7, #0
	str	r3, [sp, #48]
	bl	sub_081435e0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #48
	strh	r3, [r2, #0]
	ldr	r2, [sp, #68]
	movs	r3, #148
	lsls	r3, r3, #6
	adds	r1, r2, r3
	ldr	r0, [pc, #44]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r2, #128
	ldr	r3, [pc, #36]
	ldr	r0, [pc, #24]
	ldr	r1, [pc, #36]
	lsls	r2, r2, #8
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2407
	mov	ip, r4
	mov	lr, r5
	b.n	.L_08150cf4
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x02010000
	.4byte 0x0000014e
	.4byte 0x03000730
	.2byte 0x8000
	.2byte 0x0600
.L_08150cf4:
	adds	r4, r6, #0
	adds	r4, #96
	mov	r1, ip
	adds	r3, r4, #0
	ldr	r2, [sp, #68]
	ands	r3, r1
	lsls	r3, r3, #3
	movs	r5, #224
	mov	sl, r3
	lsls	r5, r5, #3
	adds	r3, r7, r2
	movs	r0, #0
	adds	r1, r3, r5
.L_08150d0e:
	adds	r3, r0, #0
	adds	r3, #32
	adds	r2, r3, #0
	mov	r5, ip
	ands	r2, r5
	cmp	r3, #0
	bge.n	.L_08150d1e
	adds	r3, #7
.L_08150d1e:
	asrs	r3, r3, #3
	lsls	r3, r3, #6
	adds	r3, r2, r3
	mov	r5, sl
	adds	r2, r3, r5
	adds	r3, r4, #0
	cmp	r3, #0
	bge.n	.L_08150d30
	adds	r3, #7
.L_08150d30:
	asrs	r3, r3, #3
	lsls	r3, r3, #11
	adds	r3, r2, r3
	mov	r2, lr
	ldrb	r3, [r3, r2]
	adds	r0, #1
	strb	r3, [r1, #0]
	adds	r1, #1
	cmp	r0, #40
	bne.n	.L_08150d0e
	adds	r6, #1
	adds	r7, #40
	cmp	r6, #16
	bne.n	.L_08150cf4
	mov	r4, r8
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne.n	.L_08150d66
	movs	r2, #128
	ldr	r3, [pc, #728]
	lsls	r2, r2, #19
	movs	r5, #112
	adds	r2, #40
	negs	r5, r5
	str	r3, [r2, #0]
	str	r5, [sp, #44]
	b.n	.L_08150d6a
.L_08150d66:
	movs	r0, #0
	str	r0, [sp, #44]
.L_08150d6a:
	movs	r1, #35
	movs	r0, #104
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	mov	r1, r8
	str	r3, [sp, #52]
	movs	r7, #0
	ldr	r3, [r1, #20]
	cmp	r3, #0
	beq.n	.L_08150d98
	ldr	r2, [sp, #68]
	movs	r1, #0
	adds	r2, #24
.L_08150d8a:
	str	r1, [r2, #0]
	mov	r4, r8
	ldr	r3, [r4, #20]
	adds	r7, #1
	adds	r2, #28
	cmp	r7, r3
	bne.n	.L_08150d8a
.L_08150d98:
	ldr	r5, [sp, #68]
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	adds	r2, r5, r0
	movs	r3, #1
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r2, r5, r1
	movs	r3, #0
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #636]
	bl	sub_080145a8
	mov	r2, sp
	adds	r2, #96
	str	r2, [sp, #40]
	ldr	r3, [pc, #628]
	ldmia	r3!, {r0, r4, r5}
	stmia	r2!, {r0, r4, r5}
	ldr	r3, [r3, #0]
	movs	r0, #141
	str	r3, [r2, #0]
	ldr	r1, [sp, #68]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	adds	r2, r1, r3
	movs	r3, #128
	str	r3, [r2, #0]
	bl	sub_081c0010
	movs	r4, #0
	str	r4, [sp, #60]
	mov	r5, r8
	ldr	r2, [r5, #24]
	ldr	r1, [pc, #596]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldrb	r3, [r1, r3]
	cmp	r3, #0
	bne.n	.L_08150df4
	b.n	.L_08151010
.L_08150df4:
	ldr	r0, [sp, #48]
	adds	r0, #12
	str	r0, [sp, #28]
.L_08150dfa:
	bl	sub_08014de4
	ldr	r1, [sp, #28]
	ldr	r0, [sp, #48]
	bl	sub_080156e8
	mov	r1, r8
	ldr	r3, [r1, #24]
	ldr	r4, [pc, #560]
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldrb	r3, [r4, r2]
	ldr	r5, [sp, #60]
	subs	r3, #16
	cmp	r5, r3
	bne.n	.L_08150e20
	movs	r0, #133
	bl	sub_081180e8
.L_08150e20:
	movs	r0, #0
	movs	r1, #8
	movs	r2, #16
	str	r0, [sp, #56]
	str	r0, [sp, #20]
	str	r1, [sp, #24]
	str	r2, [sp, #16]
	str	r0, [sp, #12]
.L_08150e30:
	ldr	r4, [sp, #60]
	movs	r3, #31
	ldr	r5, [sp, #16]
	ands	r3, r4
	adds	r3, #32
	cmp	r3, r5
	bne.n	.L_08150e48
	ldr	r0, [sp, #40]
	ldr	r1, [sp, #12]
	ldr	r3, [r0, r1]
	adds	r3, #32
	str	r3, [r0, r1]
.L_08150e48:
	ldr	r2, [sp, #60]
	ldr	r3, [sp, #16]
	cmp	r2, r3
	bge.n	.L_08150e52
	b.n	.L_08150f5a
.L_08150e52:
	mov	r4, r8
	ldr	r3, [r4, #24]
	ldr	r5, [pc, #484]
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r2, #1
	ldrb	r3, [r5, r2]
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #60]
	adds	r3, r0, r3
	cmp	r1, r3
	bge.n	.L_08150f5a
	subs	r0, r1, r0
	lsls	r0, r0, #10
	bl	sub_08002096
	ldr	r2, [sp, #12]
	ldr	r4, [sp, #40]
	ldr	r3, [r2, r4]
	muls	r3, r0
	asrs	r5, r3, #16
	cmp	r5, #0
	bge.n	.L_08150e82
	negs	r5, r5
.L_08150e82:
	movs	r3, #112
	ldr	r2, [sp, #68]
	subs	r3, r3, r5
	mov	fp, r3
	movs	r3, #148
	movs	r0, #40
	lsls	r3, r3, #6
	adds	r1, r2, r3
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	ldr	r2, [sp, #24]
	ldr	r4, [sp, #52]
	ldr	r0, [sp, #64]
	mov	r3, fp
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2210
	str	r2, [sp, #4]
	ldr	r2, [sp, #68]
	movs	r3, #96
	movs	r4, #224
	subs	r3, r3, r5
	lsls	r4, r4, #3
	movs	r5, #40
	str	r5, [sp, #0]
	adds	r1, r2, r4
	ldr	r0, [sp, #64]
	ldr	r2, [sp, #24]
	ldr	r5, [sp, #52]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4640
	ldr	r3, [r0, #20]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_08150f5a
	ldr	r3, [sp, #24]
	ldr	r2, [sp, #20]
	movs	r1, #72
	adds	r3, #40
	add	r1, sp
	str	r2, [sp, #36]
	str	r3, [sp, #32]
	mov	sl, r1
	add	r6, sp, #84
	mov	r9, sl
	movs	r4, #36
.L_08150ee0:
	mov	r5, r8
	ldrsh	r0, [r4, r5]
	str	r4, [sp, #8]
	bl	sub_08118098
	ldr	r5, [r0, #0]
	mov	r1, r9
	ldr	r3, [r5, #8]
	adds	r0, r6, #0
	str	r3, [r6, #0]
	ldr	r3, [r5, #12]
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	str	r3, [r6, #8]
	bl	sub_0815e1ec
	mov	r2, r9
	ldr	r3, [r2, #0]
	ldr	r0, [sp, #44]
	mov	r1, r9
	adds	r2, r3, r0
	str	r2, [r1, #0]
	ldr	r3, [sp, #36]
	ldr	r4, [sp, #8]
	adds	r3, #8
	cmp	r2, r3
	blt.n	.L_08150f32
	ldr	r3, [sp, #32]
	cmp	r2, r3
	bgt.n	.L_08150f32
	mov	r0, sl
	ldr	r3, [r0, #4]
	cmp	r3, fp
	blt.n	.L_08150f32
	movs	r3, #192
	lsls	r3, r3, #12
	str	r3, [r5, #40]
	movs	r3, #171
	lsls	r3, r3, #8
	adds	r3, #133
	str	r3, [r5, #72]
.L_08150f32:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	bge.n	.L_08150f4e
	mov	r1, r8
	movs	r3, #0
	ldrsh	r0, [r4, r1]
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #5
	subs	r3, #1
	str	r4, [sp, #8]
	bl	sub_0814cd48
	ldr	r4, [sp, #8]
.L_08150f4e:
	mov	r5, r8
	ldr	r3, [r5, #20]
	adds	r7, #1
	adds	r4, #2
	cmp	r7, r3
	bne.n	.L_08150ee0
.L_08150f5a:
	ldr	r0, [sp, #20]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #12]
	ldr	r4, [sp, #56]
	adds	r0, #40
	adds	r1, #40
	adds	r2, #4
	adds	r3, #4
	adds	r4, #1
	str	r0, [sp, #20]
	str	r1, [sp, #24]
	str	r2, [sp, #16]
	str	r3, [sp, #12]
	str	r4, [sp, #56]
	cmp	r4, #3
	beq.n	.L_08150f7e
	b.n	.L_08150e30
.L_08150f7e:
	mov	r5, r8
	ldr	r3, [r5, #20]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_08150fcc
	ldr	r5, [sp, #68]
	movs	r6, #36
.L_08150f8c:
	mov	r1, r8
	ldrsh	r0, [r6, r1]
	bl	sub_08118098
	ldr	r3, [r5, #24]
	ldr	r0, [r0, #0]
	cmp	r3, #0
	bne.n	.L_08150fbe
	ldr	r3, [r0, #12]
	cmp	r3, #0
	bgt.n	.L_08150fbe
	ldr	r3, [r0, #40]
	cmp	r3, #0
	bge.n	.L_08150fbe
	movs	r3, #1
	str	r3, [r5, #24]
	mov	r3, r8
	ldrsh	r0, [r6, r3]
	movs	r3, #5
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	adds	r3, r7, #0
	bl	sub_0814cd48
.L_08150fbe:
	mov	r0, r8
	ldr	r3, [r0, #20]
	adds	r7, #1
	adds	r5, #28
	adds	r6, #2
	cmp	r7, r3
	bne.n	.L_08150f8c
.L_08150fcc:
	mov	r1, r8
	ldr	r2, [r1, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [pc, #100]
	adds	r3, #2
	ldrb	r1, [r2, r3]
	adds	r0, r1, #0
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r4, #240
	ldr	r3, [sp, #68]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r5, [sp, #60]
	mov	r0, r8
	adds	r5, #1
	str	r5, [sp, #60]
	ldr	r1, [pc, #56]
	ldr	r2, [r0, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldrb	r3, [r1, r3]
	cmp	r5, r3
	beq.n	.L_08151010
	b.n	.L_08150dfa
.L_08151010:
	ldr	r0, [pc, #32]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #112
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xffff9000
	.4byte 0x08143001
	.4byte 0x08196e1c
	.2byte 0x82f7
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
	sub	sp, #56
	mov	sl, r0
	ldr	r0, [r3, #92]
	str	r1, [sp, #32]
	mov	r4, sl
	ldr	r2, [r3, #48]
	mov	fp, r0
	str	r2, [sp, #24]
	ldr	r3, [r3, #100]
	str	r3, [sp, #20]
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne.n	.L_08151076
	movs	r0, #1
	bl	sub_081435e0
	b.n	.L_0815107c
.L_08151076:
	movs	r0, #0
	bl	sub_081435e0
.L_0815107c:
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #108]
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r2, #0
	ldr	r1, [sp, #20]
	movs	r3, #0
	ldr	r0, [pc, #96]
	bl	sub_08157cf4
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #104]
	movs	r1, #19
	movs	r0, #188
	str	r3, [sp, #36]
	bl	sub_081963ec
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r5, sp
	adds	r5, #36
	str	r5, [sp, #12]
	movs	r2, #128
	str	r3, [r5, #4]
	ldr	r3, [pc, #44]
	lsls	r2, r2, #19
	adds	r2, #82
	mov	r1, sl
	strh	r3, [r2, #0]
	ldr	r0, [r1, #8]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	mov	r2, sl
	mov	r8, r0
	ldr	r0, [r2, #8]
	bl	sub_08118070
	mov	r4, r8
	ldr	r3, [r4, #12]
	ldr	r7, [pc, #24]
	adds	r3, r3, r0
	str	r3, [sp, #16]
	movs	r5, #0
	mov	r9, r5
	b.n	.L_081510fc
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x00000178
	.4byte 0x00000134
	.2byte 0x0000
	.2byte 0x0201
.L_081510fc:
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08014878
	adds	r5, r0, #0
	movs	r0, #127
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #127
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	bl	sub_08014878
	movs	r1, #127
	ands	r0, r1
	subs	r0, #16
	lsls	r0, r0, #16
	asrs	r0, r0, #6
	str	r0, [r7, #16]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #20]
	mov	r2, r8
	ldr	r3, [r2, #8]
	movs	r4, #1
	str	r3, [r7, #0]
	ldr	r3, [sp, #16]
	add	r9, r4
	str	r3, [r7, #4]
	mov	r5, r9
	ldr	r3, [r2, #16]
	str	r3, [r7, #8]
	movs	r3, #1
	negs	r3, r3
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r5, #64
	bne.n	.L_081510fc
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #172
	movs	r2, #0
	add	r3, fp
	str	r2, [r3, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #176
	movs	r5, #200
	add	r3, fp
	lsls	r5, r5, #4
	str	r2, [r3, #0]
	adds	r1, r5, #0
	ldr	r0, [pc, #800]
	bl	sub_080145a8
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
	str	r3, [r2, #0]
	adds	r1, r5, #0
	ldr	r0, [pc, #772]
	bl	sub_080145a8
	movs	r0, #0
	str	r0, [sp, #28]
	mov	r1, sl
	ldr	r2, [pc, #764]
	ldr	r3, [r1, #24]
	ldrb	r3, [r2, r3]
	movs	r2, #132
	lsrs	r3, r3, #1
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_081511b2
	b.n	.L_0815146e
.L_081511b2:
	ldr	r3, [sp, #24]
	adds	r3, #12
	str	r3, [sp, #8]
.L_081511b8:
	ldr	r3, [sp, #28]
	subs	r3, #17
	cmp	r3, #62
	bhi.n	.L_081511ce
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #172
	movs	r3, #128
	add	r2, fp
	lsls	r3, r3, #1
	b.n	.L_081511d8
.L_081511ce:
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #172
	add	r2, fp
	movs	r3, #0
.L_081511d8:
	str	r3, [r2, #0]
	mov	r4, sl
	ldr	r5, [pc, #704]
	ldr	r3, [r4, #24]
	ldr	r0, [sp, #28]
	ldrb	r3, [r5, r3]
	lsrs	r3, r3, #1
	adds	r3, #108
	cmp	r0, r3
	bne.n	.L_081511f2
	movs	r0, #133
	bl	sub_081180e8
.L_081511f2:
	movs	r2, #0
	movs	r3, #100
	movs	r0, #0
	movs	r1, #0
	bl	sub_08118028
	bl	sub_08014de4
	ldr	r1, [sp, #8]
	ldr	r0, [sp, #24]
	bl	sub_080156e8
	mov	r3, sl
	ldr	r2, [r3, #24]
	movs	r1, #0
	ldrb	r3, [r5, r2]
	mov	r9, r1
	adds	r1, r5, #0
	cmp	r3, #0
	bne.n	.L_0815121c
	b.n	.L_081513c2
.L_0815121c:
	ldr	r6, [pc, #644]
.L_0815121e:
	mov	r4, r9
	lsrs	r3, r4, #31
	add	r3, r9
	asrs	r3, r3, #1
	ldr	r5, [sp, #28]
	mov	r8, r3
	mov	r7, r8
	adds	r7, #48
	cmp	r5, r8
	ble.n	.L_081512ce
	ldr	r3, [r6, #24]
	movs	r0, #1
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_081512ca
	add	r5, sp, #44
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0815e1ec
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	ldr	r3, [r5, #8]
	cmp	r3, #159
	bgt.n	.L_08151256
	movs	r3, #160
	str	r3, [r5, #8]
.L_08151256:
	movs	r2, #136
	lsls	r2, r2, #2
	adds	r2, #255
	cmp	r3, r2
	ble.n	.L_08151264
	str	r2, [r5, #8]
	adds	r3, r2, #0
.L_08151264:
	adds	r2, r3, #0
	subs	r2, #160
	cmp	r2, #0
	bge.n	.L_0815126e
	adds	r2, #63
.L_0815126e:
	asrs	r2, r2, #6
	movs	r3, #10
	subs	r4, r3, r2
	ldr	r2, [sp, #28]
	mov	r7, r8
	movs	r1, #4
	adds	r7, #48
	mov	ip, r1
	cmp	r2, r7
	blt.n	.L_08151286
	movs	r3, #0
	mov	ip, r3
.L_08151286:
	ldr	r2, [pc, #544]
	lsls	r0, r4, #1
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #20]
	lsrs	r3, r4, #31
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	adds	r3, r4, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #4]
	str	r4, [sp, #0]
	ldr	r0, [sp, #12]
	mov	r5, ip
	subs	r3, r3, r4
	ldr	r4, [r5, r0]
	ldr	r0, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x6833
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
	b.n	.L_081512ce
.L_081512ca:
	mov	r7, r8
	adds	r7, #48
.L_081512ce:
	ldr	r1, [sp, #28]
	cmp	r1, r7
	ble.n	.L_081513ae
	ldr	r3, [r6, #24]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_081513ae
	mov	r3, sl
	ldr	r1, [r3, #20]
	mov	r0, r9
	bl	sub_08002064
	lsls	r0, r0, #1
	mov	r4, sl
	adds	r0, #36
	ldrsh	r0, [r4, r0]
	bl	sub_08118098
	ldr	r1, [r0, #0]
	ldr	r2, [r6, #0]
	ldr	r3, [r1, #8]
	subs	r3, r3, r2
	ldr	r2, [r6, #12]
	asrs	r3, r3, #9
	adds	r0, r2, r3
	str	r0, [r6, #12]
	ldr	r2, [r6, #4]
	ldr	r3, [r1, #12]
	subs	r3, r3, r2
	ldr	r2, [r6, #16]
	asrs	r3, r3, #9
	adds	r4, r2, r3
	str	r4, [r6, #16]
	ldr	r2, [r6, #8]
	ldr	r3, [r1, #16]
	subs	r3, r3, r2
	ldr	r2, [r6, #20]
	asrs	r3, r3, #9
	adds	r1, r2, r3
	str	r1, [r6, #20]
	ldr	r2, [sp, #28]
	mov	r3, r8
	adds	r3, #85
	cmp	r2, r3
	bge.n	.L_0815135a
	lsls	r3, r0, #4
	subs	r3, r3, r0
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L_08151336
	adds	r2, #63
.L_08151336:
	asrs	r3, r2, #6
	str	r3, [r6, #12]
	lsls	r3, r4, #4
	subs	r3, r3, r4
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L_08151346
	adds	r2, #63
.L_08151346:
	asrs	r3, r2, #6
	str	r3, [r6, #16]
	lsls	r3, r1, #4
	subs	r3, r3, r1
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L_08151356
	adds	r2, #63
.L_08151356:
	asrs	r3, r2, #6
	str	r3, [r6, #20]
.L_0815135a:
	ldr	r3, [r6, #4]
	cmp	r3, #0
	bge.n	.L_081513ae
	movs	r3, #0
	str	r3, [r6, #24]
	add	r2, sp, #44
	ldr	r3, [r2, #0]
	mov	r4, sl
	str	r3, [r6, #0]
	ldr	r3, [r2, #4]
	str	r3, [r6, #4]
	ldr	r3, [r4, #24]
	cmp	r3, #2
	bne.n	.L_0815137e
	movs	r0, #134
	bl	sub_081c0010
	b.n	.L_08151384
.L_0815137e:
	movs	r0, #136
	bl	sub_081c0010
.L_08151384:
	mov	r5, sl
	ldr	r1, [r5, #20]
	mov	r0, r9
	bl	sub_08002064
	adds	r3, r0, #0
	lsls	r2, r3, #1
	adds	r2, #36
	ldrsh	r0, [r5, r2]
	movs	r2, #4
	str	r2, [sp, #0]
	movs	r1, #10
	movs	r2, #5
	bl	sub_0814cd48
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
.L_081513ae:
	movs	r2, #1
	mov	r3, sl
	add	r9, r2
	ldr	r1, [pc, #232]
	ldr	r2, [r3, #24]
	adds	r6, #28
	ldrb	r3, [r1, r2]
	cmp	r9, r3
	beq.n	.L_081513c2
	b.n	.L_0815121e
.L_081513c2:
	ldrb	r3, [r1, r2]
	movs	r4, #0
	mov	r9, r4
	cmp	r3, #0
	beq.n	.L_08151426
	ldr	r6, [pc, #212]
.L_081513ce:
	ldr	r3, [r6, #24]
	cmp	r3, #11
	bhi.n	.L_0815141a
	lsrs	r4, r3, #31
	ldr	r2, [pc, #212]
	adds	r4, r3, r4
	asrs	r4, r4, #1
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #204]
	movs	r5, #224
	lsls	r5, r5, #3
	add	r1, fp
	adds	r1, r1, r5
	ldrb	r5, [r3, r4]
	ldr	r2, [r6, #0]
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #192]
	ldrb	r0, [r3, r4]
	ldr	r3, [r6, #4]
	str	r5, [sp, #0]
	adds	r3, r3, r0
	ldr	r0, [pc, #184]
	subs	r3, #56
	ldrb	r0, [r0, r4]
	str	r0, [sp, #4]
	ldr	r0, [sp, #12]
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69b3
	ldr	r1, [pc, #140]
	adds	r3, #1
	str	r3, [r6, #24]
	mov	r3, sl
	ldr	r2, [r3, #24]
.L_0815141a:
	ldrb	r3, [r1, r2]
	movs	r4, #1
	add	r9, r4
	adds	r6, #28
	cmp	r9, r3
	bne.n	.L_081513ce
.L_08151426:
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #176
	add	r2, fp
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_08151438
	movs	r3, #1
	str	r3, [r2, #0]
.L_08151438:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r5, [sp, #28]
	mov	r0, sl
	adds	r5, #1
	str	r5, [sp, #28]
	ldr	r2, [pc, #64]
	ldr	r3, [r0, #24]
	ldrb	r3, [r2, r3]
	lsrs	r3, r3, #1
	adds	r3, #132
	cmp	r5, r3
	beq.n	.L_0815146e
	b.n	.L_081511b8
.L_0815146e:
	ldr	r0, [pc, #44]
	bl	sub_08014644
	ldr	r0, [pc, #32]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
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
	.4byte 0x0814c929
	.4byte 0x08143001
	.4byte 0x08198300
	.4byte 0x02010000
	.4byte 0x08197410
	.4byte 0x08198316
	.4byte 0x08198303
	.4byte 0x0819830f
	.2byte 0x8309
	.2byte 0x0819
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #96
	str	r0, [sp, #48]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	movs	r6, #0
	str	r0, [sp, #44]
	movs	r0, #1
	ldr	r1, [r3, #96]
	mov	r8, r6
	str	r1, [sp, #40]
	ldr	r3, [r3, #100]
	str	r3, [sp, #32]
	bl	sub_081435e0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r2, [sp, #44]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #44]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r0, [pc, #36]
	ldr	r1, [sp, #32]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	mov	r4, sp
	adds	r4, #52
	movs	r0, #0
	adds	r1, r4, #0
	str	r4, [sp, #28]
	bl	sub_08144aac
	ldr	r5, [sp, #44]
	b.n	.L_08151530
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x0000017c
	.2byte 0x017e
	.2byte 0x0000
.L_08151530:
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r3, #56
	str	r3, [r5, #8]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	movs	r0, #1
	subs	r3, #64
	add	r8, r0
	lsls	r3, r3, #16
	mov	r1, r8
	str	r3, [r5, #4]
	adds	r5, #28
	cmp	r1, #64
	bne.n	.L_08151530
	ldr	r3, [sp, #44]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r6, [sp, #44]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #132
	adds	r2, r6, r0
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #696]
	bl	sub_080145a8
	ldr	r1, [sp, #48]
	ldr	r3, [r1, #4]
	cmp	r3, #1
	bne.n	.L_0815159a
	movs	r2, #128
	ldr	r3, [pc, #684]
	lsls	r2, r2, #19
	adds	r2, #40
	str	r3, [r2, #0]
.L_0815159a:
	movs	r2, #0
	str	r2, [sp, #36]
	ldr	r3, [sp, #48]
	ldr	r1, [pc, #672]
	ldr	r2, [r3, #24]
	movs	r4, #50
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #2
	ldrb	r3, [r1, r3]
	negs	r4, r4
	cmp	r3, r4
	bne.n	.L_081515b6
	b.n	.L_08151b00
.L_081515b6:
	mov	r6, sp
	adds	r6, #60
	str	r6, [sp, #8]
.L_081515bc:
	movs	r0, #240
	movs	r3, #0
	lsls	r0, r0, #15
	str	r3, [sp, #16]
	str	r0, [sp, #20]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #2
	ldrb	r3, [r1, r3]
	ldr	r4, [sp, #36]
	adds	r3, #11
	cmp	r4, r3
	bne.n	.L_081515dc
	movs	r0, #133
	bl	sub_081180e8
.L_081515dc:
	ldr	r0, [sp, #8]
	ldr	r6, [sp, #16]
	movs	r3, #128
	lsls	r3, r3, #18
	str	r3, [r0, #8]
	str	r6, [r0, #0]
	str	r6, [r0, #4]
	bl	sub_08014de4
	ldr	r0, [sp, #8]
	bl	sub_08015128
	ldr	r1, [sp, #36]
	subs	r1, #36
	str	r1, [sp, #12]
	cmp	r1, #27
	bhi.n	.L_0815160e
	ldr	r2, [sp, #36]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0815160e
	movs	r0, #115
	bl	sub_081c0010
.L_0815160e:
	ldr	r3, [sp, #36]
	cmp	r3, #85
	bne.n	.L_0815161a
	movs	r0, #136
	bl	sub_081c0010
.L_0815161a:
	ldr	r6, [sp, #48]
	movs	r4, #0
	ldr	r3, [r6, #20]
	mov	r8, r4
	cmp	r3, #0
	beq.n	.L_08151652
	movs	r6, #36
	movs	r5, #40
.L_0815162a:
	ldr	r0, [sp, #36]
	cmp	r0, r5
	bne.n	.L_08151646
	ldr	r1, [sp, #48]
	movs	r3, #0
	ldrsh	r0, [r6, r1]
	str	r3, [sp, #0]
	movs	r1, #9
	subs	r3, #1
	movs	r2, #5
	bl	sub_0814cd48
	ldr	r4, [sp, #48]
	ldr	r3, [r4, #20]
.L_08151646:
	movs	r0, #1
	add	r8, r0
	adds	r6, #2
	adds	r5, #4
	cmp	r8, r3
	bne.n	.L_0815162a
.L_08151652:
	ldr	r1, [sp, #48]
	ldr	r0, [pc, #492]
	ldr	r5, [r1, #24]
	ldr	r6, [sp, #36]
	lsls	r3, r5, #1
	adds	r2, r3, r5
	adds	r3, r2, #2
	ldrb	r3, [r0, r3]
	movs	r4, #16
	adds	r1, r5, #0
	str	r4, [sp, #24]
	cmp	r6, r3
	bge.n	.L_08151670
	ldrb	r2, [r0, r2]
	str	r2, [sp, #24]
.L_08151670:
	lsls	r3, r1, #1
	adds	r3, r3, r1
	adds	r3, #2
	ldrb	r3, [r0, r3]
	ldr	r0, [sp, #36]
	adds	r3, #35
	cmp	r0, r3
	blt.n	.L_08151682
	b.n	.L_081517b2
.L_08151682:
	ldr	r2, [sp, #24]
	movs	r1, #0
	mov	r8, r1
	cmp	r2, #0
	bne.n	.L_0815168e
	b.n	.L_081517b2
.L_0815168e:
	ldr	r6, [sp, #44]
	movs	r3, #84
	movs	r4, #72
	add	r3, sp
	add	r4, sp
	mov	fp, r3
	mov	r9, r4
	mov	sl, r6
.L_0815169e:
	ldr	r0, [sp, #36]
	cmp	r0, r8
	ble.n	.L_0815179e
	mov	r1, r8
	cmp	r1, #0
	bge.n	.L_081516ac
	adds	r1, #7
.L_081516ac:
	asrs	r7, r1, #3
	lsls	r3, r7, #3
	mov	r1, r8
	subs	r7, r1, r3
	lsrs	r3, r1, #31
	add	r3, r8
	asrs	r3, r3, #1
	mov	r6, sl
	movs	r2, #48
	subs	r2, r2, r3
	ldr	r3, [r6, #4]
	lsls	r2, r2, #16
	cmp	r3, r2
	bge.n	sub_08151734
	ldr	r2, [pc, #380]
	cmp	r3, r2
	ble.n	sub_08151734
	ldr	r0, [r6, #0]
	bl	sub_08002096
	ldr	r3, [r6, #8]
	mov	r4, fp
	muls	r3, r0
	str	r3, [r4, #0]
	ldr	r3, [r6, #4]
	str	r3, [r4, #4]
	ldr	r0, [r6, #0]
	bl	sub_08002090
	ldr	r3, [r6, #8]
	mov	r1, r9
	muls	r3, r0
	mov	r0, fp
	str	r3, [r0, #8]
	bl	sub_0815e1ec
	mov	r1, r9
	ldr	r2, [r1, #0]
	movs	r4, #6
	ldrsh	r3, [r1, r4]
	ldr	r0, [pc, #332]
	asrs	r2, r2, #17
	adds	r2, #64
	adds	r3, #60
	str	r2, [r1, #0]
	str	r3, [r1, #4]
	lsls	r1, r7, #1
	ldrh	r1, [r0, r1]
	ldr	r0, [sp, #44]
	movs	r4, #224
	adds	r1, r0, r1
	ldr	r0, [pc, #316]
	lsls	r4, r4, #3
	ldrb	r5, [r0, r7]
	adds	r1, r1, r4
	lsrs	r0, r5, #1
	subs	r2, r2, r0
	ldr	r0, [pc, #308]
	ldrb	r4, [r0, r7]
	str	r5, [sp, #0]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #28]
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #40]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x990c
	ldr	r2, [pc, #268]
	ldr	r5, [r1, #24]
	ldr	r4, [sp, #36]
	lsls	r3, r5, #1
	adds	r3, r3, r5
	adds	r3, #2
	ldrb	r3, [r2, r3]
	cmp	r4, r3
	bge.n	.L_08151776
	mov	r3, r8
	adds	r3, #16
	cmp	r4, r3
	ble.n	.L_0815179e
	ldr	r3, [r6, #8]
	cmp	r3, #4
	ble.n	.L_0815175a
	subs	r3, #2
	str	r3, [r6, #8]
.L_0815175a:
	ldr	r3, [r6, #4]
	ldr	r0, [pc, #248]
	cmp	r3, r0
	bgt.n	.L_0815176a
	movs	r1, #160
	lsls	r1, r1, #11
	adds	r3, r3, r1
	str	r3, [r6, #4]
.L_0815176a:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #0]
	b.n	.L_0815179e
.L_08151776:
	ldr	r3, [r6, #8]
	mov	r0, r8
	adds	r3, #8
	str	r3, [r6, #8]
	movs	r1, #5
	bl	sub_08002064
	ldr	r3, [r6, #4]
	adds	r0, #2
	lsls	r0, r0, #16
	subs	r3, r3, r0
	str	r3, [r6, #4]
	ldr	r4, [sp, #20]
	cmp	r4, r3
	ble.n	.L_08151796
	str	r3, [sp, #20]
.L_08151796:
	ldr	r6, [sp, #16]
	cmp	r6, r3
	bge.n	.L_0815179e
	str	r3, [sp, #16]
.L_0815179e:
	ldr	r2, [sp, #24]
	movs	r1, #1
	movs	r0, #28
	add	r8, r1
	add	sl, r0
	cmp	r8, r2
	beq.n	.L_081517ae
	b.n	.L_0815169e
.L_081517ae:
	ldr	r3, [sp, #48]
	ldr	r5, [r3, #24]
.L_081517b2:
	ldr	r4, [sp, #20]
	ldr	r6, [sp, #16]
	movs	r3, #128
	lsls	r3, r3, #15
	adds	r4, r4, r3
	adds	r6, r6, r3
	str	r4, [sp, #20]
	lsls	r3, r5, #1
	str	r6, [sp, #16]
	ldr	r0, [pc, #124]
	adds	r2, r3, r5
	adds	r3, r2, #2
	ldrb	r3, [r0, r3]
	ldr	r1, [sp, #36]
	cmp	r1, r3
	bge.n	.L_081518aa
	movs	r3, #0
	mov	r8, r3
	adds	r3, r2, #1
	ldrb	r3, [r0, r3]
	cmp	r3, #0
	beq.n	.L_081518aa
	ldr	r4, [pc, #124]
	ldr	r6, [pc, #124]
	mov	sl, r4
	mov	r9, r6
	movs	r7, #0
.L_081517e8:
	ldr	r0, [sp, #12]
	movs	r1, #3
	bl	sub_08002054
	cmp	r8, r0
	bge.n	.L_08151894
	movs	r1, #3
	mov	r0, r8
	bl	sub_08002064
	lsls	r3, r5, #1
	adds	r4, r0, #0
	ldr	r0, [pc, #64]
	adds	r3, r3, r5
	adds	r3, #2
	ldrb	r3, [r0, r3]
	ldr	r1, [sp, #36]
	subs	r3, #7
	cmp	r1, r3
	blt.n	.L_08151868
	lsls	r3, r4, #1
	mov	r2, sl
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #44]
	movs	r0, #224
	adds	r1, r3, r1
	lsls	r0, r0, #3
	adds	r1, r1, r0
	ldr	r0, [pc, #64]
	mov	r3, r9
	ldrb	r4, [r0, r4]
	movs	r0, #32
	ldrb	r2, [r7, r3]
	ldrb	r3, [r6, #1]
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #28]
	subs	r3, r3, r4
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #40]
	b.n	.L_0815188c
	movs	r0, r0
	.4byte 0x08143001
	.4byte 0xffff9000
	.4byte 0x08198348
	.4byte 0xffd00000
	.4byte 0x08198362
	.4byte 0x08198351
	.4byte 0x08198359
	.4byte 0x002fffff
	.4byte 0x08198372
	.4byte 0x08198322
	.2byte 0x8378
	.2byte 0x0819
.L_08151868:
	lsls	r3, r4, #1
	mov	r2, sl
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #44]
	movs	r0, #224
	adds	r1, r3, r1
	lsls	r0, r0, #3
	adds	r1, r1, r0
	ldr	r0, [pc, #680]
	ldrb	r3, [r6, #1]
	ldrb	r4, [r0, r4]
	movs	r0, #32
	ldrb	r2, [r6, #0]
	subs	r3, r3, r4
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #40]
	ldr	r4, [sp, #52]
.L_0815188c:
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x990c
	ldr	r5, [r1, #24]
.L_08151894:
	lsls	r3, r5, #1
	ldr	r4, [pc, #656]
	adds	r3, r3, r5
	adds	r3, #1
	ldrb	r3, [r4, r3]
	movs	r2, #1
	add	r8, r2
	adds	r6, #2
	adds	r7, #2
	cmp	r8, r3
	bne.n	.L_081517e8
.L_081518aa:
	lsls	r3, r5, #1
	ldr	r6, [pc, #632]
	adds	r3, r3, r5
	adds	r3, #2
	ldrb	r3, [r6, r3]
	ldr	r0, [sp, #36]
	cmp	r0, r3
	bne.n	.L_08151910
	ldr	r5, [pc, #624]
	movs	r1, #0
	mov	r8, r1
	movs	r6, #15
.L_081518c2:
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #80
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	subs	r3, #32
	lsls	r3, r3, #12
	str	r3, [r5, #8]
	bl	sub_08014878
	negs	r0, r0
	ands	r0, r6
	subs	r0, #16
	lsls	r0, r0, #13
	str	r0, [r5, #16]
	bl	sub_08014878
	movs	r2, #1
	ands	r0, r6
	add	r8, r2
	adds	r0, #16
	mov	r3, r8
	str	r0, [r5, #24]
	adds	r5, #28
	cmp	r3, #32
	bne.n	.L_081518c2
	ldr	r4, [sp, #48]
	ldr	r5, [r4, #24]
.L_08151910:
	lsls	r3, r5, #1
	ldr	r6, [pc, #532]
	adds	r3, r3, r5
	adds	r3, #2
	ldrb	r3, [r6, r3]
	ldr	r0, [sp, #36]
	cmp	r0, r3
	blt.n	.L_081519a2
	ldr	r5, [pc, #520]
	movs	r1, #0
	mov	r8, r1
.L_08151926:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_08151980
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_08151934
	adds	r3, #7
.L_08151934:
	asrs	r4, r3, #3
	lsls	r3, r4, #3
	mov	r2, r8
	subs	r4, r2, r3
	ldr	r2, [pc, #496]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	ldr	r0, [pc, #492]
	ldr	r3, [sp, #44]
	ldrb	r0, [r0, r4]
	movs	r6, #224
	adds	r1, r3, r1
	lsls	r6, r6, #3
	adds	r1, r1, r6
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	str	r0, [sp, #0]
	ldr	r0, [pc, #476]
	ldrb	r0, [r0, r4]
	str	r0, [sp, #4]
	ldr	r0, [sp, #28]
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #40]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x682b
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_08151980:
	ldr	r3, [r5, #4]
	ldr	r1, [sp, #20]
	cmp	r1, r3
	ble.n	.L_0815198a
	str	r3, [sp, #20]
.L_0815198a:
	ldr	r2, [sp, #16]
	cmp	r2, r3
	bge.n	.L_08151992
	str	r3, [sp, #16]
.L_08151992:
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r5, #28
	cmp	r4, #24
	bne.n	.L_08151926
	ldr	r6, [sp, #48]
	ldr	r5, [r6, #24]
.L_081519a2:
	ldr	r0, [sp, #20]
	ldr	r1, [sp, #16]
	asrs	r0, r0, #16
	asrs	r1, r1, #16
	str	r0, [sp, #20]
	str	r1, [sp, #16]
	cmp	r1, r0
	bgt.n	.L_081519b6
	adds	r0, #1
	str	r0, [sp, #16]
.L_081519b6:
	lsls	r3, r5, #1
	ldr	r2, [pc, #364]
	adds	r3, r3, r5
	adds	r3, #2
	ldrb	r3, [r2, r3]
	ldr	r4, [sp, #36]
	cmp	r4, r3
	bne.n	.L_08151a18
	ldr	r5, [sp, #44]
	movs	r6, #0
	mov	r8, r6
.L_081519cc:
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	lsls	r3, r3, #16
	str	r3, [r5, #12]
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #20]
	cmp	r0, r1
	bne.n	.L_081519e6
	lsls	r3, r1, #16
	str	r3, [r5, #16]
	b.n	.L_081519fc
.L_081519e6:
	bl	sub_08014878
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #20]
	subs	r1, r2, r3
	bl	sub_0800206c
	ldr	r4, [sp, #20]
	adds	r0, r0, r4
	lsls	r0, r0, #16
	str	r0, [r5, #16]
.L_081519fc:
	bl	sub_08014878
	movs	r6, #1
	movs	r3, #15
	ands	r3, r0
	add	r8, r6
	adds	r3, #20
	mov	r0, r8
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r0, #32
	bne.n	.L_081519cc
	ldr	r1, [sp, #48]
	ldr	r5, [r1, #24]
.L_08151a18:
	lsls	r3, r5, #1
	ldr	r2, [pc, #268]
	adds	r3, r3, r5
	adds	r3, #2
	ldrb	r3, [r2, r3]
	ldr	r4, [sp, #36]
	cmp	r4, r3
	blt.n	.L_08151ad0
	subs	r3, r4, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	ldr	r7, [pc, #248]
	ldr	r5, [sp, #44]
	asrs	r3, r3, #1
	movs	r6, #0
	mov	sl, r3
	mov	r8, r6
.L_08151a3a:
	ldr	r3, [r5, #24]
	cmp	r3, #17
	bhi.n	.L_08151a7a
	movs	r0, #17
	subs	r0, r0, r3
	lsrs	r3, r0, #31
	ldr	r2, [pc, #244]
	adds	r0, r0, r3
	asrs	r0, r0, #1
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #32]
	adds	r1, r2, r1
	movs	r3, #14
	ldrsh	r2, [r5, r3]
	ldr	r3, [pc, #228]
	ldrb	r4, [r3, r0]
	movs	r6, #18
	ldrsh	r3, [r5, r6]
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r6, [sp, #28]
	lsrs	r0, r4, #1
	subs	r2, r2, r0
	subs	r3, r3, r0
	mov	r0, sl
	subs	r3, r3, r0
	ldr	r4, [r6, #4]
	ldr	r0, [sp, #40]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69ab
.L_08151a7a:
	movs	r0, #1
	subs	r3, #1
	negs	r0, r0
	str	r3, [r5, #24]
	cmp	r3, r0
	beq.n	.L_08151a8a
	cmp	r3, #17
	bne.n	.L_08151ac4
.L_08151a8a:
	ldr	r1, [sp, #48]
	ldr	r3, [r1, #24]
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r2, #2
	ldrb	r3, [r7, r2]
	ldr	r2, [sp, #36]
	adds	r3, #35
	cmp	r2, r3
	bge.n	.L_08151ac4
	movs	r3, #17
	str	r3, [r5, #24]
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	lsls	r3, r3, #16
	str	r3, [r5, #12]
	bl	sub_08014878
	ldr	r3, [sp, #16]
	ldr	r4, [sp, #20]
	subs	r1, r3, r4
	bl	sub_0800206c
	ldr	r6, [sp, #20]
	adds	r0, r0, r6
	lsls	r0, r0, #16
	str	r0, [r5, #16]
.L_08151ac4:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #32
	bne.n	.L_08151a3a
.L_08151ad0:
	ldr	r3, [sp, #44]
	movs	r4, #240
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r6, [sp, #36]
	ldr	r0, [sp, #48]
	adds	r6, #1
	str	r6, [sp, #36]
	ldr	r1, [pc, #56]
	ldr	r2, [r0, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #2
	ldrb	r3, [r1, r3]
	adds	r3, #50
	cmp	r6, r3
	beq.n	.L_08151b00
	b.n	.L_081515bc
.L_08151b00:
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #52]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #96
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x08198378
	.4byte 0x08198348
	.4byte 0x02010000
	.4byte 0x0819838c
	.4byte 0x0819837b
	.4byte 0x08198383
	.4byte 0x0819744c
	.4byte 0x0819745e
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #64
	str	r0, [sp, #40]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #92]
	str	r1, [sp, #36]
	ldr	r2, [r3, #96]
	str	r2, [sp, #32]
	ldr	r3, [r3, #100]
	str	r3, [sp, #24]
	movs	r3, #0
	str	r3, [sp, #20]
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_08151b80
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #1
	bl	sub_08143b20
	b.n	.L_08151b8a
.L_08151b80:
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #1
	bl	sub_08143a88
.L_08151b8a:
	ldr	r1, [sp, #40]
	ldr	r3, [r1, #24]
	cmp	r3, #2
	bne.n	.L_08151b9c
	movs	r2, #128
	ldr	r3, [pc, #0]
	b.n	.L_08151ba0
	.2byte 0x0080
	.2byte 0x0000
.L_08151b9c:
	movs	r2, #128
	ldr	r3, [pc, #60]
.L_08151ba0:
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #104]
	movs	r1, #7
	movs	r0, #188
	str	r3, [sp, #44]
	bl	sub_081963ec
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r2, sp
	adds	r2, #44
	str	r2, [sp, #12]
	str	r3, [r2, #4]
	ldr	r3, [sp, #36]
	movs	r2, #208
	lsls	r2, r2, #4
	adds	r2, #14
	adds	r1, r3, r2
	ldr	r0, [pc, #8]
	movs	r2, #1
	movs	r3, #1
	b.n	.L_08151be4
	.4byte 0x00000100
	.2byte 0x0188
	.2byte 0x0000
.L_08151be4:
	bl	sub_08157cf4
	movs	r3, #0
	ldr	r1, [sp, #24]
	ldr	r0, [pc, #728]
	movs	r2, #0
	bl	sub_08157cf4
	ldr	r1, [sp, #40]
	ldr	r3, [r1, #24]
	cmp	r3, #2
	bne.n	.L_08151c24
	ldr	r2, [sp, #36]
	movs	r1, #240
	lsls	r1, r1, #7
	adds	r1, #240
	adds	r3, r2, r1
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_08151c18
	movs	r2, #128
	ldr	r3, [pc, #696]
	lsls	r2, r2, #19
	adds	r2, #40
	b.n	.L_08151c56
.L_08151c18:
	movs	r2, #128
	lsls	r2, r2, #19
	movs	r3, #128
	adds	r2, #40
	lsls	r3, r3, #5
	b.n	.L_08151c56
.L_08151c24:
	add	r5, sp, #52
	adds	r0, r5, #0
	bl	sub_0815e22c
	ldr	r2, [r5, #0]
	adds	r3, r2, #0
	subs	r3, #64
	str	r3, [r5, #0]
	cmp	r3, #0
	bge.n	.L_08151c3e
	str	r3, [sp, #20]
	movs	r3, #0
	b.n	.L_08151c48
.L_08151c3e:
	cmp	r3, #112
	ble.n	.L_08151c4a
	subs	r2, #176
	movs	r3, #112
	str	r2, [sp, #20]
.L_08151c48:
	str	r3, [r5, #0]
.L_08151c4a:
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #19
	negs	r3, r3
	adds	r2, #40
	lsls	r3, r3, #8
.L_08151c56:
	str	r3, [r2, #0]
	ldr	r7, [pc, #628]
	movs	r2, #0
	mov	r8, r2
.L_08151c5e:
	bl	sub_08014878
	movs	r6, #192
	lsls	r6, r6, #2
	adds	r6, #255
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r6, r0
	adds	r6, r6, r3
	bl	sub_08014878
	movs	r5, #254
	ldr	r1, [pc, #604]
	lsls	r5, r5, #7
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r5, #255
	str	r3, [r7, #0]
	ands	r5, r0
	movs	r3, #224
	adds	r5, r5, r1
	lsls	r3, r3, #7
	str	r3, [r7, #4]
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #16
	str	r3, [r7, #8]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #16
	str	r3, [r7, #20]
	movs	r3, #0
	str	r3, [r7, #24]
	movs	r2, #1
	movs	r3, #128
	add	r8, r2
	lsls	r3, r3, #3
	adds	r7, #28
	cmp	r8, r3
	bne.n	.L_08151c5e
	ldr	r1, [sp, #36]
	movs	r3, #239
	lsls	r3, r3, #7
	adds	r2, r1, r3
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r1, r3
	movs	r3, #75
	movs	r1, #200
	lsls	r1, r1, #4
	str	r3, [r2, #0]
	ldr	r0, [pc, #508]
	bl	sub_080145a8
	movs	r0, #138
	bl	sub_081c0010
	movs	r1, #0
	str	r1, [sp, #28]
.L_08151cea:
	ldr	r2, [sp, #28]
	cmp	r2, #20
	bne.n	.L_08151cf6
	movs	r0, #133
	bl	sub_081180e8
.L_08151cf6:
	ldr	r3, [sp, #28]
	cmp	r3, #15
	ble.n	.L_08151cfe
	b.n	.L_08151e96
.L_08151cfe:
	adds	r0, r3, #0
	movs	r1, #5
	bl	sub_08002064
	cmp	r0, #2
	bne.n	sub_08151d18
	movs	r1, #128
	ldr	r3, [pc, #460]
	ldr	r0, [sp, #32]
	lsls	r1, r1, #7
	ldr	r2, [pc, #460]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9a07
	movs	r1, #0
	lsls	r2, r2, #11
	str	r2, [sp, #8]
	mov	fp, r1
.L_08151d22:
	ldr	r1, [sp, #8]
	movs	r2, #128
	lsls	r2, r2, #7
	movs	r3, #0
	str	r3, [sp, #16]
	adds	r3, r1, r2
	mov	r5, fp
	muls	r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	ldr	r1, [sp, #28]
	movs	r3, #32
	subs	r3, r3, r1
	muls	r3, r0
	asrs	r3, r3, #16
	adds	r3, #64
	adds	r0, r5, #0
	mov	sl, r3
	bl	sub_08002090
	ldr	r2, [sp, #40]
	lsls	r0, r0, #3
	asrs	r0, r0, #16
	ldr	r3, [r2, #24]
	negs	r0, r0
	adds	r6, r0, #0
	subs	r6, #8
	cmp	r3, #0
	bne.n	.L_08151da2
	bl	sub_08014878
	movs	r3, #3
	ands	r0, r3
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r5, r3, #4
	subs	r5, r5, r3
	ldr	r3, [sp, #36]
	movs	r1, #208
	lsls	r5, r5, #6
	lsls	r1, r1, #4
	adds	r5, r3, r5
	adds	r1, #14
	adds	r5, r5, r1
	bl	sub_08014878
	ldr	r2, [sp, #20]
	movs	r3, #7
	ands	r0, r3
	add	r2, sl
	movs	r3, #24
	adds	r2, r2, r0
	str	r3, [sp, #0]
	movs	r3, #120
	str	r3, [sp, #4]
	subs	r2, #16
	ldr	r4, [sp, #44]
	ldr	r0, [sp, #32]
	adds	r1, r5, #0
	adds	r3, r6, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe025
.L_08151da2:
	bl	sub_08014878
	movs	r3, #3
	ands	r0, r3
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r5, r3, #4
	ldr	r1, [sp, #36]
	subs	r5, r5, r3
	movs	r2, #208
	lsls	r5, r5, #6
	lsls	r2, r2, #4
	adds	r5, r1, r5
	adds	r2, #14
	adds	r5, r5, r2
	bl	sub_08014878
	movs	r3, #7
	ands	r0, r3
	movs	r1, #1
	mov	r3, fp
	ands	r1, r3
	ldr	r2, [sp, #20]
	movs	r3, #24
	str	r3, [sp, #0]
	movs	r3, #120
	str	r3, [sp, #4]
	ldr	r3, [sp, #12]
	add	r2, sl
	adds	r2, r2, r0
	lsls	r1, r1, #2
	ldr	r4, [r1, r3]
	subs	r2, #16
	ldr	r0, [sp, #32]
	adds	r1, r5, #0
	adds	r3, r6, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c33
	adds	r3, #112
	ldr	r7, [pc, #220]
	movs	r1, #0
	lsls	r3, r3, #16
	mov	r8, r1
	mov	r9, r3
.L_08151dfc:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	bne.n	.L_08151e6e
	bl	sub_08014878
	movs	r6, #128
	lsls	r6, r6, #1
	adds	r6, #255
	ands	r6, r0
	bl	sub_08014878
	movs	r5, #254
	ldr	r2, [pc, #188]
	lsls	r5, r5, #7
	adds	r5, #255
	mov	r1, sl
	ands	r5, r0
	lsls	r3, r1, #16
	adds	r5, r5, r2
	mov	r2, r9
	str	r2, [r7, #4]
	str	r3, [r7, #0]
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r6, #128
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #9
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
	ands	r0, r3
	adds	r0, #32
	str	r0, [r7, #24]
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #40]
	adds	r1, #1
	str	r1, [sp, #16]
	ldr	r1, [pc, #132]
	ldr	r3, [r2, #24]
	ldr	r2, [sp, #16]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r1, r3]
	cmp	r2, r3
	beq.n	.L_08151e7c
.L_08151e6e:
	movs	r3, #1
	movs	r1, #128
	add	r8, r3
	lsls	r1, r1, #3
	adds	r7, #28
	cmp	r8, r1
	bne.n	.L_08151dfc
.L_08151e7c:
	movs	r2, #1
	add	fp, r2
	mov	r3, fp
	cmp	r3, #4
	beq.n	.L_08151e88
	b.n	.L_08151d22
.L_08151e88:
	ldr	r1, [sp, #36]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
.L_08151e96:
	ldr	r5, [pc, #56]
	movs	r1, #0
	mov	r8, r1
.L_08151e9c:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	ble.n	sub_08151f2e
	subs	r3, #1
	str	r3, [r5, #24]
	ldr	r2, [pc, #64]
	adds	r0, r5, #0
	movs	r1, #60
	bl	sub_08138086
	ldr	r3, [r5, #4]
	movs	r2, #240
	lsls	r2, r2, #15
	cmp	r3, r2
	ble.n	.L_08151eec
	ldr	r3, [r5, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r5, #16]
	b.n	sub_08151f2e
	.4byte 0x00000134
	.4byte 0xfffff000
	.4byte 0x02010000
	.4byte 0xffffc000
	.4byte 0x08143001
	.4byte 0x03000260
	.4byte 0x10101010
	.4byte 0x0819839c
	.2byte 0xf800
	.2byte 0xffff
.L_08151eec:
	ldr	r2, [r5, #0]
	ldr	r1, [pc, #220]
	cmp	r2, r1
	bhi.n	sub_08151f2e
	cmp	r3, #0
	blt.n	sub_08151f2e
	ldr	r0, [r5, #24]
	asrs	r6, r2, #16
	asrs	r7, r3, #16
	cmp	r0, #0
	bge.n	.L_08151f04
	adds	r0, #7
.L_08151f04:
	asrs	r0, r0, #3
	adds	r0, #1
	ldr	r2, [pc, #196]
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #20]
	adds	r1, r2, r1
	adds	r2, r3, r6
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	str	r0, [sp, #0]
	subs	r3, r7, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #32]
	ldr	r4, [sp, #44]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2101
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #3
	adds	r5, #28
	cmp	r8, r2
	bne.n	.L_08151e9c
	ldr	r3, [sp, #28]
	subs	r3, #4
	cmp	r3, #91
	bhi.n	.L_08151f7c
	ldr	r1, [sp, #40]
	movs	r3, #0
	mov	r8, r3
	ldr	r3, [r1, #20]
	cmp	r3, #0
	beq.n	.L_08151f7c
	movs	r6, #36
	movs	r5, #4
.L_08151f54:
	ldr	r2, [sp, #28]
	cmp	r2, r5
	bne.n	.L_08151f70
	ldr	r3, [sp, #40]
	movs	r2, #5
	ldrsh	r0, [r6, r3]
	movs	r3, #10
	str	r3, [sp, #0]
	movs	r1, #7
	mov	r3, r8
	bl	sub_0814cd48
	ldr	r2, [sp, #40]
	ldr	r3, [r2, #20]
.L_08151f70:
	movs	r1, #1
	add	r8, r1
	adds	r6, #2
	adds	r5, #4
	cmp	r8, r3
	bne.n	.L_08151f54
.L_08151f7c:
	movs	r0, #2
	movs	r1, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r1, #240
	ldr	r3, [sp, #36]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r3, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #28]
	adds	r2, #1
	str	r2, [sp, #28]
	cmp	r2, #64
	beq.n	.L_08151fa8
	b.n	.L_08151cea
.L_08151fa8:
	ldr	r0, [pc, #40]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #64
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x007effff
	.4byte 0x08197410
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #1
	bl	sub_08151ff0
	pop	{pc}
	.align 2, 0
