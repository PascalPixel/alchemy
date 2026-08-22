@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08005a78
	.thumb_func
Func_08005a78:
	push	{r5, r6, lr}
	ldr	r3, [pc, #56]
	adds	r6, r1, #0
	ldr	r5, [r3, #0]
	bl	Func_08005b24
	cmp	r0, #15
	bls.n	.L0
	movs	r0, #1
	b.n	.L1
.L0:
	bl	Func_080058ac
	adds	r0, r5, #0
	ldr	r3, [pc, #36]
	adds	r0, #80
	adds	r1, r6, #0
	ldr	r2, [pc, #32]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	ldr	r1, [pc, #20]
	lsls	r2, r2, #24
.L2:
	ldr	r3, [r1, #8]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L2
	movs	r0, #0
.L1:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x03001f1c
	.4byte 0x040000d4
	.4byte 0x840003fc
