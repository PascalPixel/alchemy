.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0800206c, 0x0800206c
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014dac, 0x08014dac
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_0801521c, 0x0801521c
	.set sub_081180e8, 0x081180e8
	.set sub_08138086, 0x08138086
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1fc, 0x0815e1fc
	.set sub_0815e20c, 0x0815e20c
	.set sub_0815f000, 0x0815f000
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_0818a6d4, 0x0818a6d4
	.set sub_0818ad40, 0x0818ad40
	.set sub_081963ec, 0x081963ec
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Func_0818a220
	.thumb_func
Func_0818a220:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #216
	str	r0, [sp, #84]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #92]
	mov	r9, r1
	str	r0, [sp, #80]
	movs	r0, #1
	ldr	r1, [r5, #96]
	str	r1, [sp, #76]
	ldr	r2, [r5, #100]
	str	r2, [sp, #60]
	bl	sub_081435e0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	ldr	r5, [r5, #104]
	movs	r3, #0
	ldr	r0, [pc, #36]
	ldr	r1, [sp, #60]
	movs	r2, #0
	str	r5, [sp, #64]
	str	r3, [sp, #56]
	bl	sub_08157cf4
	mov	r4, r9
	cmp	r4, #0
	bne.n	.L_0818a296
	ldr	r2, [sp, #80]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #12]
	movs	r2, #1
	movs	r3, #1
	b.n	.L_0818a290
	.4byte 0x00001010
	.4byte 0x00000134
	.2byte 0x00df
	.2byte 0x0000
.L_0818a290:
	bl	sub_08157cf4
	b.n	.L_0818a32c
.L_0818a296:
	mov	r4, r9
	cmp	r4, #1
	bne.n	.L_0818a2ba
	ldr	r2, [sp, #80]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #76]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r4, [sp, #80]
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r1, r4, r2
	ldr	r0, [pc, #64]
	b.n	.L_0818a2ea
.L_0818a2ba:
	mov	r3, r9
	cmp	r3, #2
	bne.n	.L_0818a308
	ldr	r4, [sp, #80]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r4, r2
	ldr	r0, [pc, #48]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r3, [sp, #80]
	movs	r4, #240
	lsls	r4, r4, #4
	adds	r5, r3, r4
	ldr	r0, [pc, #36]
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #28]
	adds	r1, r5, #0
.L_0818a2ea:
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	b.n	.L_0818a32c
	.4byte 0x000000e0
	.4byte 0x00000193
	.4byte 0x000000de
	.4byte 0x0000013e
	.2byte 0x0161
	.2byte 0x0000
.L_0818a308:
	ldr	r2, [sp, #80]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #88]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r4, [sp, #80]
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r1, r4, r2
	ldr	r0, [pc, #76]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
.L_0818a32c:
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [sp, #80]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r0, [sp, #80]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #132
	adds	r2, r0, r1
	movs	r3, #0
	movs	r1, #200
	str	r3, [r2, #0]
	ldr	r0, [pc, #32]
	lsls	r1, r1, #4
	bl	sub_080145a8
	movs	r2, #0
	str	r2, [sp, #72]
	ldr	r3, [pc, #24]
	mov	r4, r9
	ldrb	r3, [r3, r4]
	b.n	.L_0818a37c
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x000000e1
	.4byte 0x000000f8
	.4byte 0x08143001
	.2byte 0x9d60
	.2byte 0x0819
.L_0818a37c:
	cmp	r3, #0
	bne.n	.L_0818a384
	bl	.L_0818acda
.L_0818a384:
	mov	r0, sp
	mov	r1, sp
	adds	r0, #88
	adds	r1, #204
	subs	r2, #204
	str	r0, [sp, #32]
	str	r1, [sp, #52]
	str	r2, [sp, #24]
.L_0818a394:
	ldr	r3, [sp, #72]
	cmp	r3, #0
	bne.n	.L_0818a468
	movs	r0, #190
	bl	sub_081c0010
	ldr	r3, [sp, #80]
	movs	r4, #0
	movs	r2, #1
	mov	r8, r4
	negs	r2, r2
	adds	r3, #24
.L_0818a3ac:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r1, #56
	bne.n	.L_0818a3ac
	ldr	r4, [sp, #84]
	movs	r2, #0
	ldr	r3, [r4, #20]
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_0818a3e8
	ldr	r0, [sp, #80]
	movs	r1, #199
	lsls	r1, r1, #3
	movs	r6, #3
	adds	r5, r0, r1
.L_0818a3d0:
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #2
	str	r0, [r5, #0]
	ldr	r4, [sp, #84]
	movs	r2, #1
	ldr	r3, [r4, #20]
	add	r8, r2
	adds	r5, #28
	cmp	r8, r3
	bne.n	.L_0818a3d0
.L_0818a3e8:
	ldr	r3, [pc, #120]
	movs	r0, #0
	movs	r1, #1
	movs	r2, #128
	mov	r8, r0
	negs	r1, r1
	lsls	r2, r2, #2
.L_0818a3f6:
	movs	r4, #1
	add	r8, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_0818a3f6
	ldr	r1, [sp, #84]
	ldr	r0, [r1, #8]
	add	r1, sp, #204
	bl	sub_0815e1fc
	ldr	r4, [sp, #84]
	movs	r2, #0
	ldr	r3, [r4, #20]
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_0818a436
	add	r5, sp, #108
	movs	r6, #36
.L_0818a41c:
	ldr	r1, [sp, #84]
	ldrsh	r0, [r6, r1]
	adds	r1, r5, #0
	bl	sub_0815e20c
	ldr	r4, [sp, #84]
	movs	r3, #1
	add	r8, r3
	ldr	r3, [r4, #20]
	adds	r5, #12
	adds	r6, #2
	cmp	r8, r3
	bne.n	.L_0818a41c
.L_0818a436:
	ldr	r3, [pc, #40]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r0, [sp, #80]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r0, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	movs	r4, #128
	adds	r2, r0, r3
	lsls	r4, r4, #7
	movs	r3, #0
	str	r3, [r2, #0]
	str	r4, [sp, #56]
	b.n	.L_0818a468
	.4byte 0x00000000
	.2byte 0x4018
	.2byte 0x0201
.L_0818a468:
	ldr	r0, [sp, #72]
	cmp	r0, #47
	bgt.n	.L_0818a48a
	ldr	r1, [sp, #52]
	movs	r4, #128
	ldr	r3, [r1, #4]
	lsls	r4, r4, #8
	subs	r3, #1
	str	r3, [r1, #4]
	ldr	r2, [sp, #56]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r2, r2, r3
	str	r2, [sp, #56]
	cmp	r2, r4
	ble.n	.L_0818a48a
	str	r4, [sp, #56]
.L_0818a48a:
	ldr	r0, [sp, #72]
	cmp	r0, #95
	ble.n	.L_0818a4a0
	ldr	r1, [sp, #56]
	ldr	r2, [pc, #136]
	ldr	r4, [sp, #52]
	adds	r1, r1, r2
	str	r1, [sp, #56]
	ldr	r3, [r4, #4]
	subs	r3, #2
	str	r3, [r4, #4]
.L_0818a4a0:
	ldr	r0, [sp, #72]
	cmp	r0, #48
	bne.n	.L_0818a4fe
	mov	r1, r9
	cmp	r1, #0
	bne.n	.L_0818a4b2
	movs	r0, #148
	bl	sub_081c0010
.L_0818a4b2:
	mov	r2, r9
	cmp	r2, #1
	bne.n	.L_0818a4be
	movs	r0, #144
	bl	sub_081c0010
.L_0818a4be:
	mov	r3, r9
	cmp	r3, #2
	bne.n	.L_0818a4ca
	movs	r0, #104
	bl	sub_081c0010
.L_0818a4ca:
	mov	r4, r9
	cmp	r4, #3
	bne.n	.L_0818a4d6
	movs	r0, #221
	bl	sub_081c0010
.L_0818a4d6:
	mov	r0, r9
	cmp	r0, #0
	beq.n	.L_0818a4fe
	ldr	r1, [sp, #80]
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
	movs	r2, #128
	ldr	r3, [pc, #32]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
.L_0818a4fe:
	ldr	r0, [sp, #72]
	cmp	r0, #47
	bgt.n	.L_0818a506
	b.n	.L_0818a70e
.L_0818a506:
	mov	r1, r9
	cmp	r1, #0
	beq.n	.L_0818a538
	cmp	r1, #1
	bne.n	.L_0818a524
	ldr	r0, [pc, #12]
	bl	sub_0815f0a0
	b.n	.L_0818a538
	.4byte 0x00003f44
	.4byte 0xfffffe00
	.2byte 0x0193
	.2byte 0x0000
.L_0818a524:
	mov	r2, r9
	cmp	r2, #3
	bne.n	.L_0818a532
	ldr	r0, [pc, #620]
	bl	sub_0815f0a0
	b.n	.L_0818a538
.L_0818a532:
	ldr	r0, [pc, #616]
	bl	sub_0815f0a0
.L_0818a538:
	mov	r3, r9
	subs	r3, #1
	cmp	r3, #1
	bhi.n	.L_0818a5da
	ldr	r3, [sp, #84]
	ldr	r0, [sp, #72]
	ldr	r1, [r3, #20]
	bl	sub_08002064
	adds	r5, r0, #0
	bl	sub_08014878
	lsls	r3, r5, #1
	adds	r3, r3, r5
	add	r6, sp, #108
	lsls	r5, r3, #2
	ldr	r3, [r6, r5]
	movs	r2, #63
	ands	r0, r2
	adds	r3, r3, r0
	add	r7, sp, #96
	subs	r3, #32
	mov	r4, r9
	str	r3, [r7, #0]
	cmp	r4, #1
	bne.n	.L_0818a57e
	bl	sub_08014878
	adds	r3, r5, #4
	ldr	r3, [r6, r3]
	movs	r2, #255
	ands	r2, r0
	adds	r3, r3, r2
	subs	r3, #96
	b.n	.L_0818a596
.L_0818a57e:
	mov	r0, r9
	cmp	r0, #2
	bne.n	.L_0818a598
	str	r2, [sp, #16]
	bl	sub_08014878
	adds	r3, r5, #4
	ldr	r2, [sp, #16]
	ldr	r3, [r6, r3]
	ands	r0, r2
	adds	r3, r3, r0
	adds	r3, #32
.L_0818a596:
	str	r3, [r7, #4]
.L_0818a598:
	ldr	r1, [sp, #72]
	cmp	r1, #95
	bgt.n	.L_0818a5da
	adds	r0, r1, #0
	movs	r1, #56
	bl	sub_08002064
	ldr	r4, [sp, #52]
	ldr	r2, [sp, #80]
	ldr	r3, [r4, #0]
	lsls	r1, r0, #3
	subs	r1, r1, r0
	lsls	r1, r1, #2
	adds	r1, r2, r1
	lsls	r3, r3, #15
	str	r3, [r1, #0]
	ldr	r3, [r4, #4]
	lsls	r3, r3, #16
	str	r3, [r1, #4]
	ldr	r3, [r7, #0]
	ldr	r2, [r4, #0]
	subs	r3, r3, r2
	lsls	r3, r3, #11
	str	r3, [r1, #12]
	ldr	r3, [r7, #4]
	ldr	r2, [r4, #4]
	subs	r3, r3, r2
	lsls	r3, r3, #11
	str	r3, [r1, #16]
	movs	r3, #32
	str	r3, [r1, #8]
	movs	r3, #0
	str	r3, [r1, #24]
.L_0818a5da:
	mov	r0, r9
	cmp	r0, #1
	bne.n	.L_0818a68a
	ldr	r7, [sp, #80]
	movs	r1, #0
	mov	r8, r1
.L_0818a5e6:
	movs	r2, #192
	ldr	r6, [r7, #24]
	lsls	r2, r2, #2
	adds	r2, #255
	cmp	r6, r2
	bhi.n	.L_0818a67c
	movs	r1, #5
	mov	r0, r8
	bl	sub_08002064
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r1, #96
	adds	r0, r6, #0
	bl	sub_08002054
	movs	r1, #3
	bl	sub_08002064
	ldr	r2, [pc, #400]
	adds	r5, r5, r0
	lsls	r3, r5, #2
	ldr	r1, [r2, r3]
	ldr	r3, [sp, #80]
	movs	r0, #2
	ldrsh	r2, [r7, r0]
	adds	r1, r3, r1
	ldr	r3, [pc, #388]
	ldr	r0, [pc, #392]
	ldrb	r6, [r3, r5]
	movs	r4, #184
	lsls	r4, r4, #5
	lsrs	r3, r6, #1
	adds	r1, r1, r4
	subs	r2, r2, r3
	movs	r4, #6
	ldrsh	r3, [r7, r4]
	ldrb	r4, [r0, r5]
	str	r6, [sp, #0]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #76]
	ldr	r4, [sp, #64]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c38
	movs	r1, #64
	movs	r2, #0
	bl	sub_08138086
	ldr	r3, [r7, #4]
	movs	r0, #192
	lsls	r0, r0, #15
	cmp	r3, r0
	ble.n	.L_0818a662
	ldr	r3, [r7, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r7, #16]
.L_0818a662:
	ldr	r3, [r7, #24]
	ldr	r2, [r7, #8]
	adds	r3, r3, r2
	str	r3, [r7, #24]
	cmp	r2, #1
	ble.n	.L_0818a67c
	ldr	r1, [sp, #72]
	movs	r3, #1
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0818a67c
	subs	r3, r2, #1
	str	r3, [r7, #8]
.L_0818a67c:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r7, #28
	cmp	r3, #56
	bne.n	.L_0818a5e6
	b.n	.L_0818a70e
.L_0818a68a:
	mov	r4, r9
	cmp	r4, #2
	bne.n	.L_0818a70e
	ldr	r5, [sp, #80]
	movs	r0, #0
	mov	r8, r0
.L_0818a696:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_0818a702
	cmp	r3, #1
	ble.n	sub_0818a6d4
	cmp	r3, #0
	bge.n	.L_0818a6a6
	adds	r3, #3
.L_0818a6a6:
	asrs	r3, r3, #2
	lsls	r1, r3, #3
	ldr	r2, [sp, #80]
	adds	r1, r1, r3
	lsls	r1, r1, #7
	movs	r3, #240
	adds	r1, r2, r1
	lsls	r3, r3, #4
	movs	r4, #2
	ldrsh	r2, [r5, r4]
	adds	r1, r1, r3
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	movs	r0, #24
	str	r0, [sp, #0]
	movs	r0, #48
	str	r0, [sp, #4]
	subs	r2, #12
	subs	r3, #24
	ldr	r0, [sp, #76]
	ldr	r4, [sp, #64]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x682b
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #16]
	adds	r2, r2, r3
	movs	r3, #192
	lsls	r3, r3, #15
	str	r2, [r5, #4]
	cmp	r2, r3
	ble.n	.L_0818a6f2
	str	r3, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #16]
.L_0818a6f2:
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #24
	bne.n	.L_0818a702
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L_0818a702:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #56
	bne.n	.L_0818a696
.L_0818a70e:
	mov	r2, r9
	movs	r3, #52
	cmp	r2, #3
	beq.n	.L_0818a720
	mov	r4, r9
	movs	r3, #60
	cmp	r4, #0
	beq.n	.L_0818a720
	movs	r3, #68
.L_0818a720:
	mov	r0, r9
	movs	r2, #96
	cmp	r0, #0
	beq.n	.L_0818a72a
	movs	r2, #110
.L_0818a72a:
	ldr	r1, [sp, #72]
	cmp	r1, r3
	bge.n	.L_0818a732
	b.n	.L_0818a8b8
.L_0818a732:
	cmp	r1, r2
	blt.n	.L_0818a738
	b.n	.L_0818a8b8
.L_0818a738:
	ldr	r2, [sp, #24]
	ldr	r4, [sp, #84]
	str	r2, [sp, #48]
	movs	r3, #0
	mov	r8, r3
	ldr	r3, [r4, #20]
	cmp	r3, #0
	bne.n	.L_0818a74a
	b.n	.L_0818a8b8
.L_0818a74a:
	ldr	r1, [sp, #80]
	movs	r0, #36
	movs	r2, #196
	movs	r3, #0
	lsls	r2, r2, #3
	str	r0, [sp, #28]
	str	r3, [sp, #20]
	adds	r1, r1, r2
	mov	sl, r1
.L_0818a75c:
	mov	r4, sl
	ldr	r3, [r4, #24]
	subs	r3, #1
	str	r3, [r4, #24]
	cmp	r3, #0
	beq.n	.L_0818a76a
	b.n	.L_0818a896
.L_0818a76a:
	movs	r3, #5
	str	r3, [r4, #24]
	ldr	r0, [sp, #80]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r2, r0, r1
	movs	r3, #4
	str	r3, [r2, #0]
	mov	r2, r9
	cmp	r2, #1
	bne.n	.L_0818a78a
	movs	r0, #134
	bl	sub_081c0010
	b.n	.L_0818a7b2
.L_0818a78a:
	mov	r3, r9
	cmp	r3, #3
	bne.n	.L_0818a7ac
	movs	r0, #177
	bl	sub_081c0010
	b.n	.L_0818a7b2
	.4byte 0x00000149
	.4byte 0x00000148
	.4byte 0x08197834
	.4byte 0x0819781a
	.2byte 0x7826
	.2byte 0x0819
.L_0818a7ac:
	movs	r0, #133
	bl	sub_081c0010
.L_0818a7b2:
	mov	r4, r8
	cmp	r4, #0
	bne.n	.L_0818a7be
	movs	r0, #134
	bl	sub_081180e8
.L_0818a7be:
	ldr	r1, [sp, #28]
	ldr	r3, [sp, #84]
	ldrsh	r0, [r1, r3]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [sp, #0]
	ldr	r3, [sp, #48]
	movs	r2, #128
	adds	r3, #100
	str	r3, [sp, #4]
	movs	r3, #128
	movs	r1, #1
	lsls	r2, r2, #10
	lsls	r3, r3, #12
	bl	sub_0815f000
	ldr	r4, [sp, #28]
	ldr	r2, [sp, #84]
	movs	r3, #8
	ldrsh	r0, [r4, r2]
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	mov	r3, r8
	bl	sub_0814cd48
	movs	r3, #0
	str	r3, [sp, #44]
	ldr	r4, [sp, #28]
	ldr	r2, [sp, #84]
	add	r5, sp, #96
	ldrsh	r0, [r4, r2]
	adds	r1, r5, #0
	bl	sub_0815e20c
	ldr	r0, [pc, #900]
	ldr	r4, [sp, #20]
	movs	r3, #0
	mov	fp, r3
	adds	r7, r4, r0
.L_0818a810:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	bge.n	.L_0818a88a
	ldr	r3, [sp, #96]
	lsls	r3, r3, #15
	str	r3, [r7, #0]
	ldr	r3, [sp, #100]
	adds	r3, #8
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	bl	sub_08014878
	movs	r1, #192
	lsls	r1, r1, #8
	bl	sub_0800206c
	movs	r1, #128
	lsls	r1, r1, #6
	adds	r5, r0, #0
	adds	r5, r5, r1
	bl	sub_08014878
	movs	r3, #127
	adds	r6, r0, #0
	adds	r0, r5, #0
	ands	r6, r3
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	muls	r6, r0
	mov	r2, r9
	asrs	r1, r6, #4
	str	r1, [r7, #16]
	cmp	r2, #3
	bne.n	.L_0818a874
	ldr	r3, [r7, #12]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r7, #12]
	lsrs	r3, r6, #31
	adds	r3, r1, r3
	asrs	r3, r3, #1
	str	r3, [r7, #16]
.L_0818a874:
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #16
	str	r3, [r7, #24]
	ldr	r3, [sp, #44]
	adds	r3, #1
	str	r3, [sp, #44]
	cmp	r3, #16
	beq.n	.L_0818a896
.L_0818a88a:
	movs	r4, #1
	add	fp, r4
	mov	r0, fp
	adds	r7, #28
	cmp	r0, #64
	bne.n	.L_0818a810
.L_0818a896:
	ldr	r1, [sp, #28]
	ldr	r3, [sp, #20]
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r1, #2
	adds	r3, r3, r4
	str	r1, [sp, #28]
	str	r3, [sp, #20]
	ldr	r1, [sp, #84]
	movs	r0, #1
	ldr	r3, [r1, #20]
	movs	r2, #28
	add	r8, r0
	add	sl, r2
	cmp	r8, r3
	beq.n	.L_0818a8b8
	b.n	.L_0818a75c
.L_0818a8b8:
	movs	r0, #32
	bl	sub_08014dac
	mov	sl, r0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #712]
	ldr	r3, [sp, #88]
	adds	r7, r0, #0
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #704]
	ldr	r0, [sp, #32]
	ands	r3, r2
	movs	r2, #192
	lsls	r2, r2, #3
	orrs	r3, r2
	ldr	r2, [sp, #80]
	movs	r4, #224
	lsls	r4, r4, #3
	str	r3, [sp, #88]
	adds	r3, r2, r4
	str	r3, [r0, #4]
	movs	r3, #8
	str	r3, [r7, #0]
	ldr	r3, [pc, #680]
	mov	r1, sl
	str	r0, [r7, #16]
	str	r3, [r7, #8]
	str	r1, [r7, #12]
	ldr	r2, [sp, #72]
	cmp	r2, #0
	blt.n	.L_0818a9ce
	bl	sub_08014de4
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r1, #128
	adds	r0, r2, #0
	lsls	r1, r1, #9
	bl	sub_080151e4
	ldr	r3, [sp, #52]
	ldr	r4, [pc, #648]
	ldr	r0, [r3, #0]
	ldr	r1, [r3, #4]
	ldr	r2, [pc, #644]
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	adds	r1, r1, r2
	adds	r0, r0, r4
	movs	r2, #0
	bl	sub_08015160
	ldr	r3, [sp, #72]
	cmp	r3, #47
	bgt.n	.L_0818a978
	movs	r0, #48
	subs	r0, r0, r3
	lsls	r0, r0, #10
	bl	sub_08015024
	ldr	r4, [sp, #84]
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_0818a95e
	ldr	r1, [sp, #72]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #170
	adds	r0, r1, #0
	muls	r0, r3
	bl	sub_08002096
	negs	r0, r0
	lsls	r0, r0, #7
	movs	r1, #0
	movs	r2, #0
	bl	sub_08015160
	b.n	.L_0818a978
.L_0818a95e:
	ldr	r2, [sp, #72]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #170
	adds	r0, r2, #0
	muls	r0, r3
	bl	sub_08002096
	movs	r1, #0
	lsls	r0, r0, #7
	movs	r2, #0
	bl	sub_08015160
.L_0818a978:
	movs	r0, #184
	lsls	r0, r0, #5
	adds	r0, #112
	bl	sub_08015068
	ldr	r3, [sp, #72]
	lsls	r6, r3, #10
	adds	r0, r6, #0
	bl	sub_080150e4
	ldr	r0, [sp, #56]
	bl	sub_0801521c
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r1, #128
	adds	r0, r2, #0
	lsls	r1, r1, #10
	bl	sub_080151e4
	ldr	r5, [pc, #512]
	mov	r1, sl
	adds	r0, r5, #0
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
	mov	r4, r9
	cmp	r4, #0
	beq.n	.L_0818a9bc
	cmp	r4, #3
	bne.n	.L_0818a9d4
.L_0818a9bc:
	movs	r0, #128
	lsls	r0, r0, #7
	bl	sub_0801521c
	adds	r0, r5, #0
	mov	r1, sl
	movs	r2, #4
	bl	sub_08196958
.L_0818a9ce:
	mov	r0, r9
	cmp	r0, #3
	beq.n	.L_0818a9dc
.L_0818a9d4:
	mov	r1, r9
	cmp	r1, #0
	beq.n	.L_0818a9dc
	b.n	.L_0818abda
.L_0818a9dc:
	ldr	r3, [sp, #72]
	subs	r3, #48
	cmp	r3, #63
	bls.n	.L_0818a9e6
	b.n	.L_0818abda
.L_0818a9e6:
	mov	r3, r9
	movs	r2, #0
	cmp	r3, #3
	bne.n	.L_0818a9fe
	ldr	r4, [sp, #72]
	cmp	r4, #95
	ble.n	.L_0818a9fe
	lsls	r3, r4, #2
	movs	r0, #192
	negs	r3, r3
	lsls	r0, r0, #1
	adds	r2, r3, r0
.L_0818a9fe:
	ldr	r1, [sp, #32]
	movs	r3, #5
	strb	r3, [r1, #0]
	add	r3, sp, #88
	mov	fp, r3
	str	r3, [sp, #32]
	mov	r4, fp
	movs	r3, #7
	strb	r3, [r4, #1]
	ldr	r3, [pc, #404]
	mov	r0, sl
	mov	r1, r9
	str	r2, [r7, #20]
	str	r4, [r7, #16]
	str	r3, [r7, #8]
	str	r0, [r7, #12]
	cmp	r1, #0
	bne.n	.L_0818aa8e
	movs	r3, #9
	str	r3, [r7, #0]
	ldr	r2, [sp, #72]
	cmp	r2, #48
	bne.n	.L_0818aa5c
	bl	sub_08014878
	ldr	r5, [sp, #80]
	movs	r3, #0
	adds	r6, r0, #0
	mov	r8, r3
.L_0818aa38:
	mov	r4, r8
	lsls	r3, r4, #13
	str	r3, [r5, #16]
	movs	r1, #3
	adds	r0, r6, r4
	bl	sub_08002064
	movs	r1, #128
	movs	r2, #1
	lsls	r0, r0, #12
	lsls	r1, r1, #4
	add	r8, r2
	adds	r0, r0, r1
	mov	r3, r8
	str	r0, [r5, #8]
	adds	r5, #28
	cmp	r3, #4
	bne.n	.L_0818aa38
.L_0818aa5c:
	ldr	r0, [sp, #72]
	ldr	r5, [sp, #80]
	movs	r4, #0
	mov	r8, r4
	lsls	r6, r0, #10
.L_0818aa66:
	ldr	r0, [r5, #16]
	adds	r0, r0, r6
	bl	sub_08002096
	cmp	r0, #0
	bge.n	.L_0818aa74
	adds	r0, #15
.L_0818aa74:
	movs	r1, #188
	lsls	r1, r1, #6
	asrs	r3, r0, #4
	adds	r1, #160
	movs	r2, #1
	adds	r3, r3, r1
	add	r8, r2
	str	r3, [r5, #20]
	mov	r3, r8
	adds	r5, #28
	cmp	r3, #4
	bne.n	.L_0818aa66
	b.n	.L_0818aadc
.L_0818aa8e:
	movs	r3, #7
	str	r3, [r7, #0]
	ldr	r4, [sp, #72]
	movs	r3, #1
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L_0818aadc
	bl	sub_08014878
	ldr	r5, [sp, #80]
	adds	r6, r0, #0
	movs	r0, #0
	mov	r8, r0
.L_0818aaa8:
	bl	sub_08014878
	movs	r3, #248
	lsls	r3, r3, #5
	adds	r3, #255
	movs	r1, #253
	ands	r3, r0
	lsls	r1, r1, #5
	adds	r3, r3, r1
	mov	r2, r8
	str	r3, [r5, #20]
	adds	r0, r6, r2
	movs	r1, #3
	bl	sub_08002064
	movs	r3, #128
	lsls	r0, r0, #12
	lsls	r3, r3, #4
	movs	r4, #1
	adds	r0, r0, r3
	add	r8, r4
	str	r0, [r5, #8]
	mov	r0, r8
	adds	r5, #28
	cmp	r0, #4
	bne.n	.L_0818aaa8
.L_0818aadc:
	movs	r1, #0
	mov	r8, r1
	ldr	r1, [sp, #80]
	movs	r0, #3
.L_0818aae4:
	mov	r3, r8
	ands	r3, r0
	lsls	r3, r3, #3
	add	r3, sl
	movs	r4, #0
	ldrsh	r2, [r3, r4]
	str	r2, [r1, #0]
	movs	r2, #2
	ldrsh	r3, [r3, r2]
	str	r3, [r1, #4]
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r1, #28
	cmp	r4, #4
	bne.n	.L_0818aae4
	ldr	r5, [sp, #80]
	movs	r0, #0
	movs	r6, #128
	mov	r8, r0
	lsls	r6, r6, #8
.L_0818ab0e:
	ldr	r1, [sp, #80]
	ldr	r3, [r5, #8]
	movs	r2, #224
	adds	r3, r1, r3
	lsls	r2, r2, #3
	adds	r3, r3, r2
	mov	r4, fp
	str	r3, [r4, #4]
	bl	sub_08014de4
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	subs	r0, #60
	subs	r1, #64
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	movs	r1, #128
	adds	r0, r6, #0
	lsls	r1, r1, #9
	adds	r2, r6, #0
	bl	sub_080151e4
	ldr	r0, [sp, #84]
	ldr	r3, [r0, #4]
	cmp	r3, #1
	bne.n	.L_0818ab50
	movs	r0, #128
	lsls	r0, r0, #8
	bl	sub_08015068
.L_0818ab50:
	ldr	r0, [r5, #20]
	bl	sub_080150e4
	mov	r1, r9
	cmp	r1, #0
	bne.n	.L_0818abb0
	ldr	r2, [sp, #72]
	ldr	r4, [pc, #76]
	lsls	r3, r2, #13
	movs	r0, #128
	adds	r1, r3, r4
	lsls	r0, r0, #10
	cmp	r1, r0
	ble.n	.L_0818ab70
	movs	r1, #128
	lsls	r1, r1, #10
.L_0818ab70:
	ldr	r2, [sp, #72]
	cmp	r2, #95
	ble.n	.L_0818ab7e
	movs	r4, #192
	subs	r3, r1, r3
	lsls	r4, r4, #12
	adds	r1, r3, r4
.L_0818ab7e:
	movs	r0, #128
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r2, r2, #9
	bl	sub_080151e4
	b.n	.L_0818abbe
	.4byte 0x02014000
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x081992b0
	.4byte 0xff800000
	.4byte 0xffc00000
	.4byte 0x081991e0
	.4byte 0x08199268
	.2byte 0x0000
	.2byte 0xfffa
.L_0818abb0:
	.2byte 0x2180
	lsls	r1, r1, #10
	movs	r2, #128
	adds	r0, r1, #0
	lsls	r2, r2, #9
	bl	sub_080151e4
.L_0818abbe:
	mov	r1, sl
	ldr	r0, [pc, #308]
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #4
	bne.n	.L_0818ab0e
.L_0818abda:
	adds	r0, r7, #0
	bl	sub_08013164
	ldr	r6, [pc, #280]
	mov	r0, sl
	bl	sub_08013164
	movs	r2, #0
	mov	r8, r2
.L_0818abec:
	ldr	r0, [r6, #24]
	cmp	r0, #0
	blt.n	.L_0818ac92
	mov	r3, r9
	asrs	r5, r0, #3
	cmp	r3, #0
	beq.n	.L_0818abfe
	cmp	r3, #3
	bne.n	.L_0818ac02
.L_0818abfe:
	adds	r5, #1
	b.n	.L_0818ac04
.L_0818ac02:
	adds	r5, #3
.L_0818ac04:
	mov	r1, r9
	lsls	r3, r1, #1
	movs	r4, #0
	add	r3, r9
	lsls	r2, r3, #2
	mov	fp, r4
	ldr	r4, [pc, #236]
	adds	r1, r2, #0
	adds	r1, #8
	ldr	r3, [r4, r1]
	cmp	r3, #0
	beq.n	.L_0818ac8e
	adds	r3, r4, #0
	mov	sl, r3
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	lsls	r0, r5, #1
	asrs	r3, r3, #1
	str	r0, [sp, #8]
	str	r1, [sp, #40]
	str	r3, [sp, #36]
	adds	r7, r2, #0
.L_0818ac30:
	ldr	r3, [sp, #8]
	ldr	r2, [pc, #208]
	subs	r3, #2
	ldrh	r1, [r2, r3]
	ldr	r4, [sp, #60]
	movs	r0, #2
	ldrsh	r2, [r6, r0]
	ldr	r3, [sp, #36]
	adds	r1, r4, r1
	subs	r2, r2, r3
	ldr	r0, [sp, #8]
	movs	r4, #6
	ldrsh	r3, [r6, r4]
	str	r0, [sp, #4]
	subs	r3, r3, r5
	str	r5, [sp, #0]
	ldr	r0, [sp, #76]
	ldr	r4, [sp, #64]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x6873
	movs	r2, #208
	lsls	r2, r2, #15
	cmp	r3, r2
	ble.n	.L_0818ac70
	ldr	r3, [r6, #16]
	str	r2, [r6, #4]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #16]
.L_0818ac70:
	adds	r3, r7, #4
	mov	r0, sl
	ldr	r1, [r0, r3]
	ldr	r2, [r0, r7]
	adds	r0, r6, #0
	bl	sub_08138086
	ldr	r4, [sp, #40]
	mov	r2, sl
	ldr	r3, [r2, r4]
	movs	r1, #1
	add	fp, r1
	cmp	fp, r3
	bne.n	.L_0818ac30
	ldr	r0, [r6, #24]
.L_0818ac8e:
	subs	r3, r0, #1
	str	r3, [r6, #24]
.L_0818ac92:
	movs	r0, #1
	movs	r1, #128
	add	r8, r0
	lsls	r1, r1, #2
	adds	r6, #28
	cmp	r8, r1
	bne.n	.L_0818abec
	movs	r1, #4
	movs	r0, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r4, #240
	ldr	r3, [sp, #80]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #72]
	adds	r0, #3
	adds	r1, #1
	str	r0, [sp, #24]
	str	r1, [sp, #72]
	ldr	r3, [pc, #56]
	mov	r2, r9
	ldrb	r3, [r3, r2]
	cmp	r1, r3
	beq.n	.L_0818acda
	bl	.L_0818a394
.L_0818acda:
	ldr	r0, [pc, #48]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #216
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x081991c0
	.4byte 0x02014000
	.4byte 0x08199d64
	.4byte 0x08197410
	.4byte 0x08199d60
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #0
	bl	sub_0818ad40
	pop	{pc}
	.align 2, 0
