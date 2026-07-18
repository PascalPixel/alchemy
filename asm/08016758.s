@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08016758
	.thumb_func
Func_08016758:
	push	{r5, lr}
	ldr	r3, [pc, #76]
	movs	r1, #196
	ldr	r3, [r3, #0]
	lsls	r1, r1, #3
	adds	r2, r3, r1
	movs	r5, #0
	movs	r1, #0
	b.n	.L0
.L3:
	adds	r2, #40
	adds	r1, #1
.L0:
	cmp	r1, #3
	beq.n	.L1
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L2
	ldrh	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L3
.L2:
	adds	r5, r2, #0
.L1:
	cmp	r5, #0
	beq.n	.L4
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L5
	bl	Func_0801671c
	movs	r3, #0
	strh	r3, [r5, #6]
.L5:
	movs	r3, #0
	strh	r3, [r5, #4]
	strh	r3, [r5, #20]
	movs	r2, #15
	strh	r3, [r5, #24]
	movs	r3, #10
	strh	r2, [r5, #22]
	strh	r3, [r5, #26]
.L4:
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
