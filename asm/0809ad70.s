@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809ad70
	.thumb_func
Func_0809ad70:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	ldr	r5, [pc, #20]
	bl	Func_08004458
	lsls	r0, r0, #3
	lsrs	r0, r0, #16
	ldrsb	r1, [r5, r0]
	adds	r0, r6, #0
	bl	Func_08009240
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x0809f160
