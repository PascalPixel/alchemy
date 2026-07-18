@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080048f4
	.thumb_func
Func_080048f4:
	push	{r5, lr}
	ldr	r4, [pc, #56]
	lsls	r5, r0, #2
	ldr	r0, [r4, r5]
	cmp	r0, #0
	bne.n	.L0
	adds	r3, r1, #3
	lsrs	r3, r3, #2
	ldr	r0, [r4, #0]
	lsls	r1, r3, #2
	movs	r3, #129
	adds	r2, r0, r1
	lsls	r3, r3, #18
	cmp	r2, r3
	bcc.n	.L1
	ldr	r0, [r4, #4]
	ldr	r3, [pc, #28]
	adds	r1, r0, r1
	cmp	r1, r3
	bls.n	.L2
	movs	r0, #0
	b.n	.L0
.L2:
	str	r1, [r4, #4]
	str	r0, [r4, r5]
	b.n	.L0
.L1:
	str	r2, [r4, #0]
	str	r0, [r4, r5]
.L0:
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x03001e50
	.4byte 0x030077ff
