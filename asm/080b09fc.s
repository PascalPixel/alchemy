@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b09fc
	.thumb_func
Func_080b09fc:
	push	{r5, r6, lr}
	ldr	r5, [r0, #0]
	ldrh	r4, [r5, #6]
	ldr	r6, [pc, #16]
	strh	r4, [r0, #4]
	ldrh	r4, [r5, #8]
	strh	r1, [r0, #8]
	strh	r4, [r0, #6]
	strh	r2, [r0, #10]
	strb	r3, [r0, #13]
	strb	r6, [r0, #12]
	b.n	.L0
	.4byte 0x00000000
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
