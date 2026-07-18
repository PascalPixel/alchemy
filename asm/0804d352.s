@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0804d352
	.thumb_func
Func_0804d352:
	push	{r1, r2, r3, r4, r6, r7, lr}
	lsrs	r7, r0, #9
	ldrb	r3, [r3, #0]
	ldr	r5, [sp, #240]
	pop	{r3, r4, r5, r6, r7}
	pop	{r0, r2, r3, r6, r7, pc}
