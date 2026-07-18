@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a38d0
	.thumb_func
Func_080a38d0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	str	r0, [sp, #24]
	ldr	r3, [pc, #84]
	ldr	r6, [r3, #0]
	ldr	r1, [r6, #32]
	mov	r8, r1
	ldr	r1, [pc, #80]
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	movs	r2, #29
	ldrsb	r2, [r6, r2]
	str	r3, [sp, #20]
	movs	r3, #0
	str	r3, [sp, #12]
	str	r3, [sp, #8]
	mov	sl, r2
	movs	r3, #12
	movs	r2, #1
	str	r2, [sp, #16]
	str	r3, [sp, #0]
	movs	r2, #5
	mov	r0, r8
	movs	r1, #13
	movs	r3, #17
	bl	Func_080a23f4
	ldr	r0, [r6, #32]
	bl	Func_08015270
	movs	r3, #28
	ldrsb	r3, [r6, r3]
	movs	r1, #130
	lsls	r1, r1, #2
	lsls	r3, r3, #1
	adds	r3, r3, r1
	ldrh	r0, [r6, r3]
	bl	Func_08077008
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #16]
	bl	Func_080041d8
	b.n	.L0
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x00000219
	.4byte 0x080a3c09
.L18:
	ldr	r2, [sp, #16]
	cmp	r2, #0
	bne.n	.L1
	b.n	.L2
.L1:
	ldr	r0, [sp, #20]
	movs	r3, #0
	ldr	r1, [sp, #20]
	add	r0, sl
	str	r3, [sp, #16]
	bl	Func_080022fc
	mov	sl, r0
	mov	r2, sl
	lsls	r2, r2, #1
	movs	r3, #130
	ldr	r1, [r6, #32]
	lsls	r3, r3, #2
	str	r2, [sp, #4]
	adds	r7, r2, r3
	ldrh	r0, [r6, r7]
	mov	r8, r1
	mov	r9, r2
	bl	Func_08077008
	ldr	r3, [r6, #16]
	ldr	r2, [sp, #4]
	ldrh	r1, [r3, #12]
	add	r2, sl
	adds	r1, r1, r2
	ldr	r2, [pc, #44]
	ldr	r5, [r6, #24]
	ldr	r3, [pc, #32]
	lsls	r1, r1, #3
	subs	r1, #2
	mov	fp, r2
	strh	r1, [r5, #6]
	ands	r1, r3
	mov	r3, fp
	ands	r1, r3
	ldr	r2, [pc, #20]
	ldrh	r3, [r5, #22]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r5, #22]
	ldr	r1, [sp, #24]
	cmp	r1, #1
	bne.n	.L3
	b.n	.L4
	.4byte 0x0000ffff
	.4byte 0xfffffe00
	.4byte 0x000001ff
.L4:
	ldrh	r0, [r6, r7]
	movs	r1, #1
	bl	Func_080a3e88
	movs	r3, #9
	str	r3, [sp, #0]
	mov	r0, r8
	movs	r1, #0
	movs	r2, #9
	movs	r3, #16
	bl	Func_08015070
	movs	r3, #80
	str	r3, [sp, #0]
	mov	r0, r8
	movs	r3, #120
	movs	r1, #0
	movs	r2, #72
	bl	Func_08015068
	movs	r3, #28
	ldrsb	r3, [r6, r3]
	cmp	sl, r3
	beq.n	.L5
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	mov	r1, fp
	ldrh	r0, [r6, r7]
	ands	r1, r3
	bl	Func_080a3d9c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L6
	movs	r3, #72
	str	r3, [sp, #0]
	movs	r1, #2
	mov	r2, r8
	movs	r3, #8
	bl	Func_080150b0
	ldr	r0, [pc, #140]
	mov	r1, r8
	movs	r2, #24
	movs	r3, #72
	bl	Func_08015080
	b.n	.L7
.L6:
	ldr	r0, [pc, #128]
	mov	r1, r8
	movs	r2, #16
	movs	r3, #72
	bl	Func_08015080
.L7:
	movs	r3, #130
	lsls	r3, r3, #2
	add	r3, r9
	ldrh	r0, [r6, r3]
	bl	Func_080a3d6c
	cmp	r0, #15
	bne.n	.L5
	cmp	r5, #0
	bne.n	.L5
	ldr	r0, [pc, #100]
	mov	r1, r8
	movs	r2, #0
	movs	r3, #72
	bl	Func_08015080
.L5:
	ldr	r1, [pc, #92]
	movs	r2, #186
	adds	r3, r6, r1
	lsls	r2, r2, #1
	ldrb	r0, [r3, #0]
	adds	r3, r6, r2
	ldrh	r1, [r3, #0]
	movs	r3, #130
	lsls	r3, r3, #2
	add	r3, r9
	ldrh	r3, [r6, r3]
	movs	r2, #0
	bl	Func_080a3ef0
.L3:
	ldr	r3, [sp, #24]
	cmp	r3, #0
	bne.n	.L8
	movs	r1, #188
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	ldr	r0, [pc, #36]
	ands	r0, r3
	bl	Func_080a3ce4
	cmp	r0, #0
	beq.n	.L9
	movs	r3, #130
	lsls	r3, r3, #2
	movs	r2, #186
	add	r3, r9
	lsls	r2, r2, #1
	ldrh	r1, [r6, r3]
	adds	r3, r6, r2
	ldrh	r2, [r3, #0]
	ldr	r0, [r6, #36]
	movs	r3, #8
	bl	Func_080a112c
	b.n	.L10
	.4byte 0x000001ff
	.4byte 0x00000b2f
	.4byte 0x00000b31
	.4byte 0x00000b30
	.4byte 0x0000021a
.L9:
	movs	r3, #130
	lsls	r3, r3, #2
	movs	r2, #186
	add	r3, r9
	lsls	r2, r2, #1
	ldrh	r1, [r6, r3]
	adds	r3, r6, r2
	ldrh	r2, [r3, #0]
	ldr	r0, [r6, #36]
	movs	r3, #0
	bl	Func_080a112c
.L10:
	ldr	r0, [pc, #64]
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L11
	ldr	r3, [sp, #8]
	cmp	r3, #0
	bne.n	.L11
	ldr	r0, [r6, #44]
	bl	Func_08015270
	movs	r1, #188
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	ldr	r0, [pc, #32]
	ands	r0, r3
	ldr	r3, [pc, #36]
	movs	r2, #0
	adds	r0, r0, r3
	ldr	r1, [r6, #44]
	movs	r3, #0
	bl	Func_08015080
	movs	r2, #1
	str	r2, [sp, #8]
	b.n	.L8
.L11:
	ldr	r0, [pc, #12]
	bl	Func_080770d0
	b.n	.L8
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x00000151
	.4byte 0x00000075
.L2:
	mov	r3, sl
	lsls	r3, r3, #1
	str	r3, [sp, #4]
.L8:
	ldr	r0, [sp, #4]
	add	r0, sl
	lsls	r0, r0, #3
	movs	r1, #16
	subs	r0, #10
	bl	Func_080a1a40
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [pc, #220]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L12
	ldr	r1, [sp, #24]
	cmp	r1, #1
	bne.n	.L13
	movs	r3, #28
	ldrsb	r3, [r6, r3]
	cmp	sl, r3
	bne.n	.L13
	movs	r0, #114
	bl	Func_080f9010
	b.n	.L0
.L13:
	movs	r0, #112
	bl	Func_080f9010
	movs	r1, #130
	ldr	r2, [sp, #4]
	lsls	r1, r1, #2
	adds	r3, r2, r1
	ldrb	r3, [r6, r3]
	str	r3, [sp, #12]
	b.n	.L14
.L12:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L15
	movs	r0, #113
	bl	Func_080f9010
	movs	r2, #255
	str	r2, [sp, #12]
	b.n	.L14
.L15:
	ldr	r5, [pc, #148]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L16
	movs	r0, #111
	bl	Func_080f9010
	movs	r3, #1
	movs	r1, #1
	negs	r3, r3
	str	r1, [sp, #16]
	add	sl, r3
.L16:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	movs	r0, #111
	bl	Func_080f9010
	movs	r2, #1
	str	r2, [sp, #16]
	add	sl, r2
.L0:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L17
	b.n	.L18
.L17:
	mov	r3, sl
	lsls	r3, r3, #1
	str	r3, [sp, #4]
.L14:
	ldr	r5, [r6, #24]
	mov	r1, sl
	strb	r1, [r6, #29]
	adds	r0, r5, #0
	bl	Func_080a17c4
	movs	r3, #13
	strb	r3, [r5, #5]
	bl	Func_080a3c98
	movs	r0, #1
	bl	Func_080030f8
	mov	r2, sl
	strb	r2, [r6, #29]
	ldr	r3, [sp, #4]
	movs	r1, #130
	lsls	r1, r1, #2
	adds	r2, r3, r1
	ldrh	r3, [r6, r2]
	str	r3, [r6, #8]
	adds	r1, #19
	ldrh	r2, [r6, r2]
	adds	r3, r6, r1
	strb	r2, [r3, #0]
	ldr	r2, [sp, #12]
	lsls	r0, r2, #24
	asrs	r0, r0, #24
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001c94
	.4byte 0x03001b04
