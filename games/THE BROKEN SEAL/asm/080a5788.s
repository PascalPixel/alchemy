@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a5788
	.thumb_func
Func_080a5788:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #116]
	sub	sp, #88
	movs	r1, #0
	ldr	r7, [r3, #0]
	str	r1, [sp, #32]
	str	r1, [sp, #20]
	add	r4, sp, #32
	movs	r2, #151
	ldrb	r4, [r4, #0]
	lsls	r2, r2, #2
	adds	r3, r7, r2
	strb	r4, [r3, #0]
	adds	r5, r7, #0
	movs	r3, #14
	str	r3, [sp, #0]
	adds	r5, #52
	movs	r3, #2
	str	r3, [sp, #4]
	mov	fp, r0
	movs	r1, #13
	adds	r0, r5, #0
	movs	r2, #3
	movs	r3, #17
	bl	Func_080a10d0
	ldr	r5, [r5, #0]
	movs	r1, #0
	str	r5, [sp, #36]
	str	r1, [sp, #24]
	movs	r2, #40
	mov	r3, fp
	add	r2, sp
	lsls	r3, r3, #1
	adds	r4, r7, #2
	mov	r8, r2
	str	r3, [sp, #8]
	str	r4, [sp, #12]
	b.n	.L0
.L14:
	movs	r0, #173
	bl	Func_080f9010
	mov	r1, r8
	ldr	r3, [r1, #24]
	movs	r2, #228
	lsls	r3, r3, #1
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r7, r3]
	str	r3, [sp, #32]
	movs	r3, #1
	str	r3, [sp, #24]
	b.n	.L0
.L16:
	movs	r0, #113
	bl	Func_080f9010
	movs	r4, #1
	str	r5, [sp, #32]
	str	r4, [sp, #24]
	b.n	.L0
	.4byte 0x03001f2c
.L24:
	mov	r2, r8
	ldr	r1, [r2, #16]
	lsls	r1, r1, #4
	adds	r1, #36
	movs	r0, #98
	bl	Func_080a1a40
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L1
	ldr	r1, [sp, #20]
	lsls	r3, r1, #1
	adds	r3, #216
	mov	r2, r9
	ldrh	r3, [r2, r3]
	movs	r4, #0
	mov	sl, r4
	cmp	r3, #0
	beq.n	.L2
	lsls	r3, r1, #2
	adds	r3, #72
	ldr	r0, [r7, r3]
	bl	Func_080a17c4
.L2:
	ldr	r3, [sp, #28]
	cmp	r3, #0
	beq.n	.L3
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #0
	ldr	r0, [sp, #36]
	mov	r2, r8
	bl	Func_080a56c8
	mov	r4, fp
	cmp	r4, #0
	bne.n	.L4
	ldr	r0, [pc, #240]
	ldr	r1, [sp, #36]
	movs	r2, #0
	movs	r3, #88
	bl	Func_08015080
.L4:
	movs	r1, #0
	str	r1, [sp, #28]
.L3:
	add	r1, sp, #68
	ldr	r0, [sp, #36]
	mov	r2, r8
	bl	Func_080a5614
	movs	r4, #188
	ldr	r3, [sp, #8]
	lsls	r4, r4, #1
	mov	r1, r8
	adds	r2, r3, r4
	ldr	r3, [r1, #24]
	movs	r5, #228
	lsls	r5, r5, #1
	lsls	r3, r3, #1
	adds	r3, r3, r5
	ldrh	r3, [r7, r3]
	strh	r3, [r7, r2]
	movs	r3, #134
	ldr	r2, [sp, #12]
	lsls	r3, r3, #2
	add	r3, fp
	ldrb	r3, [r2, r3]
	ldr	r1, [r1, #24]
	adds	r0, r3, #0
	movs	r2, #0
	bl	Func_080a3ef0
	mov	r3, r8
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r5
	ldrh	r3, [r7, r3]
	cmp	r3, #0
	beq.n	.L5
	lsls	r3, r2, #2
	adds	r3, #72
	ldr	r0, [r7, r3]
	movs	r3, #9
	movs	r2, #0
	strb	r3, [r0, #5]
	movs	r3, #250
	strh	r2, [r0, #12]
	strb	r3, [r0, #15]
.L5:
	ldr	r4, [pc, #144]
	adds	r3, r7, r4
	ldrb	r3, [r3, #0]
	movs	r5, #0
	cmp	r5, r3
	bcs.n	.L1
	adds	r6, r7, r4
.L6:
	movs	r1, #138
	lsls	r3, r5, #2
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldr	r0, [r7, r3]
	movs	r1, #1
	bl	Func_08009020
	adds	r3, r5, #1
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
	ldrb	r3, [r6, #0]
	cmp	r5, r3
	bcc.n	.L6
.L1:
	ldr	r3, [pc, #104]
	ldr	r2, [r3, #0]
	movs	r3, #31
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L7
	ldr	r2, [pc, #88]
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	movs	r5, #0
	cmp	r5, r3
	bcs.n	.L7
.L9:
	movs	r4, #130
	lsls	r3, r5, #1
	lsls	r4, r4, #2
	adds	r3, r3, r4
	mov	r1, r8
	ldrh	r0, [r7, r3]
	ldr	r3, [r1, #24]
	movs	r2, #228
	lsls	r3, r3, #1
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r7, r3]
	ldr	r1, [pc, #44]
	ands	r1, r3
	bl	Func_08077218
	cmp	r0, #0
	beq.n	.L8
	movs	r4, #138
	lsls	r3, r5, #2
	lsls	r4, r4, #1
	adds	r3, r3, r4
	ldr	r0, [r7, r3]
	movs	r1, #3
	bl	Func_08009020
.L8:
	adds	r3, r5, #1
	ldr	r1, [pc, #24]
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	cmp	r5, r3
	bcc.n	.L9
	b.n	.L7
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x00000b89
	.4byte 0x00000219
	.4byte 0x03001e40
.L7:
	movs	r0, #1
	bl	Func_080030f8
	mov	r2, r8
	ldr	r2, [r2, #24]
	str	r2, [sp, #20]
	mov	r3, r8
	add	r2, sp, #48
	ldr	r1, [r3, #20]
	movs	r0, #0
	str	r2, [sp, #0]
	add	r3, sp, #56
	movs	r2, #5
	bl	Func_080a1fd4
	cmp	r0, #1
	bne.n	.L10
	movs	r4, #1
	str	r4, [sp, #28]
	mov	sl, r4
.L10:
	cmp	r0, #0
	bne.n	.L11
	movs	r1, #1
	mov	sl, r1
.L11:
	movs	r5, #1
	negs	r5, r5
	cmp	r0, r5
	bne.n	.L12
	movs	r2, #0
	mov	sl, r2
.L12:
	ldr	r1, [pc, #260]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L13
	mov	r4, r8
	ldr	r3, [r4, #24]
	movs	r2, #228
	lsls	r3, r3, #1
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r7, r3]
	cmp	r3, #0
	beq.n	.L13
	b.n	.L14
.L13:
	ldr	r2, [r1, #0]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L15
	b.n	.L16
.L15:
	ldr	r1, [pc, #220]
	ldr	r2, [r1, #0]
	adds	r3, #254
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L17
	ldr	r2, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L18
.L17:
	mov	r3, fp
	cmp	r3, #1
	bne.n	.L19
	movs	r0, #114
	bl	Func_080f9010
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L18
.L19:
	movs	r0, #111
	bl	Func_080f9010
	movs	r1, #134
	ldr	r2, [sp, #12]
	mov	r4, fp
	lsls	r1, r1, #2
	adds	r0, r4, r1
	ldrb	r3, [r2, r0]
	movs	r4, #152
	lsls	r4, r4, #2
	adds	r3, r3, r4
	mov	r4, r8
	ldr	r2, [r4, #24]
	strb	r2, [r7, r3]
	mov	r2, fp
	adds	r2, #28
	str	r2, [sp, #16]
	ldr	r3, [sp, #12]
	ldrsb	r5, [r7, r2]
	mov	r9, r0
	mov	sl, r3
.L22:
	ldr	r3, [pc, #132]
	movs	r2, #128
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L20
	adds	r5, #1
	b.n	.L21
.L20:
	subs	r5, #1
.L21:
	ldr	r4, [pc, #116]
	adds	r3, r7, r4
	ldrb	r1, [r3, #0]
	adds	r0, r5, r1
	bl	Func_080022fc
	movs	r1, #130
	adds	r5, r0, #0
	lsls	r1, r1, #2
	lsls	r6, r5, #1
	adds	r2, r6, r1
	ldrh	r3, [r7, r2]
	str	r3, [r7, #8]
	ldrh	r3, [r7, r2]
	mov	r4, r9
	mov	r2, sl
	strb	r3, [r2, r4]
	ldr	r1, [sp, #16]
	strb	r5, [r7, r1]
	ldrb	r0, [r2, r4]
	bl	Func_08077008
	movs	r2, #228
	lsls	r2, r2, #1
	adds	r1, r7, r2
	movs	r2, #0
	bl	Func_080a3ddc
	movs	r3, #134
	lsls	r3, r3, #2
	strb	r0, [r7, r3]
	lsls	r0, r0, #24
	cmp	r0, #0
	beq.n	.L22
	movs	r1, #162
	ldr	r2, [pc, #32]
	movs	r5, #0
	lsls	r1, r1, #1
.L23:
	lsls	r3, r5, #1
	adds	r3, r3, r1
	strh	r2, [r7, r3]
	adds	r3, r5, #1
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
	cmp	r5, #3
	bls.n	.L23
	movs	r4, #162
	lsls	r4, r4, #1
	ldr	r3, [pc, #8]
	adds	r2, r6, r4
	strh	r3, [r7, r2]
	b.n	.L0
	.4byte 0x0000001e
	.4byte 0x0000001a
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x00000219
.L18:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L0
	b.n	.L24
.L0:
	ldr	r1, [sp, #24]
	cmp	r1, #0
	bne.n	.L25
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L25
	movs	r6, #134
	ldr	r4, [sp, #12]
	mov	r2, fp
	lsls	r6, r6, #2
	adds	r3, r2, r6
	ldrb	r0, [r4, r3]
	bl	Func_08077008
	movs	r1, #228
	lsls	r1, r1, #1
	adds	r5, r7, r1
	movs	r2, #0
	adds	r1, r5, #0
	mov	r9, r0
	bl	Func_080a3ddc
	movs	r1, #0
	strb	r0, [r7, r6]
	adds	r0, r5, #0
	bl	Func_080a3e28
	movs	r2, #135
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldr	r2, [r3, #0]
	movs	r3, #13
	strb	r3, [r2, #5]
	mov	r1, fp
	mov	r0, r8
	bl	Func_080a5578
	mov	r3, r8
	ldr	r1, [r3, #24]
	lsls	r1, r1, #4
	adds	r1, #36
	movs	r0, #98
	bl	Func_080a1a40
	movs	r4, #1
	mov	sl, r4
	str	r4, [sp, #28]
	b.n	.L18
.L25:
	movs	r3, #96
	str	r3, [sp, #0]
	ldr	r0, [sp, #36]
	movs	r1, #0
	movs	r2, #88
	movs	r3, #120
	bl	Func_08015068
	ldr	r0, [r7, #68]
	bl	Func_080a17c4
	movs	r3, #186
	ldr	r1, [sp, #8]
	mov	r4, r8
	lsls	r3, r3, #1
	adds	r2, r1, r3
	ldr	r3, [r4, #24]
	strh	r3, [r7, r2]
	movs	r3, #134
	ldr	r1, [sp, #12]
	lsls	r3, r3, #2
	add	r3, fp
	ldrb	r3, [r1, r3]
	movs	r2, #152
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r2, [r4, #24]
	strb	r2, [r7, r3]
	ldr	r4, [sp, #8]
	movs	r1, #188
	add	r2, sp, #32
	lsls	r1, r1, #1
	ldrh	r2, [r2, #0]
	adds	r3, r4, r1
	movs	r0, #168
	strh	r2, [r7, r3]
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L26
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #32]
.L26:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r0, [sp, #32]
	add	sp, #88
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
