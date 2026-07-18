@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080bf574
	.thumb_func
Func_080bf574:
	push	{lr}
	bl	Func_08077008
	movs	r3, #163
	lsls	r3, r3, #1
	adds	r1, r0, r3
	ldrb	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L0
	adds	r3, #255
	strb	r3, [r1, #0]
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L0
	ldr	r1, [pc, #12]
	adds	r2, r0, r1
	strb	r3, [r2, #0]
	movs	r0, #1
	b.n	.L1
.L0:
	movs	r0, #0
.L1:
	pop	{r1}
	bx	r1
	.4byte 0x00000147
