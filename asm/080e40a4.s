@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080e40a4
	.thumb_func
Func_080e40a4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #112]
	adds	r2, r3, #0
	adds	r1, r0, #0
	ldmia	r2!, {r0}
	ldr	r2, [r2, #0]
	sub	sp, #112
	str	r2, [sp, #52]
	ldr	r2, [r3, #8]
	str	r2, [sp, #44]
	subs	r3, #108
	ldr	r3, [r3, #0]
	str	r3, [sp, #40]
	ldr	r3, [r1, #0]
	mov	fp, r0
	movs	r6, #1
	cmp	r3, #199
	bgt.n	.L0
	movs	r6, #0
.L0:
	ldr	r5, [pc, #80]
	add	r5, fp
	movs	r2, #130
	ldr	r0, [r1, #8]
	str	r1, [r5, #0]
	ldr	r1, [r1, #12]
	bl	Func_080b5030
	movs	r0, #1
	bl	Func_080030f8
	bl	Func_080cdd58
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L1
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	movs	r0, #46
	str	r3, [sp, #0]
	bl	Func_080ed408
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	bl	Func_080ed408
	b.n	.L2
	movs	r0, r0
	.4byte 0x00001f80
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x0400000a
.L1:
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	movs	r0, #46
	bl	Func_080ed408
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	bl	Func_080ed408
.L2:
	ldr	r3, [pc, #720]
	adds	r2, r3, #0
	adds	r2, #184
	ldr	r2, [r2, #0]
	str	r2, [sp, #56]
	adds	r3, #188
	mov	r5, sp
	ldr	r3, [r3, #0]
	adds	r5, #56
	str	r5, [sp, #16]
	str	r3, [r5, #4]
	ldr	r5, [pc, #704]
	add	r5, fp
	ldr	r3, [r5, #0]
	movs	r2, #130
	ldr	r0, [r3, #8]
	ldr	r1, [r3, #12]
	bl	Func_080b5030
	movs	r0, #1
	bl	Func_080030f8
	ldr	r0, [pc, #684]
	mov	r1, fp
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
	ldr	r3, [r5, #0]
	movs	r2, #130
	ldr	r0, [r3, #8]
	ldr	r1, [r3, #12]
	bl	Func_080b5030
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [pc, #656]
	ldr	r1, [pc, #656]
	bl	Func_080e0524
	ldr	r2, [r5, #0]
	ldr	r3, [r2, #8]
	cmp	r3, #7
	ble.n	.L3
	ldr	r0, [pc, #648]
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	movs	r2, #128
	ldr	r3, [pc, #640]
	lsls	r0, r0, #19
	bl	Func_080072f0
	ldr	r2, [r5, #0]
.L3:
	ldr	r1, [r2, #12]
	ldr	r0, [r2, #8]
	movs	r2, #130
	bl	Func_080b5030
	movs	r0, #1
	bl	Func_080030f8
	ldr	r0, [pc, #616]
	ldr	r1, [sp, #44]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	ldr	r3, [r5, #0]
	movs	r2, #130
	ldr	r0, [r3, #8]
	ldr	r1, [r3, #12]
	bl	Func_080b5030
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r2, [pc, #580]
	movs	r3, #0
	add	r2, fp
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #576]
	lsls	r1, r1, #3
	bl	Func_080041d8
	cmp	r6, #1
	beq.n	.L4
	b.n	.L5
.L4:
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	Func_080b5098
	ldr	r0, [r0, #0]
	movs	r5, #225
	mov	r8, r0
	lsls	r5, r5, #7
	movs	r0, #0
	ldr	r6, [pc, #548]
	mov	sl, r0
	movs	r7, #0
	add	r5, fp
.L6:
	bl	Func_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #16
	str	r3, [r5, #0]
	str	r7, [r5, #4]
	str	r7, [r5, #8]
	bl	Func_08004458
	ands	r0, r6
	str	r0, [r5, #12]
	bl	Func_08004458
	ands	r0, r6
	str	r0, [r5, #16]
	bl	Func_08004458
	movs	r1, #1
	add	sl, r1
	ands	r0, r6
	mov	r2, sl
	str	r0, [r5, #20]
	adds	r5, #28
	cmp	r2, #64
	bne.n	.L6
	mov	r0, r8
	movs	r1, #0
	bl	Func_08009088
	mov	r3, r8
	ldr	r3, [r3, #36]
	str	r3, [sp, #36]
	mov	r5, r8
	ldr	r5, [r5, #40]
	str	r5, [sp, #32]
	mov	r0, r8
	ldr	r0, [r0, #44]
	str	r0, [sp, #28]
	mov	r1, r8
	ldr	r1, [r1, #72]
	str	r1, [sp, #20]
	mov	r2, r8
	ldr	r2, [r2, #52]
	movs	r5, #0
	mov	r3, r8
	str	r2, [sp, #24]
	str	r5, [r3, #36]
	str	r5, [r3, #40]
	str	r5, [r3, #44]
	str	r5, [r3, #52]
	str	r5, [r3, #72]
	ldr	r3, [pc, #400]
	add	r3, fp
	ldr	r3, [r3, #0]
	mov	r1, sp
	ldr	r0, [r3, #8]
	adds	r1, #100
	str	r1, [sp, #12]
	bl	Func_080e396c
	ldr	r2, [sp, #12]
	ldr	r3, [r2, #0]
	mov	r0, sl
	subs	r0, r0, r3
	str	r0, [sp, #48]
	add	r1, sp, #48
	ldr	r2, [pc, #408]
	ldrh	r1, [r1, #0]
	movs	r3, #80
	strh	r1, [r2, #4]
	strh	r3, [r2, #6]
	ldr	r2, [pc, #404]
	movs	r3, #24
	add	r2, fp
	str	r3, [r2, #0]
	ldr	r3, [pc, #400]
	movs	r1, #200
	add	r3, fp
	str	r5, [r3, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #392]
	bl	Func_080041d8
	movs	r0, #212
	bl	Func_080f9010
	mov	r9, r5
.L12:
	ldr	r3, [pc, #328]
	add	r3, fp
	ldr	r3, [r3, #0]
	movs	r2, #130
	ldr	r0, [r3, #8]
	ldr	r1, [r3, #12]
	movs	r6, #225
	bl	Func_080b5030
	lsls	r6, r6, #7
	movs	r2, #0
	mov	sl, r2
	add	r6, fp
.L11:
	ldr	r3, [r6, #0]
	cmp	r3, #0
	blt.n	.L7
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L8
	adds	r3, #3
.L8:
	asrs	r3, r3, #2
	cmp	r9, r3
	blt.n	.L7
	mov	r5, sl
	movs	r3, #1
	ands	r3, r5
	adds	r7, r3, #5
	bl	Func_080049ac
	ldr	r0, [r6, #20]
	bl	Func_08004c6c
	ldr	r0, [r6, #12]
	bl	Func_08004bd4
	ldr	r0, [r6, #16]
	bl	Func_08004c1c
	add	r5, sp, #76
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	Func_080e3944
	ldr	r3, [r5, #0]
	adds	r3, #64
	str	r3, [r5, #0]
	ldr	r2, [sp, #104]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	adds	r3, #24
	str	r3, [r5, #4]
	movs	r2, #60
	ldr	r3, [r5, #8]
	negs	r2, r2
	cmp	r3, r2
	bge.n	.L9
	str	r2, [r5, #8]
	adds	r3, r2, #0
.L9:
	cmp	r3, #60
	ble.n	.L10
	movs	r3, #60
	str	r3, [r5, #8]
.L10:
	lsls	r0, r7, #1
	ldr	r2, [pc, #256]
	adds	r3, #60
	str	r3, [r5, #8]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #44]
	ldr	r3, [r5, #4]
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	subs	r3, r3, r7
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, r2, r7
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #52]
	bl	Func_080072f4
	ldr	r3, [r6, #0]
	subs	r3, #4
	str	r3, [r6, #0]
.L7:
	movs	r3, #1
	add	sl, r3
	mov	r5, sl
	adds	r6, #28
	cmp	r5, #64
	bne.n	.L11
	ldr	r2, [pc, #208]
	add	r2, fp
	movs	r0, #1
	str	r3, [r2, #0]
	bl	Func_080030f8
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	cmp	r1, #32
	bne.n	.L12
	ldr	r0, [pc, #180]
	bl	Func_08004278
	mov	r0, r8
	movs	r1, #16
	bl	Func_08009088
	ldr	r2, [sp, #36]
	mov	r3, r8
	str	r2, [r3, #36]
	ldr	r5, [sp, #32]
	str	r5, [r3, #40]
	ldr	r0, [sp, #28]
	str	r0, [r3, #44]
	ldr	r1, [sp, #24]
	str	r1, [r3, #52]
	ldr	r2, [sp, #20]
	str	r2, [r3, #72]
	b.n	.L13
.L5:
	mov	r3, sp
	adds	r3, #100
	str	r3, [sp, #12]
.L13:
	movs	r1, #128
	ldr	r5, [pc, #144]
	ldr	r0, [sp, #52]
	lsls	r1, r1, #7
	bl	Func_080072f8
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r0, [pc, #136]
	bl	Func_080072f8
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #80]
	movs	r3, #75
	add	r2, fp
	ldr	r5, [pc, #44]
	str	r3, [r2, #0]
	ldr	r2, [pc, #112]
	ldr	r3, [pc, #32]
	add	r5, fp
	strh	r3, [r2, #0]
	ldr	r3, [r5, #0]
	add	r6, sp, #88
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	adds	r1, r6, #0
	bl	Func_080e396c
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L14
	ldr	r2, [r6, #0]
	movs	r3, #32
	b.n	.L15
	.4byte 0x00001f81
	.4byte 0x03001e50
	.4byte 0x00007828
	.4byte 0x00000049
	.4byte 0x0000004a
	.4byte 0x02010000
	.4byte 0x0000008e
	.4byte 0x03001388
	.4byte 0x00000076
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x0000ffff
	.4byte 0x03001ad0
	.4byte 0x000077b4
	.4byte 0x000077b8
	.4byte 0x080cd4b5
	.4byte 0x080ede5c
	.4byte 0x00007824
	.4byte 0x03000164
	.4byte 0x06004000
	.4byte 0x0400000a
.L14:
	ldr	r2, [r6, #0]
	movs	r3, #96
.L15:
	subs	r3, r3, r2
	str	r3, [sp, #48]
	ldr	r2, [sp, #48]
	cmp	r2, #0
	ble.n	.L16
	movs	r3, #0
	str	r3, [sp, #48]
.L16:
	movs	r3, #128
	ldr	r5, [sp, #48]
	negs	r3, r3
	cmp	r5, r3
	bge.n	.L17
	str	r3, [sp, #48]
.L17:
	ldr	r0, [sp, #48]
	ldr	r3, [r6, #0]
	ldr	r5, [pc, #564]
	ldr	r2, [pc, #564]
	adds	r3, r3, r0
	str	r3, [r6, #0]
	add	r5, fp
	add	r1, sp, #48
	movs	r3, #80
	strh	r3, [r2, #6]
	ldrh	r1, [r1, #0]
	ldr	r3, [r5, #0]
	strh	r1, [r2, #4]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	Func_080b5098
	ldr	r3, [r5, #0]
	ldr	r6, [r0, #0]
	movs	r5, #36
	ldrsh	r0, [r3, r5]
	bl	Func_080b5070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	movs	r5, #225
	mov	r8, r0
	lsls	r5, r5, #7
	movs	r0, #0
	mov	sl, r0
	movs	r7, #255
	add	r5, fp
.L19:
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	add	r3, r8
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	Func_08004458
	ands	r0, r7
	lsls	r0, r0, #10
	str	r0, [r5, #12]
	bl	Func_08004458
	ands	r0, r7
	lsls	r0, r0, #10
	str	r0, [r5, #16]
	bl	Func_08004458
	ands	r0, r7
	subs	r0, #127
	ldr	r3, [r5, #0]
	lsls	r0, r0, #10
	str	r0, [r5, #20]
	cmp	r3, #0
	ble.n	.L18
	ldr	r3, [r5, #12]
	negs	r3, r3
	str	r3, [r5, #12]
.L18:
	movs	r1, #1
	mov	r3, sl
	add	sl, r1
	adds	r3, #16
	mov	r2, sl
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #64
	bne.n	.L19
	ldr	r5, [sp, #40]
	adds	r5, #12
	movs	r3, #0
	str	r5, [sp, #8]
	mov	r9, r3
.L31:
	mov	r0, r9
	cmp	r0, #5
	bne.n	.L20
	movs	r0, #134
	bl	Func_080b50e8
.L20:
	mov	r1, r9
	cmp	r1, #4
	bne.n	.L21
	ldr	r3, [pc, #396]
	add	r3, fp
	ldr	r3, [r3, #0]
	movs	r1, #0
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	Func_080b5088
.L21:
	ldr	r3, [pc, #380]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #12]
	ldr	r0, [r3, #8]
	bl	Func_080e396c
	ldr	r5, [sp, #12]
	ldr	r3, [r5, #4]
	mov	r0, r9
	adds	r3, #16
	str	r3, [r5, #4]
	cmp	r0, #1
	bgt.n	.L22
	movs	r1, #120
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #52]
	mov	r1, fp
	b.n	.L23
.L22:
	mov	r2, r9
	cmp	r2, #3
	bgt.n	.L24
	movs	r1, #225
	movs	r3, #120
	lsls	r1, r1, #6
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #52]
	add	r1, fp
	b.n	.L23
.L24:
	mov	r5, r9
	cmp	r5, #5
	bgt.n	.L25
	movs	r0, #120
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #52]
	ldr	r1, [pc, #308]
.L23:
	movs	r2, #0
	movs	r3, #0
	bl	Func_080072f4
	b.n	.L26
.L25:
	mov	r1, r9
	cmp	r1, #7
	bgt.n	.L26
	movs	r2, #120
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #52]
	ldr	r1, [pc, #284]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080072f4
.L26:
	bl	Func_080049ac
	ldr	r0, [sp, #40]
	ldr	r1, [sp, #8]
	bl	Func_080051d8
	mov	r3, r9
	subs	r3, #4
	cmp	r3, #27
	bhi.n	.L27
	movs	r5, #64
	movs	r3, #0
	add	r5, sp
	mov	sl, r3
	mov	r8, r5
.L29:
	mov	r0, sl
	lsrs	r3, r0, #31
	add	r3, sl
	asrs	r5, r3, #1
	lsls	r3, r5, #3
	subs	r3, r3, r5
	lsls	r3, r3, #2
	movs	r1, #225
	add	r3, fp
	lsls	r1, r1, #7
	adds	r7, r3, r1
	ldr	r6, [r7, #24]
	cmp	r6, #0
	ble.n	.L28
	mov	r1, r8
	adds	r0, r7, #0
	bl	Func_080e3944
	mov	r3, r8
	ldr	r2, [r3, #0]
	ldr	r0, [sp, #48]
	adds	r2, r2, r0
	str	r2, [r3, #0]
	asrs	r6, r6, #3
	ldr	r3, [r3, #4]
	adds	r6, #2
	movs	r0, #1
	lsls	r4, r6, #1
	adds	r3, #16
	mov	r1, r8
	ands	r0, r5
	ldr	r5, [pc, #184]
	str	r3, [r1, #4]
	subs	r1, r4, #2
	ldrh	r1, [r5, r1]
	ldr	r5, [sp, #44]
	str	r4, [sp, #0]
	adds	r1, r5, r1
	str	r4, [sp, #4]
	ldr	r5, [sp, #16]
	lsls	r0, r0, #2
	subs	r3, r3, r6
	ldr	r4, [r0, r5]
	subs	r2, r2, r6
	ldr	r0, [sp, #52]
	bl	Func_080072f4
	adds	r0, r7, #0
	movs	r1, #60
	ldr	r2, [pc, #152]
	bl	Func_080e38b8
	ldr	r3, [r7, #24]
	subs	r3, #1
	str	r3, [r7, #24]
.L28:
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	cmp	r1, #64
	bne.n	.L29
.L27:
	ldr	r2, [pc, #136]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	cmp	r3, #32
	beq.n	.L30
	b.n	.L31
.L30:
	ldr	r0, [pc, #112]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	ldr	r3, [pc, #72]
	mov	r5, r9
	strh	r5, [r3, #6]
	ldr	r3, [pc, #92]
	movs	r1, #201
	ldr	r3, [r3, #0]
	movs	r0, #0
	lsls	r1, r1, #3
	mov	r9, r0
	adds	r5, r3, r1
	movs	r6, #6
.L32:
	mov	r2, r9
	ldrh	r0, [r5, #0]
	subs	r1, r6, r2
	bl	Func_080b5048
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	cmp	r0, #7
	bne.n	.L32
	bl	Func_080cdd14
	add	sp, #112
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00007828
	.4byte 0x03001ad0
	.4byte 0x02010000
	.4byte 0x02013840
	.4byte 0x080ede5c
	.4byte 0xfffffc00
	.4byte 0x00007824
	.4byte 0x080cd261
	.4byte 0x03001e74
