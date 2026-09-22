.syntax unified
	.thumb
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
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_081180e8, 0x081180e8
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_0815585c, 0x0815585c
	.set sub_08157cf4, 0x08157cf4
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_08173800, 0x08173800
	.set sub_08173aac, 0x08173aac
	.set sub_081963ec, 0x081963ec
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Overlay_08173588
Overlay_08173588:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #84
	str	r0, [sp, #36]
	str	r1, [sp, #32]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #92]
	ldr	r3, [r3, #96]
	movs	r0, #0
	mov	fp, r1
	str	r3, [sp, #28]
	bl	sub_081435e0
	ldr	r2, [sp, #32]
	cmp	r2, #0
	bne.n	.L_08173664
	ldr	r3, [sp, #36]
	mov	r4, sp
	ldr	r1, [r3, #4]
	movs	r3, #1
	eors	r1, r3
	adds	r4, #72
	lsls	r1, r1, #4
	ldr	r0, [sp, #36]
	adds	r2, r4, #0
	orrs	r1, r3
	add	r3, sp, #60
	str	r4, [sp, #8]
	bl	sub_0815585c
	movs	r1, #216
	lsls	r1, r1, #7
	adds	r1, #192
	ldr	r0, [pc, #288]
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #276]
	add	r1, fp
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r0, #184
	lsls	r0, r0, #4
	movs	r1, #128
	lsls	r1, r1, #7
	add	r0, fp
	ldr	r3, [pc, #256]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2100
	mov	ip, r1
	movs	r7, #0
	movs	r6, #0
	mov	r5, fp
.L_0817360c:
	movs	r2, #208
	lsls	r2, r2, #4
	adds	r2, #136
	mov	r1, fp
	adds	r3, r6, r1
	adds	r0, r5, r2
	adds	r2, #47
	adds	r1, r3, r2
	adds	r3, r7, #0
	add	r3, fp
	movs	r2, #224
	mov	lr, r3
	lsls	r2, r2, #3
	movs	r4, #0
	add	r2, lr
.L_0817362a:
	ldrb	r3, [r2, #0]
	adds	r4, #1
	strb	r3, [r0, #0]
	adds	r0, #1
	ldrb	r3, [r2, #0]
	adds	r2, #1
	strb	r3, [r1, #0]
	subs	r1, #1
	cmp	r4, #24
	bne.n	.L_0817362a
	movs	r4, #1
	add	ip, r4
	mov	r1, ip
	adds	r7, #24
	adds	r5, #64
	adds	r6, #64
	cmp	r1, #48
	bne.n	.L_0817360c
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, fp
	movs	r3, #50
	b.n	.L_081736b8
.L_08173664:
	ldr	r2, [sp, #36]
	movs	r3, #1
	ldr	r1, [r2, #4]
	adds	r0, r2, #0
	eors	r1, r3
	lsls	r1, r1, #4
	movs	r3, #3
	orrs	r1, r3
	mov	r3, sp
	adds	r3, #72
	str	r3, [sp, #8]
	ldr	r2, [sp, #8]
	add	r3, sp, #48
	bl	sub_0815585c
	movs	r1, #216
	lsls	r1, r1, #7
	adds	r1, #192
	ldr	r0, [pc, #120]
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r1, #224
	lsls	r1, r1, #3
	movs	r2, #1
	movs	r3, #0
	ldr	r0, [pc, #104]
	add	r1, fp
	bl	sub_08157cf4
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, fp
	movs	r3, #75
.L_081736b8:
	str	r3, [r2, #0]
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #76]
	bl	sub_080145a8
	ldr	r3, [pc, #44]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r4, [sp, #8]
	movs	r1, #128
	ldr	r2, [r4, #0]
	movs	r3, #64
	lsls	r1, r1, #19
	subs	r3, r3, r2
	adds	r1, #40
	lsls	r3, r3, #8
	str	r3, [r1, #0]
	ldr	r1, [sp, #36]
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_08173710
	movs	r0, #104
	movs	r1, #7
	bl	sub_081963ec
	b.n	.L_08173718
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000154
	.4byte 0x0000013a
	.4byte 0x03000258
	.4byte 0x00000152
	.4byte 0x000000d5
	.2byte 0x3001
	.2byte 0x0814
.L_08173710:
	movs	r0, #104
	movs	r1, #3
	bl	sub_081963ec
.L_08173718:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r2, #0
	str	r3, [sp, #20]
	mov	r3, sp
	adds	r3, #40
	str	r3, [sp, #12]
	mov	r9, r2
.L_0817372a:
	mov	r4, r9
	cmp	r4, #0
	bne.n	.L_08173776
	movs	r1, #0
	mov	r8, r1
	movs	r6, #0
	mov	r5, fp
.L_08173738:
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	subs	r3, #16
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	subs	r3, #16
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	adds	r3, #128
	str	r3, [r5, #8]
	bl	sub_08014878
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	str	r6, [r5, #24]
	str	r0, [r5, #16]
	subs	r6, #2
	adds	r5, #28
	cmp	r3, #32
	bne.n	.L_08173738
.L_08173776:
	mov	r4, r9
	cmp	r4, #38
	bne.n	.L_08173784
	movs	r0, #1
	negs	r0, r0
	bl	sub_081180e8
.L_08173784:
	ldr	r1, [sp, #32]
	cmp	r1, #0
	bne.n	.L_08173798
	mov	r2, r9
	cmp	r2, #32
	bne.n	.L_081737c0
	movs	r0, #212
	bl	sub_081c0010
	b.n	.L_081737c0
.L_08173798:
	mov	r3, r9
	cmp	r3, #7
	ble.n	.L_081737a4
	ldr	r0, [pc, #556]
	bl	sub_0815f0a0
.L_081737a4:
	mov	r4, r9
	cmp	r4, #32
	bne.n	.L_081737c0
	ldr	r2, [sp, #36]
	movs	r3, #24
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	movs	r3, #0
	bl	sub_0814cd48
.L_081737c0:
	ldr	r4, [sp, #8]
	ldr	r3, [r4, #4]
	subs	r3, #1
	str	r3, [r4, #4]
	cmp	r3, #0
	ble.n	sub_08173800
	adds	r5, r3, #0
	cmp	r5, #39
	ble.n	.L_081737d4
	movs	r5, #40
.L_081737d4:
	mov	r1, r9
	lsls	r0, r1, #10
	bl	sub_08002096
	ldr	r4, [sp, #8]
	movs	r1, #40
	ldr	r3, [r4, #4]
	adds	r2, r0, #0
	str	r1, [sp, #0]
	movs	r1, #216
	lsls	r2, r2, #5
	lsls	r1, r1, #7
	asrs	r2, r2, #16
	adds	r1, #192
	adds	r2, #44
	subs	r3, #20
	str	r5, [sp, #4]
	ldr	r0, [sp, #28]
	add	r1, fp
	ldr	r4, [sp, #20]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xf6a1
	.2byte 0xfaf0
	.2byte 0x4a72
	movs	r3, #104
	str	r3, [r2, #16]
	movs	r0, #32
	bl	sub_08014dac
	str	r0, [sp, #16]
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #440]
	ldr	r3, [sp, #40]
	movs	r1, #6
	ands	r3, r2
	ldr	r2, [pc, #436]
	orrs	r3, r1
	ands	r3, r2
	movs	r2, #192
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r1, [r0, #0]
	ldr	r1, [sp, #12]
	str	r3, [sp, #40]
	ldr	r3, [pc, #424]
	str	r1, [r0, #16]
	str	r3, [r0, #8]
	ldr	r2, [sp, #16]
	mov	sl, r0
	str	r2, [r0, #12]
	ldr	r3, [sp, #32]
	cmp	r3, #0
	bne.n	.L_081738e6
	mov	r4, r9
	lsls	r3, r4, #3
	subs	r3, #64
	cmp	r3, #0
	ble.n	.L_08173850
	movs	r3, #0
.L_08173850:
	mov	r1, r9
	cmp	r1, #55
	ble.n	.L_0817385c
	movs	r3, #56
	subs	r3, r3, r1
	lsls	r3, r3, #3
.L_0817385c:
	mov	r2, sl
	str	r3, [r2, #20]
	ldr	r4, [sp, #12]
	movs	r3, #184
	lsls	r3, r3, #4
	add	r3, fp
	mov	r2, r9
	str	r3, [r4, #4]
	movs	r1, #0
	movs	r3, #32
	mov	r8, r1
	lsls	r7, r2, #11
	subs	r6, r3, r2
.L_08173876:
	movs	r3, #128
	lsls	r3, r3, #10
	subs	r5, r3, r7
	mov	r3, r9
	cmp	r3, #31
	ble.n	.L_08173888
	mov	r4, r8
	cmp	r4, #1
	beq.n	.L_081738da
.L_08173888:
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	cmp	r5, r1
	bgt.n	.L_08173896
	movs	r5, #128
	lsls	r5, r5, #9
.L_08173896:
	bl	sub_08014de4
	ldr	r0, [pc, #324]
	bl	sub_08015024
	mov	r2, r9
	cmp	r2, #31
	bgt.n	.L_081738c0
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L_081738b4
	lsls	r0, r6, #9
	bl	sub_08015068
	b.n	.L_081738c0
.L_081738b4:
	ldr	r1, [pc, #296]
	mov	r4, r9
	lsls	r0, r4, #9
	adds	r0, r0, r1
	bl	sub_08015068
.L_081738c0:
	asrs	r2, r5, #1
	adds	r0, r2, #0
	adds	r1, r2, #0
	bl	sub_080151e4
	ldr	r0, [pc, #280]
	ldr	r1, [sp, #16]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_081738da:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #2
	bne.n	.L_08173876
	b.n	.L_08173976
.L_081738e6:
	movs	r3, #0
	mov	r4, sl
	str	r3, [r4, #20]
	ldr	r1, [sp, #12]
	movs	r3, #224
	lsls	r3, r3, #3
	add	r3, fp
	str	r3, [r1, #4]
	movs	r2, #0
	mov	r8, r2
	mov	r7, fp
.L_081738fc:
	ldr	r0, [r7, #24]
	cmp	r0, #4
	bne.n	.L_08173914
	movs	r3, #3
	mov	r4, r8
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L_08173914
	movs	r0, #130
	bl	sub_081c0010
	ldr	r0, [r7, #24]
.L_08173914:
	cmp	r0, #15
	bhi.n	.L_08173966
	lsls	r0, r0, #11
	bl	sub_08002096
	ldr	r3, [r7, #8]
	adds	r6, r3, #0
	muls	r6, r0
	bl	sub_08014de4
	movs	r2, #0
	ldr	r1, [r7, #4]
	ldr	r0, [r7, #0]
	bl	sub_08015160
	movs	r0, #128
	asrs	r5, r6, #8
	lsls	r0, r0, #7
	lsrs	r6, r6, #31
	bl	sub_08015024
	adds	r5, r5, r6
	mov	r1, r9
	lsls	r0, r1, #9
	asrs	r5, r5, #1
	bl	sub_08015068
	adds	r0, r5, #0
	adds	r1, r5, #0
	adds	r2, r5, #0
	bl	sub_080151e4
	ldr	r0, [pc, #140]
	ldr	r1, [sp, #16]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
	ldr	r0, [r7, #24]
.L_08173966:
	movs	r2, #1
	adds	r3, r0, #1
	add	r8, r2
	str	r3, [r7, #24]
	mov	r3, r8
	adds	r7, #28
	cmp	r3, #16
	bne.n	.L_081738fc
.L_08173976:
	mov	r0, sl
	bl	sub_08013164
	ldr	r0, [sp, #16]
	bl	sub_08013164
	ldr	r2, [pc, #76]
	movs	r3, #120
	str	r3, [r2, #12]
	str	r3, [r2, #16]
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r4, #1
	add	r9, r4
	mov	r1, r9
	cmp	r1, #48
	beq.n	.L_081739ac
	b.n	.L_0817372a
.L_081739ac:
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #52]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #84
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x000000d5
	.4byte 0x030011e0
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199340
	.4byte 0xffffc000
	.4byte 0x08199210
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #0
	bl	sub_08173aac
	pop	{pc}
