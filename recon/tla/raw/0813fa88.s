.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
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
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014dac, 0x08014dac
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_0801521c, 0x0801521c
	.set sub_0801587c, 0x0801587c
	.set sub_08020010, 0x08020010
	.set sub_08020048, 0x08020048
	.set sub_08020380, 0x08020380
	.set sub_08020388, 0x08020388
	.set sub_08118010, 0x08118010
	.set sub_08118028, 0x08118028
	.set sub_08118038, 0x08118038
	.set sub_08118040, 0x08118040
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_081180f0, 0x081180f0
	.set sub_08138086, 0x08138086
	.set sub_0813ba50, 0x0813ba50
	.set sub_0814028e, 0x0814028e
	.set sub_08140c18, 0x08140c18
	.set sub_0814129e, 0x0814129e
	.set sub_08141380, 0x08141380
	.set sub_081416ee, 0x081416ee
	.set sub_081418ea, 0x081418ea
	.set sub_081434f8, 0x081434f8
	.set sub_0814355c, 0x0814355c
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cc4c, 0x0814cc4c
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815b290, 0x0815b290
	.set sub_0815b3b0, 0x0815b3b0
	.set sub_0815b410, 0x0815b410
	.set sub_0815b434, 0x0815b434
	.set sub_0815b510, 0x0815b510
	.set sub_0815e21c, 0x0815e21c
	.set sub_08164a4c, 0x08164a4c
	.set sub_08164abc, 0x08164abc
	.set sub_08164b2c, 0x08164b2c
	.set sub_0816ae40, 0x0816ae40
	.set sub_08178680, 0x08178680
	.set sub_08179e6c, 0x08179e6c
	.set sub_081885f0, 0x081885f0
	.set sub_0818caa8, 0x0818caa8
	.set sub_0818de3c, 0x0818de3c
	.set sub_081963ec, 0x081963ec
	.set sub_08196404, 0x08196404
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Func_0813fa88
	.thumb_func
