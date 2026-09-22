.syntax unified
	.thumb
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
	.set sub_08014de4, 0x08014de4
	.set sub_080156e8, 0x080156e8
	.set sub_08118040, 0x08118040
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138058, 0x08138058
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143a88, 0x08143a88
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_08145aac, 0x08145aac
	.set sub_08145aea, 0x08145aea
	.set sub_08145b18, 0x08145b18
	.set sub_08145b46, 0x08145b46
	.set sub_08145b74, 0x08145b74
	.set sub_08145ba2, 0x08145ba2
	.set sub_0814cd48, 0x0814cd48
	.set sub_081504b4, 0x081504b4
	.set sub_081504c0, 0x081504c0
	.set sub_0815585c, 0x0815585c
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e20c, 0x0815e20c
	.set sub_081963ec, 0x081963ec
	.set sub_081c0010, 0x081c0010
	.global Overlay_0814512c
Overlay_0814512c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #100
	str	r1, [sp, #48]
	str	r0, [sp, #52]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	ldr	r2, [sp, #48]
	str	r0, [sp, #44]
	ldr	r1, [r3, #96]
	str	r1, [sp, #40]
	ldr	r3, [r3, #48]
	str	r3, [sp, #20]
	cmp	r2, #7
	bne.n	.L_08145180
	movs	r0, #0
	bl	sub_081435e0
	ldr	r3, [sp, #52]
	add	r2, sp, #64
	ldr	r1, [r3, #4]
	movs	r3, #68
	lsls	r1, r1, #4
	orrs	r1, r3
	ldr	r0, [sp, #52]
	add	r3, sp, #76
	bl	sub_0815585c
	movs	r2, #128
	ldr	r3, [pc, #8]
	lsls	r2, r2, #19
	adds	r2, #12
	b.n	.L_0814518e
	movs	r0, r0
	.2byte 0x0785
	.2byte 0x0000
.L_08145180:
	movs	r0, #1
	bl	sub_081435e0
	movs	r2, #128
	ldr	r3, [pc, #52]
	lsls	r2, r2, #19
	adds	r2, #82
.L_0814518e:
	strh	r3, [r2, #0]
	ldr	r4, [sp, #44]
	ldr	r5, [pc, #48]
	movs	r0, #224
	lsls	r0, r0, #3
	adds	r1, r4, r0
	movs	r2, #1
	adds	r0, r5, #0
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r2, [sp, #44]
	movs	r3, #139
	lsls	r3, r3, #7
	adds	r1, r2, r3
	ldr	r0, [pc, #24]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r4, [sp, #48]
	cmp	r4, #0
	bne.n	.L_081451d2
	ldr	r0, [pc, #12]
	b.n	.L_081451d0
	.4byte 0x00001010
	.4byte 0x00000183
	.4byte 0x00000161
	.2byte 0x0162
	.2byte 0x0000
.L_081451d0:
	b.n	.L_0814521a
.L_081451d2:
	ldr	r0, [sp, #48]
	cmp	r0, #1
	bne.n	.L_081451dc
	ldr	r0, [pc, #684]
	b.n	.L_0814521a
.L_081451dc:
	ldr	r1, [sp, #48]
	cmp	r1, #2
	bne.n	.L_081451e6
	ldr	r0, [pc, #680]
	b.n	.L_0814521a
.L_081451e6:
	ldr	r2, [sp, #48]
	cmp	r2, #8
	bne.n	.L_081451f0
	ldr	r0, [pc, #668]
	b.n	.L_0814521a
.L_081451f0:
	ldr	r3, [sp, #48]
	cmp	r3, #3
	beq.n	.L_08145218
	ldr	r4, [sp, #48]
	cmp	r4, #4
	beq.n	.L_08145200
	cmp	r4, #7
	bne.n	.L_08145204
.L_08145200:
	adds	r0, r5, #0
	b.n	.L_0814521a
.L_08145204:
	ldr	r0, [sp, #48]
	cmp	r0, #6
	bne.n	.L_0814520e
	ldr	r0, [pc, #644]
	b.n	.L_0814521a
.L_0814520e:
	ldr	r1, [sp, #48]
	cmp	r1, #9
	bne.n	.L_08145218
	ldr	r0, [pc, #636]
	b.n	.L_0814521a
.L_08145218:
	ldr	r0, [pc, #636]
.L_0814521a:
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #632]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2200
	ldr	r3, [pc, #624]
	str	r2, [sp, #32]
	movs	r1, #1
	movs	r2, #128
	negs	r1, r1
	lsls	r2, r2, #3
.L_0814523a:
	str	r1, [r3, #0]
	ldr	r4, [sp, #32]
	adds	r3, #28
	adds	r4, #1
	str	r4, [sp, #32]
	cmp	r4, r2
	bne.n	.L_0814523a
	ldr	r1, [sp, #52]
	ldr	r0, [r1, #8]
	bl	sub_08118098
	ldr	r3, [sp, #48]
	movs	r2, #160
	lsls	r2, r2, #14
	ldr	r7, [r0, #0]
	mov	sl, r2
	cmp	r3, #7
	beq.n	.L_08145270
	ldr	r0, [sp, #48]
	movs	r4, #160
	lsls	r4, r4, #13
	mov	sl, r4
	cmp	r0, #8
	beq.n	.L_08145270
	movs	r1, #128
	lsls	r1, r1, #11
	mov	sl, r1
.L_08145270:
	movs	r2, #0
	str	r2, [sp, #28]
	ldr	r3, [sp, #52]
	ldr	r1, [r3, #20]
	cmp	r1, #0
	beq.n	.L_081452f4
	mov	r8, r2
.L_0814527e:
	ldr	r0, [sp, #28]
	bl	sub_08002064
	ldr	r4, [sp, #52]
	lsls	r0, r0, #1
	adds	r0, #36
	ldrsh	r0, [r4, r0]
	bl	sub_08118098
	ldr	r5, [pc, #528]
	movs	r2, #0
	ldr	r6, [r0, #0]
	str	r2, [sp, #32]
	add	r5, r8
.L_0814529a:
	ldr	r3, [r7, #8]
	str	r3, [r5, #0]
	mov	r3, sl
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	str	r3, [r5, #8]
	bl	sub_08014878
	movs	r3, #31
	ldr	r1, [r6, #16]
	ands	r3, r0
	subs	r3, #16
	lsls	r3, r3, #17
	ldr	r2, [r7, #8]
	adds	r1, r1, r3
	ldr	r3, [r6, #8]
	subs	r3, r3, r2
	asrs	r3, r3, #4
	str	r3, [r5, #12]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #16]
	ldr	r3, [r7, #16]
	subs	r1, r1, r3
	asrs	r1, r1, #4
	movs	r3, #0
	str	r1, [r5, #20]
	str	r3, [r5, #24]
	ldr	r4, [sp, #32]
	adds	r5, #28
	adds	r4, #1
	str	r4, [sp, #32]
	cmp	r4, #16
	bne.n	.L_0814529a
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #52]
	adds	r1, #1
	str	r1, [sp, #28]
	ldr	r3, [sp, #28]
	ldr	r1, [r2, #20]
	movs	r0, #224
	lsls	r0, r0, #1
	add	r8, r0
	cmp	r3, r1
	bne.n	.L_0814527e
.L_081452f4:
	movs	r4, #0
	str	r4, [sp, #32]
	ldr	r3, [pc, #428]
	movs	r1, #1
	movs	r2, #128
	negs	r1, r1
	lsls	r2, r2, #1
.L_08145302:
	str	r1, [r3, #0]
	ldr	r0, [sp, #32]
	adds	r3, #28
	adds	r0, #1
	str	r0, [sp, #32]
	cmp	r0, r2
	bne.n	.L_08145302
	ldr	r1, [sp, #52]
	mov	r2, sp
	adds	r2, #56
	ldr	r0, [r1, #4]
	adds	r1, r2, #0
	str	r2, [sp, #16]
	bl	sub_08144aac
	ldr	r3, [sp, #48]
	cmp	r3, #4
	beq.n	.L_0814532a
	cmp	r3, #7
	bne.n	.L_0814534c
.L_0814532a:
	ldr	r4, [sp, #44]
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	adds	r2, r4, r0
	movs	r3, #1
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r3, r4, r1
	movs	r2, #0
	str	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #80
	strh	r2, [r3, #0]
	b.n	.L_08145366
.L_0814534c:
	ldr	r3, [sp, #44]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r0, [sp, #44]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #132
	adds	r2, r0, r1
	movs	r3, #75
	str	r3, [r2, #0]
.L_08145366:
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #320]
	bl	sub_080145a8
	ldr	r3, [sp, #48]
	ldr	r2, [pc, #316]
	ldr	r4, [sp, #52]
	lsls	r3, r3, #2
	mov	fp, r3
	adds	r3, #3
	ldrsb	r2, [r2, r3]
	ldr	r3, [r4, #20]
	movs	r0, #103
	lsls	r3, r3, #3
	adds	r2, r2, r3
	str	r2, [sp, #24]
	bl	sub_081c0010
	ldr	r1, [sp, #24]
	movs	r0, #0
	str	r0, [sp, #36]
	cmp	r1, #0
	bne.n	.L_08145398
	b.n	.L_081456fc
.L_08145398:
	ldr	r2, [sp, #20]
	adds	r2, #12
	str	r2, [sp, #12]
.L_0814539e:
	bl	sub_08014de4
	ldr	r1, [sp, #12]
	ldr	r0, [sp, #20]
	bl	sub_080156e8
	movs	r3, #0
	str	r3, [sp, #28]
	ldr	r4, [sp, #52]
	ldr	r1, [r4, #20]
	cmp	r1, #0
	bne.n	.L_081453b8
	b.n	.L_081455d0
.L_081453b8:
	ldr	r2, [pc, #244]
.L_081453ba:
	movs	r0, #0
	str	r0, [sp, #32]
	mov	r3, fp
	adds	r3, #2
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	bne.n	.L_081453ca
	b.n	.L_081455c4
.L_081453ca:
	ldr	r1, [sp, #28]
	lsls	r1, r1, #3
	mov	r9, r1
.L_081453d0:
	ldr	r3, [sp, #36]
	cmp	r3, r9
	bge.n	.L_081453d8
	b.n	.L_081455a6
.L_081453d8:
	ldr	r4, [sp, #28]
	ldr	r0, [sp, #32]
	lsls	r2, r4, #4
	adds	r2, r2, r0
	lsls	r3, r2, #3
	ldr	r1, [pc, #192]
	subs	r3, r3, r2
	lsls	r3, r3, #2
	ldr	r2, [sp, #36]
	adds	r7, r3, r1
	mov	r3, r9
	adds	r3, #17
	cmp	r2, r3
	bne.n	.L_0814541a
	ldr	r3, [sp, #52]
	adds	r0, r4, #0
	ldr	r1, [r3, #20]
	bl	sub_08002064
	ldr	r4, [sp, #52]
	lsls	r0, r0, #1
	movs	r3, #16
	adds	r0, #36
	ldrsh	r0, [r4, r0]
	movs	r2, #5
	str	r3, [sp, #0]
	movs	r1, #7
	ldr	r3, [sp, #28]
	bl	sub_0814cd48
	movs	r0, #133
	bl	sub_081180e8
.L_0814541a:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	bge.n	.L_08145422
	b.n	.L_081455a4
.L_08145422:
	ldr	r2, [sp, #36]
	mov	r3, r9
	subs	r0, r2, r3
	movs	r1, #3
	bl	sub_08002054
	adds	r6, r0, #0
	cmp	r6, #9
	ble.n	.L_08145436
	movs	r6, #9
.L_08145436:
	add	r5, sp, #88
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0815e1ec
	ldr	r3, [r5, #0]
	ldr	r0, [r5, #4]
	asrs	r2, r3, #1
	adds	r3, r0, #0
	subs	r3, #8
	str	r2, [r5, #0]
	str	r3, [r5, #4]
	cmp	r6, #4
	ble.n	.L_081454b4
	ldr	r1, [pc, #92]
	mov	r3, fp
	adds	r3, #1
	ldrsb	r4, [r1, r3]
	ldr	r3, [sp, #44]
	lsls	r1, r6, #1
	adds	r1, r1, r6
	lsls	r1, r1, #8
	adds	r1, r3, r1
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r1, r3
	adds	r3, r0, #0
	movs	r0, #32
	str	r0, [sp, #0]
	movs	r0, #24
	str	r0, [sp, #4]
	ldr	r0, [sp, #16]
	lsls	r4, r4, #2
	ldr	r4, [r4, r0]
	subs	r2, #16
	subs	r3, #20
	ldr	r0, [sp, #40]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe02f
	movs	r0, r0
	.4byte 0x0000011b
	.4byte 0x00000163
	.4byte 0x00000150
	.4byte 0x0000017f
	.4byte 0x00000138
	.4byte 0x03000730
	.4byte 0x02010018
	.4byte 0x02010000
	.4byte 0x02011c18
	.4byte 0x08143001
	.2byte 0x7880
	.2byte 0x0819
.L_081454b4:
	ldr	r1, [pc, #676]
	mov	r3, fp
	adds	r3, #1
	ldrsb	r4, [r1, r3]
	ldr	r3, [sp, #44]
	lsls	r1, r6, #1
	adds	r1, r1, r6
	lsls	r1, r1, #8
	adds	r1, r3, r1
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r1, r3
	adds	r3, r0, #0
	movs	r0, #24
	str	r0, [sp, #0]
	movs	r0, #32
	str	r0, [sp, #4]
	ldr	r0, [sp, #16]
	lsls	r4, r4, #2
	ldr	r4, [r4, r0]
	subs	r2, #12
	subs	r3, #24
	ldr	r0, [sp, #40]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69bb
	cmp	r3, #0
	bne.n	.L_081454f6
	adds	r0, r7, #0
	movs	r1, #63
	ldr	r2, [pc, #620]
	bl	sub_08138058
.L_081454f6:
	ldr	r3, [r7, #4]
	cmp	r3, #0
	bge.n	.L_081455a4
	ldr	r2, [pc, #604]
	movs	r3, #0
	str	r3, [r7, #4]
	mov	r1, fp
	movs	r3, #1
	str	r3, [r7, #24]
	ldrsb	r3, [r2, r1]
	movs	r4, #4
	mov	sl, r4
	cmp	r3, #0
	beq.n	.L_08145516
	movs	r0, #16
	mov	sl, r0
.L_08145516:
	movs	r1, #0
	mov	r3, sl
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L_081455a6
	ldr	r4, [sp, #28]
	ldr	r0, [sp, #32]
	lsls	r2, r4, #2
	adds	r2, r2, r0
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r2, [pc, #564]
	lsls	r3, r3, #5
	movs	r1, #63
	adds	r5, r3, r2
.L_08145534:
	ldr	r3, [r7, #0]
	mov	r4, fp
	str	r3, [r5, #0]
	ldr	r3, [r7, #4]
	str	r3, [r5, #4]
	ldr	r3, [r7, #8]
	str	r3, [r5, #8]
	ldr	r3, [pc, #536]
	ldrsb	r6, [r3, r4]
	cmp	r6, #0
	bne.n	.L_0814556a
	str	r1, [sp, #8]
	bl	sub_08014878
	ldr	r1, [sp, #8]
	str	r6, [r5, #16]
	ands	r0, r1
	subs	r0, #32
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08014878
	ldr	r1, [sp, #8]
	ands	r0, r1
	subs	r0, #32
	lsls	r0, r0, #11
	b.n	.L_08145594
.L_0814556a:
	str	r1, [sp, #8]
	bl	sub_08014878
	ldr	r1, [sp, #8]
	ands	r0, r1
	subs	r0, #32
	lsls	r0, r0, #13
	str	r0, [r5, #12]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r3, #32
	lsls	r3, r3, #12
	str	r3, [r5, #16]
	bl	sub_08014878
	ldr	r1, [sp, #8]
	ands	r0, r1
	subs	r0, #32
	lsls	r0, r0, #13
.L_08145594:
	str	r0, [r5, #20]
	movs	r0, #1
	movs	r3, #0
	add	r8, r0
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r8, sl
	bne.n	.L_08145534
.L_081455a4:
	ldr	r2, [pc, #436]
.L_081455a6:
	ldr	r3, [sp, #32]
	ldr	r4, [pc, #432]
	adds	r3, #1
	str	r3, [sp, #32]
	mov	r3, fp
	adds	r3, #2
	ldrsb	r3, [r4, r3]
	ldr	r0, [sp, #32]
	movs	r1, #4
	add	r9, r1
	cmp	r0, r3
	beq.n	.L_081455c0
	b.n	.L_081453d0
.L_081455c0:
	ldr	r3, [sp, #52]
	ldr	r1, [r3, #20]
.L_081455c4:
	ldr	r4, [sp, #28]
	adds	r4, #1
	str	r4, [sp, #28]
	cmp	r4, r1
	beq.n	.L_081455d0
	b.n	.L_081453ba
.L_081455d0:
	movs	r0, #0
	ldr	r1, [pc, #400]
	ldr	r2, [pc, #388]
	str	r0, [sp, #32]
	mov	r8, r1
	mov	sl, r2
.L_081455dc:
	mov	r4, r8
	ldr	r3, [r4, #24]
	cmp	r3, #44
	bhi.n	.L_081456bc
	ldr	r3, [r4, #4]
	cmp	r3, #0
	blt.n	.L_081456bc
	add	r6, sp, #88
	adds	r1, r6, #0
	mov	r0, r8
	bl	sub_0815e1ec
	ldr	r3, [r6, #0]
	mov	r0, sl
	mov	r1, fp
	asrs	r2, r3, #1
	ldrsb	r3, [r0, r1]
	str	r2, [r6, #0]
	cmp	r3, #0
	bne.n	.L_0814563a
	mov	r4, r8
	ldr	r3, [r4, #24]
	cmp	r3, #0
	bge.n	.L_0814560e
	adds	r3, #7
.L_0814560e:
	asrs	r3, r3, #3
	lsls	r1, r3, #3
	ldr	r0, [sp, #44]
	adds	r1, r1, r3
	lsls	r1, r1, #7
	movs	r3, #139
	adds	r1, r0, r1
	lsls	r3, r3, #7
	movs	r0, #24
	adds	r1, r1, r3
	ldr	r3, [r6, #4]
	str	r0, [sp, #0]
	movs	r0, #48
	str	r0, [sp, #4]
	ldr	r0, [sp, #16]
	subs	r2, #12
	ldr	r4, [r0, #4]
	subs	r3, #24
	ldr	r0, [sp, #40]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe02b
.L_0814563a:
	mov	r1, r8
	ldr	r0, [r1, #24]
	movs	r1, #5
	bl	sub_08002054
	ldr	r3, [sp, #32]
	movs	r1, #1
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08145650
	adds	r0, #9
.L_08145650:
	ldr	r3, [sp, #52]
	mov	r4, r8
	ldr	r2, [r3, #4]
	ldr	r3, [r4, #12]
	cmp	r3, #0
	ble.n	.L_0814565e
	eors	r2, r1
.L_0814565e:
	lsls	r7, r2, #2
	ldr	r2, [pc, #260]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #44]
	movs	r3, #148
	adds	r1, r2, r1
	lsls	r3, r3, #6
	adds	r1, r1, r3
	ldr	r3, [pc, #248]
	ldr	r2, [r6, #0]
	ldrb	r5, [r3, r0]
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #244]
	ldrb	r4, [r3, r0]
	ldr	r3, [r6, #4]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	ldr	r0, [sp, #16]
	str	r5, [sp, #0]
	str	r4, [sp, #4]
	ldr	r4, [r7, r0]
	ldr	r0, [sp, #40]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4651
	mov	r2, fp
	ldrsb	r3, [r1, r2]
	cmp	r3, #0
	bne.n	.L_081456aa
	movs	r2, #128
	mov	r0, r8
	movs	r1, #62
	lsls	r2, r2, #4
	bl	sub_08138058
	b.n	.L_081456b4
.L_081456aa:
	mov	r0, r8
	movs	r1, #62
	ldr	r2, [pc, #176]
	bl	sub_08138058
.L_081456b4:
	mov	r4, r8
	ldr	r3, [r4, #24]
	adds	r3, #1
	str	r3, [r4, #24]
.L_081456bc:
	ldr	r1, [sp, #32]
	movs	r2, #128
	movs	r0, #28
	adds	r1, #1
	lsls	r2, r2, #1
	add	r8, r0
	str	r1, [sp, #32]
	cmp	r1, r2
	bne.n	.L_081455dc
	movs	r1, #4
	movs	r0, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r4, #240
	ldr	r3, [sp, #44]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #24]
	adds	r0, #1
	str	r0, [sp, #36]
	cmp	r0, r1
	beq.n	.L_081456fc
	b.n	.L_0814539e
.L_081456fc:
	ldr	r0, [pc, #116]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r2, [sp, #48]
	cmp	r2, #3
	beq.n	.L_08145718
	cmp	r2, #5
	bne.n	.L_0814574a
.L_08145718:
	movs	r1, #240
	ldr	r5, [pc, #92]
	lsls	r1, r1, #6
	ldr	r0, [pc, #92]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x21f0
	ldr	r0, [sp, #40]
	lsls	r1, r1, #6
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4814
	bl	sub_08014644
	ldr	r3, [sp, #48]
	cmp	r3, #3
	bne.n	.L_08145742
	ldr	r0, [sp, #52]
	bl	sub_081504c0
	b.n	.L_0814574e
.L_08145742:
	ldr	r0, [sp, #52]
	bl	sub_081504b4
	b.n	.L_0814574e
.L_0814574a:
	bl	sub_08143bb8
.L_0814574e:
	add	sp, #100
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x08197880
	.4byte 0xffff8000
	.4byte 0x02011c00
	.4byte 0x081978cc
	.4byte 0x081978a8
	.4byte 0x081978ba
	.4byte 0x08143001
	.4byte 0x03000258
	.4byte 0x06004000
	.2byte 0x3489
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r1, [r5, #92]
	sub	sp, #72
	str	r1, [sp, #36]
	mov	fp, r0
	ldr	r2, [r5, #96]
	movs	r0, #128
	movs	r3, #0
	lsls	r0, r0, #6
	str	r2, [sp, #32]
	mov	r8, r3
	bl	sub_08143a88
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r4, [sp, #36]
	ldr	r6, [pc, #48]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r4, r2
	ldr	r0, [pc, #44]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r2, #1
	adds	r1, r6, #0
	movs	r3, #0
	ldr	r0, [pc, #32]
	bl	sub_08157cf4
	movs	r1, #35
	movs	r0, #104
	bl	sub_081963ec
	ldr	r3, [r5, #104]
	b.n	.L_08145800
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00001010
	.4byte 0x02010000
	.4byte 0x0000016f
	.2byte 0x0170
	.2byte 0x0000
.L_08145800:
	movs	r1, #39
	movs	r0, #188
	str	r3, [sp, #40]
	bl	sub_081963ec
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r4, sp
	movs	r5, #200
	adds	r4, #40
	lsls	r5, r5, #4
	str	r4, [sp, #16]
	adds	r1, r5, #0
	str	r3, [r4, #4]
	ldr	r0, [pc, #204]
	bl	sub_080145a8
	ldr	r6, [sp, #36]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r6, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	adds	r3, r6, r2
	mov	r4, r8
	str	r4, [r3, #0]
	adds	r1, r5, #0
	ldr	r0, [pc, #176]
	bl	sub_080145a8
	movs	r5, #1
	str	r5, [sp, #20]
	mov	r6, fp
	ldr	r3, [r6, #4]
	cmp	r3, #1
	bne.n	.L_08145854
	ldr	r1, [pc, #164]
	str	r1, [sp, #24]
	b.n	.L_0814585a
.L_08145854:
	movs	r2, #224
	lsls	r2, r2, #15
	str	r2, [sp, #24]
.L_0814585a:
	ldr	r3, [pc, #156]
	movs	r4, #0
	str	r3, [sp, #28]
	mov	r8, r4
.L_08145862:
	mov	r6, r8
	lsls	r5, r6, #9
	adds	r0, r5, #0
	bl	sub_08002096
	ldr	r1, [sp, #24]
	lsls	r0, r0, #4
	asrs	r3, r1, #16
	asrs	r0, r0, #16
	adds	r3, r3, r0
	adds	r0, r5, #0
	adds	r7, r3, #0
	bl	sub_08002090
	ldr	r2, [sp, #28]
	lsls	r0, r0, #2
	asrs	r3, r2, #16
	asrs	r0, r0, #16
	adds	r3, r3, r0
	adds	r3, #16
	adds	r7, #48
	mov	r9, r3
	cmp	r6, #24
	bne.n	.L_081458b0
	mov	r4, fp
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne.n	.L_081458a0
	ldr	r5, [pc, #92]
	str	r5, [sp, #24]
	b.n	.L_081458a6
.L_081458a0:
	movs	r6, #144
	lsls	r6, r6, #15
	str	r6, [sp, #24]
.L_081458a6:
	movs	r1, #192
	lsls	r1, r1, #13
	movs	r2, #0
	str	r1, [sp, #28]
	str	r2, [sp, #20]
.L_081458b0:
	mov	r3, r8
	cmp	r3, #25
	bne.n	.L_081458c4
	ldr	r3, [pc, #48]
	movs	r4, #128
	lsls	r4, r4, #19
	adds	r4, #82
	movs	r5, #1
	strh	r3, [r4, #0]
	str	r5, [sp, #20]
.L_081458c4:
	mov	r6, r8
	cmp	r6, #48
	bne.n	.L_0814590a
	mov	r2, fp
	add	r5, sp, #48
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	adds	r1, r5, #0
	bl	sub_0815e20c
	mov	r4, fp
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne.n	.L_081458fc
	ldr	r3, [r5, #0]
	subs	r3, #128
	b.n	.L_08145900
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x08152475
	.4byte 0x08143001
	.4byte 0xffb00000
	.2byte 0x0000
	.2byte 0xffe0
.L_081458fc:
	.2byte 0x682b
	subs	r3, #64
.L_08145900:
	lsls	r3, r3, #16
	str	r3, [sp, #24]
	movs	r5, #0
	str	r5, [sp, #28]
	str	r5, [sp, #20]
.L_0814590a:
	mov	r6, r8
	cmp	r6, #49
	bne.n	.L_0814591e
	ldr	r3, [pc, #48]
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #82
	movs	r2, #1
	strh	r3, [r1, #0]
	str	r2, [sp, #20]
.L_0814591e:
	ldr	r3, [sp, #36]
	movs	r4, #225
	lsls	r4, r4, #7
	mov	r5, r8
	adds	r1, r3, r4
	movs	r4, #0
	cmp	r5, #23
	bgt.n	.L_0814595a
	cmp	r5, #15
	ble.n	.L_0814597a
	lsls	r3, r5, #1
	adds	r4, r3, #0
	ldr	r3, [pc, #16]
	ldr	r2, [pc, #16]
	movs	r6, #128
	subs	r3, r3, r5
	lsls	r6, r6, #19
	b.n	.L_08145950
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x0000001f
	.2byte 0x1000
	.2byte 0x0000
.L_08145950:
	orrs	r3, r2
	adds	r6, #82
	subs	r4, #32
	strh	r3, [r6, #0]
	b.n	.L_0814597a
.L_0814595a:
	mov	r2, r8
	cmp	r2, #47
	bgt.n	.L_0814597a
	cmp	r2, #31
	ble.n	.L_0814597a
	lsls	r3, r2, #1
	adds	r4, r3, #0
	ldr	r3, [pc, #36]
	movs	r5, #128
	subs	r3, r3, r2
	ldr	r2, [pc, #36]
	lsls	r5, r5, #19
	orrs	r3, r2
	adds	r5, #82
	strh	r3, [r5, #0]
	subs	r4, #64
.L_0814597a:
	cmp	r4, #0
	bge.n	.L_08145980
	movs	r4, #0
.L_08145980:
	movs	r3, #6
	subs	r3, r3, r7
	mov	r2, r8
	movs	r6, #0
	lsls	r7, r3, #8
	lsls	r5, r2, #11
	b.n	.L_08145998
	movs	r0, r0
	.4byte 0x0000002f
	.2byte 0x1000
	.2byte 0x0000
.L_08145998:
	adds	r0, r5, #0
	str	r1, [sp, #12]
	str	r4, [sp, #8]
	bl	sub_08002096
	ldr	r4, [sp, #8]
	ldr	r1, [sp, #12]
	adds	r3, r4, #0
	muls	r3, r0
	asrs	r3, r3, #10
	subs	r3, r7, r3
	stmia	r1!, {r3}
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r6, #1
	adds	r5, r5, r3
	cmp	r6, #160
	bne.n	.L_08145998
	ldr	r4, [sp, #20]
	cmp	r4, #0
	bne.n	.L_081459c4
	b.n	sub_08145ba2
.L_081459c4:
	mov	r5, fp
	ldr	r0, [r5, #4]
	cmp	r0, #0
	bne.n	.L_081459d4
	movs	r6, #0
	movs	r7, #0
	mov	sl, r6
	b.n	.L_081459da
.L_081459d4:
	movs	r1, #0
	movs	r7, #1
	mov	sl, r1
.L_081459da:
	mov	r2, r8
	cmp	r2, #71
	bgt.n	.L_08145a12
	ldr	r2, [pc, #604]
	lsls	r3, r7, #3
	mov	r4, sl
	subs	r3, r3, r7
	ldr	r1, [pc, #600]
	ldrb	r2, [r2, r3]
	lsls	r3, r4, #3
	subs	r3, r3, r4
	ldrb	r3, [r1, r3]
	movs	r1, #57
	str	r1, [sp, #0]
	movs	r1, #98
	str	r1, [sp, #4]
	ldr	r5, [sp, #16]
	lsls	r0, r0, #2
	ldr	r6, [sp, #36]
	ldr	r4, [r0, r5]
	movs	r5, #224
	lsls	r5, r5, #3
	add	r3, r9
	ldr	r0, [sp, #32]
	adds	r1, r6, r5
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe0c7
.L_08145a12:
	mov	r6, r8
	cmp	r6, #75
	bgt.n	.L_08145a56
	ldr	r2, [pc, #548]
	lsls	r6, r7, #3
	subs	r3, r6, r7
	ldrb	r2, [r2, r3]
	ldr	r1, [pc, #544]
	mov	ip, r2
	mov	r2, sl
	lsls	r5, r2, #3
	subs	r3, r5, r2
	ldrb	r3, [r1, r3]
	movs	r1, #57
	str	r1, [sp, #0]
	add	r3, r9
	movs	r1, #98
	mov	lr, r3
	str	r1, [sp, #4]
	ldr	r3, [sp, #16]
	ldr	r2, [sp, #36]
	lsls	r0, r0, #2
	ldr	r4, [r0, r3]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [sp, #32]
	mov	r2, ip
	mov	r3, lr
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x465c
	ldr	r0, [r4, #4]
	b.n	.L_08145a5c
.L_08145a56:
	mov	r1, sl
	lsls	r6, r7, #3
	lsls	r5, r1, #3
.L_08145a5c:
	ldr	r2, [pc, #480]
	subs	r6, r6, r7
	adds	r3, r6, #1
	ldrb	r3, [r2, r3]
	ldr	r7, [pc, #476]
	mov	ip, r3
	mov	r3, sl
	subs	r5, r5, r3
	adds	r3, r5, #1
	ldrb	r3, [r7, r3]
	movs	r1, #99
	str	r1, [sp, #0]
	add	r3, r9
	movs	r1, #69
	mov	lr, r3
	str	r1, [sp, #4]
	ldr	r2, [sp, #36]
	ldr	r1, [sp, #16]
	movs	r3, #224
	lsls	r3, r3, #5
	lsls	r0, r0, #2
	adds	r3, #210
	ldr	r4, [r0, r1]
	adds	r1, r2, r3
	ldr	r0, [sp, #32]
	mov	r3, lr
	mov	r2, ip
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4643
	subs	r3, #72
	cmp	r3, #1
	bhi.n	sub_08145aac
	movs	r1, #128
	ldr	r3, [pc, #420]
	ldr	r0, [sp, #32]
	lsls	r1, r1, #7
	ldr	r2, [pc, #420]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4643
	subs	r3, #74
	cmp	r3, #1
	bhi.n	sub_08145aea
	ldr	r1, [pc, #392]
	adds	r3, r6, #2
	ldrb	r3, [r1, r3]
	mov	r4, fp
	mov	ip, r3
	movs	r1, #128
	adds	r3, r5, #2
	ldr	r0, [r4, #4]
	ldrb	r3, [r7, r3]
	str	r1, [sp, #0]
	movs	r1, #91
	str	r1, [sp, #4]
	ldr	r2, [sp, #16]
	lsls	r0, r0, #2
	add	r3, r9
	mov	lr, r3
	ldr	r4, [r0, r2]
	ldr	r3, [sp, #36]
	movs	r2, #220
	lsls	r2, r2, #6
	adds	r2, #129
	adds	r1, r3, r2
	ldr	r0, [sp, #32]
	mov	r2, ip
	mov	r3, lr
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4643
	subs	r3, #76
	cmp	r3, #1
	bhi.n	sub_08145b18
	ldr	r4, [pc, #332]
	mov	r3, fp
	ldr	r0, [r3, #4]
	adds	r3, r6, #3
	ldrb	r2, [r4, r3]
	movs	r1, #128
	adds	r3, r5, #3
	ldrb	r3, [r7, r3]
	str	r1, [sp, #0]
	movs	r1, #91
	str	r1, [sp, #4]
	ldr	r1, [sp, #16]
	lsls	r0, r0, #2
	ldr	r4, [r0, r1]
	add	r3, r9
	ldr	r0, [sp, #32]
	ldr	r1, [pc, #316]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4643
	subs	r3, #78
	cmp	r3, #1
	bhi.n	sub_08145b46
	ldr	r4, [pc, #284]
	mov	r2, fp
	adds	r3, r6, #4
	ldr	r0, [r2, #4]
	movs	r1, #128
	ldrb	r2, [r4, r3]
	adds	r3, r5, #4
	ldrb	r3, [r7, r3]
	str	r1, [sp, #0]
	movs	r1, #59
	str	r1, [sp, #4]
	ldr	r1, [sp, #16]
	lsls	r0, r0, #2
	ldr	r4, [r0, r1]
	add	r3, r9
	ldr	r0, [sp, #32]
	ldr	r1, [pc, #272]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4643
	subs	r3, #80
	cmp	r3, #1
	bhi.n	sub_08145b74
	ldr	r4, [pc, #240]
	mov	r2, fp
	adds	r3, r6, #5
	ldr	r0, [r2, #4]
	movs	r1, #122
	ldrb	r2, [r4, r3]
	adds	r3, r5, #5
	ldrb	r3, [r7, r3]
	str	r1, [sp, #0]
	movs	r1, #29
	str	r1, [sp, #4]
	ldr	r1, [sp, #16]
	lsls	r0, r0, #2
	ldr	r4, [r0, r1]
	add	r3, r9
	ldr	r0, [sp, #32]
	ldr	r1, [pc, #232]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4643
	subs	r3, #82
	cmp	r3, #1
	bhi.n	sub_08145ba2
	ldr	r4, [pc, #192]
	mov	r2, fp
	adds	r3, r6, #6
	ldr	r0, [r2, #4]
	movs	r1, #76
	ldrb	r2, [r4, r3]
	adds	r3, r5, #6
	ldrb	r3, [r7, r3]
	str	r1, [sp, #0]
	movs	r1, #25
	str	r1, [sp, #4]
	ldr	r5, [sp, #16]
	lsls	r0, r0, #2
	ldr	r4, [r0, r5]
	add	r3, r9
	ldr	r0, [sp, #32]
	ldr	r1, [pc, #188]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4646
	cmp	r6, #68
	bne.n	.L_08145bae
	movs	r0, #212
	bl	sub_081c0010
.L_08145bae:
	mov	r1, r8
	cmp	r1, #72
	bne.n	.L_08145bd4
	mov	r3, fp
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r1, #0
	bl	sub_08118088
	movs	r5, #238
	ldr	r4, [sp, #36]
	lsls	r5, r5, #7
	adds	r5, #168
	adds	r2, r4, r5
	movs	r3, #8
	str	r3, [r2, #0]
	movs	r0, #144
	bl	sub_081180e8
.L_08145bd4:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r1, #240
	ldr	r6, [sp, #36]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r6, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #104
	beq.n	.L_08145c00
	b.n	.L_08145862
.L_08145c00:
	ldr	r0, [pc, #92]
	bl	sub_08014644
	ldr	r0, [pc, #92]
	bl	sub_08014644
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r4, #206
	lsls	r4, r4, #3
	adds	r3, r3, r4
	ldrh	r1, [r3, #0]
	movs	r2, #24
	movs	r0, #1
	bl	sub_08118040
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #72
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x081978f0
	.4byte 0x081978fe
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x02010000
	.4byte 0x02012d80
	.4byte 0x02014b00
	.4byte 0x020158d2
	.4byte 0x08143001
	.4byte 0x08152475
