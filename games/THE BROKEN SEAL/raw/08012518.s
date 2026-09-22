.syntax unified
	.thumb
	.set sub_08002f0c, 0x08002f0c
	.set sub_080030f8, 0x080030f8
	.set sub_08003bb4, 0x08003bb4
	.set sub_0800403c, 0x0800403c
	.set sub_080040e8, 0x080040e8
	.set sub_080041d8, 0x080041d8
	.set sub_08004858, 0x08004858
	.set sub_080048f4, 0x080048f4
	.set sub_0800b6b8, 0x0800b6b8
	.set sub_0800b8ac, 0x0800b8ac
	.set sub_0800bc70, 0x0800bc70
	.set sub_0800c004, 0x0800c004
	.set sub_08012af8, 0x08012af8
	.set sub_08012b2c, 0x08012b2c
	.set sub_08012d20, 0x08012d20
	.set sub_08012d48, 0x08012d48
	.set sub_08012d70, 0x08012d70
	.set sub_08012de8, 0x08012de8
	.set sub_08185000, 0x08185000
	.global Overlay_08012518
Overlay_08012518:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #64
	movs	r0, #144
	movs	r1, #96
	movs	r2, #1
	str	r0, [sp, #24]
	str	r1, [sp, #20]
	movs	r3, #0
	movs	r1, #160
	movs	r0, #9
	mov	sl, r3
	str	r2, [sp, #16]
	str	r2, [sp, #12]
	bl	sub_080048f4
	ldr	r2, [pc, #116]
	str	r0, [sp, #8]
	add	r4, sp, #28
	mov	r0, sl
	movs	r3, #3
	strb	r3, [r2, #0]
	str	r0, [r4, #0]
	ldr	r3, [pc, #104]
	adds	r0, r4, #0
	ldr	r1, [sp, #8]
	ldr	r2, [pc, #104]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r1, sl
	str	r1, [r4, #0]
	adds	r0, r4, #0
	add	r1, sp, #32
	ldr	r2, [pc, #92]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	movs	r1, #1
	negs	r0, r0
	bl	sub_08012af8
	add	r2, sp, #32
	mov	r8, r2
	ldr	r1, [pc, #48]
	movs	r7, #0
	movs	r2, #1
	mov	r3, r8
.L_08012580:
	adds	r7, #1
	strh	r2, [r3, #2]
	strb	r1, [r3, #5]
	strh	r0, [r3, #0]
	adds	r3, #8
	cmp	r7, #3
	bls.n	.L_08012580
	movs	r3, #1
	mov	r0, r8
	ldr	r2, [pc, #52]
	strb	r3, [r0, #4]
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #16]
	movs	r2, #160
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	b.n	.L_080125d0
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x00003f42
	.4byte 0x000001e0
	.4byte 0x03001c90
	.4byte 0x040000d4
	.4byte 0x85000001
	.4byte 0x85000008
	.4byte 0x03001f54
	.2byte 0x0050
	.2byte 0x0400
.L_080125d0:
	movs	r2, #128
	ldr	r3, [pc, #60]
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08003bb4
.L_080125de:
	bl	sub_08004858
	bl	sub_080040e8
	movs	r1, #160
	movs	r0, #9
	bl	sub_080048f4
	str	r0, [sp, #8]
	bl	sub_0800403c
	movs	r0, #2
	bl	sub_0800c004
	mov	r0, r8
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	ldr	r1, [pc, #16]
	movs	r3, #0
	movs	r0, #0
	bl	sub_0800b6b8
	mov	r2, r8
	b.n	.L_08012618
	movs	r0, r0
	.4byte 0x00001140
	.2byte 0x0000
	.2byte 0x0201
.L_08012618:
	movs	r1, #0
	ldrsh	r0, [r2, r1]
	bl	sub_08185000
	ldrb	r3, [r0, #4]
	cmp	r3, #20
	bne.n	.L_08012638
	mov	r0, r8
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	ldr	r1, [pc, #780]
	adds	r2, #1
	movs	r0, #1
	movs	r3, #0
	bl	sub_0800b6b8
.L_08012638:
	movs	r7, #0
	mov	r6, r8
.L_0801263c:
	movs	r1, #0
	ldrsh	r0, [r6, r1]
	bl	sub_08185000
	ldrb	r3, [r0, #4]
	movs	r5, #0
	cmp	r3, #20
	bne.n	.L_08012656
	movs	r3, #1
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08012656
	movs	r5, #1
.L_08012656:
	movs	r2, #0
	ldrsh	r0, [r6, r2]
	lsls	r3, r5, #12
	adds	r0, r0, r5
	adds	r0, r0, r3
	bl	sub_0800bc70
	movs	r3, #8
	ldrsh	r1, [r6, r3]
	adds	r5, r0, #0
	bl	sub_0800b8ac
	movs	r0, #16
	ldrsh	r1, [r6, r0]
	adds	r0, r5, #0
	bl	sub_0800b8ac
	adds	r0, r5, #0
	movs	r2, #24
	ldrsh	r1, [r6, r2]
	bl	sub_0800b8ac
	add	r3, sp, #12
	ldrb	r3, [r3, #0]
	adds	r5, #38
	adds	r7, #1
	strb	r3, [r5, #0]
	cmp	r7, #9
	bls.n	.L_0801263c
	mov	r6, r8
	movs	r7, #0
	mov	r5, r8
	adds	r6, #4
	movs	r2, #4
.L_0801269a:
	mov	r0, r8
	ldrb	r3, [r2, r0]
	cmp	r3, #0
	beq.n	.L_080126ac
	movs	r1, #1
	ldrsb	r1, [r6, r1]
	adds	r0, r7, #0
	str	r2, [sp, #4]
	b.n	.L_080126b2
.L_080126ac:
	adds	r0, r7, #0
	movs	r1, #8
	str	r2, [sp, #4]
.L_080126b2:
	bl	sub_08012d48
	ldr	r2, [sp, #4]
	movs	r1, #6
	ldrsb	r1, [r5, r1]
	adds	r0, r7, #0
	str	r2, [sp, #4]
	bl	sub_08012d20
	movs	r3, #2
	ldrsh	r1, [r5, r3]
	adds	r0, r7, #0
	bl	sub_08012d70
	ldr	r2, [sp, #4]
	adds	r7, #1
	adds	r5, #8
	adds	r6, #8
	adds	r2, #8
	cmp	r7, #3
	bls.n	.L_0801269a
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #8]
	bl	sub_08012b2c
	movs	r1, #200
	ldr	r0, [pc, #596]
	lsls	r1, r1, #4
	bl	sub_080041d8
.L_080126f0:
	movs	r0, #1
	bl	sub_080030f8
	ldr	r0, [pc, #588]
	ldr	r1, [pc, #588]
	mov	fp, r0
	mov	r9, r1
.L_080126fe:
	mov	r3, fp
	ldr	r2, [r3, #0]
	movs	r3, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0801275e
	mov	r0, fp
	ldr	r2, [r0, #0]
	movs	r3, #32
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0801271c
	ldr	r1, [sp, #24]
	subs	r1, #1
	str	r1, [sp, #24]
.L_0801271c:
	mov	r3, fp
	ldr	r2, [r3, #0]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0801272e
	ldr	r0, [sp, #24]
	adds	r0, #1
	str	r0, [sp, #24]
.L_0801272e:
	mov	r1, fp
	ldr	r2, [r1, #0]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08012740
	ldr	r2, [sp, #20]
	subs	r2, #1
	str	r2, [sp, #20]
.L_08012740:
	mov	r3, fp
	ldr	r2, [r3, #0]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08012752
	ldr	r0, [sp, #20]
	adds	r0, #1
	str	r0, [sp, #20]
.L_08012752:
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #8]
	bl	sub_08012b2c
	b.n	.L_080127be
.L_0801275e:
	ldr	r1, [pc, #488]
	ldr	r3, [r1, #0]
	mov	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0801277c
	movs	r0, #1
	negs	r0, r0
	add	sl, r0
	mov	r2, sl
	movs	r3, #3
	ands	r2, r3
	mov	sl, r2
.L_0801277c:
	ldr	r2, [r1, #0]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08012792
	movs	r3, #1
	add	sl, r3
	mov	r0, sl
	movs	r3, #3
	ands	r0, r3
	mov	sl, r0
.L_08012792:
	ldr	r2, [r1, #0]
	movs	r3, #32
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080127a6
	ldr	r2, [sp, #16]
	movs	r3, #3
	subs	r2, #1
	ands	r2, r3
	str	r2, [sp, #16]
.L_080127a6:
	ldr	r2, [r1, #0]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080127be
	ldr	r3, [sp, #16]
	adds	r3, #1
	str	r3, [sp, #16]
	ldr	r0, [sp, #16]
	movs	r3, #3
	ands	r0, r3
	str	r0, [sp, #16]
.L_080127be:
	ldr	r3, [pc, #396]
	ldr	r2, [r3, #0]
	movs	r3, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080127e8
	ldr	r3, [pc, #388]
	ldr	r1, [sp, #12]
	ldr	r2, [r3, #0]
	movs	r3, #1
	eors	r1, r3
	str	r1, [sp, #12]
	movs	r7, #0
	adds	r2, #38
.L_080127da:
	add	r3, sp, #12
	ldrb	r3, [r3, #0]
	adds	r7, #1
	strb	r3, [r2, #0]
	adds	r2, #56
	cmp	r7, #9
	bls.n	.L_080127da
.L_080127e8:
	ldr	r0, [sp, #16]
	cmp	r0, #1
	beq.n	.L_0801287c
	cmp	r0, #1
	bcc.n	.L_08012800
	cmp	r0, #2
	bne.n	.L_080127f8
	b.n	.L_08012954
.L_080127f8:
	cmp	r0, #3
	bne.n	.L_080127fe
	b.n	.L_080129ca
.L_080127fe:
	b.n	.L_08012a46
.L_08012800:
	mov	r1, sl
	cmp	r1, #1
	bne.n	.L_08012808
	b.n	.L_08012a46
.L_08012808:
	ldr	r1, [pc, #316]
	movs	r3, #128
	ldr	r2, [r1, #0]
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08012840
	mov	r2, sl
	mov	r3, r8
	lsls	r6, r2, #3
	adds	r2, r3, r6
	ldrh	r3, [r2, #2]
	subs	r3, #1
	strh	r3, [r2, #2]
	lsls	r3, r3, #16
	movs	r1, #0
	cmp	r3, #0
	bge.n	.L_0801282e
	strh	r1, [r2, #2]
.L_0801282e:
	mov	r0, sl
	cmp	r0, #0
	beq.n	.L_08012836
	b.n	.L_080126f0
.L_08012836:
	mov	r1, r8
	ldrh	r3, [r1, #2]
	mov	r2, r8
	strh	r3, [r2, #10]
	b.n	.L_080126f0
.L_08012840:
	ldr	r2, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_0801284e
	b.n	.L_08012a46
.L_0801284e:
	mov	r3, sl
	lsls	r6, r3, #3
	mov	r0, r8
	adds	r2, r0, r6
	ldrh	r3, [r2, #2]
	movs	r1, #198
	adds	r3, #1
	strh	r3, [r2, #2]
	lsls	r1, r1, #15
	lsls	r3, r3, #16
	cmp	r3, r1
	ble.n	.L_0801286a
	movs	r3, #99
	strh	r3, [r2, #2]
.L_0801286a:
	mov	r2, sl
	cmp	r2, #0
	beq.n	.L_08012872
	b.n	.L_080126f0
.L_08012872:
	mov	r0, r8
	ldrh	r3, [r0, #2]
	mov	r1, r8
	strh	r3, [r1, #10]
	b.n	.L_080126f0
.L_0801287c:
	mov	r3, fp
	ldr	r2, [r3, #0]
	movs	r3, #8
	ands	r2, r3
	movs	r1, #0
	movs	r4, #1
	cmp	r2, #0
	beq.n	.L_0801288e
	movs	r4, #10
.L_0801288e:
	mov	r0, r9
	ldr	r2, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080128c0
	movs	r7, #0
	cmp	r1, r4
	bcs.n	.L_080128be
	mov	r1, sl
	mov	r6, r8
	lsls	r5, r1, #3
.L_080128a8:
	movs	r1, #1
	ldrsh	r0, [r6, r5]
	negs	r1, r1
	str	r4, [sp, #0]
	bl	sub_08012af8
	ldr	r4, [sp, #0]
	adds	r7, #1
	strh	r0, [r6, r5]
	cmp	r7, r4
	bcc.n	.L_080128a8
.L_080128be:
	movs	r1, #1
.L_080128c0:
	mov	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080128f0
	movs	r7, #0
	cmp	r7, r4
	bcs.n	.L_080128ee
	mov	r0, sl
	mov	r6, r8
	lsls	r5, r0, #3
.L_080128da:
	ldrsh	r0, [r6, r5]
	movs	r1, #1
	str	r4, [sp, #0]
	bl	sub_08012af8
	ldr	r4, [sp, #0]
	adds	r7, #1
	strh	r0, [r6, r5]
	cmp	r7, r4
	bcc.n	.L_080128da
.L_080128ee:
	movs	r1, #1
.L_080128f0:
	cmp	r1, #0
	bne.n	.L_080128f6
	b.n	.L_08012a46
.L_080128f6:
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L_080128fe
	b.n	.L_080125de
.L_080128fe:
	lsls	r6, r2, #3
	adds	r5, r6, #4
	mov	r0, r8
	ldrb	r3, [r0, r5]
	cmp	r3, #0
	bne.n	.L_0801290c
	b.n	.L_080126f0
.L_0801290c:
	ldrsh	r1, [r0, r6]
	add	r5, r8
	mov	r0, sl
	bl	sub_08012de8
	movs	r1, #1
	ldrsb	r1, [r5, r1]
	mov	r0, sl
	bl	sub_08012d48
	movs	r1, #2
	ldrsb	r1, [r5, r1]
	mov	r0, sl
	bl	sub_08012d20
	mov	r0, r8
	adds	r3, r0, r6
	movs	r2, #2
	ldrsh	r1, [r3, r2]
	mov	r0, sl
	bl	sub_08012d70
	b.n	.L_080126f0
	movs	r0, r0
	.4byte 0x02018000
	.4byte 0x08012e29
	.4byte 0x03001ae8
	.4byte 0x03001b04
	.4byte 0x03001c94
	.2byte 0x1e60
	.2byte 0x0300
.L_08012954:
	mov	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	movs	r1, #0
	cmp	r2, #0
	beq.n	.L_08012980
	mov	r0, sl
	lsls	r6, r0, #3
	adds	r3, r6, #4
	mov	r1, r8
	adds	r2, r1, r3
	ldrb	r3, [r2, #1]
	subs	r3, #1
	strb	r3, [r2, #1]
	lsls	r3, r3, #24
	cmp	r3, #0
	bge.n	.L_0801297e
	movs	r3, #3
	strb	r3, [r2, #1]
.L_0801297e:
	movs	r1, #1
.L_08012980:
	mov	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080129ae
	mov	r0, sl
	lsls	r6, r0, #3
	adds	r3, r6, #4
	mov	r1, r8
	adds	r2, r1, r3
	ldrb	r3, [r2, #1]
	movs	r0, #192
	adds	r3, #1
	strb	r3, [r2, #1]
	lsls	r0, r0, #18
	lsls	r3, r3, #24
	movs	r1, #0
	cmp	r3, r0
	ble.n	.L_080129ac
	strb	r1, [r2, #1]
.L_080129ac:
	movs	r1, #1
.L_080129ae:
	cmp	r1, #0
	beq.n	.L_08012a46
	mov	r1, sl
	lsls	r6, r1, #3
	adds	r2, r6, #4
	mov	r0, r8
	ldrb	r3, [r0, r2]
	cmp	r3, #0
	bne.n	.L_080129c2
	b.n	.L_080126f0
.L_080129c2:
	adds	r3, r0, r2
	movs	r1, #1
	ldrsb	r1, [r3, r1]
	b.n	.L_08012ab0
.L_080129ca:
	mov	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	movs	r1, #0
	cmp	r2, #0
	beq.n	.L_080129f6
	mov	r0, sl
	lsls	r6, r0, #3
	adds	r3, r6, #4
	mov	r1, r8
	adds	r2, r1, r3
	ldrb	r3, [r2, #2]
	subs	r3, #1
	strb	r3, [r2, #2]
	lsls	r3, r3, #24
	cmp	r3, #0
	bge.n	.L_080129f4
	movs	r3, #15
	strb	r3, [r2, #2]
.L_080129f4:
	movs	r1, #1
.L_080129f6:
	mov	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08012a24
	mov	r0, sl
	lsls	r6, r0, #3
	adds	r3, r6, #4
	mov	r1, r8
	adds	r2, r1, r3
	ldrb	r3, [r2, #2]
	movs	r0, #240
	adds	r3, #1
	strb	r3, [r2, #2]
	lsls	r0, r0, #20
	lsls	r3, r3, #24
	movs	r1, #0
	cmp	r3, r0
	ble.n	.L_08012a22
	strb	r1, [r2, #2]
.L_08012a22:
	movs	r1, #1
.L_08012a24:
	cmp	r1, #0
	beq.n	.L_08012a46
	mov	r1, sl
	lsls	r6, r1, #3
	adds	r2, r6, #4
	mov	r0, r8
	ldrb	r3, [r0, r2]
	cmp	r3, #0
	bne.n	.L_08012a38
	b.n	.L_080126f0
.L_08012a38:
	adds	r3, r0, r2
	movs	r1, #2
	ldrsb	r1, [r3, r1]
	mov	r0, sl
	bl	sub_08012d20
	b.n	.L_080126f0
.L_08012a46:
	mov	r1, r9
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08012ab8
	ldr	r2, [sp, #16]
	cmp	r2, #0
	bne.n	.L_08012a8e
	mov	r3, sl
	cmp	r3, #1
	beq.n	.L_08012ab8
	lsls	r6, r3, #3
	adds	r3, r6, #4
	mov	r0, r8
	ldrb	r3, [r0, r3]
	cmp	r3, #0
	bne.n	.L_08012a6c
	b.n	.L_080126f0
.L_08012a6c:
	adds	r3, r0, r6
	movs	r2, #2
	ldrsh	r1, [r3, r2]
	mov	r0, sl
	bl	sub_08012d70
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_08012a80
	b.n	.L_080126f0
.L_08012a80:
	mov	r2, r8
	movs	r0, #10
	ldrsh	r1, [r2, r0]
	movs	r0, #1
	bl	sub_08012d70
	b.n	.L_080126f0
.L_08012a8e:
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_08012ab8
	lsls	r3, r3, #3
	adds	r1, r3, #4
	mov	r0, r8
	ldrb	r2, [r0, r1]
	movs	r3, #1
	eors	r2, r3
	strb	r2, [r0, r1]
	cmp	r2, #0
	beq.n	.L_08012aae
	adds	r3, r0, r1
	movs	r1, #1
	ldrsb	r1, [r3, r1]
	b.n	.L_08012ab0
.L_08012aae:
	movs	r1, #8
.L_08012ab0:
	mov	r0, sl
	bl	sub_08012d48
	b.n	.L_080126f0
.L_08012ab8:
	mov	r1, r9
	ldr	r2, [r1, #0]
	movs	r3, #4
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08012ae2
	bl	sub_080040e8
	mov	r2, fp
	ldr	r3, [r2, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08012ad8
	ldr	r0, [pc, #20]
	b.n	.L_08012ada
.L_08012ad8:
	ldr	r0, [pc, #20]
.L_08012ada:
	ldr	r1, [pc, #24]
	bl	sub_08002f0c
	b.n	.L_080125de
.L_08012ae2:
	movs	r0, #1
	bl	sub_080030f8
	b.n	.L_080126fe
	movs	r0, r0
	.4byte 0x00000011
	.4byte 0x00000012
	.4byte 0x08185000
