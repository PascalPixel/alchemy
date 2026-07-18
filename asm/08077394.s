@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08077394
	.thumb_func
Func_08077394:
	push	{lr}
	mov	r3, lr
	ldr	r2, [pc, #48]
	cmp	r0, #7
	bhi.n	.L0
	movs	r3, #166
	lsls	r3, r3, #1
	muls	r3, r0
	adds	r0, r3, r2
	b.n	.L1
.L0:
	adds	r3, r0, #0
	subs	r3, #128
	cmp	r3, #5
	bhi.n	.L2
	ldr	r3, [pc, #28]
	ldr	r2, [r3, #0]
	cmp	r2, #0
	beq.n	.L2
	movs	r3, #166
	lsls	r3, r3, #1
	muls	r3, r0
	adds	r3, r2, r3
	ldr	r2, [pc, #16]
	adds	r0, r3, r2
	b.n	.L1
.L2:
	movs	r0, #0
.L1:
	pop	{r1}
	bx	r1
	.4byte 0x02000500
	.4byte 0x03001f28
	.4byte 0xffff5a00
