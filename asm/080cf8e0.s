@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080cf8e0
	.thumb_func
Func_080cf8e0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #80
	ldr	r2, [pc, #100]
	str	r0, [sp, #56]
	adds	r3, r2, #0
	ldmia	r3!, {r0}
	ldr	r3, [r3, #0]
	str	r3, [sp, #52]
	subs	r2, #108
	ldr	r3, [pc, #92]
	mov	fp, r0
	ldr	r2, [r2, #0]
	mov	r8, r1
	ldr	r1, [sp, #56]
	add	r3, fp
	str	r2, [sp, #36]
	movs	r0, #1
	str	r1, [r3, #0]
	bl	Func_080cd594
	ldr	r3, [pc, #60]
	ldr	r2, [pc, #72]
	ldr	r7, [pc, #72]
	strh	r3, [r2, #0]
	adds	r0, r7, #0
	bl	Func_08002f40
	adds	r6, r0, #0
	movs	r0, #160
	movs	r2, #128
	adds	r1, r6, #0
	ldr	r5, [pc, #60]
	adds	r6, #128
	lsls	r0, r0, #19
	bl	Func_080072f8
	mov	r1, fp
	adds	r0, r6, #0
	bl	Func_08005340
	ldr	r0, [pc, #44]
	bl	Func_08002f40
	adds	r6, r0, #0
	movs	r0, #160
	adds	r1, r6, #0
	movs	r2, #128
	lsls	r0, r0, #19
	bl	Func_080072f8
	b.n	.L0
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000052
	.4byte 0x000000bf
	.4byte 0x03001388
	.4byte 0x0000009e
.L0:
	movs	r1, #250
	adds	r6, #128
	lsls	r1, r1, #6
	add	r1, fp
	adds	r0, r6, #0
	bl	Func_08005340
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L1
	ldr	r0, [pc, #712]
	b.n	.L2
.L1:
	mov	r3, r8
	cmp	r3, #1
	bne.n	.L3
	ldr	r0, [pc, #708]
	b.n	.L2
.L3:
	mov	r4, r8
	cmp	r4, #2
	bne.n	.L4
	ldr	r0, [pc, #700]
	b.n	.L2
.L4:
	mov	r0, r8
	cmp	r0, #3
	beq.n	.L5
	mov	r1, r8
	cmp	r1, #4
	bne.n	.L6
	adds	r0, r7, #0
	b.n	.L2
.L6:
	mov	r2, r8
	cmp	r2, #6
	bne.n	.L5
	ldr	r0, [pc, #680]
	b.n	.L2
.L5:
	ldr	r0, [pc, #680]
.L2:
	bl	Func_08002f40
	adds	r6, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #672]
	adds	r1, r6, #0
	movs	r2, #128
	lsls	r0, r0, #19
	bl	Func_080072f0
	movs	r3, #0
	mov	sl, r3
	movs	r1, #1
	movs	r2, #128
	ldr	r3, [pc, #656]
	negs	r1, r1
	lsls	r2, r2, #3
.L7:
	movs	r4, #1
	add	sl, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	sl, r2
	bne.n	.L7
	ldr	r5, [pc, #644]
	add	r5, fp
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	Func_080b5098
	ldr	r6, [r0, #0]
	movs	r0, #0
	str	r0, [sp, #44]
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	movs	r4, #128
	lsls	r4, r4, #11
	cmp	r3, #0
	beq.n	.L8
	ldr	r5, [pc, #612]
	mov	r9, r0
	add	r5, fp
	movs	r7, #0
.L10:
	ldr	r1, [sp, #44]
	ldr	r2, [r5, #0]
	lsls	r3, r1, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	str	r4, [sp, #8]
	bl	Func_080b5098
	ldr	r3, [pc, #592]
	movs	r2, #0
	ldr	r0, [r0, #0]
	ldr	r4, [sp, #8]
	mov	sl, r2
	adds	r1, r7, r3
.L9:
	ldr	r3, [r6, #8]
	str	r4, [r1, #4]
	str	r3, [r1, #0]
	ldr	r3, [r6, #16]
	str	r3, [r1, #8]
	ldr	r2, [r6, #8]
	ldr	r3, [r0, #8]
	subs	r3, r3, r2
	movs	r2, #128
	asrs	r3, r3, #4
	lsls	r2, r2, #11
	str	r3, [r1, #12]
	str	r2, [r1, #16]
	ldr	r2, [r6, #16]
	ldr	r3, [r0, #16]
	subs	r3, r3, r2
	asrs	r3, r3, #4
	movs	r2, #1
	str	r3, [r1, #20]
	add	sl, r2
	mov	r3, r9
	str	r3, [r1, #24]
	mov	r3, sl
	adds	r1, #28
	cmp	r3, #16
	bne.n	.L9
	ldr	r1, [sp, #44]
	adds	r1, #1
	str	r1, [sp, #44]
	ldr	r3, [r5, #0]
	movs	r0, #224
	ldr	r3, [r3, #20]
	lsls	r0, r0, #1
	adds	r7, r7, r0
	cmp	r1, r3
	bne.n	.L10
.L8:
	movs	r2, #0
	mov	sl, r2
	movs	r1, #1
	movs	r2, #128
	ldr	r3, [pc, #504]
	negs	r1, r1
	lsls	r2, r2, #1
.L11:
	movs	r4, #1
	add	sl, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	sl, r2
	bne.n	.L11
	ldr	r0, [sp, #56]
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L12
	movs	r1, #7
	movs	r3, #3
	movs	r6, #2
	movs	r0, #46
	movs	r2, #7
	str	r6, [sp, #0]
	bl	Func_080ed408
	ldr	r5, [pc, #468]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	mov	r1, r8
	lsls	r1, r1, #1
	str	r3, [sp, #60]
	str	r1, [sp, #28]
	ldr	r3, [pc, #456]
	ldrsb	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L13
	movs	r3, #3
	movs	r2, #7
	movs	r0, #47
	movs	r1, #7
	str	r3, [sp, #0]
	b.n	.L14
.L13:
	movs	r3, #7
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	str	r6, [sp, #0]
	bl	Func_080ed408
	adds	r3, r5, #0
	adds	r3, #188
	ldr	r3, [r3, #0]
	mov	r4, sp
	adds	r4, #60
	str	r4, [sp, #32]
	str	r3, [r4, #4]
	b.n	.L15
.L12:
	movs	r3, #7
	movs	r6, #2
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	str	r6, [sp, #0]
	bl	Func_080ed408
	ldr	r5, [pc, #384]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	mov	r0, r8
	lsls	r0, r0, #1
	str	r3, [sp, #60]
	str	r0, [sp, #28]
	ldr	r3, [pc, #372]
	ldrsb	r3, [r3, r0]
	cmp	r3, #0
	bne.n	.L16
	movs	r3, #3
	movs	r1, #7
	movs	r0, #47
	movs	r2, #7
	str	r3, [sp, #0]
	bl	Func_080ed408
	adds	r3, r5, #0
	adds	r3, #188
	ldr	r3, [r3, #0]
	mov	r1, sp
	adds	r1, #60
	str	r1, [sp, #32]
	str	r3, [r1, #4]
	b.n	.L15
.L16:
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	movs	r1, #7
	str	r6, [sp, #0]
.L14:
	bl	Func_080ed408
	adds	r3, r5, #0
	adds	r3, #188
	ldr	r3, [r3, #0]
	mov	r2, sp
	adds	r2, #60
	str	r2, [sp, #32]
	str	r3, [r2, #4]
.L15:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #300]
	movs	r3, #75
	add	r2, fp
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #296]
	lsls	r1, r1, #3
	bl	Func_080041d8
	ldr	r3, [pc, #280]
	ldr	r4, [sp, #28]
	ldrsb	r3, [r3, r4]
	cmp	r3, #0
	bne.n	.L17
	ldr	r3, [pc, #252]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #3
	adds	r3, #72
	b.n	.L18
.L17:
	ldr	r3, [pc, #240]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #3
	adds	r3, #56
.L18:
	str	r3, [sp, #40]
	movs	r0, #103
	bl	Func_080f9010
	ldr	r1, [sp, #40]
	movs	r0, #0
	str	r0, [sp, #48]
	cmp	r1, #0
	bne.n	.L19
	b.n	.L20
.L19:
	ldr	r2, [sp, #36]
	adds	r2, #12
	str	r2, [sp, #20]
.L45:
	bl	Func_080049ac
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #20]
	bl	Func_080051d8
	movs	r3, #0
	str	r3, [sp, #44]
	ldr	r3, [pc, #184]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L21
	b.n	.L22
.L21:
	ldr	r4, [pc, #176]
	movs	r0, #0
	str	r4, [sp, #16]
	str	r0, [sp, #12]
.L35:
	ldr	r1, [sp, #48]
	ldr	r2, [sp, #12]
	cmp	r1, r2
	bge.n	.L23
	b.n	.L24
.L23:
	adds	r3, r2, #0
	adds	r3, #17
	ldr	r7, [sp, #16]
	cmp	r1, r3
	bne.n	.L25
	ldr	r3, [pc, #144]
	ldr	r4, [sp, #44]
	add	r3, fp
	ldr	r2, [r3, #0]
	lsls	r3, r4, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	adds	r3, r4, #0
	bl	Func_080d6888
	movs	r0, #133
	bl	Func_080b50e8
.L25:
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #24]
	cmp	r3, #0
	bge.n	.L26
	b.n	.L24
.L26:
	ldr	r3, [sp, #48]
	ldr	r4, [sp, #12]
	movs	r1, #3
	subs	r0, r3, r4
	bl	Func_080022ec
	adds	r5, r0, #0
	cmp	r5, #9
	ble.n	.L27
	movs	r5, #9
.L27:
	add	r6, sp, #68
	ldr	r0, [sp, #16]
	adds	r1, r6, #0
	bl	Func_080e3944
	ldr	r3, [r6, #0]
	asrs	r2, r3, #1
	str	r2, [r6, #0]
	cmp	r5, #4
	ble.n	.L28
	lsls	r1, r5, #1
	ldr	r3, [r6, #4]
	movs	r0, #32
	adds	r1, r1, r5
	lsls	r1, r1, #8
	str	r0, [sp, #0]
	movs	r0, #24
	str	r0, [sp, #4]
	add	r1, fp
	subs	r2, #16
	subs	r3, #12
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #52]
	bl	Func_080072f4
	b.n	.L29
	movs	r0, r0
	.4byte 0x0000009f
	.4byte 0x00000059
	.4byte 0x000000a0
	.4byte 0x0000008d
	.4byte 0x00000077
	.4byte 0x03001388
	.4byte 0x02010018
	.4byte 0x00007828
	.4byte 0x02010000
	.4byte 0x02011c18
	.4byte 0x03001e50
	.4byte 0x080ee0b6
	.4byte 0x00007784
	.4byte 0x080cd261
.L28:
	lsls	r1, r5, #1
	ldr	r3, [r6, #4]
	movs	r0, #24
	adds	r1, r1, r5
	lsls	r1, r1, #8
	str	r0, [sp, #0]
	movs	r0, #32
	str	r0, [sp, #4]
	add	r1, fp
	subs	r2, #12
	subs	r3, #16
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #52]
	bl	Func_080072f4
.L29:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	bne.n	.L30
	adds	r0, r7, #0
	movs	r1, #63
	ldr	r2, [pc, #540]
	bl	Func_080e38b8
.L30:
	ldr	r3, [r7, #4]
	cmp	r3, #0
	bge.n	.L24
	movs	r3, #0
	str	r3, [r7, #4]
	movs	r3, #1
	str	r3, [r7, #24]
	ldr	r0, [pc, #524]
	ldr	r1, [sp, #28]
	ldrsb	r3, [r0, r1]
	movs	r2, #4
	mov	r9, r2
	cmp	r3, #0
	beq.n	.L31
	movs	r3, #16
	mov	r9, r3
.L31:
	movs	r4, #0
	mov	r0, r9
	mov	sl, r4
	cmp	r0, #0
	beq.n	.L24
	ldr	r1, [sp, #44]
	lsls	r1, r1, #2
	movs	r2, #63
	str	r1, [sp, #24]
	mov	r8, r2
.L34:
	ldr	r3, [sp, #24]
	lsls	r2, r3, #3
	add	r2, sl
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r4, [pc, #480]
	lsls	r3, r3, #2
	adds	r5, r3, r4
	ldr	r3, [r7, #0]
	str	r3, [r5, #0]
	ldr	r3, [r7, #4]
	str	r3, [r5, #4]
	ldr	r3, [r7, #8]
	str	r3, [r5, #8]
	ldr	r0, [pc, #460]
	ldr	r1, [sp, #28]
	ldrsb	r6, [r0, r1]
	cmp	r6, #0
	bne.n	.L32
	bl	Func_08004458
	mov	r2, r8
	ands	r0, r2
	subs	r0, #32
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	str	r6, [r5, #16]
	bl	Func_08004458
	mov	r3, r8
	ands	r0, r3
	subs	r0, #32
	lsls	r0, r0, #11
	b.n	.L33
.L32:
	bl	Func_08004458
	mov	r4, r8
	ands	r0, r4
	subs	r0, #32
	lsls	r0, r0, #13
	str	r0, [r5, #12]
	bl	Func_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r3, #32
	lsls	r3, r3, #12
	str	r3, [r5, #16]
	bl	Func_08004458
	mov	r1, r8
	ands	r0, r1
	subs	r0, #32
	lsls	r0, r0, #13
.L33:
	str	r0, [r5, #20]
	movs	r2, #1
	movs	r3, #0
	add	sl, r2
	str	r3, [r5, #24]
	cmp	sl, r9
	bne.n	.L34
.L24:
	ldr	r3, [sp, #16]
	movs	r4, #224
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #44]
	lsls	r4, r4, #1
	adds	r3, r3, r4
	str	r3, [sp, #16]
	adds	r0, #8
	adds	r1, #1
	ldr	r3, [pc, #356]
	str	r0, [sp, #12]
	str	r1, [sp, #44]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	cmp	r1, r3
	beq.n	.L22
	b.n	.L35
.L22:
	ldr	r3, [pc, #332]
	ldr	r4, [pc, #328]
	movs	r2, #0
	mov	sl, r2
	mov	r8, r3
	mov	r9, r4
.L44:
	mov	r0, r8
	ldr	r3, [r0, #24]
	cmp	r3, #44
	bhi.n	.L36
	ldr	r3, [r0, #4]
	cmp	r3, #0
	blt.n	.L36
	add	r6, sp, #68
	adds	r1, r6, #0
	bl	Func_080e3944
	ldr	r3, [r6, #0]
	asrs	r2, r3, #1
	str	r2, [r6, #0]
	ldr	r4, [sp, #28]
	mov	r1, r9
	ldrsb	r3, [r1, r4]
	cmp	r3, #0
	bne.n	.L37
	mov	r0, r8
	ldr	r3, [r0, #24]
	cmp	r3, #0
	bge.n	.L38
	adds	r3, #7
.L38:
	asrs	r3, r3, #3
	lsls	r1, r3, #3
	adds	r1, r1, r3
	lsls	r1, r1, #7
	movs	r3, #250
	lsls	r3, r3, #6
	movs	r0, #24
	add	r1, fp
	adds	r1, r1, r3
	ldr	r3, [r6, #4]
	str	r0, [sp, #0]
	movs	r0, #48
	str	r0, [sp, #4]
	ldr	r0, [sp, #32]
	subs	r2, #12
	ldr	r4, [r0, #4]
	subs	r3, #24
	ldr	r0, [sp, #52]
	bl	Func_080072f4
	b.n	.L39
.L37:
	mov	r1, r8
	ldr	r0, [r1, #24]
	movs	r1, #5
	bl	Func_080022ec
	movs	r1, #1
	mov	r3, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L40
	adds	r0, #9
.L40:
	ldr	r3, [sp, #56]
	mov	r4, r8
	ldr	r2, [r3, #4]
	ldr	r3, [r4, #12]
	cmp	r3, #0
	ble.n	.L41
	eors	r2, r1
.L41:
	lsls	r7, r2, #2
	ldr	r2, [pc, #200]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #200]
	movs	r2, #240
	ldrb	r5, [r3, r0]
	lsls	r2, r2, #5
	add	r1, fp
	adds	r1, r1, r2
	ldr	r2, [r6, #0]
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #184]
	ldrb	r4, [r3, r0]
	ldr	r3, [r6, #4]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	ldr	r0, [sp, #32]
	str	r5, [sp, #0]
	str	r4, [sp, #4]
	ldr	r4, [r7, r0]
	ldr	r0, [sp, #52]
	bl	Func_080072f4
.L39:
	ldr	r2, [sp, #28]
	mov	r1, r9
	ldrsb	r3, [r1, r2]
	cmp	r3, #0
	bne.n	.L42
	movs	r2, #128
	mov	r0, r8
	movs	r1, #62
	lsls	r2, r2, #4
	bl	Func_080e38b8
	b.n	.L43
.L42:
	mov	r0, r8
	movs	r1, #62
	ldr	r2, [pc, #108]
	bl	Func_080e38b8
.L43:
	mov	r4, r8
	ldr	r3, [r4, #24]
	adds	r3, #1
	str	r3, [r4, #24]
.L36:
	movs	r1, #1
	movs	r2, #128
	movs	r0, #28
	add	sl, r1
	lsls	r2, r2, #1
	add	r8, r0
	cmp	sl, r2
	bne.n	.L44
	movs	r0, #2
	movs	r1, #2
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r2, [pc, #96]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [sp, #48]
	ldr	r4, [sp, #40]
	adds	r3, #1
	str	r3, [sp, #48]
	cmp	r3, r4
	beq.n	.L20
	b.n	.L45
.L20:
	ldr	r0, [pc, #72]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #80
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0xffff8000
	.4byte 0x080ee0b6
	.4byte 0x02011c00
	.4byte 0x00007828
	.4byte 0x080ee0e8
	.4byte 0x080ee0c4
	.4byte 0x080ee0d6
	.4byte 0x00007824
	.4byte 0x080cd261
