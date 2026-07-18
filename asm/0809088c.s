@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809088c
	.thumb_func
Func_0809088c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #4
	adds	r7, r0, #0
	adds	r6, r1, #0
	adds	r5, r2, #0
	mov	r8, r3
	cmp	r3, #0
	ble.n	.L0
	ldr	r1, [pc, #52]
	ldr	r2, [pc, #52]
	mov	sl, r1
.L1:
	movs	r1, #0
	ldrsh	r3, [r7, r1]
	movs	r1, #0
	ldrsh	r0, [r6, r1]
	str	r2, [sp, #0]
	subs	r0, r0, r3
	mov	r1, r8
	bl	Func_0800730c
	ldr	r2, [sp, #0]
	subs	r2, #1
	strh	r0, [r5, #0]
	adds	r7, #2
	adds	r6, #2
	adds	r5, #2
	cmp	r2, #0
	bge.n	.L1
.L0:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03000380
	.4byte 0x0000053f
