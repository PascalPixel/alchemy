@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080fa324
	.thumb_func
Func_080fa324:
	push	{lr}
	lsls	r0, r0, #16
	ldr	r2, [pc, #28]
	ldr	r1, [pc, #32]
	lsrs	r0, r0, #13
	adds	r0, r0, r1
	ldrh	r3, [r0, #4]
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #2
	adds	r1, r1, r2
	ldr	r2, [r1, #0]
	ldr	r1, [r0, #0]
	adds	r0, r2, #0
	bl	Func_080faa58
	pop	{r0}
	bx	r0
	.4byte 0x080fc624
	.4byte 0x080fc684
