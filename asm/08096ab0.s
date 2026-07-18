@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08096ab0
	.thumb_func
Func_08096ab0:
	push	{r5, lr}
	ldr	r3, [pc, #48]
	ldr	r5, [r3, #0]
	movs	r1, #30
	ldrsh	r3, [r5, r1]
	cmp	r3, #2
	bne.n	.L0
	bl	Func_08097608
	ldr	r2, [pc, #36]
	ldr	r3, [pc, #36]
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	movs	r1, #26
	ldrsh	r3, [r5, r1]
	cmp	r2, r3
	beq.n	.L0
	ldr	r3, [r5, #20]
	movs	r2, #0
	adds	r3, #91
	strb	r2, [r3, #0]
.L0:
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f30
	.4byte 0x0000024a
	.4byte 0x02000240
