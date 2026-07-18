@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080ad35c
	.thumb_func
Func_080ad35c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #156]
	ldr	r3, [r3, #0]
	movs	r2, #128
	mov	r8, r3
	movs	r6, #141
	movs	r3, #162
	movs	r7, #137
	sub	sp, #28
	movs	r1, #0
	lsls	r2, r2, #9
	lsls	r6, r6, #2
	lsls	r3, r3, #1
	lsls	r7, r7, #2
	mov	r9, r1
	mov	fp, r2
	add	r4, sp, #4
	add	r5, sp, #12
	add	r6, r8
	mov	sl, r3
	add	r7, r8
.L1:
	ldmia	r7!, {r0}
	cmp	r0, #0
	beq.n	.L0
	mov	r3, r8
	mov	r1, sl
	ldrsh	r1, [r1, r3]
	lsls	r3, r1, #16
	movs	r1, #241
	ldrb	r2, [r0, #9]
	lsls	r1, r1, #17
	subs	r1, r1, r3
	movs	r3, #13
	mov	ip, r2
	negs	r3, r3
	adds	r2, r3, #0
	mov	r3, ip
	ands	r3, r2
	mov	r2, fp
	str	r2, [sp, #4]
	strb	r3, [r0, #9]
	str	r2, [r4, #4]
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	lsls	r3, r3, #16
	str	r1, [r5, #4]
	str	r3, [r5, #0]
	movs	r2, #8
	ldrsh	r3, [r6, r2]
	lsls	r3, r3, #16
	adds	r3, r3, r1
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
	movs	r1, #1
	add	r9, r1
	movs	r3, #2
	mov	r2, r9
	adds	r6, #2
	add	sl, r3
	cmp	r2, #3
	ble.n	.L1
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
