@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080eb754
	.thumb_func
Func_080eb754:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #96]
	ldr	r1, [r5, #0]
	sub	sp, #176
	str	r1, [sp, #80]
	subs	r3, r5, #4
	ldr	r3, [r3, #0]
	str	r3, [sp, #76]
	ldr	r4, [pc, #84]
	ldr	r2, [r5, #4]
	adds	r6, r3, r4
	str	r2, [sp, #68]
	str	r0, [r6, #0]
	movs	r0, #0
	bl	Func_080cd594
	bl	Func_080c9048
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #52]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #52]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r7, [sp, #76]
	movs	r0, #239
	lsls	r0, r0, #7
	movs	r3, #0
	adds	r7, r7, r0
	movs	r1, #144
	str	r3, [r7, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl	Func_080041d8
	movs	r0, #1
	movs	r1, #0
	bl	Func_080cd104
	ldr	r1, [pc, #28]
	movs	r0, #9
	movs	r2, #1
	b.n	.L0
	.4byte 0x00000784
	.4byte 0x00000000
	.4byte 0x03001ef0
	.4byte 0x00007828
	.4byte 0x0400000c
	.4byte 0x080cd261
	.4byte 0x00000175
.L0:
	bl	Func_080dbb24
	ldr	r2, [pc, #76]
	movs	r3, #240
	str	r3, [r2, #16]
	ldr	r0, [r6, #0]
	bl	Func_080d6750
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	subs	r2, #8
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #0
	ldr	r1, [pc, #52]
	movs	r0, #1
	bl	Func_080b5040
	movs	r0, #1
	movs	r1, #1
	bl	Func_080cd104
	ldr	r0, [pc, #40]
	ldr	r1, [sp, #68]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	ldr	r0, [pc, #32]
	ldr	r1, [sp, #76]
	movs	r2, #1
	movs	r3, #1
	b.n	.L1
	movs	r0, r0
	.4byte 0x00002737
	.4byte 0x000000ca
	.4byte 0x03001ce0
	.4byte 0x04000048
	.4byte 0x0000003a
	.4byte 0x00000073
	.4byte 0x00000095
.L1:
	bl	Func_080e0524
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	movs	r1, #0
	subs	r2, #2
	strh	r3, [r2, #0]
	str	r1, [sp, #64]
	str	r1, [sp, #60]
	ldr	r3, [pc, #32]
	ldrh	r3, [r3, #4]
	str	r3, [sp, #56]
	ldr	r5, [r5, #16]
	movs	r2, #1
	str	r5, [sp, #52]
	str	r1, [sp, #48]
	str	r2, [r7, #0]
	b.n	.L2
	movs	r0, r0
	.4byte 0x00007741
	.4byte 0x00000080
	.4byte 0x0000100e
	.4byte 0x00003f44
	.4byte 0x03001ad0
.L2:
	ldr	r4, [sp, #76]
	ldr	r7, [pc, #120]
	ldr	r0, [sp, #64]
	adds	r3, r4, r7
	str	r0, [r3, #0]
	ldr	r1, [sp, #52]
	movs	r3, #225
	str	r2, [r1, #16]
	lsls	r3, r3, #7
	movs	r2, #0
	mov	r8, r2
	movs	r6, #31
	adds	r5, r4, r3
.L3:
	bl	Func_08004458
	ands	r0, r6
	adds	r0, #16
	str	r0, [r5, #0]
	bl	Func_08004458
	ands	r0, r6
	adds	r0, #48
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	Func_08004458
	ands	r0, r6
	subs	r0, #16
	lsls	r0, r0, #16
	str	r0, [r5, #16]
	bl	Func_08004458
	movs	r1, #48
	bl	Func_08002304
	movs	r4, #1
	add	r8, r4
	adds	r0, #2
	mov	r7, r8
	str	r0, [r5, #24]
	adds	r5, #28
	cmp	r7, #64
	bne.n	.L3
	movs	r3, #3
	movs	r2, #7
	movs	r0, #46
	movs	r1, #7
	str	r3, [sp, #0]
	bl	Func_080ed408
	ldr	r3, [pc, #24]
	adds	r3, #184
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #24]
	str	r3, [sp, #72]
	movs	r0, #0
	ldr	r3, [pc, #4]
	mov	fp, r0
	strh	r3, [r2, #0]
	b.n	.L4
	.4byte 0x00000786
	.4byte 0x00007784
	.4byte 0x03001e50
	.4byte 0x0400000c
.L41:
	mov	r3, fp
	subs	r3, #24
	cmp	r3, #31
	bhi.n	.L5
	ldr	r1, [sp, #48]
	adds	r1, #1
	str	r1, [sp, #48]
.L5:
	ldr	r2, [sp, #48]
	cmp	r2, #24
	ble.n	.L6
	movs	r3, #24
	str	r3, [sp, #48]
.L6:
	mov	r4, fp
	cmp	r4, #135
	bgt.n	.L7
	ldr	r7, [pc, #472]
	ldr	r0, [sp, #48]
	ldrh	r3, [r7, #4]
	adds	r1, r7, #0
	subs	r3, r3, r0
	strh	r3, [r1, #4]
	ldr	r2, [sp, #60]
	adds	r2, r2, r0
	str	r2, [sp, #60]
.L7:
	mov	r3, fp
	cmp	r3, #149
	bgt.n	.L8
	ldr	r3, [pc, #452]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	mov	r7, fp
	str	r3, [sp, #92]
	str	r4, [sp, #96]
	movs	r4, #0
	mov	sl, r4
	cmp	r7, #103
	ble.n	.L9
	ldr	r0, [pc, #436]
	lsls	r3, r7, #4
	adds	r0, r0, r3
	mov	sl, r0
.L9:
	mov	r3, fp
	subs	r3, #8
	cmp	r3, #23
	bhi.n	.L10
	ldr	r1, [sp, #64]
	ldr	r2, [sp, #48]
	adds	r3, r1, r2
	subs	r3, #8
	str	r3, [sp, #64]
.L10:
	mov	r3, fp
	cmp	r3, #7
	ble.n	.L11
	movs	r5, #96
	cmp	r3, #104
	bgt.n	.L12
	movs	r5, #32
.L12:
	mov	r4, fp
	ldr	r7, [pc, #396]
	lsls	r3, r4, #10
	adds	r0, r3, r7
	ldr	r3, [pc, #396]
	movs	r1, #128
	ands	r0, r3
	lsls	r1, r1, #8
	cmp	r0, r1
	ble.n	.L13
	ldr	r2, [pc, #388]
	adds	r0, r0, r2
.L13:
	bl	Func_08002322
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #16
	str	r3, [sp, #44]
	mov	r4, fp
	movs	r3, #31
	ands	r3, r4
	cmp	r3, #8
	bne.n	.L11
	ldr	r7, [sp, #76]
	ldr	r0, [pc, #364]
	movs	r3, #4
	adds	r2, r7, r0
	str	r3, [r2, #0]
.L11:
	add	r3, sp, #160
	movs	r2, #0
	str	r2, [r3, #12]
	movs	r2, #255
	lsls	r2, r2, #16
	str	r2, [r3, #4]
	adds	r6, r3, #0
	ldr	r2, [sp, #76]
	ldr	r3, [pc, #344]
	movs	r1, #0
	mov	r8, r1
	add	r7, sp, #92
	adds	r5, r2, r3
.L14:
	ldr	r3, [pc, #336]
	mov	r4, r8
	ldr	r0, [sp, #64]
	ldrb	r3, [r3, r4]
	mov	r1, sl
	adds	r3, r0, r3
	subs	r3, r3, r1
	movs	r2, #224
	lsls	r2, r2, #16
	lsls	r3, r3, #16
	adds	r3, r3, r2
	str	r3, [r6, #0]
	ldr	r3, [pc, #316]
	ldrb	r3, [r3, r4]
	ldr	r4, [sp, #44]
	movs	r0, #144
	subs	r3, r3, r4
	lsls	r0, r0, #15
	lsls	r3, r3, #16
	adds	r3, r3, r0
	str	r3, [r6, #8]
	adds	r1, r6, #0
	adds	r2, r7, #0
	ldmia	r5!, {r0}
	movs	r3, #0
	bl	Func_08009008
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #9
	bne.n	.L14
.L8:
	mov	r3, fp
	cmp	r3, #26
	bgt.n	.L15
	ldr	r7, [sp, #60]
	lsls	r3, r3, #3
	adds	r7, #4
	mov	sl, r3
	cmp	r7, #10
	ble.n	.L16
	movs	r7, #10
.L16:
	mov	r4, sl
	cmp	r4, #64
	ble.n	.L17
	movs	r0, #64
	mov	sl, r0
.L17:
	movs	r1, #0
	mov	r2, sl
	mov	r8, r1
	cmp	r2, #0
	beq.n	.L15
	ldr	r3, [sp, #60]
	ldr	r4, [sp, #60]
	lsls	r3, r3, #1
	str	r3, [sp, #40]
	adds	r3, r3, r4
	lsls	r3, r3, #2
	adds	r3, #48
	str	r3, [sp, #36]
	lsrs	r3, r7, #31
	adds	r3, r7, r3
	asrs	r3, r3, #1
	lsls	r0, r7, #1
	str	r3, [sp, #32]
	mov	r9, r0
.L18:
	mov	r1, r8
	lsls	r6, r1, #10
	adds	r0, r6, #0
	bl	Func_08002322
	ldr	r3, [sp, #40]
	adds	r3, #8
	adds	r5, r3, #0
	muls	r5, r0
	ldr	r2, [sp, #60]
	asrs	r5, r5, #16
	adds	r0, r6, #0
	adds	r5, r5, r2
	bl	Func_0800231c
	ldr	r4, [sp, #36]
	adds	r3, r4, #0
	muls	r3, r0
	mov	r2, r9
	ldr	r0, [pc, #176]
	ldr	r4, [sp, #32]
	subs	r2, #2
	ldrh	r1, [r0, r2]
	adds	r5, #96
	ldr	r2, [sp, #68]
	asrs	r3, r3, #16
	subs	r5, r5, r4
	mov	r0, r9
	adds	r3, #64
	adds	r1, r2, r1
	str	r0, [sp, #4]
	subs	r3, r3, r7
	str	r7, [sp, #0]
	ldr	r0, [sp, #80]
	adds	r2, r5, #0
	ldr	r4, [sp, #72]
	bl	Func_080072f4
	movs	r0, #1
	add	r8, r0
	cmp	r8, sl
	bne.n	.L18
.L15:
	mov	r1, fp
	cmp	r1, #24
	bne.n	.L19
	ldr	r3, [sp, #76]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r7, [sp, #76]
	ldr	r0, [pc, #112]
	movs	r3, #50
	adds	r2, r7, r0
	str	r3, [r2, #0]
.L19:
	mov	r1, fp
	cmp	r1, #28
	bne.n	.L20
	ldr	r2, [pc, #104]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #0]
.L20:
	mov	r2, fp
	cmp	r2, #17
	ble.n	.L21
	ldr	r4, [sp, #76]
	movs	r7, #225
	movs	r3, #0
	lsls	r7, r7, #7
	mov	r8, r3
	movs	r6, #31
	adds	r5, r4, r7
.L28:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L22
	movs	r1, #3
	mov	r0, r8
	bl	Func_080022fc
	adds	r0, #1
	lsls	r4, r0, #1
	ldr	r2, [pc, #52]
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	b.n	.L23
	movs	r0, r0
	.4byte 0x00000784
	.4byte 0x03001ad0
	.4byte 0x080edad8
	.4byte 0xfffff980
	.4byte 0xffffe000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x000077a8
	.4byte 0x000077d8
	.4byte 0x080eef56
	.4byte 0x080eef5f
	.4byte 0x080ede48
	.4byte 0x00007784
	.4byte 0x0400000c
.L23:
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	ldr	r2, [sp, #68]
	subs	r3, r3, r0
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #80]
	ldr	r4, [sp, #72]
	bl	Func_080072f4
	ldr	r3, [r5, #0]
	adds	r3, #2
	ldr	r2, [r5, #16]
	str	r3, [r5, #0]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #4
	cmp	r3, #0
	bge.n	.L24
	adds	r3, #63
.L24:
	asrs	r3, r3, #6
	str	r3, [r5, #16]
	b.n	.L25
.L22:
	subs	r3, #1
	str	r3, [r5, #24]
.L25:
	ldr	r3, [r5, #0]
	cmp	r3, #128
	bgt.n	.L26
	ldr	r3, [r5, #24]
	cmp	r3, #1
	bne.n	.L27
.L26:
	bl	Func_08004458
	ldr	r7, [sp, #64]
	ands	r0, r6
	adds	r0, r0, r7
	adds	r0, #172
	str	r0, [r5, #0]
	bl	Func_08004458
	ldr	r1, [sp, #44]
	ands	r0, r6
	subs	r0, r0, r1
	adds	r0, #56
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	Func_08004458
	ands	r0, r6
	subs	r0, #16
	lsls	r0, r0, #15
	str	r0, [r5, #16]
.L27:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r5, #28
	cmp	r3, #48
	bne.n	.L28
.L21:
	mov	r4, fp
	cmp	r4, #31
	ble.n	.L29
	mov	r3, fp
	subs	r3, #32
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r5, r3, #1
	cmp	r5, #40
	ble.n	.L30
	movs	r5, #40
.L30:
	movs	r7, #0
	mov	r8, r7
	movs	r6, #0
	movs	r7, #120
.L31:
	bl	Func_08004458
	movs	r3, #3
	ands	r0, r3
	lsls	r1, r0, #1
	adds	r1, r1, r0
	movs	r3, #48
	ldr	r0, [sp, #76]
	str	r3, [sp, #0]
	lsls	r1, r1, #9
	movs	r3, #32
	adds	r1, r0, r1
	str	r3, [sp, #4]
	ldr	r0, [sp, #80]
	adds	r3, r6, #0
	subs	r2, r7, r5
	ldr	r4, [sp, #72]
	bl	Func_080072f4
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r6, #18
	cmp	r1, #6
	bne.n	.L31
.L29:
	ldr	r3, [sp, #76]
	ldr	r4, [pc, #32]
	adds	r2, r3, r4
	ldr	r3, [r2, #0]
	cmp	r3, #0
	ble.n	.L32
	subs	r3, #1
	str	r3, [r2, #0]
	bl	Func_08004458
	ldr	r3, [pc, #12]
	ldr	r7, [pc, #16]
	ands	r0, r3
	adds	r0, #28
	strh	r0, [r7, #6]
	b.n	.L33
	movs	r0, r0
	.4byte 0x00000007
	.4byte 0x000077a8
	.4byte 0x03001ad0
.L32:
	ldr	r0, [pc, #192]
	movs	r3, #32
	strh	r3, [r0, #6]
.L33:
	ldr	r1, [sp, #76]
	ldr	r3, [pc, #188]
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r4, #1
	add	fp, r4
.L4:
	mov	r7, fp
	cmp	r7, #120
	beq.n	.L34
	movs	r0, #0
	str	r0, [sp, #44]
	cmp	r7, #0
	bne.n	.L35
	movs	r0, #136
	bl	Func_080f9010
.L35:
	mov	r1, fp
	cmp	r1, #26
	bne.n	.L36
	movs	r0, #141
	bl	Func_080f9010
.L36:
	mov	r2, fp
	cmp	r2, #40
	bne.n	.L37
	movs	r0, #154
	bl	Func_080f9010
.L37:
	mov	r3, fp
	cmp	r3, #72
	bne.n	.L38
	movs	r0, #154
	bl	Func_080f9010
.L38:
	mov	r4, fp
	cmp	r4, #104
	bne.n	.L39
	movs	r0, #154
	bl	Func_080f9010
.L39:
	ldr	r3, [pc, #108]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L40
	b.n	.L41
.L40:
	mov	r7, fp
	cmp	r7, #16
	bgt.n	.L34
	b.n	.L41
.L34:
	add	r0, sp, #56
	ldr	r3, [pc, #76]
	ldrh	r0, [r0, #0]
	strh	r0, [r3, #4]
	ldr	r1, [sp, #52]
	movs	r2, #0
	str	r2, [r1, #16]
	bl	Func_080d67dc
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #52]
	strh	r3, [r2, #0]
	ldr	r3, [sp, #76]
	ldr	r4, [pc, #68]
	movs	r2, #0
	mov	r8, r2
	movs	r0, #12
	adds	r1, r3, r4
.L42:
	ldmia	r1!, {r2}
	ldrb	r3, [r2, #9]
	movs	r7, #1
	orrs	r3, r0
	add	r8, r7
	strb	r3, [r2, #9]
	mov	r2, r8
	cmp	r2, #9
	bne.n	.L42
	mov	r4, sp
	adds	r4, #116
	movs	r3, #224
	mov	r2, sp
	str	r4, [sp, #24]
	str	r3, [sp, #28]
	movs	r1, #0
	adds	r3, r4, #0
	adds	r2, #130
	b.n	.L43
	movs	r0, r0
	.4byte 0x000000f0
	.4byte 0x03001ad0
	.4byte 0x00007824
	.4byte 0x03001b04
	.4byte 0x04000040
	.4byte 0x000077d8
.L43:
	strb	r1, [r3, #0]
	adds	r3, #1
	cmp	r3, r2
	bne.n	.L43
	mov	r7, sp
	adds	r7, #132
	str	r7, [sp, #20]
	ldr	r5, [sp, #20]
	movs	r7, #31
	add	r6, sp, #148
.L44:
	bl	Func_08004458
	ands	r0, r7
	strb	r0, [r5, #0]
	adds	r5, #1
	cmp	r5, r6
	bne.n	.L44
	movs	r0, #0
	movs	r2, #160
	ldr	r3, [pc, #96]
	mov	r8, r0
	movs	r1, #0
	lsls	r2, r2, #1
.L45:
	movs	r4, #1
	add	r8, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L45
	ldr	r7, [sp, #76]
	movs	r0, #239
	lsls	r0, r0, #7
	ldr	r1, [pc, #72]
	adds	r2, r7, r0
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r7, r1
	movs	r3, #75
	str	r3, [r2, #0]
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #44]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #70
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	movs	r2, #0
	str	r3, [sp, #16]
	mov	fp, r2
.L69:
	mov	r4, fp
	cmp	r4, #23
	bgt.n	.L46
	ldr	r3, [pc, #44]
	ldr	r7, [sp, #28]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	subs	r7, #16
	mov	r0, fp
	str	r3, [sp, #84]
	str	r4, [sp, #88]
	str	r7, [sp, #28]
	cmp	r0, #8
	bgt.n	.L47
	b.n	.L48
	.4byte 0x00000784
	.4byte 0x00001010
	.4byte 0x02010018
	.4byte 0x00007784
	.4byte 0x0400000c
	.4byte 0xfffffe20
	.4byte 0x080edae0
.L48:
	movs	r1, #128
	lsls	r3, r0, #11
	lsls	r1, r1, #7
	movs	r2, #128
	adds	r0, r3, r1
	lsls	r2, r2, #8
	cmp	r0, r2
	ble.n	.L49
	ldr	r4, [pc, #752]
	adds	r0, r3, r4
.L49:
	bl	Func_08002322
	lsls	r0, r0, #6
	b.n	.L50
.L47:
	mov	r7, fp
	movs	r1, #128
	lsls	r3, r7, #11
	lsls	r1, r1, #7
	movs	r2, #128
	adds	r0, r3, r1
	lsls	r2, r2, #8
	cmp	r0, r2
	ble.n	.L51
	ldr	r4, [pc, #724]
	adds	r0, r3, r4
.L51:
	bl	Func_08002322
	lsls	r0, r0, #5
.L50:
	asrs	r4, r0, #16
	add	r3, sp, #100
	movs	r2, #0
	str	r2, [r3, #12]
	movs	r2, #255
	lsls	r2, r2, #16
	str	r2, [r3, #4]
	ldr	r0, [sp, #76]
	ldr	r1, [pc, #700]
	movs	r7, #0
	mov	r8, r7
	adds	r6, r3, #0
	add	r7, sp, #84
	adds	r5, r0, r1
.L52:
	ldr	r3, [pc, #692]
	mov	r2, r8
	ldr	r0, [sp, #28]
	ldrb	r3, [r3, r2]
	adds	r3, r0, r3
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	ldr	r3, [pc, #684]
	ldrb	r3, [r3, r2]
	movs	r1, #144
	subs	r3, r3, r4
	lsls	r1, r1, #15
	lsls	r3, r3, #16
	adds	r3, r3, r1
	str	r3, [r6, #8]
	adds	r2, r7, #0
	movs	r3, #0
	ldmia	r5!, {r0}
	adds	r1, r6, #0
	str	r4, [sp, #8]
	bl	Func_08009008
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	ldr	r4, [sp, #8]
	cmp	r3, #9
	bne.n	.L52
.L46:
	mov	r4, fp
	cmp	r4, #8
	bne.n	.L53
	ldr	r7, [sp, #76]
	ldr	r0, [pc, #636]
	adds	r3, r7, r0
	str	r4, [r3, #0]
	movs	r0, #145
	bl	Func_080f9010
.L53:
	mov	r1, fp
	cmp	r1, #11
	bne.n	.L54
	movs	r0, #145
	bl	Func_080f9010
.L54:
	mov	r2, fp
	cmp	r2, #46
	bne.n	.L55
	movs	r0, #137
	bl	Func_080f9010
.L55:
	ldr	r2, [pc, #604]
	ldr	r4, [sp, #76]
	movs	r3, #0
	mov	r8, r3
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L56
	movs	r0, #0
	movs	r7, #36
	str	r0, [sp, #12]
	mov	r9, r7
.L59:
	ldr	r1, [sp, #24]
	mov	r4, r8
	ldrb	r3, [r1, r4]
	cmp	r3, #0
	bne.n	.L57
	ldr	r7, [sp, #76]
	ldr	r3, [r7, r2]
	add	r5, sp, #148
	mov	r1, r9
	ldrsh	r0, [r3, r1]
	adds	r1, r5, #0
	bl	Func_080e3980
	ldr	r3, [r5, #0]
	ldr	r4, [sp, #28]
	cmp	r3, r4
	ble.n	.L57
	ldr	r7, [sp, #24]
	movs	r3, #1
	mov	r0, r8
	strb	r3, [r7, r0]
	ldr	r1, [sp, #12]
	ldr	r2, [pc, #540]
	mov	sl, r5
	movs	r6, #0
	movs	r7, #255
	adds	r5, r1, r2
.L58:
	mov	r4, sl
	ldr	r3, [r4, #0]
	lsls	r3, r3, #15
	str	r3, [r5, #0]
	ldr	r3, [r4, #4]
	subs	r3, #16
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	Func_08004458
	ands	r0, r7
	subs	r0, #128
	lsls	r0, r0, #10
	str	r0, [r5, #12]
	bl	Func_08004458
	ands	r0, r7
	subs	r0, #192
	lsls	r3, r0, #11
	ldr	r2, [r5, #12]
	str	r3, [r5, #16]
	ldr	r3, [r5, #0]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r3, [r5, #4]
	lsls	r0, r0, #13
	adds	r3, r3, r0
	str	r3, [r5, #4]
	bl	Func_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r3, #8
	adds	r6, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r6, #32
	bne.n	.L58
	ldr	r7, [sp, #76]
	ldr	r0, [pc, #448]
	adds	r3, r7, r0
	ldr	r3, [r3, #0]
	mov	r1, r9
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	Func_080b5088
	movs	r0, #134
	bl	Func_080f9010
.L57:
	ldr	r4, [sp, #12]
	movs	r7, #224
	lsls	r7, r7, #2
	adds	r4, r4, r7
	str	r4, [sp, #12]
	ldr	r2, [pc, #416]
	ldr	r1, [sp, #76]
	movs	r3, #2
	add	r9, r3
	ldr	r3, [r1, r2]
	movs	r0, #1
	ldr	r3, [r3, #20]
	add	r8, r0
	cmp	r8, r3
	bne.n	.L59
.L56:
	movs	r2, #0
	ldr	r5, [pc, #396]
	mov	r8, r2
	movs	r7, #3
	movs	r6, #6
.L61:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	ble.n	.L60
	ldr	r3, [pc, #388]
	ldrh	r1, [r3, #4]
	ldr	r3, [sp, #68]
	movs	r4, #2
	ldrsh	r2, [r5, r4]
	adds	r1, r3, r1
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	subs	r2, #1
	subs	r3, #3
	str	r7, [sp, #0]
	str	r6, [sp, #4]
	ldr	r0, [sp, #80]
	ldr	r4, [sp, #72]
	bl	Func_080072f4
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L60:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #192
	bne.n	.L61
	mov	r2, fp
	cmp	r2, #48
	bne.n	.L62
	movs	r0, #136
	bl	Func_080f9010
.L62:
	mov	r3, fp
	cmp	r3, #40
	ble.n	.L63
	ldr	r4, [sp, #76]
	movs	r7, #239
	lsls	r7, r7, #7
	ldr	r0, [pc, #300]
	adds	r2, r4, r7
	movs	r3, #0
	ldr	r6, [sp, #16]
	str	r3, [r2, #0]
	adds	r2, r4, r0
	movs	r3, #75
	movs	r1, #0
	movs	r5, #8
	str	r3, [r2, #0]
	mov	r8, r1
	negs	r5, r5
.L64:
	bl	Func_08004458
	movs	r3, #3
	ands	r0, r3
	lsls	r1, r0, #1
	ldr	r2, [sp, #76]
	ldr	r3, [sp, #20]
	adds	r1, r1, r0
	mov	r4, r8
	lsls	r1, r1, #9
	adds	r1, r2, r1
	ldrb	r2, [r3, r4]
	movs	r3, #48
	str	r3, [sp, #0]
	subs	r2, r2, r6
	movs	r3, #32
	str	r3, [sp, #4]
	adds	r2, #120
	adds	r3, r5, #0
	ldr	r0, [sp, #80]
	ldr	r7, [sp, #72]
	bl	Func_08007300
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #8
	cmp	r1, #16
	bne.n	.L64
.L63:
	mov	r2, fp
	cmp	r2, #64
	ble.n	.L65
	ldr	r3, [sp, #76]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
.L65:
	mov	r7, fp
	cmp	r7, #58
	bne.n	.L66
	ldr	r3, [pc, #184]
	ldr	r1, [sp, #76]
	ldr	r3, [r1, r3]
	ldr	r3, [r3, #20]
	movs	r0, #0
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L66
	ldr	r2, [pc, #168]
	movs	r6, #36
	adds	r5, r1, r2
.L67:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #14
	subs	r3, #1
	movs	r2, #5
	bl	Func_080d6888
	ldr	r3, [r5, #0]
	movs	r7, #1
	ldr	r3, [r3, #20]
	add	r8, r7
	adds	r6, #2
	cmp	r8, r3
	bne.n	.L67
.L66:
	movs	r0, #8
	movs	r1, #8
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r1, [pc, #132]
	ldr	r0, [sp, #76]
	movs	r3, #1
	adds	r2, r0, r1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #1
	ldr	r2, [sp, #16]
	add	fp, r3
	adds	r2, #12
	mov	r4, fp
	str	r2, [sp, #16]
	cmp	r4, #96
	beq.n	.L68
	b.n	.L69
.L68:
	movs	r0, #134
	bl	Func_080b50e8
	ldr	r1, [pc, #60]
	ldr	r0, [sp, #76]
	movs	r7, #0
	mov	r8, r7
	adds	r6, r0, r1
.L70:
	ldmia	r6!, {r0}
	bl	Func_08009038
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #9
	bne.n	.L70
	ldr	r0, [pc, #72]
	bl	Func_08004278
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #176
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0xffffc000
	.4byte 0x000077d8
	.4byte 0x080eef56
	.4byte 0x080eef5f
	.4byte 0x000077a8
	.4byte 0x00007828
	.4byte 0x02010000
	.4byte 0x080ede48
	.4byte 0x00007784
	.4byte 0x00007824
	.4byte 0x080cd261
	ldr	r2, [pc, #8]
	ldr	r3, [pc, #4]
	strh	r3, [r2, #0]
	bx	lr
	.4byte 0x00000100
	.4byte 0x04000020
