@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0828e128
	.thumb_func
Func_0828e128:
	push	{r0, r2, r3, r4, lr}
	ldrb	r6, [r3, #4]
	asrs	r3, r7, #7
	pop	{r0, r1, r2, r7, pc}
