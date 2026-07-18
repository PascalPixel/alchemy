@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080915ac
	.thumb_func
Func_080915ac:
	push	{lr}
	ldr	r3, [pc, #36]
	ldr	r2, [pc, #36]
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L0
	bl	Func_08091560
	ldrb	r0, [r0, #2]
	cmp	r0, #255
	bne.n	.L1
.L0:
	movs	r0, #0
	b.n	.L2
.L1:
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r0, r0, r3
.L2:
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x0000020a
