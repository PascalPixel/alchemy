.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.set sub_08038040, 0x08038040
	.set sub_08038120, 0x08038120
	.set sub_08038140, 0x08038140
	.set sub_080ad078, 0x080ad078
	.set sub_080ad0c8, 0x080ad0c8
	.set sub_080cad84, 0x080cad84
	.set sub_080ccd48, 0x080ccd48
	.set sub_080cdf5c, 0x080cdf5c
	.set sub_080ce31c, 0x080ce31c
	.set sub_080ceb34, 0x080ceb34
	.set sub_080d2260, 0x080d2260
	.set sub_080d22a8, 0x080d22a8
	.set sub_080d2350, 0x080d2350
	.set sub_080d295c, 0x080d295c
	.set sub_080d3be8, 0x080d3be8
	.set sub_080d407c, 0x080d407c
	.set sub_080db4b8, 0x080db4b8
	.set sub_080db670, 0x080db670
	.set sub_080db848, 0x080db848
	.set sub_080db884, 0x080db884
	.set sub_080dbcd8, 0x080dbcd8
	.set sub_080dc410, 0x080dc410
	.set sub_080dc62c, 0x080dc62c
	.set sub_080dc6d8, 0x080dc6d8
	.set sub_080dc7cc, 0x080dc7cc
	.set sub_080dc7e8, 0x080dc7e8
	.global Func_080ce458
	.thumb_func
