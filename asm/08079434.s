@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08079434
	.thumb_func
Func_08079434:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	ldr	r4, [pc, #32]
	lsls	r3, r6, #20
	movs	r0, #4
	ands	r0, r6
	movs	r5, #15
	lsrs	r6, r3, #23
	ldrb	r3, [r4, r6]
	adds	r2, r5, #0
	lsls	r2, r0
	ands	r1, r5
	bics	r3, r2
	lsls	r1, r0
	orrs	r3, r1
	strb	r3, [r4, r6]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000040
