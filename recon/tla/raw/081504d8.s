.syntax unified
	.thumb
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
	.set sub_08015128, 0x08015128
	.set sub_080156e8, 0x080156e8
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138058, 0x08138058
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_0814cd48, 0x0814cd48
	.set sub_0815085e, 0x0815085e
	.set sub_08150b76, 0x08150b76
	.set sub_08157cf4, 0x08157cf4
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_081963ec, 0x081963ec
	.set sub_081c0010, 0x081c0010
	.global Func_081504d8
	.thumb_func
Func_081504d8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #108
	str	r1, [sp, #56]
	str	r0, [sp, #60]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	str	r0, [sp, #52]
	movs	r0, #0
	ldr	r3, [r3, #96]
	str	r3, [sp, #48]
	bl	sub_081435e0
	movs	r2, #128
	ldr	r3, [pc, #36]
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r1, [sp, #56]
	cmp	r1, #0
	beq.n	.L_08150512
	cmp	r1, #3
	bne.n	.L_08150530
.L_08150512:
	ldr	r2, [sp, #52]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #16]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	b.n	.L_08150542
	movs	r0, r0
	.4byte 0x00000100
	.2byte 0x015f
	.2byte 0x0000
.L_08150530:
	ldr	r5, [sp, #52]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r5, r2
	ldr	r0, [pc, #692]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
.L_08150542:
	ldr	r3, [sp, #56]
	cmp	r3, #0
	beq.n	.L_0815055c
	ldr	r5, [sp, #56]
	cmp	r5, #3
	bne.n	.L_08150552
	ldr	r0, [pc, #676]
	b.n	.L_0815055e
.L_08150552:
	ldr	r0, [sp, #56]
	cmp	r0, #1
	bne.n	.L_0815055c
	ldr	r0, [pc, #664]
	b.n	.L_0815055e
.L_0815055c:
	ldr	r0, [pc, #664]
.L_0815055e:
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #660]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9a0d
	movs	r3, #178
	lsls	r3, r3, #6
	adds	r1, r2, r3
	ldr	r0, [pc, #644]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r0, #174
	movs	r5, #1
	lsls	r0, r0, #2
	mov	r9, r5
	mov	lr, r0
	movs	r4, #57
.L_0815058e:
	ldr	r3, [sp, #52]
	movs	r2, #178
	movs	r1, #0
	lsls	r2, r2, #6
	str	r1, [sp, #40]
	adds	r1, r3, r2
	adds	r3, r0, r3
	mov	ip, r4
	adds	r3, r3, r2
.L_081505a0:
	ldrb	r2, [r1, #0]
	adds	r1, #1
	cmp	r2, ip
	ble.n	.L_081505aa
	mov	r2, ip
.L_081505aa:
	cmp	r2, #0
	bge.n	.L_081505b0
	movs	r2, #0
.L_081505b0:
	strb	r2, [r3, #0]
	ldr	r5, [sp, #40]
	adds	r3, #1
	adds	r5, #1
	str	r5, [sp, #40]
	cmp	r5, lr
	bne.n	.L_081505a0
	movs	r2, #1
	movs	r1, #174
	add	r9, r2
	lsls	r1, r1, #2
	mov	r3, r9
	adds	r0, r0, r1
	subs	r4, #7
	cmp	r3, #8
	bne.n	.L_0815058e
	ldr	r5, [sp, #60]
	ldr	r3, [r5, #4]
	cmp	r3, #1
	bne.n	.L_081505ea
	movs	r2, #128
	ldr	r3, [pc, #552]
	lsls	r2, r2, #19
	movs	r0, #112
	adds	r2, #40
	negs	r0, r0
	str	r3, [r2, #0]
	str	r0, [sp, #36]
	b.n	.L_081505f6
.L_081505ea:
	movs	r2, #128
	lsls	r2, r2, #19
	movs	r3, #0
	adds	r2, #40
	str	r3, [r2, #0]
	str	r3, [sp, #36]
.L_081505f6:
	ldr	r5, [pc, #528]
	movs	r1, #0
	mov	r9, r1
	movs	r7, #192
.L_081505fe:
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r6, r0, #0
	ands	r6, r3
	movs	r3, #0
	str	r3, [r5, #0]
	ldr	r2, [sp, #56]
	cmp	r2, #0
	beq.n	.L_0815061a
	cmp	r2, #3
	bne.n	.L_0815064a
.L_0815061a:
	movs	r2, #31
	mov	r3, r9
	ands	r2, r3
	cmp	r2, #0
	bge.n	.L_08150626
	adds	r2, #3
.L_08150626:
	asrs	r2, r2, #2
	lsls	r3, r2, #1
	ldr	r0, [pc, #480]
	adds	r3, r3, r2
	lsls	r3, r3, #17
	adds	r3, r3, r0
	str	r3, [r5, #4]
	mov	r3, r9
	cmp	r3, #0
	bge.n	.L_0815063c
	adds	r3, #3
.L_0815063c:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	mov	r1, r9
	ldr	r2, [pc, #460]
	subs	r3, r1, r3
	lsls	r3, r3, #17
	b.n	.L_08150678
.L_0815064a:
	movs	r2, #31
	mov	r3, r9
	ands	r2, r3
	cmp	r2, #0
	bge.n	.L_08150656
	adds	r2, #3
.L_08150656:
	asrs	r2, r2, #2
	lsls	r3, r2, #1
	ldr	r0, [pc, #432]
	adds	r3, r3, r2
	lsls	r3, r3, #17
	adds	r3, r3, r0
	str	r3, [r5, #4]
	mov	r3, r9
	cmp	r3, #0
	bge.n	.L_0815066c
	adds	r3, #3
.L_0815066c:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	mov	r1, r9
	ldr	r2, [pc, #416]
	subs	r3, r1, r3
	lsls	r3, r3, #19
.L_08150678:
	adds	r3, r3, r2
	str	r3, [r5, #8]
	ldr	r0, [sp, #60]
	ldr	r3, [r0, #4]
	cmp	r3, #1
	bne.n	.L_0815068a
	movs	r3, #128
	lsls	r3, r3, #10
	b.n	.L_0815068c
.L_0815068a:
	ldr	r3, [pc, #388]
.L_0815068c:
	str	r3, [r5, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r7, #0
	muls	r3, r0
	movs	r1, #128
	lsls	r1, r1, #9
	asrs	r3, r3, #6
	adds	r3, r3, r1
	str	r3, [r5, #16]
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r3, r7, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r5, #20]
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	str	r3, [r5, #24]
	movs	r2, #1
	movs	r3, #128
	add	r9, r2
	lsls	r3, r3, #2
	adds	r5, #28
	cmp	r9, r3
	bne.n	.L_081505fe
	ldr	r5, [sp, #60]
	mov	r1, sp
	ldr	r0, [r5, #4]
	adds	r1, #64
	str	r1, [sp, #32]
	bl	sub_08144aac
	ldr	r3, [sp, #52]
	movs	r5, #239
	lsls	r5, r5, #7
	adds	r2, r3, r5
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r0, [sp, #52]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #132
	adds	r2, r0, r1
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	ldr	r0, [pc, #292]
	lsls	r1, r1, #4
	bl	sub_080145a8
	movs	r2, #0
	ldr	r3, [sp, #60]
	str	r2, [sp, #44]
	movs	r5, #64
	ldr	r2, [r3, #20]
	negs	r5, r5
	lsls	r3, r2, #2
	cmp	r3, r5
	bne.n	.L_0815070e
	b.n	.L_0815097a
.L_0815070e:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #48]
	ldr	r0, [sp, #44]
	str	r3, [sp, #28]
	cmp	r0, #72
	bne.n	.L_08150726
	movs	r0, #0
	bl	sub_081180e8
	ldr	r1, [sp, #60]
	ldr	r2, [r1, #20]
.L_08150726:
	movs	r3, #0
	str	r3, [sp, #40]
	cmp	r2, #0
	bne.n	.L_08150730
	b.n	.L_08150950
.L_08150730:
	ldr	r5, [sp, #44]
	ldr	r2, [sp, #28]
	subs	r5, #24
	ldr	r0, [sp, #44]
	movs	r1, #36
	adds	r2, #12
	str	r5, [sp, #16]
	str	r1, [sp, #12]
	str	r2, [sp, #24]
	str	r3, [sp, #8]
	mov	r8, r0
.L_08150746:
	ldr	r3, [sp, #12]
	ldr	r1, [sp, #60]
	ldrsh	r0, [r3, r1]
	bl	sub_08118098
	mov	r2, r8
	ldr	r6, [r0, #0]
	cmp	r2, #0
	bgt.n	.L_0815075a
	b.n	.L_08150922
.L_0815075a:
	bl	sub_08014de4
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #24]
	bl	sub_080156e8
	ldr	r3, [r6, #8]
	add	r5, sp, #72
	str	r3, [r5, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_08014de4
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #24]
	bl	sub_080156e8
	adds	r0, r5, #0
	bl	sub_08015128
	movs	r3, #0
	add	r0, sp, #96
	add	r5, sp, #84
	str	r3, [r0, #0]
	str	r3, [r0, #4]
	str	r3, [r0, #8]
	adds	r1, r5, #0
	bl	sub_0815e1ec
	ldr	r3, [r5, #0]
	ldr	r0, [sp, #36]
	ldr	r1, [r5, #4]
	ldr	r2, [sp, #56]
	adds	r6, r3, r0
	mov	sl, r1
	cmp	r2, #0
	beq.n	.L_081507ae
	cmp	r2, #3
	bne.n	.L_0815081c
.L_081507ae:
	mov	r3, r8
	cmp	r3, #26
	bgt.n	sub_0815085e
	mov	r0, r8
	cmp	r3, #0
	bge.n	.L_081507bc
	adds	r0, #3
.L_081507bc:
	movs	r1, #7
	asrs	r0, r0, #2
	bl	sub_08002064
	lsls	r1, r0, #4
	subs	r1, r1, r0
	ldr	r0, [sp, #52]
	lsls	r1, r1, #6
	movs	r2, #224
	adds	r1, r0, r1
	lsls	r2, r2, #3
	movs	r0, #24
	adds	r1, r1, r2
	mov	r3, sl
	adds	r2, r6, #0
	str	r0, [sp, #0]
	movs	r0, #40
	str	r0, [sp, #4]
	subs	r2, #12
	subs	r3, #20
	ldr	r4, [sp, #64]
	ldr	r0, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe037
	movs	r0, r0
	.4byte 0x0000015e
	.4byte 0x0000017b
	.4byte 0x0000017f
	.4byte 0x03000730
	.4byte 0x00000160
	.4byte 0xffff9000
	.4byte 0x02010000
	.4byte 0xfff60000
	.4byte 0xfffe0000
	.4byte 0xfff00000
	.2byte 0x3001
	.2byte 0x0814
.L_0815081c:
	mov	r3, r8
	cmp	r3, #23
	bgt.n	sub_0815085e
	mov	r0, r8
	cmp	r3, #0
	bge.n	.L_0815082a
	adds	r0, #3
.L_0815082a:
	movs	r1, #6
	asrs	r0, r0, #2
	bl	sub_08002064
	lsls	r1, r0, #1
	adds	r1, r1, r0
	lsls	r1, r1, #3
	adds	r1, r1, r0
	ldr	r0, [sp, #52]
	lsls	r1, r1, #6
	adds	r1, r0, r1
	movs	r0, #40
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	movs	r2, #224
	ldr	r0, [sp, #32]
	lsls	r2, r2, #3
	adds	r1, r1, r2
	mov	r3, sl
	adds	r2, r6, #0
	ldr	r4, [r0, #4]
	subs	r2, #20
	subs	r3, #20
	ldr	r0, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4641
	cmp	r1, #24
	bne.n	.L_0815086a
	movs	r0, #143
	bl	sub_081c0010
.L_0815086a:
	ldr	r2, [sp, #16]
	cmp	r2, #36
	bhi.n	.L_08150922
	mov	r3, r8
	movs	r1, #0
	cmp	r3, #28
	ble.n	.L_0815088a
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_08150882
	mov	r3, r8
	subs	r3, #21
.L_08150882:
	asrs	r1, r3, #2
	cmp	r1, #7
	ble.n	.L_0815088a
	movs	r1, #7
.L_0815088a:
	movs	r3, #174
	lsls	r3, r3, #2
	adds	r2, r1, #0
	muls	r2, r3
	mov	fp, r5
	ldr	r3, [sp, #8]
	ldr	r5, [pc, #264]
	str	r2, [sp, #20]
	movs	r0, #0
	mov	r9, r0
	adds	r7, r3, r5
.L_081508a0:
	mov	r3, r9
	cmp	r3, #0
	bge.n	.L_081508a8
	adds	r3, #3
.L_081508a8:
	asrs	r3, r3, #2
	mov	r0, r9
	lsls	r3, r3, #2
	subs	r3, r0, r3
	lsls	r2, r3, #1
	adds	r6, r2, r3
	ldr	r3, [r7, #24]
	mov	r1, r8
	adds	r0, r3, r1
	cmp	r0, #0
	bge.n	.L_081508c0
	adds	r0, #7
.L_081508c0:
	movs	r1, #3
	asrs	r0, r0, #3
	bl	sub_08002064
	mov	r1, fp
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	sub_0815e1ec
	mov	r2, fp
	ldr	r3, [r2, #0]
	ldr	r0, [sp, #36]
	ldr	r1, [r2, #4]
	ldr	r2, [pc, #200]
	adds	r5, r6, r5
	adds	r6, r3, r0
	lsls	r3, r5, #1
	mov	sl, r1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #20]
	ldr	r3, [sp, #52]
	adds	r1, r2, r1
	adds	r1, r3, r1
	ldr	r3, [pc, #184]
	movs	r0, #178
	ldrb	r3, [r3, r5]
	lsls	r0, r0, #6
	str	r3, [sp, #0]
	ldr	r3, [pc, #176]
	adds	r1, r1, r0
	ldrb	r3, [r3, r5]
	ldr	r4, [sp, #64]
	str	r3, [sp, #4]
	ldr	r0, [sp, #48]
	adds	r2, r6, #0
	mov	r3, sl
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c38
	movs	r1, #60
	movs	r2, #0
	bl	sub_08138058
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	adds	r7, #28
	cmp	r2, #24
	bne.n	.L_081508a0
.L_08150922:
	ldr	r3, [sp, #16]
	ldr	r0, [sp, #12]
	subs	r3, #4
	str	r3, [sp, #16]
	ldr	r1, [sp, #8]
	ldr	r3, [sp, #40]
	movs	r2, #224
	movs	r5, #4
	lsls	r2, r2, #2
	negs	r5, r5
	adds	r0, #2
	adds	r3, #1
	adds	r1, r1, r2
	str	r0, [sp, #12]
	str	r3, [sp, #40]
	add	r8, r5
	str	r1, [sp, #8]
	ldr	r5, [sp, #60]
	ldr	r0, [sp, #40]
	ldr	r3, [r5, #20]
	cmp	r0, r3
	beq.n	.L_08150950
	b.n	.L_08150746
.L_08150950:
	ldr	r1, [sp, #52]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #232
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r5, [sp, #44]
	ldr	r0, [sp, #60]
	adds	r5, #1
	str	r5, [sp, #44]
	ldr	r3, [r0, #20]
	adds	r2, r3, #0
	lsls	r3, r2, #2
	adds	r3, #64
	cmp	r5, r3
	beq.n	.L_0815097a
	b.n	.L_0815070e
.L_0815097a:
	ldr	r0, [pc, #52]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #108
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x081974dc
	.4byte 0x081974f4
	.4byte 0x08197500
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #24
	str	r0, [sp, #20]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #92]
	ldr	r1, [r5, #96]
	mov	sl, r0
	movs	r0, #0
	str	r1, [sp, #16]
	bl	sub_081435e0
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	movs	r1, #224
	adds	r2, #2
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	ldr	r0, [pc, #40]
	movs	r2, #1
	movs	r3, #1
	add	r1, sl
	bl	sub_08157cf4
	movs	r0, #104
	movs	r1, #3
	bl	sub_081963ec
	ldr	r5, [r5, #104]
	ldr	r4, [sp, #20]
	str	r5, [sp, #8]
	movs	r2, #36
	ldrsh	r3, [r4, r2]
	cmp	r3, #127
	ble.n	.L_08150a26
	b.n	.L_08150a1c
	.4byte 0x00003f46
	.4byte 0x00001010
	.2byte 0x0178
	.2byte 0x0000
.L_08150a1c:
	movs	r0, #0
	movs	r1, #1
	mov	fp, r0
	mov	r9, r1
	b.n	.L_08150a30
.L_08150a26:
	movs	r3, #1
	movs	r2, #64
	negs	r3, r3
	mov	fp, r2
	mov	r9, r3
.L_08150a30:
	ldr	r6, [pc, #460]
	movs	r4, #0
	mov	r8, r4
	movs	r7, #0
	mov	r5, sl
.L_08150a3a:
	adds	r0, r6, #0
	bl	sub_08002096
	lsls	r0, r0, #5
	asrs	r0, r0, #16
	mov	r3, r9
	muls	r3, r0
	add	r3, fp
	adds	r3, #20
	str	r3, [r5, #0]
	adds	r0, r6, #0
	bl	sub_08002090
	lsls	r0, r0, #4
	asrs	r0, r0, #16
	adds	r0, #40
	movs	r1, #1
	str	r0, [r5, #4]
	add	r8, r1
	movs	r0, #128
	lsls	r0, r0, #5
	mov	r2, r8
	str	r7, [r5, #24]
	adds	r6, r6, r0
	subs	r7, #4
	adds	r5, #28
	cmp	r2, #9
	bne.n	.L_08150a3a
	movs	r3, #239
	lsls	r3, r3, #7
	add	r3, sl
	movs	r2, #2
	str	r2, [r3, #0]
	ldr	r4, [sp, #20]
	ldr	r3, [r4, #24]
	cmp	r3, #2
	bne.n	.L_08150a90
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, sl
	movs	r3, #75
	b.n	.L_08150a9a
.L_08150a90:
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, sl
	movs	r3, #50
.L_08150a9a:
	str	r3, [r2, #0]
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #352]
	bl	sub_080145a8
	movs	r0, #136
	bl	sub_081c0010
	movs	r0, #0
	str	r0, [sp, #12]
.L_08150ab0:
	ldr	r1, [sp, #12]
	cmp	r1, #24
	bne.n	.L_08150abc
	movs	r0, #133
	bl	sub_081180e8
.L_08150abc:
	movs	r2, #0
	mov	r8, r2
	mov	r6, sl
.L_08150ac2:
	ldr	r3, [r6, #24]
	cmp	r3, #23
	bhi.n	.L_08150b78
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L_08150ad0
	adds	r2, r3, #3
.L_08150ad0:
	ldr	r3, [pc, #308]
	asrs	r5, r2, #2
	lsls	r7, r5, #1
	ldrh	r1, [r3, r7]
	ldr	r0, [pc, #304]
	movs	r4, #224
	lsls	r4, r4, #3
	add	r1, sl
	adds	r1, r1, r4
	ldrb	r4, [r0, r5]
	ldr	r2, [r6, #0]
	lsrs	r3, r4, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #292]
	ldrb	r0, [r3, r5]
	mov	r9, r3
	ldr	r3, [r6, #4]
	str	r4, [sp, #0]
	ldr	r4, [pc, #284]
	adds	r3, r3, r0
	ldrb	r0, [r4, r5]
	mov	fp, r4
	str	r0, [sp, #4]
	ldr	r4, [sp, #8]
	ldr	r0, [sp, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9805
	ldr	r3, [r0, #24]
	cmp	r3, #0
	beq.n	.L_08150b42
	ldr	r2, [pc, #248]
	ldr	r0, [pc, #248]
	ldrh	r1, [r2, r7]
	ldrb	r4, [r0, r5]
	ldr	r2, [r6, #0]
	movs	r3, #224
	lsls	r3, r3, #3
	add	r1, sl
	adds	r1, r1, r3
	lsrs	r3, r4, #1
	subs	r2, r2, r3
	mov	r3, r9
	ldrb	r0, [r3, r5]
	ldr	r3, [r6, #4]
	str	r4, [sp, #0]
	mov	r4, fp
	adds	r3, r3, r0
	ldrb	r0, [r4, r5]
	subs	r3, #16
	str	r0, [sp, #4]
	ldr	r4, [sp, #8]
	ldr	r0, [sp, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9805
	ldr	r3, [r0, #24]
.L_08150b42:
	cmp	r3, #2
	bne.n	sub_08150b76
	ldr	r2, [pc, #192]
	ldr	r0, [pc, #192]
	ldrh	r1, [r2, r7]
	ldrb	r4, [r0, r5]
	ldr	r2, [r6, #0]
	movs	r3, #224
	lsls	r3, r3, #3
	add	r1, sl
	adds	r1, r1, r3
	lsrs	r3, r4, #1
	subs	r2, r2, r3
	mov	r3, r9
	ldrb	r0, [r3, r5]
	ldr	r3, [r6, #4]
	str	r4, [sp, #0]
	mov	r4, fp
	adds	r3, r3, r0
	ldrb	r0, [r4, r5]
	subs	r3, #32
	str	r0, [sp, #4]
	ldr	r4, [sp, #8]
	ldr	r0, [sp, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69b3
.L_08150b78:
	movs	r0, #1
	add	r8, r0
	adds	r3, #1
	mov	r1, r8
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r1, #9
	bne.n	.L_08150ac2
	ldr	r4, [sp, #20]
	movs	r2, #0
	ldr	r3, [r4, #20]
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_08150bc0
	movs	r6, #36
	movs	r5, #16
.L_08150b98:
	ldr	r0, [sp, #12]
	cmp	r0, r5
	bne.n	.L_08150bb4
	ldr	r1, [sp, #20]
	movs	r3, #12
	ldrsh	r0, [r6, r1]
	str	r3, [sp, #0]
	movs	r1, #10
	mov	r3, r8
	movs	r2, #5
	bl	sub_0814cd48
	ldr	r4, [sp, #20]
	ldr	r3, [r4, #20]
.L_08150bb4:
	movs	r0, #1
	add	r8, r0
	adds	r6, #2
	adds	r5, #8
	cmp	r8, r3
	bne.n	.L_08150b98
.L_08150bc0:
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, sl
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [sp, #12]
	adds	r1, #1
	str	r1, [sp, #12]
	cmp	r1, #60
	beq.n	.L_08150be2
	b.n	.L_08150ab0
.L_08150be2:
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #24]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffffc000
	.4byte 0x08143001
	.4byte 0x0819747a
	.4byte 0x08197467
	.4byte 0x08197473
	.4byte 0x0819746d
