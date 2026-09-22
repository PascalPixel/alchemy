.syntax unified
	.thumb
	.set sub_080148e8, 0x080148e8
	.set sub_08016ca4, 0x08016ca4
	.set sub_080200a8, 0x080200a8
	.set sub_080200c0, 0x080200c0
	.set sub_080202b8, 0x080202b8
	.set sub_080ad070, 0x080ad070
	.set sub_080ad1d0, 0x080ad1d0
	.set sub_081280bc, 0x081280bc
	.set sub_081280d8, 0x081280d8
	.set sub_081280fc, 0x081280fc
	.global Overlay_0811a4e0
Overlay_0811a4e0:
.L_0811a4e0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	sub	sp, #4
	ldr	r7, [r3, #36]
	mov	fp, r0
	bl	sub_08016ca4
	movs	r5, #165
	mov	r9, r0
	lsls	r5, r5, #1
	add	r5, r9
	ldrh	r0, [r5, #0]
	bl	sub_081280fc
	movs	r1, #0
	mov	r8, r0
	ldrh	r0, [r5, #0]
	mov	sl, r1
	bl	sub_081280bc
	mov	r4, sl
	adds	r6, r0, #0
.L_0811a51a:
	movs	r3, #42
	adds	r3, #255
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0811a5e0
	movs	r2, #4
	ldrsh	r3, [r7, r2]
	movs	r5, #0
	cmp	r3, #0
	bne.n	.L_0811a53e
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L_0811a560
	movs	r1, #6
	ldrsh	r3, [r7, r1]
	cmp	r3, #0
	beq.n	.L_0811a560
.L_0811a53e:
	adds	r5, #1
	cmp	r5, #5
	bgt.n	.L_0811a560
	lsls	r2, r5, #1
	adds	r3, r2, #4
	ldrsh	r3, [r7, r3]
	cmp	r3, #0
	bne.n	.L_0811a53e
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L_0811a560
	cmp	r5, #4
	bgt.n	.L_0811a53e
	adds	r3, r2, #6
	ldrsh	r3, [r7, r3]
	cmp	r3, #0
	bne.n	.L_0811a53e
.L_0811a560:
	cmp	r5, #6
	beq.n	.L_0811a5e6
	movs	r3, #165
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r0, [r3, #0]
	str	r4, [sp, #0]
	bl	sub_081280d8
	ldr	r2, [pc, #132]
	ldr	r4, [sp, #0]
	lsls	r1, r5, #14
	adds	r3, r0, #0
	adds	r1, r1, r2
	adds	r0, r5, #0
	adds	r2, r6, r4
	bl	sub_080202b8
	ldr	r4, [sp, #0]
	cmp	r0, #0
	bne.n	.L_0811a58e
	movs	r0, #0
	b.n	.L_0811a5e8
.L_0811a58e:
	cmp	r4, #0
	bne.n	.L_0811a598
	lsls	r3, r5, #12
	orrs	r3, r6
	mov	sl, r3
.L_0811a598:
	lsls	r0, r5, #1
	adds	r3, r0, #4
	mov	r1, fp
	mov	r2, r8
	strh	r1, [r7, r3]
	cmp	r2, #0
	bne.n	.L_0811a5aa
	adds	r3, r0, #6
	strh	r1, [r7, r3]
.L_0811a5aa:
	movs	r2, #153
	lsls	r2, r2, #1
	adds	r2, #255
	cmp	r6, r2
	beq.n	.L_0811a5e0
	movs	r3, #188
	lsls	r3, r3, #1
	adds	r3, #255
	cmp	r6, r3
	beq.n	.L_0811a5e0
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #126
	cmp	r6, r1
	beq.n	.L_0811a5e0
	adds	r2, #24
	cmp	r6, r2
	beq.n	.L_0811a5e0
	subs	r3, #52
	cmp	r6, r3
	beq.n	.L_0811a5e0
	subs	r1, #57
	cmp	r6, r1
	beq.n	.L_0811a5e0
	subs	r2, #2
	cmp	r6, r2
	bne.n	.L_0811a5e6
.L_0811a5e0:
	adds	r4, #1
	cmp	r4, #1
	ble.n	.L_0811a51a
.L_0811a5e6:
	mov	r0, sl
.L_0811a5e8:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x8000
	.2byte 0x0201
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [pc, #32]
	ldr	r7, [r3, #36]
	mov	r8, r0
	movs	r6, #0
	mov	sl, r2
	movs	r5, #4
.L_0811a614:
	ldrsh	r3, [r5, r7]
	cmp	r3, r8
	bne.n	.L_0811a630
	movs	r3, #0
	adds	r0, r6, #0
	movs	r1, #0
	movs	r2, #0
	bl	sub_080202b8
	mov	r3, sl
	strh	r3, [r5, r7]
	b.n	.L_0811a630
	.2byte 0x0000
	.2byte 0x0000
.L_0811a630:
	adds	r6, #1
	adds	r5, #2
	cmp	r6, #5
	ble.n	.L_0811a614
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
.L_0811a640:
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r4, #0
	ldr	r0, [pc, #40]
.L_0811a648:
	lsls	r1, r4, #1
	ldrh	r2, [r0, r1]
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r3, r2
	cmp	r5, r3
	bne.n	.L_0811a65e
	ldrh	r3, [r0, r1]
	lsrs	r0, r3, #9
	b.n	.L_0811a66e
.L_0811a65e:
	lsls	r3, r2, #16
	movs	r2, #1
	asrs	r3, r3, #16
	negs	r2, r2
	adds	r4, #1
	cmp	r3, r2
	bne.n	.L_0811a648
	movs	r0, #6
.L_0811a66e:
	pop	{r5, pc}
	.2byte 0xcad0
	.2byte 0x0812
.L_0811a674:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	bl	sub_08016ca4
	movs	r1, #1
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_080ad1d0
	movs	r5, #0
	cmp	r0, #0
	blt.n	.L_0811a6f6
	lsls	r3, r0, #1
	adds	r3, #216
	ldrh	r3, [r6, r3]
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r3
	bl	.L_0811a640
	movs	r2, #165
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #7
	bhi.n	.L_0811a6f6
	ldr	r2, [pc, #80]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x0811a6d4
	.4byte 0x0811a6d8
	.4byte 0x0811a6dc
	.4byte 0x0811a6e0
	.4byte 0x0811a6e8
	.4byte 0x0811a6e4
	.4byte 0x0811a6ec
	.2byte 0xa6f0
	.2byte 0x0811
	ldr	r2, [pc, #40]
	b.n	.L_0811a6f2
	ldr	r2, [pc, #40]
	b.n	.L_0811a6f2
	ldr	r2, [pc, #40]
	b.n	.L_0811a6f2
	ldr	r2, [pc, #40]
	b.n	.L_0811a6f2
	ldr	r2, [pc, #40]
	b.n	.L_0811a6f2
	ldr	r2, [pc, #40]
	b.n	.L_0811a6f2
	ldr	r2, [pc, #40]
	b.n	.L_0811a6f2
	ldr	r2, [pc, #40]
.L_0811a6f2:
	lsls	r3, r0, #1
	ldrh	r5, [r2, r3]
.L_0811a6f6:
	adds	r0, r5, #0
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x0811a6b4
	.4byte 0x081287d4
	.4byte 0x081287e2
	.4byte 0x081287f0
	.4byte 0x081287fe
	.4byte 0x0812880c
	.4byte 0x0812881a
	.4byte 0x08128828
	.2byte 0x8836
	.2byte 0x0812
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	ldr	r0, [r5, #0]
	adds	r7, r1, #0
	cmp	r0, #0
	bne.n	.L_0811a73a
	movs	r0, #240
	lsls	r0, r0, #8
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_080200c0
.L_0811a73a:
	adds	r6, r0, #0
	movs	r3, #230
	lsls	r3, r3, #8
	adds	r3, #102
	movs	r2, #0
	str	r3, [r5, #24]
	str	r6, [r5, #0]
	cmp	r7, #0
	bne.n	.L_0811a756
	str	r2, [r5, #16]
	str	r2, [r5, #12]
	str	r2, [r6, #8]
	ldr	r3, [r5, #16]
	str	r3, [r6, #16]
.L_0811a756:
	ldr	r3, [pc, #60]
	str	r2, [r5, #20]
	strh	r2, [r5, #4]
	strh	r2, [r5, #6]
	strh	r2, [r5, #8]
	strh	r2, [r5, #10]
	str	r2, [r5, #32]
	strh	r2, [r5, #28]
	str	r2, [r5, #36]
	strh	r2, [r5, #40]
	adds	r2, r5, #0
	adds	r2, #42
	strb	r3, [r5, #30]
	movs	r1, #0
	strb	r3, [r2, #0]
	movs	r0, #0
	bl	sub_080148e8
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r2, r6, #0
	adds	r0, r0, r3
	adds	r2, #89
	movs	r3, #3
	strh	r0, [r6, #6]
	strb	r3, [r2, #0]
	subs	r2, #4
	movs	r3, #2
	strb	r3, [r2, #0]
	adds	r2, r5, #0
	b.n	.L_0811a798
	.2byte 0x0000
	.2byte 0x0000
.L_0811a798:
	adds	r2, #43
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r1, [pc, #8]
	adds	r0, r6, #0
	bl	sub_080200a8
	pop	{r5, r6, r7, pc}
	.2byte 0xcacc
	.2byte 0x0812
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	str	r2, [sp, #16]
	adds	r7, r0, #0
	lsls	r2, r2, #16
	movs	r0, #240
	str	r3, [sp, #12]
	str	r2, [sp, #8]
	lsls	r3, r3, #16
	mov	r9, r1
	lsls	r0, r0, #8
	adds	r1, r2, #0
	movs	r2, #0
	str	r3, [sp, #4]
	bl	sub_080200c0
	mov	r8, r0
	mov	r0, r9
	bl	sub_08016ca4
	movs	r2, #0
	mov	sl, r0
	mov	r0, r9
	mov	fp, r2
	bl	.L_0811a4e0
	movs	r3, #42
	adds	r3, #255
	add	r3, sl
	ldrb	r3, [r3, #0]
	adds	r6, r0, #0
	cmp	r3, #0
	bne.n	.L_0811a81e
	movs	r5, #165
	lsls	r5, r5, #1
	add	r5, sl
	ldrh	r0, [r5, #0]
	bl	sub_081280bc
	adds	r1, r0, #0
	cmp	r6, #0
	bne.n	.L_0811a81a
	ldrh	r0, [r5, #0]
	str	r1, [sp, #0]
	bl	sub_081280d8
	mov	fp, r0
	ldr	r1, [sp, #0]
	b.n	.L_0811a88c
.L_0811a81a:
	adds	r1, r6, #0
	b.n	.L_0811a88c
.L_0811a81e:
	movs	r3, #165
	lsls	r3, r3, #1
	add	r3, sl
	ldrh	r3, [r3, #0]
	cmp	r3, #7
	bhi.n	.L_0811a87c
	ldr	r2, [pc, #76]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x0811a854
	.4byte 0x0811a85a
	.4byte 0x0811a864
	.4byte 0x0811a85e
	.4byte 0x0811a86a
	.4byte 0x0811a87c
	.4byte 0x0811a870
	.2byte 0xa874
	.2byte 0x0811
	movs	r1, #245
	lsls	r1, r1, #1
	b.n	.L_0811a882
	movs	r1, #240
	b.n	.L_0811a880
	movs	r1, #252
	lsls	r1, r1, #1
	b.n	.L_0811a882
	movs	r1, #250
	lsls	r1, r1, #1
	b.n	.L_0811a882
	movs	r1, #254
	lsls	r1, r1, #1
	b.n	.L_0811a882
	movs	r1, #131
	b.n	.L_0811a87e
	movs	r1, #133
	b.n	.L_0811a87e
	.2byte 0xa834
	.2byte 0x0811
.L_0811a87c:
	movs	r1, #129
.L_0811a87e:
	lsls	r1, r1, #1
.L_0811a880:
	adds	r1, #255
.L_0811a882:
	mov	r3, r9
	cmp	r3, #7
	bls.n	.L_0811a88c
	movs	r2, #1
	mov	fp, r2
.L_0811a88c:
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #24]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #174
	strh	r3, [r7, #10]
	movs	r3, #165
	lsls	r3, r3, #1
	add	r3, sl
	ldrh	r3, [r3, #0]
	movs	r2, #193
	lsls	r2, r2, #1
	cmp	r3, r2
	bls.n	.L_0811a8ae
	bl	.L_0811b0a8
.L_0811a8ae:
	ldr	r2, [pc, #8]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	b.n	.L_0811a8bc
	movs	r0, r0
	.2byte 0xa8c0
	.2byte 0x0811
.L_0811a8bc:
	mov	pc, r3
	movs	r0, r0
	add	r6, sp, #816
	lsrs	r1, r2, #32
	add	r6, sp, #840
	lsrs	r1, r2, #32
	add	r6, sp, #888
	lsrs	r1, r2, #32
	add	r6, sp, #864
	lsrs	r1, r2, #32
	add	r6, sp, #976
	lsrs	r1, r2, #32
	add	r6, sp, #944
	lsrs	r1, r2, #32
	add	r6, sp, #1000
	lsrs	r1, r2, #32
	add	r6, sp, #912
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	sp, #64
	lsrs	r1, r2, #32
	add	sp, #64
	lsrs	r1, r2, #32
	add	sp, #64
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #128
	lsrs	r1, r2, #32
	sub	sp, #128
	lsrs	r1, r2, #32
	sub	sp, #128
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	sp, #304
	lsrs	r1, r2, #32
	add	sp, #304
	lsrs	r1, r2, #32
	add	sp, #408
	lsrs	r1, r2, #32
	add	sp, #432
	lsrs	r1, r2, #32
	add	sp, #432
	lsrs	r1, r2, #32
	add	sp, #432
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	sp, #184
	lsrs	r1, r2, #32
	add	sp, #208
	lsrs	r1, r2, #32
	add	sp, #208
	lsrs	r1, r2, #32
	add	sp, #144
	lsrs	r1, r2, #32
	add	sp, #144
	lsrs	r1, r2, #32
	add	sp, #144
	lsrs	r1, r2, #32
	add	sp, #144
	lsrs	r1, r2, #32
	add	sp, #144
	lsrs	r1, r2, #32
	add	sp, #144
	lsrs	r1, r2, #32
	add	sp, #144
	lsrs	r1, r2, #32
	sub	sp, #128
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	sp, #160
	lsrs	r1, r2, #32
	add	sp, #160
	lsrs	r1, r2, #32
	add	sp, #160
	lsrs	r1, r2, #32
	add	sp, #328
	lsrs	r1, r2, #32
	add	sp, #328
	lsrs	r1, r2, #32
	add	sp, #328
	lsrs	r1, r2, #32
	add	sp, #376
	lsrs	r1, r2, #32
	add	sp, #376
	lsrs	r1, r2, #32
	add	sp, #376
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	sp, #224
	lsrs	r1, r2, #32
	add	sp, #240
	lsrs	r1, r2, #32
	add	sp, #256
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	sp, #224
	lsrs	r1, r2, #32
	add	sp, #240
	lsrs	r1, r2, #32
	add	sp, #256
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	sp, #496
	lsrs	r1, r2, #32
	add	sp, #496
	lsrs	r1, r2, #32
	add	sp, #496
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	sp, #280
	lsrs	r1, r2, #32
	add	sp, #280
	lsrs	r1, r2, #32
	add	sp, #280
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	sp, #448
	lsrs	r1, r2, #32
	add	sp, #472
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	sp, #432
	lsrs	r1, r2, #32
	add	sp, #432
	lsrs	r1, r2, #32
	add	sp, #432
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	sp, #432
	lsrs	r1, r2, #32
	add	sp, #432
	lsrs	r1, r2, #32
	add	sp, #432
	lsrs	r1, r2, #32
	add	sp, #128
	lsrs	r1, r2, #32
	add	sp, #128
	lsrs	r1, r2, #32
	add	sp, #128
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	sp, #96
	lsrs	r1, r2, #32
	add	sp, #96
	lsrs	r1, r2, #32
	add	sp, #96
	lsrs	r1, r2, #32
	add	sp, #96
	lsrs	r1, r2, #32
	add	sp, #96
	lsrs	r1, r2, #32
	add	sp, #96
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	r7, sp, #0
	lsrs	r1, r2, #32
	add	r7, sp, #24
	lsrs	r1, r2, #32
	add	r7, sp, #48
	lsrs	r1, r2, #32
	add	r7, sp, #72
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	r7, sp, #264
	lsrs	r1, r2, #32
	add	r7, sp, #288
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	r7, sp, #616
	lsrs	r1, r2, #32
	add	r7, sp, #632
	lsrs	r1, r2, #32
	sub	sp, #128
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	r7, sp, #1016
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	sp, #40
	lsrs	r1, r2, #32
	add	sp, #40
	lsrs	r1, r2, #32
	add	sp, #40
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	r7, sp, #96
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	r7, sp, #648
	lsrs	r1, r2, #32
	add	r7, sp, #672
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	r7, sp, #112
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	r7, sp, #696
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	r7, sp, #720
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	r7, sp, #744
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	sp, #16
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	r7, sp, #776
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #128
	lsrs	r1, r2, #32
	sub	sp, #128
	lsrs	r1, r2, #32
	add	r7, sp, #176
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	r7, sp, #920
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	r7, sp, #144
	lsrs	r1, r2, #32
	add	r7, sp, #160
	lsrs	r1, r2, #32
	add	r7, sp, #792
	lsrs	r1, r2, #32
	add	r7, sp, #824
	lsrs	r1, r2, #32
	add	r7, sp, #856
	lsrs	r1, r2, #32
	add	r7, sp, #872
	lsrs	r1, r2, #32
	add	r7, sp, #888
	lsrs	r1, r2, #32
	add	r7, sp, #904
	lsrs	r1, r2, #32
	add	r7, sp, #200
	lsrs	r1, r2, #32
	add	r7, sp, #360
	lsrs	r1, r2, #32
	add	r7, sp, #944
	lsrs	r1, r2, #32
	add	r7, sp, #960
	lsrs	r1, r2, #32
	add	r7, sp, #984
	lsrs	r1, r2, #32
	add	sp, #40
	lsrs	r1, r2, #32
	add	sp, #40
	lsrs	r1, r2, #32
	add	sp, #40
	lsrs	r1, r2, #32
	add	sp, #40
	lsrs	r1, r2, #32
	add	sp, #40
	lsrs	r1, r2, #32
	add	r7, sp, #376
	lsrs	r1, r2, #32
	add	r7, sp, #232
	lsrs	r1, r2, #32
	add	r7, sp, #400
	lsrs	r1, r2, #32
	add	r7, sp, #424
	lsrs	r1, r2, #32
	add	r7, sp, #448
	lsrs	r1, r2, #32
	add	r7, sp, #472
	lsrs	r1, r2, #32
	add	r7, sp, #496
	lsrs	r1, r2, #32
	add	r7, sp, #520
	lsrs	r1, r2, #32
	add	r7, sp, #544
	lsrs	r1, r2, #32
	add	r7, sp, #568
	lsrs	r1, r2, #32
	add	r7, sp, #592
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #128
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #128
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #128
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #16
	lsrs	r1, r2, #32
	sub	sp, #16
	lsrs	r1, r2, #32
	sub	sp, #16
	lsrs	r1, r2, #32
	sub	sp, #16
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	sub	sp, #160
	lsrs	r1, r2, #32
	add	r7, sp, #312
	lsrs	r1, r2, #32
	add	r7, sp, #336
	lsrs	r1, r2, #32
	movs	r3, #128
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #128
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #128
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #128
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #134
	lsls	r3, r3, #9
	adds	r3, #204
	b.n	.L_0811b0a6
	movs	r3, #243
	lsls	r3, r3, #8
	adds	r3, #51
	b.n	.L_0811b0a6
	movs	r3, #128
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #230
	lsls	r3, r3, #8
	b.n	.L_0811b0a4
	movs	r3, #230
	lsls	r3, r3, #8
	b.n	.L_0811b0a4
	movs	r3, #230
	lsls	r3, r3, #8
	b.n	.L_0811b0a4
	movs	r3, #230
	lsls	r3, r3, #8
	b.n	.L_0811b0a4
	movs	r3, #230
	lsls	r3, r3, #8
	b.n	.L_0811b0a4
	ldr	r3, [pc, #368]
	b.n	.L_0811b0a6
	movs	r3, #198
	lsls	r3, r3, #9
	adds	r3, #204
	b.n	.L_0811b0a6
	ldr	r3, [pc, #360]
	b.n	.L_0811b0a6
	ldr	r3, [pc, #352]
	b.n	.L_0811b0a6
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #230
	lsls	r3, r3, #9
	adds	r3, #204
	b.n	.L_0811b0a6
	movs	r3, #230
	lsls	r3, r3, #9
	adds	r3, #204
	b.n	.L_0811b0a6
	movs	r3, #128
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #230
	lsls	r3, r3, #8
	b.n	.L_0811b0a4
	movs	r3, #128
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #230
	lsls	r3, r3, #8
	b.n	.L_0811b0a4
	ldr	r3, [pc, #312]
	b.n	.L_0811b0a6
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	ldr	r3, [pc, #244]
	b.n	.L_0811b0a6
	ldr	r3, [pc, #240]
	b.n	.L_0811b0a6
	movs	r3, #128
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #160
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #230
	lsls	r3, r3, #8
	b.n	.L_0811b0a4
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #166
	lsls	r3, r3, #9
	adds	r3, #204
	b.n	.L_0811b0a6
	ldr	r3, [pc, #204]
	b.n	.L_0811b0a6
	movs	r3, #134
	lsls	r3, r3, #9
	adds	r3, #204
	b.n	.L_0811b0a6
	movs	r3, #134
	lsls	r3, r3, #9
	adds	r3, #204
	b.n	.L_0811b0a6
	ldr	r3, [pc, #192]
	b.n	.L_0811b0a6
	ldr	r3, [pc, #188]
	b.n	.L_0811b0a6
	ldr	r3, [pc, #176]
	b.n	.L_0811b0a6
	ldr	r3, [pc, #172]
	b.n	.L_0811b0a6
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	ldr	r3, [pc, #160]
	b.n	.L_0811b0a6
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #198
	lsls	r3, r3, #9
	adds	r3, #204
	b.n	.L_0811b0a6
	movs	r3, #160
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #160
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #230
	lsls	r3, r3, #8
	b.n	.L_0811b0a4
	movs	r3, #243
	lsls	r3, r3, #8
	adds	r3, #51
	b.n	.L_0811b0a6
	movs	r3, #166
	lsls	r3, r3, #9
	adds	r3, #204
	b.n	.L_0811b0a6
	ldr	r3, [pc, #120]
	b.n	.L_0811b0a6
	ldr	r3, [pc, #112]
	b.n	.L_0811b0a6
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #192
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	ldr	r3, [pc, #96]
	b.n	.L_0811b0a6
	movs	r3, #147
	b.n	.L_0811b0a2
	ldr	r3, [pc, #80]
	b.n	.L_0811b0a6
	movs	r3, #160
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #160
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #192
	lsls	r3, r3, #8
	b.n	.L_0811b0a6
	movs	r3, #215
	lsls	r3, r3, #1
	adds	r3, #255
	strh	r3, [r7, #10]
	movs	r3, #147
	b.n	.L_0811b0a2
	movs	r3, #129
	lsls	r3, r3, #9
	adds	r3, #143
	b.n	.L_0811b0a6
	movs	r3, #160
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	ldr	r3, [pc, #40]
	b.n	.L_0811b0a6
	movs	r3, #160
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #128
	lsls	r3, r3, #9
	b.n	.L_0811b0a6
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	b.n	.L_0811b0a6
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	b.n	.L_0811b0a6
	.4byte 0x00019999
	.4byte 0x00013333
	.4byte 0x0001b333
	.4byte 0x00011999
	.2byte 0x7333
	.2byte 0x0001
	movs	r3, #179
.L_0811b0a2:
	lsls	r3, r3, #9
.L_0811b0a4:
	adds	r3, #102
.L_0811b0a6:
	str	r3, [r7, #24]
.L_0811b0a8:
	mov	r3, r8
	str	r3, [r7, #0]
	ldr	r2, [sp, #8]
	mov	r0, r9
	str	r2, [r7, #12]
	ldr	r3, [sp, #4]
	mov	r2, fp
	str	r3, [r7, #16]
	str	r2, [r7, #20]
	strh	r1, [r7, #4]
	bl	.L_0811a674
	movs	r5, #0
	ldr	r6, [pc, #48]
	strh	r5, [r7, #8]
	str	r5, [r7, #32]
	str	r5, [r7, #36]
	strh	r5, [r7, #40]
	adds	r3, r7, #0
	movs	r5, #165
	adds	r3, #42
	lsls	r5, r5, #1
	strh	r0, [r7, #6]
	add	r5, sl
	strb	r6, [r3, #0]
	adds	r3, #1
	strb	r6, [r3, #0]
	ldrh	r3, [r5, #0]
	cmp	r3, #1
	bhi.n	.L_0811b10e
	mov	r0, sl
	movs	r1, #1
	bl	sub_080ad070
	cmp	r0, #15
	bne.n	.L_0811b10e
	b.n	.L_0811b0f8
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_0811b0f8:
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_0811b102
	movs	r1, #190
	b.n	.L_0811b104
.L_0811b102:
	movs	r1, #191
.L_0811b104:
	lsls	r1, r1, #1
	adds	r1, #255
	strh	r1, [r7, #4]
	movs	r3, #0
	strh	r3, [r7, #6]
.L_0811b10e:
	ldr	r3, [sp, #12]
	cmp	r3, #0
	bge.n	.L_0811b116
	adds	r3, #7
.L_0811b116:
	asrs	r0, r3, #3
	ldr	r1, [sp, #16]
	bl	sub_080148e8
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r0, r0, r3
	mov	r2, r8
	movs	r3, #3
	strh	r0, [r2, #6]
	adds	r2, #89
	strb	r3, [r2, #0]
	movs	r3, #2
	subs	r2, #4
	strb	r3, [r2, #0]
	movs	r3, #42
	adds	r3, #255
	add	r3, sl
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0811b148
	movs	r3, #166
	lsls	r3, r3, #9
	adds	r3, #204
	b.n	.L_0811b14c
.L_0811b148:
	movs	r3, #128
	lsls	r3, r3, #9
.L_0811b14c:
	mov	r2, r8
	str	r3, [r2, #24]
	str	r3, [r2, #28]
	ldr	r1, [pc, #20]
	mov	r0, r8
	bl	sub_080200a8
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0812cacc
