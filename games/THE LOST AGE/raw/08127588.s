.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_08013164, 0x08013164
	.set sub_08014878, 0x08014878
	.set sub_08014dac, 0x08014dac
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.set sub_08016d5c, 0x08016d5c
	.set sub_080ad008, 0x080ad008
	.set sub_080ad140, 0x080ad140
	.set sub_0811a038, 0x0811a038
	.global Overlay_08127588
Overlay_08127588:
.L_08127588:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r2, #0
	mov	sl, r2
	mov	r9, r2
	lsls	r3, r0, #1
	ldr	r2, [pc, #172]
	adds	r3, r3, r0
	lsls	r3, r3, #3
	adds	r1, r3, r2
	ldrb	r3, [r1, #10]
	sub	sp, #32
	movs	r4, #0
	cmp	r3, #0
	bne.n	.L_081275be
	adds	r2, r1, #0
	adds	r2, #10
.L_081275b0:
	adds	r4, #1
	cmp	r4, #4
	bhi.n	.L_081275be
	adds	r2, #1
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_081275b0
.L_081275be:
	cmp	r4, #5
	bne.n	.L_081275c8
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0812763a
.L_081275c8:
	adds	r7, r1, #0
	movs	r3, #15
	adds	r3, r3, r7
	movs	r4, #0
	mov	r8, r3
.L_081275d2:
	mov	r2, r8
	ldrb	r3, [r2, #0]
	movs	r2, #1
	add	r8, r2
	cmp	r3, #0
	beq.n	.L_08127618
	ldrh	r5, [r7, #0]
	str	r4, [sp, #0]
	adds	r0, r5, #0
	adds	r0, #8
	bl	sub_080ad140
	adds	r6, r0, #0
	ldr	r4, [sp, #0]
	cmp	r6, #0
	beq.n	.L_08127618
	movs	r0, #186
	lsls	r0, r0, #1
	bl	sub_08016ce4
	ldr	r4, [sp, #0]
	cmp	r0, #0
	bne.n	.L_08127610
	movs	r3, #193
	lsls	r3, r3, #3
	adds	r0, r5, r3
	bl	sub_08016ce4
	ldr	r4, [sp, #0]
	cmp	r0, #0
	beq.n	.L_0812762c
.L_08127610:
	ldrb	r3, [r6, #15]
	movs	r2, #1
	add	r9, r3
	add	sl, r2
.L_08127618:
	adds	r4, #1
	adds	r7, #2
	cmp	r4, #4
	bls.n	.L_081275d2
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_08127632
	movs	r0, #3
	negs	r0, r0
	b.n	.L_0812763a
.L_0812762c:
	movs	r0, #2
	negs	r0, r0
	b.n	.L_0812763a
.L_08127632:
	mov	r0, r9
	mov	r1, sl
	bl	sub_08002054
.L_0812763a:
	add	sp, #32
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0xce7c
	.2byte 0x0812
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #0
	mov	fp, r0
	movs	r0, #128
	sub	sp, #16
	mov	r9, r2
	bl	sub_08014dac
	mov	r5, sp
	movs	r3, #0
	mov	sl, r0
	adds	r0, r5, #0
	mov	r8, r3
	bl	sub_0811a038
	adds	r7, r0, #0
	cmp	r7, #0
	ble.n	.L_08127692
	adds	r6, r5, #0
	adds	r5, r7, #0
.L_08127680:
	ldrh	r0, [r6, #0]
	bl	sub_08016ca4
	ldrb	r3, [r0, #15]
	subs	r5, #1
	adds	r6, #2
	add	r8, r3
	cmp	r5, #0
	bne.n	.L_08127680
.L_08127692:
	adds	r1, r7, #0
	mov	r0, r8
	bl	sub_08002054
	mov	r8, r0
	movs	r0, #254
	lsls	r0, r0, #2
	bl	sub_08016d5c
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	add	r8, r0
	mov	r7, r8
	cmp	r7, #0
	bgt.n	.L_081276b4
	movs	r2, #1
	mov	r8, r2
.L_081276b4:
	mov	r3, r8
	cmp	r3, #99
	ble.n	.L_081276be
	movs	r7, #99
	mov	r8, r7
.L_081276be:
	ldr	r1, [pc, #44]
	mov	r2, sl
	movs	r5, #31
.L_081276c4:
	ldrh	r3, [r2, #2]
	subs	r5, #1
	orrs	r3, r1
	strh	r3, [r2, #2]
	adds	r2, #4
	cmp	r5, #0
	bge.n	.L_081276c4
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #91
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_081276f0
	movs	r0, #172
	lsls	r0, r0, #3
	adds	r0, #255
	bl	sub_08016cfc
	b.n	.L_081276f0
	.2byte 0xffff
	.2byte 0x0000
.L_081276f0:
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #92
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_08127708
	movs	r0, #172
	lsls	r0, r0, #3
	adds	r0, #255
	bl	sub_08016cfc
.L_08127708:
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #93
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_08127720
	movs	r0, #172
	lsls	r0, r0, #3
	adds	r0, #255
	bl	sub_08016cfc
.L_08127720:
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #94
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_08127738
	movs	r0, #172
	lsls	r0, r0, #3
	adds	r0, #255
	bl	sub_08016cfc
.L_08127738:
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #107
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_08127750
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #108
	bl	sub_08016cfc
.L_08127750:
	movs	r0, #205
	lsls	r0, r0, #3
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_08127766
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #106
	bl	sub_08016cfc
.L_08127766:
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #105
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0812777e
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #106
	bl	sub_08016cfc
.L_0812777e:
	movs	r5, #224
	lsls	r5, r5, #3
	adds	r5, #113
	adds	r0, r5, #0
	bl	sub_08016d18
	movs	r0, #239
	lsls	r0, r0, #3
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0812779c
	adds	r0, r5, #0
	bl	sub_08016cfc
.L_0812779c:
	ldr	r6, [pc, #200]
	movs	r5, #0
.L_081277a0:
	ldrh	r0, [r6, #0]
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r0, r0, r2
	adds	r5, #1
	adds	r6, #2
	bl	sub_08016d18
	cmp	r5, #86
	bls.n	.L_081277a0
	movs	r0, #71
	bl	sub_08016cfc
	movs	r0, #230
	bl	sub_08016cfc
	movs	r0, #117
	bl	sub_08016cfc
	movs	r5, #0
.L_081277c8:
	adds	r0, r5, #0
	bl	.L_08127588
	cmp	r0, #0
	blt.n	.L_0812780c
	mov	r3, r8
	adds	r3, #3
	cmp	r0, r3
	bgt.n	.L_0812780c
	movs	r3, #186
	movs	r6, #1
	lsls	r3, r3, #2
	negs	r6, r6
	adds	r3, #255
	movs	r4, #0
	mov	r1, sl
.L_081277e8:
	movs	r7, #2
	ldrsh	r2, [r1, r7]
	cmp	r2, r3
	bge.n	.L_081277f4
	adds	r3, r2, #0
	adds	r6, r4, #0
.L_081277f4:
	adds	r4, #1
	adds	r1, #4
	cmp	r4, #31
	ble.n	.L_081277e8
	cmp	r6, #0
	blt.n	.L_0812780c
	lsls	r3, r6, #2
	add	r3, sl
	strh	r0, [r3, #2]
	strh	r5, [r3, #0]
	movs	r2, #1
	add	r9, r2
.L_0812780c:
	movs	r3, #202
	lsls	r3, r3, #1
	adds	r5, #1
	adds	r3, #255
	cmp	r5, r3
	bls.n	.L_081277c8
	mov	r7, r9
	cmp	r7, #32
	ble.n	.L_08127822
	movs	r2, #32
	mov	r9, r2
.L_08127822:
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L_08127848
	bl	sub_08014878
	mov	r3, r9
	muls	r3, r0
	lsrs	r3, r3, #16
	lsls	r3, r3, #2
	add	r3, sl
	movs	r7, #0
	ldrsh	r5, [r3, r7]
	movs	r2, #2
	ldrsh	r3, [r3, r2]
	mov	r7, r8
	subs	r3, r7, r3
	mov	r2, fp
	str	r3, [r2, #0]
	b.n	.L_08127850
.L_08127848:
	mov	r3, r9
	mov	r7, fp
	str	r3, [r7, #0]
	movs	r5, #1
.L_08127850:
	mov	r0, sl
	bl	sub_08013164
	adds	r0, r5, #0
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0c5c
	.2byte 0x0813
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r0, [sp, #8]
	movs	r0, #36
	mov	sl, r1
	bl	sub_08014dac
	mov	r9, r0
	ldr	r0, [sp, #8]
	bl	sub_08016ca4
	adds	r7, r0, #0
	adds	r6, r7, #0
	adds	r6, #16
	movs	r2, #36
	ldr	r3, [pc, #368]
	adds	r1, r6, #0
	mov	r0, r9
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4651
	lsls	r3, r1, #1
	add	r3, sl
	movs	r0, #0
	ldrsh	r5, [r6, r0]
	lsls	r0, r3, #5
	movs	r1, #10
	add	r0, sl
	mov	fp, r3
	bl	sub_08002054
	adds	r5, r5, r0
	mov	r0, r9
	movs	r2, #0
	ldrsh	r3, [r0, r2]
	movs	r1, #10
	lsls	r0, r3, #3
	subs	r0, r0, r3
	bl	sub_08002054
	cmp	r5, r0
	bge.n	.L_081278ce
	adds	r5, r0, #0
.L_081278ce:
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	cmp	r5, r3
	ble.n	.L_081278da
	adds	r5, r3, #0
.L_081278da:
	strh	r5, [r6, #0]
	mov	r2, sl
	lsls	r2, r2, #4
	mov	r3, sl
	movs	r1, #18
	ldrsh	r5, [r7, r1]
	subs	r0, r2, r3
	movs	r1, #10
	str	r2, [sp, #4]
	bl	sub_08002054
	mov	r1, r9
	adds	r5, r5, r0
	movs	r0, #2
	ldrsh	r3, [r1, r0]
	movs	r1, #10
	lsls	r0, r3, #3
	subs	r0, r0, r3
	bl	sub_08002054
	cmp	r5, r0
	bge.n	.L_08127908
	adds	r5, r0, #0
.L_08127908:
	movs	r3, #156
	lsls	r3, r3, #6
	adds	r3, #15
	cmp	r5, r3
	ble.n	.L_08127914
	adds	r5, r3, #0
.L_08127914:
	mov	r2, sl
	mov	r3, sl
	lsls	r2, r2, #5
	subs	r0, r2, r3
	lsls	r0, r0, #2
	strh	r5, [r7, #18]
	subs	r0, r0, r3
	movs	r1, #10
	mov	r8, r2
	bl	sub_08002054
	ldrh	r5, [r7, #24]
	movs	r1, #10
	adds	r5, r5, r0
	mov	r0, r9
	ldrh	r3, [r0, #8]
	lsls	r0, r3, #3
	subs	r0, r0, r3
	bl	sub_08002054
	cmp	r5, r0
	bge.n	.L_08127942
	adds	r5, r0, #0
.L_08127942:
	movs	r6, #186
	lsls	r6, r6, #2
	adds	r6, #255
	cmp	r5, r6
	ble.n	.L_0812794e
	adds	r5, r6, #0
.L_0812794e:
	mov	r0, r8
	strh	r5, [r7, #24]
	movs	r1, #10
	add	r0, sl
	bl	sub_08002054
	mov	r1, r9
	ldrh	r3, [r1, #10]
	ldrh	r5, [r7, #26]
	movs	r1, #10
	adds	r5, r5, r0
	lsls	r0, r3, #3
	subs	r0, r0, r3
	bl	sub_08002054
	cmp	r5, r0
	bge.n	.L_08127972
	adds	r5, r0, #0
.L_08127972:
	cmp	r5, r6
	ble.n	.L_08127978
	adds	r5, r6, #0
.L_08127978:
	mov	r2, fp
	lsls	r0, r2, #4
	strh	r5, [r7, #26]
	movs	r1, #10
	add	r0, fp
	bl	sub_08002054
	ldrh	r5, [r7, #28]
	movs	r1, #10
	adds	r5, r5, r0
	mov	r0, r9
	ldrh	r3, [r0, #12]
	lsls	r0, r3, #3
	subs	r0, r0, r3
	bl	sub_08002054
	cmp	r5, r0
	bge.n	.L_0812799e
	adds	r5, r0, #0
.L_0812799e:
	cmp	r5, r6
	ble.n	.L_081279a4
	adds	r5, r6, #0
.L_081279a4:
	strh	r5, [r7, #28]
	movs	r1, #20
	mov	r8, r1
	movs	r6, #36
	movs	r4, #3
.L_081279ae:
	ldr	r0, [sp, #4]
	ldrsh	r2, [r6, r7]
	mov	r1, sl
	subs	r3, r0, r1
	adds	r5, r2, r3
	mov	r1, r9
	mov	r2, r8
	ldrsh	r3, [r2, r1]
	movs	r1, #10
	lsls	r0, r3, #3
	subs	r0, r0, r3
	str	r4, [sp, #0]
	bl	sub_08002054
	ldr	r4, [sp, #0]
	cmp	r5, r0
	bge.n	.L_081279d2
	adds	r5, r0, #0
.L_081279d2:
	cmp	r5, #200
	ble.n	.L_081279d8
	movs	r5, #200
.L_081279d8:
	movs	r2, #4
	subs	r4, #1
	strh	r5, [r6, r7]
	add	r8, r2
	adds	r6, #4
	cmp	r4, #0
	bge.n	.L_081279ae
	ldrb	r3, [r7, #15]
	add	r3, sl
	strb	r3, [r7, #15]
	ldr	r0, [sp, #8]
	bl	sub_080ad008
	mov	r0, r9
	bl	sub_08013164
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0730
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	sl, r0
	movs	r0, #36
	bl	sub_08014dac
	mov	r8, r0
	mov	r0, sl
	bl	sub_08016ca4
	adds	r7, r0, #0
	adds	r6, r7, #0
	adds	r6, #16
	adds	r1, r6, #0
	ldr	r3, [pc, #168]
	movs	r2, #36
	mov	r0, r8
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2100
	ldrsh	r2, [r6, r1]
	mov	r1, r8
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r5, r3, #1
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	movs	r1, #10
	lsls	r0, r3, #3
	subs	r0, r0, r3
	bl	sub_08002054
	cmp	r5, r0
	bge.n	.L_08127a5a
	adds	r5, r0, #0
.L_08127a5a:
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	cmp	r5, r3
	ble.n	.L_08127a66
	adds	r5, r3, #0
.L_08127a66:
	strh	r5, [r6, #0]
	ldrh	r2, [r7, #24]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	cmp	r3, #0
	bge.n	.L_08127a74
	adds	r3, #3
.L_08127a74:
	mov	r2, r8
	asrs	r5, r3, #2
	ldrh	r3, [r2, #8]
	movs	r1, #10
	lsls	r0, r3, #3
	subs	r0, r0, r3
	bl	sub_08002054
	cmp	r5, r0
	bge.n	.L_08127a8a
	adds	r5, r0, #0
.L_08127a8a:
	movs	r6, #186
	lsls	r6, r6, #2
	adds	r6, #255
	cmp	r5, r6
	ble.n	.L_08127a96
	adds	r5, r6, #0
.L_08127a96:
	ldrh	r2, [r7, #26]
	strh	r5, [r7, #24]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	cmp	r3, #0
	bge.n	.L_08127aa4
	adds	r3, #3
.L_08127aa4:
	mov	r1, r8
	asrs	r5, r3, #2
	ldrh	r3, [r1, #10]
	movs	r1, #10
	lsls	r0, r3, #3
	subs	r0, r0, r3
	bl	sub_08002054
	cmp	r5, r0
	bge.n	.L_08127aba
	adds	r5, r0, #0
.L_08127aba:
	cmp	r5, r6
	ble.n	.L_08127ac0
	adds	r5, r6, #0
.L_08127ac0:
	mov	r0, sl
	strh	r5, [r7, #26]
	bl	sub_080ad008
	mov	r0, r8
	bl	sub_08013164
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0730
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #36]
	movs	r1, #0
	adds	r6, r5, #0
	adds	r6, #64
	ldrb	r4, [r6, #0]
	sub	sp, #4
	cmp	r1, r4
	bge.n	.L_08127b0a
	ldrh	r3, [r5, #16]
	cmp	r3, r0
	beq.n	.L_08127b0a
	adds	r2, r5, #0
	adds	r2, #16
.L_08127afc:
	adds	r1, #1
	cmp	r1, r4
	bge.n	.L_08127b0a
	adds	r2, #2
	ldrh	r3, [r2, #0]
	cmp	r3, r0
	bne.n	.L_08127afc
.L_08127b0a:
	cmp	r1, r4
	beq.n	.L_08127b70
	adds	r6, r1, #0
	adds	r6, #52
	ldrsb	r3, [r5, r6]
	movs	r4, #0
	cmp	r3, #0
	bge.n	.L_08127b2e
	movs	r3, #1
	strb	r3, [r5, r6]
	movs	r0, #128
	lsls	r3, r1, #2
	adds	r3, #28
	movs	r2, #3
	lsls	r0, r0, #8
	str	r2, [r5, r3]
	adds	r0, #1
	b.n	.L_08127b9a
.L_08127b2e:
	lsls	r7, r1, #2
	b.n	.L_08127b34
.L_08127b32:
	adds	r4, #1
.L_08127b34:
	cmp	r4, #31
	bgt.n	.L_08127b5c
	ldrsb	r0, [r5, r6]
	movs	r1, #9
	adds	r0, #1
	str	r4, [sp, #0]
	bl	sub_08002064
	strb	r0, [r5, r6]
	adds	r3, r7, #0
	adds	r3, #28
	lsls	r0, r0, #24
	ldr	r3, [r5, r3]
	asrs	r0, r0, #24
	movs	r2, #1
	lsls	r2, r0
	ands	r3, r2
	ldr	r4, [sp, #0]
	cmp	r3, #0
	bne.n	.L_08127b32
.L_08127b5c:
	ldrsb	r3, [r5, r6]
	adds	r1, r7, #0
	adds	r1, #28
	movs	r2, #1
	lsls	r2, r3
	ldr	r3, [r5, r1]
	orrs	r3, r2
	str	r3, [r5, r1]
	ldrsb	r0, [r5, r6]
	b.n	.L_08127b9a
.L_08127b70:
	cmp	r4, #4
	bgt.n	.L_08127b96
	movs	r1, #1
	adds	r2, r4, #0
	negs	r1, r1
	adds	r2, #52
	adds	r3, r1, #0
	strb	r3, [r5, r2]
	lsls	r3, r4, #1
	adds	r3, #16
	strh	r0, [r5, r3]
	lsls	r3, r4, #2
	adds	r3, #28
	movs	r2, #0
	str	r2, [r5, r3]
	adds	r3, r4, #1
	strb	r3, [r6, #0]
	movs	r0, #9
	b.n	.L_08127b9a
.L_08127b96:
	movs	r0, #1
	negs	r0, r0
.L_08127b9a:
	add	sp, #4
	pop	{r5, r6, r7, pc}