Func_080ce458:
.L_080ce458:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r1, [sp, #8]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r6, r2, #0
	ldr	r5, [r3, #16]
	mov	r9, r0
	bl	sub_080cdf5c
	bl	sub_080cad84
	ldrh	r0, [r0, #6]
	movs	r3, #1
	negs	r3, r3
	str	r0, [sp, #4]
	cmp	r6, r3
	beq.n	.L_080ce564
	ldr	r0, [r5, #0]
	cmp	r0, r3
	beq.n	.L_080ce564
	movs	r2, #255
	mov	fp, r2
	mov	sl, fp
	mov	r3, sl
	movs	r4, #255
	lsls	r4, r4, #8
	ands	r3, r6
	ands	r4, r6
	mov	sl, r3
.L_080ce4a2:
	movs	r3, #4
	ldrsh	r6, [r5, r3]
	movs	r3, #240
	lsls	r3, r3, #8
	ldrh	r2, [r5, #4]
	ands	r6, r3
	ldr	r3, [pc, #44]
	lsls	r0, r0, #16
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	mov	r7, fp
	lsrs	r0, r0, #24
	mov	r8, r3
	ands	r7, r2
	str	r4, [sp, #0]
	bl	sub_080ad078
	ldr	r3, [r5, #0]
	movs	r2, #15
	ands	r3, r2
	ldrb	r1, [r0, #6]
	ldr	r4, [sp, #0]
	cmp	r3, #5
	bne.n	.L_080ce556
	ldr	r2, [sp, #8]
	cmp	r1, r2
	bne.n	.L_080ce556
	b.n	.L_080ce4e0
	.2byte 0x0800
	.2byte 0x0000
.L_080ce4e0:
	movs	r3, #6
	ldrsh	r0, [r5, r3]
	bl	sub_080ccd48
	ldr	r4, [sp, #0]
	cmp	r0, #0
	beq.n	.L_080ce556
	mov	r2, r8
	cmp	r2, #0
	beq.n	.L_080ce50c
	ldr	r2, [sp, #4]
	subs	r3, r6, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, #0
	bge.n	.L_080ce502
	negs	r3, r3
.L_080ce502:
	movs	r2, #184
	lsls	r2, r2, #5
	adds	r2, #255
	cmp	r3, r2
	bgt.n	.L_080ce556
.L_080ce50c:
	movs	r3, #240
	lsls	r3, r3, #24
	adds	r3, #5
	cmp	r9, r3
	beq.n	.L_080ce522
	ldr	r0, [r5, #0]
	adds	r3, #10
	ands	r3, r0
	cmp	r3, r9
	bne.n	.L_080ce556
	b.n	.L_080ce524
.L_080ce522:
	ldr	r0, [r5, #0]
.L_080ce524:
	movs	r3, #128
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080ce538
	movs	r2, #128
	lsls	r2, r2, #2
	cmp	r4, r2
	bne.n	.L_080ce556
.L_080ce534:
	adds	r0, r5, #0
	b.n	.L_080ce566
.L_080ce538:
	movs	r3, #16
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080ce54e
	movs	r3, #128
	lsls	r3, r3, #1
	cmp	r4, r3
	bne.n	.L_080ce556
	cmp	sl, r7
	bne.n	.L_080ce556
	b.n	.L_080ce534
.L_080ce54e:
	cmp	r4, #0
	bne.n	.L_080ce556
	cmp	sl, r7
	beq.n	.L_080ce534
.L_080ce556:
	adds	r5, #12
	ldr	r3, [r5, #0]
	movs	r2, #1
	negs	r2, r2
	adds	r0, r3, #0
	cmp	r3, r2
	bne.n	.L_080ce4a2
.L_080ce564:
	movs	r0, #0
.L_080ce566:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
.L_080ce574:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r3, r0
	mov	r8, r3
	lsrs	r7, r0, #10
	movs	r3, #15
	mov	r0, r8
	ands	r7, r3
	bl	sub_080ad078
	ldrb	r5, [r0, #6]
	adds	r0, r5, #0
	bl	sub_080ce31c
	adds	r6, r0, #0
	movs	r0, #192
	lsls	r0, r0, #22
	adds	r1, r5, #0
	adds	r2, r6, #0
	adds	r0, #5
	bl	.L_080ce458
	mov	sl, r0
	movs	r0, #128
	lsls	r0, r0, #22
	adds	r1, r5, #0
	adds	r0, #5
	adds	r2, r6, #0
	bl	.L_080ce458
	movs	r5, #1
	mov	r3, sl
	mov	r9, r0
	negs	r5, r5
	cmp	r3, #0
	bne.n	.L_080ce5cc
	cmp	r0, #0
	beq.n	.L_080ce5de
.L_080ce5cc:
	cmp	r6, r5
	beq.n	.L_080ce5de
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_080ce5de
	movs	r5, #255
	ands	r5, r6
.L_080ce5de:
	mov	r0, r8
	movs	r1, #0
	bl	sub_080dc410
	bl	sub_080cdf5c
	adds	r1, r5, #0
	bl	sub_080dc62c
	adds	r1, r7, #0
	adds	r2, r5, #0
	mov	r0, sl
	bl	.L_080ceafc
	bl	sub_080db884
	bl	sub_080dc7cc
	adds	r1, r7, #0
	adds	r2, r5, #0
	mov	r0, r9
	bl	.L_080ceafc
	bl	sub_080dc7e8
	movs	r0, #0
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	movs	r0, #192
	movs	r3, #192
	lsls	r0, r0, #2
	adds	r0, #255
	lsls	r3, r3, #18
	mov	r1, r8
	ands	r1, r0
	ldr	r3, [r3, #108]
	mov	r9, r1
	sub	sp, #16
	mov	r0, r9
	str	r3, [sp, #12]
	bl	sub_080ad078
	mov	r2, r8
	ldrb	r0, [r0, #6]
	lsrs	r7, r2, #10
	movs	r3, #15
	ands	r7, r3
	movs	r3, #0
	str	r0, [sp, #4]
	str	r3, [sp, #0]
	bl	sub_080d2260
	ldr	r0, [sp, #12]
	movs	r1, #211
	lsls	r1, r1, #4
	adds	r2, r0, r1
	movs	r3, #255
	strb	r3, [r2, #0]
	movs	r2, #208
	lsls	r2, r2, #4
	adds	r2, #72
	adds	r3, r0, r2
	mov	r0, sp
	ldrh	r0, [r0, #0]
	strh	r0, [r3, #0]
	movs	r0, #70
	adds	r0, #255
	bl	sub_08016d18
	cmp	r7, #15
	bne.n	.L_080ce684
	movs	r7, #0
.L_080ce684:
	movs	r0, #191
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ce6aa
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08038120
	mov	r0, r9
	movs	r1, #4
	bl	sub_08038120
	ldr	r0, [pc, #604]
	movs	r1, #1
	bl	sub_08038040
	b.n	.L_080ceaec
.L_080ce6aa:
	ldr	r1, [sp, #12]
	movs	r2, #197
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080ce736
	mov	r3, r9
	cmp	r3, #144
	beq.n	.L_080ce6c6
	cmp	r3, #155
	bne.n	.L_080ce6e0
.L_080ce6c6:
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08038120
	mov	r0, r9
	movs	r1, #4
	bl	sub_08038120
	ldr	r0, [pc, #552]
	movs	r1, #1
	bl	sub_08038040
	b.n	.L_080ceaec
.L_080ce6e0:
	mov	r0, r9
	cmp	r0, #153
	bne.n	.L_080ce736
	ldr	r3, [pc, #540]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #118
	adds	r3, r3, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_080ce71c
	movs	r0, #252
	lsls	r0, r0, #3
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ce71c
	movs	r0, #72
	adds	r0, #255
	bl	sub_08016cfc
	ldr	r3, [sp, #12]
	movs	r0, #181
	lsls	r0, r0, #1
	adds	r2, r3, r0
	movs	r3, #253
	strh	r3, [r2, #0]
	b.n	.L_080ceaec
.L_080ce71c:
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08038120
	mov	r0, r9
	movs	r1, #4
	bl	sub_08038120
	ldr	r0, [pc, #464]
	movs	r1, #1
	bl	sub_08038040
	b.n	.L_080ceaec
.L_080ce736:
	ldr	r3, [pc, #460]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #118
	adds	r3, r3, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_080ce768
	mov	r3, r9
	cmp	r3, #139
	bne.n	.L_080ce768
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08038120
	movs	r0, #139
	movs	r1, #4
	bl	sub_08038120
	ldr	r0, [pc, #416]
	movs	r1, #1
	bl	sub_08038040
	b.n	.L_080ceaec
.L_080ce768:
	mov	r0, r9
	cmp	r0, #149
	bne.n	.L_080ce7ee
	movs	r0, #162
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ce794
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08038120
	movs	r0, #149
	movs	r1, #4
	bl	sub_08038120
	ldr	r0, [pc, #380]
	movs	r1, #1
	bl	sub_08038040
	b.n	.L_080ceaec
.L_080ce794:
	movs	r0, #149
	movs	r1, #4
	bl	sub_08038120
	movs	r1, #13
	ldr	r0, [pc, #364]
	bl	sub_08038040
	movs	r0, #1
	bl	sub_080d295c
	adds	r5, r0, #0
	bl	sub_08038140
	movs	r0, #0
	cmp	r5, #0
	beq.n	.L_080ce7b8
	b.n	.L_080ceaee
.L_080ce7b8:
	ldr	r1, [pc, #328]
	movs	r2, #152
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldrh	r2, [r3, #0]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r1, r0
	strh	r2, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #98
	adds	r3, r1, r2
	ldrh	r3, [r3, #0]
	adds	r0, #2
	adds	r2, r1, r0
	strh	r3, [r2, #0]
	ldr	r1, [sp, #12]
	movs	r3, #172
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	movs	r0, #1
	strh	r3, [r2, #0]
	str	r0, [sp, #0]
.L_080ce7ee:
	ldr	r0, [sp, #4]
	bl	sub_080ce31c
	adds	r6, r0, #0
	movs	r0, #128
	lsls	r0, r0, #24
	adds	r0, #5
	ldr	r1, [sp, #4]
	adds	r2, r6, #0
	bl	.L_080ce458
	cmp	r0, #0
	beq.n	.L_080ce82e
	movs	r1, #1
	negs	r1, r1
	mov	sl, r1
	cmp	r6, sl
	beq.n	.L_080ce826
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_080ce826
	movs	r2, #255
	mov	sl, r2
	mov	r3, sl
	ands	r3, r6
	mov	sl, r3
.L_080ce826:
	adds	r1, r7, #0
	mov	r2, sl
	bl	.L_080ceafc
.L_080ce82e:
	movs	r3, #128
	lsls	r3, r3, #6
	mov	r0, r8
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080ce840
	bl	.L_080ce574
	b.n	.L_080ceaee
.L_080ce840:
	ldr	r0, [sp, #4]
	bl	sub_080ce31c
	adds	r6, r0, #0
	movs	r0, #128
	lsls	r0, r0, #21
	ldr	r1, [sp, #4]
	adds	r2, r6, #0
	adds	r0, #5
	bl	.L_080ce458
	adds	r2, r6, #0
	str	r0, [sp, #8]
	ldr	r1, [sp, #4]
	movs	r0, #5
	bl	.L_080ce458
	mov	r8, r0
	movs	r0, #160
	lsls	r0, r0, #23
	ldr	r1, [sp, #4]
	adds	r0, #5
	adds	r2, r6, #0
	bl	.L_080ce458
	mov	r1, r9
	mov	fp, r0
	cmp	r1, #156
	bne.n	.L_080ce8b8
	ldr	r3, [pc, #136]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #136]
	cmp	r2, r3
	beq.n	.L_080ce8b8
	ldr	r1, [sp, #8]
	cmp	r1, #0
	bne.n	.L_080ce8b8
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_080ce8b8
	mov	r3, fp
	cmp	r3, #0
	bne.n	.L_080ce8b8
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08038120
	movs	r0, #156
	movs	r1, #4
	bl	sub_08038120
	ldr	r0, [pc, #88]
	movs	r1, #1
	bl	sub_08038040
	b.n	.L_080ceaec
.L_080ce8b8:
	cmp	r7, #7
	bgt.n	.L_080ce920
	mov	r0, r9
	bl	sub_080ad078
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	sub_08016ca4
	ldrb	r1, [r5, #9]
	movs	r2, #58
	ldrsh	r3, [r0, r2]
	cmp	r3, r1
	bge.n	.L_080ce918
	ldr	r3, [sp, #0]
	cmp	r3, #0
	beq.n	.L_080ce8e6
	ldr	r0, [sp, #12]
	movs	r1, #172
	lsls	r1, r1, #1
	adds	r2, r0, r1
	movs	r3, #0
	strh	r3, [r2, #0]
.L_080ce8e6:
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08038120
	mov	r0, r9
	movs	r1, #4
	bl	sub_08038120
	ldr	r0, [pc, #28]
	movs	r1, #1
	bl	sub_08038040
	b.n	.L_080ceaec
	.4byte 0x00000dbb
	.4byte 0x02000240
	.4byte 0x00000dbd
	.4byte 0x00000dbc
	.4byte 0x00000002
	.2byte 0x0dba
	.2byte 0x0000
.L_080ce918:
	negs	r1, r1
	adds	r0, r7, #0
	bl	sub_080ad0c8
.L_080ce920:
	ldr	r2, [sp, #4]
	cmp	r2, #23
	bne.n	.L_080ce948
	bl	sub_080cdf5c
	bl	sub_080cad84
	ldr	r3, [r0, #8]
	ldr	r1, [r0, #16]
	adds	r0, #34
	ldrb	r2, [r0, #0]
	adds	r0, r3, #0
	bl	sub_080dbcd8
	cmp	r0, #0
	bne.n	.L_080ce948
	movs	r3, #0
	str	r3, [sp, #8]
	mov	r8, r3
	mov	fp, r3
.L_080ce948:
	movs	r0, #160
	lsls	r0, r0, #1
	bl	sub_08016cfc
	movs	r0, #66
	adds	r0, #255
	bl	sub_08016d18
	ldr	r1, [sp, #8]
	movs	r0, #1
	negs	r0, r0
	mov	sl, r0
	cmp	r1, #0
	bne.n	.L_080ce970
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_080ce970
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L_080ce9b6
.L_080ce970:
	movs	r0, #1
	negs	r0, r0
	cmp	r6, r0
	beq.n	.L_080ce98c
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_080ce98c
	movs	r1, #255
	mov	sl, r1
	mov	r2, sl
	ands	r2, r6
	mov	sl, r2
.L_080ce98c:
	movs	r5, #66
	adds	r5, #255
	adds	r0, r5, #0
	bl	sub_08016cfc
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_080ce9b6
	ldrh	r2, [r3, #4]
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ce9b6
	movs	r0, #160
	lsls	r0, r0, #1
	bl	sub_08016d18
	adds	r0, r5, #0
	bl	sub_08016d18
.L_080ce9b6:
	mov	r0, r9
	movs	r1, #0
	bl	sub_080dc410
	movs	r1, #192
	ldr	r0, [sp, #12]
	lsls	r1, r1, #4
	adds	r1, #182
	adds	r2, r0, r1
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r2, [sp, #8]
	movs	r1, #0
	cmp	r2, #0
	beq.n	.L_080ce9f2
	ldrh	r2, [r2, #4]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ce9f2
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	adds	r2, r3, #0
	adds	r2, #34
	adds	r3, #35
	strb	r1, [r2, #0]
	strb	r1, [r3, #0]
.L_080ce9f2:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_080cea18
	ldrh	r2, [r3, #4]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cea18
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r2, #0
	adds	r1, r3, #0
	adds	r1, #34
	adds	r3, #35
	strb	r2, [r1, #0]
	strb	r2, [r3, #0]
.L_080cea18:
	mov	r0, fp
	cmp	r0, #0
	beq.n	.L_080cea3e
	ldrh	r2, [r0, #4]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cea3e
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r2, #0
	adds	r1, r3, #0
	adds	r1, #34
	adds	r3, #35
	strb	r2, [r1, #0]
	strb	r2, [r3, #0]
.L_080cea3e:
	bl	sub_080cdf5c
	mov	r1, sl
	bl	sub_080dc62c
	bl	sub_080dc6d8
	adds	r1, r7, #0
	ldr	r0, [sp, #8]
	mov	r2, sl
	bl	.L_080ceafc
	movs	r0, #160
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080cea78
	movs	r0, #66
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080cea74
	bl	sub_080db670
	b.n	.L_080cea78
.L_080cea74:
	bl	sub_080db4b8
.L_080cea78:
	bl	sub_080dc7cc
	ldr	r1, [sp, #12]
	movs	r2, #211
	lsls	r2, r2, #4
	adds	r5, r1, r2
	movs	r2, #0
	ldrsb	r2, [r5, r2]
	movs	r3, #1
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L_080ceab0
	movs	r3, #128
	movs	r0, #128
	lsls	r3, r3, #1
	lsls	r0, r0, #22
	orrs	r2, r3
	adds	r0, #5
	ldr	r1, [sp, #4]
	bl	.L_080ce458
	cmp	r0, #0
	beq.n	.L_080ceab0
	movs	r2, #0
	ldrsb	r2, [r5, r2]
	adds	r1, r7, #0
	bl	.L_080ceafc
.L_080ceab0:
	movs	r5, #160
	mov	r0, r8
	adds	r1, r7, #0
	mov	r2, sl
	lsls	r5, r5, #1
	bl	.L_080ceafc
	adds	r0, r5, #0
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ceacc
	bl	sub_080db848
.L_080ceacc:
	ldr	r0, [sp, #12]
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #182
	adds	r3, r0, r1
	movs	r2, #0
	strb	r2, [r3, #0]
	bl	sub_080dc7e8
	adds	r0, r5, #0
	bl	sub_08016d18
	movs	r0, #66
	adds	r0, #255
	bl	sub_08016d18
.L_080ceaec:
	movs	r0, #0
.L_080ceaee:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
.L_080ceafc:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r2, #0
	adds	r0, r1, #0
	cmp	r5, #0
	beq.n	.L_080ceb50
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq.n	sub_080ceb34
	movs	r2, #128
	lsls	r2, r2, #9
	cmp	r3, r2
	bge.n	.L_080ceb2e
	bl	sub_080d22a8
	ldr	r0, [r5, #8]
	bl	sub_080d3be8
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_080d407c
	bl	sub_080d2350
	b.n	sub_080ceb34
.L_080ceb2e:
	adds	r1, r6, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x20a1
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ceb50
	bl	sub_080d22a8
	ldr	r0, [pc, #12]
	movs	r1, #1
	bl	sub_08038040
	bl	sub_080d2350
.L_080ceb50:
	movs	r0, #0
	pop	{r5, r6, pc}
	.2byte 0x0dc3
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	lsls	r5, r5, #16
	lsrs	r5, r5, #16
	adds	r0, r5, #0
	bl	sub_080ce31c
	adds	r2, r0, #0
	movs	r0, #240
	lsls	r0, r0, #24
	adds	r1, r5, #0
	adds	r0, #5
	bl	.L_080ce458
	adds	r3, r0, #0
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	pop	{r5, pc}
	.align 2, 0
