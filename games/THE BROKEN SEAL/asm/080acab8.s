@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080acab8
	.thumb_func
Func_080acab8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #172
	mov	r9, r0
	adds	r0, r3, #0
	ldr	r5, [sp, #204]
	ldr	r6, [sp, #208]
	str	r1, [sp, #64]
	str	r2, [sp, #60]
	str	r3, [sp, #56]
	bl	Func_08077008
	ldr	r3, [pc, #828]
	str	r0, [sp, #52]
	movs	r1, #149
	ldr	r4, [r3, #0]
	lsls	r1, r1, #2
	adds	r0, r4, #2
	adds	r3, r5, r1
	ldrb	r2, [r0, r3]
	str	r2, [sp, #44]
	ldrb	r3, [r4, r3]
	str	r3, [sp, #40]
	movs	r3, #188
	lsls	r3, r3, #1
	mov	ip, r3
	lsls	r5, r5, #1
	add	r5, ip
	movs	r2, #128
	mov	lr, r0
	lsls	r2, r2, #8
	ldrh	r0, [r4, r5]
	adds	r3, r2, #0
	ands	r3, r0
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	str	r3, [sp, #36]
	adds	r1, r6, r1
	mov	r0, lr
	ldrb	r0, [r0, r1]
	str	r0, [sp, #32]
	ldrb	r1, [r4, r1]
	lsls	r6, r6, #1
	str	r1, [sp, #28]
	add	r6, ip
	ldrh	r3, [r4, r6]
	movs	r5, #166
	ands	r2, r3
	lsls	r5, r5, #1
	lsls	r2, r2, #16
	lsrs	r2, r2, #16
	adds	r0, r5, #0
	str	r2, [sp, #24]
	bl	Func_08004938
	ldr	r3, [pc, #744]
	ldr	r1, [sp, #52]
	str	r0, [sp, #48]
	adds	r2, r5, #0
	bl	Func_080072f0
	ldr	r1, [sp, #216]
	cmp	r1, #0
	beq.n	.L0
	b.n	.L1
.L0:
	ldr	r2, [sp, #212]
	cmp	r2, #3
	bne.n	.L2
	ldr	r1, [sp, #52]
	ldr	r2, [sp, #64]
	movs	r3, #52
	ldrsh	r0, [r1, r3]
	lsls	r2, r2, #3
	ldr	r3, [sp, #60]
	mov	fp, r2
	lsls	r7, r3, #3
	mov	r5, fp
	movs	r1, #56
	adds	r5, #80
	adds	r1, r1, r7
	str	r1, [sp, #0]
	mov	sl, r1
	mov	r2, r9
	adds	r3, r5, #0
	movs	r1, #3
	bl	Func_080150b0
	movs	r1, #64
	ldr	r3, [sp, #52]
	adds	r1, r1, r7
	movs	r2, #54
	ldrsh	r0, [r3, r2]
	mov	r8, r1
	str	r1, [sp, #0]
	mov	r2, r9
	adds	r3, r5, #0
	movs	r1, #3
	bl	Func_080150b0
	ldr	r3, [sp, #52]
	subs	r5, #32
	mov	r1, sl
	movs	r2, #56
	ldrsh	r0, [r3, r2]
	str	r1, [sp, #0]
	mov	r2, r9
	adds	r3, r5, #0
	movs	r1, #3
	bl	Func_080150b0
	ldr	r3, [sp, #52]
	mov	r1, r8
	movs	r2, #58
	ldrsh	r0, [r3, r2]
	str	r1, [sp, #0]
	adds	r3, r5, #0
	movs	r1, #3
	mov	r2, r9
	bl	Func_080150b0
	mov	r6, fp
	ldr	r5, [pc, #616]
	adds	r6, #72
	adds	r0, r5, #0
	mov	r1, r9
	adds	r2, r6, #0
	mov	r3, sl
	bl	Func_08015090
	adds	r0, r5, #0
	mov	r1, r9
	adds	r2, r6, #0
	mov	r3, r8
	bl	Func_08015090
	b.n	.L3
.L2:
	ldr	r3, [sp, #52]
	ldr	r1, [sp, #64]
	movs	r2, #56
	ldrsh	r0, [r3, r2]
	ldr	r2, [sp, #60]
	lsls	r1, r1, #3
	lsls	r7, r2, #3
	mov	fp, r1
	mov	r5, fp
	adds	r3, r7, #0
	adds	r5, #48
	adds	r3, #56
	str	r3, [sp, #0]
	movs	r1, #3
	adds	r3, r5, #0
	mov	r2, r9
	bl	Func_080150b0
	ldr	r1, [sp, #52]
	movs	r3, #58
	ldrsh	r0, [r1, r3]
	adds	r3, r7, #0
	adds	r3, #64
	str	r3, [sp, #0]
	movs	r1, #3
	mov	r2, r9
	adds	r3, r5, #0
	bl	Func_080150b0
.L3:
	ldr	r2, [sp, #52]
	mov	r5, fp
	adds	r3, r7, #0
	adds	r5, #48
	adds	r3, #72
	ldrh	r0, [r2, #60]
	movs	r1, #3
	str	r3, [sp, #0]
	mov	r2, r9
	adds	r3, r5, #0
	bl	Func_080150b0
	ldr	r3, [sp, #52]
	ldrh	r0, [r3, #62]
	adds	r3, r7, #0
	adds	r3, #80
	str	r3, [sp, #0]
	movs	r1, #3
	mov	r2, r9
	adds	r3, r5, #0
	bl	Func_080150b0
	ldr	r3, [sp, #52]
	adds	r3, #64
	ldrh	r0, [r3, #0]
	adds	r3, r7, #0
	adds	r3, #88
	str	r3, [sp, #0]
	movs	r1, #3
	mov	r2, r9
	adds	r3, r5, #0
	bl	Func_080150b0
	ldr	r3, [sp, #52]
	adds	r2, r7, #0
	adds	r3, #66
	ldrb	r0, [r3, #0]
	adds	r2, #96
	mov	r3, fp
	str	r2, [sp, #0]
	adds	r3, #56
	movs	r1, #2
	mov	r2, r9
	bl	Func_080150b0
.L1:
	ldr	r0, [sp, #212]
	cmp	r0, #1
	beq.n	.L4
	cmp	r0, #1
	bgt.n	.L5
	cmp	r0, #0
	beq.n	.L6
	b.n	.L7
.L5:
	ldr	r1, [sp, #212]
	cmp	r1, #2
	beq.n	.L8
	cmp	r1, #4
	beq.n	.L9
	b.n	.L7
.L6:
	ldr	r2, [sp, #28]
	movs	r5, #31
	ands	r5, r2
	ldr	r1, [sp, #32]
	adds	r2, r5, #0
	ldr	r0, [sp, #56]
	bl	Func_080771a8
	adds	r2, r5, #0
	ldr	r0, [sp, #56]
	ldr	r1, [sp, #32]
	bl	Func_080771b0
	b.n	.L7
.L4:
	ldr	r0, [sp, #40]
	movs	r3, #31
	ands	r0, r3
	str	r0, [sp, #40]
	ldr	r1, [sp, #44]
	ldr	r0, [sp, #56]
	ldr	r2, [sp, #40]
	bl	Func_080771b8
	b.n	.L7
.L8:
	ldr	r1, [sp, #36]
	cmp	r1, #0
	beq.n	.L10
	ldr	r2, [sp, #40]
	movs	r3, #31
	ands	r2, r3
	ldr	r0, [sp, #56]
	ldr	r1, [sp, #44]
	str	r2, [sp, #40]
	bl	Func_080771b8
.L10:
	ldr	r3, [sp, #28]
	movs	r5, #31
	ands	r5, r3
	ldr	r0, [sp, #56]
	ldr	r1, [sp, #32]
	adds	r2, r5, #0
	bl	Func_080771a8
	ldr	r0, [sp, #24]
	cmp	r0, #0
	beq.n	.L7
	ldr	r0, [sp, #56]
	ldr	r1, [sp, #32]
	adds	r2, r5, #0
	bl	Func_080771b0
	b.n	.L7
.L9:
	ldr	r1, [sp, #28]
	movs	r5, #31
	ands	r5, r1
	adds	r2, r5, #0
	ldr	r0, [sp, #56]
	ldr	r1, [sp, #32]
	bl	Func_080771a8
	ldr	r2, [sp, #24]
	cmp	r2, #0
	beq.n	.L7
	ldr	r0, [sp, #56]
	ldr	r1, [sp, #32]
	adds	r2, r5, #0
	bl	Func_080771b0
.L7:
	ldr	r0, [sp, #56]
	bl	Func_08077010
	ldr	r0, [sp, #56]
	bl	Func_08077008
	ldr	r3, [sp, #216]
	str	r0, [sp, #52]
	cmp	r3, #0
	bne.n	.L11
	ldr	r0, [sp, #64]
	ldr	r1, [sp, #60]
	lsls	r0, r0, #3
	mov	fp, r0
	lsls	r7, r1, #3
	mov	r6, fp
	adds	r6, #40
	adds	r5, r7, #0
	ldr	r0, [sp, #52]
	adds	r5, #16
	mov	r1, r9
	adds	r2, r6, #0
	adds	r3, r7, #0
	bl	Func_08015090
	adds	r3, r5, #0
	ldr	r0, [pc, #228]
	mov	r1, r9
	adds	r2, r6, #0
	bl	Func_08015090
	ldr	r2, [sp, #52]
	mov	r3, fp
	ldrb	r0, [r2, #15]
	adds	r3, #88
	movs	r1, #2
	mov	r2, r9
	str	r5, [sp, #0]
	bl	Func_080150b0
	ldr	r5, [pc, #204]
	adds	r3, r7, #0
	adds	r0, r5, #0
	adds	r3, #56
	mov	r1, r9
	mov	r2, fp
	bl	Func_08015080
	adds	r3, r7, #0
	adds	r0, r5, #1
	adds	r3, #64
	mov	r1, r9
	mov	r2, fp
	bl	Func_08015080
	adds	r3, r7, #0
	adds	r0, r5, #2
	adds	r3, #72
	mov	r1, r9
	mov	r2, fp
	bl	Func_08015080
	adds	r3, r7, #0
	adds	r0, r5, #3
	adds	r3, #80
	mov	r1, r9
	mov	r2, fp
	bl	Func_08015080
	adds	r3, r7, #0
	adds	r0, r5, #4
	adds	r3, #88
	mov	r1, r9
	mov	r2, fp
	bl	Func_08015080
	adds	r5, #5
	adds	r3, r7, #0
	adds	r3, #96
	adds	r0, r5, #0
	mov	r1, r9
	mov	r2, fp
	bl	Func_08015080
	ldr	r1, [pc, #120]
	ldr	r0, [sp, #48]
	adds	r3, r0, r1
	ldrb	r0, [r3, #0]
	ldr	r3, [pc, #116]
	adds	r0, r0, r3
	adds	r3, r7, #0
	adds	r3, #32
	mov	r1, r9
	mov	r2, fp
	bl	Func_08015080
.L11:
	ldr	r2, [sp, #216]
	cmp	r2, #0
	beq.n	.L12
	b.n	.L13
.L12:
	ldr	r3, [pc, #88]
	ldr	r0, [sp, #48]
	ldr	r1, [sp, #52]
	adds	r6, r0, r3
	adds	r5, r1, r3
	ldrb	r1, [r6, #0]
	ldrb	r3, [r5, #0]
	mov	ip, r1
	cmp	ip, r3
	beq.n	.L14
	ldr	r1, [sp, #60]
	ldr	r3, [pc, #68]
	ldrb	r0, [r5, #0]
	ldr	r2, [sp, #64]
	adds	r0, r0, r3
	lsls	r3, r1, #3
	lsls	r2, r2, #3
	adds	r3, #48
	mov	r1, r9
	mov	fp, r2
	bl	Func_08015080
	ldr	r3, [sp, #216]
	ldr	r2, [sp, #64]
	ldr	r1, [pc, #48]
	str	r3, [sp, #0]
	adds	r2, #2
	movs	r3, #5
	mov	r0, r9
	bl	Func_08015280
	ldrb	r1, [r6, #0]
	ldrb	r3, [r5, #0]
	b.n	.L15
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x03001388
	.4byte 0x080af290
	.4byte 0x080af28c
	.4byte 0x000008ae
	.4byte 0x00000129
	.4byte 0x00000741
	.4byte 0x0000f296
.L14:
	ldr	r0, [sp, #64]
	lsls	r0, r0, #3
	mov	fp, r0
.L15:
	mov	ip, r1
	ldr	r2, [sp, #64]
	cmp	ip, r3
	beq.n	.L16
	adds	r2, #5
.L16:
	ldr	r1, [sp, #60]
	ldr	r3, [sp, #52]
	movs	r0, #142
	movs	r4, #0
	adds	r1, #5
	lsls	r0, r0, #1
	mov	r8, r1
	mov	sl, r4
	adds	r7, r2, #1
	adds	r6, r3, r0
	adds	r5, r2, #0
.L17:
	ldr	r2, [pc, #680]
	mov	r3, sl
	adds	r1, r4, r2
	str	r3, [sp, #0]
	adds	r2, r5, #0
	mov	r0, r9
	mov	r3, r8
	str	r4, [sp, #8]
	bl	Func_08015280
	ldrb	r1, [r6, #0]
	ldr	r0, [pc, #660]
	mov	r2, sl
	adds	r1, r1, r0
	str	r2, [sp, #0]
	mov	r0, r9
	adds	r2, r7, #0
	mov	r3, r8
	bl	Func_08015280
	ldr	r4, [sp, #8]
	adds	r4, #1
	adds	r6, #1
	adds	r7, #2
	adds	r5, #2
	cmp	r4, #3
	ble.n	.L17
	ldr	r2, [sp, #52]
	movs	r3, #70
	movs	r1, #56
	ldrsh	r0, [r2, r1]
	ldr	r2, [sp, #48]
	add	r3, fp
	mov	r8, r3
	movs	r1, #56
	ldrsh	r3, [r2, r1]
	cmp	r0, r3
	beq.n	.L18
	ldr	r1, [sp, #60]
	lsls	r2, r1, #3
	adds	r5, r2, #0
	mov	r3, fp
	adds	r3, #72
	movs	r1, #4
	mov	r2, r9
	adds	r5, #56
	str	r5, [sp, #0]
	bl	Func_080150b0
	ldr	r0, [sp, #52]
	movs	r3, #56
	ldrsh	r2, [r0, r3]
	ldr	r0, [sp, #48]
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	cmp	r2, r3
	ble.n	.L19
	mov	r0, r9
	mov	r1, r8
	adds	r2, r5, #0
	movs	r3, #0
	bl	Func_080ae9f0
	b.n	.L18
.L19:
	mov	r0, r9
	mov	r1, r8
	adds	r2, r5, #0
	movs	r3, #1
	bl	Func_080ae9f0
.L18:
	ldr	r2, [sp, #52]
	movs	r1, #58
	ldrsh	r0, [r2, r1]
	ldr	r2, [sp, #48]
	movs	r1, #58
	ldrsh	r3, [r2, r1]
	cmp	r0, r3
	beq.n	.L20
	ldr	r1, [sp, #60]
	lsls	r2, r1, #3
	adds	r5, r2, #0
	mov	r3, fp
	adds	r3, #72
	movs	r1, #4
	mov	r2, r9
	adds	r5, #64
	str	r5, [sp, #0]
	bl	Func_080150b0
	ldr	r0, [sp, #52]
	movs	r3, #58
	ldrsh	r2, [r0, r3]
	ldr	r0, [sp, #48]
	movs	r1, #58
	ldrsh	r3, [r0, r1]
	cmp	r2, r3
	ble.n	.L21
	mov	r0, r9
	mov	r1, r8
	adds	r2, r5, #0
	movs	r3, #0
	bl	Func_080ae9f0
	b.n	.L20
.L21:
	mov	r0, r9
	mov	r1, r8
	adds	r2, r5, #0
	movs	r3, #1
	bl	Func_080ae9f0
.L20:
	ldr	r1, [sp, #52]
	ldr	r0, [sp, #48]
	ldrh	r2, [r1, #60]
	ldrh	r3, [r0, #60]
	cmp	r2, r3
	beq.n	.L22
	ldr	r1, [sp, #60]
	adds	r0, r2, #0
	lsls	r2, r1, #3
	adds	r5, r2, #0
	mov	r3, fp
	adds	r3, #72
	mov	r2, r9
	adds	r5, #72
	movs	r1, #4
	str	r5, [sp, #0]
	bl	Func_080150b0
	ldr	r3, [sp, #52]
	ldr	r0, [sp, #48]
	ldrh	r2, [r3, #60]
	ldrh	r3, [r0, #60]
	cmp	r2, r3
	bls.n	.L23
	mov	r0, r9
	mov	r1, r8
	adds	r2, r5, #0
	movs	r3, #0
	bl	Func_080ae9f0
	b.n	.L22
.L23:
	mov	r0, r9
	mov	r1, r8
	adds	r2, r5, #0
	movs	r3, #1
	bl	Func_080ae9f0
.L22:
	ldr	r1, [sp, #52]
	ldr	r0, [sp, #48]
	ldrh	r2, [r1, #62]
	ldrh	r3, [r0, #62]
	cmp	r2, r3
	beq.n	.L24
	ldr	r1, [sp, #60]
	adds	r0, r2, #0
	lsls	r2, r1, #3
	adds	r5, r2, #0
	mov	r3, fp
	adds	r3, #72
	mov	r2, r9
	adds	r5, #80
	movs	r1, #4
	str	r5, [sp, #0]
	bl	Func_080150b0
	ldr	r3, [sp, #52]
	ldr	r0, [sp, #48]
	ldrh	r2, [r3, #62]
	ldrh	r3, [r0, #62]
	cmp	r2, r3
	bls.n	.L25
	mov	r0, r9
	mov	r1, r8
	adds	r2, r5, #0
	movs	r3, #0
	bl	Func_080ae9f0
	b.n	.L24
.L25:
	mov	r0, r9
	mov	r1, r8
	adds	r2, r5, #0
	movs	r3, #1
	bl	Func_080ae9f0
.L24:
	ldr	r5, [sp, #52]
	ldr	r7, [sp, #48]
	adds	r5, #64
	adds	r7, #64
	ldrh	r2, [r5, #0]
	ldrh	r3, [r7, #0]
	cmp	r2, r3
	beq.n	.L26
	ldr	r1, [sp, #60]
	adds	r0, r2, #0
	lsls	r2, r1, #3
	adds	r6, r2, #0
	mov	r3, fp
	adds	r3, #72
	mov	r2, r9
	adds	r6, #88
	movs	r1, #4
	str	r6, [sp, #0]
	bl	Func_080150b0
	ldrh	r2, [r5, #0]
	ldrh	r3, [r7, #0]
	cmp	r2, r3
	bls.n	.L27
	mov	r0, r9
	mov	r1, r8
	adds	r2, r6, #0
	movs	r3, #0
	bl	Func_080ae9f0
	b.n	.L26
.L27:
	mov	r0, r9
	mov	r1, r8
	adds	r2, r6, #0
	movs	r3, #1
	bl	Func_080ae9f0
.L26:
	ldr	r7, [sp, #52]
	ldr	r6, [sp, #48]
	adds	r7, #66
	adds	r6, #66
	ldrb	r2, [r7, #0]
	ldrb	r3, [r6, #0]
	cmp	r2, r3
	beq.n	.L13
	ldr	r1, [sp, #60]
	adds	r0, r2, #0
	lsls	r2, r1, #3
	adds	r5, r2, #0
	mov	r3, fp
	adds	r3, #88
	mov	r2, r9
	adds	r5, #96
	movs	r1, #2
	str	r5, [sp, #0]
	bl	Func_080150b0
	ldrb	r2, [r7, #0]
	ldrb	r3, [r6, #0]
	cmp	r2, r3
	bls.n	.L28
	mov	r0, r9
	mov	r1, r8
	adds	r2, r5, #0
	movs	r3, #0
	bl	Func_080ae9f0
	b.n	.L13
.L28:
	mov	r0, r9
	mov	r1, r8
	adds	r2, r5, #0
	movs	r3, #1
	bl	Func_080ae9f0
.L13:
	ldr	r2, [sp, #216]
	cmp	r2, #0
	bgt.n	.L29
	b.n	.L30
.L29:
	ldr	r3, [sp, #212]
	movs	r2, #3
	eors	r2, r3
	negs	r3, r2
	orrs	r3, r2
	lsrs	r3, r3, #31
	mov	sl, r3
	mov	r0, sl
	movs	r3, #6
	subs	r0, r3, r0
	ldr	r3, [sp, #216]
	mov	sl, r0
	subs	r3, #1
	mov	r1, sl
	muls	r1, r3
	ldr	r0, [sp, #48]
	mov	r8, r1
	ldr	r1, [sp, #52]
	add	r2, sp, #68
	add	r5, sp, #76
	adds	r1, #88
	add	r3, sp, #72
	str	r2, [sp, #0]
	adds	r0, #88
	adds	r2, r5, #0
	bl	Func_080aae14
	lsls	r0, r0, #24
	str	r0, [sp, #16]
	asrs	r3, r0, #24
	ldr	r0, [sp, #64]
	lsls	r0, r0, #3
	movs	r2, #0
	movs	r1, #0
	mov	fp, r0
	cmp	r8, r3
	bge.n	.L31
	cmp	r1, sl
	bge.n	.L32
	movs	r3, #0
	str	r0, [sp, #20]
	mov	r0, r8
	str	r3, [sp, #12]
	lsls	r3, r0, #1
	adds	r7, r3, r5
.L36:
	ldr	r1, [sp, #60]
	lsls	r6, r2, #24
	asrs	r2, r6, #23
	adds	r2, r1, r2
	ldrh	r3, [r7, #0]
	ldr	r0, [pc, #64]
	lsls	r2, r2, #3
	ands	r3, r0
	adds	r2, #4
	mov	r0, r9
	mov	r1, fp
	bl	Func_080ae958
	ldrh	r2, [r7, #0]
	ldr	r3, [pc, #28]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L33
	movs	r0, #4
	bl	Func_080150b8
	b.n	.L34
.L33:
	ldr	r3, [pc, #16]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L35
	movs	r0, #2
	bl	Func_080150b8
	b.n	.L34
	.4byte 0x00008000
	.4byte 0x00004000
	.4byte 0x00005001
	.4byte 0x0000f030
	.4byte 0x00003fff
.L35:
	movs	r0, #15
	bl	Func_080150b8
.L34:
	ldr	r1, [sp, #60]
	asrs	r6, r6, #24
	ldrh	r3, [r7, #0]
	lsls	r5, r6, #1
	ldr	r0, [pc, #300]
	adds	r5, r1, r5
	ldr	r2, [sp, #20]
	ands	r0, r3
	lsls	r5, r5, #3
	ldr	r3, [pc, #292]
	adds	r5, #8
	adds	r0, r0, r3
	mov	r1, r9
	adds	r2, #16
	adds	r3, r5, #0
	bl	Func_08015080
	ldrh	r0, [r7, #0]
	bl	Func_08077080
	ldr	r3, [sp, #20]
	ldrb	r0, [r0, #9]
	movs	r1, #2
	mov	r2, r9
	adds	r3, #88
	str	r5, [sp, #0]
	bl	Func_080150a8
	movs	r1, #128
	ldr	r0, [sp, #12]
	lsls	r1, r1, #17
	adds	r3, r0, r1
	ldr	r0, [sp, #16]
	adds	r6, #1
	lsrs	r1, r3, #24
	movs	r3, #1
	lsls	r6, r6, #24
	add	r8, r3
	asrs	r3, r0, #24
	lsrs	r2, r6, #24
	adds	r7, #2
	cmp	r8, r3
	bge.n	.L31
	lsls	r3, r1, #24
	str	r3, [sp, #12]
	asrs	r3, r3, #24
	cmp	r3, sl
	blt.n	.L36
	b.n	.L31
.L32:
	ldr	r1, [sp, #64]
	lsls	r1, r1, #3
	mov	fp, r1
.L31:
	movs	r0, #15
	bl	Func_080150b8
	ldr	r3, [sp, #60]
	mov	r2, fp
	lsls	r6, r3, #3
	ldr	r0, [pc, #200]
	adds	r2, #88
	mov	r1, r9
	adds	r3, r6, #0
	bl	Func_08015080
	ldr	r0, [sp, #212]
	cmp	r0, #3
	beq.n	.L37
	ldr	r3, [sp, #72]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L38
	movs	r0, #4
	bl	Func_080150b8
	adds	r3, r6, #0
	ldr	r0, [pc, #168]
	adds	r3, #88
	mov	r1, r9
	mov	r2, fp
	bl	Func_08015080
	movs	r5, #1
.L38:
	ldr	r3, [sp, #68]
	cmp	r3, #0
	beq.n	.L39
	movs	r0, #2
	bl	Func_080150b8
	ldr	r1, [sp, #60]
	adds	r3, r1, r5
	lsls	r3, r3, #3
	ldr	r0, [pc, #140]
	adds	r3, #88
	mov	r1, r9
	mov	r2, fp
	bl	Func_08015080
	adds	r5, #1
.L39:
	cmp	r5, #0
	bne.n	.L40
	adds	r3, r6, #0
	ldr	r0, [pc, #124]
	adds	r3, #88
	mov	r1, r9
	mov	r2, fp
	bl	Func_08015080
.L40:
	movs	r0, #15
	bl	Func_080150b8
	movs	r3, #11
	str	r3, [sp, #0]
	mov	r0, r9
	movs	r1, #0
	movs	r2, #11
	movs	r3, #13
	bl	Func_08015070
.L37:
	ldr	r3, [pc, #96]
	ldr	r2, [pc, #96]
	ldr	r3, [r3, #0]
	adds	r3, r3, r2
	movs	r2, #1
	strb	r2, [r3, #0]
.L30:
	ldr	r3, [sp, #216]
	cmp	r3, #0
	bne.n	.L41
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	ldr	r0, [sp, #56]
	movs	r1, #0
	ldr	r2, [sp, #220]
	mov	r3, r9
	bl	Func_080150d8
.L41:
	movs	r2, #166
	ldr	r1, [sp, #48]
	ldr	r3, [pc, #64]
	ldr	r0, [sp, #52]
	lsls	r2, r2, #1
	bl	Func_080072f0
	ldr	r0, [sp, #48]
	bl	Func_08002df0
	movs	r0, #1
	add	sp, #172
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00003fff
	.4byte 0x00000333
	.4byte 0x00000aed
	.4byte 0x00000ba2
	.4byte 0x00000ba3
	.4byte 0x00000ba8
	.4byte 0x03001e8c
	.4byte 0x00000ea3
	.4byte 0x03001388
