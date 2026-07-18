@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a172c
	.thumb_func
Func_080a172c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #4
	mov	r8, r0
	mov	sl, r2
	adds	r7, r3, #0
	bl	Func_08004080
	adds	r5, r0, #0
	movs	r6, #0
	cmp	r5, #0
	beq.n	.L0
	ldr	r2, [pc, #40]
	movs	r1, #128
	bl	Func_08003fa4
	movs	r1, #128
	lsls	r1, r1, #23
	adds	r0, r5, #0
	mov	r2, r8
	mov	r3, sl
	str	r7, [sp, #0]
	bl	Func_080150c8
	adds	r6, r0, #0
.L0:
	adds	r0, r6, #0
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x080aea4c
