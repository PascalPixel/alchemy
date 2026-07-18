@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_081e1e34
	.thumb_func
Func_081e1e34:
	push	{r1, r3, r4, r6, r7, lr}
	adds	r4, #211
	cmp	r5, #94
	movs	r4, #36
	adds	r4, #94
	lsrs	r4, r4, #20
	pop	{r0, r1, r3, r4, r5, r6, r7, pc}
