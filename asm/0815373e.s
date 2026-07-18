@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0815373e
	.thumb_func
Func_0815373e:
	push	{r1, r2, r6, r7, lr}
	strh	r5, [r2, #52]
	strh	r0, [r1, #60]
	ldrh	r0, [r1, #4]
	ldrh	r0, [r1, #4]
	ldrh	r0, [r1, #4]
	ldrh	r0, [r1, #4]
	ldrh	r0, [r1, #4]
	ldrh	r0, [r1, #4]
	ldrh	r0, [r1, #4]
	strh	r0, [r1, #60]
	strh	r0, [r1, #60]
	add	r3, sp, #584
	pop	{r1, r2, r4, r5, r7, pc}
