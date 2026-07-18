@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a7a34
	.thumb_func
Func_080a7a34:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #112]
	ldr	r3, [r3, #0]
	movs	r1, #28
	ldrsb	r1, [r3, r1]
	movs	r2, #30
	ldrsb	r2, [r3, r2]
	mov	r8, r3
	sub	sp, #12
	movs	r3, #1
	str	r3, [sp, #8]
	mov	sl, r1
	movs	r3, #136
	movs	r1, #0
	str	r1, [sp, #4]
	lsls	r3, r3, #2
	add	r3, r8
	ldrh	r3, [r3, #0]
	mov	fp, r2
	movs	r1, #130
	mov	r2, sl
	lsls	r1, r1, #2
	lsls	r7, r2, #1
	mov	r9, r3
	mov	r2, r8
	adds	r3, r7, r1
	ldrh	r0, [r2, r3]
	bl	Func_08077008
	movs	r3, #141
	lsls	r3, r3, #2
	ldr	r0, [pc, #48]
	add	r3, r8
	movs	r1, #130
	movs	r2, #3
.L0:
	subs	r2, #1
	strh	r1, [r3, #0]
	strh	r0, [r3, #8]
	adds	r1, #32
	adds	r3, #2
	cmp	r2, #0
	bge.n	.L0
	movs	r0, #14
	bl	Func_080a2144
	movs	r1, #160
	ldr	r3, [pc, #24]
	ldr	r0, [pc, #28]
	lsls	r1, r1, #19
	ldr	r2, [pc, #28]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r1, #28
	ldr	r0, [pc, #24]
	ldr	r2, [pc, #24]
	b.n	.L1
	.4byte 0x00000080
	.4byte 0x03001f2c
	.4byte 0x040000d4
	.4byte 0x05000200
	.4byte 0x80000010
	.4byte 0x050001c8
	.4byte 0x80000001
.L1:
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r1, #4
	ldr	r0, [pc, #192]
	ldr	r2, [pc, #192]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r1, #28
	ldr	r0, [pc, #188]
	ldr	r2, [pc, #192]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	b.n	.L2
.L20:
	ldr	r3, [sp, #8]
	cmp	r3, #0
	beq.n	.L3
	movs	r1, #0
	movs	r5, #134
	str	r1, [sp, #8]
	lsls	r5, r5, #1
	add	r5, r8
	ldr	r0, [r5, #0]
	bl	Func_08015270
	ldr	r6, [pc, #164]
	ldr	r1, [r5, #0]
	adds	r0, r6, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
	movs	r0, #48
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L4
	ldr	r1, [r5, #0]
	ldr	r0, [pc, #144]
	movs	r2, #0
	movs	r3, #16
	bl	Func_08015080
.L4:
	ldr	r1, [r5, #0]
	subs	r0, r6, #3
	movs	r2, #0
	movs	r3, #8
	bl	Func_08015080
	mov	r0, sl
	mov	r1, fp
	add	r0, fp
	bl	Func_080022fc
	movs	r2, #130
	lsls	r2, r2, #2
	lsls	r7, r0, #1
	adds	r5, r7, r2
	mov	r3, r8
	mov	sl, r0
	ldrh	r0, [r3, r5]
	bl	Func_08077008
	mov	r0, r9
	movs	r1, #3
	adds	r0, #3
	bl	Func_080022fc
	mov	r1, r8
	mov	r9, r0
	ldrh	r0, [r1, r5]
	mov	r1, r9
	bl	Func_080a8088
	mov	r2, r8
	ldrh	r1, [r2, r5]
	mov	r0, r8
	bl	Func_080a1804
	movs	r3, #169
	lsls	r3, r3, #1
	ldr	r1, [pc, #28]
	adds	r6, r7, #0
	movs	r2, #7
	add	r3, r8
.L5:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L5
	movs	r3, #162
	lsls	r3, r3, #1
	adds	r2, r6, r3
	ldr	r3, [pc, #8]
	mov	r1, r8
	strh	r3, [r1, r2]
	b.n	.L6
	.4byte 0x0000001e
	.4byte 0x0000001a
	.4byte 0x05000200
	.4byte 0x80000010
	.4byte 0x050001e8
	.4byte 0x80000001
	.4byte 0x00000b0d
	.4byte 0x00000b16
.L3:
	mov	r2, sl
	lsls	r7, r2, #1
.L6:
	mov	r3, sl
	adds	r0, r7, r3
	lsls	r0, r0, #3
	movs	r1, #16
	subs	r0, #10
	bl	Func_080a1a40
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [pc, #136]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L7
	movs	r0, #112
	bl	Func_080f9010
	movs	r1, #1
	str	r1, [sp, #4]
	b.n	.L8
.L7:
	ldr	r6, [r1, #0]
	movs	r3, #2
	ands	r6, r3
	cmp	r6, #0
	beq.n	.L9
	movs	r0, #113
	bl	Func_080f9010
	movs	r2, #1
	negs	r2, r2
	str	r2, [sp, #4]
	b.n	.L8
.L9:
	ldr	r7, [pc, #92]
	movs	r3, #128
	ldr	r5, [r7, #0]
	lsls	r3, r3, #1
	ands	r5, r3
	cmp	r5, #0
	beq.n	.L10
	mov	r0, sl
	movs	r1, #1
	bl	Func_080a7f44
	cmp	r0, #0
	beq.n	.L11
	movs	r0, #112
	bl	Func_080f9010
	movs	r3, #1
	add	sl, r3
	bl	Func_080a195c
	mov	r1, r8
	ldr	r0, [r1, #16]
	movs	r2, #2
	movs	r1, #2
	movs	r3, #8
	str	r6, [sp, #0]
	bl	Func_080a1870
	movs	r3, #169
	lsls	r3, r3, #1
	ldr	r1, [pc, #24]
	movs	r2, #7
	add	r3, r8
.L12:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L12
	mov	r2, sl
	movs	r3, #162
	lsls	r3, r3, #1
	lsls	r7, r2, #1
	b.n	.L13
	movs	r0, r0
	.4byte 0x0000001e
	.4byte 0x03001c94
	.4byte 0x03001b04
.L10:
	ldr	r2, [r7, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L14
	mov	r0, sl
	movs	r1, #0
	bl	Func_080a7f44
	cmp	r0, #0
	beq.n	.L11
	movs	r0, #112
	bl	Func_080f9010
	movs	r2, #1
	negs	r2, r2
	add	sl, r2
	bl	Func_080a195c
	mov	r3, r8
	ldr	r0, [r3, #16]
	movs	r1, #2
	movs	r2, #2
	movs	r3, #8
	str	r5, [sp, #0]
	bl	Func_080a1870
	movs	r3, #169
	lsls	r3, r3, #1
	ldr	r1, [pc, #44]
	movs	r2, #7
	add	r3, r8
.L15:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L15
	mov	r1, sl
	movs	r3, #162
	lsls	r7, r1, #1
	lsls	r3, r3, #1
.L13:
	adds	r2, r7, r3
	ldr	r3, [pc, #24]
	mov	r1, r8
	strh	r3, [r1, r2]
	b.n	.L16
.L11:
	movs	r0, #114
	bl	Func_080f9010
.L16:
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L2
	.4byte 0x0000001e
	.4byte 0x0000001a
.L14:
	ldr	r2, [r1, #0]
	movs	r3, #4
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L17
	movs	r0, #48
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L17
	bl	Func_080a7850
	movs	r2, #1
	str	r2, [sp, #8]
	b.n	.L2
.L17:
	ldr	r2, [r7, #0]
	movs	r3, #32
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L18
	movs	r0, #111
	bl	Func_080f9010
	mov	r3, fp
	cmp	r3, #1
	ble.n	.L18
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	str	r2, [sp, #8]
	add	sl, r1
.L18:
	ldr	r2, [r7, #0]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L2
	movs	r0, #111
	bl	Func_080f9010
	mov	r3, fp
	cmp	r3, #1
	ble.n	.L2
	movs	r1, #1
	str	r1, [sp, #8]
	add	sl, r1
.L2:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L19
	b.n	.L20
.L19:
	mov	r2, sl
	lsls	r7, r2, #1
.L8:
	mov	r1, r8
	mov	r3, sl
	strb	r3, [r1, #28]
	movs	r3, #130
	lsls	r3, r3, #2
	adds	r2, r7, r3
	ldrh	r3, [r1, r2]
	str	r3, [r1, #8]
	ldr	r3, [pc, #24]
	ldrh	r2, [r1, r2]
	add	r3, r8
	strb	r2, [r3, #0]
	ldr	r0, [sp, #4]
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x0000021a
