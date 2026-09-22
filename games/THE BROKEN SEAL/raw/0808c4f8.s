.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080040e8, 0x080040e8
	.set sub_080048f4, 0x080048f4
	.set sub_080072e4, 0x080072e4
	.set sub_08009078, 0x08009078
	.set sub_08009080, 0x08009080
	.set sub_080090a0, 0x080090a0
	.set sub_080090a8, 0x080090a8
	.set sub_080090b0, 0x080090b0
	.set sub_080090b8, 0x080090b8
	.set sub_08009110, 0x08009110
	.set sub_08009118, 0x08009118
	.set sub_08009128, 0x08009128
	.set sub_08009130, 0x08009130
	.set sub_08009138, 0x08009138
	.set sub_08009140, 0x08009140
	.set sub_08015000, 0x08015000
	.set sub_08015040, 0x08015040
	.set sub_08015120, 0x08015120
	.set sub_080151e8, 0x080151e8
	.set sub_08015200, 0x08015200
	.set sub_08015208, 0x08015208
	.set sub_08015288, 0x08015288
	.set sub_08015290, 0x08015290
	.set sub_08015370, 0x08015370
	.set sub_08077008, 0x08077008
	.set sub_080770c0, 0x080770c0
	.set sub_080770c8, 0x080770c8
	.set sub_080770d0, 0x080770d0
	.set sub_080772f0, 0x080772f0
	.set sub_0808ace0, 0x0808ace0
	.set sub_0808b1d8, 0x0808b1d8
	.set sub_0808b674, 0x0808b674
	.set sub_0808ba38, 0x0808ba38
	.set sub_0808bb2c, 0x0808bb2c
	.set sub_0808bc44, 0x0808bc44
	.set sub_0808bc9c, 0x0808bc9c
	.set sub_0808bec0, 0x0808bec0
	.set sub_0808c44c, 0x0808c44c
	.set sub_0808c4c0, 0x0808c4c0
	.set sub_0808ce74, 0x0808ce74
	.set sub_0808cf78, 0x0808cf78
	.set sub_0808d0c8, 0x0808d0c8
	.set sub_0808d5a4, 0x0808d5a4
	.set sub_0808d5dc, 0x0808d5dc
	.set sub_0808d7d8, 0x0808d7d8
	.set sub_0808d828, 0x0808d828
	.set sub_0808d8f0, 0x0808d8f0
	.set sub_0808d9a4, 0x0808d9a4
	.set sub_0808ddec, 0x0808ddec
	.set sub_0808e118, 0x0808e118
	.set sub_0808e23c, 0x0808e23c
	.set sub_0808e680, 0x0808e680
	.set sub_0808e9c0, 0x0808e9c0
	.set sub_0808fefc, 0x0808fefc
	.set sub_080901c0, 0x080901c0
	.set sub_08091174, 0x08091174
	.set sub_08091200, 0x08091200
	.set sub_08091660, 0x08091660
	.set sub_080919d8, 0x080919d8
	.set sub_08091a58, 0x08091a58
	.set sub_080941e0, 0x080941e0
	.set sub_08094428, 0x08094428
	.set sub_08095680, 0x08095680
	.set sub_08095778, 0x08095778
	.set sub_08099810, 0x08099810
	.set sub_0809c138, 0x0809c138
	.set sub_0809c3a4, 0x0809c3a4
	.set sub_080f9010, 0x080f9010
	.set sub_080f9070, 0x080f9070
	.global Overlay_0808c4f8
