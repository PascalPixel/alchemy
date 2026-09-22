.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08014878, 0x08014878
	.set sub_0801489c, 0x0801489c
	.set sub_080148e8, 0x080148e8
	.set sub_080149e0, 0x080149e0
	.set sub_080233d0, 0x080233d0
	.set sub_08024738, 0x08024738
	.set sub_08024f20, 0x08024f20
	.set sub_0802d87c, 0x0802d87c
	.global Overlay_08025134
Overlay_08025134:
	push	{r5, lr}
	sub	sp, #12
	adds	r5, r0, #0
	mov	r0, sp
	ldr	r3, [r0, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	adds	r1, r1, r3
	ldr	r3, [r0, #4]
	ldr	r0, [r0, #8]
	adds	r2, r2, r3
	ldr	r3, [r5, #16]
	adds	r3, r3, r0
	adds	r0, r5, #0
	bl	sub_08024738
	ldrh	r3, [r5, #4]
	movs	r0, #1
	adds	r3, #3
	strh	r3, [r5, #4]
	add	sp, #12
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	str	r0, [sp, #4]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r1, #240
	mov	ip, r3
	adds	r3, #236
	ldr	r3, [r3, #0]
	ldr	r6, [r0, #104]
	lsls	r1, r1, #15
	adds	r0, r3, r1
	mov	r3, ip
	adds	r3, #240
	ldr	r2, [r6, #12]
	ldr	r3, [r3, #0]
	movs	r4, #192
	adds	r3, r3, r2
	lsls	r4, r4, #15
	adds	r1, r3, r4
	mov	r3, ip
	adds	r3, #244
	ldr	r3, [r3, #0]
	ldr	r5, [pc, #628]
	ldr	r7, [pc, #632]
	adds	r4, r3, r5
	mov	r3, ip
	adds	r3, #248
	ldr	r3, [r3, #0]
	adds	r3, r3, r2
	ldr	r2, [sp, #4]
	adds	r5, r3, r7
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	cmp	r6, #0
	bne.n	.L_080251ba
	b.n	.L_080253fa
.L_080251ba:
	ldr	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L_080251c2
	b.n	.L_080253fa
.L_080251c2:
	ldr	r2, [r6, #8]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #16]
	mov	sl, r2
	ldr	r2, [sp, #4]
	mov	fp, r3
	movs	r3, #128
	lsls	r3, r3, #24
	mov	r9, r7
	str	r3, [r2, #56]
	str	r3, [r2, #60]
	str	r3, [r2, #64]
	cmp	sl, r0
	bge.n	.L_080251e0
	mov	sl, r0
.L_080251e0:
	cmp	r9, r1
	bge.n	.L_080251e6
	mov	r9, r1
.L_080251e6:
	cmp	sl, r4
	ble.n	.L_080251ec
	mov	sl, r4
.L_080251ec:
	cmp	r9, r5
	ble.n	.L_080251f2
	mov	r9, r5
.L_080251f2:
	mov	r3, ip
	adds	r3, #252
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080252d8
	adds	r2, r3, #0
	movs	r4, #0
	ldrsh	r3, [r2, r4]
	movs	r0, #1
	negs	r0, r0
	ldrh	r1, [r2, #0]
	cmp	r3, r0
	beq.n	.L_080252d8
	ldr	r6, [r6, #12]
	mov	ip, r0
.L_08025210:
	ldr	r5, [pc, #512]
	lsls	r3, r1, #16
	adds	r0, r3, r5
	movs	r7, #2
	ldrsh	r3, [r2, r7]
	ldr	r4, [pc, #508]
	lsls	r3, r3, #16
	adds	r3, r3, r6
	adds	r1, r3, r4
	movs	r5, #4
	ldrsh	r3, [r2, r5]
	movs	r7, #240
	lsls	r3, r3, #16
	lsls	r7, r7, #15
	adds	r4, r3, r7
	movs	r5, #6
	ldrsh	r3, [r2, r5]
	movs	r7, #192
	lsls	r3, r3, #16
	adds	r3, r3, r6
	lsls	r7, r7, #15
	adds	r5, r3, r7
	cmp	sl, r0
	blt.n	.L_080252cc
	cmp	r9, r1
	blt.n	.L_080252cc
	cmp	sl, r4
	bgt.n	.L_080252cc
	cmp	r9, r5
	bgt.n	.L_080252cc
	mov	r2, sl
	mov	r8, r0
	subs	r0, r0, r2
	adds	r2, r0, #0
	cmp	r0, #0
	bge.n	.L_0802525e
	mov	r3, sl
	mov	r6, r8
	subs	r2, r3, r6
.L_0802525e:
	mov	r7, sl
	subs	r3, r4, r7
	cmp	r3, #0
	blt.n	.L_0802526c
	cmp	r2, r3
	bgt.n	.L_08025274
	b.n	.L_0802527a
.L_0802526c:
	mov	r6, sl
	subs	r3, r6, r4
	cmp	r2, r3
	ble.n	.L_0802527a
.L_08025274:
	mov	r7, sl
	mov	r8, r4
	subs	r0, r4, r7
.L_0802527a:
	mov	r2, r9
	adds	r6, r1, #0
	subs	r1, r6, r2
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_0802528a
	mov	r3, r9
	subs	r2, r3, r6
.L_0802528a:
	mov	r4, r9
	subs	r3, r5, r4
	cmp	r3, #0
	blt.n	.L_08025298
	cmp	r2, r3
	bgt.n	.L_080252a0
	b.n	.L_080252a6
.L_08025298:
	mov	r7, r9
	subs	r3, r7, r5
	cmp	r2, r3
	ble.n	.L_080252a6
.L_080252a0:
	adds	r6, r5, #0
	mov	r2, r9
	subs	r1, r6, r2
.L_080252a6:
	adds	r2, r0, #0
	cmp	r2, #0
	bge.n	.L_080252b2
	mov	r3, sl
	mov	r4, r8
	subs	r2, r3, r4
.L_080252b2:
	cmp	r1, #0
	blt.n	.L_080252bc
	cmp	r2, r1
	ble.n	.L_080252c4
	b.n	.L_080252c8
.L_080252bc:
	mov	r5, r9
	subs	r3, r5, r6
	cmp	r2, r3
	bgt.n	.L_080252c8
.L_080252c4:
	mov	sl, r8
	b.n	.L_080252d8
.L_080252c8:
	mov	r9, r6
	b.n	.L_080252d8
.L_080252cc:
	adds	r2, #8
	movs	r7, #0
	ldrsh	r3, [r2, r7]
	ldrh	r1, [r2, #0]
	cmp	r3, ip
	bne.n	.L_08025210
.L_080252d8:
	ldr	r3, [sp, #4]
	adds	r3, #100
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	beq.n	.L_080252f4
	ldr	r2, [sp, #4]
	mov	r1, sl
	mov	r3, fp
	mov	r4, r9
	str	r1, [r2, #8]
	str	r3, [r2, #12]
	str	r4, [r2, #16]
	b.n	.L_080253fa
.L_080252f4:
	ldr	r5, [sp, #4]
	mov	r6, sl
	ldr	r3, [r5, #8]
	subs	r0, r6, r3
	cmp	r0, #0
	bge.n	.L_08025308
	movs	r7, #255
	lsls	r7, r7, #8
	adds	r7, #255
	adds	r0, r0, r7
.L_08025308:
	ldr	r2, [sp, #4]
	asrs	r0, r0, #16
	ldr	r3, [r2, #16]
	mov	r4, r9
	mov	r8, r0
	subs	r0, r4, r3
	cmp	r0, #0
	bge.n	.L_08025320
	movs	r5, #255
	lsls	r5, r5, #8
	adds	r5, #255
	adds	r0, r0, r5
.L_08025320:
	asrs	r6, r0, #16
	mov	r7, r8
	mov	r0, r8
	muls	r0, r7
	adds	r3, r6, #0
	muls	r3, r6
	adds	r0, r0, r3
	ldr	r3, [pc, #236]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x0407
	ldr	r0, [sp, #4]
	mov	r1, sl
	ldr	r3, [r0, #8]
	mov	r2, fp
	subs	r1, r1, r3
	ldr	r3, [r0, #12]
	movs	r5, #128
	subs	r2, r2, r3
	str	r2, [sp, #0]
	mov	r4, r9
	ldr	r3, [r0, #16]
	lsls	r5, r5, #15
	mov	r8, r1
	subs	r6, r4, r3
	cmp	r7, r5
	bge.n	.L_08025372
	ldr	r7, [pc, #200]
	mov	r0, r8
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x1c31
	adds	r5, r0, #0
	adds	r0, r6, #0
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x182d
	adds	r0, r5, #0
	bl	sub_080149e0
	adds	r7, r0, #0
.L_08025372:
	adds	r1, r7, #0
	cmp	r7, #0
	bge.n	.L_0802537a
	adds	r1, r7, #7
.L_0802537a:
	ldr	r0, [sp, #4]
	asrs	r1, r1, #3
	ldr	r3, [r0, #48]
	mov	fp, r1
	cmp	fp, r3
	ble.n	.L_08025388
	mov	fp, r3
.L_08025388:
	movs	r1, #128
	lsls	r1, r1, #7
	cmp	r7, r1
	bge.n	.L_0802539c
	ldr	r3, [sp, #4]
	mov	r2, sl
	mov	r4, r9
	str	r2, [r3, #8]
	str	r4, [r3, #16]
	b.n	.L_080253d4
.L_0802539c:
	cmp	r7, fp
	ble.n	.L_080253c6
	ldr	r5, [pc, #128]
	mov	r1, r8
	mov	sl, r5
	adds	r0, r7, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x4d1c
	mov	r1, fp
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x1c31
	mov	r8, r0
	adds	r0, r7, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x4659
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x1c06
.L_080253c6:
	ldr	r7, [sp, #4]
	ldr	r3, [r7, #8]
	add	r3, r8
	str	r3, [r7, #8]
	ldr	r3, [r7, #16]
	adds	r3, r3, r6
	str	r3, [r7, #16]
.L_080253d4:
	ldr	r3, [sp, #0]
	cmp	r3, #0
	bge.n	.L_080253dc
	negs	r3, r3
.L_080253dc:
	movs	r0, #128
	lsls	r0, r0, #8
	cmp	r3, r0
	ble.n	.L_080253f0
	ldr	r3, [sp, #0]
	cmp	r3, #0
	bge.n	.L_080253ec
	adds	r3, #3
.L_080253ec:
	asrs	r3, r3, #2
	str	r3, [sp, #0]
.L_080253f0:
	ldr	r1, [sp, #4]
	ldr	r2, [sp, #0]
	ldr	r3, [r1, #12]
	adds	r3, r3, r2
	str	r3, [r1, #12]
.L_080253fa:
	ldr	r4, [sp, #4]
	movs	r0, #1
	ldrh	r3, [r4, #4]
	adds	r5, r4, #0
	adds	r3, #1
	strh	r3, [r5, #4]
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xff880000
	.4byte 0xffc00000
	.4byte 0x030002d4
	.4byte 0x0300021c
	.2byte 0x0230
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	mov	r8, r0
	adds	r3, r2, #0
	adds	r3, #236
	ldr	r3, [r3, #0]
	ldr	r1, [r0, #104]
	movs	r0, #240
	lsls	r0, r0, #15
	adds	r7, r3, r0
	adds	r3, r2, #0
	adds	r3, #240
	ldr	r3, [r3, #0]
	movs	r0, #192
	lsls	r0, r0, #15
	adds	r6, r3, r0
	adds	r3, r2, #0
	adds	r3, #244
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #404]
	sub	sp, #8
	adds	r4, r3, r0
	adds	r3, r2, #0
	adds	r3, #248
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #396]
	adds	r0, r3, r2
	mov	r2, r8
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	cmp	r1, #0
	bne.n	.L_0802547c
	b.n	.L_080255d8
.L_0802547c:
	ldr	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L_08025484
	b.n	.L_080255d8
.L_08025484:
	ldr	r3, [r1, #8]
	ldr	r5, [r1, #12]
	ldr	r1, [r1, #16]
	mov	fp, r3
	movs	r3, #128
	lsls	r3, r3, #24
	mov	r2, r8
	str	r1, [sp, #4]
	str	r3, [r2, #56]
	str	r3, [r2, #60]
	str	r3, [r2, #64]
	cmp	fp, r7
	bge.n	.L_080254a0
	mov	fp, r7
.L_080254a0:
	ldr	r3, [sp, #4]
	cmp	r3, r6
	bge.n	.L_080254a8
	str	r6, [sp, #4]
.L_080254a8:
	cmp	fp, r4
	ble.n	.L_080254ae
	mov	fp, r4
.L_080254ae:
	ldr	r2, [sp, #4]
	cmp	r2, r0
	ble.n	.L_080254b6
	str	r0, [sp, #4]
.L_080254b6:
	mov	r3, r8
	adds	r3, #100
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	beq.n	.L_080254d0
	mov	r3, r8
	mov	r2, fp
	str	r2, [r3, #8]
	str	r5, [r3, #12]
	ldr	r0, [sp, #4]
	str	r0, [r3, #16]
	b.n	.L_080255d8
.L_080254d0:
	mov	r2, r8
	ldr	r3, [r2, #8]
	mov	r2, fp
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L_080254e4
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r0, r0, r3
.L_080254e4:
	mov	r2, r8
	ldr	r3, [r2, #16]
	ldr	r2, [sp, #4]
	asrs	r0, r0, #16
	mov	sl, r0
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L_080254fc
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r0, r0, r3
.L_080254fc:
	mov	r2, sl
	asrs	r6, r0, #16
	adds	r3, r6, #0
	muls	r3, r6
	mov	r0, sl
	muls	r0, r2
	adds	r0, r0, r3
	ldr	r3, [pc, #240]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x0407
	mov	r0, r8
	ldr	r3, [r0, #8]
	mov	r2, fp
	subs	r2, r2, r3
	ldr	r3, [r0, #12]
	mov	sl, r2
	subs	r5, r5, r3
	ldr	r3, [r0, #16]
	ldr	r0, [sp, #4]
	movs	r2, #128
	lsls	r2, r2, #15
	mov	r9, r5
	subs	r6, r0, r3
	cmp	r7, r2
	bge.n	.L_08025552
	ldr	r3, [pc, #204]
	mov	r1, sl
	mov	r0, sl
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	adds	r7, r0, #0
	ldr	r2, [pc, #192]
	adds	r0, r6, #0
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x183f
	adds	r0, r7, #0
	str	r7, [sp, #0]
	bl	sub_080149e0
	adds	r7, r0, #0
.L_08025552:
	adds	r1, r7, #0
	cmp	r7, #0
	bge.n	.L_0802555a
	adds	r1, r7, #7
.L_0802555a:
	mov	r0, r8
	ldr	r3, [r0, #48]
	asrs	r5, r1, #3
	cmp	r5, r3
	ble.n	.L_08025566
	adds	r5, r3, #0
.L_08025566:
	movs	r2, #128
	lsls	r2, r2, #7
	cmp	r7, r2
	bge.n	.L_0802557a
	mov	r0, r8
	mov	r3, fp
	str	r3, [r0, #8]
	ldr	r2, [sp, #4]
	str	r2, [r0, #16]
	b.n	.L_080255b4
.L_0802557a:
	cmp	r7, r5
	ble.n	.L_080255a6
	mov	r1, sl
	ldr	r3, [pc, #128]
	adds	r0, r7, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4a1d
	adds	r1, r5, #0
	mov	fp, r2
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r3, [pc, #108]
	mov	sl, r0
	adds	r0, r7, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c29
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x1c06
.L_080255a6:
	mov	r0, r8
	ldr	r3, [r0, #8]
	add	r3, sl
	str	r3, [r0, #8]
	ldr	r3, [r0, #16]
	adds	r3, r3, r6
	str	r3, [r0, #16]
.L_080255b4:
	mov	r3, r9
	cmp	r3, #0
	bge.n	.L_080255bc
	negs	r3, r3
.L_080255bc:
	movs	r2, #128
	lsls	r2, r2, #8
	cmp	r3, r2
	ble.n	.L_080255d0
	mov	r3, r9
	cmp	r3, #0
	bge.n	.L_080255cc
	adds	r3, #3
.L_080255cc:
	asrs	r3, r3, #2
	mov	r9, r3
.L_080255d0:
	mov	r0, r8
	ldr	r3, [r0, #12]
	add	r3, r9
	str	r3, [r0, #12]
.L_080255d8:
	mov	r2, r8
	ldrh	r3, [r2, #4]
	mov	r0, r8
	adds	r3, #1
	strh	r3, [r0, #4]
	add	sp, #8
	movs	r0, #1
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xff880000
	.4byte 0xffc00000
	.4byte 0x030002d4
	.4byte 0x0300021c
	.2byte 0x0230
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r0, #0
	ldr	r2, [r6, #104]
	ldr	r3, [r2, #48]
	str	r3, [r6, #48]
	ldr	r3, [r2, #52]
	str	r3, [r6, #52]
	ldr	r3, [r6, #8]
	ldr	r1, [r2, #8]
	ldr	r2, [r2, #16]
	subs	r1, r1, r3
	ldr	r3, [r6, #16]
	mov	r8, r1
	subs	r2, r2, r3
	mov	sl, r2
	asrs	r3, r1, #16
	asrs	r2, r2, #16
	adds	r0, r3, #0
	muls	r0, r3
	adds	r3, r2, #0
	muls	r3, r2
	adds	r0, r0, r3
	ldr	r3, [pc, #88]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c07
	cmp	r7, #16
	ble.n	.L_08025682
	adds	r5, r7, #0
	subs	r5, #16
	mov	r0, r8
	muls	r0, r5
	adds	r1, r7, #0
	bl	sub_08002054
	adds	r1, r7, #0
	mov	r8, r0
	mov	r0, sl
	muls	r0, r5
	bl	sub_08002054
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	add	r1, r8
	adds	r3, r3, r0
	ldr	r2, [r6, #12]
	adds	r0, r6, #0
	bl	sub_08024738
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_080233d0
	ldrh	r3, [r6, #4]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r6, #4]
	b.n	.L_0802568c
.L_08025682:
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_080233d0
	movs	r0, #0
.L_0802568c:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	movs	r0, #4
	ldrsh	r2, [r6, r0]
	ldr	r3, [r6, #0]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r3, #4
	ldmia	r3!, {r2}
	sub	sp, #32
	str	r2, [sp, #4]
	ldmia	r3!, {r5}
	ldr	r3, [r3, #0]
	mov	fp, r5
	cmp	r3, #0
	bge.n	.L_080256cc
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #255
	adds	r3, r3, r0
.L_080256cc:
	asrs	r3, r3, #16
	adds	r5, r3, #0
	muls	r5, r3
	str	r3, [sp, #0]
	str	r5, [sp, #0]
	movs	r2, #0
	mov	r9, r2
.L_080256da:
	movs	r0, #1
	add	r9, r0
	mov	r2, r9
	cmp	r2, #7
	ble.n	.L_080256e6
	b.n	.L_080257f2
.L_080256e6:
	ldr	r3, [r6, #8]
	add	r7, sp, #20
	str	r3, [r7, #0]
	ldr	r3, [r6, #12]
	str	r3, [r7, #4]
	ldr	r3, [r6, #16]
	str	r3, [r7, #8]
	bl	sub_08014878
	mov	r1, fp
	ldr	r3, [pc, #300]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9b01
	adds	r3, r3, r0
	mov	r8, r3
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	ldrh	r3, [r6, #6]
	lsrs	r5, r5, #2
	lsrs	r0, r0, #2
	adds	r3, r3, r5
	subs	r3, r3, r0
	mov	sl, r3
	mov	r0, r8
	mov	r1, sl
	adds	r2, r7, #0
	bl	sub_0801489c
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	sub_08024f20
	cmp	r0, #0
	bne.n	.L_080256da
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_080256da
	ldr	r3, [r6, #8]
	movs	r5, #128
	lsls	r5, r5, #12
	add	r8, r5
	add	r5, sp, #8
	str	r3, [r5, #0]
	mov	r0, r8
	ldr	r3, [r6, #12]
	mov	r1, sl
	str	r3, [r5, #4]
	adds	r2, r5, #0
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_0801489c
	ldr	r3, [r6, #8]
	movs	r1, #128
	str	r3, [r5, #0]
	lsls	r1, r1, #6
	ldr	r3, [r6, #12]
	add	r1, sl
	str	r3, [r5, #4]
	mov	r0, r8
	ldr	r3, [r6, #16]
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl	sub_0801489c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_080256da
	ldr	r3, [r6, #8]
	ldr	r1, [pc, #164]
	str	r3, [r5, #0]
	add	r1, sl
	ldr	r3, [r6, #12]
	mov	r0, r8
	str	r3, [r5, #4]
	adds	r2, r5, #0
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_0801489c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_080256da
	ldr	r3, [r7, #0]
	adds	r1, r3, #0
	cmp	r3, #0
	bge.n	.L_080257b6
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #255
	adds	r3, r3, r0
.L_080257b6:
	adds	r2, r6, #0
	adds	r2, #100
	movs	r5, #0
	ldrsh	r2, [r2, r5]
	asrs	r3, r3, #16
	subs	r0, r3, r2
	ldr	r2, [r7, #8]
	adds	r4, r2, #0
	cmp	r2, #0
	bge.n	.L_080257d2
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r2, r2, r3
.L_080257d2:
	adds	r3, r6, #0
	adds	r3, #102
	movs	r5, #0
	ldrsh	r3, [r3, r5]
	asrs	r2, r2, #16
	subs	r2, r2, r3
	adds	r3, r0, #0
	muls	r3, r0
	adds	r0, r2, #0
	muls	r0, r2
	adds	r2, r0, #0
	adds	r3, r3, r2
	ldr	r2, [sp, #0]
	cmp	r3, r2
	ble.n	.L_08025808
	b.n	.L_080256da
.L_080257f2:
	ldrh	r3, [r6, #6]
	movs	r5, #128
	lsls	r5, r5, #8
	adds	r3, r3, r5
	adds	r2, r6, #0
	strh	r3, [r6, #6]
	adds	r2, #94
	movs	r3, #1
	strh	r3, [r2, #0]
	movs	r0, #0
	b.n	.L_0802581a
.L_08025808:
	adds	r0, r6, #0
	adds	r3, r4, #0
	ldr	r2, [r7, #4]
	bl	sub_08024738
	ldrh	r3, [r6, #4]
	movs	r0, #1
	adds	r3, #4
	strh	r3, [r6, #4]
.L_0802581a:
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.2byte 0xe000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	movs	r0, #4
	ldrsh	r2, [r6, r0]
	ldr	r3, [r6, #0]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r3, #4
	ldmia	r3!, {r1}
	sub	sp, #52
	str	r1, [sp, #24]
	ldmia	r3!, {r2}
	str	r2, [sp, #20]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bge.n	.L_08025864
	movs	r4, #255
	lsls	r4, r4, #8
	adds	r4, #255
	adds	r3, r3, r4
.L_08025864:
	asrs	r3, r3, #16
	movs	r2, #0
	movs	r0, #6
	ldrsh	r5, [r6, r0]
	adds	r1, r3, #0
	muls	r1, r3
	mov	sl, r2
	ldr	r2, [r6, #8]
	str	r3, [sp, #16]
	str	r5, [sp, #12]
	str	r1, [sp, #16]
	cmp	r2, #0
	bge.n	.L_08025886
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r2, r2, r3
.L_08025886:
	adds	r4, r6, #0
	adds	r4, #100
	str	r4, [sp, #8]
	asrs	r2, r2, #16
	movs	r5, #0
	ldrsh	r3, [r4, r5]
	subs	r2, r2, r3
	mov	fp, r2
	ldr	r2, [r6, #16]
	cmp	r2, #0
	bge.n	.L_080258a4
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #255
	adds	r2, r2, r0
.L_080258a4:
	adds	r1, r6, #0
	adds	r1, #102
	str	r1, [sp, #4]
	asrs	r2, r2, #16
	movs	r4, #0
	ldrsh	r3, [r1, r4]
	mov	r5, fp
	subs	r2, r2, r3
	mov	r9, r2
	mov	r0, r9
	mov	r3, fp
	muls	r3, r5
	mov	r2, r9
	muls	r2, r0
	ldr	r1, [sp, #16]
	adds	r3, r3, r2
	cmp	r3, r1
	ble.n	.L_080258ca
	b.n	.L_08025a84
.L_080258ca:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #7
	ble.n	.L_080258d6
	b.n	.L_08025a84
.L_080258d6:
	bl	sub_08014878
	ldr	r3, [pc, #624]
	ldr	r1, [sp, #20]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c06
	adds	r4, r4, r0
	mov	r8, r4
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	ldr	r3, [r6, #8]
	add	r7, sp, #40
	ldr	r1, [sp, #12]
	str	r3, [r7, #0]
	lsls	r2, r1, #16
	ldr	r3, [r6, #12]
	lsrs	r5, r5, #2
	str	r3, [r7, #4]
	lsrs	r2, r2, #16
	lsrs	r0, r0, #2
	adds	r2, r2, r5
	ldr	r3, [r6, #16]
	subs	r2, r2, r0
	lsls	r2, r2, #16
	lsrs	r4, r2, #16
	movs	r0, #128
	adds	r1, r4, #0
	str	r3, [r7, #8]
	lsls	r0, r0, #12
	adds	r2, r7, #0
	str	r4, [sp, #0]
	bl	sub_0801489c
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	sub_08024f20
	ldr	r4, [sp, #0]
	cmp	r0, #0
	bne.n	.L_080258ca
	ldr	r3, [r6, #8]
	adds	r1, r4, #0
	str	r3, [r7, #0]
	mov	r0, r8
	ldr	r3, [r6, #12]
	adds	r2, r7, #0
	str	r3, [r7, #4]
	ldr	r3, [r6, #16]
	str	r3, [r7, #8]
	bl	sub_0801489c
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	sub_0802d87c
	ldr	r4, [sp, #0]
	cmp	r0, #0
	bne.n	.L_080258ca
	ldr	r3, [r6, #8]
	add	r5, sp, #28
	str	r3, [r5, #0]
	movs	r2, #128
	ldr	r3, [r6, #12]
	lsls	r2, r2, #12
	str	r3, [r5, #4]
	add	r8, r2
	ldr	r3, [r6, #16]
	adds	r1, r4, #0
	str	r3, [r5, #8]
	mov	r0, r8
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	ldr	r4, [sp, #0]
	cmp	r0, #0
	bne.n	.L_080258ca
	ldr	r3, [r6, #8]
	mov	r0, r8
	str	r3, [r5, #0]
	adds	r2, r5, #0
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	movs	r3, #128
	lsls	r3, r3, #6
	adds	r1, r4, r3
	bl	sub_0801489c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	ldr	r4, [sp, #0]
	cmp	r0, #0
	bne.n	.L_080258ca
	ldr	r3, [r6, #8]
	ldr	r0, [pc, #420]
	str	r3, [r5, #0]
	adds	r1, r4, r0
	ldr	r3, [r6, #12]
	mov	r0, r8
	str	r3, [r5, #4]
	adds	r2, r5, #0
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_0801489c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	ldr	r4, [sp, #0]
	cmp	r0, #0
	beq.n	.L_080259ce
	b.n	.L_080258ca
.L_080259ce:
	ldr	r3, [r6, #8]
	movs	r2, #128
	str	r3, [r5, #0]
	lsls	r2, r2, #7
	ldr	r3, [r6, #12]
	adds	r1, r4, r2
	str	r3, [r5, #4]
	mov	r0, r8
	ldr	r3, [r6, #16]
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl	sub_0801489c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	ldr	r4, [sp, #0]
	cmp	r0, #0
	beq.n	.L_080259f8
	b.n	.L_080258ca
.L_080259f8:
	ldr	r3, [r6, #8]
	mov	r0, r8
	str	r3, [r5, #0]
	adds	r2, r5, #0
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	ldr	r3, [pc, #328]
	adds	r1, r4, r3
	bl	sub_0801489c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	beq.n	.L_08025a1e
	b.n	.L_080258ca
.L_08025a1e:
	ldr	r1, [r7, #0]
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_08025a2e
	movs	r4, #255
	lsls	r4, r4, #8
	adds	r4, #255
	adds	r2, r1, r4
.L_08025a2e:
	ldr	r0, [sp, #8]
	ldr	r4, [r7, #8]
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	asrs	r2, r2, #16
	subs	r2, r2, r3
	mov	fp, r2
	adds	r2, r4, #0
	cmp	r4, #0
	bge.n	.L_08025a4a
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r2, r4, r3
.L_08025a4a:
	ldr	r0, [sp, #4]
	asrs	r2, r2, #16
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	ldr	r0, [sp, #16]
	subs	r2, r2, r3
	mov	r9, r2
	mov	r5, r9
	mov	r2, fp
	mov	r3, fp
	muls	r3, r2
	mov	r2, r9
	muls	r2, r5
	adds	r3, r3, r2
	cmp	r3, r0
	ble.n	.L_08025a6c
	b.n	.L_080258ca
.L_08025a6c:
	adds	r0, r6, #0
	adds	r0, #89
	ldrb	r3, [r0, #0]
	movs	r2, #2
	orrs	r2, r3
	strb	r2, [r0, #0]
	ldr	r2, [r7, #4]
	adds	r0, r6, #0
	adds	r3, r4, #0
	bl	sub_08024738
	b.n	.L_08025b34
.L_08025a84:
	movs	r1, #0
	mov	sl, r1
	mov	r0, r9
	mov	r1, fp
	bl	sub_080148e8
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r0, r0, r2
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	str	r0, [sp, #12]
.L_08025a9c:
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	cmp	r4, #7
	bgt.n	.L_08025b34
	bl	sub_08014878
	ldr	r3, [pc, #160]
	ldr	r1, [sp, #20]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9d06
	adds	r5, r5, r0
	bl	sub_08014878
	mov	r8, r5
	adds	r5, r0, #0
	bl	sub_08014878
	ldr	r1, [sp, #12]
	ldr	r3, [r6, #8]
	lsls	r2, r1, #16
	lsrs	r5, r5, #2
	lsrs	r2, r2, #16
	adds	r2, r2, r5
	add	r5, sp, #40
	str	r3, [r5, #0]
	lsrs	r0, r0, #2
	ldr	r3, [r6, #12]
	subs	r2, r2, r0
	str	r3, [r5, #4]
	lsls	r2, r2, #16
	ldr	r3, [r6, #16]
	lsrs	r7, r2, #16
	movs	r0, #128
	lsls	r0, r0, #12
	adds	r1, r7, #0
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_08024f20
	cmp	r0, #0
	bne.n	.L_08025a9c
	ldr	r3, [r6, #8]
	mov	r0, r8
	str	r3, [r5, #0]
	adds	r1, r7, #0
	ldr	r3, [r6, #12]
	adds	r2, r5, #0
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_0801489c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08025a9c
	adds	r1, r6, #0
	adds	r1, #89
	ldrb	r2, [r1, #0]
	movs	r3, #253
	ands	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r6, #0
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	bl	sub_08024738
.L_08025b34:
	ldrh	r3, [r6, #4]
	movs	r0, #1
	adds	r3, #4
	strh	r3, [r6, #4]
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0300021c
	.4byte 0xffffe000
	.4byte 0xffffc000
