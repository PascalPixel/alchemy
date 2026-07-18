@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08077428
	.thumb_func
Func_08077428:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	movs	r0, #96
	sub	sp, #4
	bl	Func_08004970
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	Func_08077394
	adds	r7, r0, #0
	movs	r0, #16
	ldrsh	r3, [r7, r0]
	str	r3, [r6, #0]
	movs	r1, #18
	ldrsh	r3, [r7, r1]
	str	r3, [r6, #4]
	ldrh	r3, [r7, #24]
	str	r3, [r6, #8]
	ldrh	r3, [r7, #26]
	str	r3, [r6, #12]
	ldrh	r3, [r7, #28]
	str	r3, [r6, #16]
	ldrb	r3, [r7, #30]
	str	r3, [r6, #24]
	ldrb	r2, [r7, #31]
	movs	r3, #15
	ands	r3, r2
	str	r3, [r6, #28]
	adds	r3, r7, #0
	adds	r3, #32
	ldrb	r3, [r3, #0]
	str	r3, [r6, #32]
	adds	r3, r7, #0
	adds	r3, #33
	ldrb	r3, [r3, #0]
	adds	r1, r7, #0
	adds	r2, r6, #0
	str	r3, [r6, #36]
	adds	r1, #36
	adds	r2, #40
	movs	r5, #3
.L0:
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	str	r3, [r2, #0]
	movs	r0, #2
	ldrsh	r3, [r1, r0]
	subs	r5, #1
	str	r3, [r2, #4]
	adds	r1, #4
	adds	r2, #8
	cmp	r5, #0
	bge.n	.L0
	movs	r1, #52
	ldrsh	r2, [r7, r1]
	movs	r0, #20
	ldrsh	r3, [r7, r0]
	muls	r3, r2
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L1
	ldr	r1, [pc, #568]
	adds	r2, r3, r1
.L1:
	asrs	r0, r2, #14
	movs	r2, #56
	ldrsh	r1, [r7, r2]
	subs	r2, r0, r1
	cmp	r2, #0
	blt.n	.L2
	cmp	r2, #1
	bgt.n	.L3
	b.n	.L4
.L2:
	subs	r3, r1, r0
	cmp	r3, #1
	bgt.n	.L3
.L4:
	movs	r3, #54
	ldrsh	r2, [r7, r3]
	movs	r0, #22
	ldrsh	r3, [r7, r0]
	muls	r3, r2
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L5
	ldr	r1, [pc, #524]
	adds	r2, r3, r1
.L5:
	asrs	r0, r2, #14
	movs	r2, #58
	ldrsh	r1, [r7, r2]
	subs	r2, r0, r1
	cmp	r2, #0
	blt.n	.L6
	cmp	r2, #1
	bgt.n	.L3
	b.n	.L7
.L6:
	subs	r3, r1, r0
	cmp	r3, #1
	ble.n	.L7
.L3:
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r7, #20]
	strh	r3, [r7, #22]
	ldrh	r3, [r7, #52]
	strh	r3, [r7, #56]
	ldrh	r3, [r7, #54]
	strh	r3, [r7, #58]
.L7:
	movs	r3, #152
	lsls	r3, r3, #1
	adds	r1, r7, r3
	ldrb	r3, [r1, #0]
	movs	r0, #4
	negs	r0, r0
	ands	r0, r3
	movs	r3, #4
	ands	r3, r0
	movs	r4, #0
	strb	r0, [r1, #0]
	cmp	r3, #0
	beq.n	.L8
	movs	r2, #1
	adds	r3, r0, #0
	orrs	r3, r2
	strb	r3, [r1, #0]
.L8:
	movs	r0, #162
	lsls	r0, r0, #1
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L9
	ldr	r3, [r6, #28]
	adds	r3, #1
	str	r3, [r6, #28]
.L9:
	movs	r1, #161
	lsls	r1, r1, #1
	ldr	r2, [pc, #428]
	adds	r3, r7, r1
	strb	r4, [r3, #0]
	ldr	r0, [pc, #428]
	adds	r3, r7, r2
	strb	r4, [r3, #0]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L10
	b.n	.L11
.L10:
	movs	r5, #0
.L20:
	lsls	r3, r5, #1
	adds	r1, r3, #0
	adds	r1, #216
	ldrh	r2, [r7, r1]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L12
	b.n	.L13
.L12:
	ldrh	r0, [r7, r1]
	bl	Func_08078414
	ldrb	r2, [r0, #3]
	movs	r3, #1
	ands	r3, r2
	str	r0, [r6, #88]
	cmp	r3, #0
	beq.n	.L14
	movs	r2, #152
	lsls	r2, r2, #1
	adds	r1, r7, r2
	ldrb	r2, [r1, #0]
	movs	r3, #3
	orrs	r3, r2
	strb	r3, [r1, #0]
.L14:
	ldr	r1, [r6, #88]
	movs	r3, #8
	ldrsh	r2, [r1, r3]
	ldr	r3, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	movs	r2, #10
	ldrsb	r2, [r1, r2]
	ldr	r3, [r6, #12]
	adds	r3, r3, r2
	movs	r0, #0
	str	r3, [r6, #12]
	mov	r8, r0
.L18:
	mov	r1, r8
	ldr	r2, [r6, #88]
	lsls	r3, r1, #2
	adds	r3, #24
	ldrb	r1, [r2, r3]
	adds	r2, r2, r3
	movs	r3, #1
	ldrsb	r3, [r2, r3]
	str	r1, [r6, #72]
	str	r3, [r6, #84]
	cmp	r1, #26
	bls.n	.L15
	b.n	.L16
.L15:
	ldr	r2, [pc, #308]
	lsls	r3, r1, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080776fc
	.4byte 0x0807762c
	.4byte 0x08077636
	.4byte 0x08077640
	.4byte 0x0807764a
	.4byte 0x08077654
	.4byte 0x0807765e
	.4byte 0x080776fc
	.4byte 0x080776fc
	.4byte 0x080776fc
	.4byte 0x080776fc
	.4byte 0x080776fc
	.4byte 0x080776fc
	.4byte 0x080776fc
	.4byte 0x080776fc
	.4byte 0x08077668
	.4byte 0x08077672
	.4byte 0x0807767c
	.4byte 0x08077686
	.4byte 0x08077690
	.4byte 0x0807769a
	.4byte 0x080776a4
	.4byte 0x080776ae
	.4byte 0x080776b8
	.4byte 0x080776c4
	.4byte 0x080776d2
	.4byte 0x080776f4
	ldr	r3, [r6, #0]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #0]
	b.n	.L16
	ldr	r3, [r6, #32]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #32]
	b.n	.L16
	ldr	r3, [r6, #4]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #4]
	b.n	.L16
	ldr	r3, [r6, #36]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #36]
	b.n	.L16
	ldr	r3, [r6, #16]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #16]
	b.n	.L16
	ldr	r3, [r6, #24]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	b.n	.L16
	ldr	r3, [r6, #40]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #40]
	b.n	.L16
	ldr	r3, [r6, #48]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #48]
	b.n	.L16
	ldr	r3, [r6, #56]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #56]
	b.n	.L16
	ldr	r3, [r6, #64]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #64]
	b.n	.L16
	ldr	r3, [r6, #44]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #44]
	b.n	.L16
	ldr	r3, [r6, #52]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #52]
	b.n	.L16
	ldr	r3, [r6, #60]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #60]
	b.n	.L16
	ldr	r3, [r6, #68]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #68]
	b.n	.L16
	movs	r2, #161
	lsls	r2, r2, #1
	adds	r1, r7, r2
	ldrb	r3, [r1, #0]
	ldr	r2, [r6, #84]
	b.n	.L17
	ldr	r3, [pc, #32]
	adds	r1, r7, r3
	ldr	r2, [r6, #84]
	ldrb	r3, [r1, #0]
.L17:
	adds	r3, r3, r2
	strb	r3, [r1, #0]
	b.n	.L16
	movs	r0, #152
	lsls	r0, r0, #1
	adds	r3, r7, r0
	ldrb	r1, [r3, #0]
	movs	r2, #8
	orrs	r2, r1
	strb	r2, [r3, #0]
	b.n	.L16
	movs	r0, r0
	.4byte 0x00003fff
	.4byte 0x00000143
	.4byte 0x00000129
	.4byte 0x080775c0
	ldr	r3, [r6, #28]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #28]
.L16:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #3
	bgt.n	.L13
	b.n	.L18
.L13:
	adds	r5, #1
	cmp	r5, #14
	bgt.n	.L19
	b.n	.L20
.L19:
	movs	r3, #152
	lsls	r3, r3, #1
	adds	r1, r7, r3
	ldrb	r2, [r1, #0]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L21
	movs	r3, #10
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r1, #0]
.L21:
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, r0, r7
	movs	r4, #0
	mov	r8, r0
.L24:
	mov	r1, r8
	ldr	r1, [r1, #0]
	movs	r5, #0
	mov	sl, r1
.L23:
	movs	r3, #1
	lsls	r3, r5
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L22
	adds	r0, r4, #0
	adds	r1, r5, #0
	str	r4, [sp, #0]
	bl	Func_0807a0cc
	ldr	r3, [r6, #0]
	movs	r2, #4
	ldrsb	r2, [r0, r2]
	adds	r3, r3, r2
	str	r3, [r6, #0]
	movs	r2, #5
	ldrsb	r2, [r0, r2]
	ldr	r3, [r6, #4]
	adds	r3, r3, r2
	str	r3, [r6, #4]
	movs	r2, #6
	ldrsb	r2, [r0, r2]
	ldr	r3, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	movs	r2, #7
	ldrsb	r2, [r0, r2]
	ldr	r3, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	movs	r2, #8
	ldrsb	r2, [r0, r2]
	ldr	r3, [r6, #16]
	adds	r3, r3, r2
	str	r3, [r6, #16]
	movs	r2, #9
	ldrsb	r2, [r0, r2]
	ldr	r3, [r6, #24]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r4, [sp, #0]
.L22:
	adds	r5, #1
	cmp	r5, #19
	ble.n	.L23
	movs	r3, #4
	adds	r4, #1
	add	r8, r3
	cmp	r4, #3
	ble.n	.L24
	ldr	r0, [pc, #604]
	adds	r3, r7, r0
	ldrb	r0, [r3, #0]
	bl	Func_08079ad8
	adds	r5, r0, #0
	ldrb	r2, [r5, #8]
	ldr	r3, [r6, #0]
	movs	r1, #10
	adds	r0, r2, #0
	muls	r0, r3
	bl	Func_080022ec
	ldrb	r2, [r5, #9]
	ldr	r3, [r6, #4]
	str	r0, [r6, #0]
	movs	r1, #10
	adds	r0, r2, #0
	muls	r0, r3
	bl	Func_080022ec
	ldrb	r2, [r5, #10]
	ldr	r3, [r6, #8]
	str	r0, [r6, #4]
	movs	r1, #10
	adds	r0, r2, #0
	muls	r0, r3
	bl	Func_080022ec
	ldrb	r2, [r5, #11]
	ldr	r3, [r6, #12]
	str	r0, [r6, #8]
	movs	r1, #10
	adds	r0, r2, #0
	muls	r0, r3
	bl	Func_080022ec
	ldrb	r2, [r5, #12]
	ldr	r3, [r6, #16]
	str	r0, [r6, #12]
	movs	r1, #10
	adds	r0, r2, #0
	muls	r0, r3
	bl	Func_080022ec
	ldrb	r2, [r5, #13]
	ldr	r3, [r6, #24]
	str	r0, [r6, #16]
	movs	r1, #10
	adds	r0, r2, #0
	muls	r0, r3
	bl	Func_080022ec
	str	r0, [r6, #24]
	movs	r5, #0
.L28:
	lsls	r3, r5, #1
	adds	r1, r3, #0
	adds	r1, #216
	ldrh	r2, [r7, r1]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L25
	ldrh	r0, [r7, r1]
	bl	Func_08078414
	movs	r1, #0
	str	r0, [r6, #88]
	mov	r8, r1
.L27:
	mov	r0, r8
	ldr	r2, [r6, #88]
	lsls	r3, r0, #2
	adds	r3, #24
	ldrb	r1, [r2, r3]
	adds	r2, r2, r3
	movs	r3, #1
	ldrsb	r3, [r2, r3]
	str	r1, [r6, #72]
	subs	r1, #7
	str	r3, [r6, #84]
	cmp	r1, #7
	bhi.n	.L26
	ldr	r2, [pc, #440]
	lsls	r3, r1, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0807786c
	.4byte 0x0807787e
	.4byte 0x08077890
	.4byte 0x080778a2
	.4byte 0x080778b4
	.4byte 0x080778c6
	.4byte 0x080778d8
	.4byte 0x080778ea
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	Func_080022ec
	str	r0, [r6, #0]
	b.n	.L26
	ldr	r2, [r6, #32]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	Func_080022ec
	str	r0, [r6, #32]
	b.n	.L26
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	Func_080022ec
	str	r0, [r6, #4]
	b.n	.L26
	ldr	r2, [r6, #36]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	Func_080022ec
	str	r0, [r6, #36]
	b.n	.L26
	ldr	r2, [r6, #8]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	Func_080022ec
	str	r0, [r6, #8]
	b.n	.L26
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	Func_080022ec
	str	r0, [r6, #12]
	b.n	.L26
	ldr	r2, [r6, #16]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	Func_080022ec
	str	r0, [r6, #16]
	b.n	.L26
	ldr	r2, [r6, #24]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	Func_080022ec
	str	r0, [r6, #24]
.L26:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #3
	ble.n	.L27
.L25:
	adds	r5, #1
	cmp	r5, #14
	bgt.n	.L11
	b.n	.L28
.L11:
	ldr	r0, [pc, #244]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r2, [r6, #8]
	adds	r3, #8
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L29
	adds	r3, #7
.L29:
	asrs	r3, r3, #3
	ldr	r1, [pc, #224]
	str	r3, [r6, #8]
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r2, [r6, #12]
	adds	r3, #8
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L30
	adds	r3, #7
.L30:
	asrs	r3, r3, #3
	ldr	r2, [pc, #204]
	str	r3, [r6, #12]
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r2, [r6, #16]
	adds	r3, #8
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L31
	adds	r3, #7
.L31:
	asrs	r3, r3, #3
	str	r3, [r6, #16]
	movs	r3, #150
	lsls	r3, r3, #1
	movs	r4, #40
	movs	r5, #3
	adds	r0, r7, r3
.L32:
	ldrb	r3, [r0, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r2, r3, #0
	muls	r2, r3
	adds	r2, r2, r3
	lsls	r1, r2, #2
	ldr	r3, [r4, r6]
	adds	r1, r1, r2
	adds	r3, r3, r1
	subs	r5, #1
	str	r3, [r4, r6]
	adds	r0, #1
	adds	r4, #8
	cmp	r5, #0
	bge.n	.L32
	ldr	r1, [pc, #136]
	movs	r5, #3
	adds	r0, r7, r1
	movs	r1, #44
.L33:
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	lsls	r2, r3, #2
	adds	r2, r2, r3
	ldr	r3, [r1, r6]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	subs	r5, #1
	str	r3, [r1, r6]
	adds	r1, #8
	cmp	r5, #0
	bge.n	.L33
	ldr	r2, [pc, #84]
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L34
	movs	r1, #148
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	movs	r0, #0
	cmp	r3, #5
	bhi.n	.L35
	ldr	r2, [pc, #84]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080779dc
	.4byte 0x080779e0
	.4byte 0x080779ea
	.4byte 0x080779f2
	.4byte 0x08077a28
	.4byte 0x08077a20
	movs	r0, #136
	b.n	.L36
	movs	r0, #137
.L36:
	lsls	r0, r0, #1
	bl	Func_08079338
	b.n	.L35
	ldr	r0, [pc, #44]
	bl	Func_08079338
	b.n	.L35
	ldr	r0, [pc, #40]
	bl	Func_08079338
	b.n	.L35
	movs	r0, r0
	.4byte 0x00000129
	.4byte 0x0807784c
	.4byte 0x00000133
	.4byte 0x00000135
	.4byte 0x00000147
	.4byte 0x00000137
	.4byte 0x080779c4
	.4byte 0x00000113
	.4byte 0x00000111
	movs	r0, #137
	lsls	r0, r0, #1
	bl	Func_08079338
.L35:
	cmp	r0, #0
	beq.n	.L34
	ldr	r3, [r6, #36]
	adds	r3, #4
	str	r3, [r6, #36]
.L34:
	ldr	r3, [r6, #8]
	cmp	r3, #0
	bge.n	.L37
	movs	r3, #0
	str	r3, [r6, #8]
.L37:
	ldr	r2, [pc, #440]
	cmp	r3, r2
	ble.n	.L38
	str	r2, [r6, #8]
.L38:
	ldr	r3, [r6, #12]
	cmp	r3, #0
	bge.n	.L39
	movs	r3, #0
	str	r3, [r6, #12]
.L39:
	cmp	r3, r2
	ble.n	.L40
	str	r2, [r6, #12]
.L40:
	ldr	r3, [r6, #16]
	cmp	r3, #0
	bge.n	.L41
	movs	r3, #0
	str	r3, [r6, #16]
.L41:
	cmp	r3, r2
	ble.n	.L42
	str	r2, [r6, #16]
.L42:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	bge.n	.L43
	movs	r3, #0
	str	r3, [r6, #24]
.L43:
	cmp	r3, #99
	ble.n	.L44
	movs	r3, #99
	str	r3, [r6, #24]
.L44:
	ldr	r3, [r6, #28]
	cmp	r3, #0
	bge.n	.L45
	movs	r3, #0
	str	r3, [r6, #28]
.L45:
	cmp	r3, #2
	ble.n	.L46
	movs	r3, #2
	str	r3, [r6, #28]
.L46:
	ldr	r3, [r6, #32]
	cmp	r3, #0
	bge.n	.L47
	movs	r3, #0
	str	r3, [r6, #32]
.L47:
	ldr	r2, [pc, #360]
	cmp	r3, r2
	ble.n	.L48
	str	r2, [r6, #32]
.L48:
	ldr	r3, [r6, #36]
	cmp	r3, #0
	bge.n	.L49
	movs	r3, #0
	str	r3, [r6, #36]
.L49:
	cmp	r3, #200
	ble.n	.L50
	movs	r3, #200
	str	r3, [r6, #36]
.L50:
	movs	r2, #200
	mov	ip, r2
	adds	r1, r6, #0
	adds	r2, r6, #0
	movs	r5, #0
	movs	r0, #0
	movs	r4, #44
	adds	r2, #40
	adds	r1, #44
.L55:
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bge.n	.L51
	str	r0, [r2, #0]
	adds	r3, r0, #0
.L51:
	cmp	r3, #200
	ble.n	.L52
	mov	r3, ip
	str	r3, [r2, #0]
.L52:
	ldr	r3, [r1, #0]
	cmp	r3, #0
	bge.n	.L53
	str	r0, [r1, #0]
	adds	r3, r0, #0
.L53:
	cmp	r3, #200
	ble.n	.L54
	mov	r3, ip
	str	r3, [r6, r4]
.L54:
	adds	r5, #1
	adds	r1, #8
	adds	r4, #8
	adds	r2, #8
	cmp	r5, #3
	ble.n	.L55
	ldr	r3, [r6, #8]
	strh	r3, [r7, #60]
	ldr	r3, [r6, #12]
	strh	r3, [r7, #62]
	adds	r3, r7, #0
	ldr	r2, [r6, #16]
	adds	r3, #64
	strh	r2, [r3, #0]
	adds	r2, r7, #0
	ldr	r3, [r6, #24]
	adds	r2, #66
	strb	r3, [r2, #0]
	ldr	r3, [r6, #28]
	adds	r2, #1
	strb	r3, [r2, #0]
	adds	r3, r7, #0
	ldr	r2, [r6, #32]
	adds	r3, #68
	strb	r2, [r3, #0]
	adds	r2, r7, #0
	ldr	r3, [r6, #36]
	adds	r2, #69
	strb	r3, [r2, #0]
	adds	r1, r7, #0
	adds	r2, r6, #0
	adds	r1, #72
	adds	r2, #40
	movs	r5, #3
.L56:
	ldr	r3, [r2, #0]
	strh	r3, [r1, #0]
	ldr	r3, [r2, #4]
	subs	r5, #1
	strh	r3, [r1, #2]
	adds	r2, #8
	adds	r1, #4
	cmp	r5, #0
	bge.n	.L56
	ldr	r0, [pc, #196]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	ldr	r1, [pc, #196]
	cmp	r3, #0
	beq.n	.L57
	ldr	r1, [pc, #192]
.L57:
	movs	r3, #52
	ldrsh	r2, [r7, r3]
	ldr	r3, [r6, #0]
	cmp	r3, #0
	bge.n	.L58
	movs	r3, #0
	str	r3, [r6, #0]
.L58:
	cmp	r3, r1
	ble.n	.L59
	str	r1, [r6, #0]
	adds	r3, r1, #0
.L59:
	strh	r3, [r7, #52]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r2, r3
	beq.n	.L60
	movs	r0, #20
	ldrsh	r2, [r7, r0]
	ldr	r3, [r6, #0]
	muls	r2, r3
	cmp	r2, #0
	bge.n	.L61
	ldr	r3, [pc, #152]
	adds	r2, r2, r3
.L61:
	asrs	r2, r2, #14
	cmp	r2, #0
	bge.n	.L62
	movs	r2, #0
.L62:
	cmp	r2, r1
	ble.n	.L63
	adds	r2, r1, #0
.L63:
	movs	r0, #56
	ldrsh	r3, [r7, r0]
	cmp	r3, #0
	beq.n	.L64
	cmp	r2, #0
	bne.n	.L64
	movs	r2, #1
.L64:
	strh	r2, [r7, #56]
.L60:
	movs	r3, #54
	ldrsh	r2, [r7, r3]
	ldr	r3, [r6, #4]
	cmp	r3, #0
	bge.n	.L65
	movs	r3, #0
	str	r3, [r6, #4]
.L65:
	cmp	r3, r1
	ble.n	.L66
	str	r1, [r6, #4]
	adds	r3, r1, #0
.L66:
	strh	r3, [r7, #54]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r2, r3
	beq.n	.L67
	movs	r0, #22
	ldrsh	r2, [r7, r0]
	ldr	r3, [r6, #4]
	muls	r2, r3
	cmp	r2, #0
	bge.n	.L68
	ldr	r3, [pc, #72]
	adds	r2, r2, r3
.L68:
	asrs	r2, r2, #14
	cmp	r2, #0
	bge.n	.L69
	movs	r2, #0
.L69:
	cmp	r2, r1
	ble.n	.L70
	adds	r2, r1, #0
.L70:
	movs	r0, #58
	ldrsh	r3, [r7, r0]
	cmp	r3, #0
	beq.n	.L71
	cmp	r2, #0
	bne.n	.L71
	movs	r2, #1
.L71:
	strh	r2, [r7, #58]
.L67:
	adds	r0, r6, #0
	bl	Func_08002df0
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x000003e7
	.4byte 0x00002710
	.4byte 0x00000129
	.4byte 0x0000270f
	.4byte 0x000007cf
	.4byte 0x00003fff
