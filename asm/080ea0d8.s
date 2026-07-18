@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080ea0d8
	.thumb_func
Func_080ea0d8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #112]
	ldr	r1, [r5, #0]
	sub	sp, #184
	str	r1, [sp, #104]
	subs	r3, r5, #4
	ldr	r3, [r3, #0]
	str	r3, [sp, #100]
	ldr	r2, [r5, #16]
	str	r2, [sp, #96]
	ldr	r3, [r5, #4]
	str	r3, [sp, #80]
	adds	r3, r5, #0
	ldr	r4, [sp, #100]
	ldr	r1, [pc, #88]
	subs	r3, #112
	ldr	r3, [r3, #0]
	adds	r6, r4, r1
	str	r3, [sp, #76]
	str	r0, [r6, #0]
	movs	r0, #128
	lsls	r0, r0, #6
	bl	Func_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #56]
	mov	r8, r2
	mov	r4, r8
	strh	r3, [r4, #0]
	bl	Func_080c9048
	ldr	r2, [pc, #48]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r3, [sp, #0]
	bl	Func_080ed408
	ldr	r5, [r5, #24]
	movs	r0, #239
	str	r5, [sp, #88]
	ldr	r5, [sp, #100]
	lsls	r0, r0, #7
	adds	r3, r5, r0
	movs	r1, #144
	movs	r5, #0
	str	r5, [r3, #0]
	b.n	.L0
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x03001ef0
	.4byte 0x00007828
	.4byte 0x04000020
.L0:
	lsls	r1, r1, #3
	ldr	r0, [pc, #140]
	bl	Func_080041d8
	movs	r1, #0
	movs	r0, #0
	bl	Func_080cd104
	ldr	r0, [r6, #0]
	bl	Func_080d6750
	movs	r1, #191
	lsls	r1, r1, #1
	movs	r0, #16
	movs	r2, #1
	bl	Func_080dbb24
	ldr	r2, [pc, #112]
	movs	r3, #240
	str	r3, [r2, #16]
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #0
	ldr	r1, [pc, #100]
	movs	r0, #1
	bl	Func_080b5040
	ldr	r1, [sp, #96]
	movs	r3, #1
	str	r3, [r1, #16]
	ldr	r3, [pc, #92]
	movs	r0, #0
	strh	r5, [r3, #4]
	movs	r1, #1
	bl	Func_080cd104
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	mov	r2, r8
	strh	r3, [r2, #0]
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #44]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #60]
	ldr	r5, [pc, #64]
	movs	r4, #128
	str	r3, [sp, #72]
	ldr	r1, [sp, #100]
	lsls	r4, r4, #12
	movs	r6, #0
	ldr	r0, [pc, #56]
	movs	r2, #1
	movs	r3, #1
	str	r4, [sp, #68]
	str	r5, [sp, #64]
	str	r6, [sp, #60]
	b.n	.L1
	.4byte 0x00007741
	.4byte 0x00000080
	.4byte 0x00001010
	.4byte 0x00003f44
	.4byte 0x080cd261
	.4byte 0x03001ce0
	.4byte 0x0000003b
	.4byte 0x03001ad0
	.4byte 0x04000052
	.4byte 0xffc00000
	.4byte 0x0000ffff
	.4byte 0x000000bb
.L1:
	bl	Func_080e0524
	movs	r3, #192
	ldr	r2, [sp, #100]
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #328]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	ldr	r5, [pc, #324]
	ldr	r4, [sp, #100]
	ldr	r0, [pc, #324]
	adds	r1, r4, r5
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
	ldr	r6, [pc, #316]
	ldr	r0, [pc, #316]
	ldr	r1, [sp, #80]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	movs	r7, #0
	movs	r5, #0
	movs	r4, #64
.L5:
	ldr	r2, [sp, #100]
	adds	r3, r5, r2
	ldr	r2, [pc, #300]
	ldr	r1, [sp, #80]
	movs	r0, #0
	mov	ip, r4
	adds	r3, r3, r2
.L4:
	ldrb	r2, [r1, #0]
	adds	r1, #1
	cmp	r2, ip
	ble.n	.L2
	mov	r2, ip
.L2:
	cmp	r2, #0
	bge.n	.L3
	movs	r2, #0
.L3:
	adds	r0, #1
	strb	r2, [r3, #0]
	adds	r3, #1
	cmp	r0, r6
	bne.n	.L4
	adds	r7, #1
	adds	r5, r5, r6
	subs	r4, #7
	cmp	r7, #8
	bne.n	.L5
	ldr	r0, [pc, #256]
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #252]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	Func_080072f0
	ldr	r6, [pc, #244]
	ldr	r5, [pc, #248]
	movs	r7, #0
.L6:
	bl	Func_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r3, #72
	str	r3, [r5, #0]
	bl	Func_08004458
	ands	r0, r6
	str	r0, [r5, #12]
	bl	Func_08004458
	ands	r0, r6
	str	r0, [r5, #16]
	bl	Func_08004458
	adds	r7, #1
	ands	r0, r6
	str	r0, [r5, #16]
	adds	r5, #28
	cmp	r7, #128
	bne.n	.L6
	movs	r3, #255
	movs	r4, #0
	ldr	r6, [pc, #200]
	movs	r7, #0
	mov	sl, r3
	mov	r8, r4
.L7:
	bl	Func_08004458
	ldr	r3, [pc, #180]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	Func_08002322
	mov	r3, sl
	muls	r3, r0
	asrs	r3, r3, #2
	str	r3, [r6, #0]
	adds	r0, r5, #0
	bl	Func_0800231c
	mov	r3, sl
	muls	r3, r0
	asrs	r3, r3, #2
	str	r3, [r6, #8]
	bl	Func_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	mov	r0, r8
	lsls	r3, r3, #16
	adds	r7, #1
	str	r3, [r6, #4]
	str	r0, [r6, #12]
	str	r0, [r6, #16]
	str	r5, [r6, #24]
	adds	r6, #28
	cmp	r7, #128
	bne.n	.L7
	ldr	r1, [sp, #100]
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r4, [pc, #120]
	adds	r2, r1, r3
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r1, r4
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r2, [pc, #112]
	ldr	r3, [pc, #56]
	mov	r6, sp
	strh	r3, [r2, #0]
	mov	r0, sp
	mov	r1, sp
	ldr	r2, [pc, #104]
	ldr	r3, [pc, #104]
	movs	r5, #0
	adds	r6, #172
	adds	r0, #144
	adds	r1, #116
	str	r5, [sp, #84]
	str	r6, [sp, #16]
	str	r0, [sp, #20]
	str	r1, [sp, #24]
	str	r2, [sp, #56]
	str	r3, [sp, #12]
.L46:
	ldr	r4, [sp, #84]
	cmp	r4, #143
	bne.n	.L8
	movs	r1, #128
	ldr	r3, [pc, #80]
	ldr	r0, [sp, #104]
	lsls	r1, r1, #7
	ldr	r2, [pc, #76]
	bl	Func_080072f0
	movs	r0, #145
	b.n	.L9
	movs	r0, r0
	.4byte 0x00000784
	.4byte 0x00000067
	.4byte 0x0000095c
	.4byte 0x000000ce
	.4byte 0x00000302
	.4byte 0x00000073
	.4byte 0x00002710
	.4byte 0x00000064
	.4byte 0x03001388
	.4byte 0x0000ffff
	.4byte 0x02010000
	.4byte 0x02010e00
	.4byte 0x00007784
	.4byte 0x0400000c
	.4byte 0x03001e50
	.4byte 0xfffffa70
	.4byte 0x03000168
	.4byte 0x2a2a2a2a
.L9:
	bl	Func_080f9010
.L8:
	ldr	r5, [sp, #84]
	cmp	r5, #80
	bne.n	.L10
	movs	r0, #142
	bl	Func_080f9010
.L10:
	ldr	r6, [sp, #84]
	cmp	r6, #0
	bge.n	.L11
	bl	Func_080049ac
	ldr	r0, [sp, #76]
	adds	r1, r0, #0
	adds	r1, #12
	bl	Func_080051d8
	add	r0, sp, #160
	mov	r8, r0
	ldr	r6, [pc, #892]
	movs	r7, #0
	mov	sl, r8
.L17:
	adds	r0, r6, #0
	movs	r1, #60
	ldr	r2, [pc, #888]
	bl	Func_080e38b8
	mov	r1, sl
	adds	r0, r6, #0
	bl	Func_080e3944
	mov	r1, sl
	ldr	r3, [r1, #0]
	asrs	r3, r3, #1
	str	r3, [r1, #0]
	ldr	r3, [r1, #4]
	ldr	r0, [r1, #8]
	subs	r3, #120
	str	r3, [r1, #4]
	cmp	r0, #99
	bgt.n	.L12
	movs	r3, #100
	mov	r2, r8
	str	r3, [r2, #8]
	movs	r0, #100
.L12:
	movs	r3, #225
	lsls	r3, r3, #2
	cmp	r0, r3
	ble.n	.L13
	mov	r4, r8
	str	r3, [r4, #8]
	adds	r0, r3, #0
.L13:
	adds	r5, r0, #0
	subs	r5, #100
	mov	r0, r8
	str	r5, [r0, #8]
	movs	r1, #100
	adds	r0, r5, #0
	bl	Func_080022ec
	ldr	r1, [sp, #64]
	adds	r4, r0, #1
	cmp	r1, r5
	ble.n	.L14
	str	r5, [sp, #64]
.L14:
	ldr	r2, [sp, #60]
	cmp	r2, r5
	bge.n	.L15
	str	r5, [sp, #60]
.L15:
	lsls	r0, r4, #1
	ldr	r2, [pc, #800]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #80]
	mov	r5, r8
	adds	r1, r3, r1
	lsrs	r3, r4, #31
	ldr	r2, [r5, #0]
	adds	r3, r4, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r4, [sp, #0]
	subs	r3, r3, r4
	str	r0, [sp, #4]
	ldr	r4, [sp, #88]
	ldr	r0, [sp, #104]
	bl	Func_080072f4
	ldr	r5, [sp, #84]
	cmp	r5, #64
	bne.n	.L16
	ldr	r0, [r6, #24]
	bl	Func_08002322
	lsls	r2, r0, #8
	ldr	r3, [r6, #12]
	subs	r2, r2, r0
	asrs	r2, r2, #3
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r0, [r6, #24]
	bl	Func_0800231c
	lsls	r2, r0, #8
	ldr	r3, [r6, #16]
	subs	r2, r2, r0
	asrs	r2, r2, #3
	adds	r3, r3, r2
	str	r3, [r6, #16]
.L16:
	adds	r7, #1
	adds	r6, #28
	cmp	r7, #128
	bne.n	.L17
.L11:
	ldr	r6, [sp, #84]
	cmp	r6, #72
	bne.n	.L18
	ldr	r0, [sp, #100]
	ldr	r1, [pc, #708]
	movs	r2, #24
	adds	r3, r0, r1
	str	r2, [r3, #0]
	ldr	r3, [pc, #704]
	adds	r2, r0, r3
	movs	r3, #0
	str	r3, [r2, #0]
.L18:
	ldr	r4, [sp, #16]
	movs	r3, #0
	str	r3, [r4, #8]
	str	r3, [r4, #4]
	ldr	r6, [pc, #692]
	movs	r7, #0
.L24:
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L19
	adds	r3, r7, #3
.L19:
	asrs	r3, r3, #2
	ldr	r5, [sp, #84]
	adds	r3, #80
	cmp	r5, r3
	ble.n	.L20
	ldr	r3, [r6, #0]
	cmp	r3, #0
	ble.n	.L20
	bl	Func_080049ac
	ldr	r0, [r6, #16]
	bl	Func_08004c6c
	ldr	r0, [r6, #12]
	bl	Func_08004bd4
	ldr	r0, [r6, #16]
	lsls	r3, r5, #9
	adds	r0, r0, r3
	bl	Func_08004c1c
	ldr	r3, [r6, #0]
	ldr	r0, [sp, #16]
	str	r3, [r0, #0]
	ldr	r3, [r6, #0]
	subs	r3, #2
	str	r3, [r6, #0]
	cmp	r3, #0
	bge.n	.L21
	movs	r3, #0
	str	r3, [r6, #0]
.L21:
	add	r5, sp, #160
	ldr	r0, [sp, #16]
	adds	r1, r5, #0
	bl	Func_080e3944
	movs	r3, #60
	ldr	r0, [r5, #8]
	negs	r3, r3
	cmp	r0, r3
	bge.n	.L22
	str	r3, [r5, #8]
	adds	r0, r3, #0
.L22:
	cmp	r0, #60
	ble.n	.L23
	movs	r3, #60
	str	r3, [r5, #8]
	movs	r0, #60
.L23:
	adds	r0, #60
	str	r0, [r5, #8]
	movs	r1, #20
	bl	Func_080022ec
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	adds	r0, #2
	adds	r2, #60
	adds	r3, #80
	str	r2, [r5, #0]
	str	r3, [r5, #4]
	ldr	r4, [pc, #548]
	lsls	r5, r0, #1
	subs	r1, r5, #2
	ldrh	r1, [r4, r1]
	ldr	r4, [sp, #80]
	adds	r1, r4, r1
	lsrs	r4, r0, #31
	adds	r4, r0, r4
	asrs	r4, r4, #1
	subs	r3, r3, r0
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	subs	r2, r2, r4
	ldr	r0, [sp, #104]
	ldr	r5, [sp, #88]
	bl	Func_080072f8
.L20:
	adds	r7, #1
	adds	r6, #28
	cmp	r7, #64
	bne.n	.L24
	ldr	r3, [pc, #520]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	ldr	r6, [sp, #20]
	str	r3, [sp, #116]
	str	r4, [sp, #120]
	movs	r3, #0
	str	r3, [r6, #12]
	str	r3, [r6, #4]
	ldr	r0, [sp, #100]
	ldr	r1, [pc, #504]
	movs	r7, #0
	adds	r5, r0, r1
.L26:
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L25
	adds	r3, r7, #3
.L25:
	asrs	r3, r3, #2
	lsls	r2, r3, #2
	subs	r2, r7, r2
	movs	r4, #152
	ldr	r6, [sp, #20]
	lsls	r4, r4, #15
	lsls	r2, r2, #21
	adds	r2, r2, r4
	str	r2, [r6, #0]
	ldr	r0, [sp, #72]
	lsls	r3, r3, #21
	adds	r3, r3, r0
	str	r3, [r6, #8]
	ldmia	r5!, {r0}
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #24]
	movs	r3, #0
	adds	r7, #1
	bl	Func_08009008
	cmp	r7, #16
	bne.n	.L26
	ldr	r1, [sp, #72]
	ldr	r2, [sp, #68]
	ldr	r3, [sp, #84]
	adds	r1, r1, r2
	str	r1, [sp, #72]
	cmp	r3, #47
	bgt.n	.L27
	ldr	r4, [pc, #436]
	adds	r2, r2, r4
	str	r2, [sp, #68]
.L27:
	ldr	r5, [sp, #84]
	cmp	r5, #32
	ble.n	.L28
	ldr	r6, [sp, #68]
	lsls	r3, r6, #4
	subs	r3, r3, r6
	lsls	r3, r3, #2
	cmp	r3, #0
	bge.n	.L29
	adds	r3, #63
.L29:
	asrs	r3, r3, #6
	str	r3, [sp, #68]
.L28:
	ldr	r0, [sp, #84]
	cmp	r0, #144
	bne.n	.L30
	ldr	r1, [pc, #404]
	str	r1, [sp, #68]
.L30:
	ldr	r2, [sp, #84]
	cmp	r2, #146
	bne.n	.L31
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [sp, #68]
.L31:
	ldr	r4, [sp, #84]
	cmp	r4, #72
	bne.n	.L32
	ldr	r6, [sp, #100]
	ldr	r0, [pc, #372]
	movs	r7, #0
	adds	r5, r6, r0
.L33:
	adds	r1, r7, #0
	ldmia	r5!, {r0}
	adds	r1, #16
	adds	r7, #1
	bl	Func_08009020
	cmp	r7, #16
	bne.n	.L33
.L32:
	ldr	r1, [sp, #84]
	cmp	r1, #76
	bne.n	.L34
	ldr	r2, [sp, #100]
	ldr	r3, [pc, #344]
	movs	r7, #0
	adds	r5, r2, r3
.L35:
	adds	r1, r7, #0
	ldmia	r5!, {r0}
	adds	r1, #32
	adds	r7, #1
	bl	Func_08009020
	cmp	r7, #16
	bne.n	.L35
.L34:
	ldr	r3, [sp, #84]
	subs	r3, #116
	cmp	r3, #27
	bls.n	.L36
	b.n	.L37
.L36:
	cmp	r3, #0
	bge.n	.L38
	ldr	r3, [sp, #84]
	subs	r3, #113
.L38:
	asrs	r1, r3, #2
	cmp	r1, #6
	ble.n	.L39
	movs	r1, #6
.L39:
	ldr	r3, [pc, #308]
	ldrb	r3, [r3, r1]
	ldr	r2, [pc, #308]
	mov	sl, r3
	lsls	r3, r1, #1
	ldrh	r6, [r2, r3]
	movs	r4, #192
	lsls	r4, r4, #3
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	str	r5, [sp, #0]
	adds	r6, r6, r4
	bl	Func_080ed408
	movs	r1, #60
	mov	r9, r1
	ldr	r0, [sp, #100]
	mov	r2, r9
	mov	r3, sl
	subs	r2, r2, r3
	mov	r4, sl
	lsls	r3, r3, #1
	str	r3, [sp, #4]
	str	r4, [sp, #0]
	adds	r6, r0, r6
	movs	r5, #80
	ldr	r0, [pc, #256]
	subs	r5, r5, r3
	mov	r8, r3
	ldr	r4, [r0, #0]
	adds	r3, r5, #0
	ldr	r0, [sp, #104]
	adds	r1, r6, #0
	mov	r9, r2
	bl	Func_080072f4
	movs	r0, #47
	bl	Func_08002dd8
	movs	r1, #2
	str	r1, [sp, #0]
	movs	r2, #7
	movs	r1, #7
	movs	r3, #7
	movs	r0, #47
	bl	Func_080ed408
	mov	r3, r8
	mov	r2, sl
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	ldr	r0, [pc, #208]
	adds	r3, r5, #0
	ldr	r4, [r0, #0]
	adds	r1, r6, #0
	ldr	r0, [sp, #104]
	movs	r2, #60
	bl	Func_080072f4
	movs	r0, #47
	bl	Func_08002dd8
	movs	r1, #2
	str	r1, [sp, #0]
	movs	r2, #7
	movs	r1, #7
	movs	r3, #11
	movs	r0, #47
	bl	Func_080ed408
	mov	r3, r8
	mov	r2, sl
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	ldr	r5, [pc, #160]
	ldr	r0, [sp, #104]
	ldr	r4, [r5, #0]
	adds	r1, r6, #0
	mov	r2, r9
	movs	r3, #80
	bl	Func_080072f4
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #2
	str	r0, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #15
	movs	r0, #47
	bl	Func_080ed408
	mov	r2, r8
	mov	r1, sl
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #80
	ldr	r4, [r5, #0]
	ldr	r0, [sp, #104]
	adds	r1, r6, #0
	movs	r2, #60
	bl	Func_080072f4
	movs	r0, #47
	bl	Func_08002dd8
	ldr	r3, [pc, #100]
	movs	r7, #0
	mov	r8, r3
.L41:
	adds	r4, r7, #0
	movs	r5, #3
	ands	r4, r5
	str	r4, [sp, #8]
	bl	Func_08004458
	ldr	r3, [pc, #84]
	adds	r6, r0, #0
	ands	r6, r3
	adds	r0, r6, #0
	bl	Func_08002322
	ldr	r4, [sp, #8]
	adds	r5, r0, #0
	mov	r0, r8
	ldrb	r3, [r0, r4]
	lsls	r5, r5, #4
	lsrs	r3, r3, #1
	adds	r0, r6, #0
	asrs	r5, r5, #16
	subs	r5, r5, r3
	b.n	.L40
	.4byte 0x02010e00
	.4byte 0xfffffc00
	.4byte 0x080ede48
	.4byte 0x000077b4
	.4byte 0x000077b8
	.4byte 0x02010000
	.4byte 0x080edad0
	.4byte 0x000077d8
	.4byte 0xffffc000
	.4byte 0xfff80000
	.4byte 0x080eef28
	.4byte 0x080eef30
	.4byte 0x03001f0c
	.4byte 0x080eef4a
	.4byte 0x0000ffff
.L40:
	bl	Func_0800231c
	ldr	r2, [pc, #524]
	ldr	r4, [sp, #8]
	adds	r3, r0, #0
	ldrb	r0, [r2, r4]
	lsls	r3, r3, #4
	lsrs	r2, r0, #1
	ldr	r1, [pc, #516]
	asrs	r3, r3, #16
	subs	r3, r3, r2
	lsls	r2, r4, #1
	ldrh	r1, [r1, r2]
	ldr	r2, [sp, #100]
	mov	r6, r8
	adds	r1, r2, r1
	ldrb	r2, [r6, r4]
	adds	r5, #60
	str	r2, [sp, #0]
	str	r0, [sp, #4]
	adds	r3, #80
	ldr	r0, [sp, #104]
	adds	r2, r5, #0
	ldr	r4, [sp, #88]
	adds	r7, #1
	bl	Func_080072f4
	cmp	r7, #6
	bne.n	.L41
.L37:
	ldr	r5, [sp, #84]
	cmp	r5, #143
	ble.n	.L42
	ldr	r0, [pc, #472]
	lsls	r6, r5, #4
	movs	r1, #3
	str	r1, [sp, #0]
	adds	r6, r6, r0
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	bl	Func_080ed408
	ldr	r7, [sp, #56]
	movs	r5, #64
	movs	r2, #24
	str	r2, [sp, #0]
	str	r5, [sp, #4]
	adds	r7, #188
	ldr	r1, [sp, #100]
	ldr	r0, [sp, #104]
	ldr	r4, [r7, #0]
	adds	r3, r6, #0
	movs	r2, #36
	bl	Func_080072f4
	movs	r0, #47
	bl	Func_08002dd8
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	bl	Func_080ed408
	movs	r4, #24
	str	r5, [sp, #4]
	str	r4, [sp, #0]
	adds	r3, r6, #0
	ldr	r1, [sp, #100]
	movs	r2, #60
	ldr	r4, [r7, #0]
	ldr	r0, [sp, #104]
	bl	Func_080072f4
	movs	r0, #47
	bl	Func_08002dd8
	ldr	r3, [sp, #84]
	subs	r3, #144
	lsrs	r2, r3, #31
	ldr	r6, [sp, #100]
	ldr	r0, [pc, #372]
	adds	r3, r3, r2
	ldr	r5, [sp, #12]
	asrs	r1, r3, #1
	adds	r2, r6, r0
	movs	r3, #75
	mov	r8, r5
	str	r3, [r2, #0]
	cmp	r1, #6
	bgt.n	.L42
	lsls	r5, r1, #1
	adds	r5, r5, r1
	ldr	r3, [pc, #352]
	lsls	r5, r5, #7
	ldrh	r3, [r3, #14]
	adds	r5, r5, r1
	lsls	r5, r5, #1
	ldr	r1, [pc, #348]
	adds	r5, r5, r3
	movs	r2, #2
	adds	r5, r5, r1
	str	r2, [sp, #0]
	movs	r3, #3
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	bl	Func_080ed408
	ldr	r7, [r7, #0]
	adds	r6, r6, r5
	movs	r3, #4
	mov	r9, r7
	mov	sl, r6
	movs	r7, #0
	mov	fp, r3
.L43:
	lsls	r6, r7, #9
	adds	r0, r6, #0
	bl	Func_08002322
	mov	r5, r8
	muls	r5, r0
	mov	r4, fp
	adds	r0, r6, #0
	asrs	r5, r5, #16
	subs	r5, r5, r4
	bl	Func_0800231c
	mov	r3, r8
	muls	r3, r0
	adds	r5, #60
	movs	r0, #16
	asrs	r3, r3, #17
	movs	r6, #8
	str	r0, [sp, #4]
	adds	r3, #72
	str	r6, [sp, #0]
	ldr	r0, [sp, #104]
	mov	r1, sl
	adds	r2, r5, #0
	adds	r7, #1
	bl	Func_08007308
	cmp	r7, #128
	bne.n	.L43
	movs	r0, #47
	bl	Func_08002dd8
.L42:
	ldr	r1, [sp, #100]
	ldr	r3, [pc, #248]
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r4, [sp, #12]
	ldr	r5, [sp, #84]
	adds	r4, #10
	adds	r5, #1
	str	r4, [sp, #12]
	str	r5, [sp, #84]
	cmp	r5, #160
	beq.n	.L44
	cmp	r5, #4
	bgt.n	.L45
	b.n	.L46
.L45:
	ldr	r3, [pc, #216]
	ldr	r3, [r3, #0]
	movs	r6, #3
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L44
	b.n	.L46
.L44:
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r3, [pc, #204]
	ldr	r0, [sp, #104]
	movs	r2, #0
	bl	Func_080072f0
	ldr	r1, [pc, #196]
	ldr	r0, [sp, #100]
	movs	r7, #0
	adds	r5, r0, r1
.L47:
	ldmia	r5!, {r0}
	adds	r7, #1
	bl	Func_08009038
	cmp	r7, #16
	bne.n	.L47
	movs	r2, #13
	negs	r2, r2
	ldr	r6, [pc, #172]
	movs	r7, #0
	mov	r8, r2
.L49:
	movs	r0, #195
	lsls	r0, r0, #1
	bl	Func_08009030
	ldr	r3, [sp, #100]
	adds	r5, r0, #0
	str	r5, [r6, r3]
	cmp	r5, #0
	beq.n	.L48
	adds	r2, r5, #0
	adds	r2, #38
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #3
	adds	r0, r7, #0
	bl	Func_080022fc
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_08009020
	ldr	r4, [sp, #100]
	ldr	r2, [r6, r4]
	ldrb	r3, [r2, #9]
	mov	r5, r8
	ands	r3, r5
	strb	r3, [r2, #9]
.L48:
	adds	r7, #1
	adds	r6, #4
	cmp	r7, #16
	bne.n	.L49
	movs	r0, #46
	bl	Func_08002dd8
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	Func_080ed408
	ldr	r3, [pc, #88]
	adds	r3, #184
	ldr	r3, [r3, #0]
	ldr	r6, [sp, #100]
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r1, r6, r2
	str	r3, [sp, #88]
	ldr	r0, [pc, #76]
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	ldr	r0, [pc, #68]
	ldr	r1, [sp, #80]
	movs	r7, #0
	movs	r4, #1
.L52:
	ldrb	r3, [r1, #0]
	adds	r2, r3, #0
	cmp	r2, #32
	bls.n	.L50
	adds	r3, #224
	strb	r3, [r1, #0]
	b.n	.L51
	movs	r0, r0
	.4byte 0x080eef50
	.4byte 0x080eef3e
	.4byte 0xfffff720
	.4byte 0x00007784
	.4byte 0x080ede48
	.4byte 0x00002710
	.4byte 0x00007824
	.4byte 0x03001b04
	.4byte 0x03000168
	.4byte 0x000077d8
	.4byte 0x03001e50
	.4byte 0x00000064
	.4byte 0x00000302
.L50:
	cmp	r2, #0
	beq.n	.L51
	strb	r4, [r1, #0]
.L51:
	adds	r7, #1
	adds	r1, #1
	cmp	r7, r0
	bne.n	.L52
	ldr	r4, [sp, #100]
	ldr	r5, [pc, #88]
	ldr	r6, [pc, #92]
	adds	r3, r4, r5
	movs	r5, #0
	str	r5, [r3, #0]
	adds	r3, r4, r6
	str	r5, [r3, #0]
	ldr	r1, [pc, #84]
	movs	r0, #1
	movs	r2, #0
	bl	Func_080b5040
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #76]
	adds	r2, #8
	str	r3, [r2, #0]
	ldr	r3, [pc, #48]
	add	r4, sp, #140
	subs	r2, #28
	strh	r3, [r2, #0]
	str	r5, [r4, #0]
	ldr	r3, [pc, #64]
	adds	r0, r4, #0
	ldr	r1, [sp, #104]
	ldr	r2, [pc, #60]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #60
	ldr	r6, [sp, #100]
	str	r0, [sp, #52]
	movs	r1, #44
	movs	r0, #239
	str	r1, [sp, #48]
	movs	r3, #0
	lsls	r0, r0, #7
	ldr	r1, [pc, #44]
	str	r3, [sp, #32]
	str	r3, [sp, #40]
	b.n	.L53
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000784
	.4byte 0x000077b4
	.4byte 0x000077b8
	.4byte 0x0000003e
	.4byte 0x04000020
	.4byte 0xffffc400
	.4byte 0x040000d4
	.4byte 0x85001000
	.4byte 0x00007784
.L53:
	movs	r5, #2
	adds	r3, r6, r0
	movs	r2, #6
	str	r2, [sp, #44]
	str	r5, [sp, #36]
	adds	r2, r6, r1
	str	r5, [r3, #0]
	movs	r3, #75
	str	r3, [r2, #0]
	ldr	r2, [sp, #32]
	str	r2, [r4, #0]
	ldr	r3, [pc, #252]
	adds	r0, r4, #0
	ldr	r1, [sp, #100]
	ldr	r2, [pc, #248]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #0
	str	r3, [sp, #84]
.L146:
	ldr	r4, [sp, #84]
	cmp	r4, #66
	bne.n	.L54
	movs	r0, #145
	bl	Func_080f9010
	movs	r0, #141
	bl	Func_080f9010
.L54:
	ldr	r5, [sp, #84]
	cmp	r5, #155
	bne.n	.L55
	movs	r0, #162
	bl	Func_080f9010
.L55:
	ldr	r6, [sp, #84]
	cmp	r6, #217
	bne.n	.L56
	movs	r0, #156
	bl	Func_080f9010
.L56:
	movs	r1, #140
	ldr	r0, [sp, #84]
	lsls	r1, r1, #1
	cmp	r0, r1
	bne.n	.L57
	movs	r0, #157
	bl	Func_080f9010
.L57:
	movs	r3, #150
	ldr	r2, [sp, #84]
	lsls	r3, r3, #1
	cmp	r2, r3
	bne.n	.L58
	movs	r0, #145
	bl	Func_080b50e8
.L58:
	ldr	r3, [pc, #168]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L59
	ldr	r3, [sp, #84]
	subs	r3, #5
	cmp	r3, #144
	bhi.n	.L60
	ldr	r1, [pc, #152]
	movs	r4, #150
	str	r4, [sp, #84]
	ldr	r5, [pc, #152]
	ldrh	r3, [r5, #0]
	adds	r0, r3, #0
	movs	r6, #130
	ldr	r2, [pc, #144]
	lsls	r6, r6, #2
	strh	r6, [r2, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L61
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	adds	r3, #4
	strh	r2, [r1, #0]
	movs	r2, #128
	stmia	r3!, {r2}
	ldr	r2, [pc, #116]
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L61:
	ldr	r3, [pc, #104]
	strh	r0, [r3, #0]
	ldrh	r3, [r3, #0]
	adds	r0, r3, #0
	movs	r4, #130
	ldr	r5, [pc, #92]
	lsls	r4, r4, #2
	strh	r4, [r5, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L62
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r1
	adds	r3, #4
	adds	r2, #1
	movs	r6, #0
	stmia	r3!, {r6}
	strh	r2, [r1, #0]
	ldr	r2, [pc, #72]
	stmia	r3!, {r2}
	movs	r2, #192
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L62:
	ldr	r1, [pc, #52]
	strh	r0, [r1, #0]
	ldr	r0, [pc, #60]
	movs	r3, #128
	ldr	r2, [sp, #100]
	lsls	r3, r3, #7
	adds	r1, r2, r3
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
	b.n	.L59
.L60:
	ldr	r3, [sp, #84]
	subs	r3, #155
	cmp	r3, #58
	bhi.n	.L63
	movs	r4, #214
	str	r4, [sp, #84]
	b.n	.L59
	movs	r0, r0
	.4byte 0x040000d4
	.4byte 0x85000e10
	.4byte 0x03001b04
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x04000020
	.4byte 0x04000028
	.4byte 0x00000070
.L63:
	ldr	r3, [sp, #84]
	subs	r3, #219
	cmp	r3, #60
	bhi.n	.L59
	movs	r5, #140
	lsls	r5, r5, #1
	str	r5, [sp, #84]
.L59:
	ldr	r6, [sp, #84]
	cmp	r6, #64
	bne.n	.L64
	ldr	r1, [pc, #208]
	ldr	r0, [pc, #212]
	ldrh	r3, [r0, #0]
	adds	r0, r3, #0
	movs	r2, #130
	ldr	r3, [pc, #204]
	lsls	r2, r2, #2
	strh	r2, [r3, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L65
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	adds	r3, #4
	strh	r2, [r1, #0]
	movs	r2, #128
	stmia	r3!, {r2}
	ldr	r2, [pc, #176]
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L65:
	ldr	r4, [pc, #164]
	strh	r0, [r4, #0]
	ldrh	r3, [r4, #0]
	adds	r0, r3, #0
	movs	r5, #130
	ldr	r6, [pc, #152]
	lsls	r5, r5, #2
	strh	r5, [r6, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L66
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r1
	adds	r2, #1
	adds	r3, #4
	strh	r2, [r1, #0]
	movs	r1, #0
	stmia	r3!, {r1}
	ldr	r2, [pc, #132]
	stmia	r3!, {r2}
	movs	r2, #192
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L66:
	ldr	r2, [pc, #112]
	strh	r0, [r2, #0]
	movs	r3, #0
	add	r4, sp, #140
	str	r3, [sp, #140]
	adds	r0, r4, #0
	ldr	r3, [pc, #112]
	ldr	r1, [sp, #104]
	ldr	r2, [pc, #112]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [sp, #100]
	movs	r6, #128
	lsls	r6, r6, #7
	ldr	r0, [pc, #104]
	adds	r1, r5, r6
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
	movs	r2, #192
	lsls	r2, r2, #7
	adds	r1, r5, r2
	ldr	r0, [pc, #92]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
.L64:
	ldr	r3, [sp, #84]
	cmp	r3, #66
	bne.n	.L67
	ldr	r2, [pc, #80]
	ldr	r1, [pc, #40]
	movs	r7, #0
.L68:
	ldrh	r3, [r2, #0]
	adds	r7, #1
	eors	r3, r1
	strh	r3, [r2, #0]
	adds	r2, #2
	cmp	r7, #128
	bne.n	.L68
.L67:
	ldr	r4, [sp, #84]
	cmp	r4, #69
	bne.n	.L69
	ldr	r3, [pc, #56]
	add	r5, sp, #140
	str	r3, [sp, #140]
	adds	r0, r5, #0
	ldr	r3, [pc, #28]
	ldr	r1, [sp, #104]
	ldr	r2, [pc, #28]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	b.n	.L69
	.4byte 0x00007fff
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x04000020
	.4byte 0x04000028
	.4byte 0x040000d4
	.4byte 0x85001000
	.4byte 0x00000070
	.4byte 0x00000065
	.4byte 0x050000c0
	.4byte 0x3f3f3f3f
.L69:
	ldr	r6, [sp, #84]
	cmp	r6, #70
	bne.n	.L70
	movs	r0, #1
	ldr	r1, [pc, #280]
	movs	r2, #7
	bl	Func_080b5038
.L70:
	ldr	r0, [sp, #84]
	cmp	r0, #150
	bne.n	.L71
	movs	r3, #0
	movs	r1, #112
	movs	r2, #32
	movs	r4, #4
	movs	r5, #8
	add	r6, sp, #140
	str	r1, [sp, #52]
	str	r2, [sp, #48]
	str	r3, [sp, #40]
	str	r3, [sp, #140]
	str	r4, [sp, #36]
	str	r5, [sp, #44]
	ldr	r3, [pc, #248]
	adds	r0, r6, #0
	ldr	r1, [sp, #100]
	ldr	r2, [pc, #244]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #244]
	adds	r1, r6, #0
	str	r3, [sp, #140]
	adds	r0, r1, #0
	ldr	r3, [pc, #228]
	ldr	r1, [sp, #104]
	ldr	r2, [pc, #236]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	ldr	r1, [pc, #232]
	movs	r2, #0
	bl	Func_080b5040
.L71:
	ldr	r2, [sp, #84]
	cmp	r2, #214
	bne.n	.L72
	movs	r3, #0
	add	r4, sp, #140
	str	r3, [sp, #140]
	adds	r0, r4, #0
	ldr	r3, [pc, #192]
	ldr	r1, [sp, #100]
	ldr	r2, [pc, #192]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #192]
	adds	r5, r4, #0
	str	r3, [sp, #140]
	adds	r0, r5, #0
	ldr	r3, [pc, #176]
	ldr	r1, [sp, #104]
	ldr	r2, [pc, #184]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	ldr	r1, [pc, #184]
	movs	r2, #0
	bl	Func_080b5040
	movs	r0, #225
	ldr	r6, [sp, #100]
	lsls	r0, r0, #7
	movs	r7, #0
	adds	r5, r6, r0
.L73:
	bl	Func_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	bl	Func_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r3, #96
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	Func_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r3, #16
	lsls	r3, r3, #15
	ldr	r2, [r5, #0]
	str	r3, [r5, #16]
	movs	r3, #128
	asrs	r2, r2, #7
	lsls	r3, r3, #8
	subs	r3, r3, r2
	str	r3, [r5, #8]
	adds	r7, #1
	movs	r3, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #16
	bne.n	.L73
.L72:
	movs	r2, #140
	ldr	r1, [sp, #84]
	lsls	r2, r2, #1
	cmp	r1, r2
	bne.n	.L74
	bl	Func_080d67dc
	movs	r3, #0
	add	r4, sp, #140
	str	r3, [sp, #140]
	adds	r0, r4, #0
	ldr	r3, [pc, #56]
	ldr	r1, [sp, #100]
	ldr	r2, [pc, #56]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #72]
	adds	r5, r4, #0
	str	r3, [sp, #140]
	adds	r0, r5, #0
	ldr	r3, [pc, #40]
	ldr	r1, [sp, #104]
	ldr	r2, [pc, #48]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [sp, #96]
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #16]
	movs	r6, #0
	strh	r3, [r2, #0]
	str	r6, [r0, #16]
	ldr	r1, [sp, #100]
	movs	r2, #225
	lsls	r2, r2, #7
	movs	r7, #0
	adds	r5, r1, r2
	b.n	.L75
	.4byte 0x00001010
	.4byte 0x0000003e
	.4byte 0x040000d4
	.4byte 0x85000e10
	.4byte 0x3f3f3f3f
	.4byte 0x85001000
	.4byte 0x00000036
	.4byte 0x0000003a
	.4byte 0x01010101
	.4byte 0x04000052
.L75:
	bl	Func_08004458
	movs	r3, #63
	movs	r4, #128
	lsls	r4, r4, #1
	ands	r3, r0
	adds	r3, r3, r4
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	bl	Func_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r3, #96
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	Func_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r3, #16
	lsls	r3, r3, #15
	movs	r6, #0
	adds	r7, #1
	str	r3, [r5, #16]
	str	r6, [r5, #8]
	adds	r5, #28
	cmp	r7, #16
	bne.n	.L75
	ldr	r1, [sp, #100]
	movs	r2, #239
	movs	r0, #136
	lsls	r2, r2, #7
	ldr	r4, [pc, #72]
	adds	r3, r1, r2
	lsls	r0, r0, #1
	str	r0, [sp, #32]
	str	r6, [r3, #0]
	adds	r3, r1, r4
	str	r6, [r3, #0]
.L74:
	ldr	r5, [sp, #84]
	ldr	r6, [pc, #60]
	cmp	r5, r6
	ble.n	.L76
	ldr	r5, [pc, #56]
	ldr	r6, [pc, #44]
	movs	r7, #0
.L80:
	ldrh	r2, [r5, #0]
	lsls	r3, r2, #16
	lsrs	r0, r3, #26
	ands	r0, r6
	lsrs	r1, r3, #21
	movs	r4, #31
	ands	r1, r6
	ands	r4, r2
	adds	r0, #1
	adds	r1, #1
	adds	r4, #1
	cmp	r0, #31
	ble.n	.L77
	movs	r0, #31
.L77:
	cmp	r1, #31
	ble.n	.L78
	movs	r1, #31
.L78:
	cmp	r4, #31
	ble.n	.L79
	movs	r4, #31
	b.n	.L79
	.4byte 0x0000001f
	.4byte 0x00007784
	.4byte 0x00000117
	.4byte 0x05000002
.L79:
	lsls	r3, r0, #10
	lsls	r2, r1, #5
	orrs	r3, r2
	orrs	r3, r4
	adds	r7, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r7, #63
	bne.n	.L80
.L76:
	ldr	r0, [sp, #84]
	cmp	r0, #182
	bne.n	.L81
	ldr	r3, [pc, #788]
	add	r1, sp, #140
	str	r3, [sp, #140]
	adds	r0, r1, #0
	ldr	r3, [pc, #784]
	ldr	r1, [sp, #104]
	ldr	r2, [pc, #784]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L81:
	ldr	r2, [sp, #84]
	cmp	r2, #63
	bgt.n	.L82
	ldr	r4, [sp, #84]
	movs	r3, #7
	ldr	r5, [sp, #100]
	movs	r6, #128
	subs	r2, #4
	lsls	r6, r6, #7
	ands	r3, r4
	mov	r8, r2
	adds	r7, r5, r6
	cmp	r3, #3
	ble.n	.L83
	ldr	r0, [sp, #100]
	ldr	r1, [pc, #752]
	adds	r7, r0, r1
.L83:
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	str	r5, [sp, #0]
	bl	Func_080ed408
	movs	r2, #24
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	ldr	r3, [pc, #732]
	mov	r6, r8
	subs	r6, #24
	ldr	r0, [sp, #104]
	ldr	r4, [r3, #0]
	adds	r1, r7, #0
	adds	r3, r6, #0
	movs	r2, #36
	bl	Func_080072f4
	movs	r0, #47
	bl	Func_08002dd8
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	str	r5, [sp, #0]
	bl	Func_080ed408
	movs	r4, #24
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [pc, #684]
	adds	r3, r6, #0
	ldr	r4, [r0, #0]
	adds	r1, r7, #0
	ldr	r0, [sp, #104]
	movs	r2, #59
	bl	Func_080072f4
	movs	r0, #47
	bl	Func_08002dd8
	movs	r1, #7
	movs	r2, #7
	movs	r3, #11
	movs	r0, #47
	str	r5, [sp, #0]
	bl	Func_080ed408
	movs	r1, #24
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	ldr	r2, [pc, #644]
	adds	r6, #23
	ldr	r4, [r2, #0]
	ldr	r0, [sp, #104]
	adds	r1, r7, #0
	movs	r2, #36
	adds	r3, r6, #0
	bl	Func_080072f4
	movs	r0, #47
	bl	Func_08002dd8
	movs	r1, #7
	movs	r2, #7
	movs	r3, #15
	movs	r0, #47
	str	r5, [sp, #0]
	bl	Func_080ed408
	movs	r3, #24
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	ldr	r5, [pc, #600]
	ldr	r0, [sp, #104]
	ldr	r4, [r5, #0]
	adds	r1, r7, #0
	movs	r2, #59
	adds	r3, r6, #0
	bl	Func_080072f4
	movs	r0, #47
	bl	Func_08002dd8
.L82:
	ldr	r3, [sp, #84]
	subs	r3, #64
	cmp	r3, #1
	bhi.n	.L84
	movs	r3, #16
	ldr	r6, [sp, #100]
	movs	r2, #192
	lsls	r2, r2, #7
	str	r3, [sp, #0]
	movs	r3, #17
	str	r3, [sp, #4]
	adds	r1, r6, r2
	ldr	r0, [sp, #104]
	movs	r2, #52
	movs	r3, #51
	ldr	r4, [sp, #88]
	bl	Func_080072f4
.L84:
	ldr	r3, [sp, #84]
	subs	r3, #66
	cmp	r3, #1
	bhi.n	.L85
	ldr	r2, [pc, #536]
	ldr	r6, [sp, #100]
	movs	r3, #41
	movs	r5, #24
	str	r3, [sp, #4]
	adds	r1, r6, r2
	str	r5, [sp, #0]
	ldr	r0, [sp, #104]
	movs	r2, #48
	movs	r3, #40
	ldr	r4, [sp, #88]
	bl	Func_080072f4
.L85:
	ldr	r3, [sp, #84]
	subs	r3, #68
	cmp	r3, #7
	bhi.n	.L86
	ldr	r5, [sp, #84]
	movs	r6, #76
	subs	r6, r6, r5
	ldr	r0, [sp, #100]
	ldr	r1, [pc, #500]
	lsrs	r5, r6, #31
	adds	r5, r6, r5
	movs	r2, #38
	adds	r0, r0, r1
	mov	r8, r2
	asrs	r5, r5, #1
	ldr	r2, [sp, #88]
	movs	r4, #49
	mov	fp, r0
	subs	r4, r4, r5
	movs	r1, #44
	mov	r3, r8
	movs	r0, #22
	subs	r3, r3, r6
	str	r0, [sp, #0]
	mov	sl, r1
	str	r1, [sp, #4]
	mov	r9, r2
	str	r4, [sp, #8]
	ldr	r0, [sp, #104]
	adds	r2, r4, #0
	mov	r1, fp
	mov	r8, r3
	bl	Func_08007308
	ldr	r4, [sp, #8]
	adds	r6, #38
	movs	r3, #22
	mov	r0, sl
	adds	r2, r4, #0
	str	r3, [sp, #0]
	str	r0, [sp, #4]
	mov	r1, fp
	ldr	r0, [sp, #104]
	adds	r3, r6, #0
	bl	Func_08007308
	adds	r5, #49
	movs	r1, #22
	mov	r2, sl
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	mov	r1, fp
	adds	r2, r5, #0
	mov	r3, r8
	ldr	r0, [sp, #104]
	bl	Func_08007308
	movs	r3, #22
	mov	r4, sl
	str	r3, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #104]
	mov	r1, fp
	adds	r2, r5, #0
	adds	r3, r6, #0
	bl	Func_08007308
.L86:
	ldr	r3, [sp, #84]
	subs	r3, #78
	cmp	r3, #1
	bhi.n	.L87
	movs	r3, #16
	ldr	r5, [sp, #100]
	movs	r6, #192
	str	r3, [sp, #0]
	lsls	r6, r6, #7
	movs	r3, #17
	str	r3, [sp, #4]
	ldr	r0, [sp, #104]
	adds	r1, r5, r6
	movs	r2, #52
	movs	r3, #51
	ldr	r4, [sp, #88]
	bl	Func_080072f4
.L87:
	ldr	r3, [sp, #84]
	subs	r3, #80
	cmp	r3, #1
	bhi.n	.L88
	ldr	r2, [pc, #332]
	ldr	r6, [sp, #100]
	movs	r3, #41
	movs	r5, #24
	str	r3, [sp, #4]
	adds	r1, r6, r2
	str	r5, [sp, #0]
	ldr	r0, [sp, #104]
	movs	r2, #48
	movs	r3, #40
	ldr	r4, [sp, #88]
	bl	Func_080072f4
.L88:
	ldr	r3, [sp, #84]
	subs	r3, #82
	cmp	r3, #3
	bhi.n	.L89
	ldr	r5, [sp, #84]
	ldr	r0, [pc, #300]
	lsls	r7, r5, #1
	ldr	r6, [sp, #100]
	adds	r2, r7, #0
	subs	r2, #164
	adds	r0, r6, r0
	str	r0, [sp, #28]
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L90
	adds	r3, r7, #0
	subs	r3, #161
.L90:
	asrs	r3, r3, #2
	movs	r5, #49
	subs	r5, r5, r3
	mov	r9, r3
	movs	r6, #38
	ldr	r3, [sp, #88]
	subs	r6, r6, r2
	movs	r1, #22
	movs	r2, #44
	str	r1, [sp, #0]
	mov	r8, r2
	str	r2, [sp, #4]
	mov	sl, r3
	ldr	r1, [sp, #28]
	ldr	r0, [sp, #104]
	adds	r2, r5, #0
	adds	r3, r6, #0
	bl	Func_0800730c
	subs	r7, #126
	movs	r4, #22
	mov	fp, r7
	mov	r0, r8
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r1, [sp, #28]
	adds	r2, r5, #0
	ldr	r0, [sp, #104]
	mov	r3, fp
	bl	Func_0800730c
	movs	r1, #49
	add	r9, r1
	movs	r2, #22
	mov	r3, r8
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	ldr	r1, [sp, #28]
	mov	r2, r9
	adds	r3, r6, #0
	ldr	r0, [sp, #104]
	bl	Func_0800730c
	mov	r5, r8
	movs	r4, #22
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #104]
	ldr	r1, [sp, #28]
	mov	r2, r9
	mov	r3, fp
	bl	Func_0800730c
.L89:
	ldr	r3, [sp, #84]
	subs	r3, #72
	cmp	r3, #3
	bhi.n	.L91
	ldr	r6, [sp, #84]
	ldr	r0, [pc, #164]
	lsls	r3, r6, #4
	ldr	r2, [pc, #164]
	adds	r0, r0, r3
	movs	r1, #5
	movs	r3, #20
	mov	r8, r0
	movs	r7, #0
	mov	fp, r1
	mov	r9, r2
	mov	sl, r3
.L92:
	lsls	r6, r7, #8
	adds	r0, r6, #0
	bl	Func_08002322
	mov	r5, r8
	muls	r5, r0
	mov	r4, fp
	adds	r0, r6, #0
	asrs	r5, r5, #16
	subs	r5, r5, r4
	bl	Func_0800231c
	mov	r3, r8
	muls	r3, r0
	mov	r2, sl
	subs	r2, #2
	mov	r6, r9
	ldrh	r1, [r6, r2]
	ldr	r0, [sp, #80]
	adds	r5, #60
	movs	r2, #10
	mov	r4, sl
	asrs	r3, r3, #17
	movs	r6, #128
	adds	r1, r0, r1
	str	r2, [sp, #0]
	adds	r3, #50
	adds	r2, r5, #0
	str	r4, [sp, #4]
	ldr	r0, [sp, #104]
	ldr	r5, [sp, #88]
	adds	r7, #1
	lsls	r6, r6, #1
	bl	Func_080072f8
	cmp	r7, r6
	bne.n	.L92
.L91:
	ldr	r0, [sp, #84]
	cmp	r0, #85
	bgt.n	.L93
	b.n	.L94
.L93:
	cmp	r0, #213
	ble.n	.L95
	b.n	.L94
.L95:
	ldr	r1, [sp, #36]
	movs	r7, #0
	cmp	r1, #0
	bne.n	.L96
	b.n	.L94
.L96:
	ldr	r2, [sp, #40]
	movs	r3, #0
	mov	lr, r2
	mov	sl, r3
	mov	r8, r2
	cmp	r2, #0
	bge.n	.L97
	b.n	.L98
.L97:
	b.n	.L99
	movs	r0, r0
	.4byte 0x3f3f3f3f
	.4byte 0x040000d4
	.4byte 0x85001000
	.4byte 0x00004240
	.4byte 0x03001f0c
	.4byte 0x00006110
	.4byte 0x000064e8
	.4byte 0xfffffb90
	.4byte 0x080ede48
.L99:
	mov	r4, lr
	lsrs	r3, r4, #31
	add	r3, lr
	asrs	r3, r3, #1
	mov	r9, r3
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L100
	adds	r3, #7
.L100:
	ldr	r5, [sp, #52]
	ldr	r6, [sp, #52]
	ldr	r1, [sp, #44]
	asrs	r3, r3, #3
	adds	r0, r5, r3
	subs	r4, r6, r3
	mov	r3, sl
	muls	r3, r1
	cmp	r3, #0
	bge.n	.L101
	adds	r3, #7
.L101:
	ldr	r5, [sp, #48]
	asrs	r3, r3, #3
	mov	r6, sl
	adds	r2, r5, r3
	lsls	r3, r6, #3
	cmp	r3, #0
	bge.n	.L102
	adds	r3, #7
.L102:
	ldr	r5, [sp, #48]
	asrs	r3, r3, #3
	subs	r1, r5, r3
	cmp	r1, #0
	bge.n	.L103
	movs	r1, #0
.L103:
	cmp	r2, #119
	ble.n	.L104
	movs	r2, #119
.L104:
	cmp	r4, #0
	bge.n	.L105
	movs	r4, #0
.L105:
	cmp	r0, #119
	ble.n	.L106
	movs	r0, #119
.L106:
	lsls	r3, r2, #4
	subs	r3, r3, r2
	lsls	r5, r3, #3
	ldr	r3, [sp, #100]
	movs	r2, #20
	adds	r6, r5, r0
	strb	r2, [r3, r6]
	lsls	r3, r1, #4
	subs	r3, r3, r1
	ldr	r6, [sp, #100]
	lsls	r1, r3, #3
	adds	r3, r1, r0
	strb	r2, [r6, r3]
	adds	r3, r5, r4
	strb	r2, [r6, r3]
	adds	r3, r1, r4
	strb	r2, [r6, r3]
	ldr	r3, [sp, #52]
	ldr	r2, [sp, #52]
	add	r3, r9
	mov	r4, r9
	adds	r0, r3, #1
	subs	r3, r2, r4
	adds	r4, r3, #1
	cmp	r4, #0
	bge.n	.L107
	movs	r4, #0
.L107:
	cmp	r0, #119
	ble.n	.L108
	movs	r0, #119
.L108:
	ldr	r6, [sp, #100]
	movs	r2, #20
	adds	r3, r5, r0
	strb	r2, [r6, r3]
	adds	r3, r1, r0
	strb	r2, [r6, r3]
	adds	r3, r5, r4
	strb	r2, [r6, r3]
	mov	r0, sl
	adds	r3, r1, r4
	strb	r2, [r6, r3]
	lsrs	r3, r0, #31
	add	r3, sl
	asrs	r3, r3, #1
	mov	ip, r3
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L109
	adds	r3, #7
.L109:
	ldr	r1, [sp, #52]
	ldr	r2, [sp, #52]
	ldr	r5, [sp, #44]
	asrs	r3, r3, #3
	adds	r0, r1, r3
	subs	r4, r2, r3
	mov	r3, lr
	muls	r3, r5
	cmp	r3, #0
	bge.n	.L110
	adds	r3, #7
.L110:
	ldr	r6, [sp, #48]
	asrs	r3, r3, #3
	mov	r1, lr
	adds	r2, r6, r3
	lsls	r3, r1, #3
	cmp	r3, #0
	bge.n	.L111
	adds	r3, #7
.L111:
	ldr	r5, [sp, #48]
	asrs	r3, r3, #3
	subs	r1, r5, r3
	cmp	r4, #0
	bge.n	.L112
	movs	r4, #0
.L112:
	cmp	r0, #119
	ble.n	.L113
	movs	r0, #119
.L113:
	cmp	r1, #0
	bge.n	.L114
	movs	r1, #0
.L114:
	cmp	r2, #119
	ble.n	.L115
	movs	r2, #119
.L115:
	lsls	r3, r2, #4
	subs	r3, r3, r2
	lsls	r6, r3, #3
	ldr	r2, [sp, #100]
	movs	r5, #20
	adds	r3, r6, r0
	strb	r5, [r2, r3]
	lsls	r3, r1, #4
	subs	r3, r3, r1
	lsls	r2, r3, #3
	adds	r3, r2, r0
	ldr	r0, [sp, #100]
	strb	r5, [r0, r3]
	adds	r3, r6, r4
	strb	r5, [r0, r3]
	adds	r3, r2, r4
	strb	r5, [r0, r3]
	ldr	r3, [sp, #52]
	ldr	r1, [sp, #52]
	add	r3, ip
	mov	r4, ip
	adds	r0, r3, #1
	subs	r3, r1, r4
	adds	r4, r3, #1
	cmp	r4, #0
	bge.n	.L116
	movs	r4, #0
.L116:
	cmp	r0, #119
	ble.n	.L117
	movs	r0, #119
.L117:
	ldr	r1, [sp, #100]
	adds	r3, r6, r0
	strb	r5, [r1, r3]
	adds	r3, r2, r0
	strb	r5, [r1, r3]
	adds	r3, r6, r4
	strb	r5, [r1, r3]
	adds	r3, r2, r4
	mov	r2, sl
	mov	r4, r8
	strb	r5, [r1, r3]
	lsls	r3, r2, #1
	subs	r3, r4, r3
	subs	r3, #1
	mov	r8, r3
	cmp	r3, #0
	bge.n	.L118
	mov	r5, lr
	lsls	r3, r5, #1
	add	r3, r8
	movs	r6, #1
	subs	r3, #2
	negs	r6, r6
	mov	r8, r3
	add	lr, r6
.L118:
	movs	r0, #1
	add	sl, r0
	cmp	lr, sl
	blt.n	.L98
	b.n	.L99
.L98:
	ldr	r1, [sp, #40]
	ldr	r2, [sp, #36]
	adds	r1, #1
	adds	r7, #1
	str	r1, [sp, #40]
	cmp	r7, r2
	beq.n	.L94
	b.n	.L96
.L94:
	ldr	r3, [sp, #84]
	subs	r3, #86
	mov	r8, r3
	cmp	r3, #63
	bhi.n	.L119
	ldr	r5, [sp, #84]
	ldr	r4, [sp, #84]
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	subs	r4, #70
	asrs	r3, r3, #1
	ldr	r6, [pc, #656]
	movs	r7, #0
	mov	r9, r4
	mov	sl, r3
.L121:
	adds	r0, r6, #0
	bl	Func_08002322
	mov	r3, r8
	muls	r3, r0
	adds	r0, r6, #0
	asrs	r5, r3, #16
	bl	Func_0800231c
	mov	r3, r9
	muls	r3, r0
	mov	r1, sl
	adds	r2, r1, r7
	asrs	r0, r3, #16
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L120
	adds	r3, r2, #3
.L120:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	lsls	r1, r3, #3
	subs	r1, r1, r3
	lsls	r1, r1, #2
	ldr	r2, [sp, #100]
	subs	r1, r1, r3
	lsls	r1, r1, #6
	movs	r3, #128
	adds	r1, r2, r1
	lsls	r3, r3, #7
	adds	r1, r1, r3
	adds	r3, r0, #0
	movs	r0, #32
	adds	r2, r5, #0
	str	r0, [sp, #0]
	movs	r5, #128
	movs	r0, #54
	str	r0, [sp, #4]
	adds	r2, #44
	adds	r3, #17
	ldr	r0, [sp, #104]
	ldr	r4, [sp, #88]
	lsls	r5, r5, #5
	adds	r7, #1
	bl	Func_080072f4
	adds	r6, r6, r5
	cmp	r7, #9
	bne.n	.L121
.L119:
	movs	r3, #3
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	str	r3, [sp, #0]
	bl	Func_080ed408
	ldr	r6, [pc, #540]
	ldr	r0, [sp, #84]
	ldr	r4, [r6, #0]
	str	r4, [sp, #92]
	cmp	r0, #85
	ble.n	.L122
	movs	r3, #120
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	ldr	r0, [sp, #104]
	ldr	r1, [sp, #100]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080072f4
.L122:
	ldr	r1, [sp, #84]
	ldr	r2, [pc, #512]
	cmp	r1, r2
	ble.n	.L123
	ldr	r3, [sp, #32]
	subs	r3, #8
	str	r3, [sp, #32]
.L123:
	ldr	r4, [sp, #84]
	ldr	r5, [pc, #500]
	cmp	r4, r5
	ble.n	.L124
	ldr	r3, [pc, #500]
	ldr	r6, [sp, #100]
	ldr	r3, [r6, r3]
	ldr	r3, [r3, #20]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L124
	ldr	r0, [pc, #484]
	adds	r5, r6, r0
	movs	r6, #36
.L125:
	ldr	r3, [r5, #0]
	movs	r2, #0
	ldrsh	r0, [r3, r6]
	movs	r3, #1
	str	r2, [sp, #0]
	negs	r3, r3
	movs	r1, #14
	movs	r2, #5
	bl	Func_080d6888
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	adds	r7, #1
	adds	r6, #2
	cmp	r7, r3
	bne.n	.L125
.L124:
	ldr	r3, [sp, #84]
	cmp	r3, #238
	bne.n	.L126
	ldr	r3, [pc, #444]
	add	r4, sp, #140
	str	r3, [sp, #140]
	adds	r0, r4, #0
	ldr	r3, [pc, #440]
	ldr	r1, [sp, #104]
	ldr	r2, [pc, #440]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L126:
	ldr	r3, [sp, #84]
	subs	r3, #214
	cmp	r3, #65
	bhi.n	.L127
	ldr	r2, [pc, #432]
	ldrh	r3, [r2, #4]
	adds	r3, #8
	strh	r3, [r2, #4]
.L127:
	ldr	r3, [sp, #84]
	subs	r3, #246
	cmp	r3, #33
	bhi.n	.L128
	ldr	r5, [sp, #32]
	ldr	r6, [sp, #100]
	movs	r0, #239
	lsls	r0, r0, #7
	adds	r3, r6, r0
	adds	r5, #8
	movs	r1, #0
	str	r5, [sp, #32]
	str	r1, [r3, #0]
	ldr	r3, [pc, #400]
	adds	r2, r6, r3
	movs	r3, #75
	str	r3, [r2, #0]
.L128:
	ldr	r4, [sp, #84]
	cmp	r4, #213
	bgt.n	.L129
	b.n	.L130
.L129:
	add	r3, sp, #124
	movs	r5, #0
	str	r5, [r3, #12]
	str	r5, [r3, #4]
	ldr	r0, [sp, #100]
	movs	r6, #108
	ldr	r1, [pc, #376]
	movs	r2, #225
	add	r6, sp
	lsls	r2, r2, #7
	mov	r8, r6
	movs	r7, #0
	adds	r4, r3, #0
	adds	r6, r0, r1
	adds	r5, r0, r2
.L138:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	beq.n	.L131
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	asrs	r2, r2, #7
	ldr	r0, [pc, #352]
	adds	r3, r3, r2
	str	r3, [sp, #108]
	cmp	r3, r0
	bgt.n	.L132
	movs	r3, #128
	lsls	r3, r3, #4
	str	r3, [sp, #108]
.L132:
	mov	r1, r8
	str	r3, [r1, #4]
	movs	r2, #255
	ldr	r3, [r5, #0]
	lsls	r2, r2, #16
	str	r2, [r4, #4]
	str	r3, [r4, #0]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r4, #8]
	adds	r1, r4, #0
	mov	r2, r8
	movs	r3, #0
	ldr	r0, [r6, #0]
	str	r4, [sp, #8]
	bl	Func_08009008
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #16]
	subs	r2, r2, r3
	ldr	r3, [pc, #300]
	str	r2, [r5, #4]
	ldr	r4, [sp, #8]
	cmp	r2, r3
	bgt.n	.L131
	ldr	r0, [sp, #84]
	ldr	r1, [pc, #248]
	cmp	r0, r1
	bgt.n	.L133
	bl	Func_08004458
	movs	r3, #63
	ldr	r2, [sp, #32]
	ands	r3, r0
	adds	r3, r3, r2
	adds	r3, #32
	b.n	.L134
.L133:
	str	r4, [sp, #8]
	bl	Func_08004458
	movs	r3, #63
	ands	r3, r0
	ldr	r0, [sp, #32]
	adds	r3, r3, r0
	subs	r3, #32
.L134:
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldr	r4, [sp, #8]
	ldr	r1, [sp, #84]
	ldr	r2, [pc, #244]
	cmp	r1, r2
	ble.n	.L135
	movs	r3, #0
	str	r3, [r5, #24]
	b.n	.L131
.L135:
	ldr	r0, [sp, #84]
	cmp	r0, #245
	ble.n	.L136
	movs	r3, #192
	lsls	r3, r3, #15
	b.n	.L137
.L136:
	ldr	r2, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #16
	subs	r3, r3, r2
.L137:
	str	r3, [r5, #4]
.L131:
	adds	r7, #1
	adds	r6, #4
	adds	r5, #28
	cmp	r7, #16
	bne.n	.L138
	ldr	r1, [sp, #84]
	ldr	r2, [pc, #156]
	cmp	r1, r2
	bgt.n	.L139
	ldr	r4, [sp, #32]
	movs	r3, #15
	mov	sl, r3
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	asrs	r1, r1, #31
	asrs	r3, r3, #1
	movs	r7, #0
	mov	r8, r1
	mov	r9, r3
.L141:
	bl	Func_08004458
	movs	r2, #1
	ands	r2, r7
	mov	r5, sl
	ands	r0, r5
	lsls	r3, r2, #2
	lsrs	r5, r7, #31
	adds	r3, r3, r2
	adds	r5, r7, r5
	asrs	r5, r5, #1
	lsls	r3, r3, #2
	lsls	r2, r5, #2
	subs	r3, r3, r0
	adds	r3, r3, r2
	add	r3, r9
	adds	r6, r3, #0
	bl	Func_08004458
	mov	r2, r8
	ldr	r4, [sp, #84]
	lsrs	r3, r2, #31
	adds	r3, r4, r3
	asrs	r3, r3, #1
	mov	r1, sl
	lsls	r5, r5, #5
	ands	r0, r1
	adds	r2, r3, r7
	subs	r6, #16
	subs	r5, r5, r0
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L140
	adds	r3, r2, #3
.L140:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	lsls	r1, r3, #3
	subs	r1, r1, r3
	lsls	r1, r1, #2
	ldr	r0, [sp, #100]
	subs	r1, r1, r3
	lsls	r1, r1, #6
	movs	r3, #32
	movs	r2, #128
	adds	r1, r0, r1
	lsls	r2, r2, #7
	str	r3, [sp, #0]
	movs	r3, #54
	adds	r1, r1, r2
	str	r3, [sp, #4]
	ldr	r0, [sp, #104]
	adds	r2, r6, #0
	adds	r3, r5, #0
	ldr	r4, [sp, #92]
	adds	r7, #1
	bl	Func_080072f4
	cmp	r7, #8
	bne.n	.L141
	b.n	.L130
	movs	r0, r0
	.4byte 0xffffc000
	.4byte 0x03001f0c
	.4byte 0x00000117
	.4byte 0x0000013f
	.4byte 0x00007828
	.4byte 0x3f3f3f3f
	.4byte 0x040000d4
	.4byte 0x85001000
	.4byte 0x03001ad0
	.4byte 0x00007784
	.4byte 0x000077d8
	.4byte 0x000007ff
	.4byte 0x000fffff
	.4byte 0x0000012d
.L139:
	ldr	r0, [sp, #32]
	ldr	r5, [sp, #84]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r5, r5, #31
	movs	r6, #15
	asrs	r3, r3, #1
	movs	r7, #0
	mov	r8, r5
	mov	sl, r6
	mov	r9, r3
.L143:
	bl	Func_08004458
	movs	r2, #1
	ands	r2, r7
	lsls	r3, r2, #2
	lsrs	r5, r7, #31
	adds	r3, r3, r2
	mov	r1, sl
	adds	r5, r7, r5
	ands	r0, r1
	asrs	r5, r5, #1
	lsls	r3, r3, #2
	lsls	r2, r5, #2
	adds	r3, r3, r0
	subs	r3, r3, r2
	add	r3, r9
	adds	r6, r3, #0
	bl	Func_08004458
	mov	r2, sl
	ands	r0, r2
	lsls	r5, r5, #5
	subs	r5, r5, r0
	mov	r4, r8
	ldr	r0, [sp, #84]
	lsrs	r3, r4, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	adds	r2, r3, r7
	subs	r6, #16
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L142
	adds	r3, r2, #3
.L142:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	lsls	r1, r3, #3
	subs	r1, r1, r3
	lsls	r1, r1, #2
	ldr	r2, [sp, #100]
	subs	r1, r1, r3
	lsls	r1, r1, #6
	movs	r3, #128
	adds	r1, r2, r1
	lsls	r3, r3, #7
	adds	r1, r1, r3
	movs	r3, #32
	str	r3, [sp, #0]
	movs	r3, #54
	str	r3, [sp, #4]
	ldr	r0, [sp, #104]
	adds	r2, r6, #0
	adds	r3, r5, #0
	ldr	r4, [sp, #92]
	adds	r7, #1
	bl	Func_080072f4
	cmp	r7, #8
	bne.n	.L143
.L130:
	movs	r0, #47
	bl	Func_08002dd8
	ldr	r5, [sp, #84]
	cmp	r5, #63
	ble.n	.L144
	bl	Func_08004458
	movs	r3, #3
	ands	r3, r0
	ldr	r2, [pc, #92]
	adds	r3, #30
	strh	r3, [r2, #6]
.L144:
	ldr	r6, [sp, #100]
	ldr	r0, [pc, #88]
	movs	r3, #1
	adds	r2, r6, r0
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [sp, #84]
	movs	r2, #160
	adds	r1, #1
	lsls	r2, r2, #1
	str	r1, [sp, #84]
	cmp	r1, r2
	beq.n	.L145
	bl	.L146
.L145:
	ldr	r3, [sp, #100]
	ldr	r4, [pc, #56]
	movs	r7, #0
	adds	r6, r3, r4
.L147:
	ldmia	r6!, {r0}
	adds	r7, #1
	bl	Func_08009038
	cmp	r7, #16
	bne.n	.L147
	ldr	r0, [pc, #44]
	bl	Func_08004278
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #184
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001ad0
	.4byte 0x00007824
	.4byte 0x000077d8
	.4byte 0x080cd261
