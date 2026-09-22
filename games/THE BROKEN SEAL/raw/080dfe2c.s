.syntax unified
	.thumb
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080072f4, 0x080072f4
	.set sub_080b5088, 0x080b5088
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080cef64, 0x080cef64
	.set sub_080d6888, 0x080d6888
	.set sub_080dfddc, 0x080dfddc
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e3908, 0x080e3908
	.set sub_080f9010, 0x080f9010
	.global Overlay_080dfe2c
Overlay_080dfe2c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #872]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #36
	str	r3, [sp, #24]
	ldr	r3, [pc, #864]
	mov	r9, r1
	ldr	r2, [r2, #8]
	add	r3, r9
	str	r2, [sp, #16]
	str	r0, [r3, #0]
	movs	r0, #0
	bl	sub_080cd594
	mov	r2, sp
	adds	r2, #28
	adds	r1, r2, #0
	movs	r0, #0
	str	r2, [sp, #12]
	bl	sub_080cef64
	ldr	r5, [pc, #836]
	ldr	r1, [sp, #16]
	ldr	r0, [pc, #836]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [pc, #828]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	adds	r1, r5, #0
	ldr	r0, [pc, #820]
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r1, #170
	lsls	r1, r1, #2
	adds	r0, r5, #0
	add	r1, r9
	movs	r2, #17
	movs	r3, #104
	bl	sub_080dfddc
	movs	r3, #221
	lsls	r3, r3, #3
	movs	r1, #153
	adds	r5, r5, r3
	lsls	r1, r1, #4
	add	r1, r9
	adds	r0, r5, #0
	movs	r2, #34
	movs	r3, #65
	bl	sub_080dfddc
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #764]
	movs	r3, #75
	add	r2, r9
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #760]
	lsls	r1, r1, #3
	bl	sub_080041d8
	ldr	r3, [pc, #756]
	movs	r7, #0
	movs	r2, #1
	mov	sl, r7
	negs	r2, r2
	add	r3, r9
.L_080dfeda:
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r1, #8
	bne.n	.L_080dfeda
	movs	r2, #0
	mov	sl, r2
	movs	r1, #1
	movs	r2, #128
	ldr	r3, [pc, #724]
	negs	r1, r1
	lsls	r2, r2, #2
.L_080dfef6:
	movs	r7, #1
	add	sl, r7
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	sl, r2
	bne.n	.L_080dfef6
	movs	r0, #162
	bl	sub_080f9010
	movs	r0, #0
	str	r0, [sp, #20]
.L_080dff0c:
	ldr	r1, [sp, #20]
	cmp	r1, #56
	bne.n	.L_080dff18
	movs	r0, #133
	bl	sub_080b50e8
.L_080dff18:
	movs	r3, #225
	lsls	r3, r3, #7
	movs	r2, #0
	add	r3, r9
	mov	sl, r2
	mov	r8, r3
	mov	fp, r2
.L_080dff26:
	mov	r7, r8
	movs	r0, #1
	ldr	r3, [r7, #24]
	negs	r0, r0
	cmp	r3, r0
	beq.n	.L_080dffce
	movs	r1, #65
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	str	r1, [sp, #0]
	movs	r1, #34
	str	r1, [sp, #4]
	movs	r1, #153
	lsls	r1, r1, #4
	subs	r3, #17
	subs	r2, #16
	ldr	r4, [sp, #28]
	ldr	r0, [sp, #24]
	add	r1, r9
	bl	sub_080072f4
	ldr	r3, [r7, #0]
	subs	r3, #12
	str	r3, [r7, #0]
	ldr	r3, [r7, #24]
	adds	r3, #1
	str	r3, [r7, #24]
	cmp	r3, #5
	bne.n	.L_080dffce
	movs	r0, #133
	bl	sub_080f9010
	ldr	r2, [pc, #612]
	movs	r3, #4
	add	r2, r9
	ldr	r7, [pc, #572]
	str	r3, [r2, #0]
	movs	r4, #0
	add	r7, fp
.L_080dff74:
	str	r4, [sp, #8]
	bl	sub_08004458
	ldr	r3, [pc, #596]
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08004458
	mov	r2, r8
	ldr	r3, [r2, #0]
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	ldr	r3, [r2, #4]
	ldr	r5, [pc, #580]
	lsls	r3, r3, #16
	movs	r1, #128
	lsls	r1, r1, #1
	str	r3, [r7, #4]
	ands	r5, r0
	adds	r0, r6, #0
	adds	r5, r5, r1
	bl	sub_08002322
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #8
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #16]
	bl	sub_08004458
	movs	r3, #15
	ldr	r4, [sp, #8]
	ands	r3, r0
	adds	r3, #32
	adds	r4, #1
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r4, #32
	bne.n	.L_080dff74
.L_080dffce:
	movs	r0, #1
	movs	r7, #224
	add	sl, r0
	movs	r3, #28
	lsls	r7, r7, #2
	mov	r1, sl
	add	r8, r3
	add	fp, r7
	cmp	r1, #5
	bne.n	.L_080dff26
	ldr	r2, [sp, #20]
	cmp	r2, #95
	ble.n	.L_080dffea
	b.n	.L_080e00f0
.L_080dffea:
	lsls	r6, r2, #11
	adds	r0, r6, #0
	bl	sub_08002322
	ldr	r7, [sp, #20]
	movs	r5, #64
	lsls	r3, r7, #1
	subs	r5, r5, r3
	adds	r3, r5, #0
	muls	r3, r0
	movs	r0, #96
	asrs	r7, r3, #17
	adds	r0, r0, r7
	mov	r8, r0
	adds	r0, r6, #0
	bl	sub_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	movs	r1, #60
	asrs	r3, r3, #16
	adds	r1, r1, r3
	mov	fp, r1
	movs	r1, #20
	str	r1, [sp, #0]
	movs	r1, #34
	str	r1, [sp, #4]
	ldr	r0, [sp, #12]
	adds	r2, r7, #0
	adds	r2, #86
	adds	r3, #43
	ldr	r4, [r0, #4]
	mov	r1, r9
	ldr	r0, [sp, #24]
	bl	sub_080072f4
	ldr	r0, [pc, #420]
	movs	r1, #0
	ldrb	r3, [r0, r1]
	ldr	r2, [sp, #20]
	mov	sl, r1
	cmp	r2, r3
	bne.n	.L_080e0064
	movs	r1, #225
	lsls	r1, r1, #7
	add	r1, r9
	movs	r2, #1
	ldr	r3, [r1, #24]
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_080e0064
	adds	r3, r7, #0
	adds	r3, #88
	str	r3, [r1, #0]
	mov	r7, fp
	mov	r3, r8
	mov	r0, sl
	str	r3, [r1, #12]
	str	r7, [r1, #4]
	str	r0, [r1, #24]
	b.n	.L_080e00f0
.L_080e0064:
	mov	r1, sl
	ldrb	r3, [r0, r1]
	ldr	r2, [sp, #20]
	adds	r3, #6
	cmp	r2, r3
	bne.n	.L_080e00b0
	ldr	r3, [pc, #308]
	mov	r7, r9
	ldr	r3, [r7, r3]
	ldr	r3, [r3, #20]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_080e00b0
	ldr	r5, [pc, #296]
	movs	r6, #36
	add	r5, r9
.L_080e0084:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r2, #5
	adds	r3, r4, #0
	movs	r1, #7
	str	r4, [sp, #8]
	bl	sub_080d6888
	ldr	r3, [r5, #0]
	movs	r1, #6
	ldrsh	r0, [r3, r6]
	bl	sub_080b5088
	ldr	r3, [r5, #0]
	ldr	r4, [sp, #8]
	ldr	r3, [r3, #20]
	adds	r4, #1
	adds	r6, #2
	cmp	r4, r3
	bne.n	.L_080e0084
.L_080e00b0:
	movs	r3, #1
	add	sl, r3
	mov	r7, sl
	cmp	r7, #5
	beq.n	.L_080e00f0
	ldr	r3, [pc, #284]
	adds	r0, r3, #0
	ldrb	r3, [r0, r7]
	ldr	r1, [sp, #20]
	cmp	r1, r3
	bne.n	.L_080e0064
	lsls	r3, r7, #3
	subs	r3, r3, r7
	lsls	r3, r3, #2
	movs	r7, #225
	add	r3, r9
	lsls	r7, r7, #7
	adds	r2, r3, r7
	movs	r1, #1
	ldr	r3, [r2, #24]
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L_080e0064
	mov	r3, r8
	subs	r3, #8
	str	r3, [r2, #0]
	mov	r3, r8
	str	r3, [r2, #12]
	mov	r7, fp
	movs	r3, #0
	str	r7, [r2, #4]
	str	r3, [r2, #24]
.L_080e00f0:
	movs	r0, #0
	ldr	r5, [pc, #184]
	ldr	r6, [pc, #228]
	mov	sl, r0
.L_080e00f8:
	movs	r1, #1
	ldr	r0, [r5, #24]
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L_080e0148
	cmp	r0, #0
	bge.n	.L_080e0108
	adds	r0, #15
.L_080e0108:
	asrs	r0, r0, #4
	adds	r0, #2
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r2, [sp, #16]
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
	ldr	r0, [sp, #12]
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #24]
	bl	sub_080072f4
	movs	r2, #128
	adds	r0, r5, #0
	movs	r1, #62
	lsls	r2, r2, #6
	bl	sub_080e3908
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_080e0148:
	movs	r1, #1
	movs	r2, #128
	add	sl, r1
	lsls	r2, r2, #1
	adds	r5, #28
	cmp	sl, r2
	bne.n	.L_080e00f8
	movs	r0, #4
	movs	r1, #4
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #124]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [sp, #20]
	adds	r3, #1
	str	r3, [sp, #20]
	cmp	r3, #96
	beq.n	.L_080e017c
	b.n	.L_080dff0c
.L_080e017c:
	ldr	r0, [pc, #64]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x02010000
	.4byte 0x00000073
	.4byte 0x00000092
	.4byte 0x0000006f
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007098
	.4byte 0x02010018
	.4byte 0x000077a8
	.4byte 0x0000ffff
	.4byte 0x000001ff
	.4byte 0x080eec5a
	.4byte 0x080ede48
	.2byte 0x7824
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #96]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #40
	str	r3, [sp, #28]
	ldr	r3, [pc, #88]
	mov	r9, r1
	ldr	r2, [r2, #8]
	add	r3, r9
	str	r2, [sp, #24]
	str	r0, [r3, #0]
	movs	r0, #0
	bl	sub_080cd594
	ldr	r3, [pc, #60]
	ldr	r2, [pc, #72]
	strh	r3, [r2, #0]
	mov	r2, sp
	adds	r2, #32
	adds	r1, r2, #0
	movs	r0, #0
	str	r2, [sp, #20]
	bl	sub_080cef64
	ldr	r0, [pc, #56]
	ldr	r1, [sp, #24]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [pc, #48]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r1, #200
	lsls	r1, r1, #2
	ldr	r0, [pc, #36]
	add	r1, r9
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	b.n	.L_080e026c
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000052
	.4byte 0x00000073
	.4byte 0x00000090
	.2byte 0x0089
	.2byte 0x0000
.L_080e026c:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #640]
	movs	r3, #75
	add	r2, r9
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #632]
	lsls	r1, r1, #3
	movs	r5, #225
	bl	sub_080041d8
	lsls	r5, r5, #7
	movs	r3, #0
	mov	sl, r3
	movs	r6, #63
	add	r5, r9
.L_080e0294:
	bl	sub_08004458
	ands	r0, r6
	adds	r0, #64
	str	r0, [r5, #0]
	bl	sub_08004458
	movs	r7, #1
	ands	r0, r6
	subs	r0, #80
	add	sl, r7
	str	r0, [r5, #4]
	mov	r0, sl
	adds	r5, #28
	cmp	r0, #32
	bne.n	.L_080e0294
	movs	r1, #0
	movs	r2, #128
	ldr	r3, [pc, #580]
	mov	sl, r1
	lsls	r2, r2, #2
	subs	r1, #1
.L_080e02c0:
	movs	r7, #1
	add	sl, r7
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	sl, r2
	bne.n	.L_080e02c0
	movs	r0, #171
	bl	sub_080f9010
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #0
	str	r1, [sp, #16]
	mov	fp, r0
.L_080e02dc:
	mov	r2, fp
	cmp	r2, #56
	bne.n	.L_080e02e8
	movs	r0, #133
	bl	sub_080b50e8
.L_080e02e8:
	mov	r3, fp
	cmp	r3, #95
	bgt.n	.L_080e0326
	ldr	r0, [sp, #16]
	bl	sub_08002322
	mov	r7, fp
	lsls	r3, r7, #1
	movs	r5, #64
	subs	r5, r5, r3
	adds	r6, r5, #0
	muls	r6, r0
	ldr	r0, [sp, #16]
	bl	sub_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	movs	r2, #20
	asrs	r6, r6, #17
	asrs	r3, r3, #16
	adds	r6, #86
	str	r2, [sp, #0]
	movs	r2, #40
	str	r2, [sp, #4]
	adds	r3, #28
	ldr	r4, [sp, #32]
	ldr	r0, [sp, #28]
	mov	r1, r9
	adds	r2, r6, #0
	bl	sub_080072f4
.L_080e0326:
	movs	r1, #225
	movs	r0, #0
	lsls	r1, r1, #7
	add	r1, r9
	str	r0, [sp, #12]
	mov	sl, r0
	mov	r8, r1
.L_080e0334:
	mov	r2, sl
	lsls	r3, r2, #2
	adds	r3, #8
	cmp	fp, r3
	blt.n	.L_080e0422
	mov	r7, r8
	ldr	r3, [r7, #4]
	cmp	r3, #95
	bgt.n	.L_080e0422
	movs	r1, #40
	ldr	r2, [r7, #0]
	str	r1, [sp, #0]
	movs	r1, #64
	str	r1, [sp, #4]
	movs	r1, #200
	lsls	r1, r1, #2
	subs	r3, #32
	subs	r2, #20
	ldr	r4, [sp, #32]
	ldr	r0, [sp, #28]
	add	r1, r9
	bl	sub_080072f4
	ldr	r3, [r7, #0]
	subs	r3, #6
	str	r3, [r7, #0]
	ldr	r3, [r7, #4]
	adds	r3, #12
	str	r3, [r7, #4]
	cmp	r3, #95
	ble.n	.L_080e0422
	ldr	r0, [sp, #12]
	ldr	r1, [pc, #396]
	movs	r4, #0
	adds	r7, r0, r1
.L_080e037a:
	str	r4, [sp, #8]
	bl	sub_08004458
	ldr	r3, [pc, #388]
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08004458
	ldr	r5, [pc, #384]
	ands	r5, r0
	mov	r0, r8
	ldr	r3, [r0, #0]
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	ldr	r3, [r0, #4]
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #1
	str	r3, [r7, #4]
	adds	r0, r6, #0
	adds	r5, r5, r2
	bl	sub_08002322
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08004458
	movs	r3, #15
	ldr	r4, [sp, #8]
	ands	r3, r0
	adds	r3, #32
	adds	r4, #1
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r4, #32
	bne.n	.L_080e037a
	movs	r0, #133
	bl	sub_080f9010
	ldr	r2, [pc, #308]
	movs	r3, #4
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r3, [pc, #304]
	mov	r1, r9
	ldr	r3, [r1, r3]
	ldr	r3, [r3, #20]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_080e0422
	ldr	r5, [pc, #288]
	movs	r6, #36
	add	r5, r9
.L_080e03f6:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #7
	adds	r3, r4, #0
	movs	r2, #5
	str	r4, [sp, #8]
	bl	sub_080d6888
	ldr	r3, [r5, #0]
	movs	r1, #6
	ldrsh	r0, [r3, r6]
	bl	sub_080b5088
	ldr	r3, [r5, #0]
	ldr	r4, [sp, #8]
	ldr	r3, [r3, #20]
	adds	r4, #1
	adds	r6, #2
	cmp	r4, r3
	bne.n	.L_080e03f6
.L_080e0422:
	ldr	r1, [sp, #12]
	movs	r2, #224
	movs	r3, #1
	lsls	r2, r2, #2
	add	sl, r3
	movs	r0, #28
	adds	r1, r1, r2
	mov	r7, sl
	add	r8, r0
	str	r1, [sp, #12]
	cmp	r7, #8
	beq.n	.L_080e043c
	b.n	.L_080e0334
.L_080e043c:
	movs	r0, #0
	ldr	r5, [pc, #196]
	ldr	r6, [pc, #212]
	mov	sl, r0
.L_080e0444:
	movs	r1, #1
	ldr	r0, [r5, #24]
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L_080e0494
	cmp	r0, #0
	bge.n	.L_080e0454
	adds	r0, #15
.L_080e0454:
	asrs	r0, r0, #4
	adds	r0, #1
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r2, [sp, #24]
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
	ldr	r0, [sp, #20]
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #28]
	bl	sub_080072f4
	movs	r2, #128
	adds	r0, r5, #0
	movs	r1, #62
	lsls	r2, r2, #6
	bl	sub_080e3908
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_080e0494:
	movs	r1, #1
	movs	r2, #128
	add	sl, r1
	lsls	r2, r2, #2
	adds	r5, #28
	cmp	sl, r2
	bne.n	.L_080e0444
	movs	r1, #4
	movs	r0, #4
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #108]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r7, [pc, #96]
	ldr	r3, [sp, #16]
	movs	r0, #1
	add	fp, r0
	adds	r3, r3, r7
	mov	r1, fp
	str	r3, [sp, #16]
	cmp	r1, #96
	beq.n	.L_080e04d0
	b.n	.L_080e02dc
.L_080e04d0:
	ldr	r0, [pc, #40]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x02010018
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0x000001ff
	.4byte 0x000077a8
	.4byte 0x00007828
	.4byte 0x080ede48
	.4byte 0x00007824
	.4byte 0xfffff800
