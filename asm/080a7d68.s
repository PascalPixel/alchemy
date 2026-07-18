@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a7d68
	.thumb_func
Func_080a7d68:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #232]
	ldr	r7, [r3, #0]
	movs	r0, #28
	ldrsb	r0, [r7, r0]
	sub	sp, #8
	movs	r2, #1
	mov	r8, r0
	movs	r0, #136
	movs	r1, #30
	ldrsb	r1, [r7, r1]
	lsls	r0, r0, #2
	str	r2, [sp, #4]
	adds	r3, r7, r0
	ldrh	r3, [r3, #0]
	mov	r9, r1
	mov	r1, r8
	str	r3, [sp, #0]
	lsls	r1, r1, #1
	movs	r3, #130
	mov	sl, r1
	lsls	r3, r3, #2
	add	r3, sl
	ldrh	r0, [r7, r3]
	bl	Func_08077008
	movs	r2, #134
	lsls	r2, r2, #1
	adds	r6, r7, r2
	ldr	r0, [r6, #0]
	bl	Func_08015270
	ldr	r5, [pc, #172]
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	adds	r5, #1
	bl	Func_08015080
	movs	r3, #16
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	bl	Func_08015080
	movs	r3, #228
	lsls	r3, r3, #1
	adds	r3, r3, r7
	mov	fp, r3
.L9:
	ldr	r0, [sp, #4]
	cmp	r0, #0
	beq.n	.L0
	movs	r1, #0
	mov	r0, r8
	str	r1, [sp, #4]
	add	r0, r9
	mov	r1, r9
	bl	Func_080022fc
	mov	r8, r0
	mov	r2, r8
	lsls	r2, r2, #1
	movs	r5, #130
	mov	sl, r2
	lsls	r5, r5, #2
	add	r5, sl
	ldrh	r0, [r7, r5]
	bl	Func_08077008
	ldr	r0, [sp, #0]
	movs	r1, #3
	adds	r0, #3
	bl	Func_080022fc
	str	r0, [sp, #0]
	ldr	r1, [sp, #0]
	ldrh	r0, [r7, r5]
	bl	Func_080a8088
	ldrh	r1, [r7, r5]
	adds	r0, r7, #0
	bl	Func_080a1804
	movs	r0, #169
	lsls	r0, r0, #1
	ldr	r1, [pc, #52]
	mov	r6, sl
	movs	r2, #7
	adds	r3, r7, r0
.L1:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L1
	movs	r1, #162
	lsls	r1, r1, #1
	ldr	r3, [pc, #36]
	adds	r2, r6, r1
	strh	r3, [r7, r2]
	movs	r2, #130
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldrh	r0, [r7, r3]
	bl	Func_08077008
	movs	r2, #0
	mov	r1, fp
	bl	Func_080a68ec
	movs	r1, #134
	lsls	r1, r1, #2
	b.n	.L2
	movs	r0, r0
	.4byte 0x0000001e
	.4byte 0x0000001a
	.4byte 0x03001f2c
	.4byte 0x00000c05
.L2:
	adds	r3, r7, r1
	strb	r0, [r3, #0]
	mov	r0, fp
	bl	Func_080a68a8
	movs	r0, #96
	movs	r1, #96
	movs	r2, #8
	bl	Func_080a9b94
	mov	r0, fp
	bl	Func_080a3d24
	b.n	.L3
.L0:
	mov	r2, r8
	lsls	r2, r2, #1
	mov	sl, r2
.L3:
	mov	r0, sl
	add	r0, r8
	lsls	r0, r0, #3
	movs	r1, #16
	subs	r0, #10
	bl	Func_080a1a40
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [pc, #152]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L4
	movs	r0, #112
	bl	Func_080f9010
	movs	r0, #1
	b.n	.L5
.L4:
	ldr	r2, [r1, #0]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L6
	movs	r0, #113
	bl	Func_080f9010
	movs	r0, #1
	negs	r0, r0
	b.n	.L5
.L6:
	ldr	r5, [pc, #112]
	ldr	r2, [r5, #0]
	movs	r3, #32
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L7
	movs	r0, #111
	bl	Func_080f9010
	mov	r3, r9
	cmp	r3, #1
	ble.n	.L7
	movs	r0, #1
	movs	r1, #1
	negs	r0, r0
	str	r1, [sp, #4]
	add	r8, r0
.L7:
	ldr	r2, [r5, #0]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L8
	b.n	.L9
.L8:
	movs	r0, #111
	bl	Func_080f9010
	mov	r2, r9
	cmp	r2, #1
	bgt.n	.L10
	b.n	.L9
.L10:
	movs	r3, #1
	add	r8, r3
	str	r3, [sp, #4]
	b.n	.L9
.L5:
	mov	r1, r8
	movs	r2, #130
	strb	r1, [r7, #28]
	lsls	r2, r2, #2
	add	r2, sl
	ldrh	r3, [r7, r2]
	str	r3, [r7, #8]
	ldr	r1, [pc, #32]
	ldrh	r2, [r7, r2]
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	add	sp, #8
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
	.4byte 0x0000021a
