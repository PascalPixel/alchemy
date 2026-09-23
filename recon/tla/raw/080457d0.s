.syntax unified
	.thumb
	.set sub_08002064, 0x08002064
	.set sub_08013164, 0x08013164
	.set sub_08013438, 0x08013438
	.set sub_08013560, 0x08013560
	.set sub_08013ba4, 0x08013ba4
	.set sub_0801401c, 0x0801401c
	.set sub_08014128, 0x08014128
	.set sub_08014274, 0x08014274
	.set sub_080143ac, 0x080143ac
	.set sub_080143e0, 0x080143e0
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014a24, 0x08014a24
	.set sub_08014dac, 0x08014dac
	.set sub_08039260, 0x08039260
	.set sub_0803939c, 0x0803939c
	.set sub_080393fc, 0x080393fc
	.set sub_08042010, 0x08042010
	.set sub_08042314, 0x08042314
	.set sub_0804519c, 0x0804519c
	.set sub_080454dc, 0x080454dc
	.set sub_080456f8, 0x080456f8
	.set sub_080457b0, 0x080457b0
	.set sub_080ad000, 0x080ad000
	.set sub_080ad078, 0x080ad078
	.set sub_080ad0f0, 0x080ad0f0
	.set sub_08118090, 0x08118090
	.set sub_08118128, 0x08118128
	.set sub_08118130, 0x08118130
	.set sub_081c0010, 0x081c0010
	.global Func_080457d0
	.thumb_func
