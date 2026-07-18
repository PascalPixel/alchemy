@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080051e8
	.thumb_func
Func_080051e8:
	push	{r5, lr}
	sub	sp, #48
	mov	r5, sp
	adds	r2, r5, #0
	bl	Func_08004fe4
	ldr	r3, [pc, #12]
	adds	r0, r5, #0
	bl	Func_080072f0
	add	sp, #48
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x030002c0
