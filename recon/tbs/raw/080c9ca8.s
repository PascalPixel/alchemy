.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080b5088, 0x080b5088
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e38b8, 0x080e38b8
	.set sub_080e3944, 0x080e3944
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Func_080c9ca8
	.thumb_func
Func_080c9ca8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #84
	ldr	r6, [pc, #168]
	str	r1, [sp, #68]
	adds	r3, r6, #0
	ldmia	r3!, {r1}
	str	r1, [sp, #64]
	ldr	r3, [r3, #0]
	str	r3, [sp, #60]
	adds	r3, r6, #0
	ldr	r2, [pc, #156]
	subs	r3, #108
	ldr	r3, [r3, #0]
	adds	r5, r1, r2
	str	r3, [sp, #44]
	str	r0, [r5, #0]
	movs	r0, #1
	bl	sub_080cd594
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080c9d00
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #11
	movs	r0, #46
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [r6, #28]
	movs	r0, #47
	str	r3, [sp, #48]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	b.n	.L_080c9d1c
.L_080c9d00:
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #15
	movs	r0, #46
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r4, [r6, #28]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	str	r4, [sp, #48]
.L_080c9d1c:
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r6, [r6, #32]
	str	r6, [sp, #52]
	ldr	r0, [pc, #68]
	ldr	r1, [sp, #64]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [sp, #68]
	cmp	r0, #4
	bhi.n	.L_080c9d84
	ldr	r3, [pc, #52]
	lsls	r1, r0, #2
	ldr	r3, [r1, r3]
	mov	pc, r3
	.4byte 0x080c9d54
	.4byte 0x080c9d58
	.4byte 0x080c9d5c
	.4byte 0x080c9d60
	.2byte 0x9d84
	.2byte 0x080c
	ldr	r0, [pc, #28]
	b.n	.L_080c9d86
	ldr	r0, [pc, #28]
	b.n	.L_080c9d86
	ldr	r0, [pc, #28]
	b.n	.L_080c9d86
	ldr	r0, [pc, #28]
	b.n	.L_080c9d86
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x00000058
	.4byte 0x080c9d40
	.4byte 0x000000b4
	.4byte 0x000000a0
	.4byte 0x000000cb
	.2byte 0x0086
	.2byte 0x0000
.L_080c9d84:
	ldr	r0, [pc, #384]
.L_080c9d86:
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #380]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	movs	r4, #239
	ldr	r3, [sp, #64]
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r0, [sp, #64]
	ldr	r1, [pc, #360]
	movs	r3, #50
	adds	r2, r0, r1
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #352]
	bl	sub_080041d8
	ldr	r3, [pc, #348]
	ldr	r2, [sp, #64]
	adds	r5, r2, r3
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5098
	ldr	r3, [r5, #0]
	ldr	r1, [r3, #20]
	movs	r4, #3
	adds	r2, r1, #0
	muls	r2, r4
	lsls	r3, r2, #1
	ldr	r0, [r0, #0]
	adds	r3, r3, r2
	lsls	r3, r3, #1
	mov	sl, r0
	adds	r3, #48
	movs	r0, #0
	str	r3, [sp, #40]
	str	r0, [sp, #56]
	cmp	r1, #0
	beq.n	.L_080c9e84
	mov	r9, r0
.L_080c9de8:
	ldr	r1, [sp, #64]
	ldr	r2, [pc, #300]
	ldr	r4, [sp, #56]
	adds	r3, r1, r2
	ldr	r2, [r3, #0]
	lsls	r3, r4, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	bl	sub_080b5098
	ldr	r0, [r0, #0]
	mov	r4, r9
	mov	r8, r0
	ldr	r0, [sp, #64]
	lsls	r3, r4, #2
	movs	r1, #225
	movs	r2, #0
	adds	r3, r3, r0
	lsls	r1, r1, #7
	mov	fp, r2
	adds	r7, r3, r1
.L_080c9e12:
	mov	r2, sl
	ldr	r3, [r2, #8]
	str	r3, [r7, #0]
	movs	r4, #160
	ldr	r5, [r2, #12]
	lsls	r4, r4, #13
	adds	r5, r5, r4
	str	r5, [r7, #4]
	ldr	r6, [r2, #16]
	str	r6, [r7, #8]
	mov	r1, r8
	ldr	r0, [r1, #8]
	movs	r1, #24
	subs	r0, r0, r3
	bl	sub_080022ec
	str	r0, [r7, #12]
	mov	r2, r8
	ldr	r0, [r2, #12]
	movs	r3, #160
	lsls	r3, r3, #13
	adds	r0, r0, r3
	movs	r1, #24
	subs	r0, r0, r5
	bl	sub_080022ec
	str	r0, [r7, #16]
	mov	r4, r8
	ldr	r0, [r4, #16]
	movs	r1, #24
	subs	r0, r0, r6
	bl	sub_080022ec
	str	r0, [r7, #20]
	movs	r0, #1
	add	fp, r0
	movs	r3, #0
	mov	r1, fp
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r1, #3
	bne.n	.L_080c9e12
	movs	r2, #3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	add	r9, r3
	ldr	r3, [sp, #56]
	ldr	r4, [sp, #64]
	adds	r3, #1
	ldr	r0, [pc, #160]
	str	r3, [sp, #56]
	adds	r3, r4, r0
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #56]
	ldr	r3, [r3, #20]
	cmp	r1, r3
	bne.n	.L_080c9de8
.L_080c9e84:
	ldr	r3, [sp, #40]
	movs	r2, #0
	mov	sl, r2
	cmp	r3, #0
	bne.n	.L_080c9e90
	b.n	.L_080ca1a4
.L_080c9e90:
	ldr	r4, [sp, #44]
	subs	r3, #16
	adds	r4, #12
	str	r3, [sp, #24]
	str	r4, [sp, #28]
.L_080c9e9a:
	ldr	r0, [sp, #64]
	movs	r1, #211
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r7, #128
	mov	r3, sl
	str	r2, [sp, #56]
	adds	r6, r0, r1
	lsls	r7, r7, #12
	lsls	r5, r3, #12
.L_080c9eae:
	adds	r0, r5, #0
	bl	sub_08002322
	lsls	r0, r0, #1
	subs	r0, r7, r0
	asrs	r0, r0, #10
	stmia	r6!, {r0}
	ldr	r0, [sp, #56]
	movs	r4, #128
	lsls	r4, r4, #5
	adds	r0, #1
	adds	r5, r5, r4
	str	r0, [sp, #56]
	cmp	r0, #160
	bne.n	.L_080c9eae
	ldr	r1, [sp, #24]
	cmp	sl, r1
	ble.n	.L_080c9ee0
	ldr	r4, [sp, #40]
	mov	r0, sl
	ldr	r1, [pc, #44]
	ldr	r3, [pc, #64]
	subs	r2, r4, r0
	orrs	r2, r1
	strh	r2, [r3, #0]
.L_080c9ee0:
	bl	sub_080049ac
	ldr	r1, [sp, #28]
	ldr	r0, [sp, #44]
	bl	sub_080051d8
	movs	r1, #0
	ldr	r3, [sp, #64]
	ldr	r4, [pc, #36]
	str	r1, [sp, #56]
	adds	r2, r3, r4
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L_080c9f00
	b.n	.L_080ca17c
.L_080c9f00:
	b.n	.L_080c9f20
	movs	r0, r0
	.4byte 0x00001000
	.4byte 0x000000a3
	.4byte 0x03001388
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007828
	.2byte 0x0052
	.2byte 0x0400
.L_080c9f20:
	mov	r1, sl
	str	r2, [sp, #36]
	movs	r0, #36
	movs	r2, #0
	subs	r1, #30
	str	r0, [sp, #20]
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	str	r2, [sp, #8]
.L_080c9f32:
	ldr	r3, [sp, #12]
	cmp	sl, r3
	bge.n	.L_080c9f3a
	b.n	.L_080ca152
.L_080c9f3a:
	ldr	r0, [sp, #68]
	lsls	r0, r0, #2
	movs	r4, #0
	str	r0, [sp, #32]
	mov	fp, r4
	mov	r9, r3
.L_080c9f46:
	cmp	sl, r9
	blt.n	.L_080ca040
	ldr	r3, [sp, #8]
	add	r3, fp
	lsls	r2, r3, #3
	ldr	r1, [sp, #64]
	subs	r2, r2, r3
	lsls	r2, r2, #2
	movs	r3, #225
	adds	r2, r1, r2
	lsls	r3, r3, #7
	adds	r3, r3, r2
	mov	r8, r3
	add	r6, sp, #72
	mov	r0, r8
	adds	r1, r6, #0
	bl	sub_080e3944
	ldr	r3, [r6, #0]
	asrs	r7, r3, #1
	str	r7, [r6, #0]
	mov	r4, r8
	ldr	r5, [r4, #24]
	cmp	r5, #0
	bge.n	.L_080c9f7a
	adds	r5, #7
.L_080c9f7a:
	asrs	r2, r5, #3
	cmp	r2, #5
	ble.n	.L_080c9f82
	movs	r2, #5
.L_080c9f82:
	ldr	r3, [pc, #584]
	ldr	r0, [sp, #32]
	ldrsb	r3, [r3, r0]
	cmp	r3, #0
	beq.n	.L_080c9fe8
	mov	r1, sl
	lsls	r5, r2, #1
	lsrs	r0, r1, #31
	adds	r5, r5, r2
	add	r0, sl
	movs	r1, #3
	lsls	r5, r5, #3
	asrs	r0, r0, #1
	adds	r5, r5, r2
	bl	sub_080022fc
	lsls	r2, r0, #2
	adds	r2, r2, r0
	lsls	r3, r2, #4
	subs	r3, r3, r2
	lsls	r3, r3, #6
	ldr	r2, [sp, #64]
	lsls	r5, r5, #5
	adds	r5, r5, r3
	ldr	r3, [r6, #4]
	adds	r5, r2, r5
	movs	r4, #20
	movs	r0, #40
	adds	r2, r7, #0
	subs	r2, #10
	subs	r3, #40
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	adds	r1, r5, #0
	ldr	r4, [sp, #48]
	ldr	r0, [sp, #60]
	bl	sub_080072f4
	ldr	r2, [r6, #0]
	movs	r0, #20
	movs	r1, #40
	ldr	r3, [r6, #4]
	subs	r2, #10
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	ldr	r0, [sp, #60]
	adds	r1, r5, #0
	ldr	r4, [sp, #52]
	bl	sub_080072f4
	b.n	.L_080ca02e
.L_080c9fe8:
	lsls	r5, r2, #1
	adds	r5, r5, r2
	lsls	r5, r5, #3
	ldr	r0, [sp, #64]
	adds	r5, r5, r2
	lsls	r5, r5, #5
	movs	r1, #150
	ldr	r3, [r6, #4]
	adds	r5, r0, r5
	lsls	r1, r1, #6
	adds	r5, r5, r1
	movs	r4, #20
	movs	r0, #40
	adds	r2, r7, #0
	subs	r2, #10
	subs	r3, #40
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	adds	r1, r5, #0
	ldr	r4, [sp, #48]
	ldr	r0, [sp, #60]
	bl	sub_080072f4
	ldr	r2, [r6, #0]
	movs	r0, #20
	movs	r1, #40
	ldr	r3, [r6, #4]
	subs	r2, #10
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	ldr	r0, [sp, #60]
	adds	r1, r5, #0
	ldr	r4, [sp, #52]
	bl	sub_080072f4
.L_080ca02e:
	mov	r0, r8
	movs	r1, #64
	movs	r2, #0
	bl	sub_080e38b8
	mov	r0, r8
	ldr	r3, [r0, #24]
	adds	r3, #1
	str	r3, [r0, #24]
.L_080ca040:
	movs	r2, #1
	add	fp, r2
	movs	r1, #6
	mov	r3, fp
	add	r9, r1
	cmp	r3, #3
	beq.n	.L_080ca050
	b.n	.L_080c9f46
.L_080ca050:
	ldr	r3, [sp, #32]
	ldr	r1, [pc, #376]
	adds	r3, #3
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	beq.n	.L_080ca0ca
	ldr	r3, [sp, #12]
	adds	r3, #30
	cmp	sl, r3
	blt.n	.L_080ca0ca
	ldr	r3, [sp, #12]
	adds	r3, #62
	cmp	sl, r3
	bge.n	.L_080ca0ca
	ldr	r4, [sp, #36]
	ldr	r1, [sp, #20]
	ldr	r3, [r4, #0]
	ldrsh	r0, [r3, r1]
	bl	sub_080b5098
	ldr	r4, [sp, #12]
	mov	r3, sl
	subs	r2, r3, r4
	ldr	r3, [sp, #16]
	ldr	r0, [r0, #0]
	ldr	r1, [pc, #332]
	cmp	r3, #0
	bge.n	.L_080ca08c
	adds	r3, r2, #0
	subs	r3, #23
.L_080ca08c:
	ldr	r2, [sp, #16]
	asrs	r3, r3, #3
	lsls	r3, r3, #3
	subs	r3, r2, r3
	ldrsb	r3, [r1, r3]
	ldr	r2, [r0, #8]
	lsls	r3, r3, #16
	adds	r2, r2, r3
	str	r2, [r0, #8]
	cmp	r2, #0
	ble.n	.L_080ca0aa
	movs	r4, #128
	lsls	r4, r4, #8
	adds	r3, r2, r4
	b.n	.L_080ca0ae
.L_080ca0aa:
	ldr	r1, [pc, #296]
	adds	r3, r2, r1
.L_080ca0ae:
	str	r3, [r0, #8]
	ldr	r2, [sp, #36]
	ldr	r3, [r2, #0]
	ldr	r4, [sp, #20]
	ldrsh	r0, [r3, r4]
	movs	r3, #0
	movs	r1, #1
	str	r3, [sp, #0]
	negs	r1, r1
	movs	r2, #5
	subs	r3, #1
	bl	sub_080d6888
	ldr	r1, [pc, #256]
.L_080ca0ca:
	ldr	r3, [sp, #32]
	adds	r3, #1
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	beq.n	.L_080ca126
	ldr	r3, [sp, #12]
	adds	r3, #24
	cmp	sl, r3
	bne.n	.L_080ca106
	movs	r0, #133
	bl	sub_080f9010
	ldr	r2, [sp, #56]
	cmp	r2, #0
	bne.n	.L_080ca0f0
	movs	r0, #1
	negs	r0, r0
	bl	sub_080b50e8
.L_080ca0f0:
	ldr	r4, [sp, #36]
	ldr	r1, [sp, #20]
	ldr	r3, [r4, #0]
	ldrsh	r0, [r3, r1]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	ldr	r3, [sp, #56]
	bl	sub_080d6888
.L_080ca106:
	ldr	r3, [sp, #12]
	adds	r3, #40
	cmp	sl, r3
	bne.n	.L_080ca124
	ldr	r4, [sp, #36]
	ldr	r3, [r4, #0]
	ldr	r1, [sp, #20]
	ldrsh	r0, [r3, r1]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	ldr	r3, [sp, #56]
	bl	sub_080d6888
.L_080ca124:
	ldr	r1, [pc, #164]
.L_080ca126:
	ldr	r3, [sp, #32]
	adds	r3, #2
	ldrsb	r1, [r1, r3]
	movs	r3, #1
	negs	r3, r3
	cmp	r1, r3
	beq.n	.L_080ca152
	ldr	r3, [sp, #12]
	adds	r3, #24
	cmp	sl, r3
	bne.n	.L_080ca152
	ldr	r4, [sp, #64]
	ldr	r0, [pc, #152]
	movs	r3, #4
	adds	r2, r4, r0
	str	r3, [r2, #0]
	ldr	r2, [sp, #36]
	ldr	r4, [sp, #20]
	ldr	r3, [r2, #0]
	ldrsh	r0, [r3, r4]
	bl	sub_080b5088
.L_080ca152:
	ldr	r3, [sp, #20]
	ldr	r4, [sp, #16]
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #56]
	adds	r3, #2
	subs	r4, #32
	adds	r0, #32
	adds	r1, #3
	adds	r2, #1
	str	r4, [sp, #16]
	str	r3, [sp, #20]
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #56]
	ldr	r4, [sp, #36]
	ldr	r3, [r4, #0]
	ldr	r3, [r3, #20]
	cmp	r2, r3
	beq.n	.L_080ca17c
	b.n	.L_080c9f32
.L_080ca17c:
	movs	r0, #8
	movs	r1, #8
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r1, [pc, #80]
	ldr	r0, [sp, #64]
	movs	r3, #1
	adds	r2, r0, r1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r2, #1
	ldr	r3, [sp, #40]
	add	sl, r2
	cmp	sl, r3
	beq.n	.L_080ca1a4
	b.n	.L_080c9e9a
.L_080ca1a4:
	ldr	r0, [pc, #56]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #84
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x080edee8
	.4byte 0x080edefc
	.4byte 0xffff8000
	.4byte 0x000077a8
	.4byte 0x00007824
	.4byte 0x080cd261
