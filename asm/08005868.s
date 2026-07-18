@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08005868
	.thumb_func
Func_08005868:
	push	{r5, r6, lr}
	ldr	r3, [pc, #56]
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #56]
	lsls	r0, r0, #16
	adds	r6, r3, #0
	lsrs	r5, r0, #16
	adds	r6, #64
	ldr	r3, [r2, #0]
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	Func_080072f0
	lsls	r0, r0, #16
	cmp	r0, #0
	beq.n	.L0
	movs	r0, #1
	b.n	.L1
.L0:
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	Func_08006c68
	adds	r3, r0, #0
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
.L1:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f1c
	.4byte 0x02004c04
