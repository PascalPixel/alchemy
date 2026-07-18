@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080200cc
	.thumb_func
Func_080200cc:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r2, #0
	mov	r8, r2
	movs	r2, #128
	lsls	r2, r2, #9
	ldr	r3, [pc, #108]
	mov	r9, r2
	movs	r2, #250
	lsls	r2, r2, #17
	ldr	r3, [r3, #0]
	mov	sl, r2
	movs	r2, #141
	lsls	r2, r2, #2
	sub	sp, #28
	adds	r6, r3, r2
	subs	r2, #16
	add	r4, sp, #4
	add	r5, sp, #12
	adds	r7, r3, r2
.L1:
	ldmia	r7!, {r0}
	cmp	r0, #0
	beq.n	.L0
	mov	r3, r9
	str	r3, [sp, #4]
	str	r3, [r4, #4]
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	mov	r3, sl
	str	r3, [r5, #4]
	movs	r2, #8
	ldrsh	r3, [r6, r2]
	lsls	r3, r3, #16
	add	r3, sl
	str	r3, [r5, #8]
	movs	r3, #0
	str	r3, [r5, #12]
	movs	r3, #128
	adds	r2, r4, #0
	adds	r1, r5, #0
	lsls	r3, r3, #7
	str	r4, [sp, #0]
	bl	Func_08009008
	ldr	r4, [sp, #0]
.L0:
	movs	r3, #1
	add	r8, r3
	mov	r2, r8
	adds	r6, #2
	cmp	r2, #3
	ble.n	.L1
	add	sp, #28
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
