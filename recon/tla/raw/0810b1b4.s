.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08016ca4, 0x08016ca4
	.set sub_08020280, 0x08020280
	.set sub_080202c8, 0x080202c8
	.set sub_08038010, 0x08038010
	.set sub_08038018, 0x08038018
	.set sub_080380c8, 0x080380c8
	.set sub_080380f8, 0x080380f8
	.set sub_08038120, 0x08038120
	.set sub_080ad050, 0x080ad050
	.set sub_080ad0d0, 0x080ad0d0
	.set sub_080ad100, 0x080ad100
	.set sub_080ad1d8, 0x080ad1d8
	.set sub_080c8088, 0x080c8088
	.set sub_080c83a8, 0x080c83a8
	.set sub_080c83b0, 0x080c83b0
	.set sub_080c83b8, 0x080c83b8
	.set sub_080c85a0, 0x080c85a0
	.set sub_080c85a8, 0x080c85a8
	.set sub_080c85b0, 0x080c85b0
	.set sub_080c85b8, 0x080c85b8
	.set sub_08108148, 0x08108148
	.set sub_0810824c, 0x0810824c
	.set sub_081084e0, 0x081084e0
	.set sub_081084f4, 0x081084f4
	.set sub_0810857c, 0x0810857c
	.set sub_08108630, 0x08108630
	.set sub_081088d8, 0x081088d8
	.set sub_08108928, 0x08108928
	.set sub_08108aa8, 0x08108aa8
	.set sub_08109188, 0x08109188
	.set sub_08109ad8, 0x08109ad8
	.set sub_08109cac, 0x08109cac
	.set sub_0810a004, 0x0810a004
	.set sub_0810a748, 0x0810a748
	.set sub_0810a760, 0x0810a760
	.set sub_0810a834, 0x0810a834
	.set sub_0810b7b4, 0x0810b7b4
	.set sub_081c0010, 0x081c0010
	.global Func_0810b1b4
	.thumb_func
