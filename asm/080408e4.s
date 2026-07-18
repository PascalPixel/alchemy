@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080408e4
	.thumb_func
Func_080408e4:
	push	{r0, r1, r2, r3, r6, lr}
	ldrb	r1, [r7, r4]
	strh	r3, [r7, #4]
	pop	{r0, r1, r2, r5, r7, pc}
