@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080058ac
	.thumb_func
Func_080058ac:
	push	{r5, lr}
	ldr	r3, [pc, #72]
	ldr	r5, [r3, #0]
	lsls	r0, r0, #16
	adds	r5, #64
	movs	r3, #128
	adds	r2, r5, #0
	lsrs	r0, r0, #16
	lsls	r3, r3, #5
	movs	r1, #0
	sub	sp, #16
	bl	Func_08006ba8
	ldr	r3, [pc, #52]
	adds	r0, r5, #0
	mov	r1, sp
	ldr	r2, [pc, #48]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	ldr	r1, [pc, #36]
	lsls	r2, r2, #24
.L0:
	ldr	r3, [r1, #8]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L0
	bl	Func_08005ae0
	mov	r3, sp
	ldrh	r3, [r3, #8]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	subs	r0, r0, r3
	add	sp, #16
	pop	{r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f1c
	.4byte 0x040000d4
	.4byte 0x84000004
