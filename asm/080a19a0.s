@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a19a0
	.thumb_func
Func_080a19a0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #144]
	sub	sp, #28
	ldr	r5, [r3, #0]
	bl	Func_08077148
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	mov	sl, r0
	movs	r4, #0
	cmp	r4, sl
	bge.n	.L0
	add	r2, sp, #4
	mov	r8, r2
	movs	r3, #154
	movs	r2, #138
	lsls	r3, r3, #1
	lsls	r2, r2, #1
	adds	r7, r5, r3
	add	r6, sp, #12
	adds	r5, r5, r2
.L2:
	movs	r2, #16
	ldrsh	r3, [r7, r2]
	ldr	r0, [r5, #0]
	movs	r2, #241
	lsls	r3, r3, #16
	lsls	r2, r2, #17
	subs	r1, r2, r3
	cmp	r0, #0
	beq.n	.L1
	ldrb	r3, [r0, #9]
	mov	ip, r3
	movs	r3, #13
	negs	r3, r3
	adds	r2, r3, #0
	mov	r3, ip
	ands	r3, r2
	strb	r3, [r0, #9]
	ldr	r3, [r5, #64]
	str	r3, [sp, #4]
	ldr	r3, [r5, #64]
	mov	r2, r8
	str	r3, [r2, #4]
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	lsls	r3, r3, #16
	str	r1, [r6, #4]
	str	r3, [r6, #0]
	movs	r2, #16
	ldrsh	r3, [r7, r2]
	lsls	r3, r3, #16
	adds	r3, r3, r1
	str	r3, [r6, #8]
	movs	r3, #0
	str	r3, [r6, #12]
	movs	r3, #128
	adds	r1, r6, #0
	mov	r2, r8
	lsls	r3, r3, #7
	str	r4, [sp, #0]
	bl	Func_08009008
	ldr	r4, [sp, #0]
.L1:
	adds	r4, #1
	adds	r7, #2
	adds	r5, #4
	cmp	r4, sl
	blt.n	.L2
.L0:
	add	sp, #28
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
