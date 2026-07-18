@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08019e48
	.thumb_func
Func_08019e48:
	push	{r5, r6, lr}
	ldr	r3, [pc, #96]
	movs	r2, #160
	ldr	r6, [r3, #0]
	lsls	r2, r2, #3
	adds	r5, r6, r2
	bl	Func_08019d2c
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L0
	ldr	r2, [pc, #76]
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	cmp	r3, r0
	bne.n	.L1
	movs	r3, #1
	b.n	.L2
.L4:
	adds	r0, r5, #0
	movs	r1, #2
	bl	Func_08016418
	b.n	.L0
.L1:
	ldr	r2, [pc, #56]
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	cmp	r3, r0
	bne.n	.L0
	movs	r3, #0
.L2:
	ldr	r2, [pc, #48]
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r0, [r6, r3]
	movs	r1, #0
.L5:
	ldr	r2, [r5, #0]
	ldrb	r3, [r2, #4]
	cmp	r3, #2
	bne.n	.L3
	ldrb	r3, [r2, #14]
	cmp	r3, r0
	beq.n	.L4
.L3:
	adds	r1, #1
	adds	r5, #36
	cmp	r1, #8
	bne.n	.L5
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x000012ee
	.4byte 0x000012ec
	.4byte 0x000012f0
