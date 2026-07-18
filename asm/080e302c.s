@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080e302c
	.thumb_func
Func_080e302c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #48]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	sub	sp, #72
	str	r1, [sp, #44]
	ldr	r3, [r3, #0]
	str	r3, [sp, #40]
	ldr	r2, [r2, #8]
	str	r2, [sp, #28]
	ldr	r2, [pc, #32]
	adds	r5, r1, r2
	str	r0, [r5, #0]
	movs	r0, #0
	bl	Func_080cd594
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	cmp	r3, #0
	bne.n	.L0
	ldr	r2, [pc, #16]
	ldr	r3, [pc, #4]
	b.n	.L1
	movs	r0, r0
	.4byte 0x000000cc
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
.L0:
	cmp	r3, #1
	bne.n	.L2
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #44]
.L1:
	strh	r3, [r2, #0]
.L2:
	ldr	r4, [sp, #44]
	ldr	r6, [pc, #44]
	adds	r3, r4, r6
	ldr	r2, [r3, #0]
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L3
	movs	r0, #8
	str	r0, [sp, #24]
	ldr	r3, [r2, #24]
	movs	r1, #40
	str	r1, [sp, #20]
	cmp	r3, #0
	beq.n	.L4
	movs	r2, #36
	str	r2, [sp, #20]
	cmp	r3, #1
	beq.n	.L4
	movs	r3, #40
	str	r3, [sp, #20]
	b.n	.L4
	.4byte 0x000000aa
	.4byte 0x04000020
	.4byte 0x00007828
.L3:
	movs	r4, #16
	movs	r6, #12
	negs	r4, r4
	negs	r6, r6
	str	r4, [sp, #24]
	str	r6, [sp, #20]
.L4:
	ldr	r1, [sp, #44]
	ldr	r2, [pc, #108]
	adds	r3, r1, r2
	ldr	r1, [r3, #0]
	ldr	r3, [r1, #4]
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r3, [r1, #24]
	ldr	r0, [pc, #96]
	adds	r3, r3, r2
	ldrsb	r3, [r0, r3]
	ldr	r4, [pc, #96]
	lsls	r3, r3, #8
	str	r3, [r4, #0]
	ldr	r1, [pc, #92]
	movs	r2, #1
	movs	r3, #0
	ldr	r0, [pc, #92]
	bl	Func_080e0524
	ldr	r0, [pc, #88]
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #84]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	Func_080072f0
	ldr	r5, [pc, #76]
	ldr	r6, [pc, #44]
	movs	r7, #0
.L8:
	ldrh	r2, [r5, #0]
	lsls	r3, r2, #16
	lsrs	r0, r3, #26
	ands	r0, r6
	lsrs	r1, r3, #21
	movs	r4, #31
	ands	r1, r6
	ands	r4, r2
	subs	r0, #8
	subs	r1, #8
	subs	r4, #8
	cmp	r0, #0
	bge.n	.L5
	movs	r0, #0
.L5:
	cmp	r1, #0
	bge.n	.L6
	movs	r1, #0
.L6:
	cmp	r4, #0
	bge.n	.L7
	movs	r4, #0
	b.n	.L7
	.4byte 0x0000001f
	.4byte 0x00007828
	.4byte 0x080eeda6
	.4byte 0x04000028
	.4byte 0x02010000
	.4byte 0x0000007b
	.4byte 0x0000007c
	.4byte 0x03001388
	.4byte 0x05000002
.L7:
	lsls	r3, r0, #10
	lsls	r2, r1, #5
	orrs	r3, r2
	orrs	r3, r4
	adds	r7, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r7, #63
	bne.n	.L8
	ldr	r2, [pc, #388]
	ldr	r5, [pc, #388]
	mov	lr, r2
	movs	r4, #0
	movs	r6, #0
	mov	sl, lr
.L12:
	mov	r3, lr
	ldrb	r3, [r3, #0]
	movs	r7, #0
	mov	ip, r3
.L11:
	movs	r0, #0
	mov	r1, ip
	mov	r8, r0
	cmp	r1, #0
	beq.n	.L9
	ldrb	r0, [r2, r6]
	ldr	r2, [sp, #44]
	adds	r1, r4, r2
	adds	r2, r5, #0
.L10:
	ldrb	r3, [r2, #0]
	strb	r3, [r1, #0]
	movs	r3, #1
	add	r8, r3
	adds	r2, #1
	adds	r1, #1
	adds	r4, #1
	cmp	r8, r0
	bne.n	.L10
	mov	r2, sl
.L9:
	adds	r7, #1
	cmp	r7, #32
	bne.n	.L11
	mov	r0, lr
	ldrb	r3, [r0, #0]
	movs	r1, #1
	adds	r6, #1
	adds	r5, r5, r3
	add	lr, r1
	cmp	r6, #9
	bne.n	.L12
	movs	r6, #0
.L15:
	adds	r0, r5, #0
	movs	r7, #0
	adds	r0, #48
.L14:
	ldr	r2, [sp, #44]
	adds	r1, r4, r2
	adds	r2, r5, #0
.L13:
	ldrb	r3, [r2, #0]
	adds	r2, #1
	strb	r3, [r1, #0]
	adds	r4, #1
	adds	r1, #1
	cmp	r2, r0
	bne.n	.L13
	adds	r7, #1
	cmp	r7, #3
	bne.n	.L14
	adds	r6, #1
	adds	r5, #48
	cmp	r6, #32
	bne.n	.L15
	ldr	r3, [sp, #44]
	movs	r1, #252
	movs	r7, #0
	lsls	r1, r1, #2
	adds	r2, r4, r3
.L16:
	ldrb	r3, [r5, #0]
	adds	r7, #1
	strb	r3, [r2, #0]
	adds	r5, #1
	adds	r2, #1
	adds	r4, #1
	cmp	r7, r1
	bne.n	.L16
	ldr	r1, [pc, #248]
	ldr	r0, [pc, #248]
	adds	r6, r1, #3
.L19:
	movs	r2, #0
	mov	r8, r2
	ldrb	r3, [r0, #0]
	ldrb	r2, [r1, #0]
	muls	r3, r2
	cmp	r3, #0
	beq.n	.L17
	mov	ip, r3
	ldr	r3, [sp, #44]
	adds	r2, r4, r3
.L18:
	ldrb	r3, [r5, #0]
	strb	r3, [r2, #0]
	movs	r3, #1
	add	r8, r3
	adds	r2, #1
	adds	r5, #1
	adds	r4, #1
	cmp	r8, ip
	bne.n	.L18
.L17:
	adds	r1, #1
	adds	r0, #1
	cmp	r1, r6
	bne.n	.L19
	ldr	r0, [pc, #204]
	ldr	r1, [sp, #28]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	movs	r4, #13
	negs	r4, r4
	ldr	r5, [pc, #192]
	movs	r7, #0
	adds	r6, r4, #0
.L22:
	movs	r0, #195
	lsls	r0, r0, #1
	bl	Func_08009030
	ldr	r1, [sp, #44]
	str	r0, [r5, r1]
	cmp	r0, #0
	beq.n	.L20
	adds	r2, r0, #0
	adds	r2, #38
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r7, #0
	cmp	r7, #0
	bge.n	.L21
	adds	r1, r7, #3
.L21:
	asrs	r1, r1, #2
	bl	Func_08009020
	ldr	r2, [sp, #44]
	ldr	r1, [r5, r2]
	ldrb	r3, [r1, #9]
	movs	r2, #4
	ands	r3, r6
	orrs	r3, r2
	strb	r3, [r1, #9]
.L20:
	adds	r7, #1
	adds	r5, #4
	cmp	r7, #11
	bne.n	.L22
	movs	r6, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r6, [sp, #0]
	bl	Func_080ed408
	ldr	r5, [pc, #112]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r1, #7
	str	r3, [sp, #32]
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	str	r6, [sp, #0]
	bl	Func_080ed408
	ldr	r2, [pc, #92]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r4, [sp, #44]
	movs	r0, #239
	lsls	r0, r0, #7
	ldr	r1, [pc, #80]
	adds	r3, r4, r0
	adds	r5, #188
	str	r6, [r3, #0]
	adds	r2, r4, r1
	ldr	r5, [r5, #0]
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #64]
	lsls	r1, r1, #3
	str	r5, [sp, #36]
	bl	Func_080041d8
	ldr	r3, [pc, #60]
	ldr	r2, [sp, #44]
	movs	r7, #0
	movs	r6, #15
	adds	r5, r2, r3
	b.n	.L23
	movs	r0, r0
	.4byte 0x00003f46
	.4byte 0x00001010
	.4byte 0x080eed90
	.4byte 0x02010000
	.4byte 0x080eeda0
	.4byte 0x080eeda3
	.4byte 0x00000073
	.4byte 0x000077d8
	.4byte 0x03001e50
	.4byte 0x04000050
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007198
.L23:
	bl	Func_08004458
	ands	r0, r6
	movs	r3, #128
	adds	r0, #88
	str	r3, [r5, #4]
	str	r0, [r5, #0]
	bl	Func_08004458
	movs	r3, #1
	str	r3, [r5, #16]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #20]
	lsls	r2, r7, #2
	ands	r0, r6
	movs	r3, #44
	str	r7, [r5, #12]
	adds	r0, #2
	subs	r3, r3, r2
	adds	r7, #1
	str	r0, [r5, #8]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #11
	bne.n	.L23
	ldr	r4, [sp, #44]
	ldr	r6, [pc, #148]
	ldr	r1, [pc, #148]
	movs	r7, #0
	adds	r2, r4, r6
.L24:
	ldrb	r3, [r1, #0]
	adds	r7, #1
	str	r3, [r2, #0]
	adds	r1, #1
	adds	r2, #28
	cmp	r7, #6
	bne.n	.L24
	ldr	r5, [pc, #132]
	movs	r7, #0
	movs	r6, #63
.L25:
	bl	Func_08004458
	ldr	r1, [sp, #24]
	ands	r0, r6
	adds	r0, r0, r1
	adds	r0, #32
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	Func_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #96
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	Func_08004458
	ands	r0, r6
	adds	r0, #32
	lsls	r0, r0, #13
	str	r0, [r5, #16]
	bl	Func_08004458
	movs	r3, #31
	movs	r2, #128
	ands	r3, r0
	adds	r7, #1
	lsls	r2, r2, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, r2
	bne.n	.L25
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #0]
	ldr	r3, [sp, #44]
	ldr	r4, [pc, #64]
	adds	r2, r3, r4
	movs	r3, #250
	str	r3, [r2, #0]
	ldr	r0, [sp, #24]
	adds	r0, #64
	movs	r6, #0
	str	r0, [sp, #16]
	mov	fp, r6
.L71:
	mov	r1, fp
	cmp	r1, #0
	bne.n	.L26
	movs	r0, #212
	bl	Func_080f9010
.L26:
	mov	r2, fp
	cmp	r2, #40
	bne.n	.L27
	movs	r0, #141
	bl	Func_080f9010
	b.n	.L27
	movs	r0, r0
	.4byte 0x00000785
	.4byte 0x00007240
	.4byte 0x080eedac
	.4byte 0x02010000
	.4byte 0x0400000c
	.4byte 0x000077a8
.L27:
	mov	r3, fp
	cmp	r3, #96
	bne.n	.L28
	movs	r0, #145
	bl	Func_080f9010
.L28:
	mov	r4, fp
	cmp	r4, #120
	bne.n	.L29
	movs	r0, #134
	bl	Func_080b50e8
.L29:
	mov	r6, fp
	cmp	r6, #81
	bgt.n	.L30
	mov	r3, fp
	cmp	r6, #0
	bge.n	.L31
	adds	r3, #3
.L31:
	asrs	r7, r3, #2
	cmp	r7, #2
	ble.n	.L32
	movs	r3, #1
	ands	r3, r7
	adds	r7, r3, #1
.L32:
	ldr	r0, [pc, #876]
	ldr	r3, [pc, #876]
	lsls	r1, r7, #1
	ldrb	r4, [r3, r7]
	ldr	r2, [sp, #44]
	ldr	r6, [sp, #24]
	mov	r8, r1
	ldrh	r1, [r0, r1]
	adds	r1, r2, r1
	subs	r2, r6, r4
	ldr	r6, [pc, #864]
	mov	sl, r0
	ldrb	r0, [r6, r7]
	movs	r5, #116
	mov	r9, r3
	adds	r2, #64
	subs	r3, r5, r0
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #32]
	ldr	r0, [sp, #40]
	bl	Func_080072f4
	mov	r0, sl
	mov	r2, r8
	ldrh	r1, [r0, r2]
	ldr	r3, [sp, #44]
	ldrb	r2, [r6, r7]
	mov	r4, r9
	adds	r1, r3, r1
	ldrb	r3, [r4, r7]
	subs	r5, r5, r2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	ldr	r0, [sp, #40]
	ldr	r2, [sp, #16]
	adds	r3, r5, #0
	ldr	r6, [sp, #36]
	bl	Func_080072fc
.L30:
	mov	r3, fp
	subs	r3, #12
	cmp	r3, #75
	bhi.n	.L33
	mov	r0, fp
	subs	r0, #64
	movs	r1, #3
	bl	Func_080022ec
	adds	r5, r0, #0
	cmp	r5, #0
	bge.n	.L34
	movs	r5, #0
.L34:
	cmp	r5, #7
	ble.n	.L35
	movs	r5, #7
.L35:
	ldr	r0, [sp, #24]
	lsls	r1, r5, #1
	ldr	r2, [pc, #772]
	adds	r0, #64
	movs	r3, #32
	movs	r4, #12
	str	r1, [sp, #8]
	movs	r7, #0
	mov	r8, r0
	mov	r9, r2
	mov	sl, r3
	negs	r4, r4
.L36:
	ldr	r0, [sp, #8]
	ldr	r6, [pc, #752]
	ldrh	r1, [r6, r0]
	mov	r6, r9
	ldrb	r3, [r6, r5]
	ldr	r2, [sp, #44]
	ldr	r0, [sp, #24]
	adds	r1, r2, r1
	str	r3, [sp, #0]
	subs	r2, r0, r3
	mov	r3, sl
	str	r3, [sp, #4]
	str	r4, [sp, #12]
	adds	r3, r4, #0
	ldr	r6, [sp, #32]
	adds	r2, #64
	ldr	r0, [sp, #40]
	bl	Func_080072fc
	ldr	r0, [pc, #716]
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #44]
	ldrh	r1, [r0, r2]
	mov	r6, r9
	ldr	r4, [sp, #12]
	adds	r1, r3, r1
	ldrb	r3, [r6, r5]
	mov	r0, sl
	str	r3, [sp, #0]
	str	r0, [sp, #4]
	adds	r3, r4, #0
	ldr	r0, [sp, #40]
	mov	r2, r8
	ldr	r6, [sp, #36]
	bl	Func_080072fc
	ldr	r4, [sp, #12]
	adds	r7, #1
	adds	r4, #32
	cmp	r7, #4
	bne.n	.L36
.L33:
	mov	r0, fp
	subs	r0, #160
	cmp	r0, #23
	bhi.n	.L37
	movs	r1, #3
	bl	Func_080022ec
	movs	r3, #7
	subs	r5, r3, r0
	cmp	r5, #0
	bge.n	.L38
	movs	r5, #0
.L38:
	cmp	r5, #7
	ble.n	.L39
	movs	r5, #7
.L39:
	ldr	r0, [sp, #24]
	lsls	r1, r5, #1
	ldr	r2, [pc, #636]
	adds	r0, #64
	movs	r3, #32
	movs	r4, #12
	str	r1, [sp, #8]
	movs	r7, #0
	mov	r8, r0
	mov	r9, r2
	mov	sl, r3
	negs	r4, r4
.L40:
	ldr	r0, [sp, #8]
	ldr	r6, [pc, #616]
	ldrh	r1, [r6, r0]
	mov	r6, r9
	ldrb	r3, [r6, r5]
	ldr	r2, [sp, #44]
	ldr	r0, [sp, #24]
	adds	r1, r2, r1
	str	r3, [sp, #0]
	subs	r2, r0, r3
	mov	r3, sl
	str	r3, [sp, #4]
	str	r4, [sp, #12]
	adds	r3, r4, #0
	ldr	r6, [sp, #32]
	adds	r2, #64
	ldr	r0, [sp, #40]
	bl	Func_080072fc
	ldr	r0, [pc, #580]
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #44]
	ldrh	r1, [r0, r2]
	mov	r6, r9
	ldr	r4, [sp, #12]
	adds	r1, r3, r1
	ldrb	r3, [r6, r5]
	mov	r0, sl
	str	r3, [sp, #0]
	str	r0, [sp, #4]
	adds	r3, r4, #0
	ldr	r0, [sp, #40]
	mov	r2, r8
	ldr	r6, [sp, #36]
	bl	Func_080072fc
	ldr	r4, [sp, #12]
	adds	r7, #1
	adds	r4, #32
	cmp	r7, #4
	bne.n	.L40
.L37:
	mov	r3, fp
	subs	r3, #88
	cmp	r3, #71
	bhi.n	.L41
	ldr	r0, [sp, #44]
	movs	r1, #158
	ldr	r4, [sp, #32]
	ldr	r2, [sp, #24]
	lsls	r1, r1, #5
	adds	r6, r0, r1
	movs	r5, #96
	movs	r3, #48
	str	r3, [sp, #0]
	str	r5, [sp, #4]
	mov	sl, r4
	adds	r2, #16
	ldr	r0, [sp, #40]
	adds	r1, r6, #0
	movs	r3, #0
	mov	r9, r2
	bl	Func_0800730c
	ldr	r1, [sp, #36]
	movs	r0, #48
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	ldr	r2, [sp, #16]
	mov	r8, r1
	ldr	r0, [sp, #40]
	adds	r1, r6, #0
	movs	r3, #0
	bl	Func_08007304
	movs	r3, #151
	ldr	r2, [sp, #44]
	lsls	r3, r3, #6
	adds	r6, r2, r3
	movs	r4, #48
	movs	r5, #21
	adds	r1, r6, #0
	mov	r2, r9
	movs	r3, #96
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #40]
	bl	Func_0800730c
	movs	r0, #48
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #40]
	adds	r1, r6, #0
	ldr	r2, [sp, #16]
	movs	r3, #96
	bl	Func_08007304
.L41:
	mov	r1, fp
	cmp	r1, #87
	ble.n	.L42
	ldr	r2, [pc, #424]
	ldr	r5, [pc, #424]
	movs	r7, #0
	mov	r8, r2
.L45:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L43
	movs	r0, #3
	ands	r0, r7
	adds	r0, #5
	lsls	r4, r0, #1
	subs	r3, r4, #2
	mov	r6, r8
	ldrh	r1, [r6, r3]
	ldr	r2, [sp, #28]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #1
	subs	r2, r2, r3
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #40]
	ldr	r4, [sp, #32]
	bl	Func_080072f4
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #16]
	subs	r2, r2, r3
	str	r2, [r5, #4]
	cmp	r2, #0
	bge.n	.L44
	mov	r6, fp
	cmp	r6, #159
	bgt.n	.L44
	movs	r3, #192
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	b.n	.L44
.L43:
	subs	r3, #1
	str	r3, [r5, #24]
.L44:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #64
	bne.n	.L45
.L42:
	mov	r0, fp
	cmp	r0, #4
	bgt.n	.L46
	b.n	.L47
.L46:
	movs	r1, #6
	mov	sl, r1
	cmp	r0, #71
	ble.n	.L48
	movs	r2, #11
	mov	sl, r2
.L48:
	movs	r3, #0
	mov	r4, sl
	mov	r8, r3
	cmp	r4, #0
	bne.n	.L49
	b.n	.L47
.L49:
	ldr	r6, [sp, #44]
	ldr	r0, [pc, #300]
	adds	r5, r6, r0
.L63:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	beq.n	.L50
	b.n	.L51
.L50:
	ldr	r3, [pc, #292]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	mov	r1, fp
	str	r3, [sp, #48]
	str	r4, [sp, #52]
	cmp	r1, #71
	ble.n	.L52
	mov	r3, r8
	movs	r4, #128
	ldr	r6, [sp, #44]
	ldr	r0, [pc, #276]
	lsls	r2, r3, #12
	lsls	r4, r4, #8
	adds	r2, r2, r4
	adds	r3, r6, r0
	str	r2, [sp, #48]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #24]
	lsls	r3, r3, #14
	adds	r3, r3, r2
	b.n	.L53
.L52:
	movs	r3, #128
	lsls	r3, r3, #8
.L53:
	str	r3, [sp, #48]
	ldr	r3, [sp, #48]
	add	r7, sp, #48
	str	r3, [r7, #4]
	add	r6, sp, #56
	movs	r3, #0
	str	r3, [r6, #12]
	ldr	r1, [sp, #20]
	ldr	r3, [r5, #0]
	lsls	r2, r1, #1
	adds	r3, r3, r2
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	ldr	r3, [r5, #4]
	movs	r2, #128
	lsls	r2, r2, #18
	lsls	r3, r3, #16
	subs	r3, r2, r3
	str	r2, [r6, #8]
	mov	r2, fp
	lsrs	r0, r2, #31
	add	r0, fp
	asrs	r0, r0, #1
	str	r3, [r6, #4]
	add	r0, r8
	movs	r1, #11
	bl	Func_080022fc
	movs	r4, #1
	negs	r4, r4
	cmp	r0, r4
	beq.n	.L54
	lsls	r3, r0, #2
	ldr	r0, [pc, #188]
	ldr	r1, [sp, #44]
	adds	r3, r3, r0
	ldr	r0, [r1, r3]
	adds	r2, r7, #0
	adds	r1, r6, #0
	movs	r3, #0
	str	r4, [sp, #12]
	bl	Func_08009008
	ldr	r4, [sp, #12]
.L54:
	ldr	r3, [r5, #4]
	ldr	r2, [r5, #8]
	subs	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	adds	r2, r2, r3
	str	r2, [r5, #12]
	cmp	r2, #12
	ble.n	.L55
	adds	r3, r2, #0
	subs	r3, #12
	str	r3, [r5, #12]
.L55:
	ldr	r3, [r5, #4]
	cmp	r3, #0
	bge.n	.L56
	mov	r2, fp
	cmp	r2, #159
	ble.n	.L57
	str	r4, [r5, #24]
	b.n	.L56
.L57:
	mov	r3, fp
	cmp	r3, #87
	ble.n	.L58
	bl	Func_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #8
	str	r3, [r5, #8]
	ldr	r4, [sp, #44]
	ldr	r6, [pc, #100]
	adds	r3, r4, r6
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #24]
	cmp	r3, #0
	bne.n	.L59
	bl	Func_08004458
	movs	r1, #96
	bl	Func_08002304
	adds	r0, #42
	b.n	.L60
.L59:
	cmp	r3, #1
	bne.n	.L61
	bl	Func_08004458
	movs	r1, #112
	bl	Func_08002304
	adds	r0, #34
	b.n	.L60
.L61:
	bl	Func_08004458
	movs	r1, #160
	bl	Func_08002304
	adds	r0, #10
.L60:
	str	r0, [r5, #0]
.L58:
	movs	r3, #128
	str	r3, [r5, #4]
	movs	r3, #8
	b.n	.L62
	.4byte 0x080eed9a
	.4byte 0x080eeda0
	.4byte 0x080eeda3
	.4byte 0x080eed90
	.4byte 0x080eed7e
	.4byte 0x080ede48
	.4byte 0x02010000
	.4byte 0x00007198
	.4byte 0x080edab0
	.4byte 0x00007828
	.4byte 0x000077d8
.L51:
	subs	r3, #1
.L62:
	str	r3, [r5, #24]
.L56:
	movs	r0, #1
	add	r8, r0
	adds	r5, #28
	cmp	r8, sl
	beq.n	.L47
	b.n	.L63
.L47:
	mov	r1, fp
	cmp	r1, #158
	bgt.n	.L64
	ldr	r3, [sp, #44]
	ldr	r4, [pc, #200]
	adds	r2, r3, r4
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L64
	movs	r6, #3
	ands	r1, r6
	adds	r5, r2, #0
	mov	r8, r1
	movs	r6, #36
.L67:
	mov	r0, fp
	cmp	r0, #85
	ble.n	.L65
	movs	r1, #12
	bl	Func_080022fc
	cmp	r0, #0
	bne.n	.L66
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	adds	r3, r7, #0
	bl	Func_080d6888
.L66:
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L65
	ldr	r3, [r5, #0]
	movs	r1, #5
	ldrsh	r0, [r3, r6]
	bl	Func_080b5088
.L65:
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	adds	r7, #1
	adds	r6, #2
	cmp	r7, r3
	bne.n	.L67
.L64:
	mov	r3, fp
	subs	r3, #90
	cmp	r3, #70
	bls.n	.L68
	movs	r0, #2
	movs	r1, #2
	bl	Func_080e155c
	b.n	.L69
.L68:
	movs	r0, #8
	movs	r1, #8
	bl	Func_080e155c
.L69:
	bl	Func_080cd52c
	ldr	r0, [pc, #88]
	ldr	r6, [sp, #44]
	movs	r3, #1
	adds	r2, r6, r0
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #1
	add	fp, r1
	mov	r2, fp
	cmp	r2, #192
	beq.n	.L70
	b.n	.L71
.L70:
	ldr	r0, [pc, #64]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	ldr	r3, [pc, #52]
	movs	r7, #0
	adds	r5, r6, r3
.L72:
	ldmia	r5!, {r0}
	adds	r7, #1
	bl	Func_08009038
	cmp	r7, #11
	bne.n	.L72
	bl	Func_080cdbc0
	add	sp, #72
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00007828
	.4byte 0x00007824
	.4byte 0x080cd261
	.4byte 0x000077d8
