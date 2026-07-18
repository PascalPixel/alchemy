@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080958e4
	.thumb_func
Func_080958e4:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #60]
	ldr	r0, [pc, #60]
	ldr	r5, [r3, #0]
	bl	Func_08004278
	adds	r6, r5, #0
	adds	r6, #157
	adds	r5, #88
	movs	r7, #23
.L1:
	ldrb	r3, [r6, #0]
	lsls	r3, r3, #24
	adds	r6, #72
	cmp	r3, #0
	beq.n	.L0
	adds	r0, r5, #0
	bl	Func_0809bb34
.L0:
	subs	r7, #1
	adds	r5, #72
	cmp	r7, #0
	bge.n	.L1
	movs	r0, #56
	bl	Func_08002dd8
	movs	r0, #1
	bl	Func_080030f8
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f30
	.4byte 0x08095885
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	strh	r3, [r0, #6]
	bx	lr
