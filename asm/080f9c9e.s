@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f9c9e
	.thumb_func
Func_080f9c9e:
	push	{r0, lr}
	ldr	r3, [r0, #56]
	cmp	r3, #0
	beq.n	.L0
	ldr	r0, [r0, #60]
	bl	.L1
.L0:
	pop	{r0}
	push	{r4, r5, r6, r7}
	mov	r4, r8
	mov	r5, r9
	mov	r6, sl
	mov	r7, fp
	push	{r4, r5, r6, r7}
	adds	r7, r0, #0
	ldr	r0, [r7, #4]
	cmp	r0, #0
	bge.n	.L2
	b.n	.L3
.L2:
	ldr	r0, [pc, #552]
	ldr	r0, [r0, #0]
	mov	r8, r0
	adds	r0, r7, #0
	bl	Func_080fab7c
	ldr	r0, [r7, #4]
	cmp	r0, #0
	bge.n	.L4
	b.n	.L3
.L4:
	ldrh	r0, [r7, #34]
	ldrh	r1, [r7, #32]
	adds	r0, r0, r1
	b.n	.L5
.L28:
	ldrb	r6, [r7, #8]
	ldr	r5, [r7, #44]
	movs	r3, #1
	movs	r4, #0
.L25:
	ldrb	r0, [r5, #0]
	movs	r1, #128
	tst	r1, r0
	bne.n	.L6
	b.n	.L7
.L6:
	mov	sl, r3
	orrs	r4, r3
	mov	fp, r4
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L8
.L11:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L9
	ldrb	r0, [r4, #16]
	cmp	r0, #0
	beq.n	.L10
	subs	r0, #1
	strb	r0, [r4, #16]
	bne.n	.L10
	movs	r0, #64
	orrs	r1, r0
	strb	r1, [r4, #0]
	b.n	.L10
.L9:
	adds	r0, r4, #0
	bl	Func_080fa678
.L10:
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L11
.L8:
	ldrb	r3, [r5, #0]
	movs	r0, #64
	tst	r0, r3
	beq.n	.L12
	adds	r0, r5, #0
	bl	Func_080fa68c
	movs	r0, #128
	strb	r0, [r5, #0]
	movs	r0, #2
	strb	r0, [r5, #15]
	movs	r0, #64
	strb	r0, [r5, #19]
	movs	r0, #22
	strb	r0, [r5, #25]
	movs	r0, #1
	adds	r1, r5, #6
	strb	r0, [r1, #30]
	b.n	.L12
.L18:
	ldr	r2, [r5, #64]
	ldrb	r1, [r2, #0]
	cmp	r1, #128
	bcs.n	.L13
	ldrb	r1, [r5, #7]
	b.n	.L14
.L13:
	adds	r2, #1
	str	r2, [r5, #64]
	cmp	r1, #189
	bcc.n	.L14
	strb	r1, [r5, #7]
.L14:
	cmp	r1, #207
	bcc.n	.L15
	mov	r0, r8
	ldr	r3, [r0, #56]
	adds	r0, r1, #0
	subs	r0, #207
	adds	r1, r7, #0
	adds	r2, r5, #0
	bl	.L1
	b.n	.L12
.L15:
	cmp	r1, #176
	bls.n	.L16
	adds	r0, r1, #0
	subs	r0, #177
	strb	r0, [r7, #10]
	mov	r3, r8
	ldr	r3, [r3, #52]
	lsls	r0, r0, #2
	ldr	r3, [r3, r0]
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	.L1
	ldrb	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L17
	b.n	.L12
.L16:
	ldr	r0, [pc, #336]
	subs	r1, #128
	adds	r1, r1, r0
	ldrb	r0, [r1, #0]
	strb	r0, [r5, #1]
.L12:
	ldrb	r0, [r5, #1]
	cmp	r0, #0
	beq.n	.L18
	subs	r0, #1
	strb	r0, [r5, #1]
	ldrb	r1, [r5, #25]
	cmp	r1, #0
	beq.n	.L17
	ldrb	r0, [r5, #23]
	cmp	r0, #0
	beq.n	.L17
	ldrb	r0, [r5, #28]
	cmp	r0, #0
	beq.n	.L19
	subs	r0, #1
	strb	r0, [r5, #28]
	b.n	.L17
.L19:
	ldrb	r0, [r5, #26]
	adds	r0, r0, r1
	strb	r0, [r5, #26]
	adds	r1, r0, #0
	subs	r0, #64
	lsls	r0, r0, #24
	bpl.n	.L20
	lsls	r2, r1, #24
	asrs	r2, r2, #24
	b.n	.L21
.L20:
	movs	r0, #128
	subs	r2, r0, r1
.L21:
	ldrb	r0, [r5, #23]
	muls	r0, r2
	asrs	r2, r0, #6
	ldrb	r0, [r5, #22]
	eors	r0, r2
	lsls	r0, r0, #24
	beq.n	.L17
	strb	r2, [r5, #22]
	ldrb	r0, [r5, #0]
	ldrb	r1, [r5, #24]
	cmp	r1, #0
	bne.n	.L22
	movs	r1, #12
	b.n	.L23
.L22:
	movs	r1, #3
.L23:
	orrs	r0, r1
	strb	r0, [r5, #0]
.L17:
	mov	r3, sl
	mov	r4, fp
.L7:
	subs	r6, #1
	ble.n	.L24
	movs	r0, #80
	adds	r5, r5, r0
	lsls	r3, r3, #1
	b.n	.L25
.L24:
	ldr	r0, [r7, #12]
	adds	r0, #1
	str	r0, [r7, #12]
	cmp	r4, #0
	bne.n	.L26
	movs	r0, #128
	lsls	r0, r0, #24
	str	r0, [r7, #4]
	b.n	.L3
.L26:
	str	r4, [r7, #4]
	ldrh	r0, [r7, #34]
	subs	r0, #150
.L5:
	strh	r0, [r7, #34]
	cmp	r0, #150
	bcc.n	.L27
	b.n	.L28
.L27:
	ldrb	r2, [r7, #8]
	ldr	r5, [r7, #44]
.L37:
	ldrb	r0, [r5, #0]
	movs	r1, #128
	tst	r1, r0
	beq.n	.L29
	movs	r1, #15
	tst	r1, r0
	beq.n	.L29
	mov	r9, r2
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	Func_080fac44
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L30
.L36:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	bne.n	.L31
	adds	r0, r4, #0
	bl	Func_080fa678
	b.n	.L32
.L31:
	ldrb	r0, [r4, #1]
	movs	r6, #7
	ands	r6, r0
	ldrb	r3, [r5, #0]
	movs	r0, #3
	tst	r0, r3
	beq.n	.L33
	bl	Func_080f9f3c
	cmp	r6, #0
	beq.n	.L33
	ldrb	r0, [r4, #29]
	movs	r1, #1
	orrs	r0, r1
	strb	r0, [r4, #29]
.L33:
	ldrb	r3, [r5, #0]
	movs	r0, #12
	tst	r0, r3
	beq.n	.L32
	ldrb	r1, [r4, #8]
	movs	r0, #8
	ldrsb	r0, [r5, r0]
	adds	r2, r1, r0
	bpl.n	.L34
	movs	r2, #0
.L34:
	cmp	r6, #0
	beq.n	.L35
	mov	r0, r8
	ldr	r3, [r0, #48]
	adds	r1, r2, #0
	ldrb	r2, [r5, #9]
	adds	r0, r6, #0
	bl	.L1
	str	r0, [r4, #32]
	ldrb	r0, [r4, #29]
	movs	r1, #2
	orrs	r0, r1
	strb	r0, [r4, #29]
	b.n	.L32
.L35:
	adds	r1, r2, #0
	ldrb	r2, [r5, #9]
	ldr	r0, [r4, #36]
	bl	Func_080fa1fc
	str	r0, [r4, #32]
.L32:
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L36
.L30:
	ldrb	r0, [r5, #0]
	movs	r1, #240
	ands	r0, r1
	strb	r0, [r5, #0]
	mov	r2, r9
.L29:
	subs	r2, #1
	ble.n	.L3
	movs	r0, #80
	adds	r5, r5, r0
	bgt.n	.L37
.L3:
	ldr	r0, [pc, #24]
	str	r0, [r7, #52]
	pop	{r0, r1, r2, r3, r4, r5, r6, r7}
	mov	r8, r0
	mov	r9, r1
	mov	sl, r2
	mov	fp, r3
	pop	{r3}
.L1:
	bx	r3
	movs	r0, r0
	.4byte 0x080fba14
	.4byte 0x03007ff0
	.4byte 0x68736d53
