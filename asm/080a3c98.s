@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a3c98
	.thumb_func
Func_080a3c98:
	push	{r5, r6, lr}
	ldr	r3, [pc, #60]
	ldr	r1, [pc, #60]
	ldr	r6, [r3, #0]
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L0
	movs	r5, #0
.L1:
	asrs	r5, r5, #24
	movs	r2, #138
	lsls	r2, r2, #1
	lsls	r3, r5, #2
	adds	r3, r3, r2
	ldr	r0, [r6, r3]
	movs	r1, #1
	bl	Func_08009020
	ldr	r1, [pc, #28]
	adds	r5, #1
	adds	r3, r6, r1
	lsls	r5, r5, #24
	ldrb	r3, [r3, #0]
	asrs	r2, r5, #24
	cmp	r2, r3
	blt.n	.L1
.L0:
	ldr	r0, [pc, #16]
	bl	Func_08004278
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
	.4byte 0x00000219
	.4byte 0x080a3c09
