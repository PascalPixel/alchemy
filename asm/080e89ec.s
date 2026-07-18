@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080e89ec
	.thumb_func
Func_080e89ec:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #92]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #228
	str	r3, [sp, #44]
	movs	r3, #0
	str	r3, [sp, #36]
	str	r3, [sp, #28]
	str	r3, [sp, #24]
	ldr	r3, [pc, #76]
	mov	r9, r1
	ldr	r2, [r2, #8]
	add	r3, r9
	str	r2, [sp, #20]
	str	r0, [r3, #0]
	movs	r0, #0
	bl	Func_080cd594
	ldr	r2, [pc, #60]
	ldr	r3, [pc, #48]
	ldr	r0, [pc, #60]
	strh	r3, [r2, #0]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r5, #234
	ldr	r0, [pc, #48]
	movs	r1, #180
	lsls	r5, r5, #2
	lsls	r1, r1, #5
	mov	r2, r9
	movs	r7, #1
	mov	lr, r0
	mov	ip, r1
	adds	r4, r2, r5
.L3:
	mov	r3, lr
	mov	r2, r9
	movs	r6, #0
	lsls	r0, r7, #2
	adds	r1, r4, r3
	add	r2, ip
	b.n	.L0
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000052
	.4byte 0x000000c2
	.4byte 0xfffff1f0
.L0:
	ldrb	r3, [r2, #0]
	adds	r2, #1
	cmp	r7, #10
	ble.n	.L1
	subs	r3, r3, r0
	adds	r3, #40
	cmp	r3, #0
	bge.n	.L2
	movs	r3, #0
.L2:
	strb	r3, [r1, #0]
.L1:
	adds	r6, #1
	adds	r1, #1
	cmp	r6, r5
	bne.n	.L0
	movs	r0, #234
	lsls	r0, r0, #2
	adds	r7, #1
	adds	r4, r4, r0
	cmp	r7, #20
	bne.n	.L3
	ldr	r1, [sp, #20]
	ldr	r0, [pc, #840]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	movs	r1, #240
	lsls	r1, r1, #6
	ldr	r0, [pc, #828]
	add	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r2, #1
	ldr	r0, [pc, #820]
	ldr	r1, [pc, #820]
	movs	r3, #0
	bl	Func_080e0524
	ldr	r3, [pc, #816]
	add	r3, r9
	ldr	r3, [r3, #0]
	mov	r1, sp
	ldr	r0, [r3, #4]
	adds	r1, #48
	str	r1, [sp, #16]
	bl	Func_080cef64
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #792]
	movs	r3, #75
	add	r2, r9
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #788]
	lsls	r1, r1, #3
	bl	Func_080041d8
	ldr	r5, [pc, #784]
	movs	r6, #0
	movs	r7, #63
	add	r5, r9
.L6:
	ldr	r3, [pc, #764]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L4
	ldr	r3, [pc, #768]
	b.n	.L5
.L4:
	movs	r3, #224
	lsls	r3, r3, #14
.L5:
	str	r3, [r5, #0]
	movs	r3, #0
	str	r3, [r5, #4]
	str	r3, [r5, #8]
	bl	Func_08004458
	ands	r0, r7
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r5, #12]
	bl	Func_08004458
	ands	r0, r7
	lsls	r0, r0, #13
	str	r0, [r5, #16]
	bl	Func_08004458
	ands	r0, r7
	subs	r0, #32
	lsls	r0, r0, #14
	movs	r3, #1
	adds	r6, #1
	str	r0, [r5, #20]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r6, #40
	bne.n	.L6
	ldr	r5, [pc, #708]
	movs	r6, #0
	mov	r8, r6
	movs	r7, #63
	add	r5, r9
.L9:
	ldr	r3, [pc, #680]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L7
	ldr	r3, [pc, #684]
	b.n	.L8
.L7:
	movs	r3, #224
	lsls	r3, r3, #14
.L8:
	str	r3, [r5, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	mov	r2, r8
	str	r3, [r5, #4]
	str	r2, [r5, #8]
	bl	Func_08004458
	ands	r0, r7
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r5, #12]
	bl	Func_08004458
	ands	r0, r7
	lsls	r0, r0, #12
	str	r0, [r5, #16]
	bl	Func_08004458
	ands	r0, r7
	subs	r0, #32
	lsls	r0, r0, #14
	mov	r3, r8
	adds	r6, #1
	str	r0, [r5, #20]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r6, #16
	bne.n	.L9
	movs	r5, #225
	lsls	r5, r5, #7
	ldr	r6, [pc, #620]
	movs	r7, #0
	add	r5, r9
.L12:
	ldr	r3, [pc, #592]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L10
	adds	r0, r6, #0
	bl	Func_08002322
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
	asrs	r3, r3, #16
	adds	r3, #88
	b.n	.L11
.L10:
	adds	r0, r6, #0
	bl	Func_08002322
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
	negs	r3, r3
	asrs	r3, r3, #16
	adds	r3, #16
.L11:
	str	r3, [r5, #0]
	adds	r0, r6, #0
	bl	Func_0800231c
	lsls	r0, r0, #4
	asrs	r0, r0, #16
	adds	r0, #40
	str	r0, [r5, #4]
	lsls	r3, r7, #1
	movs	r0, #128
	negs	r3, r3
	lsls	r0, r0, #5
	adds	r7, #1
	str	r3, [r5, #24]
	adds	r6, r6, r0
	adds	r5, #28
	cmp	r7, #8
	bne.n	.L12
	ldr	r0, [pc, #536]
	bl	Func_08002f40
	ldr	r2, [pc, #500]
	movs	r1, #0
	add	r2, r9
	str	r0, [sp, #32]
	str	r1, [sp, #40]
	str	r2, [sp, #12]
.L49:
	ldr	r3, [pc, #520]
	ldr	r3, [r3, #0]
	mov	fp, r3
	ldr	r3, [sp, #40]
	cmp	r3, #83
	bne.n	.L13
	movs	r0, #134
	bl	Func_080b50e8
.L13:
	ldr	r0, [sp, #40]
	cmp	r0, #0
	bne.n	.L14
	movs	r0, #136
	bl	Func_080f9010
.L14:
	ldr	r1, [sp, #40]
	cmp	r1, #50
	bne.n	.L15
	movs	r0, #136
	bl	Func_080f9010
.L15:
	ldr	r2, [sp, #12]
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L16
	ldr	r3, [sp, #40]
	cmp	r3, #63
	bgt.n	.L17
	mov	r0, fp
	ldrh	r3, [r0, #54]
	ldr	r1, [pc, #464]
	mov	r2, fp
	adds	r3, r3, r1
	b.n	.L18
.L16:
	ldr	r3, [sp, #40]
	cmp	r3, #63
	bgt.n	.L17
	mov	r0, fp
	ldrh	r3, [r0, #54]
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r3, r3, r1
	mov	r2, fp
.L18:
	strh	r3, [r2, #54]
.L17:
	movs	r3, #100
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl	Func_080b5028
	ldr	r3, [sp, #40]
	cmp	r3, #17
	bgt.n	.L19
	adds	r0, r3, #0
	movs	r1, #3
	bl	Func_080022ec
	ldr	r2, [pc, #412]
	adds	r5, r0, #0
	lsls	r0, r5, #1
	ldrh	r1, [r2, r0]
	mov	sl, r0
	movs	r3, #240
	ldr	r0, [pc, #404]
	lsls	r3, r3, #6
	mov	r8, r3
	ldrb	r3, [r0, r5]
	ldr	r0, [pc, #400]
	ldrb	r2, [r0, r5]
	ldr	r6, [pc, #400]
	str	r2, [sp, #0]
	ldrb	r2, [r6, r5]
	add	r1, r9
	add	r1, r8
	adds	r3, #60
	str	r2, [sp, #4]
	ldr	r4, [sp, #48]
	movs	r2, #48
	ldr	r0, [sp, #44]
	bl	Func_080072f4
	ldr	r2, [pc, #364]
	ldr	r0, [pc, #368]
	mov	r3, sl
	ldrh	r1, [r2, r3]
	ldrb	r3, [r0, r5]
	ldr	r0, [pc, #364]
	ldrb	r2, [r0, r5]
	str	r2, [sp, #0]
	ldrb	r2, [r6, r5]
	str	r2, [sp, #4]
	ldr	r2, [sp, #16]
	add	r1, r9
	ldr	r4, [r2, #4]
	add	r1, r8
	adds	r3, #60
	ldr	r0, [sp, #44]
	movs	r2, #56
	bl	Func_080072f4
.L19:
	ldr	r3, [sp, #40]
	subs	r3, #18
	str	r3, [sp, #8]
	cmp	r3, #40
	bhi.n	.L20
	ldr	r0, [sp, #40]
	cmp	r0, #18
	bne.n	.L21
	ldr	r1, [sp, #32]
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	ldrb	r2, [r1, #1]
	lsls	r3, r3, #8
	adds	r3, r3, r2
	str	r3, [sp, #28]
	movs	r3, #2
	ldrsb	r3, [r1, r3]
	ldrb	r2, [r1, #3]
	lsls	r3, r3, #8
	adds	r3, r3, r2
	adds	r3, #16
	adds	r1, #4
	str	r3, [sp, #24]
	str	r1, [sp, #32]
	b.n	.L20
.L21:
	ldr	r2, [sp, #32]
	ldr	r0, [sp, #28]
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	adds	r0, r0, r3
	str	r0, [sp, #28]
	ldr	r1, [sp, #24]
	movs	r3, #1
	ldrsb	r3, [r2, r3]
	adds	r2, #2
	adds	r1, r1, r3
	str	r1, [sp, #24]
	str	r2, [sp, #32]
.L20:
	ldr	r3, [sp, #40]
	subs	r3, #78
	cmp	r3, #40
	bhi.n	.L22
	ldr	r2, [sp, #40]
	cmp	r2, #78
	bne.n	.L23
	movs	r3, #56
	negs	r3, r3
	movs	r0, #48
	str	r3, [sp, #28]
	str	r0, [sp, #24]
	b.n	.L22
.L23:
	ldr	r1, [sp, #24]
	subs	r1, #16
	str	r1, [sp, #24]
.L22:
	movs	r2, #24
	movs	r3, #39
	movs	r6, #19
	mov	sl, r2
	mov	r8, r3
	movs	r7, #156
.L26:
	adds	r3, r6, #0
	ldr	r0, [sp, #40]
	adds	r3, #18
	cmp	r0, r3
	ble.n	.L24
	adds	r3, #65
	cmp	r0, r3
	bgt.n	.L24
	lsls	r0, r6, #3
	adds	r3, r0, #0
	add	r2, sp, #68
	subs	r3, #8
	ldr	r3, [r2, r3]
	str	r3, [r2, r0]
	subs	r3, r0, #4
	ldr	r5, [r2, r3]
	str	r5, [r2, r7]
	cmp	r6, #10
	ble.n	.L25
	movs	r3, #234
	lsls	r3, r3, #2
	adds	r1, r6, #0
	muls	r1, r3
	ldr	r3, [pc, #176]
	add	r1, r9
	ldr	r2, [r2, r0]
	adds	r1, r1, r3
	mov	r0, sl
	mov	r3, r8
	str	r0, [sp, #0]
	str	r3, [sp, #4]
	ldr	r4, [sp, #48]
	ldr	r0, [sp, #44]
	adds	r3, r5, #0
	bl	Func_080072f4
	b.n	.L24
.L25:
	mov	r1, r8
	ldr	r2, [r2, r0]
	str	r1, [sp, #4]
	movs	r1, #180
	mov	r0, sl
	lsls	r1, r1, #5
	str	r0, [sp, #0]
	ldr	r4, [sp, #48]
	ldr	r0, [sp, #44]
	add	r1, r9
	adds	r3, r5, #0
	bl	Func_080072f4
.L24:
	subs	r6, #1
	subs	r7, #8
	cmp	r6, #0
	bne.n	.L26
	bl	Func_080049ac
	mov	r1, fp
	adds	r1, #12
	mov	r0, fp
	bl	Func_080051d8
	ldr	r2, [sp, #8]
	cmp	r2, #65
	bhi.n	.L27
	ldr	r0, [sp, #12]
	ldr	r3, [r0, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L28
	ldr	r3, [sp, #28]
	lsrs	r2, r3, #31
	adds	r2, r3, r2
	asrs	r2, r2, #1
	movs	r3, #64
	add	r1, sp, #56
	subs	r3, r3, r2
	b.n	.L29
	.4byte 0x00000073
	.4byte 0x000000b4
	.4byte 0x0000007d
	.4byte 0x02010000
	.4byte 0x00007828
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007160
	.4byte 0xffc80000
	.4byte 0x000075c0
	.4byte 0xffffc000
	.4byte 0x000000d3
	.4byte 0x03001e80
	.4byte 0xffffff00
	.4byte 0x080edeb2
	.4byte 0x080edeab
	.4byte 0x080ede9f
	.4byte 0x080edea5
	.4byte 0xfffff1f0
.L28:
	ldr	r0, [sp, #28]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	add	r1, sp, #56
	adds	r3, #64
.L29:
	str	r3, [r1, #0]
	ldr	r2, [sp, #24]
	movs	r3, #60
	subs	r3, r3, r2
	str	r3, [r1, #4]
	add	r4, sp, #68
	ldr	r2, [r4, #4]
	subs	r3, r3, r2
	subs	r3, #24
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r0, r3, #1
	cmp	r0, #2
	ble.n	.L30
	movs	r0, #2
.L30:
	movs	r3, #2
	negs	r3, r3
	cmp	r0, r3
	bge.n	.L31
	movs	r0, #2
	negs	r0, r0
.L31:
	ldr	r2, [sp, #36]
	adds	r2, r2, r0
	str	r2, [sp, #36]
	cmp	r2, #8
	ble.n	.L32
	movs	r3, #8
	str	r3, [sp, #36]
.L32:
	movs	r2, #8
	ldr	r0, [sp, #36]
	negs	r2, r2
	cmp	r0, r2
	bge.n	.L33
	str	r2, [sp, #36]
.L33:
	ldr	r3, [sp, #36]
	cmp	r3, #0
	bge.n	.L34
	adds	r3, #3
.L34:
	ldr	r2, [r1, #0]
	asrs	r3, r3, #2
	adds	r0, r3, #2
	adds	r3, r2, #0
	subs	r3, #12
	str	r3, [r4, #0]
	ldr	r3, [r1, #4]
	adds	r1, r3, #0
	subs	r1, #20
	str	r1, [r4, #4]
	lsls	r1, r0, #3
	adds	r1, r1, r0
	movs	r0, #24
	lsls	r1, r1, #7
	str	r0, [sp, #0]
	movs	r0, #48
	str	r0, [sp, #4]
	add	r1, r9
	subs	r2, #18
	subs	r3, #22
	ldr	r4, [sp, #48]
	ldr	r0, [sp, #44]
	bl	Func_080072f4
.L27:
	ldr	r3, [sp, #40]
	cmp	r3, #83
	bne.n	.L35
	ldr	r3, [pc, #444]
	movs	r2, #8
	add	r3, r9
	str	r2, [r3, #0]
	ldr	r0, [sp, #12]
	ldr	r3, [r0, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r1, #7
	str	r2, [sp, #0]
	movs	r3, #0
	movs	r2, #5
	bl	Func_080d6888
	ldr	r2, [sp, #12]
	ldr	r3, [r2, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	Func_080b5088
.L35:
	ldr	r2, [sp, #40]
	cmp	r2, #83
	ble.n	.L36
	ldr	r6, [pc, #400]
	movs	r7, #0
	add	r6, r9
.L43:
	ldr	r3, [r6, #4]
	cmp	r3, #0
	blt.n	.L37
	add	r5, sp, #56
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	Func_080e3944
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	ldr	r3, [r5, #8]
	cmp	r3, #159
	bgt.n	.L38
	movs	r3, #160
	str	r3, [r5, #8]
.L38:
	ldr	r2, [pc, #364]
	cmp	r3, r2
	ble.n	.L39
	str	r2, [r5, #8]
	adds	r3, r2, #0
.L39:
	adds	r2, r3, #0
	subs	r2, #160
	cmp	r2, #0
	bge.n	.L40
	adds	r2, #63
.L40:
	asrs	r2, r2, #6
	movs	r3, #9
	subs	r4, r3, r2
	cmp	r7, #47
	ble.n	.L41
	ldr	r3, [r6, #24]
	cmp	r3, #11
	bgt.n	.L42
	lsrs	r1, r3, #31
	adds	r1, r3, r1
	asrs	r1, r1, #1
	ldr	r3, [pc, #328]
	lsls	r1, r1, #11
	ldr	r2, [r5, #0]
	adds	r1, r1, r3
	movs	r0, #32
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	movs	r0, #64
	subs	r3, #32
	str	r0, [sp, #4]
	subs	r2, #16
	ldr	r4, [sp, #48]
	ldr	r0, [sp, #44]
	bl	Func_080072f4
	ldr	r3, [r6, #24]
	adds	r3, #1
	str	r3, [r6, #24]
	b.n	.L42
.L41:
	lsls	r0, r4, #1
	ldr	r2, [pc, #292]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #20]
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r4, [sp, #0]
	subs	r3, r3, r4
	str	r0, [sp, #4]
	ldr	r4, [sp, #48]
	ldr	r0, [sp, #44]
	bl	Func_080072f4
.L42:
	ldr	r3, [r6, #0]
	ldr	r2, [r6, #12]
	adds	r3, r3, r2
	ldr	r1, [r6, #16]
	str	r3, [r6, #0]
	ldr	r3, [r6, #4]
	adds	r3, r3, r1
	str	r3, [r6, #4]
	ldr	r2, [r6, #20]
	ldr	r3, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	ldr	r3, [pc, #236]
	adds	r1, r1, r3
	str	r1, [r6, #16]
.L37:
	adds	r7, #1
	adds	r6, #28
	cmp	r7, #56
	bne.n	.L43
.L36:
	ldr	r0, [sp, #40]
	cmp	r0, #50
	bne.n	.L44
	ldr	r2, [pc, #196]
	movs	r3, #12
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r1, [sp, #12]
	ldr	r3, [r1, #0]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	Func_080d6888
.L44:
	ldr	r3, [sp, #40]
	cmp	r3, #49
	ble.n	.L45
	movs	r6, #225
	lsls	r6, r6, #7
	movs	r7, #0
	add	r6, r9
.L47:
	ldr	r3, [r6, #24]
	cmp	r3, #11
	bhi.n	.L46
	lsrs	r4, r3, #31
	adds	r4, r3, r4
	asrs	r4, r4, #1
	ldr	r0, [pc, #164]
	lsls	r3, r4, #1
	ldrh	r1, [r0, r3]
	ldr	r3, [pc, #160]
	movs	r2, #240
	ldrb	r5, [r3, r4]
	lsls	r2, r2, #6
	add	r1, r9
	adds	r1, r1, r2
	ldr	r2, [r6, #0]
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #148]
	ldrb	r0, [r3, r4]
	ldr	r3, [r6, #4]
	str	r5, [sp, #0]
	adds	r3, r3, r0
	ldr	r0, [pc, #140]
	ldrb	r0, [r0, r4]
	str	r0, [sp, #4]
	ldr	r0, [sp, #16]
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #44]
	bl	Func_080072f4
	ldr	r3, [r6, #24]
.L46:
	adds	r3, #1
	adds	r7, #1
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r7, #8
	bne.n	.L47
.L45:
	movs	r1, #8
	movs	r0, #8
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r2, [pc, #104]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [sp, #40]
	adds	r1, #1
	str	r1, [sp, #40]
	cmp	r1, #150
	beq.n	.L48
	b.n	.L49
.L48:
	ldr	r0, [pc, #80]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #228
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x000077a8
	.4byte 0x00007160
	.4byte 0x0000031f
	.4byte 0x02010000
	.4byte 0x080ede48
	.4byte 0xffffe000
	.4byte 0x080edeb2
	.4byte 0x080ede9f
	.4byte 0x080edeab
	.4byte 0x080edea5
	.4byte 0x00007824
	.4byte 0x080cd261
