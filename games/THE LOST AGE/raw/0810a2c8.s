.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08038120, 0x08038120
	.set sub_08038140, 0x08038140
	.set sub_080ad010, 0x080ad010
	.set sub_080ad020, 0x080ad020
	.set sub_080ad050, 0x080ad050
	.set sub_080ad060, 0x080ad060
	.set sub_080ad1d0, 0x080ad1d0
	.set sub_080ad1d8, 0x080ad1d8
	.set sub_080ad1e8, 0x080ad1e8
	.set sub_080ad1f0, 0x080ad1f0
	.set sub_080ad258, 0x080ad258
	.set sub_080ad260, 0x080ad260
	.set sub_080f8058, 0x080f8058
	.set sub_081084f4, 0x081084f4
	.set sub_0810857c, 0x0810857c
	.set sub_08108630, 0x08108630
	.set sub_08108690, 0x08108690
	.set sub_08108af0, 0x08108af0
	.set sub_08109188, 0x08109188
	.set sub_0810928c, 0x0810928c
	.set sub_081095b0, 0x081095b0
	.set sub_0810993c, 0x0810993c
	.set sub_08109a3c, 0x08109a3c
	.set sub_08109ad8, 0x08109ad8
	.set sub_08109cac, 0x08109cac
	.set sub_0810a004, 0x0810a004
	.set sub_0810a2ac, 0x0810a2ac
	.set sub_081c0010, 0x081c0010
	.global Overlay_0810a2c8
