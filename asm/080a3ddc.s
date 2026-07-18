@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a3ddc
	.thumb_func
Func_080a3ddc:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r3, r5, #0
	ldr	r2, [pc, #48]
	adds	r3, #62
	mov	ip, r5
.L0:
	strh	r2, [r3, #0]
	subs	r3, #2
	cmp	r3, ip
	bge.n	.L0
	ldr	r3, [pc, #32]
	movs	r7, #0
	mov	ip, r3
	adds	r0, #216
	movs	r6, #0
	adds	r4, r5, #0
	movs	r1, #14
.L2:
	mov	r3, ip
	strh	r3, [r6, r5]
	ldrh	r2, [r0, #0]
	adds	r3, r2, #0
	adds	r0, #2
	cmp	r3, #0
	beq.n	.L1
	strh	r2, [r4, #0]
	adds	r7, #1
	adds	r4, #2
	b.n	.L1
	.4byte 0x00000000
.L1:
	subs	r1, #1
	adds	r6, #2
	cmp	r1, #0
	bge.n	.L2
	adds	r0, r7, #0
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