Func_0810b1b4:
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
	ldr	r3, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #3
	mov	r8, r3
	adds	r1, #220
	add	r1, r8
	ldr	r3, [r1, #0]
	movs	r2, #161
	ldrb	r3, [r3, #5]
	lsls	r2, r2, #3
	add	r2, r8
	mov	fp, r3
	movs	r3, #255
	strb	r3, [r2, #0]
	movs	r3, #13
	ldr	r2, [r1, #0]
	mov	sl, r0
	strb	r3, [r2, #5]
	movs	r3, #129
	lsls	r3, r3, #3
	adds	r3, #255
	add	r3, r8
	ldr	r2, [pc, #364]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	sub	sp, #12
	ldrsb	r0, [r2, r3]
	bl	sub_081c0010
	ldr	r0, [pc, #352]
	bl	sub_081088d8
	mov	r0, sl
	lsls	r0, r0, #2
	mov	r9, r0
	mov	r3, r9
	adds	r3, #248
	mov	r1, r8
	ldr	r0, [r1, r3]
	movs	r1, #0
	bl	sub_080202c8
	movs	r0, #20
	bl	sub_08013560
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #320]
	bl	sub_080145a8
	mov	r3, sl
	movs	r0, #140
	lsls	r2, r3, #1
	lsls	r0, r0, #1
	adds	r3, r2, r0
	mov	r1, r8
	ldrsh	r3, [r1, r3]
	mov	r6, sp
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	movs	r1, #148
	lsls	r1, r1, #1
	adds	r3, r2, r1
	mov	r2, r8
	ldrsh	r3, [r2, r3]
	ldr	r1, [pc, #288]
	lsls	r3, r3, #16
	adds	r3, r3, r1
	movs	r5, #160
	lsls	r5, r5, #3
	str	r3, [r6, #8]
	adds	r5, #12
	movs	r7, #0
	add	r5, r8
.L_0810b25e:
	movs	r1, #168
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #0]
	adds	r0, r5, #0
	lsls	r1, r1, #2
	bl	sub_080c85b0
	adds	r0, r5, #0
	ldr	r1, [pc, #260]
	bl	sub_080c85a8
	movs	r1, #7
	adds	r0, r5, #0
	bl	sub_080c85a0
	bl	sub_08014878
	lsls	r1, r0, #3
	subs	r1, r1, r0
	lsrs	r1, r1, #16
	ldr	r0, [r5, #0]
	bl	sub_08020280
	movs	r3, #179
	lsls	r3, r3, #8
	adds	r3, #51
	str	r3, [r5, #44]
	str	r3, [r5, #40]
	movs	r0, #3
	bl	sub_08013560
	cmp	r7, #5
	bne.n	.L_0810b2aa
	movs	r3, #161
	lsls	r3, r3, #3
	add	r3, r8
	mov	r2, sl
	strb	r2, [r3, #0]
.L_0810b2aa:
	adds	r7, #1
	adds	r5, #72
	cmp	r7, #17
	ble.n	.L_0810b25e
	bl	sub_081084e0
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #76
	movs	r1, #2
	add	r2, r8
	movs	r7, #23
.L_0810b2c2:
	movs	r3, #5
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L_0810b2cc
	strb	r1, [r2, #0]
.L_0810b2cc:
	subs	r7, #1
	adds	r2, #72
	cmp	r7, #0
	bge.n	.L_0810b2c2
	movs	r0, #20
	bl	sub_08013560
	movs	r0, #126
	bl	sub_081c0010
	movs	r2, #161
	lsls	r2, r2, #3
	add	r2, r8
	movs	r3, #255
	strb	r3, [r2, #0]
	mov	r3, r9
	adds	r3, #248
	mov	r1, r8
	ldr	r0, [r1, r3]
	movs	r1, #0
	bl	sub_08020280
	movs	r0, #20
	bl	sub_08013560
	movs	r6, #160
	movs	r5, #160
	lsls	r6, r6, #3
	lsls	r5, r5, #3
	adds	r6, #81
	adds	r5, #12
	add	r6, r8
	add	r5, r8
	movs	r7, #23
.L_0810b310:
	ldrb	r3, [r6, #0]
	adds	r6, #72
	lsls	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_0810b320
	adds	r0, r5, #0
	bl	sub_080c85b8
.L_0810b320:
	subs	r7, #1
	adds	r5, #72
	cmp	r7, #0
	bge.n	.L_0810b310
	ldr	r0, [pc, #64]
	bl	sub_08014644
	mov	r3, r9
	adds	r3, #248
	mov	r2, r8
	ldr	r0, [r2, r3]
	movs	r1, #16
	bl	sub_080202c8
	bl	sub_08108928
	movs	r0, #30
	bl	sub_08013560
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #220
	add	r3, r8
	ldr	r3, [r3, #0]
	mov	r0, fp
	strb	r0, [r3, #5]
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0810cf28
	.4byte 0x00202108
	.4byte 0x0810b169
	.4byte 0xfff40000
	.2byte 0xb0bd
	.2byte 0x0810
.L_0810b378:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r6, [r3, #0]
	movs	r1, #160
	ldr	r3, [pc, #92]
	lsls	r1, r1, #3
	adds	r1, #4
	ldrsb	r0, [r3, r0]
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r2, #0
	sub	sp, #4
	mov	sl, r0
	movs	r7, #0
	cmp	r2, r3
	bge.n	.L_0810b3da
	adds	r3, r6, #2
	movs	r5, #153
	mov	r8, r3
	lsls	r5, r5, #3
.L_0810b3ae:
	mov	r1, r8
	ldrsh	r0, [r1, r5]
	str	r2, [sp, #0]
	bl	sub_08016ca4
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	ldr	r2, [sp, #0]
	cmp	r3, #0
	beq.n	.L_0810b3c4
	adds	r2, #1
.L_0810b3c4:
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #4
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r7, #1
	adds	r5, #2
	cmp	r7, r3
	blt.n	.L_0810b3ae
.L_0810b3da:
	mov	r0, sl
	muls	r0, r2
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0xcf2c
	.2byte 0x0810
	push	{r5, lr}
	adds	r5, r0, #0
	bl	sub_08108148
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #6
	adds	r3, r3, r2
	movs	r2, #1
	strb	r2, [r3, #0]
	adds	r0, r5, #0
	bl	.L_0810b378
	adds	r5, r0, #0
	bl	sub_0810824c
	adds	r0, r5, #0
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #4
	adds	r5, r0, #0
	mov	r9, r1
	bl	sub_08108148
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r6, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #6
	adds	r2, r6, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	mov	r0, r9
	bl	sub_080c8088
	ldr	r3, [r0, #80]
	movs	r2, #128
	ldr	r3, [r3, #40]
	lsls	r2, r2, #3
	ldrh	r3, [r3, #0]
	adds	r2, #250
	adds	r7, r6, r2
	strh	r3, [r7, #0]
	movs	r2, #0
	movs	r3, #0
	movs	r1, #0
	ldrh	r0, [r7, #0]
	bl	sub_080380f8
	mov	r8, r0
	adds	r0, r5, #0
	bl	.L_0810b378
	movs	r1, #5
	adds	r5, r0, #0
	bl	sub_08038120
	ldr	r3, [pc, #164]
	mov	sl, r3
	mov	r0, sl
	bl	sub_081084f4
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #16
	movs	r2, #12
	movs	r3, #4
	movs	r0, #0
	bl	sub_08038010
	str	r0, [r6, #12]
	bl	sub_08109188
	movs	r0, #0
	bl	sub_08108630
	cmp	r0, #0
	beq.n	.L_0810b4a2
	mov	r0, sl
	adds	r0, #3
	b.n	.L_0810b4ae
.L_0810b4a2:
	ldr	r3, [pc, #120]
	ldr	r3, [r3, #16]
	cmp	r5, r3
	bls.n	.L_0810b4bc
	mov	r0, sl
	adds	r0, #2
.L_0810b4ae:
	bl	sub_081084f4
	ldr	r0, [r6, #12]
	movs	r1, #2
	bl	sub_08038018
	b.n	.L_0810b4fe
.L_0810b4bc:
	movs	r1, #2
	ldr	r0, [r6, #12]
	bl	sub_08038018
	mov	r0, sl
	adds	r0, #1
	bl	sub_081084f4
	movs	r1, #2
	mov	r0, r8
	bl	sub_08038018
	adds	r0, r5, #0
	bl	.L_0810b520
	mov	r0, r9
	bl	sub_080c8088
	ldr	r3, [r0, #80]
	movs	r1, #0
	ldr	r3, [r3, #40]
	movs	r2, #0
	ldrh	r3, [r3, #0]
	strh	r3, [r7, #0]
	movs	r3, #0
	ldrh	r0, [r7, #0]
	bl	sub_080380f8
	mov	r8, r0
	mov	r0, sl
	adds	r0, #4
	bl	sub_081084f4
.L_0810b4fe:
	mov	r0, r8
	movs	r1, #2
	bl	sub_08038018
	bl	sub_0810824c
	movs	r0, #0
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x000012cd
	.2byte 0x0240
	.2byte 0x0200
.L_0810b520:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #16
	mov	r8, sp
	adds	r5, r0, #0
	mov	r0, r8
	bl	sub_080ad100
	negs	r5, r5
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_080ad1d8
	cmp	r7, #0
	ble.n	.L_0810b570
	mov	sl, r8
	movs	r6, #0
	adds	r5, r7, #0
.L_0810b548:
	mov	r2, sl
	ldrsh	r0, [r6, r2]
	bl	sub_08016ca4
	movs	r2, #56
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	beq.n	.L_0810b568
	ldrh	r3, [r0, #52]
	strh	r3, [r0, #56]
	ldrh	r3, [r0, #54]
	strh	r3, [r0, #58]
	mov	r3, r8
	ldrsh	r0, [r6, r3]
	bl	sub_080ad0d0
.L_0810b568:
	subs	r5, #1
	adds	r6, #2
	cmp	r5, #0
	bne.n	.L_0810b548
.L_0810b570:
	movs	r6, #192
	lsls	r6, r6, #18
	ldr	r1, [r6, #108]
	movs	r3, #214
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r5, #218
	adds	r3, #93
	str	r3, [r2, #0]
	lsls	r5, r5, #1
	movs	r3, #60
	str	r3, [r1, r5]
	movs	r0, #20
	bl	sub_08013560
	bl	sub_080c83b0
	bl	sub_080c83b8
	movs	r0, #86
	bl	sub_081c0010
	bl	sub_081084e0
	movs	r0, #10
	bl	sub_08013560
	bl	sub_080c83a8
	bl	sub_080c83b8
	movs	r0, #30
	bl	sub_08013560
	ldr	r2, [r6, #108]
	movs	r3, #16
	str	r3, [r2, r5]
	add	sp, #16
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	adds	r5, r0, #0
	bl	sub_08108148
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r6, [r3, #0]
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #6
	adds	r3, r6, r1
	movs	r1, #1
	mov	r8, r1
	movs	r2, #0
	mov	r9, r2
	mov	r2, r8
	strb	r2, [r3, #0]
	movs	r3, #129
	lsls	r3, r3, #3
	adds	r3, #255
	adds	r2, r6, r3
	movs	r3, #4
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	bl	sub_080c8088
	ldr	r3, [r0, #80]
	movs	r1, #128
	ldr	r3, [r3, #40]
	lsls	r1, r1, #3
	ldrh	r2, [r3, #0]
	adds	r1, #250
	adds	r3, r6, r1
	strh	r2, [r3, #0]
	ldr	r1, [pc, #56]
	ldrh	r0, [r3, #0]
	mov	sl, r1
	movs	r2, #0
	movs	r1, #0
	movs	r3, #0
	bl	sub_080380f8
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #236
	adds	r3, r6, r2
	mov	fp, r0
	movs	r1, #128
	ldrh	r0, [r3, #0]
	lsls	r1, r1, #23
	mov	r3, r9
	mov	r2, fp
	str	r3, [sp, #0]
	bl	sub_080380c8
	movs	r3, #255
	adds	r5, r0, #0
	strb	r3, [r5, #15]
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #220
	mov	r1, r8
	b.n	.L_0810b658
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_0810b658:
	adds	r6, r6, r3
	strb	r1, [r5, #5]
	mov	r2, sl
	movs	r1, #32
	negs	r1, r1
	strb	r2, [r5, #4]
	adds	r0, r6, #0
	movs	r2, #112
	mov	r8, r1
	bl	sub_08108aa8
	ldr	r7, [pc, #72]
	str	r5, [r6, #0]
	adds	r0, r7, #0
	bl	sub_081084f4
	adds	r0, r7, #1
	bl	sub_081084f4
	bl	.L_0810b6bc
	movs	r2, #112
	adds	r5, r0, #0
	mov	r1, r8
	adds	r0, r6, #0
	bl	sub_08108aa8
	movs	r2, #1
	negs	r2, r2
	cmp	r5, r2
	bne.n	.L_0810b69c
	adds	r0, r7, #2
	bl	sub_081084f4
.L_0810b69c:
	mov	r0, fp
	movs	r1, #2
	bl	sub_08038018
	bl	sub_0810824c
	movs	r0, #0
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x12fd
	.2byte 0x0000
.L_0810b6bc:
	push	{lr}
	bl	sub_08109ad8
	pop	{pc}
	push	{lr}
	bl	sub_08109cac
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	mov	sl, r1
	mov	r9, r3
	adds	r7, r0, #0
	bl	sub_08016ca4
	mov	r2, sl
	lsls	r3, r2, #1
	adds	r3, #216
	ldrh	r3, [r0, r3]
	movs	r6, #128
	lsls	r6, r6, #1
	adds	r6, #255
	ands	r6, r3
	adds	r0, r6, #0
	bl	sub_0810a748
	movs	r3, #1
	adds	r5, r0, #0
	negs	r3, r3
	adds	r0, r6, #0
	mov	r8, r3
	bl	sub_0810a748
	cmp	r0, #0
	bne.n	.L_0810b718
	ldr	r0, [pc, #108]
.L_0810b712:
	bl	sub_0810857c
	b.n	.L_0810b774
.L_0810b718:
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_08038120
	cmp	r5, #1
	bne.n	.L_0810b728
	ldr	r0, [pc, #92]
	b.n	.L_0810b72a
.L_0810b728:
	ldr	r0, [pc, #92]
.L_0810b72a:
	bl	sub_0810857c
	movs	r0, #0
	bl	sub_08108630
	cmp	r0, #0
	beq.n	.L_0810b744
	cmp	r5, #1
	bne.n	.L_0810b740
	ldr	r0, [pc, #76]
	b.n	.L_0810b712
.L_0810b740:
	ldr	r0, [pc, #76]
	b.n	.L_0810b712
.L_0810b744:
	cmp	r5, #1
	bne.n	.L_0810b74c
	ldr	r0, [pc, #72]
	b.n	.L_0810b74e
.L_0810b74c:
	ldr	r0, [pc, #72]
.L_0810b74e:
	bl	sub_0810857c
	mov	r1, sl
	adds	r0, r7, #0
	bl	sub_080ad050
	mov	r2, r9
	ldr	r0, [r2, #36]
	adds	r1, r7, #0
	bl	sub_0810a004
	adds	r0, r6, #0
	bl	sub_0810a760
	adds	r6, r0, #0
	bl	sub_0810a834
	movs	r3, #0
	mov	r8, r3
.L_0810b774:
	mov	r0, r8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x00001300
	.4byte 0x00001301
	.4byte 0x00001302
	.4byte 0x00001303
	.4byte 0x00001304
	.4byte 0x00001305
	.2byte 0x1306
	.2byte 0x0000
	push	{lr}
	movs	r2, #1
	bl	sub_0810b7b4
	pop	{pc}
	.align 2, 0
