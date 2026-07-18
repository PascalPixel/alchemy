@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080046c4
	.thumb_func
Func_080046c4:
	push	{r5, lr}
	ldr	r3, [pc, #64]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L0
	ldr	r4, [pc, #60]
	ldrb	r3, [r0, #0]
	ldr	r2, [r4, #0]
	movs	r1, #0
	adds	r0, #1
	cmp	r3, #0
	beq.n	.L1
	movs	r5, #240
	ldr	r4, [pc, #48]
	lsls	r5, r5, #8
.L4:
	orrs	r3, r5
	strh	r3, [r2, #0]
	adds	r2, #2
	cmp	r2, r4
	bne.n	.L2
	ldr	r2, [pc, #36]
.L2:
	adds	r1, #1
	cmp	r1, #31
	bhi.n	.L3
	ldrb	r3, [r0, #0]
	adds	r0, #1
	cmp	r3, #0
	bne.n	.L4
.L3:
	ldr	r4, [pc, #12]
.L1:
	str	r2, [r4, #0]
.L0:
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ac4
	.4byte 0x03001cbc
	.4byte 0x06002500
	.4byte 0x06002000
