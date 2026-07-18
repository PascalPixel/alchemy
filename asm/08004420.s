@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08004420
	.thumb_func
Func_08004420:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #44]
	ldrb	r3, [r3, #0]
	adds	r7, r0, #0
	ldr	r6, [pc, #40]
	asrs	r7, r7, #8
	cmp	r3, #1
	bne.n	.L0
	movs	r5, #21
	subs	r6, #8
.L1:
	subs	r5, #1
	cmp	r5, #0
	beq.n	.L0
	adds	r6, #8
	ldrb	r3, [r6, #5]
	cmp	r3, r7
	bne.n	.L1
	ldr	r0, [r6, #0]
	bl	Func_080072e4
	b.n	.L1
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001d34
	.4byte 0x03001a20
