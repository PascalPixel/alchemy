@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f7e34
	.thumb_func
Func_080f7e34:
	push	{lr}
	ldr	r3, [pc, #36]
	ldr	r1, [r3, #0]
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #2
	adds	r4, r3, #4
	ldr	r0, [r1, r4]
	cmp	r0, #0
	beq.n	.L0
	ldr	r2, [r1, r3]
	cmp	r2, #0
	beq.n	.L1
	str	r0, [r2, #4]
.L1:
	ldr	r2, [r1, r4]
	ldr	r3, [r1, r3]
	str	r3, [r2, #0]
.L0:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02004c00
