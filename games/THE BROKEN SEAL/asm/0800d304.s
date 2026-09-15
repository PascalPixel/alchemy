@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800d304
	.thumb_func
Func_0800d304:
	push	{r5, r6, lr}
	ldr	r5, [pc, #44]
	adds	r0, r5, #0
	bl	Func_08004938
	movs	r2, #132
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	ldr	r3, [pc, #32]
	ldr	r0, [pc, #32]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	Func_080072fc
	adds	r0, r6, #0
	bl	Func_08002df0
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x000004e8
	.4byte 0x040000d4
	.4byte 0x0800a494
