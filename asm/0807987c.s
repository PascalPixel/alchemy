@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0807987c
	.thumb_func
Func_0807987c:
	push	{r5, r6, lr}
	adds	r6, r1, #0
	sub	sp, #16
	bl	Func_08077394
	adds	r1, r0, #0
	movs	r0, #0
	cmp	r6, #3
	bgt.n	.L0
	movs	r2, #148
	lsls	r2, r2, #1
	adds	r3, r1, r2
	mov	r5, sp
	ldrb	r0, [r3, #0]
	adds	r1, #248
	adds	r2, r5, #0
	bl	Func_080797fc
	lsls	r3, r6, #2
	ldr	r0, [r5, r3]
	movs	r1, #10
	bl	Func_080022ec
.L0:
	add	sp, #16
	pop	{r5, r6}
	pop	{r1}
	bx	r1
