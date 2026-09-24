.syntax unified
	.thumb
	.set sub_08002f40, 0x08002f40
	.set sub_080041d8, 0x080041d8
	.set sub_08004458, 0x08004458
	.set sub_080072f0, 0x080072f0
	.set sub_080b5098, 0x080b5098
	.set sub_080cb17a, 0x080cb17a
	.set sub_080cd594, 0x080cd594
	.set sub_080e0524, 0x080e0524
	.set sub_080e396c, 0x080e396c
	.set sub_080e3980, 0x080e3980
	.global BattleFx_RunTwelveMode
	.global Func_080ca60c
	.thumb_func
BattleFx_RunTwelveMode:
Func_080ca60c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #156
	ldr	r2, [pc, #40]
	str	r1, [sp, #96]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	str	r3, [sp, #92]
	ldr	r3, [r2, #8]
	str	r3, [sp, #80]
	subs	r2, #108
	ldr	r3, [pc, #28]
	mov	fp, r1
	ldr	r2, [r2, #0]
	add	r3, fp
	str	r2, [sp, #76]
	str	r0, [r3, #0]
	ldr	r5, [sp, #96]
	cmp	r5, #8
	bne.n	.L_080ca650
	movs	r0, #0
	bl	sub_080cd594
	b.n	.L_080ca656
	.4byte 0x03001eec
	.2byte 0x7828
	.2byte 0x0000
.L_080ca650:
	movs	r0, #1
	bl	sub_080cd594
.L_080ca656:
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r1, [sp, #80]
	ldr	r0, [pc, #60]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [pc, #52]
	mov	r1, fp
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [sp, #96]
	ldr	r1, [sp, #96]
	lsls	r0, r0, #3
	ldr	r2, [pc, #40]
	str	r0, [sp, #72]
	subs	r3, r0, r1
	ldrb	r3, [r2, r3]
	cmp	r3, #0
	bne.n	.L_080ca6b4
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #28]
	add	r1, fp
	movs	r2, #0
	b.n	.L_080ca6ac
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x04000052
	.4byte 0x00000073
	.4byte 0x000000ce
	.4byte 0x080edf04
	.2byte 0x00c5
	.2byte 0x0000
.L_080ca6ac:
	movs	r3, #0
	bl	sub_080e0524
	b.n	.L_080ca6c4
.L_080ca6b4:
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #844]
	add	r1, fp
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
.L_080ca6c4:
	ldr	r5, [sp, #72]
	ldr	r0, [sp, #96]
	ldr	r2, [pc, #832]
	subs	r3, r5, r0
	adds	r3, #3
	ldrb	r3, [r2, r3]
	cmp	r3, #5
	bhi.n	.L_080ca708
	ldr	r2, [pc, #824]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	add	r6, pc, #976
	lsrs	r4, r1, #32
	add	r6, pc, #992
	lsrs	r4, r1, #32
	add	r6, pc, #1008
	lsrs	r4, r1, #32
	add	r7, pc, #0
	lsrs	r4, r1, #32
	add	r7, pc, #16
	lsrs	r4, r1, #32
	add	r7, pc, #32
	lsrs	r4, r1, #32
	ldr	r0, [pc, #796]
	b.n	.L_080ca70a
	ldr	r0, [pc, #796]
	b.n	.L_080ca70a
	ldr	r0, [pc, #796]
	b.n	.L_080ca70a
	ldr	r0, [pc, #796]
	b.n	.L_080ca70a
	ldr	r0, [pc, #796]
	b.n	.L_080ca70a
.L_080ca708:
	ldr	r0, [pc, #796]
.L_080ca70a:
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #792]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	movs	r1, #200
	lsls	r1, r1, #6
	add	r1, fp
	movs	r2, #1
	movs	r3, #0
	ldr	r0, [pc, #776]
	bl	sub_080e0524
	mov	r2, sp
	movs	r1, #0
	adds	r2, #144
	movs	r3, #36
	str	r1, [sp, #84]
	str	r2, [sp, #68]
	str	r1, [sp, #16]
	str	r3, [sp, #12]
.L_080ca73c:
	ldr	r3, [pc, #756]
	add	r3, fp
	ldr	r5, [sp, #12]
	ldr	r3, [r3, #0]
	ldrsh	r0, [r3, r5]
	bl	sub_080b5098
	movs	r5, #225
	ldr	r0, [r0, #0]
	movs	r2, #0
	lsls	r5, r5, #7
	mov	sl, r0
	mov	r8, r2
	add	r5, fp
.L_080ca758:
	ldr	r3, [pc, #728]
	add	r3, fp
	ldr	r1, [sp, #12]
	ldr	r3, [r3, #0]
	ldrsh	r0, [r3, r1]
	ldr	r1, [sp, #68]
	bl	sub_080e3980
	ldr	r0, [sp, #68]
	ldr	r3, [r0, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	movs	r3, #160
	lsls	r3, r3, #15
	movs	r1, #0
	str	r3, [r5, #4]
	str	r1, [r5, #8]
	bl	sub_08004458
	movs	r2, #255
	ands	r0, r2
	subs	r0, #128
	lsls	r0, r0, #9
	str	r0, [r5, #12]
	bl	sub_08004458
	movs	r3, #255
	ands	r0, r3
	subs	r0, #128
	movs	r1, #1
	lsls	r0, r0, #9
	movs	r3, #1
	add	r8, r1
	str	r0, [r5, #16]
	negs	r3, r3
	movs	r0, #0
	mov	r2, r8
	str	r0, [r5, #20]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #64
	bne.n	.L_080ca758
	mov	r8, r0
	ldr	r3, [sp, #16]
	ldr	r0, [pc, #640]
	movs	r6, #255
	adds	r5, r3, r0
.L_080ca7bc:
	mov	r1, sl
	ldr	r3, [r1, #8]
	str	r3, [r5, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r5, #4]
	ldr	r3, [r1, #16]
	str	r3, [r5, #8]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	sub_08004458
	movs	r3, #1
	ands	r0, r6
	movs	r2, #1
	negs	r3, r3
	subs	r0, #128
	add	r8, r2
	str	r3, [r5, #24]
	lsls	r0, r0, #11
	mov	r3, r8
	str	r0, [r5, #20]
	adds	r5, #28
	cmp	r3, #128
	bne.n	.L_080ca7bc
	ldr	r0, [sp, #72]
	ldr	r1, [sp, #96]
	subs	r0, r0, r1
	str	r0, [sp, #64]
	ldr	r2, [pc, #560]
	adds	r0, #2
	movs	r5, #0
	str	r0, [sp, #60]
	ldr	r6, [pc, #556]
	mov	r8, r5
	movs	r7, #255
	mov	r9, r2
.L_080ca81a:
	mov	r5, sl
	ldr	r3, [r5, #8]
	str	r3, [r6, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	str	r3, [r6, #8]
	ldr	r0, [pc, #480]
	ldr	r1, [sp, #64]
	ldrb	r3, [r0, r1]
	cmp	r3, #1
	bne.n	.L_080ca85a
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r6, #12]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r6, #16]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r6, #20]
	b.n	.L_080ca8be
.L_080ca85a:
	ldr	r3, [pc, #432]
	ldr	r5, [sp, #60]
	ldrb	r2, [r3, r5]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ca890
	bl	sub_08004458
	mov	r1, r9
	ands	r0, r1
	lsls	r0, r0, #11
	str	r0, [r6, #12]
	bl	sub_08004458
	ldr	r5, [pc, #456]
	mov	r2, r9
	ands	r0, r2
	adds	r0, r0, r5
	lsls	r0, r0, #11
	str	r0, [r6, #16]
	bl	sub_08004458
	mov	r3, r9
	ands	r0, r3
	adds	r0, r0, r5
	b.n	.L_080ca8ae
.L_080ca890:
	bl	sub_08004458
	ands	r0, r7
	lsls	r0, r0, #11
	str	r0, [r6, #12]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r6, #16]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #128
.L_080ca8ae:
	lsls	r0, r0, #11
	str	r0, [r6, #20]
	ldr	r3, [r6, #0]
	cmp	r3, #0
	ble.n	.L_080ca8be
	ldr	r3, [r6, #12]
	negs	r3, r3
	str	r3, [r6, #12]
.L_080ca8be:
	movs	r3, #1
	movs	r5, #1
	movs	r0, #128
	negs	r3, r3
	add	r8, r5
	lsls	r0, r0, #2
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r8, r0
	bne.n	.L_080ca81a
	ldr	r1, [sp, #16]
	ldr	r3, [sp, #12]
	ldr	r5, [sp, #84]
	movs	r2, #224
	lsls	r2, r2, #4
	adds	r1, r1, r2
	adds	r3, #2
	adds	r5, #1
	str	r1, [sp, #16]
	str	r3, [sp, #12]
	str	r5, [sp, #84]
	cmp	r5, #1
	beq.n	.L_080ca8ee
	b.n	.L_080ca73c
.L_080ca8ee:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #332]
	movs	r3, #75
	add	r2, fp
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #324]
	bl	sub_080041d8
	ldr	r5, [pc, #296]
	add	r5, fp
	ldr	r3, [r5, #0]
	mov	r2, sp
	adds	r2, #132
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	adds	r1, r2, #0
	str	r2, [sp, #56]
	bl	sub_080e396c
	ldr	r3, [r5, #0]
	movs	r5, #36
	ldrsh	r0, [r3, r5]
	add	r5, sp, #120
	adds	r1, r5, #0
	bl	sub_080e3980
	ldr	r0, [sp, #56]
	ldr	r3, [r5, #4]
	ldr	r1, [r0, #4]
	subs	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r1, r1, r3
	str	r1, [r0, #4]
	ldr	r3, [sp, #96]
	ldr	r2, [sp, #72]
	movs	r1, #0
	ldr	r7, [pc, #196]
	str	r1, [sp, #88]
	subs	r1, r2, r3
	adds	r3, r1, #6
	adds	r5, r7, #0
	ldrb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L_080ca95a
	bl	sub_080cb17a
.L_080ca95a:
	ldr	r0, [sp, #76]
	ldr	r5, [pc, #212]
	adds	r0, #12
	str	r0, [sp, #20]
	mov	r3, sp
	adds	r0, r1, #0
	adds	r3, #100
	add	r5, fp
	str	r1, [sp, #44]
	adds	r0, #4
	adds	r1, #2
	str	r3, [sp, #24]
	str	r5, [sp, #48]
	str	r0, [sp, #40]
	str	r1, [sp, #36]
