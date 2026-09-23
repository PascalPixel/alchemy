.syntax unified
	.thumb
	.set sub_08001cb0, 0x08001cb0
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08118028, 0x08118028
	.set sub_08118038, 0x08118038
	.set sub_08118040, 0x08118040
	.set sub_081180f0, 0x081180f0
	.set sub_08138086, 0x08138086
	.set sub_0813efce, 0x0813efce
	.set sub_0813f278, 0x0813f278
	.set sub_0813f756, 0x0813f756
	.set sub_081434d8, 0x081434d8
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08143eb4, 0x08143eb4
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157cf4, 0x08157cf4
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e20c, 0x0815e20c
	.set sub_081963ec, 0x081963ec
	.set sub_08196404, 0x08196404
	.set sub_081c0010, 0x081c0010
	.global Func_0813eb70
	.thumb_func
Func_0813eb70:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #246
	sub	sp, #56
	lsls	r1, r1, #7
	str	r0, [sp, #52]
	adds	r1, #124
	movs	r0, #92
	bl	sub_08014cc0
	movs	r1, #128
	str	r0, [sp, #48]
	lsls	r1, r1, #7
	movs	r0, #96
	bl	sub_08014cc0
	str	r0, [sp, #44]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #36]
	adds	r3, #176
	str	r0, [sp, #28]
	ldr	r1, [sp, #48]
	ldr	r3, [r3, #0]
	movs	r2, #240
	ldr	r4, [sp, #52]
	lsls	r2, r2, #7
	movs	r5, #240
	adds	r2, #240
	lsls	r5, r5, #7
	mov	r8, r3
	adds	r5, #228
	adds	r3, r1, r2
	str	r4, [r3, #0]
	adds	r3, r1, r5
	movs	r5, #1
	str	r5, [r3, #0]
	bl	sub_081434d8
	ldr	r2, [pc, #92]
	mov	r7, r8
	movs	r3, #32
	str	r5, [r7, #12]
	strh	r3, [r2, #6]
	ldr	r0, [sp, #28]
	movs	r1, #206
	lsls	r1, r1, #3
	adds	r3, r0, r1
	ldrh	r1, [r3, #0]
	movs	r0, #1
	movs	r2, #0
	bl	sub_08118038
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #0
	movs	r3, #100
	movs	r0, #0
	bl	sub_08118028
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r5, #0
	adds	r3, #40
	str	r5, [r7, #12]
	movs	r2, #128
	str	r5, [r3, #0]
	ldr	r3, [pc, #32]
	lsls	r2, r2, #19
	adds	r2, #44
	str	r3, [r2, #0]
	ldr	r3, [pc, #16]
	subs	r2, #12
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #34
	strh	r5, [r3, #0]
	b.n	.L_0813ec30
	.4byte 0x00000784
	.4byte 0x00000080
	.4byte 0x03001120
	.2byte 0xf000
	.2byte 0xffff
.L_0813ec30:
	adds	r3, #2
	strh	r5, [r3, #0]
	ldr	r3, [pc, #52]
	adds	r2, #6
	strh	r3, [r2, #0]
	ldr	r1, [pc, #52]
	movs	r3, #128
	ldr	r2, [pc, #52]
	lsls	r3, r3, #19
	adds	r3, #64
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	subs	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #72
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	adds	r2, #2
	strh	r3, [r2, #0]
	movs	r6, #0
	ldr	r2, [pc, #24]
	movs	r3, #128
	b.n	.L_0813ec84
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x000000f0
	.4byte 0x00001088
	.4byte 0x00003537
	.4byte 0x00003f21
	.2byte 0x3800
	.2byte 0x0600
.L_0813ec84:
	movs	r4, #128
	str	r6, [sp, #20]
	lsls	r4, r4, #2
	lsls	r3, r3, #1
	mov	sl, r4
	mov	ip, r2
	mov	lr, r3
	movs	r4, #0
.L_0813ec94:
	movs	r7, #0
	str	r7, [sp, #24]
	mov	r1, lr
	adds	r0, r5, r1
	lsls	r1, r4, #1
.L_0813ec9e:
	adds	r3, r0, #0
	orrs	r3, r1
	lsls	r3, r3, #16
	mov	r7, ip
	adds	r2, r6, r7
	asrs	r3, r3, #16
	strh	r3, [r2, #0]
	ldr	r2, [sp, #24]
	add	r0, sl
	adds	r2, #1
	adds	r1, #2
	str	r2, [sp, #24]
	adds	r6, #2
	cmp	r2, #8
	bne.n	.L_0813ec9e
	ldr	r7, [sp, #20]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r7, #1
	adds	r5, r5, r3
	adds	r4, #8
	str	r7, [sp, #20]
	cmp	r7, #16
	bne.n	.L_0813ec94
	movs	r1, #128
	ldr	r5, [pc, #140]
	ldr	r0, [sp, #44]
	lsls	r1, r1, #7
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2180
	lsls	r1, r1, #7
	ldr	r0, [pc, #132]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4920
	ldr	r0, [pc, #132]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0813ed16
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
.L_0813ed16:
	strh	r4, [r0, #0]
	movs	r2, #128
	ldr	r3, [pc, #60]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r2, [sp, #48]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #60]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r4, [sp, #48]
	movs	r5, #239
	movs	r7, #238
	lsls	r5, r5, #7
	lsls	r7, r7, #7
	adds	r2, r4, r5
	movs	r3, #1
	adds	r7, #132
	str	r3, [r2, #0]
	movs	r1, #200
	adds	r2, r4, r7
	movs	r3, #0
	str	r3, [r2, #0]
	b.n	.L_0813ed74
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x00000000
	.4byte 0x03000258
	.4byte 0x06004000
	.4byte 0x020038e0
	.4byte 0x04000208
	.2byte 0x0106
	.2byte 0x0000
.L_0813ed74:
	lsls	r1, r1, #4
	ldr	r0, [pc, #408]
	bl	sub_080145a8
	movs	r1, #35
	movs	r0, #104
	bl	sub_081963ec
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #104]
	movs	r1, #19
	str	r0, [sp, #32]
	movs	r0, #188
	bl	sub_081963ec
	adds	r5, #188
	ldr	r5, [r5, #0]
	movs	r1, #0
	str	r5, [sp, #36]
	str	r1, [sp, #12]
	ldr	r7, [pc, #372]
	ldr	r4, [pc, #372]
	ldr	r0, [pc, #376]
	ldr	r1, [sp, #48]
	ldr	r5, [pc, #376]
	movs	r6, #0
.L_0813edaa:
	ldrb	r2, [r0, #0]
	ldrb	r3, [r4, #0]
	lsls	r2, r2, #16
	lsls	r3, r3, #16
	str	r2, [r1, #0]
	str	r3, [r1, #4]
	adds	r2, r2, r7
	adds	r3, r3, r5
	asrs	r2, r2, #2
	asrs	r3, r3, #2
	adds	r6, #1
	str	r2, [r1, #12]
	str	r3, [r1, #16]
	adds	r0, #1
	adds	r4, #1
	adds	r1, #28
	cmp	r6, #33
	bne.n	.L_0813edaa
	movs	r2, #240
	ldr	r3, [pc, #336]
	ldr	r1, [pc, #340]
	lsls	r2, r2, #7
	ldr	r0, [pc, #340]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x21f0
	ldr	r5, [pc, #336]
	lsls	r1, r1, #7
	ldr	r2, [pc, #336]
	ldr	r0, [pc, #324]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4642
	movs	r3, #1
	str	r3, [r2, #16]
	ldr	r3, [sp, #48]
	ldr	r2, [pc, #324]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #160
	adds	r1, r3, r4
	ldrh	r3, [r2, #4]
	movs	r0, #238
	str	r3, [r1, #0]
	ldr	r7, [sp, #48]
	ldrh	r3, [r2, #6]
	lsls	r0, r0, #7
	adds	r0, #164
	adds	r1, r7, r0
	str	r3, [r1, #0]
	ldr	r1, [pc, #300]
	movs	r3, #0
	strh	r3, [r2, #4]
	ldr	r0, [pc, #296]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0813ee46
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #248
	adds	r3, r3, r1
	lsls	r2, r2, #5
	adds	r3, #4
	adds	r2, #129
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #10
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0813ee46:
	strh	r4, [r0, #0]
	movs	r0, #160
	lsls	r0, r0, #19
	movs	r1, #128
	lsls	r1, r1, #1
	ldr	r2, [pc, #240]
	adds	r0, #192
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x20d4
	bl	sub_081c0010
	ldr	r2, [sp, #52]
	movs	r3, #30
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	movs	r2, #3
	bl	sub_0814cd48
	movs	r3, #0
	str	r3, [sp, #40]
.L_0813ee76:
	ldr	r4, [sp, #40]
	cmp	r4, #2
	bne.n	.L_0813ee82
	movs	r0, #212
	bl	sub_081c0010
.L_0813ee82:
	ldr	r5, [sp, #40]
	cmp	r5, #3
	bne.n	.L_0813ee8e
	movs	r0, #212
	bl	sub_081c0010
.L_0813ee8e:
	ldr	r7, [sp, #40]
	cmp	r7, #28
	bne.n	.L_0813eeaa
	ldr	r2, [sp, #52]
	movs	r3, #1
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	negs	r3, r3
	movs	r2, #0
	str	r2, [sp, #0]
	adds	r1, r3, #0
	movs	r2, #3
	bl	sub_0814cd48
.L_0813eeaa:
	ldr	r3, [sp, #40]
	cmp	r3, #32
	bne.n	.L_0813eeb6
	movs	r0, #149
	bl	sub_081c0010
.L_0813eeb6:
	ldr	r4, [sp, #40]
	cmp	r4, #5
	bne.n	.L_0813eee6
	movs	r0, #145
	bl	sub_081c0010
	ldr	r5, [sp, #48]
	movs	r7, #238
	lsls	r7, r7, #7
	adds	r7, #160
	adds	r3, r5, r7
	ldr	r2, [pc, #104]
	ldr	r3, [r3, #0]
	movs	r1, #206
	strh	r3, [r2, #4]
	ldr	r0, [sp, #28]
	lsls	r1, r1, #3
	adds	r3, r0, r1
	movs	r2, #1
	ldrh	r1, [r3, #0]
	movs	r0, #1
	negs	r2, r2
	bl	sub_08118040
.L_0813eee6:
	ldr	r2, [sp, #40]
	cmp	r2, #7
	ble.n	.L_0813efba
	ldr	r7, [sp, #28]
	movs	r0, #160
	movs	r3, #160
	ldr	r4, [pc, #24]
	lsls	r0, r0, #3
	lsls	r3, r3, #19
	adds	r0, #108
	adds	r3, #192
	movs	r5, #31
	adds	r7, r7, r0
	mov	lr, r3
	movs	r6, #0
	mov	ip, r4
	mov	r8, r5
	mov	r9, r7
	b.n	.L_0813ef48
	.4byte 0x0000001f
	.4byte 0x08143001
	.4byte 0xffe00000
	.4byte 0x081976b1
	.4byte 0x08197690
	.4byte 0xffc40000
	.4byte 0x03000730
	.4byte 0x06008000
	.4byte 0x02010000
	.4byte 0x03000260
	.4byte 0x01010101
	.4byte 0x03001120
	.4byte 0x020038e0
	.4byte 0x04000208
	.2byte 0x7fff
	.2byte 0x7fff
.L_0813ef48:
	mov	r1, lr
	ldrh	r3, [r1, #0]
	mov	r5, r8
	mov	r7, r9
	ands	r5, r3
	lsls	r3, r3, #16
	lsrs	r2, r3, #21
	lsrs	r0, r3, #26
	ldrh	r3, [r7, #0]
	movs	r1, #2
	add	r9, r1
	mov	r1, r8
	ands	r1, r3
	lsls	r3, r3, #16
	mov	r4, ip
	mov	sl, r3
	mov	r7, sl
	ands	r2, r4
	ands	r0, r4
	lsrs	r4, r3, #21
	mov	r3, ip
	ands	r4, r3
	lsrs	r3, r7, #26
	mov	r7, ip
	ands	r3, r7
	cmp	r5, r1
	bge.n	.L_0813ef82
	adds	r5, #1
	b.n	.L_0813ef88
.L_0813ef82:
	cmp	r5, r1
	ble.n	.L_0813ef88
	subs	r5, #1
.L_0813ef88:
	cmp	r2, r4
	bge.n	.L_0813ef90
	adds	r2, #1
	b.n	.L_0813ef96
.L_0813ef90:
	cmp	r2, r4
	ble.n	.L_0813ef96
	subs	r2, #1
.L_0813ef96:
	cmp	r0, r3
	bge.n	.L_0813ef9e
	adds	r0, #1
	b.n	.L_0813efa4
.L_0813ef9e:
	cmp	r0, r3
	ble.n	.L_0813efa4
	subs	r0, #1
.L_0813efa4:
	lsls	r3, r0, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	mov	r0, lr
	orrs	r3, r5
	movs	r1, #2
	adds	r6, #1
	strh	r3, [r0, #0]
	add	lr, r1
	cmp	r6, #128
	bne.n	.L_0813ef48
.L_0813efba:
	ldr	r2, [sp, #40]
	cmp	r2, #4
	bne.n	sub_0813efce
	movs	r1, #240
	ldr	r3, [pc, #852]
	ldr	r0, [pc, #852]
	lsls	r1, r1, #7
	ldr	r2, [pc, #852]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9b0a
	cmp	r3, #3
	ble.n	.L_0813efd6
	b.n	sub_0813f278
.L_0813efd6:
	lsls	r1, r3, #2
	adds	r1, #8
	lsls	r4, r3, #5
	movs	r0, #160
	lsls	r3, r1, #10
	lsls	r2, r1, #5
	lsls	r0, r0, #19
	orrs	r3, r2
	adds	r0, #4
	orrs	r3, r1
	str	r4, [sp, #16]
	strh	r3, [r0, #0]
	ldr	r5, [sp, #12]
	cmp	r5, r4
	bne.n	.L_0813eff6
	b.n	.L_0813f26a
.L_0813eff6:
	ldr	r7, [sp, #12]
	movs	r0, #0
	adds	r1, r7, #0
	mov	sl, r7
	mov	fp, r0
	str	r7, [sp, #8]
	cmp	r1, #0
	bge.n	.L_0813f008
	b.n	.L_0813f25c
.L_0813f008:
	ldr	r3, [sp, #24]
	mov	r4, sl
	ldr	r5, [sp, #20]
	ldr	r2, [sp, #24]
	subs	r0, r3, r4
	ldr	r4, [sp, #20]
	mov	r7, fp
	subs	r6, r5, r7
	add	r2, sl
	add	r4, fp
	cmp	r6, #0
	bge.n	.L_0813f022
	movs	r6, #0
.L_0813f022:
	cmp	r4, #119
	ble.n	.L_0813f028
	movs	r4, #119
.L_0813f028:
	cmp	r0, #0
	bge.n	.L_0813f02e
	movs	r0, #0
.L_0813f02e:
	cmp	r2, #255
	ble.n	.L_0813f034
	movs	r2, #255
.L_0813f034:
	movs	r5, #7
	ands	r5, r2
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_0813f040
	adds	r3, r2, #7
.L_0813f040:
	movs	r2, #7
	asrs	r1, r3, #3
	ands	r2, r4
	adds	r3, r4, #0
	cmp	r4, #0
	bge.n	.L_0813f04e
	adds	r3, r4, #7
.L_0813f04e:
	asrs	r3, r3, #3
	lsls	r3, r3, #11
	mov	ip, r3
	lsls	r3, r1, #6
	lsls	r7, r2, #3
	ldr	r1, [pc, #712]
	adds	r4, r3, r5
	adds	r3, r7, r4
	add	r3, ip
	adds	r3, r3, r1
	movs	r2, #2
	movs	r1, #7
	strb	r2, [r3, #0]
	ands	r1, r6
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L_0813f072
	adds	r3, r6, #7
.L_0813f072:
	asrs	r3, r3, #3
	lsls	r5, r1, #3
	ldr	r2, [pc, #684]
	lsls	r6, r3, #11
	adds	r3, r5, r4
	adds	r3, r6, r3
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	movs	r3, #7
	ands	r3, r0
	adds	r1, r0, #0
	cmp	r0, #0
	bge.n	.L_0813f090
	adds	r1, r0, #7
.L_0813f090:
	asrs	r1, r1, #3
	lsls	r1, r1, #6
	adds	r1, r1, r3
	ldr	r4, [pc, #652]
	adds	r3, r7, r1
	add	r3, ip
	adds	r1, r5, r1
	adds	r3, r3, r4
	movs	r2, #2
	adds	r1, r6, r1
	strb	r2, [r3, #0]
	adds	r1, r1, r4
	movs	r3, #2
	strb	r3, [r1, #0]
	movs	r0, #96
	mov	r1, sl
	subs	r3, r0, r1
	mov	r2, sl
	adds	r0, r3, #1
	adds	r2, #97
	cmp	r0, #0
	bge.n	.L_0813f0be
	movs	r0, #0
.L_0813f0be:
	cmp	r2, #255
	ble.n	.L_0813f0c4
	movs	r2, #255
.L_0813f0c4:
	movs	r1, #7
	ands	r1, r2
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_0813f0d0
	adds	r3, r2, #7
.L_0813f0d0:
	asrs	r3, r3, #3
	lsls	r3, r3, #6
	adds	r3, r3, r1
	ldr	r4, [pc, #588]
	adds	r2, r7, r3
	adds	r3, r5, r3
	add	r2, ip
	adds	r3, r6, r3
	movs	r1, #2
	adds	r3, r3, r4
	adds	r2, r2, r4
	strb	r1, [r2, #0]
	strb	r1, [r3, #0]
	movs	r3, #7
	ands	r3, r0
	adds	r1, r0, #0
	cmp	r0, #0
	bge.n	.L_0813f0f6
	adds	r1, r0, #7
.L_0813f0f6:
	asrs	r1, r1, #3
	lsls	r1, r1, #6
	adds	r1, r1, r3
	adds	r3, r7, r1
	ldr	r7, [pc, #548]
	ldr	r0, [pc, #544]
	adds	r1, r5, r1
	add	r3, ip
	adds	r3, r3, r7
	movs	r2, #2
	adds	r1, r6, r1
	strb	r2, [r3, #0]
	adds	r1, r1, r0
	movs	r3, #2
	strb	r3, [r1, #0]
	movs	r1, #96
	mov	r3, fp
	mov	r2, fp
	subs	r0, r1, r3
	mov	r4, sl
	movs	r5, #60
	mov	r7, sl
	adds	r2, #96
	mov	r9, r0
	adds	r4, #60
	subs	r6, r5, r7
	cmp	r0, #0
	bge.n	.L_0813f130
	movs	r0, #0
.L_0813f130:
	cmp	r2, #255
	ble.n	.L_0813f136
	movs	r2, #255
.L_0813f136:
	cmp	r6, #0
	bge.n	.L_0813f13c
	movs	r6, #0
.L_0813f13c:
	cmp	r4, #119
	ble.n	.L_0813f142
	movs	r4, #119
.L_0813f142:
	movs	r5, #7
	ands	r5, r2
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_0813f14e
	adds	r3, r2, #7
.L_0813f14e:
	movs	r2, #7
	asrs	r1, r3, #3
	ands	r2, r4
	adds	r3, r4, #0
	cmp	r4, #0
	bge.n	.L_0813f15c
	adds	r3, r4, #7
.L_0813f15c:
	asrs	r3, r3, #3
	lsls	r3, r3, #11
	mov	r8, r3
	lsls	r3, r1, #6
	lsls	r2, r2, #3
	ldr	r1, [pc, #444]
	adds	r5, r3, r5
	adds	r3, r2, r5
	add	r3, r8
	adds	r3, r3, r1
	mov	lr, r2
	movs	r1, #7
	movs	r2, #2
	strb	r2, [r3, #0]
	ands	r1, r6
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L_0813f182
	adds	r3, r6, #7
.L_0813f182:
	asrs	r3, r3, #3
	lsls	r3, r3, #11
	lsls	r7, r1, #3
	ldr	r2, [pc, #408]
	mov	ip, r3
	adds	r3, r7, r5
	add	r3, ip
	adds	r3, r3, r2
	movs	r5, #2
	strb	r5, [r3, #0]
	movs	r3, #7
	ands	r3, r0
	adds	r1, r0, #0
	cmp	r0, #0
	bge.n	.L_0813f1a2
	adds	r1, r0, #7
.L_0813f1a2:
	asrs	r1, r1, #3
	lsls	r1, r1, #6
	adds	r1, r1, r3
	mov	r0, lr
	ldr	r2, [pc, #376]
	adds	r3, r0, r1
	add	r3, r8
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	ldr	r3, [pc, #364]
	adds	r1, r7, r1
	add	r1, ip
	mov	r0, r9
	adds	r1, r1, r3
	mov	r2, fp
	movs	r3, #2
	adds	r0, #1
	strb	r3, [r1, #0]
	adds	r2, #97
	cmp	r0, #0
	bge.n	.L_0813f1d0
	movs	r0, #0
.L_0813f1d0:
	cmp	r2, #255
	ble.n	.L_0813f1d6
	movs	r2, #255
.L_0813f1d6:
	movs	r5, #7
	adds	r1, r2, #0
	mov	r9, r5
	ands	r1, r5
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_0813f1e6
	adds	r3, r2, #7
.L_0813f1e6:
	asrs	r3, r3, #3
	lsls	r3, r3, #6
	adds	r1, r3, r1
	mov	r5, lr
	adds	r3, r5, r1
	ldr	r5, [pc, #304]
	add	r3, r8
	adds	r3, r3, r5
	movs	r5, #2
	strb	r5, [r3, #0]
	adds	r3, r7, r1
	ldr	r1, [pc, #292]
	add	r3, ip
	adds	r3, r3, r1
	strb	r5, [r3, #0]
	adds	r2, r0, #0
	mov	r3, r9
	ands	r2, r3
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L_0813f212
	adds	r3, r0, #7
.L_0813f212:
	asrs	r3, r3, #3
	lsls	r3, r3, #6
	adds	r1, r3, r2
	mov	r4, lr
	ldr	r2, [pc, #264]
	adds	r3, r4, r1
	add	r3, r8
	adds	r3, r3, r2
	ldr	r4, [pc, #256]
	strb	r5, [r3, #0]
	adds	r3, r7, r1
	add	r3, ip
	adds	r3, r3, r4
	movs	r5, #2
	strb	r5, [r3, #0]
	ldr	r0, [sp, #8]
	mov	r7, fp
	lsls	r3, r7, #1
	subs	r3, r0, r3
	subs	r3, #1
	str	r3, [sp, #8]
	cmp	r3, #0
	bge.n	.L_0813f252
	ldr	r2, [sp, #8]
	mov	r1, sl
	lsls	r3, r1, #1
	adds	r3, r2, r3
	subs	r3, #2
	str	r3, [sp, #8]
	movs	r3, #1
	negs	r3, r3
	add	sl, r3
.L_0813f252:
	movs	r4, #1
	add	fp, r4
	cmp	sl, fp
	blt.n	.L_0813f25c
	b.n	.L_0813f008
.L_0813f25c:
	ldr	r5, [sp, #12]
	ldr	r7, [sp, #16]
	adds	r5, #1
	str	r5, [sp, #12]
	cmp	r5, r7
	beq.n	.L_0813f26a
	b.n	.L_0813eff6
.L_0813f26a:
	movs	r2, #240
	ldr	r3, [pc, #184]
	ldr	r0, [pc, #172]
	ldr	r1, [pc, #176]
	lsls	r2, r2, #7
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x980a
	cmp	r0, #50
	bgt.n	.L_0813f2c8
	ldr	r5, [sp, #48]
	movs	r6, #0
.L_0813f282:
	ldr	r2, [pc, #168]
	ldr	r0, [pc, #168]
	lsls	r3, r6, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #48]
	ldrb	r0, [r0, r6]
	movs	r3, #224
	adds	r1, r2, r1
	lsls	r3, r3, #3
	movs	r4, #2
	ldrsh	r2, [r5, r4]
	adds	r1, r1, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	ldr	r0, [pc, #144]
	ldr	r4, [sp, #32]
	ldrb	r0, [r0, r6]
	str	r0, [sp, #4]
	ldr	r0, [sp, #44]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9f0a
	cmp	r7, #3
	ble.n	.L_0813f2c0
	movs	r2, #128
	adds	r0, r5, #0
	movs	r1, #64
	lsls	r2, r2, #7
	bl	sub_08138086
.L_0813f2c0:
	adds	r6, #1
	adds	r5, #28
	cmp	r6, #33
	bne.n	.L_0813f282
.L_0813f2c8:
	ldr	r1, [sp, #40]
	subs	r1, #8
	cmp	r1, #42
	bhi.n	.L_0813f2e8
	adds	r0, r1, #0
	cmp	r0, #31
	ble.n	.L_0813f2d8
	movs	r0, #31
.L_0813f2d8:
	movs	r3, #160
	lsls	r2, r0, #10
	lsls	r1, r0, #5
	lsls	r3, r3, #19
	orrs	r2, r1
	adds	r3, #2
	orrs	r2, r0
	strh	r2, [r3, #0]
.L_0813f2e8:
	ldr	r0, [sp, #40]
	cmp	r0, #51
	bne.n	.L_0813f3c6
	ldr	r2, [sp, #48]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #64]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r0, #160
	lsls	r0, r0, #19
	movs	r6, #1
	adds	r0, #2
.L_0813f308:
	lsrs	r3, r6, #31
	adds	r3, r6, r3
	asrs	r1, r3, #1
	cmp	r1, #0
	bge.n	.L_0813f33c
	movs	r1, #0
	b.n	.L_0813f33c
	movs	r0, r0
	.4byte 0x03000260
	.4byte 0x06008000
	.4byte 0x02020202
	.4byte 0x02010000
	.4byte 0x03000730
	.4byte 0x0819764c
	.4byte 0x08197609
	.4byte 0x0819762a
	.2byte 0x013e
	.2byte 0x0000
.L_0813f33c:
	lsrs	r3, r1, #31
	adds	r3, r1, r3
	asrs	r3, r3, #1
	lsls	r2, r1, #10
	lsls	r3, r3, #5
	orrs	r2, r3
	orrs	r2, r1
	adds	r6, #1
	strh	r2, [r0, #0]
	adds	r0, #2
	cmp	r6, #64
	bne.n	.L_0813f308
	movs	r2, #128
	ldr	r3, [pc, #60]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r5, [sp, #48]
	movs	r4, #31
	movs	r6, #0
	mov	r8, r4
	movs	r7, #0
.L_0813f368:
	bl	sub_08014878
	mov	r1, r8
	ands	r0, r1
	adds	r0, #32
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	sub_08014878
	mov	r2, r8
	ands	r0, r2
	adds	r0, #80
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08014878
	movs	r3, #128
	lsls	r3, r3, #1
	ldr	r4, [pc, #8]
	adds	r3, #255
	ands	r3, r0
	b.n	.L_0813f39c
	.4byte 0x00003f44
	.2byte 0xff00
	.2byte 0xffff
.L_0813f39c:
	adds	r3, r3, r4
	lsls	r3, r3, #12
	adds	r6, #1
	str	r3, [r5, #12]
	str	r7, [r5, #16]
	str	r7, [r5, #24]
	adds	r5, #28
	cmp	r6, #32
	bne.n	.L_0813f368
	ldr	r5, [sp, #48]
	movs	r7, #239
	movs	r0, #238
	lsls	r7, r7, #7
	lsls	r0, r0, #7
	adds	r2, r5, r7
	movs	r3, #2
	adds	r0, #132
	str	r3, [r2, #0]
	adds	r2, r5, r0
	movs	r3, #50
	str	r3, [r2, #0]
.L_0813f3c6:
	ldr	r1, [sp, #40]
	cmp	r1, #52
	ble.n	.L_0813f438
	ldr	r5, [sp, #48]
	movs	r6, #0
.L_0813f3d0:
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L_0813f3d8
	adds	r3, r6, #3
.L_0813f3d8:
	ldr	r2, [sp, #40]
	asrs	r3, r3, #2
	adds	r3, #52
	cmp	r2, r3
	blt.n	.L_0813f430
	ldr	r3, [r5, #24]
	cmp	r3, #39
	bgt.n	.L_0813f430
	adds	r1, r3, #0
	cmp	r1, #0
	bge.n	.L_0813f3f0
	adds	r1, #3
.L_0813f3f0:
	asrs	r1, r1, #2
	cmp	r1, #5
	ble.n	.L_0813f3f8
	movs	r1, #5
.L_0813f3f8:
	ldr	r3, [sp, #48]
	lsls	r1, r1, #11
	movs	r7, #2
	ldrsh	r2, [r5, r7]
	adds	r1, r3, r1
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	movs	r4, #224
	movs	r0, #32
	str	r0, [sp, #0]
	lsls	r4, r4, #3
	movs	r0, #64
	adds	r1, r1, r4
	subs	r3, #32
	str	r0, [sp, #4]
	subs	r2, #16
	ldr	r0, [sp, #44]
	ldr	r4, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #60
	ldr	r2, [pc, #220]
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_0813f430:
	adds	r6, #1
	adds	r5, #28
	cmp	r6, #32
	bne.n	.L_0813f3d0
.L_0813f438:
	bl	sub_081434f8
	movs	r7, #240
	ldr	r5, [sp, #48]
	lsls	r7, r7, #7
	adds	r7, #232
	adds	r2, r5, r7
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	ldr	r0, [sp, #40]
	adds	r0, #1
	str	r0, [sp, #40]
	cmp	r0, #128
	beq.n	.L_0813f45c
	b.n	.L_0813ee76
.L_0813f45c:
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #156]
	bl	sub_08014644
	ldr	r2, [sp, #52]
	movs	r3, #1
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	negs	r3, r3
	movs	r2, #0
	str	r2, [sp, #0]
	adds	r1, r3, #0
	movs	r2, #1
	bl	sub_0814cd48
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #160
	adds	r3, r5, r4
	ldr	r2, [pc, #124]
	ldr	r3, [r3, #0]
	movs	r7, #206
	strh	r3, [r2, #4]
	movs	r3, #32
	strh	r3, [r2, #6]
	ldr	r5, [sp, #28]
	lsls	r7, r7, #3
	adds	r3, r5, r7
	ldrh	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #2
	bl	sub_08118038
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [pc, #96]
	ldr	r0, [pc, #96]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0813f4e0
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
.L_0813f4e0:
	strh	r4, [r0, #0]
	movs	r0, #100
	bl	sub_0801314c
	movs	r0, #96
	bl	sub_0801314c
	movs	r0, #92
	bl	sub_0801314c
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xfffff000
	.4byte 0x08143001
	.4byte 0x03001120
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #246
	sub	sp, #40
	lsls	r1, r1, #7
	str	r0, [sp, #24]
	adds	r1, #124
	movs	r0, #92
	bl	sub_08014cc0
	movs	r1, #128
	mov	fp, r0
	lsls	r1, r1, #7
	movs	r0, #96
	bl	sub_08014cc0
	movs	r1, #192
	lsls	r1, r1, #3
	str	r0, [sp, #20]
	adds	r1, #14
	movs	r0, #100
	bl	sub_08014cc0
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #228
	add	r2, fp
	movs	r3, #1
	str	r0, [sp, #8]
	str	r3, [r2, #0]
	movs	r3, #240
	ldr	r0, [sp, #24]
	lsls	r3, r3, #7
	adds	r3, #240
	add	r3, fp
	str	r0, [r3, #0]
	movs	r0, #0
	bl	sub_081435e0
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #180
	add	r2, fp
	movs	r3, #24
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #184
	add	r2, fp
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	movs	r1, #224
	subs	r2, #50
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	ldr	r0, [pc, #40]
	add	r1, fp
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r3, #0
	ldr	r1, [sp, #8]
	ldr	r0, [pc, #28]
	movs	r2, #0
	bl	sub_08157cf4
	ldr	r1, [sp, #24]
	ldr	r3, [r1, #0]
	cmp	r3, #1
	beq.n	.L_0813f5e4
	b.n	.L_0813f5d0
	movs	r0, r0
	.4byte 0x0000100c
	.4byte 0x00000100
	.4byte 0x00000107
	.2byte 0x0137
	.2byte 0x0000
.L_0813f5d0:
	cmp	r3, #1
	bgt.n	.L_0813f5da
	cmp	r3, #0
	beq.n	.L_0813f5e0
	b.n	.L_0813f5f8
.L_0813f5da:
	cmp	r3, #2
	beq.n	.L_0813f5e8
	b.n	.L_0813f5f8
.L_0813f5e0:
	ldr	r0, [pc, #8]
	b.n	.L_0813f5fa
.L_0813f5e4:
	ldr	r0, [pc, #8]
	b.n	.L_0813f5fa
.L_0813f5e8:
	ldr	r0, [pc, #8]
	b.n	.L_0813f5fa
	.4byte 0x0000010a
	.4byte 0x00000119
	.2byte 0x0109
	.2byte 0x0000
.L_0813f5f8:
	ldr	r0, [pc, #280]
.L_0813f5fa:
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #276]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4d43
	movs	r7, #0
.L_0813f610:
	movs	r3, #128
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	movs	r2, #128
	ands	r3, r0
	lsls	r2, r2, #3
	adds	r3, r3, r2
	str	r3, [r5, #8]
	negs	r3, r7
	adds	r7, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #128
	bne.n	.L_0813f610
	movs	r7, #0
	mov	r5, fp
.L_0813f648:
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r3, #16
	str	r3, [r5, #4]
	movs	r3, #15
	ands	r3, r7
	adds	r3, #16
	adds	r7, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #64
	bne.n	.L_0813f648
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
	ldr	r0, [pc, #144]
	bl	sub_080145a8
	movs	r1, #7
	movs	r0, #104
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r0, #140
	str	r3, [sp, #12]
	bl	sub_081c0010
	movs	r4, #28
	movs	r3, #0
	add	r4, sp
	mov	sl, r3
	mov	r9, r4
.L_0813f6b2:
	ldr	r5, [sp, #24]
	mov	r1, r9
	ldr	r0, [r5, #8]
	bl	sub_0815e20c
	mov	r0, r9
	ldr	r2, [r0, #0]
	movs	r1, #128
	movs	r3, #64
	lsls	r1, r1, #19
	subs	r3, r3, r2
	adds	r1, #40
	lsls	r3, r3, #8
	str	r3, [r1, #0]
	mov	r1, sl
	cmp	r1, #49
	ble.n	.L_0813f6e8
	mov	r3, sl
	lsls	r2, r3, #1
	ldr	r3, [pc, #48]
	movs	r1, #128
	subs	r3, r3, r2
	ldr	r2, [pc, #48]
	lsls	r1, r1, #19
	adds	r1, #82
	orrs	r3, r2
	strh	r3, [r1, #0]
.L_0813f6e8:
	mov	r4, sl
	cmp	r4, #26
	bne.n	.L_0813f724
	movs	r0, #212
	bl	sub_081c0010
	ldr	r1, [sp, #24]
	movs	r3, #20
	movs	r2, #1
	movs	r5, #36
	ldrsh	r0, [r1, r5]
	negs	r2, r2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	bl	sub_0814cd48
	b.n	.L_0813f724
	.4byte 0x00000070
	.4byte 0x00001000
	.4byte 0x00000108
	.4byte 0x03000730
	.4byte 0x02010000
	.2byte 0x3001
	.2byte 0x0814
.L_0813f724:
	mov	r0, sl
	subs	r0, #28
	cmp	r0, #20
	bhi.n	sub_0813f756
	movs	r1, #3
	bl	sub_08002054
	lsls	r1, r0, #3
	adds	r1, r1, r0
	mov	r4, r9
	ldr	r3, [r4, #4]
	lsls	r1, r1, #8
	movs	r2, #216
	lsls	r2, r2, #5
	add	r1, fp
	adds	r1, r1, r2
	movs	r2, #48
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	subs	r3, #24
	ldr	r0, [sp, #20]
	movs	r2, #40
	ldr	r5, [sp, #12]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4650
	cmp	r0, #14
	bhi.n	.L_0813f7c0
	movs	r1, #3
	bl	sub_08002054
	movs	r1, #5
	bl	sub_08002064
	lsls	r0, r0, #10
	add	r0, fp
	movs	r7, #0
	movs	r6, #1
	mov	r8, r0
.L_0813f772:
	ldr	r3, [pc, #276]
	movs	r1, #7
	ldrb	r2, [r3, r7]
	movs	r3, #3
	orrs	r3, r2
	movs	r0, #188
	movs	r2, #7
	str	r6, [sp, #0]
	bl	sub_08196404
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r4, [r3, #0]
	ldr	r3, [pc, #252]
	mov	r5, r9
	ldrsb	r2, [r3, r7]
	ldr	r3, [pc, #248]
	str	r4, [sp, #16]
	ldrsb	r1, [r3, r7]
	ldr	r3, [r5, #4]
	adds	r2, #32
	adds	r3, r3, r1
	movs	r1, #32
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	movs	r1, #224
	lsls	r1, r1, #3
	subs	r3, #32
	ldr	r0, [sp, #20]
	add	r1, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x3701
	movs	r0, #188
	bl	sub_0801314c
	cmp	r7, #4
	bne.n	.L_0813f772
.L_0813f7c0:
	mov	r0, sl
	cmp	r0, #0
	blt.n	.L_0813f834
	movs	r7, #0
	mov	r5, fp
.L_0813f7ca:
	ldr	r2, [r5, #24]
	cmp	r2, #0
	blt.n	.L_0813f82c
	ldr	r3, [r5, #4]
	cmp	r3, #0
	ble.n	.L_0813f82c
	asrs	r3, r2, #3
	ldr	r0, [r5, #0]
	adds	r6, r3, #1
	bl	sub_08002096
	ldr	r3, [r5, #4]
	muls	r3, r0
	asrs	r3, r3, #16
	adds	r3, #64
	ldr	r0, [r5, #0]
	mov	r8, r3
	bl	sub_08002090
	ldr	r3, [r5, #4]
	mov	r1, r9
	muls	r3, r0
	ldr	r2, [r1, #4]
	asrs	r3, r3, #16
	adds	r4, r3, r2
	cmp	r6, #0
	bgt.n	.L_0813f802
	movs	r6, #1
.L_0813f802:
	ldr	r2, [pc, #144]
	lsls	r0, r6, #1
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #8]
	mov	r3, r8
	adds	r1, r2, r1
	str	r0, [sp, #0]
	subs	r2, r3, r6
	str	r0, [sp, #4]
	subs	r3, r4, r6
	ldr	r0, [sp, #20]
	ldr	r4, [sp, #12]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x686b
	subs	r3, #2
	str	r3, [r5, #4]
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_0813f82c:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #64
	bne.n	.L_0813f7ca
.L_0813f834:
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, fp
	movs	r3, #1
	movs	r5, #1
	movs	r0, #1
	str	r3, [r2, #0]
	add	sl, r5
	bl	sub_08013560
	mov	r0, sl
	cmp	r0, #56
	beq.n	.L_0813f856
	b.n	.L_0813f6b2
.L_0813f856:
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #56]
	bl	sub_08014644
	bl	sub_08143bb8
	movs	r0, #100
	bl	sub_0801314c
	movs	r0, #96
	bl	sub_0801314c
	movs	r0, #92
	bl	sub_0801314c
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x081976da
	.4byte 0x081976d2
	.4byte 0x081976d6
	.4byte 0x08197424
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #92]
	ldr	r0, [r3, #96]
	ldr	r2, [pc, #36]
	ldrh	r3, [r2, #0]
	adds	r1, r3, #0
	strh	r2, [r2, #0]
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne.n	.L_0813f8c6
	movs	r3, #0
	str	r3, [r4, #4]
	strh	r1, [r2, #0]
	ldr	r1, [pc, #20]
	movs	r2, #128
	lsls	r2, r2, #7
	bl	sub_08001cb0
	b.n	.L_0813f8c8
.L_0813f8c6:
	strh	r1, [r2, #0]
.L_0813f8c8:
	pop	{pc}
	movs	r0, r0
	.4byte 0x04000208
	.2byte 0x4000
	.2byte 0x0600
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #92]
	sub	sp, #48
	str	r3, [sp, #8]
	ldr	r2, [sp, #8]
	ldr	r1, [r3, #0]
	mov	fp, r1
	mov	r3, fp
	adds	r3, #1
	str	r3, [r2, #0]
	mov	r3, fp
	cmp	r3, #0
	bne.n	.L_0813f936
	ldr	r5, [pc, #320]
	movs	r1, #0
	mov	sl, r1
.L_0813f904:
	bl	sub_08014878
	movs	r3, #15
	ands	r0, r3
	adds	r3, r0, #0
	adds	r3, #48
	adds	r0, #40
	str	r3, [r5, #0]
	str	r0, [r5, #4]
	bl	sub_08014878
	str	r0, [r5, #12]
	bl	sub_08014878
	str	r0, [r5, #16]
	bl	sub_08014878
	movs	r2, #1
	movs	r3, #128
	add	sl, r2
	lsls	r3, r3, #1
	str	r0, [r5, #20]
	adds	r5, #28
	cmp	sl, r3
	bne.n	.L_0813f904
.L_0813f936:
	add	r3, sp, #36
	movs	r2, #0
	ldr	r1, [pc, #260]
	str	r2, [r3, #4]
	str	r2, [r3, #8]
	mov	sl, r2
	mov	r9, r3
	mov	r8, r1
.L_0813f946:
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_0813f94e
	adds	r3, #3
.L_0813f94e:
	asrs	r3, r3, #2
	cmp	fp, r3
	ble.n	.L_0813fa1c
	mov	r2, r8
	ldr	r3, [r2, #0]
	cmp	r3, #0
	ble.n	.L_0813fa1c
	bl	sub_08014de4
	mov	r3, r8
	ldr	r0, [r3, #20]
	bl	sub_080150e4
	mov	r1, r8
	ldr	r0, [r1, #12]
	bl	sub_08015024
	mov	r2, r8
	ldr	r0, [r2, #16]
	bl	sub_08015068
	mov	r1, r8
	ldr	r3, [r1, #0]
	add	r4, sp, #24
	mov	r2, r9
	str	r3, [r2, #0]
	adds	r1, r4, #0
	mov	r0, r9
	str	r4, [sp, #4]
	bl	sub_0815e1ec
	ldr	r4, [sp, #4]
	mov	r1, r8
	ldr	r3, [r4, #0]
	add	r7, sp, #12
	adds	r3, #64
	str	r3, [r4, #0]
	ldr	r3, [r4, #4]
	mov	r2, r9
	adds	r3, #80
	str	r3, [r4, #4]
	mov	r0, r9
	ldr	r3, [r1, #4]
	adds	r1, r7, #0
	str	r3, [r2, #0]
	bl	sub_0815e1ec
	ldr	r3, [r7, #0]
	mov	r1, r8
	adds	r3, #64
	str	r3, [r7, #0]
	ldr	r3, [r7, #4]
	ldr	r4, [sp, #4]
	adds	r3, #80
	str	r3, [r7, #4]
	mov	r3, r8
	ldr	r2, [r3, #4]
	subs	r2, #4
	str	r2, [r3, #4]
	ldr	r3, [r3, #0]
	subs	r3, #4
	str	r3, [r1, #0]
	cmp	r2, #0
	bge.n	.L_0813f9d2
	movs	r3, #0
	str	r3, [r1, #4]
.L_0813f9d2:
	mov	r2, r8
	ldr	r5, [r2, #4]
	ldr	r0, [r7, #0]
	negs	r5, r5
	lsrs	r3, r5, #31
	ldr	r2, [r4, #0]
	adds	r5, r5, r3
	asrs	r5, r5, #1
	adds	r6, r5, #0
	ldr	r3, [r4, #4]
	ldr	r1, [r7, #4]
	subs	r0, #1
	subs	r2, #1
	adds	r6, #48
	str	r4, [sp, #4]
	str	r6, [sp, #0]
	bl	sub_08143eb4
	ldr	r4, [sp, #4]
	ldr	r1, [r7, #4]
	ldr	r3, [r4, #4]
	ldr	r2, [r4, #0]
	ldr	r0, [r7, #0]
	subs	r1, #1
	subs	r3, #1
	str	r6, [sp, #0]
	bl	sub_08143eb4
	ldr	r4, [sp, #4]
	ldr	r0, [r7, #0]
	ldr	r1, [r7, #4]
	ldr	r2, [r4, #0]
	ldr	r3, [r4, #4]
	adds	r5, #56
	str	r5, [sp, #0]
	bl	sub_08143eb4
.L_0813fa1c:
	movs	r1, #1
	add	sl, r1
	movs	r3, #28
	mov	r2, sl
	add	r8, r3
	cmp	r2, #64
	bne.n	.L_0813f946
	ldr	r1, [sp, #8]
	movs	r3, #1
	str	r3, [r1, #4]
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0201
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #92]
	ldr	r0, [r3, #36]
	ldr	r3, [r5, #8]
	cmp	r3, #0
	ble.n	.L_0813fa84
	ldr	r2, [r5, #12]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r2, #1
	adds	r3, #108
	adds	r0, r0, r3
	lsls	r3, r2, #4
	adds	r3, r3, r2
	lsls	r3, r3, #4
	str	r2, [r5, #12]
	adds	r3, r3, r2
	movs	r1, #160
	movs	r2, #128
	lsls	r3, r3, #2
	lsls	r1, r1, #19
	lsls	r2, r2, #9
	subs	r2, r2, r3
	adds	r1, #192
	movs	r3, #128
	bl	sub_081180f0
	ldr	r3, [r5, #8]
	subs	r3, #1
	str	r3, [r5, #8]
.L_0813fa84:
	pop	{r5, pc}
	.align 2, 0