Overlay_0808c4f8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r1, [pc, #744]
	movs	r0, #27
	sub	sp, #16
	bl	sub_080048f4
	movs	r7, #0
	mov	r8, r0
	add	r0, sp, #12
	str	r7, [r0, #0]
	ldr	r3, [pc, #728]
	mov	r1, r8
	ldr	r2, [pc, #728]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #728]
	bl	sub_080770d0
	ldr	r5, [pc, #724]
	movs	r1, #224
	lsls	r1, r1, #1
	adds	r3, r5, r1
	movs	r4, #228
	ldrh	r2, [r3, #0]
	lsls	r4, r4, #1
	adds	r3, r5, r4
	strh	r2, [r3, #0]
	adds	r1, #2
	adds	r3, r5, r1
	ldrh	r3, [r3, #0]
	adds	r4, #2
	adds	r2, r5, r4
	strh	r3, [r2, #0]
	adds	r1, #12
	ldr	r3, [pc, #696]
	adds	r2, r5, r1
	strh	r3, [r2, #0]
	movs	r3, #232
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #1
	negs	r3, r3
	adds	r4, #8
	strh	r3, [r2, #0]
	adds	r1, #6
	adds	r2, r5, r4
	strh	r3, [r2, #0]
	adds	r2, r5, r1
	strh	r3, [r2, #0]
	bl	sub_080040e8
	movs	r0, #0
	bl	sub_08095778
	movs	r2, #237
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	ldr	r3, [pc, #652]
	cmp	r0, r3
	bne.n	.L_0808c598
	bl	sub_08009118
	movs	r1, #137
	lsls	r1, r1, #2
	adds	r2, r5, r1
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r0, #0
	movs	r6, #3
	bl	sub_0808ace0
	b.n	.L_0808c5a4
.L_0808c598:
	bl	sub_08009110
	movs	r0, #1
	movs	r6, #2
	bl	sub_0808ace0
.L_0808c5a4:
	movs	r3, #207
	lsls	r3, r3, #1
	add	r3, r8
	strh	r6, [r3, #0]
	adds	r0, r6, #0
	bl	sub_08009078
	bl	sub_08015000
	bl	sub_0808bc44
	ldr	r5, [pc, #592]
	ldr	r0, [r5, #36]
	bl	sub_080072e4
	mov	r2, r8
	str	r0, [r2, #16]
	bl	sub_0808cf78
	ldr	r0, [r5, #28]
	bl	sub_080072e4
	bl	sub_0808b674
	ldr	r0, [pc, #568]
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_0808c5e2
	bl	sub_0808bb2c
.L_0808c5e2:
	ldr	r5, [pc, #540]
	movs	r4, #141
	lsls	r4, r4, #2
	adds	r3, r5, r4
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_0808c5f6
	bl	sub_08095680
.L_0808c5f6:
	cmp	r6, #3
	bne.n	.L_0808c614
	bl	sub_08009130
	movs	r2, #238
	lsls	r2, r2, #1
	movs	r4, #242
	adds	r3, r5, r2
	lsls	r4, r4, #1
	ldr	r0, [r3, #0]
	adds	r3, r5, r4
	ldr	r1, [r3, #0]
	bl	sub_08009138
	b.n	.L_0808c618
.L_0808c614:
	bl	sub_08009128
.L_0808c618:
	bl	sub_0808e9c0
	bl	sub_08091174
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl	sub_08091200
	movs	r2, #224
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #128
	adds	r1, r2, #0
	lsls	r3, r3, #1
	str	r1, [sp, #8]
	str	r3, [r2, #0]
	adds	r3, #200
	add	r3, r8
	movs	r4, #16
	movs	r7, #227
	movs	r2, #216
	str	r4, [r3, #0]
	mov	sl, r3
	lsls	r7, r7, #1
	ldr	r3, [pc, #456]
	lsls	r2, r2, #1
	movs	r6, #0
	add	r7, r8
	add	r2, r8
	strh	r6, [r7, #0]
	str	r3, [r2, #0]
	adds	r3, #27
	add	r3, r8
	ldr	r5, [pc, #416]
	ldr	r1, [pc, #440]
	str	r6, [r3, #0]
	adds	r3, r5, r1
	mov	fp, r4
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #2
	bne.n	.L_0808c678
	movs	r0, #162
	str	r6, [r2, #0]
	lsls	r0, r0, #1
	bl	sub_080770c8
.L_0808c678:
	movs	r3, #222
	ldr	r1, [pc, #404]
	lsls	r3, r3, #1
	ldr	r2, [pc, #412]
	add	r3, r8
	mov	r9, r1
	str	r2, [r3, #0]
	mov	r0, r9
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_0808c6bc
	bl	sub_0808b1d8
	movs	r2, #139
	lsls	r2, r2, #2
	ldr	r1, [pc, #388]
	adds	r3, r5, r2
	mov	r4, fp
	strh	r4, [r3, #0]
	adds	r3, r5, r1
	strh	r6, [r3, #0]
	movs	r3, #140
	lsls	r3, r3, #2
	adds	r2, r5, r3
	ldr	r4, [pc, #376]
	movs	r3, #1
	strh	r3, [r2, #0]
	ldr	r3, [pc, #336]
	adds	r2, r5, r4
	adds	r1, #30
	strh	r3, [r2, #0]
	adds	r3, r5, r1
	strh	r6, [r3, #0]
.L_0808c6bc:
	bl	sub_08099810
	ldr	r2, [pc, #356]
	ldr	r3, [pc, #320]
	add	r2, r8
	strh	r3, [r2, #0]
	ldr	r3, [pc, #320]
	ldr	r0, [r3, #4]
	bl	sub_080072e4
	movs	r3, #184
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	cmp	r0, #0
	beq.n	.L_0808c6ec
	adds	r7, r0, #0
	strh	r6, [r3, #0]
	b.n	.L_0808ce1c
.L_0808c6e4:
	movs	r3, #0
	adds	r7, r5, #0
	strh	r3, [r1, #0]
	b.n	.L_0808ce1c
.L_0808c6ec:
	mov	r0, r9
	bl	sub_080770d0
	bl	sub_0808bc9c
	cmp	r0, #0
	bne.n	.L_0808c794
	ldrh	r5, [r7, #0]
	cmp	r5, #0
	bne.n	.L_0808c72e
	ldr	r3, [sp, #8]
	mov	r4, sl
	ldr	r0, [r3, #0]
	ldr	r1, [r4, #0]
	bl	sub_0808fefc
	movs	r3, #1
	strh	r3, [r7, #0]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r5, [r3, #0]
	bl	sub_08094428
	cmp	r0, #0
	bne.n	.L_0808c72e
	mov	r1, sl
	ldr	r0, [r1, #0]
	adds	r0, #1
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	bl	sub_080030f8
.L_0808c72e:
	ldr	r5, [pc, #252]
	adds	r0, r5, #0
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_0808c750
	adds	r0, r5, #0
	bl	sub_080770d0
	ldr	r3, [pc, #188]
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	bl	sub_08015200
.L_0808c750:
	ldr	r5, [pc, #172]
	movs	r1, #141
	lsls	r1, r1, #2
	adds	r6, r5, r1
	movs	r2, #0
	ldrsh	r0, [r6, r2]
	cmp	r0, #0
	beq.n	.L_0808c76a
	movs	r1, #1
	bl	sub_08095778
	movs	r3, #0
	strh	r3, [r6, #0]
.L_0808c76a:
	movs	r3, #143
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_0808c794
	bl	sub_0808c44c
	ldrh	r0, [r5, #0]
	bl	sub_080919d8
	cmp	r0, #0
	bne.n	.L_0808c78c
	ldrh	r0, [r5, #0]
	movs	r1, #0
	bl	sub_08091a58
.L_0808c78c:
	bl	sub_0808c4c0
	movs	r3, #0
	strh	r3, [r5, #0]
.L_0808c794:
	ldr	r4, [pc, #104]
	movs	r1, #0
	mov	r9, r4
	mov	fp, r1
.L_0808c79c:
	movs	r0, #130
	lsls	r0, r0, #1
	bl	sub_080770c8
	ldr	r2, [pc, #136]
	ldr	r3, [r2, #0]
	lsls	r3, r3, #2
	mov	r4, r8
	adds	r3, #20
	ldr	r3, [r4, r3]
	mov	sl, r3
	mov	r0, sl
	bl	sub_08009140
	mov	r1, sl
	movs	r2, #238
	ldr	r3, [r1, #8]
	lsls	r2, r2, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r3, #240
	lsls	r3, r3, #1
	add	r3, r9
	mov	r2, fp
	str	r2, [r3, #0]
	movs	r2, #242
	ldr	r3, [r1, #16]
	lsls	r2, r2, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r2, #244
	ldrh	r3, [r1, #6]
	lsls	r2, r2, #1
	add	r2, r9
	str	r3, [r2, #0]
	mov	r3, sl
	adds	r3, #34
	ldrb	r3, [r3, #0]
	ldr	r4, [pc, #72]
	strh	r3, [r4, #0]
	b.n	.L_0808cd76
	movs	r0, r0
	.4byte 0x00000ccc
	.4byte 0x040000d4
	.4byte 0x85000333
	.4byte 0x00000103
	.4byte 0x02000240
	.4byte 0x0000ffff
	.4byte 0x00000001
	.4byte 0x02008000
	.4byte 0x00000109
	.4byte 0x00000199
	.4byte 0x0000023e
	.4byte 0x02010000
	.4byte 0x0000022e
	.4byte 0x0000024a
	.4byte 0x00000cc8
	.4byte 0x0000012f
	.4byte 0x02000434
	.2byte 0x042c
	.2byte 0x0200
.L_0808c838:
	movs	r1, #184
	lsls	r1, r1, #1
	add	r1, r8
	movs	r2, #0
	ldrsh	r5, [r1, r2]
	cmp	r5, #0
	beq.n	.L_0808c848
	b.n	.L_0808c6e4
.L_0808c848:
	movs	r2, #193
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #0
	ldrsh	r6, [r2, r3]
	cmp	r6, #0
	bne.n	.L_0808c858
	b.n	.L_0808ca0e
.L_0808c858:
	ldr	r3, [pc, #52]
	ldr	r4, [pc, #48]
	add	r3, r8
	strh	r4, [r3, #0]
	movs	r1, #0
	ldrsh	r0, [r2, r1]
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L_0808c86e
	b.n	.L_0808c9d4
.L_0808c86e:
	bl	sub_08091660
	bl	sub_0808c44c
	movs	r3, #194
	lsls	r3, r3, #1
	add	r3, r8
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	movs	r6, #0
	cmp	r5, r3
	bcs.n	.L_0808c8c0
	movs	r5, #196
	lsls	r5, r5, #1
	b.n	.L_0808c894
	.4byte 0x00000001
	.2byte 0x0cb6
	.2byte 0x0000
.L_0808c894:
	mov	r0, sl
	movs	r1, #22
	bl	sub_08009080
	mov	r1, r8
	ldrsh	r0, [r5, r1]
	movs	r1, #1
	bl	sub_08015120
	ldr	r0, [pc, #80]
	movs	r1, #1
	bl	sub_08015040
	movs	r3, #194
	lsls	r3, r3, #1
	add	r3, r8
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	adds	r6, #1
	adds	r5, #2
	cmp	r6, r3
	bcc.n	.L_0808c894
.L_0808c8c0:
	movs	r3, #195
	lsls	r3, r3, #1
	add	r3, r8
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_0808c8d0
	b.n	.L_0808c9c0
.L_0808c8d0:
	ldr	r2, [pc, #40]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_0808c900
	movs	r0, #32
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_0808c8ec
	mov	r0, sl
	movs	r1, #21
	bl	sub_08009080
	b.n	.L_0808c908
.L_0808c8ec:
	mov	r0, sl
	movs	r1, #37
	bl	sub_08009080
	b.n	.L_0808c908
	movs	r0, r0
	.4byte 0x0000091a
	.2byte 0x0434
	.2byte 0x0200
.L_0808c900:
	mov	r0, sl
	movs	r1, #19
	bl	sub_08009080
.L_0808c908:
	movs	r0, #59
	bl	sub_080f9010
	movs	r1, #1
	ldr	r0, [pc, #52]
	bl	sub_08015040
	ldr	r3, [pc, #52]
	ldr	r0, [r3, #0]
	bl	sub_08077008
	ldr	r4, [pc, #36]
	adds	r6, r0, #0
	strh	r4, [r6, #56]
	movs	r1, #1
	lsls	r5, r1, #14
	adds	r0, r5, #0
	movs	r2, #52
	ldrsh	r1, [r6, r2]
	bl	sub_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_0808c950
	movs	r5, #0
	cmp	r0, #0
	blt.n	.L_0808c950
	adds	r5, r0, #0
	b.n	.L_0808c950
	.4byte 0x00000001
	.4byte 0x0000091b
	.2byte 0x0434
	.2byte 0x0200
.L_0808c950:
	lsls	r3, r5, #16
	strh	r5, [r6, #20]
	cmp	r3, #0
	bne.n	.L_0808c964
	movs	r4, #56
	ldrsh	r3, [r6, r4]
	cmp	r3, #0
	beq.n	.L_0808c964
	ldr	r1, [pc, #32]
	strh	r1, [r6, #20]
.L_0808c964:
	movs	r2, #58
	ldrsh	r0, [r6, r2]
	movs	r3, #54
	ldrsh	r1, [r6, r3]
	lsls	r0, r0, #14
	bl	sub_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_0808c988
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_0808c988
	adds	r3, r0, #0
	b.n	.L_0808c988
	.2byte 0x0001
	.2byte 0x0000
.L_0808c988:
	strh	r3, [r6, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_0808c99c
	movs	r4, #58
	ldrsh	r3, [r6, r4]
	cmp	r3, #0
	beq.n	.L_0808c99c
	ldr	r1, [pc, #44]
	strh	r1, [r6, #22]
.L_0808c99c:
	movs	r3, #226
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r2, [r3, #0]
	movs	r3, #224
	lsls	r3, r3, #1
	add	r3, r9
	strh	r2, [r3, #0]
	movs	r3, #227
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	ldr	r2, [pc, #20]
	strh	r3, [r2, #0]
	bl	sub_0808c4c0
	ldr	r7, [pc, #16]
	b.n	.L_0808ce1c
.L_0808c9c0:
	bl	sub_0808c4c0
	b.n	.L_0808c9fa
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x02000402
	.2byte 0x03e7
	.2byte 0x0000
.L_0808c9d4:
	ldr	r3, [pc, #200]
	cmp	r0, r3
	bne.n	.L_0808c9e6
	bl	sub_08091660
	ldr	r0, [pc, #196]
	bl	sub_0809c138
	b.n	.L_0808c9fa
.L_0808c9e6:
	ldr	r4, [pc, #192]
	cmp	r0, r4
	bne.n	.L_0808c9f6
	bl	sub_08091660
	bl	sub_0809c3a4
	b.n	.L_0808c9fa
.L_0808c9f6:
	bl	sub_0808d7d8
.L_0808c9fa:
	ldr	r3, [pc, #176]
	mov	r1, fp
	add	r3, r8
	strh	r1, [r3, #0]
	movs	r3, #193
	lsls	r3, r3, #1
	add	r3, r8
	mov	r2, fp
	strh	r2, [r3, #0]
	b.n	.L_0808cd76
.L_0808ca0e:
	movs	r5, #190
	lsls	r5, r5, #1
	add	r5, r8
	movs	r3, #0
	ldrsh	r7, [r5, r3]
	cmp	r7, #0
	beq.n	.L_0808ca5e
	str	r1, [sp, #4]
	bl	sub_08091660
	bl	sub_0808ba38
	ldr	r2, [pc, #136]
	ldr	r3, [pc, #136]
	add	r2, r9
	strh	r3, [r2, #0]
	movs	r2, #224
	lsls	r2, r2, #1
	movs	r3, #255
	add	r2, r9
	lsls	r3, r3, #1
	strh	r3, [r2, #0]
	ldr	r4, [pc, #124]
	ldrh	r3, [r5, #0]
	ldr	r1, [sp, #4]
	strh	r3, [r4, #0]
	ldr	r3, [pc, #120]
	strh	r3, [r1, #0]
	movs	r1, #0
	ldrsh	r0, [r5, r1]
	bl	sub_080941e0
	movs	r3, #212
	lsls	r3, r3, #1
	ldr	r2, [pc, #108]
	add	r3, r8
	str	r6, [r3, #0]
	strh	r6, [r5, #0]
	str	r6, [r2, #0]
	b.n	.L_0808cd76
.L_0808ca5e:
	movs	r3, #182
	lsls	r3, r3, #1
	add	r3, r8
	movs	r4, #0
	ldrsh	r6, [r3, r4]
	cmp	r6, #0
	beq.n	.L_0808ca86
	ldr	r5, [pc, #60]
	ldr	r1, [pc, #44]
	add	r5, r8
	strh	r1, [r5, #0]
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	str	r3, [sp, #0]
	bl	sub_0808d828
	ldr	r3, [sp, #0]
	strh	r7, [r5, #0]
	strh	r7, [r3, #0]
	b.n	.L_0808cd76
.L_0808ca86:
	movs	r5, #183
	lsls	r5, r5, #1
	add	r5, r8
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	cmp	r0, #0
	beq.n	.L_0808cac4
	bl	sub_0808d8f0
	strh	r6, [r5, #0]
	b.n	.L_0808cd76
	.4byte 0x00000001
	.4byte 0xfffffc88
	.4byte 0x0000001b
	.4byte 0xfffffc87
	.4byte 0x00000cb6
	.4byte 0x0000021e
	.4byte 0x0000ffff
	.4byte 0x02000402
	.4byte 0x000003e7
	.2byte 0x0478
	.2byte 0x0200
.L_0808cac4:
	movs	r3, #186
	lsls	r3, r3, #1
	add	r3, r8
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #0
	beq.n	.L_0808cb4c
	bl	sub_08015208
	ldr	r1, [pc, #64]
	ldr	r0, [r1, #0]
	bl	sub_0808ddec
	movs	r2, #1
	adds	r5, r0, #0
	negs	r2, r2
	movs	r6, #0
	cmp	r5, r2
	beq.n	.L_0808caf6
	bl	sub_0808d5a4
	cmp	r0, #0
	beq.n	.L_0808caf4
	movs	r0, #1
.L_0808caf4:
	adds	r6, r0, #0
.L_0808caf6:
	cmp	r6, #0
	beq.n	.L_0808cb1c
	ldr	r3, [pc, #24]
	orrs	r5, r3
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, r8
	strh	r5, [r3, #0]
	movs	r3, #185
	lsls	r3, r3, #1
	add	r3, r8
	mov	r4, fp
	strh	r4, [r3, #0]
	b.n	.L_0808cb44
	movs	r0, r0
	.4byte 0x00001000
	.2byte 0x0434
	.2byte 0x0200
.L_0808cb1c:
	bl	sub_0808ce74
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_0808cb3a
	movs	r3, #189
	lsls	r3, r3, #1
	add	r3, r8
	strh	r6, [r3, #0]
	movs	r3, #185
	lsls	r3, r3, #1
	add	r3, r8
	mov	r1, fp
	strh	r1, [r3, #0]
	b.n	.L_0808cb44
.L_0808cb3a:
	movs	r3, #185
	lsls	r3, r3, #1
	ldr	r2, [pc, #8]
	add	r3, r8
	strh	r2, [r3, #0]
.L_0808cb44:
	movs	r3, #186
	b.n	.L_0808cd6e
	.2byte 0x0001
	.2byte 0x0000
.L_0808cb4c:
	movs	r3, #185
	lsls	r3, r3, #1
	add	r3, r8
	movs	r1, #0
	ldrsh	r7, [r3, r1]
	cmp	r7, #0
	beq.n	.L_0808cbe4
	bl	sub_08015208
	bl	sub_08091660
	movs	r0, #111
	bl	sub_080f9010
	bl	sub_0808c44c
	movs	r0, #131
	lsls	r0, r0, #1
	bl	sub_080770c8
	ldr	r3, [pc, #96]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0808cb98
	ldr	r1, [pc, #92]
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0808cb98
	ldr	r3, [r1, #0]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0808cb98
	bl	sub_08015290
	b.n	.L_0808cbbe
.L_0808cb98:
	ldr	r0, [pc, #68]
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_0808cbae
	movs	r2, #193
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #250
	strh	r3, [r2, #0]
	b.n	.L_0808cbbe
.L_0808cbae:
	movs	r5, #204
	lsls	r5, r5, #4
	add	r5, r8
	strh	r0, [r5, #0]
	bl	sub_080151e8
	ldr	r2, [pc, #24]
	strh	r2, [r5, #0]
.L_0808cbbe:
	bl	sub_0808c4c0
	movs	r0, #131
	lsls	r0, r0, #1
	bl	sub_080770d0
	bl	sub_080772f0
	movs	r3, #185
	b.n	.L_0808cd6e
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x03001f54
	.4byte 0x03001ae8
	.2byte 0x0107
	.2byte 0x0000
.L_0808cbe4:
	movs	r5, #188
	lsls	r5, r5, #1
	add	r5, r8
	movs	r1, #0
	ldrsh	r6, [r5, r1]
	cmp	r6, #0
	beq.n	.L_0808cc02
	bl	sub_0808c44c
	ldrh	r3, [r5, #0]
	ldr	r0, [pc, #36]
	ands	r0, r3
	bl	sub_0808d5dc
	b.n	.L_0808cc42
.L_0808cc02:
	movs	r5, #189
	lsls	r5, r5, #1
	add	r5, r8
	movs	r2, #0
	ldrsh	r7, [r5, r2]
	cmp	r7, #0
	beq.n	.L_0808cc24
	bl	sub_0808c44c
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl	sub_0808d9a4
	b.n	.L_0808cc6e
	movs	r0, r0
	.2byte 0x0fff
	.2byte 0x0000
.L_0808cc24:
	movs	r5, #191
	lsls	r5, r5, #1
	add	r5, r8
	movs	r4, #0
	ldrsh	r6, [r5, r4]
	cmp	r6, #0
	beq.n	.L_0808cc4a
	bl	sub_08015208
	bl	sub_0808c44c
	movs	r1, #0
	ldrsh	r0, [r5, r1]
	bl	sub_0808e680
.L_0808cc42:
	bl	sub_0808c4c0
	strh	r7, [r5, #0]
	b.n	.L_0808cd76
.L_0808cc4a:
	movs	r5, #192
	lsls	r5, r5, #1
	add	r5, r8
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L_0808cc76
	bl	sub_0808c44c
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	movs	r3, #205
	lsls	r3, r3, #1
	add	r3, r8
	movs	r4, #0
	ldrsh	r1, [r3, r4]
	bl	sub_0808e23c
.L_0808cc6e:
	bl	sub_0808c4c0
	strh	r6, [r5, #0]
	b.n	.L_0808cd76
.L_0808cc76:
	movs	r3, #187
	lsls	r3, r3, #1
	add	r3, r8
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_0808cd76
	movs	r0, #111
	bl	sub_080f9010
	bl	sub_08091660
	bl	sub_0808c44c
	movs	r0, #131
	lsls	r0, r0, #1
	bl	sub_080770c8
	ldr	r1, [pc, #164]
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_0808ccb4
	ldr	r3, [pc, #160]
	ldr	r3, [r3, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0808ccb4
	bl	sub_08015288
	b.n	.L_0808cd60
.L_0808ccb4:
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_0808ccce
	ldr	r3, [pc, #136]
	movs	r2, #128
	ldr	r3, [r3, #0]
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0808ccce
	bl	sub_0808d0c8
	b.n	.L_0808cd60
.L_0808ccce:
	ldr	r0, [pc, #120]
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_0808cce4
	movs	r2, #193
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #250
	strh	r3, [r2, #0]
	b.n	.L_0808cd60
.L_0808cce4:
	bl	sub_0808e118
	bl	sub_08015208
	bl	sub_080f9070
	ldr	r3, [pc, #88]
	add	r3, r9
	strh	r0, [r3, #0]
	movs	r0, #191
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_0808cd58
	ldr	r3, [pc, #76]
	movs	r2, #128
	ldr	r5, [r3, #0]
	ldr	r1, [pc, #72]
	movs	r0, #0
	lsls	r2, r2, #2
.L_0808cd0e:
	ldrb	r3, [r1, #0]
	adds	r1, #1
	cmp	r3, #255
	bne.n	.L_0808cd18
	adds	r0, #1
.L_0808cd18:
	subs	r2, #1
	cmp	r2, #0
	bne.n	.L_0808cd0e
	adds	r3, r0, #0
	subs	r3, #136
	cmp	r3, #0
	bge.n	.L_0808cd30
	ldr	r2, [pc, #20]
	movs	r0, #1
	strh	r2, [r5, #4]
	bl	sub_080030f8
.L_0808cd30:
	movs	r0, #0
	bl	sub_08015370
	mov	r3, fp
	strh	r3, [r5, #4]
	b.n	.L_0808cd60
	.4byte 0x00000001
	.4byte 0x03001f54
	.4byte 0x03001ae8
	.4byte 0x00000107
	.4byte 0x0000021e
	.4byte 0x03001e68
	.2byte 0x1810
	.2byte 0x0300
.L_0808cd58:
	ldr	r0, [pc, #260]
	movs	r1, #1
	bl	sub_08015040
.L_0808cd60:
	bl	sub_0808c4c0
	movs	r0, #131
	lsls	r0, r0, #1
	bl	sub_080770d0
	movs	r3, #187
.L_0808cd6e:
	lsls	r3, r3, #1
	add	r3, r8
	mov	r4, fp
	strh	r4, [r3, #0]
.L_0808cd76:
	bl	sub_0808bc9c
	cmp	r0, #0
	beq.n	.L_0808cd80
	b.n	.L_0808c838
.L_0808cd80:
	movs	r0, #130
	lsls	r0, r0, #1
	bl	sub_080770d0
	ldr	r1, [pc, #216]
	ldr	r3, [r1, #0]
	lsls	r3, r3, #2
	adds	r3, #20
	mov	r4, r8
	ldr	r3, [r4, r3]
	ldr	r2, [pc, #208]
	mov	sl, r3
	cmp	r3, #0
	beq.n	.L_0808cdd8
	movs	r1, #249
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #2
	bne.n	.L_0808cdb0
	mov	r0, sl
	bl	sub_080090b8
	b.n	.L_0808cdd8
.L_0808cdb0:
	cmp	r3, #1
	bne.n	.L_0808cdbc
	mov	r0, sl
	bl	sub_080090b0
	b.n	.L_0808cdd8
.L_0808cdbc:
	movs	r3, #207
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L_0808cdd2
	mov	r0, sl
	bl	sub_080090a8
	b.n	.L_0808cdd8
.L_0808cdd2:
	mov	r0, sl
	bl	sub_080090a0
.L_0808cdd8:
	ldr	r5, [pc, #136]
.L_0808cdda:
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [r5, #0]
	lsls	r3, r3, #2
	adds	r3, #20
	mov	r4, r8
	ldr	r3, [r4, r3]
	mov	sl, r3
	ldr	r3, [pc, #124]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0808cdfe
	ldr	r0, [pc, #120]
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_0808ce12
.L_0808cdfe:
	mov	r3, sl
	adds	r3, #34
	mov	r2, sl
	mov	r4, sl
	ldrb	r0, [r3, #0]
	ldr	r1, [r2, #8]
	ldr	r3, [r4, #16]
	ldr	r2, [r2, #12]
	bl	sub_0808bec0
.L_0808ce12:
	bl	sub_0808bc9c
	cmp	r0, #0
	beq.n	.L_0808cdda
	b.n	.L_0808c79c
.L_0808ce1c:
	movs	r6, #227
	lsls	r6, r6, #1
	add	r6, r8
	ldrh	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_0808ce46
	movs	r3, #224
	movs	r5, #228
	lsls	r3, r3, #1
	lsls	r5, r5, #1
	add	r3, r8
	add	r5, r8
	ldr	r0, [r3, #0]
	ldr	r1, [r5, #0]
	bl	sub_080901c0
	movs	r3, #0
	strh	r3, [r6, #0]
	ldr	r0, [r5, #0]
	bl	sub_080030f8
.L_0808ce46:
	movs	r0, #27
	bl	sub_08002dd8
	adds	r0, r7, #0
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00000c2f
	.4byte 0x02000434
	.4byte 0x02000240
	.4byte 0x03001f54
	.4byte 0x00000163
