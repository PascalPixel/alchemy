@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a1d08
	.thumb_func
Func_080a1d08:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #216]
	ldr	r7, [r3, #0]
	mov	r8, r2
	ldr	r2, [r7, #20]
	movs	r3, #13
	strb	r3, [r2, #5]
	movs	r2, #1
	negs	r2, r2
	sub	sp, #24
	adds	r6, r0, #0
	mov	sl, r1
	cmp	r8, r2
	beq.n	.L0
	add	r0, sp, #8
	add	r1, sp, #20
	add	r2, sp, #16
	add	r3, sp, #12
	str	r0, [sp, #0]
	adds	r0, r6, #0
	bl	Func_08015108
	ldr	r2, [sp, #8]
	adds	r5, r7, #0
	str	r2, [sp, #0]
	movs	r2, #129
	lsls	r2, r2, #1
	adds	r5, #60
	str	r2, [sp, #4]
	ldr	r3, [sp, #12]
	adds	r0, r5, #0
	mov	r1, sl
	mov	r2, r8
	bl	Func_080a10d0
	cmp	r0, #0
	bne.n	.L1
	ldr	r2, [sp, #8]
	ldr	r0, [r5, #0]
	ldr	r3, [sp, #12]
	str	r2, [sp, #0]
	mov	r1, sl
	mov	r2, r8
	bl	Func_080a23f4
.L1:
	ldr	r5, [r5, #0]
	b.n	.L2
.L0:
	ldr	r5, [r7, #44]
.L2:
	adds	r0, r5, #0
	bl	Func_08015270
	adds	r0, r5, #0
	bl	Func_08015278
	movs	r3, #1
	negs	r3, r3
	cmp	r8, r3
	bne.n	.L3
	adds	r0, r6, #0
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
	b.n	.L4
.L3:
	adds	r0, r6, #0
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015078
.L4:
	movs	r2, #1
	negs	r2, r2
	cmp	sl, r2
	beq.n	.L5
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #1
	ldr	r6, [pc, #64]
	mov	sl, r3
.L7:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [r6, #0]
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L6
	ldr	r3, [r6, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L6
	ldr	r3, [r6, #0]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L7
.L6:
	movs	r3, #1
	negs	r3, r3
	cmp	r8, r3
	bne.n	.L8
	adds	r0, r5, #0
	bl	Func_08015270
.L8:
	adds	r0, r5, #0
	bl	Func_08015278
	b.n	.L9
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x03001c94
.L5:
	ldr	r0, [pc, #40]
	bl	Func_080770c8
.L9:
	ldr	r3, [pc, #40]
	adds	r2, r7, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	ldr	r1, [pc, #24]
	ldr	r3, [r7, #20]
	movs	r2, #1
	negs	r2, r2
	strb	r1, [r3, #5]
	cmp	r8, r2
	beq.n	.L10
	adds	r0, r7, #0
	adds	r0, #60
	movs	r1, #1
	bl	Func_080a1114
	b.n	.L10
	.4byte 0x00000001
	.4byte 0x00000151
	.4byte 0x00000222
.L10:
	add	sp, #24
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