Func_080457d0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #60
	str	r0, [sp, #52]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #60]
	mov	r8, r1
	str	r0, [sp, #48]
	movs	r0, #128
	lsls	r0, r0, #3
	bl	sub_080143e0
	movs	r2, #0
	movs	r1, #1
	str	r2, [sp, #40]
	str	r1, [sp, #44]
	adds	r7, r0, #0
	movs	r0, #240
	adds	r5, #228
	lsls	r0, r0, #1
	ldr	r5, [r5, #0]
	bl	sub_08014dac
	mov	r9, r0
	mov	r3, r9
	movs	r2, #255
	adds	r3, #255
	mov	ip, r9
.L_08045814:
	strb	r2, [r3, #0]
	subs	r3, #1
	cmp	r3, ip
	bge.n	.L_08045814
	movs	r2, #128
	lsls	r2, r2, #1
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r3, [sp, #52]
	cmp	r3, #0
	bne.n	.L_0804583a
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r0, #20
	movs	r3, #3
	movs	r1, #17
	movs	r2, #10
	b.n	.L_08045846
.L_0804583a:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r0, #22
	movs	r3, #3
	movs	r1, #17
	movs	r2, #8
.L_08045846:
	bl	sub_08039260
	movs	r3, #214
	lsls	r3, r3, #1
	add	r3, r9
	str	r0, [r3, #0]
	movs	r3, #226
	lsls	r3, r3, #1
	movs	r1, #1
	add	r3, r9
	negs	r1, r1
	str	r1, [r3, #0]
	ldr	r6, [sp, #52]
	cmp	r6, #0
	bne.n	.L_0804592c
	movs	r2, #228
	movs	r3, #14
	lsls	r2, r2, #1
	mov	r0, r9
	str	r3, [r0, r2]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r6, #1
	adds	r3, #68
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0804589c
	subs	r2, #196
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	cmp	r3, r1
	bne.n	.L_0804589c
	bl	sub_080ad0f0
	cmp	r0, #4
	ble.n	.L_0804589c
	movs	r2, #230
	lsls	r2, r2, #1
	movs	r3, #49
	mov	r5, r9
	str	r3, [r5, r2]
	movs	r6, #2
.L_0804589c:
	mov	r0, r8
	cmp	r0, #0
	ble.n	.L_080458c2
	ldr	r3, [pc, #132]
	movs	r1, #166
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r3, r3, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #2
	beq.n	.L_080458c2
	movs	r2, #228
	lsls	r3, r6, #2
	lsls	r2, r2, #1
	adds	r3, r3, r2
	mov	r5, r9
	movs	r2, #4
	str	r2, [r5, r3]
	adds	r6, #1
.L_080458c2:
	movs	r1, #228
	lsls	r3, r6, #2
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r2, #7
	mov	r0, r9
	adds	r6, #1
	str	r2, [r0, r3]
	lsls	r3, r6, #2
	adds	r3, r3, r1
	subs	r2, #8
	str	r2, [r0, r3]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r3, [r3, #0]
	movs	r2, #224
	ldr	r3, [r3, #60]
	lsls	r2, r2, #1
	add	r2, r9
	str	r3, [r2, #0]
	b.n	.L_080459b4
.L_080458ee:
	movs	r1, #2
	negs	r1, r1
	mov	sl, r1
	b.n	.L_08045ff8
.L_080458f6:
	movs	r3, #224
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r3, [r3, #0]
	movs	r2, #228
	lsls	r3, r3, #2
	lsls	r2, r2, #1
	adds	r3, r3, r2
	mov	r5, r9
	ldr	r3, [r5, r3]
	mov	sl, r3
	b.n	.L_08045ff8
.L_0804590e:
	movs	r6, #1
	movs	r0, #113
	negs	r6, r6
	bl	sub_081c0010
	mov	sl, r6
	b.n	.L_08045ff8
.L_0804591c:
	movs	r3, #228
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r3, [r3, #0]
	mov	sl, r3
	b.n	.L_08045ff8
	.2byte 0x0240
	.2byte 0x0200
.L_0804592c:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r3, [r3, #0]
	movs	r2, #224
	ldr	r3, [r3, #64]
	lsls	r2, r2, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r3, #228
	mov	r0, r9
	movs	r5, #0
	lsls	r3, r3, #1
	str	r5, [r0, r3]
	adds	r3, #4
	movs	r5, #1
	str	r5, [r0, r3]
	movs	r0, #0
	bl	sub_080ad000
	ldr	r3, [r0, #0]
	movs	r5, #2
	cmp	r3, #0
	beq.n	.L_08045992
	add	r5, sp, #56
	movs	r0, #0
	adds	r1, r5, #0
	bl	sub_08118130
	mov	r2, sp
	adds	r2, #59
.L_0804596a:
	ldrb	r3, [r5, #0]
	ldr	r1, [sp, #40]
	adds	r5, #1
	adds	r1, r1, r3
	str	r1, [sp, #40]
	cmp	r5, r2
	ble.n	.L_0804596a
	movs	r2, #232
	mov	r5, r9
	lsls	r2, r2, #1
	movs	r3, #15
	str	r3, [r5, r2]
	movs	r5, #3
	cmp	r1, #0
	beq.n	.L_08045992
	adds	r2, #4
	movs	r3, #16
	mov	r6, r9
	str	r3, [r6, r2]
	movs	r5, #4
.L_08045992:
	movs	r1, #228
	lsls	r1, r1, #1
	lsls	r3, r5, #2
	mov	r0, r9
	adds	r3, r3, r1
	movs	r2, #2
	adds	r5, #1
	str	r2, [r0, r3]
	lsls	r3, r5, #2
	adds	r3, r3, r1
	movs	r2, #3
	adds	r5, #1
	str	r2, [r0, r3]
	lsls	r3, r5, #2
	adds	r3, r3, r1
	subs	r2, #4
	str	r2, [r0, r3]
.L_080459b4:
	movs	r5, #136
	lsls	r5, r5, #1
	add	r5, r9
	strh	r7, [r5, #0]
	movs	r6, #228
	lsls	r6, r6, #1
	add	r6, r9
	ldr	r2, [r6, #0]
	movs	r5, #150
	movs	r6, #230
	movs	r3, #1
	movs	r1, #0
	lsls	r5, r5, #1
	lsls	r6, r6, #1
	negs	r3, r3
	mov	r8, r1
	add	r5, r9
	add	r6, r9
	cmp	r2, r3
	beq.n	.L_080459fc
.L_080459dc:
	mov	r1, r8
	mov	r0, r9
	bl	sub_080456f8
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	cmp	r1, #5
	bgt.n	.L_080459fc
	strh	r7, [r5, #0]
	movs	r3, #1
	ldmia	r6!, {r2}
	negs	r3, r3
	adds	r5, #28
	cmp	r2, r3
	bne.n	.L_080459dc
.L_080459fc:
	movs	r0, #216
	movs	r2, #218
	lsls	r0, r0, #1
	lsls	r2, r2, #1
	movs	r3, #160
	add	r0, r9
	mov	r5, r8
	add	r2, r9
	lsls	r3, r3, #1
	str	r5, [r0, #0]
	strh	r3, [r2, #0]
	movs	r2, #219
	lsls	r2, r2, #1
	subs	r3, #16
	add	r2, r9
	strh	r3, [r2, #0]
	adds	r3, #136
	movs	r1, #0
	add	r3, r9
	strh	r1, [r3, #0]
	mov	r8, r1
	ldr	r3, [r0, #0]
	cmp	r8, r3
	bge.n	.L_08045a6a
	movs	r1, #140
	adds	r4, r0, #0
	lsls	r1, r1, #1
	movs	r0, #138
	lsls	r0, r0, #1
	movs	r5, #136
	add	r1, r9
.L_08045a3a:
	ldr	r3, [r4, #0]
	mov	r6, r8
	subs	r3, r6, r3
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsls	r2, r2, #3
	adds	r3, r2, #0
	adds	r3, #155
	mov	r7, r9
	str	r3, [r0, r7]
	ldr	r3, [sp, #52]
	cmp	r3, #0
	beq.n	.L_08045a5a
	adds	r3, r2, #0
	adds	r3, #171
	str	r3, [r0, r7]
.L_08045a5a:
	str	r5, [r1, #0]
	movs	r6, #1
	ldr	r3, [r4, #0]
	add	r8, r6
	adds	r1, #28
	adds	r0, #28
	cmp	r8, r3
	blt.n	.L_08045a3a
.L_08045a6a:
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #140]
	bl	sub_080145a8
	ldr	r2, [pc, #136]
	movs	r0, #2
	movs	r1, #136
	bl	sub_08013438
	movs	r0, #216
	movs	r2, #224
	lsls	r0, r0, #1
	lsls	r2, r2, #1
	add	r0, r9
	add	r2, r9
	adds	r7, r0, #0
	adds	r1, r2, #0
	str	r7, [sp, #16]
	str	r1, [sp, #24]
.L_08045a92:
	ldr	r3, [pc, #112]
	ldr	r1, [pc, #112]
	ldr	r3, [r3, #0]
	movs	r2, #31
	lsls	r3, r3, #1
	ands	r3, r2
	lsls	r3, r3, #1
	ldrh	r1, [r1, r3]
	ldr	r5, [pc, #104]
	str	r1, [sp, #36]
	adds	r3, r1, r5
	cmp	r3, #0
	bge.n	.L_08045ab0
	adds	r3, r1, #0
	subs	r3, #253
.L_08045ab0:
	movs	r6, #152
	asrs	r3, r3, #2
	lsls	r6, r6, #1
	adds	r6, r3, r6
	str	r6, [sp, #36]
	add	r7, sp, #36
	ldrh	r7, [r7, #0]
	movs	r3, #218
	lsls	r3, r3, #1
	add	r3, r9
	strh	r7, [r3, #0]
	movs	r3, #219
	ldr	r2, [pc, #68]
	lsls	r3, r3, #1
	ldr	r1, [pc, #40]
	add	r3, r9
	adds	r0, r7, #0
	strh	r0, [r3, #0]
	movs	r3, #32
	strh	r1, [r2, #4]
	strh	r3, [r2, #6]
	ldr	r2, [sp, #44]
	cmp	r2, #0
	bne.n	.L_08045ae2
	b.n	.L_08045d24
.L_08045ae2:
	ldr	r5, [sp, #48]
	movs	r3, #0
	str	r3, [sp, #44]
	movs	r3, #1
	strb	r3, [r5, #6]
	ldr	r6, [sp, #52]
	cmp	r6, #0
	bne.n	.L_08045af4
	b.n	.L_08045c1c
.L_08045af4:
	b.n	.L_08045b14
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x08045769
	.4byte 0x08045781
	.4byte 0x0300122c
	.4byte 0x0805e9c4
	.4byte 0xffffff00
	.2byte 0x1120
	.2byte 0x0300
.L_08045b14:
	ldr	r0, [sp, #16]
	movs	r7, #0
	ldr	r3, [r0, #0]
	movs	r2, #6
	mov	lr, r7
	subs	r3, r2, r3
	cmp	lr, r3
	bge.n	.L_08045b7a
	movs	r3, #216
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r6, [pc, #28]
	subs	r2, r2, r3
	movs	r7, #3
	mov	ip, r2
	movs	r4, #0
.L_08045b36:
	mov	r2, lr
	ldr	r5, [sp, #48]
	adds	r3, r4, r2
	movs	r1, #0
	lsls	r3, r3, #1
	mov	r8, r1
	adds	r0, r3, r5
.L_08045b44:
	movs	r2, #0
	adds	r1, r0, #0
	b.n	.L_08045b50
	movs	r0, r0
	.2byte 0xf07f
	.2byte 0x0000
.L_08045b50:
	adds	r3, r2, #0
	ands	r3, r7
	lsls	r3, r3, #1
	movs	r5, #138
	adds	r3, r3, r1
	lsls	r5, r5, #3
	adds	r3, r3, r5
	adds	r2, #1
	strh	r6, [r3, #0]
	cmp	r2, #2
	ble.n	.L_08045b50
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r0, #64
	cmp	r2, #2
	ble.n	.L_08045b44
	add	lr, r1
	adds	r4, #2
	cmp	lr, ip
	blt.n	.L_08045b36
.L_08045b7a:
	ldr	r5, [sp, #16]
	movs	r3, #0
	mov	lr, r3
	ldr	r3, [r5, #0]
	cmp	lr, r3
	blt.n	.L_08045b88
	b.n	.L_08045d24
.L_08045b88:
	movs	r3, #216
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r3, [r3, #0]
	movs	r7, #0
	mov	fp, r3
	lsls	r3, r3, #1
	add	r3, fp
	mov	sl, r3
	mov	r6, sl
	lsls	r6, r6, #1
	str	r6, [sp, #32]
	str	r7, [sp, #8]
.L_08045ba2:
	ldr	r2, [sp, #8]
	ldr	r1, [sp, #48]
	add	r2, lr
	lsls	r3, r2, #1
	movs	r0, #0
	adds	r6, r2, #0
	mov	r2, lr
	mov	r8, r0
	adds	r4, r3, r1
	lsls	r5, r2, #4
.L_08045bb6:
	str	r5, [sp, #4]
	movs	r0, #0
	mov	ip, r6
.L_08045bbc:
	adds	r1, r0, #0
	movs	r3, #3
	ands	r1, r3
	mov	r7, ip
	adds	r2, r7, r1
	mov	r3, sl
	ldr	r7, [pc, #76]
	subs	r2, r2, r3
	lsls	r2, r2, #1
	adds	r2, r2, r7
	ldr	r7, [sp, #4]
	lsls	r1, r1, #1
	adds	r3, r7, r0
	movs	r7, #128
	lsls	r7, r7, #1
	adds	r3, r3, r7
	strh	r3, [r2, #0]
	ldr	r2, [sp, #32]
	movs	r3, #128
	adds	r1, r4, r1
	lsls	r3, r3, #3
	ldr	r7, [pc, #44]
	subs	r1, r1, r2
	adds	r3, #116
	adds	r1, r1, r3
	adds	r0, #1
	strh	r7, [r1, #0]
	cmp	r0, #2
	ble.n	.L_08045bbc
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r4, #64
	adds	r6, #32
	adds	r5, #4
	cmp	r1, #2
	ble.n	.L_08045bb6
	ldr	r2, [sp, #8]
	add	lr, r0
	adds	r2, #2
	str	r2, [sp, #8]
	cmp	lr, fp
	blt.n	.L_08045ba2
	b.n	.L_08045d24
	.4byte 0x00000000
	.2byte 0xfd6c
	.2byte 0x0600
.L_08045c1c:
	ldr	r5, [sp, #16]
	movs	r3, #0
	mov	lr, r3
	ldr	r3, [r5, #0]
	movs	r2, #6
	subs	r3, r2, r3
	cmp	lr, r3
	bge.n	.L_08045c84
	movs	r3, #216
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r6, [pc, #28]
	subs	r2, r2, r3
	movs	r7, #3
	mov	ip, r2
	movs	r4, #0
.L_08045c3e:
	mov	r1, lr
	ldr	r2, [sp, #48]
	adds	r3, r4, r1
	movs	r0, #0
	lsls	r3, r3, #1
	mov	r8, r0
	adds	r0, r3, r2
.L_08045c4c:
	movs	r2, #0
	adds	r1, r0, #0
	b.n	.L_08045c58
	movs	r0, r0
	.2byte 0xf07f
	.2byte 0x0000
.L_08045c58:
	adds	r3, r2, #0
	ands	r3, r7
	movs	r5, #128
	lsls	r3, r3, #1
	lsls	r5, r5, #3
	adds	r3, r3, r1
	adds	r5, #76
	adds	r3, r3, r5
	adds	r2, #1
	strh	r6, [r3, #0]
	cmp	r2, #2
	ble.n	.L_08045c58
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r0, #64
	cmp	r2, #2
	ble.n	.L_08045c4c
	add	lr, r1
	adds	r4, #2
	cmp	lr, ip
	blt.n	.L_08045c3e
.L_08045c84:
	ldr	r5, [sp, #16]
	movs	r3, #0
	mov	lr, r3
	ldr	r3, [r5, #0]
	cmp	lr, r3
	bge.n	.L_08045d24
	movs	r3, #216
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r3, [r3, #0]
	movs	r7, #0
	mov	fp, r3
	lsls	r3, r3, #1
	add	r3, fp
	mov	sl, r3
	mov	r6, sl
	lsls	r6, r6, #1
	str	r6, [sp, #28]
	str	r7, [sp, #12]
.L_08045caa:
	ldr	r2, [sp, #12]
	ldr	r1, [sp, #48]
	add	r2, lr
	lsls	r3, r2, #1
	movs	r0, #0
	adds	r6, r2, #0
	mov	r2, lr
	mov	r8, r0
	adds	r4, r3, r1
	lsls	r5, r2, #4
.L_08045cbe:
	str	r5, [sp, #4]
	movs	r0, #0
	mov	ip, r6
.L_08045cc4:
	adds	r1, r0, #0
	movs	r3, #3
	ands	r1, r3
	mov	r7, ip
	adds	r2, r7, r1
	mov	r3, sl
	ldr	r7, [pc, #76]
	subs	r2, r2, r3
	lsls	r2, r2, #1
	adds	r2, r2, r7
	ldr	r7, [sp, #4]
	lsls	r1, r1, #1
	adds	r3, r7, r0
	movs	r7, #128
	lsls	r7, r7, #1
	adds	r3, r3, r7
	strh	r3, [r2, #0]
	ldr	r2, [sp, #28]
	adds	r1, r4, r1
	movs	r3, #142
	ldr	r7, [pc, #44]
	subs	r1, r1, r2
	lsls	r3, r3, #3
	adds	r1, r1, r3
	adds	r0, #1
	strh	r7, [r1, #0]
	cmp	r0, #2
	ble.n	.L_08045cc4
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r4, #64
	adds	r6, #32
	adds	r5, #4
	cmp	r1, #2
	ble.n	.L_08045cbe
	ldr	r2, [sp, #12]
	add	lr, r0
	adds	r2, #2
	str	r2, [sp, #12]
	cmp	lr, fp
	blt.n	.L_08045caa
	b.n	.L_08045d24
	movs	r0, r0
	.4byte 0x00000000
	.2byte 0xfd68
	.2byte 0x0600
.L_08045d24:
	movs	r7, #226
	movs	r6, #224
	lsls	r7, r7, #1
	lsls	r6, r6, #1
	add	r7, r9
	add	r6, r9
	ldr	r2, [r7, #0]
	ldr	r3, [r6, #0]
	cmp	r2, r3
	beq.n	.L_08045d7c
	movs	r5, #214
	lsls	r5, r5, #1
	add	r5, r9
	ldr	r0, [r5, #0]
	bl	sub_080393fc
	ldr	r2, [r6, #0]
	movs	r0, #142
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r0, r0, #1
	lsls	r3, r3, #2
	adds	r3, r3, r0
	mov	r1, r9
	ldr	r0, [r1, r3]
	ldr	r3, [pc, #88]
	ldr	r1, [r5, #0]
	adds	r0, r0, r3
	movs	r2, #0
	movs	r3, #0
	bl	sub_08042010
	ldr	r3, [r6, #0]
	movs	r2, #228
	str	r3, [r7, #0]
	lsls	r2, r2, #1
	ldr	r1, [r6, #0]
	mov	r5, r9
	lsls	r3, r1, #2
	adds	r3, r3, r2
	ldr	r2, [r5, r3]
	mov	r0, r9
	bl	sub_080456f8
.L_08045d7c:
	movs	r0, #218
	lsls	r0, r0, #1
	add	r0, r9
	bl	sub_0801401c
	movs	r3, #222
	lsls	r3, r3, #1
	add	r3, r9
	str	r0, [r3, #0]
	ldr	r7, [sp, #16]
	movs	r6, #0
	ldr	r3, [r7, #0]
	mov	r8, r6
	cmp	r8, r3
	bge.n	.L_08045e56
	movs	r5, #130
	movs	r0, #63
	ldr	r6, [pc, #12]
	lsls	r5, r5, #1
	negs	r0, r0
	add	r5, r9
	adds	r7, r0, #0
	b.n	.L_08045db4
	movs	r0, r0
	.4byte 0xfffffe00
	.2byte 0x003a
	.2byte 0x0000
.L_08045db4:
	ldr	r1, [sp, #24]
	ldr	r3, [r1, #0]
	cmp	r8, r3
	bne.n	.L_08045e24
	movs	r3, #222
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r2, [r3, #0]
	ldrb	r3, [r5, #7]
	movs	r1, #31
	ands	r2, r1
	lsls	r2, r2, #1
	ands	r3, r7
	orrs	r3, r2
	strb	r3, [r5, #7]
	ldrb	r3, [r5, #5]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r5, #5]
	ldr	r2, [sp, #36]
	ldr	r1, [r5, #16]
	lsls	r3, r2, #3
	subs	r2, r3, r2
	cmp	r2, #0
	bge.n	.L_08045dee
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r2, r3
.L_08045dee:
	ldr	r3, [pc, #48]
	asrs	r2, r2, #9
	adds	r2, r1, r2
	subs	r2, #14
	ands	r2, r3
	ldrh	r3, [r5, #6]
	ands	r3, r6
	orrs	r3, r2
	strh	r3, [r5, #6]
	ldr	r0, [sp, #36]
	ldr	r2, [r5, #20]
	lsls	r3, r0, #1
	adds	r3, r3, r0
	cmp	r3, #0
	bge.n	.L_08045e0e
	adds	r3, #255
.L_08045e0e:
	asrs	r3, r3, #8
	adds	r3, r2, r3
	subs	r3, #20
	strb	r3, [r5, #4]
	adds	r0, r5, #0
	movs	r1, #241
	bl	sub_08014128
	b.n	.L_08045e48
	.2byte 0x01ff
	.2byte 0x0000
.L_08045e24:
	ldr	r3, [pc, #52]
	ldr	r2, [r5, #16]
	movs	r1, #4
	ands	r2, r3
	ldrh	r3, [r5, #6]
	negs	r1, r1
	ands	r3, r6
	orrs	r3, r2
	strh	r3, [r5, #6]
	ldr	r3, [r5, #20]
	adds	r2, r1, #0
	strb	r3, [r5, #4]
	ldrb	r3, [r5, #7]
	ands	r3, r7
	strb	r3, [r5, #7]
	ldrb	r3, [r5, #5]
	ands	r3, r2
	strb	r3, [r5, #5]
.L_08045e48:
	ldr	r0, [sp, #16]
	movs	r2, #1
	ldr	r3, [r0, #0]
	add	r8, r2
	adds	r5, #28
	cmp	r8, r3
	blt.n	.L_08045db4
.L_08045e56:
	ldr	r3, [pc, #8]
	movs	r4, #192
	b.n	.L_08045e64
	.4byte 0x000001ff
	.2byte 0x1150
	.2byte 0x0300
.L_08045e64:
	lsls	r4, r4, #18
	ldr	r5, [r3, #4]
	ldr	r7, [r3, #12]
	adds	r3, r4, #0
	adds	r3, #228
	ldr	r2, [r3, #0]
	movs	r1, #216
	adds	r1, r1, r2
	mov	r8, r1
	ldr	r1, [r1, #0]
	cmp	r1, #0
	beq.n	.L_08045f64
	adds	r6, r2, #0
	adds	r6, #220
	ldr	r3, [r6, #0]
	movs	r7, #0
	movs	r5, #0
	cmp	r3, #0
	bne.n	.L_08045f60
	adds	r3, r2, #0
	adds	r3, #224
	ldr	r2, [r3, #0]
	subs	r3, r2, #5
	cmp	r3, #1
	bhi.n	.L_08045eb8
	ldr	r2, [sp, #24]
	movs	r5, #228
	ldr	r3, [r2, #0]
	lsls	r5, r5, #1
	lsls	r3, r3, #2
	adds	r3, r3, r5
	mov	r7, r9
	ldr	r3, [r7, r3]
	cmp	r3, #3
	bne.n	.L_08045eb0
	movs	r7, #1
	movs	r5, #1
	b.n	.L_08045eb4
.L_08045eb0:
	movs	r7, #32
	movs	r5, #32
.L_08045eb4:
	movs	r3, #30
	b.n	.L_08045f62
.L_08045eb8:
	cmp	r2, #4
	bne.n	.L_08045f56
	movs	r0, #224
	lsls	r0, r0, #1
	add	r0, r9
	ldr	r3, [r0, #0]
	movs	r2, #228
	lsls	r2, r2, #1
	lsls	r3, r3, #2
	adds	r3, r3, r2
	mov	r2, r9
	ldr	r3, [r2, r3]
	cmp	r3, #16
	beq.n	.L_08045ede
	ldr	r2, [sp, #40]
	cmp	r2, #0
	bne.n	.L_08045f4e
	cmp	r3, #15
	bne.n	.L_08045f4e
.L_08045ede:
	cmp	r1, #1
	bne.n	.L_08045f46
	ldr	r1, [r4, #60]
	movs	r2, #4
	ldrb	r3, [r1, #5]
	orrs	r3, r2
	strb	r3, [r1, #5]
	mov	r1, r9
	ldr	r3, [r0, #0]
	movs	r0, #228
	lsls	r3, r3, #2
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r3, [r1, r3]
	cmp	r3, #15
	bne.n	.L_08045f08
	bl	sub_0804519c
	movs	r1, #15
	adds	r0, #10
	b.n	.L_08045f14
.L_08045f08:
	cmp	r3, #16
	bne.n	.L_08045f1c
	bl	sub_0804519c
	movs	r1, #15
	adds	r0, #9
.L_08045f14:
	movs	r2, #8
	bl	sub_080457b0
	str	r0, [sp, #20]
.L_08045f1c:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #60]
	movs	r3, #251
	ldrb	r2, [r1, #5]
	movs	r0, #102
	ands	r3, r2
	strb	r3, [r1, #5]
	movs	r1, #155
	bl	sub_08118128
	movs	r1, #1
	ldr	r0, [sp, #20]
	bl	sub_0803939c
	mov	r2, r8
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	movs	r3, #45
	b.n	.L_08045f62
.L_08045f46:
	movs	r3, #200
	movs	r7, #1
	movs	r5, #1
	b.n	.L_08045f62
.L_08045f4e:
	movs	r3, #40
	movs	r7, #16
	movs	r5, #16
	b.n	.L_08045f62
.L_08045f56:
	movs	r3, #60
	str	r3, [r6, #0]
	movs	r7, #1
	movs	r5, #1
	b.n	.L_08045f64
.L_08045f60:
	subs	r3, #1
.L_08045f62:
	str	r3, [r6, #0]
.L_08045f64:
	movs	r3, #192
	lsls	r3, r3, #2
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_08045f76
	ldr	r3, [sp, #52]
	cmp	r3, #0
	beq.n	.L_08045f76
	b.n	.L_080458ee
.L_08045f76:
	movs	r3, #1
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_08045f80
	b.n	.L_080458f6
.L_08045f80:
	movs	r3, #2
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_08045f8a
	b.n	.L_0804590e
.L_08045f8a:
	movs	r3, #144
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08045faa
	movs	r0, #111
	bl	sub_081c0010
	ldr	r5, [sp, #24]
	ldr	r6, [sp, #16]
	ldr	r0, [r5, #0]
	ldr	r1, [r6, #0]
	adds	r0, #1
	bl	sub_08002064
	str	r0, [r5, #0]
	b.n	.L_08045fdc
.L_08045faa:
	movs	r3, #96
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08045fcc
	movs	r0, #111
	bl	sub_081c0010
	ldr	r7, [sp, #24]
	ldr	r2, [sp, #16]
	ldr	r0, [r7, #0]
	ldr	r1, [r2, #0]
	adds	r0, r0, r1
	subs	r0, #1
	bl	sub_08002064
	str	r0, [r7, #0]
	b.n	.L_08045fdc
.L_08045fcc:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #76]
	cmp	r3, #0
	bne.n	.L_08045fdc
	b.n	.L_0804591c
.L_08045fdc:
	movs	r1, #184
	movs	r0, #128
	lsls	r1, r1, #5
	lsls	r0, r0, #19
	adds	r1, #65
	bl	sub_08013ba4
	ldr	r5, [sp, #48]
	movs	r3, #0
	strb	r3, [r5, #6]
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_08045a92
.L_08045ff8:
	ldr	r6, [sp, #52]
	cmp	r6, #0
	beq.n	.L_08046012
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r2, [r3, #0]
	movs	r3, #224
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r3, [r3, #0]
	str	r3, [r2, #64]
	b.n	.L_08046024
.L_08046012:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r2, [r3, #0]
	movs	r3, #224
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r3, [r3, #0]
	str	r3, [r2, #60]
.L_08046024:
	movs	r3, #216
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r3, [r3, #0]
	movs	r7, #0
	mov	r8, r7
	cmp	r8, r3
	bge.n	.L_08046052
	movs	r6, #216
	movs	r5, #136
	lsls	r6, r6, #1
	lsls	r5, r5, #1
	add	r6, r9
	add	r5, r9
.L_08046040:
	ldrh	r0, [r5, #0]
	bl	sub_08014274
	ldr	r3, [r6, #0]
	movs	r0, #1
	add	r8, r0
	adds	r5, #28
	cmp	r8, r3
	blt.n	.L_08046040
.L_08046052:
	ldr	r1, [sp, #48]
	movs	r3, #1
	strb	r3, [r1, #6]
	movs	r3, #214
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	sub_0803939c
	movs	r3, #240
	movs	r7, #128
	lsls	r3, r3, #8
	movs	r2, #3
	lsls	r7, r7, #3
	adds	r3, #127
	movs	r4, #0
	mov	ip, r2
	movs	r5, #0
	adds	r7, #82
	mov	lr, r3
.L_0804607c:
	ldr	r0, [sp, #48]
	adds	r3, r5, r4
	movs	r6, #0
	lsls	r3, r3, #1
	mov	r8, r6
	adds	r1, r3, r0
.L_08046088:
	movs	r2, #0
	adds	r0, r1, #0
.L_0804608c:
	adds	r3, r2, #0
	mov	r6, ip
	ands	r3, r6
	lsls	r3, r3, #1
	adds	r3, r3, r0
	adds	r3, r3, r7
	mov	r6, lr
	adds	r2, #1
	strh	r6, [r3, #0]
	cmp	r2, #2
	ble.n	.L_0804608c
	movs	r0, #1
	add	r8, r0
	mov	r2, r8
	adds	r1, #64
	cmp	r2, #2
	ble.n	.L_08046088
	adds	r4, #1
	adds	r5, #2
	cmp	r4, #6
	ble.n	.L_0804607c
	ldr	r5, [sp, #48]
	movs	r3, #1
	strb	r3, [r5, #3]
	bl	sub_08013560
	ldr	r0, [pc, #100]
	bl	sub_08014644
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	bl	sub_08013438
	ldr	r1, [pc, #88]
	ldr	r0, [pc, #92]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_08046102
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #168
	adds	r3, r3, r1
	lsls	r2, r2, #5
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_08046102:
	strh	r4, [r0, #0]
	mov	r0, r9
	bl	sub_08013164
	ldr	r6, [sp, #48]
	movs	r3, #0
	strb	r3, [r6, #6]
	movs	r0, #1
	bl	sub_08013560
	mov	r0, sl
	add	sp, #60
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x08045769
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
.L_08046134:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r4, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	ldr	r7, [sp, #20]
	mov	lr, r3
	movs	r3, #1
	ands	r7, r3
	adds	r6, r2, #0
	lsls	r7, r7, #12
	cmp	r0, #0
	bge.n	.L_08046156
	adds	r6, r6, r0
	movs	r0, #0
.L_08046156:
	adds	r3, r0, r6
	cmp	r3, #29
	ble.n	.L_08046160
	movs	r3, #30
	subs	r6, r3, r0
.L_08046160:
	cmp	r1, #0
	bge.n	.L_08046168
	adds	r4, r4, r1
	movs	r1, #0
.L_08046168:
	adds	r3, r1, r4
	cmp	r3, #29
	ble.n	.L_08046172
	movs	r3, #20
	subs	r4, r3, r1
.L_08046172:
	cmp	r6, #0
	ble.n	.L_080461be
	cmp	r4, #0
	ble.n	.L_080461be
	lsls	r3, r0, #1
	lsls	r2, r1, #6
	add	r3, lr
	movs	r0, #2
	adds	r2, r2, r3
	mov	r8, r0
	mov	ip, r2
.L_08046188:
	mov	r0, ip
	adds	r2, r6, #0
	adds	r0, #8
	cmp	r2, #0
	beq.n	.L_080461a4
	ldr	r5, [pc, #48]
.L_08046194:
	ldrh	r3, [r0, #0]
	subs	r2, #1
	ands	r3, r5
	orrs	r3, r7
	strh	r3, [r0, #0]
	adds	r0, #2
	cmp	r2, #0
	bne.n	.L_08046194
.L_080461a4:
	lsrs	r3, r1, #2
	mov	r0, lr
	mov	r2, r8
	lsls	r2, r3
	ldrb	r3, [r0, #3]
	orrs	r2, r3
	strb	r2, [r0, #3]
	subs	r4, #1
	movs	r2, #64
	add	ip, r2
	adds	r1, #1
	cmp	r4, #0
	bne.n	.L_08046188
.L_080461be:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0xefff
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r5, r0, #0
	movs	r1, #0
	movs	r0, #1
	sub	sp, #4
	ldr	r7, [r3, #36]
	bl	sub_08118090
	mov	r8, r0
	lsls	r3, r0, #1
	add	r3, r8
	lsls	r3, r3, #1
	movs	r0, #29
	subs	r0, r0, r3
	movs	r3, #15
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r3, #5
	movs	r2, #25
	bl	.L_08046134
	ldrh	r3, [r5, #0]
	movs	r6, #0
	cmp	r3, #255
	beq.n	.L_08046256
	movs	r0, #0
.L_08046204:
	movs	r3, #88
	ldrsh	r2, [r7, r3]
	ldrh	r3, [r0, r5]
	movs	r1, #0
	b.n	.L_0804621c
.L_0804620e:
	adds	r1, #1
	cmp	r1, #3
	bgt.n	.L_08046226
	lsls	r3, r1, #1
	adds	r3, #88
	ldrsh	r2, [r7, r3]
	ldrh	r3, [r0, r5]
.L_0804621c:
	cmp	r2, r3
	beq.n	.L_08046226
	cmp	r2, #255
	bne.n	.L_0804620e
	movs	r1, #4
.L_08046226:
	cmp	r1, #4
	beq.n	.L_08046246
	mov	r3, r8
	subs	r2, r3, r1
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
	movs	r0, #29
	subs	r0, r0, r3
	movs	r3, #14
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #7
	movs	r3, #5
	bl	.L_08046134
.L_08046246:
	adds	r6, #1
	cmp	r6, #3
	bgt.n	.L_08046256
	lsls	r3, r6, #1
	adds	r0, r3, #0
	ldrh	r3, [r0, r5]
	cmp	r3, #255
	bne.n	.L_08046204
.L_08046256:
	movs	r0, #0
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r0, r1, #0
	bl	sub_08014a24
	ldr	r1, [pc, #20]
	movs	r2, #13
.L_0804626e:
	ldrb	r3, [r1, #0]
	subs	r2, #1
	strh	r3, [r5, #0]
	adds	r1, #1
	adds	r5, #2
	cmp	r2, #0
	bge.n	.L_0804626e
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0x1250
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	mov	r8, r1
	movs	r1, #0
	str	r2, [sp, #12]
	str	r3, [sp, #8]
	str	r1, [sp, #4]
	str	r1, [sp, #0]
	mov	r2, r8
	ldrh	r3, [r2, #0]
	mov	sl, r0
	mov	r9, r1
	cmp	r3, #0
	beq.n	.L_0804632e
	movs	r3, #252
	ldr	r6, [sp, #12]
	lsls	r3, r3, #6
	adds	r3, #255
	mov	fp, r3
	mov	r5, r8
	subs	r6, #2
.L_080462ba:
	ldrh	r0, [r5, #0]
	bl	sub_080ad078
	ldrb	r2, [r0, #1]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804631e
	ldrh	r2, [r5, #0]
	mov	r3, fp
	ands	r3, r2
	strh	r3, [r6, #2]
	movs	r1, #1
	add	r9, r1
	mov	r1, sl
	ldrh	r2, [r1, #0]
	ldrh	r3, [r5, #0]
	adds	r6, #2
	eors	r3, r2
	mov	r2, fp
	ands	r3, r2
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_08046302
	ldr	r7, [pc, #40]
	adds	r4, r5, #0
.L_080462ee:
	adds	r0, #1
	cmp	r0, #31
	bgt.n	.L_08046302
	adds	r1, #4
	ldrh	r3, [r4, #0]
	ldrh	r2, [r1, #0]
	eors	r3, r2
	ands	r3, r7
	cmp	r3, #0
	bne.n	.L_080462ee
.L_08046302:
	cmp	r0, #32
	bne.n	.L_0804631e
	ldr	r3, [sp, #4]
	ldr	r2, [pc, #12]
	adds	r3, #1
	str	r3, [sp, #4]
	ldrh	r3, [r6, #0]
	orrs	r3, r2
	b.n	.L_0804631c
	.4byte 0x00003fff
	.2byte 0x8000
	.2byte 0x0000
.L_0804631c:
	strh	r3, [r6, #0]
.L_0804631e:
	mov	r3, r8
	adds	r5, #4
	adds	r3, #124
	cmp	r5, r3
	bgt.n	.L_0804632e
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_080462ba
.L_0804632e:
	mov	r1, sl
	ldrh	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_080463b8
	mov	r2, r9
	lsls	r3, r2, #1
	ldr	r1, [sp, #12]
	movs	r2, #252
	lsls	r2, r2, #6
	adds	r2, #255
	mov	r5, sl
	adds	r7, r3, r1
	mov	fp, r2
.L_08046348:
	ldrh	r0, [r5, #0]
	bl	sub_080ad078
	ldrb	r2, [r0, #1]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080463a8
	mov	r1, r8
	ldrh	r2, [r1, #0]
	ldrh	r3, [r5, #0]
	movs	r0, #0
	eors	r3, r2
	mov	r2, fp
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08046382
	ldr	r6, [pc, #40]
	adds	r4, r5, #0
.L_0804636e:
	adds	r0, #1
	cmp	r0, #31
	bgt.n	.L_08046382
	adds	r1, #4
	ldrh	r3, [r4, #0]
	ldrh	r2, [r1, #0]
	eors	r3, r2
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L_0804636e
.L_08046382:
	cmp	r0, #32
	bne.n	.L_080463a8
	ldr	r3, [sp, #0]
	ldr	r2, [pc, #12]
	adds	r3, #1
	str	r3, [sp, #0]
	mov	r1, fp
	ldrh	r3, [r5, #0]
	b.n	.L_0804639c
	.4byte 0x00003fff
	.2byte 0x4000
	.2byte 0x0000
.L_0804639c:
	ands	r3, r1
	orrs	r3, r2
	movs	r2, #1
	strh	r3, [r7, #0]
	add	r9, r2
	adds	r7, #2
.L_080463a8:
	mov	r3, sl
	adds	r5, #4
	adds	r3, #124
	cmp	r5, r3
	bgt.n	.L_080463b8
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_08046348
.L_080463b8:
	ldr	r3, [sp, #4]
	ldr	r1, [sp, #8]
	mov	r0, r9
	str	r3, [r1, #0]
	ldr	r3, [sp, #48]
	ldr	r2, [sp, #0]
	add	sp, #16
	str	r2, [r3, #0]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #4
	mov	r8, r0
	mov	sl, r1
	adds	r7, r2, #0
	adds	r6, r3, #0
	bl	sub_080143ac
	adds	r5, r0, #0
	cmp	r5, #96
	beq.n	.L_08046408
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_080454dc
	movs	r1, #128
	lsls	r1, r1, #23
	adds	r0, r5, #0
	mov	r2, r8
	mov	r3, sl
	str	r7, [sp, #0]
	bl	sub_08042314
.L_08046408:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.align 2, 0
