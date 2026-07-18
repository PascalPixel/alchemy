@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080cfef4
	.thumb_func
Func_080cfef4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #100]
	adds	r3, r6, #0
	ldmia	r3!, {r1}
	sub	sp, #76
	str	r1, [sp, #40]
	ldr	r3, [r3, #0]
	str	r3, [sp, #36]
	ldr	r3, [pc, #88]
	adds	r1, r1, r3
	str	r0, [r1, #0]
	movs	r2, #0
	movs	r0, #0
	mov	r8, r1
	mov	sl, r2
	bl	Func_080cdb24
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r0, [pc, #68]
	bl	Func_08002f40
	adds	r7, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #60]
	movs	r2, #128
	adds	r1, r7, #0
	lsls	r0, r0, #19
	bl	Func_080072f0
	adds	r7, #128
	ldr	r1, [sp, #40]
	adds	r0, r7, #0
	bl	Func_08005340
	ldr	r0, [pc, #44]
	bl	Func_08002f40
	ldr	r5, [pc, #40]
	adds	r7, r0, #0
	adds	r7, #128
	adds	r1, r5, #0
	adds	r0, r7, #0
	b.n	.L0
	.4byte 0x00000100
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x000000ab
	.4byte 0x03001388
	.4byte 0x000000ac
	.4byte 0x02010000
.L0:
	bl	Func_08005340
	movs	r5, #1
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	Func_080ed408
	ldr	r3, [r6, #28]
	movs	r2, #7
	str	r3, [sp, #44]
	movs	r1, #7
	movs	r3, #7
	movs	r0, #47
	str	r5, [sp, #0]
	bl	Func_080ed408
	ldr	r3, [r6, #32]
	mov	r4, sp
	movs	r6, #144
	adds	r4, #44
	lsls	r6, r6, #3
	str	r4, [sp, #16]
	adds	r1, r6, #0
	str	r3, [r4, #4]
	ldr	r0, [pc, #224]
	bl	Func_080041d8
	movs	r2, #239
	ldr	r1, [sp, #40]
	lsls	r2, r2, #7
	ldr	r4, [pc, #216]
	adds	r3, r1, r2
	str	r5, [r3, #0]
	adds	r3, r1, r4
	mov	r1, sl
	str	r1, [r3, #0]
	ldr	r0, [pc, #208]
	adds	r1, r6, #0
	bl	Func_080041d8
	str	r5, [sp, #24]
	mov	r2, r8
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L1
	ldr	r3, [pc, #192]
	str	r3, [sp, #28]
	b.n	.L2
.L1:
	movs	r4, #224
	lsls	r4, r4, #15
	str	r4, [sp, #28]
.L2:
	ldr	r4, [pc, #184]
	ldr	r3, [sp, #40]
	ldr	r1, [pc, #184]
	adds	r4, r3, r4
	movs	r2, #0
	str	r1, [sp, #32]
	str	r4, [sp, #20]
	mov	r8, r2
.L32:
	mov	r1, r8
	lsls	r5, r1, #9
	adds	r0, r5, #0
	bl	Func_08002322
	ldr	r2, [sp, #28]
	lsls	r0, r0, #4
	asrs	r3, r2, #16
	asrs	r0, r0, #16
	adds	r3, r3, r0
	adds	r0, r5, #0
	adds	r7, r3, #0
	bl	Func_0800231c
	ldr	r4, [sp, #32]
	lsls	r0, r0, #2
	asrs	r3, r4, #16
	asrs	r0, r0, #16
	adds	r3, r3, r0
	adds	r3, #16
	mov	r1, r8
	adds	r7, #48
	mov	fp, r3
	cmp	r1, #88
	bne.n	.L3
	movs	r0, #134
	bl	Func_080f9010
.L3:
	mov	r2, r8
	cmp	r2, #32
	bne.n	.L4
	ldr	r4, [sp, #20]
	ldr	r3, [r4, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L5
	ldr	r1, [pc, #100]
	str	r1, [sp, #28]
	b.n	.L6
.L5:
	movs	r2, #144
	lsls	r2, r2, #15
	str	r2, [sp, #28]
.L6:
	movs	r3, #192
	lsls	r3, r3, #13
	movs	r4, #0
	str	r3, [sp, #32]
	str	r4, [sp, #24]
.L4:
	mov	r1, r8
	cmp	r1, #33
	bne.n	.L7
	ldr	r3, [pc, #48]
	ldr	r2, [pc, #72]
	strh	r3, [r2, #0]
	movs	r3, #1
	str	r3, [sp, #24]
.L7:
	mov	r4, r8
	cmp	r4, #64
	bne.n	.L8
	ldr	r1, [sp, #20]
	ldr	r3, [r1, #0]
	add	r5, sp, #52
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	adds	r1, r5, #0
	bl	Func_080e396c
	ldr	r4, [sp, #20]
	ldr	r3, [r4, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L9
	ldr	r3, [r5, #0]
	subs	r3, #128
	b.n	.L10
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x080dbb9d
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0xffb00000
	.4byte 0x00007828
	.4byte 0xffe00000
	.4byte 0x04000052
.L9:
	ldr	r3, [r5, #0]
	subs	r3, #64
.L10:
	lsls	r3, r3, #16
	str	r3, [sp, #28]
	movs	r1, #0
	str	r1, [sp, #32]
	str	r1, [sp, #24]
.L8:
	mov	r2, r8
	cmp	r2, #65
	bne.n	.L11
	ldr	r3, [pc, #48]
	ldr	r4, [pc, #60]
	movs	r1, #1
	strh	r3, [r4, #0]
	str	r1, [sp, #24]
.L11:
	ldr	r2, [sp, #40]
	movs	r3, #211
	lsls	r3, r3, #7
	adds	r1, r2, r3
	mov	r2, r8
	movs	r4, #0
	cmp	r2, #31
	bgt.n	.L12
	cmp	r2, #15
	ble.n	.L13
	lsls	r3, r2, #1
	adds	r4, r3, #0
	ldr	r3, [pc, #20]
	subs	r3, r3, r2
	ldr	r2, [pc, #20]
	orrs	r3, r2
	ldr	r2, [pc, #20]
	subs	r4, #32
	strh	r3, [r2, #0]
	b.n	.L14
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x0000001f
	.4byte 0x00001000
	.4byte 0x04000052
.L14:
	b.n	.L13
.L12:
	mov	r3, r8
	cmp	r3, #63
	bgt.n	.L13
	cmp	r3, #47
	ble.n	.L13
	lsls	r3, r3, #1
	adds	r4, r3, #0
	ldr	r3, [pc, #32]
	mov	r2, r8
	subs	r3, r3, r2
	ldr	r2, [pc, #32]
	orrs	r3, r2
	ldr	r2, [pc, #32]
	strh	r3, [r2, #0]
	subs	r4, #96
.L13:
	cmp	r4, #0
	bge.n	.L15
	movs	r4, #0
.L15:
	movs	r3, #6
	subs	r3, r3, r7
	lsls	r7, r3, #8
	mov	r3, r8
	movs	r6, #0
	lsls	r5, r3, #11
	b.n	.L16
	.4byte 0x0000003f
	.4byte 0x00001000
	.4byte 0x04000052
.L16:
	adds	r0, r5, #0
	str	r1, [sp, #12]
	str	r4, [sp, #8]
	bl	Func_08002322
	ldr	r4, [sp, #8]
	adds	r3, r4, #0
	muls	r3, r0
	ldr	r1, [sp, #12]
	asrs	r3, r3, #10
	movs	r2, #128
	subs	r3, r7, r3
	lsls	r2, r2, #4
	adds	r6, #1
	stmia	r1!, {r3}
	adds	r5, r5, r2
	cmp	r6, #160
	bne.n	.L16
	ldr	r3, [sp, #24]
	cmp	r3, #0
	bne.n	.L17
	b.n	.L18
.L17:
	ldr	r4, [sp, #20]
	ldr	r3, [r4, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L19
	movs	r1, #0
	movs	r7, #0
	mov	r9, r1
	b.n	.L20
.L19:
	movs	r2, #0
	movs	r7, #1
	mov	r9, r2
.L20:
	mov	r3, r8
	cmp	r3, #87
	bgt.n	.L21
	ldr	r4, [sp, #20]
	ldr	r3, [r4, #0]
	ldr	r2, [pc, #648]
	ldr	r0, [r3, #4]
	lsls	r3, r7, #3
	mov	r4, r9
	subs	r3, r3, r7
	ldrb	r2, [r2, r3]
	ldr	r1, [pc, #640]
	lsls	r3, r4, #3
	subs	r3, r3, r4
	ldrb	r3, [r1, r3]
	movs	r1, #57
	str	r1, [sp, #0]
	movs	r1, #98
	str	r1, [sp, #4]
	ldr	r1, [sp, #16]
	lsls	r0, r0, #2
	ldr	r4, [r0, r1]
	add	r3, fp
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #40]
	bl	Func_080072f4
	b.n	.L18
.L21:
	mov	r2, r8
	cmp	r2, #91
	bgt.n	.L22
	ldr	r4, [sp, #20]
	ldr	r3, [r4, #0]
	ldr	r2, [pc, #592]
	lsls	r6, r7, #3
	ldr	r0, [r3, #4]
	subs	r3, r6, r7
	ldrb	r2, [r2, r3]
	mov	r3, r9
	ldr	r1, [pc, #584]
	lsls	r5, r3, #3
	subs	r3, r5, r3
	ldrb	r3, [r1, r3]
	movs	r1, #57
	str	r1, [sp, #0]
	movs	r1, #98
	str	r1, [sp, #4]
	ldr	r1, [sp, #16]
	lsls	r0, r0, #2
	ldr	r4, [r0, r1]
	add	r3, fp
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #40]
	bl	Func_080072f4
	b.n	.L23
.L22:
	mov	r2, r9
	lsls	r6, r7, #3
	lsls	r5, r2, #3
.L23:
	ldr	r3, [sp, #40]
	ldr	r4, [pc, #544]
	adds	r3, r3, r4
	mov	sl, r3
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #528]
	subs	r6, r6, r7
	ldr	r0, [r3, #4]
	adds	r3, r6, #1
	ldrb	r3, [r1, r3]
	mov	r2, r9
	ldr	r7, [pc, #520]
	subs	r5, r5, r2
	mov	ip, r3
	adds	r3, r5, #1
	ldrb	r3, [r7, r3]
	movs	r1, #99
	str	r1, [sp, #0]
	add	r3, fp
	movs	r1, #69
	mov	lr, r3
	str	r1, [sp, #4]
	ldr	r3, [sp, #16]
	lsls	r0, r0, #2
	ldr	r4, [r0, r3]
	ldr	r2, [sp, #40]
	ldr	r3, [pc, #500]
	ldr	r0, [sp, #36]
	adds	r1, r2, r3
	mov	r3, lr
	mov	r2, ip
	bl	Func_080072f4
	mov	r3, r8
	subs	r3, #88
	cmp	r3, #1
	bhi.n	.L24
	movs	r1, #128
	ldr	r3, [pc, #480]
	ldr	r0, [sp, #36]
	lsls	r1, r1, #7
	ldr	r2, [pc, #476]
	bl	Func_080072f0
.L24:
	mov	r3, r8
	subs	r3, #90
	cmp	r3, #1
	bhi.n	.L25
	mov	r4, sl
	ldr	r3, [r4, #0]
	ldr	r1, [pc, #440]
	ldr	r0, [r3, #4]
	adds	r3, r6, #2
	ldrb	r3, [r1, r3]
	movs	r1, #128
	mov	ip, r3
	adds	r3, r5, #2
	ldrb	r3, [r7, r3]
	str	r1, [sp, #0]
	movs	r1, #91
	str	r1, [sp, #4]
	ldr	r2, [sp, #16]
	lsls	r0, r0, #2
	add	r3, fp
	mov	lr, r3
	ldr	r4, [r0, r2]
	ldr	r3, [sp, #40]
	ldr	r2, [pc, #428]
	ldr	r0, [sp, #36]
	adds	r1, r3, r2
	mov	r2, ip
	mov	r3, lr
	bl	Func_080072f4
.L25:
	mov	r3, r8
	subs	r3, #92
	cmp	r3, #1
	bhi.n	.L26
	mov	r4, sl
	ldr	r3, [r4, #0]
	ldr	r1, [pc, #380]
	ldr	r0, [r3, #4]
	adds	r3, r6, #3
	ldrb	r2, [r1, r3]
	movs	r1, #128
	adds	r3, r5, #3
	ldrb	r3, [r7, r3]
	str	r1, [sp, #0]
	movs	r1, #91
	str	r1, [sp, #4]
	ldr	r1, [sp, #16]
	lsls	r0, r0, #2
	ldr	r4, [r0, r1]
	add	r3, fp
	ldr	r0, [sp, #36]
	ldr	r1, [pc, #376]
	bl	Func_080072f4
.L26:
	mov	r3, r8
	subs	r3, #94
	cmp	r3, #1
	bhi.n	.L27
	mov	r2, sl
	ldr	r3, [r2, #0]
	ldr	r4, [pc, #332]
	ldr	r0, [r3, #4]
	adds	r3, r6, #4
	ldrb	r2, [r4, r3]
	movs	r1, #128
	adds	r3, r5, #4
	ldrb	r3, [r7, r3]
	str	r1, [sp, #0]
	movs	r1, #59
	str	r1, [sp, #4]
	ldr	r1, [sp, #16]
	lsls	r0, r0, #2
	ldr	r4, [r0, r1]
	add	r3, fp
	ldr	r0, [sp, #36]
	ldr	r1, [pc, #332]
	bl	Func_080072f4
.L27:
	mov	r3, r8
	subs	r3, #96
	cmp	r3, #1
	bhi.n	.L28
	mov	r2, sl
	ldr	r3, [r2, #0]
	ldr	r4, [pc, #284]
	ldr	r0, [r3, #4]
	adds	r3, r6, #5
	ldrb	r2, [r4, r3]
	movs	r1, #122
	adds	r3, r5, #5
	ldrb	r3, [r7, r3]
	str	r1, [sp, #0]
	movs	r1, #29
	str	r1, [sp, #4]
	ldr	r1, [sp, #16]
	lsls	r0, r0, #2
	ldr	r4, [r0, r1]
	add	r3, fp
	ldr	r0, [sp, #36]
	ldr	r1, [pc, #288]
	bl	Func_080072f4
.L28:
	mov	r3, r8
	subs	r3, #98
	cmp	r3, #1
	bhi.n	.L18
	mov	r2, sl
	ldr	r3, [r2, #0]
	ldr	r4, [pc, #236]
	ldr	r0, [r3, #4]
	adds	r3, r6, #6
	ldrb	r2, [r4, r3]
	movs	r1, #76
	adds	r3, r5, #6
	ldrb	r3, [r7, r3]
	str	r1, [sp, #0]
	movs	r1, #25
	str	r1, [sp, #4]
	ldr	r1, [sp, #16]
	lsls	r0, r0, #2
	ldr	r4, [r0, r1]
	add	r3, fp
	ldr	r0, [sp, #36]
	ldr	r1, [pc, #244]
	bl	Func_080072f4
.L18:
	mov	r2, r8
	cmp	r2, #88
	bne.n	.L29
	ldr	r4, [sp, #20]
	ldr	r3, [r4, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	bl	Func_080b5098
	movs	r3, #128
	ldr	r0, [r0, #0]
	lsls	r3, r3, #9
	str	r3, [r0, #40]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r0, #52]
	str	r3, [r0, #48]
	adds	r3, r0, #0
	movs	r5, #0
	adds	r3, #90
	str	r5, [r0, #72]
	strb	r5, [r3, #0]
	subs	r3, #2
	strb	r5, [r3, #0]
	ldr	r1, [r0, #8]
	ldr	r3, [r0, #16]
	lsls	r1, r1, #1
	movs	r2, #0
	bl	Func_08009150
	ldr	r2, [sp, #20]
	ldr	r3, [r2, #0]
	movs	r4, #36
	ldrsh	r0, [r3, r4]
	movs	r3, #1
	negs	r3, r3
	adds	r1, r3, #0
	movs	r2, #5
	str	r5, [sp, #0]
	bl	Func_080d6888
.L29:
	mov	r1, r8
	cmp	r1, #120
	bne.n	.L30
	ldr	r2, [sp, #20]
	ldr	r3, [r2, #0]
	movs	r4, #36
	ldrsh	r0, [r3, r4]
	bl	Func_080b5098
	ldr	r3, [pc, #144]
	ldr	r2, [r0, #0]
	str	r3, [r2, #72]
.L30:
	ldr	r1, [sp, #40]
	ldr	r3, [pc, #140]
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r4, #1
	add	r8, r4
	mov	r1, r8
	cmp	r1, #132
	beq.n	.L31
	b.n	.L32
.L31:
	ldr	r0, [pc, #116]
	bl	Func_08004278
	ldr	r0, [pc, #116]
	bl	Func_08004278
	ldr	r3, [pc, #112]
	movs	r2, #201
	ldr	r3, [r3, #0]
	lsls	r2, r2, #3
	adds	r3, r3, r2
	ldrh	r1, [r3, #0]
	movs	r2, #24
	movs	r0, #1
	bl	Func_080b5040
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #76
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080ee10c
	.4byte 0x080ee11a
	.4byte 0x00007828
	.4byte 0x000015d2
	.4byte 0x03000168
	.4byte 0x3f3f3f3f
	.4byte 0x00003081
	.4byte 0x02010000
	.4byte 0x02012d80
	.4byte 0x02014b00
	.4byte 0x020158d2
	.4byte 0x0000ab85
	.4byte 0x00007824
	.4byte 0x080cd261
	.4byte 0x080dbb9d
	.4byte 0x03001e74
