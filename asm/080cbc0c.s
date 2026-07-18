@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080cbc0c
	.thumb_func
Func_080cbc0c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	ldr	r1, [pc, #156]
	movs	r0, #39
	sub	sp, #56
	bl	Func_080048b0
	movs	r1, #128
	str	r0, [sp, #40]
	lsls	r1, r1, #7
	movs	r0, #40
	bl	Func_080048b0
	ldr	r1, [pc, #140]
	str	r0, [sp, #36]
	movs	r0, #41
	bl	Func_080048b0
	ldr	r3, [pc, #132]
	ldr	r0, [r3, #0]
	str	r0, [sp, #20]
	adds	r3, #140
	ldr	r1, [sp, #40]
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #124]
	mov	r9, r3
	adds	r3, r1, r2
	str	r5, [r3, #0]
	bl	Func_080cd508
	mov	r4, r9
	ldr	r2, [pc, #116]
	movs	r3, #1
	str	r3, [r4, #12]
	movs	r3, #32
	strh	r3, [r2, #6]
	ldr	r7, [sp, #20]
	movs	r0, #201
	lsls	r0, r0, #3
	adds	r3, r7, r0
	ldrh	r1, [r3, #0]
	movs	r0, #1
	movs	r2, #0
	bl	Func_080b5038
	ldr	r2, [pc, #92]
	ldr	r3, [pc, #52]
	movs	r1, #0
	strh	r3, [r2, #0]
	movs	r0, #0
	movs	r2, #0
	movs	r3, #100
	bl	Func_080b5028
	ldr	r3, [pc, #76]
	movs	r5, #0
	mov	r1, r9
	str	r5, [r1, #12]
	ldr	r2, [pc, #72]
	str	r5, [r3, #0]
	ldr	r3, [pc, #72]
	str	r3, [r2, #0]
	ldr	r3, [pc, #24]
	subs	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [pc, #68]
	strh	r5, [r3, #0]
	adds	r3, #2
	strh	r5, [r3, #0]
	ldr	r3, [pc, #16]
	adds	r2, #6
	strh	r3, [r2, #0]
	ldr	r1, [pc, #12]
	b.n	.L0
	.4byte 0x00000784
	.4byte 0x00000080
	.4byte 0x00000100
	.4byte 0x000000f0
	.4byte 0x0000782c
	.4byte 0x00000302
	.4byte 0x03001e74
	.4byte 0x00007828
	.4byte 0x03001ad0
	.4byte 0x0400000c
	.4byte 0x04000028
	.4byte 0x0400002c
	.4byte 0xfffff000
	.4byte 0x04000022
.L0:
	ldr	r3, [pc, #64]
	ldr	r2, [pc, #52]
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	subs	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	ldr	r2, [pc, #52]
	ldr	r3, [pc, #36]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #36]
	adds	r2, #2
	strh	r3, [r2, #0]
	movs	r4, #128
	ldr	r2, [pc, #40]
	movs	r3, #128
	movs	r6, #0
	lsls	r3, r3, #1
	lsls	r4, r4, #2
	mov	ip, r6
	mov	r8, r2
	mov	sl, r3
	mov	lr, r4
	movs	r7, #0
	b.n	.L1
	movs	r0, r0
	.4byte 0x00001088
	.4byte 0x00003537
	.4byte 0x00003f21
	.4byte 0x04000040
	.4byte 0x04000048
	.4byte 0x06003800
.L1:
	mov	r1, sl
	adds	r0, r7, r1
	movs	r4, #0
	lsls	r1, r5, #1
.L2:
	adds	r3, r0, #0
	orrs	r3, r1
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	mov	fp, r3
	mov	r3, r8
	adds	r2, r6, r3
	adds	r4, #1
	mov	r3, fp
	strh	r3, [r2, #0]
	add	r0, lr
	adds	r1, #2
	adds	r6, #2
	cmp	r4, #8
	bne.n	.L2
	movs	r0, #1
	movs	r4, #128
	add	ip, r0
	lsls	r4, r4, #5
	mov	r1, ip
	adds	r7, r7, r4
	adds	r5, #8
	cmp	r1, #16
	bne.n	.L1
	movs	r1, #128
	ldr	r5, [pc, #136]
	ldr	r0, [sp, #36]
	lsls	r1, r1, #7
	bl	Func_080072f8
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r0, [pc, #124]
	bl	Func_080072f8
	ldr	r1, [pc, #124]
	ldr	r0, [pc, #124]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	strh	r2, [r1, #0]
	ldr	r2, [pc, #104]
	adds	r3, #4
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L3:
	strh	r4, [r0, #0]
	ldr	r2, [pc, #88]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r1, [sp, #40]
	ldr	r0, [pc, #76]
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r4, #239
	ldr	r2, [sp, #40]
	lsls	r4, r4, #7
	adds	r3, r2, r4
	movs	r5, #1
	str	r5, [r3, #0]
	ldr	r7, [sp, #40]
	ldr	r0, [pc, #56]
	movs	r3, #0
	adds	r2, r7, r0
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #48]
	bl	Func_080041d8
	movs	r1, #7
	movs	r2, #7
	b.n	.L4
	.4byte 0x00001010
	.4byte 0x00000000
	.4byte 0x03000164
	.4byte 0x06004000
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x00007741
	.4byte 0x04000052
	.4byte 0x00000044
	.4byte 0x00007784
	.4byte 0x080cd261
.L4:
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	Func_080ed408
	ldr	r5, [pc, #388]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	str	r3, [sp, #24]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r0, #47
	movs	r3, #3
	bl	Func_080ed408
	adds	r5, #188
	ldr	r5, [r5, #0]
	movs	r2, #225
	movs	r1, #0
	lsls	r2, r2, #7
	str	r5, [sp, #28]
	str	r1, [sp, #12]
	ldr	r4, [pc, #352]
	adds	r1, r7, r2
	ldr	r0, [pc, #352]
	ldr	r7, [pc, #352]
	ldr	r5, [pc, #356]
	movs	r6, #0
.L5:
	ldrb	r2, [r0, #0]
	ldrb	r3, [r4, #0]
	lsls	r2, r2, #16
	lsls	r3, r3, #16
	str	r2, [r1, #0]
	str	r3, [r1, #4]
	adds	r2, r2, r7
	adds	r3, r3, r5
	asrs	r2, r2, #2
	asrs	r3, r3, #2
	adds	r6, #1
	str	r2, [r1, #12]
	str	r3, [r1, #16]
	adds	r0, #1
	adds	r4, #1
	adds	r1, #28
	cmp	r6, #33
	bne.n	.L5
	movs	r2, #240
	ldr	r3, [pc, #316]
	ldr	r1, [pc, #320]
	lsls	r2, r2, #7
	ldr	r0, [pc, #320]
	bl	Func_080072f0
	movs	r1, #240
	ldr	r5, [pc, #316]
	lsls	r1, r1, #7
	ldr	r2, [pc, #316]
	ldr	r0, [pc, #304]
	bl	Func_080072f8
	movs	r3, #1
	mov	r4, r9
	str	r3, [r4, #16]
	ldr	r2, [pc, #304]
	ldr	r7, [sp, #40]
	ldr	r0, [pc, #304]
	ldrh	r3, [r2, #4]
	adds	r1, r7, r0
	str	r3, [r1, #0]
	ldr	r3, [pc, #300]
	adds	r1, r7, r3
	ldrh	r3, [r2, #6]
	str	r3, [r1, #0]
	ldr	r1, [pc, #296]
	movs	r3, #0
	strh	r3, [r2, #4]
	ldr	r0, [pc, #296]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L6
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	strh	r2, [r1, #0]
	ldr	r2, [pc, #272]
	adds	r3, #4
	stmia	r3!, {r2}
	ldr	r2, [pc, #272]
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L6:
	strh	r4, [r0, #0]
	ldr	r0, [pc, #264]
	movs	r1, #128
	lsls	r1, r1, #1
	ldr	r2, [pc, #260]
	bl	Func_080072f8
	movs	r0, #212
	bl	Func_080f9010
	ldr	r5, [pc, #252]
	ldr	r4, [sp, #40]
	adds	r3, r4, r5
	ldr	r3, [r3, #0]
	movs	r7, #36
	ldrsh	r0, [r3, r7]
	movs	r3, #30
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #3
	movs	r3, #0
	bl	Func_080d6888
	movs	r0, #0
	str	r0, [sp, #32]
.L69:
	ldr	r1, [sp, #32]
	cmp	r1, #2
	bne.n	.L7
	movs	r0, #212
	bl	Func_080f9010
.L7:
	ldr	r2, [sp, #32]
	cmp	r2, #3
	bne.n	.L8
	movs	r0, #212
	bl	Func_080f9010
.L8:
	ldr	r3, [sp, #32]
	cmp	r3, #28
	bne.n	.L9
	ldr	r4, [sp, #40]
	ldr	r5, [pc, #192]
	adds	r3, r4, r5
	ldr	r3, [r3, #0]
	movs	r7, #36
	ldrsh	r0, [r3, r7]
	movs	r3, #1
	movs	r2, #0
	negs	r3, r3
	str	r2, [sp, #0]
	adds	r1, r3, #0
	movs	r2, #3
	bl	Func_080d6888
.L9:
	ldr	r0, [sp, #32]
	cmp	r0, #32
	bne.n	.L10
	movs	r0, #149
	bl	Func_080f9010
.L10:
	ldr	r1, [sp, #32]
	cmp	r1, #5
	bne.n	.L11
	movs	r0, #145
	bl	Func_080f9010
	ldr	r4, [sp, #40]
	ldr	r5, [pc, #108]
	adds	r3, r4, r5
	ldr	r2, [pc, #100]
	ldr	r3, [r3, #0]
	strh	r3, [r2, #4]
	ldr	r7, [sp, #20]
	movs	r0, #201
	lsls	r0, r0, #3
	adds	r3, r7, r0
	movs	r2, #1
	ldrh	r1, [r3, #0]
	movs	r0, #1
	negs	r2, r2
	bl	Func_080b5040
.L11:
	ldr	r1, [sp, #32]
	cmp	r1, #7
	ble.n	.L12
	ldr	r5, [sp, #20]
	ldr	r7, [pc, #104]
	ldr	r2, [pc, #92]
	ldr	r3, [pc, #16]
	movs	r4, #31
	adds	r5, r5, r7
	mov	lr, r2
	movs	r6, #0
	mov	ip, r3
	mov	r8, r4
	mov	r9, r5
	b.n	.L13
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x03001e50
	.4byte 0x080ee037
	.4byte 0x080ee016
	.4byte 0xffe00000
	.4byte 0xffc40000
	.4byte 0x03001388
	.4byte 0x06008000
	.4byte 0x02010000
	.4byte 0x03000168
	.4byte 0x01010101
	.4byte 0x03001ad0
	.4byte 0x000077a0
	.4byte 0x000077a4
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x00001f81
	.4byte 0x0400000a
	.4byte 0x050000c0
	.4byte 0x7fff7fff
	.4byte 0x00007828
	.4byte 0x00000544
.L13:
	mov	r0, lr
	ldrh	r3, [r0, #0]
	mov	r5, r8
	mov	r4, r9
	ands	r5, r3
	lsls	r3, r3, #16
	mov	r1, ip
	lsrs	r2, r3, #21
	lsrs	r0, r3, #26
	ldrh	r3, [r4, #0]
	ands	r2, r1
	ands	r0, r1
	mov	r1, r8
	ands	r1, r3
	lsls	r3, r3, #16
	movs	r7, #2
	mov	sl, r3
	lsrs	r4, r3, #21
	add	r9, r7
	mov	r3, ip
	mov	r7, sl
	ands	r4, r3
	lsrs	r3, r7, #26
	mov	r7, ip
	ands	r3, r7
	cmp	r5, r1
	bge.n	.L14
	adds	r5, #1
	b.n	.L15
.L14:
	cmp	r5, r1
	ble.n	.L15
	subs	r5, #1
.L15:
	cmp	r2, r4
	bge.n	.L16
	adds	r2, #1
	b.n	.L17
.L16:
	cmp	r2, r4
	ble.n	.L17
	subs	r2, #1
.L17:
	cmp	r0, r3
	bge.n	.L18
	adds	r0, #1
	b.n	.L19
.L18:
	cmp	r0, r3
	ble.n	.L19
	subs	r0, #1
.L19:
	lsls	r3, r0, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	mov	r0, lr
	orrs	r3, r5
	movs	r1, #2
	adds	r6, #1
	strh	r3, [r0, #0]
	add	lr, r1
	cmp	r6, #128
	bne.n	.L13
.L12:
	ldr	r2, [sp, #32]
	cmp	r2, #4
	bne.n	.L20
	movs	r1, #240
	ldr	r3, [pc, #872]
	ldr	r0, [pc, #872]
	lsls	r1, r1, #7
	ldr	r2, [pc, #872]
	bl	Func_080072f0
.L20:
	ldr	r3, [sp, #32]
	cmp	r3, #3
	ble.n	.L21
	b.n	.L22
.L21:
	lsls	r1, r3, #2
	adds	r1, #8
	lsls	r4, r3, #5
	lsls	r2, r1, #5
	lsls	r3, r1, #10
	ldr	r0, [pc, #852]
	orrs	r3, r2
	orrs	r3, r1
	str	r4, [sp, #16]
	strh	r3, [r0, #0]
	ldr	r5, [sp, #12]
	cmp	r5, r4
	bne.n	.L23
	b.n	.L24
.L23:
	ldr	r7, [sp, #12]
	movs	r0, #0
	adds	r1, r7, #0
	mov	r9, r7
	mov	fp, r0
	str	r7, [sp, #8]
	cmp	r1, #0
	bge.n	.L25
	b.n	.L26
.L25:
	mov	r4, r9
	movs	r3, #96
	movs	r5, #60
	mov	r7, fp
	subs	r0, r3, r4
	mov	r2, r9
	mov	r4, fp
	subs	r6, r5, r7
	adds	r2, #96
	mov	lr, r0
	adds	r4, #60
	cmp	r6, #0
	bge.n	.L27
	movs	r6, #0
.L27:
	cmp	r4, #119
	ble.n	.L28
	movs	r4, #119
.L28:
	cmp	r0, #0
	bge.n	.L29
	movs	r0, #0
.L29:
	cmp	r2, #255
	ble.n	.L30
	movs	r2, #255
.L30:
	movs	r5, #7
	ands	r5, r2
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L31
	adds	r3, r2, #7
.L31:
	movs	r2, #7
	asrs	r1, r3, #3
	ands	r2, r4
	adds	r3, r4, #0
	cmp	r4, #0
	bge.n	.L32
	adds	r3, r4, #7
.L32:
	asrs	r3, r3, #3
	lsls	r3, r3, #11
	mov	ip, r3
	lsls	r3, r1, #6
	lsls	r7, r2, #3
	adds	r4, r3, r5
	ldr	r1, [pc, #740]
	adds	r3, r7, r4
	add	r3, ip
	adds	r3, r3, r1
	movs	r2, #2
	movs	r1, #7
	strb	r2, [r3, #0]
	ands	r1, r6
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L33
	adds	r3, r6, #7
.L33:
	asrs	r3, r3, #3
	lsls	r5, r1, #3
	lsls	r6, r3, #11
	ldr	r2, [pc, #712]
	adds	r3, r5, r4
	adds	r3, r6, r3
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	movs	r3, #7
	ands	r3, r0
	adds	r1, r0, #0
	cmp	r0, #0
	bge.n	.L34
	adds	r1, r0, #7
.L34:
	asrs	r1, r1, #3
	lsls	r1, r1, #6
	adds	r1, r1, r3
	adds	r3, r7, r1
	ldr	r4, [pc, #680]
	add	r3, ip
	adds	r1, r5, r1
	adds	r3, r3, r4
	movs	r2, #2
	adds	r1, r6, r1
	mov	r0, lr
	strb	r2, [r3, #0]
	adds	r1, r1, r4
	movs	r3, #2
	mov	r2, r9
	adds	r0, #1
	strb	r3, [r1, #0]
	adds	r2, #97
	cmp	r0, #0
	bge.n	.L35
	movs	r0, #0
.L35:
	cmp	r2, #255
	ble.n	.L36
	movs	r2, #255
.L36:
	movs	r1, #7
	ands	r1, r2
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L37
	adds	r3, r2, #7
.L37:
	asrs	r3, r3, #3
	lsls	r3, r3, #6
	adds	r3, r3, r1
	adds	r2, r7, r3
	ldr	r1, [pc, #620]
	add	r2, ip
	adds	r2, r2, r1
	movs	r1, #2
	strb	r1, [r2, #0]
	adds	r3, r5, r3
	ldr	r2, [pc, #608]
	adds	r3, r6, r3
	adds	r3, r3, r2
	strb	r1, [r3, #0]
	movs	r3, #7
	ands	r3, r0
	adds	r1, r0, #0
	cmp	r0, #0
	bge.n	.L38
	adds	r1, r0, #7
.L38:
	asrs	r1, r1, #3
	lsls	r1, r1, #6
	adds	r1, r1, r3
	adds	r3, r7, r1
	ldr	r4, [pc, #580]
	adds	r1, r5, r1
	add	r3, ip
	adds	r3, r3, r4
	movs	r2, #2
	adds	r1, r6, r1
	strb	r2, [r3, #0]
	adds	r1, r1, r4
	movs	r3, #2
	movs	r5, #96
	mov	r7, fp
	strb	r3, [r1, #0]
	mov	r2, fp
	subs	r0, r5, r7
	mov	r4, r9
	movs	r1, #60
	mov	r3, r9
	adds	r2, #96
	mov	sl, r0
	adds	r4, #60
	subs	r6, r1, r3
	cmp	r0, #0
	bge.n	.L39
	movs	r0, #0
.L39:
	cmp	r2, #255
	ble.n	.L40
	movs	r2, #255
.L40:
	cmp	r6, #0
	bge.n	.L41
	movs	r6, #0
.L41:
	cmp	r4, #119
	ble.n	.L42
	movs	r4, #119
.L42:
	movs	r5, #7
	ands	r5, r2
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L43
	adds	r3, r2, #7
.L43:
	movs	r2, #7
	asrs	r1, r3, #3
	ands	r2, r4
	adds	r3, r4, #0
	cmp	r4, #0
	bge.n	.L44
	adds	r3, r4, #7
.L44:
	asrs	r3, r3, #3
	lsls	r3, r3, #11
	mov	r8, r3
	lsls	r3, r1, #6
	lsls	r2, r2, #3
	adds	r5, r3, r5
	adds	r3, r2, r5
	ldr	r7, [pc, #472]
	add	r3, r8
	adds	r3, r3, r7
	mov	lr, r2
	movs	r1, #7
	movs	r2, #2
	strb	r2, [r3, #0]
	ands	r1, r6
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L45
	adds	r3, r6, #7
.L45:
	asrs	r3, r3, #3
	lsls	r3, r3, #11
	lsls	r7, r1, #3
	mov	ip, r3
	ldr	r1, [pc, #440]
	adds	r3, r7, r5
	add	r3, ip
	adds	r3, r3, r1
	movs	r2, #2
	strb	r2, [r3, #0]
	movs	r3, #7
	ands	r3, r0
	adds	r1, r0, #0
	cmp	r0, #0
	bge.n	.L46
	adds	r1, r0, #7
.L46:
	asrs	r1, r1, #3
	lsls	r1, r1, #6
	adds	r1, r1, r3
	mov	r5, lr
	ldr	r0, [pc, #408]
	adds	r3, r5, r1
	adds	r1, r7, r1
	add	r3, r8
	add	r1, ip
	adds	r3, r3, r0
	movs	r2, #2
	adds	r1, r1, r0
	mov	r0, sl
	strb	r2, [r3, #0]
	adds	r0, #1
	movs	r3, #2
	mov	r2, fp
	strb	r3, [r1, #0]
	adds	r2, #97
	cmp	r0, #0
	bge.n	.L47
	movs	r0, #0
.L47:
	cmp	r2, #255
	ble.n	.L48
	movs	r2, #255
.L48:
	movs	r1, #7
	mov	sl, r1
	mov	r3, sl
	adds	r1, r2, #0
	ands	r1, r3
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L49
	adds	r3, r2, #7
.L49:
	asrs	r3, r3, #3
	lsls	r3, r3, #6
	adds	r1, r3, r1
	mov	r5, lr
	adds	r3, r5, r1
	ldr	r5, [pc, #340]
	add	r3, r8
	adds	r3, r3, r5
	movs	r5, #2
	strb	r5, [r3, #0]
	adds	r3, r7, r1
	ldr	r1, [pc, #328]
	add	r3, ip
	adds	r3, r3, r1
	strb	r5, [r3, #0]
	adds	r2, r0, #0
	mov	r3, sl
	ands	r2, r3
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L50
	adds	r3, r0, #7
.L50:
	asrs	r3, r3, #3
	lsls	r3, r3, #6
	adds	r1, r3, r2
	mov	r4, lr
	adds	r3, r4, r1
	ldr	r2, [pc, #296]
	add	r3, r8
	adds	r3, r3, r2
	strb	r5, [r3, #0]
	ldr	r4, [pc, #288]
	adds	r3, r7, r1
	add	r3, ip
	adds	r3, r3, r4
	movs	r5, #2
	strb	r5, [r3, #0]
	ldr	r0, [sp, #8]
	mov	r7, fp
	lsls	r3, r7, #1
	subs	r3, r0, r3
	subs	r3, #1
	str	r3, [sp, #8]
	cmp	r3, #0
	bge.n	.L51
	mov	r1, r9
	ldr	r2, [sp, #8]
	lsls	r3, r1, #1
	adds	r3, r2, r3
	subs	r3, #2
	str	r3, [sp, #8]
	movs	r3, #1
	negs	r3, r3
	add	r9, r3
.L51:
	movs	r4, #1
	add	fp, r4
	cmp	r9, fp
	blt.n	.L26
	b.n	.L25
.L26:
	ldr	r5, [sp, #12]
	ldr	r7, [sp, #16]
	adds	r5, #1
	str	r5, [sp, #12]
	cmp	r5, r7
	beq.n	.L24
	b.n	.L23
.L24:
	movs	r2, #240
	ldr	r3, [pc, #220]
	ldr	r0, [pc, #196]
	ldr	r1, [pc, #212]
	lsls	r2, r2, #7
	bl	Func_080072f0
.L22:
	ldr	r0, [sp, #32]
	cmp	r0, #50
	bgt.n	.L52
	ldr	r1, [sp, #40]
	movs	r2, #225
	lsls	r2, r2, #7
	movs	r6, #0
	adds	r5, r1, r2
.L54:
	ldr	r2, [pc, #196]
	ldr	r0, [pc, #196]
	lsls	r3, r6, #1
	ldrh	r1, [r2, r3]
	ldrb	r0, [r0, r6]
	ldr	r3, [sp, #40]
	movs	r4, #2
	ldrsh	r2, [r5, r4]
	adds	r1, r3, r1
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	ldr	r0, [pc, #180]
	ldrb	r0, [r0, r6]
	ldr	r4, [sp, #24]
	str	r0, [sp, #4]
	ldr	r0, [sp, #36]
	bl	Func_080072f4
	ldr	r7, [sp, #32]
	cmp	r7, #3
	ble.n	.L53
	movs	r2, #128
	adds	r0, r5, #0
	movs	r1, #64
	lsls	r2, r2, #7
	bl	Func_080e3908
.L53:
	adds	r6, #1
	adds	r5, #28
	cmp	r6, #33
	bne.n	.L54
.L52:
	ldr	r1, [sp, #32]
	subs	r1, #8
	cmp	r1, #42
	bhi.n	.L55
	adds	r0, r1, #0
	cmp	r0, #31
	ble.n	.L56
	movs	r0, #31
.L56:
	lsls	r2, r0, #10
	lsls	r1, r0, #5
	ldr	r3, [pc, #124]
	orrs	r2, r1
	orrs	r2, r0
	strh	r2, [r3, #0]
.L55:
	ldr	r0, [sp, #32]
	cmp	r0, #51
	bne.n	.L57
	ldr	r0, [pc, #112]
	ldr	r1, [sp, #40]
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
	ldr	r0, [pc, #96]
	movs	r6, #1
.L59:
	lsrs	r3, r6, #31
	adds	r3, r6, r3
	asrs	r1, r3, #1
	cmp	r1, #0
	bge.n	.L58
	movs	r1, #0
.L58:
	lsrs	r3, r1, #31
	adds	r3, r1, r3
	asrs	r3, r3, #1
	lsls	r2, r1, #10
	lsls	r3, r3, #5
	orrs	r2, r3
	orrs	r2, r1
	adds	r6, #1
	strh	r2, [r0, #0]
	adds	r0, #2
	cmp	r6, #64
	bne.n	.L59
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #32]
	strh	r3, [r2, #0]
	ldr	r2, [sp, #40]
	movs	r3, #225
	movs	r1, #31
	lsls	r3, r3, #7
	movs	r6, #0
	b.n	.L60
	movs	r0, r0
	.4byte 0x03000168
	.4byte 0x06008000
	.4byte 0x02020202
	.4byte 0x05000004
	.4byte 0x00003f44
	.4byte 0x02010000
	.4byte 0x03001388
	.4byte 0x080edfd2
	.4byte 0x080edf90
	.4byte 0x080edfb1
	.4byte 0x05000002
	.4byte 0x0000007d
	.4byte 0x04000050
.L60:
	mov	r8, r1
	adds	r5, r2, r3
	movs	r7, #0
.L61:
	bl	Func_08004458
	mov	r4, r8
	ands	r0, r4
	adds	r0, #32
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	Func_08004458
	mov	r1, r8
	ands	r0, r1
	adds	r0, #80
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	Func_08004458
	ldr	r3, [pc, #360]
	ldr	r2, [pc, #360]
	ands	r3, r0
	adds	r3, r3, r2
	lsls	r3, r3, #12
	adds	r6, #1
	str	r3, [r5, #12]
	str	r7, [r5, #16]
	str	r7, [r5, #24]
	adds	r5, #28
	cmp	r6, #32
	bne.n	.L61
	ldr	r3, [sp, #40]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r5, [sp, #40]
	ldr	r7, [pc, #328]
	movs	r3, #50
	adds	r2, r5, r7
	str	r3, [r2, #0]
.L57:
	ldr	r0, [sp, #32]
	cmp	r0, #52
	ble.n	.L62
	ldr	r1, [sp, #40]
	movs	r2, #225
	lsls	r2, r2, #7
	movs	r6, #0
	adds	r5, r1, r2
.L67:
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L63
	adds	r3, r6, #3
.L63:
	asrs	r3, r3, #2
	ldr	r4, [sp, #32]
	adds	r3, #52
	cmp	r4, r3
	blt.n	.L64
	ldr	r3, [r5, #24]
	cmp	r3, #39
	bgt.n	.L64
	adds	r1, r3, #0
	cmp	r1, #0
	bge.n	.L65
	adds	r1, #3
.L65:
	asrs	r1, r1, #2
	cmp	r1, #5
	ble.n	.L66
	movs	r1, #5
.L66:
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	ldr	r7, [sp, #40]
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	movs	r0, #32
	str	r0, [sp, #0]
	lsls	r1, r1, #11
	movs	r0, #64
	adds	r1, r7, r1
	subs	r3, #32
	str	r0, [sp, #4]
	subs	r2, #16
	ldr	r0, [sp, #36]
	ldr	r7, [sp, #28]
	bl	Func_08007300
	adds	r0, r5, #0
	movs	r1, #60
	ldr	r2, [pc, #232]
	bl	Func_080e3908
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L64:
	adds	r6, #1
	adds	r5, #28
	cmp	r6, #32
	bne.n	.L67
.L62:
	bl	Func_080cd52c
	ldr	r1, [pc, #212]
	ldr	r0, [sp, #40]
	movs	r3, #1
	adds	r2, r0, r1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r2, [sp, #32]
	adds	r2, #1
	str	r2, [sp, #32]
	cmp	r2, #128
	beq.n	.L68
	b.n	.L69
.L68:
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	ldr	r0, [pc, #176]
	bl	Func_08004278
	ldr	r5, [pc, #172]
	ldr	r4, [sp, #40]
	adds	r3, r4, r5
	ldr	r3, [r3, #0]
	movs	r7, #36
	ldrsh	r0, [r3, r7]
	movs	r3, #1
	negs	r3, r3
	movs	r2, #0
	str	r2, [sp, #0]
	adds	r1, r3, #0
	movs	r2, #1
	bl	Func_080d6888
	ldr	r1, [pc, #148]
	ldr	r0, [sp, #40]
	adds	r3, r0, r1
	ldr	r2, [pc, #148]
	ldr	r3, [r3, #0]
	strh	r3, [r2, #4]
	movs	r3, #32
	strh	r3, [r2, #6]
	ldr	r2, [sp, #20]
	movs	r4, #201
	lsls	r4, r4, #3
	adds	r3, r2, r4
	ldrh	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #2
	bl	Func_080b5038
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [pc, #116]
	ldr	r0, [pc, #120]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L70
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	strh	r2, [r1, #0]
	ldr	r2, [pc, #96]
	adds	r3, #4
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L70:
	strh	r4, [r0, #0]
	movs	r0, #41
	bl	Func_08002dd8
	movs	r0, #40
	bl	Func_08002dd8
	movs	r0, #39
	bl	Func_08002dd8
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x000001ff
	.4byte 0xffffff00
	.4byte 0x00007784
	.4byte 0xfffff000
	.4byte 0x00007824
	.4byte 0x080cd261
	.4byte 0x00007828
	.4byte 0x000077a0
	.4byte 0x03001ad0
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x00007541
