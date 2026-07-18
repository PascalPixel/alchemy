@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08005904
	.thumb_func
Func_08005904:
	push	{lr}
	ldr	r3, [pc, #20]
	lsls	r0, r0, #16
	ldr	r3, [r3, #0]
	lsrs	r0, r0, #16
	bl	Func_080072f0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02004c14
