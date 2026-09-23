.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0800206c, 0x0800206c
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_080132fc, 0x080132fc
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080144c0, 0x080144c0
	.set sub_08014878, 0x08014878
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014d00, 0x08014d00
	.set sub_08014de4, 0x08014de4
	.set sub_080156e8, 0x080156e8
	.set sub_08015778, 0x08015778
	.set sub_0801587c, 0x0801587c
	.set sub_08038000, 0x08038000
	.set sub_08038010, 0x08038010
	.set sub_08038018, 0x08038018
	.set sub_08038080, 0x08038080
	.set sub_080380b0, 0x080380b0
	.set sub_080ad1d8, 0x080ad1d8
	.set sub_080ad290, 0x080ad290
	.set sub_081b8020, 0x081b8020
	.set sub_081b80a8, 0x081b80a8
	.set sub_081b810c, 0x081b810c
	.set sub_081b8180, 0x081b8180
	.set sub_081b8274, 0x081b8274
	.set sub_081c0010, 0x081c0010
	.global Func_081b834c
	.thumb_func
Func_081b834c:
.L_081b834c:
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	movs	r2, #0
	mov	lr, r2
	ldr	r3, [r6, #4]
	ldr	r2, [r6, #0]
	sub	sp, #8
	cmp	r2, r3
	bne.n	.L_081b8370
	ldr	r3, [r6, #8]
	cmp	r2, r3
	bne.n	.L_081b8370
	ldr	r3, [r6, #12]
	cmp	r2, r3
	bne.n	.L_081b8370
	movs	r3, #5
	mov	lr, r3
	b.n	.L_081b83be
.L_081b8370:
	add	r2, sp, #8
	movs	r7, #0
	movs	r3, #0
	mov	ip, r2
.L_081b8378:
	adds	r5, r3, #1
	adds	r4, r5, #0
	cmp	r5, #4
	beq.n	.L_081b83a0
	lsls	r3, r3, #2
	ldr	r0, [r3, r6]
	mov	r2, ip
	lsls	r3, r5, #2
	adds	r1, r3, r6
	subs	r2, #8
.L_081b838c:
	ldmia	r1!, {r3}
	cmp	r0, r3
	bne.n	.L_081b839a
	stmia	r2!, {r0}
	movs	r3, #4
	add	ip, r3
	adds	r7, #1
.L_081b839a:
	adds	r4, #1
	cmp	r4, #4
	bne.n	.L_081b838c
.L_081b83a0:
	adds	r3, r5, #0
	cmp	r3, #3
	bne.n	.L_081b8378
	cmp	r7, #1
	bne.n	.L_081b83ae
	movs	r2, #2
	mov	lr, r2
.L_081b83ae:
	cmp	r7, #2
	bne.n	.L_081b83b6
	movs	r3, #3
	mov	lr, r3
.L_081b83b6:
	cmp	r7, #3
	bne.n	.L_081b83be
	movs	r2, #4
	mov	lr, r2
.L_081b83be:
	mov	r0, lr
	add	sp, #8
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #568]
	movs	r1, #128
	lsls	r1, r1, #2
	movs	r0, #96
	add	sp, r5
	bl	sub_08014cc0
	movs	r1, #246
	lsls	r1, r1, #7
	str	r0, [sp, #128]
	adds	r1, #124
	movs	r0, #92
	bl	sub_08014d00
	movs	r1, #192
	lsls	r1, r1, #3
	str	r0, [sp, #124]
	adds	r1, #20
	movs	r0, #180
	bl	sub_08014d00
	movs	r1, #76
	str	r0, [sp, #120]
	movs	r0, #48
	bl	sub_08014d00
	str	r0, [sp, #116]
	ldr	r0, [pc, #516]
	bl	sub_080132fc
	bl	sub_081b8020
	bl	sub_080144c0
	ldr	r2, [pc, #508]
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r6, [pc, #504]
	movs	r0, #0
	mov	fp, r0
	movs	r5, #0
	movs	r4, #0
.L_081b8426:
	movs	r1, #0
	adds	r0, r4, #0
.L_081b842a:
	mov	r3, fp
	adds	r2, r3, r6
	adds	r3, r1, r0
	strh	r3, [r2, #0]
	adds	r1, #1
	movs	r2, #2
	add	fp, r2
	cmp	r1, #32
	bne.n	.L_081b842a
	adds	r5, #1
	adds	r4, #30
	cmp	r5, #20
	bne.n	.L_081b8426
	ldr	r0, [pc, #468]
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	str	r0, [sp, #112]
	lsls	r3, r3, #19
	movs	r0, #160
	lsls	r2, r2, #24
	adds	r3, #212
	lsls	r0, r0, #19
	add	r1, sp, #244
	adds	r2, #128
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	lsls	r2, r2, #24
	ldr	r0, [sp, #112]
	adds	r2, #112
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [sp, #112]
	movs	r4, #224
	lsls	r4, r4, #1
	adds	r3, r3, r4
	adds	r0, r3, #0
	ldr	r1, [pc, #420]
	str	r3, [sp, #112]
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #408]
	ldr	r1, [pc, #408]
	ldr	r2, [pc, #412]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #408]
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	str	r0, [sp, #112]
	adds	r3, #212
	ldr	r1, [sp, #128]
	adds	r2, #48
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [sp, #112]
	ldr	r1, [pc, #368]
	adds	r5, #128
	adds	r0, r5, #0
	str	r5, [sp, #112]
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #352]
	ldr	r1, [pc, #364]
	ldr	r2, [pc, #368]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #364]
	bl	sub_08013300
	ldr	r1, [sp, #128]
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	str	r0, [sp, #112]
	adds	r3, #212
	adds	r1, #224
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r6, [sp, #112]
	ldr	r1, [pc, #312]
	adds	r6, #32
	adds	r0, r6, #0
	str	r6, [sp, #112]
	bl	sub_0801587c
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #288]
	ldr	r1, [pc, #316]
	adds	r2, #128
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #312]
	bl	sub_08013300
	ldr	r1, [sp, #128]
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	str	r0, [sp, #112]
	adds	r3, #212
	adds	r1, #192
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [sp, #112]
	ldr	r1, [pc, #248]
	adds	r0, #32
	str	r0, [sp, #112]
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #232]
	ldr	r1, [pc, #268]
	ldr	r2, [pc, #268]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #268]
	bl	sub_08013300
	ldr	r2, [sp, #128]
	movs	r4, #128
	lsls	r4, r4, #1
	adds	r1, r2, r4
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	str	r0, [sp, #112]
	adds	r3, #212
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [sp, #112]
	ldr	r1, [pc, #188]
	adds	r5, #32
	adds	r0, r5, #0
	str	r5, [sp, #112]
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #172]
	ldr	r1, [pc, #216]
	ldr	r2, [pc, #220]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #216]
	bl	sub_08013300
	ldr	r6, [sp, #128]
	movs	r2, #144
	lsls	r2, r2, #1
	adds	r1, r6, r2
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	str	r0, [sp, #112]
	adds	r3, #212
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [sp, #112]
	ldr	r1, [pc, #128]
	adds	r3, #32
	adds	r0, r3, #0
	str	r3, [sp, #112]
	bl	sub_0801587c
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #104]
	ldr	r1, [pc, #164]
	adds	r2, #96
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #160]
	bl	sub_08013300
	movs	r3, #128
	movs	r4, #160
	movs	r2, #132
	lsls	r4, r4, #1
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	str	r0, [sp, #112]
	adds	r3, #212
	adds	r1, r6, r4
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r5, #176
	lsls	r5, r5, #1
	adds	r4, r6, r5
	movs	r6, #0
	mov	fp, r6
	ldr	r0, [sp, #112]
	ldr	r6, [pc, #28]
	movs	r5, #31
.L_081b85ec:
	ldrh	r3, [r0, #0]
	lsrs	r2, r3, #10
	ands	r2, r6
	adds	r1, r2, #4
	adds	r2, r5, #0
	ands	r2, r3
	adds	r2, #4
	cmp	r2, #31
	ble.n	.L_081b8600
	movs	r2, #31
.L_081b8600:
	cmp	r1, #31
	ble.n	.L_081b8664
	movs	r1, #31
	b.n	.L_081b8664
	.4byte 0x0000001f
	.4byte 0xfffffd0c
	.4byte 0x0000000c
	.4byte 0x0300120c
	.4byte 0x06003000
	.4byte 0x00000085
	.4byte 0x02010000
	.4byte 0x06004000
	.4byte 0x84002580
	.4byte 0x00000087
	.4byte 0x06010000
	.4byte 0x840004c0
	.4byte 0x00000092
	.4byte 0x06011300
	.4byte 0x00000097
	.4byte 0x06011500
	.4byte 0x84000100
	.4byte 0x0000008f
	.4byte 0x06011900
	.4byte 0x84000280
	.4byte 0x00000091
	.4byte 0x06012300
	.2byte 0x008e
	.2byte 0x0000
.L_081b8664:
	lsls	r3, r2, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r1
	movs	r1, #1
	add	fp, r1
	mov	r2, fp
	strh	r3, [r4, #0]
	adds	r0, #2
	adds	r4, #2
	cmp	r2, #16
	bne.n	.L_081b85ec
	ldr	r4, [sp, #128]
	movs	r3, #128
	movs	r5, #176
	movs	r2, #132
	lsls	r5, r5, #1
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r4, r5
	ldr	r1, [pc, #140]
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r6, [sp, #112]
	ldr	r1, [pc, #132]
	adds	r6, #32
	adds	r0, r6, #0
	str	r6, [sp, #112]
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #116]
	ldr	r1, [pc, #116]
	ldr	r2, [pc, #120]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	subs	r5, #96
	ldr	r1, [pc, #116]
	ldr	r0, [sp, #128]
	movs	r2, #0
	adds	r3, r5, #0
	bl	sub_081b80a8
	movs	r1, #160
	lsls	r1, r1, #19
	movs	r2, #0
	adds	r3, r5, #0
	add	r0, sp, #244
	bl	sub_081b80a8
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #10
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #62
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	subs	r2, #74
	strh	r3, [r2, #0]
	ldr	r3, [pc, #36]
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #36]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	movs	r2, #0
	strh	r2, [r3, #4]
	strh	r2, [r3, #6]
	b.n	.L_081b8734
	movs	r0, r0
	.4byte 0x00000686
	.4byte 0x00003737
	.4byte 0x00002723
	.4byte 0x00003340
	.4byte 0x00003f44
	.4byte 0x00000810
	.4byte 0x05000360
	.4byte 0x02010000
	.4byte 0x06016600
	.4byte 0x84000140
	.4byte 0x05000200
	.2byte 0x1120
	.2byte 0x0300
.L_081b8734:
	movs	r3, #128
	ldr	r1, [pc, #56]
	lsls	r3, r3, #19
	adds	r3, #20
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r1, [r3, #0]
	ldr	r1, [pc, #40]
	ldr	r2, [pc, #40]
	adds	r3, #38
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	subs	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	movs	r3, #1
	movs	r0, #0
	negs	r3, r3
	movs	r1, #14
	movs	r2, #1
	str	r0, [sp, #104]
	str	r0, [sp, #100]
	str	r0, [sp, #96]
	b.n	.L_081b877c
	.4byte 0x0000ff60
	.4byte 0x000000f0
	.2byte 0x00a0
	.2byte 0x0000
.L_081b877c:
	str	r0, [sp, #88]
	str	r0, [sp, #84]
	str	r0, [sp, #80]
	str	r0, [sp, #76]
	str	r0, [sp, #72]
	str	r0, [sp, #68]
	str	r1, [sp, #64]
	str	r2, [sp, #60]
	str	r3, [sp, #56]
	str	r3, [sp, #52]
	ldr	r6, [sp, #124]
	movs	r4, #100
	mov	fp, r0
	movs	r7, #127
	mov	r8, r0
	mov	sl, r4
.L_081b879c:
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #64
	lsls	r0, r0, #16
	mov	r5, r8
	str	r5, [r6, #4]
	str	r0, [r6, #0]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #64
	lsls	r0, r0, #16
	str	r0, [r6, #8]
	mov	r0, fp
	lsls	r5, r0, #14
	adds	r0, r5, #0
	bl	sub_08002096
	mov	r3, sl
	muls	r3, r0
	mov	r1, r8
	str	r1, [r6, #4]
	str	r3, [r6, #0]
	adds	r0, r5, #0
	bl	sub_08002090
	mov	r3, sl
	muls	r3, r0
	str	r3, [r6, #8]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #64
	lsls	r0, r0, #14
	str	r0, [r6, #12]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r3, #32
	lsls	r3, r3, #12
	str	r3, [r6, #16]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #64
	lsls	r0, r0, #14
	str	r0, [r6, #20]
	bl	sub_08014878
	movs	r1, #6
	bl	sub_0800206c
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #10
	str	r3, [r6, #24]
	mov	r3, fp
	lsls	r2, r3, #2
	movs	r5, #1
	movs	r3, #150
	lsls	r3, r3, #1
	add	r1, sp, #228
	add	fp, r5
	str	r3, [r1, r2]
	mov	r4, r8
	add	r3, sp, #180
	mov	r0, fp
	str	r4, [r3, r2]
	adds	r6, #28
	cmp	r0, #4
	bne.n	.L_081b879c
	bl	sub_08038000
	movs	r1, #6
	str	r1, [sp, #0]
	mov	r8, r1
	movs	r2, #12
	movs	r1, #0
	movs	r3, #3
	movs	r0, #18
	bl	sub_08038010
	movs	r3, #128
	ldr	r2, [sp, #120]
	ldr	r5, [pc, #928]
	lsls	r3, r3, #3
	adds	r3, #196
	adds	r6, r2, r3
	adds	r1, r0, #0
	str	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #48
	movs	r3, #0
	bl	sub_08038080
	ldr	r4, [pc, #912]
	ldr	r2, [r6, #0]
	ldr	r0, [r4, #16]
	movs	r3, #0
	movs	r1, #6
	str	r3, [sp, #0]
	mov	sl, r4
	bl	sub_080380b0
	mov	r6, r8
	movs	r1, #3
	movs	r2, #12
	movs	r3, #3
	movs	r0, #18
	str	r6, [sp, #0]
	bl	sub_08038010
	adds	r1, r0, #0
	ldr	r0, [sp, #120]
	movs	r2, #153
	lsls	r2, r2, #3
	adds	r7, r0, r2
	subs	r5, #1
	adds	r0, r5, #0
	str	r1, [r7, #0]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	movs	r1, #16
	movs	r2, #8
	movs	r3, #4
	movs	r0, #22
	str	r6, [sp, #0]
	bl	sub_08038010
	movs	r4, #128
	ldr	r3, [sp, #120]
	ldr	r5, [pc, #840]
	lsls	r4, r4, #3
	adds	r4, #204
	adds	r6, r3, r4
	adds	r1, r0, #0
	str	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	adds	r5, #1
	bl	sub_08038080
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r3, #8
	movs	r2, #0
	bl	sub_08038080
	bl	sub_080ad290
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	str	r3, [sp, #92]
	mov	r5, sl
	ldr	r3, [r5, #16]
	ldr	r6, [sp, #92]
	cmp	r6, r3
	bls.n	.L_081b88e6
	str	r3, [sp, #92]
.L_081b88e6:
	movs	r3, #5
	ldr	r2, [r7, #0]
	ldr	r0, [sp, #92]
	str	r3, [sp, #0]
	movs	r1, #6
	movs	r3, #24
	bl	sub_080380b0
	movs	r0, #1
	bl	sub_081c0010
	movs	r0, #0
	str	r0, [sp, #108]
	ldr	r1, [sp, #116]
	adds	r1, #12
	str	r1, [sp, #36]
	bl	.L_081ba210
.L_081b890a:
	ldr	r1, [pc, #748]
	movs	r2, #2
	ldr	r3, [r1, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081b8932
	movs	r0, #113
	bl	sub_081c0010
	movs	r4, #128
	ldr	r2, [sp, #120]
	lsls	r4, r4, #3
	adds	r4, #204
	adds	r3, r2, r4
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	sub_08038018
	bl	.L_081ba282
.L_081b8932:
	ldr	r3, [r1, #12]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_081b8940
	bl	.L_081b9550
.L_081b8940:
	movs	r5, #151
	lsls	r5, r5, #1
	adds	r0, r5, #0
	bl	sub_081c0010
	movs	r0, #128
	ldr	r6, [sp, #120]
	lsls	r0, r0, #3
	adds	r0, #204
	adds	r3, r6, r0
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	sub_08038018
	adds	r0, r5, #0
	bl	sub_081c0010
	ldr	r6, [pc, #664]
	ldr	r5, [sp, #124]
	movs	r1, #0
	mov	fp, r1
	movs	r7, #127
.L_081b896c:
	movs	r3, #192
	movs	r2, #0
	lsls	r3, r3, #15
	str	r2, [r5, #8]
	str	r3, [r5, #4]
	str	r6, [r5, #0]
	mov	r8, r2
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #64
	lsls	r0, r0, #12
	str	r0, [r5, #12]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #48
	lsls	r3, r3, #13
	str	r3, [r5, #16]
	bl	sub_08014878
	ands	r0, r7
	adds	r0, #64
	lsls	r0, r0, #13
	str	r0, [r5, #20]
	bl	sub_08014878
	movs	r1, #144
	lsls	r1, r1, #7
	bl	sub_0800206c
	mov	r3, fp
	str	r0, [r5, #24]
	lsls	r2, r3, #2
	movs	r0, #1
	movs	r3, #150
	add	r1, sp, #228
	lsls	r3, r3, #1
	movs	r4, #192
	add	fp, r0
	str	r3, [r1, r2]
	lsls	r4, r4, #13
	mov	r1, fp
	adds	r6, r6, r4
	adds	r5, #28
	cmp	r1, #4
	bne.n	.L_081b896c
	bl	sub_080ad290
	lsls	r3, r0, #2
	adds	r3, r3, r0
	ldr	r7, [pc, #536]
	lsls	r3, r3, #1
	str	r3, [sp, #92]
	ldr	r3, [r7, #16]
	ldr	r2, [sp, #92]
	cmp	r2, r3
	bls.n	.L_081b89e4
	str	r3, [sp, #92]
.L_081b89e4:
	ldr	r3, [sp, #120]
	movs	r4, #153
	lsls	r4, r4, #3
	adds	r6, r3, r4
	ldr	r0, [sp, #92]
	ldr	r2, [r6, #0]
	movs	r3, #24
	movs	r5, #5
	movs	r1, #6
	str	r5, [sp, #0]
	bl	sub_080380b0
	ldr	r1, [sp, #92]
	negs	r0, r1
	bl	sub_080ad1d8
	movs	r4, #128
	ldr	r2, [sp, #120]
	lsls	r4, r4, #3
	adds	r4, #196
	adds	r3, r2, r4
	mov	r1, r8
	ldr	r2, [r3, #0]
	ldr	r0, [r7, #16]
	movs	r3, #0
	str	r1, [sp, #0]
	movs	r1, #6
	bl	sub_080380b0
	movs	r3, #0
	movs	r2, #2
	str	r2, [sp, #96]
	str	r3, [sp, #84]
	str	r3, [sp, #80]
	bl	sub_080ad290
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	str	r3, [sp, #92]
	adds	r0, r3, #0
	ldr	r2, [r6, #0]
	movs	r1, #6
	movs	r3, #24
	str	r5, [sp, #0]
	bl	sub_080380b0
	bl	.L_081b9550
.L_081b8a46:
	ldr	r4, [sp, #96]
	cmp	r4, #1
	beq.n	.L_081b8a4e
	b.n	.L_081b8d0e
.L_081b8a4e:
	ldr	r5, [sp, #80]
	cmp	r5, #7
	bgt.n	.L_081b8a5a
	adds	r5, #1
	str	r5, [sp, #80]
	b.n	.L_081b8c70
.L_081b8a5a:
	ldr	r6, [sp, #84]
	cmp	r6, #0
	ble.n	.L_081b8ae0
	adds	r6, #1
	str	r6, [sp, #84]
	cmp	r6, #16
	beq.n	.L_081b8a6a
	b.n	.L_081b8c70
.L_081b8a6a:
	movs	r0, #151
	lsls	r0, r0, #1
	bl	sub_081c0010
	ldr	r6, [pc, #392]
	ldr	r5, [sp, #124]
	movs	r0, #0
	mov	fp, r0
	movs	r7, #127
.L_081b8a7c:
	movs	r3, #0
	str	r3, [r5, #8]
	movs	r3, #192
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	str	r6, [r5, #0]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #64
	lsls	r0, r0, #12
	str	r0, [r5, #12]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #48
	lsls	r3, r3, #13
	str	r3, [r5, #16]
	bl	sub_08014878
	ands	r0, r7
	adds	r0, #64
	lsls	r0, r0, #13
	str	r0, [r5, #20]
	bl	sub_08014878
	movs	r1, #144
	lsls	r1, r1, #7
	bl	sub_0800206c
	mov	r3, fp
	str	r0, [r5, #24]
	lsls	r2, r3, #2
	movs	r0, #1
	movs	r3, #150
	add	r1, sp, #228
	lsls	r3, r3, #1
	movs	r4, #192
	add	fp, r0
	str	r3, [r1, r2]
	lsls	r4, r4, #13
	mov	r1, fp
	adds	r6, r6, r4
	adds	r5, #28
	cmp	r1, #4
	bne.n	.L_081b8a7c
	movs	r2, #2
	str	r2, [sp, #96]
	b.n	.L_081b8c70
.L_081b8ae0:
	ldr	r1, [pc, #276]
	movs	r2, #2
	ldr	r3, [r1, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081b8b46
	ldr	r3, [sp, #88]
	cmp	r3, #0
	ble.n	.L_081b8afa
	movs	r0, #114
	bl	sub_081c0010
	b.n	.L_081b8c70
.L_081b8afa:
	ldr	r4, [sp, #120]
	movs	r5, #128
	lsls	r5, r5, #3
	adds	r5, #204
	adds	r6, r4, r5
	ldr	r0, [r6, #0]
	movs	r1, #1
	bl	sub_08038018
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #16
	movs	r2, #8
	movs	r3, #4
	movs	r0, #22
	bl	sub_08038010
	ldr	r5, [pc, #212]
	adds	r1, r0, #0
	str	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	adds	r5, #1
	bl	sub_08038080
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #8
	bl	sub_08038080
	movs	r0, #113
	bl	sub_081c0010
	movs	r6, #0
	str	r6, [sp, #96]
	b.n	.L_081b8c70
.L_081b8b46:
	ldr	r3, [r1, #12]
	ldr	r0, [sp, #96]
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_081b8bc6
	movs	r0, #112
	bl	sub_081c0010
	movs	r1, #1
	ldr	r2, [sp, #120]
	movs	r4, #128
	str	r1, [sp, #84]
	lsls	r4, r4, #3
	adds	r4, #204
	adds	r3, r2, r4
	ldr	r0, [r3, #0]
	bl	sub_08038018
	ldr	r5, [sp, #88]
	cmp	r5, #0
	beq.n	.L_081b8b72
	b.n	.L_081b8c70
.L_081b8b72:
	bl	sub_080ad290
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	str	r3, [sp, #92]
	ldr	r5, [pc, #112]
	ldr	r6, [sp, #92]
	ldr	r3, [r5, #16]
	cmp	r6, r3
	bls.n	.L_081b8b8a
	str	r3, [sp, #92]
.L_081b8b8a:
	ldr	r0, [sp, #120]
	movs	r1, #153
	lsls	r1, r1, #3
	adds	r3, r0, r1
	ldr	r2, [r3, #0]
	movs	r3, #5
	str	r3, [sp, #0]
	ldr	r0, [sp, #92]
	movs	r1, #6
	movs	r3, #24
	bl	sub_080380b0
	ldr	r2, [sp, #92]
	negs	r0, r2
	bl	sub_080ad1d8
	ldr	r0, [r5, #16]
	ldr	r4, [sp, #120]
	movs	r5, #128
	lsls	r5, r5, #3
	adds	r5, #196
	ldr	r6, [sp, #88]
	adds	r3, r4, r5
	ldr	r2, [r3, #0]
	movs	r1, #6
	movs	r3, #0
	str	r6, [sp, #0]
	bl	sub_080380b0
	b.n	.L_081b8c70
.L_081b8bc6:
	ldr	r3, [r1, #12]
	movs	r2, #64
	movs	r6, #1
	ands	r3, r2
	negs	r6, r6
	cmp	r3, #0
	beq.n	.L_081b8c12
	ldr	r0, [sp, #72]
	cmp	r0, #1
	bne.n	.L_081b8c12
	ldr	r1, [sp, #64]
	cmp	r1, #24
	beq.n	.L_081b8c28
	ldr	r2, [sp, #88]
	cmp	r2, #0
	bne.n	.L_081b8c04
	ldr	r6, [pc, #24]
	b.n	.L_081b8c06
	movs	r0, r0
	.4byte 0x00000d69
	.4byte 0x02000240
	.4byte 0x00000d77
	.4byte 0x03001150
	.4byte 0xff940000
	.2byte 0x0d79
	.2byte 0x0000
.L_081b8c04:
	ldr	r6, [pc, #776]
.L_081b8c06:
	movs	r0, #111
	bl	sub_081c0010
	movs	r3, #0
	str	r3, [sp, #72]
	b.n	.L_081b8c46
.L_081b8c12:
	ldr	r3, [r1, #12]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081b8c46
	ldr	r4, [sp, #72]
	cmp	r4, #0
	bne.n	.L_081b8c46
	ldr	r5, [sp, #64]
	cmp	r5, #4
	bne.n	.L_081b8c30
.L_081b8c28:
	movs	r0, #113
	bl	sub_081c0010
	b.n	.L_081b8c46
.L_081b8c30:
	ldr	r6, [sp, #88]
	cmp	r6, #0
	bne.n	.L_081b8c3a
	ldr	r6, [pc, #732]
	b.n	.L_081b8c3c
.L_081b8c3a:
	ldr	r6, [pc, #732]
.L_081b8c3c:
	movs	r0, #111
	bl	sub_081c0010
	movs	r0, #1
	str	r0, [sp, #72]
.L_081b8c46:
	movs	r1, #1
	negs	r1, r1
	cmp	r6, r1
	beq.n	.L_081b8c70
	ldr	r2, [sp, #120]
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #204
	adds	r5, r2, r3
	ldr	r1, [r5, #0]
	adds	r0, r6, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	ldr	r0, [pc, #692]
	ldr	r1, [r5, #0]
	movs	r2, #0
	movs	r3, #8
	bl	sub_08038080
.L_081b8c70:
	ldr	r4, [sp, #72]
	cmp	r4, #0
	bne.n	.L_081b8cc2
	ldr	r5, [sp, #128]
	ldr	r2, [sp, #84]
	movs	r6, #192
	lsls	r6, r6, #1
	adds	r0, r5, r6
	lsls	r6, r2, #12
	ldr	r1, [pc, #668]
	movs	r2, #16
	adds	r3, r6, #0
	bl	sub_081b810c
	movs	r3, #208
	lsls	r3, r3, #1
	adds	r0, r5, r3
	movs	r5, #128
	lsls	r5, r5, #8
	ldr	r1, [pc, #652]
	adds	r2, r5, #0
	movs	r3, #16
	bl	sub_081b80a8
	ldr	r4, [sp, #128]
	movs	r1, #224
	lsls	r1, r1, #1
	adds	r0, r4, r1
	movs	r2, #16
	ldr	r1, [pc, #636]
	adds	r3, r6, #0
	bl	sub_081b810c
	ldr	r2, [sp, #128]
	movs	r3, #240
	lsls	r3, r3, #1
	adds	r0, r2, r3
	ldr	r1, [pc, #624]
	adds	r2, r5, #0
	bl	.L_081b94fa
.L_081b8cc2:
	ldr	r2, [sp, #84]
	ldr	r4, [sp, #128]
	movs	r5, #208
	lsls	r6, r2, #12
	lsls	r5, r5, #1
	adds	r0, r4, r5
	ldr	r1, [pc, #596]
	movs	r2, #16
	adds	r3, r6, #0
	bl	sub_081b810c
	ldr	r3, [sp, #128]
	movs	r4, #192
	movs	r5, #128
	lsls	r4, r4, #1
	lsls	r5, r5, #8
	adds	r0, r3, r4
	ldr	r1, [pc, #568]
	adds	r2, r5, #0
	movs	r3, #16
	bl	sub_081b80a8
	ldr	r1, [sp, #128]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r0, r1, r2
	adds	r3, r6, #0
	ldr	r1, [pc, #560]
	movs	r2, #16
	bl	sub_081b810c
	ldr	r3, [sp, #128]
	movs	r4, #224
	lsls	r4, r4, #1
	adds	r0, r3, r4
	ldr	r1, [pc, #540]
	adds	r2, r5, #0
	b.n	.L_081b94fa
.L_081b8d0e:
	ldr	r5, [sp, #96]
	cmp	r5, #2
	beq.n	.L_081b8d16
	b.n	.L_081b9002
.L_081b8d16:
	movs	r6, #0
	str	r6, [sp, #44]
	str	r6, [sp, #20]
	ldr	r7, [sp, #124]
	mov	fp, r6
.L_081b8d20:
	ldr	r1, [r7, #0]
	ldr	r3, [r7, #12]
	ldr	r2, [r7, #16]
	adds	r1, r1, r3
	ldr	r3, [r7, #4]
	str	r1, [r7, #0]
	adds	r3, r3, r2
	str	r3, [r7, #4]
	ldr	r2, [r7, #8]
	ldr	r3, [r7, #20]
	movs	r0, #0
	adds	r2, r2, r3
	str	r2, [r7, #8]
	adds	r5, r2, #0
	ldr	r2, [sp, #124]
	mov	r9, r0
	mov	sl, r2
.L_081b8d42:
	cmp	fp, r9
	beq.n	.L_081b8db8
	mov	r4, sl
	ldr	r3, [r4, #0]
	ldr	r2, [r7, #4]
	subs	r3, r1, r3
	asrs	r6, r3, #16
	ldr	r3, [r4, #4]
	mov	r0, sl
	subs	r2, r2, r3
	ldr	r3, [r0, #8]
	asrs	r4, r2, #16
	subs	r3, r5, r3
	asrs	r3, r3, #16
	mov	r8, r3
	adds	r2, r6, #0
	muls	r2, r6
	adds	r3, r4, #0
	muls	r3, r4
	mov	r0, r8
	adds	r2, r2, r3
	mov	r3, r8
	muls	r3, r0
	adds	r0, r2, r3
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #35
	cmp	r0, r2
	bgt.n	.L_081b8db8
	str	r4, [sp, #12]
	ldr	r3, [pc, #432]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c05
	adds	r1, r5, #0
	lsls	r0, r6, #15
	bl	sub_08002054
	ldr	r3, [r7, #12]
	ldr	r4, [sp, #12]
	adds	r3, r3, r0
	adds	r1, r5, #0
	str	r3, [r7, #12]
	lsls	r0, r4, #15
	bl	sub_08002054
	ldr	r3, [r7, #16]
	adds	r1, r5, #0
	adds	r3, r3, r0
	str	r3, [r7, #16]
	mov	r3, r8
	lsls	r0, r3, #15
	bl	sub_08002054
	ldr	r3, [r7, #20]
	ldr	r1, [r7, #0]
	adds	r3, r3, r0
	str	r3, [r7, #20]
	ldr	r5, [r7, #8]
.L_081b8db8:
	movs	r6, #1
	add	r9, r6
	movs	r4, #28
	mov	r0, r9
	add	sl, r4
	cmp	r0, #4
	bne.n	.L_081b8d42
	asrs	r3, r1, #16
	asrs	r2, r5, #16
	adds	r0, r3, #0
	muls	r0, r3
	adds	r3, r2, #0
	muls	r3, r2
	adds	r0, r0, r3
	ldr	r3, [pc, #344]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x28c7
	ble.n	.L_081b8e12
	ldr	r3, [r7, #0]
	cmp	r3, #0
	ble.n	.L_081b8dee
	ldr	r3, [r7, #12]
	cmp	r3, #0
	bge.n	.L_081b8df4
	negs	r3, r3
	b.n	.L_081b8df4
.L_081b8dee:
	ldr	r3, [r7, #12]
	cmp	r3, #0
	bge.n	.L_081b8df6
.L_081b8df4:
	negs	r3, r3
.L_081b8df6:
	str	r3, [r7, #12]
	ldr	r3, [r7, #8]
	cmp	r3, #0
	ble.n	.L_081b8e08
	ldr	r3, [r7, #20]
	cmp	r3, #0
	bge.n	.L_081b8e0e
	negs	r3, r3
	b.n	.L_081b8e0e
.L_081b8e08:
	ldr	r3, [r7, #20]
	cmp	r3, #0
	bge.n	.L_081b8e10
.L_081b8e0e:
	negs	r3, r3
.L_081b8e10:
	str	r3, [r7, #20]
.L_081b8e12:
	ldr	r3, [r7, #16]
	ldr	r1, [pc, #284]
	adds	r2, r3, r1
	ldr	r3, [r7, #4]
	str	r2, [r7, #16]
	cmp	r3, #0
	ble.n	.L_081b8e22
	b.n	.L_081b8f3c
.L_081b8e22:
	negs	r2, r2
	movs	r3, #0
	str	r3, [r7, #4]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r2, r3, #4
	cmp	r2, #0
	bge.n	.L_081b8e34
	adds	r2, #63
.L_081b8e34:
	asrs	r3, r2, #6
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #3
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L_081b8e96
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #20]
	asrs	r2, r2, #8
	asrs	r3, r3, #8
	adds	r0, r2, #0
	muls	r0, r2
	adds	r2, r3, #0
	muls	r2, r3
	adds	r3, r2, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #212]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x00c3
	subs	r3, r3, r0
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_081b8e6a
	adds	r3, #63
.L_081b8e6a:
	asrs	r6, r3, #6
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #8
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #8
	b.n	.L_081b8eb8
.L_081b8e96:
	ldr	r2, [r7, #12]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_081b8ea4
	adds	r3, #63
.L_081b8ea4:
	ldr	r2, [r7, #20]
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_081b8eb6
	adds	r3, #63
.L_081b8eb6:
	asrs	r3, r3, #6
.L_081b8eb8:
	str	r3, [r7, #20]
	ldr	r6, [sp, #20]
	add	r2, sp, #228
	ldr	r3, [r2, r6]
	cmp	r3, #19
	ble.n	.L_081b8ec8
	subs	r3, #20
	str	r3, [r2, r6]
.L_081b8ec8:
	ldr	r3, [r7, #16]
	ldr	r4, [pc, #108]
	cmp	r3, r4
	bgt.n	.L_081b8f40
	movs	r3, #0
	str	r3, [r7, #12]
	str	r3, [r7, #16]
	str	r3, [r7, #20]
	ldr	r5, [sp, #20]
	str	r3, [r2, r5]
	ldr	r5, [r7, #24]
	adds	r0, r5, #0
	cmp	r5, #0
	bge.n	.L_081b8eec
	movs	r1, #192
	lsls	r1, r1, #2
	adds	r1, #255
	adds	r0, r5, r1
.L_081b8eec:
	asrs	r0, r0, #10
	movs	r1, #3
	bl	sub_08002064
	cmp	r0, #1
	bne.n	.L_081b8f02
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r3, r5, r2
	str	r3, [r7, #24]
	b.n	.L_081b8f40
.L_081b8f02:
	cmp	r0, #2
	bne.n	.L_081b8f40
	movs	r4, #128
	lsls	r4, r4, #3
	adds	r3, r5, r4
	str	r3, [r7, #24]
	b.n	.L_081b8f40
	.4byte 0x00000d7b
	.4byte 0x00000d7a
	.4byte 0x00000d7c
	.4byte 0x00000d7d
	.4byte 0x05000380
	.4byte 0x050003a0
	.4byte 0x050003c0
	.4byte 0x050003e0
	.4byte 0x030002d4
	.4byte 0xffff8000
	.2byte 0xffff
	.2byte 0x0002
.L_081b8f3c:
	mov	r5, fp
	lsls	r6, r5, #2
.L_081b8f40:
	ldr	r3, [r7, #12]
	lsls	r2, r3, #6
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_081b8f4c
	adds	r2, #63
.L_081b8f4c:
	asrs	r3, r2, #6
	str	r3, [r7, #12]
	ldr	r3, [r7, #16]
	lsls	r2, r3, #6
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_081b8f5c
	adds	r2, #63
.L_081b8f5c:
	asrs	r3, r2, #6
	str	r3, [r7, #16]
	ldr	r3, [r7, #20]
	lsls	r2, r3, #6
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_081b8f6c
	adds	r2, #63
.L_081b8f6c:
	asrs	r3, r2, #6
	str	r3, [r7, #20]
	ldr	r0, [sp, #20]
	add	r1, sp, #228
	ldr	r3, [r1, r0]
	ldr	r2, [r7, #24]
	adds	r2, r2, r3
	movs	r3, #142
	lsls	r3, r3, #7
	adds	r3, #255
	str	r2, [r7, #24]
	cmp	r2, r3
	ble.n	.L_081b8f8c
	ldr	r4, [pc, #652]
	adds	r3, r2, r4
	str	r3, [r7, #24]
.L_081b8f8c:
	ldr	r3, [r1, r6]
	cmp	r3, #0
	ble.n	.L_081b8f96
	subs	r3, #1
	str	r3, [r1, r6]
.L_081b8f96:
	ldr	r3, [r7, #12]
	cmp	r3, #0
	bne.n	.L_081b8fc2
	ldr	r3, [r7, #16]
	cmp	r3, #0
	bne.n	.L_081b8fc2
	ldr	r3, [r7, #20]
	cmp	r3, #0
	bne.n	.L_081b8fc2
	ldr	r3, [r7, #4]
	cmp	r3, #0
	bne.n	.L_081b8fc2
	movs	r1, #192
	ldr	r0, [r7, #24]
	lsls	r1, r1, #4
	add	r5, sp, #180
	bl	sub_08002054
	str	r0, [r5, r6]
	ldr	r5, [sp, #44]
	adds	r5, #1
	str	r5, [sp, #44]
.L_081b8fc2:
	ldr	r6, [sp, #20]
	movs	r0, #1
	add	fp, r0
	adds	r6, #4
	mov	r1, fp
	str	r6, [sp, #20]
	adds	r7, #28
	cmp	r1, #4
	beq.n	.L_081b8fd6
	b.n	.L_081b8d20
.L_081b8fd6:
	ldr	r2, [sp, #44]
	cmp	r2, #4
	beq.n	.L_081b8fde
	b.n	.L_081b9550
.L_081b8fde:
	movs	r4, #0
	movs	r3, #3
	str	r3, [sp, #96]
	str	r4, [sp, #84]
	str	r4, [sp, #104]
	mov	fp, r4
	add	r2, sp, #180
.L_081b8fec:
	ldmia	r2!, {r3}
	ldr	r5, [sp, #104]
	movs	r6, #1
	adds	r3, r5, r3
	add	fp, r6
	adds	r3, #1
	mov	r0, fp
	str	r3, [sp, #104]
	cmp	r0, #4
	bne.n	.L_081b8fec
	b.n	.L_081b9550
.L_081b9002:
	ldr	r1, [sp, #96]
	cmp	r1, #3
	beq.n	.L_081b900a
	b.n	.L_081b9220
.L_081b900a:
	ldr	r2, [sp, #84]
	adds	r2, #1
	str	r2, [sp, #84]
	cmp	r2, #24
	bne.n	.L_081b901a
	movs	r0, #152
	bl	sub_081c0010
.L_081b901a:
	ldr	r3, [sp, #84]
	cmp	r3, #70
	bne.n	.L_081b9026
	movs	r0, #103
	bl	sub_081c0010
.L_081b9026:
	ldr	r4, [sp, #84]
	cmp	r4, #40
	bne.n	.L_081b9050
	ldr	r5, [sp, #100]
	cmp	r5, #1
	bls.n	.L_081b903a
	movs	r0, #55
	bl	sub_081c0010
	b.n	.L_081b904a
.L_081b903a:
	ldr	r6, [sp, #52]
	movs	r0, #1
	negs	r0, r0
	cmp	r6, r0
	beq.n	.L_081b904a
	adds	r0, r6, #0
	bl	sub_081c0010
.L_081b904a:
	movs	r1, #1
	negs	r1, r1
	str	r1, [sp, #52]
.L_081b9050:
	ldr	r2, [sp, #84]
	cmp	r2, #1
	bne.n	.L_081b90f6
	ldr	r6, [sp, #88]
	movs	r5, #1
	movs	r3, #3
	movs	r4, #0
	negs	r5, r5
	str	r3, [sp, #56]
	str	r4, [sp, #100]
	str	r5, [sp, #52]
	cmp	r6, #0
	bne.n	.L_081b90b2
	add	r0, sp, #180
	bl	.L_081b834c
	str	r0, [sp, #100]
	cmp	r0, #3
	beq.n	.L_081b909e
	cmp	r0, #3
	bgt.n	.L_081b9080
	cmp	r0, #2
	beq.n	.L_081b9094
	b.n	.L_081b90d2
.L_081b9080:
	ldr	r0, [sp, #100]
	cmp	r0, #4
	beq.n	.L_081b90a8
	cmp	r0, #5
	bne.n	.L_081b90d2
	movs	r1, #93
	movs	r2, #8
	str	r1, [sp, #52]
	str	r2, [sp, #60]
	b.n	.L_081b90d2
.L_081b9094:
	movs	r3, #91
	movs	r4, #1
	str	r3, [sp, #52]
	str	r4, [sp, #60]
	b.n	.L_081b90d2
.L_081b909e:
	movs	r5, #92
	movs	r6, #3
	str	r5, [sp, #52]
	str	r6, [sp, #60]
	b.n	.L_081b90d2
.L_081b90a8:
	movs	r0, #91
	movs	r1, #2
	str	r0, [sp, #52]
	str	r1, [sp, #60]
	b.n	.L_081b90d2
.L_081b90b2:
	ldr	r2, [sp, #72]
	cmp	r2, #0
	bne.n	.L_081b90c6
	ldr	r3, [sp, #104]
	ldr	r4, [sp, #64]
	cmp	r3, r4
	ble.n	.L_081b90d2
	movs	r5, #1
	str	r5, [sp, #100]
	b.n	.L_081b90d2
.L_081b90c6:
	ldr	r6, [sp, #104]
	ldr	r0, [sp, #64]
	cmp	r6, r0
	bge.n	.L_081b90d2
	movs	r1, #1
	str	r1, [sp, #100]
.L_081b90d2:
	ldr	r2, [sp, #100]
	cmp	r2, #1
	bne.n	.L_081b90f6
	ldr	r3, [sp, #88]
	cmp	r3, #3
	bgt.n	.L_081b90e2
	movs	r4, #91
	str	r4, [sp, #52]
.L_081b90e2:
	ldr	r5, [sp, #88]
	cmp	r5, #4
	bne.n	.L_081b90ec
	movs	r6, #92
	str	r6, [sp, #52]
.L_081b90ec:
	ldr	r0, [sp, #88]
	cmp	r0, #5
	bne.n	.L_081b90f6
	movs	r1, #93
	str	r1, [sp, #52]
.L_081b90f6:
	ldr	r2, [sp, #100]
	cmp	r2, #0
	bne.n	.L_081b9104
	ldr	r3, [sp, #84]
	cmp	r3, #90
	beq.n	.L_081b910c
	b.n	.L_081b9550
.L_081b9104:
	ldr	r4, [sp, #84]
	cmp	r4, #68
	beq.n	.L_081b910c
	b.n	.L_081b9550
.L_081b910c:
	ldr	r5, [sp, #100]
	cmp	r5, #0
	beq.n	.L_081b919a
	ldr	r6, [sp, #88]
	cmp	r6, #0
	bne.n	.L_081b9124
	ldr	r2, [sp, #92]
	ldr	r1, [sp, #60]
	adds	r0, r1, #0
	muls	r0, r2
	str	r0, [sp, #92]
	b.n	.L_081b912a
.L_081b9124:
	ldr	r3, [sp, #92]
	lsls	r3, r3, #1
	str	r3, [sp, #92]
.L_081b912a:
	ldr	r4, [sp, #120]
	movs	r5, #153
	lsls	r5, r5, #3
	adds	r3, r4, r5
	ldr	r2, [r3, #0]
	movs	r3, #5
	str	r3, [sp, #0]
	ldr	r0, [sp, #92]
	movs	r1, #6
	movs	r3, #24
	bl	sub_080380b0
	ldr	r6, [sp, #60]
	ldr	r0, [sp, #88]
	lsls	r6, r6, #1
	str	r6, [sp, #60]
	cmp	r0, #5
	bne.n	.L_081b9160
	ldr	r3, [sp, #92]
	movs	r1, #14
	movs	r2, #0
	movs	r4, #4
	str	r1, [sp, #64]
	str	r2, [sp, #88]
	str	r3, [sp, #76]
	str	r4, [sp, #96]
	b.n	.L_081b9192
.L_081b9160:
	movs	r3, #6
	movs	r5, #5
	str	r3, [sp, #0]
	movs	r1, #16
	movs	r2, #20
	movs	r3, #4
	movs	r6, #0
	movs	r0, #4
	str	r5, [sp, #96]
	str	r5, [sp, #56]
	str	r6, [sp, #68]
	bl	sub_08038010
	movs	r2, #128
	adds	r1, r0, #0
	ldr	r0, [sp, #120]
	lsls	r2, r2, #3
	adds	r2, #204
	adds	r3, r0, r2
	str	r1, [r3, #0]
	ldr	r0, [pc, #140]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
.L_081b9192:
	movs	r3, #0
	str	r3, [sp, #84]
	str	r3, [sp, #80]
	b.n	.L_081b9550
.L_081b919a:
	movs	r4, #14
	movs	r5, #0
	str	r4, [sp, #64]
	str	r5, [sp, #88]
	bl	sub_080ad290
	lsls	r3, r0, #2
	adds	r3, r3, r0
	ldr	r6, [sp, #120]
	lsls	r3, r3, #1
	str	r3, [sp, #92]
	movs	r0, #153
	lsls	r0, r0, #3
	adds	r3, r6, r0
	ldr	r2, [r3, #0]
	movs	r3, #5
	str	r3, [sp, #0]
	ldr	r0, [sp, #92]
	movs	r1, #6
	movs	r3, #24
	bl	sub_080380b0
	movs	r0, #1
	str	r5, [sp, #60]
	bl	sub_081c0010
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #16
	movs	r2, #8
	movs	r3, #4
	movs	r0, #22
	str	r5, [sp, #96]
	bl	sub_08038010
	movs	r3, #128
	ldr	r2, [sp, #120]
	ldr	r5, [pc, #52]
	lsls	r3, r3, #3
	adds	r3, #204
	adds	r1, r0, #0
	adds	r6, r2, r3
	adds	r0, r5, #0
	str	r1, [r6, #0]
	movs	r2, #0
	movs	r3, #0
	adds	r5, #1
	bl	sub_08038080
	adds	r0, r5, #0
	ldr	r1, [r6, #0]
	movs	r2, #0
	movs	r3, #8
	bl	sub_08038080
	movs	r5, #1
	movs	r4, #0
	str	r4, [sp, #80]
	str	r4, [sp, #84]
	str	r5, [sp, #56]
	b.n	.L_081b9550
	.4byte 0xffffb800
	.4byte 0x00000d7e
	.2byte 0x0d77
	.2byte 0x0000
.L_081b9220:
	ldr	r6, [sp, #96]
	cmp	r6, #4
	beq.n	.L_081b9228
	b.n	.L_081b93c6
.L_081b9228:
	ldr	r0, [sp, #84]
	adds	r0, #1
	str	r0, [sp, #84]
	cmp	r0, #20
	bne.n	.L_081b92ae
	ldr	r1, [sp, #76]
	movs	r7, #1
	cmp	r1, #9
	ble.n	.L_081b925e
	ldr	r2, [sp, #76]
	movs	r7, #2
	cmp	r2, #99
	ble.n	.L_081b925e
	ldr	r3, [sp, #76]
	movs	r4, #250
	lsls	r4, r4, #2
	movs	r7, #3
	cmp	r3, r4
	blt.n	.L_081b925e
	movs	r6, #156
	ldr	r5, [sp, #76]
	lsls	r6, r6, #6
	adds	r6, #15
	movs	r7, #4
	cmp	r5, r6
	ble.n	.L_081b925e
	movs	r7, #5
.L_081b925e:
	lsrs	r3, r7, #1
	movs	r0, #9
	subs	r0, r0, r3
	adds	r2, r7, #0
	movs	r3, #6
	str	r3, [sp, #0]
	adds	r2, #12
	movs	r1, #16
	movs	r3, #3
	bl	sub_08038010
	movs	r2, #128
	adds	r1, r0, #0
	ldr	r0, [sp, #120]
	ldr	r5, [pc, #648]
	lsls	r2, r2, #3
	adds	r2, #204
	adds	r6, r0, r2
	str	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	subs	r5, #1
	lsls	r2, r7, #3
	ldr	r1, [r6, #0]
	adds	r2, #48
	adds	r0, r5, #0
	movs	r3, #0
	bl	sub_08038080
	movs	r3, #0
	ldr	r2, [r6, #0]
	ldr	r0, [sp, #76]
	str	r3, [sp, #0]
	adds	r1, r7, #0
	movs	r3, #48
	bl	sub_080380b0
.L_081b92ae:
	ldr	r3, [sp, #84]
	cmp	r3, #19
	bgt.n	.L_081b92b6
	b.n	.L_081b9550
.L_081b92b6:
	ldr	r3, [pc, #592]
	ldr	r5, [r3, #12]
	movs	r3, #1
	ands	r5, r3
	cmp	r5, #0
	beq.n	.L_081b9372
	ldr	r4, [sp, #120]
	movs	r5, #128
	lsls	r5, r5, #3
	adds	r5, #204
	adds	r6, r4, r5
	movs	r1, #1
	ldr	r0, [r6, #0]
	bl	sub_08038018
	movs	r0, #1
	bl	sub_081c0010
	movs	r0, #112
	bl	sub_081c0010
	movs	r0, #0
	movs	r3, #6
	str	r0, [sp, #96]
	str	r3, [sp, #0]
	movs	r1, #16
	movs	r2, #8
	movs	r3, #4
	movs	r0, #22
	bl	sub_08038010
	ldr	r5, [pc, #532]
	adds	r1, r0, #0
	str	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	adds	r5, #1
	bl	sub_08038080
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r3, #8
	movs	r2, #0
	bl	sub_08038080
	movs	r1, #0
	movs	r2, #1
	str	r1, [sp, #80]
	str	r1, [sp, #84]
	str	r2, [sp, #56]
	bl	sub_080ad290
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	ldr	r4, [sp, #120]
	str	r3, [sp, #92]
	movs	r5, #153
	lsls	r5, r5, #3
	adds	r3, r4, r5
	ldr	r2, [r3, #0]
	movs	r3, #5
	str	r3, [sp, #0]
	ldr	r0, [sp, #92]
	movs	r1, #6
	movs	r3, #24
	bl	sub_080380b0
	ldr	r6, [sp, #76]
	cmp	r6, #0
	bgt.n	.L_081b9348
	b.n	.L_081b9550
.L_081b9348:
	movs	r0, #105
	bl	sub_081c0010
	ldr	r0, [sp, #76]
	bl	sub_080ad1d8
	ldr	r3, [pc, #440]
	ldr	r1, [sp, #120]
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #196
	ldr	r0, [r3, #16]
	adds	r3, r1, r2
	ldr	r2, [r3, #0]
	ldr	r3, [sp, #96]
	movs	r1, #6
	str	r3, [sp, #0]
	movs	r3, #0
	bl	sub_080380b0
	b.n	.L_081b9550
.L_081b9372:
	ldr	r4, [sp, #76]
	cmp	r4, #0
	bgt.n	.L_081b937a
	b.n	.L_081b9550
.L_081b937a:
	subs	r4, #1
	str	r4, [sp, #76]
	cmp	r4, #0
	bne.n	.L_081b9388
	movs	r0, #105
	bl	sub_081c0010
.L_081b9388:
	movs	r0, #1
	bl	sub_080ad1d8
	ldr	r3, [pc, #384]
	ldr	r6, [sp, #120]
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #196
	ldr	r0, [r3, #16]
	adds	r3, r6, r1
	ldr	r2, [r3, #0]
	movs	r1, #6
	movs	r3, #0
	str	r5, [sp, #0]
	bl	sub_080380b0
	ldr	r2, [sp, #92]
	movs	r4, #153
	subs	r2, #1
	str	r2, [sp, #92]
	lsls	r4, r4, #3
	adds	r3, r6, r4
	ldr	r2, [r3, #0]
	movs	r3, #5
	str	r3, [sp, #0]
	ldr	r0, [sp, #92]
	movs	r1, #6
	movs	r3, #24
	bl	sub_080380b0
	b.n	.L_081b9550
.L_081b93c6:
	ldr	r5, [sp, #96]
	cmp	r5, #5
	beq.n	.L_081b93ce
	b.n	.L_081b9550
.L_081b93ce:
	ldr	r6, [sp, #80]
	cmp	r6, #4
	bne.n	.L_081b93da
	movs	r0, #175
	bl	sub_081c0010
.L_081b93da:
	ldr	r0, [sp, #80]
	cmp	r0, #7
	bgt.n	.L_081b93e6
	adds	r0, #1
	str	r0, [sp, #80]
	b.n	.L_081b94d4
.L_081b93e6:
	ldr	r1, [sp, #84]
	cmp	r1, #0
	ble.n	.L_081b9488
	adds	r1, #1
	str	r1, [sp, #84]
	cmp	r1, #16
	bne.n	.L_081b94d4
	ldr	r2, [sp, #120]
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #204
	adds	r5, r2, r3
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl	sub_08038018
	ldr	r4, [sp, #68]
	cmp	r4, #0
	bne.n	.L_081b9472
	ldr	r0, [sp, #88]
	ldr	r6, [sp, #104]
	adds	r0, #1
	movs	r1, #1
	movs	r3, #6
	str	r0, [sp, #88]
	str	r1, [sp, #96]
	str	r3, [sp, #0]
	movs	r1, #16
	movs	r2, #21
	movs	r0, #4
	movs	r3, #4
	str	r6, [sp, #64]
	bl	sub_08038010
	adds	r1, r0, #0
	str	r1, [r5, #0]
	ldr	r2, [sp, #104]
	cmp	r2, #24
	bne.n	.L_081b9444
	movs	r3, #1
	str	r3, [sp, #72]
	ldr	r0, [pc, #216]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	b.n	.L_081b9452
.L_081b9444:
	movs	r4, #0
	ldr	r0, [pc, #208]
	movs	r2, #0
	movs	r3, #0
	str	r4, [sp, #72]
	bl	sub_08038080
.L_081b9452:
	ldr	r5, [sp, #120]
	movs	r6, #128
	lsls	r6, r6, #3
	adds	r6, #204
	adds	r3, r5, r6
	ldr	r1, [r3, #0]
	ldr	r0, [pc, #188]
	movs	r2, #0
	movs	r3, #8
	bl	sub_08038080
	movs	r0, #0
	movs	r1, #1
	str	r0, [sp, #80]
	str	r1, [sp, #56]
	b.n	.L_081b9482
.L_081b9472:
	ldr	r4, [sp, #92]
	movs	r2, #14
	movs	r3, #0
	movs	r5, #4
	str	r2, [sp, #64]
	str	r3, [sp, #88]
	str	r4, [sp, #76]
	str	r5, [sp, #96]
.L_081b9482:
	movs	r6, #0
	str	r6, [sp, #84]
	b.n	.L_081b94d4
.L_081b9488:
	ldr	r1, [pc, #124]
	movs	r2, #1
	ldr	r3, [r1, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081b949e
	movs	r0, #112
	str	r2, [sp, #84]
	bl	sub_081c0010
	b.n	.L_081b94d4
.L_081b949e:
	ldr	r3, [r1, #12]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081b94ba
	ldr	r0, [sp, #68]
	cmp	r0, #1
	bne.n	.L_081b94ba
	movs	r0, #111
	bl	sub_081c0010
	movs	r1, #0
	str	r1, [sp, #68]
	b.n	.L_081b94d4
.L_081b94ba:
	ldr	r3, [r1, #12]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081b94d4
	ldr	r2, [sp, #68]
	cmp	r2, #0
	bne.n	.L_081b9528
	movs	r0, #111
	bl	sub_081c0010
	movs	r3, #1
	str	r3, [sp, #68]
.L_081b94d4:
	ldr	r4, [sp, #68]
	cmp	r4, #0
	bne.n	.L_081b9528
	ldr	r5, [sp, #128]
	ldr	r2, [sp, #84]
	movs	r6, #224
	lsls	r6, r6, #1
	lsls	r3, r2, #12
	adds	r0, r5, r6
	ldr	r1, [pc, #56]
	movs	r2, #16
	bl	sub_081b810c
	movs	r3, #240
	lsls	r3, r3, #1
	movs	r2, #128
	adds	r0, r5, r3
	ldr	r1, [pc, #44]
	lsls	r2, r2, #8
.L_081b94fa:
	movs	r3, #16
	bl	sub_081b80a8
	b.n	.L_081b9550
	movs	r0, r0
	.4byte 0x00000d6a
	.4byte 0x03001150
	.4byte 0x00000d77
	.4byte 0x02000240
	.4byte 0x00000d7c
	.4byte 0x00000d7b
	.4byte 0x00000d7d
	.4byte 0x050003c0
	.2byte 0x03e0
	.2byte 0x0500
.L_081b9528:
	ldr	r4, [sp, #128]
	ldr	r6, [sp, #84]
	movs	r5, #240
	lsls	r5, r5, #1
	adds	r0, r4, r5
	ldr	r1, [pc, #248]
	lsls	r3, r6, #12
	movs	r2, #16
	bl	sub_081b810c
	ldr	r1, [sp, #128]
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r0, r1, r2
	movs	r2, #128
	ldr	r1, [pc, #232]
	lsls	r2, r2, #8
	movs	r3, #16
	bl	sub_081b80a8
.L_081b9550:
	ldr	r3, [sp, #56]
	movs	r4, #1
	negs	r4, r4
	cmp	r3, r4
	bne.n	.L_081b955c
	b.n	.L_081b9800
.L_081b955c:
	cmp	r3, #1
	beq.n	.L_081b9562
	b.n	.L_081b96aa
.L_081b9562:
	ldr	r0, [pc, #208]
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	str	r0, [sp, #112]
	adds	r3, #212
	ldr	r1, [pc, #192]
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [sp, #128]
	movs	r6, #192
	movs	r2, #132
	lsls	r6, r6, #1
	lsls	r2, r2, #24
	ldr	r0, [sp, #112]
	adds	r1, r5, r6
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [sp, #112]
	ldr	r1, [pc, #168]
	adds	r0, #32
	str	r0, [sp, #112]
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #152]
	ldr	r1, [pc, #152]
	ldr	r2, [pc, #156]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #152]
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	str	r0, [sp, #112]
	adds	r3, #212
	ldr	r1, [pc, #140]
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #208
	lsls	r2, r2, #1
	adds	r1, r5, r2
	movs	r2, #132
	lsls	r2, r2, #24
	ldr	r0, [sp, #112]
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [sp, #112]
	ldr	r1, [pc, #96]
	adds	r3, #32
	adds	r0, r3, #0
	str	r3, [sp, #112]
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #76]
	ldr	r1, [pc, #96]
	ldr	r2, [pc, #80]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #92]
	bl	sub_08013300
	adds	r6, #64
	adds	r4, r5, r6
	str	r0, [sp, #112]
	ldr	r6, [pc, #36]
	movs	r1, #0
	mov	fp, r1
	movs	r5, #31
.L_081b960a:
	ldrh	r3, [r0, #0]
	lsrs	r2, r3, #10
	ands	r2, r6
	adds	r1, r2, #4
	adds	r2, r5, #0
	ands	r2, r3
	adds	r2, #4
	cmp	r2, #31
	ble.n	.L_081b961e
	movs	r2, #31
.L_081b961e:
	cmp	r1, #31
	ble.n	.L_081b9658
	movs	r1, #31
	b.n	.L_081b9658
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x050003e0
	.4byte 0x050003c0
	.4byte 0x0000008a
	.4byte 0x05000380
	.4byte 0x02010000
	.4byte 0x06014000
	.4byte 0x84000180
	.4byte 0x0000008b
	.4byte 0x050003a0
	.4byte 0x06014600
	.2byte 0x008e
	.2byte 0x0000
.L_081b9658:
	lsls	r2, r2, #5
	lsls	r3, r1, #10
	orrs	r3, r2
	movs	r2, #1
	orrs	r3, r1
	add	fp, r2
	strh	r3, [r4, #0]
	mov	r3, fp
	adds	r0, #2
	adds	r4, #2
	cmp	r3, #16
	bne.n	.L_081b960a
	ldr	r5, [sp, #128]
	movs	r6, #224
	lsls	r6, r6, #1
	movs	r3, #128
	movs	r2, #132
	adds	r4, r5, r6
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r4, #0
	ldr	r1, [pc, #564]
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r0, r4, #0
	adds	r1, #32
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r1, r5, r2
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r0, r4, #0
	adds	r2, #8
	b.n	.L_081b97f6
.L_081b96aa:
	ldr	r3, [sp, #56]
	cmp	r3, #3
	bne.n	.L_081b975c
	ldr	r4, [sp, #100]
	cmp	r4, #5
	bhi.n	.L_081b96f4
	ldr	r2, [pc, #520]
	lsls	r3, r4, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x081b96d8
	.4byte 0x081b96dc
	.4byte 0x081b96e0
	.4byte 0x081b96e4
	.4byte 0x081b96e8
	.2byte 0x96ec
	.2byte 0x081b
	ldr	r0, [pc, #488]
	b.n	.L_081b96ee
	ldr	r0, [pc, #488]
	b.n	.L_081b96ee
	ldr	r0, [pc, #488]
	b.n	.L_081b96ee
	ldr	r0, [pc, #488]
	b.n	.L_081b96ee
	ldr	r0, [pc, #488]
	b.n	.L_081b96ee
	ldr	r0, [pc, #488]
.L_081b96ee:
	bl	sub_08013300
	str	r0, [sp, #112]
.L_081b96f4:
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [sp, #112]
	ldr	r1, [pc, #472]
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [sp, #112]
	ldr	r1, [pc, #468]
	adds	r5, #32
	adds	r0, r5, #0
	str	r5, [sp, #112]
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #448]
	ldr	r1, [pc, #452]
	ldr	r2, [pc, #452]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #452]
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	str	r0, [sp, #112]
	adds	r3, #212
	ldr	r1, [pc, #436]
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r6, [sp, #112]
	ldr	r1, [pc, #412]
	adds	r6, #32
	adds	r0, r6, #0
	str	r6, [sp, #112]
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #392]
	ldr	r1, [pc, #412]
	ldr	r2, [pc, #412]
	b.n	.L_081b97f6
.L_081b975c:
	ldr	r0, [sp, #56]
	cmp	r0, #5
	bne.n	.L_081b97fa
	ldr	r0, [pc, #408]
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	str	r0, [sp, #112]
	adds	r3, #212
	ldr	r1, [pc, #324]
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [sp, #128]
	movs	r4, #224
	lsls	r4, r4, #1
	adds	r1, r2, r4
	movs	r2, #132
	lsls	r2, r2, #24
	ldr	r0, [sp, #112]
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [sp, #112]
	ldr	r1, [pc, #332]
	adds	r5, #32
	adds	r0, r5, #0
	str	r5, [sp, #112]
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #312]
	ldr	r1, [pc, #344]
	ldr	r2, [pc, #344]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #344]
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	str	r0, [sp, #112]
	adds	r3, #212
	ldr	r1, [pc, #328]
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r6, [sp, #128]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r1, r6, r2
	movs	r2, #132
	lsls	r2, r2, #24
	ldr	r0, [sp, #112]
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [sp, #112]
	ldr	r1, [pc, #256]
	adds	r3, #32
	adds	r0, r3, #0
	str	r3, [sp, #112]
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #236]
	ldr	r1, [pc, #284]
	ldr	r2, [pc, #268]
.L_081b97f6:
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_081b97fa:
	movs	r4, #1
	negs	r4, r4
	str	r4, [sp, #56]
.L_081b9800:
	ldr	r5, [sp, #88]
	cmp	r5, #0
	bgt.n	.L_081b9808
	b.n	.L_081b9a72
.L_081b9808:
	ldr	r1, [sp, #48]
	movs	r6, #0
	movs	r0, #224
	lsls	r0, r0, #7
	lsls	r3, r1, #3
	mov	fp, r6
	movs	r7, #128
	movs	r6, #128
	lsls	r7, r7, #1
	lsls	r6, r6, #23
	mov	ip, r0
	adds	r1, r3, #0
	movs	r0, #128
	adds	r2, r3, #0
	adds	r7, #255
	adds	r6, #2
	movs	r4, #152
	lsls	r0, r0, #2
	adds	r1, #200
	adds	r2, #196
.L_081b9830:
	adds	r3, r0, #0
	ldr	r5, [sp, #120]
	ands	r3, r7
	lsls	r3, r3, #16
	orrs	r3, r6
	str	r3, [r5, r2]
	adds	r3, r4, #0
	mov	r5, ip
	orrs	r3, r5
	ldr	r5, [sp, #120]
	adds	r2, #8
	str	r3, [r5, r1]
	ldr	r3, [sp, #48]
	movs	r5, #1
	adds	r3, #1
	add	fp, r5
	str	r3, [sp, #48]
	mov	r3, fp
	adds	r1, #8
	adds	r4, #4
	adds	r0, #16
	cmp	r3, #4
	bne.n	.L_081b9830
	ldr	r4, [sp, #88]
	cmp	r4, #5
	bne.n	.L_081b9914
	ldr	r1, [sp, #48]
	movs	r0, #192
	lsls	r0, r0, #7
	lsls	r3, r1, #3
	movs	r7, #128
	movs	r6, #128
	movs	r5, #0
	lsls	r7, r7, #1
	lsls	r6, r6, #23
	mov	ip, r0
	adds	r1, r3, #0
	movs	r0, #146
	adds	r2, r3, #0
	mov	fp, r5
	adds	r7, #255
	adds	r6, #2
	movs	r4, #168
	lsls	r0, r0, #2
	adds	r1, #200
	adds	r2, #196
.L_081b988c:
	adds	r3, r0, #0
	ldr	r5, [sp, #120]
	ands	r3, r7
	lsls	r3, r3, #16
	orrs	r3, r6
	str	r3, [r5, r2]
	adds	r3, r4, #0
	mov	r5, ip
	orrs	r3, r5
	ldr	r5, [sp, #120]
	adds	r2, #8
	str	r3, [r5, r1]
	ldr	r3, [sp, #48]
	movs	r5, #1
	adds	r3, #1
	add	fp, r5
	str	r3, [sp, #48]
	mov	r3, fp
	adds	r1, #8
	adds	r4, #4
	adds	r0, #16
	cmp	r3, #5
	bne.n	.L_081b988c
	b.n	.L_081b998c
	.4byte 0x050003c0
	.4byte 0x081b96c0
	.4byte 0x00000089
	.4byte 0x00000088
	.4byte 0x00000093
	.4byte 0x00000094
	.4byte 0x00000095
	.4byte 0x00000096
	.4byte 0x050003a0
	.4byte 0x02010000
	.4byte 0x06014000
	.4byte 0x84000400
	.4byte 0x00000090
	.4byte 0x05000380
	.4byte 0x06015000
	.4byte 0x84000500
	.4byte 0x0000008c
	.4byte 0x06012480
	.4byte 0x84000280
	.4byte 0x0000008d
	.4byte 0x050003e0
	.2byte 0x2e80
	.2byte 0x0601
.L_081b9914:
	ldr	r1, [sp, #48]
	movs	r7, #128
	lsls	r3, r1, #3
	movs	r6, #128
	movs	r5, #192
	movs	r4, #0
	lsls	r7, r7, #1
	lsls	r6, r6, #23
	lsls	r5, r5, #7
	movs	r0, #152
	adds	r1, r3, #0
	adds	r2, r3, #0
	mov	fp, r4
	adds	r7, #255
	adds	r6, #2
	mov	ip, r5
	movs	r4, #188
	lsls	r0, r0, #2
	adds	r1, #200
	adds	r2, #196
.L_081b993c:
	adds	r3, r0, #0
	ldr	r5, [sp, #120]
	ands	r3, r7
	lsls	r3, r3, #16
	orrs	r3, r6
	str	r3, [r5, r2]
	adds	r3, r4, #0
	mov	r5, ip
	orrs	r3, r5
	ldr	r5, [sp, #120]
	adds	r2, #8
	str	r3, [r5, r1]
	ldr	r3, [sp, #48]
	movs	r5, #1
	adds	r3, #1
	add	fp, r5
	str	r3, [sp, #48]
	mov	r3, fp
	adds	r1, #8
	adds	r4, #4
	adds	r0, #16
	cmp	r3, #3
	bne.n	.L_081b993c
	ldr	r4, [sp, #48]
	ldr	r3, [pc, #416]
	lsls	r1, r4, #3
	ldr	r5, [sp, #120]
	adds	r2, r1, #0
	adds	r2, #196
	str	r3, [r5, r2]
	ldr	r6, [sp, #88]
	movs	r2, #128
	lsls	r3, r6, #3
	adds	r3, #200
	lsls	r2, r2, #8
	adds	r1, #200
	orrs	r3, r2
	adds	r4, #1
	str	r3, [r5, r1]
	str	r4, [sp, #48]
.L_081b998c:
	ldr	r1, [sp, #48]
	movs	r0, #0
	lsls	r3, r1, #3
	adds	r1, r3, #0
	adds	r2, r3, #0
	movs	r7, #128
	movs	r6, #128
	movs	r3, #144
	mov	fp, r0
	lsls	r7, r7, #1
	lsls	r6, r6, #23
	movs	r4, #142
	movs	r0, #144
	lsls	r3, r3, #8
	adds	r7, #255
	adds	r6, #24
	lsls	r4, r4, #1
	lsls	r0, r0, #2
	adds	r1, #200
	adds	r2, #196
	mov	ip, r3
.L_081b99b6:
	adds	r3, r0, #0
	ldr	r5, [sp, #120]
	ands	r3, r7
	lsls	r3, r3, #16
	orrs	r3, r6
	str	r3, [r5, r2]
	adds	r3, r4, #0
	mov	r5, ip
	orrs	r3, r5
	ldr	r5, [sp, #120]
	adds	r2, #8
	str	r3, [r5, r1]
	ldr	r3, [sp, #48]
	movs	r5, #1
	adds	r3, #1
	add	fp, r5
	str	r3, [sp, #48]
	mov	r3, fp
	adds	r1, #8
	adds	r4, #4
	adds	r0, #16
	cmp	r3, #2
	bne.n	.L_081b99b6
	ldr	r4, [sp, #60]
	movs	r3, #1
	cmp	r4, #9
	ble.n	.L_081b9a10
	ldr	r5, [sp, #60]
	movs	r3, #2
	cmp	r5, #99
	ble.n	.L_081b9a10
	ldr	r6, [sp, #60]
	movs	r0, #250
	lsls	r0, r0, #2
	movs	r3, #3
	cmp	r6, r0
	blt.n	.L_081b9a10
	movs	r2, #156
	ldr	r1, [sp, #60]
	lsls	r2, r2, #6
	adds	r2, #15
	movs	r3, #4
	cmp	r1, r2
	ble.n	.L_081b9a10
	movs	r3, #5
.L_081b9a10:
	lsls	r2, r3, #2
	adds	r2, r2, r3
	movs	r3, #140
	lsls	r2, r2, #1
	lsls	r3, r3, #2
	subs	r3, r3, r2
	movs	r2, #128
	ldr	r4, [sp, #48]
	lsls	r2, r2, #1
	adds	r2, #255
	ands	r3, r2
	movs	r2, #128
	ldr	r5, [sp, #120]
	lsls	r1, r4, #3
	lsls	r2, r2, #23
	adds	r0, r1, #0
	adds	r2, #24
	lsls	r3, r3, #16
	orrs	r3, r2
	adds	r0, #196
	str	r3, [r5, r0]
	movs	r3, #145
	lsls	r3, r3, #8
	adds	r1, #200
	adds	r3, #24
	str	r3, [r5, r1]
	movs	r3, #10
	adds	r4, #1
	str	r3, [sp, #0]
	movs	r3, #5
	str	r3, [sp, #4]
	adds	r0, r4, #0
	ldr	r3, [sp, #60]
	movs	r1, #48
	movs	r2, #20
	str	r4, [sp, #48]
	bl	sub_081b8180
	movs	r3, #11
	str	r3, [sp, #0]
	movs	r3, #1
	str	r3, [sp, #4]
	movs	r1, #80
	movs	r2, #20
	movs	r3, #0
	str	r0, [sp, #48]
	bl	sub_081b8180
	str	r0, [sp, #48]
.L_081b9a72:
	ldr	r6, [sp, #96]
	cmp	r6, #1
	beq.n	.L_081b9a7a
	b.n	.L_081b9bfa
.L_081b9a7a:
	ldr	r0, [sp, #84]
	cmp	r0, #0
	ble.n	.L_081b9a92
	ldr	r1, [sp, #72]
	cmp	r1, #1
	bne.n	.L_081b9a92
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #2
	adds	r5, r3, #0
	adds	r5, #64
	b.n	.L_081b9a94
.L_081b9a92:
	movs	r5, #64
.L_081b9a94:
	ldr	r2, [sp, #72]
	cmp	r2, #0
	bne.n	.L_081b9aae
	ldr	r3, [sp, #108]
	lsls	r0, r3, #10
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	ands	r0, r3
	bl	sub_08002096
	asrs	r0, r0, #14
	b.n	.L_081b9ab0
.L_081b9aae:
	movs	r0, #0
.L_081b9ab0:
	ldr	r4, [sp, #80]
	lsls	r2, r4, #4
	adds	r6, r2, #0
	subs	r3, r6, r4
	adds	r1, r5, r3
	ldr	r5, [sp, #64]
	cmp	r5, #23
	bgt.n	.L_081b9ada
	adds	r2, r0, #0
	movs	r0, #14
	str	r0, [sp, #0]
	adds	r3, r5, #0
	movs	r0, #2
	str	r0, [sp, #4]
	subs	r1, #110
	adds	r2, #60
	adds	r3, #1
	ldr	r0, [sp, #48]
	bl	sub_081b8180
	str	r0, [sp, #48]
.L_081b9ada:
	ldr	r0, [sp, #84]
	cmp	r0, #0
	ble.n	.L_081b9af2
	ldr	r1, [sp, #72]
	cmp	r1, #0
	bne.n	.L_081b9af2
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #2
	adds	r5, r3, #0
	adds	r5, #64
	b.n	.L_081b9af4
.L_081b9af2:
	movs	r5, #64
.L_081b9af4:
	ldr	r2, [sp, #72]
	cmp	r2, #1
	bne.n	.L_081b9b14
	ldr	r3, [sp, #108]
	lsls	r0, r3, #10
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	ands	r0, r3
	bl	sub_08002096
	asrs	r0, r0, #14
	b.n	.L_081b9b16
	movs	r0, r0
	.2byte 0x8000
	.2byte 0x8050
.L_081b9b14:
	movs	r0, #0
.L_081b9b16:
	ldr	r4, [sp, #80]
	subs	r6, r6, r4
	adds	r1, r5, r6
	ldr	r5, [sp, #64]
	cmp	r5, #4
	ble.n	.L_081b9b3c
	movs	r2, #94
	subs	r2, r2, r0
	movs	r0, #15
	str	r0, [sp, #0]
	adds	r3, r5, #0
	movs	r0, #2
	str	r0, [sp, #4]
	subs	r1, #110
	subs	r3, #1
	ldr	r0, [sp, #48]
	bl	sub_081b8180
	str	r0, [sp, #48]
.L_081b9b3c:
	ldr	r1, [sp, #84]
	adds	r4, r6, #0
	lsls	r3, r1, #1
	adds	r3, r3, r1
	movs	r6, #128
	movs	r7, #128
	movs	r0, #0
	lsls	r3, r3, #2
	lsls	r6, r6, #2
	lsls	r7, r7, #1
	mov	fp, r0
	mov	ip, r3
	mov	r8, r6
	mov	lr, r0
	movs	r5, #0
	adds	r7, #255
.L_081b9b5c:
	ldr	r2, [sp, #84]
	movs	r3, #64
	cmp	r2, #0
	ble.n	.L_081b9b6e
	ldr	r0, [sp, #72]
	cmp	r0, #1
	bne.n	.L_081b9b6e
	mov	r3, ip
	adds	r3, #64
.L_081b9b6e:
	ldr	r2, [sp, #48]
	adds	r3, r3, r4
	subs	r3, #120
	adds	r3, r3, r5
	lsls	r1, r2, #3
	add	r3, r8
	movs	r2, #128
	ands	r3, r7
	lsls	r2, r2, #24
	adds	r2, #52
	lsls	r3, r3, #16
	orrs	r3, r2
	ldr	r2, [sp, #120]
	adds	r0, r1, #0
	adds	r0, #196
	str	r3, [r2, r0]
	movs	r3, #192
	lsls	r3, r3, #8
	orrs	r3, r6
	adds	r1, #200
	str	r3, [r2, r1]
	ldr	r3, [sp, #48]
	ldr	r0, [sp, #84]
	adds	r3, #1
	str	r3, [sp, #48]
	movs	r3, #64
	cmp	r0, #0
	ble.n	.L_081b9bb0
	ldr	r1, [sp, #72]
	cmp	r1, #0
	bne.n	.L_081b9bb0
	mov	r3, ip
	adds	r3, #64
.L_081b9bb0:
	ldr	r2, [sp, #48]
	adds	r3, r3, r4
	subs	r3, #120
	adds	r3, r3, r5
	lsls	r1, r2, #3
	add	r3, r8
	movs	r2, #128
	ands	r3, r7
	lsls	r2, r2, #24
	adds	r2, #92
	lsls	r3, r3, #16
	orrs	r3, r2
	ldr	r2, [sp, #120]
	adds	r0, r1, #0
	adds	r0, #196
	str	r3, [r2, r0]
	movs	r3, #140
	ldr	r0, [sp, #120]
	lsls	r3, r3, #2
	movs	r2, #208
	add	r3, lr
	lsls	r2, r2, #8
	orrs	r3, r2
	adds	r1, #200
	str	r3, [r0, r1]
	ldr	r1, [sp, #48]
	movs	r3, #1
	add	fp, r3
	adds	r1, #1
	movs	r2, #16
	mov	r0, fp
	str	r1, [sp, #48]
	adds	r6, #16
	add	lr, r2
	adds	r5, #32
	cmp	r0, #3
	bne.n	.L_081b9b5c
.L_081b9bfa:
	ldr	r1, [sp, #96]
	cmp	r1, #5
	bne.n	.L_081b9cca
	ldr	r4, [sp, #84]
	ldr	r3, [sp, #80]
	movs	r2, #0
	ldr	r5, [sp, #80]
	movs	r6, #0
	mov	fp, r2
	lsls	r2, r3, #4
	lsls	r3, r4, #1
	adds	r3, r3, r4
	mov	lr, r6
	movs	r6, #128
	lsls	r3, r3, #2
	movs	r7, #128
	lsls	r6, r6, #1
	subs	r4, r2, r5
	mov	ip, r3
	movs	r5, #0
	lsls	r7, r7, #2
	adds	r6, #255
.L_081b9c26:
	ldr	r0, [sp, #84]
	movs	r3, #64
	cmp	r0, #0
	ble.n	.L_081b9c38
	ldr	r1, [sp, #68]
	cmp	r1, #1
	bne.n	.L_081b9c38
	mov	r3, ip
	adds	r3, #64
.L_081b9c38:
	ldr	r2, [sp, #48]
	adds	r3, r3, r4
	subs	r3, #120
	adds	r3, r3, r5
	lsls	r1, r2, #3
	adds	r3, r3, r7
	movs	r2, #128
	ands	r3, r6
	lsls	r2, r2, #24
	adds	r2, #52
	lsls	r3, r3, #16
	orrs	r3, r2
	ldr	r2, [sp, #120]
	adds	r0, r1, #0
	adds	r0, #196
	str	r3, [r2, r0]
	movs	r3, #146
	lsls	r3, r3, #1
	movs	r2, #224
	ldr	r0, [sp, #120]
	lsls	r2, r2, #8
	add	r3, lr
	orrs	r3, r2
	adds	r1, #200
	str	r3, [r0, r1]
	ldr	r1, [sp, #48]
	ldr	r2, [sp, #84]
	adds	r1, #1
	str	r1, [sp, #48]
	movs	r3, #64
	cmp	r2, #0
	ble.n	.L_081b9c82
	ldr	r0, [sp, #68]
	cmp	r0, #0
	bne.n	.L_081b9c82
	mov	r3, ip
	adds	r3, #64
.L_081b9c82:
	ldr	r2, [sp, #48]
	adds	r3, r3, r4
	subs	r3, #120
	adds	r3, r3, r5
	lsls	r1, r2, #3
	adds	r3, r3, r7
	movs	r2, #128
	ands	r3, r6
	lsls	r2, r2, #24
	adds	r2, #84
	lsls	r3, r3, #16
	orrs	r3, r2
	ldr	r2, [sp, #120]
	adds	r0, r1, #0
	adds	r0, #196
	str	r3, [r2, r0]
	movs	r3, #186
	ldr	r0, [sp, #120]
	lsls	r3, r3, #1
	movs	r2, #240
	add	r3, lr
	lsls	r2, r2, #8
	orrs	r3, r2
	adds	r1, #200
	str	r3, [r0, r1]
	ldr	r1, [sp, #48]
	movs	r3, #1
	add	fp, r3
	adds	r1, #1
	movs	r2, #16
	mov	r0, fp
	str	r1, [sp, #48]
	add	lr, r2
	adds	r5, #32
	cmp	r0, #5
	bne.n	.L_081b9c26
.L_081b9cca:
	ldr	r1, [sp, #96]
	cmp	r1, #3
	beq.n	.L_081b9cde
	cmp	r1, #5
	beq.n	.L_081b9cd6
	b.n	.L_081b9e70
.L_081b9cd6:
	ldr	r2, [sp, #100]
	cmp	r2, #0
	bne.n	.L_081b9cde
	b.n	.L_081b9e70
.L_081b9cde:
	ldr	r3, [sp, #84]
	ldr	r6, [pc, #544]
	lsls	r3, r3, #4
	mov	r8, r3
	add	r6, r8
	movs	r7, #48
	cmp	r6, #100
	ble.n	.L_081b9cf0
	movs	r6, #100
.L_081b9cf0:
	ldr	r4, [sp, #96]
	cmp	r4, #5
	bne.n	.L_081b9d28
	ldr	r6, [sp, #80]
	movs	r0, #128
	lsls	r5, r6, #12
	lsls	r0, r0, #5
	adds	r5, r5, r0
	adds	r0, r5, #0
	bl	sub_08002090
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #3
	mov	r1, r8
	asrs	r3, r3, #16
	subs	r3, r3, r1
	adds	r0, r5, #0
	adds	r6, r3, #0
	bl	sub_08002096
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #4
	asrs	r3, r3, #16
	adds	r6, #56
	subs	r7, r7, r3
	b.n	.L_081b9d90
.L_081b9d28:
	ldr	r2, [sp, #100]
	cmp	r2, #0
	beq.n	.L_081b9d66
	ldr	r3, [sp, #84]
	cmp	r3, #67
	ble.n	.L_081b9d90
	adds	r2, r3, #0
	movs	r3, #160
	lsls	r3, r3, #3
	subs	r2, #68
	adds	r3, #209
	adds	r5, r2, #0
	muls	r5, r3
	adds	r0, r5, #0
	bl	sub_08002090
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #4
	asrs	r3, r3, #16
	adds	r0, r5, #0
	adds	r6, r3, #0
	bl	sub_08002096
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #4
	asrs	r3, r3, #16
	adds	r6, #52
	subs	r7, r7, r3
	b.n	.L_081b9d90
.L_081b9d66:
	ldr	r4, [sp, #84]
	cmp	r4, #67
	ble.n	.L_081b9d90
	ldr	r1, [pc, #408]
	lsls	r0, r4, #12
	adds	r0, r0, r1
	lsls	r5, r4, #1
	bl	sub_08002096
	adds	r6, r5, #0
	subs	r6, #128
	adds	r3, r6, #0
	muls	r3, r0
	ldr	r2, [sp, #84]
	asrs	r3, r3, #16
	adds	r6, r3, #0
	ldr	r3, [pc, #388]
	adds	r5, r5, r2
	lsls	r5, r5, #1
	adds	r6, #100
	adds	r7, r5, r3
.L_081b9d90:
	movs	r4, #31
	negs	r4, r4
	cmp	r6, r4
	blt.n	.L_081b9dae
	movs	r3, #12
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	ldr	r0, [sp, #48]
	adds	r1, r6, #0
	adds	r2, r7, #0
	ldr	r3, [sp, #104]
	bl	sub_081b8274
	str	r0, [sp, #48]
.L_081b9dae:
	ldr	r5, [sp, #84]
	cmp	r5, #30
	bgt.n	.L_081b9dc0
	ldr	r6, [sp, #96]
	cmp	r6, #5
	bne.n	.L_081b9e70
	ldr	r0, [sp, #100]
	cmp	r0, #0
	beq.n	.L_081b9e70
.L_081b9dc0:
	ldr	r7, [pc, #332]
	ldr	r1, [sp, #84]
	movs	r0, #64
	add	r7, r8
	cmp	r1, #37
	ble.n	.L_081b9dce
	movs	r7, #60
.L_081b9dce:
	ldr	r2, [sp, #96]
	cmp	r2, #5
	bne.n	.L_081b9de4
	ldr	r4, [sp, #80]
	movs	r0, #64
	lsls	r3, r4, #1
	adds	r3, r3, r4
	lsls	r3, r3, #3
	adds	r7, r3, #0
	adds	r7, #60
	b.n	.L_081b9e10
.L_081b9de4:
	ldr	r5, [sp, #84]
	cmp	r5, #67
	ble.n	.L_081b9e10
	ldr	r1, [sp, #84]
	ldr	r2, [pc, #292]
	lsls	r0, r1, #12
	adds	r0, r0, r2
	bl	sub_08002096
	lsls	r5, r5, #1
	adds	r6, r5, #0
	subs	r6, #136
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #16
	adds	r7, r3, #0
	ldr	r3, [sp, #84]
	ldr	r4, [pc, #272]
	adds	r5, r5, r3
	lsls	r5, r5, #1
	adds	r7, #60
	adds	r0, r5, r4
.L_081b9e10:
	movs	r6, #128
	ldr	r5, [sp, #48]
	lsls	r6, r6, #2
	movs	r2, #128
	adds	r3, r7, r6
	lsls	r2, r2, #1
	subs	r6, #1
	ands	r3, r6
	adds	r0, r0, r2
	movs	r2, #255
	ands	r0, r2
	lsls	r1, r5, #3
	ldr	r2, [sp, #120]
	lsls	r3, r3, #16
	movs	r4, #192
	adds	r5, r1, #0
	orrs	r3, r0
	lsls	r4, r4, #24
	adds	r5, #196
	orrs	r3, r4
	str	r3, [r2, r5]
	movs	r3, #210
	adds	r1, #200
	lsls	r3, r3, #8
	str	r3, [r2, r1]
	ldr	r3, [sp, #48]
	movs	r5, #144
	adds	r3, #1
	lsls	r5, r5, #2
	str	r3, [sp, #48]
	lsls	r2, r3, #3
	adds	r3, r7, r5
	ands	r3, r6
	lsls	r3, r3, #16
	ldr	r6, [sp, #120]
	adds	r1, r2, #0
	orrs	r3, r0
	orrs	r3, r4
	adds	r1, #196
	str	r3, [r6, r1]
	movs	r3, #210
	lsls	r3, r3, #8
	adds	r2, #200
	adds	r3, #64
	str	r3, [r6, r2]
	ldr	r0, [sp, #48]
	adds	r0, #1
	str	r0, [sp, #48]
.L_081b9e70:
	movs	r1, #0
	mov	fp, r1
	add	r3, sp, #140
.L_081b9e76:
	movs	r4, #1
	mov	r2, fp
	add	fp, r4
	mov	r5, fp
	stmia	r3!, {r2}
	cmp	r5, #4
	bne.n	.L_081b9e76
	mov	r6, sp
	adds	r6, #144
	str	r6, [sp, #24]
	add	r7, sp, #140
	mov	fp, r4
	mov	ip, r7
.L_081b9e90:
	ldr	r1, [sp, #24]
	movs	r2, #1
	ldmia	r1!, {r5}
	negs	r2, r2
	adds	r0, r1, #0
	add	r2, fp
	str	r0, [sp, #24]
	mov	r9, r2
	cmp	r2, #0
	blt.n	.L_081b9efc
	lsls	r0, r2, #2
	ldr	r3, [r7, r0]
	lsls	r2, r3, #3
	subs	r2, r2, r3
	lsls	r3, r5, #3
	subs	r3, r3, r5
	lsls	r3, r3, #2
	adds	r1, r3, #0
	ldr	r3, [sp, #124]
	lsls	r2, r2, #2
	adds	r2, #8
	adds	r1, #8
	ldr	r2, [r3, r2]
	ldr	r3, [r3, r1]
	cmp	r2, r3
	ble.n	.L_081b9f20
	mov	r4, ip
	mov	lr, r1
	adds	r1, r0, r4
	adds	r4, r0, #0
.L_081b9ecc:
	movs	r6, #1
	ldr	r3, [r1, #0]
	negs	r6, r6
	add	r9, r6
	mov	r0, r9
	str	r3, [r1, #4]
	subs	r4, #4
	subs	r1, #4
	cmp	r0, #0
	blt.n	.L_081b9f1c
	ldr	r2, [r1, #0]
	ldr	r6, [sp, #124]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, #8
	ldr	r2, [r6, r3]
	mov	r3, lr
	ldr	r3, [r6, r3]
	adds	r0, r4, #0
	mov	r8, r3
	cmp	r2, r8
	bgt.n	.L_081b9ecc
	b.n	.L_081b9f20
.L_081b9efc:
	mov	r4, r9
	lsls	r0, r4, #2
	b.n	.L_081b9f20
	movs	r0, r0
	.4byte 0xfffffe88
	.4byte 0xfffc0000
	.4byte 0xfffffe98
	.4byte 0xfffffd94
	.4byte 0xfffbc000
	.2byte 0xfea8
	.2byte 0xffff
.L_081b9f1c:
	.2byte 0x464e
	lsls	r0, r6, #2
.L_081b9f20:
	adds	r3, r0, #4
	movs	r0, #1
	add	fp, r0
	mov	r1, fp
	str	r5, [r7, r3]
	cmp	r1, #4
	bne.n	.L_081b9e90
	ldr	r5, [sp, #48]
	add	r3, sp, #140
	mov	r8, r3
	lsls	r3, r5, #3
	adds	r6, r3, #0
	mov	r0, sp
	adds	r6, #200
	adds	r0, #196
	str	r6, [sp, #28]
	str	r0, [sp, #40]
	movs	r2, #0
	adds	r3, #196
	mov	fp, r2
	add	r4, sp, #156
	mov	r9, r3
.L_081b9f4c:
	mov	r1, r8
	ldr	r2, [r1, #0]
	adds	r1, r4, #0
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r2, [sp, #124]
	lsls	r3, r3, #2
	adds	r2, r2, r3
	mov	sl, r2
	mov	r0, sl
	str	r4, [sp, #12]
	bl	sub_08015778
	ldr	r4, [sp, #12]
	movs	r3, #94
	ldr	r2, [r4, #8]
	adds	r3, #255
	cmp	r2, r3
	bgt.n	.L_081b9f78
	adds	r3, #1
	str	r3, [r4, #8]
	adds	r2, r3, #0
.L_081b9f78:
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #138
	cmp	r2, r3
	ble.n	.L_081b9f86
	str	r3, [r4, #8]
	adds	r2, r3, #0
.L_081b9f86:
	mov	r5, r8
	ldr	r3, [r5, #0]
	ldr	r6, [pc, #868]
	ldr	r0, [sp, #40]
	lsls	r3, r3, #2
	adds	r2, r2, r6
	str	r2, [r0, r3]
	ldr	r7, [r5, #0]
	lsls	r3, r7, #2
	ldr	r5, [r0, r3]
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_081b9fa2
	adds	r3, r5, #3
.L_081b9fa2:
	asrs	r3, r3, #2
	adds	r6, r3, #0
	adds	r6, #64
	movs	r0, #192
	adds	r1, r6, #0
	lsls	r0, r0, #4
	str	r4, [sp, #12]
	bl	sub_08002054
	ldr	r4, [sp, #12]
	adds	r1, r6, #0
	ldr	r3, [r4, #0]
	str	r4, [sp, #12]
	subs	r2, r3, r0
	movs	r0, #128
	lsls	r0, r0, #4
	str	r2, [sp, #16]
	bl	sub_08002054
	ldr	r4, [sp, #12]
	movs	r1, #16
	ldr	r3, [r4, #4]
	negs	r1, r1
	subs	r0, r3, r0
	adds	r3, r0, #0
	adds	r3, #92
	ldr	r2, [sp, #16]
	cmp	r3, r1
	ble.n	.L_081ba038
	movs	r5, #158
	lsls	r5, r5, #2
	adds	r3, r2, r5
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r2, #255
	ands	r3, r2
	movs	r6, #174
	lsls	r3, r3, #16
	lsls	r1, r7, #25
	lsls	r6, r6, #1
	orrs	r1, r3
	movs	r2, #255
	adds	r3, r0, r6
	ands	r3, r2
	orrs	r1, r3
	ldr	r3, [pc, #756]
	ldr	r0, [sp, #120]
	orrs	r1, r3
	mov	r2, r9
	str	r1, [r0, r2]
	mov	r3, sl
	ldr	r0, [r3, #24]
	ldr	r2, [pc, #748]
	cmp	r0, #0
	bge.n	.L_081ba018
	movs	r5, #192
	lsls	r5, r5, #2
	adds	r5, #255
	adds	r0, r0, r5
.L_081ba018:
	asrs	r3, r0, #10
	ldrb	r3, [r2, r3]
	ldr	r0, [sp, #28]
	movs	r2, #128
	ldr	r6, [sp, #120]
	lsls	r2, r2, #3
	lsls	r3, r3, #3
	orrs	r3, r2
	str	r3, [r6, r0]
	ldr	r2, [sp, #48]
	adds	r0, #8
	adds	r2, #1
	str	r0, [sp, #28]
	str	r2, [sp, #48]
	movs	r1, #8
	add	r9, r1
.L_081ba038:
	movs	r5, #1
	add	fp, r5
	movs	r3, #4
	mov	r6, fp
	add	r8, r3
	cmp	r6, #4
	bne.n	.L_081b9f4c
	ldr	r5, [sp, #48]
	movs	r2, #128
	add	r3, sp, #212
	lsls	r2, r2, #20
	movs	r1, #168
	mov	r8, r3
	str	r2, [sp, #32]
	lsls	r3, r5, #3
	ldr	r4, [sp, #124]
	movs	r0, #0
	add	r1, sp
	adds	r3, #196
	mov	fp, r0
	mov	sl, r1
	add	r6, sp, #156
	mov	r9, r3
.L_081ba066:
	ldr	r3, [r4, #0]
	mov	r0, sl
	str	r3, [r0, #0]
	movs	r3, #0
	str	r3, [r0, #4]
	adds	r1, r6, #0
	ldr	r3, [r4, #8]
	str	r4, [sp, #12]
	str	r3, [r0, #8]
	bl	sub_08015778
	ldr	r3, [r6, #8]
	movs	r1, #94
	adds	r1, #255
	ldr	r4, [sp, #12]
	cmp	r3, r1
	bgt.n	.L_081ba08e
	movs	r3, #175
	lsls	r3, r3, #1
	str	r3, [r6, #8]
.L_081ba08e:
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #138
	cmp	r3, r2
	ble.n	.L_081ba09c
	str	r2, [r6, #8]
	adds	r3, r2, #0
.L_081ba09c:
	ldr	r2, [pc, #592]
	mov	r1, r8
	adds	r5, r3, r2
	mov	r3, r8
	str	r5, [r3, #0]
	str	r4, [sp, #12]
	movs	r0, #6
	ldrsh	r3, [r4, r0]
	movs	r0, #192
	adds	r5, r5, r3
	lsrs	r3, r5, #31
	str	r5, [r1, #0]
	adds	r5, r5, r3
	asrs	r5, r5, #1
	adds	r5, #128
	adds	r1, r5, #0
	lsls	r0, r0, #5
	bl	sub_08002054
	ldr	r3, [r6, #0]
	adds	r1, r5, #0
	subs	r7, r3, r0
	movs	r0, #192
	lsls	r0, r0, #4
	bl	sub_08002054
	ldr	r3, [r6, #4]
	movs	r2, #16
	subs	r1, r3, r0
	adds	r3, r1, #0
	adds	r3, #92
	negs	r2, r2
	ldr	r4, [sp, #12]
	cmp	r3, r2
	ble.n	.L_081ba122
	ldr	r3, [sp, #48]
	movs	r5, #158
	movs	r2, #128
	lsls	r5, r5, #2
	lsls	r2, r2, #1
	lsls	r0, r3, #3
	adds	r2, #255
	adds	r3, r7, r5
	ands	r3, r2
	ldr	r2, [sp, #32]
	movs	r5, #174
	lsls	r5, r5, #1
	lsls	r3, r3, #16
	orrs	r3, r2
	adds	r2, r1, r5
	movs	r1, #255
	ands	r2, r1
	orrs	r3, r2
	ldr	r2, [pc, #492]
	ldr	r1, [sp, #120]
	orrs	r3, r2
	mov	r2, r9
	str	r3, [r1, r2]
	movs	r3, #137
	lsls	r3, r3, #4
	adds	r0, #200
	str	r3, [r1, r0]
	ldr	r5, [sp, #48]
	movs	r3, #8
	adds	r5, #1
	str	r5, [sp, #48]
	add	r9, r3
.L_081ba122:
	ldr	r0, [sp, #32]
	movs	r1, #128
	movs	r3, #1
	lsls	r1, r1, #18
	add	fp, r3
	adds	r0, r0, r1
	movs	r2, #4
	mov	r5, fp
	str	r0, [sp, #32]
	add	r8, r2
	adds	r4, #28
	cmp	r5, #4
	bne.n	.L_081ba066
	ldr	r6, [sp, #48]
	cmp	r6, #128
	beq.n	.L_081ba162
	lsls	r3, r6, #3
	ldr	r0, [pc, #436]
	adds	r2, r3, #0
	movs	r1, #0
	adds	r2, #200
	adds	r3, #196
.L_081ba14e:
	ldr	r4, [sp, #120]
	str	r0, [r4, r3]
	str	r1, [r4, r2]
	ldr	r5, [sp, #48]
	adds	r2, #8
	adds	r5, #1
	adds	r3, #8
	str	r5, [sp, #48]
	cmp	r5, #128
	bne.n	.L_081ba14e
.L_081ba162:
	mov	r3, sp
	adds	r3, #196
	str	r3, [sp, #8]
	movs	r6, #0
	movs	r0, #224
	movs	r1, #216
	movs	r2, #208
	mov	fp, r6
	mov	sl, r6
	mov	r8, r0
	add	r6, sp, #132
	mov	lr, r1
	mov	ip, r2
	movs	r7, #200
.L_081ba17e:
	ldr	r5, [sp, #8]
	ldmia	r5!, {r3}
	adds	r4, r5, #0
	adds	r1, r3, #0
	str	r4, [sp, #8]
	cmp	r1, #0
	bge.n	.L_081ba18e
	adds	r3, r1, #3
.L_081ba18e:
	asrs	r3, r3, #2
	adds	r3, #128
	lsls	r3, r3, #16
	mov	r2, sp
	asrs	r3, r3, #16
	add	r0, sp, #136
	adds	r2, #138
	mov	r4, sl
	strh	r3, [r2, #0]
	strh	r4, [r0, #0]
	mov	r4, sp
	adds	r4, #134
	mov	r5, sl
	strh	r5, [r4, #0]
	strh	r3, [r6, #0]
	ldr	r1, [sp, #120]
	ldrh	r2, [r2, #0]
	ldr	r3, [r1, r7]
	lsls	r2, r2, #16
	orrs	r3, r2
	str	r3, [r1, r7]
	mov	r5, ip
	ldrh	r2, [r0, #0]
	ldr	r3, [r1, r5]
	lsls	r2, r2, #16
	orrs	r3, r2
	str	r3, [r1, r5]
	mov	r0, lr
	ldrh	r2, [r4, #0]
	ldr	r3, [r1, r0]
	lsls	r2, r2, #16
	orrs	r3, r2
	str	r3, [r1, r0]
	mov	r4, r8
	ldrh	r2, [r6, #0]
	ldr	r3, [r1, r4]
	lsls	r2, r2, #16
	movs	r0, #1
	orrs	r3, r2
	add	fp, r0
	movs	r5, #32
	str	r3, [r1, r4]
	mov	r1, fp
	add	r8, r5
	add	lr, r5
	add	ip, r5
	adds	r7, #32
	cmp	r1, #8
	bne.n	.L_081ba17e
	ldr	r0, [sp, #120]
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r1, #224
	adds	r3, #212
	adds	r0, #196
	lsls	r1, r1, #19
	ldr	r2, [pc, #256]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #108]
	adds	r2, #1
	str	r2, [sp, #108]
.L_081ba210:
	ldr	r4, [sp, #108]
	movs	r3, #0
	str	r3, [sp, #48]
	cmp	r4, #16
	bgt.n	.L_081ba23a
	movs	r6, #128
	lsls	r5, r4, #12
	lsls	r6, r6, #1
	ldr	r1, [pc, #224]
	adds	r2, r5, #0
	adds	r3, r6, #0
	ldr	r0, [sp, #128]
	bl	sub_081b80a8
	movs	r1, #160
	add	r0, sp, #244
	lsls	r1, r1, #19
	adds	r2, r5, #0
	adds	r3, r6, #0
	bl	sub_081b80a8
.L_081ba23a:
	bl	sub_08014de4
	ldr	r0, [sp, #116]
	ldr	r1, [sp, #36]
	bl	sub_080156e8
	ldr	r5, [sp, #96]
	cmp	r5, #0
	beq.n	.L_081ba250
	bl	.L_081b8a46
.L_081ba250:
	ldr	r6, [sp, #108]
	cmp	r6, #17
	bgt.n	.L_081ba25a
	bl	.L_081b9550
.L_081ba25a:
	ldr	r5, [pc, #172]
	bl	sub_080ad290
	ldr	r2, [r5, #16]
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	cmp	r2, r3
	bcc.n	.L_081ba270
	bl	.L_081b890a
.L_081ba270:
	ldr	r0, [sp, #120]
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #204
	adds	r3, r0, r1
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	sub_08038018
.L_081ba282:
	movs	r2, #0
	movs	r6, #128
	mov	fp, r2
	lsls	r6, r6, #1
.L_081ba28a:
	mov	r4, fp
	movs	r5, #128
	lsls	r3, r4, #12
	lsls	r5, r5, #9
	subs	r5, r5, r3
	ldr	r0, [sp, #128]
	adds	r2, r5, #0
	ldr	r1, [pc, #104]
	adds	r3, r6, #0
	bl	sub_081b80a8
	movs	r1, #160
	adds	r2, r5, #0
	add	r0, sp, #244
	lsls	r1, r1, #19
	adds	r3, r6, #0
	movs	r5, #1
	bl	sub_081b80a8
	add	fp, r5
	movs	r0, #1
	bl	sub_08013560
	mov	r0, fp
	cmp	r0, #17
	bne.n	.L_081ba28a
	movs	r0, #48
	bl	sub_0801314c
	movs	r0, #180
	bl	sub_0801314c
	movs	r0, #96
	bl	sub_0801314c
	movs	r0, #92
	bl	sub_0801314c
	movs	r0, #100
	bl	sub_0801314c
	movs	r3, #189
	lsls	r3, r3, #2
	add	sp, r3
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xfffffea2
	.4byte 0x40002300
	.4byte 0x081ba30c
	.4byte 0x40f02000
	.4byte 0x84000100
	.4byte 0x05000200
