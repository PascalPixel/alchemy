@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a8114
	.thumb_func
Func_080a8114:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #100]
	sub	sp, #40
	movs	r0, #0
	ldr	r7, [r3, #0]
	mov	sl, r0
	str	r0, [sp, #28]
	mov	r8, r0
	subs	r0, #1
	bl	Func_08077290
	negs	r3, r0
	orrs	r3, r0
	lsrs	r3, r3, #31
	str	r3, [sp, #12]
	movs	r3, #5
	str	r3, [sp, #0]
	adds	r0, r7, #0
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r1, #0
	movs	r2, #0
	movs	r3, #30
	adds	r0, #44
	bl	Func_080a10d0
	ldr	r0, [pc, #52]
	bl	Func_08004278
	ldr	r0, [pc, #52]
	ldr	r1, [pc, #36]
	movs	r2, #3
	adds	r3, r7, r0
.L0:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L0
	movs	r1, #0
	movs	r0, #10
	str	r1, [sp, #16]
	negs	r0, r0
	movs	r1, #88
	bl	Func_080a1ac0
	ldr	r2, [pc, #24]
	adds	r2, r7, r2
	str	r2, [sp, #8]
	b.n	.L1
	movs	r0, r0
	.4byte 0x00000068
	.4byte 0x03001f2c
	.4byte 0x080a19a1
	.4byte 0x00000242
	.4byte 0x0000021a
.L28:
	ldr	r3, [sp, #8]
	ldrb	r0, [r3, #0]
	bl	Func_08077008
	ldr	r2, [sp, #8]
	ldr	r0, [r7, #36]
	ldrb	r1, [r2, #0]
	movs	r2, #1
	bl	Func_080a8604
	movs	r3, #32
	ldr	r0, [sp, #8]
	add	r3, sp
	mov	fp, r3
	ldrb	r2, [r0, #0]
	movs	r1, #1
	mov	r0, fp
	bl	Func_080a8b10
	lsls	r0, r0, #24
	movs	r1, #0
	lsrs	r2, r0, #24
	str	r1, [sp, #20]
	str	r2, [sp, #24]
	cmp	r0, #0
	bne.n	.L2
	movs	r3, #1
	str	r3, [sp, #24]
	b.n	.L3
.L16:
	movs	r0, #112
	bl	Func_080f9010
	movs	r0, #1
	str	r0, [sp, #16]
	str	r0, [sp, #28]
	b.n	.L1
.L18:
	movs	r0, #113
	bl	Func_080f9010
	movs	r2, #1
	movs	r1, #1
	negs	r2, r2
	str	r1, [sp, #16]
	str	r2, [sp, #28]
	b.n	.L1
.L2:
	movs	r3, #1
	str	r3, [sp, #20]
.L3:
	movs	r0, #1
	mov	r9, r0
	b.n	.L4
.L26:
	mov	r1, r9
	cmp	r1, #0
	beq.n	.L5
	movs	r2, #0
	ldr	r0, [sp, #24]
	mov	r9, r2
	mov	r2, sl
	adds	r2, #2
	lsls	r3, r0, #24
	asrs	r1, r3, #24
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	lsls	r3, r3, #1
	subs	r2, r2, r3
	mov	sl, r2
	cmp	r2, #0
	bne.n	.L6
	mov	r2, r8
	adds	r0, r2, r1
	bl	Func_080022fc
	mov	r8, r0
	ldr	r0, [r7, #44]
	bl	Func_08015270
	ldr	r3, [sp, #20]
	cmp	r3, #0
	bne.n	.L7
	ldr	r5, [pc, #432]
	movs	r6, #24
	negs	r6, r6
	ldr	r1, [r7, #36]
	adds	r0, r5, #0
	movs	r2, #80
	adds	r3, r6, #0
	bl	Func_08015080
	ldr	r1, [r7, #36]
	adds	r0, r5, #1
	movs	r2, #0
	adds	r3, r6, #0
	bl	Func_08015080
	b.n	.L7
.L6:
	ldr	r0, [r7, #44]
	bl	Func_08015270
	ldr	r0, [sp, #12]
	cmp	r0, #0
	beq.n	.L8
	mov	r2, r8
	adds	r2, #8
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L9
	mov	r3, r8
	adds	r3, #15
.L9:
	asrs	r3, r3, #3
	lsls	r3, r3, #3
	subs	r2, r2, r3
	mov	r8, r2
	b.n	.L7
.L8:
	mov	r0, r8
	adds	r0, #7
	movs	r1, #7
	bl	Func_080022fc
	mov	r8, r0
.L7:
	mov	r1, r8
	mov	r2, fp
	movs	r3, #0
	mov	r0, sl
	bl	Func_080a847c
	ldr	r0, [r7, #44]
	bl	Func_08015278
	movs	r0, #1
	bl	Func_080030f8
	mov	r1, sl
	cmp	r1, #0
	bne.n	.L10
	ldr	r0, [r7, #44]
	mov	r1, r8
	mov	r2, fp
	bl	Func_080a8508
	b.n	.L5
.L10:
	ldr	r0, [r7, #44]
	mov	r1, r8
	ldr	r2, [sp, #12]
	bl	Func_080a8578
.L5:
	ldr	r2, [r7, #20]
	movs	r3, #1
	strb	r3, [r2, #5]
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L11
	mov	r3, r8
	lsls	r1, r3, #4
	movs	r0, #10
	adds	r1, #88
	negs	r0, r0
	bl	Func_080a1a40
	b.n	.L12
.L11:
	mov	r0, r8
	cmp	r0, #3
	bgt.n	.L13
	lsls	r1, r0, #3
	adds	r1, #48
	movs	r0, #24
	bl	Func_080a1a40
	b.n	.L12
.L13:
	mov	r2, r8
	lsls	r1, r2, #3
	adds	r1, #80
	movs	r0, #48
	bl	Func_080a1a40
.L12:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r5, [pc, #240]
	ldr	r2, [r5, #0]
	movs	r3, #240
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L14
	mov	r0, sl
	mov	r1, r8
	mov	r2, fp
	movs	r3, #1
	bl	Func_080a847c
.L14:
	ldr	r1, [pc, #220]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L15
	b.n	.L16
.L15:
	ldr	r2, [r1, #0]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L17
	b.n	.L18
.L17:
	ldr	r2, [r5, #0]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L19
	movs	r0, #111
	bl	Func_080f9010
	movs	r0, #1
	movs	r3, #1
	negs	r0, r0
	mov	r9, r3
	add	r8, r0
.L19:
	ldr	r2, [r5, #0]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L20
	movs	r0, #111
	bl	Func_080f9010
	movs	r1, #1
	mov	r9, r1
	add	r8, r1
.L20:
	ldr	r2, [r5, #0]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L21
	movs	r0, #111
	bl	Func_080f9010
	movs	r2, #1
	mov	r9, r2
	add	sl, r2
.L21:
	ldr	r2, [r5, #0]
	movs	r3, #32
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L22
	movs	r0, #111
	bl	Func_080f9010
	movs	r0, #1
	movs	r3, #1
	negs	r0, r0
	mov	r9, r3
	add	sl, r0
.L22:
	ldr	r3, [r5, #0]
	movs	r6, #128
	lsls	r6, r6, #1
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L23
	ldr	r2, [r5, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L4
.L23:
	movs	r0, #111
	bl	Func_080f9010
	ldr	r3, [r5, #0]
	ands	r3, r6
	movs	r0, #28
	ldrsb	r0, [r7, r0]
	cmp	r3, #0
	beq.n	.L24
	adds	r0, #1
	b.n	.L25
.L24:
	subs	r0, #1
.L25:
	ldr	r1, [pc, #52]
	adds	r3, r7, r1
	ldrb	r1, [r3, #0]
	adds	r0, r0, r1
	bl	Func_080022fc
	movs	r3, #130
	lsls	r2, r0, #1
	lsls	r3, r3, #2
	adds	r2, r2, r3
	ldrh	r3, [r7, r2]
	str	r3, [r7, #8]
	ldr	r1, [sp, #8]
	ldrh	r3, [r7, r2]
	strb	r3, [r1, #0]
	strb	r0, [r7, #28]
	adds	r0, r7, #0
	ldrh	r1, [r7, r2]
	bl	Func_080a1804
	b.n	.L1
	movs	r0, r0
	.4byte 0x00000b06
	.4byte 0x03001b04
	.4byte 0x03001c94
	.4byte 0x00000219
.L4:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L1
	b.n	.L26
.L1:
	ldr	r2, [sp, #16]
	cmp	r2, #0
	bne.n	.L27
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L27
	b.n	.L28
.L27:
	ldr	r0, [r7, #44]
	bl	Func_08015278
	ldr	r0, [r7, #44]
	bl	Func_08015270
	movs	r3, #96
	ldr	r0, [r7, #36]
	movs	r2, #56
	str	r3, [sp, #0]
	movs	r1, #64
	movs	r3, #224
	bl	Func_08015068
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #20]
	bl	Func_080041d8
	ldr	r1, [pc, #16]
	ldr	r3, [pc, #8]
	movs	r2, #3
	adds	r0, r7, r1
	b.n	.L29
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x080a19a1
	.4byte 0x00000242
.L29:
	subs	r2, #1
	strh	r3, [r0, #0]
	subs	r0, #2
	cmp	r2, #0
	bge.n	.L29
	bl	Func_080a9d84
	ldr	r0, [sp, #28]
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
