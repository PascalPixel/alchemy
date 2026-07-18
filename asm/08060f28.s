@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08060f28
	.thumb_func
Func_08060f28:
	push	{r0, r1, r2, r3, r4, r6, r7, lr}
	lsrs	r6, r1, #10
	ldr	r3, [r6, #80]
	strb	r2, [r7, #16]
	ldr	r2, [sp, #812]
	add	lr, r6
	subs	r5, r1, #7
	subs	r0, #39
	pop	{r2, r6, pc}
