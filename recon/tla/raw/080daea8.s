.syntax unified
	.thumb
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_08013560, 0x08013560
	.set sub_08020090, 0x08020090
	.set sub_08020218, 0x08020218
	.set sub_080cdf5c, 0x080cdf5c
	.set sub_080d2d84, 0x080d2d84
	.set sub_080da9a8, 0x080da9a8
	.set sub_080dae3c, 0x080dae3c
	.set sub_080dae70, 0x080dae70
	.set sub_080db4b8, 0x080db4b8
	.set sub_080db670, 0x080db670
	.set sub_080db848, 0x080db848
	.set sub_080db884, 0x080db884
	.set sub_081c0010, 0x081c0010
	.global Func_080daea8
	.thumb_func
Func_080daea8:
.L_080daea8:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #164
	ldr	r3, [r3, #0]
	movs	r2, #0
	adds	r0, r3, #0
	adds	r0, #228
.L_080daeb8:
	movs	r3, #18
	ldrsb	r3, [r0, r3]
	cmp	r3, #0
	beq.n	.L_080daeca
	adds	r2, #1
	adds	r0, #32
	cmp	r2, #127
	ble.n	.L_080daeb8
	movs	r0, #0
.L_080daeca:
	pop	{pc}
.L_080daecc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r0, [sp, #8]
	str	r1, [sp, #4]
	bl	sub_080d2d84
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r2, r3, #0
	adds	r2, #164
	ldr	r2, [r2, #0]
	adds	r7, r0, #0
	movs	r0, #186
	lsls	r0, r0, #1
	ldr	r5, [r3, #32]
	cmp	r2, #0
	bne.n	.L_080daf0e
	ldr	r3, [r7, #80]
	cmp	r3, #0
	beq.n	.L_080daf0a
	ldr	r3, [r3, #40]
	cmp	r3, #0
	beq.n	.L_080daf0a
	movs	r1, #0
	ldrsh	r0, [r3, r1]
.L_080daf0a:
	bl	sub_080da9a8
.L_080daf0e:
	ldr	r0, [sp, #8]
	bl	sub_080dae70
	cmp	r0, #0
	beq.n	.L_080daf1a
	b.n	.L_080db096
.L_080daf1a:
	bl	sub_080dae3c
	mov	sl, r0
	cmp	r0, #0
	bne.n	.L_080daf26
	b.n	.L_080db096
.L_080daf26:
	ldr	r3, [r7, #8]
	ldr	r2, [r7, #12]
	asrs	r3, r3, #20
	mov	r9, r3
	ldr	r3, [r7, #16]
	mov	r0, r9
	subs	r3, r3, r2
	movs	r2, #212
	asrs	r3, r3, #20
	lsls	r2, r2, #1
	mov	r8, r3
	adds	r3, r5, r2
	ldr	r1, [r3, #0]
	mov	r3, r8
	lsls	r2, r3, #9
	lsls	r3, r0, #2
	movs	r0, #128
	movs	r4, #0
	movs	r6, #4
	adds	r2, r2, r3
	lsls	r0, r0, #2
.L_080daf50:
	adds	r2, r2, r0
	adds	r3, r1, r2
	subs	r6, #1
	strb	r4, [r3, #2]
	cmp	r6, #0
	bge.n	.L_080daf50
	ldr	r2, [pc, #324]
	ldr	r3, [r7, #8]
	movs	r1, #128
	ands	r3, r2
	lsls	r1, r1, #12
	adds	r3, r3, r1
	ldr	r5, [r7, #12]
	mov	r9, r3
	ldr	r3, [r7, #16]
	ands	r5, r2
	movs	r6, #128
	adds	r1, r1, r5
	ands	r3, r2
	lsls	r6, r6, #13
	adds	r3, r3, r6
	adds	r0, r7, #0
	mov	fp, r1
	movs	r1, #0
	mov	r8, r3
	bl	sub_08020090
	adds	r2, r7, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	adds	r2, #4
	strb	r3, [r2, #0]
	ldr	r3, [r7, #16]
	ldr	r1, [pc, #272]
	adds	r3, r3, r6
	str	r3, [r7, #16]
	ldr	r3, [r7, #12]
	adds	r0, r7, #0
	adds	r3, r3, r1
	str	r3, [r7, #12]
	movs	r1, #0
	bl	sub_08020218
	movs	r2, #240
	mov	r7, sl
	lsls	r2, r2, #12
	movs	r4, #6
	adds	r7, #24
	movs	r6, #0
	adds	r5, r5, r2
	b.n	.L_080daff2
.L_080dafb8:
	mov	r3, r9
	mov	r1, r8
	str	r3, [r0, #4]
	str	r5, [r0, #8]
	str	r1, [r0, #12]
	cmp	r6, #0
	bne.n	.L_080dafca
	movs	r3, #1
	b.n	.L_080dafe6
.L_080dafca:
	cmp	r6, #1
	bne.n	.L_080dafd2
	movs	r3, #2
	b.n	.L_080dafe6
.L_080dafd2:
	cmp	r6, r4
	bne.n	.L_080dafda
	movs	r3, #5
	b.n	.L_080dafe6
.L_080dafda:
	lsrs	r3, r6, #31
	adds	r3, r6, r3
	asrs	r3, r3, #1
	lsls	r3, r3, #1
	subs	r3, r6, r3
	adds	r3, #3
.L_080dafe6:
	strb	r3, [r0, #18]
	ldr	r2, [pc, #192]
	str	r0, [r7, #0]
	adds	r5, r5, r2
	adds	r7, r0, #0
	adds	r6, #1
.L_080daff2:
	cmp	r6, r4
	bgt.n	.L_080db002
	str	r4, [sp, #0]
	bl	.L_080daea8
	ldr	r4, [sp, #0]
	cmp	r0, #0
	bne.n	.L_080dafb8
.L_080db002:
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne.n	.L_080db024
	mov	r1, sl
	movs	r6, #0
	ldr	r0, [r1, #24]
	cmp	r6, r4
	bgt.n	.L_080db076
	cmp	r0, #0
	beq.n	.L_080db076
.L_080db016:
	adds	r6, #1
	ldr	r0, [r0, #0]
	cmp	r6, r4
	bgt.n	.L_080db076
	cmp	r0, #0
	bne.n	.L_080db016
	b.n	.L_080db076
.L_080db024:
	movs	r7, #128
	movs	r5, #1
	lsls	r7, r7, #9
.L_080db02a:
	mov	r2, sl
	movs	r6, #0
	ldr	r0, [r2, #24]
	cmp	r6, r4
	bgt.n	.L_080db060
	cmp	r0, #0
	beq.n	.L_080db060
	lsls	r3, r5, #15
	negs	r2, r3
	mov	ip, r7
.L_080db03e:
	mov	r3, r9
	str	r3, [r0, #4]
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	mov	r1, fp
	subs	r3, r1, r3
	str	r3, [r0, #8]
	adds	r6, #1
	mov	r3, r8
	str	r3, [r0, #12]
	add	r2, ip
	ldr	r0, [r0, #0]
	cmp	r6, r4
	bgt.n	.L_080db060
	cmp	r0, #0
	bne.n	.L_080db03e
.L_080db060:
	movs	r0, #1
	str	r4, [sp, #0]
	bl	sub_08013560
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r5, #1
	adds	r7, r7, r0
	ldr	r4, [sp, #0]
	cmp	r5, #28
	ble.n	.L_080db02a
.L_080db076:
	mov	r1, sl
	movs	r3, #1
	str	r3, [r1, #20]
	add	r2, sp, #8
	movs	r3, #0
	str	r3, [r1, #16]
	ldrh	r2, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	str	r3, [r1, #4]
	mov	r3, sl
	strh	r2, [r3, #0]
	movs	r3, #224
	lsls	r3, r3, #12
	mov	r0, sl
	str	r3, [r0, #8]
.L_080db096:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xfff00000
	.4byte 0xffc00000
	.2byte 0x0000
	.2byte 0xfff2
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r0, #0
	sub	sp, #20
	mov	r9, r0
	bl	sub_080cdf5c
	bl	sub_080d2d84
	mov	r8, r0
	adds	r0, r5, #0
	bl	sub_080d2d84
	mov	r1, r8
	cmp	r1, #0
	bne.n	.L_080db0de
	b.n	.L_080db428
.L_080db0de:
	cmp	r0, #0
	bne.n	.L_080db0e4
	b.n	.L_080db428
.L_080db0e4:
	movs	r2, #85
	add	r2, r8
	ldrb	r3, [r2, #0]
	mov	fp, r2
	str	r3, [sp, #8]
	ldr	r4, [r1, #80]
	str	r4, [sp, #4]
	ldr	r2, [r1, #8]
	ldr	r3, [r0, #8]
	cmp	r2, r3
	bge.n	.L_080db0fe
	movs	r0, #1
	mov	r9, r0
.L_080db0fe:
	adds	r0, r5, #0
	bl	sub_080dae70
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L_080db10c
	b.n	.L_080db428
.L_080db10c:
	mov	r2, r9
	lsls	r2, r2, #15
	ldr	r1, [r7, #4]
	str	r2, [sp, #0]
	ldr	r3, [r7, #16]
	eors	r3, r2
	movs	r2, #128
	lsls	r2, r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080db138
	adds	r5, r1, #0
	cmp	r5, #0
	blt.n	.L_080db136
.L_080db128:
	str	r5, [r7, #4]
	movs	r0, #1
	subs	r5, #64
	bl	sub_08013560
	cmp	r5, #0
	bge.n	.L_080db128
.L_080db136:
	movs	r1, #0
.L_080db138:
	movs	r3, #192
	adds	r5, r1, #0
	lsls	r3, r3, #3
	cmp	r5, r3
	bgt.n	.L_080db15e
	ldr	r4, [sp, #0]
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r6, r4, r0
.L_080db14a:
	str	r5, [r7, #4]
	str	r6, [r7, #16]
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #192
	adds	r5, #64
	lsls	r1, r1, #3
	cmp	r5, r1
	ble.n	.L_080db14a
.L_080db15e:
	mov	r4, r9
	ldr	r3, [r7, #16]
	movs	r2, #1
	eors	r2, r4
	movs	r0, #255
	movs	r4, #128
	lsls	r0, r0, #8
	lsls	r1, r2, #15
	lsls	r4, r4, #7
	ands	r3, r0
	adds	r2, r1, r4
	cmp	r3, r2
	beq.n	.L_080db194
	movs	r2, #192
	lsls	r2, r2, #3
	mov	sl, r2
	adds	r5, r1, r4
	adds	r6, r0, #0
.L_080db182:
	mov	r3, sl
	str	r3, [r7, #4]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [r7, #16]
	ands	r3, r6
	cmp	r3, r5
	bne.n	.L_080db182
.L_080db194:
	mov	r4, r8
	mov	r0, r9
	ldr	r6, [r4, #80]
	movs	r5, #1
	cmp	r0, #0
	beq.n	.L_080db1ae
	movs	r1, #162
	mov	sl, r1
	subs	r5, #2
	b.n	.L_080db1b2
.L_080db1a8:
	movs	r3, #0
	strh	r3, [r6, #18]
	b.n	.L_080db424
.L_080db1ae:
	movs	r2, #146
	mov	sl, r2
.L_080db1b2:
	movs	r3, #1
	str	r3, [r7, #20]
	mov	r0, r8
	movs	r1, #32
	bl	sub_08020090
	mov	r3, sl
	lsls	r3, r3, #16
	mov	sl, r3
.L_080db1c4:
	movs	r3, #1
	mov	r4, r9
	eors	r3, r4
	movs	r0, #128
	lsls	r3, r3, #15
	lsls	r0, r0, #7
	adds	r3, r3, r0
	str	r3, [r7, #16]
	ldr	r3, [pc, #328]
	ldr	r2, [r3, #0]
	movs	r3, #63
	ands	r2, r3
	cmp	r2, #18
	bhi.n	.L_080db25c
	lsls	r3, r2, #2
	ldr	r2, [pc, #320]
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0xb234
	lsrs	r5, r1, #32
	.2byte 0xb244
	lsrs	r5, r1, #32
	.2byte 0xb244
	lsrs	r5, r1, #32
	.2byte 0xb25c
	lsrs	r5, r1, #32
	.2byte 0xb25c
	lsrs	r5, r1, #32
	.2byte 0xb25c
	lsrs	r5, r1, #32
	.2byte 0xb25c
	lsrs	r5, r1, #32
	.2byte 0xb25c
	lsrs	r5, r1, #32
	.2byte 0xb25c
	lsrs	r5, r1, #32
	.2byte 0xb25c
	lsrs	r5, r1, #32
	.2byte 0xb25c
	lsrs	r5, r1, #32
	.2byte 0xb25c
	lsrs	r5, r1, #32
	.2byte 0xb25c
	lsrs	r5, r1, #32
	.2byte 0xb25c
	lsrs	r5, r1, #32
	.2byte 0xb25c
	lsrs	r5, r1, #32
	.2byte 0xb25c
	lsrs	r5, r1, #32
	.2byte 0xb234
	lsrs	r5, r1, #32
	.2byte 0xb244
	lsrs	r5, r1, #32
	.2byte 0xb244
	lsrs	r5, r1, #32
	movs	r3, #196
	lsls	r3, r3, #3
	str	r3, [r7, #4]
	movs	r3, #232
	lsls	r3, r3, #12
	str	r3, [r7, #8]
	lsls	r3, r5, #10
	b.n	.L_080db28a
	ldr	r3, [r7, #4]
	movs	r1, #128
	adds	r3, #16
	str	r3, [r7, #4]
	ldr	r3, [r7, #8]
	lsls	r1, r1, #7
	adds	r3, r3, r1
	str	r3, [r7, #8]
	lsls	r2, r5, #9
	ldrh	r3, [r6, #18]
	adds	r3, r3, r2
	b.n	.L_080db28a
.L_080db25c:
	ldr	r3, [r7, #4]
	movs	r2, #192
	lsls	r2, r2, #3
	cmp	r3, r2
	ble.n	.L_080db26c
	subs	r3, #32
	str	r3, [r7, #4]
	b.n	.L_080db26e
.L_080db26c:
	str	r2, [r7, #4]
.L_080db26e:
	ldr	r3, [r7, #8]
	movs	r2, #224
	lsls	r2, r2, #12
	cmp	r3, r2
	ble.n	.L_080db27e
	ldr	r4, [pc, #172]
	adds	r3, r3, r4
	str	r3, [r7, #8]
.L_080db27e:
	ldrh	r2, [r6, #18]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080db28c
	lsls	r3, r5, #7
	subs	r3, r2, r3
.L_080db28a:
	strh	r3, [r6, #18]
.L_080db28c:
	ldr	r1, [pc, #156]
	mov	r0, sl
	ldr	r2, [r1, #4]
	lsrs	r3, r0, #16
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_080db1a8
	ldr	r3, [r1, #4]
	cmp	r3, #0
	beq.n	.L_080db314
	strh	r2, [r6, #18]
	movs	r1, #6
	str	r2, [r7, #20]
	mov	r0, r8
	bl	sub_08020090
	movs	r0, #6
	bl	sub_08013560
	movs	r0, #152
	bl	sub_081c0010
	mov	r0, r8
	movs	r1, #7
	bl	sub_08020090
	movs	r3, #192
	mov	r1, r8
	lsls	r3, r3, #10
	str	r3, [r1, #48]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r1, #52]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r1, #40]
	mov	r3, fp
	ldrb	r2, [r3, #0]
	movs	r3, #126
	ands	r3, r2
	mov	r4, fp
	strb	r3, [r4, #0]
	movs	r1, #0
	mov	r0, r8
	bl	sub_08020218
	movs	r0, #5
	bl	sub_08013560
	mov	r0, r8
	movs	r1, #16
	bl	sub_08020090
	ldr	r5, [pc, #36]
	mov	r0, fp
	strb	r5, [r0, #0]
	ldr	r3, [r7, #24]
	movs	r2, #130
	ldr	r3, [r3, #0]
	lsls	r2, r2, #15
	ldr	r1, [r3, #4]
	mov	sl, r2
	str	r1, [sp, #16]
	ldr	r3, [r3, #8]
	str	r3, [sp, #12]
	movs	r3, #8
	mov	r9, r3
	b.n	.L_080db3ae
.L_080db314:
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_080db1c4
	.4byte 0x00000000
	.4byte 0x0300122c
	.4byte 0x080db1e8
	.4byte 0xfffff800
	.2byte 0x1150
	.2byte 0x0300
.L_080db330:
	mov	r4, r9
	cmp	r4, #0
	beq.n	.L_080db34c
	ldr	r3, [r7, #8]
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r3, r3, r1
	movs	r0, #160
	movs	r2, #1
	str	r3, [r7, #8]
	lsls	r0, r0, #7
	negs	r2, r2
	add	sl, r0
	add	r9, r2
.L_080db34c:
	movs	r3, #128
	lsls	r3, r3, #4
	str	r3, [r7, #4]
	movs	r3, #1
	str	r3, [r7, #20]
	ldr	r0, [r7, #16]
	ldr	r3, [pc, #220]
	adds	r0, r0, r3
	bl	sub_08002096
	ldr	r1, [r7, #4]
	ldr	r5, [pc, #216]
	lsls	r3, r1, #2
	adds	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r1, r1, r3
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x1c06
	bl	sub_08002096
	mov	r1, sl
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9c04
	mov	r1, r8
	subs	r0, r4, r0
	str	r0, [r1, #8]
	adds	r0, r6, #0
	bl	sub_08002090
	mov	r1, sl
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9a03
	mov	r3, r8
	subs	r0, r2, r0
	str	r0, [r3, #12]
	ldr	r3, [r3, #8]
	mov	r4, r8
	str	r3, [r4, #56]
	str	r0, [r4, #60]
	ldr	r0, [sp, #4]
	strh	r6, [r0, #18]
	movs	r0, #1
	bl	sub_08013560
.L_080db3ae:
	ldr	r3, [r7, #16]
	ldr	r1, [sp, #0]
	movs	r2, #255
	movs	r4, #224
	lsls	r2, r2, #8
	lsls	r4, r4, #6
	ands	r3, r2
	adds	r2, r1, r4
	cmp	r3, r2
	bne.n	.L_080db330
	movs	r3, #1
	str	r3, [r7, #20]
	mov	r0, r8
	movs	r1, #7
	bl	sub_08020090
	movs	r0, #2
	ldr	r3, [sp, #8]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	mov	r1, fp
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r2, r8
	str	r3, [r2, #40]
	movs	r5, #7
.L_080db3e6:
	ldr	r3, [r7, #8]
	ldr	r4, [pc, #84]
	movs	r0, #1
	adds	r3, r3, r4
	str	r3, [r7, #8]
	subs	r5, #1
	bl	sub_08013560
	cmp	r5, #0
	bge.n	.L_080db3e6
	movs	r3, #224
	lsls	r3, r3, #12
	str	r3, [r7, #8]
	ldr	r0, [sp, #4]
	movs	r3, #0
	strh	r3, [r0, #18]
	movs	r1, #6
	mov	r0, r8
	bl	sub_08020090
	mov	r0, r8
	movs	r1, #1
	bl	sub_08020218
	add	r1, sp, #8
	ldrb	r1, [r1, #0]
	mov	r2, fp
	strb	r1, [r2, #0]
	movs	r0, #6
	bl	sub_08013560
.L_080db424:
	movs	r3, #1
	str	r3, [r7, #20]
.L_080db428:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xfffff060
	.4byte 0x0300021c
	.2byte 0xf000
	.2byte 0xffff
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	bl	sub_080d2d84
	cmp	r0, #0
	beq.n	.L_080db484
	adds	r3, r0, #0
	adds	r3, #85
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080db478
	ldr	r3, [r0, #16]
	ldr	r2, [pc, #40]
	movs	r1, #0
	adds	r3, r3, r2
	str	r3, [r0, #16]
	ldr	r3, [r0, #12]
	movs	r2, #128
	lsls	r2, r2, #15
	adds	r3, r3, r2
	str	r3, [r0, #12]
	adds	r0, r5, #0
	bl	.L_080daecc
	b.n	.L_080db484
.L_080db478:
	cmp	r6, #0
	beq.n	.L_080db484
	adds	r0, r5, #0
	movs	r1, #0
	bl	.L_080daecc
.L_080db484:
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb500
	cmp	r0, #0
	bne.n	.L_080db498
	bl	sub_080db4b8
	b.n	.L_080db4b4
.L_080db498:
	cmp	r0, #1
	bne.n	.L_080db4a2
	bl	sub_080db670
	b.n	.L_080db4b4
.L_080db4a2:
	cmp	r0, #2
	bne.n	.L_080db4ac
	bl	sub_080db848
	b.n	.L_080db4b4
.L_080db4ac:
	cmp	r0, #3
	bne.n	.L_080db4b4
	bl	sub_080db884
.L_080db4b4:
	pop	{pc}
	.2byte 0x0000
