@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080d6970
	.thumb_func
Func_080d6970:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #200]
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #100
	str	r3, [sp, #64]
	ldr	r3, [pc, #192]
	mov	r9, r1
	add	r3, r9
	str	r0, [r3, #0]
	ldr	r1, [pc, #188]
	movs	r0, #8
	movs	r2, #1
	bl	Func_080dbb24
	movs	r2, #0
	mov	sl, r2
	movs	r1, #128
	ldr	r2, [pc, #176]
	movs	r0, #127
	lsls	r1, r1, #3
.L0:
	mov	r3, sl
	ands	r3, r0
	strb	r3, [r2, #0]
	movs	r3, #1
	add	sl, r3
	adds	r2, #1
	cmp	sl, r1
	bne.n	.L0
	movs	r4, #0
	movs	r0, #127
	mov	sl, r4
	mov	r8, r0
	mov	fp, r4
.L2:
	movs	r7, #0
	mov	r6, fp
.L1:
	bl	Func_08004458
	mov	r1, r8
	adds	r5, r0, #0
	ands	r5, r1
	bl	Func_08004458
	mov	r2, r8
	ldr	r3, [pc, #124]
	ands	r0, r2
	adds	r0, r6, r0
	adds	r5, r6, r5
	adds	r0, r0, r3
	adds	r5, r5, r3
	ldrb	r2, [r0, #0]
	ldrb	r3, [r5, #0]
	adds	r7, #1
	strb	r3, [r0, #0]
	strb	r2, [r5, #0]
	cmp	r7, #128
	bne.n	.L1
	movs	r0, #1
	add	sl, r0
	movs	r4, #128
	mov	r1, sl
	add	fp, r4
	cmp	r1, #8
	bne.n	.L2
	movs	r0, #0
	bl	Func_080cd594
	ldr	r2, [pc, #84]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r2, #48
	strh	r3, [r2, #0]
	ldr	r0, [pc, #76]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r6, #1
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r6, [sp, #0]
	bl	Func_080ed408
	ldr	r5, [pc, #52]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r1, #7
	str	r3, [sp, #68]
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	str	r6, [sp, #0]
	b.n	.L3
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x00000177
	.4byte 0x02010000
	.4byte 0x04000020
	.4byte 0x000000b2
	.4byte 0x03001e50
.L3:
	bl	Func_080ed408
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r2, sp
	adds	r2, #68
	str	r2, [sp, #36]
	str	r3, [r2, #4]
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r2, [pc, #752]
	add	r3, r9
	str	r6, [r3, #0]
	add	r2, r9
	movs	r3, #0
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #744]
	lsls	r1, r1, #3
	bl	Func_080041d8
	ldr	r3, [pc, #740]
	ldr	r2, [pc, #740]
	movs	r4, #90
	movs	r5, #225
	str	r3, [r2, #0]
	negs	r4, r4
	movs	r3, #0
	lsls	r5, r5, #7
	mov	sl, r3
	movs	r7, #7
	mov	r8, r4
	movs	r6, #0
	add	r5, r9
.L6:
	mov	r0, sl
	cmp	r0, #4
	bgt.n	.L4
	str	r6, [r5, #0]
	bl	Func_08004458
	ands	r0, r7
	adds	r0, #104
	b.n	.L5
.L4:
	mov	r1, r8
	str	r1, [r5, #0]
	bl	Func_08004458
	ands	r0, r7
	adds	r0, #108
.L5:
	str	r0, [r5, #4]
	bl	Func_08004458
	ands	r0, r7
	adds	r0, #4
	str	r0, [r5, #16]
	bl	Func_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r3, #16
	str	r3, [r5, #24]
	movs	r3, #1
	add	sl, r3
	movs	r2, #20
	mov	r4, sl
	add	r8, r2
	adds	r6, #20
	adds	r5, #28
	cmp	r4, #16
	bne.n	.L6
	ldr	r5, [pc, #648]
	movs	r0, #0
	mov	sl, r0
	add	r5, r9
.L7:
	bl	Func_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	str	r3, [r5, #0]
	bl	Func_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r3, #64
	str	r3, [r5, #4]
	bl	Func_08004458
	movs	r3, #7
	ands	r3, r0
	movs	r1, #1
	negs	r3, r3
	add	sl, r1
	subs	r3, #8
	mov	r2, sl
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #16
	bne.n	.L7
	movs	r5, #232
	movs	r3, #0
	lsls	r5, r5, #7
	mov	sl, r3
	movs	r6, #0
	add	r5, r9
.L8:
	movs	r3, #128
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	movs	r3, #128
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	bl	Func_08004458
	movs	r3, #255
	ands	r3, r0
	adds	r3, #200
	movs	r4, #1
	negs	r3, r3
	add	sl, r4
	lsls	r3, r3, #9
	mov	r0, sl
	str	r3, [r5, #12]
	str	r6, [r5, #16]
	str	r6, [r5, #24]
	adds	r5, #28
	cmp	r0, #16
	bne.n	.L8
	ldr	r5, [pc, #540]
	mov	r1, r9
	ldr	r0, [r1, r5]
	bl	Func_080d6750
	ldr	r2, [pc, #532]
	movs	r3, #0
	str	r2, [sp, #56]
	str	r3, [sp, #52]
	str	r3, [sp, #60]
.L95:
	ldr	r3, [pc, #528]
	ldr	r5, [r3, #0]
	ldr	r3, [pc, #528]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L9
	ldr	r4, [sp, #60]
	cmp	r4, #190
	ble.n	.L9
	ldr	r0, [pc, #512]
	cmp	r4, r0
	bgt.n	.L9
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r3, [pc, #508]
	ldr	r0, [sp, #64]
	bl	Func_080072f0
	movs	r1, #143
	lsls	r1, r1, #1
	str	r1, [sp, #60]
.L9:
	ldr	r2, [sp, #60]
	cmp	r2, #224
	bne.n	.L10
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #0
	str	r3, [r2, #0]
.L10:
	bl	Func_080049ac
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	Func_080051d8
	ldr	r3, [sp, #60]
	cmp	r3, #31
	bne.n	.L11
	ldr	r2, [pc, #460]
	movs	r3, #8
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #157
	bl	Func_080f9010
	ldr	r1, [pc, #424]
	mov	r0, r9
	ldr	r3, [r0, r1]
	ldr	r3, [r3, #20]
	movs	r4, #0
	mov	sl, r4
	cmp	r3, #0
	beq.n	.L11
	ldr	r5, [pc, #408]
	movs	r6, #36
	add	r5, r9
.L12:
	ldr	r3, [r5, #0]
	movs	r1, #6
	ldrsh	r0, [r3, r6]
	bl	Func_080b5088
	movs	r3, #1
	add	sl, r3
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	adds	r6, #2
	cmp	sl, r3
	bne.n	.L12
.L11:
	ldr	r4, [sp, #60]
	cmp	r4, #72
	bne.n	.L13
	movs	r0, #136
	bl	Func_080f9010
.L13:
	ldr	r0, [sp, #60]
	cmp	r0, #140
	bne.n	.L14
	movs	r0, #156
	bl	Func_080f9010
.L14:
	ldr	r1, [sp, #52]
	movs	r2, #128
	ldr	r3, [sp, #56]
	lsls	r2, r2, #7
	adds	r1, r1, r2
	movs	r4, #128
	adds	r3, r3, r1
	lsls	r4, r4, #15
	str	r1, [sp, #52]
	str	r3, [sp, #56]
	cmp	r3, r4
	ble.n	.L15
	str	r4, [sp, #56]
.L15:
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #16
	ldr	r2, [sp, #56]
	bl	Func_080e6d3c
	ldr	r0, [sp, #60]
	subs	r0, #48
	cmp	r0, #48
	bhi.n	.L16
	movs	r1, #24
	bl	Func_080022ec
	movs	r1, #3
	bl	Func_080022fc
	ldr	r3, [pc, #324]
	ldr	r6, [pc, #324]
	adds	r5, r0, #0
	add	r3, r9
	lsls	r5, r5, #1
	ldrb	r1, [r6, r5]
	ldr	r0, [r3, #0]
	bl	Func_08009020
	ldr	r3, [pc, #312]
	adds	r5, #1
	add	r3, r9
	ldr	r0, [r3, #0]
	ldrb	r1, [r6, r5]
	bl	Func_08009020
.L16:
	ldr	r3, [sp, #60]
	subs	r3, #72
	cmp	r3, #55
	bhi.n	.L17
	movs	r6, #232
	movs	r0, #0
	lsls	r6, r6, #7
	mov	sl, r0
	add	r6, r9
.L20:
	mov	r3, sl
	ldr	r1, [sp, #60]
	adds	r3, #72
	cmp	r1, r3
	blt.n	.L18
	ldr	r5, [r6, #4]
	ldr	r2, [pc, #272]
	cmp	r5, r2
	bgt.n	.L18
	adds	r0, r1, #0
	add	r0, sl
	cmp	r0, #0
	bge.n	.L19
	adds	r0, #3
.L19:
	movs	r1, #5
	asrs	r0, r0, #2
	bl	Func_080022fc
	ldr	r4, [pc, #252]
	lsls	r1, r0, #1
	ldrh	r1, [r4, r1]
	ldr	r4, [pc, #252]
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	asrs	r3, r5, #16
	ldrb	r5, [r4, r0]
	lsrs	r4, r5, #1
	subs	r2, r2, r4
	ldr	r4, [pc, #240]
	ldrb	r4, [r4, r0]
	lsrs	r0, r4, #1
	add	r1, r9
	subs	r3, r3, r0
	str	r4, [sp, #4]
	str	r5, [sp, #0]
	ldr	r4, [sp, #68]
	ldr	r0, [sp, #64]
	bl	Func_080072f4
	movs	r2, #128
	adds	r0, r6, #0
	movs	r1, #64
	lsls	r2, r2, #5
	bl	Func_080e3908
.L18:
	movs	r4, #1
	add	sl, r4
	mov	r0, sl
	adds	r6, #28
	cmp	r0, #16
	bne.n	.L20
.L17:
	ldr	r1, [sp, #60]
	cmp	r1, #128
	bne.n	.L21
	ldr	r5, [pc, #132]
	movs	r2, #0
	mov	sl, r2
	movs	r6, #255
	add	r5, r9
.L22:
	bl	Func_08004458
	movs	r1, #96
	bl	Func_08002304
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	Func_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #88
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	Func_08004458
	ands	r0, r6
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	Func_08004458
	ands	r0, r6
	negs	r0, r0
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	Func_08004458
	movs	r3, #15
	ands	r3, r0
	negs	r3, r3
	subs	r3, #16
	str	r3, [r5, #24]
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	adds	r5, #28
	cmp	r4, #48
	bne.n	.L22
	ldr	r2, [pc, #40]
	movs	r3, #0
	str	r3, [r2, #0]
.L21:
	ldr	r0, [sp, #60]
	subs	r0, #128
	str	r0, [sp, #48]
	cmp	r0, #96
	bls.n	.L23
	b.n	.L24
.L23:
	str	r0, [sp, #44]
	cmp	r0, #80
	ble.n	.L25
	movs	r1, #80
	str	r1, [sp, #44]
	b.n	.L26
	movs	r0, r0
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0xffffe000
	.4byte 0x04000028
	.4byte 0x00007240
	.4byte 0x00007828
	.4byte 0xffc00000
	.4byte 0x03001e80
	.4byte 0x03001b04
	.4byte 0x0000011d
	.4byte 0x03000164
	.4byte 0x000077a8
	.4byte 0x000077e4
	.4byte 0x080ee910
	.4byte 0x000077e8
	.4byte 0x0067ffff
	.4byte 0x080ee916
	.4byte 0x080ee920
	.4byte 0x080ee925
.L25:
	ldr	r2, [pc, #556]
	movs	r3, #2
	add	r2, r9
	str	r3, [r2, #0]
.L26:
	movs	r7, #225
	movs	r2, #0
	lsls	r7, r7, #7
	mov	sl, r2
	add	r7, r9
.L38:
	ldr	r3, [r7, #24]
	ldr	r4, [sp, #44]
	cmp	r4, r3
	bgt.n	.L27
	b.n	.L28
.L27:
	mov	r0, sl
	mov	fp, r9
	cmp	r0, #5
	ble.n	.L29
	movs	r1, #216
	lsls	r1, r1, #3
	add	r1, r9
	mov	fp, r1
.L29:
	ldr	r4, [sp, #44]
	subs	r2, r4, r3
	ldr	r3, [r7, #16]
	adds	r0, r3, #0
	muls	r0, r2
	str	r0, [sp, #40]
	adds	r6, r0, #0
	cmp	r0, #184
	ble.n	.L30
.L31:
	subs	r6, #64
	cmp	r6, #184
	bgt.n	.L31
.L30:
	cmp	r6, #119
	bgt.n	.L32
	movs	r1, #1
	mov	r0, sl
	ands	r0, r1
	movs	r1, #24
	ldr	r3, [r7, #4]
	ldr	r2, [r7, #0]
	str	r1, [sp, #0]
	movs	r1, #8
	str	r1, [sp, #4]
	ldr	r1, [sp, #36]
	lsls	r0, r0, #2
	subs	r3, r3, r6
	ldr	r4, [r0, r1]
	subs	r3, #8
	ldr	r0, [sp, #64]
	mov	r1, fp
	bl	Func_080072f4
.L32:
	mov	r3, sl
	movs	r4, #1
	ands	r3, r4
	lsls	r3, r3, #2
	movs	r2, #0
	str	r3, [sp, #32]
	mov	r8, r2
.L36:
	ldr	r1, [r7, #4]
	mov	r0, r8
	lsls	r3, r0, #6
	subs	r2, r1, r6
	adds	r5, r2, r3
	movs	r3, #64
	negs	r3, r3
	movs	r2, #0
	movs	r0, #64
	cmp	r5, r3
	blt.n	.L33
	cmp	r5, #0
	bge.n	.L34
	negs	r2, r5
	lsls	r3, r2, #1
	adds	r0, r5, #0
	adds	r3, r3, r2
	lsls	r2, r3, #3
	adds	r0, #64
	movs	r5, #0
.L34:
	adds	r3, r5, r0
	cmp	r3, r1
	ble.n	.L35
	subs	r3, r3, r1
	subs	r0, r0, r3
.L35:
	mov	r4, fp
	movs	r3, #24
	adds	r1, r4, r2
	ldr	r2, [r7, #0]
	str	r3, [sp, #0]
	str	r0, [sp, #4]
	ldr	r3, [sp, #36]
	ldr	r0, [sp, #32]
	adds	r1, #192
	ldr	r4, [r0, r3]
	ldr	r0, [sp, #64]
	adds	r3, r5, #0
	bl	Func_080072f4
.L33:
	movs	r4, #1
	add	r8, r4
	mov	r0, r8
	cmp	r0, #3
	bne.n	.L36
	mov	r6, sl
	ands	r6, r4
	cmp	r6, #0
	beq.n	.L28
	ldr	r1, [sp, #40]
	ldr	r5, [r7, #4]
	movs	r2, #127
	subs	r3, r5, r1
	ands	r3, r2
	subs	r3, #16
	movs	r1, #3
	mov	r0, sl
	mov	r8, r3
	bl	Func_080022fc
	ldr	r3, [pc, #320]
	adds	r1, r0, #0
	ldrb	r4, [r3, r1]
	mov	r2, r8
	adds	r3, r2, r4
	mov	ip, r4
	cmp	r3, r5
	ble.n	.L37
	subs	r3, r3, r5
	subs	r4, r4, r3
.L37:
	cmp	r4, #0
	ble.n	.L28
	ldr	r2, [pc, #300]
	lsls	r3, r1, #1
	ldrh	r1, [r2, r3]
	mov	r3, ip
	ldr	r2, [r7, #0]
	str	r3, [sp, #0]
	str	r4, [sp, #4]
	ldr	r3, [sp, #36]
	lsls	r0, r6, #2
	ldr	r4, [r0, r3]
	add	r1, r9
	adds	r2, #8
	ldr	r0, [sp, #64]
	mov	r3, r8
	bl	Func_080072f4
.L28:
	movs	r4, #1
	add	sl, r4
	mov	r0, sl
	adds	r7, #28
	cmp	r0, #10
	beq.n	.L24
	b.n	.L38
.L24:
	ldr	r1, [sp, #48]
	cmp	r1, #95
	bhi.n	.L39
	movs	r5, #232
	movs	r2, #0
	lsls	r5, r5, #7
	mov	sl, r2
	movs	r6, #255
	add	r5, r9
.L42:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L40
	movs	r1, #5
	mov	r0, sl
	bl	Func_080022fc
	ldr	r2, [pc, #228]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	ldr	r4, [pc, #216]
	ldrb	r4, [r4, r0]
	str	r4, [sp, #0]
	ldr	r4, [pc, #216]
	ldrb	r0, [r4, r0]
	add	r1, r9
	str	r0, [sp, #4]
	ldr	r4, [sp, #68]
	ldr	r0, [sp, #64]
	bl	Func_080072f4
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #16]
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r1, #240
	adds	r2, r2, r3
	lsls	r1, r1, #15
	adds	r3, r3, r0
	str	r2, [r5, #4]
	str	r3, [r5, #16]
	cmp	r2, r1
	bls.n	.L41
	ldr	r2, [sp, #60]
	cmp	r2, #159
	bgt.n	.L41
	bl	Func_08004458
	movs	r1, #96
	bl	Func_08002304
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	Func_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #88
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	Func_08004458
	ands	r0, r6
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	Func_08004458
	ands	r0, r6
	negs	r0, r0
	lsls	r0, r0, #11
	str	r0, [r5, #16]
.L41:
	ldr	r3, [r5, #24]
.L40:
	adds	r3, #1
	str	r3, [r5, #24]
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	adds	r5, #28
	cmp	r4, #32
	bne.n	.L42
.L39:
	ldr	r3, [sp, #60]
	subs	r3, #224
	cmp	r3, #23
	bhi.n	.L43
	ldr	r0, [sp, #60]
	movs	r3, #3
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L43
	ldr	r2, [pc, #48]
	movs	r4, #160
	movs	r1, #0
	lsls	r4, r4, #19
	mov	sl, r1
	mov	r8, r2
.L50:
	ldrh	r3, [r4, #0]
	movs	r7, #31
	ands	r7, r3
	lsls	r3, r3, #16
	mov	r0, r8
	lsrs	r6, r3, #21
	lsrs	r5, r3, #26
	ands	r6, r0
	ands	r5, r0
	adds	r0, r7, r6
	adds	r0, r0, r5
	movs	r1, #3
	str	r4, [sp, #8]
	bl	Func_080022ec
	ldr	r4, [sp, #8]
	cmp	r7, r0
	ble.n	.L44
	subs	r7, #1
	b.n	.L44
	.4byte 0x0000001f
	.4byte 0x000077a8
	.4byte 0x080ee930
	.4byte 0x080ee92a
	.4byte 0x080ee934
	.4byte 0x080ee93e
	.4byte 0x080ee943
.L44:
	cmp	r7, r0
	bge.n	.L45
	adds	r7, #1
.L45:
	cmp	r6, r0
	ble.n	.L46
	subs	r6, #1
.L46:
	cmp	r6, r0
	bge.n	.L47
	adds	r6, #1
.L47:
	cmp	r5, r0
	ble.n	.L48
	subs	r5, #1
.L48:
	cmp	r5, r0
	bge.n	.L49
	adds	r5, #1
.L49:
	lsls	r2, r6, #5
	lsls	r3, r5, #10
	movs	r1, #1
	orrs	r3, r2
	add	sl, r1
	orrs	r3, r7
	mov	r2, sl
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r2, #64
	bne.n	.L50
.L43:
	ldr	r3, [sp, #48]
	cmp	r3, #172
	bhi.n	.L51
	ldr	r2, [pc, #500]
	mov	r0, r9
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	movs	r4, #0
	mov	sl, r4
	cmp	r3, #0
	beq.n	.L51
	add	r1, sp, #76
	mov	r8, r1
	add	r6, sp, #88
	mov	fp, r4
.L56:
	mov	r4, sl
	mov	r3, r9
	ldr	r2, [r3, r2]
	lsls	r3, r4, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	bl	Func_080b5098
	ldr	r2, [r0, #0]
	ldr	r3, [r2, #8]
	str	r3, [r6, #0]
	ldr	r3, [r2, #12]
	str	r3, [r6, #4]
	ldr	r3, [r2, #16]
	adds	r0, r6, #0
	str	r3, [r6, #8]
	mov	r1, r8
	bl	Func_080e3944
	mov	r2, fp
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	ldr	r4, [pc, #432]
	add	r3, r9
	movs	r7, #0
	adds	r5, r3, r4
.L55:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	bne.n	.L52
	bl	Func_08004458
	movs	r1, #15
	ldr	r3, [sp, #76]
	ands	r0, r1
	adds	r3, r3, r0
	subs	r3, #8
	str	r3, [r5, #0]
	bl	Func_08004458
	movs	r2, #15
	ldr	r3, [sp, #80]
	ands	r0, r2
	adds	r3, r3, r0
	subs	r3, #40
	str	r3, [r5, #4]
	ldr	r0, [r5, #24]
.L52:
	cmp	r0, #4
	bhi.n	.L53
	ldr	r2, [pc, #384]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #384]
	ldrb	r0, [r3, r0]
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	lsrs	r4, r0, #1
	subs	r2, r2, r4
	subs	r3, r3, r4
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	add	r1, r9
	ldr	r4, [sp, #68]
	ldr	r0, [sp, #64]
	bl	Func_080072f4
	ldr	r0, [r5, #24]
.L53:
	adds	r3, r0, #1
	str	r3, [r5, #24]
	ldr	r4, [sp, #60]
	cmp	r4, #199
	bgt.n	.L54
	cmp	r3, #5
	bne.n	.L54
	bl	Func_08004458
	movs	r3, #7
	ands	r3, r0
	negs	r3, r3
	str	r3, [r5, #24]
.L54:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #6
	bne.n	.L55
	ldr	r2, [pc, #308]
	mov	r4, r9
	ldr	r3, [r4, r2]
	movs	r1, #1
	ldr	r3, [r3, #20]
	movs	r0, #6
	add	sl, r1
	add	fp, r0
	cmp	sl, r3
	bne.n	.L56
.L51:
	ldr	r0, [sp, #60]
	cmp	r0, #232
	ble.n	.L57
	ldr	r1, [pc, #296]
	lsls	r3, r0, #1
	adds	r6, r3, r1
	movs	r2, #0
	movs	r3, #0
	mov	sl, r2
	mov	ip, r3
	adds	r4, r6, #0
.L62:
	movs	r7, #0
.L61:
	cmp	r4, #127
	bhi.n	.L58
	movs	r5, #7
	adds	r0, r4, #0
	ands	r0, r5
	lsls	r3, r0, #5
	add	r3, sl
	ldr	r1, [pc, #268]
	lsls	r3, r3, #2
	adds	r3, r3, r7
	adds	r3, r3, r1
	ldrb	r1, [r3, #0]
	adds	r3, r4, #0
	cmp	r4, #0
	bge.n	.L59
	adds	r3, r4, #7
.L59:
	asrs	r3, r3, #3
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L60
	adds	r2, r1, #7
.L60:
	asrs	r2, r2, #3
	lsls	r3, r3, #4
	adds	r3, r3, r2
	lsls	r3, r3, #3
	adds	r3, r3, r0
	ldr	r2, [sp, #64]
	ands	r1, r5
	lsls	r3, r3, #3
	adds	r3, r3, r1
	adds	r3, r2, r3
	mov	r0, ip
	strb	r0, [r3, #0]
.L58:
	adds	r7, #1
	cmp	r7, #4
	bne.n	.L61
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	adds	r4, #1
	cmp	r2, #32
	bne.n	.L62
	movs	r4, #0
	movs	r3, #0
	mov	lr, r4
	mov	sl, r3
	adds	r4, r6, #1
.L67:
	movs	r7, #0
	mov	ip, r4
.L66:
	cmp	r4, #127
	bhi.n	.L63
	movs	r5, #7
	mov	r0, ip
	ands	r0, r5
	lsls	r3, r0, #5
	add	r3, sl
	ldr	r1, [pc, #172]
	lsls	r3, r3, #2
	adds	r3, r3, r7
	adds	r3, r3, r1
	ldrb	r1, [r3, #0]
	mov	r3, ip
	cmp	r3, #0
	bge.n	.L64
	adds	r3, #7
.L64:
	asrs	r3, r3, #3
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L65
	adds	r2, r1, #7
.L65:
	asrs	r2, r2, #3
	lsls	r3, r3, #4
	adds	r3, r3, r2
	lsls	r3, r3, #3
	adds	r3, r3, r0
	ldr	r2, [sp, #64]
	ands	r1, r5
	lsls	r3, r3, #3
	adds	r3, r3, r1
	adds	r3, r2, r3
	mov	r0, lr
	strb	r0, [r3, #0]
.L63:
	adds	r7, #1
	cmp	r7, #4
	bne.n	.L66
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	adds	r4, #1
	cmp	r2, #32
	bne.n	.L67
.L57:
	ldr	r3, [sp, #60]
	subs	r3, #161
	cmp	r3, #62
	bhi.n	.L68
	ldr	r2, [pc, #72]
	movs	r3, #0
	mov	r4, r9
	mov	sl, r3
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L68
.L73:
	mov	r0, sl
	lsls	r3, r0, #3
	ldr	r1, [sp, #60]
	adds	r3, #160
	cmp	r1, r3
	ble.n	.L69
	mov	r3, r9
	ldr	r2, [r3, r2]
	lsls	r3, r0, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	bl	Func_080b5098
	adds	r6, r0, #0
	ldr	r2, [r6, #0]
	movs	r0, #128
	ldr	r3, [r2, #12]
	lsls	r0, r0, #12
	movs	r1, #128
	adds	r3, r3, r0
	lsls	r1, r1, #16
	str	r3, [r2, #12]
	cmp	r3, r1
	ble.n	.L70
	str	r1, [r2, #12]
.L70:
	movs	r3, #0
	movs	r5, #0
	str	r3, [r2, #72]
	b.n	.L71
	movs	r0, r0
	.4byte 0x00007828
	.4byte 0x00007240
	.4byte 0x080ee948
	.4byte 0x080ee952
	.4byte 0xfffffe10
	.4byte 0x02010000
.L72:
	movs	r1, #5
	bl	Func_08009020
	adds	r5, #1
.L71:
	ldr	r0, [r6, #0]
	adds	r1, r5, #0
	bl	Func_080b50d8
	cmp	r0, #0
	bne.n	.L72
.L69:
	ldr	r2, [pc, #296]
	mov	r4, r9
	ldr	r3, [r4, r2]
	movs	r1, #1
	ldr	r3, [r3, #20]
	add	sl, r1
	cmp	sl, r3
	bne.n	.L73
.L68:
	ldr	r2, [pc, #280]
	mov	r1, r9
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	movs	r0, #0
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L74
	movs	r5, #143
	movs	r7, #8
	movs	r6, #36
	lsls	r5, r5, #1
.L77:
	ldr	r3, [sp, #60]
	cmp	r3, r5
	bne.n	.L75
	mov	r4, r9
	ldr	r3, [r4, r2]
	ldrsh	r0, [r3, r6]
	bl	Func_080b5098
	movs	r3, #192
	ldr	r2, [r0, #0]
	lsls	r3, r3, #15
	str	r3, [r2, #12]
	ldr	r3, [pc, #236]
	str	r3, [r2, #72]
.L75:
	adds	r3, r5, #0
	ldr	r2, [sp, #60]
	adds	r3, #16
	cmp	r2, r3
	bne.n	.L76
	ldr	r3, [pc, #220]
	add	r3, r9
	ldr	r3, [r3, #0]
	movs	r2, #1
	ldrsh	r0, [r3, r6]
	movs	r1, #7
	mov	r3, sl
	negs	r2, r2
	str	r7, [sp, #0]
	bl	Func_080d6888
	movs	r0, #134
	bl	Func_080f9010
	ldr	r3, [pc, #200]
	add	r3, r9
	str	r7, [r3, #0]
.L76:
	ldr	r2, [pc, #184]
	mov	r1, r9
	ldr	r3, [r1, r2]
	movs	r0, #1
	ldr	r3, [r3, #20]
	add	sl, r0
	adds	r6, #2
	adds	r5, #5
	cmp	sl, r3
	bne.n	.L77
.L74:
	movs	r3, #151
	ldr	r2, [sp, #60]
	lsls	r3, r3, #1
	cmp	r2, r3
	beq.n	.L78
	b.n	.L79
.L78:
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	ldr	r0, [pc, #152]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
	movs	r1, #180
	lsls	r1, r1, #5
	ldr	r0, [pc, #140]
	add	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r6, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r6, [sp, #0]
	bl	Func_080ed408
	ldr	r5, [pc, #116]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r1, #7
	str	r3, [sp, #68]
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	str	r6, [sp, #0]
	bl	Func_080ed408
	adds	r5, #188
	ldr	r4, [sp, #36]
	ldr	r3, [r5, #0]
	ldr	r2, [pc, #92]
	str	r3, [r4, #4]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	subs	r2, #48
	strh	r3, [r2, #0]
	adds	r2, #8
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r2, [pc, #72]
	add	r3, r9
	str	r6, [r3, #0]
	add	r2, r9
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #60]
	bl	Func_080041d8
	ldr	r2, [pc, #20]
	mov	r1, r9
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	movs	r0, #0
	mov	sl, r0
	b.n	.L80
	movs	r0, r0
	.4byte 0x00003f46
	.4byte 0x00000080
	.4byte 0x00007828
	.4byte 0x0000ab85
	.4byte 0x000077a8
	.4byte 0x00000098
	.4byte 0x000000c0
	.4byte 0x03001e50
	.4byte 0x04000050
	.4byte 0x00007784
	.4byte 0x080cd261
.L80:
	cmp	r3, #0
	beq.n	.L79
.L82:
	mov	r4, sl
	mov	r3, r9
	ldr	r2, [r3, r2]
	lsls	r3, r4, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	bl	Func_080b5098
	mov	r3, sl
	lsls	r2, r3, #2
	add	r2, sl
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r0, [r0, #0]
	lsls	r3, r3, #3
	movs	r4, #225
	add	r3, r9
	lsls	r4, r4, #7
	mov	r8, r0
	movs	r7, #0
	adds	r6, r3, r4
.L81:
	mov	r0, r8
	ldr	r3, [r0, #8]
	str	r3, [r6, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r6, #4]
	ldr	r3, [r0, #16]
	str	r3, [r6, #8]
	ldr	r3, [pc, #584]
	adds	r5, r7, #0
	muls	r5, r3
	adds	r0, r5, #0
	bl	Func_08002322
	lsls	r0, r0, #2
	str	r0, [r6, #12]
	bl	Func_08004458
	ldr	r3, [pc, #568]
	movs	r1, #128
	ands	r3, r0
	lsls	r1, r1, #9
	adds	r3, r3, r1
	str	r3, [r6, #16]
	adds	r0, r5, #0
	bl	Func_0800231c
	adds	r7, #1
	lsls	r0, r0, #2
	movs	r3, #0
	str	r0, [r6, #20]
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r7, #10
	bne.n	.L81
	movs	r2, #1
	add	sl, r2
	ldr	r2, [pc, #532]
	mov	r4, r9
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	cmp	sl, r3
	bne.n	.L82
.L79:
	ldr	r0, [sp, #60]
	ldr	r1, [pc, #524]
	cmp	r0, r1
	bgt.n	.L83
	b.n	.L84
.L83:
	movs	r2, #0
	mov	sl, r2
	ldr	r2, [pc, #508]
	mov	r4, r9
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L85
	b.n	.L84
.L85:
	ldr	r1, [sp, #60]
	ldr	r3, [pc, #500]
	mov	r0, sp
	adds	r0, #76
	adds	r3, r1, r3
	movs	r4, #151
	mov	r1, sp
	str	r0, [sp, #24]
	lsls	r4, r4, #1
	movs	r0, #0
	adds	r1, #88
	str	r3, [sp, #20]
	str	r4, [sp, #16]
	str	r0, [sp, #12]
	str	r1, [sp, #28]
.L90:
	ldr	r3, [sp, #60]
	ldr	r4, [sp, #16]
	cmp	r3, r4
	blt.n	.L86
	ldr	r0, [sp, #12]
	movs	r1, #157
	lsls	r1, r1, #1
	ldr	r4, [sp, #60]
	adds	r3, r0, r1
	cmp	r4, r3
	bge.n	.L86
	ldr	r0, [sp, #20]
	mov	r1, r9
	mov	r4, sl
	ldr	r2, [r1, r2]
	lsls	r3, r4, #1
	adds	r3, #36
	lsrs	r6, r0, #31
	adds	r6, r0, r6
	ldrsh	r0, [r2, r3]
	bl	Func_080b5098
	ldr	r2, [r0, #0]
	ldr	r4, [sp, #28]
	ldr	r3, [r2, #8]
	str	r3, [r4, #0]
	movs	r3, #0
	str	r3, [r4, #4]
	ldr	r3, [r2, #16]
	str	r3, [r4, #8]
	ldr	r0, [sp, #24]
	mov	r8, r0
	mov	r1, r8
	ldr	r0, [sp, #28]
	bl	Func_080e3944
	asrs	r6, r6, #1
	mov	r1, r8
	ldr	r2, [r1, #0]
	lsls	r5, r6, #4
	subs	r5, r5, r6
	ldr	r3, [r1, #4]
	asrs	r2, r2, #1
	lsls	r5, r5, #5
	str	r2, [r1, #0]
	add	r5, r9
	movs	r4, #20
	movs	r0, #24
	subs	r2, #20
	subs	r3, #24
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #68]
	adds	r1, r5, #0
	ldr	r0, [sp, #64]
	bl	Func_080072f4
	mov	r1, r8
	movs	r4, #20
	movs	r0, #24
	ldr	r3, [r1, #4]
	ldr	r2, [r1, #0]
	str	r0, [sp, #4]
	ldr	r1, [sp, #36]
	str	r4, [sp, #0]
	subs	r3, #24
	ldr	r4, [r1, #4]
	ldr	r0, [sp, #64]
	adds	r1, r5, #0
	bl	Func_080072f4
.L86:
	ldr	r3, [sp, #16]
	ldr	r2, [sp, #60]
	adds	r3, #6
	cmp	r2, r3
	blt.n	.L87
	ldr	r2, [sp, #12]
	add	r3, sp, #76
	add	r2, sl
	mov	r8, r3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #3
	movs	r4, #225
	add	r3, r9
	lsls	r4, r4, #7
	movs	r0, #12
	movs	r7, #0
	mov	r6, r8
	adds	r5, r3, r4
	mov	fp, r0
.L89:
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	Func_080e3944
	ldr	r3, [r6, #0]
	asrs	r2, r3, #1
	str	r2, [r6, #0]
	ldr	r3, [r5, #24]
	cmp	r3, #26
	bhi.n	.L88
	ldr	r3, [pc, #272]
	mov	r4, fp
	ldrh	r1, [r3, r4]
	ldr	r3, [pc, #272]
	ldrh	r4, [r3, r4]
	mov	r3, r8
	ldr	r3, [r3, #4]
	lsrs	r0, r4, #1
	subs	r2, r2, r0
	subs	r3, r3, r0
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	add	r1, r9
	ldr	r4, [sp, #68]
	ldr	r0, [sp, #64]
	bl	Func_080072f4
.L88:
	movs	r2, #128
	adds	r0, r5, #0
	movs	r1, #60
	lsls	r2, r2, #5
	bl	Func_080e3908
	ldr	r3, [r5, #24]
	adds	r7, #1
	adds	r3, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #5
	bne.n	.L89
.L87:
	ldr	r4, [sp, #20]
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #12]
	subs	r4, #4
	movs	r2, #1
	adds	r0, #4
	adds	r1, #4
	str	r4, [sp, #20]
	str	r0, [sp, #16]
	str	r1, [sp, #12]
	add	sl, r2
	ldr	r2, [pc, #180]
	mov	r4, r9
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	cmp	sl, r3
	beq.n	.L84
	b.n	.L90
.L84:
	ldr	r0, [sp, #60]
	cmp	r0, #127
	bgt.n	.L91
	movs	r0, #4
	movs	r1, #16
	bl	Func_080e155c
	b.n	.L92
.L91:
	ldr	r1, [sp, #60]
	ldr	r2, [pc, #152]
	cmp	r1, r2
	bgt.n	.L93
	movs	r0, #2
	movs	r1, #2
	bl	Func_080e155c
	b.n	.L92
.L93:
	movs	r0, #4
	movs	r1, #8
	bl	Func_080e155c
.L92:
	bl	Func_080cd52c
	ldr	r2, [pc, #140]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [sp, #60]
	movs	r4, #183
	adds	r3, #1
	lsls	r4, r4, #1
	str	r3, [sp, #60]
	cmp	r3, r4
	beq.n	.L94
	bl	.L95
.L94:
	ldr	r0, [pc, #112]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	movs	r0, #134
	bl	Func_080b50e8
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #16
	ldr	r2, [sp, #56]
	ldr	r5, [pc, #84]
	bl	Func_080e6eac
	movs	r0, #0
	mov	sl, r0
	add	r5, r9
.L96:
	ldmia	r5!, {r0}
	bl	Func_08009038
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #8
	bne.n	.L96
	bl	Func_080cdbc0
	add	sp, #100
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00003334
	.4byte 0x00007fff
	.4byte 0x00007828
	.4byte 0x0000012d
	.4byte 0xfffffed2
	.4byte 0x080ee958
	.4byte 0x080ee966
	.4byte 0x00007824
	.4byte 0x080cd261
	.4byte 0x000077d8
