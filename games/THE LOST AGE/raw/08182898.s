.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0800206c, 0x0800206c
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_08014274, 0x08014274
	.set sub_080143e0, 0x080143e0
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014c4c, 0x08014c4c
	.set sub_08014dac, 0x08014dac
	.set sub_08014de4, 0x08014de4
	.set sub_08014e38, 0x08014e38
	.set sub_08014ea8, 0x08014ea8
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_0801521c, 0x0801521c
	.set sub_080156e8, 0x080156e8
	.set sub_0801587c, 0x0801587c
	.set sub_08020010, 0x08020010
	.set sub_08020030, 0x08020030
	.set sub_08020040, 0x08020040
	.set sub_08020048, 0x08020048
	.set sub_08020090, 0x08020090
	.set sub_08020138, 0x08020138
	.set sub_08020148, 0x08020148
	.set sub_08020380, 0x08020380
	.set sub_08020388, 0x08020388
	.set sub_08118010, 0x08118010
	.set sub_08118028, 0x08118028
	.set sub_08118040, 0x08118040
	.set sub_08118070, 0x08118070
	.set sub_08118078, 0x08118078
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_081180f0, 0x081180f0
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
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_08158d68, 0x08158d68
	.set sub_0815b290, 0x0815b290
	.set sub_0815b3b0, 0x0815b3b0
	.set sub_0815b410, 0x0815b410
	.set sub_0815b434, 0x0815b434
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e21c, 0x0815e21c
	.set sub_0815f000, 0x0815f000
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_08163c2c, 0x08163c2c
	.set sub_08164a4c, 0x08164a4c
	.set sub_08164abc, 0x08164abc
	.set sub_08164b2c, 0x08164b2c
	.set sub_0816ae40, 0x0816ae40
	.set sub_08178680, 0x08178680
	.set sub_08179e6c, 0x08179e6c
	.set sub_081848ec, 0x081848ec
	.set sub_08185582, 0x08185582
	.set sub_0818559a, 0x0818559a
	.set sub_081855b2, 0x081855b2
	.set sub_08185dd8, 0x08185dd8
	.set sub_08185ed8, 0x08185ed8
	.set sub_081889b0, 0x081889b0
	.set sub_08188f3e, 0x08188f3e
	.set sub_081890e4, 0x081890e4
	.set sub_08189238, 0x08189238
	.set sub_081894c2, 0x081894c2
	.set sub_081896b6, 0x081896b6
	.set sub_0818a220, 0x0818a220
	.set sub_0818caa8, 0x0818caa8
	.set sub_081963ec, 0x081963ec
	.set sub_08196404, 0x08196404
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Overlay_08182898
Overlay_08182898:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r5, #192
	lsls	r5, r5, #18
	mov	r9, r0
	ldr	r0, [r5, #96]
	sub	sp, #24
	str	r0, [sp, #20]
	ldr	r0, [pc, #464]
	ldr	r1, [r5, #92]
	mov	fp, r1
	movs	r1, #200
	lsls	r1, r1, #4
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
	movs	r3, #50
	str	r3, [r2, #0]
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	ldr	r5, [r5, #104]
	ldr	r0, [pc, #420]
	str	r5, [sp, #12]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #412]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x21e0
	lsls	r1, r1, #3
	movs	r2, #0
	ldr	r0, [pc, #400]
	add	r1, fp
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #0
	str	r2, [sp, #8]
.L_0818290a:
	ldr	r3, [sp, #8]
	cmp	r3, #0
	bne.n	.L_08182968
	ldr	r5, [pc, #384]
	movs	r4, #0
	mov	r8, r4
	movs	r7, #127
	movs	r6, #31
.L_0818291a:
	bl	sub_08014878
	ands	r0, r7
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	sub_08014878
	ands	r0, r7
	adds	r0, #120
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #16
	lsls	r0, r0, #13
	str	r0, [r5, #12]
	bl	sub_08014878
	movs	r3, #8
	ands	r0, r6
	subs	r3, r3, r0
	lsls	r3, r3, #12
	str	r3, [r5, #16]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #5
	movs	r0, #1
	movs	r1, #128
	str	r3, [r5, #8]
	add	r8, r0
	movs	r3, #32
	lsls	r1, r1, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r8, r1
	bne.n	.L_0818291a
.L_08182968:
	ldr	r3, [pc, #300]
	ldr	r7, [pc, #296]
	movs	r2, #0
	mov	r8, r2
	mov	sl, r3
.L_08182972:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	blt.n	.L_081829c8
	ldr	r6, [r7, #8]
	ldr	r2, [pc, #288]
	lsls	r3, r6, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #284]
	movs	r0, #2
	ldrsh	r2, [r7, r0]
	ldrb	r5, [r3, r6]
	movs	r4, #224
	lsls	r4, r4, #3
	lsrs	r3, r5, #1
	mov	r0, sl
	add	r1, fp
	adds	r1, r1, r4
	subs	r2, r2, r3
	movs	r4, #6
	ldrsh	r3, [r7, r4]
	ldrb	r4, [r0, r6]
	str	r5, [sp, #0]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #20]
	ldr	r4, [sp, #12]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c38
	ldr	r2, [pc, #244]
	movs	r1, #63
	bl	sub_08138086
	mov	r0, sl
	ldrb	r3, [r0, r6]
	ldr	r2, [r7, #4]
	lsls	r3, r3, #16
	cmn	r2, r3
	bge.n	.L_081829c8
	movs	r3, #1
	negs	r3, r3
	str	r3, [r7, #24]
.L_081829c8:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #1
	adds	r7, #28
	cmp	r8, r2
	bne.n	.L_08182972
	mov	r4, r9
	ldr	r2, [r4, #20]
	movs	r3, #0
	mov	r8, r3
	cmp	r2, #0
	beq.n	.L_08182a42
	movs	r6, #16
	movs	r5, #36
.L_081829e6:
	mov	r0, r8
	ldr	r1, [sp, #8]
	lsls	r3, r0, #3
	adds	r3, #18
	cmp	r1, r3
	bne.n	.L_08182a38
	cmp	r0, #0
	bne.n	.L_08182a1c
	mov	r3, r9
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r2, #1
	negs	r2, r2
	movs	r1, #7
	movs	r3, #0
	str	r6, [sp, #0]
	bl	sub_0814cd48
	movs	r0, #126
	bl	sub_081180e8
	movs	r0, #126
	bl	sub_081c0010
	mov	r4, r9
	ldr	r2, [r4, #20]
	b.n	.L_08182a38
.L_08182a1c:
	movs	r0, #126
	bl	sub_081c0010
	mov	r1, r9
	ldrsh	r0, [r5, r1]
	movs	r2, #1
	negs	r2, r2
	mov	r3, r8
	movs	r1, #7
	str	r6, [sp, #0]
	bl	sub_0814cd48
	mov	r3, r9
	ldr	r2, [r3, #20]
.L_08182a38:
	movs	r4, #1
	add	r8, r4
	adds	r5, #2
	cmp	r8, r2
	bne.n	.L_081829e6
.L_08182a42:
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, fp
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	ldr	r0, [sp, #8]
	adds	r0, #1
	str	r0, [sp, #8]
	cmp	r0, #60
	beq.n	.L_08182a64
	b.n	.L_0818290a
.L_08182a64:
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #24]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x08143001
	.4byte 0x00000190
	.4byte 0x03000730
	.4byte 0x000000fd
	.4byte 0x02015000
	.4byte 0x0819967e
	.4byte 0x0819968e
	.4byte 0x0819966f
	.2byte 0xc000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #164
	str	r0, [sp, #80]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #96]
	movs	r7, #0
	str	r0, [sp, #76]
	movs	r5, #80
	ldr	r1, [r3, #92]
	mov	fp, r7
	str	r1, [sp, #72]
	negs	r5, r5
	ldr	r3, [r3, #100]
	movs	r7, #1
	mov	sl, r3
	bl	sub_0813ba50
	movs	r0, #0
	bl	sub_081435e0
	bl	sub_08179e6c
	ldr	r3, [sp, #72]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r1, #200
	movs	r3, #0
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #308]
	bl	sub_080145a8
	movs	r0, #128
	ldr	r1, [sp, #72]
	movs	r2, #240
	lsls	r0, r0, #19
	lsls	r2, r2, #7
	adds	r0, #40
	adds	r2, #232
	mov	r8, r0
	adds	r6, r1, r2
.L_08182b0a:
	adds	r5, #4
	lsls	r3, r5, #8
	mov	r4, r8
	str	r3, [r4, #0]
	movs	r0, #1
	str	r7, [r6, #0]
	bl	sub_08013560
	movs	r0, #1
	add	fp, r0
	mov	r1, fp
	cmp	r1, #27
	bne.n	.L_08182b0a
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	mov	r1, sl
	str	r3, [sp, #64]
	ldr	r0, [pc, #244]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r3, [sp, #72]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r7, [sp, #72]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #132
	adds	r2, r7, r0
	movs	r3, #75
	movs	r1, #236
	str	r3, [r2, #0]
	lsls	r1, r1, #7
	movs	r2, #8
	adds	r1, #64
	negs	r2, r2
	movs	r3, #0
	adds	r4, r7, r1
	mov	sl, r2
	mov	r8, r3
.L_08182b6c:
	movs	r7, #1
	add	r8, r7
	mov	r0, r8
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r0, #15
	bne.n	.L_08182b6c
.L_08182b7a:
	mov	r5, r8
	subs	r5, #15
	movs	r1, #15
	adds	r0, r5, #0
	str	r4, [sp, #12]
	bl	sub_08002054
	mov	r1, sl
	subs	r0, r1, r0
	adds	r7, r0, #0
	adds	r7, #32
	adds	r3, r5, #0
	ldr	r4, [sp, #12]
	cmp	r5, #0
	bge.n	.L_08182b9a
	mov	r3, r8
.L_08182b9a:
	mov	r2, sl
	asrs	r3, r3, #4
	subs	r3, r2, r3
	movs	r1, #17
	adds	r0, r5, #0
	adds	r6, r3, #0
	str	r4, [sp, #12]
	bl	sub_08002054
	mov	r3, sl
	adds	r6, #16
	subs	r1, r3, r0
	ldr	r4, [sp, #12]
	cmp	r7, #0
	bge.n	.L_08182bba
	movs	r7, #0
.L_08182bba:
	cmp	r7, #31
	ble.n	.L_08182bc0
	movs	r7, #31
.L_08182bc0:
	cmp	r6, #0
	bge.n	.L_08182bc6
	movs	r6, #0
.L_08182bc6:
	cmp	r6, #31
	ble.n	.L_08182bcc
	movs	r6, #31
.L_08182bcc:
	cmp	r1, #0
	bge.n	.L_08182bd2
	movs	r1, #0
.L_08182bd2:
	cmp	r1, #31
	ble.n	.L_08182bd8
	movs	r1, #31
.L_08182bd8:
	lsls	r3, r7, #10
	lsls	r2, r6, #5
	movs	r7, #1
	orrs	r3, r2
	add	r8, r7
	orrs	r3, r1
	mov	r0, r8
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r0, #135
	bne.n	.L_08182b7a
	ldr	r3, [pc, #52]
.L_08182bf0:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r2, #160
	bne.n	.L_08182bf0
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #254
	ldr	r0, [pc, #40]
	bl	sub_080145a8
	movs	r2, #32
	movs	r3, #0
	ldr	r0, [pc, #36]
	ldr	r1, [pc, #36]
	str	r3, [sp, #52]
	.2byte 0xf005
	.2byte 0xfcec
	movs	r3, #255
	add	r0, sp, #136
	strh	r3, [r0, #0]
	movs	r1, #0
	b.n	.L_08182c3c
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x08143001
	.4byte 0x00000134
	.4byte 0x08164bb5
	.4byte 0x02014400
	.2byte 0x4800
	.2byte 0x0201
.L_08182c3c:
	bl	sub_08118010
	ldr	r0, [pc, #364]
	bl	sub_08013300
	movs	r2, #128
	adds	r5, r0, #0
	ldr	r6, [pc, #360]
	adds	r1, r5, #0
	lsls	r2, r2, #2
	ldr	r0, [pc, #356]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x2480
	lsls	r4, r4, #2
	adds	r5, r5, r4
	adds	r0, r5, #0
	ldr	r1, [pc, #348]
	bl	sub_0801587c
	movs	r7, #0
	ldr	r0, [pc, #344]
	ldr	r1, [sp, #72]
	movs	r2, #238
	mov	r8, r7
	lsls	r2, r2, #7
	movs	r3, #13
	ldr	r7, [pc, #328]
	adds	r2, #220
	negs	r3, r3
	mov	sl, r6
	mov	r9, r0
	adds	r5, r1, r2
	adds	r6, r3, #0
.L_08182c80:
	movs	r1, #32
	ldr	r2, [pc, #320]
	movs	r3, #0
	movs	r0, #32
	bl	sub_0815b290
	ldrb	r3, [r0, #9]
	movs	r2, #8
	ands	r3, r6
	orrs	r3, r2
	strb	r3, [r0, #9]
	ldrb	r3, [r0, #16]
	stmia	r5!, {r0}
	lsls	r3, r3, #2
	add	r3, r9
	ldrh	r0, [r3, #2]
	ldr	r4, [pc, #292]
	movs	r2, #128
	adds	r1, r7, #0
	lsls	r2, r2, #3
	adds	r0, r0, r4
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x2101
	movs	r0, #128
	add	r8, r1
	lsls	r0, r0, #3
	mov	r2, r8
	adds	r7, r7, r0
	cmp	r2, #12
	bne.n	.L_08182c80
	bl	sub_0815b410
	ldr	r0, [pc, #264]
	ldr	r1, [pc, #244]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r3, [sp, #72]
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r1, r3, r4
	ldr	r0, [pc, #228]
	movs	r2, #64
	movs	r3, #64
	bl	sub_0816ae40
	ldr	r7, [sp, #72]
	movs	r2, #142
	lsls	r2, r2, #7
	adds	r1, r7, r2
	ldr	r0, [pc, #228]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r3, #158
	lsls	r3, r3, #7
	adds	r5, r7, r3
	ldr	r0, [pc, #216]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r0, #180
	movs	r1, #176
	lsls	r0, r0, #7
	movs	r4, #0
	lsls	r1, r1, #4
	adds	r0, #184
	mov	r8, r4
	adds	r1, #184
	adds	r2, r7, r0
.L_08182d16:
	ldrb	r3, [r5, #0]
	adds	r5, #1
	lsrs	r3, r3, #1
	strb	r3, [r2, #0]
	movs	r3, #1
	add	r8, r3
	adds	r2, #1
	cmp	r8, r1
	bne.n	.L_08182d16
	ldr	r0, [pc, #172]
	ldr	r1, [pc, #144]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r1, [pc, #164]
	ldr	r0, [pc, #168]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_08182d66
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #234
	adds	r3, r3, r1
	lsls	r2, r2, #7
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_08182d66:
	strh	r4, [r0, #0]
	ldr	r2, [pc, #120]
	movs	r3, #240
	str	r3, [r2, #16]
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	adds	r2, #8
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #40
	strh	r3, [r2, #0]
	ldr	r7, [pc, #96]
	ldr	r3, [pc, #36]
	movs	r4, #0
	adds	r2, #2
	strh	r3, [r2, #0]
	str	r7, [sp, #20]
	str	r4, [sp, #16]
	mov	fp, r4
.L_08182d94:
	ldr	r3, [pc, #84]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08182e04
	b.n	.L_08182df0
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x00003f44
	.4byte 0x00001010
	.4byte 0x000000a7
	.4byte 0x03000730
	.4byte 0x05000200
	.4byte 0x02010000
	.4byte 0x020036e0
	.4byte 0x80002000
	.4byte 0x06010000
	.4byte 0x000000fb
	.4byte 0x000000fc
	.4byte 0x000000fd
	.4byte 0x000000c2
	.4byte 0x020038e0
	.4byte 0x04000208
	.4byte 0x030011e0
	.4byte 0xde500000
	.2byte 0x1150
	.2byte 0x0300
.L_08182df0:
	movs	r0, #163
	lsls	r0, r0, #1
	adds	r0, #255
	cmp	fp, r0
	bgt.n	.L_08182e04
	ldr	r1, [sp, #52]
	cmp	r1, #0
	bne.n	.L_08182e04
	movs	r2, #1
	str	r2, [sp, #52]
.L_08182e04:
	ldr	r3, [sp, #52]
	cmp	r3, #0
	ble.n	.L_08182e44
	ldr	r0, [sp, #52]
	lsls	r3, r3, #8
	adds	r3, r3, r0
	ldr	r4, [sp, #72]
	lsls	r2, r3, #16
	movs	r7, #238
	adds	r3, r3, r2
	lsls	r7, r7, #7
	ldr	r2, [pc, #436]
	adds	r7, #132
	adds	r1, r4, r7
	lsls	r3, r3, #2
	str	r3, [r1, #0]
	cmp	r3, r2
	ble.n	.L_08182e2a
	str	r2, [r1, #0]
.L_08182e2a:
	ldr	r1, [sp, #72]
	movs	r3, #239
	lsls	r3, r3, #7
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r4, [sp, #52]
	adds	r4, #1
	str	r4, [sp, #52]
	cmp	r4, #19
	bne.n	.L_08182e44
	bl	.L_08183ff0
.L_08182e44:
	mov	r7, fp
	cmp	r7, #0
	bne.n	.L_08182ea2
	ldr	r3, [pc, #392]
	movs	r0, #0
	movs	r1, #1
	movs	r2, #192
	mov	r8, r0
	negs	r1, r1
	lsls	r2, r2, #1
.L_08182e58:
	movs	r4, #1
	add	r8, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_08182e58
	ldr	r3, [sp, #72]
	movs	r7, #0
	movs	r2, #1
	mov	r8, r7
	negs	r2, r2
	adds	r3, #24
.L_08182e70:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r1, #64
	bne.n	.L_08182e70
	ldr	r4, [sp, #72]
	movs	r2, #176
	movs	r3, #144
	movs	r7, #239
	movs	r0, #238
	lsls	r2, r2, #15
	lsls	r3, r3, #14
	lsls	r7, r7, #7
	lsls	r0, r0, #7
	str	r2, [sp, #56]
	str	r3, [sp, #60]
	adds	r2, r4, r7
	movs	r3, #2
	adds	r0, #132
	str	r3, [r2, #0]
	adds	r2, r4, r0
	movs	r3, #50
	str	r3, [r2, #0]
.L_08182ea2:
	mov	r1, fp
	cmp	r1, #82
	bne.n	.L_08182ebc
	movs	r1, #128
	ldr	r3, [pc, #300]
	ldr	r0, [sp, #76]
	lsls	r1, r1, #7
	ldr	r2, [pc, #296]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2082
	bl	sub_081c0010
.L_08182ebc:
	mov	r2, fp
	cmp	r2, #210
	bne.n	.L_08182ed6
	movs	r1, #128
	ldr	r3, [pc, #272]
	ldr	r0, [sp, #76]
	lsls	r1, r1, #7
	ldr	r2, [pc, #272]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2082
	bl	sub_081c0010
.L_08182ed6:
	movs	r3, #137
	lsls	r3, r3, #1
	cmp	fp, r3
	bne.n	.L_08182ee4
	movs	r0, #139
	bl	sub_081c0010
.L_08182ee4:
	movs	r4, #155
	lsls	r4, r4, #1
	cmp	fp, r4
	bne.n	.L_08182ef2
	movs	r0, #206
	bl	sub_081c0010
.L_08182ef2:
	movs	r7, #158
	lsls	r7, r7, #1
	cmp	fp, r7
	bne.n	.L_08182f00
	movs	r0, #154
	bl	sub_081c0010
.L_08182f00:
	movs	r0, #233
	lsls	r0, r0, #1
	cmp	fp, r0
	bne.n	.L_08182f0e
	movs	r0, #142
	bl	sub_081c0010
.L_08182f0e:
	movs	r1, #249
	lsls	r1, r1, #1
	cmp	fp, r1
	bne.n	.L_08182f1c
	movs	r0, #154
	bl	sub_081c0010
.L_08182f1c:
	movs	r2, #248
	adds	r2, #255
	cmp	fp, r2
	bne.n	.L_08182f2a
	movs	r0, #154
	bl	sub_081c0010
.L_08182f2a:
	movs	r3, #254
	lsls	r3, r3, #1
	cmp	fp, r3
	bne.n	.L_08182f38
	movs	r0, #154
	bl	sub_081c0010
.L_08182f38:
	movs	r4, #130
	lsls	r4, r4, #2
	cmp	fp, r4
	bne.n	.L_08182f46
	movs	r0, #212
	bl	sub_081c0010
.L_08182f46:
	movs	r7, #135
	lsls	r7, r7, #2
	cmp	fp, r7
	bne.n	.L_08182f54
	movs	r0, #186
	bl	sub_081c0010
.L_08182f54:
	ldr	r0, [pc, #136]
	cmp	fp, r0
	ble.n	.L_08182f5c
	b.n	.L_08183094
.L_08182f5c:
	ldr	r1, [sp, #72]
	movs	r2, #236
	lsls	r2, r2, #7
	adds	r2, #64
	adds	r1, r1, r2
	mov	r3, fp
	mov	sl, r1
	cmp	r3, #0
	bge.n	.L_08182f70
	adds	r3, #31
.L_08182f70:
	asrs	r3, r3, #5
	adds	r4, r3, #0
	subs	r4, #8
	cmp	r4, #8
	ble.n	.L_08182f7c
	movs	r4, #8
.L_08182f7c:
	movs	r3, #0
	mov	r8, r3
.L_08182f80:
	movs	r1, #1
	add	r8, r1
	mov	r7, sl
	movs	r0, #2
	mov	r2, r8
	strh	r3, [r7, #0]
	add	sl, r0
	cmp	r2, #15
	bne.n	.L_08182f80
	movs	r3, #142
	lsls	r3, r3, #1
	adds	r3, #255
	cmp	fp, r3
	ble.n	.L_08182fac
	ldr	r3, [pc, #68]
	add	r3, fp
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r4, r3, #0
	adds	r4, #8
	b.n	.L_0818300a
.L_08182fac:
	movs	r7, #56
	adds	r7, #255
	cmp	fp, r7
	ble.n	.L_08182ff0
	ldr	r2, [pc, #48]
	add	r2, fp
	cmp	r2, #0
	bge.n	.L_08182fc0
	ldr	r2, [pc, #44]
	add	r2, fp
.L_08182fc0:
	asrs	r2, r2, #2
	movs	r3, #14
	subs	r4, r3, r2
	cmp	r4, #7
	bgt.n	.L_0818300a
	movs	r4, #8
	b.n	.L_0818300a
	movs	r0, r0
	.4byte 0x3f3f3f3f
	.4byte 0x02015018
	.4byte 0x03000260
	.4byte 0x2f2f2f2f
	.4byte 0x001e847f
	.4byte 0xfffffde4
	.4byte 0xfffffec8
	.2byte 0xfecb
	.2byte 0xffff
.L_08182ff0:
	movs	r0, #44
	adds	r0, #255
	cmp	fp, r0
	ble.n	.L_0818300a
	ldr	r3, [pc, #188]
	add	r3, fp
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r4, r4, r3
	cmp	r4, #14
	ble.n	.L_0818300a
	movs	r4, #14
.L_0818300a:
	movs	r1, #15
	mov	r8, r1
.L_0818300e:
	mov	r6, r8
	subs	r6, #15
	adds	r0, r6, #0
	movs	r1, #15
	str	r4, [sp, #12]
	bl	sub_08002054
	ldr	r4, [sp, #12]
	adds	r3, r6, #0
	subs	r0, r4, r0
	adds	r7, r0, #0
	adds	r7, #32
	cmp	r6, #0
	bge.n	.L_0818302c
	mov	r3, r8
.L_0818302c:
	asrs	r3, r3, #4
	subs	r3, r4, r3
	movs	r1, #17
	adds	r0, r6, #0
	adds	r5, r3, #0
	str	r4, [sp, #12]
	bl	sub_08002054
	ldr	r4, [sp, #12]
	adds	r5, #16
	subs	r1, r4, r0
	cmp	r7, #0
	bge.n	.L_08183048
	movs	r7, #0
.L_08183048:
	cmp	r7, #31
	ble.n	.L_0818304e
	movs	r7, #31
.L_0818304e:
	cmp	r5, #0
	bge.n	.L_08183054
	movs	r5, #0
.L_08183054:
	cmp	r5, #31
	ble.n	.L_0818305a
	movs	r5, #31
.L_0818305a:
	cmp	r1, #0
	bge.n	.L_08183060
	movs	r1, #0
.L_08183060:
	cmp	r1, #31
	ble.n	.L_08183066
	movs	r1, #31
.L_08183066:
	lsls	r3, r7, #10
	lsls	r2, r5, #5
	orrs	r3, r2
	movs	r7, #1
	orrs	r3, r1
	mov	r2, sl
	add	r8, r7
	strh	r3, [r2, #0]
	mov	r0, r8
	movs	r3, #2
	add	sl, r3
	cmp	r0, #135
	bne.n	.L_0818300e
	ldr	r3, [pc, #48]
.L_08183082:
	movs	r4, #1
	add	r8, r4
	mov	r1, sl
	movs	r2, #2
	mov	r7, r8
	strh	r3, [r1, #0]
	add	sl, r2
	cmp	r7, #160
	bne.n	.L_08183082
.L_08183094:
	ldr	r2, [pc, #36]
	add	r2, fp
	cmp	r2, #17
	bhi.n	.L_081830e4
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	mov	r2, fp
	adds	r3, #14
	ldr	r1, [pc, #24]
	subs	r2, #253
	cmp	r3, #31
	ble.n	.L_081830c4
	movs	r3, #31
	b.n	.L_081830c4
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0xfffffed4
	.4byte 0xfffffee6
	.2byte 0x0200
	.2byte 0x0500
.L_081830c4:
	cmp	r2, #31
	ble.n	.L_081830ca
	movs	r2, #31
.L_081830ca:
	lsls	r3, r3, #5
	lsls	r2, r2, #10
	movs	r0, #0
	orrs	r2, r3
	movs	r3, #128
	mov	r8, r0
	lsls	r3, r3, #1
.L_081830d8:
	movs	r4, #1
	add	r8, r4
	strh	r2, [r1, #0]
	adds	r1, #2
	cmp	r8, r3
	bne.n	.L_081830d8
.L_081830e4:
	movs	r7, #44
	adds	r7, #255
	cmp	fp, r7
	bgt.n	.L_081830ee
	b.n	.L_081831f6
.L_081830ee:
	ldr	r0, [pc, #72]
	bl	sub_08013300
	ldr	r1, [pc, #68]
	movs	r4, #60
	movs	r2, #31
	movs	r3, #15
	adds	r4, #255
	mov	lr, r1
	str	r2, [sp, #48]
	str	r2, [sp, #44]
	mov	sl, r3
	cmp	fp, r4
	ble.n	.L_08183116
	movs	r7, #4
	movs	r1, #12
	str	r7, [sp, #48]
	str	r1, [sp, #44]
	movs	r2, #16
	mov	sl, r2
.L_08183116:
	movs	r3, #82
	adds	r3, #255
	cmp	fp, r3
	ble.n	.L_08183126
	movs	r4, #0
	str	r4, [sp, #48]
	str	r4, [sp, #44]
	mov	sl, r4
.L_08183126:
	ldr	r1, [pc, #12]
	movs	r7, #0
	movs	r2, #31
	mov	r8, r7
	mov	ip, r1
	mov	r9, r2
	b.n	.L_08183140
	.4byte 0x0000001f
	.4byte 0x000000a7
	.2byte 0x0200
	.2byte 0x0500
.L_08183140:
	mov	r3, lr
	ldrh	r2, [r3, #0]
	ldrh	r1, [r0, #0]
	lsls	r3, r2, #16
	mov	r5, r9
	lsrs	r6, r3, #26
	mov	r4, ip
	ands	r5, r2
	lsls	r2, r1, #16
	mov	r7, ip
	ands	r6, r4
	lsrs	r4, r3, #21
	lsrs	r3, r2, #26
	str	r2, [sp, #8]
	ands	r3, r7
	mov	r2, sl
	ands	r4, r7
	adds	r7, r3, r2
	ldr	r3, [sp, #8]
	lsrs	r2, r3, #21
	mov	r3, ip
	ands	r2, r3
	ldr	r3, [sp, #44]
	adds	r2, r2, r3
	str	r2, [sp, #8]
	ldr	r2, [sp, #48]
	mov	r3, r9
	ands	r3, r1
	adds	r1, r3, r2
	subs	r3, r7, #1
	cmp	r6, r3
	bge.n	.L_08183184
	adds	r6, #2
	b.n	.L_0818318c
.L_08183184:
	adds	r3, r7, #1
	cmp	r6, r3
	ble.n	.L_0818318c
	subs	r6, #2
.L_0818318c:
	ldr	r3, [sp, #8]
	subs	r3, #1
	cmp	r4, r3
	bge.n	.L_08183198
	adds	r4, #2
	b.n	.L_081831a2
.L_08183198:
	ldr	r3, [sp, #8]
	adds	r3, #1
	cmp	r4, r3
	ble.n	.L_081831a2
	subs	r4, #2
.L_081831a2:
	subs	r3, r1, #1
	cmp	r5, r3
	bge.n	.L_081831ac
	adds	r5, #2
	b.n	.L_081831b4
.L_081831ac:
	adds	r3, r1, #1
	cmp	r5, r3
	ble.n	.L_081831b4
	subs	r5, #2
.L_081831b4:
	cmp	r6, #31
	ble.n	.L_081831ba
	movs	r6, #31
.L_081831ba:
	cmp	r4, #31
	ble.n	.L_081831c0
	movs	r4, #31
.L_081831c0:
	cmp	r5, #31
	ble.n	.L_081831c6
	movs	r5, #31
.L_081831c6:
	cmp	r6, #0
	bge.n	.L_081831cc
	movs	r6, #0
.L_081831cc:
	cmp	r4, #0
	bge.n	.L_081831d2
	movs	r4, #0
.L_081831d2:
	cmp	r5, #0
	bge.n	.L_081831d8
	movs	r5, #0
.L_081831d8:
	lsls	r2, r4, #5
	lsls	r3, r6, #10
	orrs	r3, r2
	movs	r7, #1
	movs	r2, #128
	mov	r4, lr
	orrs	r3, r5
	movs	r1, #2
	add	r8, r7
	lsls	r2, r2, #1
	strh	r3, [r4, #0]
	adds	r0, #2
	add	lr, r1
	cmp	r8, r2
	bne.n	.L_08183140
.L_081831f6:
	movs	r3, #132
	lsls	r3, r3, #1
	adds	r3, #255
	cmp	fp, r3
	ble.n	.L_0818327a
	ldr	r5, [pc, #68]
	ldr	r0, [pc, #72]
	add	r5, fp
	bl	sub_08013300
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	ldr	r6, [pc, #64]
	ldr	r7, [pc, #48]
	movs	r4, #0
	asrs	r3, r3, #1
	mov	r8, r4
	mov	ip, r3
.L_0818321a:
	ldrh	r3, [r0, #0]
	movs	r2, #31
	ands	r2, r3
	lsls	r3, r3, #16
	adds	r4, r2, r5
	lsrs	r2, r3, #21
	lsrs	r1, r3, #26
	ands	r2, r7
	adds	r3, r5, #0
	add	r2, ip
	ands	r1, r7
	cmp	r3, #0
	bge.n	.L_08183236
	adds	r3, #3
.L_08183236:
	asrs	r3, r3, #2
	adds	r3, r1, r3
	cmp	r4, #31
	ble.n	.L_08183254
	movs	r4, #31
	b.n	.L_08183254
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0xfffffdf8
	.4byte 0x000000a7
	.2byte 0x0200
	.2byte 0x0500
.L_08183254:
	cmp	r2, #31
	ble.n	.L_0818325a
	movs	r2, #31
.L_0818325a:
	cmp	r3, #31
	ble.n	.L_08183260
	movs	r3, #31
.L_08183260:
	lsls	r2, r2, #5
	lsls	r3, r3, #10
	orrs	r3, r2
	movs	r1, #1
	movs	r2, #128
	orrs	r3, r4
	add	r8, r1
	lsls	r2, r2, #1
	strh	r3, [r6, #0]
	adds	r0, #2
	adds	r6, #2
	cmp	r8, r2
	bne.n	.L_0818321a
.L_0818327a:
	ldr	r3, [pc, #772]
	add	r3, fp
	cmp	r3, #253
	bhi.n	.L_08183314
	ldr	r3, [pc, #768]
	add	r1, sp, #120
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r3, [sp, #100]
	str	r4, [sp, #104]
	movs	r3, #255
	lsls	r3, r3, #16
	movs	r4, #180
	str	r3, [r1, #4]
	lsls	r4, r4, #1
	movs	r3, #0
	str	r3, [r1, #12]
	cmp	fp, r4
	bne.n	.L_081832a6
	movs	r7, #144
	lsls	r7, r7, #14
	str	r7, [sp, #60]
.L_081832a6:
	ldr	r3, [pc, #736]
	add	r3, fp
	cmp	r3, #111
	bhi.n	.L_081832b8
	ldr	r0, [sp, #60]
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r2, r0, r2
	str	r2, [sp, #60]
.L_081832b8:
	movs	r3, #142
	lsls	r3, r3, #1
	adds	r3, #255
	cmp	fp, r3
	ble.n	.L_081832cc
	ldr	r4, [sp, #60]
	movs	r7, #128
	lsls	r7, r7, #13
	adds	r7, r4, r7
	str	r7, [sp, #60]
.L_081832cc:
	adds	r6, r1, #0
	movs	r2, #238
	ldr	r1, [sp, #72]
	lsls	r2, r2, #7
	movs	r0, #0
	adds	r2, #220
	mov	r8, r0
	add	r7, sp, #100
	adds	r5, r1, r2
.L_081832de:
	mov	r0, r8
	movs	r1, #3
	bl	sub_08002064
	ldr	r3, [sp, #56]
	lsls	r0, r0, #21
	adds	r0, r0, r3
	str	r0, [r6, #0]
	movs	r1, #3
	mov	r0, r8
	bl	sub_08002054
	ldr	r4, [sp, #60]
	lsls	r0, r0, #21
	adds	r0, r0, r4
	str	r0, [r6, #8]
	adds	r1, r6, #0
	ldmia	r5!, {r0}
	adds	r2, r7, #0
	movs	r3, #0
	bl	sub_08020010
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	cmp	r1, #12
	bne.n	.L_081832de
.L_08183314:
	ldr	r3, [pc, #628]
	add	r3, fp
	cmp	r3, #23
	bhi.n	.L_08183366
	ldr	r2, [pc, #624]
	lsls	r5, r3, #3
	subs	r5, r5, r3
	lsls	r5, r5, #2
	adds	r5, r5, r2
	bl	sub_08014878
	movs	r6, #15
	ands	r0, r6
	adds	r0, #52
	movs	r3, #200
	lsls	r3, r3, #15
	lsls	r0, r0, #16
	str	r3, [r5, #4]
	str	r0, [r5, #0]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	subs	r3, #16
	lsls	r3, r3, #12
	str	r3, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #8
	negs	r0, r0
	lsls	r0, r0, #12
	str	r0, [r5, #16]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #5
	str	r3, [r5, #8]
	movs	r3, #32
	str	r3, [r5, #24]
.L_08183366:
	movs	r3, #155
	lsls	r3, r3, #1
	cmp	fp, r3
	bne.n	.L_081833ca
	ldr	r5, [pc, #548]
	movs	r4, #0
	mov	r8, r4
	movs	r6, #31
.L_08183376:
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r3, #28
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r1, #80
	bl	sub_0800206c
	adds	r0, #56
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #16
	lsls	r0, r0, #12
	str	r0, [r5, #12]
	bl	sub_08014878
	movs	r3, #8
	ands	r0, r6
	subs	r3, r3, r0
	lsls	r3, r3, #12
	str	r3, [r5, #16]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	movs	r7, #1
	adds	r3, #5
	add	r8, r7
	str	r3, [r5, #8]
	mov	r0, r8
	movs	r3, #32
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r0, #80
	bne.n	.L_08183376
.L_081833ca:
	movs	r1, #0
	mov	sl, r1
.L_081833ce:
	ldr	r3, [pc, #456]
	mov	r4, sl
	lsls	r2, r4, #1
	ldrh	r3, [r3, r2]
	cmp	fp, r3
	bne.n	.L_08183466
	movs	r7, #0
	mov	r8, r7
	movs	r6, #3
.L_081833e0:
	movs	r1, #9
	mov	r0, sl
	bl	sub_08002064
	lsls	r3, r0, #3
	subs	r3, r3, r0
	add	r3, r8
	ldr	r0, [sp, #72]
	lsls	r5, r3, #3
	subs	r5, r5, r3
	lsls	r5, r5, #2
	adds	r5, r0, r5
	bl	sub_08014878
	ldr	r3, [pc, #412]
	mov	r2, sl
	ldrb	r1, [r3, r2]
	bl	sub_0800206c
	ldr	r3, [pc, #408]
	mov	r4, sl
	ldrb	r3, [r3, r4]
	mov	r7, sl
	adds	r0, r0, r3
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	sub_08014878
	ldr	r3, [pc, #392]
	ands	r0, r6
	ldrb	r3, [r3, r7]
	mov	r1, r8
	muls	r1, r3
	adds	r3, r1, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #384]
	ldrb	r3, [r3, r7]
	adds	r0, r0, r3
	ldr	r3, [pc, #380]
	lsls	r0, r0, #16
	ldrsb	r3, [r3, r7]
	str	r0, [r5, #4]
	lsls	r3, r3, #12
	str	r3, [r5, #12]
	ldr	r3, [pc, #372]
	ldrsb	r3, [r3, r7]
	lsls	r3, r3, #12
	str	r3, [r5, #16]
	bl	sub_08014878
	ldr	r3, [pc, #364]
	ands	r0, r6
	ldrsb	r3, [r3, r7]
	movs	r2, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #360]
	lsls	r0, r0, #16
	ldrsb	r3, [r3, r7]
	str	r0, [r5, #8]
	lsls	r3, r3, #10
	str	r3, [r5, #20]
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	str	r2, [r5, #24]
	cmp	r4, #7
	bne.n	.L_081833e0
.L_08183466:
	movs	r7, #1
	add	sl, r7
	mov	r0, sl
	cmp	r0, #9
	bne.n	.L_081833ce
	movs	r1, #104
	adds	r1, #255
	cmp	fp, r1
	bgt.n	.L_081834ea
	ldr	r3, [pc, #320]
	ldr	r7, [pc, #280]
	movs	r2, #0
	mov	r8, r2
	mov	sl, r3
.L_08183482:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	blt.n	.L_081834da
	ldr	r6, [r7, #8]
	ldr	r2, [pc, #308]
	lsls	r3, r6, #1
	ldrh	r1, [r2, r3]
	movs	r3, #2
	ldrsh	r2, [r7, r3]
	ldr	r3, [pc, #300]
	ldr	r4, [sp, #72]
	ldrb	r5, [r3, r6]
	movs	r0, #158
	adds	r1, r4, r1
	lsls	r0, r0, #7
	adds	r1, r1, r0
	lsrs	r3, r5, #1
	mov	r0, sl
	subs	r2, r2, r3
	movs	r4, #6
	ldrsh	r3, [r7, r4]
	ldrb	r4, [r0, r6]
	str	r5, [sp, #0]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #76]
	ldr	r4, [sp, #64]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c38
	ldr	r2, [pc, #260]
	movs	r1, #63
	bl	sub_08138086
	mov	r0, sl
	ldrb	r3, [r0, r6]
	ldr	r2, [r7, #4]
	lsls	r3, r3, #16
	cmn	r2, r3
	bge.n	.L_081834da
	movs	r3, #1
	negs	r3, r3
	str	r3, [r7, #24]
.L_081834da:
	movs	r1, #1
	movs	r2, #192
	add	r8, r1
	lsls	r2, r2, #1
	adds	r7, #28
	cmp	r8, r2
	bne.n	.L_08183482
	b.n	.L_08183820
.L_081834ea:
	bl	sub_08014de4
	movs	r2, #128
	movs	r0, #0
	movs	r1, #0
	lsls	r2, r2, #17
	bl	sub_08015160
	movs	r3, #142
	lsls	r3, r3, #1
	adds	r3, #255
	cmp	fp, r3
	ble.n	.L_0818351e
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #220
	mov	r0, fp
	muls	r0, r3
	ldr	r4, [pc, #188]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r0, r0, r4
	adds	r1, r2, #0
	bl	sub_080151e4
	b.n	.L_08183550
.L_0818351e:
	movs	r7, #131
	lsls	r7, r7, #1
	adds	r7, #255
	cmp	fp, r7
	ble.n	.L_08183536
	movs	r2, #128
	movs	r0, #0
	movs	r1, #0
	lsls	r2, r2, #9
	bl	sub_080151e4
	b.n	.L_08183550
.L_08183536:
	movs	r0, #230
	adds	r0, #255
	cmp	fp, r0
	ble.n	.L_08183550
	mov	r1, fp
	lsls	r3, r1, #11
	ldr	r1, [pc, #140]
	movs	r2, #128
	subs	r1, r1, r3
	adds	r0, r1, #0
	lsls	r2, r2, #9
	bl	sub_080151e4
.L_08183550:
	movs	r2, #142
	lsls	r2, r2, #1
	adds	r2, #255
	cmp	fp, r2
	ble.n	.L_08183564
	mov	r3, fp
	lsls	r0, r3, #10
	bl	sub_08015068
	b.n	.L_081835f4
.L_08183564:
	movs	r4, #131
	lsls	r4, r4, #1
	adds	r4, #255
	cmp	fp, r4
	ble.n	.L_081835d4
	movs	r0, #128
	lsls	r0, r0, #9
	bl	sub_08015068
	movs	r0, #128
	lsls	r0, r0, #8
	bl	sub_08015024
	b.n	.L_081835f4
	.4byte 0xfffffedc
	.4byte 0x08196ef0
	.4byte 0xfffffe74
	.4byte 0xfffffeee
	.4byte 0x020158c0
	.4byte 0x02015000
	.4byte 0x081996e8
	.4byte 0x0819970c
	.4byte 0x081996fa
	.4byte 0x08199715
	.4byte 0x08199703
	.4byte 0x0819971e
	.4byte 0x08199727
	.4byte 0x08199730
	.4byte 0x08199739
	.4byte 0x081996bb
	.4byte 0x081996ca
	.4byte 0x081996ac
	.4byte 0xffffc000
	.4byte 0xfff3b3f0
	.2byte 0x3000
	.2byte 0x0010
.L_081835d4:
	movs	r7, #198
	adds	r7, #255
	cmp	fp, r7
	ble.n	.L_081835f4
	ldr	r5, [pc, #448]
	add	r5, fp
	lsls	r5, r5, #10
	adds	r0, r5, #0
	bl	sub_08015068
	adds	r0, r5, #0
	bl	sub_08002096
	asrs	r0, r0, #3
	bl	sub_08015024
.L_081835f4:
	movs	r0, #216
	movs	r1, #177
	movs	r2, #216
	lsls	r0, r0, #8
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r0, #240
	adds	r1, #224
	adds	r2, #120
	bl	sub_080151e4
	movs	r0, #180
	lsls	r0, r0, #1
	cmp	fp, r0
	bne.n	.L_081836a6
	ldr	r1, [sp, #52]
	cmp	r1, #0
	bne.n	.L_08183632
	ldr	r3, [sp, #72]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r7, [sp, #72]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #132
	adds	r2, r7, r0
	movs	r3, #75
	str	r3, [r2, #0]
.L_08183632:
	ldr	r5, [pc, #368]
	movs	r1, #0
	mov	r8, r1
.L_08183638:
	bl	sub_08014878
	movs	r3, #3
	ands	r3, r0
	str	r3, [r5, #24]
	bl	sub_08014878
	movs	r1, #160
	bl	sub_0800206c
	subs	r0, #80
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	sub_08014878
	movs	r1, #160
	bl	sub_0800206c
	adds	r0, #48
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08014878
	movs	r1, #160
	bl	sub_0800206c
	subs	r0, #80
	movs	r2, #0
	lsls	r0, r0, #16
	str	r2, [r5, #12]
	str	r0, [r5, #8]
	bl	sub_08014878
	ldr	r3, [r5, #24]
	movs	r2, #63
	ands	r2, r0
	adds	r2, #32
	movs	r1, #16
	subs	r1, r1, r3
	negs	r2, r2
	movs	r3, #1
	lsls	r3, r1
	lsls	r2, r2, #10
	subs	r2, r2, r3
	str	r2, [r5, #16]
	bl	sub_08014878
	movs	r3, #1
	movs	r4, #192
	add	r8, r3
	lsls	r4, r4, #1
	str	r0, [r5, #20]
	adds	r5, #28
	cmp	r8, r4
	bne.n	.L_08183638
.L_081836a6:
	movs	r7, #135
	lsls	r7, r7, #2
	cmp	fp, r7
	bne.n	.L_081836d6
	ldr	r5, [pc, #244]
	movs	r0, #0
	mov	r8, r0
	movs	r6, #63
.L_081836b6:
	movs	r1, #0
	str	r1, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #32
	negs	r0, r0
	movs	r2, #1
	movs	r3, #192
	lsls	r0, r0, #15
	add	r8, r2
	lsls	r3, r3, #1
	str	r0, [r5, #16]
	adds	r5, #28
	cmp	r8, r3
	bne.n	.L_081836b6
.L_081836d6:
	ldr	r7, [pc, #204]
	movs	r4, #0
	mov	r8, r4
.L_081836dc:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	bge.n	.L_081836e4
	b.n	.L_08183812
.L_081836e4:
	add	r6, sp, #108
	adds	r1, r6, #0
	adds	r0, r7, #0
	bl	sub_0815e1ec
	movs	r0, #10
	ldrsh	r3, [r6, r0]
	movs	r1, #50
	subs	r3, #228
	lsls	r0, r3, #1
	adds	r0, r0, r3
	str	r3, [r6, #8]
	lsls	r0, r0, #1
	bl	sub_08002054
	ldr	r2, [pc, #164]
	ldr	r3, [r7, #24]
	ldrb	r3, [r2, r3]
	adds	r0, r0, r3
	cmp	r0, #0
	bge.n	.L_08183710
	movs	r0, #0
.L_08183710:
	cmp	r0, #14
	ble.n	.L_08183716
	movs	r0, #14
.L_08183716:
	movs	r1, #2
	ldrsh	r3, [r6, r1]
	movs	r4, #6
	ldrsh	r2, [r6, r4]
	movs	r1, #142
	lsls	r1, r1, #1
	adds	r3, #60
	adds	r1, #255
	str	r2, [r6, #4]
	str	r3, [r6, #0]
	cmp	fp, r1
	ble.n	.L_0818373a
	mov	r4, fp
	ldr	r1, [pc, #120]
	lsls	r3, r4, #4
	adds	r3, r2, r3
	adds	r3, r3, r1
	b.n	.L_08183764
.L_0818373a:
	movs	r3, #131
	lsls	r3, r3, #1
	adds	r3, #255
	cmp	fp, r3
	ble.n	.L_0818374a
	adds	r3, r2, #0
	adds	r3, #16
	b.n	.L_08183764
.L_0818374a:
	movs	r4, #198
	adds	r4, #255
	cmp	fp, r4
	bgt.n	.L_08183758
	adds	r3, r2, #0
	adds	r3, #80
	b.n	.L_08183764
.L_08183758:
	mov	r1, fp
	subs	r3, r2, r1
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #22
	adds	r3, r3, r2
.L_08183764:
	str	r3, [r6, #4]
	ldr	r3, [r7, #24]
	cmp	r3, #1
	bgt.n	.L_081837bc
	ldr	r2, [pc, #64]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #72]
	ldr	r2, [r6, #0]
	adds	r1, r3, r1
	ldr	r3, [pc, #56]
	movs	r4, #158
	ldrb	r5, [r3, r0]
	lsls	r4, r4, #7
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #48]
	adds	r1, r1, r4
	ldrb	r4, [r3, r0]
	ldr	r3, [r6, #4]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r4, [sp, #4]
	str	r5, [sp, #0]
	ldr	r0, [sp, #76]
	ldr	r4, [sp, #64]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe027
	movs	r0, r0
	.4byte 0xfffffe3a
	.4byte 0x02015000
	.4byte 0x08199742
	.4byte 0xffffde60
	.4byte 0x081996ca
	.4byte 0x081996ac
	.2byte 0x96bb
	.2byte 0x0819
.L_081837bc:
	ldr	r2, [pc, #888]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #72]
	movs	r3, #180
	lsls	r3, r3, #7
	adds	r1, r2, r1
	adds	r3, #184
	adds	r1, r1, r3
	ldr	r3, [pc, #876]
	ldr	r2, [r6, #0]
	ldrb	r5, [r3, r0]
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #868]
	ldrb	r4, [r3, r0]
	ldr	r3, [r6, #4]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r4, [sp, #4]
	str	r5, [sp, #0]
	ldr	r0, [sp, #76]
	ldr	r4, [sp, #64]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20c6
	adds	r0, #255
	cmp	fp, r0
	ble.n	.L_0818380a
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_081837fe
	adds	r3, #15
.L_081837fe:
	movs	r1, #135
	asrs	r3, r3, #4
	lsls	r1, r1, #2
	adds	r3, r3, r1
	cmp	fp, r3
	blt.n	.L_08183812
.L_0818380a:
	ldr	r3, [r7, #4]
	ldr	r2, [r7, #16]
	adds	r3, r3, r2
	str	r3, [r7, #4]
.L_08183812:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r7, #28
	cmp	r3, #80
	beq.n	.L_08183820
	b.n	.L_081836dc
.L_08183820:
	movs	r4, #0
	str	r4, [sp, #24]
	mov	sl, r4
.L_08183826:
	ldr	r1, [sp, #24]
	movs	r7, #0
	lsls	r3, r1, #3
	ldr	r2, [sp, #72]
	subs	r3, r3, r1
	str	r7, [sp, #40]
	movs	r0, #0
	lsls	r3, r3, #2
	mov	r8, r0
	adds	r6, r3, r2
.L_0818383a:
	movs	r3, #196
	mov	r4, sl
	muls	r4, r3
	ldr	r7, [sp, #72]
	adds	r3, r4, #0
	adds	r3, #192
	ldr	r3, [r7, r3]
	cmp	r3, #0
	blt.n	.L_081838ee
	movs	r0, #10
	ldrsh	r2, [r6, r0]
	movs	r3, #14
	movs	r1, #7
	mov	r0, r8
	subs	r7, r3, r2
	bl	sub_08002064
	mov	r1, fp
	lsls	r0, r0, #3
	subs	r0, r1, r0
	lsls	r0, r0, #10
	bl	sub_08002096
	ldr	r3, [r6, #0]
	lsls	r0, r0, #2
	adds	r3, r3, r0
	mov	r2, r8
	asrs	r0, r3, #16
	cmp	r2, #0
	bne.n	.L_0818387a
	ldr	r3, [r6, #4]
	str	r3, [sp, #40]
.L_0818387a:
	cmp	r7, #0
	bge.n	.L_08183880
	movs	r7, #0
.L_08183880:
	ldr	r2, [pc, #692]
	lsls	r3, r7, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #692]
	ldr	r4, [sp, #72]
	ldrb	r5, [r3, r7]
	movs	r2, #180
	lsls	r2, r2, #7
	adds	r1, r4, r1
	adds	r2, #184
	adds	r1, r1, r2
	lsrs	r2, r5, #1
	ldr	r4, [sp, #40]
	subs	r2, r0, r2
	ldr	r0, [pc, #672]
	asrs	r3, r4, #16
	ldrb	r4, [r0, r7]
	mov	r9, r0
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r4, [sp, #4]
	str	r5, [sp, #0]
	ldr	r0, [sp, #76]
	ldr	r4, [sp, #64]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c30
	movs	r1, #64
	movs	r2, #0
	bl	sub_08138058
	ldr	r3, [r6, #8]
	cmp	r3, #0
	bge.n	.L_081838c8
	movs	r0, #0
	str	r0, [r6, #8]
.L_081838c8:
	mov	r1, r8
	cmp	r1, #6
	bne.n	.L_081838e0
	mov	r3, r9
	ldrb	r2, [r3, r7]
	ldr	r3, [r6, #4]
	lsls	r2, r2, #16
	cmn	r3, r2
	bge.n	.L_081838e0
	movs	r3, #1
	negs	r3, r3
	str	r3, [r6, #24]
.L_081838e0:
	ldr	r3, [r6, #8]
	ldr	r4, [sp, #40]
	movs	r7, #128
	adds	r3, r4, r3
	lsls	r7, r7, #12
	adds	r7, r3, r7
	str	r7, [sp, #40]
.L_081838ee:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r6, #28
	cmp	r1, #7
	bne.n	.L_0818383a
	ldr	r2, [sp, #24]
	add	sl, r0
	adds	r2, #7
	mov	r3, sl
	str	r2, [sp, #24]
	cmp	r3, #9
	bne.n	.L_08183826
	movs	r0, #128
	lsls	r0, r0, #2
	bl	sub_08014dac
	str	r0, [sp, #36]
	movs	r0, #1
	bl	sub_081969f8
	mov	r4, r8
	add	r3, sp, #92
	strb	r4, [r3, #0]
	str	r3, [r0, #16]
	ldr	r3, [pc, #544]
	movs	r7, #93
	add	r7, sp
	strb	r4, [r7, #0]
	str	r3, [r0, #8]
	ldr	r1, [sp, #36]
	mov	r2, fp
	subs	r2, #210
	str	r1, [r0, #12]
	mov	sl, r0
	str	r2, [sp, #32]
	cmp	r2, #107
	bls.n	.L_0818393c
	b.n	.L_08183a5c
.L_0818393c:
	mov	r4, fp
	lsls	r3, r4, #1
	ldr	r0, [pc, #516]
	add	r3, fp
	lsls	r3, r3, #13
	movs	r1, #250
	movs	r2, #192
	lsls	r1, r1, #4
	adds	r7, r3, r0
	lsls	r2, r2, #13
	mov	r8, r1
	cmp	r7, r2
	ble.n	.L_0818395a
	movs	r7, #192
	lsls	r7, r7, #13
.L_0818395a:
	movs	r3, #178
	lsls	r3, r3, #1
	adds	r3, #255
	mov	r4, fp
	muls	r4, r3
	ldr	r0, [pc, #484]
	movs	r1, #216
	adds	r3, r4, #0
	lsls	r1, r1, #8
	adds	r6, r3, r0
	adds	r1, #240
	cmp	r6, r1
	ble.n	.L_0818397a
	movs	r6, #216
	lsls	r6, r6, #8
	adds	r6, #240
.L_0818397a:
	movs	r2, #18
	adds	r2, #255
	cmp	fp, r2
	ble.n	.L_081839d8
	ldr	r0, [pc, #460]
	mov	r4, fp
	lsls	r3, r4, #15
	movs	r1, #128
	adds	r7, r3, r0
	lsls	r1, r1, #14
	cmp	r7, r1
	ble.n	.L_08183996
	movs	r7, #128
	lsls	r7, r7, #14
.L_08183996:
	mov	r3, fp
	lsls	r2, r3, #6
	movs	r3, #168
	lsls	r3, r3, #7
	adds	r3, #32
	movs	r4, #176
	subs	r3, r3, r2
	lsls	r4, r4, #4
	mov	r8, r3
	adds	r4, #183
	cmp	r8, r4
	bgt.n	.L_081839b6
	movs	r0, #176
	lsls	r0, r0, #4
	adds	r0, #184
	mov	r8, r0
.L_081839b6:
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r3, #83
	mov	r1, fp
	muls	r1, r3
	ldr	r2, [pc, #400]
	adds	r3, r1, #0
	adds	r3, r6, r3
	adds	r6, r3, r2
	movs	r3, #167
	lsls	r3, r3, #9
	adds	r3, #32
	cmp	r6, r3
	ble.n	.L_081839d8
	movs	r6, #167
	lsls	r6, r6, #9
	adds	r6, #32
.L_081839d8:
	movs	r3, #6
	mov	r4, sl
	str	r3, [r4, #0]
	ldr	r0, [sp, #72]
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r4, [pc, #368]
	adds	r3, r0, r1
	mov	r2, fp
	str	r3, [sp, #96]
	lsls	r3, r2, #3
	adds	r5, r3, r4
	cmp	r5, #0
	ble.n	.L_081839f6
	movs	r5, #0
.L_081839f6:
	movs	r0, #54
	adds	r0, #255
	cmp	fp, r0
	ble.n	.L_08183a08
	movs	r3, #155
	lsls	r3, r3, #1
	mov	r1, fp
	subs	r3, r3, r1
	lsls	r5, r3, #3
.L_08183a08:
	mov	r2, sl
	str	r5, [r2, #20]
	bl	sub_08014de4
	movs	r0, #0
	adds	r1, r7, #0
	movs	r2, #0
	bl	sub_08015160
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r1, #128
	adds	r0, r2, #0
	lsls	r1, r1, #9
	bl	sub_080151e4
	adds	r0, r6, #0
	bl	sub_0801521c
	movs	r3, #18
	adds	r3, #255
	cmp	fp, r3
	bgt.n	.L_08183a40
	ldr	r4, [sp, #16]
	ldr	r7, [pc, #288]
	adds	r0, r4, r7
	bl	sub_080150e4
.L_08183a40:
	mov	r0, r8
	bl	sub_08015024
	ldr	r0, [sp, #16]
	bl	sub_08015068
	ldr	r0, [pc, #272]
	ldr	r1, [sp, #36]
	movs	r2, #64
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_08183a5c:
	ldr	r0, [pc, #260]
	add	r0, fp
	cmp	r0, #7
	bhi.n	.L_08183a90
	ldr	r1, [sp, #52]
	cmp	r1, #0
	bne.n	.L_08183a90
	ldr	r3, [sp, #72]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r7, [sp, #72]
	movs	r2, #238
	lsls	r2, r2, #7
	lsls	r3, r0, #8
	adds	r2, #132
	adds	r3, r3, r0
	adds	r1, r7, r2
	lsls	r2, r3, #16
	adds	r3, r3, r2
	ldr	r2, [pc, #220]
	lsls	r3, r3, #2
	subs	r2, r2, r3
	str	r2, [r1, #0]
.L_08183a90:
	movs	r3, #26
	adds	r3, #255
	cmp	fp, r3
	bne.n	.L_08183ab6
	ldr	r4, [sp, #52]
	cmp	r4, #0
	bne.n	.L_08183ab6
	ldr	r7, [sp, #72]
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	adds	r2, r7, r0
	movs	r3, #2
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r2, r7, r1
	movs	r3, #50
	str	r3, [r2, #0]
.L_08183ab6:
	ldr	r3, [pc, #180]
	add	r3, fp
	cmp	r3, #27
	bhi.n	.L_08183b1a
	ldr	r4, [pc, #176]
	mov	r2, fp
	ldr	r7, [pc, #176]
	lsls	r3, r2, #11
	movs	r1, #38
	adds	r6, r3, r4
	mov	r0, sl
	movs	r3, #7
	adds	r1, #255
	str	r7, [sp, #96]
	str	r3, [r0, #0]
	movs	r5, #0
	cmp	fp, r1
	ble.n	.L_08183ae2
	movs	r3, #147
	lsls	r3, r3, #1
	subs	r3, r3, r2
	lsls	r5, r3, #3
.L_08183ae2:
	mov	r2, sl
	str	r5, [r2, #20]
	bl	sub_08014de4
	movs	r1, #224
	lsls	r1, r1, #13
	movs	r2, #0
	movs	r0, #0
	bl	sub_08015160
	adds	r0, r6, #0
	bl	sub_0801521c
	movs	r0, #250
	lsls	r0, r0, #4
	bl	sub_08015024
	ldr	r0, [sp, #16]
	bl	sub_08015068
	ldr	r0, [pc, #84]
	ldr	r1, [sp, #36]
	movs	r2, #64
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_08183b1a:
	mov	r3, fp
	cmp	r3, #80
	bgt.n	.L_08183b22
	b.n	.L_08183d52
.L_08183b22:
	ldr	r7, [sp, #20]
	movs	r4, #0
	mov	r8, r4
	mov	r9, r7
.L_08183b2a:
	ldr	r2, [pc, #76]
	mov	r0, r8
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	cmp	fp, r1
	ble.n	.L_08183bf0
	b.n	.L_08183b7c
	.4byte 0x081996ca
	.4byte 0x081996ac
	.4byte 0x081996bb
	.4byte 0x08199364
	.4byte 0xffb14000
	.4byte 0xfffe4aca
	.4byte 0xff8f0000
	.4byte 0xfff8292a
	.4byte 0xfffff930
	.4byte 0xfffedc00
	.4byte 0x08199210
	.4byte 0xfffffeef
	.4byte 0x3f3f3f3f
	.4byte 0xfffffeee
	.4byte 0xfff7b000
	.4byte 0x02010000
	.2byte 0x9746
	.2byte 0x0819
.L_08183b7c:
	adds	r3, r1, #0
	adds	r3, #10
	cmp	fp, r3
	bge.n	.L_08183bf0
	ldr	r3, [pc, #800]
	movs	r5, #16
	mov	r4, sl
	str	r3, [sp, #96]
	negs	r5, r5
	movs	r3, #6
	mov	r2, fp
	str	r3, [r4, #0]
	str	r5, [r4, #20]
	subs	r7, r2, r1
	bl	sub_08014de4
	mov	r0, r8
	lsls	r6, r7, #13
	cmp	r0, #1
	ble.n	.L_08183bc6
	movs	r2, #142
	lsls	r2, r2, #1
	adds	r2, #255
	ldr	r1, [pc, #768]
	cmp	fp, r2
	ble.n	.L_08183bb2
	add	r1, r9
.L_08183bb2:
	ldr	r0, [pc, #764]
	movs	r2, #0
	bl	sub_08015160
	adds	r0, r6, #0
	adds	r1, r6, #0
	adds	r2, r6, #0
	bl	sub_080151e4
	b.n	.L_08183bd0
.L_08183bc6:
	lsls	r1, r7, #14
	adds	r0, r6, #0
	adds	r2, r6, #0
	bl	sub_080151e4
.L_08183bd0:
	movs	r0, #128
	lsls	r0, r0, #7
	bl	sub_08015024
	mov	r3, fp
	lsls	r0, r3, #9
	bl	sub_08015068
	ldr	r0, [pc, #720]
	ldr	r1, [sp, #36]
	movs	r2, #64
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_08183bf0:
	movs	r4, #1
	add	r8, r4
	mov	r7, r8
	cmp	r7, #4
	bne.n	.L_08183b2a
	mov	r0, fp
	cmp	r0, #80
	bgt.n	.L_08183c02
	b.n	.L_08183d52
.L_08183c02:
	ldr	r2, [pc, #692]
	movs	r1, #0
	mov	r8, r1
	mov	r9, r2
	movs	r7, #0
.L_08183c0c:
	mov	r4, r9
	ldrh	r3, [r7, r4]
	cmp	fp, r3
	bne.n	.L_08183c26
	movs	r1, #128
	ldr	r3, [pc, #676]
	ldr	r0, [sp, #76]
	lsls	r1, r1, #7
	ldr	r2, [pc, #672]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x464c
	ldrh	r3, [r7, r4]
.L_08183c26:
	cmp	fp, r3
	ble.n	.L_08183c9c
	adds	r2, r3, #0
	adds	r2, #10
	cmp	fp, r2
	bge.n	.L_08183c9c
	mov	r0, fp
	subs	r1, r0, r3
	movs	r3, #160
	lsls	r2, r1, #13
	lsls	r3, r3, #9
	subs	r6, r3, r2
	lsls	r1, r1, #4
	ldr	r2, [pc, #612]
	adds	r5, r1, #0
	movs	r3, #7
	mov	r4, sl
	subs	r5, #56
	str	r2, [sp, #96]
	str	r3, [r4, #0]
	cmp	r5, #0
	ble.n	.L_08183c54
	movs	r5, #0
.L_08183c54:
	mov	r0, sl
	str	r5, [r0, #20]
	bl	sub_08014de4
	ldr	r0, [pc, #592]
	ldr	r1, [pc, #588]
	movs	r2, #0
	bl	sub_08015160
	lsls	r1, r6, #1
	adds	r0, r6, #0
	adds	r2, r6, #0
	bl	sub_080151e4
	mov	r1, r8
	movs	r2, #128
	lsls	r0, r1, #14
	lsls	r2, r2, #6
	adds	r0, r0, r2
	bl	sub_080150e4
	ldr	r0, [pc, #580]
	bl	sub_08015024
	mov	r3, fp
	lsls	r0, r3, #9
	bl	sub_08015068
	ldr	r0, [pc, #548]
	ldr	r1, [sp, #36]
	movs	r2, #64
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_08183c9c:
	movs	r4, #1
	add	r8, r4
	mov	r0, r8
	adds	r7, #2
	cmp	r0, #3
	bne.n	.L_08183c0c
	mov	r1, fp
	cmp	r1, #80
	ble.n	.L_08183d52
	ldr	r3, [sp, #20]
	movs	r2, #0
	mov	r8, r2
	mov	r9, r3
.L_08183cb6:
	ldr	r2, [pc, #528]
	mov	r4, r8
	lsls	r3, r4, #1
	ldrh	r2, [r2, r3]
	cmp	fp, r2
	ble.n	.L_08183d48
	adds	r3, r2, #0
	adds	r3, #100
	cmp	fp, r3
	bge.n	.L_08183d48
	movs	r3, #176
	mov	r0, fp
	lsls	r3, r3, #4
	subs	r2, r0, r2
	adds	r3, #162
	movs	r1, #255
	adds	r6, r2, #0
	muls	r6, r3
	lsls	r1, r1, #8
	adds	r1, #255
	ldr	r7, [pc, #460]
	cmp	r6, r1
	ble.n	.L_08183ce8
	movs	r6, #128
	lsls	r6, r6, #9
.L_08183ce8:
	ldr	r2, [sp, #72]
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r3, r2, r4
	movs	r5, #16
	mov	r0, sl
	str	r3, [sp, #96]
	negs	r5, r5
	movs	r3, #6
	str	r3, [r0, #0]
	str	r5, [r0, #20]
	bl	sub_08014de4
	movs	r1, #142
	lsls	r1, r1, #1
	adds	r1, #255
	cmp	fp, r1
	ble.n	.L_08183d10
	ldr	r7, [pc, #412]
	add	r7, r9
.L_08183d10:
	ldr	r0, [pc, #412]
	adds	r1, r7, #0
	movs	r2, #0
	bl	sub_08015160
	lsrs	r1, r6, #31
	adds	r1, r6, r1
	asrs	r1, r1, #1
	adds	r2, r6, #0
	adds	r0, r6, #0
	bl	sub_080151e4
	movs	r0, #128
	lsls	r0, r0, #7
	bl	sub_08015024
	mov	r2, fp
	lsls	r0, r2, #11
	bl	sub_08015068
	ldr	r0, [pc, #376]
	ldr	r1, [sp, #36]
	movs	r2, #64
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_08183d48:
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	cmp	r4, #3
	bne.n	.L_08183cb6
.L_08183d52:
	add	r7, sp, #92
	adds	r2, r7, #0
	movs	r1, #7
	movs	r3, #4
	strb	r1, [r7, #0]
	strb	r3, [r2, #1]
	ldr	r0, [sp, #72]
	movs	r4, #142
	lsls	r4, r4, #7
	adds	r3, r0, r4
	ldr	r0, [pc, #356]
	mov	r7, sl
	str	r3, [r2, #4]
	str	r1, [r7, #0]
	str	r0, [r7, #8]
	ldr	r1, [sp, #36]
	movs	r4, #128
	movs	r2, #0
	movs	r3, #0
	lsls	r4, r4, #8
	str	r1, [r7, #12]
	strb	r2, [r7, #24]
	strb	r2, [r7, #25]
	mov	r8, r3
	mov	r9, r4
	movs	r7, #0
.L_08183d86:
	ldr	r0, [pc, #328]
	mov	r1, r8
	ldrb	r2, [r0, r1]
	cmp	fp, r2
	bge.n	.L_08183d92
	b.n	.L_08183f8c
.L_08183d92:
	movs	r3, #38
	adds	r3, #255
	cmp	fp, r3
	ble.n	.L_08183d9c
	b.n	.L_08183ef0
.L_08183d9c:
	ldr	r3, [pc, #308]
	mov	r4, fp
	ldrh	r1, [r3, r7]
	ldr	r3, [pc, #308]
	subs	r2, r4, r2
	ldrh	r4, [r3, r7]
	adds	r3, r4, #0
	muls	r3, r2
	adds	r6, r1, r3
	ldr	r3, [pc, #300]
	ldrh	r3, [r3, r7]
	cmp	r6, r3
	ble.n	.L_08183db8
	adds	r6, r3, #0
.L_08183db8:
	mov	r0, fp
	cmp	r0, #209
	ble.n	.L_08183dc6
	ldr	r1, [sp, #32]
	adds	r3, r4, #0
	muls	r3, r1
	subs	r6, r6, r3
.L_08183dc6:
	bl	sub_08014de4
	movs	r1, #128
	mov	r2, r9
	mov	r0, r9
	lsls	r1, r1, #9
	bl	sub_080151e4
	movs	r2, #18
	adds	r2, #255
	cmp	fp, r2
	ble.n	.L_08183de8
	mov	r4, fp
	ldr	r0, [pc, #252]
	lsls	r3, r4, #13
	adds	r3, r6, r3
	adds	r6, r3, r0
.L_08183de8:
	adds	r0, r6, #0
	bl	sub_0801521c
	mov	r1, r8
	cmp	r1, #0
	bne.n	.L_08183e20
	mov	r2, fp
	cmp	r2, #209
	ble.n	.L_08183e0c
	ldr	r1, [sp, #32]
	cmp	r1, #48
	ble.n	.L_08183e02
	movs	r1, #48
.L_08183e02:
	lsls	r1, r1, #16
	movs	r0, #0
	movs	r2, #0
	bl	sub_08015160
.L_08183e0c:
	ldr	r3, [pc, #212]
	mov	r4, fp
	ldrh	r0, [r3, r7]
	lsls	r3, r4, #8
	subs	r0, r0, r3
	movs	r5, #16
	bl	sub_080150e4
	negs	r5, r5
	b.n	.L_08183e74
.L_08183e20:
	movs	r0, #48
	mov	r1, fp
	negs	r0, r0
	cmp	r1, #127
	ble.n	.L_08183e2e
	mov	r0, fp
	subs	r0, #176
.L_08183e2e:
	movs	r6, #16
	negs	r6, r6
	cmp	r0, r6
	ble.n	.L_08183e38
	adds	r0, r6, #0
.L_08183e38:
	lsls	r0, r0, #16
	movs	r1, #0
	movs	r2, #0
	bl	sub_08015160
	ldr	r3, [pc, #160]
	mov	r2, fp
	ldrh	r0, [r3, r7]
	lsls	r5, r2, #8
	adds	r0, r0, r5
	bl	sub_080150e4
	mov	r3, fp
	cmp	r3, #209
	ble.n	.L_08183e5e
	ldr	r4, [pc, #144]
	adds	r0, r5, r4
	bl	sub_080150e4
.L_08183e5e:
	ldr	r0, [pc, #112]
	mov	r1, r8
	ldrb	r3, [r0, r1]
	mov	r2, fp
	subs	r3, r2, r3
	lsls	r3, r3, #2
	adds	r5, r3, #0
	subs	r5, #64
	cmp	r5, r6
	ble.n	.L_08183e74
	adds	r5, r6, #0
.L_08183e74:
	movs	r3, #18
	adds	r3, #255
	cmp	fp, r3
	ble.n	.L_08183f4c
	mov	r4, fp
	movs	r3, #135
	lsls	r2, r4, #2
	lsls	r3, r3, #3
	mov	r0, r8
	subs	r5, r3, r2
	cmp	r0, #0
	bne.n	.L_08183e9a
	movs	r0, #137
	lsls	r0, r0, #1
	subs	r0, r0, r4
	lsls	r0, r0, #10
	bl	sub_080150e4
	b.n	.L_08183f4c
.L_08183e9a:
	ldr	r1, [sp, #16]
	ldr	r2, [pc, #76]
	adds	r0, r1, r2
	bl	sub_080150e4
	b.n	.L_08183f4c
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0xffd40000
	.4byte 0xfffc0000
	.4byte 0x08199210
	.4byte 0x0819974e
	.4byte 0x03000260
	.4byte 0x2f2f2f2f
	.4byte 0xfffff000
	.4byte 0x08199754
	.4byte 0x02014800
	.4byte 0x08199756
	.4byte 0x08199758
	.4byte 0x08199760
	.4byte 0x08199764
	.4byte 0xffddc000
	.4byte 0x0819975c
	.4byte 0xffff2e00
	.2byte 0xdc00
	.2byte 0xfffd
.L_08183ef0:
	.2byte 0x465c
	mov	r0, r8
	ldr	r1, [pc, #848]
	lsls	r2, r0, #3
	lsls	r3, r4, #2
	subs	r3, r3, r2
	movs	r2, #16
	adds	r5, r3, r1
	negs	r2, r2
	cmp	r5, r2
	ble.n	.L_08183f0a
	movs	r5, #16
	negs	r5, r5
.L_08183f0a:
	movs	r3, #206
	lsls	r3, r3, #7
	adds	r3, #16
	mov	r4, r8
	muls	r4, r3
	mov	r0, fp
	lsls	r2, r0, #12
	adds	r3, r4, #0
	ldr	r1, [pc, #816]
	subs	r3, r3, r2
	movs	r2, #252
	lsls	r2, r2, #6
	adds	r6, r3, r1
	adds	r2, #255
	cmp	r6, r2
	ble.n	.L_08183f8c
	bl	sub_08014de4
	movs	r1, #128
	mov	r0, r9
	lsls	r1, r1, #9
	mov	r2, r9
	bl	sub_080151e4
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #14
	movs	r2, #0
	bl	sub_08015160
	adds	r0, r6, #0
	bl	sub_0801521c
.L_08183f4c:
	movs	r3, #64
	negs	r3, r3
	cmp	r5, r3
	ble.n	.L_08183f8c
	movs	r3, #82
	mov	r0, r8
	muls	r0, r3
	mov	r1, fp
	adds	r3, r0, #0
	subs	r3, r1, r3
	movs	r2, #22
	adds	r0, r3, #0
	muls	r0, r2
	ldr	r2, [pc, #744]
	mov	r4, sl
	adds	r0, r0, r2
	str	r5, [r4, #20]
	bl	sub_08015024
	mov	r3, fp
	negs	r0, r3
	lsls	r0, r0, #10
	bl	sub_08015068
	ldr	r0, [pc, #724]
	ldr	r1, [sp, #36]
	movs	r2, #64
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_08183f8c:
	movs	r4, #1
	add	r8, r4
	mov	r0, r8
	adds	r7, #2
	cmp	r0, #2
	beq.n	.L_08183f9a
	b.n	.L_08183d86
.L_08183f9a:
	mov	r0, sl
	bl	sub_08013164
	ldr	r0, [sp, #36]
	bl	sub_08013164
	movs	r3, #240
	ldr	r1, [sp, #72]
	lsls	r3, r3, #7
	adds	r3, #228
	adds	r2, r1, r3
.L_08183fb0:
	ldr	r3, [r2, #0]
	cmp	r3, #1
	bls.n	.L_08183fb0
	ldr	r4, [sp, #72]
	movs	r7, #240
	lsls	r7, r7, #7
	adds	r7, #232
	adds	r2, r4, r7
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #20]
	ldr	r2, [sp, #16]
	movs	r1, #128
	movs	r3, #128
	movs	r7, #128
	lsls	r1, r1, #13
	lsls	r3, r3, #2
	movs	r4, #1
	lsls	r7, r7, #2
	adds	r0, r0, r1
	adds	r2, r2, r3
	add	fp, r4
	adds	r7, #90
	str	r0, [sp, #20]
	str	r2, [sp, #16]
	cmp	fp, r7
	beq.n	.L_08183ff0
	bl	.L_08182d94
.L_08183ff0:
	movs	r1, #128
	ldr	r3, [pc, #612]
	lsls	r1, r1, #7
	ldr	r2, [pc, #612]
	ldr	r0, [pc, #612]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4999
	ldr	r0, [pc, #612]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_08184030
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #238
	adds	r3, r3, r1
	lsls	r2, r2, #7
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_08184030:
	strh	r4, [r0, #0]
	movs	r0, #0
	ldr	r1, [sp, #72]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #220
	mov	r8, r0
	adds	r5, r1, r2
.L_08184040:
	ldmia	r5!, {r0}
	bl	sub_08020048
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	cmp	r4, #12
	bne.n	.L_08184040
	bl	sub_08014c4c
	movs	r3, #128
	movs	r1, #160
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r1, r1, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #520]
	adds	r1, #160
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #512]
	movs	r2, #160
	ldrh	r3, [r3, #0]
	lsls	r2, r2, #19
	adds	r2, #188
	strh	r3, [r2, #0]
	ldr	r7, [sp, #72]
	movs	r0, #240
	lsls	r0, r0, #7
	adds	r0, #240
	adds	r3, r7, r0
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [pc, #484]
	bl	sub_08014644
	ldr	r2, [pc, #480]
	movs	r3, #120
	str	r3, [r2, #16]
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [pc, #472]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #468]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2180
	ldr	r3, [pc, #416]
	ldr	r0, [sp, #76]
	lsls	r1, r1, #7
	ldr	r2, [pc, #412]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2100
	movs	r2, #11
	movs	r0, #0
	bl	sub_08164b2c
	mov	r2, sp
	adds	r2, #84
	str	r2, [sp, #28]
	movs	r1, #0
	mov	fp, r1
.L_081840d6:
	mov	r3, fp
	cmp	r3, #16
	bne.n	.L_081840f4
	ldr	r4, [sp, #72]
	movs	r7, #239
	movs	r0, #238
	lsls	r7, r7, #7
	lsls	r0, r0, #7
	adds	r2, r4, r7
	movs	r3, #2
	adds	r0, #132
	str	r3, [r2, #0]
	adds	r2, r4, r0
	movs	r3, #75
	str	r3, [r2, #0]
.L_081840f4:
	mov	r1, fp
	cmp	r1, #52
	ble.n	.L_08184106
	movs	r2, #4
	negs	r2, r2
	adds	r0, r2, #0
	adds	r1, r2, #0
	bl	sub_08164a4c
.L_08184106:
	mov	r2, fp
	cmp	r2, #48
	ble.n	.L_0818411e
	mov	r3, fp
	subs	r3, #48
	negs	r1, r3
	movs	r2, #11
	lsls	r3, r3, #1
	subs	r2, r2, r3
	adds	r0, r1, #0
	bl	sub_08164b2c
.L_0818411e:
	mov	r3, fp
	cmp	r3, #0
	bne.n	.L_08184194
	ldr	r4, [sp, #72]
	movs	r7, #239
	lsls	r7, r7, #7
	adds	r2, r4, r7
	movs	r3, #3
	movs	r0, #238
	str	r3, [r2, #0]
	lsls	r0, r0, #7
	ldr	r3, [pc, #332]
	adds	r0, #132
	adds	r2, r4, r0
	str	r3, [r2, #0]
	ldr	r5, [pc, #328]
	movs	r1, #0
	mov	r8, r1
	movs	r6, #31
.L_08184144:
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	adds	r3, #120
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #16
	lsls	r0, r0, #13
	str	r0, [r5, #12]
	bl	sub_08014878
	movs	r3, #8
	ands	r0, r6
	subs	r3, r3, r0
	lsls	r3, r3, #12
	str	r3, [r5, #16]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #5
	str	r3, [r5, #8]
	movs	r2, #1
	movs	r3, #32
	str	r3, [r5, #24]
	add	r8, r2
	adds	r3, #224
	adds	r5, #28
	cmp	r8, r3
	bne.n	.L_08184144
.L_08184194:
	ldr	r7, [pc, #244]
	movs	r4, #0
	mov	sl, r7
	ldr	r7, [pc, #236]
	mov	r8, r4
.L_0818419e:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	blt.n	.L_081841f6
	ldr	r6, [r7, #8]
	ldr	r2, [pc, #232]
	lsls	r3, r6, #1
	ldrh	r1, [r2, r3]
	ldr	r0, [sp, #72]
	movs	r2, #158
	adds	r1, r0, r1
	lsls	r2, r2, #7
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r7, r3]
	ldr	r3, [pc, #216]
	mov	r0, sl
	ldrb	r5, [r3, r6]
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	movs	r4, #6
	ldrsh	r3, [r7, r4]
	ldrb	r4, [r0, r6]
	str	r5, [sp, #0]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #76]
	ldr	r4, [sp, #64]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c38
	ldr	r2, [pc, #184]
	movs	r1, #63
	bl	sub_08138086
	mov	r0, sl
	ldrb	r3, [r0, r6]
	ldr	r2, [r7, #4]
	lsls	r3, r3, #16
	cmn	r2, r3
	bge.n	.L_081841f6
	movs	r3, #1
	negs	r3, r3
	str	r3, [r7, #24]
.L_081841f6:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #1
	adds	r7, #28
	cmp	r8, r2
	bne.n	.L_0818419e
	ldr	r4, [sp, #80]
	movs	r3, #0
	ldr	r2, [r4, #20]
	mov	r8, r3
	cmp	r2, #0
	beq.n	.L_081842c2
	movs	r6, #16
	movs	r5, #36
.L_08184214:
	mov	r7, r8
	lsls	r3, r7, #3
	adds	r3, #18
	cmp	fp, r3
	bne.n	.L_081842b8
	cmp	r7, #0
	bne.n	.L_0818429c
	ldr	r2, [sp, #80]
	movs	r3, #0
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r2, #1
	negs	r2, r2
	movs	r1, #7
	str	r6, [sp, #0]
	bl	sub_0814cd48
	movs	r0, #126
	bl	sub_081180e8
	movs	r0, #126
	bl	sub_081c0010
	ldr	r3, [sp, #80]
	ldr	r2, [r3, #20]
	b.n	.L_081842b8
	.4byte 0xfffffb28
	.4byte 0x00138710
	.4byte 0xfffff448
	.4byte 0x02014400
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x06004000
	.4byte 0x020038e0
	.4byte 0x04000208
	.4byte 0x05000200
	.4byte 0x050001e8
	.4byte 0x08164bb5
	.4byte 0x030011e0
	.4byte 0x00000130
	.4byte 0x03000730
	.4byte 0x04040404
	.4byte 0x02015000
	.4byte 0x081996bb
	.4byte 0x081996ca
	.4byte 0x081996ac
	.2byte 0xc000
	.2byte 0xffff
.L_0818429c:
	.2byte 0x207e
	bl	sub_081c0010
	ldr	r4, [sp, #80]
	movs	r2, #1
	ldrsh	r0, [r5, r4]
	negs	r2, r2
	movs	r1, #7
	mov	r3, r8
	str	r6, [sp, #0]
	bl	sub_0814cd48
	ldr	r0, [sp, #80]
	ldr	r2, [r0, #20]
.L_081842b8:
	movs	r1, #1
	add	r8, r1
	adds	r5, #2
	cmp	r8, r2
	bne.n	.L_08184214
.L_081842c2:
	movs	r0, #128
	lsls	r0, r0, #2
	bl	sub_08014dac
	mov	r9, r0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r1, [sp, #28]
	adds	r6, r0, #0
	movs	r2, #7
	add	r3, sp, #84
	str	r1, [r6, #16]
	str	r3, [sp, #28]
	strb	r2, [r3, #0]
	ldr	r3, [pc, #256]
	strb	r2, [r1, #1]
	str	r3, [r6, #8]
	mov	r2, r9
	movs	r3, #6
	str	r2, [r6, #12]
	str	r3, [r6, #0]
	ldr	r4, [sp, #72]
	movs	r7, #224
	lsls	r7, r7, #3
	adds	r3, r4, r7
	str	r3, [r1, #4]
	mov	r1, fp
	lsls	r3, r1, #3
	ldr	r2, [pc, #232]
	subs	r3, #128
	movs	r0, #0
	mov	sl, r3
	mov	r5, fp
	lsls	r3, r1, #13
	mov	r8, r0
	adds	r7, r3, r2
	subs	r5, #16
.L_0818430e:
	cmp	r5, #0
	blt.n	.L_0818437e
	movs	r3, #0
	cmp	r5, #15
	ble.n	.L_0818431e
	movs	r3, #16
	subs	r3, r3, r5
	lsls	r3, r3, #3
.L_0818431e:
	movs	r4, #64
	negs	r4, r4
	str	r3, [r6, #20]
	cmp	r3, r4
	ble.n	.L_0818437e
	bl	sub_08014de4
	mov	r0, sl
	movs	r1, #112
	subs	r1, r1, r0
	lsls	r1, r1, #16
	movs	r0, #0
	movs	r2, #0
	bl	sub_08015160
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	lsls	r0, r0, #8
	bl	sub_080151e4
	adds	r0, r7, #0
	bl	sub_0801521c
	mov	r1, r8
	movs	r2, #128
	lsls	r0, r1, #14
	lsls	r2, r2, #6
	adds	r0, r0, r2
	bl	sub_080150e4
	ldr	r0, [pc, #136]
	bl	sub_08015024
	mov	r3, fp
	lsls	r0, r3, #10
	bl	sub_08015068
	ldr	r0, [pc, #128]
	mov	r1, r9
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_0818437e:
	ldr	r0, [pc, #116]
	movs	r1, #1
	movs	r4, #64
	add	r8, r1
	negs	r4, r4
	mov	r2, r8
	add	sl, r4
	adds	r7, r7, r0
	subs	r5, #8
	cmp	r2, #3
	bne.n	.L_0818430e
	adds	r0, r6, #0
	bl	sub_08013164
	mov	r0, r9
	bl	sub_08013164
	bl	sub_081434f8
	movs	r4, #240
	ldr	r3, [sp, #72]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r7, #1
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	add	fp, r7
	bl	sub_08013560
	mov	r0, fp
	cmp	r0, #60
	beq.n	.L_081843c4
	b.n	.L_081840d6
.L_081843c4:
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #44]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #164
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x08199364
	.4byte 0xfffe0000
	.4byte 0xfffff000
	.4byte 0x08199210
	.4byte 0xffff0000
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #272
	str	r0, [sp, #172]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #96]
	ldr	r2, [pc, #116]
	str	r0, [sp, #168]
	mov	r8, r2
	ldr	r1, [r5, #92]
	str	r1, [sp, #164]
	ldrh	r3, [r2, #4]
	str	r3, [sp, #152]
	adds	r3, r5, #0
	adds	r3, #176
	ldr	r3, [r3, #0]
	str	r3, [sp, #148]
	ldr	r4, [r5, #100]
	str	r4, [sp, #144]
	ldr	r6, [r5, #36]
	str	r6, [sp, #140]
	ldr	r0, [r6, #84]
	bl	sub_08014274
	bl	sub_08020380
	movs	r0, #128
	lsls	r0, r0, #6
	bl	sub_081435e0
	movs	r6, #128
	ldr	r3, [pc, #60]
	lsls	r6, r6, #19
	adds	r6, #32
	strh	r3, [r6, #0]
	bl	sub_0813ba50
	ldr	r2, [pc, #52]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	ldr	r5, [r5, #104]
	ldr	r7, [sp, #164]
	movs	r0, #239
	lsls	r0, r0, #7
	str	r5, [sp, #156]
	adds	r3, r7, r0
	movs	r5, #0
	movs	r1, #200
	str	r5, [r3, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #20]
	bl	sub_080145a8
	b.n	.L_08184494
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x03001120
	.2byte 0x3001
	.2byte 0x0814
.L_08184494:
	movs	r0, #0
	movs	r1, #0
	bl	sub_08163c2c
	movs	r3, #255
	add	r0, sp, #244
	strh	r3, [r0, #0]
	movs	r1, #0
	bl	sub_08118010
	ldr	r2, [pc, #96]
	movs	r3, #240
	str	r3, [r2, #16]
	movs	r0, #104
	bl	sub_0801314c
	ldr	r1, [pc, #88]
	movs	r0, #1
	movs	r2, #0
	bl	sub_08118040
	movs	r2, #31
	negs	r2, r2
	adds	r1, r2, #0
	adds	r0, r2, #0
	bl	sub_08164b2c
	ldr	r1, [sp, #148]
	movs	r3, #1
	str	r3, [r1, #16]
	ldr	r3, [pc, #40]
	mov	r2, r8
	strh	r5, [r2, #4]
	strh	r3, [r6, #0]
	movs	r2, #128
	ldr	r3, [pc, #36]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r1, [pc, #40]
	movs	r7, #0
	movs	r6, #0
.L_081844ee:
	cmp	r7, #14
	ble.n	.L_0818452c
	ldr	r4, [pc, #20]
	movs	r2, #0
	adds	r0, r6, #0
	b.n	.L_08184518
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x00001010
	.4byte 0x00003f44
	.4byte 0x0000000f
	.4byte 0x030011e0
	.4byte 0x00000072
	.2byte 0xf800
	.2byte 0x0600
.L_08184518:
	adds	r3, r2, #0
	ands	r3, r4
	adds	r3, r0, r3
	adds	r3, #48
	adds	r2, #1
	strh	r3, [r1, #0]
	adds	r1, #2
	cmp	r2, #32
	bne.n	.L_08184518
	b.n	.L_08184546
.L_0818452c:
	ldr	r5, [pc, #48]
	ldr	r4, [pc, #52]
	movs	r2, #0
	adds	r0, r6, #0
.L_08184534:
	adds	r3, r2, #0
	ands	r3, r5
	adds	r3, r0, r3
	adds	r3, r3, r4
	adds	r2, #1
	strh	r3, [r1, #0]
	adds	r1, #2
	cmp	r2, #32
	bne.n	.L_08184534
.L_08184546:
	adds	r7, #1
	adds	r6, #32
	cmp	r7, #30
	bne.n	.L_081844ee
	ldr	r0, [pc, #24]
	bl	sub_08013300
	movs	r2, #192
	adds	r7, r0, #0
	ldr	r5, [pc, #16]
	adds	r1, r7, #0
	b.n	.L_08184570
	movs	r0, r0
	.4byte 0x0000000f
	.4byte 0x00000200
	.4byte 0x000000ab
	.2byte 0x0730
	.2byte 0x0300
.L_08184570:
	lsls	r2, r2, #1
	ldr	r0, [pc, #808]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x23c0
	lsls	r3, r3, #1
	adds	r7, r7, r3
	adds	r0, r7, #0
	ldr	r1, [pc, #796]
	bl	sub_0801587c
	ldr	r6, [pc, #796]
	ldr	r0, [sp, #164]
	movs	r1, #238
	lsls	r1, r1, #7
	movs	r2, #13
	ldr	r7, [pc, #780]
	movs	r4, #0
	adds	r1, #220
	negs	r2, r2
	mov	sl, r6
	mov	r8, r5
	mov	r9, r4
	adds	r6, r0, r1
	adds	r5, r2, #0
.L_081845a2:
	movs	r1, #32
	ldr	r2, [pc, #768]
	movs	r3, #0
	movs	r0, #32
	bl	sub_0815b290
	ldrb	r3, [r0, #9]
	movs	r2, #8
	ands	r3, r5
	orrs	r3, r2
	strb	r3, [r0, #9]
	ldrb	r3, [r0, #16]
	stmia	r6!, {r0}
	lsls	r3, r3, #2
	add	r3, sl
	ldrh	r0, [r3, #2]
	ldr	r3, [pc, #744]
	movs	r2, #128
	adds	r1, r7, #0
	adds	r0, r0, r3
	lsls	r2, r2, #3
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x2001
	movs	r4, #128
	add	r9, r0
	lsls	r4, r4, #3
	mov	r1, r9
	adds	r7, r7, r4
	cmp	r1, #24
	bne.n	.L_081845a2
	ldr	r3, [pc, #704]
	ldr	r4, [sp, #164]
	movs	r0, #238
	lsls	r0, r0, #7
	ldr	r7, [pc, #708]
	ldr	r6, [pc, #712]
	movs	r2, #0
	adds	r0, #220
	mov	r9, r2
	mov	r8, r3
	adds	r5, r4, r0
.L_081845f6:
	movs	r3, #0
	movs	r1, #32
	ldr	r2, [pc, #684]
	movs	r0, #32
	bl	sub_0815b3b0
	ldr	r1, [r5, #0]
	str	r0, [r5, #96]
	movs	r2, #24
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x6e2b
	ldr	r1, [pc, #668]
	ldrb	r3, [r3, #16]
	movs	r2, #128
	lsls	r3, r3, #2
	add	r3, r8
	ldrh	r0, [r3, #2]
	lsls	r2, r2, #3
	adds	r0, r0, r1
	adds	r1, r6, #0
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x2301
	movs	r2, #128
	add	r9, r3
	lsls	r2, r2, #3
	mov	r4, r9
	adds	r6, r6, r2
	adds	r5, #4
	cmp	r4, #4
	bne.n	.L_081845f6
	ldr	r5, [pc, #616]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r0, r2, r5
	ldr	r1, [pc, #628]
	ldr	r5, [pc, #620]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x20ee
	ldr	r7, [sp, #164]
	lsls	r0, r0, #7
	movs	r6, #0
	adds	r0, #220
	mov	r9, r6
	adds	r6, r7, r0
.L_08184654:
	movs	r1, #32
	ldr	r2, [pc, #592]
	movs	r3, #0
	movs	r0, #32
	bl	sub_0815b3b0
	movs	r2, #24
	str	r0, [r6, #112]
	ldmia	r6!, {r1}
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2101
	add	r9, r1
	mov	r2, r9
	cmp	r2, #6
	bne.n	.L_08184654
	ldr	r0, [pc, #576]
	bl	sub_08013300
	adds	r7, r0, #0
	ldr	r6, [pc, #560]
	adds	r1, r7, #0
	movs	r2, #32
	ldr	r0, [pc, #568]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x9b29
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r5, r3, r4
	adds	r7, #32
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0801587c
	ldr	r0, [pc, #520]
	ldr	r1, [sp, #164]
	movs	r2, #240
	lsls	r2, r2, #7
	movs	r3, #13
	movs	r7, #0
	adds	r2, #100
	negs	r3, r3
	mov	r9, r7
	mov	r8, r6
	mov	sl, r0
	adds	r7, r1, r2
	adds	r6, r3, #0
.L_081846b4:
	movs	r2, #128
	movs	r3, #192
	movs	r1, #32
	lsls	r2, r2, #24
	lsls	r3, r3, #8
	movs	r0, #32
	bl	sub_0815b290
	ldrb	r3, [r0, #9]
	movs	r2, #4
	ands	r3, r6
	orrs	r3, r2
	strb	r3, [r0, #9]
	ldrb	r3, [r0, #16]
	stmia	r7!, {r0}
	lsls	r3, r3, #2
	add	r3, sl
	ldrh	r0, [r3, #2]
	ldr	r4, [pc, #464]
	movs	r2, #128
	adds	r1, r5, #0
	lsls	r2, r2, #2
	adds	r0, r0, r4
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x2101
	movs	r0, #128
	add	r9, r1
	lsls	r0, r0, #2
	mov	r2, r9
	adds	r5, r5, r0
	cmp	r2, #5
	bne.n	.L_081846b4
	ldr	r0, [pc, #456]
	bl	sub_08013300
	adds	r7, r0, #0
	ldr	r6, [pc, #432]
	adds	r1, r7, #0
	movs	r2, #32
	ldr	r0, [pc, #444]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x9b29
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r5, r3, r4
	adds	r7, #32
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0801587c
	ldr	r0, [pc, #388]
	ldr	r1, [sp, #164]
	movs	r2, #240
	lsls	r2, r2, #7
	movs	r3, #13
	movs	r7, #0
	adds	r2, #120
	negs	r3, r3
	mov	r9, r7
	mov	r8, r6
	mov	sl, r0
	adds	r7, r1, r2
	adds	r6, r3, #0
.L_08184736:
	movs	r2, #128
	movs	r3, #208
	movs	r1, #32
	lsls	r2, r2, #24
	lsls	r3, r3, #8
	movs	r0, #32
	bl	sub_0815b290
	ldrb	r3, [r0, #9]
	movs	r2, #4
	ands	r3, r6
	orrs	r3, r2
	strb	r3, [r0, #9]
	ldrb	r3, [r0, #16]
	stmia	r7!, {r0}
	lsls	r3, r3, #2
	add	r3, sl
	ldrh	r0, [r3, #2]
	ldr	r4, [pc, #336]
	movs	r2, #128
	adds	r1, r5, #0
	lsls	r2, r2, #2
	adds	r0, r0, r4
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x2101
	movs	r0, #128
	add	r9, r1
	lsls	r0, r0, #2
	mov	r2, r9
	adds	r5, r5, r0
	cmp	r2, #5
	bne.n	.L_08184736
	ldr	r0, [pc, #332]
	bl	sub_08013300
	adds	r7, r0, #0
	ldr	r6, [pc, #300]
	adds	r1, r7, #0
	movs	r2, #32
	ldr	r0, [pc, #324]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x9b29
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r5, r3, r4
	adds	r7, #32
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0801587c
	ldr	r0, [pc, #260]
	ldr	r1, [sp, #164]
	movs	r2, #240
	lsls	r2, r2, #7
	movs	r3, #13
	movs	r7, #0
	adds	r2, #140
	negs	r3, r3
	mov	r9, r7
	mov	r8, r6
	mov	sl, r0
	adds	r7, r1, r2
	adds	r6, r3, #0
.L_081847b8:
	movs	r2, #128
	movs	r3, #224
	movs	r1, #32
	lsls	r2, r2, #24
	lsls	r3, r3, #8
	movs	r0, #32
	bl	sub_0815b290
	ldrb	r3, [r0, #9]
	movs	r2, #4
	ands	r3, r6
	orrs	r3, r2
	strb	r3, [r0, #9]
	ldrb	r3, [r0, #16]
	stmia	r7!, {r0}
	lsls	r3, r3, #2
	add	r3, sl
	ldrh	r0, [r3, #2]
	ldr	r4, [pc, #204]
	movs	r2, #128
	adds	r1, r5, #0
	lsls	r2, r2, #2
	adds	r0, r0, r4
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x2101
	movs	r0, #128
	add	r9, r1
	lsls	r0, r0, #2
	mov	r2, r9
	adds	r5, r5, r0
	cmp	r2, #6
	bne.n	.L_081847b8
	ldr	r3, [sp, #164]
	movs	r4, #174
	lsls	r4, r4, #7
	movs	r2, #240
	adds	r0, r3, r4
	ldr	r5, [pc, #168]
	ldr	r1, [pc, #148]
	lsls	r2, r2, #1
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9e29
	movs	r7, #224
	lsls	r7, r7, #3
	adds	r1, r6, r7
	ldr	r0, [pc, #184]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #156
	lsls	r2, r2, #6
	adds	r1, r6, r2
	movs	r3, #0
	movs	r2, #0
	ldr	r0, [pc, #168]
	bl	sub_08157cf4
	ldr	r0, [pc, #164]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4826
	ldr	r1, [sp, #144]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #144]
	ldr	r1, [pc, #144]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r5, #238
	movs	r4, #239
	movs	r3, #0
	lsls	r4, r4, #7
	lsls	r5, r5, #7
	str	r3, [sp, #136]
	str	r3, [sp, #132]
	str	r3, [sp, #128]
	str	r3, [sp, #124]
	str	r3, [sp, #120]
	str	r3, [sp, #116]
	adds	r2, r6, r4
	movs	r3, #2
	adds	r5, #132
	str	r3, [r2, #0]
	adds	r2, r6, r5
	movs	r3, #50
	str	r3, [r2, #0]
	movs	r0, #104
	movs	r1, #19
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	ldr	r7, [pc, #88]
	movs	r6, #0
	mov	fp, r6
	str	r3, [sp, #156]
	str	r7, [sp, #32]
	str	r6, [sp, #28]
	bl	.L_08185b9c
	lsls	r0, r0, #8
	lsls	r0, r0, #20
	movs	r0, r0
	lsls	r1, r0, #8
	adds	r6, #224
	lsls	r0, r0, #8
	movs	r0, #0
	strh	r0, [r0, #0]
	movs	r0, r0
	lsls	r1, r0, #24
	lsls	r0, r6, #28
	lsls	r0, r0, #12
	str	r0, [r0, #0]
	lsls	r1, r0, #8
	lsls	r6, r5, #2
	movs	r0, r0
	lsls	r0, r0, #14
	lsls	r0, r0, #20
	lsls	r4, r5, #2
	movs	r0, r0
	lsls	r0, r4, #14
	lsls	r0, r0, #20
	lsls	r5, r5, #2
	movs	r0, r0
	lsls	r0, r0, #15
	lsls	r0, r0, #20
	lsls	r1, r7, #3
	movs	r0, r0
	lsls	r0, r7, #3
	movs	r0, r0
	lsls	r0, r1, #6
	movs	r0, r0
	lsls	r4, r6, #4
	movs	r0, r0
	lsls	r4, r6, #2
	movs	r0, r0
	movs	r0, #0
	lsls	r1, r0, #8
	movs	r1, #192
	.2byte 0xff90
	.2byte 0x4658
	cmp	r0, #0
	bne.n	.L_08184916
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [sp, #164]
	movs	r1, #3
	movs	r2, #1
	mov	r9, r1
	negs	r2, r2
	adds	r3, #108
.L_08184908:
	movs	r4, #1
	add	r9, r4
	mov	r5, r9
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r5, #64
	bne.n	.L_08184908
.L_08184916:
	movs	r6, #220
	lsls	r6, r6, #1
	cmp	fp, r6
	bne.n	.L_08184934
	movs	r2, #128
	ldr	r3, [pc, #8]
	lsls	r2, r2, #19
	b.n	.L_08184930
	movs	r0, r0
	.4byte 0x00000786
	.2byte 0x0784
	.2byte 0x0000
.L_08184930:
	adds	r2, #12
	strh	r3, [r2, #0]
.L_08184934:
	movs	r7, #114
	adds	r7, #255
	cmp	fp, r7
	bgt.n	.L_08184956
	movs	r3, #167
	lsls	r3, r3, #1
	movs	r1, #124
	adds	r1, #255
	mov	r0, fp
	muls	r0, r3
	bl	sub_08002054
	ldr	r1, [pc, #160]
	lsls	r0, r0, #16
	adds	r1, r0, r1
	str	r1, [sp, #136]
	b.n	.L_081849ca
.L_08184956:
	ldr	r0, [pc, #156]
	add	r0, fp
	cmp	r0, #19
	bhi.n	.L_08184976
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #102
	muls	r0, r3
	bl	sub_08002096
	movs	r2, #172
	lsls	r3, r0, #3
	adds	r3, r3, r0
	lsls	r2, r2, #14
	adds	r2, r3, r2
	b.n	.L_081849c8
.L_08184976:
	ldr	r3, [pc, #128]
	add	r3, fp
	cmp	r3, #95
	bhi.n	.L_08184988
	ldr	r3, [sp, #136]
	ldr	r4, [pc, #120]
	adds	r3, r3, r4
	str	r3, [sp, #136]
	b.n	.L_081849ca
.L_08184988:
	ldr	r3, [pc, #116]
	add	r3, fp
	cmp	r3, #19
	bhi.n	.L_0818499a
	ldr	r5, [sp, #136]
	ldr	r6, [pc, #112]
	adds	r5, r5, r6
	str	r5, [sp, #136]
	b.n	.L_081849ca
.L_0818499a:
	ldr	r3, [pc, #108]
	add	r3, fp
	cmp	r3, #47
	bhi.n	.L_081849ca
	mov	r7, fp
	lsls	r3, r7, #3
	subs	r3, r3, r7
	lsls	r3, r3, #3
	ldr	r0, [pc, #96]
	add	r3, fp
	movs	r1, #234
	lsls	r3, r3, #7
	lsls	r1, r1, #9
	adds	r3, r3, r0
	adds	r1, #192
	cmp	r3, r1
	ble.n	.L_081849c2
	movs	r3, #234
	lsls	r3, r3, #9
	adds	r3, #192
.L_081849c2:
	ldr	r2, [sp, #136]
	lsls	r3, r3, #1
	subs	r2, r2, r3
.L_081849c8:
	str	r2, [sp, #136]
.L_081849ca:
	movs	r3, #124
	adds	r3, #255
	cmp	fp, r3
	bgt.n	.L_08184a14
	movs	r3, #104
	movs	r1, #124
	mov	r0, fp
	muls	r0, r3
	adds	r1, #255
	bl	sub_08002054
	ldr	r3, [pc, #8]
	ldr	r5, [pc, #44]
	subs	r3, r3, r0
	strh	r3, [r5, #6]
	b.n	.L_08184a34
	movs	r0, r0
	.4byte 0x00000068
	.4byte 0xfee60000
	.4byte 0xfffffe8e
	.4byte 0xfffffe7a
	.4byte 0xffff8000
	.4byte 0xfffffe1a
	.4byte 0xffff0000
	.4byte 0xfffffe06
	.4byte 0xffc87150
	.2byte 0x1120
	.2byte 0x0300
.L_08184a14:
	ldr	r3, [pc, #288]
	ldr	r2, [pc, #292]
	add	r3, fp
	cmp	r3, #0
	bge.n	.L_08184a22
	ldr	r3, [pc, #288]
	add	r3, fp
.L_08184a22:
	asrs	r3, r3, #2
	movs	r4, #208
	strh	r3, [r2, #6]
	lsls	r4, r4, #15
	lsls	r3, r3, #16
	cmp	r3, r4
	bls.n	.L_08184a34
	movs	r3, #104
	strh	r3, [r2, #6]
.L_08184a34:
	ldr	r3, [pc, #268]
	add	r3, fp
	cmp	r3, #59
	bhi.n	.L_08184a52
	lsls	r0, r3, #4
	adds	r0, r0, r3
	lsls	r0, r0, #4
	adds	r0, r0, r3
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	bl	sub_08002096
	lsls	r0, r0, #4
	str	r0, [sp, #124]
.L_08184a52:
	ldr	r3, [pc, #244]
	add	r3, fp
	cmp	r3, #51
	bhi.n	.L_08184a70
	ldr	r6, [sp, #124]
	movs	r3, #210
	ldr	r7, [pc, #236]
	lsls	r3, r3, #1
	mov	r5, fp
	subs	r3, r3, r5
	lsls	r3, r3, #12
	adds	r6, r6, r7
	str	r3, [sp, #128]
	str	r6, [sp, #124]
	b.n	.L_08184ad6
.L_08184a70:
	ldr	r3, [pc, #220]
	add	r3, fp
	cmp	r3, #13
	bhi.n	.L_08184a8e
	ldr	r0, [sp, #128]
	movs	r1, #234
	lsls	r3, r3, #12
	lsls	r1, r1, #9
	subs	r3, r0, r3
	adds	r1, #192
	adds	r1, r3, r1
	ldr	r2, [sp, #124]
	ldr	r3, [pc, #200]
	str	r1, [sp, #128]
	b.n	.L_08184ac0
.L_08184a8e:
	ldr	r3, [pc, #200]
	add	r3, fp
	cmp	r3, #8
	bhi.n	.L_08184ab4
	ldr	r4, [sp, #128]
	ldr	r7, [pc, #192]
	movs	r5, #128
	mov	r6, fp
	lsls	r5, r5, #6
	lsls	r0, r6, #7
	adds	r4, r4, r5
	adds	r0, r0, r7
	str	r4, [sp, #128]
	bl	sub_08002096
	ldr	r1, [sp, #124]
	subs	r1, r1, r0
	str	r1, [sp, #124]
	b.n	.L_08184ad6
.L_08184ab4:
	ldr	r3, [pc, #168]
	add	r3, fp
	cmp	r3, #29
	bhi.n	.L_08184ac6
	ldr	r2, [sp, #124]
	ldr	r3, [pc, #164]
.L_08184ac0:
	adds	r2, r2, r3
	str	r2, [sp, #124]
	b.n	.L_08184ad6
.L_08184ac6:
	ldr	r3, [pc, #160]
	add	r3, fp
	cmp	r3, #9
	bhi.n	.L_08184ad6
	ldr	r4, [sp, #124]
	ldr	r5, [pc, #152]
	adds	r4, r4, r5
	str	r4, [sp, #124]
.L_08184ad6:
	mov	r6, fp
	cmp	r6, #0
	bne.n	.L_08184ae6
	movs	r7, #1
	negs	r7, r7
	movs	r0, #0
	str	r7, [sp, #120]
	str	r0, [sp, #116]
.L_08184ae6:
	ldr	r1, [sp, #116]
	cmp	r1, #17
	ble.n	.L_08184aee
	b.n	.L_08184c0a
.L_08184aee:
	ldr	r3, [pc, #128]
	lsls	r5, r1, #1
	ldrh	r3, [r3, r5]
	movs	r6, #1
	negs	r6, r6
	cmp	fp, r3
	bne.n	.L_08184b7e
	ldr	r3, [pc, #116]
	ldrh	r3, [r3, r5]
	str	r3, [sp, #120]
	movs	r3, #3
	ands	r3, r1
	cmp	r3, #1
	beq.n	.L_08184b26
	cmp	r3, #1
	bgt.n	.L_08184b14
	cmp	r3, #0
	beq.n	.L_08184b1e
	b.n	.L_08184b7e
.L_08184b14:
	cmp	r3, #2
	beq.n	.L_08184b2e
	cmp	r3, #3
	beq.n	.L_08184b78
	b.n	.L_08184b7e
.L_08184b1e:
	movs	r0, #227
	bl	sub_081c0010
	b.n	.L_08184b7e
.L_08184b26:
	movs	r0, #171
	bl	sub_081c0010
	b.n	.L_08184b7e
.L_08184b2e:
	movs	r0, #208
	bl	sub_081c0010
	b.n	.L_08184b7e
	movs	r0, r0
	.4byte 0xfffffe84
	.4byte 0x03001120
	.4byte 0xfffffe87
	.4byte 0xfffffe8e
	.4byte 0xfffffe5c
	.4byte 0xffffc000
	.4byte 0xfffffe28
	.4byte 0x00012710
	.4byte 0xfffffe1a
	.4byte 0xffff0d00
	.4byte 0xfffffdd6
	.4byte 0xffff0000
	.4byte 0xfffffdb8
	.4byte 0xfff80000
	.4byte 0x08199768
	.2byte 0x978c
	.2byte 0x0819
.L_08184b78:
	movs	r0, #172
	bl	sub_081c0010
.L_08184b7e:
	ldr	r2, [sp, #120]
	cmp	r2, #0
	blt.n	.L_08184ba6
	ldr	r3, [pc, #96]
	lsls	r0, r2, #5
	ldrh	r1, [r3, r5]
	subs	r0, r0, r2
	bl	sub_08002054
	ldr	r3, [sp, #120]
	movs	r4, #1
	subs	r3, #1
	negs	r4, r4
	adds	r6, r0, #0
	str	r3, [sp, #120]
	cmp	r3, r4
	bne.n	.L_08184ba6
	ldr	r5, [sp, #116]
	adds	r5, #1
	str	r5, [sp, #116]
.L_08184ba6:
	cmp	r6, #0
	blt.n	.L_08184c0a
	ldr	r7, [sp, #140]
	movs	r1, #160
	movs	r4, #160
	lsls	r1, r1, #3
	ldr	r5, [pc, #48]
	lsls	r4, r4, #19
	adds	r1, #108
	movs	r2, #0
	adds	r0, r7, r1
	adds	r4, #192
	mov	r9, r2
	movs	r7, #31
.L_08184bc2:
	ldrh	r3, [r0, #0]
	adds	r1, r7, #0
	ands	r1, r3
	lsls	r3, r3, #16
	lsrs	r2, r3, #21
	lsrs	r3, r3, #26
	ands	r3, r5
	ands	r2, r5
	adds	r3, r3, r6
	cmp	r1, #31
	ble.n	.L_08184bda
	movs	r1, #31
.L_08184bda:
	cmp	r2, #31
	ble.n	.L_08184bec
	movs	r2, #31
	b.n	.L_08184bec
	movs	r0, r0
	.4byte 0x0000001f
	.2byte 0x978c
	.2byte 0x0819
.L_08184bec:
	cmp	r3, #31
	ble.n	.L_08184bf2
	movs	r3, #31
.L_08184bf2:
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r1
	strh	r3, [r4, #0]
	movs	r3, #1
	add	r9, r3
	mov	r1, r9
	adds	r0, #2
	adds	r4, #2
	cmp	r1, #128
	bne.n	.L_08184bc2
.L_08184c0a:
	movs	r3, #0
	movs	r4, #0
	movs	r2, #0
	str	r3, [sp, #208]
	str	r4, [sp, #212]
	mov	r3, fp
	mov	r8, r2
	mov	ip, r2
	mov	lr, r2
	cmp	r3, #0
	bne.n	.L_08184c30
	movs	r4, #31
	negs	r4, r4
	mov	ip, r4
	movs	r5, #1
	mov	r8, ip
	mov	lr, ip
	str	r5, [sp, #208]
	b.n	.L_08184d52
.L_08184c30:
	mov	r6, fp
	cmp	r6, #106
	bne.n	.L_08184c4a
	movs	r7, #31
	negs	r7, r7
	movs	r0, #15
	mov	ip, r7
	negs	r0, r0
	movs	r1, #1
	mov	r8, ip
	mov	lr, r0
	str	r1, [sp, #208]
	b.n	.L_08184d52
.L_08184c4a:
	mov	r2, fp
	subs	r2, #154
	cmp	r2, #59
	bhi.n	.L_08184c6c
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	movs	r1, #5
	lsls	r2, r3, #1
	subs	r2, r1, r2
	mov	ip, r2
	subs	r1, r1, r3
	movs	r2, #1
	mov	r8, ip
	mov	lr, r1
	str	r2, [sp, #208]
	b.n	.L_08184d52
.L_08184c6c:
	mov	r2, fp
	subs	r2, #226
	cmp	r2, #15
	bhi.n	.L_08184c8c
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	subs	r3, #31
	mov	ip, r3
	ldr	r3, [pc, #492]
	movs	r4, #1
	add	r3, fp
	mov	r8, ip
	mov	lr, r3
	str	r4, [sp, #208]
	b.n	.L_08184d52
.L_08184c8c:
	mov	r5, fp
	cmp	r5, #242
	bne.n	.L_08184ca4
	movs	r6, #24
	movs	r7, #31
	negs	r6, r6
	negs	r7, r7
	movs	r0, #1
	mov	r8, r6
	mov	ip, r7
	str	r0, [sp, #208]
	b.n	.L_08184d52
.L_08184ca4:
	ldr	r4, [pc, #456]
	add	r4, fp
	cmp	r4, #21
	bhi.n	.L_08184cc8
	movs	r2, #24
	lsls	r3, r4, #1
	negs	r2, r2
	subs	r2, r2, r3
	mov	r8, r2
	movs	r2, #31
	negs	r2, r2
	subs	r2, r2, r3
	negs	r4, r4
	movs	r1, #1
	mov	ip, r2
	mov	lr, r4
	str	r1, [sp, #208]
	b.n	.L_08184d52
.L_08184cc8:
	ldr	r2, [pc, #424]
	add	r2, fp
	cmp	r2, #40
	bhi.n	.L_08184ce2
	negs	r3, r2
	mov	ip, r3
	movs	r3, #16
	subs	r3, r3, r2
	movs	r4, #1
	mov	r8, ip
	mov	lr, r3
	str	r4, [sp, #208]
	b.n	.L_08184d52
.L_08184ce2:
	movs	r5, #162
	lsls	r5, r5, #1
	cmp	fp, r5
	bne.n	.L_08184cfc
	movs	r6, #31
	movs	r7, #1
	mov	r8, r6
	mov	ip, r6
	mov	lr, r6
	add	r6, sp, #208
	str	r7, [sp, #208]
	str	r7, [r6, #4]
	b.n	.L_08184d54
.L_08184cfc:
	ldr	r2, [pc, #376]
	add	r2, fp
	cmp	r2, #59
	bhi.n	.L_08184d1e
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	movs	r2, #30
	subs	r2, r2, r3
	mov	ip, r2
	movs	r0, #1
	add	r6, sp, #208
	str	r0, [sp, #208]
	mov	r8, ip
	mov	lr, ip
	str	r0, [r6, #4]
	b.n	.L_08184d54
.L_08184d1e:
	ldr	r5, [pc, #348]
	add	r6, sp, #208
	add	r5, fp
	cmp	r5, #42
	bhi.n	.L_08184d54
	adds	r2, r5, #0
	cmp	r5, #0
	bge.n	.L_08184d32
	ldr	r2, [pc, #336]
	add	r2, fp
.L_08184d32:
	asrs	r2, r2, #2
	adds	r0, r5, #0
	movs	r1, #6
	mov	r8, r2
	bl	sub_08002054
	mov	ip, r0
	adds	r0, r5, #0
	cmp	r0, #0
	bge.n	.L_08184d4a
	ldr	r0, [pc, #316]
	add	r0, fp
.L_08184d4a:
	movs	r1, #1
	str	r1, [sp, #208]
	asrs	r0, r0, #4
	mov	lr, r0
.L_08184d52:
	add	r6, sp, #208
.L_08184d54:
	movs	r2, #0
	mov	sl, r2
.L_08184d58:
	mov	r4, sl
	lsls	r3, r4, #2
	ldr	r3, [r3, r6]
	cmp	r3, #0
	beq.n	.L_08184de4
	cmp	r4, #0
	bne.n	.L_08184d76
	ldr	r7, [sp, #164]
	movs	r0, #174
	lsls	r0, r0, #7
	adds	r0, #2
	adds	r5, r7, r0
	movs	r1, #240
	ldr	r7, [pc, #276]
	b.n	.L_08184d88
.L_08184d76:
	ldr	r1, [sp, #140]
	movs	r2, #160
	movs	r7, #160
	lsls	r2, r2, #3
	lsls	r7, r7, #19
	adds	r2, #108
	adds	r5, r1, r2
	adds	r7, #192
	movs	r1, #128
.L_08184d88:
	movs	r3, #1
	mov	r9, r3
	cmp	r1, #1
	beq.n	.L_08184de4
.L_08184d90:
	ldrh	r3, [r5, #0]
	movs	r2, #31
	ands	r2, r3
	mov	r0, r8
	lsls	r3, r3, #16
	adds	r4, r2, r0
	lsrs	r2, r3, #21
	movs	r0, #31
	lsrs	r3, r3, #26
	ands	r2, r0
	ands	r3, r0
	add	r2, ip
	add	r3, lr
	cmp	r4, #31
	ble.n	.L_08184db0
	movs	r4, #31
.L_08184db0:
	cmp	r2, #31
	ble.n	.L_08184db6
	movs	r2, #31
.L_08184db6:
	cmp	r3, #31
	ble.n	.L_08184dbc
	movs	r3, #31
.L_08184dbc:
	cmp	r4, #0
	bge.n	.L_08184dc2
	movs	r4, #0
.L_08184dc2:
	cmp	r2, #0
	bge.n	.L_08184dc8
	movs	r2, #0
.L_08184dc8:
	cmp	r3, #0
	bge.n	.L_08184dce
	movs	r3, #0
.L_08184dce:
	lsls	r2, r2, #5
	lsls	r3, r3, #10
	orrs	r3, r2
	movs	r2, #1
	orrs	r3, r4
	add	r9, r2
	strh	r3, [r7, #0]
	adds	r5, #2
	adds	r7, #2
	cmp	r9, r1
	bne.n	.L_08184d90
.L_08184de4:
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	cmp	r4, #2
	bne.n	.L_08184d58
	ldr	r2, [pc, #156]
	add	r2, fp
	cmp	r2, #31
	bhi.n	.L_08184e1e
	adds	r3, r2, #0
	movs	r5, #1
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L_08184e1e
	ldr	r6, [sp, #140]
	movs	r7, #160
	lsls	r3, r2, #10
	lsls	r7, r7, #3
	movs	r2, #128
	movs	r1, #160
	lsls	r2, r2, #9
	adds	r7, #108
	lsls	r1, r1, #19
	subs	r2, r2, r3
	adds	r0, r6, r7
	adds	r1, #192
	movs	r3, #128
	bl	sub_081180f0
.L_08184e1e:
	ldr	r0, [sp, #132]
	cmp	r0, #0
	bne.n	.L_08184ea0
	movs	r1, #84
	adds	r1, #255
	cmp	fp, r1
	bne.n	.L_08184eea
	movs	r2, #1
	ldr	r4, [sp, #164]
	movs	r0, #240
	lsls	r0, r0, #7
	str	r2, [sp, #132]
	ldr	r7, [pc, #88]
	ldr	r6, [pc, #88]
	movs	r3, #0
	adds	r0, #60
	mov	r9, r3
	adds	r5, r4, r0
.L_08184e42:
	ldmia	r5!, {r3}
	ldr	r1, [pc, #80]
	ldrb	r3, [r3, #16]
	mov	r2, r9
	lsls	r3, r3, #2
	adds	r3, r3, r7
	ldrh	r0, [r3, #2]
	ldr	r3, [pc, #72]
	adds	r0, r0, r1
	lsls	r1, r2, #10
	movs	r2, #128
	adds	r1, r1, r3
	lsls	r2, r2, #3
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x2401
	add	r9, r4
	mov	r0, r9
	cmp	r0, #4
	bne.n	.L_08184e42
	b.n	.L_08184eea
	.4byte 0xfffffeff
	.4byte 0xffffff00
	.4byte 0xfffffee6
	.4byte 0xfffffeb4
	.4byte 0xfffffe17
	.4byte 0xfffffe1a
	.4byte 0xfffffe26
	.4byte 0x05000202
	.4byte 0xfffffdd6
	.4byte 0x020036e0
	.4byte 0x03000730
	.4byte 0x06010000
	.2byte 0x0000
	.2byte 0x0201
.L_08184ea0:
	movs	r1, #225
	lsls	r1, r1, #1
	cmp	fp, r1
	bne.n	.L_08184eea
	movs	r2, #0
	ldr	r3, [sp, #164]
	movs	r0, #240
	lsls	r0, r0, #7
	str	r2, [sp, #132]
	ldr	r4, [pc, #880]
	ldr	r7, [pc, #880]
	ldr	r6, [pc, #884]
	adds	r0, #60
	mov	r9, r2
	adds	r5, r3, r0
.L_08184ebe:
	ldmia	r5!, {r3}
	ldr	r1, [pc, #876]
	ldrb	r3, [r3, #16]
	movs	r2, #128
	lsls	r3, r3, #2
	adds	r3, r3, r4
	ldrh	r0, [r3, #2]
	lsls	r2, r2, #3
	adds	r0, r0, r1
	str	r4, [sp, #8]
	adds	r1, r6, #0
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x2301
	movs	r2, #128
	add	r9, r3
	lsls	r2, r2, #3
	mov	r0, r9
	adds	r6, r6, r2
	ldr	r4, [sp, #8]
	cmp	r0, #4
	bne.n	.L_08184ebe
.L_08184eea:
	movs	r1, #169
	lsls	r1, r1, #1
	adds	r1, #255
	cmp	fp, r1
	ble.n	.L_08184ef6
	b.n	.L_0818507c
.L_08184ef6:
	ldr	r3, [pc, #828]
	add	r7, sp, #228
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	movs	r2, #0
	str	r3, [sp, #200]
	str	r4, [sp, #204]
	movs	r3, #0
	str	r3, [r7, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	ldr	r3, [sp, #164]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #220
	mov	r9, r2
	adds	r6, r7, #0
	adds	r5, r3, r4
.L_08184f1c:
	ldr	r3, [pc, #792]
	mov	r0, r9
	ldrb	r3, [r3, r0]
	movs	r1, #128
	lsls	r3, r3, #16
	lsls	r1, r1, #12
	adds	r3, r3, r1
	str	r3, [r6, #0]
	ldr	r3, [pc, #780]
	ldr	r2, [sp, #136]
	ldrb	r3, [r3, r0]
	ldr	r4, [pc, #780]
	lsls	r3, r3, #16
	adds	r3, r3, r2
	str	r3, [r6, #8]
	cmp	r3, r4
	ble.n	.L_08184f52
	movs	r0, #168
	lsls	r0, r0, #16
	cmp	r3, r0
	bgt.n	.L_08184f52
	ldr	r0, [r5, #0]
	adds	r1, r7, #0
	add	r2, sp, #200
	movs	r3, #0
	bl	sub_08020010
.L_08184f52:
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	adds	r5, #4
	cmp	r2, #28
	bne.n	.L_08184f1c
	ldr	r4, [sp, #164]
	movs	r0, #240
	lsls	r0, r0, #7
	movs	r3, #0
	adds	r0, #100
	mov	r9, r3
	adds	r6, r7, #0
	adds	r5, r4, r0
.L_08184f6e:
	mov	r1, r9
	lsrs	r2, r1, #31
	add	r2, r9
	asrs	r2, r2, #1
	ldr	r4, [sp, #128]
	lsls	r3, r2, #1
	subs	r3, r1, r3
	lsls	r3, r3, #21
	movs	r0, #144
	adds	r3, r3, r4
	lsls	r0, r0, #15
	adds	r3, r3, r0
	str	r3, [r6, #0]
	ldr	r1, [sp, #136]
	ldr	r3, [sp, #124]
	lsls	r2, r2, #21
	adds	r2, r2, r1
	movs	r4, #240
	ldr	r0, [pc, #684]
	adds	r2, r2, r3
	lsls	r4, r4, #14
	adds	r2, r2, r4
	str	r2, [r6, #8]
	cmp	r2, r0
	ble.n	.L_08184fb4
	movs	r1, #168
	lsls	r1, r1, #16
	cmp	r2, r1
	bgt.n	.L_08184fb4
	ldr	r0, [r5, #0]
	adds	r1, r7, #0
	add	r2, sp, #200
	movs	r3, #0
	bl	sub_08020010
.L_08184fb4:
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	adds	r5, #4
	cmp	r3, #5
	bne.n	.L_08184f6e
	movs	r4, #0
	ldr	r0, [sp, #164]
	movs	r1, #240
	mov	r9, r4
	lsls	r1, r1, #7
	ldr	r4, [pc, #632]
	adds	r1, #120
	adds	r6, r7, #0
	adds	r5, r0, r1
.L_08184fd2:
	ldr	r3, [pc, #628]
	mov	r2, r9
	ldrb	r3, [r3, r2]
	ldr	r0, [sp, #128]
	lsls	r3, r3, #16
	movs	r1, #132
	subs	r3, r3, r0
	lsls	r1, r1, #16
	adds	r3, r3, r1
	str	r3, [r6, #0]
	ldrh	r3, [r4, #0]
	ldr	r2, [sp, #136]
	ldr	r0, [sp, #124]
	lsls	r3, r3, #16
	adds	r3, r3, r2
	movs	r1, #232
	ldr	r2, [pc, #588]
	adds	r3, r3, r0
	lsls	r1, r1, #13
	adds	r3, r3, r1
	adds	r4, #2
	str	r3, [r6, #8]
	cmp	r3, r2
	ble.n	.L_0818501a
	movs	r0, #168
	lsls	r0, r0, #16
	cmp	r3, r0
	bgt.n	.L_0818501a
	ldr	r0, [r5, #0]
	adds	r1, r7, #0
	add	r2, sp, #200
	movs	r3, #0
	str	r4, [sp, #8]
	bl	sub_08020010
	ldr	r4, [sp, #8]
.L_0818501a:
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	adds	r5, #4
	cmp	r2, #11
	bne.n	.L_08184fd2
	ldr	r3, [pc, #548]
	ldr	r4, [sp, #164]
	movs	r0, #240
	lsls	r0, r0, #7
	str	r3, [sp, #200]
	adds	r0, #76
	movs	r3, #0
	mov	r9, r3
	adds	r6, r7, #0
	adds	r5, r4, r0
.L_0818503a:
	ldr	r3, [pc, #532]
	mov	r1, r9
	ldrb	r3, [r3, r1]
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r6, #0]
	ldr	r3, [pc, #520]
	ldr	r4, [sp, #136]
	ldrb	r3, [r3, r1]
	ldr	r0, [pc, #492]
	lsls	r3, r3, #16
	adds	r3, r3, r4
	str	r3, [r6, #8]
	cmp	r3, r0
	ble.n	.L_08185070
	movs	r1, #168
	lsls	r1, r1, #16
	cmp	r3, r1
	bgt.n	.L_08185070
	ldr	r0, [r5, #0]
	adds	r1, r7, #0
	add	r2, sp, #200
	movs	r3, #0
	bl	sub_08020010
.L_08185070:
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	adds	r5, #4
	cmp	r3, #6
	bne.n	.L_0818503a
.L_0818507c:
	movs	r0, #128
	lsls	r0, r0, #2
	bl	sub_08014dac
	str	r0, [sp, #112]
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #456]
	ldr	r3, [sp, #192]
	movs	r4, #0
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #448]
	str	r4, [sp, #108]
	ands	r3, r2
	movs	r2, #192
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #192]
	ldr	r3, [pc, #440]
	add	r5, sp, #192
	str	r3, [r0, #8]
	movs	r3, #6
	str	r5, [r0, #16]
	str	r3, [r0, #0]
	ldr	r6, [sp, #112]
	movs	r5, #128
	str	r6, [r0, #12]
	mov	r8, r0
	mov	r9, r4
	lsls	r5, r5, #8
.L_081850be:
	ldr	r2, [pc, #420]
	mov	r7, r9
	lsls	r3, r7, #1
	ldrh	r1, [r2, r3]
	cmp	fp, r1
	blt.n	.L_08185130
	adds	r3, r1, #0
	adds	r3, #8
	cmp	fp, r3
	bge.n	.L_08185130
	ldr	r3, [pc, #404]
	mov	r0, fp
	ldrb	r2, [r3, r7]
	subs	r3, r1, r0
	lsls	r3, r3, #3
	mov	r1, r8
	str	r3, [sp, #108]
	str	r3, [r1, #20]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #164]
	lsls	r3, r3, #9
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r3, r2, r3
	adds	r3, r3, r4
	str	r3, [sp, #196]
	bl	sub_08014de4
	ldr	r3, [pc, #368]
	movs	r2, #0
	ldrb	r0, [r3, r7]
	ldr	r3, [pc, #368]
	subs	r0, #48
	ldrb	r1, [r3, r7]
	lsls	r0, r0, #16
	subs	r1, #44
	lsls	r1, r1, #16
	bl	sub_08015160
	movs	r0, #0
	bl	sub_080150e4
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #9
	adds	r2, r5, #0
	bl	sub_080151e4
	ldr	r0, [pc, #336]
	ldr	r1, [sp, #112]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, r8
	bl	sub_08196a7c
.L_08185130:
	movs	r6, #1
	add	r9, r6
	mov	r7, r9
	cmp	r7, #7
	bne.n	.L_081850be
	movs	r0, #234
	adds	r0, #255
	cmp	fp, r0
	bne.n	.L_0818515a
	ldr	r3, [sp, #164]
	movs	r1, #0
	mov	r9, r1
	movs	r2, #0
	adds	r3, #24
.L_0818514c:
	movs	r4, #1
	add	r9, r4
	mov	r5, r9
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r5, #3
	bne.n	.L_0818514c
.L_0818515a:
	movs	r6, #3
	ldr	r5, [sp, #164]
	mov	sl, r6
	ldr	r6, [pc, #276]
	adds	r5, #84
.L_08185164:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_081851a6
	asrs	r0, r0, #3
	adds	r0, #1
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r7, [sp, #144]
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
	ldr	r0, [sp, #168]
	ldr	r4, [sp, #156]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #63
	movs	r2, #0
	bl	sub_08138086
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_081851a6:
	movs	r7, #1
	add	sl, r7
	mov	r0, sl
	adds	r5, #28
	cmp	r0, #64
	bne.n	.L_08185164
	ldr	r3, [sp, #164]
	ldr	r2, [pc, #196]
	movs	r5, #238
	movs	r4, #239
	lsls	r5, r5, #7
	ldr	r7, [pc, #192]
	movs	r1, #0
	lsls	r4, r4, #7
	adds	r5, #132
	mov	r9, r1
	adds	r0, r3, r4
	adds	r1, r3, r5
	mov	ip, r2
	movs	r4, #0
	movs	r6, #2
	movs	r5, #50
.L_081851d2:
	mov	r2, ip
	ldrh	r3, [r4, r2]
	adds	r2, r3, #0
	adds	r2, #242
	cmp	fp, r2
	bne.n	.L_081851e4
	movs	r2, #1
	str	r2, [r0, #0]
	str	r7, [r1, #0]
.L_081851e4:
	adds	r3, #243
	cmp	fp, r3
	bne.n	.L_081851ee
	str	r6, [r0, #0]
	str	r5, [r1, #0]
.L_081851ee:
	movs	r3, #1
	add	r9, r3
	mov	r2, r9
	adds	r4, #2
	cmp	r2, #17
	bne.n	.L_081851d2
	ldr	r3, [pc, #136]
	add	r3, fp
	cmp	r3, #55
	bhi.n	.L_081852a0
	movs	r2, #1
	mov	r3, fp
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08185288
	ldr	r4, [sp, #164]
	movs	r5, #239
	lsls	r5, r5, #7
	adds	r3, r4, r5
	movs	r6, #238
	str	r2, [r3, #0]
	lsls	r6, r6, #7
	ldr	r3, [pc, #100]
	adds	r6, #132
	adds	r2, r4, r6
	b.n	.L_0818529e
	movs	r0, r0
	.4byte 0x020036e0
	.4byte 0x03000730
	.4byte 0x02011000
	.4byte 0x06010000
	.4byte 0x08196ef8
	.4byte 0x081997b0
	.4byte 0x081997cc
	.4byte 0x000fffff
	.4byte 0x081997f4
	.4byte 0x081997e8
	.4byte 0xfffeffff
	.4byte 0x0819980a
	.4byte 0x08199810
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x0819931c
	.4byte 0x08199816
	.4byte 0x08199832
	.4byte 0x08199824
	.4byte 0x0819982b
	.4byte 0x081991e0
	.4byte 0x08197410
	.4byte 0x0819983a
	.4byte 0x1f1f1f1f
	.2byte 0xfde7
	.2byte 0xffff
.L_08185288:
	ldr	r7, [sp, #164]
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	adds	r2, r7, r0
	movs	r3, #2
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r2, r7, r1
	movs	r3, #50
.L_0818529e:
	str	r3, [r2, #0]
.L_081852a0:
	movs	r2, #162
	lsls	r2, r2, #1
	cmp	fp, r2
	bne.n	.L_081852ae
	movs	r0, #206
	bl	sub_081c0010
.L_081852ae:
	movs	r3, #140
	lsls	r3, r3, #2
	cmp	fp, r3
	bne.n	.L_081852bc
	movs	r0, #220
	bl	sub_081c0010
.L_081852bc:
	movs	r4, #128
	lsls	r4, r4, #2
	adds	r4, #90
	cmp	fp, r4
	bne.n	.L_081852cc
	movs	r0, #145
	bl	sub_081c0010
.L_081852cc:
	movs	r3, #5
	add	r5, sp, #192
	strb	r3, [r5, #0]
	adds	r6, r5, #0
	movs	r3, #7
	str	r6, [sp, #52]
	mov	r0, r8
	strb	r3, [r6, #1]
	movs	r3, #6
	ldr	r7, [sp, #108]
	str	r3, [r0, #0]
	ldr	r3, [pc, #268]
	str	r7, [r0, #20]
	str	r3, [r0, #8]
	str	r6, [r0, #16]
	ldr	r3, [pc, #264]
	ldr	r1, [sp, #112]
	add	r3, fp
	str	r1, [r0, #12]
	cmp	r3, #130
	bls.n	.L_081852f8
	b.n	.L_0818556a
.L_081852f8:
	movs	r2, #0
	str	r2, [sp, #108]
	str	r2, [sp, #36]
	ldr	r4, [sp, #164]
	mov	r9, r2
.L_08185302:
	movs	r3, #13
	mov	r5, r9
	muls	r5, r3
	ldr	r0, [r4, #24]
	ldr	r2, [pc, #236]
	adds	r3, r5, #0
	adds	r3, r3, r0
	lsls	r3, r3, #1
	ldrh	r3, [r2, r3]
	ldr	r6, [pc, #228]
	movs	r7, #234
	adds	r3, r3, r6
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r7, #255
	adds	r3, r3, r7
	cmp	fp, r3
	beq.n	.L_0818532a
	b.n	.L_081854a6
.L_0818532a:
	ldr	r3, [sp, #36]
	movs	r1, #0
	str	r1, [sp, #104]
	ldr	r2, [pc, #204]
	adds	r1, r3, r0
	lsls	r3, r1, #1
	ldrsh	r2, [r2, r3]
	cmp	r2, #0
	beq.n	.L_08185350
	ldr	r3, [pc, #196]
	ldrb	r3, [r3, r1]
	lsls	r3, r3, #3
	str	r3, [r4, #0]
	ldr	r3, [pc, #192]
	ldrb	r3, [r3, r1]
	lsls	r3, r3, #3
	str	r3, [r4, #4]
	lsls	r3, r2, #8
	str	r3, [r4, #8]
.L_08185350:
	ldr	r6, [sp, #36]
	ldr	r3, [pc, #184]
	adds	r2, r0, r6
	ldrb	r3, [r3, r2]
	lsls	r2, r2, #1
	str	r3, [r4, #16]
	ldr	r3, [pc, #176]
	ldrsh	r3, [r3, r2]
	str	r3, [r4, #20]
	ldr	r3, [pc, #176]
	ldrsh	r2, [r3, r2]
	adds	r3, r0, #1
	str	r3, [r4, #24]
	movs	r3, #1
	negs	r3, r3
	str	r2, [r4, #12]
	cmp	r2, r3
	bne.n	.L_08185376
	b.n	.L_08185556
.L_08185376:
	ldr	r3, [pc, #160]
	ldr	r5, [sp, #144]
	ldrh	r3, [r3, #18]
	ldr	r2, [r4, #0]
	movs	r7, #10
	movs	r0, #20
	adds	r1, r5, r3
	cmp	r2, #0
	bge.n	.L_0818538a
	adds	r2, #7
.L_0818538a:
	ldr	r3, [sp, #128]
	movs	r6, #128
	lsls	r6, r6, #12
	subs	r5, r6, r3
	lsrs	r3, r5, #31
	asrs	r2, r2, #3
	adds	r3, r5, r3
	asrs	r3, r3, #17
	subs	r2, #5
	adds	r6, r2, r3
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bge.n	.L_081853a6
	adds	r3, #7
.L_081853a6:
	asrs	r3, r3, #3
	subs	r3, #10
	mov	ip, r3
	ldr	r2, [sp, #136]
	ldr	r3, [sp, #124]
	str	r7, [sp, #0]
	adds	r2, r2, r3
	asrs	r2, r2, #16
	add	ip, r2
	str	r0, [sp, #4]
	adds	r2, r6, #0
	str	r4, [sp, #8]
	ldr	r0, [sp, #168]
	mov	r3, ip
	ldr	r6, [sp, #156]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x464f
	ldr	r4, [sp, #8]
	cmp	r7, #1
	beq.n	.L_081853e8
	cmp	r7, #1
	bgt.n	.L_081853da
	cmp	r7, #0
	beq.n	.L_081853e2
	b.n	.L_08185426
.L_081853da:
	mov	r0, r9
	cmp	r0, #2
	beq.n	.L_0818541c
	b.n	.L_08185426
.L_081853e2:
	movs	r0, #186
	str	r4, [sp, #8]
	b.n	.L_08185420
.L_081853e8:
	movs	r0, #187
	str	r4, [sp, #8]
	b.n	.L_08185420
	movs	r0, r0
	.4byte 0x08199268
	.4byte 0xfffffe17
	.4byte 0x081998a4
	.4byte 0xfffffe14
	.4byte 0x08199916
	.4byte 0x0819985c
	.4byte 0x08199880
	.4byte 0x081998f2
	.4byte 0x0819995e
	.4byte 0x081999a6
	.2byte 0x7410
	.2byte 0x0819
.L_0818541c:
	movs	r0, #222
	str	r4, [sp, #8]
.L_08185420:
	bl	sub_081c0010
	ldr	r4, [sp, #8]
.L_08185426:
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	ldr	r7, [sp, #164]
	asrs	r3, r3, #1
	str	r3, [sp, #100]
	movs	r1, #3
	mov	sl, r1
	adds	r7, #84
.L_08185436:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	bge.n	.L_0818549a
	ldr	r3, [r4, #0]
	ldr	r2, [sp, #100]
	lsls	r3, r3, #13
	adds	r3, r3, r2
	str	r3, [r7, #0]
	ldr	r5, [sp, #136]
	ldr	r3, [r4, #4]
	ldr	r6, [sp, #124]
	lsls	r3, r3, #13
	adds	r3, r3, r5
	adds	r3, r3, r6
	str	r3, [r7, #4]
	str	r4, [sp, #8]
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08014878
	movs	r5, #255
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #127
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #16
	str	r3, [r7, #24]
	ldr	r0, [sp, #104]
	ldr	r4, [sp, #8]
	adds	r0, #1
	str	r0, [sp, #104]
	cmp	r0, #10
	beq.n	.L_081854a6
.L_0818549a:
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	adds	r7, #28
	cmp	r2, #64
	bne.n	.L_08185436
.L_081854a6:
	ldr	r3, [r4, #12]
	movs	r5, #1
	negs	r5, r5
	cmp	r3, r5
	beq.n	.L_08185556
	ldr	r6, [sp, #108]
	mov	r7, r8
	str	r6, [r7, #20]
	ldr	r0, [sp, #164]
	lsls	r3, r3, #12
	movs	r1, #156
	adds	r3, r0, r3
	lsls	r1, r1, #6
	adds	r3, r3, r1
	str	r3, [sp, #196]
	str	r4, [sp, #8]
	bl	sub_08014de4
	ldr	r4, [sp, #8]
	ldr	r3, [r4, #0]
	cmp	r3, #0
	bge.n	.L_081854d4
	adds	r3, #7
.L_081854d4:
	ldr	r6, [sp, #128]
	movs	r5, #128
	lsls	r5, r5, #12
	subs	r2, r5, r6
	lsrs	r1, r2, #31
	adds	r2, r2, r1
	asrs	r3, r3, #3
	ldr	r1, [r4, #4]
	subs	r3, #64
	lsls	r3, r3, #16
	asrs	r2, r2, #1
	adds	r0, r3, r2
	cmp	r1, #0
	bge.n	.L_081854f2
	adds	r1, #7
.L_081854f2:
	ldr	r7, [sp, #136]
	asrs	r1, r1, #3
	ldr	r2, [sp, #124]
	subs	r1, #64
	lsls	r1, r1, #16
	adds	r1, r1, r7
	adds	r1, r1, r2
	movs	r2, #0
	str	r4, [sp, #8]
	movs	r5, #128
	bl	sub_08015160
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r5, r5, #8
	adds	r1, r2, #0
	adds	r0, r5, #0
	bl	sub_080151e4
	ldr	r4, [sp, #8]
	ldr	r0, [r4, #8]
	bl	sub_080150e4
	movs	r1, #167
	movs	r0, #128
	lsls	r1, r1, #9
	lsls	r0, r0, #11
	adds	r1, #32
	adds	r2, r5, #0
	bl	sub_080151e4
	movs	r2, #4
	ldr	r0, [pc, #404]
	ldr	r1, [sp, #112]
	bl	sub_08196958
	mov	r0, r8
	bl	sub_08196a7c
	ldr	r4, [sp, #8]
	ldr	r2, [r4, #16]
	ldr	r3, [r4, #4]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [r4, #20]
	str	r3, [r4, #4]
	ldr	r3, [r4, #8]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	str	r3, [r4, #8]
.L_08185556:
	ldr	r3, [sp, #36]
	movs	r5, #1
	add	r9, r5
	adds	r3, #12
	mov	r6, r9
	str	r3, [sp, #36]
	adds	r4, #28
	cmp	r6, #3
	beq.n	.L_0818556a
	b.n	.L_08185302
.L_0818556a:
	movs	r7, #128
	lsls	r7, r7, #2
	adds	r7, #90
	cmp	fp, r7
	bne.n	sub_08185582
	movs	r1, #128
	ldr	r3, [pc, #340]
	ldr	r0, [sp, #168]
	lsls	r1, r1, #7
	ldr	r2, [pc, #336]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2080
	lsls	r0, r0, #2
	adds	r0, #118
	cmp	fp, r0
	bne.n	sub_0818559a
	movs	r1, #128
	ldr	r3, [pc, #316]
	ldr	r0, [sp, #168]
	lsls	r1, r1, #7
	ldr	r2, [pc, #316]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2180
	lsls	r1, r1, #2
	adds	r1, #122
	cmp	fp, r1
	bne.n	sub_081855b2
	movs	r1, #128
	ldr	r3, [pc, #292]
	ldr	r0, [sp, #168]
	lsls	r1, r1, #7
	ldr	r2, [pc, #292]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b49
	add	r3, fp
	cmp	r3, #43
	bhi.n	.L_08185656
	mov	r3, fp
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_081855ee
	bl	sub_08014878
	movs	r3, #252
	lsls	r3, r3, #6
	ldr	r5, [sp, #164]
	adds	r3, #255
	movs	r4, #192
	lsls	r4, r4, #7
	ands	r3, r0
	adds	r3, r3, r4
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r1, #3
	bl	sub_0800206c
	movs	r6, #128
	lsls	r0, r0, #12
	lsls	r6, r6, #6
	adds	r0, r0, r6
	str	r0, [r5, #8]
.L_081855ee:
	ldr	r7, [sp, #164]
	movs	r0, #224
	ldr	r3, [r7, #8]
	lsls	r0, r0, #3
	adds	r3, r7, r3
	adds	r3, r3, r0
	str	r3, [sp, #196]
	bl	sub_08014de4
	ldr	r2, [sp, #128]
	movs	r1, #128
	lsls	r1, r1, #12
	subs	r0, r1, r2
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r3, [sp, #136]
	movs	r6, #128
	movs	r4, #204
	lsls	r4, r4, #16
	lsls	r6, r6, #11
	asrs	r0, r0, #1
	adds	r1, r3, r4
	adds	r0, r0, r6
	movs	r2, #0
	bl	sub_08015160
	movs	r5, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r5, r5, #8
	adds	r1, r2, #0
	adds	r0, r5, #0
	bl	sub_080151e4
	ldr	r0, [r7, #4]
	bl	sub_080150e4
	movs	r1, #167
	lsls	r1, r1, #9
	adds	r0, r6, #0
	adds	r1, #32
	adds	r2, r5, #0
	bl	sub_080151e4
	ldr	r0, [pc, #128]
	ldr	r1, [sp, #112]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, r8
	bl	sub_08196a7c
.L_08185656:
	ldr	r3, [pc, #132]
	add	r3, fp
	cmp	r3, #15
	bls.n	.L_08185668
	ldr	r3, [pc, #128]
	add	r3, fp
	cmp	r3, #3
	bls.n	.L_08185668
	b.n	.L_081857a0
.L_08185668:
	movs	r5, #1
	str	r5, [sp, #96]
	bl	sub_08014878
	movs	r1, #3
	bl	sub_0800206c
	movs	r7, #178
	movs	r6, #160
	lsls	r7, r7, #1
	lsls	r6, r6, #11
	adds	r7, #255
	mov	sl, r6
	str	r0, [sp, #92]
	cmp	fp, r7
	ble.n	.L_0818569c
	movs	r3, #241
	lsls	r3, r3, #8
	adds	r3, #149
	mov	r0, fp
	muls	r0, r3
	ldr	r1, [pc, #80]
	adds	r3, r0, #0
	subs	r3, r6, r3
	adds	r1, r1, r3
	mov	sl, r1
.L_0818569c:
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	sl, r2
	bgt.n	.L_081856ac
	movs	r3, #128
	lsls	r3, r3, #9
	mov	sl, r3
.L_081856ac:
	movs	r4, #179
	lsls	r4, r4, #1
	adds	r4, #255
	cmp	fp, r4
	bgt.n	.L_081856f6
	movs	r5, #175
	lsls	r5, r5, #1
	adds	r5, #255
	cmp	fp, r5
	ble.n	.L_081856e8
	movs	r6, #3
	str	r6, [sp, #96]
	b.n	.L_081856f6
	movs	r0, r0
	.4byte 0x081991d0
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x1f1f1f1f
	.4byte 0xfffffdd8
	.4byte 0xfffffda6
	.4byte 0xfffffd8a
	.2byte 0x8834
	.2byte 0x0241
.L_081856e8:
	movs	r7, #174
	lsls	r7, r7, #1
	adds	r7, #255
	cmp	fp, r7
	ble.n	.L_081856f6
	movs	r0, #2
	str	r0, [sp, #96]
.L_081856f6:
	movs	r3, #6
	mov	r1, r8
	str	r3, [r1, #0]
	ldr	r2, [sp, #108]
	movs	r3, #0
	str	r2, [r1, #20]
	ldr	r4, [sp, #96]
	mov	r9, r3
	cmp	r4, #0
	beq.n	.L_081857a0
	ldr	r5, [sp, #164]
	movs	r7, #128
	movs	r6, #128
	lsls	r7, r7, #9
	lsls	r6, r6, #8
.L_08185714:
	bl	sub_08014878
	movs	r1, #8
	bl	sub_0800206c
	movs	r1, #4
	subs	r0, r0, r1
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	ldr	r2, [sp, #136]
	movs	r4, #200
	lsls	r4, r4, #16
	adds	r3, r2, r4
	str	r3, [r5, #4]
	ldr	r0, [sp, #92]
	movs	r1, #3
	add	r0, r9
	bl	sub_08002064
	movs	r1, #128
	lsls	r1, r1, #6
	lsls	r0, r0, #12
	adds	r0, r0, r1
	str	r0, [r5, #8]
	ldr	r2, [sp, #164]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r0, r2, r0
	adds	r0, r0, r3
	str	r0, [sp, #196]
	bl	sub_08014de4
	ldr	r1, [r5, #4]
	adds	r0, r7, #0
	movs	r2, #0
	bl	sub_08015160
	mov	r0, sl
	adds	r1, r7, #0
	adds	r2, r6, #0
	bl	sub_080151e4
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl	sub_08015160
	movs	r1, #128
	lsls	r1, r1, #10
	adds	r2, r7, #0
	adds	r0, r6, #0
	bl	sub_080151e4
	adds	r0, r6, #0
	bl	sub_080150e4
	ldr	r0, [pc, #892]
	ldr	r1, [sp, #112]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, r8
	bl	sub_08196a7c
	ldr	r0, [sp, #96]
	movs	r4, #1
	add	r9, r4
	adds	r5, #28
	cmp	r9, r0
	bne.n	.L_08185714
.L_081857a0:
	movs	r1, #138
	lsls	r1, r1, #2
	cmp	fp, r1
	bne.n	.L_081857b4
	ldr	r0, [pc, #860]
	ldr	r1, [pc, #864]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
.L_081857b4:
	movs	r2, #148
	lsls	r2, r2, #1
	adds	r2, #255
	cmp	fp, r2
	bne.n	.L_081857e2
	ldr	r3, [sp, #164]
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r1, r3, r4
	ldr	r0, [pc, #840]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r5, [sp, #164]
	movs	r6, #184
	lsls	r6, r6, #5
	ldr	r0, [pc, #828]
	adds	r1, r5, r6
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
.L_081857e2:
	movs	r7, #128
	lsls	r7, r7, #2
	adds	r7, #90
	cmp	fp, r7
	bne.n	.L_081857f8
	ldr	r0, [pc, #808]
	ldr	r1, [pc, #796]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
.L_081857f8:
	ldr	r3, [pc, #800]
	add	r3, fp
	cmp	r3, #6
	bhi.n	.L_0818580a
	ldr	r0, [sp, #136]
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r0, r0, r1
	str	r0, [sp, #136]
.L_0818580a:
	ldr	r0, [pc, #788]
	add	r0, fp
	cmp	r0, #58
	bls.n	.L_08185814
	b.n	.L_08185a34
.L_08185814:
	movs	r3, #136
	lsls	r3, r3, #5
	adds	r3, #72
	adds	r7, r0, #0
	muls	r7, r3
	ldr	r3, [pc, #772]
	movs	r2, #16
	negs	r2, r2
	str	r2, [sp, #108]
	cmp	r7, r3
	ble.n	.L_0818582c
	ldr	r7, [pc, #760]
.L_0818582c:
	add	r4, sp, #192
	adds	r5, r4, #0
	movs	r3, #6
	strb	r3, [r4, #0]
	mov	r0, r8
	str	r5, [sp, #52]
	strb	r3, [r5, #1]
	movs	r3, #7
	ldr	r6, [sp, #108]
	str	r3, [r0, #0]
	ldr	r3, [pc, #740]
	str	r6, [r0, #20]
	str	r5, [r0, #16]
	str	r3, [r0, #8]
	ldr	r1, [sp, #112]
	movs	r4, #184
	str	r1, [r0, #12]
	ldr	r2, [sp, #164]
	lsls	r4, r4, #5
	adds	r3, r2, r4
	str	r3, [r5, #4]
	bl	sub_08014de4
	ldr	r6, [sp, #128]
	movs	r5, #128
	lsls	r5, r5, #12
	subs	r5, r5, r6
	ldr	r2, [sp, #136]
	lsrs	r0, r5, #31
	adds	r0, r5, r0
	movs	r1, #160
	movs	r3, #230
	lsls	r3, r3, #16
	lsls	r1, r1, #11
	asrs	r0, r0, #1
	adds	r0, r0, r1
	adds	r1, r2, r3
	movs	r2, #0
	str	r5, [sp, #88]
	bl	sub_08015160
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r1, #128
	adds	r0, r2, #0
	lsls	r1, r1, #9
	bl	sub_080151e4
	movs	r0, #146
	lsls	r0, r0, #7
	adds	r0, #80
	bl	sub_08015024
	mov	r4, fp
	negs	r5, r4
	lsls	r5, r5, #10
	adds	r0, r5, #0
	bl	sub_080150e4
	adds	r0, r7, #0
	bl	sub_0801521c
	movs	r0, #0
	ldr	r1, [pc, #640]
	movs	r2, #0
	bl	sub_08015160
	ldr	r0, [pc, #636]
	ldr	r1, [sp, #112]
	movs	r2, #4
	bl	sub_08196958
	movs	r1, #240
	lsls	r1, r1, #12
	movs	r2, #0
	movs	r0, #0
	bl	sub_08015160
	mov	r0, r8
	bl	sub_08196a7c
	ldr	r6, [sp, #164]
	movs	r0, #224
	ldr	r1, [sp, #52]
	lsls	r0, r0, #3
	adds	r3, r6, r0
	str	r3, [r1, #4]
	ldr	r3, [sp, #28]
	movs	r4, #168
	lsls	r4, r4, #6
	movs	r6, #168
	movs	r2, #0
	adds	r4, #170
	lsls	r6, r6, #6
	mov	r9, r2
	mov	sl, r3
	adds	r5, r5, r4
	adds	r6, #170
.L_081858f0:
	bl	sub_08014e38
	adds	r0, r6, #0
	bl	sub_080150e4
	adds	r0, r5, #0
	bl	sub_08002090
	movs	r3, #24
	lsls	r0, r0, #3
	asrs	r0, r0, #16
	negs	r3, r3
	subs	r3, r3, r0
	mov	r0, r8
	str	r3, [r0, #20]
	ldr	r1, [pc, #548]
	movs	r0, #0
	movs	r2, #0
	bl	sub_08015160
	movs	r1, #162
	lsls	r1, r1, #1
	adds	r1, #255
	cmp	fp, r1
	ble.n	.L_08185938
	ldr	r0, [pc, #532]
	movs	r2, #154
	add	r0, sl
	lsls	r2, r2, #7
	cmp	r0, r2
	ble.n	.L_08185932
	movs	r0, #154
	lsls	r0, r0, #7
.L_08185932:
	negs	r0, r0
	bl	sub_08015024
.L_08185938:
	ldr	r1, [sp, #112]
	movs	r2, #4
	ldr	r0, [pc, #508]
	bl	sub_08196958
	mov	r0, r8
	bl	sub_08196a7c
	bl	sub_08014ea8
	movs	r3, #170
	movs	r4, #1
	lsls	r3, r3, #7
	add	r9, r4
	adds	r3, #85
	mov	r0, r9
	adds	r5, r5, r3
	adds	r6, r6, r3
	cmp	r0, #3
	bne.n	.L_081858f0
	movs	r2, #167
	lsls	r2, r2, #1
	adds	r2, #255
	cmp	fp, r2
	ble.n	.L_0818597c
	movs	r3, #172
	lsls	r3, r3, #6
	adds	r3, #52
	mov	r1, fp
	muls	r1, r3
	ldr	r4, [pc, #456]
	adds	r3, r1, #0
	adds	r3, r7, r3
	adds	r7, r3, r4
.L_0818597c:
	cmp	r7, #0
	ble.n	.L_08185a34
	movs	r5, #173
	lsls	r5, r5, #1
	adds	r5, #255
	cmp	fp, r5
	bgt.n	.L_08185a3e
	movs	r6, #16
	negs	r6, r6
	str	r6, [sp, #108]
	cmp	fp, r2
	ble.n	.L_081859a4
	mov	r0, fp
	movs	r1, #128
	lsls	r3, r0, #1
	lsls	r1, r1, #3
	subs	r3, r6, r3
	adds	r1, #156
	adds	r1, r3, r1
	str	r1, [sp, #108]
.L_081859a4:
	ldr	r2, [sp, #108]
	movs	r3, #64
	negs	r3, r3
	cmp	r2, r3
	ble.n	.L_08185a34
	ldr	r3, [pc, #348]
	movs	r5, #149
	mov	r4, r8
	lsls	r5, r5, #1
	str	r2, [r4, #20]
	adds	r5, #255
	str	r3, [sp, #196]
	bl	sub_08014de4
	cmp	fp, r5
	ble.n	.L_081859ec
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #42
	mov	r6, fp
	subs	r3, r3, r6
	lsls	r2, r3, #5
	subs	r2, r2, r3
	lsls	r2, r2, #2
	adds	r2, r2, r3
	lsls	r3, r2, #4
	ldr	r0, [pc, #360]
	subs	r3, r3, r2
	lsls	r1, r3, #6
	cmp	r1, r0
	bge.n	.L_081859e4
	ldr	r1, [pc, #352]
.L_081859e4:
	movs	r0, #0
	movs	r2, #0
	bl	sub_08015160
.L_081859ec:
	ldr	r1, [sp, #88]
	ldr	r3, [sp, #136]
	lsrs	r0, r1, #31
	adds	r0, r1, r0
	movs	r2, #160
	movs	r4, #220
	lsls	r4, r4, #16
	lsls	r2, r2, #11
	asrs	r0, r0, #1
	adds	r1, r3, r4
	adds	r0, r0, r2
	movs	r2, #0
	bl	sub_08015160
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r1, #250
	adds	r0, r2, #0
	lsls	r1, r1, #7
	mov	r5, fp
	bl	sub_080151e4
	lsls	r0, r5, #11
	bl	sub_080150e4
	adds	r0, r7, #0
	bl	sub_0801521c
	ldr	r0, [pc, #264]
	ldr	r1, [sp, #112]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, r8
	bl	sub_08196a7c
.L_08185a34:
	movs	r6, #173
	lsls	r6, r6, #1
	adds	r6, #255
	cmp	fp, r6
	ble.n	.L_08185ab8
.L_08185a3e:
	movs	r0, #178
	lsls	r0, r0, #1
	movs	r7, #0
	adds	r0, #255
	str	r7, [sp, #108]
	cmp	fp, r0
	ble.n	.L_08185a5a
	mov	r1, fp
	lsls	r3, r1, #3
	movs	r2, #153
	negs	r3, r3
	lsls	r2, r2, #5
	adds	r2, r3, r2
	str	r2, [sp, #108]
.L_08185a5a:
	ldr	r3, [sp, #108]
	movs	r4, #64
	negs	r4, r4
	cmp	r3, r4
	ble.n	.L_08185ab8
	mov	r5, r8
	str	r3, [r5, #20]
	ldr	r3, [pc, #220]
	movs	r2, #7
	str	r3, [r5, #8]
	str	r2, [r5, #0]
	ldr	r6, [sp, #52]
	ldr	r3, [pc, #152]
	movs	r5, #128
	str	r3, [r6, #4]
	add	r3, sp, #192
	strb	r2, [r3, #0]
	lsls	r5, r5, #9
	strb	r2, [r6, #1]
	bl	sub_08014de4
	adds	r0, r5, #0
	ldr	r1, [pc, #172]
	movs	r2, #0
	bl	sub_08015160
	movs	r1, #250
	lsls	r1, r1, #7
	adds	r2, r5, #0
	adds	r0, r5, #0
	mov	r7, fp
	bl	sub_080151e4
	lsls	r0, r7, #10
	bl	sub_080150e4
	ldr	r0, [sp, #32]
	bl	sub_0801521c
	ldr	r0, [pc, #132]
	ldr	r1, [sp, #112]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, r8
	bl	sub_08196a7c
.L_08185ab8:
	mov	r0, r8
	bl	sub_08013164
	ldr	r0, [sp, #112]
	bl	sub_08013164
	ldr	r2, [pc, #132]
	add	r2, fp
	cmp	r2, #51
	bhi.n	.L_08185b6e
	lsrs	r3, r2, #31
	ldr	r6, [pc, #128]
	adds	r3, r2, r3
	movs	r0, #150
	asrs	r3, r3, #1
	movs	r2, #90
	lsls	r0, r0, #2
	add	r6, fp
	subs	r5, r2, r3
	cmp	fp, r0
	ble.n	.L_08185aec
	mov	r1, fp
	ldr	r2, [pc, #108]
	lsls	r3, r1, #4
	adds	r3, r5, r3
	adds	r5, r3, r2
.L_08185aec:
	cmp	r5, #63
	bgt.n	.L_08185af2
	movs	r5, #64
.L_08185af2:
	cmp	r6, #38
	bgt.n	.L_08185b60
	ldr	r0, [pc, #96]
	adds	r1, r6, #0
	ldr	r2, [pc, #96]
	bl	sub_0815b434
	b.n	.L_08185b62
	movs	r0, r0
	.4byte 0x081991d0
	.4byte 0x000000b4
	.4byte 0x02012000
	.4byte 0x000000ee
	.4byte 0x000000ef
	.4byte 0x000000c2
	.4byte 0xfffffda6
	.4byte 0xfffffdd8
	.4byte 0x00022538
	.4byte 0x08199340
	.4byte 0xfff10000
	.4byte 0x081991e0
	.4byte 0xfff00000
	.4byte 0xfff64ce0
	.4byte 0x081991f0
	.4byte 0xff9c6e28
	.4byte 0xfff32ec0
	.4byte 0x08199364
	.4byte 0xfffffdd6
	.4byte 0xfffffdd7
	.4byte 0xffffda70
	.4byte 0x02010000
	.2byte 0x7530
	.2byte 0x0001
.L_08185b60:
	movs	r6, #38
.L_08185b62:
	ldr	r0, [pc, #360]
	movs	r1, #62
	adds	r2, r5, #0
	adds	r3, r6, #0
	bl	sub_0818caa8
.L_08185b6e:
	ldr	r4, [sp, #164]
	movs	r5, #240
	lsls	r5, r5, #7
	adds	r5, #232
	adds	r3, r4, r5
	movs	r6, #1
	str	r6, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #188
	ldr	r7, [sp, #32]
	ldr	r1, [sp, #28]
	lsls	r0, r0, #6
	movs	r2, #137
	adds	r0, #160
	lsls	r2, r2, #3
	adds	r7, r7, r0
	adds	r1, r1, r2
	str	r7, [sp, #32]
	str	r1, [sp, #28]
	movs	r3, #1
	add	fp, r3
.L_08185b9c:
	movs	r4, #191
	lsls	r4, r4, #1
	adds	r4, #255
	cmp	fp, r4
	beq.n	.L_08185bc8
	mov	r5, fp
	cmp	r5, #15
	bgt.n	.L_08185bb8
	lsls	r2, r5, #1
	subs	r2, #30
	adds	r0, r2, #0
	adds	r1, r2, #0
	bl	sub_08164b2c
.L_08185bb8:
	ldr	r3, [pc, #276]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08185bc8
	bl	sub_081848ec
.L_08185bc8:
	ldr	r7, [sp, #164]
	movs	r0, #238
	lsls	r0, r0, #7
	movs	r6, #0
	adds	r0, #220
	mov	r9, r6
	adds	r5, r7, r0
.L_08185bd6:
	ldmia	r5!, {r0}
	bl	sub_08020048
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	cmp	r2, #50
	bne.n	.L_08185bd6
	bl	sub_08020388
	movs	r0, #128
	bl	sub_080143e0
	ldr	r3, [sp, #140]
	movs	r5, #240
	str	r0, [r3, #84]
	ldr	r4, [sp, #164]
	lsls	r5, r5, #7
	adds	r5, #240
	adds	r3, r4, r5
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	ldr	r7, [sp, #172]
	movs	r6, #0
	ldr	r3, [r7, #20]
	mov	r9, r6
	cmp	r3, #0
	beq.n	.L_08185c44
	subs	r6, #1
	movs	r7, #0
	movs	r5, #36
.L_08185c16:
	ldr	r1, [sp, #172]
	adds	r3, r6, #0
	ldrsh	r0, [r5, r1]
	movs	r1, #1
	adds	r2, r6, #0
	str	r7, [sp, #0]
	bl	sub_0814cd48
	ldr	r3, [sp, #172]
	movs	r1, #0
	ldrsh	r0, [r5, r3]
	adds	r2, r6, #0
	adds	r3, r6, #0
	str	r7, [sp, #0]
	bl	sub_0814cd48
	ldr	r1, [sp, #172]
	movs	r0, #1
	ldr	r3, [r1, #20]
	add	r9, r0
	adds	r5, #2
	cmp	r9, r3
	bne.n	.L_08185c16
.L_08185c44:
	bl	sub_08014c4c
	bl	sub_0814cca8
	ldr	r2, [pc, #132]
	movs	r3, #240
	str	r3, [r2, #16]
	ldr	r2, [sp, #164]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #180
	adds	r3, r2, r4
	movs	r2, #0
	str	r2, [r3, #0]
	ldr	r5, [sp, #164]
	movs	r6, #238
	lsls	r6, r6, #7
	adds	r6, #184
	adds	r3, r5, r6
	str	r2, [r3, #0]
	ldr	r2, [pc, #104]
	movs	r3, #32
	strh	r3, [r2, #6]
	ldr	r1, [pc, #104]
	movs	r0, #1
	movs	r2, #0
	bl	sub_08118040
	movs	r2, #16
	negs	r2, r2
	adds	r1, r2, #0
	movs	r0, #0
	bl	sub_08164b2c
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	movs	r7, #240
	strh	r3, [r2, #0]
	ldr	r0, [pc, #72]
	ldr	r1, [pc, #52]
	movs	r2, #1
	movs	r3, #1
	lsls	r7, r7, #4
	bl	sub_08157cf4
	adds	r1, r5, r7
	movs	r2, #1
	ldr	r0, [pc, #60]
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #216
	lsls	r1, r1, #5
	adds	r1, #86
	adds	r0, r5, r1
	movs	r1, #128
	ldr	r3, [pc, #44]
	lsls	r1, r1, #7
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x23d8
	lsls	r3, r3, #5
	b.n	.L_08185cec
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x02010000
	.4byte 0x03001150
	.4byte 0x030011e0
	.4byte 0x03001120
	.4byte 0x00000075
	.4byte 0x00000188
	.4byte 0x00000192
	.2byte 0x0258
	.2byte 0x0300
.L_08185cec:
	movs	r2, #0
	adds	r3, #90
	mov	r9, r2
	movs	r7, #0
	mov	ip, r3
.L_08185cf6:
	ldr	r0, [sp, #164]
	mov	r4, r9
	lsls	r3, r4, #12
	movs	r6, #0
	lsls	r5, r7, #6
	adds	r4, r3, r0
.L_08185d02:
	ldr	r3, [pc, #300]
	mov	r2, ip
	adds	r1, r4, r2
	movs	r0, #0
	adds	r2, r5, r3
.L_08185d0c:
	ldrb	r3, [r2, #0]
	adds	r0, #1
	strb	r3, [r1, #0]
	adds	r2, #1
	adds	r1, #1
	cmp	r0, #24
	bne.n	.L_08185d0c
	adds	r6, #1
	adds	r5, #24
	adds	r4, #32
	cmp	r6, #120
	bne.n	.L_08185d02
	movs	r4, #1
	add	r9, r4
	mov	r5, r9
	adds	r7, #45
	cmp	r5, #4
	bne.n	.L_08185cf6
	ldr	r0, [pc, #256]
	ldr	r1, [pc, #252]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #248]
	ldr	r1, [pc, #252]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #244]
	ldr	r1, [pc, #248]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r7, [sp, #164]
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	movs	r6, #0
	adds	r2, r7, r0
	movs	r3, #2
	adds	r1, #132
	str	r6, [sp, #84]
	str	r3, [r2, #0]
	adds	r2, r7, r1
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r3, [pc, #212]
	movs	r2, #3
	ldr	r3, [r3, #12]
	mov	fp, r6
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08185d80
	b.n	.L_08186536
.L_08185d80:
	mov	r2, sp
	adds	r2, #184
	str	r2, [sp, #56]
	str	r6, [sp, #20]
.L_08185d88:
	mov	r3, fp
	cmp	r3, #0
	bne.n	sub_08185dd8
	movs	r1, #128
	ldr	r3, [pc, #184]
	ldr	r0, [sp, #168]
	lsls	r1, r1, #7
	movs	r2, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4c2c
	movs	r2, #16
	negs	r2, r2
	adds	r1, r2, #0
	movs	r0, #0
	str	r4, [sp, #84]
	bl	sub_08164b2c
	ldr	r5, [sp, #164]
	movs	r6, #239
	movs	r7, #238
	lsls	r6, r6, #7
	lsls	r7, r7, #7
	adds	r2, r5, r6
	movs	r3, #2
	adds	r7, #132
	str	r3, [r2, #0]
	adds	r2, r5, r7
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r0, [pc, #140]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #136]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9815
	ldr	r1, [pc, #128]
	cmp	r0, r1
	bgt.n	.L_08185de8
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r0, r0, r2
	str	r0, [sp, #84]
.L_08185de8:
	mov	r0, fp
	subs	r0, #96
	cmp	r0, #23
	bhi.n	.L_08185e0c
	mov	r2, fp
	movs	r1, #16
	subs	r2, #112
	negs	r0, r0
	negs	r1, r1
	bl	sub_08164b2c
	movs	r4, #238
	ldr	r3, [sp, #164]
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	movs	r3, #8
	str	r3, [r2, #0]
.L_08185e0c:
	mov	r5, fp
	cmp	r5, #139
	ble.n	.L_08185e7c
	ldr	r6, [sp, #164]
	movs	r7, #239
	lsls	r7, r7, #7
	adds	r2, r6, r7
	movs	r3, #1
	str	r3, [r2, #0]
	cmp	r5, #153
	ble.n	.L_08185e64
	movs	r0, #238
	lsls	r0, r0, #7
	ldr	r3, [pc, #56]
	adds	r0, #132
	adds	r2, r6, r0
	b.n	.L_08185e70
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x000000ee
	.4byte 0x000000ef
	.4byte 0x02011000
	.4byte 0x000000c2
	.4byte 0x02012000
	.4byte 0x03001150
	.4byte 0x03000260
	.4byte 0xffc00000
	.4byte 0x00000149
	.4byte 0x03000730
	.4byte 0x000fffff
	.2byte 0x3f3f
	.2byte 0x3f3f
.L_08185e64:
	ldr	r1, [sp, #164]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r1, r3
	ldr	r3, [pc, #60]
.L_08185e70:
	str	r3, [r2, #0]
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	bl	sub_08164a4c
.L_08185e7c:
	mov	r4, fp
	cmp	r4, #80
	beq.n	.L_08185e86
	cmp	r4, #83
	bne.n	.L_08185eb0
.L_08185e86:
	movs	r2, #160
	ldr	r1, [pc, #28]
	lsls	r2, r2, #19
	movs	r5, #0
	adds	r2, #192
	mov	r9, r5
.L_08185e92:
	ldrh	r3, [r2, #0]
	movs	r6, #1
	add	r9, r6
	eors	r3, r1
	mov	r7, r9
	strh	r3, [r2, #0]
	adds	r2, #2
	cmp	r7, #128
	bne.n	.L_08185e92
	b.n	.L_08185eb0
	movs	r0, r0
	.4byte 0x00007fff
	.2byte 0x1010
	.2byte 0x1010
.L_08185eb0:
	mov	r0, fp
	cmp	r0, #82
	bne.n	sub_08185ed8
	ldr	r1, [sp, #164]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	adds	r3, r1, r2
	movs	r2, #8
	str	r2, [r3, #0]
	movs	r0, #208
	bl	sub_081c0010
	ldr	r3, [pc, #80]
	movs	r1, #128
	ldr	r0, [sp, #168]
	lsls	r1, r1, #7
	ldr	r2, [pc, #76]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x465b
	cmp	r3, #96
	bne.n	.L_08185ee4
	movs	r0, #104
	bl	sub_081c0010
.L_08185ee4:
	mov	r4, fp
	cmp	r4, #128
	bne.n	.L_08185f24
	ldr	r5, [sp, #164]
	movs	r6, #238
	lsls	r6, r6, #7
	adds	r6, #168
	adds	r2, r5, r6
	movs	r3, #16
	str	r3, [r2, #0]
	ldr	r1, [pc, #28]
	movs	r2, #160
	lsls	r2, r2, #19
	movs	r7, #0
	adds	r2, #192
	mov	r9, r7
.L_08185f04:
	ldrh	r3, [r2, #0]
	movs	r0, #1
	eors	r3, r1
	add	r9, r0
	strh	r3, [r2, #0]
	mov	r3, r9
	adds	r2, #2
	cmp	r3, #128
	bne.n	.L_08185f04
	b.n	.L_08185f24
	.4byte 0x00007fff
	.4byte 0x03000260
	.2byte 0x3f3f
	.2byte 0x3f3f
.L_08185f24:
	mov	r4, fp
	cmp	r4, #128
	ble.n	.L_08185f40
	mov	r3, fp
	subs	r3, #128
	lsls	r3, r3, #1
	adds	r0, r3, #0
	mov	r1, fp
	movs	r2, #3
	subs	r0, #11
	subs	r1, #136
	subs	r2, r2, r3
	bl	sub_08164b2c
.L_08185f40:
	ldr	r6, [sp, #84]
	mov	r0, fp
	movs	r7, #16
	asrs	r5, r6, #16
	cmp	r0, #92
	ble.n	.L_08185f50
	mov	r7, fp
	subs	r7, #75
.L_08185f50:
	mov	r1, fp
	cmp	r1, #79
	bgt.n	.L_08185f62
	lsls	r0, r1, #11
	bl	sub_08002096
	lsls	r0, r0, #1
	asrs	r0, r0, #16
	adds	r7, r7, r0
.L_08185f62:
	cmp	r7, #64
	bgt.n	.L_08185f78
	ldr	r2, [sp, #164]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r0, r2, r3
	adds	r1, r7, #0
	ldr	r2, [pc, #628]
	bl	sub_0815b434
	b.n	.L_08185f7a
.L_08185f78:
	movs	r7, #64
.L_08185f7a:
	lsrs	r3, r7, #31
	adds	r3, r7, r3
	ldr	r4, [sp, #164]
	asrs	r3, r3, #1
	subs	r3, r5, r3
	movs	r5, #224
	adds	r3, #60
	lsls	r5, r5, #3
	str	r3, [sp, #80]
	adds	r2, r3, #0
	adds	r0, r4, r5
	movs	r1, #60
	adds	r3, r7, #0
	mov	r6, fp
	bl	sub_0818caa8
	cmp	r6, #95
	ble.n	.L_08186042
	movs	r0, #0
	mov	r9, r0
.L_08185fa2:
	movs	r1, #3
	mov	r4, r9
	ands	r4, r1
	str	r4, [sp, #8]
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08002096
	ldr	r2, [pc, #564]
	ldr	r4, [sp, #8]
	adds	r6, r7, #0
	muls	r6, r0
	ldrb	r3, [r2, r4]
	asrs	r6, r6, #17
	lsrs	r3, r3, #1
	adds	r0, r5, #0
	adds	r6, #60
	mov	sl, r2
	subs	r6, r6, r3
	bl	sub_08002090
	adds	r5, r7, #0
	muls	r5, r0
	ldr	r3, [sp, #80]
	ldr	r4, [sp, #8]
	ldr	r0, [pc, #536]
	asrs	r5, r5, #17
	adds	r5, r3, r5
	ldrb	r3, [r0, r4]
	mov	r8, r0
	lsrs	r3, r3, #1
	subs	r5, r5, r3
	bl	sub_08014878
	ldr	r3, [pc, #520]
	movs	r1, #3
	ands	r0, r1
	ldrb	r2, [r3, r0]
	movs	r3, #3
	orrs	r3, r2
	movs	r2, #0
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r0, #188
	bl	sub_08196404
	ldr	r4, [sp, #8]
	ldr	r2, [pc, #496]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #164]
	movs	r3, #240
	adds	r1, r2, r1
	lsls	r3, r3, #4
	mov	r0, sl
	adds	r1, r1, r3
	ldrb	r3, [r0, r4]
	mov	r2, r8
	str	r3, [sp, #0]
	ldr	r0, [sp, #168]
	ldrb	r3, [r2, r4]
	adds	r2, r6, #0
	str	r3, [sp, #4]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r4, [r3, #0]
	adds	r3, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r4, #1
	add	r9, r4
	mov	r5, r9
	cmp	r5, #4
	bne.n	.L_08185fa2
.L_08186042:
	movs	r0, #128
	lsls	r0, r0, #2
	bl	sub_08014dac
	str	r0, [sp, #76]
	movs	r0, #1
	bl	sub_081969f8
	mov	r6, fp
	adds	r7, r0, #0
	cmp	r6, #127
	ble.n	.L_0818605c
	b.n	.L_08186198
.L_0818605c:
	movs	r6, #167
	lsls	r6, r6, #9
	mov	r0, fp
	adds	r6, #32
	cmp	r0, #71
	ble.n	.L_0818606e
	ldr	r1, [pc, #400]
	lsls	r3, r0, #11
	adds	r6, r3, r1
.L_0818606e:
	movs	r2, #16
	negs	r2, r2
	mov	r3, fp
	str	r2, [sp, #72]
	cmp	r3, #119
	ble.n	.L_08186084
	lsls	r2, r3, #3
	movs	r3, #236
	lsls	r3, r3, #2
	subs	r3, r3, r2
	str	r3, [sp, #72]
.L_08186084:
	ldr	r4, [pc, #376]
	cmp	r6, r4
	ble.n	.L_0818608c
	ldr	r6, [pc, #372]
.L_0818608c:
	ldr	r3, [sp, #184]
	ldr	r2, [pc, #372]
	movs	r1, #6
	ands	r3, r2
	ldr	r2, [pc, #368]
	ldr	r5, [sp, #72]
	orrs	r3, r1
	ands	r3, r2
	movs	r2, #192
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r5, [r7, #20]
	str	r1, [r7, #0]
	ldr	r0, [sp, #56]
	str	r3, [sp, #184]
	ldr	r3, [pc, #352]
	str	r0, [r7, #16]
	str	r3, [r7, #8]
	ldr	r1, [sp, #76]
	ldr	r3, [pc, #348]
	str	r1, [r7, #12]
	str	r3, [r0, #4]
	bl	sub_08014de4
	ldr	r0, [pc, #340]
	ldr	r1, [sp, #84]
	movs	r2, #0
	bl	sub_08015160
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r1, #128
	lsls	r1, r1, #9
	adds	r0, r2, #0
	bl	sub_080151e4
	movs	r0, #146
	lsls	r0, r0, #7
	adds	r0, #80
	bl	sub_08015024
	mov	r2, fp
	lsls	r5, r2, #9
	adds	r0, r5, #0
	bl	sub_080150e4
	adds	r0, r6, #0
	bl	sub_0801521c
	movs	r0, #0
	ldr	r1, [pc, #292]
	movs	r2, #0
	bl	sub_08015160
	ldr	r0, [pc, #288]
	ldr	r1, [sp, #76]
	movs	r2, #4
	bl	sub_08196958
	movs	r1, #240
	movs	r0, #0
	lsls	r1, r1, #12
	movs	r2, #0
	bl	sub_08015160
	adds	r0, r7, #0
	bl	sub_08196a7c
	ldr	r3, [pc, #264]
	ldr	r4, [sp, #56]
	movs	r0, #168
	lsls	r0, r0, #6
	str	r3, [r4, #4]
	movs	r6, #0
	adds	r0, #170
	mov	r9, r6
	mov	r8, r0
	adds	r6, r5, r0
.L_08186128:
	adds	r0, r6, #0
	bl	sub_08002090
	ldr	r1, [sp, #72]
	adds	r5, r0, #0
	lsls	r5, r5, #3
	asrs	r5, r5, #16
	adds	r5, r1, r5
	subs	r5, #16
	bl	sub_08014e38
	mov	r0, r8
	bl	sub_080150e4
	movs	r0, #0
	movs	r2, #0
	str	r5, [r7, #20]
	ldr	r1, [pc, #216]
	bl	sub_08015160
	movs	r0, #154
	mov	r2, fp
	lsls	r0, r0, #7
	cmp	r2, #63
	ble.n	.L_08186164
	lsls	r3, r2, #10
	movs	r4, #128
	subs	r3, r0, r3
	lsls	r4, r4, #9
	adds	r0, r3, r4
.L_08186164:
	cmp	r0, #0
	bge.n	.L_0818616a
	movs	r0, #0
.L_0818616a:
	negs	r0, r0
	bl	sub_08015024
	movs	r2, #4
	ldr	r1, [sp, #76]
	ldr	r0, [pc, #176]
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
	movs	r5, #170
	bl	sub_08014ea8
	movs	r0, #1
	lsls	r5, r5, #7
	add	r9, r0
	adds	r5, #85
	mov	r1, r9
	adds	r6, r6, r5
	add	r8, r5
	cmp	r1, #3
	bne.n	.L_08186128
.L_08186198:
	ldr	r3, [sp, #20]
	movs	r2, #0
	mov	r9, r2
	ldr	r1, [pc, #140]
	lsls	r2, r3, #7
	subs	r2, r2, r3
	ldr	r4, [pc, #136]
	lsls	r3, r2, #4
	lsls	r2, r2, #5
	adds	r1, r1, r2
	mov	r5, fp
	movs	r2, #80
	adds	r4, r4, r3
	movs	r0, #160
	lsls	r3, r5, #3
	str	r2, [sp, #40]
	negs	r3, r3
	lsls	r0, r0, #2
	mov	sl, r4
	adds	r6, r3, r0
	mov	r8, r1
.L_081861c2:
	ldr	r5, [sp, #40]
	mov	r4, r9
	lsls	r3, r4, #3
	cmp	fp, r5
	blt.n	.L_081862aa
	movs	r0, #0
	str	r0, [sp, #72]
	cmp	r4, #0
	bne.n	.L_08186234
	movs	r5, #128
	lsls	r5, r5, #6
	adds	r3, #96
	add	r5, r8
	cmp	fp, r3
	blt.n	.L_08186248
	adds	r1, r6, #0
	adds	r1, #128
	str	r1, [sp, #72]
	b.n	.L_08186248
	.4byte 0x00017530
	.4byte 0x08197492
	.4byte 0x08197498
	.4byte 0x081999ee
	.4byte 0x08197486
	.4byte 0xffff0e20
	.4byte 0x00022538
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199340
	.4byte 0x02011000
	.4byte 0xfffe0000
	.4byte 0xfff10000
	.4byte 0x081991e0
	.4byte 0x02010000
	.4byte 0xfff00000
	.4byte 0x081991f0
	.4byte 0xfff11e00
	.2byte 0x8f00
	.2byte 0xfff8
.L_08186234:
	.2byte 0x2580
	lsls	r5, r5, #6
	adds	r3, #112
	add	r5, sl
	cmp	fp, r3
	blt.n	.L_08186248
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r2, r6, r2
	str	r2, [sp, #72]
.L_08186248:
	ldr	r3, [sp, #72]
	movs	r4, #64
	negs	r4, r4
	cmp	r3, r4
	ble.n	.L_081862aa
	str	r3, [r7, #20]
	ldr	r3, [pc, #624]
	mov	r0, sp
	str	r3, [r7, #8]
	movs	r3, #6
	str	r3, [r7, #0]
	ldr	r2, [sp, #56]
	ldr	r3, [pc, #616]
	adds	r0, #184
	str	r3, [r2, #4]
	movs	r3, #7
	str	r0, [sp, #56]
	strb	r3, [r0, #0]
	strb	r3, [r2, #1]
	bl	sub_08014de4
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #13
	movs	r2, #0
	bl	sub_08015160
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r1, #250
	adds	r0, r2, #0
	lsls	r1, r1, #7
	bl	sub_080151e4
	mov	r1, fp
	lsls	r0, r1, #10
	bl	sub_080150e4
	adds	r0, r5, #0
	bl	sub_0801521c
	ldr	r0, [pc, #564]
	ldr	r1, [sp, #76]
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_081862aa:
	ldr	r4, [sp, #40]
	ldr	r2, [pc, #548]
	ldr	r3, [pc, #552]
	movs	r5, #1
	add	r9, r5
	adds	r4, #8
	mov	r0, r9
	add	sl, r2
	adds	r6, #64
	add	r8, r3
	str	r4, [sp, #40]
	cmp	r0, #2
	beq.n	.L_081862c6
	b.n	.L_081861c2
.L_081862c6:
	ldr	r1, [sp, #56]
	movs	r3, #5
	movs	r2, #184
	strb	r3, [r1, #0]
	add	r2, sp
	movs	r3, #7
	str	r2, [sp, #56]
	strb	r3, [r2, #1]
	movs	r3, #6
	str	r3, [r7, #0]
	ldr	r3, [pc, #512]
	mov	r4, fp
	str	r3, [r7, #8]
	movs	r3, #0
	mov	r8, r2
	str	r3, [r7, #20]
	cmp	r4, #0
	bne.n	.L_08186330
	ldr	r6, [pc, #500]
	ldr	r5, [sp, #164]
	mov	sl, r6
	mov	r9, r3
	movs	r6, #0
.L_081862f4:
	mov	r0, sl
	ldrsh	r3, [r6, r0]
	str	r3, [r5, #8]
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	adds	r3, #128
	str	r3, [r5, #20]
	bl	sub_08014878
	movs	r3, #1
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L_08186318
	ldr	r3, [r5, #20]
	negs	r3, r3
	str	r3, [r5, #20]
.L_08186318:
	bl	sub_08014878
	movs	r3, #3
	movs	r2, #1
	ands	r3, r0
	add	r9, r2
	str	r3, [r5, #24]
	mov	r3, r9
	adds	r5, #28
	adds	r6, #2
	cmp	r3, #16
	bne.n	.L_081862f4
.L_08186330:
	movs	r4, #0
	mov	r9, r4
	movs	r5, #128
.L_08186336:
	cmp	fp, r5
	bne.n	.L_08186340
	movs	r0, #144
	bl	sub_081c0010
.L_08186340:
	movs	r6, #1
	add	r9, r6
	mov	r0, r9
	adds	r5, #4
	cmp	r0, #8
	bne.n	.L_08186336
	ldr	r5, [sp, #164]
	movs	r1, #0
	mov	r9, r1
.L_08186352:
	mov	r2, r9
	adds	r2, #128
	cmp	fp, r2
	blt.n	.L_081863b6
	mov	r3, r9
	adds	r3, #144
	cmp	fp, r3
	bge.n	.L_081863b6
	mov	r4, fp
	subs	r3, r4, r2
	movs	r0, #128
	lsls	r6, r3, #13
	lsls	r0, r0, #9
	cmp	r6, r0
	ble.n	.L_08186374
	movs	r6, #128
	lsls	r6, r6, #9
.L_08186374:
	ldr	r3, [r5, #24]
	ldr	r1, [sp, #164]
	movs	r2, #216
	lsls	r3, r3, #12
	lsls	r2, r2, #5
	adds	r3, r1, r3
	adds	r2, #86
	adds	r3, r3, r2
	mov	r4, r8
	str	r3, [r4, #4]
	bl	sub_08014de4
	ldr	r0, [r5, #8]
	bl	sub_080150e4
	ldr	r3, [r5, #8]
	ldr	r2, [r5, #20]
	lsls	r0, r6, #2
	adds	r3, r3, r2
	movs	r2, #128
	str	r3, [r5, #8]
	adds	r1, r6, #0
	lsls	r2, r2, #9
	bl	sub_080151e4
	ldr	r0, [pc, #316]
	ldr	r1, [sp, #76]
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_081863b6:
	movs	r6, #1
	add	r9, r6
	mov	r0, r9
	adds	r5, #28
	cmp	r0, #16
	bne.n	.L_08186352
	adds	r0, r7, #0
	bl	sub_08013164
	ldr	r0, [sp, #76]
	bl	sub_08013164
	mov	r1, fp
	cmp	r1, #133
	ble.n	.L_08186494
	cmp	r1, #134
	bne.n	.L_0818643c
	ldr	r3, [sp, #164]
	movs	r4, #224
	movs	r2, #0
	lsls	r4, r4, #1
	mov	r9, r2
	adds	r7, r3, r4
.L_081863e4:
	bl	sub_08014878
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #85
	mov	r6, r9
	muls	r6, r3
	movs	r5, #31
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #16
	adds	r3, #60
	str	r3, [r7, #0]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #16
	adds	r3, #60
	str	r3, [r7, #4]
	bl	sub_08014878
	movs	r3, #3
	ands	r3, r0
	adds	r3, #5
	str	r3, [r7, #8]
	bl	sub_08014878
	movs	r5, #1
	movs	r3, #15
	ands	r3, r0
	add	r9, r5
	negs	r3, r3
	mov	r6, r9
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r6, #48
	bne.n	.L_081863e4
.L_0818643c:
	ldr	r0, [sp, #164]
	movs	r1, #224
	ldr	r6, [pc, #164]
	movs	r7, #0
	lsls	r1, r1, #1
	mov	r9, r7
	adds	r5, r0, r1
.L_0818644a:
	ldr	r0, [r5, #24]
	cmp	r0, #7
	bhi.n	.L_08186484
	lsls	r0, r0, #12
	bl	sub_08002096
	ldr	r3, [r5, #8]
	ldr	r2, [sp, #144]
	muls	r0, r3
	asrs	r0, r0, #16
	adds	r0, #1
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	lsrs	r3, r0, #31
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #168]
	ldr	r4, [sp, #156]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69a8
.L_08186484:
	movs	r7, #1
	add	r9, r7
	adds	r3, r0, #1
	mov	r0, r9
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r0, #48
	bne.n	.L_0818644a
.L_08186494:
	ldr	r1, [sp, #164]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	adds	r7, r1, r2
	ldr	r3, [r7, #0]
	cmp	r3, #0
	ble.n	.L_081864f0
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	movs	r6, #7
	ldr	r3, [pc, #56]
	ands	r5, r6
	ands	r0, r6
	subs	r5, #4
	adds	r0, #28
	strh	r5, [r3, #4]
	strh	r0, [r3, #6]
	ldr	r3, [r7, #0]
	subs	r3, #1
	str	r3, [r7, #0]
	b.n	.L_08186504
	movs	r0, r0
	.4byte 0x08199364
	.4byte 0x02012000
	.4byte 0x081991e0
	.4byte 0xffff4180
	.4byte 0xfffe8300
	.4byte 0x08199268
	.4byte 0x081999f2
	.4byte 0x081991b0
	.4byte 0x08197410
	.2byte 0x1120
	.2byte 0x0300
.L_081864f0:
	ldr	r2, [pc, #292]
	movs	r3, #0
	strh	r3, [r2, #4]
	ldr	r4, [sp, #164]
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #164
	adds	r3, r4, r5
	ldr	r3, [r3, #0]
	strh	r3, [r2, #6]
.L_08186504:
	ldr	r6, [sp, #164]
	movs	r7, #240
	lsls	r7, r7, #7
	adds	r7, #232
	adds	r2, r6, r7
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #20]
	movs	r1, #1
	add	fp, r1
	adds	r0, #3
	mov	r2, fp
	str	r0, [sp, #20]
	cmp	r2, #160
	beq.n	.L_08186536
	ldr	r3, [pc, #240]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08186536
	b.n	.L_08185d88
.L_08186536:
	movs	r3, #0
	movs	r1, #128
	str	r3, [sp, #68]
	str	r3, [sp, #64]
	ldr	r0, [sp, #168]
	lsls	r1, r1, #7
	ldr	r3, [pc, #220]
	ldr	r2, [pc, #220]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x26f0
	ldr	r4, [sp, #164]
	lsls	r6, r6, #7
	adds	r6, #232
	adds	r2, r4, r6
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	add	r7, sp, #152
	ldr	r3, [pc, #180]
	ldrh	r7, [r7, #0]
	movs	r1, #206
	strh	r7, [r3, #4]
	ldr	r0, [sp, #140]
	lsls	r1, r1, #3
	adds	r3, r0, r1
	ldrh	r1, [r3, #0]
	movs	r0, #1
	movs	r2, #24
	bl	sub_08118040
	movs	r1, #16
	negs	r1, r1
	movs	r2, #4
	adds	r0, r1, #0
	negs	r2, r2
	bl	sub_08164b2c
	ldr	r2, [sp, #164]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #152]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r4, [sp, #164]
	movs	r6, #240
	lsls	r6, r6, #4
	adds	r1, r4, r6
	ldr	r0, [pc, #136]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #132]
	ldr	r1, [sp, #144]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r7, [sp, #164]
	ldr	r5, [pc, #120]
	movs	r2, #248
	lsls	r2, r2, #5
	adds	r1, r7, r2
	ldr	r0, [pc, #116]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #112]
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #32
	movs	r3, #11
	ldr	r0, [pc, #100]
	movs	r1, #4
	bl	sub_08178680
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	mov	r4, sp
	adds	r2, #12
	adds	r4, #176
	strh	r3, [r2, #0]
	str	r4, [sp, #60]
	movs	r3, #0
	mov	fp, r3
.L_081865f6:
	ldr	r3, [pc, #36]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08186644
	mov	r5, fp
	cmp	r5, #2
	ble.n	.L_08186644
	cmp	r5, #87
	bgt.n	.L_08186644
	movs	r6, #88
	mov	fp, r6
	b.n	.L_08186644
	movs	r0, r0
	.4byte 0x00000784
	.4byte 0x03001120
	.4byte 0x03001150
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x000000f4
	.4byte 0x000000e8
	.4byte 0x00000134
	.4byte 0x02014000
	.4byte 0x0000013e
	.4byte 0x00000188
	.2byte 0x2000
	.2byte 0x0201
.L_08186644:
	mov	r7, fp
	cmp	r7, #88
	bne.n	.L_08186690
	movs	r1, #128
	ldr	r3, [pc, #908]
	lsls	r1, r1, #7
	ldr	r2, [pc, #908]
	ldr	r0, [sp, #168]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9829
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r0, r1
	movs	r3, #3
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r0, r3
	ldr	r3, [pc, #884]
	add	r4, sp, #152
	str	r3, [r2, #0]
	ldrh	r4, [r4, #0]
	ldr	r3, [pc, #880]
	ldr	r2, [pc, #884]
	movs	r5, #238
	lsls	r5, r5, #7
	strh	r4, [r3, #4]
	adds	r5, #152
	movs	r3, #120
	str	r3, [r2, #12]
	movs	r6, #0
	adds	r3, r0, r5
	str	r6, [r3, #0]
	movs	r0, #134
	bl	sub_081180e8
.L_08186690:
	mov	r7, fp
	cmp	r7, #0
	bne.n	.L_08186708
	movs	r1, #128
	ldr	r3, [pc, #832]
	ldr	r0, [sp, #168]
	lsls	r1, r1, #7
	ldr	r2, [pc, #832]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9829
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #152
	movs	r3, #16
	adds	r2, r0, r1
	negs	r3, r3
	str	r3, [r2, #0]
	ldr	r2, [pc, #820]
	ldr	r3, [pc, #824]
	movs	r5, #136
	str	r3, [r2, #12]
	movs	r3, #120
	str	r3, [r2, #16]
	ldr	r4, [sp, #152]
	ldr	r2, [pc, #804]
	lsls	r5, r5, #3
	adds	r3, r4, r5
	strh	r3, [r2, #4]
	bl	sub_0815b410
	ldr	r6, [sp, #164]
	movs	r7, #239
	lsls	r7, r7, #7
	adds	r2, r6, r7
	movs	r3, #3
	movs	r0, #238
	str	r3, [r2, #0]
	lsls	r0, r0, #7
	ldr	r3, [pc, #772]
	adds	r0, #132
	adds	r2, r6, r0
	str	r3, [r2, #0]
	ldr	r3, [pc, #780]
	movs	r1, #0
	movs	r2, #1
	mov	r9, r1
	negs	r2, r2
.L_081866f0:
	movs	r4, #1
	add	r9, r4
	mov	r5, r9
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r5, #164
	bne.n	.L_081866f0
	movs	r6, #128
	lsls	r6, r6, #16
	movs	r7, #0
	str	r6, [sp, #68]
	str	r7, [sp, #64]
.L_08186708:
	mov	r0, fp
	cmp	r0, #16
	bne.n	.L_08186726
	ldr	r1, [sp, #164]
	movs	r3, #239
	movs	r4, #238
	lsls	r3, r3, #7
	lsls	r4, r4, #7
	adds	r2, r1, r3
	adds	r4, #132
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r1, r4
	movs	r3, #75
	str	r3, [r2, #0]
.L_08186726:
	mov	r3, fp
	subs	r3, #16
	cmp	r3, #47
	bhi.n	.L_08186734
	ldr	r0, [pc, #712]
	bl	sub_0815f0a0
.L_08186734:
	mov	r5, fp
	cmp	r5, #66
	bne.n	.L_08186820
	ldr	r7, [sp, #172]
	movs	r6, #0
	ldr	r3, [r7, #20]
	mov	r9, r6
	cmp	r3, #0
	beq.n	.L_0818680c
	mov	r0, sp
	adds	r0, #216
	str	r0, [sp, #48]
	str	r6, [sp, #24]
	movs	r1, #36
	mov	r8, r1
.L_08186752:
	ldr	r4, [sp, #172]
	mov	r2, r8
	ldrsh	r0, [r2, r4]
	movs	r3, #150
	str	r3, [sp, #4]
	movs	r5, #128
	movs	r2, #128
	movs	r3, #128
	lsls	r5, r5, #11
	movs	r1, #1
	lsls	r2, r2, #11
	lsls	r3, r3, #12
	str	r5, [sp, #0]
	bl	sub_0815f000
	ldr	r1, [sp, #172]
	mov	r6, r8
	movs	r3, #16
	ldrsh	r0, [r6, r1]
	movs	r2, #5
	str	r3, [sp, #0]
	movs	r1, #7
	mov	r3, r9
	bl	sub_0814cd48
	ldr	r3, [sp, #172]
	ldr	r1, [sp, #48]
	ldrsh	r0, [r6, r3]
	bl	sub_0815e21c
	ldr	r6, [pc, #620]
	ldr	r5, [sp, #24]
	movs	r4, #0
	mov	sl, r4
	adds	r7, r5, r6
.L_08186798:
	ldr	r0, [sp, #48]
	movs	r6, #254
	ldr	r3, [r0, #0]
	lsls	r6, r6, #7
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	movs	r3, #160
	lsls	r3, r3, #15
	str	r3, [r7, #4]
	bl	sub_08014878
	adds	r6, #255
	movs	r1, #128
	lsls	r1, r1, #7
	ands	r6, r0
	adds	r6, r6, r1
	bl	sub_08014878
	movs	r5, #127
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #128
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	movs	r2, #1
	asrs	r3, r3, #4
	str	r3, [r7, #16]
	add	sl, r2
	movs	r3, #16
	str	r3, [r7, #24]
	mov	r3, sl
	adds	r7, #28
	cmp	r3, #16
	bne.n	.L_08186798
	ldr	r5, [sp, #24]
	movs	r6, #224
	lsls	r6, r6, #1
	ldr	r7, [sp, #172]
	adds	r5, r5, r6
	str	r5, [sp, #24]
	ldr	r3, [r7, #20]
	movs	r4, #2
	add	r9, r2
	add	r8, r4
	cmp	r9, r3
	bne.n	.L_08186752
.L_0818680c:
	ldr	r0, [sp, #164]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r2, r0, r1
	movs	r3, #16
	str	r3, [r2, #0]
	movs	r0, #145
	bl	sub_081c0010
.L_08186820:
	mov	r3, fp
	subs	r3, #72
	cmp	r3, #15
	bhi.n	.L_0818684c
	ldr	r3, [sp, #164]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #4
	str	r3, [r2, #0]
	ldr	r5, [sp, #164]
	movs	r6, #238
	lsls	r6, r6, #7
	ldr	r3, [pc, #452]
	adds	r6, #132
	adds	r2, r5, r6
	str	r3, [r2, #0]
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	bl	sub_08164a4c
.L_0818684c:
	mov	r7, fp
	cmp	r7, #0
	blt.n	.L_0818692c
	movs	r3, #0
	cmp	r7, #71
	ble.n	.L_08186862
	lsls	r3, r7, #3
	movs	r0, #144
	negs	r3, r3
	lsls	r0, r0, #2
	adds	r3, r3, r0
.L_08186862:
	adds	r3, #116
	mov	r8, r3
	movs	r2, #31
	mov	r3, fp
	ands	r2, r3
	lsls	r3, r2, #3
	ldr	r4, [pc, #404]
	subs	r3, r3, r2
	movs	r1, #0
	lsls	r3, r3, #3
	mov	r9, r1
	adds	r7, r3, r4
.L_0818687a:
	mov	r5, r8
	lsls	r3, r5, #16
	str	r3, [r7, #0]
	bl	sub_08014878
	movs	r1, #112
	bl	sub_0800206c
	adds	r0, #4
	lsls	r0, r0, #16
	str	r0, [r7, #4]
	bl	sub_08014878
	movs	r6, #254
	lsls	r6, r6, #7
	adds	r6, #255
	ands	r6, r0
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r6, r6, r0
	bl	sub_08014878
	movs	r5, #127
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #128
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	movs	r2, #1
	asrs	r3, r3, #6
	add	r9, r2
	str	r3, [r7, #16]
	movs	r1, #0
	mov	r3, r9
	str	r1, [r7, #24]
	adds	r7, #28
	cmp	r3, #2
	bne.n	.L_0818687a
	ldr	r5, [pc, #296]
	mov	r9, r1
.L_081868dc:
	ldr	r3, [r5, #24]
	cmp	r3, #11
	bhi.n	.L_08186920
	lsrs	r1, r3, #31
	ldr	r4, [sp, #164]
	adds	r1, r3, r1
	movs	r7, #2
	ldrsh	r2, [r5, r7]
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	asrs	r1, r1, #1
	movs	r0, #32
	lsls	r1, r1, #11
	movs	r6, #248
	str	r0, [sp, #0]
	adds	r1, r4, r1
	movs	r0, #64
	lsls	r6, r6, #5
	subs	r3, #32
	str	r0, [sp, #4]
	adds	r1, r1, r6
	subs	r2, #16
	ldr	r0, [sp, #168]
	ldr	r4, [sp, #156]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #63
	movs	r2, #0
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_08186920:
	movs	r6, #1
	add	r9, r6
	mov	r7, r9
	adds	r5, #28
	cmp	r7, #64
	bne.n	.L_081868dc
.L_0818692c:
	mov	r0, fp
	cmp	r0, #87
	ble.n	.L_08186934
	b.n	.L_08186d30
.L_08186934:
	movs	r4, #0
	mov	sl, r4
	mov	r8, r4
	movs	r2, #0
	mov	r7, fp
	movs	r1, #0
.L_08186940:
	ldr	r0, [pc, #196]
	movs	r3, #0
	mov	r9, r3
	movs	r6, #0
	adds	r5, r1, r0
.L_0818694a:
	lsls	r0, r7, #10
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	str	r4, [sp, #8]
	bl	sub_08002096
	lsls	r0, r0, #1
	ldr	r4, [sp, #8]
	asrs	r0, r0, #16
	adds	r0, r6, r0
	adds	r0, r0, r4
	subs	r0, #5
	strb	r0, [r5, #0]
	ldr	r2, [sp, #12]
	movs	r0, #1
	mov	r3, r8
	add	r9, r0
	strb	r3, [r5, #2]
	mov	r3, r9
	strb	r2, [r5, #1]
	adds	r6, #10
	adds	r5, #4
	ldr	r1, [sp, #16]
	cmp	r3, #2
	bne.n	.L_0818694a
	bl	sub_08014878
	ldr	r4, [sp, #8]
	movs	r3, #7
	ldr	r2, [sp, #12]
	ldr	r1, [sp, #16]
	movs	r5, #1
	ands	r3, r0
	add	sl, r5
	adds	r3, r4, r3
	mov	r6, sl
	subs	r4, r3, #4
	subs	r2, #6
	adds	r7, #2
	adds	r1, #8
	cmp	r6, #12
	bne.n	.L_08186940
	movs	r0, #1
	bl	sub_081969f8
	ldr	r3, [pc, #100]
	adds	r7, r0, #0
	movs	r1, #0
	movs	r0, #6
	add	r2, sp, #176
	mov	r4, fp
	str	r0, [r7, #0]
	str	r2, [r7, #16]
	str	r1, [r7, #20]
	strb	r1, [r7, #25]
	str	r3, [r7, #12]
	cmp	r4, #16
	bne.n	.L_081869c2
	ldr	r5, [pc, #80]
	str	r5, [sp, #64]
.L_081869c2:
	mov	r3, fp
	subs	r3, #48
	cmp	r3, #5
	bhi.n	.L_081869d0
	movs	r6, #128
	lsls	r6, r6, #12
	str	r6, [sp, #64]
.L_081869d0:
	mov	r0, fp
	cmp	r0, #59
	ble.n	.L_08186a18
	ldr	r1, [pc, #60]
	str	r1, [sp, #64]
	b.n	.L_08186a18
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x02020202
	.4byte 0x03001120
	.4byte 0x030011e0
	.4byte 0xfffffc38
	.4byte 0x02016d18
	.4byte 0x00000149
	.4byte 0x02017400
	.4byte 0x01010101
	.4byte 0x02016d00
	.4byte 0x02010000
	.4byte 0x02011000
	.4byte 0xfff80000
	.2byte 0x0000
	.2byte 0xfff0
.L_08186a18:
	.2byte 0x465b
	cmp	r3, #31
	ble.n	.L_08186a30
	ldr	r4, [sp, #64]
	lsls	r3, r4, #1
	adds	r3, r3, r4
	lsls	r3, r3, #4
	cmp	r3, #0
	bge.n	.L_08186a2c
	adds	r3, #63
.L_08186a2c:
	asrs	r3, r3, #6
	str	r3, [sp, #64]
.L_08186a30:
	ldr	r5, [sp, #68]
	ldr	r6, [sp, #64]
	movs	r0, #6
	adds	r5, r5, r6
	str	r5, [sp, #68]
	strb	r0, [r2, #0]
	ldr	r1, [sp, #60]
	movs	r3, #5
	strb	r3, [r1, #1]
	ldr	r2, [sp, #164]
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r3, r2, r4
	mov	r6, fp
	ldr	r5, [pc, #872]
	str	r3, [r1, #4]
	movs	r2, #127
	lsls	r3, r6, #3
	ands	r3, r2
	strb	r3, [r7, #24]
	str	r5, [r7, #8]
	bl	sub_08014de4
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r1, #128
	adds	r0, r2, #0
	lsls	r1, r1, #9
	bl	sub_080151e4
	lsls	r3, r6, #1
	add	r3, fp
	lsls	r0, r3, #7
	subs	r0, r0, r3
	lsls	r0, r0, #4
	mov	r8, r0
	bl	sub_08002096
	lsls	r6, r6, #13
	mov	sl, r6
	adds	r5, r0, #0
	mov	r0, sl
	bl	sub_08002090
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
	lsls	r5, r5, #5
	adds	r5, r5, r3
	ldr	r0, [sp, #68]
	adds	r1, r5, #0
	movs	r2, #0
	bl	sub_08015160
	movs	r0, #216
	lsls	r0, r0, #8
	adds	r0, #240
	bl	sub_0801521c
	bl	sub_08014e38
	movs	r0, #128
	lsls	r0, r0, #7
	bl	sub_080150e4
	ldr	r0, [pc, #776]
	bl	sub_0801521c
	ldr	r1, [pc, #772]
	movs	r2, #24
	ldr	r0, [pc, #772]
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
	bl	sub_08014ea8
	ldr	r3, [pc, #760]
	ldr	r2, [sp, #60]
	movs	r1, #6
	str	r3, [r7, #8]
	movs	r3, #0
	strb	r1, [r2, #0]
	strb	r1, [r2, #1]
	strb	r3, [r7, #24]
	ldr	r4, [sp, #164]
	movs	r5, #240
	lsls	r5, r5, #4
	adds	r3, r4, r5
	mov	r6, fp
	str	r3, [r2, #4]
	lsls	r0, r6, #10
	bl	sub_08002096
	adds	r1, r0, #0
	ldr	r0, [pc, #728]
	lsls	r1, r1, #2
	adds	r1, r1, r0
	movs	r2, #0
	ldr	r0, [pc, #724]
	bl	sub_08015160
	ldr	r0, [pc, #724]
	bl	sub_080150e4
	mov	r0, r8
	bl	sub_08002096
	adds	r6, r0, #0
	mov	r0, sl
	bl	sub_08002090
	movs	r1, #184
	lsls	r1, r1, #5
	adds	r1, #208
	lsls	r3, r0, #1
	add	r8, r1
	adds	r3, r3, r0
	lsls	r3, r3, #3
	lsls	r6, r6, #5
	mov	r0, r8
	adds	r6, r6, r3
	bl	sub_08002096
	movs	r2, #128
	lsls	r2, r2, #6
	add	sl, r2
	adds	r5, r0, #0
	mov	r0, sl
	bl	sub_08002090
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r5, r5, #5
	lsls	r3, r3, #3
	adds	r5, r5, r3
	subs	r0, r6, r5
	cmp	r0, #0
	bge.n	.L_08186b50
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r0, r0, r3
.L_08186b50:
	ldr	r4, [pc, #644]
	asrs	r0, r0, #9
	adds	r0, r0, r4
	bl	sub_080150e4
	movs	r1, #206
	lsls	r1, r1, #9
	movs	r2, #128
	ldr	r0, [pc, #632]
	adds	r1, #64
	lsls	r2, r2, #9
	bl	sub_080151e4
	ldr	r1, [pc, #596]
	movs	r2, #4
	ldr	r0, [pc, #624]
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
	mov	r5, fp
	adds	r0, r7, #0
	bl	sub_08013164
	cmp	r5, #87
	ble.n	.L_08186b88
	b.n	.L_08186d30
.L_08186b88:
	ldr	r7, [sp, #164]
	movs	r0, #16
	str	r0, [sp, #44]
	movs	r6, #0
	mov	r9, r6
	mov	r8, r7
.L_08186b94:
	ldr	r1, [sp, #44]
	cmp	fp, r1
	bne.n	.L_08186c68
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r3, #64
	mov	r2, r8
	str	r3, [r2, #0]
	bl	sub_08014878
	mov	r3, r8
	str	r0, [r3, #24]
	ldr	r4, [sp, #164]
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #168
	movs	r3, #4
	adds	r2, r4, r5
	str	r3, [r2, #0]
	mov	r6, r9
	movs	r3, #3
	ands	r3, r6
	cmp	r3, #1
	beq.n	.L_08186be4
	cmp	r3, #1
	bgt.n	.L_08186bd2
	cmp	r3, #0
	beq.n	.L_08186bdc
	b.n	.L_08186bfa
.L_08186bd2:
	cmp	r3, #2
	beq.n	.L_08186bec
	cmp	r3, #3
	beq.n	.L_08186bf4
	b.n	.L_08186bfa
.L_08186bdc:
	movs	r0, #227
	bl	sub_081c0010
	b.n	.L_08186bfa
.L_08186be4:
	movs	r0, #171
	bl	sub_081c0010
	b.n	.L_08186bfa
.L_08186bec:
	movs	r0, #208
	bl	sub_081c0010
	b.n	.L_08186bfa
.L_08186bf4:
	movs	r0, #172
	bl	sub_081c0010
.L_08186bfa:
	movs	r2, #7
	mov	r0, r9
	ands	r2, r0
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #2
	ldr	r1, [pc, #476]
	adds	r3, r3, r2
	movs	r7, #0
	lsls	r3, r3, #4
	mov	sl, r7
	adds	r7, r3, r1
.L_08186c12:
	mov	r2, r8
	ldr	r3, [r2, #0]
	movs	r6, #254
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	movs	r3, #208
	lsls	r3, r3, #15
	str	r3, [r7, #4]
	bl	sub_08014878
	lsls	r6, r6, #7
	adds	r6, #255
	movs	r3, #128
	lsls	r3, r3, #7
	ands	r6, r0
	adds	r6, r6, r3
	bl	sub_08014878
	movs	r5, #127
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #128
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	movs	r4, #1
	asrs	r3, r3, #4
	add	sl, r4
	str	r3, [r7, #16]
	mov	r5, sl
	movs	r3, #16
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r5, #12
	bne.n	.L_08186c12
.L_08186c68:
	ldr	r6, [sp, #44]
	cmp	fp, r6
	blt.n	.L_08186cae
	adds	r3, r6, #0
	adds	r3, #8
	cmp	fp, r3
	bge.n	.L_08186cae
	mov	r7, r8
	ldr	r3, [r7, #24]
	ldr	r0, [pc, #364]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	movs	r2, #3
	ands	r3, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsls	r1, r2, #4
	subs	r1, r1, r2
	ldr	r2, [r7, #0]
	movs	r3, #24
	str	r3, [sp, #0]
	lsls	r1, r1, #6
	movs	r3, #120
	adds	r1, r1, r0
	str	r3, [sp, #4]
	subs	r2, #12
	movs	r3, #0
	ldr	r0, [sp, #168]
	ldr	r4, [sp, #156]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69bb
	adds	r3, #1
	str	r3, [r7, #24]
.L_08186cae:
	ldr	r6, [sp, #44]
	movs	r7, #1
	add	r9, r7
	movs	r5, #28
	adds	r6, #8
	mov	r0, r9
	add	r8, r5
	str	r6, [sp, #44]
	cmp	r0, #6
	beq.n	.L_08186cc4
	b.n	.L_08186b94
.L_08186cc4:
	ldr	r5, [pc, #284]
	movs	r1, #0
	mov	r9, r1
.L_08186cca:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_08186d24
	asrs	r0, r0, #2
	adds	r0, #2
	ldr	r2, [pc, #276]
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #144]
	ldr	r7, [sp, #156]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #168]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r5, #0
	movs	r1, #56
	lsls	r2, r2, #6
	bl	sub_08138086
	movs	r1, #238
	ldr	r0, [sp, #164]
	lsls	r1, r1, #7
	adds	r1, #152
	adds	r3, r0, r1
	ldr	r2, [r3, #0]
	ldr	r3, [r5, #12]
	lsls	r2, r2, #12
	subs	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_08186d24:
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	adds	r5, #28
	cmp	r3, #100
	bne.n	.L_08186cca
.L_08186d30:
	ldr	r4, [sp, #164]
	movs	r5, #238
	ldr	r1, [pc, #184]
	lsls	r5, r5, #7
	adds	r5, #152
	adds	r0, r4, r5
	ldr	r3, [r1, #12]
	ldr	r2, [r0, #0]
	movs	r6, #0
	subs	r3, r3, r2
	str	r3, [r1, #12]
	ldr	r1, [pc, #172]
	ldr	r2, [r0, #0]
	ldrh	r3, [r1, #4]
	mov	r9, r6
	adds	r3, r3, r2
	strh	r3, [r1, #4]
.L_08186d52:
	mov	r3, r9
	adds	r3, #62
	cmp	fp, r3
	bne.n	.L_08186d60
	ldr	r3, [r0, #0]
	adds	r3, #1
	str	r3, [r0, #0]
.L_08186d60:
	movs	r7, #1
	add	r9, r7
	mov	r1, r9
	cmp	r1, #16
	bne.n	.L_08186d52
	movs	r0, #8
	bl	sub_08158d68
	bl	sub_081434f8
	movs	r4, #240
	ldr	r3, [sp, #164]
	lsls	r4, r4, #7
	adds	r4, #232
	movs	r5, #1
	adds	r2, r3, r4
	add	fp, r5
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	mov	r6, fp
	bl	sub_08013560
	cmp	r6, #104
	beq.n	.L_08186d94
	b.n	.L_081865f6
.L_08186d94:
	ldr	r7, [sp, #148]
	movs	r3, #0
	str	r3, [r7, #16]
	ldr	r0, [pc, #92]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #272
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02012000
	.4byte 0x00042ee0
	.4byte 0x02011000
	.4byte 0x02010000
	.4byte 0x08199340
	.4byte 0xfffa0000
	.4byte 0xfffe0000
	.4byte 0xfffff800
	.4byte 0xfffffc00
	.4byte 0x00021170
	.4byte 0x081991e0
	.4byte 0x02017400
	.4byte 0x02014000
	.4byte 0x08197410
	.4byte 0x030011e0
	.4byte 0x03001120
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #192
	str	r0, [sp, #100]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #36]
	movs	r7, #239
	str	r0, [sp, #96]
	movs	r0, #0
	ldr	r1, [r3, #96]
	lsls	r7, r7, #7
	str	r1, [sp, #92]
	ldr	r2, [r3, #92]
	str	r2, [sp, #88]
	ldr	r4, [r3, #100]
	adds	r3, #176
	str	r4, [sp, #72]
	ldr	r3, [r3, #0]
	str	r3, [sp, #68]
	bl	sub_081435e0
	bl	sub_0813ba50
	ldr	r2, [pc, #56]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r5, [sp, #88]
	movs	r1, #200
	adds	r6, r5, r7
	movs	r5, #0
	str	r5, [r6, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #40]
	mov	r8, r1
	bl	sub_080145a8
	movs	r1, #0
	movs	r0, #1
	bl	sub_08163c2c
	ldr	r3, [pc, #20]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	movs	r3, #128
	subs	r2, #40
	lsls	r3, r3, #3
	b.n	.L_08186e7c
	.4byte 0x00000000
	.4byte 0x00003f40
	.2byte 0x3001
	.2byte 0x0814
.L_08186e7c:
	str	r3, [r2, #0]
	ldr	r2, [pc, #496]
	movs	r3, #240
	str	r3, [r2, #16]
	ldr	r2, [sp, #88]
	movs	r4, #240
	lsls	r4, r4, #7
	adds	r4, #240
	adds	r3, r2, r4
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	ldr	r3, [pc, #476]
	ldr	r7, [sp, #68]
	movs	r2, #1
	str	r2, [r7, #16]
	strh	r5, [r3, #4]
	str	r2, [r6, #0]
	ldr	r0, [sp, #88]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #132
	adds	r3, r0, r1
	str	r5, [r3, #0]
	mov	r1, r8
	ldr	r0, [pc, #456]
	bl	sub_080145a8
	movs	r1, #202
	movs	r2, #0
	lsls	r1, r1, #1
	str	r2, [sp, #48]
	str	r2, [sp, #44]
	str	r2, [sp, #40]
	adds	r1, #255
	movs	r0, #2
	movs	r2, #2
	movs	r5, #240
	bl	sub_08152404
	movs	r7, #13
	movs	r3, #0
	lsls	r5, r5, #7
	mov	r8, r3
	negs	r7, r7
	movs	r6, #4
	adds	r5, #12
.L_08186eda:
	movs	r0, #165
	lsls	r0, r0, #2
	bl	sub_08020040
	ldr	r4, [sp, #88]
	str	r0, [r5, r4]
	cmp	r0, #0
	beq.n	.L_08186f0c
	movs	r3, #0
	strb	r3, [r0, #26]
	movs	r1, #0
	bl	sub_08020030
	ldr	r0, [sp, #88]
	adds	r3, r7, #0
	ldr	r1, [r5, r0]
	ldrb	r2, [r1, #9]
	ands	r3, r2
	ldrb	r2, [r1, #5]
	orrs	r3, r6
	strb	r3, [r1, #9]
	adds	r3, r7, #0
	ands	r3, r2
	orrs	r3, r6
	strb	r3, [r1, #5]
.L_08186f0c:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r5, #4
	cmp	r2, #4
	bne.n	.L_08186eda
	movs	r5, #240
	movs	r4, #13
	movs	r3, #0
	lsls	r5, r5, #7
	negs	r4, r4
	mov	r8, r3
	adds	r5, #28
	adds	r6, r4, #0
.L_08186f28:
	movs	r0, #203
	lsls	r0, r0, #1
	adds	r0, #255
	bl	sub_08020040
	ldr	r7, [sp, #88]
	str	r0, [r5, r7]
	cmp	r0, #0
	beq.n	.L_08186f50
	movs	r3, #0
	strb	r3, [r0, #26]
	movs	r1, #0
	bl	sub_08020030
	ldr	r1, [r5, r7]
	movs	r2, #4
	ldrb	r3, [r1, #9]
	ands	r3, r6
	orrs	r3, r2
	strb	r3, [r1, #9]
.L_08186f50:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #4
	cmp	r1, #2
	bne.n	.L_08186f28
	ldr	r2, [pc, #284]
	movs	r1, #64
	movs	r3, #0
	movs	r0, #64
	bl	sub_0815b290
	movs	r4, #240
	ldr	r2, [sp, #88]
	lsls	r4, r4, #7
	adds	r4, #36
	adds	r3, r2, r4
	ldrb	r2, [r0, #5]
	str	r0, [r3, #0]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r0, #5]
	ldrb	r3, [r0, #16]
	ldr	r2, [pc, #248]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r5, [sp, #88]
	ldrh	r3, [r3, #2]
	ldr	r0, [pc, #244]
	movs	r7, #246
	lsls	r7, r7, #7
	adds	r7, #116
	adds	r1, r5, r7
	adds	r3, r3, r0
	str	r3, [r1, #0]
	ldr	r1, [sp, #88]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r5, r1, r2
	ldr	r0, [pc, #224]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r1, [pc, #216]
	adds	r0, r5, #0
	movs	r2, #64
	movs	r3, #64
	bl	sub_0816ae40
	movs	r2, #1
	movs	r3, #0
	ldr	r0, [pc, #204]
	adds	r1, r5, #0
	bl	sub_08157cf4
	movs	r2, #184
	movs	r3, #0
	lsls	r2, r2, #5
	mov	r8, r3
	adds	r2, #129
.L_08186fd2:
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_08186fde
	lsrs	r3, r3, #2
	adds	r3, #224
	strb	r3, [r5, #0]
.L_08186fde:
	movs	r4, #1
	add	r8, r4
	adds	r5, #1
	cmp	r8, r2
	bne.n	.L_08186fd2
	ldr	r7, [sp, #88]
	movs	r1, #142
	lsls	r1, r1, #7
	adds	r5, r7, r1
	adds	r1, r5, #0
	movs	r2, #0
	ldr	r0, [pc, #156]
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #0
	ldr	r1, [pc, #152]
	mov	r8, r2
	movs	r2, #128
	lsls	r2, r2, #3
.L_08187006:
	ldr	r3, [r5, #0]
	lsrs	r3, r3, #1
	ands	r3, r1
	stmia	r5!, {r3}
	movs	r3, #1
	add	r8, r3
	cmp	r8, r2
	bne.n	.L_08187006
	ldr	r2, [pc, #132]
	movs	r4, #0
	mov	r8, r4
	movs	r0, #2
	movs	r4, #3
	movs	r1, #1
.L_08187022:
	mov	r5, r8
	movs	r7, #1
	lsls	r3, r5, #1
	add	r8, r7
	strb	r3, [r2, #0]
	mov	r3, r8
	strb	r1, [r2, #1]
	strb	r0, [r2, #2]
	strb	r1, [r2, #4]
	strb	r4, [r2, #5]
	strb	r0, [r2, #6]
	adds	r4, #2
	adds	r0, #2
	adds	r1, #2
	adds	r2, #8
	cmp	r3, #31
	bne.n	.L_08187022
	ldr	r2, [pc, #88]
	movs	r3, #0
	movs	r4, #160
	strb	r3, [r2, #0]
	strb	r3, [r2, #1]
	strb	r3, [r2, #2]
	lsls	r4, r4, #19
	movs	r5, #1
	adds	r4, #2
	mov	r8, r5
.L_08187058:
	mov	r7, r8
	cmp	r7, #31
	bgt.n	.L_081870a4
	movs	r0, #0
	movs	r2, #0
	mov	r1, r8
	cmp	r7, #0
	bge.n	.L_0818706a
	movs	r1, #0
.L_0818706a:
	strh	r1, [r4, #0]
	b.n	.L_081870ba
	movs	r0, r0
	.4byte 0x030011e0
	.4byte 0x03001120
	.4byte 0x08143115
	.4byte 0xc0002000
	.4byte 0x020036e0
	.4byte 0x06010000
	.4byte 0x000000f7
	.4byte 0x02014000
	.4byte 0x00000178
	.4byte 0x000000b4
	.4byte 0x7f7f7f7f
	.4byte 0x02010ae8
	.2byte 0x0be0
	.2byte 0x0201
.L_081870a4:
	mov	r3, r8
	subs	r3, #32
	lsls	r0, r3, #2
	adds	r1, r0, #0
	adds	r2, r3, #0
	cmp	r1, #31
	ble.n	.L_081870b4
	movs	r1, #31
.L_081870b4:
	cmp	r0, #31
	ble.n	.L_081870ba
	movs	r0, #31
.L_081870ba:
	lsls	r3, r0, #10
	lsls	r2, r2, #5
	movs	r0, #1
	orrs	r3, r2
	add	r8, r0
	orrs	r3, r1
	mov	r1, r8
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r1, #64
	bne.n	.L_08187058
	ldr	r4, [pc, #708]
	movs	r2, #0
	mov	r8, r2
	movs	r5, #64
	movs	r0, #0
.L_081870da:
	mov	r7, r8
	lsls	r3, r7, #1
	adds	r1, r3, #2
	adds	r2, r0, #2
	cmp	r1, #31
	ble.n	.L_081870e8
	movs	r1, #31
.L_081870e8:
	cmp	r2, #31
	ble.n	.L_081870ee
	movs	r2, #31
.L_081870ee:
	lsls	r3, r2, #10
	orrs	r3, r5
	orrs	r3, r1
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	strh	r3, [r4, #0]
	adds	r5, #32
	adds	r0, #3
	adds	r4, #2
	cmp	r2, #16
	bne.n	.L_081870da
	movs	r2, #0
	ldr	r1, [pc, #656]
	movs	r0, #1
	bl	sub_08118040
	movs	r3, #0
	str	r3, [sp, #76]
	ldr	r3, [pc, #648]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08187124
	.2byte 0xf000
	.2byte 0xfe97
.L_08187124:
	ldr	r4, [pc, #636]
	ldr	r5, [pc, #640]
	str	r4, [sp, #16]
	str	r5, [sp, #12]
.L_0818712c:
	ldr	r7, [sp, #76]
	cmp	r7, #0
	beq.n	.L_08187134
	b.n	.L_08187260
.L_08187134:
	ldr	r0, [sp, #88]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #132
	adds	r3, r0, r1
	str	r7, [r3, #0]
	ldr	r5, [sp, #96]
	movs	r2, #216
	movs	r7, #160
	lsls	r2, r2, #15
	movs	r3, #240
	movs	r0, #160
	lsls	r7, r7, #3
	movs	r4, #0
	str	r2, [sp, #60]
	lsls	r3, r3, #15
	lsls	r0, r0, #19
	adds	r7, #108
	movs	r2, #128
	str	r3, [sp, #64]
	lsls	r2, r2, #1
	ldr	r3, [pc, #588]
	str	r4, [sp, #52]
	str	r4, [sp, #56]
	adds	r0, #192
	adds	r1, r5, r7
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b90
	movs	r0, #0
	mov	r8, r0
	movs	r2, #1
.L_08187174:
	movs	r1, #1
	add	r8, r1
	mov	r4, r8
	strb	r2, [r3, #0]
	strb	r2, [r3, #1]
	adds	r3, #2
	cmp	r4, #31
	bne.n	.L_08187174
	ldr	r0, [pc, #556]
	ldr	r2, [sp, #88]
	movs	r5, #0
	movs	r4, #128
	mov	r8, r5
	lsls	r4, r4, #13
	movs	r1, #0
.L_08187192:
	ldrb	r3, [r0, #0]
	movs	r7, #1
	lsls	r3, r3, #16
	add	r8, r7
	str	r3, [r2, #0]
	mov	r3, r8
	str	r4, [r2, #4]
	str	r1, [r2, #12]
	str	r1, [r2, #16]
	str	r1, [r2, #24]
	adds	r0, #1
	adds	r2, #28
	cmp	r3, #4
	bne.n	.L_08187192
	ldr	r5, [sp, #88]
	movs	r4, #0
	mov	r8, r4
	movs	r7, #0
	movs	r6, #0
	adds	r5, #112
.L_081871ba:
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #24
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	str	r6, [r5, #24]
	str	r3, [r5, #4]
	str	r7, [r5, #12]
	str	r7, [r5, #16]
	adds	r6, #3
	adds	r5, #28
	cmp	r1, #8
	bne.n	.L_081871ba
	ldr	r2, [sp, #88]
	movs	r4, #192
	lsls	r4, r4, #3
	adds	r4, #228
	adds	r3, r2, r4
	movs	r2, #240
	lsls	r2, r2, #14
	str	r2, [r3, #0]
	movs	r2, #224
	movs	r5, #0
	lsls	r2, r2, #15
	str	r5, [r3, #24]
	mov	r8, r5
	str	r2, [r3, #4]
	ldr	r5, [pc, #436]
	movs	r7, #0
	movs	r6, #0
.L_08187206:
	adds	r0, r6, #0
	bl	sub_08002090
	lsls	r0, r0, #6
	negs	r0, r0
	asrs	r0, r0, #16
	strb	r0, [r5, #0]
	adds	r0, r6, #0
	bl	sub_08002096
	lsls	r3, r0, #3
	adds	r3, r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	mov	r0, r8
	asrs	r3, r3, #16
	strb	r3, [r5, #1]
	lsls	r3, r0, #2
	subs	r3, #64
	strb	r3, [r5, #4]
	strb	r7, [r5, #2]
	adds	r0, r6, #0
	bl	sub_08002090
	lsls	r0, r0, #6
	negs	r0, r0
	asrs	r0, r0, #16
	strb	r0, [r5, #4]
	adds	r0, r6, #0
	bl	sub_08002096
	lsls	r3, r0, #3
	adds	r3, r3, r0
	movs	r2, #1
	lsrs	r3, r3, #15
	movs	r1, #248
	add	r8, r2
	strb	r3, [r5, #5]
	lsls	r1, r1, #2
	mov	r3, r8
	strb	r7, [r5, #6]
	adds	r6, r6, r1
	adds	r5, #8
	cmp	r3, #32
	bne.n	.L_08187206
.L_08187260:
	ldr	r4, [sp, #76]
	cmp	r4, #31
	bhi.n	.L_0818727c
	movs	r1, #3
	lsls	r0, r4, #1
	bl	sub_08002054
	ldr	r2, [sp, #76]
	adds	r1, r0, #0
	subs	r1, #35
	subs	r2, #35
	adds	r0, r1, #0
	bl	sub_08164b2c
.L_0818727c:
	ldr	r3, [sp, #76]
	subs	r3, #242
	cmp	r3, #21
	bhi.n	.L_08187298
	ldr	r3, [sp, #76]
	movs	r5, #1
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_08187298
	movs	r0, #1
	movs	r1, #0
	movs	r2, #0
	bl	sub_08164abc
.L_08187298:
	ldr	r7, [sp, #76]
	movs	r0, #22
	adds	r0, #255
	cmp	r7, r0
	bne.n	.L_081872ae
	movs	r2, #12
	negs	r2, r2
	movs	r0, #8
	adds	r1, r2, #0
	bl	sub_08164b2c
.L_081872ae:
	ldr	r1, [sp, #76]
	movs	r2, #104
	adds	r2, #255
	cmp	r1, r2
	ble.n	.L_081872e6
	ldr	r3, [pc, #256]
	adds	r1, r1, r3
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_081872c8
	ldr	r4, [sp, #76]
	ldr	r5, [pc, #248]
	adds	r2, r4, r5
.L_081872c8:
	asrs	r2, r2, #2
	movs	r3, #8
	subs	r0, r3, r2
	adds	r2, r1, #0
	cmp	r2, #0
	bge.n	.L_081872da
	ldr	r7, [sp, #76]
	ldr	r1, [pc, #236]
	adds	r2, r7, r1
.L_081872da:
	asrs	r2, r2, #3
	movs	r1, #12
	subs	r2, #12
	negs	r1, r1
	bl	sub_08164b2c
.L_081872e6:
	ldr	r2, [sp, #76]
	cmp	r2, #48
	bne.n	.L_081872f2
	movs	r0, #136
	bl	sub_081c0010
.L_081872f2:
	ldr	r3, [sp, #76]
	cmp	r3, #74
	bne.n	.L_081872fe
	movs	r0, #136
	bl	sub_081c0010
.L_081872fe:
	ldr	r4, [sp, #76]
	cmp	r4, #100
	bne.n	.L_0818730a
	movs	r0, #136
	bl	sub_081c0010
.L_0818730a:
	ldr	r5, [sp, #76]
	cmp	r5, #126
	bne.n	.L_08187316
	movs	r0, #136
	bl	sub_081c0010
.L_08187316:
	ldr	r7, [sp, #76]
	cmp	r7, #140
	bne.n	.L_08187322
	movs	r0, #141
	bl	sub_081c0010
.L_08187322:
	ldr	r0, [sp, #76]
	movs	r1, #133
	lsls	r1, r1, #1
	cmp	r0, r1
	ble.n	.L_0818732e
	b.n	.L_0818782e
.L_0818732e:
	movs	r2, #192
	lsls	r2, r2, #1
	adds	r2, #255
	cmp	r0, r2
	bgt.n	.L_08187420
	add	r3, sp, #176
	movs	r4, #0
	str	r4, [r3, #4]
	str	r4, [r3, #12]
	ldr	r7, [sp, #88]
	mov	r8, r4
	mov	sl, r3
.L_08187346:
	movs	r3, #26
	mov	r5, r8
	muls	r5, r3
	ldr	r0, [sp, #76]
	adds	r3, r5, #0
	cmp	r0, r3
	ble.n	.L_08187414
	ldr	r3, [r7, #24]
	cmp	r3, #0
	bne.n	.L_081873cc
	ldr	r3, [r7, #0]
	mov	r1, sl
	str	r3, [r1, #0]
	ldr	r3, [r7, #4]
	movs	r5, #1
	str	r3, [r1, #8]
	movs	r2, #240
	mov	r3, r8
	ldr	r4, [sp, #88]
	ands	r3, r5
	lsls	r2, r2, #7
	adds	r2, #28
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r0, [r4, r3]
	ldr	r2, [pc, #76]
	movs	r3, #0
	bl	sub_08020010
	movs	r2, #128
	adds	r0, r7, #0
	movs	r1, #63
	lsls	r2, r2, #6
	bl	sub_08138086
	movs	r0, #6
	ldrsh	r3, [r7, r0]
	cmp	r3, #119
	ble.n	.L_08187414
	str	r5, [r7, #24]
	b.n	.L_08187414
	.4byte 0x050003c0
	.4byte 0x0000005a
	.4byte 0x03001150
	.4byte 0xffffe3d7
	.4byte 0xfffffef5
	.4byte 0x03000730
	.4byte 0x02010fa0
	.4byte 0x08199a3e
	.4byte 0x020104f0
	.4byte 0xfffffe98
	.4byte 0xfffffe9b
	.4byte 0xfffffe9f
	.2byte 0x9a44
	.2byte 0x0819
.L_081873cc:
	cmp	r3, #23
	bgt.n	.L_08187414
	ldr	r3, [r7, #0]
	mov	r1, sl
	str	r3, [r1, #0]
	ldr	r3, [r7, #4]
	movs	r2, #1
	str	r3, [r1, #8]
	mov	r5, r8
	movs	r3, #240
	ands	r5, r2
	ldr	r4, [sp, #88]
	lsls	r3, r3, #7
	adds	r3, #12
	lsls	r5, r5, #2
	adds	r5, r5, r3
	ldr	r0, [r7, #24]
	movs	r1, #6
	ldr	r6, [r4, r5]
	bl	sub_08002054
	movs	r1, #3
	ands	r1, r0
	adds	r0, r6, #0
	bl	sub_08020030
	ldr	r1, [sp, #88]
	movs	r3, #0
	ldr	r0, [r1, r5]
	ldr	r2, [pc, #600]
	mov	r1, sl
	bl	sub_08020010
	ldr	r3, [r7, #24]
	adds	r3, #1
	str	r3, [r7, #24]
.L_08187414:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r7, #28
	cmp	r3, #4
	bne.n	.L_08187346
.L_08187420:
	ldr	r4, [sp, #76]
	cmp	r4, #46
	bgt.n	.L_08187428
	b.n	.L_0818760a
.L_08187428:
	movs	r0, #32
	bl	sub_08014dac
	str	r0, [sp, #36]
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #556]
	ldr	r3, [sp, #152]
	movs	r1, #6
	ands	r3, r2
	ldr	r2, [pc, #552]
	orrs	r3, r1
	ldr	r5, [sp, #76]
	ands	r3, r2
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r7, r0, #0
	ldr	r0, [sp, #88]
	orrs	r3, r2
	movs	r2, #142
	subs	r5, #47
	lsls	r2, r2, #7
	str	r3, [sp, #152]
	mov	r8, r5
	adds	r3, r0, r2
	add	r5, sp, #152
	str	r3, [r5, #4]
	ldr	r6, [sp, #76]
	subs	r6, #111
	cmp	r6, #0
	ble.n	.L_0818746a
	movs	r6, #0
.L_0818746a:
	ldr	r3, [pc, #512]
	str	r1, [r7, #0]
	str	r6, [r7, #20]
	str	r5, [r7, #16]
	str	r3, [r7, #8]
	ldr	r3, [sp, #36]
	str	r3, [r7, #12]
	bl	sub_08014de4
	movs	r1, #144
	lsls	r1, r1, #14
	movs	r2, #0
	movs	r0, #0
	bl	sub_08015160
	movs	r0, #250
	lsls	r0, r0, #4
	bl	sub_08015024
	mov	r4, r8
	negs	r0, r4
	lsls	r0, r0, #8
	bl	sub_08015068
	movs	r2, #234
	mov	r1, r8
	lsls	r2, r2, #8
	lsls	r0, r1, #10
	adds	r2, #96
	cmp	r0, r2
	ble.n	.L_081874ae
	movs	r0, #234
	lsls	r0, r0, #8
	adds	r0, #96
.L_081874ae:
	bl	sub_0801521c
	movs	r2, #4
	ldr	r1, [sp, #36]
	ldr	r0, [pc, #440]
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
	movs	r3, #5
	strb	r3, [r5, #0]
	strb	r3, [r5, #1]
	ldr	r4, [sp, #88]
	movs	r0, #184
	lsls	r0, r0, #5
	adds	r3, r4, r0
	str	r3, [r5, #4]
	movs	r3, #9
	str	r3, [r7, #0]
	ldr	r3, [pc, #412]
	str	r5, [r7, #16]
	str	r3, [r7, #8]
	ldr	r1, [sp, #36]
	str	r6, [r7, #20]
	str	r1, [r7, #12]
	bl	sub_08014de4
	movs	r1, #144
	movs	r0, #0
	movs	r2, #0
	lsls	r1, r1, #14
	bl	sub_08015160
	mov	r2, r8
	lsls	r3, r2, #1
	add	r3, r8
	movs	r4, #128
	movs	r5, #151
	lsls	r3, r3, #8
	lsls	r4, r4, #3
	lsls	r5, r5, #8
	adds	r0, r3, r4
	adds	r5, #112
	cmp	r0, r5
	ble.n	.L_08187510
	movs	r0, #151
	lsls	r0, r0, #8
	adds	r0, #112
.L_08187510:
	bl	sub_0801521c
	ldr	r0, [sp, #76]
	cmp	r0, #73
	ble.n	.L_081875ba
	cmp	r0, #74
	bne.n	.L_08187524
	movs	r1, #0
	str	r1, [sp, #48]
	str	r1, [sp, #44]
.L_08187524:
	ldr	r0, [sp, #48]
	movs	r1, #13
	bl	sub_08002064
	ldr	r5, [sp, #48]
	adds	r4, r0, #0
	adds	r5, #1
	movs	r1, #13
	adds	r0, r5, #0
	str	r4, [sp, #8]
	bl	sub_08002064
	ldr	r2, [sp, #44]
	ldr	r1, [pc, #312]
	mov	r9, r2
	mov	r3, r9
	adds	r3, #1
	str	r3, [sp, #44]
	ldr	r4, [sp, #8]
	ldr	r2, [sp, #44]
	ldrb	r3, [r1, r4]
	mov	ip, r1
	cmp	r2, r3
	bne.n	.L_0818755a
	movs	r3, #0
	str	r3, [sp, #44]
	str	r5, [sp, #48]
.L_0818755a:
	movs	r5, #0
	lsls	r1, r4, #5
	lsls	r0, r0, #5
	mov	r8, r5
	mov	fp, r1
	mov	sl, r0
	b.n	.L_0818756c
.L_08187568:
	ldr	r2, [pc, #268]
	mov	ip, r2
.L_0818756c:
	ldr	r2, [pc, #268]
	ldr	r5, [pc, #272]
	mov	r3, r8
	mov	r1, r8
	lsls	r6, r3, #3
	add	r1, fp
	add	r3, sl
	adds	r6, r6, r5
	ldrb	r3, [r2, r3]
	ldrb	r5, [r2, r1]
	mov	r2, ip
	subs	r3, r3, r5
	mov	r0, r9
	muls	r0, r3
	ldrb	r1, [r2, r4]
	str	r4, [sp, #8]
	bl	sub_08002054
	movs	r3, #1
	adds	r5, r5, r0
	negs	r5, r5
	add	r8, r3
	strb	r5, [r6, #1]
	mov	r5, r8
	ldr	r4, [sp, #8]
	cmp	r5, #32
	bne.n	.L_08187568
	ldr	r2, [pc, #220]
	movs	r0, #0
	mov	r8, r0
	subs	r3, #65
.L_081875aa:
	movs	r1, #1
	add	r8, r1
	mov	r4, r8
	strb	r3, [r2, #0]
	adds	r2, #8
	adds	r3, #4
	cmp	r4, #32
	bne.n	.L_081875aa
.L_081875ba:
	ldr	r1, [pc, #200]
	ldr	r2, [pc, #200]
	movs	r5, #0
	movs	r0, #1
	str	r5, [r7, #4]
	str	r0, [r7, #0]
	str	r1, [r7, #8]
	str	r2, [r7, #12]
	ldr	r3, [sp, #76]
	subs	r3, #236
	cmp	r3, #31
	bhi.n	.L_081875ea
	ldr	r3, [sp, #76]
	ldr	r2, [pc, #180]
	subs	r3, #235
	mov	r8, r5
.L_081875da:
	movs	r4, #1
	add	r8, r4
	mov	r5, r8
	strb	r3, [r2, #0]
	strb	r3, [r2, #1]
	adds	r2, #2
	cmp	r5, #31
	bne.n	.L_081875da
.L_081875ea:
	ldr	r0, [pc, #160]
	ldr	r1, [pc, #152]
	movs	r2, #64
	str	r0, [r7, #20]
	ldr	r0, [pc, #140]
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
	adds	r0, r7, #0
	bl	sub_08013164
	ldr	r0, [sp, #36]
	bl	sub_08013164
.L_0818760a:
	ldr	r3, [pc, #132]
	movs	r1, #160
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	add	r1, sp
	movs	r2, #0
	str	r3, [sp, #144]
	str	r4, [sp, #148]
	str	r2, [r1, #12]
	ldr	r3, [sp, #76]
	mov	r9, r1
	cmp	r3, #149
	bgt.n	.L_08187626
	b.n	.L_0818782e
.L_08187626:
	ldr	r5, [sp, #60]
	ldr	r0, [sp, #64]
	ldr	r4, [sp, #52]
	ldr	r7, [sp, #56]
	adds	r4, r4, r5
	adds	r7, r7, r0
	str	r4, [sp, #60]
	str	r7, [sp, #64]
	cmp	r3, #159
	bgt.n	.L_08187644
	ldr	r1, [sp, #56]
	ldr	r2, [pc, #84]
	adds	r2, r1, r2
	str	r2, [sp, #56]
	b.n	.L_081876ba
.L_08187644:
	ldr	r3, [sp, #76]
	subs	r3, #210
	cmp	r3, #13
	bhi.n	.L_081876a0
	ldr	r3, [sp, #52]
	ldr	r5, [sp, #56]
	ldr	r4, [pc, #68]
	ldr	r7, [pc, #72]
	adds	r4, r3, r4
	adds	r7, r5, r7
	str	r4, [sp, #52]
	str	r7, [sp, #56]
	b.n	.L_081876ba
	movs	r0, r0
	.4byte 0x08199a44
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199340
	.4byte 0x08199210
	.4byte 0x08199244
	.4byte 0x08199bec
	.4byte 0x08199a4c
	.4byte 0x020104f0
	.4byte 0x02010ae8
	.4byte 0x02010000
	.4byte 0x02010fa0
	.4byte 0x08196f00
	.4byte 0xffffc000
	.4byte 0xfffff800
	.2byte 0xf000
	.2byte 0xffff
.L_081876a0:
	.2byte 0x9b13
	subs	r3, #236
	cmp	r3, #3
	bhi.n	.L_081876ba
	ldr	r0, [sp, #52]
	ldr	r2, [sp, #56]
	ldr	r3, [pc, #924]
	movs	r1, #128
	lsls	r1, r1, #6
	adds	r1, r0, r1
	adds	r3, r2, r3
	str	r1, [sp, #52]
	str	r3, [sp, #56]
.L_081876ba:
	ldr	r4, [sp, #52]
	lsls	r3, r4, #4
	subs	r3, r3, r4
	lsls	r3, r3, #2
	cmp	r3, #0
	bge.n	.L_081876c8
	adds	r3, #63
.L_081876c8:
	ldr	r5, [sp, #56]
	asrs	r3, r3, #6
	str	r3, [sp, #52]
	lsls	r3, r5, #4
	subs	r3, r3, r5
	lsls	r3, r3, #2
	cmp	r3, #0
	bge.n	.L_081876da
	adds	r3, #63
.L_081876da:
	asrs	r3, r3, #6
	str	r3, [sp, #56]
	movs	r7, #144
	movs	r3, #128
	add	r7, sp
	lsls	r3, r3, #9
	str	r3, [sp, #144]
	str	r3, [r7, #4]
	ldr	r1, [sp, #88]
	movs	r2, #238
	lsls	r2, r2, #7
	ldr	r6, [sp, #64]
	movs	r0, #0
	adds	r2, #220
	mov	sl, r7
	mov	r8, r0
	mov	r5, r9
	adds	r7, r1, r2
.L_081876fe:
	ldr	r3, [sp, #60]
	ldmia	r7!, {r0}
	str	r3, [r5, #0]
	movs	r3, #136
	lsls	r3, r3, #17
	subs	r3, r3, r6
	str	r3, [r5, #4]
	movs	r3, #128
	lsls	r3, r3, #17
	str	r3, [r5, #8]
	adds	r1, r5, #0
	mov	r2, sl
	movs	r3, #0
	bl	sub_08020010
	movs	r0, #1
	movs	r4, #128
	add	r8, r0
	lsls	r4, r4, #14
	mov	r1, r8
	adds	r6, r6, r4
	cmp	r1, #2
	bne.n	.L_081876fe
	ldr	r3, [sp, #76]
	subs	r3, #150
	cmp	r3, #116
	bhi.n	.L_081877d2
	str	r0, [sp, #0]
	movs	r2, #6
	movs	r3, #3
	movs	r0, #188
	movs	r1, #6
	bl	sub_08196404
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r3, [r3, #0]
	ldr	r6, [sp, #88]
	str	r3, [sp, #84]
	movs	r2, #0
	mov	r8, r2
	adds	r6, #112
.L_08187754:
	ldr	r0, [r6, #24]
	cmp	r0, #0
	bge.n	.L_0818775c
	adds	r0, #3
.L_0818775c:
	movs	r1, #6
	asrs	r0, r0, #2
	bl	sub_08002064
	ldr	r2, [pc, #744]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #88]
	ldr	r2, [r6, #0]
	adds	r1, r3, r1
	ldr	r3, [pc, #736]
	movs	r4, #224
	ldrb	r5, [r3, r0]
	lsls	r4, r4, #3
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #728]
	adds	r1, r1, r4
	ldrb	r4, [r3, r0]
	ldr	r3, [r6, #4]
	str	r5, [sp, #0]
	adds	r3, r3, r4
	ldr	r4, [pc, #720]
	ldr	r5, [sp, #88]
	ldrb	r0, [r4, r0]
	movs	r7, #207
	lsls	r7, r7, #7
	str	r0, [sp, #4]
	ldr	r4, [sp, #84]
	adds	r0, r5, r7
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69b3
	adds	r3, #1
	str	r3, [r6, #24]
	cmp	r3, #24
	bne.n	.L_081877c0
	movs	r5, #0
	str	r5, [r6, #24]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #24
	str	r3, [r6, #0]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	str	r3, [r6, #4]
.L_081877c0:
	movs	r7, #1
	add	r8, r7
	mov	r0, r8
	adds	r6, #28
	cmp	r0, #8
	bne.n	.L_08187754
	movs	r0, #188
	bl	sub_0801314c
.L_081877d2:
	ldr	r1, [sp, #76]
	movs	r3, #141
	lsls	r3, r3, #3
	ldr	r2, [pc, #644]
	muls	r3, r1
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #11
	str	r3, [sp, #144]
	cmp	r3, r2
	ble.n	.L_081877ea
	str	r2, [sp, #144]
.L_081877ea:
	ldr	r4, [sp, #76]
	movs	r3, #141
	lsls	r3, r3, #2
	muls	r3, r4
	ldr	r5, [pc, #624]
	movs	r2, #128
	adds	r3, r3, r5
	mov	r7, sl
	lsls	r2, r2, #10
	str	r3, [r7, #4]
	cmp	r3, r2
	ble.n	.L_08187804
	str	r2, [r7, #4]
.L_08187804:
	ldr	r2, [sp, #64]
	movs	r3, #240
	lsls	r3, r3, #15
	mov	r1, r9
	subs	r3, r3, r2
	ldr	r0, [sp, #60]
	str	r3, [r1, #4]
	movs	r3, #128
	lsls	r3, r3, #16
	str	r0, [r1, #0]
	str	r3, [r1, #8]
	ldr	r4, [sp, #88]
	movs	r5, #240
	lsls	r5, r5, #7
	adds	r5, #36
	adds	r3, r4, r5
	ldr	r0, [r3, #0]
	mov	r2, sl
	movs	r3, #0
	bl	sub_08020010
.L_0818782e:
	ldr	r7, [sp, #76]
	movs	r5, #12
	adds	r5, #255
	cmp	r7, r5
	bne.n	.L_0818783e
	movs	r0, #212
	bl	sub_081c0010
.L_0818783e:
	ldr	r0, [sp, #76]
	movs	r1, #16
	adds	r1, #255
	cmp	r0, r1
	bne.n	.L_0818784e
	movs	r0, #144
	bl	sub_081c0010
.L_0818784e:
	ldr	r2, [sp, #12]
	cmp	r2, #9
	bls.n	.L_08187856
	b.n	.L_081879d2
.L_08187856:
	ldr	r3, [sp, #76]
	cmp	r3, r5
	bne.n	.L_081878c0
	movs	r2, #160
	lsls	r2, r2, #19
	movs	r4, #1
	adds	r2, #2
	mov	r8, r4
.L_08187866:
	mov	r3, r8
	cmp	r3, #31
	ble.n	.L_0818786e
	movs	r3, #31
.L_0818786e:
	cmp	r3, #0
	bge.n	.L_08187874
	movs	r3, #0
.L_08187874:
	movs	r5, #1
	add	r8, r5
	mov	r7, r8
	strh	r3, [r2, #0]
	adds	r2, #2
	cmp	r7, #64
	bne.n	.L_08187866
	ldr	r2, [sp, #88]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #476]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #468]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #464]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c16
	movs	r5, #239
	lsls	r5, r5, #7
	adds	r3, r4, r5
	movs	r7, #1
	movs	r0, #238
	str	r7, [r3, #0]
	lsls	r0, r0, #7
	ldr	r3, [pc, #440]
	adds	r0, #132
	adds	r2, r4, r0
	str	r3, [r2, #0]
.L_081878c0:
	movs	r0, #32
	bl	sub_08014dac
	mov	sl, r0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #424]
	ldr	r3, [sp, #136]
	movs	r1, #7
	ands	r3, r2
	ldr	r2, [pc, #420]
	orrs	r3, r1
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #136]
	ldr	r3, [pc, #408]
	add	r2, sp, #136
	adds	r7, r0, #0
	str	r3, [r2, #4]
	movs	r3, #0
	str	r3, [r7, #20]
	ldr	r3, [pc, #400]
	mov	r4, sl
	str	r1, [r7, #0]
	str	r2, [r7, #16]
	str	r3, [r7, #8]
	str	r4, [r7, #12]
	mov	r8, r2
	bl	sub_08014de4
	movs	r0, #0
	ldr	r1, [pc, #384]
	movs	r2, #0
	bl	sub_08015160
	ldr	r5, [sp, #76]
	movs	r1, #236
	movs	r0, #134
	lsls	r1, r1, #9
	lsls	r0, r0, #1
	adds	r1, #240
	cmp	r5, r0
	ble.n	.L_08187922
	ldr	r2, [pc, #364]
	lsls	r3, r5, #12
	adds	r1, r3, r2
.L_08187922:
	asrs	r2, r1, #1
	adds	r0, r2, #0
	bl	sub_080151e4
	ldr	r0, [pc, #356]
	mov	r1, sl
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
	ldr	r3, [sp, #12]
	cmp	r3, #5
	bhi.n	.L_081879c6
	movs	r1, #216
	ldr	r3, [pc, #336]
	ldr	r0, [pc, #336]
	lsls	r1, r1, #4
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9d04
	movs	r6, #0
	lsls	r4, r5, #7
	movs	r5, #0
.L_08187954:
	ldr	r2, [pc, #320]
	ldr	r3, [sp, #88]
	adds	r1, r5, r2
	adds	r3, r3, r4
	movs	r2, #224
	mov	ip, r3
	lsls	r2, r2, #3
	movs	r0, #0
	add	r2, ip
.L_08187966:
	ldrb	r3, [r2, #0]
	adds	r0, #1
	strb	r3, [r1, #0]
	adds	r2, #1
	adds	r1, #1
	cmp	r0, #48
	bne.n	.L_08187966
	adds	r6, #1
	adds	r4, #48
	adds	r5, #64
	cmp	r6, #72
	bne.n	.L_08187954
	ldr	r4, [sp, #12]
	cmp	r4, #5
	bhi.n	.L_081879c6
	movs	r3, #6
	mov	r5, r8
	add	r2, sp, #136
	strb	r3, [r5, #0]
	movs	r3, #7
	strb	r3, [r2, #1]
	ldr	r0, [pc, #260]
	ldr	r3, [pc, #264]
	str	r0, [r2, #4]
	str	r3, [r7, #8]
	bl	sub_08014de4
	movs	r0, #0
	ldr	r1, [pc, #232]
	movs	r2, #0
	bl	sub_08015160
	movs	r1, #167
	lsls	r1, r1, #10
	movs	r2, #128
	ldr	r0, [pc, #240]
	adds	r1, #64
	lsls	r2, r2, #10
	bl	sub_080151e4
	ldr	r0, [pc, #216]
	mov	r1, sl
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_081879c6:
	adds	r0, r7, #0
	bl	sub_08013164
	mov	r0, sl
	bl	sub_08013164
.L_081879d2:
	ldr	r1, [sp, #76]
	movs	r2, #138
	lsls	r2, r2, #1
	cmp	r1, r2
	bgt.n	.L_081879de
	.2byte 0xe20e
.L_081879de:
	movs	r3, #22
	adds	r3, #255
	cmp	r1, r3
	beq.n	.L_081879e8
	.2byte 0xe0b5
.L_081879e8:
	ldr	r4, [sp, #88]
	movs	r5, #239
	movs	r0, #238
	lsls	r5, r5, #7
	lsls	r0, r0, #7
	adds	r3, r4, r5
	movs	r7, #1
	adds	r0, #132
	str	r7, [r3, #0]
	movs	r1, #0
	adds	r3, r4, r0
	movs	r2, #1
	str	r1, [r3, #0]
	str	r2, [sp, #40]
	adds	r5, #92
	adds	r3, r4, r5
	ldr	r0, [r3, #0]
	bl	sub_08020048
	movs	r0, #238
	ldr	r7, [sp, #88]
	lsls	r0, r0, #7
	adds	r0, #224
	adds	r3, r7, r0
	ldr	r0, [r3, #0]
	bl	sub_08020048
	movs	r2, #240
	lsls	r2, r2, #7
	movs	r1, #0
	adds	r2, #12
	mov	r8, r1
	adds	r5, r7, r2
.L_08187a2a:
	ldmia	r5!, {r0}
	bl	sub_08020048
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	cmp	r4, #4
	bne.n	.L_08187a2a
	ldr	r7, [sp, #88]
	movs	r0, #240
	lsls	r0, r0, #7
	movs	r5, #0
	adds	r0, #28
	mov	r8, r5
	adds	r5, r7, r0
	.2byte 0xe02c
	movs	r0, r0
	.4byte 0xffffc000
	.4byte 0x0819747a
	.4byte 0x08197467
	.4byte 0x08197473
	.4byte 0x0819746d
	.2byte 0x6b10
	.2byte 0xfffe
	.2byte 0xb588
	.2byte 0xffff
	.2byte 0x017a
	movs	r0, r0
	lsls	r0, r2, #5
	movs	r0, r0
	lsls	r0, r6, #28
	lsls	r0, r0, #12
	lsls	r1, r0, #4
	lsls	r1, r0, #4
	.2byte 0xff00
	.2byte 0xffff
	lsls	r7, r7, #3
	.2byte 0xffff
	.2byte 0x4000
	lsls	r1, r0, #8
	str	r3, [sp, #400]
	lsrs	r1, r3, #32
	movs	r0, r0
	.2byte 0xfffc
	.2byte 0x08f0
	.2byte 0xfff1
	.2byte 0x91e0
	lsrs	r1, r3, #32
	lsls	r0, r3, #9
	lsls	r0, r0, #12
	movs	r0, r0
	lsls	r1, r0, #8
	str	r3, [sp, #544]
	lsrs	r1, r3, #32
	.2byte 0xb1e0
	movs	r1, r0
.L_08187aa4:
	ldmia	r5!, {r0}
	bl	sub_08020048
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #2
	bne.n	.L_08187aa4
	ldr	r4, [sp, #88]
	movs	r5, #240
	lsls	r5, r5, #7
	adds	r5, #36
	adds	r3, r4, r5
	ldr	r0, [r3, #0]
	bl	sub_08020048
	movs	r1, #202
	lsls	r1, r1, #1
	movs	r0, #11
	adds	r1, #255
	movs	r2, #2
	bl	sub_08152404
	ldr	r6, [pc, #684]
	ldr	r5, [sp, #88]
	movs	r7, #0
	mov	r8, r7
	movs	r7, #31
.L_08187adc:
	ldrb	r2, [r6, #0]
	ldrb	r1, [r6, #1]
	lsrs	r2, r2, #1
	lsls	r3, r2, #16
	str	r3, [r5, #0]
	lsls	r3, r1, #16
	str	r3, [r5, #4]
	movs	r3, #60
	subs	r3, r3, r1
	subs	r2, #120
	lsls	r2, r2, #12
	lsls	r3, r3, #10
	movs	r0, #0
	str	r2, [r5, #12]
	str	r3, [r5, #16]
	str	r0, [r5, #8]
	bl	sub_08014878
	ldr	r3, [r5, #12]
	ands	r0, r7
	subs	r0, #16
	lsls	r0, r0, #13
	adds	r3, r3, r0
	str	r3, [r5, #12]
	bl	sub_08014878
	ldr	r3, [r5, #16]
	ands	r0, r7
	subs	r0, #16
	lsls	r0, r0, #13
	adds	r3, r3, r0
	str	r3, [r5, #16]
	bl	sub_08014878
	movs	r3, #127
	ands	r0, r3
	lsls	r3, r0, #2
	movs	r1, #160
	adds	r3, r3, r0
	lsls	r1, r1, #2
	adds	r3, r3, r1
	str	r3, [r5, #20]
	bl	sub_08014878
	movs	r2, #1
	ands	r0, r2
	cmp	r0, #0
	beq.n	.L_08187b42
	ldr	r3, [r5, #20]
	negs	r3, r3
	str	r3, [r5, #20]
.L_08187b42:
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r6, #10
	adds	r5, #28
	cmp	r4, #31
	bne.n	.L_08187adc
	movs	r5, #0
	str	r5, [sp, #52]
.L_08187b54:
	ldr	r2, [pc, #556]
	movs	r7, #255
	ldrh	r3, [r2, #4]
	lsls	r7, r7, #8
	adds	r7, #244
	adds	r3, r3, r7
	strh	r3, [r2, #4]
	ldr	r3, [pc, #548]
	add	r6, sp, #160
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	movs	r0, #0
	str	r3, [sp, #128]
	str	r4, [sp, #132]
	str	r0, [r6, #12]
	ldr	r1, [sp, #76]
	lsls	r5, r1, #9
	adds	r0, r5, #0
	bl	sub_08002096
	movs	r2, #216
	lsls	r0, r0, #3
	lsls	r2, r2, #15
	adds	r2, r0, r2
	adds	r0, r5, #0
	str	r2, [sp, #60]
	bl	sub_08002090
	movs	r3, #224
	lsls	r3, r3, #14
	lsls	r0, r0, #3
	subs	r0, r3, r0
	ldr	r3, [sp, #76]
	movs	r4, #198
	lsls	r4, r4, #1
	str	r0, [sp, #64]
	cmp	r3, r4
	ble.n	.L_08187bc2
	ldr	r5, [pc, #488]
	ldr	r7, [sp, #52]
	ldr	r1, [sp, #56]
	adds	r2, r3, r5
	ldr	r0, [sp, #60]
	lsls	r3, r2, #14
	lsls	r2, r2, #13
	subs	r3, r7, r3
	subs	r2, r1, r2
	adds	r0, r3, r0
	str	r3, [sp, #52]
	str	r2, [sp, #56]
	adds	r2, r3, #0
	ldr	r3, [sp, #64]
	str	r0, [sp, #60]
	adds	r2, r2, r3
	str	r2, [sp, #64]
.L_08187bc2:
	ldr	r0, [sp, #88]
	movs	r1, #238
	adds	r7, r6, #0
	lsls	r1, r1, #7
	ldr	r6, [pc, #452]
	movs	r4, #0
	adds	r1, #220
	mov	r8, r4
	adds	r5, r0, r1
	add	r4, sp, #128
.L_08187bd6:
	ldrh	r3, [r6, #0]
	ldr	r2, [sp, #60]
	lsls	r3, r3, #16
	adds	r3, r3, r2
	str	r3, [r7, #0]
	ldrh	r3, [r6, #2]
	ldr	r0, [sp, #64]
	lsls	r3, r3, #16
	movs	r2, #136
	adds	r3, r3, r0
	lsls	r2, r2, #17
	subs	r2, r2, r3
	movs	r3, #128
	lsls	r3, r3, #17
	str	r2, [r7, #4]
	str	r3, [r7, #8]
	adds	r2, r4, #0
	adds	r1, r7, #0
	ldmia	r5!, {r0}
	movs	r3, #0
	str	r4, [sp, #8]
	bl	sub_08020010
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r6, #4
	ldr	r4, [sp, #8]
	cmp	r2, #11
	bne.n	.L_08187bd6
	movs	r0, #32
	bl	sub_08014dac
	str	r0, [sp, #32]
	movs	r0, #1
	bl	sub_081969f8
	ldr	r3, [sp, #88]
	movs	r4, #224
	movs	r5, #240
	ldr	r7, [sp, #76]
	mov	sl, r0
	lsls	r4, r4, #3
	lsls	r5, r5, #4
	movs	r0, #22
	adds	r4, r3, r4
	adds	r5, r3, r5
	adds	r0, #255
	str	r4, [sp, #28]
	str	r5, [sp, #24]
	cmp	r7, r0
	bne.n	.L_08187cb2
	ldr	r0, [sp, #24]
	ldr	r7, [pc, #316]
	movs	r5, #0
	movs	r1, #0
	movs	r2, #2
	mov	ip, r1
	mov	fp, r2
	mov	r9, r5
	mov	lr, r5
.L_08187c50:
	movs	r3, #0
	ldr	r4, [pc, #300]
	mov	r8, r3
	mov	r3, lr
	add	r3, ip
	lsls	r3, r3, #1
	ldr	r2, [sp, #28]
	adds	r3, r3, r4
	ldrb	r6, [r7, #1]
	ldrb	r4, [r7, #0]
	adds	r1, r3, #2
	add	r2, r9
.L_08187c68:
	ldrb	r3, [r1, #0]
	strb	r5, [r2, #2]
	subs	r3, r3, r4
	strb	r3, [r2, #0]
	ldrb	r3, [r1, #1]
	adds	r1, #2
	subs	r3, r3, r6
	strb	r3, [r2, #1]
	movs	r3, #1
	add	r8, r3
	mov	r3, r8
	adds	r2, #4
	cmp	r3, #4
	bne.n	.L_08187c68
	movs	r4, #1
	strb	r4, [r0, #1]
	movs	r4, #1
	mov	r1, fp
	movs	r3, #3
	add	ip, r4
	strb	r1, [r0, #2]
	strb	r1, [r0, #5]
	strb	r3, [r0, #6]
	movs	r2, #16
	movs	r3, #4
	mov	r1, ip
	strb	r5, [r0, #0]
	strb	r5, [r0, #4]
	strb	r5, [r0, #8]
	strb	r5, [r0, #9]
	strb	r5, [r0, #10]
	adds	r7, #10
	adds	r0, #12
	add	r9, r2
	add	lr, r3
	cmp	r1, #31
	bne.n	.L_08187c50
.L_08187cb2:
	ldr	r3, [sp, #120]
	ldr	r2, [pc, #220]
	mov	r5, sl
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	ldr	r2, [pc, #216]
	movs	r4, #1
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	ldr	r2, [pc, #208]
	str	r3, [sp, #120]
	add	r3, sp, #120
	str	r2, [r3, #4]
	str	r3, [r5, #16]
	str	r4, [r5, #0]
	ldr	r7, [sp, #32]
	ldr	r0, [pc, #196]
	ldr	r3, [pc, #196]
	str	r7, [r5, #12]
	str	r0, [r5, #20]
	adds	r2, r3, #0
	movs	r1, #1
	adds	r2, #31
.L_08187ce6:
	strb	r1, [r3, #0]
	adds	r3, #1
	cmp	r3, r2
	bne.n	.L_08187ce6
	ldr	r6, [sp, #24]
	ldr	r5, [sp, #88]
	movs	r1, #0
	mov	r8, r1
.L_08187cf6:
	movs	r4, #2
	ldrsh	r3, [r5, r4]
	mov	r2, sl
	adds	r3, #16
	str	r6, [r2, #8]
	cmp	r3, #152
	bhi.n	.L_08187dcc
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	movs	r0, #16
	negs	r0, r0
	cmp	r3, r0
	blt.n	.L_08187dcc
	cmp	r3, #136
	bgt.n	.L_08187dcc
	bl	sub_08014de4
	ldr	r3, [pc, #136]
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	adds	r0, r0, r3
	adds	r1, r1, r3
	movs	r2, #0
	bl	sub_08015160
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r1, #128
	lsls	r1, r1, #9
	adds	r0, r2, #0
	bl	sub_080151e4
	movs	r3, #3
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #1
	beq.n	.L_08187d62
	cmp	r3, #1
	bgt.n	.L_08187d4a
	cmp	r3, #0
	beq.n	.L_08187d54
	b.n	.L_08187db6
.L_08187d4a:
	cmp	r3, #2
	beq.n	.L_08187d72
	cmp	r3, #3
	beq.n	.L_08187da8
	b.n	.L_08187db6
.L_08187d54:
	ldr	r0, [r5, #8]
	bl	sub_08015024
	ldr	r0, [r5, #8]
	bl	sub_080150e4
	b.n	.L_08187db6
.L_08187d62:
	ldr	r0, [r5, #8]
	bl	sub_08015024
	ldr	r0, [r5, #8]
	negs	r0, r0
	bl	sub_080150e4
	b.n	.L_08187db6
.L_08187d72:
	ldr	r0, [r5, #8]
	bl	sub_08015024
	ldr	r0, [r5, #8]
	bl	sub_08015068
	b.n	.L_08187db6
	.4byte 0x08199bf9
	.4byte 0x03001120
	.4byte 0x08196f08
	.4byte 0xfffffe73
	.4byte 0x08199a12
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x02010000
	.4byte 0x02010fa0
	.2byte 0x0000
	.2byte 0xffc0
.L_08187da8:
	.2byte 0x68a8
	bl	sub_080150e4
	ldr	r0, [r5, #8]
	negs	r0, r0
	bl	sub_08015068
.L_08187db6:
	ldr	r3, [sp, #28]
	mov	r2, r8
	lsls	r0, r2, #4
	adds	r0, r3, r0
	ldr	r1, [sp, #32]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_08187dcc:
	adds	r0, r5, #0
	movs	r1, #62
	movs	r2, #0
	bl	sub_08138058
	ldr	r3, [r5, #0]
	movs	r4, #128
	movs	r7, #1
	lsls	r4, r4, #11
	add	r8, r7
	adds	r3, r3, r4
	mov	r0, r8
	str	r3, [r5, #0]
	adds	r6, #12
	adds	r5, #28
	cmp	r0, #31
	beq.n	.L_08187df0
	b.n	.L_08187cf6
.L_08187df0:
	mov	r0, sl
	bl	sub_08013164
	ldr	r0, [sp, #32]
	bl	sub_08013164
.L_08187dfc:
	bl	sub_081434f8
	movs	r2, #240
	ldr	r1, [sp, #88]
	lsls	r2, r2, #7
	adds	r2, #232
	adds	r3, r1, r2
	movs	r2, #1
	str	r2, [r3, #0]
	ldr	r3, [sp, #76]
	movs	r4, #133
	lsls	r4, r4, #1
	cmp	r3, r4
	bgt.n	.L_08187e22
	movs	r5, #240
	lsls	r5, r5, #7
	adds	r5, #236
	adds	r3, r1, r5
	str	r2, [r3, #0]
.L_08187e22:
	movs	r0, #1
	bl	sub_08013560
	ldr	r7, [sp, #16]
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #76]
	movs	r2, #220
	adds	r7, #27
	adds	r0, #1
	adds	r1, #1
	lsls	r2, r2, #1
	str	r7, [sp, #16]
	str	r0, [sp, #12]
	str	r1, [sp, #76]
	cmp	r1, r2
	beq.n	.L_08187e52
	ldr	r3, [pc, #128]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08187e52
	bl	.L_0818712c
.L_08187e52:
	ldr	r3, [sp, #40]
	cmp	r3, #0
	bne.n	.L_08187ec8
	ldr	r4, [sp, #88]
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #220
	adds	r3, r4, r5
	ldr	r0, [r3, #0]
	bl	sub_08020048
	movs	r0, #238
	ldr	r7, [sp, #88]
	lsls	r0, r0, #7
	adds	r0, #224
	adds	r3, r7, r0
	ldr	r0, [r3, #0]
	bl	sub_08020048
	movs	r2, #240
	lsls	r2, r2, #7
	movs	r1, #0
	adds	r2, #12
	mov	r8, r1
	adds	r5, r7, r2
.L_08187e84:
	ldmia	r5!, {r0}
	bl	sub_08020048
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	cmp	r4, #4
	bne.n	.L_08187e84
	ldr	r7, [sp, #88]
	movs	r0, #240
	lsls	r0, r0, #7
	movs	r5, #0
	adds	r0, #28
	mov	r8, r5
	adds	r5, r7, r0
.L_08187ea2:
	ldmia	r5!, {r0}
	bl	sub_08020048
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #2
	bne.n	.L_08187ea2
	ldr	r4, [sp, #88]
	movs	r5, #240
	lsls	r5, r5, #7
	adds	r5, #36
	adds	r3, r4, r5
	ldr	r0, [r3, #0]
	bl	sub_08020048
	b.n	.L_08187ee6
	.2byte 0x1150
	.2byte 0x0300
.L_08187ec8:
	ldr	r0, [sp, #88]
	movs	r1, #238
	lsls	r1, r1, #7
	movs	r7, #0
	adds	r1, #220
	mov	r8, r7
	adds	r5, r0, r1
.L_08187ed6:
	ldmia	r5!, {r0}
	bl	sub_08020048
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #11
	bne.n	.L_08187ed6
.L_08187ee6:
	ldr	r4, [sp, #68]
	movs	r3, #0
	str	r3, [r4, #16]
	ldr	r0, [pc, #268]
	bl	sub_08014644
	bl	sub_0814cca8
	ldr	r2, [pc, #264]
	movs	r1, #202
	movs	r3, #120
	lsls	r1, r1, #1
	str	r3, [r2, #16]
	adds	r1, #255
	movs	r2, #2
	movs	r0, #11
	bl	sub_08152404
	ldr	r0, [pc, #248]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #240]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9d16
	movs	r7, #224
	lsls	r7, r7, #3
	adds	r1, r5, r7
	ldr	r0, [pc, #228]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #216
	ldr	r3, [pc, #220]
	ldr	r0, [pc, #220]
	lsls	r1, r1, #7
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2000
	mov	r8, r0
	mov	ip, r0
	movs	r7, #0
.L_08187f44:
	mov	r2, r8
	mov	r1, ip
	adds	r3, r7, r2
	movs	r6, #0
	lsls	r5, r1, #7
	lsls	r0, r3, #9
.L_08187f50:
	ldr	r2, [sp, #88]
	ldr	r3, [pc, #192]
	adds	r2, r2, r5
	mov	lr, r2
	movs	r2, #224
	lsls	r2, r2, #3
	movs	r4, #0
	adds	r1, r0, r3
	add	r2, lr
.L_08187f62:
	ldrb	r3, [r2, #0]
	adds	r4, #1
	strb	r3, [r1, #0]
	adds	r2, #1
	adds	r1, #1
	cmp	r4, #48
	bne.n	.L_08187f62
	adds	r6, #1
	adds	r5, #48
	adds	r0, #64
	cmp	r6, #72
	bne.n	.L_08187f50
	movs	r4, #1
	add	r8, r4
	movs	r3, #27
	mov	r5, r8
	add	ip, r3
	adds	r7, #8
	cmp	r5, #6
	bne.n	.L_08187f44
	ldr	r7, [sp, #88]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r7, r2
	ldr	r0, [pc, #132]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #124]
	ldr	r1, [sp, #72]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r3, #239
	movs	r4, #238
	lsls	r3, r3, #7
	lsls	r4, r4, #7
	adds	r2, r7, r3
	adds	r4, #132
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r7, r4
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r0, #104
	movs	r1, #19
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r5, #0
	movs	r7, #104
	str	r3, [sp, #80]
	str	r5, [sp, #76]
	str	r5, [sp, #20]
	add	r7, sp
	mov	r9, r7
.L_08187fe4:
	ldr	r0, [sp, #76]
	cmp	r0, #0
	bne.n	.L_0818804a
	ldr	r3, [pc, #52]
	ldr	r2, [pc, #52]
	movs	r4, #192
	movs	r1, #135
	lsls	r4, r4, #11
	lsls	r1, r1, #17
	b.n	.L_08188028
	.4byte 0x00001010
	.4byte 0x08143115
	.4byte 0x030011e0
	.4byte 0x00000148
	.4byte 0x03000730
	.4byte 0x0000017a
	.4byte 0x03000258
	.4byte 0x02010000
	.4byte 0x000000f4
	.4byte 0x00000134
	.4byte 0xfff40000
	.2byte 0x0000
	.2byte 0xffea
.L_08188028:
	.2byte 0x930d
	str	r4, [sp, #56]
	ldr	r3, [pc, #560]
	str	r1, [sp, #60]
	str	r2, [sp, #64]
	movs	r5, #0
	movs	r1, #1
	movs	r2, #128
	mov	r8, r5
	negs	r1, r1
	lsls	r2, r2, #2
.L_0818803e:
	movs	r7, #1
	add	r8, r7
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_0818803e
.L_0818804a:
	ldr	r0, [sp, #76]
	cmp	r0, #20
	bne.n	.L_08188064
	ldr	r1, [pc, #528]
	ldr	r2, [pc, #532]
	movs	r3, #192
	movs	r4, #192
	lsls	r3, r3, #12
	lsls	r4, r4, #11
	str	r1, [sp, #60]
	str	r2, [sp, #64]
	str	r3, [sp, #52]
	str	r4, [sp, #56]
.L_08188064:
	ldr	r5, [sp, #76]
	cmp	r5, #40
	bne.n	.L_0818807e
	ldr	r1, [pc, #512]
	movs	r7, #130
	movs	r0, #128
	lsls	r7, r7, #17
	lsls	r0, r0, #15
	movs	r2, #0
	str	r7, [sp, #60]
	str	r0, [sp, #64]
	str	r1, [sp, #52]
	str	r2, [sp, #56]
.L_0818807e:
	ldr	r6, [pc, #496]
	ldr	r5, [pc, #496]
	movs	r3, #0
	mov	r8, r3
.L_08188086:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_081880c8
	asrs	r0, r0, #3
	adds	r0, #1
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r7, [sp, #72]
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
	ldr	r0, [sp, #92]
	ldr	r4, [sp, #80]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #63
	ldr	r2, [pc, #440]
	bl	sub_08138086
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_081880c8:
	movs	r7, #1
	movs	r0, #128
	add	r8, r7
	lsls	r0, r0, #2
	adds	r5, #28
	cmp	r8, r0
	bne.n	.L_08188086
	ldr	r1, [sp, #76]
	cmp	r1, #39
	bgt.n	.L_08188154
	lsls	r3, r1, #1
	adds	r3, r3, r1
	movs	r2, #0
	mov	r8, r2
	lsls	r2, r3, #3
	subs	r2, r2, r3
	ldr	r4, [sp, #60]
	ldr	r3, [pc, #392]
	lsls	r2, r2, #4
	adds	r7, r2, r3
	lsrs	r3, r4, #31
	adds	r3, r3, r4
	asrs	r3, r3, #1
	mov	sl, r3
.L_081880f8:
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	subs	r3, #16
	lsls	r3, r3, #16
	add	r3, sl
	str	r3, [r7, #0]
	bl	sub_08014878
	movs	r3, #31
	ldr	r5, [sp, #64]
	ands	r3, r0
	lsls	r3, r3, #16
	adds	r3, r3, r5
	str	r3, [r7, #4]
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08014878
	movs	r5, #127
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #128
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	movs	r0, #1
	asrs	r3, r3, #6
	add	r8, r0
	str	r3, [r7, #16]
	mov	r1, r8
	movs	r3, #16
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r1, #12
	bne.n	.L_081880f8
.L_08188154:
	ldr	r2, [sp, #76]
	cmp	r2, #73
	ble.n	.L_0818815c
	b.n	.L_08188298
.L_0818815c:
	ldr	r3, [pc, #284]
	add	r1, sp, #160
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	subs	r2, #20
	str	r3, [sp, #112]
	str	r4, [sp, #116]
	movs	r3, #0
	str	r3, [r1, #12]
	mov	sl, r2
	cmp	r2, #19
	bhi.n	.L_081881ca
	ldr	r3, [pc, #264]
	adds	r7, r1, #0
	ldr	r0, [sp, #88]
	movs	r1, #238
	lsls	r1, r1, #7
	ldr	r6, [pc, #260]
	str	r3, [sp, #112]
	adds	r1, #220
	movs	r3, #0
	mov	r8, r3
	add	r4, sp, #112
	adds	r5, r0, r1
.L_0818818c:
	ldrh	r3, [r6, #0]
	ldr	r2, [sp, #60]
	lsls	r3, r3, #16
	subs	r3, r2, r3
	str	r3, [r7, #0]
	ldrh	r3, [r6, #2]
	ldr	r0, [sp, #64]
	lsls	r3, r3, #16
	movs	r2, #136
	adds	r3, r3, r0
	lsls	r2, r2, #17
	subs	r2, r2, r3
	movs	r3, #128
	lsls	r3, r3, #17
	str	r2, [r7, #4]
	str	r3, [r7, #8]
	adds	r2, r4, #0
	adds	r1, r7, #0
	ldmia	r5!, {r0}
	movs	r3, #0
	str	r4, [sp, #8]
	bl	sub_08020010
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r6, #4
	ldr	r4, [sp, #8]
	cmp	r2, #11
	bne.n	.L_0818818c
	b.n	.L_0818821a
.L_081881ca:
	ldr	r7, [sp, #88]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #220
	adds	r6, r7, r0
	ldr	r7, [pc, #172]
	movs	r3, #0
	mov	r8, r3
	add	r4, sp, #112
	adds	r5, r1, #0
.L_081881de:
	ldrh	r3, [r7, #0]
	ldr	r1, [sp, #60]
	lsls	r3, r3, #16
	adds	r3, r3, r1
	str	r3, [r5, #0]
	ldrh	r3, [r7, #2]
	ldr	r2, [sp, #64]
	lsls	r3, r3, #16
	adds	r3, r3, r2
	movs	r2, #136
	lsls	r2, r2, #17
	subs	r2, r2, r3
	movs	r3, #128
	lsls	r3, r3, #17
	str	r2, [r5, #4]
	str	r3, [r5, #8]
	ldmia	r6!, {r0}
	adds	r2, r4, #0
	movs	r3, #0
	adds	r1, r5, #0
	str	r4, [sp, #8]
	bl	sub_08020010
	movs	r3, #1
	add	r8, r3
	mov	r0, r8
	adds	r7, #4
	ldr	r4, [sp, #8]
	cmp	r0, #11
	bne.n	.L_081881de
.L_0818821a:
	ldr	r2, [sp, #60]
	ldr	r4, [sp, #64]
	ldr	r1, [sp, #52]
	ldr	r3, [sp, #56]
	ldr	r5, [sp, #76]
	adds	r1, r1, r2
	adds	r3, r3, r4
	str	r1, [sp, #60]
	str	r3, [sp, #64]
	cmp	r5, #19
	bhi.n	.L_08188244
	ldr	r7, [sp, #52]
	ldr	r1, [sp, #56]
	ldr	r0, [pc, #64]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r0, r7, r0
	adds	r2, r1, r2
	str	r0, [sp, #52]
	str	r2, [sp, #56]
	b.n	.L_08188298
.L_08188244:
	mov	r3, sl
	cmp	r3, #19
	bhi.n	.L_08188288
	ldr	r4, [sp, #52]
	ldr	r7, [sp, #56]
	movs	r5, #128
	movs	r0, #128
	lsls	r5, r5, #8
	lsls	r0, r0, #7
	adds	r5, r4, r5
	adds	r0, r7, r0
	str	r5, [sp, #52]
	str	r0, [sp, #56]
	b.n	.L_08188298
	.4byte 0x02014018
	.4byte 0xfff60000
	.4byte 0xffea0000
	.4byte 0xfff80000
	.4byte 0x08197410
	.4byte 0x02014000
	.4byte 0xffff8000
	.4byte 0x08196f10
	.4byte 0xfffeffff
	.2byte 0x9a12
	.2byte 0x0819
.L_08188288:
	ldr	r3, [sp, #76]
	subs	r3, #40
	cmp	r3, #19
	bhi.n	.L_08188298
	ldr	r1, [sp, #52]
	ldr	r2, [pc, #656]
	adds	r2, r1, r2
	str	r2, [sp, #52]
.L_08188298:
	ldr	r3, [sp, #76]
	cmp	r3, #10
	bne.n	.L_081882a4
	movs	r0, #212
	bl	sub_081c0010
.L_081882a4:
	ldr	r4, [sp, #76]
	cmp	r4, #14
	bne.n	.L_08188306
	movs	r0, #144
	bl	sub_081c0010
	ldr	r7, [sp, #100]
	movs	r5, #0
	ldr	r3, [r7, #20]
	mov	r8, r5
	cmp	r3, #0
	beq.n	.L_08188306
	ldr	r0, [sp, #88]
	movs	r1, #238
	lsls	r1, r1, #7
	movs	r6, #128
	adds	r1, #168
	lsls	r6, r6, #11
	movs	r5, #36
	adds	r7, r0, r1
.L_081882cc:
	ldr	r2, [sp, #100]
	movs	r1, #1
	ldrsh	r0, [r5, r2]
	movs	r3, #150
	str	r3, [sp, #4]
	movs	r3, #128
	adds	r2, r6, #0
	lsls	r3, r3, #12
	str	r6, [sp, #0]
	bl	sub_0815f000
	ldr	r4, [sp, #100]
	movs	r3, #16
	ldrsh	r0, [r5, r4]
	movs	r2, #5
	str	r3, [sp, #0]
	movs	r1, #7
	mov	r3, r8
	bl	sub_0814cd48
	movs	r3, #8
	str	r3, [r7, #0]
	ldr	r4, [sp, #100]
	movs	r2, #1
	ldr	r3, [r4, #20]
	add	r8, r2
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_081882cc
.L_08188306:
	ldr	r5, [sp, #76]
	cmp	r5, #30
	bne.n	.L_08188312
	movs	r0, #212
	bl	sub_081c0010
.L_08188312:
	ldr	r7, [sp, #76]
	cmp	r7, #34
	bne.n	.L_08188374
	movs	r0, #144
	bl	sub_081c0010
	ldr	r1, [sp, #100]
	movs	r0, #0
	ldr	r3, [r1, #20]
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L_08188374
	ldr	r2, [sp, #88]
	movs	r3, #238
	lsls	r3, r3, #7
	movs	r6, #128
	adds	r3, #168
	lsls	r6, r6, #11
	movs	r5, #36
	adds	r7, r2, r3
.L_0818833a:
	ldr	r4, [sp, #100]
	movs	r3, #50
	ldrsh	r0, [r5, r4]
	str	r3, [sp, #4]
	movs	r3, #128
	lsls	r3, r3, #12
	movs	r1, #1
	adds	r2, r6, #0
	str	r6, [sp, #0]
	bl	sub_0815f000
	ldr	r2, [sp, #100]
	movs	r1, #7
	ldrsh	r0, [r5, r2]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r2, #5
	mov	r3, r8
	bl	sub_0814cd48
	movs	r3, #8
	str	r3, [r7, #0]
	ldr	r0, [sp, #100]
	movs	r4, #1
	ldr	r3, [r0, #20]
	add	r8, r4
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_0818833a
.L_08188374:
	ldr	r1, [sp, #76]
	cmp	r1, #58
	bne.n	.L_081883d6
	movs	r0, #145
	bl	sub_081c0010
	ldr	r4, [sp, #100]
	movs	r2, #0
	ldr	r3, [r4, #20]
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_081883d6
	ldr	r0, [sp, #88]
	movs	r1, #238
	lsls	r1, r1, #7
	movs	r6, #128
	adds	r1, #168
	lsls	r6, r6, #11
	movs	r5, #36
	adds	r7, r0, r1
.L_0818839c:
	ldr	r2, [sp, #100]
	movs	r1, #1
	ldrsh	r0, [r5, r2]
	movs	r3, #200
	str	r3, [sp, #4]
	movs	r3, #128
	adds	r2, r6, #0
	lsls	r3, r3, #12
	str	r6, [sp, #0]
	bl	sub_0815f000
	ldr	r4, [sp, #100]
	movs	r3, #16
	ldrsh	r0, [r5, r4]
	movs	r2, #5
	str	r3, [sp, #0]
	movs	r1, #7
	mov	r3, r8
	bl	sub_0814cd48
	movs	r3, #8
	str	r3, [r7, #0]
	ldr	r4, [sp, #100]
	movs	r2, #1
	ldr	r3, [r4, #20]
	add	r8, r2
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_0818839c
.L_081883d6:
	movs	r0, #32
	bl	sub_08014dac
	adds	r7, r0, #0
	movs	r0, #1
	bl	sub_081969f8
	movs	r3, #8
	adds	r6, r0, #0
	negs	r3, r3
	str	r3, [r6, #20]
	movs	r3, #7
	str	r3, [r6, #0]
	add	r5, sp, #104
	movs	r3, #6
	strb	r3, [r5, #0]
	mov	r2, r9
	movs	r3, #5
	strb	r3, [r2, #1]
	ldr	r3, [pc, #296]
	str	r2, [r6, #16]
	str	r3, [r6, #8]
	str	r7, [r6, #12]
	ldr	r0, [sp, #88]
	movs	r1, #224
	lsls	r1, r1, #3
	adds	r3, r0, r1
	str	r3, [r2, #4]
	ldr	r3, [sp, #20]
	movs	r2, #127
	ands	r3, r2
	strb	r3, [r6, #24]
	movs	r2, #0
	mov	r8, r2
.L_0818841a:
	ldr	r3, [pc, #272]
	mov	r4, r8
	ldrb	r2, [r3, r4]
	ldr	r5, [sp, #76]
	cmp	r5, r2
	blt.n	.L_081884b0
	adds	r3, r2, #0
	adds	r3, #16
	cmp	r5, r3
	bge.n	.L_081884b0
	subs	r0, r5, r2
	lsls	r0, r0, #11
	bl	sub_08002096
	cmp	r0, #0
	bge.n	.L_0818843c
	adds	r0, #7
.L_0818843c:
	asrs	r0, r0, #3
	movs	r1, #4
	ldr	r5, [pc, #236]
	mov	fp, r0
	negs	r1, r1
	movs	r0, #0
	mov	sl, r0
	mov	r9, r1
.L_0818844c:
	bl	sub_08014de4
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_0818846a
	mov	r3, r9
	adds	r0, r5, #0
	lsls	r1, r3, #16
	movs	r2, #0
	bl	sub_08015160
	ldr	r0, [pc, #208]
	bl	sub_080150e4
	b.n	.L_08188482
.L_0818846a:
	ldr	r4, [pc, #196]
	mov	r2, r9
	adds	r0, r5, r4
	lsls	r1, r2, #16
	movs	r2, #0
	bl	sub_08015160
	movs	r0, #156
	lsls	r0, r0, #6
	adds	r0, #208
	bl	sub_080150e4
.L_08188482:
	movs	r0, #192
	movs	r2, #128
	lsls	r0, r0, #10
	mov	r1, fp
	lsls	r2, r2, #10
	bl	sub_080151e4
	ldr	r0, [pc, #164]
	adds	r1, r7, #0
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
	movs	r4, #1
	movs	r3, #160
	add	sl, r4
	lsls	r3, r3, #13
	mov	r0, sl
	adds	r5, r5, r3
	cmp	r0, #3
	bne.n	.L_0818844c
.L_081884b0:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #2
	bne.n	.L_0818841a
	movs	r3, #6
	add	r4, sp, #104
	strb	r3, [r4, #0]
	adds	r1, r4, #0
	movs	r3, #5
	strb	r3, [r1, #1]
	ldr	r3, [pc, #96]
	mov	r9, r1
	str	r3, [r6, #8]
	movs	r3, #7
	str	r3, [r6, #0]
	ldr	r3, [sp, #76]
	subs	r3, #56
	cmp	r3, #31
	bhi.n	.L_08188570
	ldr	r5, [sp, #76]
	movs	r3, #0
	cmp	r5, #79
	ble.n	.L_081884e6
	movs	r3, #80
	subs	r3, r3, r5
	lsls	r3, r3, #3
.L_081884e6:
	str	r3, [r6, #20]
	ldr	r2, [sp, #20]
	movs	r3, #127
	ands	r2, r3
	strb	r2, [r6, #24]
	ldr	r0, [sp, #88]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r3, r0, r2
	str	r3, [r1, #4]
	ldr	r3, [sp, #76]
	cmp	r3, #71
	bgt.n	.L_08188508
	ldr	r4, [pc, #56]
	lsls	r0, r3, #10
	adds	r0, r0, r4
	b.n	.L_08188514
.L_08188508:
	ldr	r5, [sp, #76]
	cmp	r5, #79
	ble.n	.L_08188544
	ldr	r1, [pc, #48]
	lsls	r0, r5, #10
	adds	r0, r0, r1
.L_08188514:
	bl	sub_08002096
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #4
	asrs	r0, r3, #16
	b.n	.L_08188546
	movs	r0, r0
	.4byte 0xffff8000
	.4byte 0x081992f8
	.4byte 0x08199d2f
	.4byte 0xfff00000
	.4byte 0xffffe000
	.4byte 0x081991e0
	.4byte 0xffff2000
	.2byte 0x0000
	.2byte 0xffff
.L_08188544:
	.2byte 0x2050
.L_08188546:
	ldr	r3, [pc, #36]
	movs	r1, #4
	subs	r3, r3, r0
	strh	r3, [r7, #2]
	strh	r3, [r7, #10]
	adds	r3, r0, #0
	movs	r2, #123
	adds	r3, #60
	strh	r1, [r7, #0]
	strh	r2, [r7, #8]
	strh	r1, [r7, #16]
	strh	r3, [r7, #18]
	strh	r2, [r7, #24]
	strh	r3, [r7, #26]
	adds	r0, r6, #0
	bl	sub_08196a7c
	b.n	.L_08188570
	movs	r0, r0
	.2byte 0x003c
	.2byte 0x0000
.L_08188570:
	adds	r0, r6, #0
	bl	sub_08013164
	adds	r0, r7, #0
	bl	sub_08013164
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r4, #240
	ldr	r3, [sp, #88]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r5, [sp, #20]
	ldr	r7, [sp, #76]
	adds	r5, #8
	adds	r7, #1
	str	r5, [sp, #20]
	str	r7, [sp, #76]
	cmp	r7, #92
	beq.n	.L_081885ae
	b.n	.L_08187fe4
.L_081885ae:
	ldr	r1, [sp, #88]
	movs	r2, #238
	lsls	r2, r2, #7
	movs	r0, #0
	adds	r2, #220
	mov	r8, r0
	adds	r5, r1, r2
.L_081885bc:
	ldmia	r5!, {r0}
	bl	sub_08020048
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	cmp	r4, #11
	bne.n	.L_081885bc
	ldr	r0, [pc, #28]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #192
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x3001
	.2byte 0x0814
.L_081885f0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	movs	r3, #15
	str	r1, [sp, #20]
	movs	r1, #0
	str	r1, [sp, #16]
	mov	fp, r3
	subs	r1, #36
	movs	r3, #0
	str	r0, [sp, #24]
	str	r1, [sp, #12]
	str	r3, [sp, #8]
	adds	r4, r2, #0
.L_08188616:
	movs	r1, #0
	mov	sl, r1
	cmp	r4, #0
	beq.n	.L_081886de
	ldr	r3, [sp, #8]
	mov	r9, r1
	mov	r8, r3
.L_08188624:
	movs	r0, #128
	adds	r1, r4, #0
	lsls	r0, r0, #9
	str	r4, [sp, #0]
	bl	sub_08002054
	mov	r6, sl
	muls	r6, r0
	adds	r0, r6, #0
	bl	sub_08002096
	mov	r7, r8
	ldr	r1, [sp, #24]
	lsls	r3, r0, #1
	add	r7, sl
	adds	r3, r3, r0
	lsls	r5, r7, #2
	adds	r5, r5, r1
	lsrs	r3, r3, #11
	strb	r3, [r5, #0]
	add	r3, sp, #12
	ldrb	r3, [r3, #0]
	adds	r0, r6, #0
	strb	r3, [r5, #1]
	bl	sub_08002090
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsrs	r3, r3, #11
	strb	r3, [r5, #2]
	ldr	r1, [sp, #16]
	ldr	r4, [sp, #0]
	cmp	r1, #0
	bgt.n	.L_081886d4
	ldr	r3, [sp, #20]
	lsls	r5, r7, #1
	adds	r5, r5, r7
	lsls	r5, r5, #3
	adds	r5, r5, r3
	mov	r1, sl
	lsls	r2, r1, #3
	adds	r6, r5, #0
	mov	r1, r9
	adds	r6, #12
	strb	r1, [r5, #5]
	adds	r3, r2, #7
	mov	r1, fp
	mov	r7, sl
	strb	r3, [r5, #4]
	strb	r3, [r5, #6]
	strb	r1, [r5, #7]
	strb	r2, [r5, #8]
	strb	r1, [r5, #9]
	adds	r7, #1
	mov	r1, r9
	strb	r3, [r6, #6]
	mov	r3, fp
	strb	r3, [r6, #9]
	strb	r2, [r6, #4]
	strb	r1, [r6, #5]
	strb	r1, [r6, #7]
	strb	r2, [r6, #8]
	adds	r1, r4, #0
	adds	r0, r7, #0
	bl	sub_08002064
	ldr	r4, [sp, #0]
	mov	r1, r8
	adds	r2, r0, r1
	adds	r0, r0, r4
	add	r0, r8
	strb	r2, [r5, #0]
	strb	r0, [r5, #1]
	adds	r1, r4, #0
	mov	r0, sl
	str	r2, [sp, #4]
	bl	sub_08002064
	ldr	r4, [sp, #0]
	ldr	r2, [sp, #4]
	adds	r3, r0, r4
	add	r3, r8
	add	r0, r8
	strb	r3, [r5, #2]
	strb	r0, [r6, #0]
	strb	r2, [r6, #1]
	strb	r3, [r6, #2]
	b.n	.L_081886d8
.L_081886d4:
	mov	r7, sl
	adds	r7, #1
.L_081886d8:
	mov	sl, r7
	cmp	sl, r4
	bne.n	.L_08188624
.L_081886de:
	ldr	r3, [sp, #12]
	ldr	r1, [sp, #8]
	adds	r3, #18
	str	r3, [sp, #12]
	ldr	r3, [sp, #16]
	adds	r1, r1, r4
	adds	r3, #1
	str	r1, [sp, #8]
	str	r3, [sp, #16]
	cmp	r3, #2
	bne.n	.L_08188616
	ldr	r1, [sp, #20]
	lsls	r3, r4, #1
	adds	r3, r3, r4
	lsls	r3, r3, #3
	movs	r2, #0
	adds	r3, r3, r1
	strb	r2, [r3, #0]
	strb	r2, [r3, #1]
	strb	r2, [r3, #2]
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #104
	str	r0, [sp, #56]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	str	r0, [sp, #52]
	movs	r0, #0
	ldr	r3, [r3, #96]
	str	r3, [sp, #48]
	bl	sub_081435e0
	ldr	r2, [sp, #52]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #84]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r3, #0
	ldr	r0, [pc, #76]
	ldr	r1, [pc, #80]
	movs	r2, #0
	bl	sub_08157cf4
	ldr	r4, [sp, #56]
	ldr	r3, [r4, #24]
	cmp	r3, #1
	bne.n	.L_081887ac
	ldr	r0, [pc, #68]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x20ee
	ldr	r5, [sp, #52]
	lsls	r0, r0, #7
	adds	r0, #132
	adds	r2, r5, r0
	movs	r3, #50
	str	r3, [r2, #0]
	movs	r0, #104
	movs	r1, #19
	bl	sub_081963ec
	movs	r2, #128
	ldr	r3, [pc, #8]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	b.n	.L_081887cc
	.4byte 0x00001010
	.4byte 0x0000013e
	.4byte 0x000000c2
	.4byte 0x02010000
	.4byte 0x00000149
	.2byte 0x0730
	.2byte 0x0300
.L_081887ac:
	ldr	r1, [sp, #52]
	movs	r3, #239
	movs	r4, #238
	lsls	r3, r3, #7
	lsls	r4, r4, #7
	adds	r2, r1, r3
	adds	r4, #132
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r1, r4
	movs	r3, #75
	str	r3, [r2, #0]
	movs	r0, #104
	movs	r1, #19
	bl	sub_081963ec
.L_081887cc:
	ldr	r5, [sp, #52]
	movs	r0, #239
	lsls	r0, r0, #7
	adds	r2, r5, r0
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #780]
	str	r3, [sp, #36]
	bl	sub_080145a8
	ldr	r1, [sp, #56]
	ldr	r0, [r1, #8]
	bl	sub_08118098
	ldr	r3, [sp, #56]
	adds	r5, r0, #0
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r5, [r5, #0]
	str	r0, [sp, #32]
	movs	r4, #90
	ldr	r6, [r5, #8]
	ldr	r3, [r0, #8]
	mov	r8, r4
	subs	r3, r3, r6
	mov	r0, r8
	muls	r0, r3
	movs	r1, #100
	bl	sub_08002054
	mov	r9, r5
	ldr	r5, [sp, #32]
	adds	r6, r6, r0
	mov	r0, r9
	ldr	r3, [r5, #16]
	ldr	r5, [r0, #16]
	movs	r1, #100
	subs	r3, r3, r5
	mov	r0, r8
	muls	r0, r3
	bl	sub_08002054
	ldr	r1, [sp, #56]
	adds	r5, r5, r0
	ldr	r0, [r1, #8]
	bl	sub_08118070
	ldr	r3, [sp, #56]
	str	r0, [sp, #28]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_08118070
	ldr	r1, [sp, #56]
	str	r0, [sp, #24]
	mov	r2, sp
	adds	r2, #92
	movs	r4, #36
	ldrsh	r0, [r1, r4]
	adds	r1, r2, #0
	str	r2, [sp, #20]
	bl	sub_0815e21c
	mov	r0, r9
	bl	sub_08020138
	adds	r3, r5, #0
	movs	r2, #0
	mov	r0, r9
	adds	r1, r6, #0
	bl	sub_08020148
	movs	r1, #2
	mov	r0, r9
	bl	sub_08020090
	mov	r3, r9
	movs	r2, #1
	adds	r3, #88
	strb	r2, [r3, #0]
	adds	r3, #2
	str	r3, [sp, #16]
	strb	r2, [r3, #0]
	movs	r3, #128
	mov	r4, r9
	lsls	r3, r3, #10
	str	r3, [r4, #52]
	movs	r3, #128
	lsls	r3, r3, #12
	movs	r0, #20
	str	r3, [r4, #48]
	bl	sub_08013560
	movs	r0, #56
	movs	r1, #92
	movs	r5, #0
	negs	r0, r0
	negs	r1, r1
	str	r5, [sp, #44]
	str	r0, [sp, #12]
	str	r1, [sp, #8]
.L_081888a6:
	ldr	r2, [sp, #44]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #48]
	cmp	r2, #0
	bne.n	.L_081888ca
	ldr	r3, [sp, #32]
	movs	r2, #240
	lsls	r2, r2, #12
	str	r2, [r3, #40]
	ldr	r4, [sp, #32]
	movs	r3, #145
	lsls	r3, r3, #8
	adds	r3, #235
	mov	r0, r9
	str	r3, [r4, #72]
	str	r2, [r0, #40]
	str	r3, [r0, #72]
.L_081888ca:
	ldr	r1, [sp, #44]
	cmp	r1, #11
	bne.n	.L_081888f0
	ldr	r2, [sp, #32]
	mov	r4, r9
	ldr	r3, [r2, #28]
	negs	r3, r3
	str	r3, [r2, #28]
	ldr	r3, [r4, #28]
	negs	r3, r3
	str	r3, [r4, #28]
	ldr	r0, [sp, #28]
	ldr	r3, [r4, #12]
	adds	r3, r3, r0
	str	r3, [r4, #12]
	ldr	r1, [sp, #24]
	ldr	r3, [r2, #12]
	adds	r3, r3, r1
	str	r3, [r2, #12]
.L_081888f0:
	ldr	r2, [sp, #44]
	cmp	r2, #54
	bne.n	.L_08188950
	ldr	r4, [sp, #56]
	movs	r1, #7
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	movs	r3, #10
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	ldr	r0, [sp, #32]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r0, #40]
	movs	r3, #171
	lsls	r3, r3, #8
	adds	r3, #133
	str	r3, [r0, #72]
	movs	r3, #160
	mov	r1, r9
	lsls	r3, r3, #11
	str	r3, [r1, #40]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #81
	str	r3, [r1, #72]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r1, #52]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r1, #48]
	ldr	r2, [sp, #16]
	movs	r3, #0
	strb	r3, [r2, #0]
	mov	r0, r9
	bl	sub_08020138
	mov	r4, r9
	ldr	r3, [r4, #16]
	mov	r0, r9
	movs	r1, #0
	movs	r2, #0
	bl	sub_08020148
.L_08188950:
	ldr	r0, [sp, #56]
	ldr	r3, [r0, #24]
	cmp	r3, #0
	bne.n	.L_08188a1e
	bl	sub_08014de4
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	sub_080156e8
	mov	r1, r9
	ldr	r3, [r1, #8]
	add	r0, sp, #80
	str	r3, [r0, #0]
	movs	r2, #68
	ldr	r3, [r1, #12]
	add	r2, sp
	str	r3, [r0, #4]
	mov	sl, r2
	ldr	r3, [r1, #16]
	mov	r1, sl
	str	r3, [r0, #8]
	bl	sub_0815e1ec
	mov	r4, sl
	ldr	r3, [r4, #0]
	asrs	r2, r3, #1
	str	r2, [r4, #0]
	ldr	r3, [sp, #44]
	subs	r3, #54
	cmp	r3, #1
	bhi.n	sub_081889b0
	ldr	r3, [r4, #4]
	ldr	r5, [sp, #52]
	movs	r1, #32
	movs	r4, #224
	str	r1, [sp, #0]
	lsls	r4, r4, #3
	movs	r1, #64
	str	r1, [sp, #4]
	subs	r2, #16
	adds	r1, r5, r4
	subs	r3, #16
	ldr	r0, [sp, #48]
	ldr	r5, [sp, #36]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9803
	cmp	r0, #11
	bls.n	.L_081889b8
	b.n	.L_08188af8
.L_081889b8:
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	ldr	r2, [sp, #52]
	asrs	r3, r3, #1
	lsls	r3, r3, #11
	ldr	r7, [sp, #8]
	movs	r1, #0
	adds	r2, r2, r3
	mov	r8, r1
	mov	fp, r2
.L_081889cc:
	mov	r3, r8
	lsls	r6, r3, #12
	adds	r0, r6, #0
	bl	sub_08002096
	mov	r4, sl
	adds	r3, r7, #0
	muls	r3, r0
	ldr	r5, [r4, #0]
	asrs	r3, r3, #16
	adds	r0, r6, #0
	adds	r5, r5, r3
	bl	sub_08002090
	adds	r3, r7, #0
	muls	r3, r0
	ldr	r0, [sp, #44]
	movs	r2, #32
	subs	r5, #16
	str	r2, [sp, #0]
	asrs	r3, r3, #16
	movs	r2, #64
	movs	r1, #224
	subs	r3, r3, r0
	str	r2, [sp, #4]
	lsls	r1, r1, #3
	adds	r2, r5, #0
	movs	r5, #1
	adds	r3, #100
	ldr	r0, [sp, #48]
	add	r1, fp
	ldr	r4, [sp, #36]
	add	r8, r5
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4640
	cmp	r0, #16
	bne.n	.L_081889cc
	ldr	r1, [sp, #56]
	ldr	r3, [r1, #24]
	b.n	.L_08188afc
.L_08188a1e:
	ldr	r2, [sp, #44]
	cmp	r2, #55
	ble.n	.L_08188afc
	cmp	r2, #56
	bne.n	.L_08188a8e
	ldr	r4, [sp, #20]
	ldr	r7, [sp, #52]
	movs	r3, #0
	mov	r8, r3
	mov	sl, r4
.L_08188a32:
	mov	r5, sl
	ldr	r3, [r5, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	ldr	r3, [r5, #4]
	movs	r5, #255
	subs	r3, #16
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08014878
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #64
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	mov	r0, r8
	asrs	r3, r3, #5
	str	r3, [r7, #16]
	negs	r3, r0
	cmp	r3, #0
	bge.n	.L_08188a7e
	adds	r3, #3
.L_08188a7e:
	movs	r1, #1
	add	r8, r1
	asrs	r3, r3, #2
	mov	r2, r8
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r2, #32
	bne.n	.L_08188a32
.L_08188a8e:
	ldr	r5, [sp, #52]
	movs	r3, #0
	mov	r8, r3
.L_08188a94:
	ldr	r3, [r5, #24]
	cmp	r3, #23
	bhi.n	.L_08188ad8
	adds	r1, r3, #0
	cmp	r3, #0
	bge.n	.L_08188aa2
	adds	r1, r3, #3
.L_08188aa2:
	ldr	r4, [sp, #52]
	asrs	r1, r1, #2
	lsls	r1, r1, #11
	movs	r0, #224
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	adds	r1, r4, r1
	lsls	r0, r0, #3
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	adds	r1, r1, r0
	movs	r0, #32
	str	r0, [sp, #0]
	movs	r0, #64
	subs	r3, #32
	str	r0, [sp, #4]
	subs	r2, #16
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #62
	ldr	r2, [pc, #32]
	bl	sub_08138086
	ldr	r3, [r5, #24]
.L_08188ad8:
	movs	r0, #1
	add	r8, r0
	adds	r3, #1
	mov	r1, r8
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r1, #16
	bne.n	.L_08188a94
	ldr	r2, [sp, #56]
	ldr	r3, [r2, #24]
	b.n	.L_08188afc
	movs	r0, r0
	.4byte 0x08143001
	.2byte 0xe000
	.2byte 0xffff
.L_08188af8:
	.2byte 0x9c0e
	ldr	r3, [r4, #24]
.L_08188afc:
	cmp	r3, #1
	bne.n	.L_08188bca
	movs	r0, #128
	lsls	r0, r0, #2
	bl	sub_08014dac
	mov	sl, r0
	movs	r0, #1
	bl	sub_081969f8
	adds	r5, r0, #0
	ldr	r0, [sp, #44]
	cmp	r0, #55
	ble.n	.L_08188bbe
	ldr	r1, [sp, #12]
	movs	r2, #128
	lsls	r3, r1, #13
	lsls	r2, r2, #7
	adds	r6, r3, r2
	ldr	r3, [sp, #44]
	movs	r0, #0
	cmp	r3, #63
	ble.n	.L_08188b32
	ldr	r4, [sp, #44]
	movs	r3, #64
	subs	r3, r3, r4
	lsls	r0, r3, #2
.L_08188b32:
	movs	r1, #64
	negs	r1, r1
	cmp	r0, r1
	ble.n	.L_08188bbe
	ldr	r3, [sp, #60]
	ldr	r2, [pc, #340]
	movs	r1, #7
	ands	r3, r2
	ldr	r2, [pc, #340]
	orrs	r3, r1
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #60]
	ldr	r3, [pc, #328]
	ldr	r4, [pc, #332]
	add	r2, sp, #60
	str	r3, [r5, #8]
	mov	r3, sl
	str	r1, [r5, #0]
	str	r2, [r5, #16]
	str	r3, [r5, #12]
	str	r4, [r2, #4]
	str	r0, [r5, #20]
	movs	r7, #128
	movs	r0, #0
	mov	r8, r0
	lsls	r7, r7, #6
.L_08188b6c:
	bl	sub_08014de4
	ldr	r1, [sp, #20]
	movs	r2, #0
	ldr	r0, [r1, #0]
	movs	r1, #128
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #64
	lsls	r0, r0, #16
	lsls	r1, r1, #14
	bl	sub_08015160
	adds	r1, r6, #0
	adds	r2, r6, #0
	adds	r0, r6, #0
	bl	sub_080151e4
	adds	r0, r7, #0
	bl	sub_080150e4
	ldr	r0, [pc, #264]
	bl	sub_08015024
	movs	r2, #4
	ldr	r0, [pc, #260]
	mov	r1, sl
	bl	sub_08196958
	adds	r0, r5, #0
	bl	sub_08196a7c
	movs	r3, #1
	movs	r2, #128
	add	r8, r3
	lsls	r2, r2, #7
	mov	r4, r8
	adds	r7, r7, r2
	cmp	r4, #2
	bne.n	.L_08188b6c
.L_08188bbe:
	adds	r0, r5, #0
	bl	sub_08013164
	mov	r0, sl
	bl	sub_08013164
.L_08188bca:
	ldr	r5, [sp, #44]
	cmp	r5, #64
	bne.n	.L_08188bf8
	ldr	r0, [sp, #32]
	mov	r1, r9
	ldr	r3, [r0, #28]
	negs	r3, r3
	str	r3, [r0, #28]
	ldr	r3, [r1, #28]
	negs	r3, r3
	str	r3, [r1, #28]
	ldr	r2, [sp, #28]
	ldr	r3, [r1, #12]
	subs	r3, r3, r2
	str	r3, [r1, #12]
	ldr	r4, [sp, #24]
	ldr	r3, [r0, #12]
	movs	r1, #0
	subs	r3, r3, r4
	str	r3, [r0, #12]
	mov	r0, r9
	bl	sub_08020090
.L_08188bf8:
	ldr	r5, [sp, #44]
	cmp	r5, #54
	bne.n	.L_08188c14
	ldr	r0, [sp, #56]
	ldr	r3, [r0, #24]
	cmp	r3, #0
	bne.n	.L_08188c0e
	movs	r0, #134
	bl	sub_081180e8
	b.n	.L_08188c14
.L_08188c0e:
	movs	r0, #145
	bl	sub_081180e8
.L_08188c14:
	ldr	r1, [sp, #44]
	cmp	r1, #0
	bne.n	.L_08188c2e
	movs	r0, #136
	bl	sub_081c0010
	ldr	r3, [sp, #52]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	movs	r3, #6
	str	r3, [r2, #0]
.L_08188c2e:
	ldr	r5, [sp, #44]
	cmp	r5, #53
	bne.n	.L_08188c42
	ldr	r0, [sp, #52]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r2, r0, r1
	movs	r3, #6
	str	r3, [r2, #0]
.L_08188c42:
	movs	r1, #16
	movs	r0, #16
	bl	sub_08158ce0
	movs	r4, #240
	ldr	r3, [sp, #52]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	ldr	r5, [sp, #12]
	ldr	r0, [sp, #8]
	ldr	r1, [sp, #44]
	adds	r5, #1
	adds	r0, #2
	adds	r1, #1
	str	r5, [sp, #12]
	str	r0, [sp, #8]
	str	r1, [sp, #44]
	cmp	r1, #96
	beq.n	.L_08188c76
	b.n	.L_081888a6
.L_08188c76:
	ldr	r0, [pc, #52]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #104
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199364
	.4byte 0x02010000
	.4byte 0xfffff000
	.4byte 0x08199210
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
	ldr	r1, [r3, #92]
	ldr	r3, [r3, #96]
	sub	sp, #40
	mov	sl, r0
	movs	r0, #1
	str	r3, [sp, #16]
	mov	fp, r1
	bl	sub_081435e0
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	mov	r2, sl
	ldr	r3, [r2, #24]
	cmp	r3, #0
	bne.n	.L_08188cf0
	movs	r2, #128
	ldr	r3, [pc, #32]
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
.L_08188cf0:
	movs	r5, #224
	lsls	r5, r5, #3
	add	r5, fp
	movs	r3, #1
	ldr	r0, [pc, #20]
	adds	r1, r5, #0
	movs	r2, #1
	bl	sub_08157cf4
	mov	r4, sl
	ldr	r3, [r4, #24]
	b.n	.L_08188d14
	.4byte 0x00001010
	.4byte 0x00000100
	.2byte 0x0123
	.2byte 0x0000
.L_08188d14:
	cmp	r3, #0
	bne.n	.L_08188d34
	movs	r1, #139
	lsls	r1, r1, #7
	movs	r2, #1
	movs	r3, #0
	ldr	r0, [pc, #872]
	add	r1, fp
	bl	sub_08157cf4
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, fp
	movs	r3, #50
	b.n	.L_08188d90
.L_08188d34:
	movs	r6, #0
	movs	r2, #250
	mov	r8, r6
	movs	r0, #63
	lsls	r2, r2, #6
	adds	r1, r5, #0
.L_08188d40:
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_08188d58
	lsls	r3, r3, #1
	adds	r3, #32
	movs	r4, #252
	strb	r3, [r1, #0]
	lsls	r4, r4, #22
	lsls	r3, r3, #24
	cmp	r3, r4
	bls.n	.L_08188d58
	strb	r0, [r1, #0]
.L_08188d58:
	movs	r6, #1
	add	r8, r6
	adds	r1, #1
	cmp	r8, r2
	bne.n	.L_08188d40
	movs	r1, #139
	lsls	r1, r1, #7
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [pc, #800]
	bl	sub_08157cf4
	ldr	r0, [pc, #800]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #792]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x22ee
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, fp
	movs	r3, #75
.L_08188d90:
	str	r3, [r2, #0]
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #760]
	bl	sub_080145a8
	mov	r1, sl
	ldr	r0, [r1, #4]
	add	r1, sp, #20
	bl	sub_08144aac
	movs	r2, #0
	mov	r3, fp
	mov	r8, r2
	adds	r3, #24
.L_08188db8:
	movs	r4, #1
	add	r8, r4
	mov	r6, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r6, #64
	bne.n	.L_08188db8
	mov	r1, sl
	movs	r3, #224
	ldr	r0, [r1, #8]
	lsls	r3, r3, #11
	movs	r2, #36
	ldrsh	r1, [r1, r2]
	movs	r2, #16
	bl	sub_08118078
	movs	r6, #8
	movs	r3, #0
	negs	r6, r6
	movs	r4, #28
	str	r3, [sp, #12]
	str	r6, [sp, #8]
	add	r4, sp
	mov	r9, r4
.L_08188de8:
	mov	r1, sl
	ldr	r3, [r1, #24]
	cmp	r3, #0
	bne.n	.L_08188e94
	ldr	r0, [r1, #8]
	mov	r1, r9
	bl	sub_0815e21c
	mov	r2, sl
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_08188e0a
	mov	r3, r9
	ldr	r2, [r3, #0]
	movs	r1, #128
	movs	r3, #80
	b.n	.L_08188e12
.L_08188e0a:
	mov	r4, r9
	ldr	r2, [r4, #0]
	movs	r1, #128
	movs	r3, #32
.L_08188e12:
	lsls	r1, r1, #19
	subs	r3, r3, r2
	adds	r1, #40
	lsls	r3, r3, #8
	str	r3, [r1, #0]
	ldr	r6, [sp, #8]
	cmp	r6, #15
	bls.n	.L_08188e24
	b.n	sub_08188f3e
.L_08188e24:
	lsrs	r3, r6, #31
	adds	r3, r6, r3
	asrs	r5, r3, #1
	cmp	r5, #6
	ble.n	.L_08188e30
	movs	r5, #6
.L_08188e30:
	mov	r1, sl
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_08188e60
	ldr	r2, [pc, #612]
	lsls	r3, r5, #1
	ldrh	r1, [r2, r3]
	movs	r3, #224
	lsls	r3, r3, #3
	add	r1, fp
	adds	r1, r1, r3
	ldr	r3, [pc, #604]
	mov	r4, r9
	ldrsb	r2, [r3, r5]
	ldr	r3, [pc, #600]
	ldr	r0, [r4, #4]
	ldrsb	r3, [r3, r5]
	adds	r2, #30
	adds	r3, r3, r0
	ldr	r0, [pc, #596]
	subs	r3, #60
	ldrb	r0, [r0, r5]
	ldr	r4, [sp, #20]
	b.n	.L_08188eec
.L_08188e60:
	ldr	r6, [pc, #572]
	lsls	r3, r5, #1
	ldrh	r1, [r6, r3]
	ldr	r3, [pc, #572]
	movs	r2, #224
	lsls	r2, r2, #3
	add	r1, fp
	adds	r1, r1, r2
	ldrsb	r2, [r3, r5]
	ldr	r3, [pc, #568]
	mov	r6, r9
	ldrb	r4, [r3, r5]
	ldr	r3, [pc, #556]
	ldr	r0, [r6, #4]
	ldrsb	r3, [r3, r5]
	str	r4, [sp, #0]
	adds	r3, r3, r0
	ldr	r0, [pc, #556]
	negs	r2, r2
	ldrb	r0, [r0, r5]
	subs	r2, r2, r4
	str	r0, [sp, #4]
	adds	r2, #98
	subs	r3, #60
	ldr	r4, [sp, #20]
	b.n	.L_08188ef4
.L_08188e94:
	mov	r2, sl
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	mov	r1, r9
	bl	sub_0815e21c
	ldr	r3, [sp, #8]
	cmp	r3, #15
	bhi.n	sub_08188f3e
	ldr	r4, [sp, #8]
	lsrs	r3, r3, #31
	adds	r3, r4, r3
	asrs	r5, r3, #1
	cmp	r5, #6
	ble.n	.L_08188eb4
	movs	r5, #6
.L_08188eb4:
	mov	r6, sl
	ldr	r3, [r6, #4]
	cmp	r3, #0
	bne.n	.L_08188efc
	ldr	r2, [pc, #480]
	lsls	r3, r5, #1
	ldrh	r1, [r2, r3]
	mov	r4, r9
	ldr	r2, [r4, #0]
	movs	r3, #224
	lsls	r3, r3, #3
	add	r1, fp
	adds	r1, r1, r3
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [pc, #464]
	asrs	r2, r2, #1
	ldrsb	r3, [r3, r5]
	ldr	r0, [r4, #4]
	adds	r2, r2, r3
	ldr	r3, [pc, #456]
	subs	r2, #16
	ldrsb	r3, [r3, r5]
	ldr	r4, [sp, #20]
	adds	r3, r3, r0
	ldr	r0, [pc, #452]
	subs	r3, #92
	ldrb	r0, [r0, r5]
.L_08188eec:
	str	r0, [sp, #0]
	ldr	r0, [pc, #448]
	ldrb	r0, [r0, r5]
	str	r0, [sp, #4]
.L_08188ef4:
	ldr	r0, [sp, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe020
.L_08188efc:
	ldr	r6, [pc, #416]
	lsls	r3, r5, #1
	ldrh	r1, [r6, r3]
	movs	r2, #224
	lsls	r2, r2, #3
	mov	r3, r9
	add	r1, fp
	adds	r1, r1, r2
	ldr	r2, [r3, #0]
	mov	r6, r9
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [pc, #396]
	asrs	r2, r2, #1
	ldrsb	r3, [r3, r5]
	ldr	r0, [r6, #4]
	subs	r2, r2, r3
	ldr	r3, [pc, #396]
	ldrb	r4, [r3, r5]
	ldr	r3, [pc, #388]
	subs	r2, r2, r4
	ldrsb	r3, [r3, r5]
	str	r4, [sp, #0]
	adds	r3, r3, r0
	ldr	r0, [pc, #384]
	adds	r2, #16
	ldrb	r0, [r0, r5]
	subs	r3, #92
	str	r0, [sp, #4]
	ldr	r4, [sp, #20]
	ldr	r0, [sp, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9903
	cmp	r1, #18
	bne.n	.L_0818900c
	mov	r3, sl
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	mov	r1, r9
	bl	sub_0815e21c
	movs	r0, #134
	bl	sub_081180e8
	mov	r6, sl
	movs	r3, #8
	movs	r4, #36
	ldrsh	r0, [r6, r4]
	movs	r1, #7
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	movs	r1, #36
	ldrsh	r0, [r6, r1]
	movs	r1, #6
	bl	sub_08118088
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, fp
	movs	r3, #4
	str	r3, [r2, #0]
	movs	r4, #16
	ldr	r3, [r6, #24]
	movs	r2, #0
	lsls	r3, r3, #4
	negs	r4, r4
	mov	r8, r2
	cmp	r3, r4
	beq.n	.L_0818900c
	mov	r5, fp
.L_08188f92:
	bl	sub_08014878
	movs	r6, #128
	movs	r3, #255
	ands	r3, r0
	lsls	r6, r6, #1
	adds	r7, r3, r6
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r6, r0, #0
	mov	r1, sl
	ands	r6, r3
	ldr	r3, [r1, #24]
	cmp	r3, #0
	bne.n	.L_08188fbc
	movs	r3, #128
	lsls	r3, r3, #15
	b.n	.L_08188fc8
.L_08188fbc:
	mov	r2, r9
	ldr	r3, [r2, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
.L_08188fc8:
	str	r3, [r5, #0]
	movs	r3, #160
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r3, r7, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r5, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r7, #0
	muls	r3, r0
	negs	r3, r3
	asrs	r3, r3, #6
	str	r3, [r5, #16]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #16
	str	r3, [r5, #24]
	mov	r4, sl
	movs	r3, #1
	add	r8, r3
	ldr	r3, [r4, #24]
	adds	r5, #28
	lsls	r3, r3, #4
	adds	r3, #16
	cmp	r8, r3
	bne.n	.L_08188f92
.L_0818900c:
	movs	r6, #0
	mov	r8, r6
	mov	r5, fp
.L_08189012:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	ble.n	sub_081890e4
	subs	r3, #1
	str	r3, [r5, #24]
	movs	r1, #60
	adds	r0, r5, #0
	movs	r2, #0
	bl	sub_08138086
	ldr	r3, [r5, #4]
	movs	r1, #208
	lsls	r1, r1, #15
	cmp	r3, r1
	ble.n	.L_0818903e
	ldr	r3, [r5, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r5, #16]
	b.n	sub_081890e4
.L_0818903e:
	ldr	r2, [r5, #0]
	ldr	r4, [pc, #112]
	cmp	r2, r4
	bhi.n	sub_081890e4
	cmp	r3, #0
	blt.n	sub_081890e4
	mov	r1, sl
	asrs	r7, r3, #16
	ldr	r3, [r1, #24]
	asrs	r6, r2, #16
	cmp	r3, #0
	bne.n	.L_081890b8
	ldr	r0, [sp, #12]
	add	r0, r8
	cmp	r0, #0
	bge.n	.L_08189060
	adds	r0, #3
.L_08189060:
	movs	r1, #6
	asrs	r0, r0, #2
	bl	sub_08002064
	adds	r1, r0, #0
	lsls	r1, r1, #8
	movs	r2, #139
	lsls	r2, r2, #7
	add	r1, fp
	movs	r0, #16
	adds	r1, r1, r2
	adds	r3, r7, #0
	adds	r2, r6, #0
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, #8
	subs	r3, #8
	ldr	r4, [sp, #20]
	ldr	r0, [sp, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe02b
	.4byte 0x0000012e
	.4byte 0x0000016d
	.4byte 0x00000130
	.4byte 0x03000730
	.4byte 0x08143001
	.4byte 0x08199d40
	.4byte 0x08199d4e
	.4byte 0x08199d55
	.4byte 0x08199d31
	.4byte 0x08199d38
	.2byte 0xffff
	.2byte 0x007e
.L_081890b8:
	movs	r1, #3
	mov	r0, r8
	bl	sub_08002064
	lsls	r1, r0, #3
	adds	r1, r1, r0
	lsls	r1, r1, #6
	movs	r3, #139
	lsls	r3, r3, #7
	add	r1, fp
	movs	r0, #24
	adds	r1, r1, r3
	adds	r2, r6, #0
	adds	r3, r7, #0
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, #12
	subs	r3, #12
	ldr	r4, [sp, #20]
	ldr	r0, [sp, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2401
	add	r8, r4
	mov	r6, r8
	adds	r5, #28
	cmp	r6, #64
	bne.n	.L_08189012
	movs	r1, #8
	movs	r0, #8
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
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #12]
	adds	r1, #1
	adds	r2, #1
	str	r1, [sp, #8]
	str	r2, [sp, #12]
	cmp	r2, #70
	beq.n	.L_08189120
	b.n	.L_08188de8
.L_08189120:
	ldr	r0, [pc, #32]
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
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #248
	str	r0, [sp, #52]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #96]
	movs	r2, #0
	ldr	r0, [r3, #92]
	str	r1, [sp, #48]
	str	r2, [sp, #40]
	str	r2, [sp, #32]
	str	r2, [sp, #28]
	mov	fp, r0
	ldr	r3, [r3, #100]
	movs	r0, #0
	str	r3, [sp, #24]
	bl	sub_081435e0
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	movs	r1, #224
	adds	r2, #82
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	ldr	r0, [pc, #40]
	movs	r3, #1
	add	r1, fp
	movs	r2, #1
	bl	sub_08157cf4
	ldr	r4, [pc, #32]
	movs	r6, #234
	movs	r5, #236
	movs	r3, #1
	lsls	r6, r6, #2
	lsls	r5, r5, #5
	mov	r7, fp
	mov	ip, r4
	mov	r8, r3
	mov	lr, r5
	adds	r4, r7, r6
	b.n	.L_081891b8
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x00000186
	.2byte 0xf8f0
	.2byte 0xffff
.L_081891b8:
	mov	r1, r8
	mov	r2, ip
	lsls	r0, r1, #2
	adds	r1, r4, r2
	mov	r2, fp
	movs	r5, #0
	add	r2, lr
.L_081891c6:
	mov	r7, r8
	ldrb	r3, [r2, #0]
	adds	r2, #1
	cmp	r7, #10
	ble.n	.L_081891dc
	subs	r3, r3, r0
	adds	r3, #40
	cmp	r3, #0
	bge.n	.L_081891da
	movs	r3, #0
.L_081891da:
	strb	r3, [r1, #0]
.L_081891dc:
	adds	r5, #1
	adds	r1, #1
	cmp	r5, r6
	bne.n	.L_081891c6
	movs	r1, #1
	movs	r0, #234
	add	r8, r1
	lsls	r0, r0, #2
	mov	r2, r8
	adds	r4, r4, r0
	cmp	r2, #20
	bne.n	.L_081891b8
	ldr	r1, [sp, #24]
	ldr	r0, [pc, #524]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #134
	lsls	r1, r1, #7
	ldr	r0, [pc, #512]
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r3, #0
	ldr	r0, [pc, #504]
	ldr	r1, [pc, #504]
	movs	r2, #1
	bl	sub_08157cf4
	ldr	r4, [sp, #52]
	ldr	r3, [r4, #24]
	cmp	r3, #1
	bne.n	sub_08189238
	ldr	r0, [pc, #492]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #488]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9d0d
	mov	r7, sp
	adds	r7, #56
	ldr	r0, [r5, #4]
	adds	r1, r7, #0
	str	r7, [sp, #20]
	bl	sub_08144aac
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
	ldr	r0, [pc, #440]
	bl	sub_080145a8
	movs	r1, #36
	ldrsh	r0, [r5, r1]
	bl	sub_08118098
	ldr	r7, [r0, #0]
	movs	r2, #0
	mov	r5, fp
	mov	r8, r2
	movs	r6, #63
	adds	r5, #224
.L_0818927c:
	ldr	r3, [r7, #8]
	ldr	r2, [pc, #416]
	str	r3, [r5, #0]
	movs	r3, #0
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	str	r3, [r5, #8]
	ldr	r4, [sp, #52]
	ldr	r3, [r4, #24]
	ldrb	r3, [r2, r3]
	cmp	r8, r3
	blt.n	.L_081892aa
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	lsls	r0, r0, #12
	b.n	.L_081892be
.L_081892aa:
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	lsls	r0, r0, #13
.L_081892be:
	str	r0, [r5, #16]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r5, #20]
	movs	r3, #1
	add	r8, r3
	mov	r0, r8
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r0, #56
	bne.n	.L_0818927c
	ldr	r3, [pc, #328]
	movs	r1, #0
	movs	r2, #128
	mov	r8, r1
	lsls	r2, r2, #2
	subs	r1, #1
.L_081892e6:
	movs	r4, #1
	add	r8, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_081892e6
	movs	r5, #0
	mov	r8, r5
	ldr	r5, [pc, #304]
	movs	r6, #63
.L_081892fa:
	ldr	r3, [r7, #8]
	str	r3, [r5, #0]
	movs	r3, #0
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	str	r3, [r5, #8]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	lsls	r0, r0, #12
	str	r0, [r5, #16]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r5, #20]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	movs	r0, #1
	add	r8, r0
	adds	r3, #24
	mov	r1, r8
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r1, #128
	bne.n	.L_081892fa
	ldr	r6, [pc, #232]
	movs	r2, #0
	mov	r8, r2
	mov	r5, fp
.L_08189348:
	ldr	r4, [sp, #52]
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne.n	.L_08189362
	adds	r0, r6, #0
	bl	sub_08002096
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
	asrs	r3, r3, #16
	adds	r3, #88
	b.n	.L_08189374
.L_08189362:
	adds	r0, r6, #0
	bl	sub_08002096
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
	negs	r3, r3
	asrs	r3, r3, #16
	adds	r3, #16
.L_08189374:
	str	r3, [r5, #0]
	adds	r0, r6, #0
	bl	sub_08002090
	lsls	r0, r0, #4
	asrs	r0, r0, #16
	adds	r0, #40
	str	r0, [r5, #4]
	mov	r7, r8
	movs	r1, #1
	lsls	r3, r7, #1
	movs	r0, #128
	add	r8, r1
	negs	r3, r3
	lsls	r0, r0, #5
	mov	r2, r8
	str	r3, [r5, #24]
	adds	r6, r6, r0
	adds	r5, #28
	cmp	r2, #8
	bne.n	.L_08189348
	ldr	r0, [pc, #144]
	bl	sub_08013300
	movs	r3, #0
	str	r0, [sp, #36]
	str	r3, [sp, #44]
.L_081893aa:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #48]
	ldr	r4, [sp, #44]
	str	r3, [sp, #16]
	cmp	r4, #83
	bne.n	.L_081893ce
	ldr	r5, [sp, #52]
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L_081893c8
	movs	r0, #134
	bl	sub_081180e8
	b.n	.L_081893ce
.L_081893c8:
	movs	r0, #145
	bl	sub_081180e8
.L_081893ce:
	ldr	r7, [sp, #44]
	cmp	r7, #0
	bne.n	.L_081893da
	movs	r0, #136
	bl	sub_081c0010
.L_081893da:
	ldr	r0, [sp, #44]
	cmp	r0, #50
	bne.n	.L_081893e6
	movs	r0, #136
	bl	sub_081c0010
.L_081893e6:
	ldr	r1, [sp, #52]
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_08189438
	ldr	r2, [sp, #44]
	cmp	r2, #63
	bgt.n	.L_0818944c
	ldr	r4, [sp, #16]
	ldr	r5, [pc, #60]
	ldrh	r3, [r4, #54]
	adds	r7, r4, #0
	adds	r3, r3, r5
	strh	r3, [r7, #54]
	b.n	.L_0818944c
	movs	r0, r0
	.4byte 0x00000134
	.4byte 0x00000178
	.4byte 0x0000013e
	.4byte 0x02010000
	.4byte 0x00000148
	.4byte 0x03000730
	.4byte 0x08143001
	.4byte 0x08199d5c
	.4byte 0x02014018
	.4byte 0x02014000
	.4byte 0xffffc000
	.4byte 0x00000196
	.2byte 0xff00
	.2byte 0xffff
.L_08189438:
	ldr	r0, [sp, #44]
	cmp	r0, #63
	bgt.n	.L_0818944c
	ldr	r1, [sp, #16]
	movs	r2, #128
	ldrh	r3, [r1, #54]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r4, r1, #0
	strh	r3, [r4, #54]
.L_0818944c:
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #100
	bl	sub_08118028
	ldr	r5, [sp, #44]
	cmp	r5, #17
	bgt.n	sub_081894c2
	adds	r0, r5, #0
	movs	r1, #3
	bl	sub_08002054
	ldr	r3, [pc, #660]
	adds	r5, r0, #0
	ldr	r0, [pc, #660]
	lsls	r7, r5, #1
	ldrh	r1, [r0, r7]
	ldrb	r0, [r3, r5]
	ldr	r4, [pc, #656]
	movs	r2, #134
	lsls	r2, r2, #7
	mov	sl, r2
	lsrs	r3, r0, #1
	movs	r2, #56
	subs	r2, r2, r3
	ldr	r6, [pc, #644]
	ldrb	r3, [r4, r5]
	str	r0, [sp, #0]
	add	r1, fp
	ldrb	r0, [r6, r5]
	add	r1, sl
	mov	r8, r4
	adds	r3, #60
	str	r0, [sp, #4]
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4899
	ldr	r2, [pc, #604]
	ldrh	r1, [r0, r7]
	ldrb	r0, [r2, r5]
	mov	r4, r8
	lsrs	r3, r0, #1
	movs	r2, #64
	subs	r2, r2, r3
	ldrb	r3, [r4, r5]
	str	r0, [sp, #0]
	add	r1, fp
	ldrb	r0, [r6, r5]
	ldr	r5, [sp, #20]
	str	r0, [sp, #4]
	add	r1, sl
	adds	r3, #60
	ldr	r4, [r5, #4]
	ldr	r0, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9f0d
	ldr	r3, [r7, #24]
	cmp	r3, #1
	bne.n	.L_081895c0
	ldr	r0, [sp, #44]
	cmp	r0, #0
	bne.n	.L_08189532
	ldr	r7, [pc, #568]
	movs	r1, #0
	mov	r8, r1
.L_081894d6:
	movs	r3, #240
	lsls	r3, r3, #14
	str	r3, [r7, #0]
	movs	r3, #216
	lsls	r3, r3, #15
	str	r3, [r7, #4]
	bl	sub_08014878
	movs	r6, #254
	lsls	r6, r6, #7
	adds	r6, #255
	movs	r2, #128
	lsls	r2, r2, #7
	ands	r6, r0
	adds	r6, r6, r2
	bl	sub_08014878
	movs	r5, #255
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #127
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #24
	str	r3, [r7, #24]
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r7, #28
	cmp	r4, #128
	bne.n	.L_081894d6
.L_08189532:
	ldr	r6, [pc, #472]
	movs	r5, #0
	mov	r8, r5
.L_08189538:
	ldr	r0, [r6, #24]
	cmp	r0, #0
	blt.n	.L_081895b4
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_08189546
	adds	r3, #7
.L_08189546:
	ldr	r7, [sp, #44]
	asrs	r3, r3, #3
	cmp	r7, r3
	blt.n	.L_081895b4
	asrs	r0, r0, #4
	adds	r0, #1
	ldr	r2, [pc, #444]
	lsls	r5, r0, #1
	subs	r3, r5, #2
	mov	r1, r8
	movs	r4, #1
	ands	r4, r1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #24]
	lsls	r4, r4, #2
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r6, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r5, [sp, #4]
	ldr	r0, [sp, #20]
	ldr	r4, [r4, r0]
	ldr	r0, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	movs	r1, #62
	lsls	r2, r2, #7
	adds	r0, r6, #0
	bl	sub_08138086
	movs	r3, #3
	mov	r1, r8
	ands	r3, r1
	ldr	r2, [r6, #4]
	adds	r3, #108
	lsls	r3, r3, #16
	cmp	r2, r3
	ble.n	.L_081895ae
	ldr	r3, [r6, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #16]
.L_081895ae:
	ldr	r3, [r6, #24]
	subs	r3, #1
	str	r3, [r6, #24]
.L_081895b4:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r6, #28
	cmp	r3, #64
	bne.n	.L_08189538
.L_081895c0:
	ldr	r4, [sp, #44]
	subs	r4, #18
	str	r4, [sp, #12]
	cmp	r4, #40
	bhi.n	.L_0818960e
	ldr	r5, [sp, #44]
	cmp	r5, #18
	bne.n	.L_081895f4
	ldr	r7, [sp, #36]
	movs	r3, #0
	ldrsb	r3, [r7, r3]
	ldrb	r2, [r7, #1]
	lsls	r3, r3, #8
	adds	r3, r3, r2
	str	r3, [sp, #32]
	adds	r0, r7, #0
	movs	r3, #2
	ldrsb	r3, [r7, r3]
	ldrb	r2, [r7, #3]
	lsls	r3, r3, #8
	adds	r3, r3, r2
	adds	r3, #16
	adds	r0, #4
	str	r3, [sp, #28]
	str	r0, [sp, #36]
	b.n	.L_0818960e
.L_081895f4:
	ldr	r1, [sp, #36]
	ldr	r2, [sp, #32]
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	ldr	r4, [sp, #28]
	adds	r2, r2, r3
	str	r2, [sp, #32]
	movs	r3, #1
	ldrsb	r3, [r1, r3]
	adds	r1, #2
	adds	r4, r4, r3
	str	r4, [sp, #28]
	str	r1, [sp, #36]
.L_0818960e:
	ldr	r5, [sp, #44]
	subs	r5, #78
	mov	r9, r5
	cmp	r5, #40
	bhi.n	.L_08189642
	ldr	r1, [sp, #52]
	add	r5, sp, #76
	movs	r7, #36
	ldrsh	r0, [r1, r7]
	adds	r1, r5, #0
	bl	sub_0815e21c
	ldr	r2, [sp, #44]
	cmp	r2, #78
	bne.n	.L_0818963c
	ldr	r3, [r5, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [sp, #32]
	movs	r3, #48
	str	r3, [sp, #28]
	b.n	.L_08189642
.L_0818963c:
	ldr	r4, [sp, #28]
	subs	r4, #16
	str	r4, [sp, #28]
.L_08189642:
	movs	r7, #24
	movs	r0, #39
	mov	sl, r7
	movs	r5, #19
	movs	r7, #156
	mov	r8, r0
.L_0818964e:
	ldr	r1, [sp, #44]
	adds	r3, r5, #0
	adds	r3, #18
	cmp	r1, r3
	ble.n	sub_081896b6
	adds	r3, #65
	cmp	r1, r3
	bgt.n	sub_081896b6
	lsls	r0, r5, #3
	adds	r3, r0, #0
	add	r2, sp, #88
	subs	r3, #8
	ldr	r3, [r2, r3]
	str	r3, [r2, r0]
	subs	r3, r0, #4
	ldr	r6, [r2, r3]
	str	r6, [r2, r7]
	cmp	r5, #10
	ble.n	.L_0818969a
	movs	r3, #234
	lsls	r3, r3, #2
	adds	r1, r5, #0
	muls	r1, r3
	ldr	r2, [r2, r0]
	ldr	r3, [pc, #148]
	mov	r4, sl
	mov	r0, r8
	add	r1, fp
	adds	r1, r1, r3
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, #12
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #48]
	adds	r3, r6, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe00d
.L_0818969a:
	ldr	r2, [r2, r0]
	mov	r1, sl
	str	r1, [sp, #0]
	movs	r1, #236
	mov	r3, r8
	lsls	r1, r1, #5
	str	r3, [sp, #4]
	subs	r2, #12
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #48]
	add	r1, fp
	adds	r3, r6, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x3d01
	subs	r7, #8
	cmp	r5, #0
	bne.n	.L_0818964e
	bl	sub_08014de4
	ldr	r1, [sp, #16]
	ldr	r0, [sp, #16]
	adds	r1, #12
	bl	sub_080156e8
	ldr	r4, [sp, #12]
	cmp	r4, #65
	bls.n	.L_081896d4
	b.n	.L_0818981e
.L_081896d4:
	mov	r5, r9
	cmp	r5, #40
	bhi.n	.L_081896e2
	ldr	r7, [sp, #32]
	add	r5, sp, #64
	str	r7, [r5, #0]
	b.n	.L_08189726
.L_081896e2:
	ldr	r0, [sp, #52]
	ldr	r3, [r0, #4]
	cmp	r3, #1
	bne.n	.L_08189718
	ldr	r1, [sp, #32]
	movs	r3, #64
	lsrs	r2, r1, #31
	adds	r2, r1, r2
	asrs	r2, r2, #1
	add	r5, sp, #64
	subs	r3, r3, r2
	b.n	.L_08189724
	movs	r0, r0
	.4byte 0x08197467
	.4byte 0x0819747a
	.4byte 0x08197473
	.4byte 0x0819746d
	.4byte 0x02014e00
	.4byte 0x08197410
	.2byte 0xf8f0
	.2byte 0xffff
.L_08189718:
	ldr	r2, [sp, #32]
	add	r5, sp, #64
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	adds	r3, #64
.L_08189724:
	str	r3, [r5, #0]
.L_08189726:
	ldr	r4, [sp, #28]
	movs	r3, #60
	subs	r3, r3, r4
	str	r3, [r5, #4]
	add	r4, sp, #88
	ldr	r2, [r4, #4]
	subs	r3, r3, r2
	subs	r3, #24
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r0, r3, #1
	cmp	r0, #2
	ble.n	.L_08189742
	movs	r0, #2
.L_08189742:
	movs	r7, #2
	negs	r7, r7
	cmp	r0, r7
	bge.n	.L_0818974e
	movs	r0, #2
	negs	r0, r0
.L_0818974e:
	ldr	r1, [sp, #40]
	adds	r1, r1, r0
	str	r1, [sp, #40]
	cmp	r1, #8
	ble.n	.L_0818975c
	movs	r2, #8
	str	r2, [sp, #40]
.L_0818975c:
	ldr	r3, [sp, #40]
	movs	r7, #8
	negs	r7, r7
	cmp	r3, r7
	bge.n	.L_0818976c
	movs	r0, #8
	negs	r0, r0
	str	r0, [sp, #40]
.L_0818976c:
	ldr	r3, [sp, #40]
	cmp	r3, #0
	bge.n	.L_08189774
	adds	r3, #3
.L_08189774:
	ldr	r2, [r5, #0]
	asrs	r3, r3, #2
	str	r2, [r4, #0]
	adds	r0, r3, #2
	ldr	r3, [r5, #4]
	subs	r2, #12
	adds	r1, r3, #0
	subs	r1, #20
	str	r1, [r4, #4]
	lsls	r1, r0, #3
	adds	r1, r1, r0
	lsls	r1, r1, #7
	movs	r0, #24
	movs	r4, #224
	lsls	r4, r4, #3
	str	r0, [sp, #0]
	add	r1, fp
	movs	r0, #48
	adds	r1, r1, r4
	str	r0, [sp, #4]
	subs	r3, #22
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x980b
	movs	r7, #0
	lsls	r0, r0, #2
	mov	r8, r7
	mov	sl, r0
	adds	r7, r5, #0
.L_081897b2:
	mov	r3, sl
	add	r3, r8
	movs	r2, #127
	movs	r1, #128
	lsls	r1, r1, #1
	ands	r3, r2
	adds	r3, r3, r1
	lsls	r6, r3, #3
	ldr	r2, [pc, #800]
	subs	r6, r6, r3
	ldr	r3, [r7, #0]
	lsls	r6, r6, #2
	adds	r6, r6, r2
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	ldr	r3, [r7, #4]
	movs	r5, #255
	lsls	r3, r3, #16
	str	r3, [r6, #4]
	bl	sub_08014878
	adds	r2, r0, #0
	str	r2, [sp, #8]
	bl	sub_08014878
	ldr	r2, [sp, #8]
	ands	r5, r0
	adds	r0, r2, #0
	bl	sub_08002096
	adds	r5, #127
	adds	r3, r5, #0
	muls	r3, r0
	ldr	r2, [sp, #8]
	asrs	r3, r3, #7
	str	r3, [r6, #12]
	adds	r0, r2, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r6, #16]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #24
	str	r3, [r6, #24]
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	cmp	r4, #4
	bne.n	.L_081897b2
.L_0818981e:
	ldr	r5, [sp, #44]
	cmp	r5, #82
	bgt.n	.L_08189826
	b.n	.L_0818997a
.L_08189826:
	movs	r7, #0
	mov	r8, r7
	mov	r7, fp
	adds	r7, #224
.L_0818982e:
	ldr	r3, [r7, #4]
	cmp	r3, #0
	blt.n	.L_081898fa
	add	r6, sp, #64
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl	sub_0815e1ec
	ldr	r3, [r6, #0]
	asrs	r3, r3, #1
	str	r3, [r6, #0]
	ldr	r3, [r6, #8]
	cmp	r3, #159
	bgt.n	.L_0818984e
	movs	r3, #160
	str	r3, [r6, #8]
.L_0818984e:
	movs	r2, #136
	lsls	r2, r2, #2
	adds	r2, #255
	cmp	r3, r2
	ble.n	.L_0818985c
	str	r2, [r6, #8]
	adds	r3, r2, #0
.L_0818985c:
	adds	r2, r3, #0
	subs	r2, #160
	cmp	r2, #0
	bge.n	.L_08189866
	adds	r2, #63
.L_08189866:
	ldr	r0, [sp, #52]
	movs	r3, #9
	asrs	r2, r2, #6
	subs	r5, r3, r2
	ldr	r4, [r0, #24]
	ldr	r3, [pc, #628]
	ldrb	r3, [r3, r4]
	cmp	r8, r3
	blt.n	.L_081898ca
	cmp	r4, #0
	bne.n	.L_08189886
	ldr	r3, [r7, #24]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r1, r3, #1
	b.n	.L_08189890
.L_08189886:
	ldr	r1, [r7, #24]
	cmp	r1, #0
	bge.n	.L_0818988e
	adds	r1, #3
.L_0818988e:
	asrs	r1, r1, #2
.L_08189890:
	cmp	r1, #5
	bgt.n	.L_081898fa
	ldr	r2, [pc, #596]
	movs	r0, #32
	lsls	r1, r1, #11
	ldr	r3, [r6, #4]
	adds	r1, r1, r2
	ldr	r2, [r6, #0]
	str	r0, [sp, #0]
	movs	r0, #64
	str	r0, [sp, #4]
	ldr	r5, [sp, #20]
	lsls	r4, r4, #2
	subs	r2, #16
	subs	r3, #40
	ldr	r4, [r4, r5]
	ldr	r0, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69bb
	movs	r2, #128
	adds	r3, #1
	str	r3, [r7, #24]
	adds	r0, r7, #0
	movs	r1, #62
	lsls	r2, r2, #6
	bl	sub_08138058
	b.n	.L_081898fa
.L_081898ca:
	ldr	r2, [pc, #548]
	lsls	r0, r5, #1
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #24]
	lsrs	r3, r5, #31
	adds	r1, r2, r1
	ldr	r2, [r6, #0]
	adds	r3, r5, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r6, #4]
	ldr	r4, [sp, #56]
	str	r0, [sp, #4]
	subs	r3, r3, r5
	str	r5, [sp, #0]
	ldr	r0, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c38
	movs	r1, #64
	ldr	r2, [pc, #508]
	bl	sub_08138058
.L_081898fa:
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r7, #28
	cmp	r4, #56
	bne.n	.L_0818982e
	ldr	r5, [sp, #52]
	ldr	r3, [r5, #24]
	cmp	r3, #1
	bne.n	.L_081899e6
	ldr	r6, [pc, #468]
	movs	r7, #0
	mov	r8, r7
	add	r7, sp, #64
.L_08189916:
	ldr	r5, [r6, #24]
	cmp	r5, #0
	blt.n	.L_0818996e
	adds	r1, r7, #0
	adds	r0, r6, #0
	bl	sub_0815e1ec
	ldr	r2, [r7, #0]
	mov	r1, r8
	asrs	r5, r5, #3
	movs	r0, #1
	adds	r5, #1
	ands	r0, r1
	ldr	r1, [pc, #444]
	lsls	r4, r5, #1
	asrs	r2, r2, #1
	str	r2, [r7, #0]
	subs	r3, r4, #2
	ldrh	r1, [r1, r3]
	ldr	r3, [sp, #24]
	lsls	r0, r0, #2
	adds	r1, r3, r1
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	str	r5, [sp, #0]
	subs	r3, r3, r5
	str	r4, [sp, #4]
	ldr	r5, [sp, #20]
	ldr	r4, [r0, r5]
	ldr	r0, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r6, #0
	movs	r1, #62
	lsls	r2, r2, #6
	bl	sub_08138058
	ldr	r3, [r6, #24]
	subs	r3, #1
	str	r3, [r6, #24]
.L_0818996e:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r6, #28
	cmp	r1, #128
	bne.n	.L_08189916
.L_0818997a:
	ldr	r2, [sp, #52]
	ldr	r3, [r2, #24]
	cmp	r3, #1
	bne.n	.L_081899e6
	ldr	r4, [pc, #364]
	ldr	r6, [pc, #368]
	movs	r3, #0
	mov	r8, r3
	mov	sl, r4
.L_0818998c:
	ldr	r0, [r6, #24]
	cmp	r0, #0
	blt.n	.L_081899da
	asrs	r0, r0, #4
	adds	r0, #1
	mov	r5, r8
	movs	r4, #1
	ands	r4, r5
	lsls	r5, r0, #1
	subs	r3, r5, #2
	mov	r7, sl
	ldrh	r1, [r7, r3]
	ldr	r2, [sp, #24]
	lsls	r4, r4, #2
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r6, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r5, [sp, #4]
	ldr	r0, [sp, #20]
	ldr	r4, [r4, r0]
	ldr	r0, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c30
	movs	r1, #60
	ldr	r2, [pc, #300]
	bl	sub_08138086
	ldr	r3, [r6, #24]
	subs	r3, #1
	str	r3, [r6, #24]
.L_081899da:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r6, #28
	cmp	r2, #128
	bne.n	.L_0818998c
.L_081899e6:
	ldr	r3, [sp, #44]
	cmp	r3, #83
	bne.n	.L_08189a14
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	movs	r2, #8
	add	r3, fp
	str	r2, [r3, #0]
	ldr	r5, [sp, #52]
	movs	r1, #7
	movs	r4, #36
	ldrsh	r0, [r5, r4]
	movs	r3, #0
	str	r2, [sp, #0]
	movs	r2, #5
	bl	sub_0814cd48
	movs	r7, #36
	ldrsh	r0, [r5, r7]
	movs	r1, #1
	bl	sub_08118088
.L_08189a14:
	ldr	r0, [sp, #44]
	cmp	r0, #50
	bne.n	.L_08189a3a
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, fp
	movs	r3, #12
	str	r3, [r2, #0]
	ldr	r2, [sp, #52]
	movs	r3, #8
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
.L_08189a3a:
	ldr	r3, [sp, #44]
	cmp	r3, #49
	ble.n	.L_08189a96
	movs	r4, #0
	mov	r8, r4
	mov	r6, fp
.L_08189a46:
	ldr	r3, [r6, #24]
	cmp	r3, #11
	bhi.n	.L_08189a86
	lsrs	r4, r3, #31
	ldr	r5, [pc, #176]
	ldr	r0, [pc, #176]
	adds	r4, r3, r4
	asrs	r4, r4, #1
	lsls	r3, r4, #1
	ldrh	r1, [r5, r3]
	ldrb	r5, [r0, r4]
	ldr	r2, [r6, #0]
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #164]
	movs	r7, #134
	ldrb	r0, [r3, r4]
	ldr	r3, [r6, #4]
	str	r5, [sp, #0]
	adds	r3, r3, r0
	ldr	r0, [pc, #156]
	ldr	r5, [sp, #20]
	ldrb	r0, [r0, r4]
	add	r1, fp
	str	r0, [sp, #4]
	lsls	r7, r7, #7
	adds	r1, r1, r7
	ldr	r4, [r5, #4]
	ldr	r0, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69b3
.L_08189a86:
	movs	r7, #1
	add	r8, r7
	adds	r3, #1
	mov	r0, r8
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r0, #8
	bne.n	.L_08189a46
.L_08189a96:
	movs	r1, #8
	movs	r0, #8
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
	ldr	r1, [sp, #44]
	adds	r1, #1
	str	r1, [sp, #44]
	cmp	r1, #150
	beq.n	.L_08189ac0
	b.n	.L_081893aa
.L_08189ac0:
	ldr	r0, [pc, #76]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #248
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02014000
	.4byte 0x08199d5c
	.4byte 0x02010000
	.4byte 0x08197410
	.4byte 0xffffe000
	.4byte 0x02015c00
	.4byte 0xffffc000
	.4byte 0x0819747a
	.4byte 0x08197467
	.4byte 0x08197473
	.4byte 0x0819746d
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #88
	str	r0, [sp, #52]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	str	r0, [sp, #48]
	movs	r0, #1
	ldr	r1, [r3, #96]
	str	r1, [sp, #44]
	ldr	r3, [r3, #100]
	str	r3, [sp, #28]
	bl	sub_081435e0
	ldr	r3, [pc, #24]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r2, [sp, #52]
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_08189b5c
	movs	r0, #104
	movs	r1, #3
	bl	sub_081963ec
	b.n	.L_08189b64
	.2byte 0x1010
	.2byte 0x0000
.L_08189b5c:
	movs	r0, #104
	movs	r1, #7
	bl	sub_081963ec
.L_08189b64:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	ldr	r0, [pc, #360]
	str	r3, [sp, #32]
	ldr	r1, [pc, #360]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r3, [sp, #48]
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r1, r3, r4
	ldr	r0, [pc, #340]
	movs	r2, #64
	movs	r3, #64
	bl	sub_0816ae40
	ldr	r0, [pc, #336]
	ldr	r1, [sp, #28]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r5, [sp, #48]
	movs	r6, #142
	lsls	r6, r6, #7
	adds	r1, r5, r6
	ldr	r0, [pc, #320]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r2, #174
	lsls	r2, r2, #7
	adds	r1, r5, r2
	ldr	r0, [pc, #308]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #300]
	ldr	r1, [pc, #284]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r3, #239
	movs	r4, #238
	lsls	r3, r3, #7
	lsls	r4, r4, #7
	adds	r2, r5, r3
	adds	r4, #132
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r1, #200
	adds	r2, r5, r4
	movs	r3, #50
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #268]
	bl	sub_080145a8
	movs	r5, #0
	str	r5, [sp, #40]
	ldr	r6, [sp, #52]
	movs	r1, #86
	ldr	r0, [r6, #24]
	negs	r1, r1
	lsls	r3, r0, #4
	cmp	r3, r1
	bne.n	.L_08189bf8
	b.n	.L_0818a1b6
.L_08189bf8:
	mov	r2, sp
	mov	r3, sp
	adds	r2, #56
	adds	r3, #76
	str	r2, [sp, #16]
	str	r3, [sp, #24]
.L_08189c04:
	ldr	r4, [sp, #40]
	cmp	r4, #0
	bne.n	.L_08189c94
	ldr	r6, [sp, #52]
	movs	r1, #64
	movs	r5, #36
	ldrsh	r0, [r6, r5]
	add	r1, sp
	mov	sl, r1
	bl	sub_0815e21c
	ldr	r3, [r6, #4]
	mov	r5, sl
	cmp	r3, #0
	bne.n	.L_08189c2c
	mov	r2, sl
	ldr	r3, [r2, #0]
	ldr	r6, [sp, #24]
	adds	r3, #80
	b.n	.L_08189c34
.L_08189c2c:
	mov	r4, sl
	ldr	r3, [r4, #0]
	ldr	r6, [sp, #24]
	subs	r3, #80
.L_08189c34:
	str	r3, [r6, #0]
	ldr	r3, [r5, #4]
	movs	r0, #0
	subs	r3, #80
	str	r3, [r6, #4]
	ldr	r1, [sp, #48]
	mov	r9, r0
	adds	r4, r5, #0
	adds	r0, r6, #0
	movs	r5, #0
.L_08189c48:
	ldr	r3, [r0, #0]
	lsls	r3, r3, #15
	str	r3, [r1, #0]
	ldr	r3, [r0, #4]
	lsls	r3, r3, #16
	str	r3, [r1, #4]
	ldr	r2, [r0, #0]
	ldr	r3, [r4, #0]
	subs	r3, r3, r2
	lsls	r3, r3, #11
	str	r3, [r1, #12]
	ldr	r2, [r0, #4]
	ldr	r3, [r4, #4]
	str	r5, [r1, #24]
	subs	r3, r3, r2
	movs	r2, #1
	lsls	r3, r3, #12
	add	r9, r2
	str	r3, [r1, #16]
	mov	r3, r9
	adds	r1, #28
	cmp	r3, #3
	bne.n	.L_08189c48
	ldr	r3, [pc, #120]
	movs	r4, #0
	movs	r1, #1
	movs	r2, #128
	mov	r9, r4
	negs	r1, r1
	lsls	r2, r2, #2
.L_08189c84:
	movs	r5, #1
	add	r9, r5
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r9, r2
	bne.n	.L_08189c84
	ldr	r6, [sp, #52]
	ldr	r0, [r6, #24]
.L_08189c94:
	movs	r2, #1
	movs	r1, #0
	negs	r2, r2
	mov	r9, r1
	cmp	r0, r2
	bne.n	.L_08189ca2
	b.n	.L_08189e46
.L_08189ca2:
	ldr	r3, [sp, #48]
	str	r1, [sp, #12]
	str	r1, [sp, #8]
	mov	r8, r3
.L_08189caa:
	ldr	r3, [sp, #12]
	ldr	r4, [sp, #40]
	adds	r3, #54
	cmp	r4, r3
	bne.n	.L_08189cba
	movs	r0, #212
	bl	sub_081c0010
.L_08189cba:
	ldr	r3, [sp, #12]
	ldr	r5, [sp, #40]
	adds	r3, #59
	cmp	r5, r3
	bne.n	.L_08189da4
	ldr	r6, [sp, #52]
	ldr	r3, [r6, #24]
	cmp	r9, r3
	bne.n	.L_08189cf4
	movs	r0, #144
	bl	sub_081180e8
	b.n	.L_08189cfa
	.4byte 0x000000ec
	.4byte 0x02010000
	.4byte 0x00000134
	.4byte 0x000000e7
	.4byte 0x000000e9
	.4byte 0x000000c2
	.4byte 0x08143001
	.2byte 0x4018
	.2byte 0x0201
.L_08189cf4:
	movs	r0, #144
	bl	sub_081c0010
.L_08189cfa:
	ldr	r2, [sp, #52]
	movs	r3, #0
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	str	r3, [sp, #0]
	movs	r3, #100
	str	r3, [sp, #4]
	movs	r1, #1
	movs	r3, #0
	movs	r2, #0
	bl	sub_0815f000
	ldr	r4, [sp, #52]
	movs	r1, #7
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	movs	r6, #238
	ldr	r5, [sp, #48]
	lsls	r6, r6, #7
	adds	r6, #168
	adds	r2, r5, r6
	movs	r3, #4
	str	r3, [r2, #0]
	ldr	r2, [sp, #8]
	ldr	r3, [pc, #776]
	movs	r1, #64
	movs	r0, #0
	add	r1, sp
	mov	fp, r0
	mov	sl, r1
	adds	r7, r2, r3
.L_08189d44:
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r6, #128
	lsls	r3, r3, #15
	str	r3, [r7, #0]
	movs	r3, #192
	lsls	r3, r3, #15
	str	r3, [r7, #4]
	bl	sub_08014878
	movs	r1, #192
	lsls	r1, r1, #8
	bl	sub_0800206c
	adds	r5, r0, #0
	bl	sub_08014878
	lsls	r6, r6, #6
	adds	r5, r5, r6
	movs	r3, #127
	adds	r6, r0, #0
	adds	r0, r5, #0
	ands	r6, r3
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #4
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #3
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	movs	r0, #1
	add	fp, r0
	adds	r3, #16
	mov	r1, fp
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r1, #128
	bne.n	.L_08189d44
.L_08189da4:
	ldr	r3, [sp, #12]
	ldr	r2, [sp, #40]
	adds	r3, #50
	cmp	r2, r3
	blt.n	.L_08189e22
	mov	r5, r8
	movs	r4, #2
	ldrsh	r3, [r5, r4]
	movs	r1, #80
	adds	r2, r3, #0
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	subs	r2, #20
	adds	r4, r3, #0
	subs	r4, #50
	cmp	r4, #24
	ble.n	.L_08189dca
	movs	r3, #104
	subs	r1, r3, r4
.L_08189dca:
	movs	r3, #40
	str	r3, [sp, #0]
	ldr	r3, [sp, #48]
	movs	r5, #142
	lsls	r5, r5, #7
	str	r1, [sp, #4]
	ldr	r0, [sp, #44]
	adds	r1, r3, r5
	ldr	r6, [sp, #32]
	adds	r3, r4, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4640
	ldr	r5, [r0, #24]
	cmp	r5, #12
	bgt.n	.L_08189e1c
	ldr	r3, [r0, #0]
	ldr	r0, [r0, #12]
	mov	r1, r8
	adds	r3, r3, r0
	str	r3, [r1, #0]
	ldr	r4, [r1, #16]
	ldr	r3, [r1, #4]
	adds	r3, r3, r4
	str	r3, [r1, #4]
	ldr	r6, [sp, #24]
	add	r1, sp, #64
	ldr	r2, [r6, #0]
	ldr	r3, [r1, #0]
	subs	r3, r3, r2
	lsls	r3, r3, #9
	adds	r0, r0, r3
	mov	r2, r8
	str	r0, [r2, #12]
	ldr	r3, [r1, #4]
	ldr	r2, [r6, #4]
	subs	r3, r3, r2
	lsls	r3, r3, #10
	adds	r4, r4, r3
	mov	r3, r8
	str	r4, [r3, #16]
.L_08189e1c:
	adds	r3, r5, #1
	mov	r4, r8
	str	r3, [r4, #24]
.L_08189e22:
	ldr	r5, [sp, #12]
	ldr	r0, [sp, #8]
	movs	r1, #224
	lsls	r1, r1, #4
	adds	r5, #16
	adds	r0, r0, r1
	str	r5, [sp, #12]
	str	r0, [sp, #8]
	ldr	r4, [sp, #52]
	movs	r2, #1
	ldr	r3, [r4, #24]
	movs	r6, #28
	add	r9, r2
	adds	r3, #1
	add	r8, r6
	cmp	r9, r3
	beq.n	.L_08189e46
	b.n	.L_08189caa
.L_08189e46:
	movs	r5, #0
	mov	r9, r5
	ldr	r5, [pc, #500]
.L_08189e4c:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_08189eaa
	asrs	r0, r0, #2
	adds	r0, #1
	ldr	r2, [pc, #492]
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r6, [sp, #28]
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	adds	r1, r6, r1
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #44]
	ldr	r4, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x686b
	movs	r2, #208
	lsls	r2, r2, #15
	cmp	r3, r2
	ble.n	.L_08189e98
	ldr	r3, [r5, #16]
	str	r2, [r5, #4]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r5, #16]
.L_08189e98:
	movs	r2, #128
	adds	r0, r5, #0
	movs	r1, #62
	lsls	r2, r2, #7
	bl	sub_08138086
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_08189eaa:
	movs	r6, #1
	movs	r0, #128
	add	r9, r6
	lsls	r0, r0, #2
	adds	r5, #28
	cmp	r9, r0
	bne.n	.L_08189e4c
	movs	r0, #32
	bl	sub_08014dac
	str	r0, [sp, #20]
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #384]
	ldr	r3, [sp, #56]
	movs	r1, #7
	ands	r3, r2
	ldr	r2, [pc, #380]
	orrs	r3, r1
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #56]
	ldr	r3, [sp, #48]
	ldr	r4, [sp, #16]
	adds	r2, r3, r2
	ldr	r3, [pc, #364]
	str	r2, [r4, #4]
	str	r3, [r0, #8]
	str	r1, [r0, #0]
	str	r4, [r0, #16]
	ldr	r5, [sp, #20]
	mov	sl, r0
	str	r5, [r0, #12]
	ldr	r3, [sp, #40]
	subs	r3, #16
	cmp	r3, #91
	bhi.n	.L_08189fac
	ldr	r6, [sp, #40]
	ldr	r0, [sp, #40]
	ldr	r2, [pc, #340]
	lsls	r3, r6, #1
	adds	r6, r3, #0
	lsls	r3, r0, #13
	adds	r5, r3, r2
	movs	r3, #48
	subs	r6, #96
	negs	r3, r3
	movs	r1, #0
	cmp	r6, r3
	ble.n	.L_08189f18
	movs	r6, #48
	negs	r6, r6
.L_08189f18:
	ldr	r4, [sp, #52]
	ldr	r0, [sp, #40]
	ldr	r3, [r4, #24]
	lsls	r2, r3, #4
	adds	r3, r2, #0
	adds	r3, #60
	cmp	r0, r3
	blt.n	.L_08189f30
	subs	r3, r0, r2
	lsls	r3, r3, #2
	subs	r3, #240
	negs	r1, r3
.L_08189f30:
	movs	r2, #128
	lsls	r2, r2, #10
	cmp	r5, r2
	ble.n	.L_08189f3c
	movs	r5, #128
	lsls	r5, r5, #10
.L_08189f3c:
	mov	r3, sl
	str	r1, [r3, #20]
	bl	sub_08014de4
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r1, #128
	adds	r0, r2, #0
	lsls	r1, r1, #9
	bl	sub_080151e4
	ldr	r4, [sp, #24]
	lsls	r1, r6, #16
	ldr	r0, [r4, #0]
	movs	r2, #0
	subs	r0, #128
	lsls	r0, r0, #16
	bl	sub_08015160
	movs	r0, #178
	lsls	r0, r0, #7
	bl	sub_08015024
	ldr	r6, [sp, #52]
	ldr	r3, [r6, #4]
	cmp	r3, #0
	bne.n	.L_08189f7c
	movs	r0, #200
	lsls	r0, r0, #5
	bl	sub_08015068
	b.n	.L_08189f82
.L_08189f7c:
	ldr	r0, [pc, #216]
	bl	sub_08015068
.L_08189f82:
	ldr	r1, [sp, #40]
	lsls	r0, r1, #9
	bl	sub_080150e4
	adds	r0, r5, #0
	bl	sub_0801521c
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r0, r2, #0
	adds	r1, r2, #0
	bl	sub_080151e4
	ldr	r0, [pc, #188]
	ldr	r1, [sp, #20]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_08189fac:
	ldr	r2, [pc, #176]
	ldr	r3, [sp, #16]
	movs	r6, #1
	str	r2, [r3, #4]
	ldr	r5, [sp, #52]
	movs	r4, #0
	ldr	r0, [r5, #24]
	negs	r6, r6
	mov	r9, r4
	cmp	r0, r6
	beq.n	.L_0818a0a0
	ldr	r1, [sp, #40]
	ldr	r2, [pc, #156]
	movs	r7, #128
	lsls	r3, r1, #3
	lsls	r7, r7, #8
	adds	r6, r3, r2
.L_08189fce:
	mov	r4, r9
	lsls	r3, r4, #4
	ldr	r5, [sp, #40]
	adds	r2, r3, #0
	adds	r2, #50
	cmp	r5, r2
	blt.n	.L_0818a094
	subs	r3, r5, r3
	adds	r2, r3, #0
	movs	r3, #64
	subs	r1, r3, r6
	subs	r2, #50
	cmp	r1, #0
	ble.n	.L_08189fec
	movs	r1, #0
.L_08189fec:
	movs	r3, #64
	negs	r3, r3
	cmp	r1, r3
	ble.n	.L_0818a094
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r2, r3, #7
	mov	r4, sl
	adds	r3, r3, r2
	lsls	r3, r3, #5
	str	r1, [r4, #20]
	adds	r5, r3, r7
	bl	sub_08014de4
	movs	r1, #128
	adds	r0, r7, #0
	lsls	r1, r1, #9
	adds	r2, r7, #0
	bl	sub_080151e4
	ldr	r1, [sp, #24]
	movs	r2, #0
	ldr	r0, [r1, #0]
	ldr	r1, [pc, #76]
	subs	r0, #128
	lsls	r0, r0, #16
	bl	sub_08015160
	movs	r0, #178
	lsls	r0, r0, #7
	bl	sub_08015024
	ldr	r2, [sp, #52]
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_0818a06c
	movs	r0, #200
	lsls	r0, r0, #5
	bl	sub_08015068
	b.n	.L_0818a072
	movs	r0, r0
	.4byte 0x02014000
	.4byte 0x08197410
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199364
	.4byte 0xfffe0000
	.4byte 0xffffe700
	.4byte 0x081991e0
	.4byte 0x02010000
	.4byte 0xfffffe70
	.2byte 0x0000
	.2byte 0xffd0
.L_0818a06c:
	.2byte 0x4859
	bl	sub_08015068
.L_0818a072:
	ldr	r3, [sp, #40]
	lsls	r0, r3, #9
	bl	sub_080150e4
	adds	r0, r5, #0
	bl	sub_0801521c
	ldr	r0, [pc, #340]
	ldr	r1, [sp, #20]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
	ldr	r4, [sp, #52]
	ldr	r0, [r4, #24]
.L_0818a094:
	movs	r5, #1
	add	r9, r5
	adds	r3, r0, #1
	subs	r6, #128
	cmp	r9, r3
	bne.n	.L_08189fce
.L_0818a0a0:
	ldr	r6, [sp, #40]
	cmp	r6, #49
	bgt.n	.L_0818a172
	ldr	r0, [sp, #16]
	movs	r3, #5
	strb	r3, [r0, #0]
	add	r3, sp, #56
	str	r3, [sp, #16]
	ldr	r1, [sp, #16]
	movs	r3, #6
	strb	r3, [r1, #1]
	ldr	r2, [sp, #48]
	movs	r4, #174
	lsls	r4, r4, #7
	adds	r3, r2, r4
	str	r3, [r1, #4]
	ldr	r3, [pc, #280]
	mov	r5, sl
	str	r3, [r5, #8]
	ldr	r0, [sp, #40]
	ldr	r1, [sp, #40]
	movs	r6, #0
	lsls	r0, r0, #12
	lsls	r7, r1, #9
	mov	r9, r6
	mov	fp, r0
	add	r6, sp, #76
	mov	r8, r7
.L_0818a0d8:
	movs	r3, #0
	mov	r2, sl
	str	r3, [r2, #20]
	bl	sub_08014de4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #8
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl	sub_080151e4
	ldr	r0, [r6, #0]
	ldr	r1, [pc, #232]
	subs	r0, #128
	movs	r2, #0
	lsls	r0, r0, #16
	bl	sub_08015160
	movs	r0, #142
	lsls	r0, r0, #7
	adds	r0, #208
	ldr	r5, [pc, #220]
	bl	sub_08015024
	adds	r0, r7, #0
	bl	sub_080150e4
	movs	r3, #128
	add	r5, fp
	lsls	r3, r3, #9
	cmp	r5, r3
	ble.n	.L_0818a120
	movs	r5, #128
	lsls	r5, r5, #9
.L_0818a120:
	ldr	r4, [sp, #40]
	cmp	r4, #31
	ble.n	.L_0818a12e
	movs	r3, #192
	lsls	r3, r3, #10
	mov	r0, fp
	subs	r5, r3, r0
.L_0818a12e:
	mov	r0, r8
	bl	sub_08002096
	cmp	r0, #0
	bge.n	.L_0818a13a
	adds	r0, #7
.L_0818a13a:
	asrs	r2, r0, #3
	adds	r2, r2, r5
	movs	r0, #128
	movs	r1, #128
	lsls	r2, r2, #1
	lsls	r0, r0, #8
	lsls	r1, r1, #9
	bl	sub_080151e4
	movs	r2, #4
	ldr	r1, [sp, #20]
	ldr	r0, [pc, #148]
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
	movs	r3, #1
	movs	r1, #128
	movs	r2, #128
	add	r9, r3
	lsls	r1, r1, #5
	lsls	r2, r2, #6
	mov	r4, r9
	add	r8, r1
	adds	r7, r7, r2
	cmp	r4, #8
	bne.n	.L_0818a0d8
.L_0818a172:
	mov	r0, sl
	bl	sub_08013164
	ldr	r0, [sp, #20]
	bl	sub_08013164
	movs	r1, #4
	movs	r0, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r6, #240
	ldr	r5, [sp, #48]
	lsls	r6, r6, #7
	adds	r6, #232
	adds	r2, r5, r6
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #40]
	ldr	r1, [sp, #52]
	adds	r0, #1
	str	r0, [sp, #40]
	ldr	r2, [sp, #40]
	ldr	r3, [r1, #24]
	adds	r0, r3, #0
	lsls	r3, r0, #4
	adds	r3, #86
	cmp	r2, r3
	beq.n	.L_0818a1b6
	b.n	.L_08189c04
.L_0818a1b6:
	ldr	r0, [pc, #52]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #88
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffffe700
	.4byte 0x081991e0
	.4byte 0x081992b0
	.4byte 0xffb00000
	.4byte 0xffffc000
	.4byte 0x08199200
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #0
	bl	sub_0818a220
	pop	{pc}
