@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080e99c0
	.thumb_func
Func_080e99c0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #96]
	adds	r3, r6, #0
	ldmia	r3!, {r1}
	sub	sp, #52
	str	r1, [sp, #36]
	ldr	r3, [r3, #0]
	str	r3, [sp, #32]
	ldr	r3, [pc, #84]
	ldr	r2, [r6, #8]
	adds	r5, r1, r3
	str	r2, [sp, #20]
	str	r0, [r5, #0]
	movs	r0, #1
	bl	Func_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #60]
	strh	r3, [r2, #0]
	ldr	r3, [r5, #0]
	add	r5, sp, #40
	movs	r4, #36
	ldrsh	r0, [r3, r4]
	adds	r1, r5, #0
	bl	Func_080e3980
	ldr	r3, [r5, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [sp, #16]
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	Func_080ed408
	ldr	r0, [r6, #28]
	movs	r3, #1
	str	r0, [sp, #24]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	b.n	.L0
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000052
.L0:
	bl	Func_080ed408
	ldr	r3, [pc, #500]
	ldr	r2, [sp, #36]
	ldr	r6, [r6, #32]
	adds	r1, r2, r3
	ldr	r0, [pc, #496]
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #28]
	bl	Func_080e0524
	ldr	r0, [pc, #488]
	ldr	r1, [sp, #36]
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
	movs	r6, #221
	ldr	r4, [sp, #36]
	lsls	r6, r6, #4
	adds	r1, r4, r6
	ldr	r0, [pc, #472]
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
	ldr	r0, [pc, #468]
	ldr	r1, [sp, #20]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	movs	r1, #239
	ldr	r0, [sp, #36]
	lsls	r1, r1, #7
	adds	r3, r0, r1
	str	r5, [r3, #0]
	ldr	r3, [pc, #448]
	movs	r1, #144
	adds	r2, r0, r3
	movs	r3, #75
	str	r3, [r2, #0]
	ldr	r0, [pc, #444]
	lsls	r1, r1, #3
	bl	Func_080041d8
	movs	r6, #225
	ldr	r5, [sp, #36]
	movs	r4, #0
	lsls	r6, r6, #7
	mov	r8, r4
	adds	r7, r5, r6
.L1:
	mov	r0, r8
	lsls	r6, r0, #1
	bl	Func_08004458
	ldr	r3, [pc, #420]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	Func_08002322
	adds	r3, r6, #0
	muls	r3, r0
	adds	r0, r5, #0
	str	r3, [r7, #0]
	bl	Func_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	mov	r1, r8
	negs	r3, r3
	str	r3, [r7, #4]
	lsrs	r3, r1, #31
	add	r3, r8
	asrs	r3, r3, #1
	movs	r2, #1
	adds	r3, #25
	add	r8, r2
	str	r3, [r7, #24]
	mov	r3, r8
	adds	r7, #28
	cmp	r3, #32
	bne.n	.L1
	movs	r4, #0
	movs	r1, #1
	movs	r2, #171
	ldr	r3, [pc, #360]
	mov	r8, r4
	negs	r1, r1
	lsls	r2, r2, #2
.L2:
	movs	r5, #1
	add	r8, r5
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L2
	ldr	r0, [sp, #16]
	lsls	r0, r0, #16
	movs	r6, #0
	str	r0, [sp, #8]
	ldr	r7, [pc, #336]
	mov	r8, r6
.L3:
	bl	Func_08004458
	ldr	r6, [pc, #332]
	ands	r6, r0
	bl	Func_08004458
	ldr	r3, [pc, #312]
	adds	r5, r0, #0
	ldr	r1, [sp, #8]
	ands	r5, r3
	movs	r3, #176
	lsls	r3, r3, #15
	str	r1, [r7, #0]
	str	r3, [r7, #4]
	adds	r0, r5, #0
	bl	Func_08002322
	adds	r6, #32
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	Func_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	negs	r3, r3
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	Func_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #32
	str	r3, [r7, #24]
	movs	r2, #1
	movs	r3, #170
	add	r8, r2
	lsls	r3, r3, #1
	adds	r7, #28
	cmp	r8, r3
	bne.n	.L3
	movs	r4, #0
	mov	fp, r4
.L52:
	mov	r3, fp
	subs	r3, #25
	cmp	r3, #22
	bhi.n	.L4
	ldr	r0, [pc, #244]
	bl	Func_080e46f0
.L4:
	mov	r5, fp
	cmp	r5, #56
	ble.n	.L5
	ldr	r0, [pc, #236]
	bl	Func_080e46f0
.L5:
	mov	r6, fp
	cmp	r6, #8
	bne.n	.L6
	ldr	r0, [sp, #36]
	ldr	r1, [pc, #224]
	adds	r3, r0, r1
	str	r6, [r3, #0]
.L6:
	mov	r2, fp
	cmp	r2, #48
	bne.n	.L7
	ldr	r3, [sp, #36]
	ldr	r4, [pc, #212]
	adds	r2, r3, r4
	movs	r3, #8
	str	r3, [r2, #0]
.L7:
	mov	r5, fp
	cmp	r5, #60
	bne.n	.L8
	ldr	r6, [sp, #36]
	ldr	r0, [pc, #196]
	movs	r3, #16
	adds	r2, r6, r0
	str	r3, [r2, #0]
.L8:
	mov	r1, fp
	cmp	r1, #4
	bne.n	.L9
	movs	r0, #212
	bl	Func_080f9010
.L9:
	mov	r2, fp
	cmp	r2, #32
	bne.n	.L10
	movs	r0, #164
	bl	Func_080f9010
.L10:
	mov	r3, fp
	cmp	r3, #60
	bne.n	.L11
	movs	r0, #145
	bl	Func_080f9010
	movs	r0, #134
	bl	Func_080b50e8
.L11:
	mov	r4, fp
	cmp	r4, #55
	ble.n	.L12
	ldr	r6, [pc, #144]
	ldr	r0, [sp, #36]
	movs	r1, #225
	movs	r5, #0
	lsls	r1, r1, #7
	mov	r8, r5
	mov	sl, r6
	adds	r5, r0, r1
.L16:
	ldr	r0, [sp, #16]
	movs	r4, #2
	ldrsh	r3, [r5, r4]
	adds	r6, r3, r0
	ldr	r0, [r5, #24]
	movs	r2, #6
	ldrsh	r7, [r5, r2]
	cmp	r0, #17
	bhi.n	.L13
	movs	r1, #3
	bl	Func_080022ec
	mov	r2, sl
	ldrb	r1, [r2, r0]
	ldr	r3, [sp, #36]
	movs	r0, #32
	lsls	r1, r1, #11
	movs	r4, #221
	adds	r1, r3, r1
	lsls	r4, r4, #4
	adds	r2, r6, #0
	str	r0, [sp, #0]
	adds	r3, r7, #0
	movs	r0, #64
	str	r0, [sp, #4]
	adds	r1, r1, r4
	subs	r2, #16
	adds	r3, #48
	ldr	r0, [sp, #32]
	ldr	r6, [sp, #24]
	bl	Func_080072fc
	ldr	r0, [r5, #24]
.L13:
	cmp	r0, #0
	ble.n	.L14
	subs	r3, r0, #1
	b.n	.L15
	.4byte 0x00004e20
	.4byte 0x00000056
	.4byte 0x00000085
	.4byte 0x0000007d
	.4byte 0x00000073
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x0000ffff
	.4byte 0x02010018
	.4byte 0x02014ad0
	.4byte 0x000001ff
	.4byte 0x000000c0
	.4byte 0x000000c4
	.4byte 0x000077a8
	.4byte 0x080eef12
.L14:
	movs	r3, #1
	negs	r3, r3
.L15:
	str	r3, [r5, #24]
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #16
	bne.n	.L16
.L12:
	mov	r2, fp
	cmp	r2, #28
	bne.n	.L17
	movs	r3, #0
	movs	r4, #63
	ldr	r7, [pc, #760]
	mov	r8, r3
	mov	sl, r4
.L19:
	movs	r5, #1
	ldr	r3, [r7, #24]
	negs	r5, r5
	cmp	r3, r5
	bne.n	.L18
	bl	Func_08004458
	adds	r6, r0, #0
	mov	r0, sl
	ands	r6, r0
	bl	Func_08004458
	ldr	r3, [pc, #736]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	Func_08002322
	adds	r3, r6, #0
	muls	r3, r0
	ldr	r1, [sp, #8]
	asrs	r3, r3, #3
	adds	r3, r3, r1
	str	r3, [r7, #0]
	adds	r0, r5, #0
	bl	Func_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	movs	r2, #192
	lsls	r2, r2, #15
	asrs	r3, r3, #2
	adds	r3, r3, r2
	str	r3, [r7, #4]
	bl	Func_08004458
	mov	r3, sl
	ands	r0, r3
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r7, #12]
	bl	Func_08004458
	mov	r4, sl
	ands	r0, r4
	negs	r0, r0
	subs	r0, #8
	lsls	r0, r0, #13
	movs	r3, #0
	str	r0, [r7, #16]
	str	r3, [r7, #24]
.L18:
	movs	r5, #1
	movs	r6, #128
	add	r8, r5
	lsls	r6, r6, #1
	adds	r7, #28
	cmp	r8, r6
	bne.n	.L19
.L17:
	mov	r0, fp
	subs	r0, #32
	str	r0, [sp, #12]
	cmp	r0, #31
	bhi.n	.L20
	movs	r1, #0
	movs	r2, #63
	ldr	r7, [pc, #624]
	mov	r9, r1
	mov	r8, r1
	mov	sl, r2
.L23:
	movs	r4, #1
	ldr	r3, [r7, #24]
	negs	r4, r4
	cmp	r3, r4
	bne.n	.L21
	bl	Func_08004458
	adds	r6, r0, #0
	bl	Func_08004458
	mov	r5, sl
	ldr	r3, [pc, #600]
	ands	r6, r5
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	Func_08002322
	adds	r3, r6, #0
	muls	r3, r0
	ldr	r0, [sp, #8]
	asrs	r3, r3, #3
	adds	r3, r3, r0
	str	r3, [r7, #0]
	adds	r0, r5, #0
	bl	Func_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	movs	r1, #192
	lsls	r1, r1, #15
	asrs	r3, r3, #2
	adds	r3, r3, r1
	str	r3, [r7, #4]
	bl	Func_08004458
	mov	r2, sl
	ands	r0, r2
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r7, #12]
	bl	Func_08004458
	mov	r3, sl
	ands	r0, r3
	negs	r0, r0
	movs	r4, #1
	subs	r0, #8
	add	r9, r4
	lsls	r0, r0, #13
	movs	r3, #0
	mov	r5, r9
	str	r0, [r7, #16]
	str	r3, [r7, #24]
	cmp	r5, #16
	beq.n	.L22
.L21:
	movs	r6, #1
	movs	r0, #171
	add	r8, r6
	lsls	r0, r0, #2
	adds	r7, #28
	cmp	r8, r0
	bne.n	.L23
.L22:
	ldr	r1, [sp, #12]
	cmp	r1, #31
	bhi.n	.L20
	mov	r2, fp
	ldr	r3, [sp, #16]
	ldr	r1, [pc, #488]
	lsls	r0, r2, #4
	movs	r4, #34
	subs	r3, #17
	adds	r0, r0, r1
	movs	r1, #104
	mov	sl, r3
	mov	r8, r4
	ldr	r5, [sp, #24]
	bl	Func_080022fc
	mov	r9, r5
	mov	r2, r8
	adds	r5, r0, #0
	movs	r6, #104
	movs	r3, #4
	subs	r3, r3, r5
	str	r2, [sp, #0]
	ldr	r1, [sp, #36]
	mov	r2, sl
	str	r6, [sp, #4]
	ldr	r0, [sp, #32]
	bl	Func_08007308
	movs	r3, #108
	mov	r4, r8
	subs	r3, r3, r5
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #36]
	mov	r2, sl
	bl	Func_08007308
.L20:
	mov	r5, fp
	cmp	r5, #71
	bgt.n	.L24
	movs	r6, #0
	ldr	r5, [pc, #404]
	mov	r8, r6
.L36:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L25
	mov	r0, r8
	movs	r1, #3
	bl	Func_080022fc
	ldr	r3, [r5, #16]
	adds	r4, r0, #2
	cmp	r3, #0
	ble.n	.L26
	adds	r4, #2
.L26:
	mov	r0, fp
	cmp	r0, #68
	ble.n	.L27
	cmp	r4, #5
	bgt.n	.L27
	movs	r4, #6
.L27:
	mov	r1, fp
	cmp	r1, #70
	ble.n	.L28
	cmp	r4, #6
	bgt.n	.L28
	movs	r4, #7
.L28:
	mov	r2, fp
	cmp	r2, #72
	ble.n	.L29
	cmp	r4, #7
	bgt.n	.L29
	movs	r4, #8
.L29:
	mov	r3, fp
	cmp	r3, #74
	ble.n	.L30
	cmp	r4, #8
	bgt.n	.L30
	movs	r4, #9
.L30:
	mov	r6, fp
	cmp	r6, #76
	ble.n	.L31
	movs	r4, #10
.L31:
	lsls	r0, r4, #1
	ldr	r2, [pc, #332]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #20]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	str	r4, [sp, #0]
	subs	r3, r3, r4
	str	r0, [sp, #4]
	ldr	r4, [sp, #24]
	ldr	r0, [sp, #32]
	bl	Func_080072f4
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r1, [r5, #16]
	ldr	r3, [r5, #4]
	mov	r6, fp
	adds	r3, r3, r1
	str	r3, [r5, #4]
	cmp	r6, #80
	ble.n	.L32
	ldr	r0, [pc, #272]
	adds	r3, r1, r0
	b.n	.L33
.L32:
	movs	r2, #3
	mov	r4, r8
	ldr	r3, [pc, #268]
	ands	r2, r4
	lsls	r2, r2, #2
	ldr	r3, [r3, r2]
	adds	r3, r1, r3
.L33:
	str	r3, [r5, #16]
	ldr	r2, [r5, #12]
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r3, r3, #1
	cmp	r3, #0
	bge.n	.L34
	adds	r3, #63
.L34:
	ldr	r2, [r5, #16]
	asrs	r3, r3, #6
	str	r3, [r5, #12]
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r2, r3, #1
	cmp	r2, #0
	bge.n	.L35
	adds	r2, #63
.L35:
	ldr	r3, [r5, #24]
	asrs	r2, r2, #6
	adds	r3, #1
	str	r2, [r5, #16]
	str	r3, [r5, #24]
	cmp	r2, #0
	ble.n	.L25
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	cmp	r3, #108
	ble.n	.L25
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L25:
	movs	r0, #1
	movs	r1, #171
	add	r8, r0
	lsls	r1, r1, #1
	adds	r5, #28
	cmp	r8, r1
	bne.n	.L36
.L24:
	mov	r2, fp
	cmp	r2, #95
	bgt.n	.L37
	ldr	r2, [sp, #16]
	mov	r3, fp
	subs	r2, #18
	movs	r1, #120
	cmp	r3, #60
	ble.n	.L38
	ldr	r5, [pc, #168]
	lsls	r3, r3, #3
	adds	r4, r3, r5
	b.n	.L39
.L38:
	mov	r6, fp
	cmp	r6, #32
	ble.n	.L40
	ldr	r0, [sp, #12]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	adds	r4, r3, #0
	adds	r4, #16
	b.n	.L39
.L40:
	mov	r3, fp
	cmp	r3, #9
	bgt.n	.L41
	lsls	r3, r3, #4
	adds	r4, r3, #0
	subs	r4, #128
	b.n	.L39
.L41:
	movs	r4, #16
.L39:
	adds	r3, r4, #0
	adds	r3, #120
	cmp	r3, #108
	ble.n	.L42
	subs	r3, r1, r4
	adds	r1, r3, #0
	subs	r1, #12
.L42:
	cmp	r1, #0
	ble.n	.L37
	ldr	r5, [sp, #36]
	ldr	r6, [pc, #108]
	movs	r3, #36
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	adds	r3, r4, #0
	ldr	r0, [sp, #32]
	adds	r1, r5, r6
	ldr	r4, [sp, #28]
	bl	Func_080072f4
.L37:
	mov	r5, fp
	cmp	r5, #59
	ble.n	.L43
	movs	r6, #0
	ldr	r7, [pc, #84]
	mov	r8, r6
.L46:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	ble.n	.L44
	movs	r2, #128
	adds	r0, r7, #0
	movs	r1, #64
	lsls	r2, r2, #6
	bl	Func_080e3908
	ldr	r3, [r7, #24]
	movs	r1, #216
	ldr	r6, [r7, #4]
	subs	r0, r3, #1
	lsls	r1, r1, #15
	str	r0, [r7, #24]
	cmp	r6, r1
	ble.n	.L45
	ldr	r3, [r7, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r7, #16]
	b.n	.L44
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0xffffff00
	.4byte 0x080ede48
	.4byte 0xffff8000
	.4byte 0x080eef18
	.4byte 0xfffffe3e
	.4byte 0x00004e20
	.4byte 0x02014ad0
.L45:
	ldr	r5, [r7, #0]
	ldr	r2, [pc, #264]
	cmp	r5, r2
	bhi.n	.L44
	cmp	r6, #0
	blt.n	.L44
	movs	r1, #5
	bl	Func_080022ec
	adds	r0, #1
	lsls	r4, r0, #1
	ldr	r2, [pc, #248]
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #20]
	adds	r1, r3, r1
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	asrs	r5, r5, #16
	asrs	r6, r6, #16
	subs	r5, r5, r3
	subs	r6, r6, r0
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #32]
	adds	r2, r5, #0
	adds	r3, r6, #0
	ldr	r4, [sp, #24]
	bl	Func_080072f4
.L44:
	movs	r5, #1
	movs	r6, #170
	add	r8, r5
	lsls	r6, r6, #1
	adds	r7, #28
	cmp	r8, r6
	bne.n	.L46
.L43:
	mov	r0, fp
	cmp	r0, #68
	bne.n	.L47
	ldr	r3, [pc, #192]
	ldr	r2, [sp, #36]
	ldr	r3, [r2, r3]
	ldr	r3, [r3, #20]
	movs	r1, #0
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L47
	ldr	r3, [pc, #176]
	movs	r6, #36
	adds	r5, r2, r3
.L48:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r3, r8
	bl	Func_080d6888
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r1, #7
	bl	Func_080b5088
	ldr	r3, [r5, #0]
	movs	r2, #1
	ldr	r3, [r3, #20]
	add	r8, r2
	adds	r6, #2
	cmp	r8, r3
	bne.n	.L48
.L47:
	mov	r3, fp
	cmp	r3, #9
	bne.n	.L49
	movs	r1, #128
	ldr	r3, [pc, #124]
	ldr	r0, [sp, #32]
	lsls	r1, r1, #7
	ldr	r2, [pc, #120]
	bl	Func_080072f0
.L49:
	mov	r4, fp
	cmp	r4, #60
	bne.n	.L50
	movs	r1, #128
	ldr	r3, [pc, #104]
	ldr	r0, [sp, #32]
	lsls	r1, r1, #7
	ldr	r2, [pc, #100]
	bl	Func_080072f0
.L50:
	movs	r1, #16
	movs	r0, #16
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r6, [pc, #88]
	ldr	r5, [sp, #36]
	movs	r3, #1
	adds	r2, r5, r6
	movs	r0, #1
	str	r3, [r2, #0]
	bl	Func_080030f8
	movs	r0, #1
	add	fp, r0
	mov	r1, fp
	cmp	r1, #102
	beq.n	.L51
	b.n	.L52
.L51:
	ldr	r0, [pc, #64]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x007effff
	.4byte 0x080ede48
	.4byte 0x00007828
	.4byte 0x03000168
	.4byte 0x3f3f3f3f
	.4byte 0x00007824
	.4byte 0x080cd261
