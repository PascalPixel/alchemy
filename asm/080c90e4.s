@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c90e4
	.thumb_func
Func_080c90e4:
	push	{r5, lr}
	ldr	r3, [pc, #60]
	ldr	r1, [pc, #60]
	ldr	r4, [r3, #0]
	adds	r5, r4, r1
	ldr	r3, [r5, #0]
	adds	r3, #1
	str	r3, [r5, #0]
	adds	r1, #4
	adds	r2, r4, r1
	ldr	r2, [r2, #0]
	cmp	r3, r2
	bne.n	.L0
	ldr	r2, [pc, #44]
	ldr	r1, [pc, #44]
	adds	r3, r4, r2
	ldr	r2, [r3, #0]
	ldrh	r3, [r1, #4]
	adds	r3, r3, r2
	ldr	r2, [pc, #40]
	strh	r3, [r1, #4]
	adds	r3, r4, r2
	ldr	r2, [r3, #0]
	ldrh	r3, [r1, #6]
	movs	r0, #0
	adds	r3, r3, r2
	strh	r3, [r1, #6]
	str	r0, [r5, #0]
.L0:
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x00007790
	.4byte 0x00007798
	.4byte 0x03001ad0
	.4byte 0x0000779c
