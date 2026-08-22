@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808c4f8
	.thumb_func
Func_0808c4f8:
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
	bl	Func_080048f4
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
	bl	Func_080770d0
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
	bl	Func_080040e8
	movs	r0, #0
	bl	Func_08095778
	movs	r2, #237
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	ldr	r3, [pc, #652]
	cmp	r0, r3
	bne.n	.L0
	bl	Func_08009118
	movs	r1, #137
	lsls	r1, r1, #2
	adds	r2, r5, r1
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r0, #0
	movs	r6, #3
	bl	Func_0808ace0
	b.n	.L1
.L0:
	bl	Func_08009110
	movs	r0, #1
	movs	r6, #2
	bl	Func_0808ace0
.L1:
	movs	r3, #207
	lsls	r3, r3, #1
	add	r3, r8
	strh	r6, [r3, #0]
	adds	r0, r6, #0
	bl	Func_08009078
	bl	Func_08015000
	bl	Func_0808bc44
	ldr	r5, [pc, #592]
	ldr	r0, [r5, #36]
	bl	Func_080072e4
	mov	r2, r8
	str	r0, [r2, #16]
	bl	Func_0808cf78
	ldr	r0, [r5, #28]
	bl	Func_080072e4
	bl	Func_0808b674
	ldr	r0, [pc, #568]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L2
	bl	Func_0808bb2c
.L2:
	ldr	r5, [pc, #540]
	movs	r4, #141
	lsls	r4, r4, #2
	adds	r3, r5, r4
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L3
	bl	Func_08095680
.L3:
	cmp	r6, #3
	bne.n	.L4
	bl	Func_08009130
	movs	r2, #238
	lsls	r2, r2, #1
	movs	r4, #242
	adds	r3, r5, r2
	lsls	r4, r4, #1
	ldr	r0, [r3, #0]
	adds	r3, r5, r4
	ldr	r1, [r3, #0]
	bl	Func_08009138
	b.n	.L5
.L4:
	bl	Func_08009128
.L5:
	bl	Func_0808e9c0
	bl	Func_08091174
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl	Func_08091200
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
	bne.n	.L6
	movs	r0, #162
	str	r6, [r2, #0]
	lsls	r0, r0, #1
	bl	Func_080770c8
.L6:
	movs	r3, #222
	ldr	r1, [pc, #404]
	lsls	r3, r3, #1
	ldr	r2, [pc, #412]
	add	r3, r8
	mov	r9, r1
	str	r2, [r3, #0]
	mov	r0, r9
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L7
	bl	Func_0808b1d8
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
.L7:
	bl	Func_08099810
	ldr	r2, [pc, #356]
	ldr	r3, [pc, #320]
	add	r2, r8
	strh	r3, [r2, #0]
	ldr	r3, [pc, #320]
	ldr	r0, [r3, #4]
	bl	Func_080072e4
	movs	r3, #184
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	cmp	r0, #0
	beq.n	.L8
	adds	r7, r0, #0
	strh	r6, [r3, #0]
	b.n	.L9
.L17:
	movs	r3, #0
	adds	r7, r5, #0
	strh	r3, [r1, #0]
	b.n	.L9
.L8:
	mov	r0, r9
	bl	Func_080770d0
	bl	Func_0808bc9c
	cmp	r0, #0
	bne.n	.L10
	ldrh	r5, [r7, #0]
	cmp	r5, #0
	bne.n	.L11
	ldr	r3, [sp, #8]
	mov	r4, sl
	ldr	r0, [r3, #0]
	ldr	r1, [r4, #0]
	bl	Func_0808fefc
	movs	r3, #1
	strh	r3, [r7, #0]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r5, [r3, #0]
	bl	Func_08094428
	cmp	r0, #0
	bne.n	.L11
	mov	r1, sl
	ldr	r0, [r1, #0]
	adds	r0, #1
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	bl	Func_080030f8
.L11:
	ldr	r5, [pc, #252]
	adds	r0, r5, #0
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L12
	adds	r0, r5, #0
	bl	Func_080770d0
	ldr	r3, [pc, #188]
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	bl	Func_08015200
.L12:
	ldr	r5, [pc, #172]
	movs	r1, #141
	lsls	r1, r1, #2
	adds	r6, r5, r1
	movs	r2, #0
	ldrsh	r0, [r6, r2]
	cmp	r0, #0
	beq.n	.L13
	movs	r1, #1
	bl	Func_08095778
	movs	r3, #0
	strh	r3, [r6, #0]
.L13:
	movs	r3, #143
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L10
	bl	Func_0808c44c
	ldrh	r0, [r5, #0]
	bl	Func_080919d8
	cmp	r0, #0
	bne.n	.L14
	ldrh	r0, [r5, #0]
	movs	r1, #0
	bl	Func_08091a58
.L14:
	bl	Func_0808c4c0
	movs	r3, #0
	strh	r3, [r5, #0]
.L10:
	ldr	r4, [pc, #104]
	movs	r1, #0
	mov	r9, r4
	mov	fp, r1
.L73:
	movs	r0, #130
	lsls	r0, r0, #1
	bl	Func_080770c8
	ldr	r2, [pc, #136]
	ldr	r3, [r2, #0]
	lsls	r3, r3, #2
	mov	r4, r8
	adds	r3, #20
	ldr	r3, [r4, r3]
	mov	sl, r3
	mov	r0, sl
	bl	Func_08009140
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
	b.n	.L15
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
	.4byte 0x0200042c
.L65:
	movs	r1, #184
	lsls	r1, r1, #1
	add	r1, r8
	movs	r2, #0
	ldrsh	r5, [r1, r2]
	cmp	r5, #0
	beq.n	.L16
	b.n	.L17
.L16:
	movs	r2, #193
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #0
	ldrsh	r6, [r2, r3]
	cmp	r6, #0
	bne.n	.L18
	b.n	.L19
.L18:
	ldr	r3, [pc, #52]
	ldr	r4, [pc, #48]
	add	r3, r8
	strh	r4, [r3, #0]
	movs	r1, #0
	ldrsh	r0, [r2, r1]
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L20
	b.n	.L21
.L20:
	bl	Func_08091660
	bl	Func_0808c44c
	movs	r3, #194
	lsls	r3, r3, #1
	add	r3, r8
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	movs	r6, #0
	cmp	r5, r3
	bcs.n	.L22
	movs	r5, #196
	lsls	r5, r5, #1
	b.n	.L23
	.4byte 0x00000001
	.4byte 0x00000cb6
.L23:
	mov	r0, sl
	movs	r1, #22
	bl	Func_08009080
	mov	r1, r8
	ldrsh	r0, [r5, r1]
	movs	r1, #1
	bl	Func_08015120
	ldr	r0, [pc, #80]
	movs	r1, #1
	bl	Func_08015040
	movs	r3, #194
	lsls	r3, r3, #1
	add	r3, r8
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	adds	r6, #1
	adds	r5, #2
	cmp	r6, r3
	bcc.n	.L23
.L22:
	movs	r3, #195
	lsls	r3, r3, #1
	add	r3, r8
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L24
	b.n	.L25
.L24:
	ldr	r2, [pc, #40]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L26
	movs	r0, #32
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L27
	mov	r0, sl
	movs	r1, #21
	bl	Func_08009080
	b.n	.L28
.L27:
	mov	r0, sl
	movs	r1, #37
	bl	Func_08009080
	b.n	.L28
	movs	r0, r0
	.4byte 0x0000091a
	.4byte 0x02000434
.L26:
	mov	r0, sl
	movs	r1, #19
	bl	Func_08009080
.L28:
	movs	r0, #59
	bl	Func_080f9010
	movs	r1, #1
	ldr	r0, [pc, #52]
	bl	Func_08015040
	ldr	r3, [pc, #52]
	ldr	r0, [r3, #0]
	bl	Func_08077008
	ldr	r4, [pc, #36]
	adds	r6, r0, #0
	strh	r4, [r6, #56]
	movs	r1, #1
	lsls	r5, r1, #14
	adds	r0, r5, #0
	movs	r2, #52
	ldrsh	r1, [r6, r2]
	bl	Func_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L29
	movs	r5, #0
	cmp	r0, #0
	blt.n	.L29
	adds	r5, r0, #0
	b.n	.L29
	.4byte 0x00000001
	.4byte 0x0000091b
	.4byte 0x02000434
.L29:
	lsls	r3, r5, #16
	strh	r5, [r6, #20]
	cmp	r3, #0
	bne.n	.L30
	movs	r4, #56
	ldrsh	r3, [r6, r4]
	cmp	r3, #0
	beq.n	.L30
	ldr	r1, [pc, #32]
	strh	r1, [r6, #20]
.L30:
	movs	r2, #58
	ldrsh	r0, [r6, r2]
	movs	r3, #54
	ldrsh	r1, [r6, r3]
	lsls	r0, r0, #14
	bl	Func_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L31
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L31
	adds	r3, r0, #0
	b.n	.L31
	.4byte 0x00000001
.L31:
	strh	r3, [r6, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L32
	movs	r4, #58
	ldrsh	r3, [r6, r4]
	cmp	r3, #0
	beq.n	.L32
	ldr	r1, [pc, #44]
	strh	r1, [r6, #22]
.L32:
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
	bl	Func_0808c4c0
	ldr	r7, [pc, #16]
	b.n	.L9
.L25:
	bl	Func_0808c4c0
	b.n	.L33
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x02000402
	.4byte 0x000003e7
.L21:
	ldr	r3, [pc, #200]
	cmp	r0, r3
	bne.n	.L34
	bl	Func_08091660
	ldr	r0, [pc, #196]
	bl	Func_0809c138
	b.n	.L33
.L34:
	ldr	r4, [pc, #192]
	cmp	r0, r4
	bne.n	.L35
	bl	Func_08091660
	bl	Func_0809c3a4
	b.n	.L33
.L35:
	bl	Func_0808d7d8
.L33:
	ldr	r3, [pc, #176]
	mov	r1, fp
	add	r3, r8
	strh	r1, [r3, #0]
	movs	r3, #193
	lsls	r3, r3, #1
	add	r3, r8
	mov	r2, fp
	strh	r2, [r3, #0]
	b.n	.L15
.L19:
	movs	r5, #190
	lsls	r5, r5, #1
	add	r5, r8
	movs	r3, #0
	ldrsh	r7, [r5, r3]
	cmp	r7, #0
	beq.n	.L36
	str	r1, [sp, #4]
	bl	Func_08091660
	bl	Func_0808ba38
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
	bl	Func_080941e0
	movs	r3, #212
	lsls	r3, r3, #1
	ldr	r2, [pc, #108]
	add	r3, r8
	str	r6, [r3, #0]
	strh	r6, [r5, #0]
	str	r6, [r2, #0]
	b.n	.L15
.L36:
	movs	r3, #182
	lsls	r3, r3, #1
	add	r3, r8
	movs	r4, #0
	ldrsh	r6, [r3, r4]
	cmp	r6, #0
	beq.n	.L37
	ldr	r5, [pc, #60]
	ldr	r1, [pc, #44]
	add	r5, r8
	strh	r1, [r5, #0]
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	str	r3, [sp, #0]
	bl	Func_0808d828
	ldr	r3, [sp, #0]
	strh	r7, [r5, #0]
	strh	r7, [r3, #0]
	b.n	.L15
.L37:
	movs	r5, #183
	lsls	r5, r5, #1
	add	r5, r8
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	cmp	r0, #0
	beq.n	.L38
	bl	Func_0808d8f0
	strh	r6, [r5, #0]
	b.n	.L15
	.4byte 0x00000001
	.4byte 0xfffffc88
	.4byte 0x0000001b
	.4byte 0xfffffc87
	.4byte 0x00000cb6
	.4byte 0x0000021e
	.4byte 0x0000ffff
	.4byte 0x02000402
	.4byte 0x000003e7
	.4byte 0x02000478
.L38:
	movs	r3, #186
	lsls	r3, r3, #1
	add	r3, r8
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #0
	beq.n	.L39
	bl	Func_08015208
	ldr	r1, [pc, #64]
	ldr	r0, [r1, #0]
	bl	Func_0808ddec
	movs	r2, #1
	adds	r5, r0, #0
	negs	r2, r2
	movs	r6, #0
	cmp	r5, r2
	beq.n	.L40
	bl	Func_0808d5a4
	cmp	r0, #0
	beq.n	.L41
	movs	r0, #1
.L41:
	adds	r6, r0, #0
.L40:
	cmp	r6, #0
	beq.n	.L42
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
	b.n	.L43
	movs	r0, r0
	.4byte 0x00001000
	.4byte 0x02000434
.L42:
	bl	Func_0808ce74
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L44
	movs	r3, #189
	lsls	r3, r3, #1
	add	r3, r8
	strh	r6, [r3, #0]
	movs	r3, #185
	lsls	r3, r3, #1
	add	r3, r8
	mov	r1, fp
	strh	r1, [r3, #0]
	b.n	.L43
.L44:
	movs	r3, #185
	lsls	r3, r3, #1
	ldr	r2, [pc, #8]
	add	r3, r8
	strh	r2, [r3, #0]
.L43:
	movs	r3, #186
	b.n	.L45
	.4byte 0x00000001
.L39:
	movs	r3, #185
	lsls	r3, r3, #1
	add	r3, r8
	movs	r1, #0
	ldrsh	r7, [r3, r1]
	cmp	r7, #0
	beq.n	.L46
	bl	Func_08015208
	bl	Func_08091660
	movs	r0, #111
	bl	Func_080f9010
	bl	Func_0808c44c
	movs	r0, #131
	lsls	r0, r0, #1
	bl	Func_080770c8
	ldr	r3, [pc, #96]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L47
	ldr	r1, [pc, #92]
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L47
	ldr	r3, [r1, #0]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L47
	bl	Func_08015290
	b.n	.L48
.L47:
	ldr	r0, [pc, #68]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L49
	movs	r2, #193
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #250
	strh	r3, [r2, #0]
	b.n	.L48
.L49:
	movs	r5, #204
	lsls	r5, r5, #4
	add	r5, r8
	strh	r0, [r5, #0]
	bl	Func_080151e8
	ldr	r2, [pc, #24]
	strh	r2, [r5, #0]
.L48:
	bl	Func_0808c4c0
	movs	r0, #131
	lsls	r0, r0, #1
	bl	Func_080770d0
	bl	Func_080772f0
	movs	r3, #185
	b.n	.L45
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x03001f54
	.4byte 0x03001ae8
	.4byte 0x00000107
.L46:
	movs	r5, #188
	lsls	r5, r5, #1
	add	r5, r8
	movs	r1, #0
	ldrsh	r6, [r5, r1]
	cmp	r6, #0
	beq.n	.L50
	bl	Func_0808c44c
	ldrh	r3, [r5, #0]
	ldr	r0, [pc, #36]
	ands	r0, r3
	bl	Func_0808d5dc
	b.n	.L51
.L50:
	movs	r5, #189
	lsls	r5, r5, #1
	add	r5, r8
	movs	r2, #0
	ldrsh	r7, [r5, r2]
	cmp	r7, #0
	beq.n	.L52
	bl	Func_0808c44c
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl	Func_0808d9a4
	b.n	.L53
	movs	r0, r0
	.4byte 0x00000fff
.L52:
	movs	r5, #191
	lsls	r5, r5, #1
	add	r5, r8
	movs	r4, #0
	ldrsh	r6, [r5, r4]
	cmp	r6, #0
	beq.n	.L54
	bl	Func_08015208
	bl	Func_0808c44c
	movs	r1, #0
	ldrsh	r0, [r5, r1]
	bl	Func_0808e680
.L51:
	bl	Func_0808c4c0
	strh	r7, [r5, #0]
	b.n	.L15
.L54:
	movs	r5, #192
	lsls	r5, r5, #1
	add	r5, r8
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L55
	bl	Func_0808c44c
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	movs	r3, #205
	lsls	r3, r3, #1
	add	r3, r8
	movs	r4, #0
	ldrsh	r1, [r3, r4]
	bl	Func_0808e23c
.L53:
	bl	Func_0808c4c0
	strh	r6, [r5, #0]
	b.n	.L15
.L55:
	movs	r3, #187
	lsls	r3, r3, #1
	add	r3, r8
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L15
	movs	r0, #111
	bl	Func_080f9010
	bl	Func_08091660
	bl	Func_0808c44c
	movs	r0, #131
	lsls	r0, r0, #1
	bl	Func_080770c8
	ldr	r1, [pc, #164]
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L56
	ldr	r3, [pc, #160]
	ldr	r3, [r3, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L56
	bl	Func_08015288
	b.n	.L57
.L56:
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L58
	ldr	r3, [pc, #136]
	movs	r2, #128
	ldr	r3, [r3, #0]
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L58
	bl	Func_0808d0c8
	b.n	.L57
.L58:
	ldr	r0, [pc, #120]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L59
	movs	r2, #193
	lsls	r2, r2, #1
	add	r2, r8
	movs	r3, #250
	strh	r3, [r2, #0]
	b.n	.L57
.L59:
	bl	Func_0808e118
	bl	Func_08015208
	bl	Func_080f9070
	ldr	r3, [pc, #88]
	add	r3, r9
	strh	r0, [r3, #0]
	movs	r0, #191
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L60
	ldr	r3, [pc, #76]
	movs	r2, #128
	ldr	r5, [r3, #0]
	ldr	r1, [pc, #72]
	movs	r0, #0
	lsls	r2, r2, #2
.L62:
	ldrb	r3, [r1, #0]
	adds	r1, #1
	cmp	r3, #255
	bne.n	.L61
	adds	r0, #1
.L61:
	subs	r2, #1
	cmp	r2, #0
	bne.n	.L62
	adds	r3, r0, #0
	subs	r3, #136
	cmp	r3, #0
	bge.n	.L63
	ldr	r2, [pc, #20]
	movs	r0, #1
	strh	r2, [r5, #4]
	bl	Func_080030f8
.L63:
	movs	r0, #0
	bl	Func_08015370
	mov	r3, fp
	strh	r3, [r5, #4]
	b.n	.L57
	.4byte 0x00000001
	.4byte 0x03001f54
	.4byte 0x03001ae8
	.4byte 0x00000107
	.4byte 0x0000021e
	.4byte 0x03001e68
	.4byte 0x03001810
.L60:
	ldr	r0, [pc, #260]
	movs	r1, #1
	bl	Func_08015040
.L57:
	bl	Func_0808c4c0
	movs	r0, #131
	lsls	r0, r0, #1
	bl	Func_080770d0
	movs	r3, #187
.L45:
	lsls	r3, r3, #1
	add	r3, r8
	mov	r4, fp
	strh	r4, [r3, #0]
.L15:
	bl	Func_0808bc9c
	cmp	r0, #0
	beq.n	.L64
	b.n	.L65
.L64:
	movs	r0, #130
	lsls	r0, r0, #1
	bl	Func_080770d0
	ldr	r1, [pc, #216]
	ldr	r3, [r1, #0]
	lsls	r3, r3, #2
	adds	r3, #20
	mov	r4, r8
	ldr	r3, [r4, r3]
	ldr	r2, [pc, #208]
	mov	sl, r3
	cmp	r3, #0
	beq.n	.L66
	movs	r1, #249
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #2
	bne.n	.L67
	mov	r0, sl
	bl	Func_080090b8
	b.n	.L66
.L67:
	cmp	r3, #1
	bne.n	.L68
	mov	r0, sl
	bl	Func_080090b0
	b.n	.L66
.L68:
	movs	r3, #207
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L69
	mov	r0, sl
	bl	Func_080090a8
	b.n	.L66
.L69:
	mov	r0, sl
	bl	Func_080090a0
.L66:
	ldr	r5, [pc, #136]
.L72:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [r5, #0]
	lsls	r3, r3, #2
	adds	r3, #20
	mov	r4, r8
	ldr	r3, [r4, r3]
	mov	sl, r3
	ldr	r3, [pc, #124]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L70
	ldr	r0, [pc, #120]
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L71
.L70:
	mov	r3, sl
	adds	r3, #34
	mov	r2, sl
	mov	r4, sl
	ldrb	r0, [r3, #0]
	ldr	r1, [r2, #8]
	ldr	r3, [r4, #16]
	ldr	r2, [r2, #12]
	bl	Func_0808bec0
.L71:
	bl	Func_0808bc9c
	cmp	r0, #0
	beq.n	.L72
	b.n	.L73
.L9:
	movs	r6, #227
	lsls	r6, r6, #1
	add	r6, r8
	ldrh	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L74
	movs	r3, #224
	movs	r5, #228
	lsls	r3, r3, #1
	lsls	r5, r5, #1
	add	r3, r8
	add	r5, r8
	ldr	r0, [r3, #0]
	ldr	r1, [r5, #0]
	bl	Func_080901c0
	movs	r3, #0
	strh	r3, [r6, #0]
	ldr	r0, [r5, #0]
	bl	Func_080030f8
.L74:
	movs	r0, #27
	bl	Func_08002dd8
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
