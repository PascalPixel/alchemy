@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800ea54
	.thumb_func
Func_0800ea54:
	push	{lr}
	cmp	r0, #0
	beq.n	.L0
	str	r1, [r0, #108]
.L0:
	pop	{r0}
	bx	r0