Func_0813fa88:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r0
	movs	r1, #16
	movs	r0, #92
	sub	sp, #8
	bl	sub_08014cc0
	movs	r1, #128
	lsls	r1, r1, #7
	mov	r9, r0
	movs	r0, #96
	bl	sub_08014cc0
	str	r0, [sp, #4]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #36]
	adds	r3, #176
	ldr	r3, [r3, #0]
	mov	r8, r3
	bl	sub_0813ba50
	mov	r2, r8
	movs	r3, #1
	str	r3, [r2, #12]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [pc, #24]
	movs	r2, #128
	ldr	r6, [pc, #24]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r5, [pc, #20]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_0813fb16
	b.n	.L_0813faf4
	.4byte 0x00000000
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
.L_0813faf4:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r6, #0]
	movs	r2, #184
	adds	r3, r3, r6
	lsls	r2, r2, #5
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0813fb16:
	strh	r1, [r5, #0]
	ldr	r2, [pc, #108]
	movs	r3, #32
	strh	r3, [r2, #6]
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #206
	lsls	r2, r2, #3
	adds	r3, r7, r2
	ldrh	r1, [r3, #0]
	movs	r0, #1
	movs	r2, #0
	bl	sub_08118038
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_0813fb62
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r6, #0]
	movs	r2, #230
	adds	r3, r3, r6
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
.L_0813fb62:
	strh	r1, [r5, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [pc, #24]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #12
	strh	r2, [r3, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_0813fbae
	b.n	.L_0813fb8c
	movs	r0, r0
	.4byte 0x00000784
	.2byte 0x1120
	.2byte 0x0300
.L_0813fb8c:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r6, #0]
	movs	r2, #230
	adds	r3, r3, r6
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
.L_0813fbae:
	strh	r1, [r5, #0]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #100
	bl	sub_08118028
	mov	r2, r8
	movs	r3, #0
	str	r3, [r2, #12]
	movs	r0, #1
	bl	sub_08013560
	bl	sub_0814355c
	ldr	r3, [pc, #28]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #24]
	adds	r2, #40
	strh	r3, [r2, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_0813fc16
	b.n	.L_0813fbf4
	movs	r0, r0
	.4byte 0x00000100
	.2byte 0x3537
	.2byte 0x0000
.L_0813fbf4:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r6, #0]
	movs	r2, #238
	adds	r3, r3, r6
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
.L_0813fc16:
	strh	r1, [r5, #0]
	movs	r5, #0
	ldr	r3, [pc, #240]
	movs	r2, #128
	mov	r8, r3
	movs	r3, #128
	lsls	r2, r2, #1
	lsls	r3, r3, #2
	mov	ip, r5
	mov	sl, r2
	mov	lr, r3
	movs	r7, #0
	movs	r6, #0
.L_0813fc30:
	mov	r2, sl
	movs	r4, #0
	adds	r0, r7, r2
	lsls	r1, r6, #1
.L_0813fc38:
	adds	r3, r0, #0
	orrs	r3, r1
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	str	r3, [sp, #0]
	mov	r3, r8
	adds	r2, r5, r3
	mov	r3, sp
	ldrh	r3, [r3, #0]
	adds	r4, #1
	strh	r3, [r2, #0]
	add	r0, lr
	adds	r1, #2
	adds	r5, #2
	cmp	r4, #8
	bne.n	.L_0813fc38
	movs	r2, #128
	movs	r3, #1
	lsls	r2, r2, #5
	add	ip, r3
	adds	r7, r7, r2
	mov	r2, ip
	adds	r6, #8
	cmp	r2, #16
	bne.n	.L_0813fc30
	movs	r1, #128
	ldr	r0, [sp, #4]
	ldr	r5, [pc, #160]
	lsls	r1, r1, #7
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2180
	lsls	r1, r1, #7
	ldr	r0, [pc, #152]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2001
	bl	sub_08013560
	mov	r3, fp
	cmp	r3, #4
	bhi.n	.L_0813fcb8
	ldr	r2, [pc, #136]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0813fca8
	.4byte 0x0813fcac
	.4byte 0x0813fcb0
	.4byte 0x0813fcb4
	.2byte 0xfcb8
	.2byte 0x0813
	ldr	r0, [pc, #112]
	b.n	.L_0813fcba
	ldr	r0, [pc, #112]
	b.n	.L_0813fcba
	ldr	r0, [pc, #112]
	b.n	.L_0813fcba
	ldr	r0, [pc, #112]
	b.n	.L_0813fcba
.L_0813fcb8:
	ldr	r0, [pc, #112]
.L_0813fcba:
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #108]
	movs	r2, #128
	lsls	r0, r0, #19
	movs	r6, #144
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2500
	mov	r2, r9
	movs	r3, #24
	lsls	r6, r6, #3
	str	r3, [r2, #8]
	str	r5, [r2, #12]
	adds	r1, r6, #0
	ldr	r0, [pc, #84]
	bl	sub_080145a8
	mov	r3, r9
	movs	r1, #200
	str	r5, [r3, #4]
	lsls	r1, r1, #4
	ldr	r0, [pc, #76]
	bl	sub_080145a8
	mov	r2, r9
	str	r5, [r2, #0]
	adds	r1, r6, #0
	ldr	r0, [pc, #68]
	bl	sub_080145a8
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x06003800
	.4byte 0x03000258
	.4byte 0x06004000
	.4byte 0x0813fc94
	.4byte 0x0000018c
	.4byte 0x00000193
	.4byte 0x00000178
	.4byte 0x0000018f
	.4byte 0x00000182
	.4byte 0x03000730
	.4byte 0x0813fa45
	.4byte 0x0813f89d
	.2byte 0xf8d5
	.2byte 0x0813
	push	{lr}
	ldr	r0, [pc, #44]
	bl	sub_08014644
	ldr	r0, [pc, #40]
	bl	sub_08014644
	ldr	r0, [pc, #40]
	bl	sub_08014644
	movs	r1, #128
	ldr	r3, [pc, #36]
	lsls	r1, r1, #7
	ldr	r0, [pc, #36]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2060
	bl	sub_0801314c
	movs	r0, #92
	bl	sub_0801314c
	pop	{pc}
	movs	r0, r0
	.4byte 0x0813f8d5
	.4byte 0x0813f89d
	.4byte 0x0813fa45
	.4byte 0x03000258
	.2byte 0x4000
	.2byte 0x0600
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #196
	str	r0, [sp, #132]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r1, [r5, #96]
	ldr	r0, [r5, #92]
	str	r1, [sp, #128]
	adds	r3, r5, #0
	ldr	r2, [r5, #100]
	adds	r3, #176
	str	r2, [sp, #112]
	mov	fp, r0
	ldr	r3, [r3, #0]
	str	r3, [sp, #108]
	bl	sub_0813ba50
	movs	r0, #0
	bl	sub_081435e0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	ldr	r5, [r5, #104]
	adds	r2, #132
	add	r2, fp
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #16]
	str	r5, [sp, #120]
	bl	sub_080145a8
	ldr	r3, [pc, #12]
	ldr	r4, [sp, #132]
	b.n	.L_0813fe00
	.4byte 0x00001010
	.4byte 0x08143001
	.2byte 0x1120
	.2byte 0x0300
.L_0813fe00:
	ldrh	r3, [r3, #4]
	str	r3, [sp, #104]
	movs	r3, #0
	str	r3, [sp, #100]
	str	r3, [sp, #96]
	ldr	r0, [r4, #8]
	bl	sub_08118098
	ldr	r6, [r0, #0]
	ldr	r5, [sp, #96]
	str	r5, [r6, #72]
	bl	sub_0815b410
	movs	r0, #140
	bl	sub_081c0010
	movs	r0, #0
	movs	r1, #224
	lsls	r1, r1, #3
	str	r0, [sp, #116]
	add	r1, fp
	add	r7, sp, #184
	mov	r8, r1
	mov	r9, r0
.L_0813fe30:
	ldr	r2, [sp, #116]
	cmp	r2, #24
	bne.n	.L_0813fe4c
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #4
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	ldr	r3, [pc, #236]
	adds	r2, #132
	add	r2, fp
	str	r3, [r2, #0]
.L_0813fe4c:
	ldr	r3, [sp, #132]
	mov	r5, r9
	ldr	r0, [r3, #8]
	adds	r1, r7, #0
	adds	r5, #2
	bl	sub_0815e21c
	cmp	r5, #96
	ble.n	.L_0813fe60
	movs	r5, #96
.L_0813fe60:
	movs	r2, #128
	mov	r0, r8
	adds	r1, r5, #0
	lsls	r2, r2, #9
	bl	sub_0815b434
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	lsrs	r3, r1, #31
	adds	r1, r1, r3
	asrs	r1, r1, #1
	subs	r2, #16
	mov	r0, r8
	adds	r3, r5, #0
	bl	sub_0818caa8
	ldr	r4, [sp, #116]
	cmp	r4, #15
	ble.n	.L_0813fe90
	ldr	r3, [r6, #12]
	movs	r5, #128
	lsls	r5, r5, #13
	adds	r3, r3, r5
	str	r3, [r6, #12]
.L_0813fe90:
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #232
	movs	r0, #1
	add	r3, fp
	str	r0, [r3, #0]
	mov	sl, r0
	bl	sub_08013560
	ldr	r2, [sp, #116]
	movs	r1, #6
	adds	r2, #1
	add	r9, r1
	str	r2, [sp, #116]
	cmp	r2, #36
	bne.n	.L_0813fe30
	movs	r3, #171
	lsls	r3, r3, #8
	movs	r5, #0
	adds	r3, #133
	movs	r1, #128
	str	r3, [r6, #72]
	str	r5, [r6, #12]
	lsls	r1, r1, #7
	ldr	r3, [pc, #116]
	ldr	r2, [pc, #120]
	ldr	r0, [pc, #120]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4a1e
	movs	r3, #240
	str	r3, [r2, #16]
	lsls	r3, r3, #7
	adds	r3, #240
	add	r3, fp
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	ldr	r1, [pc, #104]
	movs	r0, #1
	movs	r2, #0
	bl	sub_08118040
	ldr	r4, [sp, #108]
	mov	r3, sl
	str	r3, [r4, #16]
	ldr	r3, [pc, #92]
	movs	r2, #128
	strh	r5, [r3, #4]
	ldr	r3, [pc, #60]
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #40
	str	r5, [r3, #0]
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r5, #224
	str	r3, [sp, #120]
	ldr	r1, [sp, #112]
	ldr	r0, [pc, #56]
	movs	r2, #0
	movs	r3, #0
	lsls	r5, r5, #3
	bl	sub_08157cf4
	add	r5, fp
	ldr	r0, [pc, #44]
	ldr	r1, [pc, #48]
	movs	r2, #0
	movs	r3, #0
	b.n	.L_0813ff5c
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x06060606
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x06004000
	.4byte 0x030011e0
	.4byte 0x00000071
	.4byte 0x03001120
	.4byte 0x00000134
	.4byte 0x000000c2
	.2byte 0x0000
	.2byte 0x0201
.L_0813ff5c:
	bl	sub_08157cf4
	adds	r1, r5, #0
	ldr	r0, [pc, #180]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	adds	r0, r5, #0
	ldr	r1, [pc, #172]
	movs	r2, #64
	movs	r3, #64
	bl	sub_0816ae40
	movs	r1, #180
	lsls	r1, r1, #6
	adds	r1, #72
	ldr	r0, [pc, #160]
	add	r1, fp
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #244
	lsls	r1, r1, #6
	adds	r1, #72
	ldr	r0, [pc, #144]
	add	r1, fp
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #244
	lsls	r1, r1, #6
	adds	r1, #136
	ldr	r0, [pc, #132]
	add	r1, fp
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r7, [pc, #124]
	movs	r5, #0
	mov	sl, r5
	movs	r6, #2
.L_0813ffb6:
	ldrh	r0, [r5, r7]
	movs	r1, #130
	lsls	r1, r1, #7
	adds	r1, #72
	add	r0, fp
	movs	r2, #128
	adds	r0, r0, r1
	lsls	r2, r2, #9
	adds	r1, r6, #0
	bl	sub_0815b510
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	adds	r6, #2
	adds	r5, #2
	cmp	r3, #10
	bne.n	.L_0813ffb6
	bl	sub_0815b410
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
	movs	r2, #128
	ldr	r3, [pc, #28]
	lsls	r2, r2, #19
	movs	r4, #0
	adds	r2, #82
	strh	r3, [r2, #0]
	str	r4, [sp, #92]
	str	r4, [sp, #88]
	str	r4, [sp, #84]
	str	r4, [sp, #80]
	str	r4, [sp, #76]
	str	r4, [sp, #72]
	str	r4, [sp, #68]
	mov	sl, r4
	mov	r5, fp
	b.n	.L_08140030
	.4byte 0x00001010
	.4byte 0x000000ec
	.4byte 0x02014000
	.4byte 0x000000b4
	.4byte 0x000000c9
	.4byte 0x000000ca
	.2byte 0x7438
	.2byte 0x0819
.L_08140030:
	bl	sub_08014878
	movs	r3, #160
	lsls	r3, r3, #4
	adds	r3, #170
	mov	r2, sl
	muls	r2, r3
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #255
	ands	r3, r0
	adds	r2, r2, r3
	str	r2, [r5, #8]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	movs	r0, #1
	add	sl, r0
	negs	r3, r3
	mov	r1, sl
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r1, #24
	bne.n	.L_08140030
	movs	r5, #224
	movs	r2, #0
	lsls	r5, r5, #2
	mov	sl, r2
	movs	r6, #0
	add	r5, fp
.L_0814006e:
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r3, #88
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #3
	ands	r3, r0
	adds	r3, #5
	str	r3, [r5, #8]
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	str	r6, [r5, #24]
	subs	r6, #2
	adds	r5, #28
	cmp	r4, #31
	bne.n	.L_0814006e
	mov	r0, sp
	movs	r5, #0
	adds	r0, #160
	str	r5, [sp, #116]
	str	r0, [sp, #44]
.L_081400aa:
	ldr	r3, [pc, #560]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081400c0
	ldr	r1, [sp, #116]
	cmp	r1, #168
	bgt.n	.L_081400c0
	movs	r2, #169
	str	r2, [sp, #116]
.L_081400c0:
	ldr	r3, [sp, #116]
	cmp	r3, #4
	bne.n	.L_081400cc
	movs	r0, #220
	bl	sub_081c0010
.L_081400cc:
	ldr	r4, [sp, #116]
	cmp	r4, #40
	bne.n	.L_081400d8
	movs	r0, #154
	bl	sub_081c0010
.L_081400d8:
	ldr	r5, [sp, #116]
	cmp	r5, #64
	bne.n	.L_081400e4
	movs	r0, #139
	bl	sub_081c0010
.L_081400e4:
	ldr	r0, [sp, #116]
	cmp	r0, #74
	bne.n	.L_081400f0
	movs	r0, #139
	bl	sub_081c0010
.L_081400f0:
	ldr	r1, [sp, #116]
	cmp	r1, #84
	bne.n	.L_081400fc
	movs	r0, #139
	bl	sub_081c0010
.L_081400fc:
	ldr	r2, [sp, #116]
	cmp	r2, #94
	bne.n	.L_08140108
	movs	r0, #139
	bl	sub_081c0010
.L_08140108:
	ldr	r3, [sp, #116]
	cmp	r3, #124
	bne.n	.L_08140114
	movs	r0, #144
	bl	sub_081c0010
.L_08140114:
	movs	r4, #0
	mov	sl, r4
	movs	r5, #140
.L_0814011a:
	ldr	r0, [sp, #116]
	cmp	r0, r5
	bne.n	.L_08140126
	movs	r0, #104
	bl	sub_081c0010
.L_08140126:
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	adds	r5, #4
	cmp	r2, #6
	bne.n	.L_0814011a
	ldr	r3, [sp, #116]
	cmp	r3, #169
	bne.n	.L_0814014a
	movs	r0, #145
	bl	sub_081c0010
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, fp
	movs	r3, #16
	str	r3, [r2, #0]
.L_0814014a:
	ldr	r4, [sp, #116]
	cmp	r4, #0
	bne.n	.L_0814015a
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl	sub_08164b2c
.L_0814015a:
	ldr	r5, [sp, #116]
	cmp	r5, #32
	bne.n	.L_08140176
	ldr	r1, [pc, #380]
	ldr	r2, [pc, #384]
	movs	r0, #0
	movs	r3, #28
	str	r0, [sp, #92]
	str	r0, [sp, #88]
	str	r0, [sp, #84]
	str	r1, [sp, #80]
	str	r2, [sp, #76]
	str	r1, [sp, #72]
	str	r3, [sp, #68]
.L_08140176:
	ldr	r4, [sp, #116]
	cmp	r4, #124
	bne.n	.L_08140198
	ldr	r1, [pc, #360]
	movs	r5, #248
	movs	r0, #128
	movs	r2, #0
	lsls	r5, r5, #13
	lsls	r0, r0, #12
	movs	r3, #1
	str	r5, [sp, #92]
	str	r0, [sp, #88]
	str	r1, [sp, #84]
	str	r2, [sp, #80]
	str	r2, [sp, #76]
	str	r2, [sp, #72]
	str	r3, [sp, #68]
.L_08140198:
	ldr	r4, [sp, #116]
	cmp	r4, #128
	bne.n	.L_081401d2
	ldr	r3, [pc, #332]
	ldr	r5, [sp, #92]
	subs	r3, r3, r5
	cmp	r3, #0
	bge.n	.L_081401aa
	adds	r3, #15
.L_081401aa:
	asrs	r3, r3, #4
	ldr	r0, [sp, #88]
	str	r3, [sp, #80]
	movs	r3, #192
	lsls	r3, r3, #11
	subs	r3, r3, r0
	cmp	r3, #0
	bge.n	.L_081401bc
	adds	r3, #15
.L_081401bc:
	ldr	r1, [sp, #84]
	asrs	r3, r3, #4
	str	r3, [sp, #76]
	negs	r3, r1
	cmp	r3, #0
	bge.n	.L_081401ca
	adds	r3, #15
.L_081401ca:
	asrs	r3, r3, #4
	movs	r2, #16
	str	r3, [sp, #72]
	str	r2, [sp, #68]
.L_081401d2:
	ldr	r3, [sp, #116]
	cmp	r3, #144
	bne.n	.L_0814020e
	ldr	r3, [pc, #276]
	ldr	r4, [sp, #92]
	subs	r3, r3, r4
	cmp	r3, #0
	bge.n	.L_081401e4
	adds	r3, #7
.L_081401e4:
	asrs	r3, r3, #3
	ldr	r5, [sp, #88]
	str	r3, [sp, #80]
	movs	r3, #208
	lsls	r3, r3, #12
	subs	r3, r3, r5
	cmp	r3, #0
	bge.n	.L_081401f6
	adds	r3, #7
.L_081401f6:
	asrs	r3, r3, #3
	str	r3, [sp, #76]
	ldr	r0, [sp, #84]
	ldr	r3, [pc, #244]
	subs	r3, r3, r0
	cmp	r3, #0
	bge.n	.L_08140206
	adds	r3, #7
.L_08140206:
	asrs	r3, r3, #3
	movs	r1, #8
	str	r3, [sp, #72]
	str	r1, [sp, #68]
.L_0814020e:
	ldr	r2, [sp, #116]
	cmp	r2, #156
	bne.n	.L_0814024c
	ldr	r4, [sp, #92]
	movs	r3, #248
	lsls	r3, r3, #13
	subs	r3, r3, r4
	cmp	r3, #0
	bge.n	.L_08140222
	adds	r3, #15
.L_08140222:
	asrs	r3, r3, #4
	ldr	r5, [sp, #88]
	str	r3, [sp, #80]
	movs	r3, #128
	lsls	r3, r3, #12
	subs	r3, r3, r5
	cmp	r3, #0
	bge.n	.L_08140234
	adds	r3, #15
.L_08140234:
	asrs	r3, r3, #4
	str	r3, [sp, #76]
	ldr	r0, [sp, #84]
	ldr	r3, [pc, #172]
	subs	r3, r3, r0
	cmp	r3, #0
	bge.n	.L_08140244
	adds	r3, #15
.L_08140244:
	asrs	r3, r3, #4
	movs	r1, #16
	str	r3, [sp, #72]
	str	r1, [sp, #68]
.L_0814024c:
	ldr	r2, [sp, #68]
	cmp	r2, #0
	ble.n	.L_0814027a
	subs	r2, #1
	ldr	r1, [sp, #84]
	str	r2, [sp, #68]
	ldr	r2, [sp, #72]
	ldr	r3, [sp, #92]
	ldr	r4, [sp, #80]
	ldr	r5, [sp, #88]
	ldr	r0, [sp, #76]
	adds	r1, r1, r2
	adds	r3, r3, r4
	str	r1, [sp, #84]
	adds	r5, r5, r0
	str	r3, [sp, #92]
	asrs	r0, r3, #16
	ldr	r3, [sp, #84]
	asrs	r1, r5, #16
	asrs	r2, r3, #16
	str	r5, [sp, #88]
	bl	sub_08164b2c
.L_0814027a:
	ldr	r4, [sp, #116]
	cmp	r4, #124
	bne.n	sub_0814028e
	movs	r1, #128
	ldr	r3, [pc, #116]
	ldr	r0, [sp, #128]
	lsls	r1, r1, #7
	ldr	r2, [pc, #112]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9d1d
	cmp	r5, #123
	bgt.n	.L_0814034e
	movs	r0, #58
	movs	r7, #128
	movs	r6, #140
	mov	r8, r0
	lsls	r7, r7, #10
	cmp	r5, #29
	bgt.n	.L_081402a8
	lsls	r3, r5, #2
	subs	r6, r6, r3
	b.n	.L_081402b2
.L_081402a8:
	ldr	r1, [sp, #116]
	movs	r2, #114
	lsls	r3, r1, #1
	adds	r3, r3, r1
	subs	r6, r2, r3
.L_081402b2:
	ldr	r2, [sp, #116]
	cmp	r2, #7
	ble.n	.L_081402c0
	movs	r3, #144
	lsls	r2, r2, #11
	lsls	r3, r3, #10
	subs	r7, r3, r2
.L_081402c0:
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	cmp	r7, r3
	bgt.n	.L_081402ce
	movs	r7, #128
	lsls	r7, r7, #8
.L_081402ce:
	ldr	r4, [sp, #116]
	cmp	r4, #35
	ble.n	.L_08140300
	movs	r5, #20
	mov	r8, r5
	b.n	.L_0814031a
	movs	r0, r0
	.4byte 0x03001150
	.4byte 0xffffdb6e
	.4byte 0xffff924a
	.4byte 0xfffc0000
	.4byte 0xffff0000
	.4byte 0xfff70000
	.4byte 0xfffb0000
	.4byte 0x03000260
	.2byte 0x3f3f
	.2byte 0x3f3f
.L_08140300:
	ldr	r0, [sp, #116]
	cmp	r0, #19
	ble.n	.L_0814031a
	ldr	r1, [pc, #912]
	lsls	r0, r0, #10
	adds	r0, r0, r1
	bl	sub_08002090
	movs	r3, #44
	muls	r3, r0
	asrs	r3, r3, #16
	adds	r3, #14
	mov	r8, r3
.L_0814031a:
	cmp	r6, #7
	bgt.n	.L_08140320
	movs	r6, #8
.L_08140320:
	ldr	r2, [sp, #116]
	cmp	r2, #67
	ble.n	.L_08140332
	adds	r0, r2, #0
	subs	r0, #68
	movs	r1, #6
	bl	sub_08002054
	adds	r6, r6, r0
.L_08140332:
	movs	r5, #224
	lsls	r5, r5, #3
	add	r5, fp
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	sub_0815b434
	adds	r0, r5, #0
	movs	r1, #60
	mov	r2, r8
	adds	r3, r6, #0
	bl	sub_0818caa8
.L_0814034e:
	ldr	r3, [sp, #116]
	cmp	r3, #127
	ble.n	.L_081403aa
	movs	r5, #224
	ldr	r6, [pc, #836]
	movs	r4, #0
	lsls	r5, r5, #2
	mov	sl, r4
	add	r5, fp
.L_08140360:
	ldr	r0, [r5, #24]
	cmp	r0, #15
	bhi.n	.L_0814039a
	lsls	r0, r0, #10
	bl	sub_08002096
	ldr	r3, [r5, #8]
	movs	r2, #130
	muls	r0, r3
	asrs	r0, r0, #16
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	lsls	r2, r2, #7
	adds	r2, #72
	add	r1, fp
	adds	r1, r1, r2
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	subs	r2, r2, r0
	lsls	r0, r0, #2
	subs	r3, r3, r4
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #120]
	ldr	r0, [sp, #128]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69a8
.L_0814039a:
	adds	r3, r0, #1
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r1, #31
	bne.n	.L_08140360
.L_081403aa:
	movs	r0, #32
	bl	sub_08014dac
	str	r0, [sp, #64]
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #740]
	ldr	r3, [sp, #160]
	ldr	r4, [pc, #740]
	ands	r3, r2
	movs	r2, #7
	orrs	r3, r2
	ldr	r2, [pc, #736]
	movs	r5, #0
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #160]
	movs	r3, #6
	str	r3, [r0, #0]
	ldr	r2, [sp, #44]
	ldr	r3, [pc, #720]
	str	r2, [r0, #16]
	str	r3, [r0, #8]
	ldr	r3, [sp, #64]
	str	r5, [r0, #20]
	str	r3, [r0, #12]
	str	r4, [r2, #4]
	mov	sl, r5
	ldr	r5, [sp, #116]
	mov	r8, r0
	ldr	r0, [pc, #704]
	lsls	r3, r5, #15
	adds	r6, r5, #0
	adds	r7, r3, r0
	subs	r6, #64
.L_081403f6:
	cmp	r6, #15
	bhi.n	.L_08140440
	movs	r5, #128
	lsls	r5, r5, #12
	bl	sub_08014de4
	subs	r5, r5, r7
	ldr	r0, [pc, #684]
	ldr	r1, [pc, #688]
	movs	r2, #0
	bl	sub_08015160
	lsrs	r0, r5, #31
	adds	r0, r5, r0
	adds	r2, r5, #0
	adds	r1, r5, #0
	asrs	r0, r0, #1
	bl	sub_080151e4
	ldr	r3, [pc, #668]
	mov	r1, sl
	lsls	r5, r1, #1
	ldrh	r0, [r3, r5]
	bl	sub_08015068
	ldr	r3, [pc, #660]
	ldrh	r0, [r3, r5]
	bl	sub_08015024
	ldr	r0, [pc, #656]
	ldr	r1, [sp, #64]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, r8
	bl	sub_08196a7c
.L_08140440:
	ldr	r2, [pc, #644]
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	adds	r7, r7, r2
	subs	r6, #10
	cmp	r4, #4
	bne.n	.L_081403f6
	ldr	r2, [sp, #116]
	subs	r2, #124
	cmp	r2, #15
	bhi.n	.L_08140492
	lsls	r6, r2, #15
	movs	r3, #64
	lsls	r2, r2, #3
	subs	r5, r3, r2
	cmp	r5, #0
	ble.n	.L_08140466
	movs	r5, #0
.L_08140466:
	mov	r0, r8
	str	r5, [r0, #20]
	bl	sub_08014de4
	ldr	r0, [pc, #580]
	ldr	r1, [pc, #580]
	movs	r2, #0
	bl	sub_08015160
	asrs	r0, r6, #1
	adds	r1, r6, #0
	adds	r2, r6, #0
	bl	sub_080151e4
	ldr	r0, [pc, #576]
	ldr	r1, [sp, #64]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, r8
	bl	sub_08196a7c
.L_08140492:
	ldr	r6, [sp, #116]
	subs	r6, #24
	cmp	r6, #99
	bhi.n	.L_08140574
	movs	r3, #192
	movs	r1, #255
	lsls	r2, r6, #11
	lsls	r3, r3, #10
	lsls	r1, r1, #8
	subs	r7, r3, r2
	adds	r1, #255
	cmp	r7, r1
	bgt.n	.L_081404b0
	movs	r7, #128
	lsls	r7, r7, #9
.L_081404b0:
	movs	r5, #32
	negs	r5, r5
	cmp	r6, #7
	bgt.n	.L_081404be
	lsls	r3, r6, #2
	adds	r5, r3, #0
	subs	r5, #64
.L_081404be:
	ldr	r2, [sp, #116]
	cmp	r2, #63
	ble.n	.L_081404ca
	lsls	r2, r2, #2
	movs	r3, #224
	subs	r5, r3, r2
.L_081404ca:
	mov	r3, r8
	str	r5, [r3, #20]
	ldr	r4, [sp, #44]
	ldr	r3, [pc, #504]
	lsls	r6, r6, #9
	str	r3, [r4, #4]
	bl	sub_08014de4
	movs	r2, #0
	ldr	r0, [pc, #468]
	ldr	r1, [pc, #472]
	bl	sub_08015160
	movs	r1, #3
	adds	r0, r7, #0
	bl	sub_08002054
	adds	r1, r0, #0
	asrs	r0, r7, #1
	mov	sl, r0
	mov	r2, sl
	adds	r0, r7, #0
	bl	sub_080151e4
	adds	r0, r6, #0
	bl	sub_080150e4
	movs	r1, #64
	negs	r1, r1
	cmp	r5, r1
	ble.n	.L_08140518
	ldr	r0, [pc, #440]
	ldr	r1, [sp, #64]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, r8
	bl	sub_08196a7c
.L_08140518:
	movs	r3, #180
	ldr	r2, [sp, #44]
	lsls	r3, r3, #6
	adds	r3, #72
	add	r3, fp
	str	r3, [r2, #4]
	movs	r3, #6
	strb	r3, [r2, #0]
	strb	r3, [r2, #1]
	ldr	r3, [pc, #420]
	mov	r4, r8
	str	r3, [r4, #8]
	ldr	r5, [sp, #116]
	cmp	r5, #63
	ble.n	.L_08140574
	ldr	r0, [pc, #412]
	lsls	r3, r5, #2
	adds	r5, r3, r0
	cmp	r5, #0
	ble.n	.L_08140542
	movs	r5, #0
.L_08140542:
	mov	r1, r8
	str	r5, [r1, #20]
	bl	sub_08014de4
	ldr	r0, [pc, #360]
	ldr	r1, [pc, #360]
	movs	r2, #0
	bl	sub_08015160
	mov	r1, sl
	mov	r2, sl
	adds	r0, r7, #0
	bl	sub_080151e4
	adds	r0, r6, #0
	bl	sub_080150e4
	ldr	r0, [pc, #348]
	ldr	r1, [sp, #64]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, r8
	bl	sub_08196a7c
.L_08140574:
	ldr	r2, [sp, #116]
	cmp	r2, #123
	bgt.n	.L_0814057c
	b.n	.L_08140760
.L_0814057c:
	ldr	r4, [sp, #44]
	movs	r3, #5
	add	r5, sp, #160
	strb	r3, [r4, #0]
	movs	r3, #2
	str	r5, [sp, #44]
	strb	r3, [r5, #1]
	movs	r3, #244
	lsls	r3, r3, #6
	adds	r3, #72
	add	r3, fp
	str	r3, [r5, #4]
	ldr	r3, [pc, #320]
	mov	r0, r8
	str	r3, [r0, #8]
	movs	r3, #6
	mov	r9, r5
	str	r3, [r0, #0]
	movs	r5, #16
	negs	r5, r5
	movs	r1, #0
	movs	r7, #128
	str	r5, [r0, #20]
	mov	sl, r1
	lsls	r7, r7, #8
	mov	r5, fp
.L_081405b0:
	ldr	r1, [r5, #24]
	cmp	r1, #0
	blt.n	.L_08140608
	movs	r2, #156
	lsls	r2, r2, #9
	lsls	r6, r1, #13
	adds	r2, #128
	cmp	r6, r2
	ble.n	.L_081405c8
	movs	r6, #156
	lsls	r6, r6, #9
	adds	r6, #128
.L_081405c8:
	bl	sub_08014de4
	ldr	r0, [pc, #228]
	ldr	r1, [pc, #232]
	movs	r2, #0
	bl	sub_08015160
	movs	r1, #128
	lsls	r1, r1, #9
	adds	r2, r7, #0
	adds	r0, r7, #0
	bl	sub_080151e4
	ldr	r0, [r5, #8]
	bl	sub_080150e4
	movs	r0, #236
	lsls	r0, r0, #7
	adds	r0, #176
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	sub_080151e4
	ldr	r0, [pc, #228]
	ldr	r1, [sp, #64]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, r8
	bl	sub_08196a7c
	ldr	r1, [r5, #24]
.L_08140608:
	adds	r3, r1, #1
	str	r3, [r5, #24]
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	adds	r5, #28
	cmp	r4, #24
	bne.n	.L_081405b0
	ldr	r5, [sp, #116]
	cmp	r5, #163
	ble.n	.L_0814063c
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	ldr	r3, [pc, #180]
	adds	r2, #132
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #2
	movs	r1, #2
	movs	r2, #2
	bl	sub_08164a4c
.L_0814063c:
	ldr	r0, [sp, #116]
	cmp	r0, #159
	bgt.n	.L_08140644
	b.n	.L_08140760
.L_08140644:
	ldr	r1, [pc, #156]
	movs	r6, #192
	lsls	r6, r6, #3
	lsls	r3, r0, #1
	adds	r6, #228
	adds	r7, r3, r1
	add	r6, fp
	cmp	r7, #64
	ble.n	.L_08140658
	movs	r7, #64
.L_08140658:
	ldr	r2, [sp, #116]
	cmp	r2, #160
	bne.n	.L_08140668
	movs	r3, #128
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	ldr	r3, [pc, #72]
	str	r3, [r6, #4]
.L_08140668:
	ldr	r3, [sp, #116]
	cmp	r3, #171
	bgt.n	.L_081406f8
	ldr	r3, [r6, #0]
	ldr	r4, [pc, #116]
	movs	r5, #128
	adds	r3, r3, r4
	str	r3, [r6, #0]
	ldr	r3, [r6, #4]
	lsls	r5, r5, #13
	adds	r3, r3, r5
	movs	r5, #224
	lsls	r5, r5, #3
	add	r5, fp
	movs	r2, #128
	str	r3, [r6, #4]
	adds	r0, r5, #0
	adds	r1, r7, #0
	lsls	r2, r2, #9
	bl	sub_0815b434
	movs	r0, #2
	ldrsh	r1, [r6, r0]
	b.n	.L_081406ec
	.4byte 0xffffb000
	.4byte 0x08197438
	.4byte 0xffffff00
	.4byte 0x02010000
	.4byte 0xffff00ff
	.4byte 0x08199364
	.4byte 0xffe00000
	.4byte 0xfffc0000
	.4byte 0xffd40000
	.4byte 0x081976f8
	.4byte 0x081976f0
	.4byte 0x081991e0
	.4byte 0xfffb0000
	.4byte 0x02014000
	.4byte 0x08199340
	.4byte 0xfffffec0
	.4byte 0x08199220
	.4byte 0x081991c0
	.4byte 0x01010101
	.4byte 0xfffffee0
	.2byte 0x0000
	.2byte 0xfff8
.L_081406ec:
	.2byte 0x2306
	ldrsh	r2, [r6, r3]
	adds	r0, r5, #0
	adds	r3, r7, #0
	bl	sub_0818caa8
.L_081406f8:
	mov	r4, r8
	movs	r3, #6
	str	r3, [r4, #0]
	movs	r3, #244
	lsls	r3, r3, #6
	movs	r5, #0
	adds	r3, #136
	str	r5, [r4, #20]
	add	r3, fp
	mov	r5, r9
	str	r3, [r5, #4]
	bl	sub_08014de4
	ldr	r0, [r6, #0]
	ldr	r1, [pc, #120]
	ldr	r2, [pc, #124]
	adds	r0, r0, r1
	ldr	r1, [r6, #4]
	movs	r5, #128
	adds	r1, r1, r2
	movs	r2, #0
	bl	sub_08015160
	lsls	r5, r5, #8
	movs	r1, #128
	lsls	r1, r1, #9
	adds	r2, r5, #0
	adds	r0, r5, #0
	bl	sub_080151e4
	movs	r0, #160
	lsls	r0, r0, #8
	bl	sub_080150e4
	movs	r3, #144
	lsls	r3, r3, #6
	adds	r3, #184
	movs	r1, #128
	adds	r0, r7, #0
	muls	r0, r3
	lsls	r1, r1, #10
	adds	r2, r5, #0
	bl	sub_080151e4
	ldr	r0, [pc, #68]
	ldr	r1, [sp, #64]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, r8
	bl	sub_08196a7c
.L_08140760:
	mov	r0, r8
	bl	sub_08013164
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #168
	ldr	r0, [sp, #64]
	add	r5, fp
	bl	sub_08013164
	ldr	r3, [r5, #0]
	cmp	r3, #0
	ble.n	.L_081407a0
	bl	sub_08014878
	movs	r3, #7
	ldr	r2, [pc, #24]
	ands	r3, r0
	adds	r3, #28
	strh	r3, [r2, #6]
	ldr	r3, [r5, #0]
	subs	r3, #1
	str	r3, [r5, #0]
	b.n	.L_081407ae
	.4byte 0xffc10000
	.4byte 0xffc00000
	.4byte 0x081976e0
	.2byte 0x1120
	.2byte 0x0300
.L_081407a0:
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #164
	add	r2, fp
	ldr	r3, [pc, #336]
	ldr	r2, [r2, #0]
	strh	r2, [r3, #6]
.L_081407ae:
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #228
	add	r2, fp
.L_081407ba:
	ldr	r3, [r2, #0]
	ldr	r3, [r2, #0]
	cmp	r3, #1
	bls.n	.L_081407ba
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [sp, #116]
	adds	r3, #1
	str	r3, [sp, #116]
	cmp	r3, #180
	beq.n	.L_081407e0
	b.n	.L_081400aa
.L_081407e0:
	movs	r4, #1
	negs	r4, r4
	movs	r2, #16
	ldr	r0, [pc, #280]
	movs	r1, #8
	movs	r3, #32
	str	r4, [sp, #60]
	bl	sub_08178680
	movs	r2, #0
.L_081407f4:
	lsls	r5, r2, #5
	str	r5, [sp, #36]
	movs	r0, #0
	movs	r7, #0
	mov	r9, r0
.L_081407fe:
	ldr	r3, [pc, #260]
	ldrb	r6, [r3, r2]
	cmp	r7, #15
	ble.n	.L_0814080e
	lsls	r3, r7, #1
	adds	r3, r6, r3
	adds	r6, r3, #0
	subs	r6, #32
.L_0814080e:
	ldr	r4, [sp, #36]
	ldr	r0, [pc, #244]
	adds	r3, r4, r2
	adds	r3, r3, r7
	lsls	r3, r3, #3
	adds	r5, r3, r0
	mov	r4, r9
	movs	r3, #126
	subs	r4, r3, r4
	lsrs	r3, r6, #31
	movs	r1, #0
	adds	r3, r6, r3
	mov	r8, r4
	mov	sl, r1
	asrs	r4, r3, #1
.L_0814082c:
	movs	r3, #144
	lsls	r3, r3, #4
	adds	r3, #44
	mov	r0, r8
	strb	r0, [r5, #0]
	adds	r0, r7, #0
	muls	r0, r3
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	str	r4, [sp, #8]
	bl	sub_08002096
	ldr	r3, [pc, #196]
	ldr	r2, [sp, #12]
	ldr	r1, [sp, #16]
	ldrb	r3, [r3, r2]
	ldr	r4, [sp, #8]
	muls	r3, r0
	asrs	r3, r3, #16
	adds	r3, r3, r1
	subs	r3, r3, r4
	movs	r0, #1
	strb	r3, [r5, #1]
	add	sl, r0
	movs	r3, #0
	strb	r3, [r5, #2]
	mov	r3, sl
	adds	r1, r1, r6
	adds	r5, #4
	cmp	r3, #2
	bne.n	.L_0814082c
	movs	r4, #7
	adds	r7, #1
	add	r9, r4
	cmp	r7, #33
	bne.n	.L_081407fe
	adds	r2, #1
	cmp	r2, #2
	bne.n	.L_081407f4
	movs	r3, #239
	movs	r2, #238
	lsls	r3, r3, #7
	lsls	r2, r2, #7
	add	r3, fp
	mov	r5, sl
	adds	r2, #132
	str	r5, [r3, #0]
	add	r2, fp
	movs	r3, #50
	str	r3, [r2, #0]
	movs	r1, #128
	ldr	r0, [sp, #128]
	ldr	r6, [pc, #120]
	lsls	r1, r1, #7
	ldr	r2, [pc, #120]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x22f0
	lsls	r2, r2, #7
	adds	r2, #232
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #128
	ldr	r0, [sp, #128]
	lsls	r1, r1, #7
	movs	r2, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4b0e
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r1, [pc, #80]
	movs	r0, #1
	movs	r2, #0
	bl	sub_08118040
	movs	r0, #31
	movs	r1, #31
	movs	r2, #31
	bl	sub_08164abc
	movs	r1, #244
	lsls	r1, r1, #6
	adds	r1, #72
	ldr	r0, [pc, #56]
	add	r1, fp
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #244
	lsls	r1, r1, #6
	adds	r1, #200
	movs	r3, #0
	add	r1, fp
	b.n	.L_08140920
	.4byte 0x00000786
	.4byte 0x03001120
	.4byte 0x02012000
	.4byte 0x08197702
	.4byte 0x02010000
	.4byte 0x08197700
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x00000045
	.2byte 0x00ca
	.2byte 0x0000
.L_08140920:
	movs	r2, #0
	ldr	r0, [pc, #392]
	bl	sub_08157cf4
	bl	sub_0815b410
	ldr	r0, [pc, #384]
	bl	sub_08013300
	movs	r1, #32
	adds	r5, r0, #0
	ldr	r2, [pc, #380]
	ldr	r0, [pc, #380]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x21e0
	adds	r5, #32
	lsls	r1, r1, #3
	movs	r6, #238
	add	r1, fp
	adds	r0, r5, #0
	lsls	r6, r6, #7
	bl	sub_0801587c
	adds	r6, #220
	movs	r0, #0
	movs	r7, #0
	mov	r9, r0
	add	r6, fp
	mov	r8, r0
.L_0814095c:
	lsls	r3, r7, #12
	movs	r2, #224
	movs	r1, #0
	add	r3, fp
	lsls	r2, r2, #3
	mov	sl, r1
	adds	r5, r3, r2
.L_0814096a:
	movs	r2, #128
	movs	r3, #240
	movs	r1, #32
	lsls	r2, r2, #24
	lsls	r3, r3, #8
	movs	r0, #32
	bl	sub_0815b290
	mov	r3, r8
	movs	r4, #238
	add	r3, sl
	lsls	r4, r4, #7
	adds	r4, #220
	lsls	r3, r3, #2
	adds	r3, r3, r4
	mov	r1, fp
	str	r0, [r1, r3]
	movs	r4, #13
	ldrb	r3, [r0, #9]
	negs	r4, r4
	adds	r2, r4, #0
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r0, #9]
	ldrb	r3, [r0, #16]
	ldr	r0, [pc, #284]
	lsls	r3, r3, #2
	adds	r3, r3, r0
	ldrh	r0, [r3, #2]
	ldr	r1, [pc, #280]
	movs	r2, #128
	adds	r0, r0, r1
	lsls	r2, r2, #2
	adds	r1, r5, #0
	ldr	r3, [pc, #272]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2001
	movs	r4, #128
	add	sl, r0
	lsls	r4, r4, #2
	mov	r1, sl
	adds	r5, r5, r4
	cmp	r1, #8
	bne.n	.L_0814096a
	movs	r2, #128
	movs	r3, #240
	movs	r1, #32
	lsls	r2, r2, #24
	lsls	r3, r3, #8
	movs	r0, #32
	bl	sub_0815b3b0
	mov	r5, r9
	ldr	r1, [r6, #0]
	movs	r2, #24
	ldr	r3, [pc, #228]
	str	r0, [r6, #32]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x19ea
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	movs	r0, #238
	add	r3, fp
	lsls	r2, r2, #2
	lsls	r0, r0, #7
	movs	r4, #0
	adds	r1, r3, #0
	add	r2, fp
	adds	r0, #220
	mov	sl, r4
	adds	r1, #24
	adds	r2, r2, r0
.L_08140a02:
	ldmia	r2!, {r3}
	ldrh	r3, [r3, #8]
	lsls	r3, r3, #22
	lsrs	r3, r3, #22
	str	r3, [r1, #0]
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	adds	r1, #28
	cmp	r4, #8
	bne.n	.L_08140a02
	movs	r5, #8
	movs	r0, #9
	adds	r7, #1
	add	r9, r5
	adds	r6, #36
	add	r8, r0
	cmp	r7, #2
	bne.n	.L_0814095c
	movs	r0, #1
	bl	sub_08013560
	mov	r2, sp
	mov	r3, sp
	mov	r4, sp
	movs	r1, #0
	adds	r2, #152
	adds	r3, #168
	adds	r4, #144
	str	r1, [sp, #116]
	str	r2, [sp, #48]
	str	r3, [sp, #40]
	str	r4, [sp, #52]
	str	r1, [sp, #24]
.L_08140a46:
	ldr	r3, [pc, #128]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08140a86
	ldr	r5, [sp, #116]
	cmp	r5, #84
	bgt.n	.L_08140a86
	ldr	r0, [sp, #60]
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	bne.n	.L_08140a86
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #4
	str	r3, [r2, #0]
	movs	r2, #238
	ldr	r3, [pc, #92]
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, fp
	str	r3, [r2, #0]
	movs	r2, #17
	str	r2, [sp, #60]
	ldr	r3, [pc, #40]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
.L_08140a86:
	ldr	r3, [sp, #116]
	cmp	r3, #17
	bgt.n	.L_08140b4a
	ldr	r4, [sp, #24]
	movs	r2, #31
	subs	r1, r2, r4
	adds	r3, r4, r3
	adds	r0, r1, #0
	subs	r2, r2, r3
	cmp	r1, #0
	bge.n	.L_08140a9e
	movs	r0, #0
.L_08140a9e:
	cmp	r1, #5
	bgt.n	.L_08140ad0
	movs	r1, #6
	b.n	.L_08140ad0
	movs	r0, r0
	.4byte 0x00000784
	.4byte 0x00000104
	.4byte 0x000000a0
	.4byte 0x7fff7fff
	.4byte 0x050003e0
	.4byte 0x020036e0
	.4byte 0x06010000
	.4byte 0x03000730
	.4byte 0x03001150
	.2byte 0x0404
	.2byte 0x0404
.L_08140ad0:
	movs	r5, #18
	negs	r5, r5
	cmp	r2, r5
	bge.n	.L_08140adc
	movs	r2, #18
	negs	r2, r2
.L_08140adc:
	bl	sub_08164b2c
	ldr	r0, [sp, #116]
	cmp	r0, #16
	bgt.n	.L_08140b4a
	ldr	r0, [pc, #60]
	bl	sub_08013300
	ldr	r2, [sp, #24]
	ldr	r5, [pc, #56]
	ldr	r6, [pc, #44]
	movs	r1, #0
	movs	r3, #32
	mov	sl, r1
	subs	r4, r3, r2
.L_08140afa:
	ldrh	r3, [r0, #0]
	movs	r2, #31
	ands	r2, r3
	lsls	r3, r3, #16
	adds	r1, r2, r4
	lsrs	r2, r3, #21
	lsrs	r3, r3, #26
	ands	r2, r6
	ands	r3, r6
	adds	r2, r2, r4
	adds	r3, r3, r4
	cmp	r1, #31
	ble.n	.L_08140b16
	movs	r1, #31
.L_08140b16:
	cmp	r2, #31
	ble.n	.L_08140b2c
	movs	r2, #31
	b.n	.L_08140b2c
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x000000a0
	.2byte 0x03e0
	.2byte 0x0500
.L_08140b2c:
	cmp	r3, #31
	ble.n	.L_08140b32
	movs	r3, #31
.L_08140b32:
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r1
	strh	r3, [r5, #0]
	movs	r3, #1
	add	sl, r3
	mov	r1, sl
	adds	r0, #2
	adds	r5, #2
	cmp	r1, #16
	bne.n	.L_08140afa
.L_08140b4a:
	ldr	r2, [sp, #116]
	cmp	r2, #0
	bne.n	.L_08140b5a
	movs	r2, #128
	ldr	r3, [pc, #52]
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
.L_08140b5a:
	ldr	r3, [sp, #116]
	cmp	r3, #75
	bne.n	.L_08140b6a
	movs	r2, #128
	ldr	r3, [pc, #40]
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
.L_08140b6a:
	ldr	r4, [sp, #116]
	cmp	r4, #85
	bne.n	.L_08140b96
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #4
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	ldr	r3, [pc, #16]
	adds	r2, #132
	add	r2, fp
	b.n	.L_08140b94
	movs	r0, r0
	.4byte 0x00000786
	.4byte 0x00000784
	.2byte 0x0404
	.2byte 0x0404
.L_08140b94:
	str	r3, [r2, #0]
.L_08140b96:
	movs	r0, #132
	lsls	r0, r0, #2
	bl	sub_08014dac
	str	r0, [sp, #56]
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #760]
	ldr	r3, [sp, #152]
	ldr	r5, [sp, #48]
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #752]
	movs	r1, #0
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #2
	orrs	r3, r2
	str	r3, [sp, #152]
	movs	r3, #244
	lsls	r3, r3, #6
	adds	r3, #72
	add	r3, fp
	str	r3, [r5, #4]
	ldr	r3, [pc, #732]
	str	r1, [r0, #20]
	str	r3, [r0, #8]
	str	r5, [r0, #16]
	ldr	r2, [sp, #56]
	movs	r3, #6
	str	r2, [r0, #12]
	str	r3, [r0, #0]
	mov	r9, r0
	mov	sl, r1
.L_08140bde:
	ldr	r3, [pc, #716]
	mov	r4, sl
	ldrb	r3, [r3, r4]
	ldr	r0, [sp, #116]
	subs	r5, r0, r3
	cmp	r5, #0
	bne.n	sub_08140c18
	cmp	r4, #5
	bne.n	.L_08140bf8
	movs	r0, #145
	bl	sub_081c0010
	b.n	.L_08140bfe
.L_08140bf8:
	movs	r0, #144
	bl	sub_081c0010
.L_08140bfe:
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	add	r3, fp
	movs	r2, #8
	str	r2, [r3, #0]
	movs	r1, #128
	ldr	r3, [pc, #672]
	ldr	r0, [sp, #128]
	lsls	r1, r1, #7
	ldr	r2, [pc, #672]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2d0f
	bhi.n	.L_08140cb0
	lsls	r0, r5, #11
	bl	sub_08002096
	ldr	r3, [pc, #660]
	mov	r1, sl
	ldrb	r3, [r3, r1]
	muls	r3, r0
	asrs	r3, r3, #16
	adds	r7, r3, #2
	cmp	r7, #1
	ble.n	.L_08140cb0
	ldr	r3, [pc, #648]
	movs	r0, #224
	ldrb	r3, [r3, r1]
	lsls	r0, r0, #3
	mov	r8, r3
	lsls	r3, r5, #3
	add	r8, r3
	ldr	r3, [pc, #636]
	movs	r2, #128
	ldrb	r6, [r3, r1]
	lsls	r2, r2, #9
	add	r0, fp
	adds	r1, r7, #0
	bl	sub_0815b434
	movs	r1, #19
	movs	r0, #188
	bl	sub_081963ec
	lsrs	r5, r7, #31
	adds	r5, r7, r5
	asrs	r5, r5, #1
	str	r5, [sp, #0]
	str	r7, [sp, #4]
	movs	r0, #192
	subs	r6, r6, r7
	lsls	r0, r0, #18
	movs	r1, #224
	mov	r3, r8
	adds	r6, #8
	adds	r0, #188
	lsls	r1, r1, #3
	subs	r2, r3, r5
	ldr	r4, [r0, #0]
	adds	r3, r6, #0
	ldr	r0, [sp, #128]
	add	r1, fp
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r1, #23
	movs	r0, #188
	bl	sub_081963ec
	movs	r1, #192
	str	r5, [sp, #0]
	str	r7, [sp, #4]
	lsls	r1, r1, #18
	adds	r1, #188
	ldr	r4, [r1, #0]
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [sp, #128]
	add	r1, fp
	mov	r2, r8
	adds	r3, r6, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
.L_08140cb0:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #6
	bne.n	.L_08140bde
	movs	r5, #128
	lsls	r5, r5, #8
	mov	r8, r5
	movs	r7, #224
	movs	r5, #252
	movs	r4, #0
	lsls	r5, r5, #1
	lsls	r7, r7, #3
	mov	sl, r4
	add	r5, fp
	add	r7, fp
.L_08140cd0:
	ldr	r3, [pc, #496]
	mov	r0, sl
	ldrb	r3, [r3, r0]
	ldr	r1, [sp, #116]
	lsrs	r2, r3, #1
	cmp	r1, #0
	bne.n	.L_08140cea
	ldr	r3, [pc, #488]
	str	r1, [r5, #4]
	ldrb	r3, [r3, r0]
	str	r1, [r5, #24]
	lsls	r3, r3, #16
	str	r3, [r5, #0]
.L_08140cea:
	ldr	r3, [sp, #116]
	cmp	r3, r2
	blt.n	.L_08140da2
	ldr	r4, [sp, #116]
	adds	r3, r2, #0
	adds	r3, #32
	cmp	r4, r3
	bge.n	.L_08140da2
	ldr	r3, [pc, #464]
	mov	r0, sl
	ldrb	r6, [r3, r0]
	ldr	r3, [pc, #460]
	ldr	r1, [r5, #4]
	ldrb	r3, [r3, r0]
	lsls	r3, r3, #16
	cmp	r1, r3
	bge.n	.L_08140d24
	ldr	r3, [pc, #452]
	ldrb	r2, [r3, r0]
	ldr	r3, [r5, #0]
	lsls	r2, r2, #16
	subs	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r3, [pc, #444]
	ldrb	r3, [r3, r0]
	lsls	r3, r3, #16
	adds	r3, r1, r3
	str	r3, [r5, #4]
	b.n	.L_08140d2a
.L_08140d24:
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_08140d2a:
	ldr	r3, [r5, #0]
	movs	r1, #128
	lsls	r1, r1, #12
	adds	r3, r3, r1
	str	r3, [r5, #0]
	bl	sub_08014de4
	ldr	r3, [pc, #416]
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	adds	r0, r0, r3
	adds	r1, r1, r3
	movs	r2, #0
	bl	sub_08015160
	movs	r1, #128
	lsls	r1, r1, #9
	mov	r0, r8
	mov	r2, r8
	bl	sub_080151e4
	ldr	r2, [pc, #392]
	mov	r4, sl
	lsls	r3, r4, #2
	ldr	r0, [r2, r3]
	bl	sub_080150e4
	movs	r3, #144
	lsls	r3, r3, #6
	adds	r3, #184
	movs	r1, #128
	adds	r0, r6, #0
	muls	r0, r3
	lsls	r1, r1, #10
	mov	r2, r8
	bl	sub_080151e4
	ldr	r0, [pc, #364]
	ldr	r1, [sp, #56]
	movs	r2, #4
	bl	sub_08196958
	movs	r2, #128
	adds	r0, r7, #0
	adds	r1, r6, #0
	lsls	r2, r2, #9
	bl	sub_0815b434
	movs	r0, #2
	ldrsh	r1, [r5, r0]
	movs	r3, #6
	ldrsh	r2, [r5, r3]
	adds	r0, r7, #0
	subs	r1, #1
	adds	r3, r6, #0
	bl	sub_0818caa8
	mov	r0, r9
	bl	sub_08196a7c
.L_08140da2:
	movs	r4, #1
	add	sl, r4
	mov	r0, sl
	adds	r5, #28
	cmp	r0, #4
	bne.n	.L_08140cd0
	ldr	r1, [sp, #48]
	movs	r5, #6
	add	r3, sp, #152
	strb	r5, [r1, #0]
	str	r3, [sp, #48]
	ldr	r2, [sp, #48]
	movs	r3, #5
	strb	r3, [r2, #1]
	movs	r3, #244
	lsls	r3, r3, #6
	adds	r3, #200
	add	r3, fp
	str	r3, [r2, #4]
	ldr	r0, [pc, #284]
	movs	r3, #32
	movs	r1, #2
	movs	r2, #32
	bl	sub_08178680
	ldr	r3, [pc, #272]
	mov	r4, r9
	str	r3, [r4, #8]
	movs	r3, #64
	strb	r3, [r4, #24]
	ldr	r6, [pc, #268]
	str	r5, [r4, #0]
	movs	r5, #0
	mov	sl, r5
	movs	r7, #127
.L_08140de8:
	movs	r3, #46
	mov	r0, sl
	muls	r0, r3
	ldr	r1, [sp, #116]
	adds	r3, r0, #0
	subs	r3, r1, r3
	adds	r5, r3, #0
	subs	r5, #12
	cmp	r5, #45
	bls.n	.L_08140dfe
	b.n	.L_08140f12
.L_08140dfe:
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L_08140e24
	lsls	r2, r5, #1
	adds	r3, r2, #0
	adds	r3, #63
	adds	r4, r7, #0
	bics	r4, r3
	adds	r3, r4, #0
	mov	r0, r9
	strb	r3, [r0, #24]
	cmp	r5, #32
	bgt.n	.L_08140e48
	ldr	r1, [pc, #204]
	adds	r3, r2, r5
	lsls	r3, r3, #3
	adds	r3, r3, r1
	mov	r2, sl
	b.n	.L_08140e42
.L_08140e24:
	lsls	r3, r5, #2
	adds	r3, #63
	adds	r4, r7, #0
	bics	r4, r3
	adds	r3, r4, #0
	mov	r0, r9
	movs	r2, #0
	strb	r3, [r0, #24]
	cmp	r5, #15
	bgt.n	.L_08140e48
	lsls	r3, r5, #1
	ldr	r1, [pc, #172]
	adds	r3, r3, r5
	lsls	r3, r3, #4
	adds	r3, r3, r1
.L_08140e42:
	strb	r2, [r3, #0]
	strb	r2, [r3, #1]
	strb	r2, [r3, #2]
.L_08140e48:
	bl	sub_08014de4
	ldr	r0, [pc, #160]
	cmp	r5, #0
	blt.n	.L_08140e56
	lsls	r3, r5, #16
	adds	r0, r3, r0
.L_08140e56:
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L_08140e68
	cmp	r5, #31
	ble.n	.L_08140e76
	lsls	r3, r5, #3
	subs	r3, r3, r5
	subs	r3, #224
	b.n	.L_08140e72
.L_08140e68:
	cmp	r5, #15
	ble.n	.L_08140e76
	lsls	r3, r5, #3
	subs	r3, r3, r5
	subs	r3, #112
.L_08140e72:
	lsls	r3, r3, #16
	adds	r0, r0, r3
.L_08140e76:
	ldr	r1, [pc, #124]
	movs	r2, #0
	bl	sub_08015160
	movs	r0, #177
	lsls	r0, r0, #8
	movs	r1, #128
	movs	r2, #128
	adds	r0, #224
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl	sub_080151e4
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_08140efc
	ldr	r0, [pc, #96]
	bl	sub_080150e4
	b.n	.L_08140f02
	movs	r0, r0
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199220
	.4byte 0x08197704
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x0819770a
	.4byte 0x08197710
	.4byte 0x08197716
	.4byte 0x0819771c
	.4byte 0x08197724
	.4byte 0x08197720
	.4byte 0x08197730
	.4byte 0x08197728
	.4byte 0x0819772c
	.4byte 0xffc00000
	.4byte 0x08197734
	.4byte 0x081976e0
	.4byte 0x02012000
	.4byte 0x02010000
	.4byte 0xffe80000
	.4byte 0xfff00000
	.2byte 0xf418
	.2byte 0xffff
.L_08140efc:
	.2byte 0x4866
	bl	sub_080150e4
.L_08140f02:
	adds	r0, r6, #0
	ldr	r1, [sp, #56]
	movs	r2, #66
	bl	sub_08196958
	mov	r0, r9
	bl	sub_08196a7c
.L_08140f12:
	movs	r5, #1
	movs	r4, #132
	add	sl, r5
	lsls	r4, r4, #1
	mov	r0, sl
	adds	r6, r6, r4
	cmp	r0, #2
	beq.n	.L_08140f24
	b.n	.L_08140de8
.L_08140f24:
	mov	r0, r9
	bl	sub_08013164
	ldr	r0, [sp, #56]
	bl	sub_08013164
	ldr	r3, [pc, #360]
	ldr	r2, [sp, #40]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	movs	r1, #0
	str	r3, [sp, #144]
	str	r4, [sp, #148]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r1, [r2, #12]
	str	r3, [r2, #4]
	ldr	r3, [sp, #100]
	movs	r4, #192
	ldr	r5, [pc, #340]
	lsls	r4, r4, #13
	adds	r3, r3, r4
	str	r3, [sp, #100]
	cmp	r3, r5
	ble.n	.L_08140f76
	ldr	r2, [sp, #96]
	adds	r2, #7
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_08140f64
	ldr	r3, [sp, #96]
	adds	r3, #14
.L_08140f64:
	ldr	r0, [sp, #100]
	ldr	r1, [pc, #316]
	asrs	r3, r3, #3
	str	r3, [sp, #96]
	lsls	r3, r3, #3
	subs	r2, r2, r3
	adds	r0, r0, r1
	str	r2, [sp, #96]
	str	r0, [sp, #100]
.L_08140f76:
	ldr	r2, [pc, #304]
	movs	r4, #255
	ldrh	r3, [r2, #4]
	lsls	r4, r4, #8
	adds	r4, #248
	adds	r3, r3, r4
	strh	r3, [r2, #4]
	ldr	r5, [sp, #40]
	movs	r6, #238
	lsls	r6, r6, #7
	adds	r6, #252
	movs	r7, #0
	mov	r8, r5
	movs	r4, #0
	add	r6, fp
.L_08140f94:
	movs	r2, #238
	lsls	r3, r4, #2
	lsls	r2, r2, #7
	add	r3, fp
	adds	r2, #220
	ldr	r5, [sp, #96]
	adds	r2, r3, r2
	str	r2, [sp, #20]
	movs	r0, #0
	lsls	r1, r7, #21
	mov	sl, r0
	mov	r9, r1
	adds	r5, #64
.L_08140fae:
	ldr	r1, [sp, #100]
	mov	r0, sl
	lsls	r3, r0, #21
	adds	r3, r3, r1
	mov	r2, r8
	str	r3, [r2, #0]
	movs	r3, #242
	lsls	r3, r3, #15
	add	r3, r9
	str	r3, [r2, #8]
	cmp	r0, #8
	bne.n	.L_08140fde
	ldr	r3, [sp, #96]
	ldr	r1, [r6, #0]
	adds	r3, #71
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L_08140fd6
	ldr	r2, [sp, #96]
	adds	r2, #78
.L_08140fd6:
	asrs	r2, r2, #3
	lsls	r2, r2, #3
	subs	r2, r3, r2
	b.n	.L_08141004
.L_08140fde:
	mov	r1, sl
	movs	r2, #238
	adds	r3, r4, r1
	lsls	r2, r2, #7
	adds	r2, #220
	lsls	r3, r3, #2
	adds	r3, r3, r2
	mov	r0, fp
	ldr	r1, [r0, r3]
	ldr	r3, [sp, #96]
	adds	r2, r5, #0
	add	r3, sl
	cmp	r5, #0
	bge.n	.L_08140ffe
	adds	r2, r3, #0
	adds	r2, #71
.L_08140ffe:
	asrs	r2, r2, #3
	lsls	r2, r2, #3
	subs	r2, r5, r2
.L_08141004:
	adds	r2, r4, r2
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, #24
	mov	r0, fp
	ldr	r2, [r0, r3]
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r2, r3
	ldr	r0, [pc, #124]
	ldrh	r3, [r1, #8]
	ands	r3, r0
	orrs	r3, r2
	strh	r3, [r1, #8]
	ldr	r1, [sp, #40]
	ldr	r2, [pc, #132]
	ldr	r3, [r1, #0]
	cmp	r3, r2
	ble.n	.L_08141034
	ldr	r0, [pc, #128]
	adds	r3, r3, r0
	str	r3, [r1, #0]
.L_08141034:
	ldr	r2, [sp, #20]
	movs	r3, #0
	ldmia	r2!, {r0}
	str	r4, [sp, #8]
	adds	r1, r2, #0
	str	r1, [sp, #20]
	ldr	r2, [sp, #52]
	ldr	r1, [sp, #40]
	bl	sub_08020010
	movs	r3, #1
	add	sl, r3
	mov	r0, sl
	adds	r5, #1
	ldr	r4, [sp, #8]
	cmp	r0, #9
	bne.n	.L_08140fae
	adds	r7, #1
	adds	r4, #9
	adds	r6, #36
	cmp	r7, #2
	bne.n	.L_08140f94
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #228
	add	r2, fp
.L_0814106c:
	ldr	r3, [r2, #0]
	ldr	r3, [r2, #0]
	cmp	r3, #1
	bls.n	.L_0814106c
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #168
	add	r5, fp
	ldr	r3, [r5, #0]
	cmp	r3, #0
	ble.n	.L_081410b4
	bl	sub_08014878
	movs	r3, #7
	ldr	r2, [pc, #28]
	ands	r3, r0
	adds	r3, #28
	strh	r3, [r2, #6]
	ldr	r3, [r5, #0]
	subs	r3, #1
	str	r3, [r5, #0]
	b.n	.L_081410c2
	.4byte 0xfffffc00
	.4byte 0x08196dd8
	.4byte 0x001fffff
	.4byte 0xffe00000
	.4byte 0x03001120
	.4byte 0x00ffffff
	.2byte 0x0000
	.2byte 0xfee0
.L_081410b4:
	.2byte 0x22ee
	lsls	r2, r2, #7
	adds	r2, #164
	add	r2, fp
	ldr	r3, [pc, #288]
	ldr	r2, [r2, #0]
	strh	r2, [r3, #6]
.L_081410c2:
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #116]
	adds	r1, #2
	adds	r2, #1
	str	r1, [sp, #24]
	str	r2, [sp, #116]
	cmp	r2, #102
	beq.n	.L_081410f6
	ldr	r3, [sp, #60]
	cmp	r3, #0
	bgt.n	.L_081410ec
	b.n	.L_08140a46
.L_081410ec:
	subs	r3, #1
	str	r3, [sp, #60]
	cmp	r3, #0
	beq.n	.L_081410f6
	b.n	.L_08140a46
.L_081410f6:
	add	r4, sp, #104
	ldr	r1, [pc, #228]
	ldrh	r4, [r4, #0]
	movs	r3, #0
	strh	r4, [r1, #4]
	ldr	r5, [sp, #108]
	movs	r0, #206
	str	r3, [r5, #16]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	lsls	r0, r0, #3
	adds	r3, r3, r0
	movs	r2, #32
	strh	r2, [r1, #6]
	movs	r0, #1
	ldrh	r1, [r3, #0]
	movs	r2, #24
	bl	sub_08118040
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [pc, #188]
	movs	r3, #120
	movs	r0, #195
	movs	r5, #238
	str	r3, [r2, #16]
	lsls	r0, r0, #1
	lsls	r5, r5, #7
	bl	sub_081c0010
	adds	r5, #220
	movs	r1, #0
	mov	sl, r1
	add	r5, fp
.L_0814113e:
	ldmia	r5!, {r0}
	bl	sub_08020048
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #18
	bne.n	.L_0814113e
	bl	sub_08014c4c
	movs	r3, #128
	movs	r1, #160
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r1, r1, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #132]
	adds	r1, #160
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #128]
	movs	r2, #160
	ldrh	r3, [r3, #0]
	lsls	r2, r2, #19
	adds	r2, #188
	strh	r3, [r2, #0]
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #3
	str	r3, [r2, #0]
	movs	r2, #238
	ldr	r3, [pc, #108]
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, fp
	str	r3, [r2, #0]
	movs	r1, #128
	ldr	r0, [sp, #128]
	ldr	r3, [pc, #96]
	lsls	r1, r1, #7
	ldr	r2, [pc, #96]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b0e
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r4, [sp, #132]
	ldr	r2, [pc, #48]
	ldr	r3, [r4, #20]
	adds	r5, r4, #0
	lsls	r3, r3, #1
	adds	r3, #36
	adds	r0, r5, #0
	strh	r2, [r5, r3]
	adds	r0, #36
	movs	r1, #0
	bl	sub_08118010
	movs	r1, #253
	lsls	r1, r1, #6
	ldr	r0, [pc, #56]
	add	r1, fp
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #128
	ldr	r3, [pc, #12]
	lsls	r2, r2, #19
	b.n	.L_08141200
	.4byte 0x00000784
	.4byte 0x000000ff
	.4byte 0x00001010
	.4byte 0x03001120
	.4byte 0x030011e0
	.4byte 0x05000200
	.4byte 0x050001e8
	.4byte 0x02020202
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.2byte 0x00ca
	.2byte 0x0000
.L_08141200:
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r0, #0
	movs	r1, #224
	mov	sl, r0
	lsls	r1, r1, #3
.L_0814120c:
	mov	r2, sl
	lsrs	r3, r2, #31
	add	r3, sl
	asrs	r2, r3, #1
	cmp	r2, #63
	ble.n	.L_0814121a
	movs	r2, #63
.L_0814121a:
	mov	r3, sl
	add	r3, fp
	movs	r7, #0
	adds	r3, r3, r1
.L_08141222:
	adds	r7, #1
	strb	r2, [r3, #0]
	adds	r3, #120
	cmp	r7, #120
	bne.n	.L_08141222
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	cmp	r4, #120
	bne.n	.L_0814120c
	ldr	r0, [pc, #708]
	ldr	r1, [pc, #708]
	movs	r5, #0
	movs	r2, #80
	str	r5, [sp, #116]
	str	r0, [sp, #32]
	str	r1, [sp, #28]
	negs	r2, r2
	mov	r9, r2
.L_08141248:
	ldr	r3, [sp, #116]
	cmp	r3, #0
	bne.n	.L_0814127e
	ldr	r5, [sp, #132]
	movs	r4, #0
	ldr	r3, [r5, #20]
	mov	sl, r4
	cmp	r3, #0
	beq.n	.L_0814127e
	movs	r5, #36
	movs	r6, #16
.L_0814125e:
	ldr	r1, [sp, #132]
	mov	r3, sl
	ldrsh	r0, [r5, r1]
	movs	r2, #1
	movs	r1, #0
	negs	r2, r2
	str	r6, [sp, #0]
	bl	sub_0814cd48
	ldr	r4, [sp, #132]
	movs	r3, #1
	add	sl, r3
	ldr	r3, [r4, #20]
	adds	r5, #2
	cmp	sl, r3
	bne.n	.L_0814125e
.L_0814127e:
	ldr	r5, [sp, #116]
	cmp	r5, #127
	bne.n	.L_0814128a
	movs	r0, #134
	bl	sub_081180e8
.L_0814128a:
	ldr	r0, [sp, #116]
	cmp	r0, #107
	bne.n	sub_0814129e
	movs	r1, #128
	ldr	r3, [pc, #624]
	ldr	r0, [sp, #128]
	lsls	r1, r1, #7
	ldr	r2, [pc, #620]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x991d
	cmp	r1, #107
	ble.n	.L_081412c4
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #3
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	ldr	r3, [pc, #600]
	adds	r2, #132
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl	sub_08164b2c
.L_081412c4:
	ldr	r2, [sp, #116]
	cmp	r2, #32
	bne.n	.L_081412d0
	movs	r0, #140
	bl	sub_081c0010
.L_081412d0:
	ldr	r3, [sp, #116]
	cmp	r3, #48
	bne.n	.L_081412e8
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, fp
	movs	r3, #90
	str	r3, [r2, #0]
	movs	r0, #145
	bl	sub_081c0010
.L_081412e8:
	ldr	r4, [sp, #116]
	cmp	r4, #72
	bne.n	.L_081412f4
	movs	r0, #208
	bl	sub_081c0010
.L_081412f4:
	ldr	r5, [sp, #116]
	cmp	r5, #109
	ble.n	.L_081412fc
	b.n	.L_08141484
.L_081412fc:
	cmp	r5, #31
	ble.n	.L_08141326
	adds	r1, r5, #0
	subs	r1, #32
	cmp	r1, #0
	bge.n	.L_0814130a
	adds	r1, #3
.L_0814130a:
	ldr	r3, [sp, #116]
	asrs	r0, r1, #2
	movs	r2, #0
	cmp	r3, #47
	ble.n	.L_0814131c
	subs	r3, #48
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r2, r3, #1
.L_0814131c:
	lsrs	r1, r1, #31
	adds	r1, r0, r1
	asrs	r1, r1, #1
	bl	sub_08164abc
.L_08141326:
	mov	r4, r9
	cmp	r4, #31
	bhi.n	.L_0814134a
	ldr	r3, [sp, #28]
	ldr	r0, [sp, #32]
	ldr	r2, [pc, #476]
	movs	r5, #0
	orrs	r3, r0
	mov	sl, r5
	orrs	r3, r4
.L_0814133a:
	movs	r1, #1
	add	sl, r1
	mov	r4, sl
	strh	r3, [r2, #0]
	adds	r2, #2
	cmp	r4, #223
	bne.n	.L_0814133a
	b.n	.L_0814135e
.L_0814134a:
	ldr	r5, [sp, #116]
	cmp	r5, #47
	ble.n	sub_08141380
	ldr	r1, [pc, #448]
	movs	r2, #240
	adds	r0, r1, #0
	lsls	r2, r2, #8
	movs	r3, #224
	bl	sub_081180f0
.L_0814135e:
	ldr	r0, [sp, #116]
	cmp	r0, #47
	ble.n	sub_08141380
	lsls	r3, r0, #1
	movs	r2, #216
	movs	r1, #224
	subs	r2, r2, r3
	lsls	r1, r1, #3
	movs	r3, #120
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	ldr	r0, [sp, #128]
	add	r1, fp
	movs	r3, #0
	ldr	r4, [sp, #120]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2020
	bl	sub_08014dac
	mov	sl, r0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #392]
	ldr	r3, [sp, #136]
	movs	r5, #0
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #384]
	mov	r1, sl
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #2
	orrs	r3, r2
	str	r3, [sp, #136]
	movs	r3, #253
	lsls	r3, r3, #6
	add	r2, sp, #136
	add	r3, fp
	str	r3, [r2, #4]
	ldr	r3, [pc, #364]
	str	r5, [r0, #20]
	str	r3, [r0, #8]
	movs	r3, #6
	str	r2, [r0, #16]
	str	r1, [r0, #12]
	str	r3, [r0, #0]
	ldr	r2, [sp, #116]
	mov	r8, r0
	cmp	r2, #31
	ble.n	.L_08141478
	movs	r6, #192
	lsls	r3, r2, #1
	lsls	r6, r6, #3
	adds	r7, r3, #0
	adds	r6, #228
	subs	r7, #32
	add	r6, fp
	cmp	r7, #64
	ble.n	.L_081413dc
	movs	r7, #64
.L_081413dc:
	ldr	r3, [sp, #116]
	cmp	r3, #32
	bne.n	.L_081413ec
	movs	r3, #128
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	ldr	r3, [pc, #312]
	str	r3, [r6, #4]
.L_081413ec:
	ldr	r4, [sp, #116]
	cmp	r4, #43
	bgt.n	.L_08141426
	ldr	r3, [r6, #0]
	ldr	r5, [pc, #304]
	movs	r0, #128
	adds	r3, r3, r5
	str	r3, [r6, #0]
	ldr	r3, [r6, #4]
	movs	r5, #255
	lsls	r0, r0, #13
	lsls	r5, r5, #6
	adds	r3, r3, r0
	add	r5, fp
	movs	r2, #128
	str	r3, [r6, #4]
	lsls	r2, r2, #9
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	sub_0815b434
	movs	r2, #2
	ldrsh	r1, [r6, r2]
	adds	r0, r5, #0
	movs	r3, #6
	ldrsh	r2, [r6, r3]
	adds	r3, r7, #0
	bl	sub_0818caa8
.L_08141426:
	bl	sub_08014de4
	ldr	r5, [pc, #256]
	ldr	r4, [pc, #256]
	ldr	r0, [r6, #0]
	ldr	r1, [r6, #4]
	adds	r0, r0, r4
	adds	r1, r1, r5
	movs	r2, #0
	movs	r5, #128
	bl	sub_08015160
	lsls	r5, r5, #8
	movs	r1, #128
	lsls	r1, r1, #9
	adds	r2, r5, #0
	adds	r0, r5, #0
	bl	sub_080151e4
	movs	r0, #160
	lsls	r0, r0, #8
	bl	sub_080150e4
	movs	r3, #144
	lsls	r3, r3, #6
	adds	r3, #184
	movs	r1, #128
	adds	r0, r7, #0
	muls	r0, r3
	lsls	r1, r1, #10
	adds	r2, r5, #0
	bl	sub_080151e4
	ldr	r0, [pc, #200]
	mov	r1, sl
	movs	r2, #4
	bl	sub_08196958
	mov	r0, r8
	bl	sub_08196a7c
.L_08141478:
	mov	r0, r8
	bl	sub_08013164
	mov	r0, sl
	bl	sub_08013164
.L_08141484:
	ldr	r0, [sp, #116]
	movs	r1, #8
	cmp	r0, #130
	ble.n	.L_08141490
	movs	r1, #2
	b.n	.L_081414a2
.L_08141490:
	ldr	r2, [sp, #116]
	cmp	r2, #120
	ble.n	.L_0814149a
	movs	r1, #4
	b.n	.L_081414a2
.L_0814149a:
	ldr	r3, [sp, #116]
	cmp	r3, #110
	ble.n	.L_081414a2
	movs	r1, #6
.L_081414a2:
	adds	r0, r1, #0
	bl	sub_08158ce0
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r4, [sp, #32]
	ldr	r5, [sp, #28]
	ldr	r2, [sp, #116]
	movs	r0, #128
	lsls	r0, r0, #3
	adds	r4, #32
	adds	r5, r5, r0
	movs	r1, #1
	adds	r2, #1
	str	r4, [sp, #32]
	str	r5, [sp, #28]
	add	r9, r1
	str	r2, [sp, #116]
	cmp	r2, #140
	beq.n	.L_081414da
	b.n	.L_08141248
.L_081414da:
	bl	sub_08014c4c
	ldr	r0, [pc, #88]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #196
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xfffff600
	.4byte 0xfffec000
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x02020202
	.4byte 0x05000202
	.4byte 0x05000200
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199220
	.4byte 0xffe00000
	.4byte 0xfff80000
	.4byte 0xffc00000
	.4byte 0xffc10000
	.4byte 0x081976e0
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #184
	str	r0, [sp, #104]
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r0, [r2, #96]
	ldr	r3, [pc, #564]
	str	r0, [sp, #100]
	movs	r0, #0
	ldr	r1, [r2, #92]
	movs	r7, #239
	str	r1, [sp, #96]
	lsls	r7, r7, #7
	ldrh	r3, [r3, #4]
	str	r3, [sp, #80]
	adds	r3, r2, #0
	adds	r3, #176
	ldr	r3, [r3, #0]
	str	r3, [sp, #76]
	ldr	r3, [r2, #100]
	str	r3, [sp, #72]
	ldr	r2, [r2, #36]
	str	r2, [sp, #68]
	bl	sub_081435e0
	bl	sub_08179e6c
	ldr	r4, [sp, #96]
	movs	r3, #0
	adds	r2, r4, r7
	movs	r1, #200
	lsls	r1, r1, #4
	str	r3, [r2, #0]
	ldr	r0, [pc, #516]
	bl	sub_080145a8
	movs	r0, #80
	negs	r0, r0
	movs	r1, #0
	str	r0, [sp, #64]
	str	r1, [sp, #84]
.L_0814159a:
	ldr	r2, [sp, #84]
	cmp	r2, #27
	beq.n	.L_081415a2
	b.n	sub_081416ee
.L_081415a2:
	add	r0, sp, #156
	movs	r3, #255
	movs	r1, #0
	strh	r3, [r0, #0]
	bl	sub_08118010
	ldr	r3, [sp, #68]
	movs	r7, #238
	ldr	r0, [r3, #84]
	bl	sub_08014274
	bl	sub_08020380
	ldr	r2, [pc, #468]
	movs	r3, #240
	str	r3, [r2, #16]
	movs	r0, #1
	ldr	r1, [pc, #464]
	movs	r2, #0
	bl	sub_08118040
	ldr	r4, [sp, #96]
	lsls	r7, r7, #7
	movs	r1, #238
	adds	r7, #144
	lsls	r1, r1, #7
	adds	r3, r4, r7
	movs	r0, #0
	adds	r1, #148
	str	r0, [r3, #0]
	movs	r2, #1
	adds	r3, r4, r1
	str	r2, [r3, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #152
	adds	r2, r4, r3
	movs	r3, #1
	negs	r3, r3
	adds	r7, #12
	str	r3, [r2, #0]
	movs	r1, #144
	adds	r3, r4, r7
	str	r0, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #412]
	bl	sub_080145a8
	ldr	r1, [sp, #76]
	movs	r0, #1
	str	r0, [r1, #16]
	ldr	r0, [pc, #404]
	bl	sub_08013300
	movs	r2, #240
	adds	r5, r0, #0
	adds	r1, r5, #0
	lsls	r2, r2, #1
	ldr	r3, [pc, #396]
	ldr	r0, [pc, #396]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x24f0
	lsls	r4, r4, #1
	adds	r5, r5, r4
	adds	r0, r5, #0
	ldr	r1, [pc, #388]
	bl	sub_0801587c
	ldr	r0, [pc, #384]
	ldr	r1, [pc, #372]
	ldr	r2, [sp, #96]
	movs	r3, #238
	lsls	r3, r3, #7
	movs	r4, #13
	ldr	r6, [pc, #368]
	movs	r7, #0
	adds	r3, #220
	negs	r4, r4
	mov	r8, r7
	mov	fp, r0
	mov	r9, r1
	adds	r5, r2, r3
	mov	sl, r4
.L_0814164a:
	movs	r1, #32
	ldr	r2, [pc, #356]
	movs	r3, #0
	movs	r0, #32
	bl	sub_0815b290
	ldrb	r3, [r0, #9]
	mov	r7, sl
	ands	r3, r7
	movs	r7, #8
	orrs	r3, r7
	strb	r3, [r0, #9]
	ldrb	r3, [r0, #16]
	stmia	r5!, {r0}
	lsls	r3, r3, #2
	add	r3, fp
	ldrh	r0, [r3, #2]
	ldr	r1, [pc, #328]
	movs	r2, #128
	adds	r0, r0, r1
	lsls	r2, r2, #3
	adds	r1, r6, #0
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x2301
	movs	r2, #128
	add	r8, r3
	lsls	r2, r2, #3
	mov	r4, r8
	adds	r6, r6, r2
	cmp	r4, #31
	bne.n	.L_0814164a
	ldr	r0, [pc, #304]
	bl	sub_08013300
	movs	r2, #240
	adds	r5, r0, #0
	adds	r1, r5, #0
	lsls	r2, r2, #1
	ldr	r3, [pc, #264]
	ldr	r0, [pc, #268]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x24f0
	lsls	r4, r4, #1
	adds	r5, r5, r4
	ldr	r1, [pc, #260]
	adds	r0, r5, #0
	bl	sub_0801587c
	movs	r1, #32
	ldr	r2, [pc, #256]
	movs	r3, #0
	movs	r0, #32
	bl	sub_0815b290
	movs	r2, #240
	ldr	r1, [sp, #96]
	lsls	r2, r2, #7
	adds	r2, #88
	adds	r3, r1, r2
	str	r0, [r3, #0]
	movs	r4, #13
	ldrb	r3, [r0, #9]
	negs	r4, r4
	adds	r2, r4, #0
	ands	r3, r2
	orrs	r3, r7
	strb	r3, [r0, #9]
	ldrb	r3, [r0, #16]
	ldr	r2, [pc, #216]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r0, [r3, #2]
	ldr	r7, [pc, #216]
	movs	r2, #128
	adds	r0, r0, r7
	ldr	r1, [pc, #196]
	lsls	r2, r2, #3
	ldr	r3, [pc, #184]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c10
	movs	r6, #128
	adds	r4, #4
	lsls	r6, r6, #19
	lsls	r3, r4, #8
	adds	r6, #40
	str	r4, [sp, #64]
	str	r3, [r6, #0]
	ldr	r7, [sp, #96]
	movs	r0, #240
	lsls	r0, r0, #7
	adds	r0, #232
	adds	r3, r7, r0
	movs	r1, #1
	str	r1, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #84]
	adds	r2, #1
	str	r2, [sp, #84]
	cmp	r2, #52
	beq.n	.L_0814171e
	b.n	.L_0814159a
.L_0814171e:
	movs	r1, #240
	ldr	r0, [sp, #100]
	ldr	r5, [pc, #156]
	lsls	r1, r1, #6
	movs	r2, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x21f0
	lsls	r1, r1, #6
	movs	r2, #0
	ldr	r0, [pc, #144]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4823
	ldr	r1, [sp, #72]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	subs	r2, #2
	strh	r3, [r2, #0]
	movs	r3, #0
	str	r3, [r6, #0]
	movs	r4, #238
	str	r3, [sp, #60]
	str	r3, [sp, #56]
	str	r3, [sp, #52]
	str	r3, [sp, #48]
	movs	r3, #239
	lsls	r3, r3, #7
	lsls	r4, r4, #7
	adds	r2, r7, r3
	adds	r4, #132
	movs	r3, #2
	str	r3, [r2, #0]
	b.n	.L_081417cc
	.4byte 0x00007741
	.4byte 0x00000080
	.4byte 0x00001010
	.4byte 0x00003f44
	.4byte 0x03001120
	.4byte 0x08143001
	.4byte 0x030011e0
	.4byte 0x00000078
	.4byte 0x0813baed
	.4byte 0x000000a4
	.4byte 0x03000730
	.4byte 0x05000200
	.4byte 0x02010000
	.4byte 0x020036e0
	.4byte 0x80002000
	.4byte 0x06010000
	.4byte 0x000000a3
	.4byte 0x03000260
	.4byte 0x06004000
	.2byte 0x0134
	.2byte 0x0000
.L_081417cc:
	adds	r2, r7, r4
	movs	r3, #50
	str	r3, [r2, #0]
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	ldr	r0, [pc, #828]
	str	r3, [sp, #88]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #820]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2700
	str	r7, [sp, #84]
	ldr	r3, [pc, #812]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08141808
	b.n	.L_08141f7a
.L_08141808:
	mov	r0, sp
	adds	r0, #140
	str	r0, [sp, #20]
.L_0814180e:
	ldr	r1, [sp, #84]
	cmp	r1, #0
	bne.n	.L_08141856
	ldr	r0, [sp, #96]
	ldr	r2, [pc, #788]
	movs	r1, #238
	ldr	r7, [pc, #788]
	movs	r3, #212
	lsls	r1, r1, #7
	lsls	r3, r3, #16
	adds	r1, #180
	movs	r4, #128
	str	r2, [sp, #60]
	str	r3, [sp, #56]
	adds	r2, r0, r1
	lsls	r4, r4, #11
	movs	r3, #1
	str	r4, [sp, #52]
	str	r7, [sp, #48]
	str	r3, [r2, #0]
	movs	r2, #238
	ldr	r4, [sp, #84]
	lsls	r2, r2, #7
	adds	r2, #184
	movs	r7, #236
	adds	r3, r0, r2
	lsls	r7, r7, #1
	str	r4, [r3, #0]
	adds	r3, r0, r7
	str	r4, [r3, #0]
	ldr	r0, [pc, #744]
	ldr	r1, [pc, #744]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
.L_08141856:
	ldr	r0, [sp, #84]
	cmp	r0, #136
	bne.n	.L_081418a6
	ldr	r4, [pc, #732]
	ldr	r6, [pc, #708]
	ldr	r5, [pc, #732]
	movs	r1, #0
	movs	r7, #128
	mov	r8, r1
	lsls	r7, r7, #3
.L_0814186a:
	ldr	r3, [pc, #728]
	mov	r2, r8
	ldrb	r3, [r3, r2]
	movs	r0, #238
	ldr	r1, [sp, #96]
	lsls	r0, r0, #7
	adds	r0, #220
	lsls	r3, r3, #2
	adds	r3, r3, r0
	ldr	r3, [r1, r3]
	ldr	r2, [pc, #712]
	ldrb	r3, [r3, #16]
	adds	r1, r5, #0
	lsls	r3, r3, #2
	adds	r3, r3, r4
	ldrh	r0, [r3, #2]
	str	r4, [sp, #8]
	adds	r0, r0, r2
	adds	r2, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x2001
	movs	r3, #128
	add	r8, r0
	lsls	r3, r3, #3
	mov	r1, r8
	adds	r5, r5, r3
	ldr	r4, [sp, #8]
	cmp	r1, #7
	bne.n	.L_0814186a
.L_081418a6:
	ldr	r2, [sp, #84]
	cmp	r2, #48
	bne.n	.L_081418b2
	movs	r0, #139
	bl	sub_081c0010
.L_081418b2:
	ldr	r3, [sp, #84]
	cmp	r3, #136
	bne.n	.L_081418be
	movs	r0, #140
	bl	sub_081c0010
.L_081418be:
	ldr	r4, [sp, #84]
	cmp	r4, #172
	bne.n	.L_081418ca
	movs	r0, #144
	bl	sub_081c0010
.L_081418ca:
	movs	r7, #0
	mov	r8, r7
	ldr	r6, [pc, #636]
	ldr	r7, [pc, #636]
	ldr	r5, [pc, #640]
.L_081418d4:
	ldrb	r3, [r5, #0]
	ldr	r0, [sp, #84]
	adds	r5, #1
	cmp	r0, r3
	bne.n	sub_081418ea
	movs	r1, #240
	ldr	r2, [r6, #0]
	ldr	r0, [sp, #100]
	lsls	r1, r1, #6
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x2101
	add	r8, r1
	mov	r2, r8
	adds	r6, #4
	cmp	r2, #17
	bne.n	.L_081418d4
	ldr	r3, [pc, #608]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r3, [sp, #132]
	str	r4, [sp, #136]
	ldr	r4, [sp, #20]
	movs	r3, #0
	str	r3, [r4, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r4, #4]
	ldr	r7, [sp, #60]
	ldr	r1, [sp, #56]
	ldr	r0, [sp, #52]
	ldr	r2, [sp, #48]
	ldr	r3, [sp, #84]
	adds	r7, r7, r0
	adds	r1, r1, r2
	str	r7, [sp, #60]
	str	r1, [sp, #56]
	cmp	r3, #171
	ble.n	.L_0814195e
	cmp	r3, #183
	ble.n	.L_08141934
	ldr	r4, [pc, #564]
	movs	r7, #128
	lsls	r7, r7, #3
	adds	r0, r0, r4
	adds	r2, r2, r7
	str	r0, [sp, #52]
	str	r2, [sp, #48]
.L_08141934:
	ldr	r0, [sp, #84]
	cmp	r0, #205
	bgt.n	.L_081419a4
	ldr	r1, [sp, #52]
	movs	r2, #54
	adds	r3, r1, #0
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L_08141948
	adds	r3, #63
.L_08141948:
	ldr	r4, [sp, #48]
	asrs	r3, r3, #6
	str	r3, [sp, #52]
	adds	r3, r4, #0
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L_08141958
	adds	r3, #63
.L_08141958:
	asrs	r3, r3, #6
	str	r3, [sp, #48]
	b.n	.L_081419a4
.L_0814195e:
	ldr	r7, [sp, #48]
	cmp	r7, #0
	bge.n	.L_0814196e
	movs	r1, #128
	adds	r0, r7, #0
	lsls	r1, r1, #7
	adds	r0, r0, r1
	str	r0, [sp, #48]
.L_0814196e:
	ldr	r2, [sp, #52]
	cmp	r2, #0
	ble.n	.L_0814197a
	ldr	r3, [pc, #488]
	adds	r2, r2, r3
	str	r2, [sp, #52]
.L_0814197a:
	ldr	r4, [sp, #84]
	cmp	r4, #31
	ble.n	.L_081419a4
	adds	r0, r4, #0
	subs	r0, #32
	lsls	r0, r0, #9
	bl	sub_08002096
	ldr	r7, [sp, #56]
	lsls	r0, r0, #2
	asrs	r0, r0, #4
	adds	r7, r7, r0
	ldr	r0, [sp, #84]
	str	r7, [sp, #56]
	cmp	r0, #159
	bgt.n	.L_081419a4
	ldr	r1, [sp, #60]
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r1, r1, r2
	str	r1, [sp, #60]
.L_081419a4:
	ldr	r4, [sp, #96]
	movs	r7, #238
	lsls	r7, r7, #7
	ldr	r5, [sp, #20]
	movs	r3, #0
	adds	r7, #220
	mov	r8, r3
	adds	r6, r4, r7
.L_081419b4:
	mov	r0, r8
	cmp	r0, #25
	bne.n	.L_081419d4
	ldr	r1, [sp, #84]
	cmp	r1, #135
	ble.n	.L_081419d4
	ldr	r2, [sp, #60]
	movs	r4, #128
	lsls	r4, r4, #14
	adds	r3, r2, r4
	str	r3, [r5, #0]
	ldr	r7, [sp, #56]
	movs	r0, #128
	lsls	r0, r0, #15
	adds	r3, r7, r0
	b.n	.L_081419ec
.L_081419d4:
	ldr	r3, [pc, #396]
	mov	r1, r8
	ldrb	r3, [r3, r1]
	ldr	r2, [sp, #60]
	lsls	r3, r3, #16
	adds	r3, r2, r3
	str	r3, [r5, #0]
	ldr	r3, [pc, #388]
	ldr	r4, [sp, #56]
	ldrb	r3, [r3, r1]
	lsls	r3, r3, #16
	adds	r3, r4, r3
.L_081419ec:
	str	r3, [r5, #8]
	ldr	r7, [sp, #20]
	ldr	r0, [pc, #376]
	ldr	r3, [r7, #8]
	cmp	r3, r0
	bgt.n	.L_08141a04
	ldr	r0, [r6, #0]
	adds	r1, r7, #0
	add	r2, sp, #132
	movs	r3, #0
	bl	sub_08020010
.L_08141a04:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r6, #4
	cmp	r2, #32
	bne.n	.L_081419b4
	ldr	r3, [sp, #84]
	cmp	r3, #136
	bne.n	.L_08141a30
	ldr	r4, [sp, #96]
	movs	r7, #238
	lsls	r7, r7, #7
	movs	r0, #238
	adds	r7, #180
	lsls	r0, r0, #7
	adds	r2, r4, r7
	movs	r3, #24
	adds	r0, #184
	str	r3, [r2, #0]
	adds	r2, r4, r0
	movs	r3, #0
	str	r3, [r2, #0]
.L_08141a30:
	ldr	r1, [sp, #60]
	ldr	r2, [sp, #96]
	movs	r4, #236
	lsls	r4, r4, #1
	lsrs	r3, r1, #31
	adds	r5, r2, r4
	adds	r3, r1, r3
	ldr	r1, [r5, #0]
	asrs	r3, r3, #1
	lsls	r2, r1, #13
	movs	r7, #224
	subs	r3, r3, r2
	lsls	r7, r7, #13
	ldr	r0, [sp, #56]
	adds	r7, r3, r7
	ldr	r3, [sp, #84]
	lsls	r1, r1, #14
	movs	r2, #224
	adds	r1, r0, r1
	lsls	r2, r2, #14
	adds	r2, r1, r2
	subs	r3, #136
	str	r7, [sp, #44]
	str	r2, [sp, #40]
	str	r3, [sp, #36]
	cmp	r3, #84
	bls.n	.L_08141a68
	b.n	.L_08141ba2
.L_08141a68:
	ldr	r4, [sp, #84]
	cmp	r4, #136
	bne.n	.L_08141a98
	movs	r3, #4
	str	r3, [r5, #0]
	ldr	r7, [sp, #96]
	movs	r0, #224
	lsls	r0, r0, #1
	movs	r1, #228
	adds	r3, r7, r0
	movs	r2, #0
	lsls	r1, r1, #1
	str	r2, [r3, #0]
	adds	r3, r7, r1
	str	r2, [r3, #0]
	movs	r2, #224
	ldr	r1, [r5, #0]
	lsls	r2, r2, #3
	adds	r0, r7, r2
	movs	r2, #128
	lsls	r1, r1, #1
	lsls	r2, r2, #9
	bl	sub_0815b434
.L_08141a98:
	ldr	r3, [sp, #84]
	cmp	r3, #171
	ble.n	.L_08141aea
	cmp	r3, #172
	bne.n	.L_08141ac2
	movs	r3, #64
	str	r3, [r5, #0]
	ldr	r4, [sp, #96]
	movs	r7, #224
	lsls	r7, r7, #3
	movs	r2, #128
	adds	r0, r4, r7
	movs	r1, #128
	lsls	r2, r2, #9
	bl	sub_0815b434
	ldr	r1, [pc, #180]
	movs	r0, #192
	lsls	r0, r0, #10
	str	r0, [sp, #52]
	str	r1, [sp, #48]
.L_08141ac2:
	ldr	r2, [sp, #84]
	cmp	r2, #203
	ble.n	.L_08141b7c
	ldr	r3, [r5, #0]
	cmp	r3, #1
	ble.n	.L_08141b7c
	subs	r3, #4
	str	r3, [r5, #0]
	cmp	r3, #1
	bgt.n	.L_08141ada
	movs	r3, #2
	str	r3, [r5, #0]
.L_08141ada:
	ldr	r1, [r5, #0]
	ldr	r3, [sp, #96]
	movs	r4, #224
	lsls	r4, r4, #3
	movs	r2, #128
	lsls	r1, r1, #1
	adds	r0, r3, r4
	b.n	.L_08141b18
.L_08141aea:
	ldr	r7, [sp, #84]
	cmp	r7, #135
	ble.n	.L_08141b7c
	ldr	r0, [sp, #52]
	ldr	r2, [sp, #48]
	ldr	r1, [pc, #124]
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r0, r0, r1
	adds	r2, r2, r3
	str	r0, [sp, #52]
	str	r2, [sp, #48]
	ldr	r3, [r5, #0]
	adds	r1, r3, #1
	str	r1, [r5, #0]
	cmp	r1, #32
	bgt.n	.L_08141b78
	ldr	r4, [sp, #96]
	movs	r7, #224
	lsls	r7, r7, #3
	movs	r2, #128
	lsls	r1, r1, #1
	adds	r0, r4, r7
.L_08141b18:
	lsls	r2, r2, #9
	bl	sub_0815b434
	b.n	.L_08141b7c
	.4byte 0x00000130
	.4byte 0x03000730
	.4byte 0x03001150
	.4byte 0xffd00000
	.4byte 0xfff60000
	.4byte 0x000000b4
	.4byte 0x02014000
	.4byte 0x020036e0
	.4byte 0x02010400
	.4byte 0x08197748
	.4byte 0x06010000
	.4byte 0x08197760
	.4byte 0x03000260
	.4byte 0x0819774f
	.4byte 0x08196de0
	.4byte 0xfffffe00
	.4byte 0xffffe000
	.4byte 0x081977a4
	.4byte 0x081977c4
	.4byte 0x00c7ffff
	.4byte 0xfffd0000
	.2byte 0xf800
	.2byte 0xffff
.L_08141b78:
	.2byte 0x4640
	str	r0, [r5, #0]
.L_08141b7c:
	ldr	r1, [sp, #96]
	movs	r2, #236
	lsls	r2, r2, #1
	ldr	r4, [sp, #44]
	ldr	r7, [sp, #40]
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	asrs	r4, r4, #16
	asrs	r7, r7, #16
	movs	r2, #224
	lsls	r2, r2, #3
	mov	fp, r4
	mov	r9, r7
	adds	r0, r1, r2
	lsls	r3, r3, #1
	mov	r1, fp
	mov	r2, r9
	bl	sub_0818caa8
.L_08141ba2:
	ldr	r3, [sp, #84]
	cmp	r3, #47
	bgt.n	.L_08141baa
	b.n	.L_08141dfe
.L_08141baa:
	cmp	r3, #48
	bne.n	.L_08141c16
	ldr	r5, [sp, #96]
	movs	r4, #0
	mov	r8, r4
	movs	r6, #0
.L_08141bb6:
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	adds	r3, #128
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #1
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L_08141be2
	ldr	r3, [r5, #4]
	negs	r3, r3
	str	r3, [r5, #4]
.L_08141be2:
	movs	r7, #1
	add	r8, r7
	mov	r0, r8
	str	r6, [r5, #24]
	subs	r6, #8
	adds	r5, #28
	cmp	r0, #16
	bne.n	.L_08141bb6
	ldr	r2, [sp, #96]
	movs	r3, #156
	lsls	r3, r3, #6
	adds	r1, r2, r3
	ldr	r0, [pc, #616]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r4, [sp, #96]
	movs	r7, #142
	lsls	r7, r7, #7
	ldr	r0, [pc, #604]
	adds	r1, r4, r7
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
.L_08141c16:
	movs	r0, #32
	bl	sub_08014dac
	mov	r9, r0
	movs	r0, #1
	bl	sub_081969f8
	movs	r2, #0
	adds	r6, r0, #0
	str	r2, [r6, #20]
	ldr	r3, [sp, #124]
	ldr	r2, [pc, #572]
	ldr	r0, [sp, #96]
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #568]
	movs	r1, #142
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #2
	orrs	r3, r2
	lsls	r1, r1, #7
	str	r3, [sp, #124]
	add	r7, sp, #124
	adds	r3, r0, r1
	str	r3, [r7, #4]
	movs	r3, #6
	str	r3, [r6, #0]
	ldr	r3, [pc, #544]
	mov	r2, r9
	str	r3, [r6, #8]
	str	r7, [r6, #16]
	str	r2, [r6, #12]
	ldr	r4, [pc, #540]
	movs	r0, #128
	ldr	r5, [sp, #96]
	movs	r3, #0
	lsls	r0, r0, #9
	mov	r8, r3
	mov	fp, r4
	mov	sl, r0
.L_08141c6a:
	ldr	r3, [r5, #24]
	ldr	r2, [r5, #0]
	ldr	r1, [r5, #4]
	adds	r3, #1
	adds	r2, r2, r1
	str	r3, [r5, #24]
	str	r2, [r5, #0]
	cmp	r3, #47
	bhi.n	.L_08141ce6
	bl	sub_08014de4
	mov	r2, r8
	movs	r1, #3
	ldr	r4, [sp, #44]
	ands	r1, r2
	movs	r3, #128
	ldr	r2, [sp, #40]
	lsls	r3, r3, #10
	lsls	r0, r1, #17
	adds	r1, #2
	adds	r0, r0, r3
	lsls	r1, r1, #16
	adds	r0, r4, r0
	subs	r1, r2, r1
	add	r0, fp
	add	r1, fp
	movs	r2, #0
	bl	sub_08015160
	movs	r0, #128
	mov	r1, sl
	mov	r2, sl
	lsls	r0, r0, #8
	bl	sub_080151e4
	ldr	r0, [r5, #0]
	bl	sub_080150e4
	ldr	r2, [r5, #24]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #170
	adds	r0, r2, #0
	muls	r0, r3
	bl	sub_08002096
	movs	r3, #128
	lsls	r3, r3, #7
	movs	r1, #128
	adds	r0, r0, r3
	lsls	r1, r1, #10
	mov	r2, sl
	bl	sub_080151e4
	ldr	r0, [pc, #420]
	mov	r1, r9
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_08141ce6:
	movs	r4, #1
	add	r8, r4
	mov	r0, r8
	adds	r5, #28
	cmp	r0, #8
	bne.n	.L_08141c6a
	ldr	r1, [sp, #36]
	cmp	r1, #35
	bhi.n	.L_08141d80
	movs	r3, #184
	lsls	r3, r3, #5
	adds	r3, #112
	adds	r2, r1, #0
	muls	r2, r3
	movs	r3, #128
	lsls	r3, r3, #11
	subs	r5, r3, r2
	lsls	r3, r1, #2
	adds	r2, r3, #0
	movs	r3, #16
	subs	r2, #64
	negs	r3, r3
	cmp	r2, r3
	ble.n	.L_08141d1a
	movs	r2, #16
	negs	r2, r2
.L_08141d1a:
	movs	r3, #6
	str	r2, [r6, #20]
	add	r2, sp, #124
	strb	r3, [r7, #0]
	strb	r3, [r2, #1]
	movs	r3, #7
	ldr	r4, [pc, #344]
	str	r3, [r6, #0]
	ldr	r3, [pc, #344]
	mov	r7, r9
	str	r4, [r2, #4]
	str	r2, [r6, #16]
	str	r3, [r6, #8]
	str	r7, [r6, #12]
	bl	sub_08014de4
	ldr	r4, [pc, #332]
	ldr	r1, [sp, #44]
	ldr	r3, [sp, #40]
	ldr	r2, [pc, #328]
	adds	r0, r1, r2
	adds	r1, r3, r4
	movs	r2, #0
	bl	sub_08015160
	adds	r1, r5, #0
	adds	r2, r5, #0
	asrs	r0, r5, #1
	bl	sub_080151e4
	movs	r0, #128
	lsls	r0, r0, #6
	bl	sub_08015024
	movs	r0, #160
	lsls	r0, r0, #6
	bl	sub_080150e4
	ldr	r7, [sp, #84]
	negs	r0, r7
	lsls	r0, r0, #10
	bl	sub_08015068
	ldr	r0, [pc, #284]
	mov	r1, r9
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_08141d80:
	ldr	r3, [sp, #84]
	subs	r3, #172
	cmp	r3, #48
	bhi.n	.L_08141df2
	bl	sub_08014de4
	ldr	r4, [pc, #248]
	ldr	r1, [sp, #44]
	ldr	r3, [sp, #40]
	ldr	r2, [pc, #248]
	movs	r5, #128
	adds	r0, r1, r2
	adds	r1, r3, r4
	movs	r2, #0
	bl	sub_08015160
	lsls	r5, r5, #9
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r1, r5, #0
	adds	r2, r5, #0
	bl	sub_080151e4
	movs	r0, #160
	lsls	r0, r0, #6
	bl	sub_080150e4
	ldr	r7, [sp, #84]
	cmp	r7, #203
	ble.n	.L_08141dd4
	adds	r3, r7, #0
	subs	r3, #204
	movs	r0, #128
	lsls	r3, r3, #15
	lsls	r0, r0, #12
	movs	r1, #128
	subs	r0, r0, r3
	lsls	r1, r1, #10
	adds	r2, r5, #0
	bl	sub_080151e4
	b.n	.L_08141de2
.L_08141dd4:
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #12
	lsls	r1, r1, #10
	adds	r2, r5, #0
	bl	sub_080151e4
.L_08141de2:
	ldr	r0, [pc, #176]
	mov	r1, r9
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_08141df2:
	adds	r0, r6, #0
	bl	sub_08013164
	mov	r0, r9
	bl	sub_08013164
.L_08141dfe:
	ldr	r3, [sp, #84]
	subs	r3, #148
	cmp	r3, #71
	bls.n	.L_08141e08
	b.n	.L_08141f48
.L_08141e08:
	ldr	r1, [sp, #44]
	ldr	r2, [sp, #40]
	movs	r0, #0
	asrs	r1, r1, #16
	asrs	r2, r2, #16
	mov	sl, r0
	mov	fp, r1
	mov	r9, r2
.L_08141e18:
	ldr	r7, [sp, #84]
	ldr	r4, [sp, #84]
	add	r7, sl
	movs	r3, #3
	ands	r7, r3
	cmp	r4, #171
	bgt.n	.L_08141ea0
	ldr	r0, [sp, #96]
	movs	r1, #236
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldr	r5, [r3, #0]
	bl	sub_08014878
	lsls	r5, r5, #1
	adds	r1, r5, #0
	bl	sub_0800206c
	ldr	r2, [pc, #88]
	asrs	r6, r5, #1
	ldrb	r3, [r2, r7]
	add	r0, fp
	lsrs	r3, r3, #1
	subs	r0, r0, r6
	subs	r0, r0, r3
	mov	r8, r0
	bl	sub_08014878
	adds	r1, r5, #0
	bl	sub_0800206c
	ldr	r3, [pc, #68]
	add	r0, r9
	ldrb	r3, [r3, r7]
	subs	r0, r0, r6
	lsrs	r3, r3, #1
	subs	r5, r0, r3
	b.n	.L_08141eea
	.4byte 0x00000192
	.4byte 0x000000c9
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199220
	.4byte 0xffc00000
	.4byte 0x081991b0
	.4byte 0x02014000
	.4byte 0x08199340
	.4byte 0xffbe0000
	.4byte 0xffc20000
	.4byte 0x08199210
	.4byte 0x081991c0
	.4byte 0x08197492
	.2byte 0x7498
	.2byte 0x0819
.L_08141ea0:
	bl	sub_08014878
	ldr	r4, [sp, #96]
	adds	r2, r0, #0
	movs	r0, #236
	movs	r3, #63
	lsls	r0, r0, #1
	ands	r2, r3
	adds	r3, r4, r0
	ldr	r6, [r3, #0]
	str	r2, [sp, #12]
	bl	sub_08014878
	ldr	r4, [pc, #400]
	ldr	r2, [sp, #12]
	ldrb	r3, [r4, r7]
	lsrs	r5, r2, #1
	mov	r1, fp
	lsrs	r3, r3, #1
	subs	r5, r1, r5
	adds	r1, r6, #0
	subs	r5, r5, r3
	bl	sub_0800206c
	lsrs	r3, r6, #31
	adds	r6, r6, r3
	ldr	r3, [pc, #376]
	ldr	r2, [sp, #12]
	ldrb	r3, [r3, r7]
	adds	r5, r5, r0
	asrs	r6, r6, #1
	subs	r5, r5, r6
	subs	r5, #8
	add	r2, r9
	lsrs	r3, r3, #1
	mov	r8, r5
	subs	r5, r2, r3
.L_08141eea:
	bl	sub_08014878
	ldr	r3, [pc, #356]
	movs	r1, #3
	ands	r0, r1
	ldrb	r2, [r3, r0]
	movs	r3, #3
	orrs	r3, r2
	movs	r2, #1
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r0, #188
	bl	sub_08196404
	ldr	r2, [pc, #332]
	lsls	r3, r7, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #96]
	ldr	r4, [pc, #312]
	movs	r3, #156
	adds	r1, r2, r1
	lsls	r3, r3, #6
	adds	r1, r1, r3
	ldrb	r3, [r4, r7]
	ldr	r0, [sp, #100]
	str	r3, [sp, #0]
	ldr	r3, [pc, #300]
	mov	r2, r8
	ldrb	r3, [r3, r7]
	movs	r7, #192
	lsls	r7, r7, #18
	str	r3, [sp, #4]
	adds	r7, #188
	ldr	r4, [r7, #0]
	adds	r3, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	cmp	r1, #4
	beq.n	.L_08141f48
	b.n	.L_08141e18
.L_08141f48:
	movs	r0, #188
	bl	sub_0801314c
	movs	r4, #240
	ldr	r3, [sp, #96]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r7, [sp, #84]
	adds	r7, #1
	str	r7, [sp, #84]
	cmp	r7, #220
	beq.n	.L_08141f7a
	ldr	r3, [pc, #236]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08141f7a
	b.n	.L_0814180e
.L_08141f7a:
	ldr	r0, [pc, #228]
	bl	sub_08014644
	add	r0, sp, #80
	ldr	r3, [pc, #224]
	ldrh	r0, [r0, #0]
	movs	r2, #0
	strh	r0, [r3, #4]
	ldr	r1, [sp, #76]
	mov	r8, r2
	str	r2, [r1, #16]
	ldr	r2, [sp, #96]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #220
	adds	r5, r2, r3
.L_08141f9a:
	ldmia	r5!, {r0}
	bl	sub_08020048
	movs	r4, #1
	add	r8, r4
	mov	r7, r8
	cmp	r7, #32
	bne.n	.L_08141f9a
	bl	sub_08020388
	movs	r0, #128
	bl	sub_080143e0
	ldr	r1, [sp, #68]
	movs	r4, #240
	str	r0, [r1, #84]
	ldr	r2, [sp, #96]
	lsls	r4, r4, #7
	adds	r4, #240
	adds	r3, r2, r4
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	ldr	r0, [sp, #104]
	movs	r7, #0
	ldr	r3, [r0, #20]
	mov	r8, r7
	cmp	r3, #0
	beq.n	.L_08142008
	movs	r6, #1
	negs	r6, r6
	movs	r5, #36
.L_08141fda:
	ldr	r1, [sp, #104]
	adds	r3, r6, #0
	ldrsh	r0, [r5, r1]
	movs	r1, #1
	adds	r2, r6, #0
	str	r7, [sp, #0]
	bl	sub_0814cd48
	ldr	r3, [sp, #104]
	movs	r1, #0
	ldrsh	r0, [r5, r3]
	adds	r2, r6, #0
	adds	r3, r6, #0
	str	r7, [sp, #0]
	bl	sub_0814cd48
	ldr	r1, [sp, #104]
	movs	r0, #1
	ldr	r3, [r1, #20]
	add	r8, r0
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_08141fda
.L_08142008:
	bl	sub_08014c4c
	ldr	r3, [pc, #56]
	ldr	r2, [pc, #84]
	movs	r6, #128
	lsls	r6, r6, #19
	strh	r3, [r6, #0]
	movs	r3, #32
	strh	r3, [r2, #6]
	ldr	r2, [sp, #68]
	movs	r4, #206
	lsls	r4, r4, #3
	adds	r3, r2, r4
	ldrh	r1, [r3, #0]
	movs	r0, #1
	movs	r2, #24
	bl	sub_08118040
	ldr	r5, [pc, #56]
	movs	r1, #128
	ldr	r0, [sp, #100]
	lsls	r1, r1, #7
	movs	r2, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x480c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0xe013
	.4byte 0x00000001
	.4byte 0x08197492
	.4byte 0x08197498
	.4byte 0x08197744
	.4byte 0x08197486
	.4byte 0x03001150
	.4byte 0x0813baed
	.4byte 0x03001120
	.4byte 0x03000260
	.2byte 0x4000
	.2byte 0x0600
	movs	r0, #1
	bl	sub_08013560
	movs	r7, #128
	movs	r0, #128
	lsls	r7, r7, #19
	lsls	r0, r0, #5
	adds	r7, #82
	adds	r0, #16
	mov	r8, r0
	mov	sl, r7
	mov	r1, r8
	mov	r2, sl
	strh	r1, [r2, #0]
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	ldr	r2, [pc, #48]
	strh	r3, [r6, #0]
	movs	r3, #120
	str	r3, [r2, #16]
	ldr	r3, [sp, #96]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #3
	str	r3, [r2, #0]
	ldr	r7, [sp, #96]
	movs	r0, #238
	ldr	r3, [pc, #28]
	lsls	r0, r0, #7
	adds	r0, #132
	adds	r2, r7, r0
	str	r3, [r2, #0]
	movs	r1, #128
	ldr	r0, [sp, #100]
	lsls	r1, r1, #7
	ldr	r2, [pc, #16]
	b.n	.L_081420d8
	.4byte 0x00003f46
	.4byte 0x00007741
	.4byte 0x030011e0
	.4byte 0x02020202
	.2byte 0x3f3f
	.2byte 0x3f3f
.L_081420d8:
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4b0e
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r1, [sp, #104]
	ldr	r2, [pc, #48]
	ldr	r3, [r1, #20]
	adds	r4, r1, #0
	lsls	r3, r3, #1
	adds	r0, r4, #0
	adds	r3, #36
	strh	r2, [r4, r3]
	adds	r0, #36
	movs	r1, #0
	bl	sub_08118010
	movs	r2, #253
	lsls	r2, r2, #6
	adds	r1, r7, r2
	movs	r3, #0
	ldr	r0, [pc, #24]
	movs	r2, #0
	bl	sub_08157cf4
	mov	r3, r8
	mov	r4, sl
	strh	r3, [r4, #0]
	movs	r7, #0
	b.n	.L_08142124
	.4byte 0x00000784
	.4byte 0x000000ff
	.2byte 0x00ca
	.2byte 0x0000
.L_08142124:
	movs	r1, #224
	mov	r8, r7
	lsls	r1, r1, #3
.L_0814212a:
	mov	r0, r8
	lsrs	r3, r0, #31
	add	r3, r8
	asrs	r2, r3, #1
	cmp	r2, #63
	ble.n	.L_08142138
	movs	r2, #63
.L_08142138:
	movs	r3, #0
	mov	sl, r3
	ldr	r3, [sp, #96]
	add	r3, r8
	adds	r3, r3, r1
.L_08142142:
	movs	r4, #1
	add	sl, r4
	mov	r7, sl
	strb	r2, [r3, #0]
	adds	r3, #120
	cmp	r7, #120
	bne.n	.L_08142142
	add	r8, r4
	mov	r0, r8
	cmp	r0, #120
	bne.n	.L_0814212a
	movs	r1, #0
	movs	r2, #128
	str	r1, [sp, #84]
	lsls	r2, r2, #8
	mov	r9, r1
	mov	sl, r2
.L_08142164:
	ldr	r3, [sp, #84]
	cmp	r3, #39
	ble.n	.L_08142182
	ldr	r4, [sp, #96]
	movs	r7, #239
	lsls	r7, r7, #7
	adds	r2, r4, r7
	movs	r3, #4
	movs	r0, #238
	str	r3, [r2, #0]
	lsls	r0, r0, #7
	ldr	r3, [pc, #380]
	adds	r0, #132
	adds	r2, r4, r0
	str	r3, [r2, #0]
.L_08142182:
	ldr	r1, [sp, #84]
	cmp	r1, #16
	bne.n	.L_0814218e
	movs	r0, #140
	bl	sub_081c0010
.L_0814218e:
	ldr	r2, [sp, #84]
	cmp	r2, #18
	bne.n	.L_081421e4
	ldr	r3, [sp, #96]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	movs	r3, #90
	str	r3, [r2, #0]
	movs	r0, #145
	bl	sub_081c0010
	movs	r1, #128
	ldr	r3, [pc, #336]
	ldr	r0, [sp, #100]
	lsls	r1, r1, #7
	ldr	r2, [pc, #332]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x981a
	movs	r7, #0
	ldr	r3, [r0, #20]
	mov	r8, r7
	cmp	r3, #0
	beq.n	.L_081421e4
	movs	r6, #4
	movs	r5, #36
.L_081421c6:
	ldr	r1, [sp, #104]
	mov	r3, r8
	ldrsh	r0, [r5, r1]
	movs	r1, #7
	movs	r2, #5
	str	r6, [sp, #0]
	bl	sub_0814cd48
	ldr	r4, [sp, #104]
	movs	r3, #1
	add	r8, r3
	ldr	r3, [r4, #20]
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_081421c6
.L_081421e4:
	ldr	r7, [sp, #84]
	cmp	r7, #109
	bgt.n	.L_081422e2
	movs	r0, #32
	bl	sub_08014dac
	mov	r8, r0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #264]
	ldr	r3, [sp, #116]
	adds	r7, r0, #0
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #256]
	ldr	r0, [sp, #96]
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #2
	movs	r1, #253
	orrs	r3, r2
	lsls	r1, r1, #6
	str	r3, [sp, #116]
	add	r2, sp, #116
	adds	r3, r0, r1
	str	r3, [r2, #4]
	ldr	r3, [pc, #236]
	mov	r4, r8
	str	r3, [r7, #8]
	mov	r3, r9
	str	r3, [r7, #20]
	movs	r3, #6
	str	r2, [r7, #16]
	str	r4, [r7, #12]
	str	r3, [r7, #0]
	ldr	r0, [sp, #84]
	cmp	r0, #15
	ble.n	.L_081422d6
	ldr	r1, [sp, #96]
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r2, #228
	adds	r6, r1, r2
	cmp	r0, #16
	bne.n	.L_0814224c
	movs	r3, #128
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	ldr	r3, [pc, #196]
	str	r3, [r6, #4]
.L_0814224c:
	ldr	r3, [sp, #84]
	cmp	r3, #19
	bgt.n	.L_08142264
	ldr	r3, [r6, #0]
	ldr	r4, [pc, #188]
	movs	r0, #128
	adds	r3, r3, r4
	str	r3, [r6, #0]
	ldr	r3, [r6, #4]
	lsls	r0, r0, #15
	adds	r3, r3, r0
	str	r3, [r6, #4]
.L_08142264:
	ldr	r1, [sp, #84]
	cmp	r1, #27
	bgt.n	.L_0814228e
	ldr	r2, [sp, #96]
	movs	r3, #255
	lsls	r3, r3, #6
	adds	r5, r2, r3
	movs	r2, #128
	adds	r0, r5, #0
	movs	r1, #96
	lsls	r2, r2, #9
	bl	sub_0815b434
	movs	r0, #6
	ldrsh	r2, [r6, r0]
	movs	r4, #2
	ldrsh	r1, [r6, r4]
	adds	r0, r5, #0
	movs	r3, #96
	bl	sub_0818caa8
.L_0814228e:
	bl	sub_08014de4
	ldr	r0, [r6, #0]
	ldr	r1, [pc, #128]
	ldr	r2, [pc, #132]
	adds	r0, r0, r1
	ldr	r1, [r6, #4]
	adds	r1, r1, r2
	movs	r2, #0
	bl	sub_08015160
	movs	r1, #128
	lsls	r1, r1, #9
	mov	r2, sl
	mov	r0, sl
	bl	sub_080151e4
	movs	r0, #152
	lsls	r0, r0, #8
	bl	sub_080150e4
	movs	r1, #167
	lsls	r1, r1, #10
	ldr	r0, [pc, #96]
	adds	r1, #64
	mov	r2, sl
	bl	sub_080151e4
	ldr	r0, [pc, #92]
	mov	r1, r8
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_081422d6:
	adds	r0, r7, #0
	bl	sub_08013164
	mov	r0, r8
	bl	sub_08013164
.L_081422e2:
	ldr	r3, [sp, #84]
	movs	r1, #8
	cmp	r3, #130
	ble.n	.L_081422ee
	movs	r1, #2
	b.n	.L_08142330
.L_081422ee:
	ldr	r4, [sp, #84]
	cmp	r4, #120
	ble.n	.L_08142328
	movs	r1, #4
	b.n	.L_08142330
	.4byte 0x04040404
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199220
	.4byte 0xffd80000
	.4byte 0xffe00000
	.4byte 0xffc10000
	.4byte 0xffc00000
	.4byte 0x000dc500
	.2byte 0x91c0
	.2byte 0x0819
.L_08142328:
	ldr	r7, [sp, #84]
	cmp	r7, #110
	ble.n	.L_08142330
	movs	r1, #6
.L_08142330:
	adds	r0, r1, #0
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r1, #240
	ldr	r0, [sp, #96]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r3, r0, r1
	movs	r5, #1
	str	r5, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #84]
	adds	r2, #1
	str	r2, [sp, #84]
	cmp	r2, #48
	beq.n	.L_0814235a
	b.n	.L_08142164
.L_0814235a:
	ldr	r4, [sp, #76]
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #32]
	ldr	r1, [pc, #940]
	str	r5, [r4, #16]
	movs	r0, #1
	movs	r2, #0
	bl	sub_08118040
	movs	r1, #31
	movs	r0, #31
	movs	r2, #31
	bl	sub_08164b2c
	ldr	r2, [pc, #920]
	movs	r3, #0
	strh	r3, [r2, #4]
	ldr	r2, [pc, #920]
	movs	r3, #240
	str	r3, [r2, #16]
	movs	r0, #1
	bl	sub_08013560
	ldr	r7, [sp, #96]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r7, r2
	ldr	r0, [pc, #904]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r3, #176
	lsls	r3, r3, #4
	adds	r1, r7, r3
	ldr	r0, [pc, #892]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r4, #253
	lsls	r4, r4, #6
	adds	r1, r7, r4
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #876]
	bl	sub_08157cf4
	ldr	r3, [pc, #872]
	ldr	r1, [sp, #32]
	movs	r7, #0
	movs	r2, #128
	mov	r8, r7
	lsls	r2, r2, #2
.L_081423c8:
	movs	r0, #1
	add	r8, r0
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_081423c8
	ldr	r1, [sp, #96]
	movs	r3, #239
	movs	r4, #238
	lsls	r3, r3, #7
	lsls	r4, r4, #7
	adds	r2, r1, r3
	adds	r4, #132
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r1, r4
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r2, [pc, #804]
	movs	r3, #32
	movs	r7, #0
	strh	r3, [r2, #6]
	str	r7, [sp, #84]
.L_081423f6:
	ldr	r3, [pc, #820]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08142410
	ldr	r0, [sp, #32]
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	bne.n	.L_08142410
	movs	r2, #8
	str	r2, [sp, #32]
.L_08142410:
	ldr	r3, [sp, #84]
	cmp	r3, #6
	bgt.n	.L_0814244a
	ldr	r4, [sp, #32]
	movs	r7, #1
	negs	r7, r7
	cmp	r4, r7
	bne.n	.L_0814244a
	lsls	r2, r3, #3
	movs	r3, #31
	subs	r1, r3, r2
	adds	r2, r1, #0
	subs	r3, #41
	adds	r0, r1, #0
	cmp	r2, r3
	bge.n	.L_08142434
	movs	r0, #10
	negs	r0, r0
.L_08142434:
	movs	r4, #15
	negs	r4, r4
	cmp	r2, r4
	bge.n	.L_08142440
	movs	r1, #15
	negs	r1, r1
.L_08142440:
	cmp	r2, #3
	bgt.n	.L_08142446
	movs	r2, #4
.L_08142446:
	bl	sub_08164b2c
.L_0814244a:
	ldr	r7, [sp, #84]
	cmp	r7, #17
	bgt.n	.L_08142456
	ldr	r0, [sp, #32]
	cmp	r0, #0
	ble.n	.L_0814246c
.L_08142456:
	movs	r1, #4
	movs	r0, #4
	movs	r2, #4
	bl	sub_08164abc
	ldr	r1, [sp, #32]
	subs	r1, #1
	str	r1, [sp, #32]
	cmp	r1, #0
	bne.n	.L_0814246c
	b.n	.L_08142864
.L_0814246c:
	ldr	r3, [sp, #84]
	ldr	r4, [sp, #84]
	movs	r7, #15
	movs	r2, #0
	lsls	r3, r3, #4
	lsls	r4, r4, #1
	mov	sl, r7
	movs	r7, #240
	mov	r8, r2
	mov	fp, r3
	mov	r9, r4
	lsls	r7, r7, #14
.L_08142484:
	movs	r3, #128
	mov	r2, fp
	lsls	r3, r3, #1
	adds	r3, #255
	add	r2, r8
	ands	r2, r3
	lsls	r6, r2, #3
	ldr	r0, [pc, #668]
	subs	r6, r6, r2
	lsls	r6, r6, #2
	adds	r6, r6, r0
	bl	sub_08014878
	adds	r2, r0, #0
	str	r2, [sp, #12]
	bl	sub_08014878
	ldr	r2, [sp, #12]
	mov	r1, sl
	adds	r5, r0, #0
	adds	r0, r2, #0
	ands	r5, r1
	bl	sub_08002096
	add	r5, r9
	adds	r3, r5, #0
	muls	r3, r0
	ldr	r2, [sp, #12]
	adds	r3, r3, r7
	adds	r0, r2, #0
	str	r3, [r6, #0]
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	adds	r3, r3, r7
	str	r3, [r6, #4]
	movs	r3, #0
	str	r3, [r6, #12]
	str	r3, [r6, #16]
	bl	sub_08014878
	mov	r2, sl
	movs	r3, #1
	ands	r0, r2
	add	r8, r3
	adds	r0, #16
	mov	r4, r8
	str	r0, [r6, #24]
	cmp	r4, #16
	bne.n	.L_08142484
	ldr	r6, [pc, #584]
	ldr	r5, [pc, #576]
	movs	r7, #0
	mov	r8, r7
.L_081424f2:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_08142534
	asrs	r0, r0, #3
	adds	r0, #1
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r2, [sp, #72]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #100]
	ldr	r4, [sp, #88]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #60
	ldr	r2, [pc, #524]
	bl	sub_08138086
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_08142534:
	movs	r7, #1
	movs	r0, #128
	add	r8, r7
	lsls	r0, r0, #2
	adds	r5, #28
	cmp	r8, r0
	bne.n	.L_081424f2
	movs	r0, #1
	bl	sub_081969f8
	ldr	r3, [sp, #84]
	mov	r2, sp
	movs	r1, #0
	adds	r2, #108
	mov	sl, r0
	str	r1, [sp, #24]
	str	r2, [sp, #16]
	cmp	r3, #31
	bgt.n	.L_081425e4
	movs	r6, #128
	lsls	r6, r6, #9
	cmp	r3, #15
	ble.n	.L_08142568
	lsls	r3, r3, #12
	subs	r3, r6, r3
	adds	r6, r3, r6
.L_08142568:
	ldr	r3, [sp, #108]
	ldr	r2, [pc, #464]
	ldr	r7, [sp, #96]
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #456]
	mov	r4, r8
	ands	r3, r2
	movs	r0, #253
	mov	r1, sp
	orrs	r3, r4
	lsls	r0, r0, #6
	str	r3, [sp, #108]
	adds	r1, #108
	adds	r3, r7, r0
	str	r1, [sp, #16]
	str	r3, [r1, #4]
	ldr	r3, [pc, #436]
	mov	r2, sl
	str	r3, [r2, #8]
	ldr	r3, [sp, #24]
	ldr	r4, [pc, #432]
	str	r3, [r2, #20]
	movs	r3, #6
	str	r4, [r2, #12]
	str	r3, [r2, #0]
	str	r1, [r2, #16]
	bl	sub_08014de4
	movs	r1, #128
	ldr	r0, [pc, #420]
	lsls	r1, r1, #13
	movs	r2, #0
	movs	r5, #128
	bl	sub_08015160
	lsls	r5, r5, #8
	movs	r1, #128
	lsls	r1, r1, #9
	adds	r2, r5, #0
	adds	r0, r5, #0
	bl	sub_080151e4
	movs	r0, #152
	lsls	r0, r0, #8
	bl	sub_080150e4
	movs	r1, #128
	adds	r0, r6, #0
	lsls	r1, r1, #10
	adds	r2, r5, #0
	bl	sub_080151e4
	ldr	r0, [pc, #376]
	ldr	r1, [pc, #368]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_081425e4:
	ldr	r7, [sp, #84]
	cmp	r7, #0
	bne.n	.L_0814267c
	movs	r0, #208
	bl	sub_081c0010
	movs	r3, #8
	ldr	r0, [pc, #352]
	movs	r1, #16
	movs	r2, #8
	str	r3, [sp, #0]
	bl	sub_0818de3c
	movs	r0, #0
	str	r0, [sp, #28]
	mov	r9, r0
	mov	fp, r0
.L_08142606:
	mov	r0, r9
	bl	sub_08002090
	negs	r0, r0
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #5
	asrs	r3, r3, #16
	mov	r0, r9
	mov	r8, r3
	bl	sub_08002096
	ldr	r6, [pc, #312]
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #4
	asrs	r2, r3, #16
	movs	r7, #0
	add	r6, fp
.L_0814262c:
	lsls	r5, r7, #12
	adds	r0, r5, #0
	str	r2, [sp, #12]
	bl	sub_08002096
	ldr	r2, [sp, #12]
	mov	r1, r8
	adds	r3, r2, #0
	muls	r3, r0
	asrs	r3, r3, #16
	strb	r3, [r6, #0]
	strb	r1, [r6, #1]
	adds	r0, r5, #0
	bl	sub_08002090
	ldr	r2, [sp, #12]
	adds	r7, #1
	adds	r3, r2, #0
	muls	r3, r0
	asrs	r3, r3, #16
	strb	r3, [r6, #2]
	adds	r6, #4
	cmp	r7, #16
	bne.n	.L_0814262c
	ldr	r4, [sp, #28]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r2, #136
	movs	r3, #64
	adds	r4, #1
	add	r9, r2
	add	fp, r3
	str	r4, [sp, #28]
	cmp	r4, #8
	bne.n	.L_08142606
	ldr	r0, [pc, #232]
	ldr	r1, [pc, #232]
	movs	r2, #32
	bl	sub_081885f0
.L_0814267c:
	ldr	r0, [sp, #16]
	add	r6, sp, #108
	movs	r7, #5
	strb	r7, [r0, #0]
	strb	r7, [r6, #1]
	ldr	r1, [sp, #96]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r3, r1, r2
	ldr	r0, [pc, #196]
	ldr	r1, [pc, #180]
	mov	r4, sl
	str	r3, [r6, #4]
	movs	r3, #6
	str	r3, [r4, #0]
	str	r0, [r4, #8]
	str	r1, [r4, #12]
	str	r6, [r4, #16]
	ldr	r2, [sp, #24]
	movs	r3, #3
	str	r3, [r4, #4]
	str	r2, [r4, #20]
	ldr	r4, [sp, #84]
	movs	r0, #128
	lsls	r3, r4, #9
	lsls	r0, r0, #8
	movs	r1, #128
	adds	r5, r3, r0
	lsls	r1, r1, #9
	cmp	r5, r1
	ble.n	.L_081426be
	movs	r5, #128
	lsls	r5, r5, #9
.L_081426be:
	bl	sub_08014de4
	movs	r1, #192
	lsls	r1, r1, #12
	movs	r2, #0
	ldr	r0, [pc, #128]
	bl	sub_08015160
	movs	r0, #128
	lsls	r0, r0, #6
	bl	sub_08015024
	ldr	r2, [sp, #84]
	negs	r0, r2
	lsls	r0, r0, #10
	bl	sub_08015068
	adds	r0, r5, #0
	bl	sub_0801521c
	ldr	r1, [pc, #96]
	movs	r2, #128
	ldr	r0, [pc, #108]
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
	movs	r2, #7
	strb	r7, [r6, #1]
	strb	r2, [r6, #0]
	ldr	r4, [sp, #96]
	ldr	r1, [pc, #96]
	movs	r7, #176
	lsls	r7, r7, #4
	mov	r0, sl
	adds	r3, r4, r7
	str	r3, [r6, #4]
	str	r6, [r0, #16]
	b.n	.L_08142764
	movs	r0, r0
	.4byte 0x00000075
	.4byte 0x03001120
	.4byte 0x030011e0
	.4byte 0x000000cd
	.4byte 0x000000c1
	.4byte 0x000000ca
	.4byte 0x02014018
	.4byte 0x03001150
	.4byte 0x02014000
	.4byte 0x08197410
	.4byte 0xffffc000
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199220
	.4byte 0x02010000
	.4byte 0xfffe0000
	.4byte 0x081991c0
	.4byte 0x02011000
	.4byte 0x02010800
	.4byte 0x02012000
	.2byte 0x2800
	.2byte 0x0201
.L_08142764:
	str	r2, [r0, #0]
	str	r1, [r0, #8]
	add	r7, sp, #24
	ldr	r2, [sp, #24]
	ldr	r3, [pc, #412]
	ldrb	r7, [r7, #0]
	str	r3, [r0, #12]
	str	r2, [r0, #4]
	strb	r2, [r0, #24]
	strb	r7, [r0, #25]
	ldr	r1, [sp, #84]
	movs	r0, #0
	lsls	r3, r1, #2
	adds	r6, r1, #0
	adds	r3, #96
	lsls	r1, r1, #10
	mov	r8, r0
	mov	fp, r3
	adds	r6, #24
	mov	r9, r1
.L_0814278c:
	cmp	r6, #63
	bhi.n	.L_08142826
	ldr	r3, [pc, #380]
	mov	r2, r8
	ldrb	r3, [r3, r2]
	movs	r4, #0
	muls	r3, r6
	lsls	r7, r3, #10
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r5, r7, r3
	str	r4, [sp, #24]
	cmp	r6, #15
	bgt.n	.L_081427ae
	mov	r0, fp
	subs	r0, #64
	str	r0, [sp, #24]
.L_081427ae:
	cmp	r6, #40
	ble.n	.L_081427ba
	movs	r3, #40
	subs	r3, r3, r6
	lsls	r3, r3, #2
	str	r3, [sp, #24]
.L_081427ba:
	ldr	r1, [sp, #24]
	mov	r2, sl
	str	r1, [r2, #20]
	bl	sub_08014de4
	movs	r1, #128
	ldr	r0, [pc, #332]
	lsls	r1, r1, #14
	movs	r2, #0
	bl	sub_08015160
	adds	r0, r5, #0
	cmp	r5, #0
	bge.n	.L_081427de
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r3, #3
	adds	r0, r7, r3
.L_081427de:
	asrs	r2, r0, #2
	adds	r0, r2, #0
	adds	r1, r2, #0
	bl	sub_080151e4
	movs	r0, #156
	lsls	r0, r0, #6
	adds	r0, #16
	bl	sub_08015024
	mov	r0, r9
	bl	sub_08015068
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r1, #192
	adds	r0, r2, #0
	lsls	r1, r1, #11
	bl	sub_080151e4
	ldr	r3, [pc, #272]
	mov	r4, r8
	ldrsb	r1, [r3, r4]
	movs	r0, #0
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	ldr	r0, [pc, #260]
	ldr	r1, [pc, #240]
	movs	r2, #64
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_08142826:
	movs	r1, #1
	movs	r7, #24
	movs	r0, #128
	add	r8, r1
	negs	r7, r7
	lsls	r0, r0, #6
	mov	r2, r8
	add	fp, r7
	subs	r6, #6
	add	r9, r0
	cmp	r2, #4
	bne.n	.L_0814278c
	mov	r0, sl
	bl	sub_08013164
	movs	r4, #240
	ldr	r3, [sp, #96]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r7, [sp, #84]
	adds	r7, #1
	str	r7, [sp, #84]
	cmp	r7, #26
	beq.n	.L_08142864
	b.n	.L_081423f6
.L_08142864:
	movs	r1, #240
	ldr	r5, [pc, #184]
	lsls	r1, r1, #6
	ldr	r2, [pc, #184]
	ldr	r0, [pc, #184]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0xa814
	ldr	r3, [pc, #180]
	ldrh	r0, [r0, #0]
	movs	r2, #0
	strh	r0, [r3, #4]
	ldr	r1, [sp, #76]
	movs	r3, #120
	str	r2, [r1, #16]
	ldr	r2, [pc, #172]
	movs	r4, #206
	str	r3, [r2, #16]
	ldr	r2, [sp, #68]
	lsls	r4, r4, #3
	adds	r3, r2, r4
	ldrh	r1, [r3, #0]
	movs	r2, #24
	movs	r0, #1
	bl	sub_08118040
	movs	r0, #1
	bl	sub_08013560
	ldr	r7, [sp, #96]
	movs	r0, #239
	lsls	r0, r0, #7
	adds	r2, r7, r0
	movs	r3, #3
	movs	r1, #238
	str	r3, [r2, #0]
	lsls	r1, r1, #7
	ldr	r3, [pc, #132]
	adds	r1, #132
	adds	r2, r7, r1
	str	r3, [r2, #0]
	bl	sub_0815b410
	movs	r1, #240
	ldr	r2, [pc, #100]
	ldr	r0, [sp, #100]
	lsls	r1, r1, #6
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2086
	bl	sub_081180e8
	movs	r2, #0
	movs	r3, #240
	lsls	r3, r3, #7
	str	r2, [sp, #84]
	adds	r3, #232
	adds	r5, r7, r3
	movs	r6, #1
.L_081428da:
	str	r6, [r5, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r4, [sp, #84]
	adds	r4, #1
	str	r4, [sp, #84]
	cmp	r4, #32
	bne.n	.L_081428da
	ldr	r0, [pc, #72]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #184
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x081977e4
	.4byte 0xfffe0000
	.4byte 0x081977e8
	.4byte 0x02012000
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x06004000
	.4byte 0x03001120
	.4byte 0x030011e0
	.4byte 0x02020202
	.2byte 0x3001
	.2byte 0x0814
	bx	lr
	movs	r0, r0
	bx	lr
	.2byte 0x0000