Overlay_0810a2c8:
	push	{lr}
	bl	sub_08109ad8
	pop	{pc}
	push	{lr}
	bl	sub_08109cac
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	str	r1, [sp, #12]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	adds	r7, r0, #0
	str	r3, [sp, #8]
	bl	sub_08016ca4
	ldr	r2, [sp, #12]
	mov	r8, r0
	lsls	r2, r2, #1
	str	r2, [sp, #4]
	adds	r6, r2, #0
	adds	r6, #216
	ldrh	r3, [r0, r6]
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r2, #255
	ands	r2, r3
	mov	sl, r2
	mov	r0, sl
	bl	sub_080ad010
	adds	r5, r0, #0
	ldrb	r1, [r5, #2]
	adds	r0, r7, #0
	bl	sub_080ad1d0
	str	r0, [sp, #0]
	mov	r3, r8
	ldrh	r0, [r3, r6]
	bl	sub_0810a2ac
	ldrb	r1, [r5, #12]
	mov	r9, r0
	cmp	r1, #2
	beq.n	.L_0810a344
	mov	r0, sl
	movs	r1, #2
	bl	sub_08038120
	ldr	r0, [pc, #312]
	bl	sub_0810857c
	b.n	.L_0810a468
.L_0810a344:
	mov	r3, r8
	ldrh	r2, [r3, r6]
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0810a362
	mov	r0, sl
	movs	r1, #2
	bl	sub_08038120
	ldr	r0, [pc, #288]
	bl	sub_0810857c
	b.n	.L_0810a468
.L_0810a362:
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0810a384
	ldrb	r3, [r5, #3]
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0810a384
	mov	r0, sl
	movs	r1, #2
	bl	sub_08038120
	ldr	r0, [pc, #256]
	bl	sub_0810857c
	b.n	.L_0810a468
.L_0810a384:
	ldr	r3, [pc, #252]
	ldr	r3, [r3, #16]
	cmp	r9, r3
	bls.n	.L_0810a394
	ldr	r0, [pc, #248]
	bl	sub_0810857c
	b.n	.L_0810a468
.L_0810a394:
	mov	r0, sl
	movs	r1, #2
	bl	sub_08038120
	mov	r0, r9
	movs	r1, #5
	bl	sub_08038120
	ldr	r2, [pc, #228]
	mov	fp, r2
	mov	r0, fp
	bl	sub_0810857c
	movs	r0, #0
	bl	sub_08108630
	cmp	r0, #0
	beq.n	.L_0810a3c2
	mov	r0, fp
	adds	r0, #1
	bl	sub_0810857c
	b.n	.L_0810a468
.L_0810a3c2:
	ldr	r5, [sp, #4]
	mov	r2, r8
	adds	r5, #216
	mov	r3, r8
	ldrh	r6, [r3, r5]
	strh	r0, [r2, r5]
	ldr	r3, [sp, #8]
	adds	r1, r7, #0
	ldr	r0, [r3, #36]
	bl	sub_0810a004
	movs	r1, #2
	mov	r0, sl
	bl	sub_08038120
	mov	r0, fp
	adds	r0, #2
	bl	sub_0810857c
	bl	sub_08038140
	movs	r0, #10
	bl	sub_08013560
	movs	r0, #100
	bl	sub_081c0010
	movs	r0, #110
	bl	sub_08013560
	movs	r0, #100
	bl	sub_081c0010
	movs	r0, #110
	bl	sub_08013560
	movs	r0, #100
	bl	sub_081c0010
	movs	r0, #110
	bl	sub_08013560
	movs	r0, #112
	bl	sub_081c0010
	movs	r0, #20
	bl	sub_08013560
	mov	r2, r8
	strh	r6, [r2, r5]
	ldr	r1, [sp, #12]
	adds	r0, r7, #0
	bl	sub_080ad060
	mov	r3, r9
	negs	r0, r3
	bl	sub_080ad1d8
	bl	sub_08109188
	ldr	r2, [sp, #8]
	adds	r1, r7, #0
	ldr	r0, [r2, #36]
	bl	sub_0810a004
	movs	r1, #2
	mov	r0, sl
	bl	sub_08038120
	mov	r0, fp
	adds	r0, #3
	bl	sub_0810857c
	adds	r0, r7, #0
	ldr	r1, [sp, #12]
	bl	sub_0810993c
	cmp	r0, #0
	beq.n	.L_0810a468
	adds	r0, r7, #0
	ldr	r1, [sp, #0]
	bl	sub_08109a3c
.L_0810a468:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0000126b
	.4byte 0x0000126c
	.4byte 0x0000126d
	.4byte 0x02000240
	.4byte 0x0000126e
	.2byte 0x126f
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r6, [r3, #0]
	sub	sp, #8
	movs	r3, #0
	mov	sl, r0
	movs	r0, #128
	str	r3, [sp, #4]
	lsls	r0, r0, #3
	adds	r0, #252
	adds	r7, r6, r0
	mov	r9, r1
	ldrh	r1, [r7, #0]
	movs	r4, #158
	str	r1, [sp, #0]
	ldr	r1, [pc, #396]
	lsls	r4, r4, #1
	mov	r8, r3
	adds	r3, r1, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r2, [pc, #384]
	lsls	r3, r3, #1
	ldrsh	r0, [r2, r3]
	mov	fp, r0
	movs	r0, #156
	lsls	r0, r0, #1
	adds	r3, r1, r0
	ldr	r3, [r3, #0]
	cmp	fp, r3
	ble.n	.L_0810a4e2
	b.n	.L_0810a63e
.L_0810a4e2:
	movs	r3, #228
	strh	r3, [r7, #0]
	movs	r1, #2
	movs	r0, #228
	bl	sub_08038120
	ldr	r5, [pc, #356]
	adds	r0, r5, #0
	bl	sub_0810857c
	adds	r5, #1
	ldrh	r0, [r7, #0]
	movs	r1, #2
	bl	sub_08038120
	adds	r0, r5, #0
	bl	sub_0810857c
	movs	r7, #2
.L_0810a508:
	cmp	r7, #0
	beq.n	.L_0810a57e
	ldr	r4, [sp, #4]
	movs	r1, #153
	lsls	r1, r1, #3
	lsls	r2, r4, #1
	adds	r3, r6, #2
	adds	r2, r2, r1
	ldrsh	r0, [r3, r2]
	adds	r3, r4, #0
	mov	r8, r0
	cmp	r4, #0
	bge.n	.L_0810a524
	adds	r3, r4, #3
.L_0810a524:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r4, r3
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #3
	movs	r2, #0
	subs	r1, #12
	mov	r0, sl
	bl	sub_08108af0
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #5
	adds	r2, r6, r3
	movs	r3, #3
	strb	r3, [r2, #0]
	cmp	r7, #2
	bne.n	.L_0810a560
	ldr	r0, [sp, #4]
	cmp	r0, #0
	bge.n	.L_0810a552
	adds	r0, #3
.L_0810a552:
	asrs	r0, r0, #2
	lsls	r0, r0, #2
	bl	sub_080f8058
	movs	r0, #1
	bl	sub_08013560
.L_0810a560:
	movs	r4, #128
	lsls	r4, r4, #3
	adds	r4, #252
	adds	r5, r6, r4
	ldr	r1, [sp, #4]
	ldrh	r2, [r5, #0]
	mov	r0, sl
	bl	sub_0810928c
	ldrh	r2, [r5, #0]
	mov	r0, r9
	mov	r1, r8
	bl	sub_081095b0
	movs	r7, #0
.L_0810a57e:
	ldr	r1, [pc, #216]
	movs	r3, #1
	ldr	r2, [r1, #4]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0810a5fa
	movs	r0, #128
	lsls	r0, r0, #3
	adds	r0, #252
	adds	r5, r6, r0
	ldrh	r1, [r5, #0]
	mov	r0, r8
	bl	sub_080ad020
	adds	r1, r0, #0
	cmp	r1, #0
	bge.n	.L_0810a5d0
	movs	r0, #113
	bl	sub_081c0010
	mov	r0, r8
	movs	r1, #1
	bl	sub_08038120
	ldrh	r0, [r5, #0]
	movs	r1, #2
	bl	sub_08038120
	mov	r0, r8
	bl	sub_080ad1f0
	cmp	r0, #15
	bne.n	.L_0810a5c8
	ldr	r0, [pc, #152]
	bl	sub_081084f4
	b.n	.L_0810a508
.L_0810a5c8:
	ldr	r0, [pc, #148]
	bl	sub_081084f4
	b.n	.L_0810a508
.L_0810a5d0:
	mov	r0, r8
	bl	sub_080ad050
	movs	r0, #101
	bl	sub_081c0010
	ldr	r0, [pc, #132]
	bl	sub_0810857c
	ldrh	r1, [r5, #0]
	mov	r0, r8
	bl	sub_080ad020
	mov	r1, fp
	negs	r0, r1
	bl	sub_080ad258
	movs	r0, #1
	bl	sub_080ad260
	b.n	.L_0810a630
.L_0810a5fa:
	ldr	r3, [r1, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0810a612
	ldr	r0, [pc, #96]
	bl	sub_0810857c
	movs	r0, #113
	bl	sub_081c0010
	b.n	.L_0810a630
.L_0810a612:
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #4
	adds	r3, r6, r2
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	add	r0, sp, #4
	movs	r2, #4
	bl	sub_08108690
	adds	r7, r0, #0
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_0810a508
.L_0810a630:
	movs	r4, #128
	mov	r0, sp
	lsls	r4, r4, #3
	ldrh	r0, [r0, #0]
	adds	r4, #252
	adds	r3, r6, r4
	strh	r0, [r3, #0]
.L_0810a63e:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0810c38e
	.4byte 0x00001274
	.4byte 0x03001150
	.4byte 0x0000124f
	.4byte 0x00001257
	.4byte 0x00001252
	.2byte 0x1276
	.2byte 0x0000
	movs	r0, #32
	bx	lr
	push	{r5, r6, lr}
	movs	r3, #128
	adds	r5, r0, #0
	lsls	r3, r3, #3
	adds	r6, r5, r3
	adds	r0, r6, #0
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_0810a6b2
	adds	r0, r6, #0
	bl	sub_08016cfc
	lsls	r3, r5, #5
	ldr	r2, [pc, #36]
	adds	r3, r3, r5
	lsls	r3, r3, #1
	adds	r3, r3, r2
	adds	r5, r3, #0
	movs	r6, #0
	adds	r5, #48
.L_0810a69a:
	ldrh	r3, [r5, #0]
	adds	r5, #2
	lsls	r3, r3, #16
	asrs	r0, r3, #16
	cmp	r0, #0
	beq.n	.L_0810a6ac
	movs	r1, #1
	bl	sub_080ad1e8
.L_0810a6ac:
	adds	r6, #1
	cmp	r6, #7
	ble.n	.L_0810a69a
.L_0810a6b2:
	pop	{r5, r6, pc}
	.2byte 0xc3f4
	.2byte 0x0810
	push	{r5, lr}
	lsls	r3, r0, #5
	adds	r5, r1, #0
	ldr	r1, [pc, #52]
	adds	r3, r3, r0
	lsls	r2, r3, #1
	ldrsh	r3, [r1, r2]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_0810a6e6
	adds	r0, r5, #0
	adds	r2, r2, r1
.L_0810a6d0:
	ldrh	r3, [r2, #0]
	adds	r4, #1
	strh	r3, [r0, #0]
	adds	r2, #2
	adds	r0, #2
	cmp	r4, #23
	bgt.n	.L_0810a6e6
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	bne.n	.L_0810a6d0
.L_0810a6e6:
	ldr	r3, [pc, #8]
	lsls	r2, r4, #1
	strh	r3, [r2, r5]
	adds	r0, r4, #0
	pop	{r5, pc}
	.4byte 0x00000000
	.2byte 0xc3f4
	.2byte 0x0810
	lsls	r3, r0, #5
	ldr	r2, [pc, #12]
	adds	r3, r3, r0
	lsls	r3, r3, #1
	adds	r3, #64
	ldrsh	r0, [r2, r3]
	bx	lr
	movs	r0, r0
	.2byte 0xc3f4
	.2byte 0x0810
.L_0810a70c:
	push	{lr}
	adds	r1, r0, #0
	movs	r3, #128
	ldr	r0, [pc, #48]
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r1, r3
	movs	r4, #0
	ldrsh	r3, [r0, r4]
	movs	r4, #1
	negs	r4, r4
	ldrh	r2, [r0, #0]
	cmp	r3, r4
	beq.n	.L_0810a73e
	mov	ip, r4
.L_0810a72a:
	lsls	r3, r2, #16
	asrs	r3, r3, #16
	cmp	r3, r1
	beq.n	.L_0810a740
	adds	r0, #36
	movs	r4, #0
	ldrsh	r3, [r0, r4]
	ldrh	r2, [r0, #0]
	cmp	r3, ip
	bne.n	.L_0810a72a
.L_0810a73e:
	movs	r0, #0
.L_0810a740:
	pop	{pc}
	movs	r0, r0
	.2byte 0xcc34
	.2byte 0x0810
	push	{lr}
	bl	.L_0810a70c
	cmp	r0, #0
	beq.n	.L_0810a75a
	movs	r3, #2
	ldrsh	r0, [r0, r3]
	adds	r0, #1
	b.n	.L_0810a75c
.L_0810a75a:
	movs	r0, #0
.L_0810a75c:
	pop	{pc}
