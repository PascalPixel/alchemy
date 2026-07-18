@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08006c24
	.thumb_func
Func_08006c24:
	push	{r4, r5, lr}
	adds	r4, r0, #0
	adds	r3, r1, #0
	lsls	r2, r2, #16
	ldr	r0, [pc, #28]
	adds	r2, r2, r0
	lsrs	r2, r2, #16
	ldr	r0, [pc, #28]
	cmp	r2, r0
	beq.n	.L0
	adds	r5, r0, #0
.L3:
	ldrb	r1, [r3, #0]
	ldrb	r0, [r4, #0]
	adds	r4, #1
	adds	r3, #1
	cmp	r1, r0
	beq.n	.L1
	subs	r0, r3, #1
	b.n	.L2
	movs	r0, r0
	.4byte 0xffff0000
	.4byte 0x0000ffff
.L1:
	subs	r0, r2, #1
	lsls	r0, r0, #16
	lsrs	r2, r0, #16
	cmp	r2, r5
	bne.n	.L3
.L0:
	movs	r0, #0
.L2:
	pop	{r4, r5}
	pop	{r1}
	bx	r1
