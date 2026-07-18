@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_081eccfc
	.thumb_func
Func_081eccfc:
	push	{r1, r4, lr}
	lsrs	r3, r3, #28
	subs	r0, #28
	asrs	r5, r7, #25
	cmp	r4, #9
	pop	{r0, r1, r2, r3, r4, r5, r6, pc}
